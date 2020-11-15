/**************************************************
* 描述：本例实现了通过相同场景下的不同角度的两幅图像
* 来提取图像几何结构。
* 步骤：①
* 作者：张肖波
* 日期：2019-06-20
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

/*检查所得的旋转部分是否为一个有效的旋转矩阵*/
bool CheckCoherentRotation(Mat_<double>& R)
{
	if (fabsf(determinant(R)) - 1.0 > 1e-7)
	{
		cerr << "det(R) !=±1.0，this is not a rotation matrix" << endl;
		return false;
	}
	return true;
}

/*计算相机的标准型矩阵，摄像机的基础矩阵通过计算得到，并存放在变量P1中*/
void FindcameraMatrices(const Mat& K,
	const vector<KeyPoint> Imgpts1,
	const vector<KeyPoint> Imgpts2,
	Matx34d& P1   //另外一台摄像机的基础矩阵
	)
{
	Mat F = findFundamentalMat(Imgpts1,Imgpts2,FM_RANSAC,1,0.99);
	Mat_<double> E = K.t()*F*K;
	//将本征矩阵使用SVD(奇异值)分解为旋转和平移两部分
	SVD svd(E, SVD::MODIFY_A);
	Mat svd_u = svd.u;
	Mat svd_vt = svd.vt;
	Mat svd_w = svd.w;

	Matx33d W(0, -1, 0,
		1, 0, 0,
		0, 0, 1);
	Mat_<double> R = svd_u*Mat(W)*svd_vt;	//计算旋转矩阵
	Mat_<double> t = svd_u.col(2);			//计算平移向量

	if (!CheckCoherentRotation(R))
	{
		cout << "Resulting rotation is not coherent." << endl;
		P1 = 0;
		return;
	}
	P1 = Matx34d(R(0, 0), R(0, 1), R(0, 2), t(0), R(1, 0), R(1, 1), R(1, 2), t(1), R(2, 0), R(2, 1), R(2, 2), t(2));
}
/*求解非其次线性方程*/
Mat_<double> LinearLSTringulation(   
	Point3d u,   //第一个图像中已经匹配的单应性像素点
	Matx34d P,   //第一个摄像机的基础矩阵
	Point3d u1,  //第二个图像中已经匹配的单应性像素点
	Matx34d P1)  //第二个相机中的基础矩阵
{		
	//建立x和x'矩阵
	Matx43d A(u.x*P(2, 0) - P(0, 0), u.x*P(2, 1) - P(0, 1), u.x*P(2, 2) - P(0, 2),
		u.y*P(2, 0) - P(1, 0), u.y*P(2, 1) - P(1, 1), u.y*P(2, 2) - P(1, 2),
		u1.x*P1(2, 0) - P1(0, 0), u1.x*P1(2, 1) - P1(0, 1), u1.x*P1(2, 2) - P1(0, 2),
		u1.y*P1(2, 0) - P1(1, 0), u1.y*P1(2, 1) - P1(1, 1), u1.y*P1(2, 2) - P1(1, 2));
	Matx41d B(-(u.x*P(2, 3) - P(0, 3)),
		-(u.y*P(2, 3) - P(0, 3)),
		-(u1.x*P1(2, 3) - P1(1, 3)),
		-(u1.y*P1(2, 3) - P1(1, 3)));
	Mat_<double> X;
	solve(A, B, X, DECOMP_SVD);//特征值分解
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
	/*第一步：获取图像*/
	Mat srcImage1 = imread("1.jpg");
	Mat srcImage2 = imread("2.jpg");
	//将图片转换为灰度图像
	Mat GrayImage1, GrayImage2;
	if (srcImage1.channels() == 3)//如果源图像为彩色图像
	{
		cvtColor(srcImage1, GrayImage1, CV_RGB2GRAY);
		cvtColor(srcImage2, GrayImage2, CV_RGB2GRAY);
	}
	else
	{
		GrayImage1 = srcImage1;
		GrayImage2 = srcImage2;
	}
	/*第二步：计算两个图像的匹配点*/
	/*方法一：采用SURF算法和暴力匹配的方法进行特征点匹配*/
	/*//定义SURF特征点检测类对象
	Ptr<SurfFeatureDetector> surfDetector = SURF::create();
	//定义特征点变量
	vector<KeyPoint> KeyPoint1;
	vector<KeyPoint> KeyPoint2;
	//计算特征点
	surfDetector->detect(srcImage1,KeyPoint1);
	surfDetector->detect(srcImage2, KeyPoint2);
	//计算特征点描述符/特征向量提取
	Ptr<SurfDescriptorExtractor> desicritor = SURF::create();
	Mat desicritor1;
	desicritor->compute(srcImage1, KeyPoint1, desicritor1);
	Mat desicritor2;
	desicritor->compute(srcImage2, KeyPoint2, desicritor2);
	//对描述符进行BFMatch暴力匹配
	Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create(2);//实例化暴力匹配器,2对应BRUTEFORCE,即暴力匹配
	//定义匹配结果变量,暴力匹配中已经含有交叉滤波算法
	vector<DMatch> matches;
	matcher->match(desicritor1, desicritor2,matches);*/
	/*方法二：采用FAST算法和光流方法进行特征点匹配*/
	//使用FAST算法查找图像的特征点，这个算法没有描述子，也就是说该算法不具有旋转不变性
	Ptr<FastFeatureDetector> FastDetector = FastFeatureDetector::create();
	vector<KeyPoint> KeyPoint1;
	vector<KeyPoint> KeyPoint2;
	FastDetector->detect(srcImage1, KeyPoint1);
	FastDetector->detect(srcImage2, KeyPoint2);
	//为光流法匹配做准备，获取特征点的坐标信息
	vector<Point2f> Points1;
	KeyPoint::convert(KeyPoint1, Points1);
	vector<Point2f> Points2(KeyPoint2.size());
	//计算光流域,计算图1中每个点在两个图像中的光流域
	vector<uchar> vstatus;//如果两个图像中的特征点匹配，则标志位1，否则为0
	vector<float> verror;//定义匹配测量过程中的错误类型
	//使用Lucas-Kanade金字塔光流法计算匹配 
	calcOpticalFlowPyrLK(GrayImage1,GrayImage2,Points1, Points2, vstatus, verror);//该函数在vedio/tracking库中
	//剔除不匹配的点
	vector<Point2f> Points2Find;    //剔除后的点
	vector<int> Points2Index;			//匹配的点的索引
	for (unsigned int i = 0;i < vstatus.size();i++)
	{
		if (vstatus[i] == 1 && verror[i] < 12.0)
		{
			Points2Index.push_back(i);//对数据的空间重新分配,释放为空的数据
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
	
	//对描述符进行BFMatch暴力匹配
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(2);//实例化暴力匹配器,2对应BRUTEFORCE,即暴力匹配
																  //定义匹配结果变量,暴力匹配中已经含有交叉滤波算法
	vector<vector<DMatch>> NearestNeighbors;
	vector<DMatch> matches;
	matcher->radiusMatch(Points2FindFlat, Feature2Flat, NearestNeighbors,2.0f);

	set<int> FoundPoints2;
	for (int i = 0;i < NearestNeighbors.size();i++)
	{
		DMatch _m;
		if (NearestNeighbors[i].size() == 1)
		{
			_m = NearestNeighbors[i][0];//仅有一个邻域
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
				continue;			//没有通过测试
			}
		}
		else
		{
			continue;			//没有邻域
		}
		if (FoundPoints2.find(_m.trainIdx) == FoundPoints2.end())
		{
			_m.queryIdx = Points2Index[_m.queryIdx];
			matches.push_back(_m);
			FoundPoints2.insert(_m.trainIdx);
		}
	}
	cout << "pruned" << matches.size() << "/" << NearestNeighbors.size() << "matches" << endl;
	/*第三步：计算两个摄像机的对应矩阵*/
	vector<Point2f> ImgPts1, ImgPts2;
	vector<uchar> RANSACStatus;
	for (unsigned int i = 0;i < matches.size();i++)
	{
		ImgPts1.push_back(KeyPoint1[matches[i].queryIdx].pt);
		ImgPts2.push_back(KeyPoint2[matches[i].trainIdx].pt);
	}
	Mat F = findFundamentalMat(ImgPts1,ImgPts2,FM_RANSAC,0.1,0.99, RANSACStatus);
	//Mat K;//该矩阵为摄像机的标定矩阵，由于需要计算相机的标定矩阵，暂时先不计算
	//Mat_<double> E = K.t()*F*K;//K.t()表示K矩阵的转置矩阵

	return 0;
}