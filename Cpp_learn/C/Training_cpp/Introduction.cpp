#include <iostream> 
#include "num.hpp" 
#include<stdio.h> 
#define SIZE 10

num addition(num,num);
void change_age_second_method(num&);

void change_age(num& value){
  fun_scope;
  value=17;
}


int main(int argc, char* argv[]) { 
  fun_scope;
  // Basic in c++ very easy part //////////////////////////////////////////////
  char name[10];
  num age {"age",25};
  num a {"a",10};
  ___;
  std::cout << scope_indent << "|  Welcome to c++ learn  |"<< std::endl;
  ___;
  rem("Define a varibales:");
  std::cout <<scope_indent <<"What is your name ? " <<std::flush;
  //std::cin>> name ;
  std::cout <<scope_indent <<"Your name is "<< name  <<std::endl;
  
  std::cout<<scope_indent<< "J'ai "<< age << " ans"<<std::endl;
  change_age(age);
  //change_age_second_method(&age);
  if(age>=18){
    std::cout <<scope_indent <<"You have the majority"<< name  <<std::endl;
  }else{
    std::cout <<scope_indent <<"Young guy drink some water"<< name  <<std::endl;
  }
  // For loop without using pointers : //////////////////////////////////////
  num tableau[SIZE];
  for(int i=0;i<SIZE;++i){
    rem("For Loop");
    num value {"value",i};
    tableau[i]= value;
  }
  // For loop by using pointers : /////////////////////////////////////////
  ___;
  rem("By using poiters with tab (tricky part)");
  num* pointer= tableau;
  num* end = tableau + SIZE;
  while(pointer != end)
    *(pointer++) *=2;
    //*(pointer++) = *pointer + 1;



  // By using a little function ///////////////////////////////////////////
  num result {"result",addition(tableau[4],tableau[5])};
  std::cout <<scope_indent <<"The result is "<< result  <<std::endl;
  rem("Way faster :");
  std::cout<< scope_indent << "The result is "<< addition(tableau[6],tableau[7])<<std::endl;
  
  
  
  // Pointers section ///////////////////////////::::
  num* address_of_age {&age};
  num* address_of_a{&age};
  num b {"b",0};
  b= *address_of_a;
  std::cout <<scope_indent <<"The adress of age is  "<< address_of_age  <<std::endl;
  std::cout <<scope_indent <<"The value of age is  "<< *address_of_age  <<std::endl;
  std::cout <<scope_indent <<"The adress of a is  "<< address_of_a  <<std::endl;
  std::cout <<scope_indent <<"The value and the adress of b  "<< &b << " and "<< b <<std::endl;

  // Dynamic allocation  part of the program :
  num* ptr = &age;
  int array_size;
  std::cout << scope_indent << "Enter the size of the array : " << std::flush;
  std::cin >> array_size;
  ptr = new num[array_size]; // This allocates array_size contiguous nums in the heap.
  rem("The array is allocated, and initialized by default.");
  num* iter = ptr;
  for(int i=0; i < array_size; /* increment of i is done within the loop */)
    *(iter++) = 2 * (i++) + 1;
  end = ptr + array_size;
  std::cout << scope_indent;
  for(iter = ptr; iter != end; ) std::cout << *(iter++) << ' ';
  std::cout << std::endl;
  ptr = nullptr;
  delete ptr;
  return 0;
} 


//Define our first function that just somme the two numbers

num addition(num a,num b){
  fun_scope;
  return a + b;
}

