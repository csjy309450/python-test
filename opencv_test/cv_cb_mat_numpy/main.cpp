#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "CallPython.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {
    CallPython py;
    py.test0();
    py.test0();
//    py.numpy_test();
    return 0;
}