// digitalImage-test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "ImageProcess.h"
#include "CareRayAPIDemo.h"
#include <thread>


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
					cv::Mat image16Bit = images[4];
					// ImageProcess类测试
					ImageProcess *pImageProcess = new ImageProcess(image16Bit);
					cv::Mat image8Bit = pImageProcess->To8BitLinear(200, 50);
					string savePath = "./digital-image-test-results/";
					imwrite(savePath + "1.jpg", image8Bit);
					cv::Mat flipImage8Bit = pImageProcess->Flip(image8Bit, 0);
					imwrite(savePath + "2.jpg", flipImage8Bit);
					cv::Mat rotateImage8Bit = pImageProcess->Rotate(image8Bit, 75, 1);
					imwrite(savePath + "3.jpg", rotateImage8Bit);
					cv::Mat scaleImage8Bit = pImageProcess->ScaleResize(image8Bit, 0.4, 0.4);
					imwrite(savePath + "4.jpg", scaleImage8Bit);
					cv::Mat fixedImage8Bit = pImageProcess->FixedResize(image8Bit, 600, 600);
					imwrite(savePath + "5.jpg", fixedImage8Bit);
					cv::Mat negImage8Bit = pImageProcess->Negative(image8Bit);
					imwrite(savePath + "6.jpg", negImage8Bit);
					cv::Mat colorImage8Bit = pImageProcess->ColorMap(image8Bit);
					imwrite(savePath + "7.jpg", colorImage8Bit);
					cout << colorImage8Bit.size() << endl;
					cout << image8Bit.size() << endl;
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
