// Saku RTSS CLI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include "osd_manager.h" 
#include <wtypes.h>
#include "../../../../../../Program Files (x86)/RivaTuner Statistics Server/SDK/Include/RTSSSharedMemory.h" 

OSDManager::OSDManager() {
     
}

OSDManager::~OSDManager() {
     
}

void OSDManager::displayText(const char* text) {
	if (text != "") {
		std::string modifiedText(text); 
		std::string searchStr = "<Br>"; 
		std::string replaceStr = "\n"; 

		// Replace all occurrences of <Br> with \n
		size_t pos = 0; 
		while ((pos = modifiedText.find(searchStr, pos)) != std::string::npos) { 
			modifiedText.replace(pos, searchStr.length(), replaceStr); 
			pos += replaceStr.length(); 
		} 
		GetSharedMemoryVersion();
		GetClientsNum(); 
		UpdateOSD(modifiedText.c_str()); 
		std::cout << "New text set successfully!\n" << std::endl;
		//Refresh();
		//set new text
		//OSDManager::OnDestroy();
    }
    else {
        std::cerr << "Failed to create OSD instance." << std::endl;
    }
} 

static void printUsage(const char* programName) {
    std::cout << "Usage: \"" << programName << "\" --text \"Your text here\"\n" 
    << "Reset OSD Text: \"" << programName << "\" --reset\n" 
	<< "<Br> - Next line\n"
    << "<P0><L0> - define layer 0 located in sticky position 0 (top left corner) \n" 
    << "<P2><L1> - define layer 1 located in sticky position 1 (top right corner) \n"
    << "<P6><L2> - define layer 2 located in sticky position 2 (bottom left corner)\n"
    << "<P8><L3> - define layer 3 located in sticky position 3 (bottom right corner)\n"
    << "<P=10,10> - move to position 10,10 (in zoomed pixel units)\n"
    << "Note: take a note that position is specified in absolute coordinates so use this tag with caution because your text may\n"
    << "overlap with text slots displayed by other applications, so in this demo we explicitly disable this tag usage if more than\n"
    << "one client is currently rendering something in OSD and we don't use sticky layers\n"
    << "<A0=-5> - define align variable A[0] as right alignment by 5 symbols (positive is left, negative is right)\n"
    << "<A1=4> - define align variable A[1] as left alignment by 4 symbols (positive is left, negative is right)\n"
    << "<C0=FFA0A0> - define color variable C[0] as R=FF,G=A0 and B=A0\n"
    << "<C1=FF00A0> - define color variable C[1] as R=FF,G=00 and B=A0\n"
    << "<C2=FFFFFF> - define color variable C[1] as R=FF,G=FF and B=FF\n"
    << "<S0=-50> - define size variable S[0] as 50% subscript (positive is superscript, negative is subscript)\n"
    << "<S1=50> - define size variable S[0] as 50% supercript (positive is superscript, negative is subscript)\n"
    << "Note: we could apply explicit alignment,size and color definitions when necerrary (e.g. <C=FFFFFF>, however\n"
    << "variables usage makes tagged text more compact and readable\n"
    << "Example: \"" << programName << "\" --text \"<C0=FFA0A0><C1=A0FFA0><C0>Welcome to <Br><C1>Saku Overclock!\"\n"
    << "\n"
    << "\n"
    << "About\n"
    << "Saku Riva Tuner Statistics Server CLI Application - v1.0.0\n"
    << "@ Serzhik Sakurazhima 2024\n" 
		<< std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
		if (argc == 2 && std::string(argv[1]) == "--reset") {
			OSDManager osdManager; 
			osdManager.OnDestroy(); 
			std::cout << "OSD reset successfully." << std::endl;
			return 0;
		}
		else {
			printUsage(argv[0]);
			return 1;
		}
    }

    std::string arg = argv[1];
    if (arg == "--text") { 

        OSDManager osdManager;
        osdManager.displayText(argv[2]);
    }
    else { 
        printUsage(argv[0]);
        return 1;
    }

    return 0;
} 
/////////////////////////////////////////////////////////////////////////////
void OSDManager::OnDestroy()
{   
	ReleaseOSD(); 
}
/////////////////////////////////////////////////////////////////////////////
DWORD OSDManager::GetSharedMemoryVersion()
{
	DWORD dwResult = 0;

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"RTSSSharedMemoryV2");

	if (hMapFile)
	{
		LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

		if (pMem)
		{
			if ((pMem->dwSignature == 'RTSS') &&
				(pMem->dwVersion >= 0x00020000))
				dwResult = pMem->dwVersion;

			UnmapViewOfFile(pMapAddr);
		}

		CloseHandle(hMapFile);
	}

	return dwResult;
}
/////////////////////////////////////////////////////////////////////////////
DWORD OSDManager::EmbedGraph(DWORD dwOffset, FLOAT* lpBuffer, DWORD dwBufferPos, DWORD dwBufferSize, LONG dwWidth, LONG dwHeight, LONG dwMargin, FLOAT fltMin, FLOAT fltMax, DWORD dwFlags)
{
	DWORD dwResult = 0;

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"RTSSSharedMemoryV2");

	if (hMapFile)
	{
		LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

		if (pMem)
		{
			if ((pMem->dwSignature == 'RTSS') &&
				(pMem->dwVersion >= 0x00020000))
			{
				for (DWORD dwPass = 0; dwPass < 2; dwPass++)
					//1st pass : find previously captured OSD slot
					//2nd pass : otherwise find the first unused OSD slot and capture it
				{
					for (DWORD dwEntry = 1; dwEntry < pMem->dwOSDArrSize; dwEntry++)
						//allow primary OSD clients (i.e. EVGA Precision / MSI Afterburner) to use the first slot exclusively, so third party
						//applications start scanning the slots from the second one
					{
						RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY pEntry = (RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY)((LPBYTE)pMem + pMem->dwOSDArrOffset + dwEntry * pMem->dwOSDEntrySize);

						if (dwPass)
						{
							if (!strlen(pEntry->szOSDOwner))
								strcpy_s(pEntry->szOSDOwner, sizeof(pEntry->szOSDOwner), "RTSSSharedMemorySample");
						}

						if (!strcmp(pEntry->szOSDOwner, "RTSSSharedMemorySample"))
						{
							if (pMem->dwVersion >= 0x0002000c)
								//embedded graphs are supported for v2.12 and higher shared memory
							{
								if (dwOffset + sizeof(RTSS_EMBEDDED_OBJECT_GRAPH) + dwBufferSize * sizeof(FLOAT) > sizeof(pEntry->buffer))
									//validate embedded object offset and size and ensure that we don't overrun the buffer
								{
									UnmapViewOfFile(pMapAddr);

									CloseHandle(hMapFile);

									return 0;
								}

								LPRTSS_EMBEDDED_OBJECT_GRAPH lpGraph = (LPRTSS_EMBEDDED_OBJECT_GRAPH)(pEntry->buffer + dwOffset);
								//get pointer to object in buffer

								lpGraph->header.dwSignature = RTSS_EMBEDDED_OBJECT_GRAPH_SIGNATURE;
								lpGraph->header.dwSize = sizeof(RTSS_EMBEDDED_OBJECT_GRAPH) + dwBufferSize * sizeof(FLOAT);
								lpGraph->header.dwWidth = dwWidth;
								lpGraph->header.dwHeight = dwHeight;
								lpGraph->header.dwMargin = dwMargin;
								lpGraph->dwFlags = dwFlags;
								lpGraph->fltMin = fltMin;
								lpGraph->fltMax = fltMax;
								lpGraph->dwDataCount = dwBufferSize;

								if (lpBuffer && dwBufferSize)
								{
									for (DWORD dwPos = 0; dwPos < dwBufferSize; dwPos++)
									{
										FLOAT fltData = lpBuffer[dwBufferPos];

										lpGraph->fltData[dwPos] = (fltData == FLT_MAX) ? 0 : fltData;

										dwBufferPos = (dwBufferPos + 1) & (dwBufferSize - 1);
									}
								}

								dwResult = lpGraph->header.dwSize;
							}

							break;
						}
					}

					if (dwResult)
						break;
				}
			}

			UnmapViewOfFile(pMapAddr);
		}

		CloseHandle(hMapFile);
	}

	return dwResult;
}
/////////////////////////////////////////////////////////////////////////////
BOOL OSDManager::UpdateOSD(LPCSTR lpText)
{
	BOOL bResult = FALSE;

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"RTSSSharedMemoryV2");

	if (hMapFile)
	{
		LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

		if (pMem)
		{
			if ((pMem->dwSignature == 'RTSS') &&
				(pMem->dwVersion >= 0x00020000))
			{
				for (DWORD dwPass = 0; dwPass < 2; dwPass++)
					//1st pass : find previously captured OSD slot
					//2nd pass : otherwise find the first unused OSD slot and capture it
				{
					for (DWORD dwEntry = 1; dwEntry < pMem->dwOSDArrSize; dwEntry++)
						//allow primary OSD clients (i.e. EVGA Precision / MSI Afterburner) to use the first slot exclusively, so third party
						//applications start scanning the slots from the second one
					{
						RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY pEntry = (RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY)((LPBYTE)pMem + pMem->dwOSDArrOffset + dwEntry * pMem->dwOSDEntrySize);

						if (dwPass)
						{
							if (!strlen(pEntry->szOSDOwner))
								strcpy_s(pEntry->szOSDOwner, sizeof(pEntry->szOSDOwner), "RTSSSharedMemorySample");
						}

						if (!strcmp(pEntry->szOSDOwner, "RTSSSharedMemorySample"))
						{
							if (pMem->dwVersion >= 0x00020007)
								//use extended text slot for v2.7 and higher shared memory, it allows displaying 4096 symbols
								//instead of 256 for regular text slot
							{
								if (pMem->dwVersion >= 0x0002000e)
									//OSD locking is supported on v2.14 and higher shared memory
								{
									DWORD dwBusy = _interlockedbittestandset(&pMem->dwBusy, 0);
									//bit 0 of this variable will be set if OSD is locked by renderer and cannot be refreshed
									//at the moment

									if (!dwBusy)
									{
										strncpy_s(pEntry->szOSDEx, sizeof(pEntry->szOSDEx), lpText, sizeof(pEntry->szOSDEx) - 1);

										pMem->dwBusy = 0;
									}
								}
								else
									strncpy_s(pEntry->szOSDEx, sizeof(pEntry->szOSDEx), lpText, sizeof(pEntry->szOSDEx) - 1);

							}
							else
								strncpy_s(pEntry->szOSD, sizeof(pEntry->szOSD), lpText, sizeof(pEntry->szOSD) - 1);

							pMem->dwOSDFrame++;

							bResult = TRUE;

							break;
						}
					}

					if (bResult)
						break;
				}
			}

			UnmapViewOfFile(pMapAddr);
		}

		CloseHandle(hMapFile);
	}

	return bResult;
}
/////////////////////////////////////////////////////////////////////////////
void OSDManager::ReleaseOSD()
{
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"RTSSSharedMemoryV2");

	if (hMapFile)
	{
		LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

		if (pMem)
		{
			if ((pMem->dwSignature == 'RTSS') &&
				(pMem->dwVersion >= 0x00020000))
			{
				for (DWORD dwEntry = 1; dwEntry < pMem->dwOSDArrSize; dwEntry++)
				{
					RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY pEntry = (RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY)((LPBYTE)pMem + pMem->dwOSDArrOffset + dwEntry * pMem->dwOSDEntrySize);

					if (!strcmp(pEntry->szOSDOwner, "RTSSSharedMemorySample"))
					{
						memset(pEntry, 0, pMem->dwOSDEntrySize);
						pMem->dwOSDFrame++;
					}
				}
			}

			UnmapViewOfFile(pMapAddr);
		}

		CloseHandle(hMapFile);
	}
}
/////////////////////////////////////////////////////////////////////////////
DWORD OSDManager::GetClientsNum()
{
	DWORD dwClients = 0;

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"RTSSSharedMemoryV2");

	if (hMapFile)
	{
		LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

		if (pMem)
		{
			if ((pMem->dwSignature == 'RTSS') &&
				(pMem->dwVersion >= 0x00020000))
			{
				for (DWORD dwEntry = 0; dwEntry < pMem->dwOSDArrSize; dwEntry++)
				{
					RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY pEntry = (RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY)((LPBYTE)pMem + pMem->dwOSDArrOffset + dwEntry * pMem->dwOSDEntrySize);

					if (strlen(pEntry->szOSDOwner))
						dwClients++;
				}
			}

			UnmapViewOfFile(pMapAddr);
		}

		CloseHandle(hMapFile);
	}

	return dwClients;
} 
void OSDManager::Refresh()
{
	//init RivaTuner Statistics Server installation path

	if (m_strInstallPath.IsEmpty())
	{
		HKEY hKey;

		if (ERROR_SUCCESS == RegOpenKey(HKEY_LOCAL_MACHINE, L"Software\\Unwinder\\RTSS", &hKey))
		{
			char buf[MAX_PATH];

			DWORD dwSize = MAX_PATH;
			DWORD dwType;

			if (ERROR_SUCCESS == RegQueryValueEx(hKey, L"InstallPath", 0, &dwType, (LPBYTE)buf, &dwSize))
			{
				if (dwType == REG_SZ)
					m_strInstallPath = buf;
			}

			RegCloseKey(hKey);
		}
	}

	//validate RivaTuner Statistics Server installation path

	if (_taccess(m_strInstallPath, 0))
		m_strInstallPath = "";
	 

	//init shared memory version

	DWORD dwSharedMemoryVersion = GetSharedMemoryVersion();

	//init max OSD text size, we'll use extended text slot for v2.7 and higher shared memory, 
	//it allows displaying 4096 symbols /instead of 256 for regular text slot

	DWORD dwMaxTextSize = (dwSharedMemoryVersion >= 0x00020007) ? sizeof(RTSS_SHARED_MEMORY::RTSS_SHARED_MEMORY_OSD_ENTRY().szOSDEx) : sizeof(RTSS_SHARED_MEMORY::RTSS_SHARED_MEMORY_OSD_ENTRY().szOSD);

	// RivaTuner based products use similar CGroupedString object for convenient OSD text formatting and length control
	// You may use it to format your OSD similar to RivaTuner's one or just use your own routines to format OSD text

	BOOL bFormatTagsSupported = (dwSharedMemoryVersion >= 0x0002000b);
	//text format tags are supported for shared memory v2.11 and higher
	BOOL bObjTagsSupported = (dwSharedMemoryVersion >= 0x0002000c);
	//embedded object tags are supporoted for shared memory v2.12 and higher

	CString strOSD;

	if (bFormatTagsSupported && m_bFormatTags)
	{
		if (m_bStickyLayers)
		{
			strOSD += "<P0><L0>+";
			//define layer 0 located in sticky position 0 (top left corner)
			strOSD += "<P2><L1>+";
			//define layer 1 located in sticky position 1 (top right corner)
			strOSD += "<P6><L2>+";
			//define layer 2 located in sticky position 2 (bottom left corner)
			strOSD += "<P8><L3>+";
			//define layer 3 located in sticky position 3 (bottom right corner)
		}

		if ((GetClientsNum() == 1) || m_bStickyLayers || m_bAbsolutePosition)
			strOSD += "<P=10,10>";
		//move to position 10,10 (in zoomed pixel units)

		//Note: take a note that position is specified in absolute coordinates so use this tag with caution because your text may
		//overlap with text slots displayed by other applications, so in this demo we explicitly disable this tag usage if more than
		//one client is currently rendering something in OSD and we don't use sticky layers

		strOSD += "<A0=-5>";
		//define align variable A[0] as right alignment by 5 symbols (positive is left, negative is right)
		strOSD += "<A1=4>";
		//define align variable A[1] as left alignment by 4 symbols (positive is left, negative is right)
		strOSD += "<C0=FFA0A0>";
		//define color variable C[0] as R=FF,G=A0 and B=A0
		strOSD += "<C1=FF00A0>";
		//define color variable C[1] as R=FF,G=00 and B=A0
		strOSD += "<C2=FFFFFF>";
		//define color variable C[1] as R=FF,G=FF and B=FF
		strOSD += "<S0=-50>";
		//define size variable S[0] as 50% subscript (positive is superscript, negative is subscript)
		strOSD += "<S1=50>";
		//define size variable S[0] as 50% supercript (positive is superscript, negative is subscript)

		strOSD += "\r";
		//add \r just for this demo to make tagged text more readable in demo preview window, OSD ignores \r anyway

	//Note: we could apply explicit alignment,size and color definitions when necerrary (e.g. <C=FFFFFF>, however
	//variables usage makes tagged text more compact and readable
	}
	else
		strOSD = ""; 

	LPCSTR strGroup; 
	if (bFormatTagsSupported && m_bFormatTags)
		strGroup = "<C1>RAM<C>";
	//set color previously defined by C[1] then restore defaults
	else
		strGroup = "RAM";

	LPCSTR strValue;
	if (bFormatTagsSupported && m_bFormatTags)
		strValue = "<A0>%d<A><A1><S1> MB<S><A>";
	//align the text surrounded by alignment tags
	else
		strValue = "%d MB";  
	BOOL bTruncated = FALSE; 
	strOSD += strValue; 
	if (bObjTagsSupported && m_bFormatTags)
	{
		strOSD += "\n"; 
		DWORD dwObjectOffset = 0;
		DWORD dwObjectSize = 0;
		DWORD dwFlags = m_bFillGraphs ? RTSS_EMBEDDED_OBJECT_GRAPH_FLAG_FILLED : 0; 
	}
	if (!strOSD.IsEmpty())
	{
		BOOL bResult = UpdateOSD((LPCSTR)(LPCTSTR)strOSD); 
		//m_richEditCtrl.SetWindowText(m_strStatus);
	}
} 
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
