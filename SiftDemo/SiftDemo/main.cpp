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

	//����SIFT������������
	Ptr<SiftFeatureDetector>  siftDetector1 = SIFT::create();
	Ptr<SiftFeatureDetector>  siftDetector2 = SIFT::create();
	//����keyPoint����
	vector<KeyPoint> KeyPoint1;
	vector<KeyPoint> KeyPoint2;

	//��������
	siftDetector1->detect(srcImg1,KeyPoint1);
	siftDetector2->detect(srcImg2,KeyPoint2);
	//����������
	Mat Feature_Img1, Feature_Img2;
//	drawKeypoints(srcImg1,KeyPoint1,Feature_Img1,Scalar(0,0,255));
//	drawKeypoints(srcImg2,KeyPoint2,Feature_Img2,Scalar(0,0,255));
	drawKeypoints(srcImg1, KeyPoint1, Feature_Img1, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(srcImg2, KeyPoint2, Feature_Img2, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//��ʾԭͼ
	imshow("src1", srcImg1);
	imshow("src2", srcImg2);
	//��ʾ���
	imshow("feature1", Feature_Img1);
	imshow("feature2", Feature_Img2);

	//����������������/����������ȡ
	Ptr<SiftDescriptorExtractor> desicritor=SIFT::create();
	Mat desicritor1;
	desicritor->compute(srcImg1, KeyPoint1, desicritor1);
	Mat desicritor2;
	desicritor->compute(srcImg2, KeyPoint2, desicritor2);
	cout << desicritor1.cols << endl;
	cout << desicritor2.rows << endl;

	//����BFMatch����ƥ��
	Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create(2);//ʵ��������ƥ����,2��ӦOpencv2.4.8�е�NORM_L2
	vector<DMatch> matches;//����ƥ��������
	matcher->match(desicritor1, desicritor2, matches);//ʵ��������֮���ƥ��

	//ƥ����ɾѡ����һ��
	/*nth_element(matches.begin(), matches.begin() + 29, matches.end());//��ȡ��ǰ30�����ƥ����
	matches.erase(matches.begin() + 30, matches.end());*///�޳������ƥ����
	
	//ƥ����ɾѡ��������
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
			good_matches.push_back(matches[i]);//��ƥ��ȽϺõ�Ч���Ľ������good_matches��
		}
	}


	Mat result;
 	drawMatches(srcImg1, KeyPoint1, srcImg2, KeyPoint2, good_matches, result, Scalar(0, 255, 0), Scalar::all(-1));
	imshow("Match result", result);

	waitKey(0);
	destroyAllWindows();
}

