// Theta5p0Client.cpp : 定义控制台应用程序的入口点。
//
#include "CareRayAPIDemo.h"

CCareRayAPIDemo::CCareRayAPIDemo()
{
	m_nCurrentDetrIndex = 1;//1 is the default index
	for (map<int, CR_ModeInfo*>::iterator iter = m_mapModeInfos.begin();  iter != m_mapModeInfos.end();)
	{
		iter->second = NULL;
	}

	for (map<int, CCallbackImp*>::iterator iter = m_mapCallbackPtrs.begin(); iter != m_mapCallbackPtrs.end();)
	{
		iter->second = NULL;
	}

	for (map<int, unsigned short*>::iterator iter = m_mapFrmBuf.begin(); iter != m_mapFrmBuf.end();)
	{
		iter->second = NULL;
	}
}

CCareRayAPIDemo::~CCareRayAPIDemo()
{
	for (map<int, CR_ModeInfo*>::iterator iter = m_mapModeInfos.begin();  iter != m_mapModeInfos.end();)
	{
		if (NULL != iter->second)
		{
			delete[] iter->second;
			iter->second = NULL;
			iter = m_mapModeInfos.erase(iter);
		}
	}

	for (map<int, CCallbackImp*>::iterator iter = m_mapCallbackPtrs.begin(); iter != m_mapCallbackPtrs.end();)
	{
		if (NULL != iter->second)
		{
			delete iter->second;
			iter->second = NULL;
			iter = m_mapCallbackPtrs.erase(iter);
		}
	}

	for (map<int, unsigned short*>::iterator iter = m_mapFrmBuf.begin(); iter != m_mapFrmBuf.end();)
	{
		if (NULL != iter->second)
		{
			delete[] iter->second;
			iter->second = NULL;
			iter = m_mapFrmBuf.erase(iter);
		}
	}
}

void CCareRayAPIDemo::PrintModeList(int nDetrIdx)
{
	cout << endl << endl;	
	cout << "----------------- List of Supported Application Modes ----------------" << endl;	                                                               
	for(int i = 0; i < m_mapModeNum[nDetrIdx]; i++)
	{
		cout << " Mode "<< i+1<< ": ";		
		switch (m_mapModeInfos[nDetrIdx][i].nModeID)
		{
		case CR_Radiography:
			cout << "Radiography" << endl; 
			break;
		case CR_Fluoroscopy:
			cout << "Fluoroscopy" << endl; 
			break;
		case CR_NondestructiveTesting:
			cout << "NondestructiveTesting (NDT)" << endl;
			break;
		case CR_Preview: 
			cout << "Preview." << endl; 
			break;
		case CR_FluoroLag:
			cout << "FluoroLag." << endl; 
			break;
		case CR_FluoroCustom:
			cout << "FluoroCustom." << endl;
			break;
		default:	
			break;
		}

		cout << "\n\tMode ID = " << m_mapModeInfos[nDetrIdx][i].nModeID << ";" << endl;
		cout << "\tImage dimension = " << m_mapModeInfos[nDetrIdx][i].nImageWidth <<" x "<<m_mapModeInfos[nDetrIdx][i].nImageHeight<< ";" << endl;
		cout << "\tBinning scheme = " << m_mapModeInfos[nDetrIdx][i].nBinX <<" x "<<m_mapModeInfos[nDetrIdx][i].nBinY<< ";" << endl;
		cout << "\tPixel depth = " << m_mapModeInfos[nDetrIdx][i].nPixelDepth << endl;
		cout << "\tMax frame rate = " << m_mapModeInfos[nDetrIdx][i].fMaxFrmRate <<" fps"<< endl;
		cout << "\tDescription = " << m_mapModeInfos[nDetrIdx][i].szDesc << endl;
	}
	cout << "---------------------------- End of List -----------------------------" << endl;
}

vector<int> CCareRayAPIDemo::GetModeIdList()
{
	vector<int> modeIdList;
	int nDetrIdx = m_nCurrentDetrIndex;
	for (int i = 0; i < m_mapModeNum[nDetrIdx]; i++)
	{
		modeIdList.push_back(m_mapModeInfos[nDetrIdx][i].nModeID);
	}
	return modeIdList;
}

