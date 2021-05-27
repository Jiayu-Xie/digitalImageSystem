// digitalImage-test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "CareRayAPIDemo.h"
#include "opencv2/highgui/highgui.hpp"

int main()
{
	CCareRayAPIDemo* pCareRayAPIDemo = new CCareRayAPIDemo();
	int nRet = OK;
	nRet = pCareRayAPIDemo->Initialize();
	vector<int> modeIds = pCareRayAPIDemo->GetModeIdList();
	for (int i = 0; i < modeIds.size(); i++) 
	{
		cout << modeIds[i] << endl;
	}
	vector<CR_ModeInfo> modeInfos = pCareRayAPIDemo->GetModeInfoList();
	for (int i = 0; i < modeIds.size(); i++)
	{
		cout << modeInfos[i].nPixelDepth << endl;
	}
	int modeId;
	cout << "输入modeId" << endl;
	cin >> modeId;
	
	nRet = pCareRayAPIDemo->SetMode(modeId);
	if (GetReturnCode(nRet) > CR_WARN) 
	{
		char info[256];
		CR_GetLastIntlMsg(info, 256);
		cout << info << endl;
	} 
	else
	{
		float maxFrameRate = pCareRayAPIDemo->GetMaxFrameRate(modeId);
		float frameRate;
		cout << "最大帧率" << maxFrameRate << endl;
		cout << "输入framRate" << endl;
		cin >> frameRate;
		if (frameRate <= maxFrameRate)
		{
			nRet = pCareRayAPIDemo->SetFrameRate(modeId, frameRate);
			if (GetReturnCode(nRet) > CR_WARN)
			{
				char info[256];
				CR_GetLastIntlMsg(info, 256);
				cout << info << endl;
			}
			else
			{
				cout << "set frame rate successfully" << endl;
				// acquisition测试
				vector<cv::Mat> images;
				/*nRet = pCareRayAPIDemo->Acquisition(5);*/
				nRet = pCareRayAPIDemo->AcquisitionImages(5, images);
				/*cout << nRet << endl;*/
				if (GetReturnCode(nRet) > CR_WARN)
				{
					char info[256];
					CR_GetLastIntlMsg(info, 256);
					cout << info << endl;
				}
				else 
				{
					cout << "acquisition successfully" << endl;
					/*for (int i = 0; i < images[1].rows; i++)
					{
						for (int j = 0; j < images[1].cols; j++)
						{
							cout << images[1].at<ushort>(i, j) << " ";
						}
					}
					cout << endl;*/
					cv::Mat tmp;
					cv::Mat dst8 = cv::Mat::zeros(images[4].size(), CV_8U);
					double mymin, mymax;
					cv::minMaxIdx(images[4], &mymin, &mymax);
					cout << mymin << " " << mymax << endl;
					cv::normalize(images[4], tmp, 0, 255, cv::NORM_MINMAX);
					cv::convertScaleAbs(tmp, dst8);
					cv::imshow("image", dst8);
					cv::waitKey(0);
				}

				// 校验测试
				/*int startCalib = 0;
				cout << "输入1进行暗场校验" << endl;
				cin >> startCalib;
				if (startCalib == 1)
				{
					nRet = pCareRayAPIDemo->DoOffsetCalibration();
					if (GetReturnCode(nRet) > CR_WARN)
					{
						char info[256];
						CR_GetLastIntlMsg(info, 256);
						cout << info << endl;
					}
					else
					{
						cout << "calibrate successfully" << endl;
					}
				}*/
			}
		}
		else
		{
			cout << "maxFrameRate is " << maxFrameRate << endl;
		}
	}

	getchar();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
