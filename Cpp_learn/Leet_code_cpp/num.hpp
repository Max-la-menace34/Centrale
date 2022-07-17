#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

#define num_SCOPE_INCR_SIZE  4
#define num_NAME_WIDTH       7
#define num_VALUE_WIDTH      4
#define num_METHOD_WIDTH    45

#define UNICODE_DECORATION

class scope_ {
private:
  
  std::string name;
  
public:

  static int depth;

  static std::string depth_str() {
	  std::string str = "\e[0m\e[94m";
	  for(int i = 0 ; i < depth - 1; ++i)
#ifndef UNICODE_DECORATION
	  	str += " |  ";
#else
	  str += " \u2502  ";
#endif
	  str += "\e[0m";
	  return str;
  }

  scope_(const std::string& s) : name(s) {
    if(depth > 0)
      std::cout << depth_str() 
		<< "\e[1m\e[94m "
#ifndef UNICODE_DECORATION
		<< "+- "
#else
		<< "\u250C\u2500 "
#endif
		<< name << "\e[0m" << std::endl;
    else
      std::cout << "\e[1m\e[94m"
#ifndef UNICODE_DECORATION
		<< "--- "
#else
		<< "\u2500\u2500\u2500 "
#endif
		<< name << "\e[0m" << std::endl;
    ++depth;
  }
  ~scope_() {
	--depth;
    if(depth > 0)
      std::cout << depth_str() 
		<< "\e[1m\e[94m "
#ifndef UNICODE_DECORATION
		<< "+- "
#else
		<< "\u2514\u2500 "
#endif
		<< "\e[0m\e[94m(" << name << ")\e[0m" << std::endl;
    else
      std::cout << "\e[1m\e[94m"
#ifndef UNICODE_DECORATION
		<< "--- "
#else
		<< "\u2500\u2500\u2500 "
#endif
		<< "\e[0m\e[94m(" << name << ")\e[0m" << std::endl;
  }
};


#define fun_scope  scope_ ssssss(__func__)
#define scope(msg) scope_ ssssss(msg)
#define ___ thebar()

inline void rem(const std::string msg) {
  std::cout << scope_::depth_str() << "\e[94m//  " << msg << "\e[0m" << std::endl;
}

inline void thebar() {
  int length = std::max(num_NAME_WIDTH + num_VALUE_WIDTH + num_METHOD_WIDTH - num_SCOPE_INCR_SIZE*scope_::depth, 0) + 30;
#ifndef UNICODE_DECORATION
  std::cout << scope_::depth_str() << "\e[94m" << std::string(length, '~') << "\e[0m" << std::endl;
#else
  std::cout << scope_::depth_str() << "\e[94m";
  for(int i = 0; i < length; ++i)
    std::cout << "\u2500";
  std::cout << "\e[0m" << std::endl;
#endif
}
	
										
inline std::ostream& scope_indent(std::ostream& os) {
  os << scope_::depth_str();
  return os;
}									

class num {
private:
  
  enum class mode : char {
    Allocate = 'a',
    Free     = 'f',
    Modify   = 'm'
  };
  
  int value;
  std::string name;

  std::string idf() {
    std::ostringstream ofs;
    ofs << '#' << (reinterpret_cast<uintptr_t>(this) % 8192);
    return ofs.str();
  }

  void display(mode m, const std::string& method, const std::string& msg) {
    switch(m) {
    case mode::Allocate :
      std::cout << scope_::depth_str() << "\e[1m\e[92m+++ " << std::setw(num_NAME_WIDTH) << std::left << name << "\e[0m";
      break;
	case mode::Free : 
      std::cout << scope_::depth_str() << "\e[1m\e[95m--- " << std::setw(num_NAME_WIDTH) << std::left << name << "\e[0m";
      break;
	case mode::Modify :
	  std::cout << scope_::depth_str() << "\e[1m\e[94m=== " << std::setw(num_NAME_WIDTH) << std::left << name << "\e[0m";
      break;
    }
    std::cout << " \e[1m\e[94m" << std::setw(num_VALUE_WIDTH) << std::right;
    if(value == std::numeric_limits<int>::max())
      std::cout << "?!?";
    else
      std::cout << value;
    std::cout << "  "
	      << std::setw(std::max(0, num_METHOD_WIDTH - num_SCOPE_INCR_SIZE*scope_::depth)) << std::left << method << " // "
	      << msg
	      << ".\e[0m" << std::endl;
  }

  num(int res, const std::string& op, const num& arg1, const num& arg2) : value(res), name() {
    name = idf();
    display(mode::Allocate, arg1.name + op + arg2.name, "binary operator");
  }

  num(int res, const std::string& op, const num& arg1, int arg2) : value(res), name() {
    name = idf();
    display(mode::Allocate, arg1.name + op + "int(" + std::to_string(arg2) + ")", "binary operator");
  }

  num(int res, const std::string& op, const num& arg1) : value(res), name() {
    name = idf();
    display(mode::Allocate, op + arg1.name, "unary operator");
  }

  void make_moved() {
    value = std::numeric_limits<int>::max();
    name = std::string("(") + name + std::string(")");
  }
   
public:
  
  num() : value(std::numeric_limits<int>::max()), name() {
    name = idf();
    display(mode::Allocate, "by default", "uninitialized");
  }
  
  num(const char* name, int value) : value(value), name(name) {
    std::ostringstream ostr;
    display(mode::Allocate, std::string("int(") + std::to_string(value) + ")", "from int value");
  }
  
  num(const char* name) : num(name, 0) {}
  
