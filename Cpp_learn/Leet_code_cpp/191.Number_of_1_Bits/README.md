```cpp

//Runtime: 4 ms, faster than 36.92% of C++ online submissions for Number of 1 Bits.
//Memory Usage: 5.8 MB, less than 96.51% of C++ online submissions for Number of 1 Bits.


class Solution {
public:
    int hammingWeight(uint32_t n) {
        int count = 0;
        while (n){
            n &= n-1;
            cout<<n<<endl;
            count++;
        }
        return count;
    }
};


```
