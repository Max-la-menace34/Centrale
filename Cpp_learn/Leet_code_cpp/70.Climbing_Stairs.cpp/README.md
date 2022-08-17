```cpp

//Runtime: 0 ms, faster than 100.00% of C++ online submissions for Climbing Stairs.
//Memory Usage: 6 MB, less than 56.42% of C++ online submissions for Climbing Stairs.

class Solution {
public:
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
};


```
