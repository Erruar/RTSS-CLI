#pragma once

#ifndef OSD_MANAGER_H
#define OSD_MANAGER_H
 
#include <string>
#include <afxdisp.h>
#include <afxcmn.h>

class OSDManager {
public:
    OSDManager();
    ~OSDManager();

    void displayText(const char* text);
	// Implementation
	afx_msg void OnDestroy();
protected:
	void Refresh();

	DWORD						EmbedGraph(DWORD dwOffset, FLOAT* lpBuffer, DWORD dwBufferPos, DWORD dwBufferSize, LONG dwWidth, LONG dwHeight, LONG dwMargin, FLOAT fltMin, FLOAT fltMax, DWORD dwFlags);

	DWORD						GetClientsNum();
	DWORD						GetSharedMemoryVersion();
	BOOL						UpdateOSD(LPCSTR lpText);
	void						ReleaseOSD(); 

	BOOL						m_bMultiLineOutput;
	BOOL						m_bFormatTags;
	BOOL						m_bFillGraphs;
	BOOL						m_bConnected;
	BOOL						m_bStickyLayers;
	BOOL						m_bAbsolutePosition;

	HICON						m_hIcon;
	UINT						m_nTimerID; 

	DWORD						m_dwNumberOfProcessors; 

	DWORD						m_dwTickCount;
	LARGE_INTEGER				m_idleTime;
	FLOAT						m_fltCpuUsage;

	FLOAT						m_fltCpuUsageHistory;
	FLOAT						m_fltRamUsageHistory;
	DWORD						m_dwHistoryPos;

	CFont						m_font;
	CRichEditCtrl				m_richEditCtrl;

	CString						m_strStatus;
	CString						m_strInstallPath; 


	// Generated message map functions
	//{{AFX_MSG(CRTSSSharedMemorySampleDlg) 
	
};

#endif // OSD_MANAGER_H