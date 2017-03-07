#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  
   
#include <iostream>  
   
using namespace cv;  
using namespace std;  

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
  
// Normalizes a given image into a value range between 0 and 255.  
Mat norm_0_255(const Mat& src) {  
    // Create and return normalized image:  
    Mat dst;  
    switch(src.channels()) {  
    case 1:  
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);  
        break;  
    case 3:  
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);  
        break;  
    default:  
        src.copyTo(dst);  
        break;  
    }  
    return dst;  
}  
  
int main(int argc, const char *argv[]) {  
    // Get filename to the source image:  
    // Load image & get skin proportions:  
    //Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);  
	for(int i=0;i<25;i++)
	{
		Mat image = imread(path+files[i], CV_LOAD_IMAGE_COLOR);  
    	// Convert to floating point:  
    	Mat X;  
    	image.convertTo(X, CV_32FC1);  
    	//image.convertTo(X, CV_32F);  
    	// Start preprocessing:  
    	Mat I;  
    	float gamma = 1/2.2;  
    	pow(X, gamma, I);  
  
  
    	// Draw it on screen:  
    	imshow("Original Image", image);  
    	imshow("Gamma correction image", norm_0_255(I));  
    	//imwrite("origin.jpg", image);  
    	imwrite(path+"gammaCorrection/test"+files[i], norm_0_255(I));  
    	// Show the images:  
    	waitKey(0);  
	}
    
    // Success!  
    return 0;  
}  
