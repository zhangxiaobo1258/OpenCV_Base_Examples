/*ROI 图像融合*/

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

void main()
{
	Mat img = imread("1.jpg");
	Mat Logo = imread("opencv.jpg");
	Mat maskImg = imread("opencv.jpg",0);
	bitwise_not(maskImg,maskImg);
	//imshow("mask", maskImg);
	threshold(maskImg,maskImg,90,255,THRESH_BINARY);
	//imshow("mask", maskImg);
	//Mat maskImg = Mat::zeros(Logo.size(),CV_8UC1);//先创建一个纯黑的图像
	//circle(maskImg,Point(maskImg.rows/2,maskImg.cols/2),50,Scalar(255),-1,8);//画一个填充的白色圆

	//Mat imgROI = img(Rect(20,20,200,200));//Rect方法定义ROI
	//Mat imgROI = img(Range(20,220),Range(20,220));//Range方法定义ROI
	Mat imgROI = img(Rect(20,20,Logo.cols,Logo.rows));//Rect方法定义ROI
	//Logo.copyTo(imgROI);
	Logo.copyTo(imgROI, maskImg);


	imshow("dst", img);
	//imshow("ROI", imgROI);
	//imwrite("ROI.jpg",imgROI);
	waitKey(0);
}
