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
	//���죬��������
	snapshot();
	virtual ~snapshot();

	/**
	* ������������string����ת��Ϊwchar_t����
	* ����Ĳ���Ϊchar*����Ҳ�����ת��
	* ʹ����wchar_t*��delete[]�ͷ��ڴ�
	*/
	wchar_t *multi_Byte_To_Wide_Char(const string& str);

	/**
	* ��ͼ�������ˮӡ
	* \param image  �����Ƶ�ͼ��
	* \param text �ı�����
	* \param pos  �ı�λ�ã�����ԭ�����Ͻǻ������½�ȡ����bottomLeftOrigin
	* \param color  ��������ɫ��Ĭ��Ϊ��ɫ
	* \param fontPath  �����ļ�·����Ĭ��Ϊ��
	* \param fontScale  �����С��ֵԽ������Խ��Ĭ��50
	* \param fontInterval  ���ּ����Ĭ��0.1
	*
	*\return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1.
	*/
	int putText(cv::Mat& image, const string& str, cv::Point pos, cv::Scalar color = cv::Scalar(255, 255, 255), double fontScale = 50.0, string fontPath = "", double fontInterval = 0.1);

	//�洢ͼƬ
	void storeImage(string filename);

	/**
	* ��ֵ���ӽ���
	* \param matArr  mat���͵�vector���������mat
	* \param num  mat����ĳ��ȣ�mat������
	*
	*\return ��ֵ����֮���mat
	*/
	cv::Mat MeanNoiseReduction(vector<cv::Mat> &matArr, int num);

	/**
	* ��ֵ���ӽ���
	* \param matArr  mat���͵�vector���������mat
	* \param num  mat����ĳ��ȣ�mat������
	*
	*\return ��ֵ����֮���mat
	*/
	cv::Mat MedianNoiseReduction(vector<cv::Mat> &matArr, int num);

	//���ţ����ڸ�����ֵ���ӽ���
	void quickSort(int left, int right, int *arr);


	//�������ˮӡ
	void TestAddWaterMark();

	//���Ծ�ֵ���ӽ���
	void TestMeanNoiseReduction();
	//������ֵ���ӽ���
	void TestMedianNoiseReduction();

	void registerPrivateTags();
	void addPrivateElements(DcmItem &item);


	void printInfo();
};