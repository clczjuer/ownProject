#ifndef fsgrgr
#define fsgrgr

#include "cv.h"
#include <vector>
using namespace std;
struct myblock
{
	int left;
	int right;
	int len;
};
#define MAX_CHAR_NUM 62



vector<myblock> prjct2x(IplImage *thres_x)
{
	int nHeight = thres_x->height;
	int nWidth = thres_x->width;

	double nSum[1000] = {0};

	for (int i = 0; i < thres_x->height; i++)
	{
		for (int j = 0; j < thres_x->width; j++)
		{
			nSum[j] += CV_IMAGE_ELEM(thres_x, uchar, i, j);
		}
	}
	
	for (int i = 0; i < thres_x->width; i++)
	{
		nSum[i] = nSum[i] / thres_x->height;
	}

	bool bFlag = false;
	int index = 0;
	int nCount = 0;

	vector <myblock> vecBlock;
	myblock temp;

	for (int i = 1; i <nWidth - 1; i++)
	{
		int x1 = nSum[i];
		int x2 = nSum[i+1];
		if (x1 > 20 || x2 > 20)
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
		temp.right = nWidth - 1;
		temp.len = nCount;
		vecBlock.push_back(temp);
	}
	return vecBlock;
}

myblock prjct2y(IplImage* thres_y)
{
	int nHeight = thres_y->height;
	int nWidth = thres_y->width;
	double nSum[1000] = { 0 };

	for (int i = 0; i < thres_y->height; i++)
	{
		for (int j = 0; j < thres_y->width; j++)
		{
			nSum[i] += CV_IMAGE_ELEM(thres_y, uchar, i, j);
		}
	}

	for (int i = 0; i < nHeight; i++)
	{
		nSum[i] = nSum[i] / nWidth;
	}

	bool bFlag = false;
	int index = 0;
	int nCount = 0;

	vector <myblock> vecBlock;
	myblock temp;

	for (int i = 1; i < nHeight - 1; i++)
	{
		int x1 = nSum[i];
		int x2 = nSum[i+1];
		if (x1 > 20 || x2 > 20)
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
		temp.right = nHeight - 1;
		temp.len = temp.right - temp.left + 1;
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
#endif