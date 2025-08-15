#pragma once

#ifndef OSD_MANAGER_H
#define OSD_MANAGER_H

#include <string>
#include <afxdisp.h>
#include <afxcmn.h>

#ifdef __cplusplus 
extern "C" { 
#ifdef _WIN32
#define EXP __declspec(dllexport)
#define CALL __stdcall
#else
#define EXP __attribute__((visibility("default")))
#define CALL
#endif  
		EXP void CALL displayText(const char* text);
		// Implementation
		afx_msg void OnDestroy();
		EXP int CALL Refresh();

		EXP DWORD CALL EmbedGraph(DWORD dwOffset, FLOAT* lpBuffer, DWORD dwBufferPos, DWORD dwBufferSize, LONG dwWidth, LONG dwHeight, LONG dwMargin, FLOAT fltMin, FLOAT fltMax, DWORD dwFlags);

		EXP DWORD CALL GetClientsNum();
		EXP DWORD CALL GetSharedMemoryVersion();
		EXP BOOL CALL UpdateOSD(LPCSTR lpText);
		EXP BOOL CALL UpdateOSDSpan(const char* lpText, int length);
		EXP void CALL displayTextSpan(const char* text, int length);
		EXP int	CALL ReleaseOSD();

		BOOL                        m_bIsInitialized = false;
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
 
}
#endif
#endif // OSD_MANAGER_H
