#pragma
#include <iostream>
#include <algorithm>
#include "opencv2/opencv.hpp"

class ImageProcess
{
public:
	// center代表窗位，width代表窗宽，不传递默认全部像素转化
	cv::Mat To8BitLinear(cv::Mat image16Bit, int center = -1, int width = -1);
	// 翻转，type为0表示垂直翻转，1表示水平翻转
	cv::Mat Flip(cv::Mat src, int type);
	// 旋转，angle表示逆时针旋转的角度,scale表示缩放大小
	cv::Mat Rotate(cv::Mat src, int angle, double scale);
	// 根据比例缩放，fx表示宽的缩放比例，fy表示长的缩放比例
	cv::Mat ScaleResize(cv::Mat src, double fx, double fy);
	// 根据固定大小缩放，width表示缩放的宽,height表示缩放的高，用来实现适应窗口缩放等功能
	cv::Mat FixedResize(cv::Mat src, int width, int height);
	// 负片处理
	cv::Mat Negative(cv::Mat src);
	// 伪彩色处理,输入是一个CV_8UC1的灰度图，输出是一个CV_8UC3的彩色图
	cv::Mat ColorMap(cv::Mat src);
};

