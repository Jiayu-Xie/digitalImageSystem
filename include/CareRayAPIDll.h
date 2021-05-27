#ifndef __CARERAY_API_DLL_H__
#define __CARERAY_API_DLL_H__

#include <map>
#include "ICallback.h"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _CARERAY_API_
#define CARERAY_API_PORT __declspec(dllexport)
#else
#define CARERAY_API_PORT __declspec(dllimport)
#endif

CARERAY_API_PORT int CR_GetDetectorIndexAndIPAddress(CR_DetrIdxAndIPAddr* pDetrIdxAndIPAddr, int* pDetrNum);
CARERAY_API_PORT int CR_Connect(int nDetrIndex);
CARERAY_API_PORT int CR_Disconnect(int nDetrIndex);
CARERAY_API_PORT int CR_ResetDetector(int nDetrIndex, bool needReboot);
CARERAY_API_PORT int CR_DoSelfTest(int nDetrIndex);
CARERAY_API_PORT int CR_GetSystemInformation(int nDetrIndex, CR_SystemInfo* pSystemInformation);
CARERAY_API_PORT int CR_GetApplicationMode(int nDetrIndex, CR_ModeInfo* pModeInfo, int* pModeNum);
CARERAY_API_PORT int CR_SetApplicationMode(int nDetrIndex, int nModeIndex);
CARERAY_API_PORT int CR_SetApplicationModeWithParam(int nDetrIndex, int nModeIndex, float *pFrameRate, int *pExposureTime, int nTriggType, int nGainIndex);
CARERAY_API_PORT int CR_StartAcquisition(int nDetrIndex, int nFrameNum, void* pBuffer, int nNumFrmReqFromDetr = -1); // nNumFrmReqFromDetr == -1 means that detector sends out frames continuously until StopAcquisition() is received.
CARERAY_API_PORT int CR_StopAcquisition(int nDetrIndex);
CARERAY_API_PORT int CR_GetDetrStatus(int nDetrIndex, CR_DetrStatus* pDetrStatus);
CARERAY_API_PORT int CR_StartDarkCalibration(int nDetrIndex);
CARERAY_API_PORT int CR_StartGainCalibration(int nDetrIndex);
CARERAY_API_PORT int CR_StopCalibration(int nDetrIndex);
CARERAY_API_PORT int CR_QueryCalibrationStatus(int nDetrIndex, CR_CalibrationInfo* pCalProgInfo);
CARERAY_API_PORT int CR_RegisterEventCallback(int nDetrIndex, ICallback* pCallback);

// CARERAY_API_PORT int CR_ValidateAccount(const int nDetrIndex, const string strName, const string strPassword);
// CARERAY_API_PORT int CR_ChangePassword(const int nDetrIndex, const string strName, const string strOldPassword, const string strNewPassword);
// CARERAY_API_PORT int CR_DeleteProcChain(const int nDetrIndex, const int nChainID);
// CARERAY_API_PORT int CR_QueryAccounts(const int nDetrIndex, vector<string>& accountNames);
// CARERAY_API_PORT int CR_QueryConfItemInfo(const int nDetrIndex, string strModule, int nPermission, vector<CR_ConfItemInfo> &itemsVector);
// CARERAY_API_PORT int CR_ModifyConfItems(const int nDetrIndex, map<string, string>& itemsMap);
// CARERAY_API_PORT int CR_QueryProcChains(const int nDetrIndex, map<int, string>& chainMap);
// CARERAY_API_PORT int CR_QueryProcChainInfo(const int nDetrIndex, int nChainID, CR_ProcessChain& chainInfo);
// CARERAY_API_PORT int CR_CreateProcChain(const int nDetrIndex, string strChainName, CR_ProcessChain& chainInfo);
// CARERAY_API_PORT int CR_ModifyProcChain(const int nDetrIndex, int nChainID, CR_ProcessChain& chainInfo);
// CARERAY_API_PORT int CR_RestoreProcChain(const int nDetrIndex, int nChainID);
CARERAY_API_PORT void CR_GetLastIntlMsg(char* szMsgBuf, int nBufLen, int nErrorCode = -1);
#ifdef __cplusplus
}
#endif

#endif