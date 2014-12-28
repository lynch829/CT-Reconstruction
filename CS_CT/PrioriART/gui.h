/* GUI for debug and image display 
 * Huayu Zhang, Dec 2014 */

#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <string>

using namespace std;
using namespace cv;

void double2Byte(const Mat& doubleMat, Mat& byteMat);	// for show
void show2Dimg(const Mat& image, string window, int flags = WINDOW_AUTOSIZE);

void mouseCallback(int mouseevent, int x, int y, int flags, void* param/* it is an image */);
void keyboradCallback(char keypress, const Mat& I);