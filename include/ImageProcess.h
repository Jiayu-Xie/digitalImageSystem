#pragma
#include <iostream>
#include <algorithm>
#include "opencv2/opencv.hpp"

class ImageProcess
{
public:
	// center����λ��width������������Ĭ��ȫ������ת��
	cv::Mat To8BitLinear(cv::Mat image16Bit, int center = -1, int width = -1);
	// ��ת��typeΪ0��ʾ��ֱ��ת��1��ʾˮƽ��ת
	cv::Mat Flip(cv::Mat src, int type);
	// ��ת��angle��ʾ��ʱ����ת�ĽǶ�,scale��ʾ���Ŵ�С
	cv::Mat Rotate(cv::Mat src, int angle, double scale);
	// ���ݱ������ţ�fx��ʾ������ű�����fy��ʾ�������ű���
	cv::Mat ScaleResize(cv::Mat src, double fx, double fy);
	// ���ݹ̶���С���ţ�width��ʾ���ŵĿ�,height��ʾ���ŵĸߣ�����ʵ����Ӧ�������ŵȹ���
	cv::Mat FixedResize(cv::Mat src, int width, int height);
	// ��Ƭ����
	cv::Mat Negative(cv::Mat src);
	// α��ɫ����,������һ��CV_8UC1�ĻҶ�ͼ�������һ��CV_8UC3�Ĳ�ɫͼ
	cv::Mat ColorMap(cv::Mat src);
};

