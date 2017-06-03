#if !defined(_DEFINE_H)
#define _DEFINE_H

#define VISION_SETUP_VERSION         _T(" NVisionSetup VER 3.0(15.5.15) ")

#define CAL_UP_FILE_PATH             _T("C:\\APPL\\VLaminateApp\\SETUP\\CAL_UP.dat")
#define CAL_DN_FILE_PATH             _T("C:\\APPL\\VLaminateApp\\SETUP\\CAL_DN.dat")
#define SYS_UP_FILE_PATH             _T("C:\\APPL\\VLaminateApp\\SETUP\\SYS_UP.dat")
#define SYS_DN_FILE_PATH             _T("C:\\APPL\\VLaminateApp\\SETUP\\SYS_DN.dat")
#define OFFSET_DATA_FILE             _T("C:\\APPL\\VLaminateApp\\SETUP\\ConfigData.txt")

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////              ���� ��� ����            /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#define _DUO_CELL                      1

#define DEF_INIT_CAMUP_SCALE         0.23f
#define DEF_INIT_CAMDN_SCALE         0.14f

//#define DEF_INIT_CAMUP_SCALE       0.175f
//#define DEF_INIT_CAMDN_SCALE       0.10f

#define DEF_TILTING_ALG                1
#define DEF_TURNING_VALUE           0.03f

#define IMAGE_WIDTH				     640
#define IMAGE_HEIGHT			     480
#define FRAME_WIDTH				     956
#define FRAME_HEIGHT			     850

#define IMAGE_BAND                     1
#define IMAGE_DEPTH                    8

#define IMAGE_LEVEL                  256
#define BLACK_LEVEL                    0
#define MEDIUM_LEVEL                 128
#define WHITE_LEVEL                  255
#define MAXIMUM_ANGLE                360


///////////////////////////// ���� �Լ� ����
#define PI             3.141592653589793
#define RAD                 (PI / 180.0)
#define DEG                 (180.0 / PI)
///////////////////////////// ���� �Լ� ����

#define NONE_TYPE                      0
#define SPARE_MAX_NUM                 10
#define GOM_MAX_NUM                   50
#define EDGE_MAX_NUM                5000
#define TEMP_MAX_NUM                5120
#define MAX_PATTERN_NUM               10
#define LOCATE_MAX_NUM                20
#define PROCESS_MAX_NUM               20
#define LINK_MAX_NUM                  50
#define DEF_LIGHT_NUM                 10
#define DEF_LIGHT_VALUE              200
#define CAM_MAX_NUM                    4
#define DEFECT_RECT_NUM               20

#define DEF_CONT_EXIT_SETUP            0
#define DEF_CONT_LIGHT                 1
#define DEF_POSITION_CAL               2

typedef struct 
{
	float x;
	float y;
} POINT2;
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////              ���� ��� ����            /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////
//////////////////////          ���� �˻� ����ü ����         /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////            ���� ���� ����              /////////////////////////
#define RECKER_CENTER                 1
#define RECKER_LEFT_TOP               2
#define RECKER_RIGHT_TOP              3
#define RECKER_LEFT_BOT               4
#define RECKER_RIGHT_BOT              5
#define RECKER_MID_TOP                6
#define RECKER_MID_LEFT               7
#define RECKER_MID_RIGHT              8
#define RECKER_MID_BOT                9

// Overlay Type ����
typedef enum 
{
	GOM_LINE=200,
	GOM_RECT,
	GOM_CIRCLE,
	GOM_CROSS,
	GOM_TEXT,
	GOM_CIRCLE2,
	GOM_MASK_RECT
};
// Overlay Type ����

// Edge Detect ��� ����
typedef enum
{
    SCAN_PROJECT_FT = 300,              // Projection Profile ��ȣ  
	SCAN_PROJECT_DFDT,                  // Projection Profile ��ȣ �̺� F'(t)
	SCAN_PROFILE_FT,                    // Profile ��ȣ
	SCAN_PROFILE_DFDT                   // Profile ��ȣ F'(t)
};
// Edge Detect ��� ����

