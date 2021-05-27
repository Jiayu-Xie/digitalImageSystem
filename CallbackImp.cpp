/**
* \file                CallbackImp.cpp
* \date              2016/11/22
* \version          5.0.0.2
* \author           shuai.li
* \brief              Implement the callback function declared in CallbackImp.h
*  Copyright 2016 CARERAY LTD. All Rights Reserved. 
*/
#include "stdafx.h"
#include "CallbackImp.h"

void CCallbackImp::Process(int nEventID, CR_Event* pEventParam)
{
	/*cout << nEventID << endl;*/
	switch (nEventID)
	{
	case CR_EVT_NEW_FRAME:
		{
			//printf("Callback: detector id = %d.\n", pEventParam->nDetrIndex);
			//printf("Callback: event ID = %d, buff index = %d.\n", nEventID, *(int*)(pEventParam->pData));
			if (m_isImageSaved)
			{
				int nBuffIdx = *(int*)pEventParam->pData;

				stringstream ss;
				// cout << pEventParam->nDetrIndex << endl;
				ss << "D:\\" << pEventParam->nDetrIndex << "\\";
				string folder = ss.str();
				_mkdir(folder.c_str());
				
				ss<< pEventParam->nWidth << "x" << pEventParam->nHeight;
				folder = ss.str();
				_mkdir(folder.c_str());

				//int nImgSize = pEventParam->nWidth*pEventParam->nHeight*pEventParam->nDepth/8 + FRAME_HEADER_LEN;
				int nImgSize = pEventParam->nWidth*pEventParam->nHeight*2 + FRAME_HEADER_LEN;

				if (m_pBuffStartAddr != NULL)
				{
					char* pCurrFrame = (char*)m_pBuffStartAddr + nBuffIdx*nImgSize;

					//printf("Detector index in call back image is %d.\n", pCurrFrame[22]);

					int nFrameId = *(int*)pCurrFrame;
					ss.str("");
					ss << folder << "\\ "<< setw(3)<< setfill('0') <<nFrameId % 300 <<".raw";
					PrintMatrix(ss.str(), string(pCurrFrame, nImgSize));
				}
			}
		}
		break;
	case CR_EVT_CALIBRATION_IN_PROGRESS:
		{
			if (m_isPrintFlag)
			{
				CR_CalibrationInfo calInfo = *(CR_CalibrationInfo*)pEventParam->pData;
				cout<< "Callback: calibration progress: " <<calInfo.nCurrentFrameNum<<" / "<< calInfo.nTotalFrameNum <<", current frame mean = "<<calInfo.nCurrentFrameMeanValue<< '\r';
				SetProcResult(calInfo.nResult);
			}
		}
		break;
	case CR_EVT_CALIBRATION_FINISHED:
		{
			if (m_isPrintFlag)
			{
				CR_CalibrationInfo calInfo = *(CR_CalibrationInfo*)pEventParam->pData;
				cout<<endl<< "Callback: calibration result: "<<calInfo.nResult<< endl;
				m_oCalInfo = calInfo;
				SetProcResult(calInfo.nResult);
			}
		}
		break;
	case CR_EVT_TEMPERATURE_INFO:
		{
			if (!m_isDisableDebugFlag)
			{
				CR_Temperature temperature = *(CR_Temperature*)pEventParam->pData;
				cout<<"Average temperature: "<<temperature.aveTemperature<<endl;
				cout<<"Max temperature:"<<temperature.maxTemperature<<endl;
				cout<<"Over hot flag: "<<temperature.overhot_flag<<endl;
			}
		}
		break;
	case CR_EVT_ACQ_STAT_INFO:
		{
			CR_AcquisitionStatInfo acqStatInfo = *(CR_AcquisitionStatInfo*)pEventParam->pData;
			stringstream ss;
			ss<<"Statistic of current acquisition:"<<"duration: "<<acqStatInfo.nAcqDuration/1000.0<<" s, "
				<<"frames received: "<<acqStatInfo.nTotalFrameNum<<", frames lost: "<< acqStatInfo.nLostFrameNum <<", "
				<<"frame rate: "<<acqStatInfo.fStatFrameRate<<" fps, speed: "<<acqStatInfo.fTransmissionSpeed<<" MB/s"<<endl;
			cout<<endl<<ss.str()<<endl;
			
		}
		break;
	default:
		break;
	}
}
