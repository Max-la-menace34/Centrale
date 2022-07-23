```cpp

//Runtime: 7 ms, faster than 21.51% of C++ online submissions for Reverse Linked List II.
//Memory Usage: 7.6 MB, less than 17.76% of C++ online submissions for Reverse Linked List II.

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        if(!head || !head->next) return head;
        ListNode* dummy = new ListNode(-1);
        dummy->next = head;
        int compteur =0;
        int stop = (right-left)/2;
        int length =0;
        // Caclul longeur de la liste
        struct ListNode* first = head;
        while(head != NULL){
            head = head->next;
            length++;
        }
        int tab[length+1];
        //Set le pointeur au début et remplie le tableau
        head=first;
        while(head != NULL){
            tab[compteur]=head->val;
            //ut<<head->val<<"";
            head = head->next;
            compteur++;
        }
        int i =0;
        int change1;
        while(i<=stop){
            change1 = tab[i+left-1];
            tab[i+left-1]=tab[right-i-1];
            tab[right-i-1] = change1;
            i++;
        }
        head = first;
        int compteur_final = 0;
        while(head!=NULL){
            head->val=tab[compteur_final];
            cout<<head->val<<"";
            head = head->next;
            
            compteur_final ++;
        }
    return dummy->next;
    }
};




```
