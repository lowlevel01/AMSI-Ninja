# AMSI-Ninja
A project that encompasses various aspects of dealing with AMSI

# Usage
````
Usage: amsininja.exe <command> <file_path>


Commands:

[+] scanfile : check if file's content contains malicious signatures or not
[+] findflagged : find exact parts in the file that are statically flagged by Defender
````
# Features
  - [x] Scan file's content via AMSI
  - [x] Find exact parts in the file that are statically flagged by Defender

# TODO
- [ ] Optimize search function over all substrings of the payload.
      
# How to contribute
  the file ````utils.cpp```` contains the functions for each feature. Implement every function there and invoke it in ````amsininja.cpp````
