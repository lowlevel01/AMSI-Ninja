#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

int GetWindowsBuild();
int AmsiFlagged(const std::wstring& paylod);
std::wstring ReadFileToWString(const std::wstring& filePath);


typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);