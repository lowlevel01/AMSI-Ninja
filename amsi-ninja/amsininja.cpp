#include "amsininja.h"
#include "utils.h"


int main(int argc, char* argv[]) {

    printBanner();
    if (argc < 3) {
        std::cout << "Usage: amsininja.exe <command> <file_path>" << std::endl;
        std::cout << "\n\nCommands:\n" << std::endl;
        std::cout << "[+] scanfile : check if file's content is malicious or not" << std::endl;
        std::cout << "[+] findflagged : find exact parts flagged by defender" << std::endl;
        return 1;
    }

    std::string command(argv[1]);

    if (command == "scanfile") {
        // Convert argv[1] from UTF-8 (char*) to std::wstring
        std::string utf8Path(argv[2]);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring filePath = converter.from_bytes(utf8Path);

        std::wstring content = ConvertToWideString(ReadFileToString(filePath));
        int amsiResult = AmsiFlagged(content);
        if (amsiResult < 0) {
            printf("Error : Make sure Windows Defender is turned on");
            return -1;
        }

        if (amsiResult) {
            printf("**The file contains malicious content**");
        }
        else {
            printf("The file is clean!");
        }
    }
    
    
    else if (command == "findflagged") {
        // Convert argv[1] from UTF-8 (char*) to std::wstring
        std::string utf8Path(argv[2]);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring filePath = converter.from_bytes(utf8Path);

        std::string content = ReadFileToString(filePath);

        removeMatchingSubstrings(content);
    }
    
    else {
        std::cout << "Usage: amsininja.exe <command> <file_path>" << std::endl;
        std::cout << "commands:" << std::endl;
        std::cout << "scanfile : check if file's content is malicious or not" << std::endl;
        std::cout << "findflagged : find exact parts flagged by defender" << std::endl;
    }

    

    
    
    
}
