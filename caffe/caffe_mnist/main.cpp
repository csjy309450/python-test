#include <caffe/caffe.hpp>
#include <string>
#include <vector>
using namespace caffe;

int main(argc, arcv**)
{
    ///初始化网络
    char *proto = "deploy.prototxt"; /* 加载CaffeNet的配置 */
    Phase phase = TEST; /* or TRAIN */
    Caffe::set_mode(Caffe::CPU);
    // Caffe::set_mode(Caffe::GPU);
    // Caffe::SetDevice(0);

    //Note: 后文所有提到的net，都是这个net
    boost::shared_ptr< Net<float> > net(new caffe::Net<float>(proto, phase));


    ///加载已训练好的模型
    char *model = "bvlc_reference_caffenet.caffemodel";
    net->CopyTrainedLayersFrom(model);

    //!读取模型中的每层的结构配置参数
    char *model = "bvlc_reference_caffenet.caffemodel";
    NetParameter param;
    ReadNetParamsFromBinaryFileOrDie(model, ¶m);
    int num_layers = param.layer_size();
    for (int i = 0; i < num_layers; ++i)
    {
        // 结构配置参数:name，type，kernel size，pad，stride等
        LOG(ERROR) << "Layer " << i << ":" << param.layer(i).name() << "\t" << param.layer(i).type();
        if (param.layer(i).type() == "Convolution")
        {
            ConvolutionParameter conv_param = param.layer(i).convolution_param();
            LOG(ERROR) << "\t\tkernel size: " << conv_param.kernel_size()
            << ", pad: " << conv_param.pad()
            << ", stride: " << conv_param.stride();
    }
}

    return 0;
}

