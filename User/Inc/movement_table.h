# ifndef __MOVEMENT_TABLE_HH
# define __MOVEMENT_TABLE_HH

# include "stm32g431xx.h"

# define MOVEMENT_TICK_PERIOD_MS 20

enum MVCODE
{
	OK = 0,
	TIMEOUT = 1
};

struct MovementControl
{
	float distance;
	float speed;
	float timeout;
	int exit_code;
};

# endif