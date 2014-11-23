#include "final.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_final;
static TextLayer *s_textlayer_1;
static BitmapLayer *s_bitmaplayer_1;
static TextLayer *s_textlayer_2;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_image_final = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FINAL);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(18, 25, 100, 20));
  text_layer_set_background_color(s_textlayer_1, GColorBlack);
  text_layer_set_text_color(s_textlayer_1, GColorWhite);
  text_layer_set_text(s_textlayer_1, "Winner!");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(52, 64, 31, 35));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_image_final);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(-1, 104, 144, 20));
  text_layer_set_background_color(s_textlayer_2, GColorBlack);
  text_layer_set_text_color(s_textlayer_2, GColorWhite);
  text_layer_set_text(s_textlayer_2, "Here's a fudgsicle.");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  bitmap_layer_destroy(s_bitmaplayer_1);
  text_layer_destroy(s_textlayer_2);
  gbitmap_destroy(s_res_image_final);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_final(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_final(void) {
  window_stack_remove(s_window, true);
}
