#include <iostream> 
#include "num.hpp" 
#include<stdio.h>
#include <string>
#include <string.h>
using namespace std;
// Run the code with the following input (Linux Terminal):
//In the same directory files :
// g++ -o test -std=c++17 num.cpp name_of_the_file.cpp
// Then : ./test







int lengthOfLastWord(string s) {
    
    std::cout << "Last caractère "<<s.substr(s.length()-1,s.length()-1)<< std::endl;
    
    while(s.substr(s.length()-1,s.length()-1)==" "){
      std::cout << s.length()<< std::endl;
      s= s.substr(0,s.length()-1);
      std::cout << s.length()<< std::endl;
    }
    std::string delim = " ";
    auto start = 0U;
    auto end = s.find(delim);
    while (end != std::string::npos)
    {
        //std::cout << s.substr(start, end - start) << std::endl;
        start = end + delim.length();
        end = s.find(delim, start);
        //std::cout << s.substr(start, end) <<endl;
    }
    std::cout << s.substr(start, end)<<endl;
    return s.substr(start, end).length();
}

int lengthOfLastWord_correction(string s) {
        int cnt = 0;
        int flag = 0;
        for(int i = s.length() - 1; i >= 0; i--) {
            if(flag == 0 && s[i] == ' ') {
                continue;
            }
            if(s[i] != ' ') {
                flag = 1;
                cnt++;
            } else {
                break;
            }
        }
        return cnt;
}

int main(int argc, char* argv[]) {
  string word="   fly me   to   the moon  ";
  int response = lengthOfLastWord(word);
  std ::cout << scope_indent <<response<<std::endl;
  return response;
}