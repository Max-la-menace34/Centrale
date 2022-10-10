This is a tutorial for entering into C++, you are supposed to do the
exercises one by one, in the order suggested by the filenames. This is
self-study but you are welcome to ask questions by e-mail or to visit
the C++ teachers.

Work as a group if possible.

There is no other support than the .cpp files, which can be compiled
and executed. Every instruction is in the comments. You won't be asked
to produce code, you will only have to read and understand the given
examples.

You will need a good text file editor, since such tools highlight the
code nicely. You can use "visual studio code", geany, emacs, vim... or
any other that handles really C++ editing. We recommand "visual studio
code" if you do not have already your favorite editor.

Even if everything is given, you can try to modify the examples, make
errors on purpose, in other to see how the compiler or the execution
reacts. You can give this self-study an experimental flavor easily.

The content of the current directory is :

- README.txt : this file
- num.hpp : this defines a type of data called "num". We will use it
     in the examples instead of standard type for integer values
     "int". Using "num" instead of "int" generates outputs on the
     terminal, telling what happens to your data (when they are
     created in the memory, deleted, ...). The "num" behaves as "int"
     and many other types, but when a data is handled during the
     execution of a program, it does not write its story on the
     terminal... "num" actually does, so that you understand what
     happens to any data, even if they are silent. THERE IS NO NEED
     FOR YOU TO READ THE CONTENT OF THIS FILE.
- *.cpp : example files. Read them one by one in the order suggested
     by their names.

C++ is a compiled language. It means that you write your code in a
text file (.cpp usually), and then you ask the compiler (g++, clang,
...) to make a binary executable file from the .cpp text file you have
written. This is compiling.

So, let us suppose you have a foo.cpp file, the following command
makes the "test" executable from it.

g++ -o test -std=c++17 num.cpp foo.cpp

Then, execute "test" by launching the following command on the
terminal

./test

Fasten your seatbelt...

