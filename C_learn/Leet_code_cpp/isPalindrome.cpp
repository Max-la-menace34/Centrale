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

bool isPalindrome(int x) {
  int x_copy = x;
  // Easy way to check that x is not a Palindrome
  if(x<0 || (x % 10 == 0 && x != 0)){
    return false;
  }

  // In this case we are spliting x in two parts and checking if the rigth part is equal to the left part.
  // You have two possible cases : odd and even (see the return )
  int inversed_number = 0,reminder;
  while(x>inversed_number){
    reminder = x%10;
    inversed_number = inversed_number*10 + reminder;
    x /=10;
  }
  std ::cout << scope_indent <<x<<"   "<< inversed_number<<std::endl;
  return x == inversed_number || x == inversed_number/10;
    }

int main(int argc, char* argv[]) {
  int x=121; // You can change the value x here
  int response = isPalindrome(x);
  std ::cout << scope_indent <<response<<std::endl;
  return x ;
}