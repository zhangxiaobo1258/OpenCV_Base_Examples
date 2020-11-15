/********************************************************************************************
改程序为未进行改进时的LM算法的C语言实现
********************************************************************************************/

#include "levenberg_marquardt.h"
#include "math.h"

double max(double x,double y)
{
	return (x>y?x:y);
}

CvRatation_Parameter cvLevenberg_Marquardt(CvPoint* Object_Point_Val,CvPoint* Reference_Point_Val,int Heigth_Val,int Width_Val,int count)
{
	CvRatation_Parameter Result_Temp;
	CvRatation_Parameter Dalta_Para;
	double Lambda=0.001;  
	double Loch=0;//平方差
	double Dalta_Loch=0;//
	double Rho=0;
	double Hessian_matrix[3][3]={{0,0,0},{0,0,0},{0,0,0}};
	double Hessian_Inverse_matrix[3][3]={{0,0,0},{0,0,0},{0,0,0}};
	double B[3]={0,0,0};
	double Cos_Thata,Sin_Thata;
	double Matrix_Determinant;
	Dalta_Para.Center_X=0;
	Dalta_Para.Center_Y=0;
	Dalta_Para.Thata=0;

	int i,j;

	Result_Temp.Thata=0;//弧度
	Result_Temp.Center_X=0;//旋转中心坐标X的初始值
	Result_Temp.Center_Y=0;//旋转中心坐标Y的初始值

	for(i=0;i<count;i++)
	{
		Loch+=(Object_Point_Val[i].x-Reference_Point_Val[i].x)*(Object_Point_Val[i].x-Reference_Point_Val[i].x);
	}
	while((fabs(Loch)>0))
	{
		Cos_Thata=cos(Result_Temp.Thata);
		Sin_Thata=sin(Result_Temp.Thata);
		for (i=0;i<3;i++)
		{
			B[i]=0;
			for(j=0;j<3;j++)
			{
				Hessian_matrix[i][j]=0;
				Hessian_Inverse_matrix[i][j]=0;
			}
		}
	/*****************************************************************************************************************
	求B矩阵：B=sum（JT*[Δx，Δy]T）
	*****************************************************************************************************************/
		/*A=
		[                             1,                           0,                                     - y*cos(Thata) - x*sin(Thata)]
		[                             0,                           1,                                       x*cos(Thata) - y*sin(Thata)]
		[ - y*cos(Thata) - x*sin(Thata), x*cos(Thata) - y*sin(Thata), (y*cos(Thata) + x*sin(Thata))^2 + (x*cos(Thata) - y*sin(Thata))^2]
		*/
		for(i=0;i<count;i++)
		{
			B[0]+=(Object_Point_Val[i].x-Reference_Point_Val[i].x);
			B[1]+=(Object_Point_Val[i].y-Reference_Point_Val[i].y);
			B[2]+=(Object_Point_Val[i].x-Reference_Point_Val[i].x)*(-1)*                                           \
				(Sin_Thata*Object_Point_Val[i].x+Cos_Thata*Object_Point_Val[i].y)+                                   \
				(Object_Point_Val[i].y-Reference_Point_Val[i].y)*                                                \
				(Cos_Thata*Object_Point_Val[i].x-Sin_Thata*Object_Point_Val[i].y);
			Hessian_matrix[0][2]+=(-1)*Object_Point_Val[i].y*Cos_Thata-Object_Point_Val[i].x*Sin_Thata;
			Hessian_matrix[1][2]+=Object_Point_Val[i].x*Cos_Thata-Object_Point_Val[i].y*Sin_Thata;
			Hessian_matrix[2][2]+=(Object_Point_Val[i].y*Cos_Thata+Object_Point_Val[i].x*Sin_Thata)*                  \
				(Object_Point_Val[i].y*Cos_Thata+Object_Point_Val[i].x*Sin_Thata)+                                    \
				(Object_Point_Val[i].x*Cos_Thata-Object_Point_Val[i].y*Sin_Thata)*                                    \
				(Object_Point_Val[i].x*Cos_Thata-Object_Point_Val[i].y*Sin_Thata);
		}
		Hessian_matrix[0][0]=count;
		Hessian_matrix[1][1]=count;
		Hessian_matrix[2][0]=Hessian_matrix[0][2];
		Hessian_matrix[2][1]=Hessian_matrix[1][2];

		/****************************************************************************************************************************
		(H+lambda*diag(H))
		****************************************************************************************************************************/
		Hessian_matrix[0][0]+=Lambda*Hessian_matrix[0][0];
		Hessian_matrix[1][1]+=Lambda*Hessian_matrix[1][1];
		Hessian_matrix[2][2]+=Lambda*Hessian_matrix[2][2];
		
		Matrix_Determinant=Hessian_matrix[0][0]*Hessian_matrix[1][1]*Hessian_matrix[2][2]+                \
			Hessian_matrix[0][1]*Hessian_matrix[1][2]*Hessian_matrix[2][0]+                               \
			Hessian_matrix[1][0]*Hessian_matrix[2][1]*Hessian_matrix[0][2]-                               \
			Hessian_matrix[2][0]*Hessian_matrix[1][1]*Hessian_matrix[0][2]-                               \
			Hessian_matrix[2][1]*Hessian_matrix[1][2]*Hessian_matrix[0][0]-                               \
			Hessian_matrix[1][0]*Hessian_matrix[0][1]*Hessian_matrix[2][2];
		/***********************************************************************************************************************************************************
		求Hessian矩阵的逆矩阵
		***********************************************************************************************************************************************************/
		Hessian_Inverse_matrix[0][0]=(Hessian_matrix[1][1]*Hessian_matrix[2][2]-Hessian_matrix[1][2]*Hessian_matrix[2][1])/Matrix_Determinant;
		Hessian_Inverse_matrix[1][0]=(-1)*(Hessian_matrix[1][0]*Hessian_matrix[2][2]-Hessian_matrix[1][2]*Hessian_matrix[2][0])/Matrix_Determinant;
		Hessian_Inverse_matrix[2][0]=(Hessian_matrix[1][0]*Hessian_matrix[2][1]-Hessian_matrix[1][1]*Hessian_matrix[2][0])/Matrix_Determinant;
		Hessian_Inverse_matrix[0][1]=(-1)*(Hessian_matrix[0][1]*Hessian_matrix[2][2]-Hessian_matrix[0][2]*Hessian_matrix[2][1])/Matrix_Determinant;
		Hessian_Inverse_matrix[1][1]=(Hessian_matrix[0][0]*Hessian_matrix[2][2]-Hessian_matrix[0][2]*Hessian_matrix[2][0])/Matrix_Determinant;
		Hessian_Inverse_matrix[2][1]=(-1)*(Hessian_matrix[0][0]*Hessian_matrix[2][1]-Hessian_matrix[2][0]*Hessian_matrix[0][1])/Matrix_Determinant;
		Hessian_Inverse_matrix[0][2]=(Hessian_matrix[0][1]*Hessian_matrix[1][2]-Hessian_matrix[0][2]*Hessian_matrix[1][1])/Matrix_Determinant;
		Hessian_Inverse_matrix[1][2]=(-1)*(Hessian_matrix[0][0]*Hessian_matrix[1][2]-Hessian_matrix[1][0]*Hessian_matrix[0][2])/Matrix_Determinant;
		Hessian_Inverse_matrix[2][2]=(Hessian_matrix[1][1]*Hessian_matrix[0][0]-Hessian_matrix[1][0]*Hessian_matrix[0][1])/Matrix_Determinant;
		/************************************************************************************************************************************************************
		求delta矩阵
		************************************************************************************************************************************************************/
		Dalta_Para.Center_X=(Hessian_Inverse_matrix[0][0]*B[0]+Hessian_Inverse_matrix[0][1]*B[1]+Hessian_Inverse_matrix[0][2]*B[2]);
		Dalta_Para.Center_Y=(Hessian_Inverse_matrix[1][0]*B[0]+Hessian_Inverse_matrix[1][1]*B[1]+Hessian_Inverse_matrix[1][2]*B[2]);
		Dalta_Para.Thata=(Hessian_Inverse_matrix[2][0]*B[0]+Hessian_Inverse_matrix[2][1]*B[1]+Hessian_Inverse_matrix[2][2]*B[2]);

		for(i=0;i<count;i++)
		{
			Dalta_Loch+=(Object_Point_Val[i].x-((cos(Result_Temp.Thata+Dalta_Para.Thata)*Object_Point_Val[i].x-                                         \
				sin(Result_Temp.Thata+Dalta_Para.Thata)*Object_Point_Val[i].y)+(Result_Temp.Center_X+Dalta_Para.Center_X)))*         \
				(Object_Point_Val[i].x-((cos(Result_Temp.Thata+Dalta_Para.Thata)*Object_Point_Val[i].x-                                                    \
				sin(Result_Temp.Thata+Dalta_Para.Thata)*Object_Point_Val[i].y)+(Result_Temp.Center_X+Dalta_Para.Center_X)));
		}
		
		Rho=(Loch*Loch-Dalta_Loch*Dalta_Loch)/(2*(1+Lambda)*Dalta_Para.Center_X*Dalta_Para.Center_X);

		if (Dalta_Loch<Loch)
		{
			Lambda=Lambda*(1-(2*Rho-1)*(2*Rho-1)*(2*Rho-1));
			Result_Temp.Center_X+=Dalta_Para.Center_X;
			Result_Temp.Center_Y+=Dalta_Para.Center_Y;
			Result_Temp.Thata+=Dalta_Para.Thata;
			Loch=Dalta_Loch;
			Dalta_Loch=0;
		}
		else
		{
			Lambda=Lambda*3;
			Loch=Dalta_Loch;
			Dalta_Loch=0;
			continue;
		}
	}

	//printf("%f,%f,%f\n",Result_Temp.Thata,Result_Temp.Center_X,Result_Temp.Center_Y);

	if(Result_Temp.Thata==0)
	{
		return Result_Temp;
	}
	else
	{
		//printf("%d,%d\n",Heigth_Val,Width_Val);
		Result_Temp.Center_X=Result_Temp.Center_X-(Heigth_Val*sin(Result_Temp.Thata))/2;
		Result_Temp.Center_Y=Result_Temp.Center_Y+(Width_Val*sin(Result_Temp.Thata))/2;
		Result_Temp.Thata=Result_Temp.Thata;
		return Result_Temp;
	}
}

