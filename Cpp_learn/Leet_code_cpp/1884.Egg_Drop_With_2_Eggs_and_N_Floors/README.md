```cpp
//Runtime: 3 ms, faster than 81.36% of C++ online submissions for Egg Drop With 2 Eggs and N Floors.
//Memory Usage: 6 MB, less than 49.95% of C++ online submissions for Egg Drop With 2 Eggs and N Floors.


class Solution {
public:
    int twoEggDrop(int n) {
        double x= round(sqrt(n));
        while((pow(x,2)+x)/2 <n){
            x++;
        }
        return x;
    }
};




```
