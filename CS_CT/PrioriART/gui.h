/* GUI for debug and image display 
 * Huayu Zhang, Dec 2014 */

#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <string>

using namespace std;
using namespace cv;

void double2Byte(const Mat& doubleMat, Mat& byteMat);	// for show
void double2Byte(vector<Mat>& doubleMat, vector<Mat>& byteMat);

void show2Dimg(const Mat& image, string window, int flags = WINDOW_NORMAL);
void show3Dimg(vector<Mat>& I, string winname, int flags = WINDOW_NORMAL);

void mouseCallback(int mouseevent, int x, int y, int flags, void* param/* it is an image */);
void keyboradCallback(char keypress, const Mat& I);