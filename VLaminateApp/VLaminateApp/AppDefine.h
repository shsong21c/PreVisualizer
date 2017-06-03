#ifndef __AppDefine_H__
#define __AppDefine_H__

// 버전 정보 정의
#define MAIN_WINDOW_TITLE      _T(" VLaminate Inspection Ver 3.1(17.05.28) ")
// 버전 정보 정의

// 파일 로드, 저장 관련 패스의 위치 정의
#define SAVE_DATA_PATH         _T("D:\\DATABASE\\DATA\\")
#define LOG_DATA_FILE          _T("D:\\DATABASE\\LOG\\")
#define SAVE_SHORTDATA_PATH    _T("D:\\DATABASE\\SHORTDATA\\")
#define SAVE_EXCEL_PATH        _T("D:\\DATABASE\\EXCEL\\")
#define SAVE_PLOT_PATH         _T("D:\\DATABASE\\PLOT\\")
#define SAVE_IMAGE_PATH        _T("DATABASE\\IMAGE\\")

#define MODEL_FILE_PATH        _T("C:\\APPL\\VLaminateApp\\MODEL\\")
#define SETTING_DATA_FILE      _T("C:\\APPL\\VLaminateApp\\SETUP\\Config.cfg")
#define ETC_DATA_FILE          _T("C:\\APPL\\VLaminateApp\\SETUP\\Count.cfg")
#define MODEL_IMAGE_PATH       _T("C:\\APPL\\VLaminateApp\\MODELIMAGE\\")
#define DEF_SHORTPLOT_PATHNAME _T("C:\\APPL\\VLaminateApp\\PLOT\\TestShortage.plt")
#define DEF_GAUSPLOT_PATHNAME  _T("C:\\APPL\\VLaminateApp\\PLOT\\Gausian.plt")
#define DEF_LISTPLOT_PATHNAME  _T("C:\\APPL\\VLaminateApp\\PLOT\\List.plt")	
#define DEF_MINPLOT_PATHNAME   _T("C:\\APPL\\VLaminateApp\\PLOT\\Min.plt")
#define DEF_MAXPLOT_PATHNAME   _T("C:\\APPL\\VLaminateApp\\PLOT\\Max.plt")
#define DEF_RESULT_PATHNAME    _T("C:\\APPL\\VLaminateApp\\PLOT\\")
#define DEF_ETCDATA_FILE       _T("C:\\APPL\\VLaminateApp\\SETUP\\SensorLimit.txt")
// 파일 로드, 저장 관련 패스의 위치 정의

// 현재 메세지, 상태 정보를 정의
#define DEF_MSG_READY		   _T("READY")
#define DEF_MSG_IMAGE_GRAB     _T("Image Grabbing ...")
#define DEF_MSG_INSPECT        _T("On Inspection ...")
#define DEF_MSG_COMPLETE 	   _T("Inspection has been completed .")
#define DEF_MSG_UMAC_ERROR     _T("UMac. Ethenet communication open Error  !")
#define DEF_MSG_PLC_ERROR      _T("PLC. Ethenet communication open Error  !")
#define DEF_MSG_RS232_ERROR    _T("RS232C communication open Error !")
#define DEF_SOCKET_INIT_FAIL   _T(" Can not initialize windows socket ") 
// 현재 메세지, 상태 정보를 정의

// 컴파일 Option 정의 
#define _DUO_CELL                        1   // 0:카메라당 1개 CELL를 검사, 1:카메라당 2개 CELL를 검사
#define _EURESIS_CAM                     1   // 0:카메라를 사용하지 않음, 1:EURESIS 보드를 사용하여 영상을 획득
#define _UMAC                            0   // 0:UMAC통신 사용하지 않음, 1:UMAC과 연결하여 Ethernet 통신 
#define _DUO_SHORT                       0   // 0:SHORT TESTER는 하나만 사용함, 1:SHORT TESTER를 두 대 사용하여 모니터링 함
#define _AJIN_EXT                        1   // 1:AJIN_EXT DIO보드를 사용함
#define _GAUGE_RNR                       0   // 0:정상적인 실행 운영상태로 컴파일함, 1:반복정도 데이타 출력시 사용함
#define _MELSEC                          0
#define _OMRON                           0   // shsong
#define _ALIGNER_PC1                     0   // 컴파일 Option 정의 
// DCF, 영상크기 및 영상의 ZOOM 지정 

