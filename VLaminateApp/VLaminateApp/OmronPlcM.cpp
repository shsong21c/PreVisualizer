// OmronPlcM.cpp : implementation file
//

#include "stdafx.h"
#include "VLaminateApp.h"
#include "OmronPlcM.h"

// COmronPlcM

IMPLEMENT_DYNAMIC(COmronPlcM, CWnd)

COmronPlcM::COmronPlcM()
{
	m_bBusy = FALSE;
}

COmronPlcM::~COmronPlcM()
{
	
}


BEGIN_MESSAGE_MAP(COmronPlcM, CWnd)
END_MESSAGE_MAP()

BOOL COmronPlcM::OpenPLC()
{
	BOOL bRet=FALSE;
	DWORD dwIPAddress; 

	dwIPAddress = inet_addr(DEF_IP_ADDRESS);

#if _OMRON
	CIPNetworkPathInfo networkPathInfo;		// EPATH
	CIPPortSegmentInfo portSeg[2];			// Port Segment
	CIPNetworkPath networkPath;
	BYTE buff[100];

	int					messageType;		// UCMM or CLASS3
	BYTE				generalStatus;		// resoponce status
	CIPAdditionalStatus	additionalStatus;	// additional staus
	BYTE				remainingPathSize;	// reserved
	DWORD				timeoutMilisec;		// timeout
	
	CIPAPP_STARTUP();

	m_hAppIP = CIPApp_new(_T("HALF_LOADER"));
	if (m_hAppIP)
	{
		// construct NetworkPath
		portSeg[0].portID = 2;		// Ethernet port
		portSeg[0].nodeID = dwIPAddress;	// Peer IP Address
		portSeg[1].portID = 1;		// Backplane Port
		portSeg[1].nodeID = 0;		// CPU Unit

		networkPathInfo.portSegment = portSeg;
		networkPathInfo.segmentCount = 2;
		networkPath.path = buff;
		networkPath.byteSize = sizeof(buff);
		networkPath.byteSize = CIPUtil_constructNetworkPath(&networkPathInfo, &networkPath);

		// conecct to device
		messageType = CIPExplicitType_UCMM;
		timeoutMilisec = 1000;
		m_hConnectIP = CIPApp_openConnectionExplicit(m_hAppIP, messageType, &networkPath, &generalStatus, 
			                                                                &additionalStatus, &remainingPathSize, timeoutMilisec, 0);
		if (m_hConnectIP)
			bRet = TRUE;
		Sleep(10);
	}
#endif
	
	return bRet;
}

void COmronPlcM::ClosePLC()
{
#if _OMRON
	BYTE generalStatus;		                // resoponce status
	CIPAdditionalStatus	additionalStatus;	// additional staus

	CIPApp_closeConnectionExplicit(m_hAppIP, m_hConnectIP, &generalStatus, &additionalStatus, NULL);
	CIPApp_delete(m_hAppIP);
#endif
}

