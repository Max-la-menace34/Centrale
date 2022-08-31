```cpp
//Runtime: 2 ms, faster than 43.33% of C++ online submissions for Subtract the Product and Sum of Digits of an Integer.
//Memory Usage: 5.8 MB, less than 71.37% of C++ online submissions for Subtract the Product and Sum of Digits of an Integer.



class Solution {
public:
    int subtractProductAndSum(int n) {
        int product=1;
    int sum = 0;    
    while(n>0){      
      int rem= n%10;
      sum += rem;
      product *= rem;
      n=n/10;
    }
    return product-sum;
    }
};

```
