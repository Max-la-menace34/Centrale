```cpp

//Runtime: 50 ms, faster than 15.83% of C++ online submissions for N-ary Tree Level Order Traversal.
//Memory Usage: 11.9 MB, less than 34.37% of C++ online submissions for N-ary Tree Level Order Traversal.
    


/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> result;
        if (!root) return result;
        queue<Node*> queue;
        queue.push(root);
        while(!queue.empty()){
            int size = queue.size();
            vector<int> line;
            while(size --){
                
                auto ptr = queue.front();
                queue.pop();
                line.push_back(ptr->val);
                
                for(auto it :ptr ->children){
                    queue.push(it);
                }
            }
            result.push_back(line);
        }
    return result;
    }
};

```
