#ifndef CONFIG_H
#define CONFIG_H

//====================================================================================
// Defines and Macros
//====================================================================================
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WORLD_WIDTH 2048
#define WORLD_HEIGHT 2048

#define CAMERA_SMOOTHNESS 5.0f
#define CAMERA_LOOK_AHEAD_FACTOR 0.25f
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

#endif 