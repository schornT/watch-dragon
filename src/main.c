#include "header.h"
#include "pebble.h"
  
#define KEY_COLOUR 1

static Window *window;
static Layer *s_hands_layer;


#ifdef PBL_PLATFORM_APLITE
static BitmapLayer *s_black_layer, *s_white_layer;
static GBitmap *s_white_bitmap, *s_black_bitmap;
#elif PBL_PLATFORM_BASALT
static BitmapLayer *s_image_layer, *s_door_layer;
static GBitmap *s_image_bitmap, *s_door_bitmap;
#endif
  
static GPath *s_minute_arrow, *s_hour_arrow;

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(window));
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  int16_t second_hand_length = bounds.size.w / 2;

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  int32_t second_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  GPoint second_hand = {
    .x = (int16_t)(sin_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.y,
  };

  // second hand
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, second_hand, center);

  // minute/hour hand
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorBlack);

  gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * t->tm_min / 60);
  gpath_draw_filled(ctx, s_minute_arrow);
  gpath_draw_outline(ctx, s_minute_arrow);

  gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);

  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(bounds.size.w / 2 - 1, bounds.size.h / 2 - 1, 3, 3), 0, GCornerNone);
}
  
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  int32_t colourChosen = RESOURCE_ID_DRAGON;

  
  #ifdef PBL_PLATFORM_APLITE
    s_white_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DRAGON_WHITE);
    s_black_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DRAGON_BLACK);
  #elif PBL_PLATFORM_BASALT
    s_image_bitmap = gbitmap_create_with_resource(colourChosen);
    s_door_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WOOD_DOOR_2);
  #endif
  
  
  //door
  #ifdef PBL_PLATFORM_BASALT
    s_door_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(s_door_layer, s_door_bitmap);
    bitmap_layer_set_compositing_mode(s_door_layer, GCompOpSet); 
    layer_add_child(window_layer, bitmap_layer_get_layer(s_door_layer));
  #endif
  
  //hands
  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);

  

  //s_image_layer = bitmap_layer_create(bounds);
  
  //dragon
  #ifdef PBL_PLATFORM_APLITE
    s_white_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(s_white_layer, s_white_bitmap);
    bitmap_layer_set_compositing_mode(s_white_layer, GCompOpOr);
    layer_add_child(window_layer, bitmap_layer_get_layer(s_white_layer));
        
    s_black_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(s_black_layer, s_black_bitmap);    
    bitmap_layer_set_compositing_mode(s_black_layer, GCompOpClear);
    layer_add_child(window_layer, bitmap_layer_get_layer(s_black_layer));
  #elif PBL_PLATFORM_BASALT
    s_image_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
    bitmap_layer_set_compositing_mode(s_image_layer, GCompOpSet); 
    layer_add_child(window_layer, bitmap_layer_get_layer(s_image_layer));
  #endif
  
  
  //bitmap_layer_set_alignment(s_image_layer, GAlignCenter);
  //
}

static void window_unload(Window *window) {
  
  
  #ifdef PBL_PLATFORM_APLITE
    gbitmap_destroy(s_black_bitmap);
    gbitmap_destroy(s_white_bitmap);
    bitmap_layer_destroy(s_white_layer);
    bitmap_layer_destroy(s_black_layer);
  #elif PBL_PLATFORM_BASALT
    gbitmap_destroy(s_image_bitmap);
    bitmap_layer_destroy(s_image_layer);
    gbitmap_destroy(s_door_bitmap);
    bitmap_layer_destroy(s_door_layer);
  #endif
  
  layer_destroy(s_hands_layer);
}
  
static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  // init hand paths
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}
  
static void deinit() {
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);

  tick_timer_service_unsubscribe();
  window_destroy(window);
}
    
int main(void) {
  init();
  app_event_loop();
  deinit();
}
