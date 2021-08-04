# Mini-Python-Interpreter

Language: C++

A program which acts as a Python interpreter for basic functionalities.
Reads a python file and interprets it. Works well with setting variables to integers and strings and overriding them.
Performs addition or subtraction of integers or variables that are integers.
Performs addition of strings or variables that are strings.
Capable of reading if/else statements with ==, <, >, <=, and >=.
Capable of reading one nested if/else statements.

Examples of Python programs in which the Mini_Python program is able to interpret are in the Testcases folder.

Compile in Linux:
g++ o- mini_python mini_python.cpp

Run program in Linux:
./mini_python tc1.py

Bugs:
  Lines inside of the if/else only works if it starts with two spaces and the nested if/else with four spaces.
