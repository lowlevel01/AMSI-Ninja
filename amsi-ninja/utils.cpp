#include "utils.h"
#include "amsininja.h"

int GetWindowsBuild() {
    HMODULE hMod = GetModuleHandle(L"ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr) {
            RTL_OSVERSIONINFOW rovi = { 0 };
            rovi.dwOSVersionInfoSize = sizeof(rovi);

            if (fxPtr(&rovi) == 0) {
                return rovi.dwBuildNumber;
            }
        }
    }
}

int AmsiFlagged(const std::wstring& payload) {

    // get Windows build number to be used in AmsiContext
    int build_number = GetWindowsBuild();
    std::wstring fullPath = L"PowerShell_C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe_" + std::to_wstring(build_number);

    HAMSICONTEXT amsiContext = nullptr;
    HRESULT hr = AmsiInitialize(fullPath.c_str(), &amsiContext);
    if (FAILED(hr)) {
        AmsiUninitialize(amsiContext);
        return -1;
    }

    // Create a modifiable buffer for the payload
    //std::wstring payload = L"invoke-mimikatz";
    //ULONG length = (ULONG)(wcslen(payload) * sizeof(wchar_t));
    AMSI_RESULT result;


    hr = AmsiScanString(amsiContext, payload.c_str(), nullptr, nullptr, &result);
    if (FAILED(hr)) {
        AmsiUninitialize(amsiContext);
        return -1;
    }

    if (AmsiResultIsMalware(result)) {
        AmsiUninitialize(amsiContext);
        return 1;
    }
    else {
        AmsiUninitialize(amsiContext);
        return 0;
    }

}



// Function to read file content
std::string ReadFileToString(const std::wstring& filePath) {
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return {};
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
        CloseHandle(hFile);
        return {};
    }

    std::vector<char> buffer(fileSize);
    DWORD bytesRead;
    if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
        CloseHandle(hFile);
        return {};
    }

    CloseHandle(hFile);
    return std::string(buffer.begin(), buffer.end());
}

// Function that converts ASCII to Wide String
std::wstring ConvertToWideString(const std::string& input) {
    if (input.empty()) {
        return {};
    }

    int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, input.data(), static_cast<int>(input.size()), NULL, 0);
    if (wideCharCount == 0) {
        return {};
    }

    std::wstring wideStr(wideCharCount, L'\0');
    if (MultiByteToWideChar(CP_UTF8, 0, input.data(), static_cast<int>(input.size()), &wideStr[0], wideCharCount) == 0) {
        return {};
    }

    return wideStr;
}

// Function to read the contents of the file
std::wstring ReadFileToWString(const std::wstring& filePath) {

    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return L"";
    }

    // Get the file's size
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return L"";
    }

    // Allocate a buffer for the content
    std::vector<char> buffer(fileSize);
    DWORD bytesRead;
    if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
        CloseHandle(hFile);
        return L"";
    }

    CloseHandle(hFile);

    // Convert the buffer to a wide string
    int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, buffer.data(), bytesRead, NULL, 0);
    if (wideCharCount == 0) {
        return L"";
    }

    std::wstring wideStr(wideCharCount, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, buffer.data(), bytesRead, &wideStr[0], wideCharCount);

    return wideStr;
}

void removeMatchingSubstrings(std::string& s) {
    std::cout << "Finding all Flagged parts :"<< std::endl;

    bool foundAny = true;
    int iteration = 0;

    while (foundAny && !s.empty()) {
        foundAny = false;
        iteration++;

        // Test from length 2 up to current string length
        for (size_t len = 2; len <= s.length() && !foundAny; ++len) {
            for (size_t i = 0; i <= s.length() - len; ++i) {
                std::string sub = s.substr(i, len);

                int amsiResult = AmsiFlagged(ConvertToWideString(sub));

                if (amsiResult == 1) {
                    std::cout << "  Pos " << i << ": '" << sub << "'";
                    std::cout << " -> *** FLAGGED! ***" << std::endl;
                    s.erase(i, len);
                    foundAny = true;
                    break; // Exit inner loop to restart from length 2
                }
            }
        }

        if (!foundAny) {
            std::cout << "\nNo more flagged substrings found." << std::endl;
        }

        // Safety check to prevent infinite loops
        if (iteration > 10000) {
            break;
        }
    }

}


// Function to print banner


void printBanner() {
    std::cout << R"(
    _    __  __ ____ ___      _   _ _        _       
   / \  |  \/  / ___|_ _|    | \ | (_)_ __  (_) __ _ 
  / _ \ | |\/| \___ \| |_____|  \| | | '_ \ | |/ _` |
 / ___ \| |  | |___) | |_____| |\  | | | | || | (_| |
/_/   \_\_|  |_|____/___|    |_| \_|_|_| |_|/ |\__,_|
                                          |__/        

)" << std::endl;
}