//#define CAMERA_DCF_FILE        "C:\\APPL\\NLaminateApp\\EXE\\XCL-5005_P15RG-HWTRG"
//#define CAMERA_DCF_FILE        "C:\\APPL\\VLaminateApp\\EXE\\Falcon2-12M_8bit8TAP 4096x3072_RG"
//#define CAMERA_DCF_FILE        "C:\\APPL\\VLaminateApp\\EXE\\S-25A30_P32RG"
#define CAMERA_DCF_FILE          "C:\\APPL\\VLaminateApp\\EXE\\HV-M1230C_RG_10TAP"

#define UP_IMAGE_WIDTH		  	     4080
#define UP_IMAGE_HEIGHT			     3072
#define DN_IMAGE_WIDTH		   	     4080
#define DN_IMAGE_HEIGHT			     3072

#define DEF_INIT_ZOOM_SCALE          0.21f
#define DEF_MINI_ZOOM_SCALE          0.13f
//#define DEF_INIT_ZOOM_SCALE          0.175f
//#define DEF_MINI_ZOOM_SCALE          0.10f
// DCF, 영상크기 및 영상의 ZOOM 지정 

// 응용 프로그램 화면 크기 지정 
#define APP_WIDTH                     1280
#define APP_HEIGHT                    1024
// 응용 프로그램 화면 크기 지정 

// 불량영상 저장시 불량영상의 크기 지정 
#define SMALL_IMAGE_WIDTH              932
#define SMALL_IMAGE_HEIGHT             780
// 불량영상 저장시 불량영상의 크기 지정 

// 영상출력 화면의 크기를 지정 
#define FRAME_WIDTH					   932
#define FRAME_HEIGHT				   860
// 영상출력 화면의 크기를 지정 

// 카메라의 수, 카메라 ID정의 
#define VIEW_MAX_NUM                     2
#define CAM_MAX_NUM						 2
#define CAM_UP                           0
#define CAM_DN                           1
// 카메라의 수, 카메라 ID정의 

// 검사항목의 최대 수, 항목연결 검사항목의 최대 수 정의 
#define GOM_MAX_NUM					    50
#define LINK_MAX_NUM                    50
// 검사항목의 최대 수, 항목연결 검사항목의 최대 수 정의 

#define ONE_PLOT_NUM                     3
#define LIGHT_MAX_NUM                   10
#define PLOT_MAX_NUM                    16
#define MODEL_MAX_NUM                  100
#define PLOT_MAX_DATA_NUM              100
#define PROD_MAXDATA_NUM               500
#define OFFSET_MAX_NUM                 200
#define SHORT_BUFFER_MAX_NUM            10
#define DEF_LIGHT_MAX_NUM              500
#define DEF_LIGHT_CHANNEL_NUM           10

// MESSAGE EVENT ID
#define WM_SENDDATA_PLC         (WM_USER+1001)
#define UM_SPLASH_INIT			(WM_USER+1002)
#define WM_DELETE_DATA          (WM_USER+1003)
#define WM_LEFTMOUSEDOWN        (WM_USER+1004)
// MESSAGE EVENT ID

// SPLASH 윈도우 PROCESS ID
#define ID_LOADING_FILE			 		 0
#define ID_INITIAL_CONTROL				 1
#define ID_INITIAL_REGIST				 2
#define ID_INITIAL_FINISH				 3
// SPLASH 윈도우 PROCESS ID

