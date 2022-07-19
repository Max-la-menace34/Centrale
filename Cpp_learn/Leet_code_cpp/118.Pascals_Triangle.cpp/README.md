```cpp
#include <iostream> 
#include "num.hpp" 
#include<stdio.h>
#include <vector>

//Runtime: 3 ms, faster than 38.93% of C++ online submissions for Pascal's Triangle.
//Memory Usage: 6.5 MB, less than 71.06% of C++ online submissions for Pascal's Triangle.

// Run the code with the following input (Linux Terminal):
//In the same directory files :
// g++ -o test -std=c++17 num.cpp name_of_the_file.cpp
// Then : ./test



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
    
int main(int argc, char* argv[]) {
  int number_of_row=10;
  vector<vector<int>> response = generate(number_of_row);
  
  for (int i = 0; i < response.size(); i++) {
        for (int j = 0; j < response[i].size(); j++)
            cout << response[i][j] << " ";
        cout << endl;
    }
  return 0 ;
}
    
```
