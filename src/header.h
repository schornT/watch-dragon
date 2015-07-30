#pragma once

#include "pebble.h"
  
  
static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -2, 7 },
    { 2, 7 },
    { 2, -80 },
    { -2, -80 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    {-2, 7},
    {2, 7},
    {2, -25},
    {-2, -25}
  }
};
