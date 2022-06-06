/*
	File Name: PlayList.cpp
	Date: November 6, 2018
	Name: Nancy Yoon
	Class: CSCI 235 - Fall 2018
	[Project 4] MazeSolver
		: maze solver will read the maze from an input file,
		  find a path that leads to the exit and print the solution to the standard out
		  (solution = maze with highlighted path from start position to exit)
*/
#include "MazeSolver.h"
#include <cctype>
#include <stack>

MazeSolver::MazeSolver(std::string input_file)
{
	std::ifstream in_stream;
	in_stream.open(input_file);

	if(in_stream.fail())
	{
		std::cout << "Cannot read from input_file" << std::endl;
		exit(1);
	}
	else
	{
		in_stream >> maze_rows_ >> maze_columns_;
		initializeMaze(maze_rows_, maze_columns_);

		fillMaze(in_stream);
		initializeSolution();
		maze_ready = true;
	}//end of else loop
	in_stream.close();
}// end of constructor

MazeSolver::~MazeSolver()
{
	if(maze_ready != true)
	{
		delete [] solution_;
		delete [] maze_;
	}
	else
	{
		delete [] *solution_;
		delete [] solution_;
		delete [] *maze_;
		delete [] maze_;
	}
}//end of destructor

bool MazeSolver::mazeIsReady()
{
	if(maze_ready == true)
		return true;
	return false;
}//end of mazeIsReady

//STUB
bool MazeSolver::solveMaze()
{
	Position current_position;
	current_position.row = 0;
	current_position.column = 0;

	while(!backtrack_stack_.empty())
	{
		if(maze_[current_position.row][current_position.column] == '$')
		{
			std::cout << "Found exit!!!" << std::endl;
			return true;
		}
		else if(extendPath(current_position))
		{

			current_position = backtrack_stack_.top();
		}
		else
		{
			maze_[current_position.row][current_position.column] = 'X';
			solution_[current_position.row][current_position.column] = '@';
			backtrack_stack_.pop();

			if(!backtrack_stack_.empty())
			{
				current_position = backtrack_stack_.top();
			}
			else
			{
				std::cout << "This maze has no solution." << std::endl;
				return false;
			}
		}
	}
	return true;
}//end of solveMaze

void MazeSolver::printSolution()
{
	std::cout << "The solution to this mase is:" << std::endl;
	for(int i=0; i<maze_rows_; i++)
	{
		for(int j=0; j<maze_columns_; j++)
		{
			std::cout << solution_[i][j] << " ";
		}
		std::cout << std::endl;
	}
}//end of printSolution


//PRIVATE

void MazeSolver::initializeMaze(int rows, int columns)
{
	if(rows >= 0 && columns >= 0)
	{
		maze_ = new char *[rows];
		for(int i=0; i<rows; i++)
			maze_[i] = new char [columns];		
	}//end of if condition
}//end of initializeMaze

void MazeSolver::fillMaze(std::ifstream& input_stream)
{
	for(int i=0; i<maze_rows_; i++)
	{
		for(int j=0; j<maze_columns_; j++)
		{
			input_stream >> maze_[i][j];
		}
	}//end of for loop		
}//end of fillMaze

void MazeSolver::initializeSolution()
{
	copyMazetoSolution();

	Position viable_paths;
	solution_[0][0] = '>';
	viable_paths.row = 0;
	viable_paths.column = 0;
	backtrack_stack_.push(viable_paths);	
}//end of initializeSolution

void MazeSolver::copyMazetoSolution()
{
	solution_ = new char *[maze_rows_];
	for(int i=0; i<maze_rows_; i++)
		solution_[i] = new char [maze_columns_];		

	for(int i=0; i<maze_rows_; i++)
	{
		for(int j=0; j<maze_columns_; j++)
		{
			solution_[i][j] = maze_[i][j];
		}
	}//end of for loop
}//end of copyMazetoSolution

bool MazeSolver::extendPath(Position current_position)
{
	bool extended = false;
	if(isExtensible(current_position, SOUTH))
	{
		backtrack_stack_.push(getNewPosition(current_position, SOUTH));
		if(maze_[current_position.row+1][current_position.column] == '_')
		{
			solution_[current_position.row+1][current_position.column] = '>';			
		}
		else
		{
			solution_[current_position.row+1][current_position.column] = '$';			
		}

        extended = true;
    }
	if(isExtensible(current_position, EAST))
	{
		backtrack_stack_.push(getNewPosition(current_position, EAST));
		if(maze_[current_position.row][current_position.column+1] == '_')
		{
			solution_[current_position.row][current_position.column+1] = '>';			
		}
		else
		{
			solution_[current_position.row][current_position.column+1] = '$';			
		}
		extended = true;
	}
	return extended;
}//end of extendPath

Position MazeSolver::getNewPosition(Position old_position, direction dir)
{
	Position newPosition;
	if(dir == SOUTH)
	{
		newPosition.row = old_position.row+1;
		newPosition.column = old_position.column;
	}
	if(dir == EAST)
	{
		newPosition.row = old_position.row;
		newPosition.column = old_position.column+1;
	}
	return newPosition;
}//end of getNewPosition

bool MazeSolver::isExtensible(Position current_position, direction dir)
{
	bool extensible = false;
	if(dir == SOUTH && current_position.row < maze_rows_ -1)
	{
		if(maze_[current_position.row+1][current_position.column] == '_' || maze_[current_position.row+1][current_position.column] == '$')
			extensible = true;
	}
	if(dir == EAST && current_position.column < maze_columns_ -1)
	{
		if(maze_[current_position.row][current_position.column+1] == '_' || maze_[current_position.row][current_position.column+1] == '$')
			extensible = true;
	}
	return extensible;
}//end of isExtensible

