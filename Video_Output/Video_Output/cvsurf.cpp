/*
 * An OpenCV Implementation of SURF
 * Further Information Refer to "SURF: Speed-Up Robust Feature"
 * Author: Zhang Xiaobo
 * zhangxiaobo1258@aliyun.com
 *
 * There are still serveral lacks for this experimental implementation:
 * 1.The interpolation of sub-pixel mentioned in article was not implemented yet;
 * 2.A comparision with original libSurf.so shows that the hessian detector is not a 100% match to their implementation;
 * 3.Due to above reasons, I recommanded the original one for study and reuse;
 *
 * However, the speed of this implementation is something comparable to original one.
 */

#include "cvsurf.h"
#include <opencv2\calib3d.hpp>
#include <opencv2\core\types_c.h>
#include <iostream>

using namespace std;

#define ScanOctave (3)
#define FilterScale (4)
#define SamplingStep (1)

CV_INLINE CvSURFPoints CvSURFPoint( int x, int y, int laplacian, int size, int octave, int scale )
{
    CvSURFPoints p;
    p.x = x;
    p.y = y;
    p.laplacian = laplacian;
    p.size = size;
    p.octave = octave;
    p.scale = scale;
    return p;
}

CV_INLINE double
icvCalHaarPattern( int* origin,
		   int* t,
		   int widthStep )
{
	double d = 0;
	int *p0 = 0, *p1 = 0, *p2 = 0, *p3 = 0;
	int n = t[0];
	for ( int k = 0; k < n; k++ )
	{
		p0 = origin+t[1]+t[2]*widthStep;
		p1 = origin+t[1]+t[4]*widthStep;
		p2 = origin+t[3]+t[2]*widthStep;
		p3 = origin+t[3]+t[4]*widthStep;
		d += (double)((*p3-*p2-*p1+*p0)*t[6])/(double)(t[5]);
		t+=6;
	}
	return d;
}

CV_INLINE void
icvResizeHaarPattern( int* t_s,
		      int* t_d,
		      int OldSize,
		      int NewSize )
{
	int n = t_d[0] = t_s[0];
	for ( int k = 0; k < n; k++ )
	{
		t_d[1] = t_s[1]*NewSize/OldSize;
		t_d[2] = t_s[2]*NewSize/OldSize;
		t_d[3] = t_s[3]*NewSize/OldSize;
		t_d[4] = t_s[4]*NewSize/OldSize;
		t_d[5] = (t_d[3]-t_d[1]+1)*(t_d[4]-t_d[2]+1);
		t_d[6] = t_s[6];
		t_d+=6;
		t_s+=6;
	}
}

template<typename Number>
CV_INLINE int
icvSign( Number x )
{
	return (( x < 0 ) ? -1 : 1);
}

