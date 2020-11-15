#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\videoio\videoio.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

/*�����˶����壬��������˶��������������Ӿ���*/
void  MotionDetect(Mat Ref_Img,										//�ο�ͼ��
	Mat Dist_Img,													//Ŀ��ͼ��
	double Threshold,												//��ֵ����ֵ
	InputArray ErodeKernel,											//��ʴ�˳ߴ� 
	InputArray DilateKernel											//���ͺ˳ߴ�
	)
{
												
	Mat Gray_Ref;
	Mat Gray_Dist;
	Mat Diff_Img;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	
	cvtColor(Ref_Img, Gray_Ref, CV_BGR2GRAY);								//�ο�ͼ��ҶȻ�����
	cvtColor(Dist_Img, Gray_Dist, CV_BGR2GRAY);								//Ŀ��ͼ��ҶȻ�����
	absdiff(Gray_Ref, Gray_Dist, Diff_Img);									//�ο�ͼ���Ŀ��ͼ��ľ��Բ�ֵ
	imshow("absdiff Image", Diff_Img);
	threshold(Diff_Img, Diff_Img, Threshold, 255, THRESH_BINARY);			//��������ͼ����ж�ֵ��
	imshow("Threshold Image", Diff_Img);
	erode(Diff_Img, Diff_Img, ErodeKernel);
	dilate(Diff_Img, Diff_Img, DilateKernel);
	//dilate(Diff_Img, Diff_Img, DilateKernel);
	imshow("Diff img", Diff_Img);
	findContours(Diff_Img, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);//��������
	vector<Rect> boundRect(contours.size());//������Ӿ��εļ���
	int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
	//drawContours(Dist_Img, contours, -1, Scalar(0, 0, 255), 3, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//����ÿ����������Ӿ���
		x0 = boundRect[i].x;
		y0 = boundRect[i].y;
		w0 = boundRect[i].width;
		h0 = boundRect[i].height;
		rectangle(Dist_Img, Point(x0, y0), Point(x0 + w0, y0 + h0),Scalar(0,255,0), 2, 8);
	}
}

void main()
{

	Mat ErodeKernel = getStructuringElement(MORPH_RECT, Size(5, 5));		//���帯ʴ�˴�С
	Mat DilateKernel = getStructuringElement(MORPH_RECT, Size(50, 50));		//�������ͺ˵Ĵ�С

	VideoCapture Video;
	double fps;
	int count=1;
	Mat Ref_Image,Dist_Image,Frame_Image;
	Video.open("2.avi");
	if ( ! Video.isOpened())
	{
		cout << "Video opened failed!" << endl;
		exit(1);
	}
	fps = Video.get(CV_CAP_PROP_FPS);//��ȡ��Ƶ��֡��
	Video >> Frame_Image;//����һ֡����д��ο�֡��
	Frame_Image.copyTo(Ref_Image);
	while(1)
	{
		count++;
		Video >> Frame_Image;//���ڶ�֡����д��Ŀ��֡
		Frame_Image.copyTo(Dist_Image);
		if (Dist_Image.empty())
		{
			break;
		}
		MotionDetect(Ref_Image, Dist_Image, 30, ErodeKernel, DilateKernel);
		Frame_Image.copyTo(Ref_Image);
		imshow("��Ƶ��ʾ", Dist_Image);
		int c = waitKey(30);
		if (c == 'q' || c == 'Q' || (c & 255) == 27)
			break;
	}
	destroyAllWindows();
	exit(1);
}