vector<CR_ModeInfo> CCareRayAPIDemo::GetModeInfoList()
{
	vector<CR_ModeInfo> modeInfoList;
	int nDetrIdx = m_nCurrentDetrIndex;
	for (int i = 0; i < m_mapModeNum[nDetrIdx]; i++)
	{
		modeInfoList.push_back(m_mapModeInfos[nDetrIdx][i]);
	}
	return modeInfoList;
}

// 返回最大帧率，如果modeId无效则返回0
float CCareRayAPIDemo::GetMaxFrameRate(int modeId)
{
	float maxFrameRate = 0;
	for (int i = 0; i < m_mapModeNum[m_nCurrentDetrIndex]; i++)
	{
		if (m_mapModeInfos[m_nCurrentDetrIndex][i].nModeID == modeId)
		{
			maxFrameRate = m_mapModeInfos[m_nCurrentDetrIndex][i].fMaxFrmRate;
		}
	}
	return maxFrameRate;
}

void CCareRayAPIDemo::PrintDetrInfo(int nDetrIdx)
{
	CR_SystemInfo sysInfo;
	CR_DetrStatus detrStatus;

	int nRet = CR_GetSystemInformation(nDetrIdx, &sysInfo);
	if (OK == nRet)
	{
		cout << "System information:"<< endl;
		cout << "\tRaw image width (unit: pixels) = " << sysInfo.nRawImageWidth << ";" << endl;
		cout << "\tRaw image height (unit: pixels) = " << sysInfo.nRawImageHeight << ";" << endl;
		cout << "\tHardware version = " << string(sysInfo.szHardwareVersion) << ";" << endl;
		cout << "\tSerial number = " << string(sysInfo.szSerialNumber) << ";" << endl;
		cout << "\tSoftware version = " << string(sysInfo.szSoftwareVersion) << ";" << endl;
		cout << "\tFirmware version = " << string(sysInfo.szFirmwareVersion) << ";" << endl;
		cout << "\tMachine ID = " << string(sysInfo.szDetrMachineID) << ";" << endl;
		cout << "\tDetector description  = " << string(sysInfo.szDetrDesc) << ";" << endl;
		cout << endl;

		nRet = CR_GetDetrStatus(nDetrIdx, &detrStatus);
		if (OK == nRet)
		{
			cout << "Readings from temperature sensors inside the detector:"<< endl;			
			cout << "\tCurrent operating temperature = " << detrStatus.oDetrTemperature.aveTemperature << " degrees Celsius" << endl
				 << "\t                              = " << 32 + 9 * detrStatus.oDetrTemperature.aveTemperature / 5 << " degrees Fahrenheit" << endl;
			cout << "\tOverheat = " << ((1 == detrStatus.oDetrTemperature.overhot_flag)? "true" : "false") << endl;			
		}
	}
	cout << "**********************************************************************" << endl;
}

