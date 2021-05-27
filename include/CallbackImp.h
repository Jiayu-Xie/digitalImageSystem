/**
* \file               CallbackImp.h
* \date             2016/11/22
* \version         5.0.0.2
* \author          Shuai Li
* \brief             Declaration of Callback function which would be implemented by integrator
*  Copyright 2016 CARERAY LTD. All Rights Reserved. 
*/

#ifndef __CALLBACK_IMP_H__
#define __CALLBACK_IMP_H__

#include "ICallback.h"
#include <iostream>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include "IntlMsgAliasID.h"
#include "CareRayAPIDll.h"
using namespace std;

/** 
* \class CCallbackImp
* \brief methods  and variables declare for callback function
*/
class CCallbackImp : public ICallback
{
public:

	CCallbackImp() : ICallback() { m_isDisableDebugFlag = true; m_pBuffStartAddr = NULL; m_isImageSaved = false; m_isPrintFlag = false; };

	/**
	* @brief set flag of processing result
	* @param[in] nRet  ( passed in parameter )
	*/
	void SetProcResult(int nRet) { m_nRet = nRet; };

	/**
	* @brief get flag of processing result
	*/
	int GetProcResult() { return m_nRet; };

	/**
	* @brief set flag of printing calibration progress information
	* @param[in] nRet  ( passed in parameter )
	*/
	void SetPrintFlag(bool isPrint) { m_isPrintFlag = isPrint; };

	CR_CalibrationInfo* GetCalInfo() { return &m_oCalInfo; };

	/**
	* @brief virtual function which would be override written used to process each event
	* @param[in] nEventID  ( trigger event id )
	* @param[in] pEventParam  ( the event data attached to the event id )
	*/
	virtual void Process(int nEventID, CR_Event* pEventParam);

	void SetImageSaveFlag(bool isImageSaved) { m_isImageSaved = isImageSaved; };
	
	void SetBufferStartAddr(void* pBuffStartAddr)
	{
		m_pBuffStartAddr = pBuffStartAddr;
	};

private:
	
	bool m_isImageSaved;
	void* m_pBuffStartAddr;

	int m_nRet;
	bool m_isPrintFlag;
	bool m_isDisableDebugFlag;
	CR_CalibrationInfo m_oCalInfo;

	void PrintMatrix(string strName, string imgDataStr)
	{
		ofstream fout;
		fout.open(strName, ios::binary);
		if(true != fout.good())
		{
			//throw CException(0,"Failed to open file.");
			cout<<"Failed to save frame to ."<< strName.c_str() << endl;
		}

		fout.write(imgDataStr.c_str(), imgDataStr.length());			
		fout.close();
	}
};

#endif