// INSPECT TYPE ����
typedef enum
{
	INSP_MEASURE_SIDE_ONE=400,          // �� ���� EDGE ����(������Ʈ ���)
	INSP_MEASURE_SIDE_TWO,              // �� ���� EDGE ����(������Ʈ ���)
	INSP_SLINE,                         // ������ �˻�
	INSP_STRAIGHTNESS,            
	INSP_DATUM_C,
	INSP_DATUM_B,
	INSP_DEFECT_VARI,                   // Defect �ٸ� �˻�
	INSP_MARK,
	INSP_PATTERN_MATCH,                 // ���� ��Ī 
	INSP_CAMERA_CAL,
	INSP_BLOB_ANALYSIS,                 // BLOB �ؼ� 
	INSP_MEASURE_CIRCLE,                // ���� �˻�
	INSP_BLOB_CIRCLE,
	INSP_DEFECT_NORMAL,                 // Defect �˻� Type  
	INSP_TAB_FOLDING,
	INSP_GRAY_VALUE,                    // Gray Value�� �˻� Type 
	INSP_SHARP_VALUE,               // Sharpness Value
	INSP_IS_OBJECT,
	INSP_SCALE_FACTOR
};
// INSPECT TYPE ����

// TEACHING TYPE ����
typedef enum
{
	TEACH_NONE = 500,                  // �˻� ���°� ����  
	TEACH_DIST_PNT2PNT,                // �� ���� �Ÿ� ���� TYPE
	TEACH_DIST_PNT2LINE,
	TEACH_DIST_LINE2LINE,              // �� ������ �Ÿ� ���� TYPE
	TEACH_SLINE_RIGHT_ANGLE,
	TEACH_SLINE_PARALLEL,
	TEACH_SLINE_SURFACE,
	TEACH_DIFF_GAP2GAP,
	TEACH_DIST_CIRCLE2LINE,
	TEACH_ANG_PNT2PNT,                 // �� ���� ���� ���� Type  
	TEACH_ANG_LINE4PNT,                // 4������ ����� 2������ ���� ���� Type 
	TEACH_CROSS_POINT,
	TEACH_ANG_LINE2LINE,               // �� ������ ���� ���� TYPE
	TEACH_ANG_3PNT,                    // �� ���� ���� ���� TYPE
	TEACH_DIFF_PNT2PNT,                // �� ���� OFFSET ���� TYPE 
	TEACH_CIR_ROI2ROI
};
// TEACHING TYPE ���� 

// Dir Type ����
typedef enum
{
	DIR_LEFT_RIGHT=600,			    	// ������� Left->Right ��ĵ ����  
	DIR_RIGHT_LEFT,						// ������� Right->Left ��ĵ ����  
	DIR_TOP_BOTTOM,						// ������� TOP->BOTTOM ��ĵ ����
	DIR_BOTTOM_TOP,						// ������� BOTTOM->TOP ��ĵ ����
	DIR_INSIDE_OUTSIDE,                 // ������� ����->�ܺ� ��ĵ ����(��)
	DIR_OUTSIDE_INSIDE                  // ������� �ܺ�->���� ��ĵ ����(��)
};
// Dir Type ����

// POLARITY Type ����
typedef enum
{
	POL_WHITE=700,						// ���
	POL_BLACK,							// ������
	POL_WHITE_BLACK,					// ������� ���������� ���� ���� �˻�
	POL_BLACK_WHITE 					// ���������� ������� ���� ���� �˻�
};
// POLARITY Type ����

// Edge Detect ��� ����
typedef enum
{
    MORPH_ERODE = 900,              // Projection Profile ��ȣ  
	MORPH_DILATE,                   // Projection Profile ��ȣ �̺� F'(t)
	MORPH_OPEN,                     // Profile ��ȣ 
	MORPH_CLOSE                     // Profile ��ȣ F'(t) 
};
// Edge Detect ��� ����

