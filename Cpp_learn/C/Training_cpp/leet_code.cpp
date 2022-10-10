#include <iostream> 
#include "num.hpp" 
#include<stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include<cstring>
#include <cmath>

#include <bits/stdc++.h>
using namespace std;

/*
Premier code en c++ !!

*/
int exercice1() {

    std::string str;
    string result ="";
    int num;
    int* ptr = &num;
    string string1 = "";
    string string2 = "";
    
    std::cout <<scope_indent <<"String1 : " <<std::flush;
    std::cin>> string1 ;
    std::cout <<scope_indent <<"String2 : " <<std::flush;
    std::cin>> string2 ;
    int string1_int = atoi(string1.c_str());
    int string2_int = atoi(string2.c_str());
    num = string1_int + string2_int;
    result = to_string(num);
    int size_of_string = result.length();
    int tab[size_of_string];
    for(int i=0;i<size_of_string;i++){
        tab[i]= result[i]-'0';
        //std ::cout << scope_indent <<tab[i]<<std::endl;
    }
    //std ::cout << scope_indent <<size_of_string<<std::endl;
    size_of_string --;
  int i =0;
  while(i != size_of_string-1){
    
    if(tab[size_of_string-i]==2){
        tab[size_of_string-i]=0;
        tab[size_of_string-i-1] +=1;
    }
    i++;
  }
  string final_result ="";
  for(int i=0;i<=size_of_string;i++){
    final_result = final_result + to_string(tab[i]);
  }
  string num1="1";
  if(tab[0]==2){
    num1.append(final_result);
    num1[1]='0';
  }
    std ::cout << scope_indent <<final_result<<std::endl;
    std ::cout << scope_indent <<num1<<std::endl;
    return string1_int;
}

/*
Given an integer x, return true if x is palindrome integer.

An integer is a palindrome when it reads the same backward as forward.

For example, 121 is a palindrome while 123 is not.
*/
bool isPalindrome(int x) {
  int x_copy = x;
  if(x<0 || (x % 10 == 0 && x != 0)){
    return false;
  }
  int inversed_number = 0,reminder;
  while(x>inversed_number){
    reminder = x%10;
    inversed_number = inversed_number*10 + reminder;
    x /=10;
  }
  std ::cout << scope_indent <<x<<"   "<< inversed_number<<std::endl;
  return x == inversed_number || x == inversed_number/10;
    }
/*
Given a string s consisting of words and spaces, return the length of the last word in the string.

A word is a maximal substring consisting of non-space characters only.


*/
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

vector<vector<int>> generate(int numRows) {

        vector<vector<int>> g1;
        for(int i=1;i<=numRows;i++){
          vector <int> v1;          
          for (int j = 0; j < i; j++) {
            v1.push_back(1);}
          g1.push_back(v1);          
          if(i>2){
            for(int number=1;number<g1[i-1].size()-1;number++){
                g1[i-1][number]=g1[i-2][number]+g1[i-2][number-1];
            }
          }
          
        }
          
  return g1;      
    }
int singleNumber(vector<int>& nums) {  
    if(nums.size()==1){return nums[0];}  
    int response;
    sort(nums.begin(), nums.end());
    for(int i =0;i<nums.size();i++){
      if(nums[i]==nums[i+1]){
        i++;
      }else{response=nums[i];}
    }
    return response;

    }
int hammingWeight(uint32_t n) {
      int count = 0;
        while (n){
            n &= n-1;
            count++;
        }
        return count;
    }

int climbStairs(int n) {
        int f1=1;
        int f2=2;
        int fi;
        int swap;
        if(n==1){
            return f1;
        }
        else if (n==2){
            return f2;
        }
        for(int i=3;i<n+1;i++){
            fi = f1 + f2;
            swap = f2;
            f2=fi;
            f1=swap;
        }
        return fi;
    } 

int missingNumber(vector<int>& nums) {
    sort(nums.begin(),nums.end());
    for(int i =0;i<nums.size();i++){
        if(i != nums[i]){
          return i;
        }
    }

     return nums.size();
    }

bool isPowerOfTwo(int n) {
    double x=n;
    if(n==1){
      return true;
    }
    else if (n%2==1){
      return false;
    }
    else{
      while(x>1){
        x=x/2;
        std::cout<<x<<endl;

      }
      if(x==1){return true;}
      else{return false;}
    }
    return false;
    }


char findTheDifference(string s, string t) {
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        for(int index = 0;index<s.length();index++){
            if(s[index] != t[index]){
                return t[index];
            }
        }
        return t[s.length()];
    }




