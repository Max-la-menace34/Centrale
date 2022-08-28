```cpp

//Runtime: 4 ms, faster than 44.05% of C++ online submissions for Minimum Cost to Move Chips to The Same Position.
//Memory Usage: 7.4 MB, less than 9.59% of C++ online submissions for Minimum Cost to Move Chips to The Same Position.


class Solution {
public:
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
};

```
