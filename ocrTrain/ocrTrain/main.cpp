
#include <QtCore/QCoreApplication>
#include "CharFeature.h"



int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);


	CvANN_MLP bp;
	// Set up BPNetwork's parameters  
	CvANN_MLP_TrainParams params;
	params.train_method = CvANN_MLP_TrainParams::BACKPROP;
	params.bp_dw_scale = 0.1;
	params.bp_moment_scale = 0.1;
	//params.train_method=CvANN_MLP_TrainParams::RPROP;  
	//params.rp_dw0 = 0.1;   
	//params.rp_dw_plus = 1.2;   
	//params.rp_dw_minus = 0.5;  
	//params.rp_dw_min = FLT_EPSILON;   
	//params.rp_dw_max = 50.;  

	// Set up training data  
	float labels[3][5] = { { 0, 0, 0, 0, 0 }, { 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0 } };
	Mat labelsMat(3, 5, CV_32FC1, labels);

	float trainingData[3][5] = { { 1, 2, 3, 4, 5 }, { 111, 112, 113, 114, 115 }, { 21, 22, 23, 24, 25 } };
	Mat trainingDataMat(3, 5, CV_32FC1, trainingData);
	Mat layerSizes = (Mat_<int>(1, 5) << 5, 2, 2, 2, 5);
	bp.create(layerSizes, CvANN_MLP::SIGMOID_SYM);//CvANN_MLP::SIGMOID_SYM  
	//CvANN_MLP::GAUSSIAN  
	//CvANN_MLP::IDENTITY  
	bp.train(trainingDataMat, labelsMat, Mat(), Mat(), params);


	// Data for visual representation  
	int width = 512, height = 512;
	Mat image = Mat::zeros(height, width, CV_8UC3);
	Vec3b green(0, 255, 0), blue(255, 0, 0);
	// Show the decision regions given by the SVM  
	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j)
		{
			Mat sampleMat = (Mat_<float>(1, 5) << i, j, 0, 0, 0);
			Mat responseMat;
			bp.predict(sampleMat, responseMat);
			float* p = responseMat.ptr<float>(0);
			float response = 0.0f;
			for (int k = 0; k < 5; k++){
				//  cout<<p[k]<<" ";  
				response += p[k];
			}
			if (response >2)
				image.at<Vec3b>(j, i) = green;
			else
				image.at<Vec3b>(j, i) = blue;
		}

	// Show the training data  
	int thickness = -1;
	int lineType = 8;
	circle(image, Point(501, 10), 5, Scalar(0, 0, 0), thickness, lineType);
	circle(image, Point(255, 10), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(image, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(image, Point(10, 501), 5, Scalar(255, 255, 255), thickness, lineType);

	imwrite("result.png", image);        // save the image   

	imshow("BP Simple Example", image); // show it to the user  
	waitKey(0);

	return 1;
	
	float test = sumMatValue(Mat());


/*	int ar[] = { 1, 2, 3 };
	Mat layerSizes(1, 3, CV_32S, ar);
	CvANN_MLP mlp(layerSizes, CvANN_MLP::SIGMOID_SYM);
	
	CvANN_MLP_TrainParams param;
	param.term_crit = cvTermCriteria( CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 5000, 0.01);
	param.train_method = CvANN_MLP_TrainParams::BACKPROP;
	param.bp_dw_scale = 0.1;
	param.bp_moment_scale = 0.1;

	int nSamples = 100;
	int nDims = 2;
	Mat input(nSamples, nDims, CV_32FC1);

	Mat output;
	int res = mlp.train(input, output, Mat(), Mat(), param);

	*/
	a.exec();

	return 1;
}
