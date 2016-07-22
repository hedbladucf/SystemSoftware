// Name: Luis Duque and Oscar Hedblad
// PIDs: L2946242 and O3415424
// System Software COP 3402
// Instructor: Euripides Montagne
// Program 3: Parser and Code Generator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "header.h"


int main(int argc, char** argv){

	
	int l, a, v, i, numLines;
	
	
	l = a = v = i = numLines = 0;
	
	for (i = 1; i < argc; i++)
	{
		switch (argv[i][1])
		{
			case 'l':
				l = 1;
				break;
			case 'a':
				a = 1;
				break;
			case 'v':
				v = 1;
				break;
			default:
				break;
		}
	}
	
	scanner(l);
	numLines = parser();
	virtualMachine(a,v, numLines);

	return 0;
}