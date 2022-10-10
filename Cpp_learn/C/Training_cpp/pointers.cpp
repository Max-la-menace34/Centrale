#include <iostream> 
#include "num.hpp" 
#include<stdio.h> 
#define SIZE 10
using namespace std;

void new_year(int&);
void two_year(int*);
//g++ -o test -std=c++17 num.cpp pointers.cpp 
int main(int argc, char* argv[]) { 
fun_scope;

int age_maxime(22);
int age_juliette(12);
int* ptr_maxime(&age_maxime);
int* ptr_juliette(&age_juliette);
std::cout << *ptr_maxime << std::endl;
new_year(age_maxime);
two_year(ptr_juliette);
std::cout << age_maxime << std::endl;

 num tab[SIZE];
 tab[0]=1;
 num* pointer= tab+1;
    num* end = tab + SIZE;
  while(pointer != end)
    *(pointer++) = *(pointer-1) + 1;

for( auto i = tab; i <end;++i){
      std::cout << *i << std::endl;
}
  ___;
return 0;
} 


void new_year(int& age){
    fun_scope;
    age += 1;

    ___;
}
void two_year(int* age){
    fun_scope;
    age+=2;
    ___;
}