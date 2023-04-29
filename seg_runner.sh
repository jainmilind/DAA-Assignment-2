#!bin/bash
g++ segmented_least_squares.cpp -o segment.out -g
./segment.out < testcases/$1.txt > ./input_visualizer.txt
python3 visualizer.py
