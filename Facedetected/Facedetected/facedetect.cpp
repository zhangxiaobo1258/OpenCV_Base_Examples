#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void help()//显示程序的信息
{
    cout << "\nThis program demonstrates the use of cv::CascadeClassifier class to detect objects (Face + eyes). You can use Haar or LBP features.\n"
            "This classifier can recognize many kinds of rigid objects, once the appropriate classifier is trained.\n"
            "It's most known use is for faces.\n"
            "Usage:\n"
            "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
               "   [--nested-cascade[=nested_cascade_path this an optional secondary classifier such as eyes]]\n"
               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>]\n"
               "   [--try-flip]\n"
               "   [filename|camera_index]\n\n"
            "see facedetect.cmd for one call:\n"
            "./facedetect --cascade=\"../../data/haarcascades/haarcascade_frontalface_alt.xml\" --nested-cascade=\"../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml\" --scale=1.3\n\n"
            "During execution:\n\tHit any key to quit.\n"
            "\tUsing OpenCV version " << CV_VERSION << "\n" << endl;
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );
string cascadeName;											//分类器名称
string nestedCascadeName;									//分类器名称
int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame, image;
    string inputName;
    bool tryflip;
    CascadeClassifier cascade, nestedCascade;
    double scale;
	//命令行解析函数
    cv::CommandLineParser parser(argc, argv,
        "{help h||}"
        "{cascade|haarcascade_frontalface_alt.xml|}"
        "{nested-cascade|haarcascade_eye_tree_eyeglasses.xml|}"
        "{scale|1|}{try-flip||}{@filename||}"
    );
	//解析help命令
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    cascadeName = parser.get<string>("cascade");
    nestedCascadeName = parser.get<string>("nested-cascade");
    scale = parser.get<double>("scale");    //该值为命令中scale后面的值
    if (scale < 1)
        scale = 1;
    tryflip = parser.has("try-flip");
    inputName = parser.get<string>("@filename");
    if (!parser.check())                  //检查命令是否正确
    {
        parser.printErrors();
        return 0;
    }
	//加载nestedCascade分类器
	if ( !nestedCascade.load( nestedCascadeName ) )
        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
	//加载Cascade分类器
	if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        help();
        return -1;
    }
	///	inputName.empty()返回inputName是否为空
	/// isdigit()检查inputName是否为十进制数字字符
	/// inputName.size()检测inputName的长度
	if( inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1) )
    {
		//视频输入为摄像头
		int camera = inputName.empty() ? 0 : inputName[0] - '0';
		//打开计算机上的摄像头，如果打开失败，弹出下面信息
		if(!capture.open(camera))
            cout << "Capture from camera #" <<  camera << " didn't work" << endl;
    }
	//输入文件名长度为非0情况下
	else if( inputName.size() )
    {
		//读取输入文件
		image = imread( inputName, 1 );
		//image为非空
		if( image.empty() )
        {
            if(!capture.open( inputName ))
                cout << "Could not read " << inputName << endl;
        }
    }
	//默认输入
	else
    {
        image = imread( "lena.jpg", 1 );
        if(image.empty()) cout << "Couldn't read lena.jpg" << endl;
    }
	//判断摄像头是否初始化完成
	if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;
        for(;;)
        {
			//将摄像头读取的数据传输至Frame中
			capture >> frame;
            if( frame.empty() )
                break;
			//复制frame到frame1
			Mat frame1 = frame.clone();
			//将Frame1中的图像数据采用cascade和nestCascade分类器进行检测
			detectAndDraw( frame1, cascade, nestedCascade, scale, tryflip );
            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
    }
	//其他情况，即检测图片中的人脸
    else
	{
        cout << "Detecting face(s) in " << inputName << endl;
		//如果image.empty()返回0，则说明image非空
		if( !image.empty() )
        {
            //对图片采用cascade和nestCascade分类器进行检测
			detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
            waitKey(0);
        }
		//如果输入文件返回0，则说明inputName非空
        else if( !inputName.empty() )
        {
            /* assume it is a text file containing the
            list of the image filenames to be processed - one per line */
			//以只读方式打开文本文档，只允许读取数据，inputName.c_str()函数得到一个等效C的字符串
			FILE* f;
			fopen_s(&f, inputName.c_str(), "rt" );
			//FILE* f = fopen( inputName.c_str(), "rt" );  //原文中使用该函数，但在VS2015编译过程中会出现警告，建议使用fopen_s()函数更安全。
			if( f )
            {
                char buf[1000+1];
				//从文件f中读取一行数据存储在buf中
				while( fgets( buf, 1000, f ) )
                {
					//计算读取的buf的长度
					int len = (int)strlen(buf);
					//如果buf非空
					while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';
                    cout << "file " << buf << endl;
                    image = imread( buf, 1 );
                    if( !image.empty() )
                    {
                        detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
                        char c = (char)waitKey(0);
                        if( c == 27 || c == 'q' || c == 'Q' )
                            break;
                    }
                    else
                    {
                        cerr << "Aw snap, couldn't read image " << buf << endl;
                    }
                }
                fclose(f);
            }
        }
	}
    return 0;
}
/*
* img 需要检测的图片
* cascade    分类器
* nestedcascade  内嵌分类器
* scale        图片缩小尺寸比例
* tryflip 
*/
void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip )
{
    double t = 0;
    vector<Rect> faces, faces2;//face和face2是一个空的vector，它潜在的元素是Rect型的，执行默认初始化
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;
    cvtColor( img, gray, COLOR_BGR2GRAY );//将彩色图像转换成灰度图像
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT ); //按照scale缩小图像
	//直方图均衡化缩小后的图像，用于提高图像质量
	equalizeHist( smallImg, smallImg );
	t = (double)getTickCount();//计算从启动到此刻的毫秒数
	//多尺度检测人脸，返回值为矩形框
	cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
    if( tryflip )
    {
		//沿x，y轴翻转
		flip(smallImg, smallImg, 1);
		//输入图像不同尺度下检测目标，返回值为矩形框
		cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                 //|CASCADE_DO_ROUGH_SEARCH
                                 |CASCADE_SCALE_IMAGE,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
        {
			//push_back添加元素到矩阵的底部。Rect画出矩形
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)getTickCount() - t;
    printf( "detection time = %g ms\n", t*1000/getTickFrequency());
	//标出人脸的位置
	for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        double aspect_ratio = (double)r.width/r.height;
		//标出人脸的位置
		if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width*0.5)*scale);//计算圆心的x轴坐标
            center.y = cvRound((r.y + r.height*0.5)*scale);//计算圆心的y轴坐标
            radius = cvRound((r.width + r.height)*0.25*scale);//计算圆心半径
            circle( img, center, radius, color, 3, 8, 0 );//在原图上画圈
        }
        else
            rectangle( img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
                       cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                       color, 3, 8, 0);
        if( nestedCascade.empty() )
            continue;
		//将脸部区域复制给smallImgROI
		smallImgROI = smallImg( r );
		//识别眼睛
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            //|CASCADE_DO_CANNY_PRUNING
            |CASCADE_SCALE_IMAGE,
            Size(30, 30) );
		//标出眼睛的位置
		for ( size_t j = 0; j < nestedObjects.size(); j++ )
        {
            Rect nr = nestedObjects[j];
            center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
            center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
            radius = cvRound((nr.width + nr.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
    }
    imshow( "result", img );
}

