#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using  namespace cv;

void main()
{
	Mat srcImg = imread("1.jpg");
	Mat dstImg;

	//�����˲�
	//boxFilter(srcImg, dstImg, -1, Size(3, 3), Point(-1, -1), true);//�����˲���
	//blur(srcImg, dstImg, Size(3, 3));//��ֵ�˲�
	//GaussianBlur(srcImg, dstImg, Size(3, 3), 1);//��˹�˲����õñȽ϶�

	//�������˲�
	//medianBlur(srcImg, dstImg, 3);//��ֵ�˲��ķ���
	bilateralFilter(srcImg, dstImg, 5, 10.0, 2);//˫���˲���

	imshow("src", srcImg);
	imshow("dst", dstImg);
	waitKey(0);
}
