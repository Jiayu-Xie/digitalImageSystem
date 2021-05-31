#include "ImageProcess.h"
using namespace std;


ImageProcess::ImageProcess(cv::Mat image)
{
	originImage16Bit = image;
}

cv::Mat ImageProcess::To8BitLinear(int center, int width)
{
	double minVal, maxVal;
	cv::minMaxIdx(originImage16Bit, &minVal, &maxVal);
	cout << minVal << maxVal << endl;
	if (center > -1 && width > -1)
	{
		minVal = max(0, center - width / 2);
		maxVal = min(65535, center + width / 2);
	}
	cv::Mat image8Bit = cv::Mat::zeros(originImage16Bit.size(), CV_8UC1);
	for (int i = 0; i < originImage16Bit.rows; i++)
	{
		for (int j = 0; j < originImage16Bit.cols; j++)
		{
			ushort pixelVal = originImage16Bit.at<ushort>(i, j);
			uchar val8Bit;
			if (pixelVal < minVal)
			{
				val8Bit = 0;
			}
			else if (pixelVal > maxVal)
			{
				val8Bit = 255;
			}
			else
			{
				if (minVal == maxVal)
				{
					val8Bit = 128;
				}
				else
				{
					val8Bit = (pixelVal - minVal) / (maxVal - minVal) * 255;
				}
			}
			image8Bit.at<uchar>(i, j) = val8Bit;
		}
	}
	return image8Bit;
}

cv::Mat ImageProcess::Flip(cv::Mat src, int type)
{
	cv::Mat res;
	cv::flip(src, res, type);
	return res;
}

cv::Mat ImageProcess::Rotate(cv::Mat src, int angle, double scale)
{
	cv::Mat dst;
	cv::Point2f center = cv::Point(src.cols / 2, src.rows / 2);
	int h = src.cols, w = src.rows;
	int bound_w = (h * fabs(sin(angle * CV_PI / 180)) + w * fabs(cos(angle * CV_PI / 180))) * scale;
	int bound_h = (h * fabs(cos(angle * CV_PI / 180)) + w * fabs(sin(angle * CV_PI / 180))) * scale;
	cv::Mat m = getRotationMatrix2D(center, angle, scale);
	m.at<double>(0, 2) += (bound_w - src.cols) / 2;
	m.at<double>(1, 2) += (bound_h - src.rows) / 2;
	warpAffine(src, dst, m, cv::Size2i(bound_h, bound_w));
	return dst;
}

cv::Mat ImageProcess::ScaleResize(cv::Mat src, double fx, double fy)
{
	cv::Mat res;
	resize(src, res, cv::Size(0, 0), fx, fy);
	return res;
}

cv::Mat ImageProcess::FixedResize(cv::Mat src, int width, int height)
{
	cv::Mat res;
	cv::resize(src, res, cv::Size(width, height), 0, 0);
	return res;
}

cv::Mat ImageProcess::Negative(cv::Mat src)
{
	cv::Mat res = cv::Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			res.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);
		}
	}
	return res;
}

cv::Mat ImageProcess::ColorMap(cv::Mat src)
{
	cv::Mat res;
	cv::applyColorMap(src, res, cv::COLORMAP_JET);
	return res;
}


