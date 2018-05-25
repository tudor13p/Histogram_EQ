// Implementing Histogram Equalization
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

struct Histogram{
	int count[256];
	float cumulative_probability[256];
};

///////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	Mat imgOriginal;
	Mat imgEqualization;

	int rows, cols;
	Histogram histo;

	// initialise histo
	for(int i=0; i<256; i++ ) {
		histo.count[i]=0;
		histo.cumulative_probability[i]=0.f;
	}


	imgOriginal = imread("image.jpg");

	//check for invalid image
	if (imgOriginal.empty()) {
		std::cout << "error: image not read from file\n\n";
		return(0);
	}

  // convert to grayscale
	cvtColor(imgOriginal, imgEqualization, CV_BGR2GRAY);

	rows=imgEqualization.rows;
	cols=imgEqualization.cols;

	// count how many pixels for each value
	for(int i=0; i<rows; i++ )
		for(int j=0; j<cols; j++)
			histo.count[imgEqualization.at<uchar>(i,j)]++;

	//create cumulative probabilities
	int nr_pixels = rows*cols;
	int cumulate = 0;
	for(int i=0; i<256; i++ ){
		cumulate += histo.count[i];
		float f = (float)cumulate / nr_pixels;
		histo.cumulative_probability[i] = (float)floor(f * 255); // *256 because that is the desired scale for the new image's histogram
	}

	//update pixels in matrix
	for(int i=0; i<rows; i++ )
		for(int j=0; j<cols; j++)
			imgEqualization.at<uchar>(i,j) = (uchar)(histo.cumulative_probability[imgEqualization.at<uchar>(i,j)]);

///////////////////////////////////////////////////////////////////////////////////////////////////////

	//Verification
	imwrite("imgEqualised_program.png", imgEqualization);

	Mat imgEqualization2;
	cvtColor(imgOriginal, imgEqualization2, CV_BGR2GRAY);
	equalizeHist(imgEqualization2, imgEqualization2);

	imwrite("imgEqualised_function.png", imgEqualization2);

	//difference between the two output + error
	Mat difference(rows, cols, CV_8UC1, Scalar(255));

	for(int i=0; i<rows; i++ )
		for(int j=0; j<cols; j++)
			if(abs(imgEqualization.at<uchar>(i,j) - imgEqualization2.at<uchar>(i,j)) > 1)
				difference.at<uchar>(i,j) = abs(imgEqualization.at<uchar>(i,j) - imgEqualization2.at<uchar>(i,j));

	imwrite("difference.png", difference);

	waitKey(0);

	return(0);
}
