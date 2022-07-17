#include <iostream> 
#include "num.hpp" 
#include<stdio.h>
//Runtime 0ms faster than 100% of C++ submissions
//Memory Usage: 5.9 MB, less than 73.32% of C++ submissions




// Run the code with the following input (Linux Terminal):
//In the same directory files :
// g++ -o test -std=c++17 num.cpp name_of_the_file.cpp
// Then : ./test


// Newton's method in order to find the root.
// We are getting closer and closer to our point 
//with our treshold equal to 1 in our case.
// if you put 0.0001 it will be closer to the real root
// Don't forget to change the return with a float :) 
int mySqrt(int x) {
        double result=x;
        while(result*result - x >=1){
            result = (result+x/result)/2;
        }
        return int(result);
    }

int main(int argc, char* argv[]) {
  int x=121; // You can change the value x here
  int response = mySqrt(x);
  std ::cout << scope_indent <<response<<std::endl;
  return x ;
}