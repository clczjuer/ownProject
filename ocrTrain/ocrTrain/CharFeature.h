#include "cv.h"
#include "highgui.h"
#include <highgui/highgui.hpp>  
#include <ml/ml.hpp>  
using namespace cv;

void calcGradientFeat(const Mat& imgSrc, vector<float>& feat)
{
	float sumMatValue(const Mat& image);
	// ����ͼ�������ػҶ�ֵ�ܺ� 

	Mat image;
	cvtColor(imgSrc, image, CV_BGR2GRAY);
	resize(image, image, Size(8, 16));


	// ����x�����y�����ϵ��˲� 
	float mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
	Mat x_mask = y_mask.t();
	// ת�� 
	Mat sobelX, sobelY;

	filter2D(image, sobelX, CV_32F, x_mask);
	filter2D(image, sobelY, CV_32F, y_mask);

	sobelX = abs(sobelX);
	sobelY = abs(sobelY);

	float totleValueX = sumMatValue(sobelX);
	float totleValueY = sumMatValue(sobelY);


	// ��ͼ�񻮷�Ϊ4*2��8�����ӣ�����ÿ��������Ҷ�ֵ�ܺ͵İٷֱ� 
	for (int i = 0; i < image.rows; i = i + 4)
	{
		for (int j = 0; j < image.cols; j = j + 4)
		{
			Mat subImageX = sobelX(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageX) / totleValueX);
			Mat subImageY = sobelY(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageY) / totleValueY);
		}
	}
}
float sumMatValue(const Mat& image)
{
	float sumValue = 0;
	int r = image.rows;
	int c = image.cols;
	if (image.isContinuous())
	{
		c = r*c;
		r = 1;
	}
	for (int i = 0; i < r; i++)
	{
		const uchar* linePtr = image.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			sumValue += linePtr[j];
		}
	}
	return sumValue;
}