//OpenCV自带的例程
/*#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
	cout << "\nThis program demonstrates the famous watershed segmentation algorithm in OpenCV: watershed()\n"
		"Usage:\n"
		"./watershed [image_name -- default is ../data/fruits.jpg]\n" << endl;


	cout << "Hot keys: \n"
		"\tESC - quit the program\n"
		"\tr - restore the original image\n"
		"\tw or SPACE - run watershed segmentation algorithm\n"
		"\t\t(before running it, *roughly* mark the areas to segment on the image)\n"
		"\t  (before that, roughly outline several markers on the image)\n";
}
Mat markerMask, img;
Point prevPt(-1, -1);

static void onMouse(int event, int x, int y, int flags, void*)
{
	if (x < 0 || x >= img.cols || y < 0 || y >= img.rows)
		return;
	if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
		prevPt = Point(-1, -1);
	else if (event == EVENT_LBUTTONDOWN)
		prevPt = Point(x, y);
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point pt(x, y);
		if (prevPt.x < 0)
			prevPt = pt;
		line(markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0);
		line(img, prevPt, pt, Scalar::all(255), 5, 8, 0);
		prevPt = pt;
		imshow("image", img);
	}
}

int main(int argc, char** argv)
{
	cv::CommandLineParser parser(argc, argv, "{help h | | }{ @input | ../data/fruits.jpg | }");
	if (parser.has("help"))
	{
		help();
		return 0;
	}
	string filename = ("fruits.jpg");//parser.get<string>("@input");
	Mat img0 = imread(filename, 1), imgGray;

	if (img0.empty())
	{
		cout << "Couldn'g open image " << filename << ". Usage: watershed <image_name>\n";
		return 0;
	}
	help();
	namedWindow("image", 1);

	img0.copyTo(img);
	cvtColor(img, markerMask, COLOR_BGR2GRAY);
	cvtColor(markerMask, imgGray, COLOR_GRAY2BGR);
	markerMask = Scalar::all(0);
	imshow("image", img);
	setMouseCallback("image", onMouse, 0);

	for (;;)
	{
		char c = (char)waitKey(0);

		if (c == 27)
			break;

		if (c == 'r')
		{
			markerMask = Scalar::all(0);
			img0.copyTo(img);
			imshow("image", img);
		}

		if (c == 'w' || c == ' ')
		{
			int i, j, compCount = 0;
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			findContours(markerMask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

			if (contours.empty())
				continue;
			Mat markers(markerMask.size(), CV_32S);
			markers = Scalar::all(0);
			int idx = 0;
			for (; idx >= 0; idx = hierarchy[idx][0], compCount++)
				drawContours(markers, contours, idx, Scalar::all(compCount + 1), -1, 8, hierarchy, INT_MAX);

			if (compCount == 0)
				continue;

			vector<Vec3b> colorTab;
			for (i = 0; i < compCount; i++)
			{
				int b = theRNG().uniform(0, 255);
				int g = theRNG().uniform(0, 255);
				int r = theRNG().uniform(0, 255);

				colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
			}

			double t = (double)getTickCount();
			watershed(img0, markers);
			t = (double)getTickCount() - t;
			printf("execution time = %gms\n", t*1000. / getTickFrequency());

			Mat wshed(markers.size(), CV_8UC3);

			// paint the watershed image
			for (i = 0; i < markers.rows; i++)
				for (j = 0; j < markers.cols; j++)
				{
					int index = markers.at<int>(i, j);
					if (index == -1)
						wshed.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
					else if (index <= 0 || index > compCount)
						wshed.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
					else
						wshed.at<Vec3b>(i, j) = colorTab[index - 1];
				}

			wshed = wshed*0.5 + imgGray*0.5;
			imshow("watershed transform", wshed);
		}
	}

	return 0;
}*/

//例程

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/photo/photo.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	//分水岭算法
	Mat srcImg = imread("1.jpg");
	imshow("src", srcImg);
	Mat dstImg = srcImg.clone();
	Mat imgMark(srcImg.size(), CV_8U, Scalar(0));
	//标示背景图像
	rectangle(imgMark, Point(1, 1), Point(srcImg.cols - 2, srcImg.rows - 2), Scalar(255), 1);
	//标示鸟
	rectangle(imgMark, Point(227, 146), Point(247, 166), Scalar(128), 10);
	//标示岩石
	rectangle(imgMark, Point(198,300), Point(224, 330), Scalar(64), 10);
	imshow("mark", imgMark);
	imgMark.convertTo(imgMark, CV_32S);
	watershed(srcImg, imgMark);//使用分水岭算法
	Mat mark1, mark2;
	imgMark.convertTo(mark1, CV_8U);
	imshow("marker", mark1);
	
	Mat mark3 = mark1.clone();
	bitwise_not(mark1, mark2);
	threshold(mark2, mark2, 64, 255, CV_THRESH_BINARY);
	threshold(mark2, mark2, 128, 255, CV_THRESH_BINARY);
	threshold(mark3, mark3, 128, 255, CV_THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(mark1, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(dstImg, contours, -1, Scalar(255, 0, 0), -1, 8);
	
	vector<vector<Point>> contours2;
	vector<Vec4i> hierarchy2;
	findContours(mark2, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(dstImg, contours2, -1, Scalar(0, 255, 0), -1, 8);
	
	vector<vector<Point>> contours3;
	vector<Vec4i> hierarchy3;
	findContours(mark3, contours3, hierarchy3, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(dstImg, contours3, -1, Scalar(255, 0, 0), -1, 8);	
	
	Mat Result = srcImg*0.5 + dstImg*0.5;
	imshow("result", Result);
	waitKey(0);

}


