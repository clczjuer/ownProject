#include "cv.h"
#include "highgui.h"
#include "CaluArea.h"

using namespace cv;

struct block
{
	int left;
	int right;
	int len;
};


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
int main()
{

	Mat imgBig = imread("f://test2//src.jpg", 0);

	Mat src = imgBig(Rect(600, 350, 300, 100));

	threshold(src, src, 1, 255.0, THRESH_OTSU);
	//threshold(src, src, 128, 255.0, THRESH_BINARY_INV);
	//Mat src = imread("f://test2//2.jpg", 0);

	Mat sobel_x, sobel_y;
	threshold(src, sobel_x, 128, 255.0, THRESH_BINARY_INV);
	threshold(src, sobel_y, 128, 255.0, THRESH_BINARY_INV);
	//genSobelImage(src, &sobel_x, &sobel_y);

	/***************** cut **********************/
	//Sobel(src, sobel_x, src.depth(), 1, 0, 3);
	//Sobel(src, sobel_y, src.depth(), 0, 1, 1);

	block yBlock = prjct2y(sobel_y);
	sobel_x = sobel_x(Rect(0, yBlock.left, sobel_x.cols, yBlock.len));
	vector<block> vecBlock = prjct2x(sobel_x);
	
	line(src, Point(0, yBlock.left), Point(src.cols, yBlock.left), Scalar(0), 1);
	line(src, Point(0, yBlock.right), Point(src.cols, yBlock.right), Scalar(0), 1);
	for (int i = 0; i < vecBlock.size(); i++)
	{
		line(src, Point(vecBlock[i].left, 0), Point(vecBlock[i].left, src.rows), Scalar(0), 1);
		line(src, Point(vecBlock[i].right, 0), Point(vecBlock[i].right, src.rows), Scalar(0), 1);
	}
	
	//**************** reg ********************//
	//vecBlock[6].right -= 7;
	//vecBlock[6].len -= 7;
	for (int i = 0; i < vecBlock.size(); i++)
	{
		if (vecBlock[i].len > 10)
		{
			Rect rect(vecBlock[i].left, yBlock.left, vecBlock[i].len, yBlock.len);
			Mat tmp = src(rect);
			char str[203];
			sprintf_s(str, "f://test2//%d-image.bmp", i);
			imwrite(str, tmp);
		}
	}
	


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
