#include "gui.h"
#include <cstdio>
#include <Windows.h>
#include <tchar.h>

static Vec2i pos;

void double2Byte(const Mat& doubleMat, Mat& byteMat){
	double lod, hid;	// boundary	 double 
	const int lob = 0, hib = 255;	// boundary byte 
	minMaxLoc(doubleMat, &lod, &hid);	// find the range of double matrix [lod,hid]
	byteMat = Mat(doubleMat.rows, doubleMat.cols, CV_8UC1);
	doubleMat.convertTo(byteMat, CV_8UC1, (hib - lob) / (hid - lod),
		(hid * lob - lod * hib) / (hid - lod));
}

void double2Byte(vector<Mat>& doubleMat, vector<Mat>& byteMat) {
	double lod, hid;	// boundary
	const int lob = 0, hib = 255;	// boundary 
	vector<double> lods(doubleMat.size()), hids(doubleMat.size());
	for (int k = 0; k < doubleMat.size(); k++){
		minMaxLoc(doubleMat[k], &lods[k], &hids[k]);
		byteMat.push_back(Mat(doubleMat[k].rows, doubleMat[k].cols, CV_8UC1));
	}
	lod = *min_element(lods.begin(), lods.end());
	hid = *max_element(hids.begin(), hids.end());
	for (int k = 0; k < doubleMat.size(); k++){
		doubleMat[k].convertTo(byteMat[k], CV_8UC1, (hib - lob) / (hid - lod), (hid * lob - lod * hib) / (hid - lod));
	}
}

void show2Dimg(const Mat& image, string window, int flags){
	assert(image.rows < 600 && image.cols < 600);
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
	destroyWindow(window);
}

void show3Dimg(vector<Mat>& I, string winname, int flags){
	vector<Mat> Is;
	Mat Iss; // 8bit image for show
	int keypress; // Up or Down
	int s = I.size() / 2;		// slice
	double2Byte(I, Is);
	namedWindow(winname, flags);
	Iss = Is[s].clone();
	imshow(winname, Iss);
	while ((keypress = cv::waitKey(0)) != 27/*Esc*/){
		keyboradCallback(keypress, Iss);
		if (keypress == 2490368){			// UpKey
			if (I.size() - 1 == s){
				MessageBox(0, _T("Top Reached", ), _T("Top Reached"), MB_OK);
				continue;
			}
			else{
				s++;
				Iss = Is[s].clone();
			}
			imshow(winname, Iss);
			
		}
		else if (keypress == 2621440){			// DownKey
			if (s == 0){
				MessageBox(0, _T("Bottom Reached", ), _T("Bottom Reached"), MB_OK);
				continue;
			}
			else{
				s--;
				Iss = Is[s].clone();
			}
			imshow(winname, Iss);
		}
	}
	destroyWindow(winname);
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