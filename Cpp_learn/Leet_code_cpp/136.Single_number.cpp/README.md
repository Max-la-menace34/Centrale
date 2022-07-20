```cpp
#include <iostream> 
#include "num.hpp" 
#include<stdio.h>
#include<vector>
using namespace std;

//Runtime: 38 ms, faster than 34.87% of C++ online submissions for Single Number.
//Memory Usage: 16.9 MB, less than 51.54% of C++ online submissions for Single Number.

// Run the code with the following input (Linux Terminal):
//In the same directory files :
// g++ -o test -std=c++17 num.cpp name_of_the_file.cpp
// Then : ./test


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

int main(int argc, char* argv[]) {
  vector<int> input={2,4,2,1,1};
  int response = singleNumber(input);
  std::cout<<response<<endl;
  return 0 ;
}

```
