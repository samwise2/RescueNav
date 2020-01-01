# Simulated-Annealing-Travelling-Salesman
Simulated Annealing for Disaster Management.

The input for this program is given by a text file which contains a different integer on each line
The first integer is read as an x-coordinate of a city, and the second as its y-coordinate pair. This is intended to represent longitude and latitutde coordinates of victims in a rescure sitiuation.

The program will continue to read through coordinate pairs until the end of the file.

The program will then calculate the most efficent recovery path.

Ex:

coords.txt contains:
2
4
5
-8

This would be read as (2,4) and (5, -8) as x,y coordinates of the cities which need to be visited by the salesman.
An example file is included.

This program uses simulated annealing in order to create a heuristic solution to the problem. 
Once the algorithm reaches its end the program will output:
The length of the initial route, the new path and its length, and the overall improvement percentage.
