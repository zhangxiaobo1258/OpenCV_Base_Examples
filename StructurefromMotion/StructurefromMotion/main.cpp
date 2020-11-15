/**************************************************
* ����������ʵ����ͨ����ͬ�����µĲ�ͬ�Ƕȵ�����ͼ��
* ����ȡͼ�񼸺νṹ��
* ���裺��
* ���ߣ���Ф��
* ���ڣ�2019-06-20
**************************************************/
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/flann/flann.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

/*������õ���ת�����Ƿ�Ϊһ����Ч����ת����*/
bool CheckCoherentRotation(Mat_<double>& R)
{
	if (fabsf(determinant(R)) - 1.0 > 1e-7)
	{
		cerr << "det(R) !=��1.0��this is not a rotation matrix" << endl;
		return false;
	}
	return true;
}

/*��������ı�׼�;���������Ļ�������ͨ������õ���������ڱ���P1��*/
void FindcameraMatrices(const Mat& K,
	const vector<KeyPoint> Imgpts1,
	const vector<KeyPoint> Imgpts2,
	Matx34d& P1   //����һ̨������Ļ�������
	)
{
	Mat F = findFundamentalMat(Imgpts1,Imgpts2,FM_RANSAC,1,0.99);
	Mat_<double> E = K.t()*F*K;
	//����������ʹ��SVD(����ֵ)�ֽ�Ϊ��ת��ƽ��������
	SVD svd(E, SVD::MODIFY_A);
	Mat svd_u = svd.u;
	Mat svd_vt = svd.vt;
	Mat svd_w = svd.w;

	Matx33d W(0, -1, 0,
		1, 0, 0,
		0, 0, 1);
	Mat_<double> R = svd_u*Mat(W)*svd_vt;	//������ת����
	Mat_<double> t = svd_u.col(2);			//����ƽ������

	if (!CheckCoherentRotation(R))
	{
		cout << "Resulting rotation is not coherent." << endl;
		P1 = 0;
		return;
	}
	P1 = Matx34d(R(0, 0), R(0, 1), R(0, 2), t(0), R(1, 0), R(1, 1), R(1, 2), t(1), R(2, 0), R(2, 1), R(2, 2), t(2));
}
/*����������Է���*/
Mat_<double> LinearLSTringulation(   
	Point3d u,   //��һ��ͼ�����Ѿ�ƥ��ĵ�Ӧ�����ص�
	Matx34d P,   //��һ��������Ļ�������
	Point3d u1,  //�ڶ���ͼ�����Ѿ�ƥ��ĵ�Ӧ�����ص�
	Matx34d P1)  //�ڶ�������еĻ�������
{		
	//����x��x'����
	Matx43d A(u.x*P(2, 0) - P(0, 0), u.x*P(2, 1) - P(0, 1), u.x*P(2, 2) - P(0, 2),
		u.y*P(2, 0) - P(1, 0), u.y*P(2, 1) - P(1, 1), u.y*P(2, 2) - P(1, 2),
		u1.x*P1(2, 0) - P1(0, 0), u1.x*P1(2, 1) - P1(0, 1), u1.x*P1(2, 2) - P1(0, 2),
		u1.y*P1(2, 0) - P1(1, 0), u1.y*P1(2, 1) - P1(1, 1), u1.y*P1(2, 2) - P1(1, 2));
	Matx41d B(-(u.x*P(2, 3) - P(0, 3)),
		-(u.y*P(2, 3) - P(0, 3)),
		-(u1.x*P1(2, 3) - P1(1, 3)),
		-(u1.y*P1(2, 3) - P1(1, 3)));
	Mat_<double> X;
	solve(A, B, X, DECOMP_SVD);//����ֵ�ֽ�
	return X;
}

double TriangulatePoint(const vector<KeyPoint>& pt_set1,
	const vector<KeyPoint>& pt_set2,
	const Mat& Kinv,
	const Matx34d& P,
	const Matx33d& P1,
	vector<Point3d> PointCloud)
{
	vector<double> reproj_error;
	for (unsigned int i = 0;i < pts_size;i++)
	{

	}
}

