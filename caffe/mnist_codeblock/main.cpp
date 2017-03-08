#include <caffe/caffe.hpp>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif  // USE_OPENCV
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#ifdef USE_OPENCV
using namespace caffe;  // NOLINT(build/namespaces)
using std::string;

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class Classifier {
public:
    Classifier(const string& model_file,
        const string& trained_file,
        const string& mean_file,
        const string& label_file);

    std::vector<Prediction> Classify(const cv::Mat& img, int N = 1);

private:
    void SetMean(const string& mean_file);

    std::vector<float> Predict(const cv::Mat& img);

    void WrapInputLayer(std::vector<cv::Mat>* input_channels);

    void Preprocess(const cv::Mat& img,
        std::vector<cv::Mat>* input_channels);

private:
    shared_ptr<Net<float> > net_;
    cv::Size input_geometry_;
    int num_channels_;
    cv::Mat mean_;
    std::vector<string> labels_;
};

Classifier::Classifier(const string& model_file,
    const string& trained_file,
    const string& mean_file,
    const string& label_file) {
#ifdef CPU_ONLY
    Caffe::set_mode(Caffe::CPU);
    std::cout<<"CPU"<<std::endl;
#else
    /// set runing mode
    caffe::Caffe::set_mode(Caffe::GPU);
    std::cout<<"GPU"<<std::endl;
#endif

    /// Load the network.
    net_.reset(new Net<float>(model_file, TEST));
    net_->CopyTrainedLayersFrom(trained_file);
    // 检测输入参数对否
    CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input.";
    CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output.";

    /// 获取输入数据Blob对象及其相关属性
    caffe::Blob<float>* input_layer = net_->input_blobs()[0];
    num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1)
        << "Input layer should have 1 or 3 channels.";
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

    /// Load the binaryproto mean file. 未使用
    //SetMean(mean_file);

    /// Load labels.
    std::ifstream labels(label_file.c_str());
    CHECK(labels) << "Unable to open labels file " << label_file;
    string line;
    while (std::getline(labels, line))
        labels_.push_back(string(line));

    /// 获取Output Layer 的数据Blob对象指针
    caffe::Blob<float>* output_layer = net_->output_blobs()[0];
    CHECK_EQ(labels_.size(), output_layer->channels())
        << "Number of labels is different from the output layer dimension.";
}

/*std::pair对象排序的比较器*/
static bool PairCompare(const std::pair<float, int>& lhs,
    const std::pair<float, int>& rhs) {
    return lhs.first > rhs.first;
}

/* Return the indices of the top N values of vector v. */
static std::vector<int> Argmax(const std::vector<float>& v, int N) {
    std::vector<std::pair<float, int> > pairs;
    for (size_t i = 0; i < v.size(); ++i)
        pairs.push_back(std::make_pair(v[i], i));
    std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

    std::vector<int> result;
    for (int i = 0; i < N; ++i)
        result.push_back(pairs[i].second);
    return result;
}

/* Return the top N predictions. */
std::vector<Prediction> Classifier::Classify(const cv::Mat& img, int N) {
    /// 调用成员函数 Predict() 输入图像到网络,并得到结果
    std::vector<float> output = Predict(img);

    /// 处理预测值,转换为对应标签
    // N必须小于等于labels_.size()即分类的数量
    N = std::min<int>(labels_.size(), N);
    // 返回预测向量中最大的N个值下标
    std::vector<int> maxN = Argmax(output, N);
    // 预测值转换为对应标签
    std::vector<Prediction> predictions;
    for (int i = 0; i < N; ++i) {
        int idx = maxN[i];
        predictions.push_back(std::make_pair(labels_[idx], output[idx]));
    }

    return predictions;
}

/* Load the mean file in binaryproto format.
这个函数的功能就是从序列化文件中读取数据存入BlobProto对象,
然后在将从这个对象取出数据放入Blob对象(以上为caffe从序列化
文件取数据的流程),再然后将Blob对象中图片(仍然只考虑一张图片)
的各个通道构造为一个 vector<Mat>,然后合并vector 为一个Mat,
最后做mean.*/
void Classifier::SetMean(const string& mean_file) {
    /// 定义一个BlobProto对象,用于从序列化二进制文件输入数据
    caffe::BlobProto blob_proto;
    // 从二进制文件读去数据,存放到blob_proto对象中
    caffe::ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

    ///Convert from BlobProto to Blob<float>
    caffe::Blob<float> mean_blob;
    // Blob 对象的FromProto成员函数,就是从序列化数据对象BlobProto读取数据
    mean_blob.FromProto(blob_proto);
    CHECK_EQ(mean_blob.channels(), num_channels_)
        << "Number of channels of mean file doesn't match input layer.";

    /// The format of the mean file is planar 32-bit float BGR or grayscale.
    /// 以Blob的各个通道构造vector<Mat>
    std::vector<cv::Mat> channels;
    float* data = mean_blob.mutable_cpu_data();
    for (int i = 0; i < num_channels_; ++i) {
        /* Extract an individual channel. */
        cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
        channels.push_back(channel);
        data += mean_blob.height() * mean_blob.width();
    }

    /* Merge the separate channels into a single image. */
    cv::Mat mean;
    cv::merge(channels, mean);

    /* Compute the global mean pixel value and create a mean image
    * filled with this value. */
    cv::Scalar channel_mean = cv::mean(mean);
    mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);
}