// TIMER ID
#define TIMER_TEMP_INSP                  0
#define TIMER_TOTAL_INSP                 3
#define TIMER_ON_PROCESS                 4
#define DEF_PROC_TIMEOUT               0.5
#define DEF_SEND_TIMEOUT              0.09

#define TIMEMSG_GRAB_CAM_UP              0
#define TIMEMSG_GRAB_CAM_DN              2
#define TIMEMSG_COMM_ERROR               3
#define TIMEMSG_PIO_INSP_CLEAR           4
#define TIMEMSG_RESET_CLEAR              5
#define TIMEMSG_CAM_UP_CLEAR             6
#define TIMEMSG_CAM_DN_CLEAR             7
#define TIMEMSG_WAIT_MOVE_DONE           8
// TIMER ID

// MAIN DIALOG 메뉴 ID 
#define DEF_BUTTON_AUTO 		         0
#define DEF_BUTTON_RESULT                1
#define DEF_BUTTON_PLOT                  2
#define DEF_BUTTON_SEARCH			     3
#define DEF_BUTTON_SETUP		         4
#define DEF_BUTTON_EXIT			         5
// MAIN DIALOG 메뉴 ID 

// 언어 선택 ID
#define DEF_LANG_KOREA                   0
#define DEF_LANG_ENGLISH                 1
#define DEF_LANG_CHANA                   2
// 언어 선택 ID

// SHORT DIALOG 메뉴 ID 
#define DEF_BUTTON_SHORT_MAIN            0
#define DEF_BUTTON_SHORT_SETUP           1
#define DEF_BUTTON_SHORT_QUERY           2
#define DEF_BUTTON_SHORT_EXIT            3
// SHORT DIALOG 메뉴 ID 

#define DEF_V1_SMALL_LEFT    		     0
#define DEF_V1_SMALL_TOP			     0
#define DEF_V1_SMALL_WIDTH			   600
#define DEF_V1_SMALL_HEIGHT			   424

#define DEF_V2_SMALL_LEFT    		     0
#define DEF_V2_SMALL_TOP			   426
#define DEF_V2_SMALL_WIDTH			   600
#define DEF_V2_SMALL_HEIGHT			   424

// MAIN DIALOG 위치 
#define DEF_DIALOG_LEFT				     2
#define DEF_DIALOG_TOP				    58
#define DEF_DIALOG_WIDTH			  1280
#define DEF_DIALOG_HEIGHT			   892
// MAIN DIALOG 위치 

// SUB BAR 위치
#define DEF_SUBBAR_LEFT		             1
#define DEF_SUBBAR_TOP	 			   952
#define DEF_SUBBAR_WIDTH			  1288
#define DEF_SUBBAR_HEIGHT			    66
// SUB BAR 위치

// MAIN BAR 위치 
#define DEF_MAINBAR_LEFT			     0
#define DEF_MAINBAR_TOP				     0
#define DEF_MAINBAR_WIDTH			  1288
#define DEF_MAINBAR_HEIGHT			    57
// MAIN BAR 위치 

// 리스트의 위치
#define DEF_LISTLOC_LEFT               944
#define DEF_LISTLOC_TOP                 41
#define DEF_LISTLOC_WIDTH              336
#define DEF_LISTLOC_HEIGHT             620
// 리스트의 위치

// TEST SHORT DIALOG 위치
#define DEF_TESTSHORT_LEFT            1280
#define DEF_TESTSHORT_TOP                0
#define DEF_TESTSHORT_WIDTH           1280
#define DEF_TESTSHORT_HEIGHT          1024
// TEST SHORT DIALOG 위치

// PLOT의 최소/최대 크기 
#define PLOT_PAGE_MAX_NUM                2
#define PLOT_MAX_NUM                    16
#define PLOT_ONE_MAX_NUM                 3
#define PLOT_MAX_DATA_NUM              100
#define PLOT_MIN_WIDTH                 625
#define PLOT_MIN_HEIGHT                105
#define PLOT_MAX_WIDTH                1260
#define PLOT_MAX_HEIGHT                845

