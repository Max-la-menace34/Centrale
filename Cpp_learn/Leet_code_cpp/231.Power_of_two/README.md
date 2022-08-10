```cpp
// First we check if n is an odd number => return false 
// Second step we devide x times n by two , 
// If the reminder is equal to 1 => return false , else => return true

class Solution {
public:
    bool isPowerOfTwo(int n) {
        double x=n;
        if(n==1){
          return true;
        }
        else if (n%2==1){
          return false;
        }
        else{
          while(x>1){
            x=x/2;
            std::cout<<x<<endl;

          }
          if(x==1){return true;}
          else{return false;}
        }
        return false;
    }
};

```
