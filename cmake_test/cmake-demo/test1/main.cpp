#include "a/a0/a0.h"
#include "a/a.h"
#include "b/b0/b0.h"
#include <opencv/highgui.h>

int main(void){
	IplImage *img = cvLoadImage("test.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	a(1);
	a0(2);
	b0(3);

    cvShowImage("image", img);
    cvWaitKey(0);
	return 0;
}