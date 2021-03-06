// digitalImage-test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "ImageProcess.h"
#include "CareRayAPIDemo.h"
#include <thread>
#include <Windows.h>

vector<cv::Mat> curImages;

void acquisionImagesThread(CCareRayAPIDemo* pCareRayAPIDemo)
{
	int cnt = 1;
	while (true)
	{
		vector<cv::Mat> tmp;
		int nRet = pCareRayAPIDemo->AcquisitionImages(cnt, tmp);
		if (GetReturnCode(nRet) > CR_WARN || cnt > 5)
		{
			break;
		}
		curImages = tmp;
		cout << cnt << " times" << endl;
		cnt++;
	}
	return;
}

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
				// 多线程取图测试
				thread th(acquisionImagesThread, pCareRayAPIDemo);
				th.detach();
				cout << "sleeping" << endl;
				Sleep(4000);
				cout << "size: " << curImages.size() << endl;
				// acquisition测试
				//vector<cv::Mat> images;
				///*nRet = pCareRayAPIDemo->Acquisition(5);*/
				//nRet = pCareRayAPIDemo->AcquisitionImages(5, images);
				///*cout << nRet << endl;*/
				//if (GetReturnCode(nRet) > CR_WARN)
				//{
				//	char info[256];
				//	CR_GetLastIntlMsg(info, 256);
				//	cout << info << endl;
				//}
				//else 
				//{
				//	cout << "acquisition successfully" << endl;
				//	/*for (int i = 0; i < images[1].rows; i++)
				//	{
				//		for (int j = 0; j < images[1].cols; j++)
				//		{
				//			cout << images[1].at<ushort>(i, j) << " ";
				//		}
				//	}
				//	cout << endl;*/
					
					// ImageProcess类测试
					cv::Mat image16Bit = curImages[0];
					ImageProcess *pImageProcess = new ImageProcess();
					cv::Mat image8Bit = pImageProcess->To8BitLinear(image16Bit, 200, 50);
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
				//}

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

	Sleep(3000);

	getchar();
}