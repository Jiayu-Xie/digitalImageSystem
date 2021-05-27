/**
* \file               CCareRayAPIDemo.h
* \date             2016/12/01
* \version         5.0.0.2
* \author          Shuai Li
* \brief             CareRay API demo header file
*  Copyright 2016 CARERAY LTD. All Rights Reserved. 
*/

#ifndef __CARERAY_API_DEMO_H__
#define __CARERAY_API_DEMO_H__
#include "stdafx.h"
#include <iostream>
#include <Windows.h>

#include "CallbackImp.h"
#include "IntlMsgAliasID.h"
#include <map>
#include <vector>
#include "opencv2/core/core.hpp"

using namespace std;

inline int GetReturnCode(int nCode) { return nCode / 100000;}

/** 
* \class CCareRayAPIDemo
* \brief methods  and variables
*/
class CCareRayAPIDemo
{
public:

	CCareRayAPIDemo();
	~CCareRayAPIDemo();

	/**
	* @brief initialize detector (connect)
	*/
	int Initialize();

	/**
	* @brief select application mode and set to detector
	*/
	int SetMode(int modeId);

	int SetFrameRate(int modeId, float frameRate);

	float GetMaxFrameRate(int modeId);

	/**
	* @brief perform offset calibration
	*/
	int DoOffsetCalibration();

	/**
	* @brief perform gain calibration
	*/
	int DoGainCalibration();

	/**
	* @brief calibration detector, include offset and gain calibration by polling way
	*/
	int Calibration();

	/**
	* @brief calibration detector, include offset and gain calibration by interrupt way
	*/
/*	void CalibrationUsingInterrupts();*/

	/**
	* @brief fluoroscopy images acquisition process
	*/
	int Acquisition(int nFrameNum);

	int AcquisitionImages(int nFrameNum, vector<cv::Mat>& images);

	int AcqAllDetrImgs();
	
	/**
	* @brief disconnect detector
	*/
	int Quit();

	void PrintDetrInfo(int nDetrIdx);

	void PrintModeList(int nDetrIdx);

	vector<int> GetModeIdList();

	vector<CR_ModeInfo> GetModeInfoList();

	void SetCurrDetrIdx(int nDetrIdx)
	{
		m_nCurrentDetrIndex = nDetrIdx;
		//m_pCallback = m_mapCallbackPtrs[nDetrIdx];
	}
	
	int GetCurrDetrIdx()
	{
		return m_nCurrentDetrIndex;
	}

	void GetDetrIdxs(vector<int> *pDetrIdxs)
	{
		for(map<int, string>::iterator iter = m_mapDetrIdxnIPAddr.begin(); iter != m_mapDetrIdxnIPAddr.end(); ++iter)
		{
			pDetrIdxs->push_back(iter->first);
		}
	}

	void PrintDetrIpIdx()
	{
		for(map<int, string>::iterator iter = m_mapDetrIdxnIPAddr.begin(); iter != m_mapDetrIdxnIPAddr.end(); ++iter)
		{
			cout << "Detector index " << iter->first << ", IP " << iter->second << endl;
		}
	}

private:

/*	static int CalibrationThread(CCallbackImp* pCallbackImp);*/
	//static int GetReturnCode(int nCode) { return nCode / 100000; };
	//CCallbackImp* m_pCallback;
	//vector<ModeInfo> m_vecModeInfo;
	//CR_ModeInfo m_oModeInfos[16];
	//int m_nModeNum;
	int m_nModeID;
	//unsigned short* m_pFrmBuf;
	int m_nCurrentDetrIndex; // current Detector Index used, this property is reserved until there are more than one detectors.

	map<int, unsigned short*> m_mapFrmBuf;
	map<int, int> m_mapModeNum;
	map<int, string> m_mapDetrIdxnIPAddr;
	map<int, CCallbackImp*> m_mapCallbackPtrs;
	map<int, CR_ModeInfo> m_mapActiveModes;
	map<int, CR_ModeInfo*> m_mapModeInfos;
};

#endif