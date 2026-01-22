# ifndef __TRANSFORM_H
# define __TRANSFORM_H

struct Transform
{
	/** Horizontal position in mm */
	float _x;

	/** Vertical position in mm */
	float _y;

	/** The rotation, in degrees, in the trigonometric direction. + */
	float _z;

	/** When the transform is mirrored, all setters still work  */
	int _mirror;
};

# endif