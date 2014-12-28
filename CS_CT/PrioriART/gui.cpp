#include "gui.h"
#include <cstdio>

static Vec2i pos;

void double2Byte(const Mat& doubleMat, Mat& byteMat){
	double lod, hid;	// boundary	 double 
	const int lob = 0, hib = 255;	// boundary byte 
	minMaxLoc(doubleMat, &lod, &hid);	// find the range of double matrix [lod,hid]
	byteMat = Mat(doubleMat.rows, doubleMat.cols, CV_8UC1);
	doubleMat.convertTo(byteMat, CV_8UC1, (hib - lob) / (hid - lod),
		(hid * lob - lod * hib) / (hid - lod));
}

void show2Dimg(const Mat& image, string window, int flags){
	char keypress;
	Mat I = image;
	Mat byteMat;
	double2Byte(image, byteMat);
	namedWindow(window, flags);
	imshow(window, byteMat);
	setMouseCallback(window, mouseCallback, &I);
	while ((keypress = cv::waitKey(0)) != 27/*Esc*/){
		keyboradCallback(keypress, I);
	}
}


void mouseCallback(int mouseevent, int x, int y, int flags, void* param/* it is an image */){
	Mat* I = (Mat*)param;
	if (mouseevent == CV_EVENT_LBUTTONDOWN){
		pos = Vec2i(x, y);
		printf("I(%d,%d) = %f\n", y, x, I->at<double>(y, x));
	}
}

void keyboradCallback(char keypress, const Mat& I){
	switch (tolower(keypress))
	{
	case 'a': 
		pos(0)--;
		printf("I(%d,%d) = %f\n", pos(1), pos(0), I.at<double>(pos));
		break;
	case 'd':
		pos(0)++;
		printf("I(%d,%d) = %f\n", pos(1), pos(0), I.at<double>(pos));
		break;
	case 'w':
		pos(1)++;
		printf("I(%d,%d) = %f\n", pos(1), pos(0), I.at<double>(pos));
		break;
	case 's':
		pos(1)--;
		printf("I(%d,%d) = %f\n", pos(1), pos(0), I.at<double>(pos));
		break;
	default:
		break;
	}
	
}