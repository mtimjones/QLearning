# QLearning
Reinforcement Learning / Q Learning

This is a sample demonstration of Q Learning (reinforcement learning).

# Building and Running

Linux source can be built using Linux with make.  Resulting executable is called qlearn.

```
$ make
gcc -c -o main.o main.c -I. -std=c99 -Wall
gcc -g -o qlearn main.o -I. -std=c99 -Wall -lm
$ ./qlearn 

+ - - - - - - - - - - - - - - - - - - + 
| .               # # #   # # # # #   | 
| . .   # # #     # # #   # # # # #   | 
| # .     # # #           # #         | 
| # .       # # # # # # # #           | 
| # .       # # # # # # # #   # #     | 
|   .   # # #         # #       # #   | 
|   . # # # # #       # #     # # #   | 
|   . # #                       # #   | 
|   . . . . . . . .   # # #     # #   | 
|   #       # # # .   # # #   # # # # | 
| # # #     # # # . .   # #   # $ . . | 
| # # #   # # # #   . . . . . # # # . | 
| # # #     # # #   # # # # . # # # . | 
| # # #       # #   # # #   . . # # . | 
| # #           #   #       # . # # . | 
| #       # #           # # # . # # . | 
|       # # # #         # # # . . . . | 
|       # # # # #                     | 
+ - - - - - - - - - - - - - - - - - - + 

$
```