//PLC ����
BOOL COmronPlcM::SendMessageToPLC(char *strTagName, DWORD nMsgID, int nSize, int *szCommBuff)
{
	BOOL bRet = FALSE;

	m_bBusy = TRUE;

#if _OMRON
	CIPRequestPath 				requestPath = {0};		// Request path
	CIPRequestData 				requestData = {0};		// Request header
	CIPExplicitRequestHeader*	cipHeader;				// Rqquest message buffer
	DWORD						requestPathSize;
	DWORD						requestDataByteSize;
	DWORD	recvMessageID;			// receive messageID
	DWORD	buffSize;				// receive buffer size
	DWORD	timeout = 1000;			// time out(ms)
	DWORD	recMaxSize;				// this value always returns as 0
	int	recDataSize;	

	int i;
	BYTE	recvBuff[256] = {0};	// receive buffer
	BYTE serviceCode = 0x4D;
	int classID = 1;
	int instanceID = 1;
	int attributeID = 0;
	int sendCount = 1;
	int ac = 0;
	
	OpenPLC();

	cipHeader = ( CIPExplicitRequestHeader* )(unsigned char *)recvBuff;
	requestPath.path 	 = (BYTE *)( cipHeader + 1 );
	requestPath.byteSize = sizeof( recvBuff ) - sizeof(CIPExplicitRequestHeader);	
	requestPathSize = CIPUtil_constructRequestPathWithTagName((LPCTSTR)strTagName, &requestPath);

	cipHeader = ( CIPExplicitRequestHeader* )(unsigned char *)recvBuff;
	cipHeader->serviceCode = serviceCode;
	cipHeader->requestPathSize = (BYTE)requestPathSize / sizeof(WORD);
	requestDataByteSize = sizeof(CIPExplicitRequestHeader) + requestPathSize;

	recvBuff[requestDataByteSize] = 0xD2;
	recvBuff[requestDataByteSize+1] = 0x00;
	recvBuff[requestDataByteSize + 2] = (unsigned char)nSize;
	recvBuff[requestDataByteSize + 3] = 0x00;
	requestDataByteSize += 4;
	
	for(i=0; i<nSize; i++)
	{
		recvBuff[requestDataByteSize] = (unsigned char)(szCommBuff[i] & 0x00ff);
		requestDataByteSize++;
		recvBuff[requestDataByteSize] = (unsigned char)((szCommBuff[i]>>8) & 0x00ff);
		requestDataByteSize++;
	}

	buffSize = sizeof(recvBuff);
	bRet = CIPApp_sendRequestExplicit(m_hAppIP, m_hConnectIP, nMsgID, requestDataByteSize, (unsigned char *)recvBuff);
	if (bRet)
	{
		bRet = FALSE;

		recDataSize = CIPApp_receiveExplicit(m_hAppIP, &recvMessageID, buffSize, (unsigned char *)recvBuff, timeout, &recMaxSize);
		if (recDataSize>=0 && nMsgID == recvMessageID)
            bRet = TRUE;
	}

	ClosePLC();
#endif

	m_bBusy = FALSE;
	return bRet;
}


//PLC �б�
int COmronPlcM::ReceiveMessageFromPLC_Int(char *strTagName, DWORD nMsgID, char *szCommBuff)
{
	BOOL bRet = FALSE;
	int nCnt=0, nInt=0;

	m_bBusy = TRUE;

#if _OMRON
	CIPRequestPath 				requestPath = {0};		// Request path
	CIPRequestData 				requestData = {0};		// Request header
	CIPExplicitRequestHeader*	cipHeader;				// Rqquest message buffer
	DWORD						requestPathSize;
	DWORD						requestDataByteSize;
	DWORD	recvMessageID;			// receive messageID
	DWORD	buffSize;				// receive buffer size
	DWORD	timeout = 1000;			// time out(ms)
	DWORD	recMaxSize;				// this value always returns as 0

	int nDataLen;
	BYTE	recvBuff[256] = {0};	// receive buffer
	BYTE serviceCode = 0x4C;

	int classID = 1;
	int instanceID = 1;
	int attributeID = 0;
	int sendCount = 1;
	int nPosD, ac = 0;
	
	OpenPLC();

	cipHeader = ( CIPExplicitRequestHeader* )(unsigned char *)recvBuff;
	requestPath.path 	 = (BYTE *)( cipHeader + 1 );
	requestPath.byteSize = sizeof( recvBuff ) - sizeof(CIPExplicitRequestHeader);	
	requestPathSize = CIPUtil_constructRequestPathWithTagName((LPCTSTR)strTagName, &requestPath);

	cipHeader = ( CIPExplicitRequestHeader* )(unsigned char *)recvBuff;
	cipHeader->serviceCode = serviceCode;
	cipHeader->requestPathSize = (BYTE)requestPathSize / sizeof(WORD);
	requestDataByteSize = sizeof(CIPExplicitRequestHeader) + requestPathSize;

	recvBuff[requestDataByteSize] = 0x01;
	recvBuff[requestDataByteSize+1] = 0x00;
	requestDataByteSize += 2;

	nInt = -1;
	nDataLen = 0;
	strcpy(szCommBuff, "");

	////�߰��κ� nPos �ʱ�ȭ - �ʱ�ȭ ������ 7�� �Ǿ for(i=nPosD+1; i<nCnt; i++) Ÿ�� �ʰ� �ٷ� ����.
	nPosD=requestDataByteSize;
	bRet = CIPApp_sendRequestExplicit(m_hAppIP, m_hConnectIP, nMsgID, requestDataByteSize, (unsigned char *)recvBuff);
	if (bRet)
	{
		bRet = FALSE;
		buffSize = 256;;
		nCnt = CIPApp_receiveExplicit(m_hAppIP, &recvMessageID, buffSize, (unsigned char *)recvBuff, 1000, &recMaxSize);
		if (nCnt>6)
		{
			nInt = recvBuff[6];
		}
	}


	ClosePLC();
#endif
	
	m_bBusy = FALSE;
	return nInt;
}

