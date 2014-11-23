#include "pebble.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MATH_PI 3.141592653589793238462
#define NUM_DISCS 1
#define DISC_DENSITY 0.25
#define ACCEL_RATIO 0.05
#define ACCEL_STEP_MS 50

typedef struct Vec2d {
  double x;
  double y;
} Vec2d;

typedef struct Disc {
  Vec2d pos;
  Vec2d vel;
  double mass;
  double radius;
} Disc;

static Disc discs[NUM_DISCS];

static double next_radius = 8;

static Window *window;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static TextLayer *s_score_layer;

static GRect window_frame;

static Layer *disc_layer;

static AppTimer *timer;

static int score = 0;
static char score_buffer[11] = "Score: 0";


static double disc_calc_mass(Disc *disc) {
  return MATH_PI * disc->radius * disc->radius * DISC_DENSITY;
}

static void disc_init(Disc *disc) {
  GRect frame = window_frame;
  disc->pos.x = frame.size.w/2;
  disc->pos.y = frame.size.h/2;
  disc->vel.x = 0;
  disc->vel.y = 0;
  disc->radius = next_radius;
  disc->mass = disc_calc_mass(disc);
  next_radius += 0.5;
}

static void disc_apply_force(Disc *disc, Vec2d force) {
  disc->vel.x += force.x / disc->mass;
  disc->vel.y += force.y / disc->mass;
}

static void disc_apply_accel(Disc *disc, AccelData accel) {
  Vec2d force;
  force.x = accel.x * ACCEL_RATIO;
  force.y = -accel.y * ACCEL_RATIO;
  disc_apply_force(disc, force);
}

static void disc_update(Disc *disc) {
  const GRect frame = window_frame;
  double e = 0.5;
  if ((disc->pos.x - disc->radius < 0 && disc->vel.x < 0)
    || (disc->pos.x + disc->radius > frame.size.w && disc->vel.x > 0)) {
    disc->vel.x = -disc->vel.x * e;
  }
  if ((disc->pos.y - disc->radius < 0 && disc->vel.y < 0)
    || (disc->pos.y + disc->radius > frame.size.h && disc->vel.y > 0)) {
    disc->vel.y = -disc->vel.y * e;
  }
  
  if((disc->pos.y <  frame.size.h - 10 && disc->pos.y > frame.size.h - 40) && (disc->pos.x < frame.size.w / 2 + 25 && disc->pos.x > frame.size.w / 2 - 13))
  {
    int x = rand() % 144;
    int y = rand() % 168;
    
    while((y <  frame.size.h - 10 && y > frame.size.h - 40) && (x < frame.size.w / 2 + 25 && x > frame.size.w / 2 - 13))
    {
      x = rand() % 144;
      y = rand() % 168;
    }
    
    disc->pos.x = x;
    disc->pos.y = y;
    
    score++;
    
    int tmp;
    
    snprintf(score_buffer, 10, "Score: %i", score);
      
    text_layer_set_text(s_score_layer, score_buffer);
  }
  
  disc->pos.x += disc->vel.x;
  disc->pos.y += disc->vel.y;
}

static void disc_draw(GContext *ctx, Disc *disc) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(disc->pos.x, disc->pos.y), disc->radius);
}

static void disc_layer_update_callback(Layer *me, GContext *ctx) {
  for (int i = 0; i < NUM_DISCS; i++) {
    disc_draw(ctx, &discs[i]);
  }
}

static void timer_callback(void *data) {
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };

  accel_service_peek(&accel);

  for (int i = 0; i < NUM_DISCS; i++) {
    Disc *disc = &discs[i];
    disc_apply_accel(disc, accel);
    disc_update(disc);
  }

  layer_mark_dirty(disc_layer);

  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect frame = window_frame = layer_get_frame(window_layer);
  
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NICK);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  s_score_layer = text_layer_create(GRect(38, 20, 65, 50));
  text_layer_set_background_color(s_score_layer, GColorClear);
  text_layer_set_text_color(s_score_layer, GColorBlack);
  text_layer_set_text(s_score_layer, score_buffer);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_score_layer));
  
  text_layer_set_font(s_score_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_score_layer, GTextAlignmentCenter);
  
  disc_layer = layer_create(frame);
  layer_set_update_proc(disc_layer, disc_layer_update_callback);
  layer_add_child(window_layer, disc_layer);
  
  for (int i = 0; i < NUM_DISCS; i++) {
    disc_init(&discs[i]);
  }
}

static void window_unload(Window *window) {
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  text_layer_destroy(s_score_layer);
  
  layer_destroy(disc_layer);
  
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(window, true /* Animated */);
  //window_set_background_color(window, GColorBlack);

  accel_data_service_subscribe(0, NULL);

  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

static void deinit(void) {
  accel_data_service_unsubscribe();

  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
