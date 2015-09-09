#include "cv.h"
#include "highgui.h"
#include "CaluArea.h"
#include "ml\ml.hpp"
#include "process.h"

using namespace cv;
using namespace std;

struct block
{
	int left;
	int right;
	int len;
};
#define MAX_CHAR_NUM 62



vector<block> prjct2x(Mat &sobleX)
{
	sobleX.convertTo(sobleX, CV_32FC1);
	Mat prj_x(1, sobleX.cols, CV_32FC1);
	prj_x.setTo(0);
	for (int i = 0; i < sobleX.rows; i++)
	{
		prj_x += sobleX.row(i)/255;
	}
	prj_x.convertTo(prj_x, CV_8UC1);

	prj_x = prj_x * 255 / sobleX.rows;

	bool bFlag = false;
	int index = 0;
	int nCount = 0;

	vector <block> vecBlock;
	block temp;

	for (int i = 1; i < prj_x.cols - 1; i++)
	{
		int x1 = prj_x.at<uchar>(0, i);
		int x2 = prj_x.at<uchar>(0, i + 1);
		if (x1 > 60 || x2 > 60)
		{
			if (bFlag == true)
			{
				;
			}
			else
			{
				temp.left = i;

			}
			nCount++;
			bFlag = true;
		}
		else
		{
			if (bFlag == true)
			{
				temp.right = i;
				temp.len = nCount;
				vecBlock.push_back(temp);
			}
			bFlag = false;
			nCount = 0;
		}
	}
	if (bFlag == true)
	{
		temp.right = prj_x.cols - 1;
		temp.len = nCount;
		vecBlock.push_back(temp);
	}
	return vecBlock;
}

block prjct2y(Mat &sobelY)
{
	sobelY.convertTo(sobelY, CV_32FC1);
	Mat prj_y(sobelY.rows, 1, CV_32FC1);
	prj_y.setTo(0);
	for (int i = 0; i < sobelY.cols; i++)
	{
		Mat t = sobelY.col(i)/255;
		prj_y += sobelY.col(i)/255;
	}
	prj_y.convertTo(prj_y, CV_8UC1);

	bool bFlag = false;
	int index = 0;
	int nCount = 0;

	vector <block> vecBlock;
	block temp;

	for (int i = 1; i < prj_y.rows - 1; i++)
	{
		int x1 = prj_y.at<uchar>(i, 0);
		int x2 = prj_y.at<uchar>(i+1, 0);
		if (x1 > 50 || x2 > 50)
		{
			if (bFlag == true)
			{
				;
			}
			else
			{
				temp.left = i;

			}
			nCount++;
			bFlag = true;
		}
		else
		{
			if (bFlag == true)
			{
				temp.right = i;
				temp.len = nCount;
				vecBlock.push_back(temp);
			}
			bFlag = false;
			nCount = 0;
		}
	}
	if (bFlag == true)
	{
		temp.right = prj_y.rows - 1;
		temp.len = temp.right-temp.left+1;
		vecBlock.push_back(temp);
	}

	int bIndex = 0;
	for (int i = 0; i < vecBlock.size(); i++)
	{
		if (vecBlock[i].len > vecBlock[bIndex].len)
		{
			bIndex = i;
		}
	}


	return vecBlock[bIndex];
}

double sim(Mat &src, Mat &com)
{
	return 1;
}

void getROI(Mat &src, Mat &dst, vector<block> vecBlock, block yBlock, int index)
{
	Rect rect2 = Rect(vecBlock[index].left, yBlock.left, vecBlock[index].len, yBlock.len);
	dst = src(rect2);
}


