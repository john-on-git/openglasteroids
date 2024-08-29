#pragma once
#include <GLFW/glfw3.h>

constexpr auto WINDOW_TITLE  = "My First (Successful) OpenGL Project";
constexpr auto WINDOW_WIDTH  = 1000;
constexpr auto WINDOW_HEIGHT = 1000;
constexpr auto FRAMERATE   = 144;
constexpr auto FRAME_DELAY = 1000 / FRAMERATE;

constexpr auto SHIP_MOVERATE_MULT		= 0.00004f;
constexpr auto SHIP_TURNRATE_MULT		= 0.666f;
constexpr auto SHIP_MIN_VELOCITY		= 0.0f;
constexpr auto SHIP_MAX_VELOCITY		= 0.0333f;

constexpr auto PROJECTILE_VELOCITY_MULT = 0.02f;
constexpr auto FIRE_DELAY			= FRAMERATE / 4;
constexpr auto PROJECTILE_DURATION = FIRE_DELAY * 10;

constexpr auto ARENA_H = 1.0f;
constexpr auto ARENA_W = 1.0f;

constexpr auto SHOW_DEBUG_INFO_TOGGLE_DELAY = FRAMERATE/10;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
