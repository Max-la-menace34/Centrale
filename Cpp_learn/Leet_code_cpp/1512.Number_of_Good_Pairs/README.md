```cpp
//Runtime: 3 ms, faster than 58.09% of C++ online submissions for Number of Good Pairs.
//Memory Usage: 7.2 MB, less than 92.28% of C++ online submissions for Number of Good Pairs.

class Solution {
public:
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
};

```