CvSeq*
icvFastHessianDetector( const CvMat* sum,
			CvMemStorage* storage,
			double quality )    //该函数用于Hessian检测
{
	double t = (double)cvGetTickCount();
	CvSeq* points = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvSURFPoints), storage );
	CvMat* hessians[ScanOctave*(FilterScale+2)];
	CvMat* traces[ScanOctave*(FilterScale+2)];
	int size, size_cache[ScanOctave*(FilterScale+2)];
	int scale, scale_cache[ScanOctave*(FilterScale+2)];
	double *hessian_ptr, *hessian_ptr_cache[ScanOctave*(FilterScale+2)];
	double *trace_ptr, *trace_ptr_cache[ScanOctave*(FilterScale+2)];
	int dx_s[] = { 3, 0, 2, 3, 7, 0, 1, 3, 2, 6, 7, 0, -2, 6, 2, 9, 7, 0, 1 };  //
	int dy_s[] = { 3, 2, 0, 7, 3, 0, 1, 2, 3, 7, 6, 0, -2, 2, 6, 7, 9, 0, 1 };
	int dxy_s[] = { 4, 1, 1, 4, 4, 0, 1, 5, 1, 8, 4, 0, -1, 1, 5, 4, 8, 0, -1, 5, 5, 8, 8, 0, 1 };
	int dx_t[] = { 3, 0, 2, 3, 7, 0, 1, 3, 2, 6, 7, 0, -2, 6, 2, 9, 7, 0, 1 };
	int dy_t[] = { 3, 2, 0, 7, 3, 0, 1, 2, 3, 7, 6, 0, -2, 2, 6, 7, 9, 0, 1 };
	int dxy_t[] = { 4, 1, 1, 4, 4, 0, 1, 5, 1, 8, 4, 0, -1, 1, 5, 4, 8, 0, -1, 5, 5, 8, 8, 0, 1 };
	double dx = 0, dy = 0, dxy = 0;
	int k = 0;
	int hessian_rows, hessian_rows_cache[ScanOctave*(FilterScale+2)];
	int hessian_cols, hessian_cols_cache[ScanOctave*(FilterScale+2)];
	/* hessian detector */
	for ( int o = 0; o < ScanOctave; o++ )   //每一组
	{
		t = (double)cvGetTickCount();   //计算运行的时间
		for ( int s = -1; s < FilterScale+1; s++ )  //每一尺度
		{
			if ( s < 0 )
				size_cache[k] = size = 7<<o; // gaussian scale 1.0;
			else
				size_cache[k] = size = (s*6+9)<<o; // gaussian scale size*1.2/9.;
			scale_cache[k] = scale = MAX( size, 9 )*SamplingStep;
			hessian_rows_cache[k] = hessian_rows = (sum->rows)*9/scale;
			hessian_cols_cache[k] = hessian_cols = (sum->cols)*9/scale;
			hessians[k] = cvCreateMat( hessian_rows, hessian_cols, CV_64FC1 );
			traces[k] = cvCreateMat( hessian_rows, hessian_cols, CV_64FC1 );
			int* sum_ptr = (int*)sum->data.ptr;
			icvResizeHaarPattern( dx_s, dx_t, 9, size );
			icvResizeHaarPattern( dy_s, dy_t, 9, size );
			icvResizeHaarPattern( dxy_s, dxy_t, 9, size );
			hessian_ptr_cache[k] = hessian_ptr = (double*)hessians[k]->data.ptr;
			trace_ptr_cache[k] = trace_ptr = (double*)traces[k]->data.ptr;
			hessian_ptr+=4/SamplingStep+(4/SamplingStep)*hessian_cols;
			trace_ptr+=4/SamplingStep+(4/SamplingStep)*hessian_cols;
			int oy = 0, y = 0;
			for ( int j = 0; j < hessian_rows-9/SamplingStep; j++ )
			{
				int * sum_line_ptr = sum_ptr;
				double* trace_line_ptr = trace_ptr;
				double* hessian_line_ptr = hessian_ptr;
				int ox = 0, x = 0;
				/**********************************************************
				求Hessian矩阵的行列式的值。
				**********************************************************/
				for ( int i = 0; i < hessian_cols-9/SamplingStep; i++ )
				{
					dx = icvCalHaarPattern( sum_line_ptr, dx_t, sum->cols );
					dy = icvCalHaarPattern( sum_line_ptr, dy_t, sum->cols );
					dxy = icvCalHaarPattern( sum_line_ptr, dxy_t, sum->cols );
					*hessian_line_ptr = (dx*dy-dxy*dxy*0.81);
					*trace_line_ptr = dx+dy;
					x = (i+1)*scale/9;
					sum_line_ptr+=x-ox;
					ox = x;
					trace_line_ptr++;
					hessian_line_ptr++;
				}
				y = (j+1)*scale/9;
				sum_ptr+=(y-oy)*sum->cols;
				oy = y;
				trace_ptr+=hessian_cols;
				hessian_ptr+=hessian_cols;
			}
			k++;
		}
		//t = (double)cvGetTickCount()-t;//计算时间
		//printf( "octave time = %g us\n", t/((double)cvGetTickFrequency()*1000.) );
	}
	double min_accept = quality*300;
	//t = (double)cvGetTickCount()-t;
	//printf( "hessian filter time = %g us\n", t/((double)cvGetTickFrequency()*1000.) );
    //    t = (double)cvGetTickCount();
	k = 0;
	for ( int o = 0; o < ScanOctave; o++ )
	{
		k++;
		for ( int s = 0; s < FilterScale; s++ )
		{
			size = size_cache[k];
			scale = scale_cache[k];
			hessian_rows = hessian_rows_cache[k];
			hessian_cols = hessian_cols_cache[k];
			int margin = (5/SamplingStep)*scale_cache[k+1]/scale;
			hessian_ptr = hessian_ptr_cache[k]+margin+margin*hessian_cols;
			trace_ptr = trace_ptr_cache[k];
			for ( int j = margin; j < hessian_rows-margin; j++ )
			{
				double* hessian_line_ptr = hessian_ptr;
				for ( int i = margin; i < hessian_cols-margin; i++ )
				{
					if ( *hessian_line_ptr > min_accept )
					{
						bool suppressed = false;
						/* 非极大值抑制*/
						for ( int z = k-1; z < k+2; z++ )
						{
							double* temp_hessian_ptr = hessian_ptr_cache[z]+i*scale/scale_cache[z]-1+(j*scale/scale_cache[z]-1)*hessian_cols_cache[z];
							for ( int y = 0; y < 3; y++ )
							{
								double* temp_hessian_line_ptr = temp_hessian_ptr;
								for ( int x = 0; x < 3; x++ )
								{
									if ((( z != k )||( y != 1 )||( x != 1 ))&&( *temp_hessian_line_ptr > *hessian_line_ptr ))
									{
										suppressed = true;
										break;
									}
									temp_hessian_line_ptr++;
								}
								if ( suppressed )
									break;
								temp_hessian_ptr+=hessian_cols_cache[z];
							}
							if ( suppressed )
								break;
						}
						if ( !suppressed )
						{
							CvSURFPoints point = CvSURFPoint( i*scale/9, j*scale/9, icvSign(trace_ptr[i+j*hessian_cols]), size_cache[k], o, s );
							cvSeqPush( points, &point );
						}
					}
					hessian_line_ptr++;
				}
				hessian_ptr+=hessian_cols;
			}
			k++;
		}
		k++;
	}
	k = 0;
	for ( int o = 0; o < ScanOctave; o++ )
		for ( int s = -1; s < FilterScale+1; s++ )
		{
			cvReleaseMat( &hessians[k] );
			cvReleaseMat( &traces[k] );
			k++;
		}
 //       t = (double)cvGetTickCount()-t;
 //       printf( "hessian selector time = %g us\n", t/((double)cvGetTickFrequency()*1000.) );
	return points;
}

