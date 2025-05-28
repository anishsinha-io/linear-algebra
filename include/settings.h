#pragma once

#include <raylib.h>

static const char* HOWTO =
    "Keybinds (press s to close this dialog)\n\n"
    "- Arrow keys to move the first basis\n  vector (i)\n"
    "- Shift+Arrow keys to move the second\n  basis vector (j)\n"
    "- + to add more grid lines\n"
    "- - to remove grid lines\n"
    "- spacebar to reset the grid to a square\n  15x15 grid\n"
    "- s to toggle the help dialog\n"
    "- u to toggle the HUD";

static Rectangle calc_settings_rec(float screen_width, float screen_height) {
  if (screen_width > 1000) {
    return (Rectangle){
        .x      = (screen_width - 550) / 2,
        .y      = 200,
        .width  = 500,
        .height = 500,
    };
  } else if (screen_width > 800) {
    return (Rectangle){
        .x      = (screen_width - 550) / 2,
        .y      = 200,
        .width  = 550,
        .height = 400,
    };
  } else if (screen_width > 600) {
    return (Rectangle){
        .x      = (screen_width - 450) / 2,
        .y      = 200,
        .width  = 450,
        .height = 400,
    };
  } else if (screen_width > 400) {
    return (Rectangle){
        .x      = (screen_width - 350) / 2,
        .y      = 200,
        .width  = 350,
        .height = 300,
    };
  } else {
    return (Rectangle){
        .x      = (screen_width - 280) / 2,
        .y      = 200,
        .width  = 280,
        .height = 300,
    };
  }
}

void draw_settings(float screen_width, float screen_height) {
  Rectangle menu_rec = calc_settings_rec(screen_width, screen_height);

  DrawRectangleRec(menu_rec, WHITE);

  DrawText(HOWTO, menu_rec.x + 10, menu_rec.y + 10, 15, BLACK);
}
