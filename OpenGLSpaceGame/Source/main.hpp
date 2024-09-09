#pragma once
#include <GLFW/glfw3.h>

constexpr auto WINDOW_TITLE  = "Space Game";
constexpr auto WINDOW_WIDTH  = 1000;
constexpr auto WINDOW_HEIGHT = 1000;
constexpr auto FPS			 = 144;
constexpr auto FRAME_DELAY	 = 1000 / FPS;
constexpr auto FONT_CHAR_SIZE	 = 32; //TODO placeholder

constexpr auto SHIP_MOVERATE_MULT = 0.00004f;
constexpr auto SHIP_TURNRATE_MULT = 1.f;
constexpr auto SHIP_MIN_VELOCITY  = 0.0f;
constexpr auto SHIP_MAX_VELOCITY  = 0.0333f;
constexpr auto SHIP_STUN_DURATION = FPS * 60;

constexpr auto PROJECTILE_VELOCITY_MULT = 0.02f;
constexpr auto FIRE_DELAY				= FPS * .25;
constexpr auto PROJECTILE_DURATION		= FIRE_DELAY * 10;

constexpr auto NUM_TARGET_ASTEROIDS = 6;
constexpr auto ASTEROID_INIT_VEL	= .005;

constexpr auto ARENA_H					= 1.1f;
constexpr auto ARENA_W					= 1.1f;

constexpr auto SHOW_DEBUG_INFO_TOGGLE_DELAY = FPS/10;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
