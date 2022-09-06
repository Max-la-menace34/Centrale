```cpp


//Runtime: 3 ms, faster than 58.56% of C++ online submissions for Ugly Number.
//Memory Usage: 5.7 MB, less than 98.97% of C++ online submissions for Ugly Number.



class Solution {
public:
    
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
};





```