//int CCareRayAPIDemo::SetMode()
//{
//	int nRet = OK, nIdx = 0;
//	//Set frame or exposure time, set exposure time as default, and the other one as 0
//	cout<<"Set an application mode" <<endl;		
//	int nExpTime = 0, nTrigType = 0, nGainId = 0;
//	float fFrameRate = 0;		
//
//	cout <<endl<< "(1/4) Specify the mode ID as one of [ ";
//	for(nIdx = 0; nIdx < m_mapModeNum[m_nCurrentDetrIndex]; nIdx++)
//	{
//		if (nIdx != 0)
//		{
//			cout << " | ";
//		}
//		cout<<m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nModeID;
//	}
//	cout <<  " ]" << endl;
//	
//	cout << "Mode ID >> ";
//	cin >> m_nModeID;
//
//	for (nIdx = 0; nIdx < m_mapModeNum[m_nCurrentDetrIndex]; nIdx++)
//	{
//		if (m_nModeID == m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nModeID)
//		{
//			break;
//		}
//	}
//
//	if (nIdx < m_mapModeNum[m_nCurrentDetrIndex])
//	{
//		m_mapActiveModes[m_nCurrentDetrIndex] = m_mapModeInfos[m_nCurrentDetrIndex][nIdx];
//
//		cout <<endl<< "(2/4) Set frame rate (unit: fps, max frame rate: "<<m_mapActiveModes[m_nCurrentDetrIndex].fMaxFrmRate << ") >> ";
//		cin >> fFrameRate;
//
//		cout <<endl<< "(3/4) Specify the trigger type as one of [ ";
//		for (size_t rIdx2 = 0; rIdx2 < m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nTrigTypeNum; rIdx2++)
//		{
//			if (rIdx2 != 0)
//			{
//				cout << " | ";
//			}
//			switch (m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nTrigTypes[rIdx2])
//			{
//			case CR_FluReserved: 
//				cout << "7:free trigger"; 
//				break;
//			case CR_FluExtSync: 
//				cout << "8:external trigger";	
//				break;
//			case CR_FluIntSync: 
//				cout << "9:internal trigger"; 
//				break;
//			default: 
//				break;
//			}
//		}
//		cout <<  " ]" << endl;
//		cout << "Trigger type >> ";
//		cin >> nTrigType;
//		cout <<endl<< "(4/4) Specify the gain value as one of [ ";
//		for (size_t rIdx2 = 0; rIdx2 < size_t(m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nGainLevelNum); rIdx2++)
//		{
//			if (rIdx2 != 0)
//			{
//				cout << " | ";
//			}
//			cout<<m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nGainLevels[rIdx2];
//		}
//		cout <<  " ], or press 0 to use the default gain value of " << m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nDefaultGainLevel << endl;
//		cout << "Gain value >> ";
//		cin >> nGainId;
//		if (nGainId == 0)
//			nGainId = m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nDefaultGainLevel;
//
//		nRet = CR_SetApplicationModeWithParam(m_nCurrentDetrIndex, m_nModeID, &fFrameRate, &nExpTime, nTrigType, nGainId);
//		if (GetReturnCode(nRet) > CR_WARN)
//		{
//			
//			//string strIntlMsg = GetLastIntlMsg();
//			//cout<<"SetApplicationModeWithParam failed, "<<strIntlMsg.c_str()<<endl;
//			char info[256];
//			CR_GetLastIntlMsg(info, 256);
//			cout<<"SetApplicationModeWithParam Warning, "<<string(info)<<endl;
//		}else if (CR_WARN == GetReturnCode(nRet))
//		{
//			/*string strIntlMsg = GetLastIntlMsg();
//			cout<<"SetApplicationModeWithParam Warning, "<<strIntlMsg.c_str()<<endl;*/
//			char info[256];
//			CR_GetLastIntlMsg(info, 256);
//			cout<<"SetApplicationModeWithParam Warning, "<<string(info)<<endl;
//		}
//		else
//		{
//			cout<<"Set application mode successfully"<<endl;
//		}
//	}else
//	{
//		cout<<"Invalid mode ID parameter"<<endl;
//	}
//
//	return nRet;
//}

int CCareRayAPIDemo::SetMode(int modeId)
{
	int nRet = OK;
	nRet = CR_SetApplicationMode(m_nCurrentDetrIndex, modeId);
	if (GetReturnCode(nRet) <= CR_WARN)
	{
		int nIdx = 0;
		for (nIdx = 0; nIdx < m_mapModeNum[m_nCurrentDetrIndex]; nIdx++)
		{
			if (modeId == m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nModeID)
			{
				break;
			}
		}
		m_mapActiveModes[m_nCurrentDetrIndex] = m_mapModeInfos[m_nCurrentDetrIndex][nIdx];
	}
	return nRet;
}

int CCareRayAPIDemo::SetFrameRate(int modeId, float frameRate)
{
	int nRet = OK;
	int nExpTime = 0, nTrigType = 0, nGainLevel = 0;
	for (int i = 0; i < m_mapModeNum[m_nCurrentDetrIndex]; i++)
	{
		if (m_mapModeInfos[m_nCurrentDetrIndex][i].nModeID == modeId)
		{
			nTrigType = m_mapModeInfos[m_nCurrentDetrIndex][i].nDefaultTrigType;
			nGainLevel = m_mapModeInfos[m_nCurrentDetrIndex][i].nDefaultGainLevel;
		}
	}
	nRet = CR_SetApplicationModeWithParam(m_nCurrentDetrIndex, modeId, &frameRate, &nExpTime, nTrigType, nGainLevel);
	return nRet;
}

