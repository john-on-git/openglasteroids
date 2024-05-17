#pragma once
#include <GLFW/glfw3.h>

constexpr auto WINDOW_TITLE  = "My First (Successful) OpenGL Project";
constexpr auto WINDOW_WIDTH  = 1000;
constexpr auto WINDOW_HEIGHT = 1000;
constexpr auto FRAMERATE   = 144;
constexpr auto FRAME_DELAY = 1000 / FRAMERATE;
constexpr auto SHIP_MOVERATE_MULT = 0.00003f;
constexpr auto SHIP_TURNRATE_MULT = 0.666f;
constexpr auto SHIP_MIN_VELOCITY  = 0.0f;
constexpr auto SHIP_MAX_VELOCITY  = 0.0333f;
constexpr auto BULLET_VELOCITY_MULT = 0.01f;
constexpr auto FIRE_DELAY			= FRAMERATE / 4;
constexpr auto ARENA_H = 1.0f;
constexpr auto ARENA_W = 1.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);