/*输入一张图像,经过网络输入预测值*/
std::vector<float> Classifier::Predict(const cv::Mat& img) {
    /// 获取输入层数据 Blob 指针
    caffe::Blob<float>* input_layer = net_->input_blobs()[0];
    input_layer->Reshape(1, num_channels_,
        input_geometry_.height, input_geometry_.width);
    /// Forward dimension change to all layers.
    net_->Reshape();

    /// 以输入层的Blob对象的data_数组构造一个 std::vector<cv::Mat>对象,以便我们之后的操作
    /*caffe的数据Blob对象中一张多通道图像的shape = [channels, width, height],
    ,和cv::Mat中多通道图像组织形式不同.所以需要转换,而WrapInputLayer()函数则
    将Blob对象的data_数组的每一通道分别构造为一个cv::Mat对象,存入vector容器.
    input_channels容器将一张图片的每一通道分别存储,且其中每一元素cv::Mat对
    应着Net对象输入层数据Blob(内存地址上的对应).因此input_channels对象就是
    input Layer的数据Blob的一张图片,只是将其做了封装*/
    std::vector<cv::Mat> input_channels;
    WrapInputLayer(&input_channels);
    /// 对输入图像做预处理,并放入Net的Input Layer
    Preprocess(img, &input_channels);
    /// Net 做一次正向传播
    net_->Forward();

    /// Copy the output layer to a std::vector
    // 获取输出层数据Blob对象指针
    caffe::Blob<float>* output_layer = net_->output_blobs()[0];
    // 获取其data_数组的只读指针
    const float* begin = output_layer->cpu_data();
    /*由于整个Predict函数仅处理一张图片,所以只需从data_数组获取channels个元
    素. 如果对N张图片做预测,则需要 end = begin + N * output_layer->channels()*/
    const float* end = begin + output_layer->channels();
    // 使用指针构造vector对象返回
    return std::vector<float>(begin, end);
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void Classifier::WrapInputLayer(std::vector<cv::Mat>* input_channels) {
    /// 获取输入层数据Blob指针,及相关参数
    caffe::Blob<float>* input_layer = net_->input_blobs()[0];
    int width = input_layer->width();
    int height = input_layer->height();
   // 获取Blob对象的data_数组的读写指针
    float* input_data = input_layer->mutable_cpu_data();
    /// 依次访问data_的每一通道(data_中仅有一张图)
    for (int i = 0; i < input_layer->channels(); ++i) {
        // 以height*width个元素(一个通道元素个数)构造Mat对象,指向的仍然是原先的内存区域
        cv::Mat channel(height, width, CV_32FC1, input_data);
        // 将表示图像一个通道的Mat对象压入vector对象
        input_channels->push_back(channel);
        // 计算一下通道的偏移量
        input_data += width * height;
    }
}

/* Convert the input image to the input image format of the network. */
void Classifier::Preprocess(const cv::Mat& img,
    std::vector<cv::Mat>* input_channels) {
    /// 首先是将输入图像转化为Net的输入层要求的通道
    cv::Mat sample;
    if (img.channels() == 3 && num_channels_ == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
    else if (img.channels() == 4 && num_channels_ == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
    else if (img.channels() == 4 && num_channels_ == 3)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
    else if (img.channels() == 1 && num_channels_ == 3)
        cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
    else
        sample = img;

    /// 将输入图像转化为Net输入层要求的Size
    cv::Mat sample_resized;
    if (sample.size() != input_geometry_)
        cv::resize(sample, sample_resized, input_geometry_);
    else
        sample_resized = sample;

    /// 输入图像转化成Net 输入层要求的数值类型
    cv::Mat sample_float;
    if (num_channels_ == 3)
        sample_resized.convertTo(sample_float, CV_32FC3);
    else
        sample_resized.convertTo(sample_float, CV_32FC1);

    /// 归一化处理,未启用
    cv::Mat sample_normalized;
  //cv::subtract(sample_float, mean_, sample_normalized);

    /* This operation will write the separate BGR planes directly to the
    * input layer of the network because it is wrapped by the cv::Mat
    * objects in input_channels. */
    /// 将做完预处理的图像的通道分离,存入容器input_channels,即放入Net的输入层数据Blob
    cv::split(sample_float, *input_channels);
    // 检测数据是否成功放入Net的输入层
    CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
        == net_->input_blobs()[0]->cpu_data())
        << "Input channels are not wrapping the input layer of the network.";
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0]
            << " ./samples/img_0_3.png" << std::endl;
        return 1;
    }
    /// 初始化glog库
    ::google::InitGoogleLogging(argv[0]);
    /// 定义模型相关变量
    string model_file   = "./mnist_deploy.prototxt";
    string trained_file = "./lenet_iter_10000.caffemodel";
    string mean_file    = "";
    string label_file   = "./labels.txt";
    /// 构造分类器对象 classifier
    Classifier classifier(model_file, trained_file, mean_file, label_file);

    /// 准备测试图像
    //string file = "./samples/img_0_3.png";
    string file = argv[1];
    std::cout << "---------- Prediction for "
        << file << " ----------" << std::endl;
    // 加载测试图像
    cv::Mat img = cv::imread(file, -1);
    CHECK(!img.empty()) << "Unable to decode image " << file;
    /// 使用分类器对象对图像分类
    std::vector<Prediction> predictions = classifier.Classify(img);

    /* Print the top N predictions. */
    for (size_t i = 0; i < predictions.size(); ++i) {
        Prediction p = predictions[i];
        std::cout << "Image" << file << "  Classified as >> " << p.first << std::endl;
        imshow("test", img);
        cv::waitKey(0);
        //std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
        //        << p.first << "\"" << std::endl;
    }
    img.release();
    cv::destroyAllWindows();
    return 0;
}
#else
int main(int argc, char** argv) {
    LOG(FATAL) << "This example requires OpenCV; compile with USE_OPENCV.";
}
#endif  // USE_OPENCV