void
icvSURFGaussian( CvMat* mat, double s )   //高斯函数各个点对应值的矩阵
{
	int w = mat->cols;
	int h = mat->rows;
	double x, y;
	double c2 = 1./(s*s*2);
	double over_exp = 1./(3.14159*2*s*s);
	for ( int i = 0; i < w; i++ )
		for ( int j = 0; j < h; j++ )
		{
			x = i-w/2.;
			y = j-h/2.;
			cvmSet( mat, j, i, exp(-(x*x+y*y)*c2)*over_exp );
		}
}

CvMat* wrap = 0;
IplImage* regions_cache[ScanOctave*FilterScale];
IplImage* region_cache;
CvMat* dx_cache;
CvMat* dy_cache;
CvMat* gauss_kernel_cache;
double CosCache[3600];
double SinCache[3600];

void
cvSURFInitialize()    //计算SURF开始时的一些参数的计算。
{
	wrap = cvCreateMat( 2, 3, CV_32FC1 );
	int k = 0;   ///准备SURF计算过程中所需的存储空间
	for ( int o = 0; o < ScanOctave; o++ )
		for ( int s = 0; s < FilterScale; s++ )
		{
			double scal = ((s*6+9)<<o)*1.2/9.;
			regions_cache[k] = cvCreateImage( cvSize(cvRound(21*scal), cvRound(21*scal)), 8, 1 );
			k++;
		}
	region_cache = cvCreateImage( cvSize(21, 21), 8, 1 );
	dx_cache = cvCreateMat( 20, 20, CV_64FC1 );
	dy_cache = cvCreateMat( 20, 20, CV_64FC1 );
	gauss_kernel_cache = cvCreateMat( 20, 20, CV_64FC1 );
	/***********************************************************
	在生成特征点描述算子过程中，对dx、dy进行高斯加权计算
	***********************************************************/
	icvSURFGaussian( gauss_kernel_cache, 3.3 );   //高斯核函数计算
	for ( int i = 0; i < 3600; i++ )               
	{
		CosCache[i] = cos(i*0.001745329);
		SinCache[i] = sin(i*0.001745329);
	}
}