int CCareRayAPIDemo::Initialize()
{
// 	m_pCallback = new CCallbackImp();
// 	CR_RegisterEventCallback(m_nCurrentDetrIndex, m_pCallback);

	cout << "Initializing ..." << endl << endl;
	int nRet = 0;
	int nDetrNum;
	CR_DetrIdxAndIPAddr oDetrIdxAndIPAddr[16];
	CR_GetDetectorIndexAndIPAddress(oDetrIdxAndIPAddr, &nDetrNum);
	for (int i = 0; i < nDetrNum; i++)
	{
		m_mapDetrIdxnIPAddr.insert(make_pair(oDetrIdxAndIPAddr[i].nIdx, string(oDetrIdxAndIPAddr[i].szIPAddr)));
		m_mapModeInfos[oDetrIdxAndIPAddr[i].nIdx] = new CR_ModeInfo[16];
	}

	cout << "Total " << m_mapDetrIdxnIPAddr.size() << " detectors" << endl;
	for(map<int, string>::iterator iter = m_mapDetrIdxnIPAddr.begin(); iter != m_mapDetrIdxnIPAddr.end(); ++iter)
	{
		cout << "**************************** Detector "<< iter->first << " ******************************" << endl;
		CCallbackImp *pCallBack = new CCallbackImp();
		cout << "IP: " << iter->second << endl;

		m_mapCallbackPtrs[iter->first] = pCallBack;
		CR_RegisterEventCallback(iter->first, pCallBack);
		
		//m_pCallback = pCallBack;

		nRet = CR_Connect(iter->first);	
		if (GetReturnCode(nRet) > CR_WARN)
		{
			char info[256];
			CR_GetLastIntlMsg(info, 256);
			cout<<"Open failed, "<<string(info)<<endl;
		}
		else
		{
			nRet = CR_ResetDetector(iter->first, false);
			if (GetReturnCode(nRet) > CR_WARN)
			{
				char info[256];
				CR_GetLastIntlMsg(info, 256);
				cout<<"Reset failed, "<<string(info)<<endl;
			}
			else
			{
				nRet = CR_GetApplicationMode(iter->first, m_mapModeInfos[iter->first], &m_mapModeNum[iter->first]);
				if (GetReturnCode(nRet) > CR_WARN)
				{
				char info[256];
				CR_GetLastIntlMsg(info, 256);
				cout<<"GetApplicationMode failed, "<<string(info)<<endl;
				}
			}
		}

		if (OK == nRet)
		{
			PrintDetrInfo(iter->first);
			PrintModeList(iter->first);
		}
	}

	cout << "**********************************************************************" << endl;
	return nRet;
}

int CCareRayAPIDemo::Quit()
{
	int nRet = OK;
	nRet = CR_Disconnect(m_nCurrentDetrIndex);
	if (GetReturnCode(nRet) > CR_WARN)
	{
		/*string strIntlMsg = GetLastIntlMsg();
		cout<<"Close failed, "<<strIntlMsg.c_str()<<endl;*/
		char info[256];
		CR_GetLastIntlMsg(info, 256);
		cout<<"Close failed, "<<string(info)<<endl;
	}
	return nRet;
}

int CCareRayAPIDemo::DoOffsetCalibration()
{
	int nRet = OK, nCurrFrmNum = 0, nPrevFrmNum = nCurrFrmNum - 1, nKeyToContinue = 0;
	CR_CalibrationInfo calProgInfo;
	m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetPrintFlag(false);

	//////////////////////////////////////////////////////////////////////////
	//1. Dark Calibration, the X-ray shouldn't expose the detector
	//////////////////////////////////////////////////////////////////////////
	cout<<"Dark calibration starts"<<endl;
	nRet = CR_StartDarkCalibration(m_nCurrentDetrIndex);
	if (GetReturnCode(nRet) > CR_WARN)
	{
		/*string strIntlMsg = GetLastIntlMsg();
		cout<<"StartDarkCalibration failed, "<<strIntlMsg.c_str()<<endl;*/
		char info[256];
		CR_GetLastIntlMsg(info, 256);
		cout<<"StartDarkCalibration failed, "<<string(info)<<endl;
		return nRet;
	}

	CR_QueryCalibrationStatus(m_nCurrentDetrIndex, &calProgInfo);
	nCurrFrmNum = 0;
	nPrevFrmNum = nCurrFrmNum;
	while(-1 == calProgInfo.nResult)
	{
		if (_kbhit())
		{
			nRet = CR_StopCalibration(m_nCurrentDetrIndex);
			return nRet;
		}
		if(nCurrFrmNum > nPrevFrmNum)
		{
			nPrevFrmNum = nCurrFrmNum;
			cout<< "Dark calibration progress: " <<calProgInfo.nCurrentFrameNum<<" / "<< calProgInfo.nTotalFrameNum<< '\r';						
		}
		nCurrFrmNum = calProgInfo.nCurrentFrameNum;					
		CR_QueryCalibrationStatus(m_nCurrentDetrIndex, &calProgInfo);
		Sleep(5);
	}
	cout << endl;

	nRet = calProgInfo.nResult;
	if (OK == nRet)
	{
		cout<<"Offset calibration completes successfully."<<endl;
	}else if (CR_WARN == GetReturnCode(nRet))
	{
		cout<<"StartDarkCalibration warns, "<<string(calProgInfo.szErrorMsg)<<endl;
		return nRet;
	}else if (GetReturnCode(nRet) > CR_WARN)
	{
		cout<<"StartDarkCalibration failed, "<<string(calProgInfo.szErrorMsg)<<endl;
		return nRet;
	}

	return nRet;
}

