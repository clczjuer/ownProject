#include "cvthin.h" 
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>  
void cvThin( IplImage* src, IplImage* dst, int iterations)  
{  
	//此时的src是一个二值化的图片  
	CvSize size = cvGetSize(src);  
	cvCopy(src, dst);  
	int n = 0,i = 0,j = 0;  
	for(n=0; n<iterations; n++)//开始进行迭代  
	{  
		IplImage* t_image = cvCloneImage(dst);  
		for(i=0; i<size.height;  i++)  
		{  
			for(j=0; j<size.width; j++)  
			{  
				if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)  
				{  
					int ap=0;  
					int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);  
					int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);  
					if (p2==0 && p3==1)  
					{  
						ap++;  
					}  

					int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);  
					if(p3==0 && p4==1)  
					{  
						ap++;  
					}  

					int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);  
					if(p4==0 && p5==1)  
					{  
						ap++;  
					}  

					int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);  
					if(p5==0 && p6==1)  
					{  
						ap++;  
					}  

					int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);  
					if(p6==0 && p7==1)  
					{  
						ap++;  
					}  

					int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);  
					if(p7==0 && p8==1)  
					{  
						ap++;  
					}  

					int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);  
					if(p8==0 && p9==1)  
					{  
						ap++;  
					}  
					if(p9==0 && p2==1)  
					{  
						ap++;  
					}  

					if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)  
					{  
						if(ap==1)  
						{  
							if(!(p2 && p4 && p6))  
							{  
								if(!(p4 && p6 && p8))   
								{  
									CV_IMAGE_ELEM(dst,byte,i,j)=0;//设置目标图像中像素值为0的点  
								}  
							}  
						}  
					}  

				}  
			}  
		}  

		cvReleaseImage(&t_image);  

		t_image = cvCloneImage(dst);  
		for(i=0; i<size.height;  i++)  
		{  
			for(int j=0; j<size.width; j++)  
			{  
				if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)  
				{  
					int ap=0;  
					int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);  
					int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);  
					if (p2==0 && p3==1)  
					{  
						ap++;  
					}  
					int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);  
					if(p3==0 && p4==1)  
					{  
						ap++;  
					}  
					int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);  
					if(p4==0 && p5==1)  
					{  
						ap++;  
					}  
					int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);  
					if(p5==0 && p6==1)  
					{  
						ap++;  
					}  
					int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);  
					if(p6==0 && p7==1)  
					{  
						ap++;  
					}  
					int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);  
					if(p7==0 && p8==1)  
					{  
						ap++;  
					}  
					int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);  
					if(p8==0 && p9==1)  
					{  
						ap++;  
					}  
					if(p9==0 && p2==1)  
					{  
						ap++;  
					}  
					if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)  
					{  
						if(ap==1)  
						{  
							if(p2*p4*p8==0)  
							{  
								if(p2*p6*p8==0)  
								{  
									CV_IMAGE_ELEM(dst, byte,i,j)=0;  
								}  
							}  
						}  
					}                      
				}  

			}  

		}              
		cvReleaseImage(&t_image);  
	}  

}