void
cvSURFFinalize()  //SURF计算结束
{
	cvReleaseMat( &wrap );
	int k = 0;
	for ( int o = 0; o < ScanOctave; o++ )
		for ( int s = 0; s < FilterScale; s++ )
		{
			cvReleaseImage( &regions_cache[k] );
			k++;
		}
	cvReleaseImage( &region_cache );
	cvReleaseMat( &dx_cache );
	cvReleaseMat( &dy_cache );
	cvReleaseMat( &gauss_kernel_cache );
}

/************************************************************************
将图像的描述子存储于storage内存块中
CvArr* _img为输入的图像，
************************************************************************/
CvSeq*
cvSURFDescriptor( const CvArr* _img,
		  CvMemStorage* storage,
		  double quality,
		  int flags )   //SURF描述子的计算
{
	IplImage* img = (IplImage*)_img;
	CvMat* sum = 0;
	sum = cvCreateMat( img->height+1, img->width+1, CV_32SC1 );
	cvIntegral( img, sum );
	CvMemStorage* point_storage = cvCreateChildMemStorage( storage );
	CvSeq* points = icvFastHessianDetector( sum, point_storage, quality );
        double t = (double)cvGetTickCount();
	CvSeq* descriptors = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvSURFDescriptor), storage );
	int dx_s[] = {2, 0, 0, 2, 4, 0, -1, 2, 0, 4, 4, 0, 1};
	int dy_s[] = {2, 0, 0, 4, 2, 0, -1, 0, 2, 4, 4, 0, 1};
	int dx_t[] = {2, 0, 0, 2, 4, 0, -1, 2, 0, 4, 4, 0, 1};
	int dy_t[] = {2, 0, 0, 4, 2, 0, -1, 0, 2, 4, 4, 0, 1};
	double x[81], *iter_x;
	double y[81], *iter_y;
	double angle[81], *iter_angle;
	double sumx, sumy;
	double temp_mod;
	int angle_n;
	for ( int k = 0; k < points->total; k++ )
	{
		/***********************************************************
		使用2s的高斯加权函数对haar小波的响应值进行高斯加权，并计算出该特
		征点的一个矢量对应的长度和角度
		***********************************************************/
		CvSURFPoints* point = (CvSURFPoints*)cvGetSeqElem( points, k );
		CvSURFDescriptor descriptor;
		descriptor.x = cvRound(point->x);
		descriptor.y = cvRound(point->y);
		descriptor.laplacian = point->laplacian;
		int size = point->size;
		int layer = point->octave*FilterScale+point->scale;
		descriptor.s = size*1.2/9.;
		descriptor.mod = 0;
		/* repeatable orientation */
		iter_x = x;
		iter_y = y;
		iter_angle = angle;
		angle_n = 0;
		icvResizeHaarPattern( dx_s, dx_t, 9, size );
		icvResizeHaarPattern( dy_s, dy_t, 9, size );
		int* sum_ptr = (int*)sum->data.ptr;
		double c2 = 1./(descriptor.s*descriptor.s*2.5*2.5*2);
		double over_exp = 1./(3.14159*2*descriptor.s*descriptor.s*2.5*2.5);
		for ( int j = -6; j <=2; j++ )
		{
			int y = descriptor.y+j*size/9;
			if (( y >= 0 )&&( y < sum->rows-size ))
			{
				double ry = j+2;
				for ( int i = -6; i <=2; i++ )
				{
					int x = descriptor.x+i*size/9;
					if (( x >= 0 )&&( x < sum->cols-size ))
					{
						double rx = j+2;
						double radius = rx*rx+ry*ry;   
						if ( radius <= 16 )
						{
							rx*=descriptor.s;    
							ry*=descriptor.s;
							/*********************************************************************************************
							使用尺度为2s的Haar小波对子窗口进行其响应值的计算
							*********************************************************************************************/
							*iter_x = icvCalHaarPattern( sum_ptr+x+y*sum->cols, dx_t, sum->cols )*exp(-radius*c2)*over_exp;
							*iter_y = icvCalHaarPattern( sum_ptr+x+y*sum->cols, dy_t, sum->cols )*exp(-radius*c2)*over_exp;
							*iter_angle = cvFastArctan( *iter_y, *iter_x );
							iter_x++;
							iter_y++;
							iter_angle++;
							angle_n++;
						}
					}
				}
			}
		}
		/******************************************************************************************************************
		特征点的主方向的确定
		******************************************************************************************************************/
		double bestx = 0;
		double besty = 0;
		for ( int i = 0; i < 360; i+=5 )  //扇形滑动窗口的步长为5°
		{
			sumx = 0;
			sumy = 0;
			iter_x = x;
			iter_y = y;
			iter_angle = angle;
			for ( int j = 0; j < angle_n; j++ )
			{
				if ( ( ( *iter_angle < i+60 )&&( *iter_angle > i ) )||
				( ( (*iter_angle+360) < i+60 )&&( (*iter_angle+360) > i ) ) )
				{
					sumx+=*iter_x;
					sumy+=*iter_y;
				}
				iter_x++;
				iter_y++;
				iter_angle++;
			}
			temp_mod = sumx*sumx+sumy*sumy;
			if ( temp_mod > descriptor.mod )
			{
				descriptor.mod = temp_mod;
				bestx = sumx;
				besty = sumy;
			}
		}
		descriptor.dir = cvFastArctan( besty, bestx );   //特征点主方向的角度
		/* get sub-region (CV_INTER_AREA approximately retain the information of total image for haar feature while reduce the time consuming */
		/**********************************************************************
		特征点描述算子的生成
		**********************************************************************/		
		double cos_dir = CosCache[MAX(cvRound(descriptor.dir*10)+3600, 0)%3600];
		double sin_dir = SinCache[MAX(cvRound(descriptor.dir*10)+3600, 0)%3600];
		cvmSet( wrap, 0, 0, cos_dir );
		cvmSet( wrap, 0, 1, -sin_dir );
		cvmSet( wrap, 0, 2, descriptor.x );
		cvmSet( wrap, 1, 0, sin_dir );
		cvmSet( wrap, 1, 1, cos_dir );
		cvmSet( wrap, 1, 2, descriptor.y );

		cvGetQuadrangleSubPix( img, regions_cache[layer], wrap );
		cvResize( regions_cache[layer], region_cache, CV_INTER_AREA );
		uchar* region_d;
		int region_step;
		//cvGetImageRawData( region_cache, &region_d, &region_step );
		cvGetRawData(region_cache, &region_d, &region_step);
		uchar* region_x = region_d+1;
		uchar* region_y = region_d+region_step;
		uchar* region_xy = region_d+1+region_step;
		region_step-=20;
		double* iter_dx = (double*)dx_cache->data.ptr;
		double* iter_dy = (double*)dy_cache->data.ptr;
		for ( int i = 0; i < 20; i++ )
		{
			for ( int j = 0; j < 20; j++ )
			{
				*iter_dx = *region_y-*region_d-*region_x+*region_xy;
				*iter_dy = *region_x-*region_d-*region_y+*region_xy;
				iter_dx++;
				iter_dy++;
				region_d++;
				region_x++;
				region_y++;
				region_xy++;
			}
			region_d+=region_step;
			region_x+=region_step;
			region_y+=region_step;
			region_xy+=region_step;
		}
		cvMul( gauss_kernel_cache, dx_cache, dx_cache );
		cvMul( gauss_kernel_cache, dy_cache, dy_cache );
		
		double tx, ty;
		double* iter_vector = descriptor.vector;
		if ( flags&CV_SURF_EXTENDED )
		{
			/***********************************************
			128维描述算子的生成，具体的方法是在求sum（dx）和
			sum（|dx|）时区分dy>=0和dy<0两种情况，在求sum（dy）
			和sum（|dy|）时区分dx>=0和dx<0两种情况。
			***********************************************/
			for ( int i = 0; i < 4; i++ )
				for ( int j = 0; j < 4; j++ )
				{
					iter_vector[0] = 0;
					iter_vector[1] = 0;
					iter_vector[2] = 0;
					iter_vector[3] = 0;
					iter_vector[4] = 0;
					iter_vector[5] = 0;
					iter_vector[6] = 0;
					iter_vector[7] = 0;
					for ( int x = i*5; x < i*5+5; x++ )
					{
						for ( int y = j*5; y < j*5+5; y++ )
						{
							tx = cvGetReal2D( dx_cache, x, y );
							ty = cvGetReal2D( dy_cache, x, y );
							if ( ty >= 0 )
							{
								iter_vector[0] += tx;
								iter_vector[1] += fabs(tx);
							} 
							else
							{
								iter_vector[2] += tx;
								iter_vector[3] += fabs(tx);
							}
							if ( tx >= 0 )
							{
								iter_vector[4] += ty;
								iter_vector[5] += fabs(ty);
							}
							else 
							{
								iter_vector[6] += ty;
								iter_vector[7] += fabs(ty);
							}
						}
					}
					/* unit vector is essential for contrast invariant */
					double normalize = 0;
					for ( int k = 0; k < 8; k++ )
						normalize+=iter_vector[k]*iter_vector[k];
					normalize = sqrt(normalize);
					for ( int k = 0; k < 8; k++ )
						iter_vector[k] = iter_vector[k]/normalize;
					iter_vector+=8;
				}
		} 
		else 
		{
			/* 64-bin descriptor 64维描述算子的生成*/
			for ( int i = 0; i < 4; i++ )
				for ( int j = 0; j < 4; j++ )
				{
					iter_vector[0] = 0;
					iter_vector[1] = 0;
					iter_vector[2] = 0;
					iter_vector[3] = 0;
					for ( int x = i*5; x < i*5+5; x++ )
					{
						for ( int y = j*5; y < j*5+5; y++ )
						{
							tx = cvGetReal2D( dx_cache, x, y );
							ty = cvGetReal2D( dy_cache, x, y );
							iter_vector[0] += tx;
							iter_vector[1] += ty;
							iter_vector[2] += fabs(tx);
							iter_vector[3] += fabs(ty);
						}
					}
					/******************************************
					对比度不变性是通过将特征矢量进行归一化来实现的
					******************************************/
					double normalize = 0;
					for ( int k = 0; k < 4; k++ )
						normalize+=iter_vector[k]*iter_vector[k];
					normalize = sqrt(normalize);
					for (int  k = 0; k < 4; k++ )
						iter_vector[k] = iter_vector[k]/normalize;
					iter_vector+=4;
				}
		}
		cvSeqPush( descriptors, &descriptor );
	}
	cvReleaseMemStorage( &point_storage );
	cvReleaseMat( &sum );
    //    t = (double)cvGetTickCount()-t;
   //     printf( "descriptor time = %g us\n", t/((double)cvGetTickFrequency()*1000.) );
	return descriptors;
}