int CCareRayAPIDemo::DoGainCalibration()
{
	int nRet = OK, nCurrFrmNum = 0, nPrevFrmNum = nCurrFrmNum - 1, nKeyToContinue = 0;
	CR_CalibrationInfo calProgInfo;
	m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetPrintFlag(false);

	//////////////////////////////////////////////////////////////////////////
	//2. Gain Calibration, the X-ray should expose the detector
	//////////////////////////////////////////////////////////////////////////
	/*cout << endl;
	cout << "Make sure that the detector receives X-ray before gain calibration starts" << endl;
	cout << "Enter any integer to continue" << endl << "Your input >> ";
	cin >> nKeyToContinue;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore();
		cout << "Invalid input, enter again" << endl << "Your input >> ";
		cin>>nKeyToContinue;
	}*/
	nCurrFrmNum = 0;
	nPrevFrmNum = nCurrFrmNum;
	nRet = CR_StartGainCalibration(m_nCurrentDetrIndex);
	if (GetReturnCode(nRet) > CR_WARN)
	{
		/*string strIntlMsg = GetLastIntlMsg();
		cout<<"StartGainCalibration failed, "<<strIntlMsg.c_str()<<endl;*/
		char info[256];
		CR_GetLastIntlMsg(info, 256);
		cout<<"StartGainCalibration failed, "<<string(info)<<endl;
		return nRet;
	}

	CR_QueryCalibrationStatus(m_nCurrentDetrIndex, &calProgInfo);
	while(-1 == calProgInfo.nResult)
	{
		if (_kbhit())
		{
			nRet = CR_StopCalibration(m_nCurrentDetrIndex);
			return nRet;
		}

		if(nCurrFrmNum > nPrevFrmNum)
		{
			nPrevFrmNum = nCurrFrmNum;
			cout<< "Gain calibration progress: " <<calProgInfo.nCurrentFrameNum<<
				" / "<< calProgInfo.nTotalFrameNum <<", current frame mean = "<<calProgInfo.nCurrentFrameMeanValue << '\r';
		}
		nCurrFrmNum = calProgInfo.nCurrentFrameNum;					
		CR_QueryCalibrationStatus(m_nCurrentDetrIndex, &calProgInfo);
		Sleep(5);
	}
	cout << endl;

	nRet = calProgInfo.nResult;
	if(OK == nRet)
	{
		cout<<"Gain calibration completes successfully."<<endl;
	}else if(GetReturnCode(nRet) > CR_WARN)
	{
		cout<<"StartGainCalibration failed, "<<string(calProgInfo.szErrorMsg)<<endl;
		return nRet;
	}else if (CR_WARN == GetReturnCode(nRet))
	{
		cout<<"StartGainCalibration warns, "<<string(calProgInfo.szErrorMsg)<<endl;
	}

	return nRet;
}

int CCareRayAPIDemo::Calibration()
{
	int nRet = DoOffsetCalibration();
	if (OK == nRet)
	{
		nRet = DoGainCalibration();
	}

	return nRet;
}

