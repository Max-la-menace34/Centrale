```cpp

//Runtime: 0 ms, faster than 100.00% of C++ online submissions for Unique Morse Code Words.
//Memory Usage: 8.9 MB, less than 11.70% of C++ online submissions for Unique Morse Code Words.


class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        string traduction;
          vector<string> solutions;
          vector<string> dictionnary_Morse= {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
          for (string word : words){
            traduction="";
            for (char letter : word){
              traduction += dictionnary_Morse[letter-'a'];
            }
            solutions.push_back(traduction);
          }
          sort(solutions.begin(), solutions.end());
          auto it = std::unique(solutions.begin(), solutions.end());

          solutions.resize(std::distance(solutions.begin(), it));
          for(auto i=solutions.begin(); i != solutions.end();++i){
            std::cout<<*i<<endl;
          }
            return size(solutions) ;
    }
};



```