void thin(Mat &src, Mat &dst, vector<block> vecBlock, block yBlock, int index)
{
	getROI(src, dst, vecBlock, yBlock, index);

	vector<vector<Point> > contour;
	vector<Vec4i> hierarchy;
	findContours(dst, contour, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

	drawContours(dst, contour, 2, Scalar(255), -1, 8, hierarchy);
	drawContours(dst, contour, 4, Scalar(255), -1, 8, hierarchy);
	drawContours(dst, contour, 0, Scalar(255), -1, 8, hierarchy);
	drawContours(dst, contour, 1, Scalar(0), -1, 8, hierarchy);
	drawContours(dst, contour, 3, Scalar(0), -1, 8, hierarchy);

	threshold(dst, dst, 1, 1, THRESH_BINARY);
	//drawContours(dst, contour, 2, Scalar(255), -1, 8);
	Mat tt;
	Mat input;
	dst.copyTo(input);
	cvThin(input, tt, 1);
	



	int i = 0, j = 0;

	for (i = 0; i < dst.rows; i++)
	{
		for (j = 0; j < dst.cols; j++)
		{
			if (tt.at<uchar>(i, j) == 1)
			{
				tt.at<uchar>(i, j) = 255;
			}
			else
			{
				tt.at<uchar>(i, j) = 0;
			}
		}
	}
	int s = -1;
	//ThiningDIBSkeleton(dst.data, dst.cols, dst.rows);
}




int main()
{
	Mat imgBig = imread("f://test2//src.jpg", 0);

	Mat src = imgBig(Rect(640, 370, 240, 80));

	imwrite("f://src.tif", src);

	threshold(src, src, 1, 255.0, THRESH_OTSU);
	threshold(src, src, 128, 255.0, THRESH_BINARY_INV);
	//Mat src = imread("f://test2//2.jpg", 0);

	Mat sobel_x, sobel_y;
	threshold(src, sobel_x, 128, 255.0, THRESH_BINARY);
	threshold(src, sobel_y, 128, 255.0, THRESH_BINARY);
	//genSobelImage(src, &sobel_x, &sobel_y);

	/***************** cut **********************/
	//Sobel(src, sobel_x, src.depth(), 1, 0, 3);
	//Sobel(src, sobel_y, src.depth(), 0, 1, 1);

	block yBlock = prjct2y(sobel_y);
	sobel_x = sobel_x(Rect(0, yBlock.left, sobel_x.cols, yBlock.len));
	vector<block> vecBlock = prjct2x(sobel_x);
	
	Mat show;
	src.copyTo(show);
	line(show, Point(0, yBlock.left), Point(src.cols, yBlock.left), Scalar(255), 1);
	line(show, Point(0, yBlock.right), Point(src.cols, yBlock.right), Scalar(255), 1);
	for (int i = 0; i < vecBlock.size(); i++)
	{
		line(show, Point(vecBlock[i].left, 0), Point(vecBlock[i].left, src.rows), Scalar(255), 1);
		line(show, Point(vecBlock[i].right, 0), Point(vecBlock[i].right, src.rows), Scalar(255), 1);
	}
	//imwrite("f://test.png", src);


	for (int i = 0; i < vecBlock.size(); i++)
	{
		Mat dst;
		thin(src, dst, vecBlock, yBlock, i);
		int sd = 1;
	}
	{

	}
	

	//**************** reg ********************//
	for (int i = 0; i < vecBlock.size(); i++)
	{
		if (vecBlock[i].len > 10)
		{
			Rect rect(vecBlock[i].left, yBlock.left, vecBlock[i].len, yBlock.len);
			Mat tmp = src(rect);
			resize(tmp, tmp, Size(40, 50));
			char str[203];
			sprintf_s(str, "f://test2//%d-image.tif", i);
			imwrite(str, tmp);


			
			for (int i = 0; i < MAX_CHAR_NUM; i++)
			{

			}




		}
	}
	
// 	tesseract::TessBaseAPI api;
// 	if (api.Init(NULL, "eng", tesseract::OEM_DEFAULT))
// 	{
// 		fprintf(stderr, "Could not initialize tesseract.\n");
// 		return "err";
// 	}
// 
// 	api.SetImage((unsigned char*)img_erode_dilate->imageData, img_erode_dilate->width,
// 		img_erode_dilate->height, img_erode_dilate->nChannels, img_erode_dilate->widthStep);
// 	api.SetVariable("tessedit_char_whitelist", "0123456789ABCDEFGHIJKLMNOPGRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
// 	char* outText = api.GetUTF8Text();
// 	return outText;


//	KNearest knn = new KNearest(,);

	/*
	clc::CaluArea objArea;
	Mat src;
	Mat dst;
	src = imread("test.jpg", -1);
	threshold(src, dst, 50, 255.0, THRESH_BINARY);
	//dst.setTo(0);
	//line(dst, Point(100, 100), Point(101, 103), Scalar(255), 1);
	//line(dst, Point(200, 100), Point(201, 103), Scalar(255), 1);
	vector< vector<Point> > contours;
	findContours(dst, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	src.setTo(0);
	list<int> list_area = objArea.getArea(src.size(), contours, -1);
	for (int i = 0; i < contours.size(); i++)
	{
		
		//drawContours(src, contours,  i, Scalar(255), -1);
		double area = abs(contourArea(contours[i]));

		
		printf("%f_____%d\n", area, list_area.front());
		list_area.pop_front();
	}

	cvWaitKey(0);*/
	return 0;
}
