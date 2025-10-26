# ifndef __STATEVARS_H
# define __STATEVARS_H

# include <stdio.h>


volatile struct Position
{
	/*
	The position in millimeters
	*/
	float posX, posY;

	/*
	The orientation in degrees.
	Uses trigonometric orientation with +x as zero, +y as 90°, ect
	*/
	volatile float orientation;
};


extern struct Position position = {.posX = 0, .posY = 0, .orientation = 0};



# endif // __STATEVARS_H