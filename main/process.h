#include <stdlib.h>
#include <stdio.h>
#include <windows.h>  

#include "cv.h"   
#include "highgui.h"   
#include "cxcore.h" 

void cvThin(Mat &src, Mat &dst, int iterations)
{
	int nWidth = src.cols;
	int nHeight = src.rows;
	//此时的src是一个二值化的图片  
	src.copyTo(dst);
	int n = 0, i = 0, j = 0;
	for (n = 0; n < iterations; n++)//开始进行迭代  
	{
		Mat t_image;
		dst.copyTo(t_image);
		for (i = 0; i < src.rows; i++)
		{
			for (j = 0; j < src.cols; j++)
			{
				if ( t_image.at<uchar>(i, j) == 1)
				{
					int ap = 0;
					int p2 = (i == 0) ? 0 : t_image.at<uchar>(i-1, j);
					int p3 = (i == 0 || j == nWidth - 1) ? 0 : t_image.at<uchar>(i-1, j+1);
					if (p2 == 0 && p3 == 1)
					{
						ap++;
					}

					int p4 = (j == nWidth - 1) ? 0 : t_image.at<uchar>(i, j+1);
					if (p3 == 0 && p4 == 1)
					{
						ap++;
					}

					int p5 = (i == nHeight - 1 || j == nWidth - 1) ? 0 : t_image.at<uchar>(i+1, j+1);
					if (p4 == 0 && p5 == 1)
					{
						ap++;
					}

					int p6 = (i == nHeight - 1) ? 0 : t_image.at<uchar>(i + 1, j);
					if (p5 == 0 && p6 == 1)
					{
						ap++;
					}

					int p7 = (i == nHeight - 1 || j == 0) ? 0 : t_image.at<uchar>(i + 1, j - 1);
					if (p6 == 0 && p7 == 1)
					{
						ap++;
					}

					int p8 = (j == 0) ? 0 : t_image.at<uchar>(i, j-1);
					if (p7 == 0 && p8 == 1)
					{
						ap++;
					}

					int p9 = (i == 0 || j == 0) ? 0 : t_image.at<uchar>(i-1, j-1);
					if (p8 == 0 && p9 == 1)
					{
						ap++;
					}
					if (p9 == 0 && p2 == 1)
					{
						ap++;
					}

					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) > 1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) < 7)
					{
						if (ap == 1)
						{
							if (!(p2 && p4 && p6))
							{
								if (!(p4 && p6 && p8))
								{
									t_image.at<uchar>(i, j) = 0;
								//	CV_IMAGE_ELEM(dst, byte, i, j) = 0;//设置目标图像中像素值为0的点  
								}
							}
						}
					}

				}
			}
		}

	
		dst.copyTo(t_image);
		for (i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				if (t_image.at<uchar>(i, j) == 1)
				{
					int ap = 0;
					int p2 = (i == 0) ? 0 : t_image.at<uchar>(i-1, j);
					int p3 = (i == 0 || j == src.cols - 1) ? 0 : t_image.at<uchar>(i-1, j+1);
					if (p2 == 0 && p3 == 1)
					{
						ap++;
					}
					int p4 = (j == src.cols - 1) ? 0 : t_image.at<uchar>(i, j+1);
					if (p3 == 0 && p4 == 1)
					{
						ap++;
					}
					int p5 = (i == nHeight - 1 || j == nWidth - 1) ? 0 : t_image.at<uchar>(i + 1, j + 1);
					if (p4 == 0 && p5 == 1)
					{
						ap++;
					}
					int p6 = (i == nHeight - 1) ? 0 : t_image.at<uchar>(i + 1, j);
					if (p5 == 0 && p6 == 1)
					{
						ap++;
					}
					int p7 = (i == nHeight - 1 || j == 0) ? 0 : t_image.at<uchar>(i + 1, j - 1);
					if (p6 == 0 && p7 == 1)
					{
						ap++;
					}
					int p8 = (j == 0) ? 0 : t_image.at<uchar>(i, j-1);
					if (p7 == 0 && p8 == 1)
					{
						ap++;
					}
					int p9 = (i == 0 || j == 0) ? 0 : t_image.at<uchar>(i-1, j-1);
					if (p8 == 0 && p9 == 1)
					{
						ap++;
					}
					if (p9 == 0 && p2 == 1)
					{
						ap++;
					}
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) > 1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) < 7)
					{
						if (ap == 1)
						{
							if (p2*p4*p8 == 0)
							{
								if (p2*p6*p8 == 0)
								{
									t_image.at<uchar>(i, j) = 0;
								}
							}
						}
					}
				}

			}

		}
		//cvReleaseImage(&t_image);
	}

}