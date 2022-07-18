```cpp

#include <iostream> 
#include "num.hpp" 
#include<stdio.h>
#include <string>
#include <string.h>
using namespace std;

//Runtime: 0 ms, faster than 100.00% of C++ online submissions for Implement strStr().
//Memory Usage: 6.3 MB, less than 75.34% of C++ online submissions for Implement strStr().



// Run the code with the following input (Linux Terminal):
//In the same directory files :
// g++ -o test -std=c++17 num.cpp name_of_the_file.cpp
// Then : ./test


int strStr(string haystack, string needle) {
    char str1[haystack.length()+1];
    strcpy(str1,haystack.c_str());
    char str2[needle.length()+1];
    strcpy(str2,needle.c_str());
    //std::cout << str1 <<endl;
    char *ptr=strstr(str1,str2);
    if(ptr){
      return ptr-str1;
    }else{ return -1;}
      }
```
