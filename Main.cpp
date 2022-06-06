#include <iostream>
#include "MazeSolver.h"

int main()
{
	MazeSolver solver("Input.txt");

	if(solver.mazeIsReady())
	{
		solver.solveMaze();
		solver.printSolution();
	}//end of if loop

	return 0;
}//end of main