#define PLOT_MODE_XY                     0
#define PLOT_MODE_TY                     1
#define PLOT_MODE_STICK                  2
#define PLOT_MODE_GAUSIAN                3
// PLOT의 최소/최대 크기 

#define DEF_SHORT_PLOT_NUM              56
#define DEF_GAUSE_PLOT_NUM              57
#define DEF_SHORT_GAUSE_PLOT_NUM        58
#define DEF_LIST_PLOT_NUM               59
#define DEF_SHORT_LIST_PLOT_NUM         60

#define DEF_IMAGE_MAX_NUM               20
#define DEF_IMAGE_REDUCE_RATE            4
#define DEF_QUEUE_MAX_NUM               15
#define DEF_EXPOSURE_TIME              1000

#define DEF_CAL_STEP_ANGLE             3.0f
#define DEF_LIMIT_M_RANGE_X             -20
#define DEF_LIMIT_P_RANGE_X              30
#define DEF_LIMIT_RANGE_Y                30
#define DEF_LIMIT_RANGE_T               3.0f

#define DEF_LIMIT_TAPE_RANGE_X         5.0f
#define DEF_LIMIT_TAPE_RANGE_Y         5.0f
#define DEF_LIMIT_TAPE_RANGE_T         2.0f

#define DEF_LIGHT_NUM                   10
#define DEF_REMOTE_LIGHT_COMPORT         4
#define DEF_CONT_EXIT_SETUP              0
#define DEF_CONT_LIGHT                   1
#define DEF_POSITION_CAL                 2

#define PLOT_MODE_XY                     0
#define PLOT_MODE_TY                     1
#define PLOT_MODE_STICK                  2
#define PLOT_MODE_GAUSIAN                3

#define DEF_DIO_PORTNUM                 16
#define PIO_IN_READ_RECIPE       		 0
#define PIO_IN_READ_CAM_UP  		     3
#define PIO_IN_READ_CAM_DN               5
#define PIO_IN_READ_RESET                7

#define PIO_OUT_READY_PORT               0
#define PIO_OUT_COMM_PORT                1
#define PIO_OUT_UP_END_PORT   	 	     3
#define PIO_OUT_DN_END_PORT   	 	     5
#define PIO_OUT_RESET_PORT   	 	     6
#define PIO_OUT_ERROR_PORT               7


#define DEF_IP_ADDRESS            "192.168.250.31" 

#define OMRON_IN_LOT_ID			  "PLC_Lot_ID[00]"
#define OMRON_IN_PROD_MODEL       "PLC_Prod_Model"
#define OMRON_IN_RECIPI_NUM       "PLC_Recipe_Number"
#define OMRON_IN_STOP_PORT        "PLC_Machine_Status"
#define OMRON_IN_COMM_PORT        "PLC_Comm_Status"
#define OMRON_IN_PLC_STATUS		  "PLC_STATUS[00]"
#define OMRON_OUT_PC_STATUS		  "PC_STATUS[00]"

#define OMRON_IN_UP_GRAB                 5
#define OMRON_IN_DN_GRAB                 7
#define OMRON_IN_JIG_NO                 11
#define OMRON_IN_RESET                  13

#define OMRON_OUT_READY                  0
#define OMRON_OUT_COMM                   1
#define OMRON_OUT_UP_GRAB_END            2
#define OMRON_OUT_DN_GRAB_END            3

#define OMRON_OUT_UP_X_ALIGN             4
#define OMRON_OUT_UP_Y_ALIGN             5
#define OMRON_OUT_UP_T_ALIGN             6
#define OMRON_OUT_UP_Z_ALIGN             7
#define OMRON_OUT_UP_ERROR               8
#define OMRON_OUT_UP_GO_EXEC             9

