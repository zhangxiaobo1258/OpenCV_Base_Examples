#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

void main()
{
	//���Ƶ㼯��͹��
	/*Mat img(500, 500, CV_8UC3, Scalar::all(0));//�������ͼ����ͨ��������Ϊ0
	RNG rng;//�������������

	while (1)
	{
		char key;
		int count = (unsigned int)rng % 100;//�����ĸ���
		vector<Point>points;
		for (int i = 0;i < count;i++)
		{
			Point pt;
			pt.x = rng.uniform(img.cols / 4, img.cols * 3 / 4);//�趨���x��Χ
			pt.y = rng.uniform(img.rows / 4, img.rows * 3 / 4);//�趨���y��Χ
			points.push_back(pt);
		}
		//���͹��
		vector<int> hull;
		convexHull(Mat(points), hull, true);

		img = Scalar::all(0);
		for (int i = 0;i < count;i++)
		{
			circle(img, points[i], 3, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), CV_FILLED, CV_AA);
		}
		int hullcount = (int)hull.size();//͹���ı���
		Point point0 = points[hull[hullcount - 1]];//����͹���ߵ�������ʼ��Ϊ���һ����

		//����͹���ı�
		for (int i = 0;i < hullcount;i++)
		{
			Point point = points[hull[i]];
			line(img, point0, point, Scalar(255, 255, 255), 2, CV_AA);
			point0 = point;
		}

		imshow("img", img);
		key = (char)waitKey();
		if (key == 27 || key == 'Q' || key == 'q')
		{
			break;
		}
	}
	return;*/

	//����������͹��
	Mat srcImg = imread("1.jpg");
	Mat dstImg(srcImg.rows, srcImg.cols, CV_8UC3, Scalar::all(0));
	cvtColor(srcImg, srcImg, CV_BGR2BGRA);
	threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY_INV);
	vector<vector<Point>> Contours;
	vector<Vec4i> hierarchy;
	findContours(srcImg, Contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	vector<vector<Point>> hull(Contours.size());
	for (int i = 0;i < Contours.size();i++)
	{
		convexHull(Mat(Contours[i]), hull[i], true);//����͹��
	}
	for (int i = 0;i < Contours.size();i++)
	{
		drawContours(dstImg, Contours, i, Scalar(255, 255, 255), 1, 8);//��������
		drawContours(dstImg, hull, i, Scalar(rand()%255, rand()%255, rand()%255), 1, 8);//��������
	}

	imshow("dst", dstImg);
	waitKey(0);
}

