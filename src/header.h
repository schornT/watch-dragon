#pragma once

#include "pebble.h"
  
  
static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -3, 7 },
    { 3, 7 },
    { 3, -80 },
    { -3, -80 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    {-3, 7},
    {3, 7},
    {3, -25},
    {-3, -25}
  }
};