#define OMRON_OUT_DN_X_ALIGN            10
#define OMRON_OUT_DN_Y_ALIGN            11
#define OMRON_OUT_DN_T_ALIGN            12
#define OMRON_OUT_DN_Z_ALIGN            13
#define OMRON_OUT_DN_ERROR              14
#define OMRON_OUT_DN_GO_EXEC            15
#define OMRON_OUT_CAL_MODE              16

// PC<-UMAC 번지 
#define UMAC_IN_COMM                  5000
#define UMAC_IN_ERROR_CNT             5150
// PC<-UMAC 번지 

#if _ALIGNER_PC1
	#define PLC_OUT_VISION_READY    _T("D7100")
	#define PLC_OUT_ROTATE_T        _T("D7116")
	#define PLC_OUT_OFFSET_X1       _T("D7110")
	#define PLC_OUT_OFFSET_Y1       _T("D7112") 
	#define PLC_OUT_OFFSET_T1       _T("D7114") 
	#define PLC_OUT_OFFSET_X2       _T("D7116")
	#define PLC_OUT_OFFSET_Y2       _T("D7118") 
	#define PLC_OUT_OFFSET_T2       _T("D7120") 

	#define PLC_OUT_TAPE_X1         _T("D7200") 
	#define PLC_OUT_TAPE_Y1         _T("D7201") 
#else
	#define PLC_OUT_VISION_READY    _T("D7100")
	#define PLC_OUT_ROTATE_T        _T("D7117")
	#define PLC_OUT_OFFSET_X1       _T("D7130")
	#define PLC_OUT_OFFSET_Y1       _T("D7132") 
	#define PLC_OUT_OFFSET_T1       _T("D7134") 
	#define PLC_OUT_OFFSET_X2       _T("D7136")
	#define PLC_OUT_OFFSET_Y2       _T("D7138") 
	#define PLC_OUT_OFFSET_T2       _T("D7140") 

	#define PLC_OUT_TAPE_X1         _T("D7220") 
	#define PLC_OUT_TAPE_Y1         _T("D7221") 
#endif


// LIST UI ID 정의
#define LIST_MAIN_ID                     0
#define LIST_INFO_ID                     1
#define LIST_RESULT_ID                   2
#define LIST_SETUP_ID                    3
#define LIST_SHORT_ID                    4

// 수학 MACRO 정의
#define PI               3.141592653589793
#define RAD                   (PI / 180.0)
#define DEG                   (180.0 / PI)
// 수학 MACRO 정의

typedef struct
{
	float   x;
	float   y;
} POINT2;

typedef struct 
{
	TCHAR   m_strModelName[100];
	TCHAR   m_strPassWord[50];
	TCHAR   m_strImgPath[50];
	TCHAR 	m_strSelDriver[50];

	BOOL	m_bOKImageSave[CAM_MAX_NUM];
	BOOL	m_bNGImageSave[CAM_MAX_NUM];
	BOOL    m_bExecInsp[CAM_MAX_NUM];
	BOOL	m_bSaveImageType[CAM_MAX_NUM];

	long	m_nDataPeriod;
	long	m_nSaveTypeImage;
	float   m_fCamScale[CAM_MAX_NUM];

	long    m_nCamSel;
	long    m_nPortNo;
	long    m_nCOM1;
	long    m_nCOM2;
	long    m_nSelLanguage;
	long    m_nSpace[18];
	long    m_OrderNum[MODEL_MAX_NUM];
	TCHAR   m_ModelName[MODEL_MAX_NUM][100];

	BOOL    m_bExecCutOffset;
	BOOL    m_bExecEPCOffset;
	BOOL    m_bExecT2BOffset;
	BOOL    m_bExecDataTrans;
	BOOL    m_bRecipeOption;

	long    m_nSkipCnt;
	long    m_nDaqUnitCnt;
	long    m_nTotalShortCount;

	float   m_fTestVolt;
	float   m_fLimitCurr;
	float   m_fStartDelayTime;
	float   m_fRampingTime;
	float   m_fHoldingTime;
	float   m_fContactCurr;

	long    m_nOldHour;
	long    m_nSelUpDn;
	long    m_nLightV[DEF_LIGHT_MAX_NUM];
	long    m_nExposureTime[CAM_MAX_NUM];

	SYSTEMTIME  m_sysOldTime;
	long      m_nRetryNum;

	BOOL    m_bSWGrab;
	long    m_nDelayTime;

	float     m_fCoordiX;
	float     m_fCoordiY;
	float     m_fCamX[2];
	float     m_fCamY[2];
		
} SETTING_DATA;

