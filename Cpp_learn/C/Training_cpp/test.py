
from collections import namedtuple
from itertools import count
from math import sqrt
from pickle import FALSE, TRUE
from re import T
from urllib import response
import numpy as np
from numpy.linalg import inv

def plusOne(digits):
        """
        :type digits: List[int]
        :rtype: List[int]
        """
        length_digits=len(digits)
        if(digits[length_digits-1] <9):
            digits[length_digits-1]=digits[length_digits-1]+1
            return digits
        k=length_digits-1
        while digits[k]==9:
            digits[k]= 0
            if k==0:
                digits = [1,*digits]
                return digits
            k= k-1        
        
        digits[k]=digits[k]+1
        return digits
def addBinary(a, b):
        """
        :type a: str
        :type b: str
        :rtype: str
        """
        if(a==""):
            return b 
        elif(b==""):
            return a
        elif(b=="" and a==""):
            return False
        a = int(a)
        b=int(b)
        result = str(a+b)
        result = list(result)
        result = [int(i) for i in result]
        length_string = len(result)-1
        
        while(length_string >= 1):
            if(result[length_string]==2):
                result[length_string]=0
                result[length_string-1] +=1
            length_string -=1
        string=""
        if(result[0]==2):
            result[0]=0
            for i in result :
                string = string + str(i)

            result = "1"+ string
        else:
            for i in result :
                string = string + str(i)
            return string
        return result




def lengthOfLastWord(s: str) -> int:
    s = s.split(" ")
    length_s = len(s);
    for index in range(length_s) :
        if s[length_s-1-index] != "":
            return len(s[length_s-1-index])

    return len(s)


def mySqrt(x: int) -> int:
    result = x
    while  result * result - x >= 1:
        result = (result + x / result) / 2
        print(result)

    return int(result)


def generate(numRows: int):
    pascal_triangle=[];
    for index in range(1,numRows+1,1):
        pascal_triangle.append([1]*index)
        if(index>2):
            for number in range(1,len(pascal_triangle[index-1])-1,1):
                pascal_triangle[index-1][number]=pascal_triangle[index-2][number]+pascal_triangle[index-2][number-1]
    for number in pascal_triangle:
        print(number)
    return pascal_triangle





def singleNumber(nums):
    counts = dict()
    for item in nums:
        counts[item]=counts.get(item,0)+1
    return list(counts.keys())[list(counts.values()).index(1)]
list_exercicce = [4,1,2,1,2]

    
def hammingWeight(n: int) -> int:
    n=list(str(n))
    count=0
    for i in n:
        if i =="1":
            count +=1
    return count


def isValid(s):
    stack = []
    dict = {"]":"[", "}":"{", ")":"("}
    for char in s:        
        if char in dict.values():
            stack.append(char)
        elif char in dict.keys():
            if stack == [] or dict[char] != stack.pop():
                return False
        else:
            return False
    return stack == []

def climbStairs(n: int) -> int:
    f_1=1
    f_2=2
    if n==1:
        return f_1
    if n==2:
        return f_2
    for i in range(3,n+1,1):
        f_i = f_1 +f_2
        f_2,f_1=f_i,f_2
    return f_i


def missingNumber(nums):
    nums = list(set(nums))
    print(nums)
    for i in range(len(nums)):
        if(nums[i] != i):
            return i
    return i+1
    

def isPowerOfTwo(n: int) -> bool:
    if(n%2 ==1):
        return False
    else:
        while(n>1):            
            n = n/2 
    if(n==1):
        return True
    else:
        return False
    return False
def findTheDifference(s: str, t: str) -> str:
    s= sorted(s)
    t = sorted(t)
    for index in range(len(t)):
        if index==len(s):
            return t[-1]
        if t[index] != s[index]:
            return t[index]
    return t[-1]

def uniqueMorseRepresentations(words) -> int:
    List_of_Morse = [".-","-...","-.-.","-..",".","..-.","--.",
                 "....","..",".---","-.-",".-..","--","-.",
                 "---",".--.","--.-",".-.","...","-","..-",
                 "...-",".--","-..-","-.--","--.."]
    solution = ["".join(List_of_Morse[ord(letter)-ord('a')] for letter in word) for word in words]
    return len(set(solution))



def canConstruct_1(ransomNote: str, magazine: str) -> bool:
        dic_ransomNote = {x:ransomNote.count(x) for x in ransomNote}
        dic_magazine = {x:magazine.count(x) for x in magazine}
        for letter in dic_ransomNote.keys():
            if dic_ransomNote.get(letter) > dic_magazine.get(letter,False):
                return False
        return True

def canConstruct_2(ransomNote: str, magazine: str) -> bool:
        dictionnary = [0]*26
        for letter in magazine:
            dictionnary[ord(letter)-97] += 1
        for letter in ransomNote:
            dictionnary[ord(letter)-97]  -= 1
            if  dictionnary[ord(letter)-97]<0:
                return False
        return True
def numIslands(grid) -> int:
    number_of_islande =0
    m= len(grid)
    n= len(grid[0])
    def balayage(i,j):
        if (i<0 or j<0 or i==m or j==n or grid[i][j]=="0"):
            return 
        grid[i][j]="0"
        balayage(i,j+1)
        balayage(i,j-1)
        balayage(i+1,j)
        balayage(i-1,j)
        return 

    for i in range(m):
        for j in range(n):
            if(grid[i][j]=="1"):
                number_of_islande +=1
                balayage(i,j)
    return number_of_islande

def pacificAtlantic(heights):
    result=[];
    m = len(heights)
    n= len(heights[0])
    #Atlantic : en bas (i = m-1) ou la droite (j = n-1)
    def Atlantic(i,j):
        h=heights[i][j]
        if(j==n-1 or i==m-1):
            return True
        if(i>0 and h>=heights[i-1][j]):
            Atlantic(i-1,j)
        if(j>0 and h>=heights[i][j-1]):
            Atlantic(i,j-1)
        if(h>=heights[i+1][j]):
            Atlantic(i+1,j)
        if(h>= heights[i][j+1]):
            Atlantic(i,j+1)
        return False
    #Pacific : en haut (i=0) ou la gauche (j=0)
    def Pacific(i,j):
            h=heights[i][j]
            if(i==0 or j==0):
                return True
            if(i>0 and h>=heights[i-1][j]):
                Atlantic(i-1,j)
            if(j>0 and h>=heights[i][j-1]):
                Atlantic(i,j-1)
            if(h>=heights[i+1][j] and i<m):
                Atlantic(i+1,j)
            if(h>= heights[i][j+1] and j<n):
                Atlantic(i,j+1)
            return False
    
    for i in range(m):
        for j in range(n):
            i_prev= i;
            j_prev = j;
            bool_atlantic = Atlantic(i_prev,j_prev)
            bool_pacific = Pacific(i_prev,j_prev)
            # if Pacific and Atlantic return true alors on ajoute les coordonnés à result 
            if(bool_atlantic and bool_pacific):
                result.append([i,j])
    return result


def countDigitOne(n: int) -> int:
    compteur =0
    while(i>0):
        n = i%10
        if n==1:
            compteur +=1
        i = int(i/10)
    return compteur 

def reverseWords(s):
    reversed_s ="";
    s = s.split(" ")
    for word in s :
        print(word)
        word_reverse = word[::-1]
        reversed_s = reversed_s + " " + word_reverse
    return reversed_s[1::]

s = "Let's take LeetCode contest"
print(reverseWords(s))