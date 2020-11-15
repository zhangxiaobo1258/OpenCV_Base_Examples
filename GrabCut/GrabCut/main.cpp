#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

void main()
{
	//利用Rect做分割
	/*Mat srcImg = imread("1.jpg");
	Rect rect(55, 34, 316, 318);
	Mat result;
	Mat bgModel, fgModel;

	grabCut(srcImg, result, rect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
	imshow("grab", result);

	compare(result, GC_PR_FGD, result, CMP_EQ);
	imshow("result", result);
	Mat foreground(srcImg.size(), CV_8UC3, Scalar(255, 255, 255));
	srcImg.copyTo(foreground, result);
	imshow("foreground", foreground);
	waitKey(0);*/

	//利用mask做分割
	/*Mat srcImg = imread("1.jpg");
	Rect rect;
	Mat bgModel, fgModel;
	Mat result(srcImg.size(),CV_8U,Scalar(0));
	Mat ROI(result(Rect(55, 34, 316, 318)));
	ROI.setTo(GC_PR_FGD);
	grabCut(srcImg, result, rect, bgModel, fgModel, 1, GC_INIT_WITH_MASK);
	imshow("grab", result);

	compare(result, GC_PR_FGD, result, CMP_EQ);
	imshow("result", result);
	Mat foreground(srcImg.size(), CV_8UC3, Scalar(255, 255, 255));
	srcImg.copyTo(foreground, result);
	imshow("foreground", foreground);
	waitKey(0);*/


}