// void CCareRayAPIDemo::CalibrationUsingInterrupts()
// {
// 	m_pCallback->SetPrintFlag(true);
// 	thread t1(&CCareRayAPIDemo::CalibrationThread, m_pCallback);
// 	t1.join();
// }

// int CCareRayAPIDemo::CalibrationThread(CCallbackImp* pCallbackImp)
// {
// 	//////////////////////////////////////////////////////////////////////////
// 	//1. Dark Calibration, the X-ray shouldn't expose the detector
// 	//////////////////////////////////////////////////////////////////////////
// 	cout<<"Dark calibration starts ..."<<endl;
// 	int nRet = StartDarkCalibration();
// 	if (GetReturnCode(nRet) > WARN)
// 	{
// 		string strIntlMsg = GetLastIntlMsg();
// 		cout<<"StartDarkCalibration failed, "<<strIntlMsg.c_str()<<endl;
// 		return nRet;
// 	}
// 
// 	unique_lock<std::mutex> uniqLock(pCallbackImp->m_mtx);
// 	pCallbackImp->m_cndVar.wait(uniqLock);
// 	nRet = pCallbackImp->GetProcResult();
// 	if (GetReturnCode(nRet) > WARN)
// 	{
// 		cout<<"StartDarkCalibration failed, "<<pCallbackImp->GetCalInfo()->strReason.c_str()<<endl;
// 		return nRet;
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	//2. Gain Calibration, the X-ray should expose the detector
// 	//////////////////////////////////////////////////////////////////////////
// 	int nKeyToContinue = 0;
// 	cout<<"Gain calibration starts. Make sure that the detector receives X-ray, then press any number key (0-9) to continue..."<<endl;
// 	cin>> nKeyToContinue;
// 	nRet = StartGainCalibration();
// 	if (GetReturnCode(nRet) > WARN)
// 	{
// 		string strIntlMsg = GetLastIntlMsg();
// 		cout<<"StartGainCalibration failed, "<<strIntlMsg.c_str()<<endl;
// 		return nRet;
// 	}
// 
// 	pCallbackImp->m_cndVar.wait(uniqLock);
// 	nRet = pCallbackImp->GetProcResult();
// 	if (GetReturnCode(nRet) > WARN)
// 	{
// 		cout<<"StartGainCalibration failed, "<<pCallbackImp->GetCalInfo()->strReason.c_str()<<endl;
// 	}else if (WARN == GetReturnCode(nRet))
// 	{
// 		cout<<"Calibration warns, "<<pCallbackImp->GetCalInfo()->strReason.c_str()<<endl;
// 	}
// 
// 	return nRet;
// }

