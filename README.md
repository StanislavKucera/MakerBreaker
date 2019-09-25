# MakerBreaker

[Installation:]
:~ g++ -std=c++11 -O3 maker-breaker.cpp -o maker-breaker

[Usage:]
As the most basic function, the program expect a description of a multigraph on the standard input in the format:
"n m" where n is the number of vertices and m is the number of edges
"u v" m pairs of 0-(n-1) valued pair of vertices describing the edges separated by spaces or new lines

The program returns either 'M' if Maker wins the Connectivity (1/2,1/2) game on the given graph or 'B' if Breaker wins.


[Examples:]
:~ ./maker-breaker
5 10
0 1
0 2
0 3
0 4
1 2
1 3
1 4
2 3
2 4
3 4
-> M
(K_5 is Maker's win)

:~ ./maker-breaker
2 2
0 1 0 1
-> B
(2 parallel edges are Breaker's win)
