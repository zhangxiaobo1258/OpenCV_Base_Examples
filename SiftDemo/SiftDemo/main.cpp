#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

void main()
{
	Mat srcImg1 = imread("00.png");
	Mat srcImg2 = imread("01.jpg");

	//定义SIFT特征检测类对象
	Ptr<SiftFeatureDetector>  siftDetector1 = SIFT::create();
	Ptr<SiftFeatureDetector>  siftDetector2 = SIFT::create();
	//定义keyPoint变量
	vector<KeyPoint> KeyPoint1;
	vector<KeyPoint> KeyPoint2;

	//特征点检测
	siftDetector1->detect(srcImg1,KeyPoint1);
	siftDetector2->detect(srcImg2,KeyPoint2);
	//绘制特征点
	Mat Feature_Img1, Feature_Img2;
//	drawKeypoints(srcImg1,KeyPoint1,Feature_Img1,Scalar(0,0,255));
//	drawKeypoints(srcImg2,KeyPoint2,Feature_Img2,Scalar(0,0,255));
	drawKeypoints(srcImg1, KeyPoint1, Feature_Img1, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(srcImg2, KeyPoint2, Feature_Img2, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//显示原图
	imshow("src1", srcImg1);
	imshow("src2", srcImg2);
	//显示结果
	imshow("feature1", Feature_Img1);
	imshow("feature2", Feature_Img2);

	//计算特征点描述符/特征向量提取
	Ptr<SiftDescriptorExtractor> desicritor=SIFT::create();
	Mat desicritor1;
	desicritor->compute(srcImg1, KeyPoint1, desicritor1);
	Mat desicritor2;
	desicritor->compute(srcImg2, KeyPoint2, desicritor2);
	cout << desicritor1.cols << endl;
	cout << desicritor2.rows << endl;

	//进行BFMatch暴力匹配
	Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create(2);//实例化暴力匹配器,2对应Opencv2.4.8中的NORM_L2
	vector<DMatch> matches;//定义匹配结果变量
	matcher->match(desicritor1, desicritor2, matches);//实现描述符之间的匹配

	//匹配结果删选方法一：
	/*nth_element(matches.begin(), matches.begin() + 29, matches.end());//提取出前30个最佳匹配结果
	matches.erase(matches.begin() + 30, matches.end());*///剔除其余的匹配结果
	
	//匹配结果删选方法二：
	double max_dist = 0, min_dist = 100;
	for (int i = 0;i < desicritor1.rows;i++)
	{
		if (matches.at(i).distance > max_dist)
		{
			max_dist = matches[i].distance;
		}
		if (matches.at(i).distance < min_dist)
		{
			min_dist = matches[i].distance;
		}
	}
	cout << "min_distance = " << min_dist << endl;
	cout << "max_distance = " << max_dist << endl;
	vector<DMatch> good_matches;
	for (int i = 0;i < matches.size();i++)
	{
		if (matches[i].distance < 2 * min_dist)
		{
			good_matches.push_back(matches[i]);//将匹配比较好的效果的结果放在good_matches中
		}
	}


	Mat result;
 	drawMatches(srcImg1, KeyPoint1, srcImg2, KeyPoint2, good_matches, result, Scalar(0, 255, 0), Scalar::all(-1));
	imshow("Match result", result);

	waitKey(0);
	destroyAllWindows();
}

