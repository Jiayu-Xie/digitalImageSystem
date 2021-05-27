#ifndef __STRUCTURE_H__	
#define __STRUCTURE_H__

#include <iostream>
#include <vector>
using std::vector;
using std::string;

#define FRAME_HEADER_LEN 64

namespace CareRay
{
	enum CR_TrigTypeOpt
	{
		CR_RadExtSync		= 0x01,
		CR_RadSoftSync		= 0x02,
		CR_RadAutoSync		= 0x03,
		CR_RadManualSync	= 0x04,
		CR_RadScanSync		= 0x05,
		CR_RadAEDSync		= 0x06,
		CR_FluReserved		= 0x07,
		CR_FluExtSync		= 0x08,
		CR_FluIntSync	= 0x09,
	};

	enum CR_GainLevelOpt
	{
		CR_G0 = 0,
		CR_G1 = 1,
		CR_G2 = 2,
		CR_G3 = 3,
		CR_G4 = 4,
		CR_G5 = 5,
		CR_G6 = 6,
		CR_G7 = 7,
	};

	enum CR_AcqModeOpt
	{
		CR_Radiography	= 0x10,
		CR_Fluoroscopy= 0x11,
		CR_FluoroLag	= 0x12,
		CR_FluoroCustom = 0x13,
		CR_InnerTest	= 0x14,
		CR_Bin22 = 0x15,
		CR_NondestructiveTesting	= 0x16,
		CR_Preview	 = 0x17,
		CR_Bin11 = 0x18,
	};

	enum CR_ReturnCodeType
	{
		CR_INFOR = 1,
		CR_WARN,
		CR_ERROR,
		CR_FATALERROR
	};

	struct CR_SystemInfo
	{
		unsigned int nRawImageWidth;
		unsigned int nRawImageHeight;
		char szHardwareVersion[64];
		char szSerialNumber[64];
		char szSoftwareVersion[64];
		char szFirmwareVersion[64];
		char szDetrMachineID[64];
		char szDetrDesc[128];
		char szReserved[512];
	};

	struct CR_Temperature
	{
		float reserved[5];
		float maxTemperature;
		float aveTemperature;
		unsigned overhot_flag;
	};

	struct CR_DetrStatus
	{
		CR_Temperature oDetrTemperature;
	};

	struct CR_DetrIdxAndIPAddr
	{
		int nIdx;
		char szIPAddr[64];
		char szReserved[128];
	};

	struct CR_ModeInfo
	{
		int nModeID;
		int nImageWidth;	// ROI dimension along x direction after binning
		int nImageHeight;	// ROI dimension along y direction after binning
		int nCutoffX;			// number of cut-off lines (along x direction) from left and right after binning
		int nCutoffY;			// number of cut-off lines (along y direction) from top and bottom after binning
		int nBinX;				// binning scheme along x direction
		int nBinY;				// binning scheme along y direction
		float fMaxFrmRate;	// in Hz
		int nMaxExpTime;		// in ms
		int nPixelDepth;			// bit depth of each pixel sending out from detector
		int nTrigTypes[16];
		int nTrigTypeNum;
		int nGainLevels[16];
		int nGainLevelNum;
		int nDefaultTrigType;
		int nDefaultGainLevel;
		int nRoiX;					// x coordinate of top left point after binning
		int nRoiY;					// y coordinate of top left point after binning
		char szDesc[256];		// description of this mode
	};

	struct CR_CalibrationInfo
	{
		int nTotalFrameNum;
		int nCurrentFrameNum;
		int nCurrentFrameMeanValue;
		char szErrorMsg[256];
		int nResult;
	};

	struct CR_ConfItemInfo
	{
		int nId;
		string strName;
		string strModule;
		string strValue;
		string strDefaultValue;
		string strRegex;
		int nPermission;
		string strDesc;
	};

	struct CR_ProcessParam
	{
		int nParamID;
		int nModuleID;
		int nChainID;
		string strName;
		string strValue;
		string strDefaultValue;
		int nType;
		string strRegex;
		string strDesc;
	};

	struct CR_ProcessModule
	{
		int nModuleID;
		int nChainID;
		string strName;
		int nOrderID;
		int nEnabled;
		int nDefaultEnabled;
		int nAutoSave;
		int nUpdatePolicy;
		vector<CR_ProcessParam> vecParam;
	};

	struct CR_ProcessChain
	{
		int nChainID;
		string strName;
		vector<CR_ProcessModule> vecModule;
	};

enum CR_EventID
{
	CR_EVT_SERVER_DISCONNECTED,
	CR_EVT_DETR_DISCONNECTED,
	CR_EVT_EXPOSURE_INFO,
	CR_EVT_TEMPERATURE_INFO,
	CR_EVT_BATTERY_INFO,
	CR_EVT_WIRELESS_INFO,
	CR_EVT_NEW_FRAME,
	CR_EVT_CALIBRATION_IN_PROGRESS,
	CR_EVT_CALIBRATION_FINISHED,
	CR_EVT_ACQ_STAT_INFO
};

	struct CR_Event
	{
		int nDetrIndex;
		int nWidth;
		int nHeight;
		int nPixelDepth;
		void* pData;
	};

	struct CR_AcquisitionStatInfo
	{
		int nTotalFrameNum;
		int nLostFrameNum;
		float fStatFrameRate;
		float fTransmissionSpeed;
		long long nAcqDuration;
	};
}

#endif