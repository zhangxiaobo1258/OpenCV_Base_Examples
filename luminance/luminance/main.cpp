#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#define WIN_NAME "���ͼ��"

Mat srcImg,dstImg;
int Contrase_value = 20;
int Bright_Vaule = 20;

void OnChange(int, void*)
{
	for (int i = 0;i < srcImg.rows;i++)
	{
		for (int j = 0;j < srcImg.cols;j++)
		{
			dstImg.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(Contrase_value*0.01*srcImg.at<Vec3b>(i, j)[0] + Bright_Vaule);
			dstImg.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(Contrase_value*0.01*srcImg.at<Vec3b>(i, j)[1] + Bright_Vaule);
			dstImg.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(Contrase_value*0.01*srcImg.at<Vec3b>(i, j)[2] + Bright_Vaule);
		}
	}
	imshow("ԭͼ",srcImg);
	imshow(WIN_NAME,dstImg);
}

void main()
{
	srcImg = imread("1.jpg");
	dstImg = Mat::zeros(srcImg.size(),srcImg.type());//��Mat��Ԫ�ض���Ϊ0
	//Mat::zeros();//��Mat��Ԫ�ض���Ϊ0
	//Mat::ones();//��Mat��Ԫ�ض���Ϊ1

	namedWindow(WIN_NAME,CV_WINDOW_AUTOSIZE);
	createTrackbar("�Աȶ�",WIN_NAME,&Contrase_value,300,OnChange,0);
	createTrackbar("����",WIN_NAME,&Contrase_value,200,OnChange,0);
	OnChange(Contrase_value,0);//�ص�������ʼ��
	OnChange(Bright_Vaule,0);//�ص�������ʼ��
	waitKey(0);
}