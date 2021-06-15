#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <windows.h>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include "CvxText.h"

using namespace std;

#define PRIVATE_CREATOR_NAME "helloCom"

#define PRIVATE_CREATOR_TAG 0x0029,0x0011

#define PRV_PrivateCreator DcmTag(PRIVATE_CREATOR_TAG)

class snapshot {
private:
	cv::Mat addWaterMark_img;

public:
	//构造，析构函数
	snapshot();
	virtual ~snapshot();

	/**
	* 辅助函数，将string类型转化为wchar_t类型
	* 传入的参数为char*类型也可完成转换
	* 使用完wchar_t*后delete[]释放内存
	*/
	wchar_t *multi_Byte_To_Wide_Char(const string& str);

	/**
	* 在图像中添加水印
	* \param image  待绘制的图像
	* \param text 文本内容
	* \param pos  文本位置，坐标原点左上角或者左下角取决于bottomLeftOrigin
	* \param color  线条的颜色，默认为白色
	* \param fontPath  字体文件路径，默认为空
	* \param fontScale  字体大小，值越大文字越大，默认50
	* \param fontInterval  文字间隔，默认0.1
	*
	*\return 返回成功输出的字符长度，失败返回-1.
	*/
	int putText(cv::Mat& image, const string& str, cv::Point pos, cv::Scalar color = cv::Scalar(255, 255, 255), double fontScale = 50.0, string fontPath = "", double fontInterval = 0.1);

	//存储图片
	void storeImage(string filename);

	/**
	* 均值叠加降噪
	* \param matArr  mat类型的vector，包含多个mat
	* \param num  mat数组的长度，mat的数量
	*
	*\return 均值降噪之后的mat
	*/
	cv::Mat MeanNoiseReduction(vector<cv::Mat> &matArr, int num);

	/**
	* 中值叠加降噪
	* \param matArr  mat类型的vector，包含多个mat
	* \param num  mat数组的长度，mat的数量
	*
	*\return 均值降噪之后的mat
	*/
	cv::Mat MedianNoiseReduction(vector<cv::Mat> &matArr, int num);

	//快排，用于辅助中值叠加降噪
	void quickSort(int left, int right, int *arr);


	//测试添加水印
	void TestAddWaterMark();

	//测试均值叠加降噪
	void TestMeanNoiseReduction();
	//测试中值叠加降噪
	void TestMedianNoiseReduction();

	void registerPrivateTags();
	void addPrivateElements(DcmItem &item);


	void printInfo();
};