int uniqueMorseRepresentations(vector<string>& words) {
      string traduction;
      vector<string> solutions;
      vector<string> dictionnary_Morse= {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
      for (string word : words){
        traduction="";
        for (char letter : word){
          traduction += dictionnary_Morse[letter-'a'];
        }
        solutions.push_back(traduction);
      }
      sort(solutions.begin(), solutions.end());
      auto it = std::unique(solutions.begin(), solutions.end());

      solutions.resize(std::distance(solutions.begin(), it));
      for(auto i=solutions.begin(); i != solutions.end();++i){
        std::cout<<*i<<endl;
      }
        return sizeof(solutions) ;
    }

/*
int maxProfit(vector<int>& prices) {
        int max_price = 0;
        int profit;
        for(int i=0;i<size(prices)-1;++i){
          for(int j =i+1;j<size(prices);++j){
            profit= prices[j]-prices[i];

           if(profit>max_price){
            max_price=profit;
          }
          }         
        }
    return max_price; 
    }
    */


 bool canConstruct(string ransomNote, string magazine) {
        vector<int>  dictionnary(128);
        
        for(const char letter :magazine ){
            ++dictionnary[letter];
        }
        for(const char letter : ransomNote){
            if(--dictionnary[letter]<0){
                return false;
            }
        }
        return true;
    }



int minCostToMoveChips(vector<int>& position) {
        int count_odd=0;
        int count_even=0;
        for (auto i=position.begin();i != position.end();++i){
          std::cout<<*i<<endl;
          if(*i%2==0){count_even++;}
          else{count_odd++;}
        }
    return min(count_even,count_odd);
    }



vector<vector<int>> diagonalSort(vector<vector<int>>& mat) {
    int m = mat.size();
    int n = mat[0].size();
    vector <int> k;
    for(int row=0 ; row<m;++row){
      k.clear();
      for(int j=0,i=row;j<m && i<n;++j,++i){k.push_back(mat[i][j]);}
      sort(k.begin(),k.end());
      for(int j=0,i=row;j<m && i<n;++j,++i){mat[i][j]=k[i];}

    }
    for(int column =0; column<n; ++column){
      k.clear();
      for (int i =0,j=column; i<n && j<m; ++i,++j){k.push_back(mat[i][j]);}
      sort(k.begin(),k.end());
      for (int i =0,j=column; i<n && j<m; ++i,++j){k.push_back(mat[i][j]=k[i]);}
    }
    return mat;
    }
int numIdenticalPairs(vector<int>& nums) {
        int number_of_pair=0;
        for(auto i= nums.begin(); i != nums.end();++i){
          for(auto j = i+1; j != nums.end();++j){
            if(*i == *j){
              number_of_pair++;
            }
          }
        }
      return number_of_pair;
    }
/*

Given an integer number n, return the difference between the product of its digits and the sum of its digits.

*/
int subtractProductAndSum(int n) {
    int product=1;
    int sum = 0;    
    while(n>0){      
      int rem= n%10;
      sum += rem;
      product *= rem;
      n=n/10;
    }
    return product-sum;
    }

void tracking(vector<vector<char>>& grid,int i , int j , int n ,int m ){
          if(i <0||j<0||i==m||j==n|| grid[i][j]=='0') return ;
          grid[i][j]='0';
          tracking(grid,i+1,j,n,m);
          tracking(grid,i,j+1,n,m);
          tracking(grid,i-1,j,n,m);
          tracking(grid,i,j-1,n,m);
          return;
      }
    
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size();
      int n = grid[0].size();
      int number_of_island=0;
      
      for(int i =0; i != m ;++i){
        for(int j =0; j != n ; ++j){
          if(grid[i][j]=='1'){
            number_of_island++;
            tracking(grid,i,j,n,m);
          }
        }
      }
        return number_of_island;
    }

void rotate(vector<vector<int>>& matrix) {
        
    }
double myPow(double x, int n) {
        double result=1;
        if(x==1){return 1;}
        else if(x==0){return 0;}
        else if(n==0)return 1;
        else{
          for(int i=0;i!=abs(n);++i){
            result *=x;

          }

        }
        if(n<0){return 1/result;}
        else {return result;}
    }
void facto(int &k, int f ){
    if(k%f != 0) return;
    k = k/f;
    facto(k,f);
}
bool isUgly(int n) {
        if(n==0)return false;
        // Copy n which is the input 
        int k=n;
        facto(k,2);
        if(k !=1 ){facto(k,3);}
        if(k !=1){facto(k,5);}
        if(k!=1){return false;}
        return true;


    }

/*
A perfect number is a positive integer that is equal to the sum of its positive divisors, 
excluding the number itself. A divisor of an integer x is an integer that can divide x evenly.
Given an integer n, return true if n is a perfect number, otherwise return false.

*/
bool checkPerfectNumber(int num) {
        int result=0;
        for(int i =1;i<num/2+1;++i){
          if(num%i==0){result +=i;}
        }
        return result==num;
    }



    
int rev(int number){
  int reverse_number=0;
  int i =0;
  int size = trunc(log10(number)) ;
  while(number>0){
      int n = number%10;
      reverse_number += n*pow(10,size-i);
      number = number/10;
      i++;
  }
  while(reverse_number%10==0){
    reverse_number /= 10;
  }
  return reverse_number;

}

int countNicePairs(vector<int>& nums) {
  int result =0;
        for(int i =0;i<nums.size();++i){
          for(int j = i+1;j<nums.size();++j){
              if(rev(nums[i])+nums[j]==rev(nums[j])+nums[i]){
                result +=1;
              }
          }
        }
    return result;
    }

vector<int> syr(int n){
  vector<int> response ;
  response.push_back(n);
  while(*response.end() !=1){
    std::cout<<*response.end()<<endl;
    if(*response.end()%2==0){
      n= n/2;
      response.push_back(n);
    }else{
      n = 3*(*response.end())+1;
      response.push_back(n);
    }
  }
  return response;
}

/*
Given a string s, reverse the order of characters in each word within a sentence while still preserving whitespace and initial word order.

*/
string reverse(string str)
{
    for (int i = str.length() - 1; i >= 0; i--)
        cout << str[i];
    return str;
}
string reverseWords(string s) {
  vector<string> split_list ;
  string reversed_s ;
  string tmp; 
  string separator ="   ";
  stringstream ss(s);
  while(getline(ss, tmp, ' ')){
      split_list.push_back(tmp);
      
  }
  for( string word : split_list){
    std::cout<<word<<endl;
    reversed_s += separator + reverse(word);
  }
    return s;
    }


//g++ -o test -std=c++17 num.cpp leet_code.cpp 
int main(int argc, char* argv[]) {
  string s = "Let's take LeetCode contest";
  string response = reverseWords(s);
  std::cout<<response<<endl;
  
  return 0 ;
}