///////////////////////////  REAL TIME DATA ���� ����ü  ////////////////////////////
typedef struct
{
	long		 m_nAlignPosCnt;         // Brightness Value ����
	
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
/////////////////////////////  SYSTEM DATA ���� ����ü //////////////////////////////
/////////////////////////////  SYSTEM DATA ���� ����ü //////////////////////////////
typedef struct
{
	long		 m_nExposureTime;         // Brightness Value ����
	long         m_LightV[DEF_LIGHT_NUM];              // ���� 

	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} SYSTEM_DATA;
/////////////////////////////  SYSTEM DATA ���� ����ü //////////////////////////////

////////////////////////////// Overlay ROI ���� ����ü //////////////////////////////
typedef struct 
{
	TCHAR        m_strTitle[100];       // ROI Label Title 

	long         m_nGOMType;            // OBJECT TYPE (��, ����, �簢, ��, Ÿ�� ��) 
	long         m_nThick;              // OBJECT �β� 

	BOOL         m_bExecShowROI;        // ������ �׸��� ���̰� �� 
	BOOL         m_bEjectROI;          
	BOOL         m_bShowROI;            // OBJECT�׸���(Show/Hide)
	BOOL         m_bShowTitle;          // OBJECT TITLE(Show/Hide)
	BOOL         m_bSelectedROI;        // ���콺�� ������ OBJECT(ON/OFF)

	POINT2       m_ptPoint1;            // ������ ù��° ��ǥ 
	POINT2       m_ptPoint2;            // ������ ù��° ��ǥ 
	POINT2       m_ptPoint3;            // ������ ù��° ��ǥ 
	POINT2       m_ptPoint4;            // ������ �ι�° ��ǥ 

	POINT2       m_ptStart[SPARE_MAX_NUM];// ������ ù��° ��ǥ 
	POINT2       m_ptEnd[SPARE_MAX_NUM];  // ������ �ι�° ��ǥ 

	float        m_dCX;                 // ������ �߽� ��ǥ(X)
	float        m_dCY;                 // ������ �߽� ��ǥ(Y)
	float        m_dAngle;              // ������ ���� 
	float		 m_dInRadius;           // ���� ������
	float        m_dOutRadius;          // ���� ������ 

	COLORREF	 m_nNormalColor;        // ���� ������ OBJECT COLOR
	COLORREF	 m_nSelectedColor;      // ���õ� ������ OBJECT COLOR
	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} GOM_ATTR;
////////////////////////////// Overlay ROI ���� ����ü //////////////////////////////

///////////////////////////   SCAN EDGE ����Ÿ ���� ����ü   //////////////////////////
typedef struct
{
	long         m_nScanType;           // EDGE DETECTIN TYPE

	long         m_nSlope;
	long		 m_nFirstThresh;        // First Threshold ����ġ
	long		 m_nSecondThresh;       // Second Threshold ����ġ

    long		 m_nFirstScanDirType;   // ��ĵ ���� ����
	long		 m_nSecondScanDirType;  // ��ĵ ���� ����
	long		 m_nFirstPolDirType;    // First Polarity ���� ����
	long		 m_nSecondPolDirType;   // Second Polarity ���� ����

	float        m_dFirstA;
	float        m_dFirstB;
	float        m_dFirstT;

	float        m_dSecondA;
	float        m_dSecondB;
	float        m_dSecondT;

	long         m_nFirstNum;                 // SCAN EDGE ����Ÿ �� 
	POINT2       m_ptFirstMin;
	POINT2       m_ptFirstMax;
	bool         m_bFirstDec[EDGE_MAX_NUM];
	float		 m_dFirstPX[EDGE_MAX_NUM];    // ����Ÿ X ��ġ
	float		 m_dFirstPY[EDGE_MAX_NUM];    // ����Ÿ Y ��ġ 

	long         m_nSecondNum;                // SCAN EDGE ����Ÿ �� 
	POINT2       m_ptSecondMin;
	POINT2       m_ptSecondMax;
	bool         m_bSecondDec[EDGE_MAX_NUM];
	float		 m_dSecondPX[EDGE_MAX_NUM];                 // ����Ÿ X ��ġ
	float		 m_dSecondPY[EDGE_MAX_NUM];                 // ����Ÿ Y ��ġ 

	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} SCAN_ATTR;
//////////////////////////   SCAN EDGE ����Ÿ ���� ����ü  //////////////////////////

//////////////////////////   TEACHING ����Ÿ ���� ����ü   //////////////////////////
typedef struct
{
	TCHAR        m_strTitle[100];       // �˻� Label Title
	BOOL         m_bDecision;

	BOOL         m_bSel1;               // 0:Inspect Item 1:
	BOOL         m_bSel2;
	BOOL         m_bLinkExec;
	BOOL         m_bEjectROI;

	long         m_nLinkType;
    long         m_nIndex1;
	long         m_nIndex2;
	long         m_nPlotPos;
	long         m_nPlotSelNum;

	float        m_dCX;
	float        m_dCY;
	float        m_dValue;
	float        m_dValueS;

	float		 m_dStandLowLimit;       // ���ذ� Low  Limit(SETUP)
	float		 m_dStandHighLimit;      // ���ذ� High Limit(SETUP)
	float        m_dStdSpec;
	float        m_dToleSpec;
	float        m_dOffsetV;
	float        m_dValueA;
	
	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} TEACH_ATTR;
//////////////////////////   TEACHING ����Ÿ ���� ����ü   //////////////////////////

/////////////////////////  CALIBRATION ����Ÿ ���� ����ü  //////////////////////////
typedef struct
{
	float        m_dScaleFactor;
	float        m_dScaleLT_X;
	float        m_dScaleLB_X;
	float        m_dScaleRT_X;
	float        m_dScaleRB_X;
	float        m_dScaleL_Y;
	float        m_dScaleR_Y;

	float        m_dRealT_X;
	float        m_dRealB_X;
	float        m_dReal_Y;
	float        m_dScaleX;             // ������ Scale Factor X  
	float        m_dScaleY;             // ������ Scale Factor Y

	double       m_dFacA;
	double       m_dFacB;
	double       m_dFacC;
	long         m_nAverBright;
} CAL_DATA;
/////////////////////////  CALIBRATION ����Ÿ ���� ����ü  //////////////////////////

/////////////////////////////// Ķ���극�̼�  ����ü  ///////////////////////////////
typedef struct
{
	long		 m_nOldMouseX;       
	long		 m_nOldMouseY;  

	float        m_fZoomScale;          // ������ Scale Factor 
	CPoint       m_ptMouse;
	CRect        m_fmRect;
} IMAGE_DATA;
///////////////////////////// Ķ���극�̼�  ����ü ////////////////////////////////////

/////////////////////////////  Enhencement ���� ����ü //////////////////////////////
typedef struct
{
	BOOL		 m_bUseEnhence;         // Mapping ���� ����

	long		 m_nContrastLow;        // Low Position ����
	long		 m_nContrastHigh;       // High Position ����
	long		 m_nBrightness;         // Brightness Value ����
	long         m_LightV[DEF_LIGHT_NUM];              // ���� 
	long		 m_nLUT[IMAGE_LEVEL];   // Contrast ����

	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} ENHENCE_DATA;
/////////////////////////////  Enhencement ���� ����ü //////////////////////////////

///////////////////////////////   �˻� ���� ����ü  /////////////////////////////////
typedef struct
{
	BOOL		 m_bInspMode;           // �ش� ROI �˻�
	BOOL		 m_bDecision;           // �ش� ROI ���� (�����) : TRUE, FALSE / -1 : ������� ����
	BOOL         m_bLocROI;
	BOOL         m_bScanMethod;

    long		 m_nInspectType;        // �˻� ����

	BOOL         m_bOverlapRate;
	BOOL         m_bBlobFeret;          // 0 : ���� �˻縸      1 : Felet �˻�   
	long         m_nBlobOption;         // 0 : ������ �ִ� ���� 1 : ���� ����  
	long         m_nBlobThresh;         // ����ġ 
	float		 m_dBlobMinArea;        // Blob�� �ּҸ���
	long		 m_nBlobPolarity;       // Blob�� ���� POL

	long         m_nPatScore;           // ���� ��Ī�� 
	long         m_nPatSearchRate;      // ������ ������� ��ġ ������ ��� ����  
	CRect		 m_rcPatSearchROI;      // Search Area

	long         m_nPatWidth;           // ���� ���� ������ 
	long         m_nPatHeight;          // ���� ���� ������ 
	long         m_nPlotPos;            // "1" ��, "2" ��
	long         m_nPlotSelNum;
	long         m_nSampleNum;
	long         m_nPatNum;             // ���� �� 
	LPBYTE		 m_pPat[MAX_PATTERN_NUM]; // Pattern ���� 

	float        m_dAnyValue;           // ���� ����� 
	float        m_dTeachValue;         // �˻� �׸� ���� ���尪  
	float        m_dSensity;            // ���� ���� �� 
	float		 m_dSumV;        // Blob�� �ִ����

	float		 m_dStandLowLimit;      // ���ذ� Low  Limit(SETUP)
	float		 m_dStandHighLimit;     // ���ذ� High Limit(SETUP)
	float        m_dStdSpec;
	float        m_dToleSpec;
	float        m_dOffsetV;            // Object Y �߽� 
	
	float        m_dCX;                 // Object X �߽� 
	float        m_dCY;                 // Object Y �߽� 
	float        m_dRadius;

	POINT2		 m_ptFirstMeas1;        // First  ���� ��ġ ���� 1
	POINT2       m_ptFirstMeas2;        // First  ���� ��ġ ���� 2
	POINT2		 m_ptSecondMeas1;       // Second ���� ��ġ ���� 1
	POINT2		 m_ptSecondMeas2;       // Second ���� ��ġ ���� 2

	long         m_nDefectCnt;
	CRect        m_rcPosFind[DEFECT_RECT_NUM];
	
	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} INSPECT_DATA;
///////////////////////////////   �˻� ���� ����ü     ///////////////////////////////

///////////////////////////////   TEACH ���� ����ü    ///////////////////////////////
typedef struct
{
	long         m_nPreNum;
	long         m_nLocNum;
	long         m_nCamNum;
	long         m_nLinkNum;

	BOOL         m_bPreDecision;
	BOOL         m_bLocDecision;
	BOOL         m_bLinkDecision;
	BOOL         m_bEntire;
	BOOL         m_bDispAllROI;

    TEACH_ATTR   m_PRE[PROCESS_MAX_NUM];
	TEACH_ATTR   m_LOC[LOCATE_MAX_NUM];
	TEACH_ATTR   m_CAM[CAM_MAX_NUM];
	TEACH_ATTR   m_LINK[LINK_MAX_NUM];

	float        m_fOffsetX;
	float        m_fOffsetY;
	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} TEACH_DATA;
///////////////////////////////   TEACH ���� ����ü    ///////////////////////////////

typedef struct
{
	TCHAR		 m_strTitle[100];       // �˻���� Ÿ��Ʋ (SETUP)
	BOOL		 m_bRealTimeMode;       // ���� �˻� ��� (�˻�� ����)
	BOOL         m_bSetupMode;          // Setup�� ����  
    BOOL		 m_bDecision;           // ��� ����

   	float		 m_dSourX;              // ��ġ ������ ���� X��ǥ(ƼĪ�� �ڵ� ����)
	float		 m_dSourY;              // ��ġ ������ ���� Y��ǥ(ƼĪ�� �ڵ� ����)
	float		 m_dSourT;              // ���� ������ ���� T����(ƼĪ�� �ڵ� ����)

	float		 m_dDeltaX;             // ��ġ ������ ���� X Offset(�˻�� ����)
	float		 m_dDeltaY;             // ��ġ ������ ���� Y Offset(�˻�� ����)
	float		 m_dDeltaT;             // ���� ������ ���� T Offset(�˻�� ����)

	long		 m_nImageWidth;         // ��������(Width)
	long		 m_nImageHeight;        // ��������(Height)
	long         m_nSpare[SPARE_MAX_NUM];

   	float		 m_dSourX2;             // ��ġ ������ ���� X��ǥ(ƼĪ�� �ڵ� ����)
	float		 m_dSourY2;             // ��ġ ������ ���� Y��ǥ(ƼĪ�� �ڵ� ����)
	float		 m_dSourT2;             // ���� ������ ���� T����(ƼĪ�� �ڵ� ����)

	float		 m_dDeltaX2;            // ��ġ ������ ���� X Offset(�˻�� ����)
	float		 m_dDeltaY2;            // ��ġ ������ ���� Y Offset(�˻�� ����)
	float		 m_dDeltaT2;            // ���� ������ ���� T Offset(�˻�� ����)

	float        m_fSpare[4];

	SCAN_ATTR    m_Scan[GOM_MAX_NUM];
	GOM_ATTR     m_GOM[GOM_MAX_NUM];
	ENHENCE_DATA m_Enhence;             // ���� ���� ����ü
	IMAGE_DATA   m_IMG;
    INSPECT_DATA m_INSP[GOM_MAX_NUM];   // �˻� ����ü
	CAL_DATA     m_CAL;
	TEACH_DATA   m_Teach;
} SETUP_MODEL_DATA;

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
} NAppDll_DATA;
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////          ���� �˻� ����ü ����         /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#endif 