inline
double
icvCompareSURFDescriptor( CvSURFDescriptor* descriptor1,
			  CvSURFDescriptor* descriptor2,
			  double best,
			  int length = 64 )    //比较两个描述子函数，此处用64维的特征矢量
{
	double* iter_vector1 = descriptor1->vector;   //第一个描述子向量
	double* iter_vector2 = descriptor2->vector;   //第二个描述子向量
	double total_cost = 0; 
	for ( int i = 0; i < length; i++ )
	{
		total_cost+=(*iter_vector1-*iter_vector2)*(*iter_vector1-*iter_vector2);
		if ( total_cost > best )
			break;
		iter_vector1++;
		iter_vector2++;
	}
	return total_cost;
}

inline int
icvNaiveNearestNeighbor( CvSURFDescriptor* descriptor,
			 CvSeq* model_descriptors,
			 int length   )  //最近邻方法
{
	int neighbor = -1;
	double d;
	double dist1 = 0xffffff, dist2 = 0xffffff;
	for ( int i = 0; i < model_descriptors->total; i++ )
	{
		CvSURFDescriptor* model_descriptor = (CvSURFDescriptor*)cvGetSeqElem( model_descriptors, i );
		if ( descriptor->laplacian != model_descriptor->laplacian )
			continue;
		d = icvCompareSURFDescriptor( descriptor, model_descriptor, dist2, length );
		if ( d < dist1 )
		{
			dist2 = dist1;
			dist1 = d;
			neighbor = i;
		} 
		else 
		{
			if ( d < dist2 )
				dist2 = d;
		}
	}
	if ( dist1 < 0.6*dist2 )
		return neighbor;
	return -1;
}