typedef struct 
{
	long      m_nTotalInspCount[CAM_MAX_NUM];
	long      m_nTotalGoodCount[CAM_MAX_NUM];
	long      m_nTotalBadCount[CAM_MAX_NUM];
	long      m_nUpBadCount;
	long      m_nDnBadCount;
	long      m_nUpFBadCount;
	long      m_nUpRBadCount;
	long      m_nDnFBadCount;
	long      m_nDnRBadCount;
	long      m_CntROI[CAM_MAX_NUM][GOM_MAX_NUM];
	long      m_CntLink[CAM_MAX_NUM][LINK_MAX_NUM];
	long      m_nPriorityNum;

	float     m_fCellWidth;
	float     m_fCellHeight;

	float     m_fTapePosX[20];
	float     m_fTapePosY[20];
	long      m_nSpace[18];

	float     m_fLimitMinusX;
	float     m_fLimitPlusX;
	float     m_fLimitMinusY;
	float     m_fLimitPlusY;
	float     m_fLimitMinusT;
	float     m_fLimitPlusT;

	float     m_fOffsetX[2];
	float     m_fOffsetY[2];
	float     m_fOffsetT[2];
} ETC_DATA;

typedef struct
{
	long    m_nTotalCnt;
	long    m_nIndexPnt;
	long    m_InspType[CAM_MAX_NUM][GOM_MAX_NUM];
	BOOL   *m_bDec[CAM_MAX_NUM][GOM_MAX_NUM];
	float  *m_fValue[CAM_MAX_NUM][GOM_MAX_NUM];
	float  *m_fValueS[CAM_MAX_NUM][GOM_MAX_NUM];
} PRODUCT_DATA;

typedef struct
{
	TCHAR   m_strModelName[100];
	TCHAR   m_strLotID[100];

	BOOL    m_bCam1;
	BOOL    m_bCam2;
	BOOL    m_bGood;
	BOOL    m_bBad;
	BOOL    m_bPeriod;
	BOOL    m_bModel;
	BOOL    m_bInspItem;

	long 	m_tmFromYY;
	long 	m_tmFromMM;
	long 	m_tmFromDD;
	long 	m_tmFromTT;
	long    m_tmFromMT;
	long 	m_tmToYY;
	long    m_tmToMM;
	long 	m_tmToDD;
	long 	m_tmToTT;
	long    m_tmToMT;
} SEARCH_DATA;