BOOL COmronPlcM::ReceiveMessageFromPLC(char *strTagName, DWORD nMsgID, char *szCommBuff)
{
	BOOL bRet = FALSE;
	int nCnt=0;

	m_bBusy = TRUE;

#if _OMRON
	CIPRequestPath 				requestPath = {0};		// Request path
	CIPRequestData 				requestData = {0};		// Request header
	CIPExplicitRequestHeader*	cipHeader;				// Rqquest message buffer
	DWORD						requestPathSize;
	DWORD						requestDataByteSize;
	DWORD	recvMessageID;			// receive messageID
	DWORD	buffSize;				// receive buffer size
	DWORD	timeout = 1000;			// time out(ms)
	DWORD	recMaxSize;				// this value always returns as 0

	int i, nDataLen;
	BYTE	recvBuff[256] = {0};	// receive buffer
	BYTE serviceCode = 0x4C;

	int classID = 1;
	int instanceID = 1;
	int attributeID = 0;
	int sendCount = 1;
	int nPosD, ac = 0;
	
	OpenPLC();

	cipHeader = ( CIPExplicitRequestHeader* )(unsigned char *)recvBuff;
	requestPath.path 	 = (BYTE *)( cipHeader + 1 );
	requestPath.byteSize = sizeof( recvBuff ) - sizeof(CIPExplicitRequestHeader);	
	requestPathSize = CIPUtil_constructRequestPathWithTagName((LPCTSTR)strTagName, &requestPath);

	cipHeader = ( CIPExplicitRequestHeader* )(unsigned char *)recvBuff;
	cipHeader->serviceCode = serviceCode;
	cipHeader->requestPathSize = (BYTE)requestPathSize / sizeof(WORD);
	requestDataByteSize = sizeof(CIPExplicitRequestHeader) + requestPathSize;

	recvBuff[requestDataByteSize] = 0x09;
	recvBuff[requestDataByteSize+1] = 0x00;
	requestDataByteSize += 2;

	nDataLen = 0;
	strcpy(szCommBuff, "");

	////�߰��κ� nPos �ʱ�ȭ - �ʱ�ȭ ������ 7�� �Ǿ for(i=nPosD+1; i<nCnt; i++) Ÿ�� �ʰ� �ٷ� ����.
	nPosD=requestDataByteSize;
	bRet = CIPApp_sendRequestExplicit(m_hAppIP, m_hConnectIP, nMsgID, requestDataByteSize, (unsigned char *)recvBuff);
	if (bRet)
	{
		bRet = FALSE;
		buffSize = 256;
		nCnt = CIPApp_receiveExplicit(m_hAppIP, &recvMessageID, buffSize, (unsigned char *)recvBuff, 1000, &recMaxSize);
		if (nCnt>6)
		{
			//nNum = _ttoi(recvBuff[6]);
			for(i=6; i<nCnt; i+=2)
			{
				szCommBuff[nDataLen] = (char)recvBuff[i+1];
				nDataLen++;
				szCommBuff[nDataLen] = (char)recvBuff[i];
				nDataLen++;
			}

			szCommBuff[nDataLen] = '\0';
			bRet = TRUE;
		}
	}

	ClosePLC();
#endif
	
	m_bBusy = FALSE;
	return bRet;
}


// COmronPlcM message handlers


