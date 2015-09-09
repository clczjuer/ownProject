#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "baseapi.h"
#include "strngs.h"
#include "cvthin.h"
#include "process.h"

using namespace std;
using namespace cv;
#pragma  comment(lib,"libtesseract302.lib")



char* usepic(IplImage *img_erode_diate,tesseract::TessBaseAPI api);
void main()
{
	IplImage *imagesource=cvLoadImage("test.png",1);
	IplImage *img_gray,*img_equaliz,*img_thread,*img_thin,*img_erode_dilate;



	//cvNamedWindow("showsource",1);
	//cvShowImage("showsource",imagesource);

	img_gray=cvCreateImage(cvGetSize(imagesource),IPL_DEPTH_8U,1);
	img_equaliz=cvCreateImage(cvGetSize(imagesource),IPL_DEPTH_8U,1);
	img_thread=cvCreateImage(cvGetSize(imagesource),IPL_DEPTH_8U,1);
	img_thin=cvCreateImage(cvGetSize(imagesource),IPL_DEPTH_8U,1);
	img_erode_dilate=cvCreateImage(cvGetSize(imagesource),IPL_DEPTH_8U,1);

	cvSmooth(imagesource,imagesource,CV_GAUSSIAN,5,0);
	cvCvtColor(imagesource,img_gray,CV_BGR2GRAY);
	//cvNamedWindow("showgray",1);
	//cvShowImage("showgray",img_gray);
	IplImage* thresh_x = cvCreateImage(cvGetSize(img_gray), 8, 1);
	IplImage* thresh_y = cvCreateImage(cvGetSize(img_gray), 8, 1);
	cvThreshold(img_gray, thresh_x, 128, 255.0, CV_THRESH_BINARY );
	cvThreshold(img_gray, thresh_y, 128, 255.0, CV_THRESH_BINARY);

	myblock yBlock = prjct2y(thresh_y);
	cvSetImageROI(thresh_x, cvRect(0, yBlock.left, thresh_x->width, yBlock.len));
	vector<myblock> vecBlock = prjct2x(thresh_x);

	IplImage *pImgShow = cvCreateImage(cvGetSize(img_gray), 8, 1);
	cvCopy(img_gray, pImgShow);
	cvLine(pImgShow, Point(0, yBlock.left), Point(pImgShow->width, yBlock.left), Scalar(255), 1);
	cvLine(pImgShow, Point(0, yBlock.right), Point(pImgShow->width, yBlock.right), Scalar(255), 1);
	for (int i = 0; i < vecBlock.size(); i++)
	{
		cvLine(pImgShow, Point(vecBlock[i].left, 0), Point(vecBlock[i].left, pImgShow->width), Scalar(255), 1);
		cvLine(pImgShow, Point(vecBlock[i].right, 0), Point(vecBlock[i].right, pImgShow->width), Scalar(255), 1);
	}


	for (int i = 0; i < vecBlock.size(); i++)
	{
		int nWidth = vecBlock[i].len;
		int nHeight = yBlock.len;
		IplImage* pImgChar = cvCreateImage(cvSize(nWidth, nHeight), 8, 1);
		cvSetImageROI(img_gray, cvRect(vecBlock[i].left, yBlock.left, nWidth, nHeight));
		cvCopy(img_gray, pImgChar);
		cvResetImageROI(img_gray);

		IplImage*pCharThin = cvCreateImage(cvSize(nWidth, nHeight), 8, 1);
		cvThreshold(pImgChar, pImgChar, 55, 1, CV_THRESH_BINARY);
		cvZero(pImgChar);
		cvThin(pCharThin, pCharThin, 8);

		int a = 0, b = 0;
		CvSize size = cvGetSize(pCharThin);
		for (a = 0; a < size.height; a++)
		{
			for (b = 0; b< size.width; b++)
			{
				if (CV_IMAGE_ELEM(pCharThin, uchar, a, b) == 1)
				{
					CV_IMAGE_ELEM(pCharThin, uchar, a, b) = 0;
				}
				else
				{
					CV_IMAGE_ELEM(pCharThin, uchar, a, b) = 255;
				}
			}
		}
		int sd = 1;
	}
	//cvCopy(img_gray, img_equaliz);
	cvEqualizeHist(img_gray,img_equaliz);
	//cvNamedWindow("showequaliz",1);
	//cvShowImage("showequaliz",img_equaliz);

	cvThreshold(img_equaliz,img_thread,55,1,CV_THRESH_BINARY);
	//cvNamedWindow("showthreshold",1);
	//cvShowImage("showthreshold",img_thread);

	cvZero(img_thin);
	cvThin(img_thread,img_thin,8);

	int i = 0,j = 0;  
	CvSize size = cvGetSize(img_thin);  
	for(i=0; i<size.height;  i++)  
	{  
		for(j=0; j<size.width; j++)  
		{  
			if(CV_IMAGE_ELEM(img_thin,uchar,i,j)==1)  
			{  
				CV_IMAGE_ELEM(img_thin,uchar,i,j) = 0;  
			}  
			else  
			{  
				CV_IMAGE_ELEM(img_thin,uchar,i,j) = 255;  
			}  
		}  
	} 
	//cvNamedWindow("showthin",1);
	//cvShowImage("showthin",img_thin);

	cvErode(img_thin,img_erode_dilate,NULL,1);
	//cvNamedWindow("showerode",1);
	//cvShowImage("showerode",img_erode_dilate);

	IplConvKernel *conv=cvCreateStructuringElementEx(3, 3, 0, 0, CV_SHAPE_RECT);
	cvDilate(img_erode_dilate,img_erode_dilate,conv,1);
	cvNamedWindow("showdilate",1);
	cvShowImage("showdilate",img_erode_dilate);
	cvReleaseStructuringElement(&conv);

	tesseract::TessBaseAPI api ;
	char* outText;
	outText=usepic(img_thin,api);
	cout<<outText<<endl;

	cvReleaseImage(&imagesource);
	cvReleaseImage(&img_gray);
	cvReleaseImage(&img_equaliz);
	cvReleaseImage(&img_thread);
	cvReleaseImage(&img_thin);
	cvReleaseImage(&img_erode_dilate);
	cvWaitKey();
}

char* usepic(IplImage *img_erode_dilate,tesseract::TessBaseAPI api)
{
	if (api.Init(NULL, "eng", tesseract::OEM_DEFAULT) )
	{
		fprintf(stderr, "Could not initialize tesseract.\n");
		return "err";
	}

	api.SetImage((unsigned char*)img_erode_dilate->imageData, img_erode_dilate->width,
		img_erode_dilate->height, img_erode_dilate->nChannels, img_erode_dilate->widthStep);
	api.SetVariable( "tessedit_char_whitelist", "0123456789" ); //ABCDEFGHIJKLMNOPGRSTUVWXYZabcdefghijklmnopqrstuvwxyz
	char* outText = api.GetUTF8Text();
	return outText;
}