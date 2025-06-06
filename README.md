# AMSI-Ninja
A project that encompasses various aspects of dealing with AMSI (Currently in development)

# Usage
````
Usage: amsininja.exe <command> <file_path>


Commands:

[+] scanfile : check if file's content is malicious or not
[+] findflagged : find exact parts flagged by defender
````
# Features
  - [x] Scan file's content via AMSI
  - [x] Find exact parts in your payload that are flagged by Defender

# TODO
- [ ] Optimize search function over all substrings of the payload.
      
# How to contribute
  the file ````utils.cpp```` contains the functions for each feature. Implement every function there and invoke it in ````amsininja.cpp````
