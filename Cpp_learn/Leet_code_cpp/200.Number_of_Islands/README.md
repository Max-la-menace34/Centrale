```cpp
//Runtime: 77 ms, faster than 20.89% of C++ online submissions for Number of Islands.
//Memory Usage: 12.2 MB, less than 97.03% of C++ online submissions for Number of Islands.

class Solution {
public:
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
};

```
