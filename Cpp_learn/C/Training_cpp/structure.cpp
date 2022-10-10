#include <optional>
#include <iostream> 
#include "num.hpp" 
#include<stdio.h>
#include <iomanip>




struct Etudiant{
    int age;
    char name[10];
    float wieght;
    bool married;
};
void display_informations(Etudiant& name){
    std::cout << name.age<<std::endl;
    std::cout << name.name<<std::endl;
    std::cout << name.wieght<<std::endl;
    std::cout << std::boolalpha << name.married << std::endl;
}

void fill_informations(Etudiant &name){
    name.age=22;
}

struct grille{
int x;
int y;
int z ;
grille(int x, int y ,int z);
grille(const grille&);
grille();
void reset();
void display_grille();
grille operator+(const grille&) const;
};







grille:: grille(int x, int y , int z): x(x),y(y),z(z) {}
grille:: grille(const grille& other_point) : x(other_point.x),y(other_point.y),z(other_point.z){}
grille:: grille(): x(0),y(0),z(0) {}
void grille ::reset(){
    this -> x  =0;
    this ->y = 0;
    this -> z =0;
}
void grille::display_grille(){
    std::cout <<"("<< this->x <<","<< this->y<<"," <<this->z << ")"<<std::endl;
}
grille grille::operator+(const grille& other_point) const{
    grille resutl {this->x + other_point.x,this-> y + other_point.y, this->z + other_point.z};
    return resutl;
}
    
struct points{
    double x ;
    double y ;
    //points():x(0),y(0){}


};

struct Segment{
    points A;
    points B;
    Segment(const points& A, const points&B): A(A),B(B){}

};

auto operator&(const Segment& s1, const Segment& s2){
    std::optional<points> intersection; // Cette ligne fonction mais compilateur pas content
  // fake math here....
  if(s1.A.x < s2.B.y) intersection = {s1.B.x, s2.A.y}; 
  return intersection;
}


//g++ -o test -std=c++17 num.cpp structure.cpp 
int main(int argc, char* argv[]){
    /*
    struct Etudiant Maxime={20,"Maxime",63.5,false};
    fill_informations(Maxime);
    display_informations(Maxime);
    
   grille a {10,3,5};
   grille b {-a.y,a.x,a.z};
   grille result  = a+b;
   result.display_grille();
   grille c;
   a.reset();
   a.display_grille();
   c.display_grille();*/
  points A = {2.3, 4.8};
  points B = {1.0, 1.2};
  points C = {5.5, 4.1};
  points D = {0.0, 0.5};

  if(auto oI = Segment(A,B) & Segment(C,D); oI) { // if(def-init; test)
    auto& I = *oI;
    std::cout << "Intersection at " << I.x << ',' << I.y << std::endl;
  }
  else
    std::cout << "No intersection" << std::endl;
    
  return 0;

}