int CCareRayAPIDemo::Acquisition(int nFrameNum)
{
	for (map<int, unsigned short*>::iterator iter = m_mapFrmBuf.begin(); iter != m_mapFrmBuf.end();)
	{
		delete[] iter->second;
		iter->second = NULL;
		iter = m_mapFrmBuf.erase(iter);
	}
	
	int nRet = OK, nIdx = 0, nFrameWidth = 0, nFrameHeight = 0;
	const int nFrameHeaderSize = 32;	

	for (nIdx = 0; nIdx < m_mapModeNum[m_nCurrentDetrIndex]; nIdx++)
	{
		if (m_mapActiveModes[m_nCurrentDetrIndex].nModeID == m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nModeID)
		{
			break;
		}
	}

	if (nIdx < m_mapModeNum[m_nCurrentDetrIndex])
	{
		int nSaveFlag = 0, nStartAcq = 0;
		cout<<"Press 1 if you want to save fluoroscopic images. Otherwise, press 0" << endl << "Your input >> ";
		cin>>nSaveFlag;
		while (cin.fail())
		{
			cin.clear();
			cin.ignore();
			cout << "Invalid input, enter again" << endl << "Your input >> ";
			cin >> nSaveFlag;
		}
		if (0 != nSaveFlag)
		{
			m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetImageSaveFlag(true);
		}else
		{
			m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetImageSaveFlag(false);
		}

		cout <<endl <<"Press any key to quit anytime during the course of image acquisition." << endl;
		cout << "Enter any integer to start fluoroscopic image acquisition" << endl << "Your input >> ";
		cin >> nStartAcq;
		while (cin.fail())
		{
			cin.clear();
			cin.ignore();
			cout << "Invalid input, enter again" << endl << "Your input >> ";
			cin >> nStartAcq;
		}

		nFrameWidth = m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nImageWidth, nFrameHeight = m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nImageHeight;
		if (NULL == m_mapFrmBuf[m_nCurrentDetrIndex])
		{
			m_mapFrmBuf[m_nCurrentDetrIndex] = (unsigned short*)malloc(((nFrameWidth * nFrameHeight + nFrameHeaderSize) * nFrameNum) * sizeof(unsigned short));
			if (NULL == m_mapFrmBuf[m_nCurrentDetrIndex])
			{
				return ALLOCATE_BUFF_FAIL;
			}
		}
		
		m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetBufferStartAddr(m_mapFrmBuf[m_nCurrentDetrIndex]);

		nRet = CR_StartAcquisition(m_nCurrentDetrIndex, nFrameNum, m_mapFrmBuf[m_nCurrentDetrIndex]);
		if (GetReturnCode(nRet) > CR_WARN)
		{
			/*string strIntlMsg = GetLastIntlMsg();
			cout<<"StartAcquisition failed, "<<strIntlMsg.c_str()<<endl;*/
			char info[256];
			CR_GetLastIntlMsg(info, 256);
			cout<<"StartAcquisition failed, "<<string(info)<<endl;
			return nRet;
		}

		//Press any key to stop the acquisition process
		while (!_kbhit())
		{
		}

		nRet = CR_StopAcquisition(m_nCurrentDetrIndex);
		if (GetReturnCode(nRet) > CR_WARN)
		{
			char info[256];
			CR_GetLastIntlMsg(info, 256);
			cout<<"StopAcquisition failed, "<<string(info)<<endl;
			return nRet;
		}
		/*int nImgSize = nFrameWidth * nFrameHeight + nFrameHeaderSize;
		unsigned short* m_pBuffStartAddr = m_mapFrmBuf[m_nCurrentDetrIndex];
		for (int i = 0; i < 1; i++)
		{
			unsigned short* pCurFrame = m_pBuffStartAddr + i * nImgSize;
			int j = 0;
			while (j < nImgSize) {
				cout << *pCurFrame << " ";
				pCurFrame++;
				j++;
			}
			cout << endl;
			cout << nImgSize << endl;
		}*/
		m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetBufferStartAddr(NULL);
	}
	
	return nRet;
}

int CCareRayAPIDemo::AcquisitionImages(int nFrameNum, vector<cv::Mat>& images)
{
	for (map<int, unsigned short*>::iterator iter = m_mapFrmBuf.begin(); iter != m_mapFrmBuf.end();)
	{
		delete[] iter->second;
		iter->second = NULL;
		iter = m_mapFrmBuf.erase(iter);
	}

	int nRet = OK, nIdx = 0, nFrameWidth = 0, nFrameHeight = 0;
	const int nFrameHeaderSize = 32;

	for (nIdx = 0; nIdx < m_mapModeNum[m_nCurrentDetrIndex]; nIdx++)
	{
		if (m_mapActiveModes[m_nCurrentDetrIndex].nModeID == m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nModeID)
		{
			break;
		}
	}

	if (nIdx < m_mapModeNum[m_nCurrentDetrIndex])
	{
		nFrameWidth = m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nImageWidth, nFrameHeight = m_mapModeInfos[m_nCurrentDetrIndex][nIdx].nImageHeight;
		if (NULL == m_mapFrmBuf[m_nCurrentDetrIndex])
		{
			m_mapFrmBuf[m_nCurrentDetrIndex] = (unsigned short*)malloc(((nFrameWidth * nFrameHeight + nFrameHeaderSize) * nFrameNum) * sizeof(unsigned short));
			if (NULL == m_mapFrmBuf[m_nCurrentDetrIndex])
			{
				return ALLOCATE_BUFF_FAIL;
			}
		}

		m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetBufferStartAddr(m_mapFrmBuf[m_nCurrentDetrIndex]);

		nRet = CR_StartAcquisition(m_nCurrentDetrIndex, nFrameNum, m_mapFrmBuf[m_nCurrentDetrIndex]);
		if (GetReturnCode(nRet) > CR_WARN)
		{
			/*string strIntlMsg = GetLastIntlMsg();
			cout<<"StartAcquisition failed, "<<strIntlMsg.c_str()<<endl;*/
			char info[256];
			CR_GetLastIntlMsg(info, 256);
			cout << "StartAcquisition failed, " << string(info) << endl;
			return nRet;
		}

		Sleep(1000 * nFrameNum);
		nRet = CR_StopAcquisition(m_nCurrentDetrIndex);
		vector<cv::Mat> result;
		if (GetReturnCode(nRet) > CR_WARN)
		{
			char info[256];
			CR_GetLastIntlMsg(info, 256);
			cout << "StopAcquisition failed, " << string(info) << endl;
			return nRet;
		}

		int nImgSize = nFrameWidth * nFrameHeight + nFrameHeaderSize;
		unsigned short* m_pBuffStartAddr = m_mapFrmBuf[m_nCurrentDetrIndex];
		for (int i = 0; i < nFrameNum; i++)
		{
			unsigned short* pCurFrame = m_pBuffStartAddr + i * nImgSize + nFrameHeaderSize;
			cv::Mat curImage(nFrameHeight, nFrameWidth, CV_16UC1);
			for (int j = 0; j < nFrameHeight; j++)
			{
				for (int k = 0; k < nFrameWidth; k++)
				{
					curImage.at<ushort>(j, k) = *pCurFrame;
					pCurFrame++;
				}
			}
			images.push_back(curImage);
		}
		m_mapCallbackPtrs[m_nCurrentDetrIndex]->SetBufferStartAddr(NULL);
	}

	return nRet;
}

