#include <stdlib.h>
#include <math.h>

#define JOYSTICK_CONSTANT (255/200)

enum joystick_dir {
  NEUTRAL = 0,
  UP = 1,
  DOWN = 2,
  LEFT = 3,
  RIGHT = 4,
};

struct joystick {
  int x;
  int y;
  int neutralx;
  int neutraly;
  enum joystick_dir dir;
};

void joystick_init(struct joystick* joy);

void analog_position(struct joystick* joy);

void analog_direction(struct joystick* joy);
