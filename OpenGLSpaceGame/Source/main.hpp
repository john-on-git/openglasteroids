#pragma once
#include <GLFW/glfw3.h>

//glfw/window
static constexpr auto WINDOW_TITLE  = "Space Game";
static constexpr auto WINDOW_WIDTH  = 1000;
static constexpr auto WINDOW_HEIGHT = 1000;
static constexpr auto FPS			= 144;
static constexpr auto FRAME_DELAY	= 1000 / FPS;

//font
static constexpr char FONT_RESOLUTION = 16;
static constexpr char RGBA_STRIDE = 4;

//debug info
static constexpr auto SHOW_DEBUG_INFO_TOGGLE_DELAY = FPS / 10;

//world
static constexpr auto ARENA_H = 1.1f;
static constexpr auto ARENA_W = 1.1f;

//ship
static constexpr auto SHIP_ACCELERATION = 0.00004f;
static constexpr auto SHIP_TURNRATE_MULT = 1.f;
static constexpr auto SHIP_MIN_VELOCITY  = 0.0f;
static constexpr auto SHIP_MAX_VELOCITY  = 0.0333f;
static constexpr size_t SHIP_STUN_DURATION = FPS * 0.0f;

//projectiles
static constexpr auto PROJECTILE_VELOCITY_MULT = 0.02f;
static constexpr auto SHIP_FIRE_DELAY			   = FPS * .25f;
static constexpr size_t PROJECTILE_DURATION	   = SHIP_FIRE_DELAY * 10;

//asteroids
static constexpr auto NUM_TARGET_ASTEROIDS = 0;
static constexpr auto ASTEROID_INIT_VEL	   = 0.005;

//aliens
static constexpr auto INITIAL_NUM_TARGET_ALIENS = 1;
static constexpr auto ALIEN_ACCELERATION = 0.0001f;
static constexpr auto ALIEN_TURNRATE_MULT = 1.f;
static constexpr auto ALIEN_MIN_VELOCITY  = 0.0f;
static constexpr auto ALIEN_MAX_VELOCITY  = 0.01f;
static constexpr size_t ALIEN_STUN_DURATION = FPS * 0.0f;
static constexpr auto ALIEN_FIRE_DELAY = FPS * 1.0f;