typedef struct
{
	long    m_nCamID;
	BOOL    m_bDecision;
	BOOL    m_bStatus;
	BOOL    m_bFDecision;
	BOOL    m_bRDecision;
	BOOL    m_bFDataDec;
	BOOL    m_bRDataDec;

	BOOL    m_bSensorDecF;
	BOOL    m_bSensorDecR;

	float   m_dSensorDataF;
	float   m_dSensorDataR;
	float   m_dOffsetEPC;

	TCHAR   m_strTitle[GOM_MAX_NUM][100];
	BOOL    m_bDecisionROI[GOM_MAX_NUM];
	BOOL    m_bLocROI[GOM_MAX_NUM];
	BOOL    m_bExecROI[GOM_MAX_NUM];
	BOOL    m_bEjectROI[GOM_MAX_NUM];
	long    m_nPlotNum[GOM_MAX_NUM];
    long    m_nInspType[GOM_MAX_NUM];

	float   m_dPosRoiX1[GOM_MAX_NUM];
	float   m_dPosRoiY1[GOM_MAX_NUM];
	float   m_dPosRoiX2[GOM_MAX_NUM];
	float   m_dPosRoiY2[GOM_MAX_NUM];

	float   m_dValue[GOM_MAX_NUM];
	float   m_dLowLimit[GOM_MAX_NUM];
	float   m_dHighLimit[GOM_MAX_NUM];
	float   m_dStdSpec[GOM_MAX_NUM];
	float   m_dToleSpec[GOM_MAX_NUM];
	float   m_dRefDiff[GOM_MAX_NUM];

	long    m_nLinkNum;
	TCHAR   m_strLinkTitle[LINK_MAX_NUM][100];
	BOOL    m_bLinkDecROI[LINK_MAX_NUM];
	BOOL    m_bLinkExecROI[LINK_MAX_NUM];
	BOOL    m_bLinkEjectROI[LINK_MAX_NUM];
	BOOL    m_bLinkLocROI[LINK_MAX_NUM];
	long    m_nLinkPlotPos[LINK_MAX_NUM];
	long    m_nLinkPlotNum[LINK_MAX_NUM];
    long    m_nLinkType[LINK_MAX_NUM];
	float   m_dLinkValue[LINK_MAX_NUM];
	float   m_dLinkValueS[LINK_MAX_NUM];
	float   m_dLinkLowLimit[LINK_MAX_NUM];
	float   m_dLinkHighLimit[LINK_MAX_NUM];
	float   m_dLinkStdSpec[LINK_MAX_NUM];
	float   m_dLinkToleSpec[LINK_MAX_NUM];
	float   m_dLinkRefDiff[LINK_MAX_NUM];

	float   m_dSourX;
	float   m_dSourY;
	float   m_dSourT;

	float   m_dCX;
	float   m_dCY;
	float   m_dCT;

	float   m_dCorPx[4];
	float   m_dCorPy[4];
	float   m_dCenPx;
	float   m_dCenPy;

	float   m_dPixelSX;
	float   m_dPixelSY;
	float   m_dPixelCX;
	float   m_dPixelCY;

	float   m_fAlignX;
	float   m_fAlignY;
	float   m_fAlignT;

	float   m_fVerifyX;
	float   m_fVerifyY;
	float   m_fVerifyT;

	float   m_fPosRoiX1[GOM_MAX_NUM];
	float   m_fPosRoiY1[GOM_MAX_NUM];
	float   m_fPosRoiX2[GOM_MAX_NUM];
	float   m_fPosRoiY2[GOM_MAX_NUM];
} INSP_RESULT_DATA;

typedef struct 
{
	float   m_A1;
	float   m_A2;
	float   m_A3;
	float   m_A4;
	float   m_T2B;
	float   m_TE;
} OFFSET_DATA;

typedef struct
{
	long    m_nImgHeadPnt;
	long    m_nImgTailPnt;

	long    m_nWidth;
	long    m_nHeight;

	long    m_nImgIndex;
	long    m_ImgIndexP[DEF_QUEUE_MAX_NUM];
	long    m_ImgCam[DEF_QUEUE_MAX_NUM];
    long    m_nBadIndex[CAM_MAX_NUM];

	long    m_nMelHeadPnt;
	long    m_nMelTailPnt;

	INSP_RESULT_DATA m_ImgRes[DEF_QUEUE_MAX_NUM];
	INSP_RESULT_DATA m_BadRes[CAM_MAX_NUM][DEF_QUEUE_MAX_NUM];
	INSP_RESULT_DATA m_MelRes[CAM_MAX_NUM][DEF_QUEUE_MAX_NUM];
} REALTIME_DATA;

///////////////////////////  REAL TIME DATA 관련 구조체  ////////////////////////////
typedef struct
{
	long		 m_nAlignPosCnt;         // Brightness Value 지정
	
	POINT2       m_ptAlignPos[10];
	POINT2       m_ptAlignCen;
	POINT2       m_ptPosCAL;
	POINT2       m_ptRotAxis;
	float        m_fAngle;

	float        m_fOffsetX;
	float        m_fOffsetY;

	BOOL         m_bResultDec;
	BOOL         m_bPosCAL;
} REALTIME_TEMP;

