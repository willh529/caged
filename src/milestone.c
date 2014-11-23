#include "milestone.h"
#include "final.h"
#include <pebble.h>

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  show_final();
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);
}

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_arrow;
static TextLayer *s_textlayer_1;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, true);
  
  s_res_image_arrow = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ARROW);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(0, 43, 115, 104));
  text_layer_set_text(s_textlayer_1, "You have saved the world! But more importantly, The Cage is satisfied.");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_arrow);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_image_arrow);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_milestone(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_milestone(void) {
  window_stack_remove(s_window, true);
}
