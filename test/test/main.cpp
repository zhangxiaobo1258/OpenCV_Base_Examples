#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;
int main()
{
	Mat img = imread("2.jpg",0);
	if (img.empty())
	{
		cout << "error";
		return -1;
	}
	imwrite("22.jpg", img);
	imshow("xxµÄö¦ÕÕ", img);
	waitKey();
	return 0;
}