/* a rough implementation for object location */
bool
cvSURFObjectLocate( CvSeq* ImageDescriptor,
		    CvSeq* ObjectDescriptor,
		    CvPoint* points,
		    int flags )   //目标定位，返回1表示是该目标，返回0表示不是该目标
{
	CvSURFDescriptor* correspond = (CvSURFDescriptor*)malloc( sizeof(CvSURFDescriptor)*ObjectDescriptor->total*2 );
	int length = ( flags&CV_SURF_EXTENDED ) ? 128 : 64;

	int chaos=0, correspond_total = 0;
	CvSURFDescriptor* correspond_ptr = correspond;
	for ( int i = 0; i < ObjectDescriptor->total; i++ )  //采用最近邻方法进行匹配
	{
		CvSURFDescriptor* descriptor = (CvSURFDescriptor*)cvGetSeqElem( ObjectDescriptor, i );
		int nearest_neighbor = icvNaiveNearestNeighbor( descriptor, ImageDescriptor, length );
		if ( nearest_neighbor >= 0 )
		{
			*correspond_ptr = *descriptor;
			correspond_ptr++;
			*correspond_ptr = *(CvSURFDescriptor*)cvGetSeqElem( ImageDescriptor, nearest_neighbor );
			correspond_ptr++;
			chaos+=nearest_neighbor;
			correspond_total++;
		}
	}

	int correspond_n = correspond_total;

	if ( correspond_n < 4 )
		return 0;

	CvMat* correspond_points1;
	CvMat* correspond_points2;
	CvMat* homography_matrix;

	correspond_points1 = cvCreateMat( 4, 1, CV_32FC2 );
	correspond_points2 = cvCreateMat( 4, 1, CV_32FC2 );
	homography_matrix = cvCreateMat( 3, 3, CV_32FC1 );

	CvRNG rng = cvRNG(chaos);
	double h11, h12, h13, h21, h22, h23, h31, h32, h33;//单应性矩阵中的元素
	double x, y, z, d;                                 
	double best_d = 0xffffff;
	int best_clan = 0;
	int clan = 0;
	CvPoint corner[4];
	for ( int k = 0; k < MIN( correspond_n*100, correspond_n*(correspond_n-1)*(correspond_n-2) ); k++ )
	{
		int selector[4];
		for( int i = 0; i < 4; i++ )
		{
			while ( selector[i] = cvRandInt(&rng)%correspond_n )
			{
				bool recalc = false;
				for ( int j = 0; j < i; j++ )
					if ( selector[i] == selector[j] )
						recalc = true;
				if ( !recalc )
					break;
			}
			CvSURFDescriptor* correspond_descriptor1 = correspond+(selector[i]<<1);
			CvSURFDescriptor* correspond_descriptor2 = correspond_descriptor1+1;
			cvSet2D( correspond_points1, i, 0, cvScalar( (float)correspond_descriptor1->x, (float)correspond_descriptor1->y ) );
			cvSet2D( correspond_points2, i, 0, cvScalar( (float)correspond_descriptor2->x, (float)correspond_descriptor2->y ) );
		}
		cvFindHomography( correspond_points1, correspond_points2, homography_matrix );  
		//求单应性矩阵homography_matrix，即查找两个图像的旋转和平移的矩阵
		//该函数的实现D:\opencv\modules\calib3d\src\fundam.cpp
		//参考维基百科对单应性的介绍，特征描述算子correspond_descriptor2=correspond_descriptor1*homography_matrix。
		h11 = cvmGet( homography_matrix, 0, 0 );
		h12 = cvmGet( homography_matrix, 0, 1 );
		h13 = cvmGet( homography_matrix, 0, 2 );
		h21 = cvmGet( homography_matrix, 1, 0 );
		h22 = cvmGet( homography_matrix, 1, 1 );
		h23 = cvmGet( homography_matrix, 1, 2 );
		h31 = cvmGet( homography_matrix, 2, 0 );
		h32 = cvmGet( homography_matrix, 2, 1 );
		h33 = cvmGet( homography_matrix, 2, 2 );
		d = 0;
		clan = 0;
		for ( int i = 0; i < correspond_n; i++ )
		{
			CvSURFDescriptor* correspond_descriptor1 = correspond+(i<<1);
			CvSURFDescriptor* correspond_descriptor2 = correspond_descriptor1+1;
			//两个描述算子对应的仿射计算
			z = 1/(h31*correspond_descriptor1->x+h32*correspond_descriptor1->y+h33);
			x = (h11*correspond_descriptor1->x+h12*correspond_descriptor1->y+h13)*z-correspond_descriptor2->x;
			y = (h21*correspond_descriptor1->x+h22*correspond_descriptor1->y+h23)*z-correspond_descriptor2->y;
			if ( x*x+y*y < 256 )  //
			{
				d+=x*x+y*y;      //计算出平移距离的平方
				clan++;
			}
		}
		/*****************************************************************************************************
		在有效的阈值范围内，
		*****************************************************************************************************/
		if ( ( clan >= best_clan )&&( d < best_d ) )
		{
			for ( int i = 0; i < 4; i++ )
			{
				z = 1./(points[i].x*h31+points[i].y*h32+h33);
				corner[i] = cvPoint( (int)((points[i].x*h11+points[i].y*h12+h13)*z),
						     (int)((points[i].x*h21+points[i].y*h22+h23)*z) );
			}
			bool err = false;
			char dir = 0;
			for ( int i = 0; i < 4; i++ )
			{
				int vx1 = corner[(i+1)%4].x-corner[i%4].x;
				int vy1 = corner[(i+1)%4].y-corner[i%4].y;
				int vx2 = corner[(i+2)%4].x-corner[(i+1)%4].x;
				int vy2 = corner[(i+2)%4].y-corner[(i+1)%4].y;
				char tmp_dir = ( vx2*vy1-vx1*vy2 > 0 ) ? 1 : -1;
				if ( dir != 0 )
				{
					if ( dir != tmp_dir )
					{
						err = true;
						break;
					}
				}
				else
					dir = tmp_dir;
			}
			if ( !err )
			{
				best_d = d;
				best_clan = clan;
			}
		}
	}

	cvReleaseMat( &homography_matrix );
	cvReleaseMat( &correspond_points1 );
	cvReleaseMat( &correspond_points2 );
	free( correspond );

	if ( best_clan < 4 )
		return 0;

	for (int  i = 0; i < 4; i++ )
		points[i] = corner[i];

	return 1;
}

