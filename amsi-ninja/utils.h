#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

int GetWindowsBuild();
int AmsiFlagged(const std::wstring& payload);
std::string ReadFileToString(const std::wstring& filePath);
std::wstring ConvertToWideString(const std::string& input);
void removeMatchingSubstrings(std::string& s);
void printBanner();

std::wstring ReadFileToWString(const std::wstring& filePath);


typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);