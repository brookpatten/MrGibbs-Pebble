#include <pebble.h>
#include "Dashboard.h"
#include "Messages.h"
#include "RaceMenu.h"
#include "MarkSelectMenu.h"
#include "Compass.h"
  
static uint16_t race_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return 3;
}

static void race_menu_draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "Course", NULL, NULL);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "Marks", NULL, NULL);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "New", NULL, NULL);
      break;
    default:
      break;
  }
}

static void race_menu_select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
        compass_init(course_heading);
      break;
    case 1:
        mark_select_menu_init();
      break;
    case 2:
        message_send_new_race();
        return_to_dashboard(S_FALSE);
      break;
    default:
      break;
  }
}

static void race_menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *context) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Race");
}

static int16_t race_menu_get_header_height_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static uint16_t race_menu_get_num_sections_callback(struct MenuLayer *menu_layer, void *context) {
  return 1;
}

static void race_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  race_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(race_menu_layer, window);
  menu_layer_set_callbacks(race_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)race_menu_get_num_rows_callback,
      .draw_row = (MenuLayerDrawRowCallback)race_menu_draw_row_callback,
      //.get_cell_height = (MenuLayerGetCellHeightCallback)race_menu_get_cell_height_callback,
      .select_click = (MenuLayerSelectCallback)race_menu_select_callback,
      .draw_header = (MenuLayerDrawHeaderCallback)race_menu_draw_header_callback,
      .get_header_height = (MenuLayerGetHeaderHeightCallback)race_menu_get_header_height_callback,
      .get_num_sections = (MenuLayerGetNumberOfSectionsCallback)race_menu_get_num_sections_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(race_menu_layer));
}

static void race_window_unload(Window *window) {
  menu_layer_destroy(race_menu_layer);
  
  window_destroy(race_menu_window);
}

void race_menu_init(void) {
  race_menu_window = window_create();
  window_set_window_handlers(race_menu_window, (WindowHandlers) {
      .load = race_window_load,
      .unload = race_window_unload,
  });
  window_stack_push(race_menu_window, true);
}