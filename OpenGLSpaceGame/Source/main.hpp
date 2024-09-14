#pragma once
#include <GLFW/glfw3.h>

//glfw/window
static constexpr auto WINDOW_TITLE  = "Space Game";
static constexpr auto WINDOW_WIDTH  = 1000;
static constexpr auto WINDOW_HEIGHT = 1000;
static constexpr auto FPS			= 144;
static constexpr auto FRAME_DELAY	= 1000 / FPS;

//font
static constexpr char FONT_RESOLUTION = 64;
static constexpr char RGBA_STRIDE = 4;

//debug info
static constexpr auto SHOW_DEBUG_INFO_TOGGLE_DELAY = FPS / 10;

//world
static constexpr auto ARENA_H = 1.1f;
static constexpr auto ARENA_W = 1.1f;

//ship
static constexpr auto SHIP_MOVERATE_MULT = 0.00004f;
static constexpr auto SHIP_TURNRATE_MULT = 1.f;
static constexpr auto SHIP_MIN_VELOCITY  = 0.0f;
static constexpr auto SHIP_MAX_VELOCITY  = 0.0333f;
static constexpr auto SHIP_STUN_DURATION = FPS * 60;

//projectiles
static constexpr auto PROJECTILE_VELOCITY_MULT = 0.02f;
static constexpr auto FIRE_DELAY			   = FPS * .25;
static constexpr auto PROJECTILE_DURATION	   = FIRE_DELAY * 10;

//asteroids
static constexpr auto NUM_TARGET_ASTEROIDS = 6;
static constexpr auto ASTEROID_INIT_VEL	   = 0.005;