CvSeq*
cvSURFFindPair( CvSeq* ImageDescriptor,
		CvSeq* ObjectDescriptor,
		CvMemStorage* storage,
		int flags )//查找目标对
{
	CvSeq* correspond = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvSURFDescriptor), storage );
	int length = ( flags&CV_SURF_EXTENDED ) ? 128 : 64;
	int chaos=0;
	for ( int i = 0; i < ObjectDescriptor->total; i++ )
	{
		CvSURFDescriptor* descriptor = (CvSURFDescriptor*)cvGetSeqElem( ObjectDescriptor, i );
		int nearest_neighbor = icvNaiveNearestNeighbor( descriptor, ImageDescriptor, length );
		if ( nearest_neighbor >= 0 )
		{
			cvSeqPush( correspond, descriptor );
			cvSeqPush( correspond, (CvSURFDescriptor*)cvGetSeqElem( ImageDescriptor, nearest_neighbor ) );
			chaos += nearest_neighbor;
		}
	}

	return correspond;
}

IplImage* cvRotation(IplImage* Object_Image,CvPoint2D32f Center_val,double Angle_val)
{
	IplImage* Image_Result;
	int w_src = Object_Image->width;
	int h_src = Object_Image->height;
	double Angle_Rad = Angle_val  * CV_PI / 180.; // angle in radian
	double a = sin(Angle_Rad), b = cos(Angle_Rad); // sine and cosine of angle

	// Make w_dst and h_dst to fit the output image
	int w_dst = int(h_src * fabs(a) + w_src * fabs(b));
	int h_dst = int(w_src * fabs(a) + h_src * fabs(b));
	// map matrix for WarpAffine, stored in statck array
	double map[6];
	CvMat map_matrix = cvMat(2, 3, CV_64FC1, map);

	cv2DRotationMatrix(Center_val, Angle_val, 1.0, &map_matrix);

	map[2] +=(w_dst - w_src) / 2;
	map[5] +=(h_dst - h_src) / 2;

	Image_Result = cvCreateImage(cvSize(w_dst,h_dst), 8, 3);
	cvWarpAffine(
		Object_Image, 
		Image_Result,
		&map_matrix,
		CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,
		cvScalarAll(0)
		);
	return Image_Result;
}