int CCareRayAPIDemo::AcqAllDetrImgs()
{
	for (map<int, unsigned short*>::iterator iter = m_mapFrmBuf.begin(); iter != m_mapFrmBuf.end();)
	{
		delete[] iter->second;
		iter->second = NULL;
		iter = m_mapFrmBuf.erase(iter);
	}

	const int nFrameHeaderSize = 32, nFrameNum = 20;
	int nRet = OK, nFrameWidth = 0, nFrameHeight = 0;
	for(map<int, CR_ModeInfo>::iterator iterMode = m_mapActiveModes.begin(); iterMode != m_mapActiveModes.end(); ++iterMode)
	{
		m_mapCallbackPtrs[iterMode->first]->SetImageSaveFlag(false);
		nFrameWidth = iterMode->second.nImageWidth;
		nFrameHeight = iterMode->second.nImageHeight;

		if (NULL == m_mapFrmBuf[iterMode->first])
		{
			m_mapFrmBuf[iterMode->first] = (unsigned short*)malloc(((nFrameWidth * nFrameHeight + nFrameHeaderSize) * nFrameNum) * sizeof(unsigned short));
			m_mapCallbackPtrs[iterMode->first]->SetBufferStartAddr(m_mapFrmBuf[iterMode->first]);
		}
// 		unsigned short *pFrmBuf = (unsigned short*)malloc(((nFrameWidth * nFrameHeight + nFrameHeaderSize) * nFrameNum) * sizeof(unsigned short));
// 		m_mapCallbackPtrs[iterMode->first]->SetBufferStartAddr(pFrmBuf);

		nRet = CR_StartAcquisition(iterMode->first, nFrameNum, m_mapFrmBuf[iterMode->first]);
		if (GetReturnCode(nRet) > CR_WARN)
		{
			char info[256];
			CR_GetLastIntlMsg(info, 256);
			cout<<"StartAcquisition failed, "<<string(info)<<endl;
			return nRet;
		}
		else
		{
			cout<<"StartAcquisition for detector "<< iterMode->first <<endl;
		}
	}
	
	//Press any key to stop the acquisition process
	while (!_kbhit())
	{
	}

	for(map<int, CCallbackImp*>::iterator iterCallBack = m_mapCallbackPtrs.begin(); iterCallBack != m_mapCallbackPtrs.end(); ++iterCallBack)
	{
		nRet = CR_StopAcquisition(iterCallBack->first);
		if (GetReturnCode(nRet) > CR_WARN)
		{
			char info[256];
			CR_GetLastIntlMsg(info, 256);
			cout<<"StopAcquisition failed, "<<string(info)<<endl;
			return nRet;

		}
		else
		{
			cout<<"StopAcquisition for detector "<< iterCallBack->first <<endl;
		}

		iterCallBack->second->SetBufferStartAddr(NULL);		
	}

	return nRet;
}