// INSPECT TYPE 정의
typedef enum
{
	INSP_MEASURE_SIDE_ONE=400,          // 일 방향 EDGE 측정(프로젝트 방법)
	INSP_MEASURE_SIDE_TWO,              // 두 방향 EDGE 측정(프로젝트 방법)
	INSP_SLINE,                         // 직선성 검사
	INSP_STRAIGHTNESS,            
	INSP_DATUM_C,
	INSP_DATUM_B,
	INSP_DEFECT_VARI,                   // Defect 바리 검사
	INSP_MARK,
	INSP_PATTERN_MATCH,                 // 패턴 매칭 
	INSP_CAMERA_CAL,
	INSP_BLOB_ANALYSIS,                 // BLOB 해석 
	INSP_MEASURE_CIRCLE,                // 원형 검사
	INSP_BLOB_CIRCLE,
	INSP_DEFECT_NORMAL,                 // Defect 검사 Type  
	INSP_DEFECT_CIRCLE,
	INSP_GRAY_VALUE,                    // Gray Value값 검사 Type 
	INSP_SHARPNESS_VALUE,               // Sharpness Value
	INSP_IS_OBJECT,
	INSP_SCALE_FACTOR
};
// INSPECT TYPE 정의

// TEACHING TYPE 정의
typedef enum
{
	TEACH_NONE = 500,                  // 검사 형태가 없음  
	TEACH_DIST_PNT2PNT,                // 두 점의 거리 측정 TYPE
	TEACH_DIST_PNT2LINE,
	TEACH_DIST_LINE2LINE,              // 두 라인의 거리 측정 TYPE
	TEACH_SLINE_RIGHT_ANGLE,
	TEACH_SLINE_PARALLEL,
	TEACH_SLINE_SURFACE,
	TEACH_DIFF_GAP2GAP,
	TEACH_DIST_CIRCLE2LINE,
	TEACH_ANG_PNT2PNT,                 // 두 점의 각도 측정 Type  
	TEACH_ANG_LINE4PNT,                // 4점으로 만드는 2라인의 각도 측정 Type 
	TEACH_CROSS_POINT,
	TEACH_ANG_LINE2LINE,               // 두 라인의 각도 측정 TYPE
	TEACH_ANG_3PNT,                    // 세 점의 각도 측정 TYPE
	TEACH_DIFF_PNT2PNT,                // 두 점의 OFFSET 측정 TYPE 
	TEACH_CIR_ROI2ROI
};
// TEACHING TYPE 정의 

typedef struct
{
	char   m_sLogTitle1[50];
	char   m_sLogTitle2[50];
	char   m_sLogTitle3[50];
	char   m_sLogTitle4[50];
	char   m_sLogTitle5[50];
	char   m_sLogTitle6[50];
	char   m_sLogTitle7[50];
	char   m_sLogTitle8[50];
	char   m_sLogTitle9[50];
	char   m_sLogTitle10[50];
	char   m_sLogTitle11[50];
} LOG_LIST;
// INSPECT TYPE 정의

typedef struct
{
    long    m_nCamID;
	long    m_nPosX;
	long    m_nPosY;
	long    m_nGrayLevel;
	POINT2  m_ptPosCAL;

	BOOL    m_bPosCAL;
	BOOL    m_bModify;
	BOOL    m_bContNo;
	long    m_nCh;
	long    m_nExposureTime;
	long    m_LightV[DEF_LIGHT_NUM];
} NAppDll_CallDATA;

typedef struct
{
    long    m_nPlotID;
	long    m_nPosX;
	long    m_nPosY;
	long    m_nGrayLevel;
} NPlotDll_CallDATA;

#endif __AppDefine_H__
