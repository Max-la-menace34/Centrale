```cpp

//Runtime: 29 ms, faster than 60.26% of C++ online submissions for Missing Number.
//Memory Usage: 17.9 MB, less than 93.43% of C++ online submissions for Missing Number

// In this case we sort our vector
// Then we check if the number is equal to its rank
// If not we return de the number, in the other hand we return the vector size 

class Solution {
public:
    int missingNumber(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        for(int i =0;i<nums.size();i++){
            if(i != nums[i]){
              return i;
            }
        }

         return nums.size();
    }
};


```
