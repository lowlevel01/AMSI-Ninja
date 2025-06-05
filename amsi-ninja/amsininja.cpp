#include "amsininja.h"
#include "utils.h";


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: amsininja.exe <file_path>" << std::endl;
        return 1;
    }

    // Convert argv[1] from UTF-8 (char*) to std::wstring
    std::string utf8Path(argv[1]);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring filePath = converter.from_bytes(utf8Path);
    //std::wstring filePath = L"abcd.txt";

    std::wstring content = ReadFileToWString(filePath);
    //printf("%ls\n", content.c_str());

    int amsiResult = AmsiFlagged(content);
    if (amsiResult < 0) {
        printf("Error : Make sure Windows Defender is turned on");
    }

    if (amsiResult) {
        printf("Malicious!!!!");
    }
    else {
        printf("Not malicious!");
    }
}
