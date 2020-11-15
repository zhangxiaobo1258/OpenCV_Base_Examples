#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace cv;
using namespace std;

void main()
{
	Mat img = imread("1.jpg");
	Mat dstimg;
	vector<Mat> channels;
	split(img, channels);//通道分离，显示的结果是灰度图
	Mat blueChannel = channels.at(0);
	Mat greenChannel = channels.at(1);
	Mat redChannel = channels.at(2);

	threshold(blueChannel, blueChannel, 200, 255, THRESH_BINARY);//二值化
	threshold(greenChannel, greenChannel, 200, 255, THRESH_BINARY);//二值化
	threshold(redChannel, redChannel, 200, 255, THRESH_BINARY);//二值化
	merge(channels,dstimg);//通道合并

	imshow("Blue", blueChannel);
	imshow("Green", greenChannel);
	imshow("Red", redChannel);
	imshow("dst", dstimg);

	waitKey(0);
}
