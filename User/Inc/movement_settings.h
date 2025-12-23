# ifndef __MOVEMENT_SETTINGS_H
# define __MOVEMENT_SETTINGS_H

/**
 * @file movement_settings.h
 * @brief struct that store infos on how the current movement should be done
 */

struct MovementSettings
{
	float speed;
	float distance;
};

/**
 * @brief Variables telling the movement statemachine how the current movement should be done.
 */
extern struct MovementSettings movement_settings;

# endif