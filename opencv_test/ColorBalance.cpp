//该代码实现白平衡算法中的灰度世界法，能有效改善图像发红发蓝发绿的现象；
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	string path = "/home/yangzheng/testData/pet/test/";
	std::string files[25] = {
"frame_0000.jpg", 
"frame_0001.jpg", 
"frame_0002.jpg", 
"frame_0003.jpg", 
"frame_0004.jpg", 
"frame_0005.jpg", 
"frame_0006.jpg", 
"frame_0007.jpg", 
"frame_0008.jpg", 
"frame_0009.jpg", 
"frame_0010.jpg", 
"frame_0011.jpg", 
"frame_0012.jpg", 
"frame_0013.jpg", 
"frame_0014.jpg", 
"frame_0015.jpg", 
"frame_0016.jpg", 
"frame_0017.jpg", 
"frame_0018.jpg", 
"frame_0019.jpg", 
"frame_0020.jpg", 
"frame_0021.jpg", 
"frame_0022.jpg", 
"frame_0023.jpg", 
"frame_0024.jpg"
};
    for(int i=0;i<25;i++)
    {
    	std::cout<<path+files[i]<<std::endl;
		Mat g_srcImage,dstImage;
    	vector<Mat> g_vChannels;
    	g_srcImage = imread(path+files[i]);
    	imshow("org",g_srcImage);
    	//waitKey(0);

    	//分离通道
    	split(g_srcImage,g_vChannels);
   	 	Mat imageBlueChannel = g_vChannels.at(0);
    	Mat imageGreenChannel = g_vChannels.at(1);
    	Mat imageRedChannel = g_vChannels.at(2);

    	double imageBlueChannelAvg=0;
    	double imageGreenChannelAvg=0;
    	double imageRedChannelAvg=0;

    	//求各通道的平均值
    	imageBlueChannelAvg = mean(imageBlueChannel)[0];
    	imageGreenChannelAvg = mean(imageGreenChannel)[0];
    	imageRedChannelAvg = mean(imageRedChannel)[0];

    	//求出个通道所占增益
   	 	double K = (imageRedChannelAvg+imageGreenChannelAvg+imageRedChannelAvg)/3;
    	double Kb = K/imageBlueChannelAvg;
    	double Kg = K/imageGreenChannelAvg;
    	double Kr = K/imageRedChannelAvg;

    	//更新白平衡后的各通道BGR值
    	addWeighted(imageBlueChannel,Kb,0,0,0,imageBlueChannel);
    	addWeighted(imageGreenChannel,Kg,0,0,0,imageGreenChannel);
    	addWeighted(imageRedChannel,Kr,0,0,0,imageRedChannel);

    	merge(g_vChannels,dstImage);//图像各通道合并
    	imshow("output",dstImage);
		imwrite(path+"test"+files[i], dstImage);
    	waitKey(0);
    }
    return 0;
}
