#pragma once
#include <GLFW/glfw3.h>

//glfw/window
static constexpr auto WINDOW_TITLE  = "Space Game";

static constexpr size_t FPS				= 144;
static constexpr size_t LOGIC_RATE		= 144;
static constexpr size_t UPDATE_RATE		= 144; //imposes a max limit on logic/FPS

static constexpr size_t UPDATE_DELAY	= 1000 / UPDATE_RATE;
static constexpr size_t DRAW_EVERY		= UPDATE_RATE/FPS; //144 -> 1
static constexpr size_t TICK_EVERY		= UPDATE_RATE/LOGIC_RATE;

//font
static constexpr char FONT_RESOLUTION = 16;
static constexpr char RGBA_STRIDE = 4;

//debug info
static constexpr auto SHOW_DEBUG_INFO_TOGGLE_DELAY = FPS / 10;

//score
static constexpr auto SCORE_KILL_ALIEN = 100;

//ship
static constexpr auto SHIP_ACCELERATION = 0.00004f;
static constexpr auto SHIP_TURNRATE_MULT = 1.f;
static constexpr auto SHIP_MIN_VELOCITY  = 0.0f;
static constexpr auto SHIP_MAX_VELOCITY  = 0.0333f;
static constexpr size_t SHIP_STUN_DURATION = FPS * 1;
static constexpr size_t SHIP_FIRE_DELAY = FPS / 4;

//projectiles
static constexpr auto PROJECTILE_VELOCITY_MULT = 0.02f;
static constexpr size_t PROJECTILE_DURATION	   = SHIP_FIRE_DELAY * 5;

//asteroids
static constexpr size_t NUM_ASTEROID_TYPES = 3;
static constexpr size_t NUM_TARGET_ASTEROIDS = 6;
static constexpr auto ASTEROID_INIT_VEL	   = 0.005f;

//aliens
static constexpr size_t INITIAL_NUM_TARGET_ALIENS = 3;
static constexpr size_t ALIEN_CAP_INCREASE_PERIOD = FPS * 10;
static constexpr size_t MAX_ALIENS = 10;

static constexpr auto ALIEN_ACCELERATION = 0.0001f;
static constexpr auto ALIEN_TURNRATE_MULT = 0.1f;
static constexpr auto ALIEN_MIN_VELOCITY  = 0.0f;
static constexpr auto ALIEN_MAX_VELOCITY  = 0.01f;

static constexpr unsigned char HALF_ALIEN_TRACKING_CONE_DEGREES = 5;
static constexpr size_t ALIEN_FIRE_DELAY = FPS * 1.5;

static constexpr size_t ALIEN_STUN_DURATION = FPS * 2;
static constexpr size_t ALIEN_GRACE_PERIOD = ALIEN_FIRE_DELAY * 1;