```cpp
//Runtime: 5 ms, faster than 51.22% of C++ online submissions for Find the Difference.
//Memory Usage: 6.5 MB, less than 98.85% of C++ online submissions for Find the Difference.

class Solution {
public:
    char findTheDifference(string s, string t) {
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        for(int index = 0;index<s.length();index++){
            if(s[index] != t[index]){
                return t[index];
            }
        }
        return t[s.length()];
    }
};


```
