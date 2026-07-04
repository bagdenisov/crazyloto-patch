//MIT License
//
//Copyright (c) 2026 bagdenisov
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include <windows.h>

extern "C" {

HKEY virtualReg = (HKEY)malloc(4);
const char* targetReg = "Software\\Buka\\RUD Present\\Crazy Loto 4";
const char* GameDir = "GameDir";
const char* GameDirValue = ".";
const char* CDGameDir = "CDGameDir";
const char* CDGameDirValue = "D:";

__declspec(dllexport) LSTATUS WINAPI RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) {
	if (strcmp(lpSubKey, targetReg) == 0) {
		*phkResult = virtualReg;
		return ERROR_SUCCESS;
	}
	static LSTATUS(WINAPI* pfn)(HKEY, LPCSTR, DWORD, REGSAM, PHKEY) = nullptr;
    if (!pfn) {
        HMODULE h = GetModuleHandleA("advapi32.dll");
        if (!h) h = LoadLibraryA("advapi32.dll");
        pfn = (LSTATUS(WINAPI*)(HKEY, LPCSTR, DWORD, REGSAM, PHKEY))GetProcAddress(h, "RegOpenKeyExA");
    }
    return pfn ? pfn(hKey, lpSubKey, ulOptions, samDesired, phkResult) : ERROR_PROC_NOT_FOUND;
}

__declspec(dllexport) LSTATUS WINAPI RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) {
	int result;
	if (hKey == virtualReg) {
		if (lpData == NULL) {
			if (lpcbData != NULL) {
				// Просто возвращаем нужный размер (например, для строки "MyString")
				*lpcbData = (DWORD)(strlen("MyString") + 1); 
			}
			return ERROR_SUCCESS; // Или возвращаем успех, как это делает Windows
		}

		// 2. ПРОВЕРКА НА РАЗМЕР: Проверяем, хватит ли места в буфере пользователя
		DWORD requiredSize = (DWORD)(strlen("MyString") + 1);

		if (lpcbData == NULL || *lpcbData < requiredSize) {
			if (lpcbData != NULL) {
				*lpcbData = requiredSize; // Сообщаем, сколько байт нам нужно
			}
			return ERROR_MORE_DATA; // Ошибка: буфер слишком мал
		}
		result = ERROR_SUCCESS;
		if ((strcmp(lpValueName, GameDir) == 0)) {
			*lpType = REG_SZ;
			strcpy((char*)lpData, GameDirValue);
			*lpcbData = (DWORD)(strlen((char*)lpData) + 1);
		}
		
		if ((strcmp(lpValueName, CDGameDir) == 0)) {
			*lpType = REG_SZ;
			strcpy((char*)lpData, CDGameDirValue);
			*lpcbData = (DWORD)(strlen((char*)lpData) + 1);		

		}
	} else {
		static LSTATUS(WINAPI* pfn)(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD) = nullptr;
		if (!pfn) {
			HMODULE h = GetModuleHandleA("advapi32.dll");
			if (!h) h = LoadLibraryA("advapi32.dll");
			pfn = (LSTATUS(WINAPI*)(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD))GetProcAddress(h, "RegQueryValueExA");
		}
		result = pfn ? pfn(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData) : ERROR_PROC_NOT_FOUND;
	}
	return result;
}

__declspec(dllexport) LSTATUS WINAPI RegFlushKey(HKEY hKey) {
	if (hKey == virtualReg) return ERROR_SUCCESS;	
	static LSTATUS(WINAPI* pfn)(HKEY) = nullptr;
    if (!pfn) {
        HMODULE h = GetModuleHandleA("advapi32.dll");
        if (!h) h = LoadLibraryA("advapi32.dll");
        pfn = (LSTATUS(WINAPI*)(HKEY))GetProcAddress(h, "RegFlushKey");
    }
    return pfn ? pfn(hKey) : ERROR_PROC_NOT_FOUND;
}

__declspec(dllexport) LSTATUS WINAPI RegCloseKey(HKEY hKey) {
	if (hKey == virtualReg) return ERROR_SUCCESS;
    static LSTATUS(WINAPI* pfn)(HKEY) = nullptr;
    if (!pfn) {
        HMODULE h = GetModuleHandleA("advapi32.dll");
        if (!h) h = LoadLibraryA("advapi32.dll");
        pfn = (LSTATUS(WINAPI*)(HKEY))GetProcAddress(h, "RegCloseKey");
    }
    return pfn ? pfn(hKey) : ERROR_PROC_NOT_FOUND;
}

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    return TRUE;
}
