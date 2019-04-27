#!/bin/bash
clear

g++ -c ./main.cpp
g++ -g ./main.cpp
mv a.out debug.out

g++ -Ofast -o task.out main.o

./task.out