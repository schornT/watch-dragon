#pragma once

#include "pebble.h"
  
  
static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -2, 20 },
    { 2, 20 },
    { 2, -80 },
    { -2, -80 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    {-2, 20},
    {2, 20},
    {2, -60},
    {-2, -60}
  }
};