  num(const std::string& name, int value) : value(value), name(name) {
    std::ostringstream ostr;
    display(mode::Allocate, std::string("int(") + std::to_string(value) + ")", "from int value");
  }
  
  num(const std::string& name) : num(name, 0) {}

  num(int value) : num("X", value) {}
  
  num(const num& cp) : value(cp.value), name(cp.name + "'") {
    display(mode::Allocate, "by copy", std::string("from ") + cp.name);
  }
  
  num(num&& cp) : value(cp.value), name(cp.name) {
    display(mode::Allocate, "by move", std::string("from ") + cp.name);
    cp.make_moved();
  }

  ~num() {
    display(mode::Free, "", "memory free");
  }

  operator int         () const {return value;}
  operator std::string () const {return name;}

  num& operator=(const num& cp) {
    value = cp.value;
    display(mode::Modify, name + "=" + cp.name, "assignment");
    
    return *this;
  }
  
  num& operator=(num&& cp) {
    value = cp.value;
    display(mode::Modify, name + "=move(" + cp.name + ")", "assignment");
    cp.make_moved();
    return *this;
  }
  
  num& operator=(int v) {
    value = v;
    display(mode::Modify, name + "=" + std::to_string(v), "assignment from int");
    
    return *this;
  }
  
  num& operator++() {
    ++value;
    display(mode::Modify, "++", "modification (+1)");
    return *this;
  }
  
  num operator++(int) {
    num tmp(*this);
    operator++(); 
    return tmp;  
  }
  
  num& operator--() {
    --value;
    display(mode::Modify, "--", "modification (-1)");
    return *this;
  }
  
  num operator--(int) {
    num tmp(*this);
    operator--(); 
    return tmp;  
  }
  
  num operator+() const {
    return num(value, "+", *this);
  }
  
  num operator-() const {
    return num(-value, "-", *this);
  }
  
  num operator+(const num& other) const {
    return num(value+other.value, "+", *this, other);
  }
  
  num operator-(const num& other) const {
    return num(value-other.value, "-", *this, other);
  }

  num operator*(const num& other) const {
    return num(value*other.value, "*", *this, other);
  }

  num operator/(const num& other) const {
    return num(value/other.value, "/", *this, other);
  }
  
  num operator+(int v) const {
    return num(value+v, "+", *this, v);
  }
  
  num operator-(int v) const {
    return num(value-v, "-", *this, v);
  }

  num operator*(int v) const {
    return num(value*v, "*", *this, v);
  }

  num operator/(int v) const {
    return num(value/v, "/", *this, v);
  }
  
  num& operator+=(const num& other) {
    value += other.value;
    display(mode::Modify, name + "+=" + other.name, "modification (+"+std::to_string(other.value)+')');
    return *this;
  }
  
  num& operator-=(const num& other) {
    value -= other.value;
    display(mode::Modify, name + "-=" + other.name, "modification (+"+std::to_string(other.value)+')');
    return *this;
  }
  
  num& operator*=(const num& other) {
    value *= other.value;
    display(mode::Modify, name + "*=" + other.name, "modification (*"+std::to_string(other.value)+')');
    return *this;
  }
  
  num& operator/=(const num& other) {
    value /= other.value;
    display(mode::Modify, name + "/=" + other.name, "modification (/"+std::to_string(other.value)+')');
    return *this;
  }
  
  num& operator+=(int v) {
    value += v;
    display(mode::Modify, name + "+=int(" + std::to_string(v) + ")", "modification (+"+std::to_string(v)+')');
    return *this;
  }
  
  num& operator-=(int v) {
    value -= v;
    display(mode::Modify, name + "-=int(" + std::to_string(v) + ")", "modification (-"+std::to_string(v)+')');
    return *this;
  }
  
  num& operator*=(int v) {
    value *= v;
    display(mode::Modify, name + "*=int(" + std::to_string(v) + ")", "modification (*"+std::to_string(v)+')');
    return *this;
  }
  
  num& operator/=(int v) {
    value /= v;
    display(mode::Modify, name + "/=int(" + std::to_string(v) + ")", "modification (/"+std::to_string(v)+')');
    return *this;
  }

  bool operator<(const num& other) const {
    return value < other.value;
  }

  bool operator<(int other) const {
    return value < other;
  }

  bool operator<=(const num& other) const {
    return value <= other.value;
  }

  bool operator<=(int other) const {
    return value <= other;
  }

  bool operator>(const num& other) const {
    return value > other.value;
  }

  bool operator>(int other) const {
    return value > other;
  }

  bool operator>=(const num& other) const {
    return value >= other.value;
  }

  bool operator>=(int other) const {
    return value >= other;
  }

  bool operator!=(const num& other) const {
    return value != other.value;
  }

  bool operator!=(int other) const {
    return value != other;
  }

  bool operator==(const num& other) const {
    return value == other.value;
  }

  bool operator==(int other) const {
    return value == other;
  }

  
  friend std::ostream& operator<<(std::ostream&, const num&);
};



inline std::ostream& operator<<(std::ostream& os, const num& i) {os << i.value; return os;}

inline num operator+  (int a, const num& b) {return b + a;}
inline num operator*  (int a, const num& b) {return b * a;}
inline num operator-  (int a, const num& b) {return -(b - a);}
inline bool operator< (int a, const num& b) {return b >= a;}
inline bool operator<=(int a, const num& b) {return b > a;}
inline bool operator> (int a, const num& b) {return b <= a;}
inline bool operator>=(int a, const num& b) {return b < a;}
inline bool operator==(int a, const num& b) {return b == a;}
inline bool operator!=(int a, const num& b) {return b != a;}