int main()
{
	/*��һ������ȡͼ��*/
	Mat srcImage1 = imread("1.jpg");
	Mat srcImage2 = imread("2.jpg");
	//��ͼƬת��Ϊ�Ҷ�ͼ��
	Mat GrayImage1, GrayImage2;
	if (srcImage1.channels() == 3)//���Դͼ��Ϊ��ɫͼ��
	{
		cvtColor(srcImage1, GrayImage1, CV_RGB2GRAY);
		cvtColor(srcImage2, GrayImage2, CV_RGB2GRAY);
	}
	else
	{
		GrayImage1 = srcImage1;
		GrayImage2 = srcImage2;
	}
	/*�ڶ�������������ͼ���ƥ���*/
	/*����һ������SURF�㷨�ͱ���ƥ��ķ�������������ƥ��*/
	/*//����SURF�������������
	Ptr<SurfFeatureDetector> surfDetector = SURF::create();
	//�������������
	vector<KeyPoint> KeyPoint1;
	vector<KeyPoint> KeyPoint2;
	//����������
	surfDetector->detect(srcImage1,KeyPoint1);
	surfDetector->detect(srcImage2, KeyPoint2);
	//����������������/����������ȡ
	Ptr<SurfDescriptorExtractor> desicritor = SURF::create();
	Mat desicritor1;
	desicritor->compute(srcImage1, KeyPoint1, desicritor1);
	Mat desicritor2;
	desicritor->compute(srcImage2, KeyPoint2, desicritor2);
	//������������BFMatch����ƥ��
	Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create(2);//ʵ��������ƥ����,2��ӦBRUTEFORCE,������ƥ��
	//����ƥ��������,����ƥ�����Ѿ����н����˲��㷨
	vector<DMatch> matches;
	matcher->match(desicritor1, desicritor2,matches);*/
	/*������������FAST�㷨�͹�����������������ƥ��*/
	//ʹ��FAST�㷨����ͼ��������㣬����㷨û�������ӣ�Ҳ����˵���㷨��������ת������
	Ptr<FastFeatureDetector> FastDetector = FastFeatureDetector::create();
	vector<KeyPoint> KeyPoint1;
	vector<KeyPoint> KeyPoint2;
	FastDetector->detect(srcImage1, KeyPoint1);
	FastDetector->detect(srcImage2, KeyPoint2);
	//Ϊ������ƥ����׼������ȡ�������������Ϣ
	vector<Point2f> Points1;
	KeyPoint::convert(KeyPoint1, Points1);
	vector<Point2f> Points2(KeyPoint2.size());
	//���������,����ͼ1��ÿ����������ͼ���еĹ�����
	vector<uchar> vstatus;//�������ͼ���е�������ƥ�䣬���־λ1������Ϊ0
	vector<float> verror;//����ƥ����������еĴ�������
	//ʹ��Lucas-Kanade����������������ƥ�� 
	calcOpticalFlowPyrLK(GrayImage1,GrayImage2,Points1, Points2, vstatus, verror);//�ú�����vedio/tracking����
	//�޳���ƥ��ĵ�
	vector<Point2f> Points2Find;    //�޳���ĵ�
	vector<int> Points2Index;			//ƥ��ĵ������
	for (unsigned int i = 0;i < vstatus.size();i++)
	{
		if (vstatus[i] == 1 && verror[i] < 12.0)
		{
			Points2Index.push_back(i);//�����ݵĿռ����·���,�ͷ�Ϊ�յ�����
			Points2Find.push_back(Points2Find[i]);
		}
		else
		{
			vstatus[i] = 0;
		}
	}
	//
	Mat Points2FindFlat = Mat(Points2Find).reshape(1, Points2Find.size());
	vector<Point2f> Feature2;
	KeyPoint::convert(KeyPoint2,Feature2);
	Mat Feature2Flat = Mat(Feature2).reshape(Feature2.size());
	
	//������������BFMatch����ƥ��
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(2);//ʵ��������ƥ����,2��ӦBRUTEFORCE,������ƥ��
																  //����ƥ��������,����ƥ�����Ѿ����н����˲��㷨
	vector<vector<DMatch>> NearestNeighbors;
	vector<DMatch> matches;
	matcher->radiusMatch(Points2FindFlat, Feature2Flat, NearestNeighbors,2.0f);

	set<int> FoundPoints2;
	for (int i = 0;i < NearestNeighbors.size();i++)
	{
		DMatch _m;
		if (NearestNeighbors[i].size() == 1)
		{
			_m = NearestNeighbors[i][0];//����һ������
		}
		else if (NearestNeighbors[i].size() > 1)
		{
			double Ratio = NearestNeighbors[i][0].distance / NearestNeighbors[i][1].distance;
			if (Ratio < 0.7)
			{
				_m = NearestNeighbors[i][0];
			}
			else
			{
				continue;			//û��ͨ������
			}
		}
		else
		{
			continue;			//û������
		}
		if (FoundPoints2.find(_m.trainIdx) == FoundPoints2.end())
		{
			_m.queryIdx = Points2Index[_m.queryIdx];
			matches.push_back(_m);
			FoundPoints2.insert(_m.trainIdx);
		}
	}
	cout << "pruned" << matches.size() << "/" << NearestNeighbors.size() << "matches" << endl;
	/*����������������������Ķ�Ӧ����*/
	vector<Point2f> ImgPts1, ImgPts2;
	vector<uchar> RANSACStatus;
	for (unsigned int i = 0;i < matches.size();i++)
	{
		ImgPts1.push_back(KeyPoint1[matches[i].queryIdx].pt);
		ImgPts2.push_back(KeyPoint2[matches[i].trainIdx].pt);
	}
	Mat F = findFundamentalMat(ImgPts1,ImgPts2,FM_RANSAC,0.1,0.99, RANSACStatus);
	//Mat K;//�þ���Ϊ������ı궨����������Ҫ��������ı궨������ʱ�Ȳ�����
	//Mat_<double> E = K.t()*F*K;//K.t()��ʾK�����ת�þ���

	return 0;
}