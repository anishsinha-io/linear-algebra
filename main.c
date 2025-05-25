#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "grid.h"

const int   SCREEN_HEIGHT = 800;
const int   SCREEN_WIDTH  = 1200;
const char* SCREEN_TITLE  = "Linear Transformations";

static inline bool shift_pressed() {
  return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
  SetTargetFPS(60);

  grid g = {
      .origin         = {.x = 600, .y = 400},
      .line_thickness = 1,
      .line_color     = RAYWHITE,
      .line_count     = 15,

      .basis = {.i = {2, -1}, .j = {-1, 1}},
  };

  circle_path c = {3, g.origin};

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_UP)) {
      if (shift_pressed()) {
        g.basis.j.y += 0.05;
      } else {
        g.basis.i.y += 0.05;
      }
    }

    if (IsKeyDown(KEY_DOWN)) {
      if (shift_pressed()) {
        g.basis.j.y -= 0.05;
      } else {
        g.basis.i.y -= 0.05;
      }
    }

    if (IsKeyDown(KEY_LEFT)) {
      if (shift_pressed()) {
        g.basis.j.x -= 0.05;
      } else {
        g.basis.i.x -= 0.05;
      }
    }

    if (IsKeyDown(KEY_RIGHT)) {
      if (shift_pressed()) {
        g.basis.j.x += 0.05;
      } else {
        g.basis.i.x += 0.05;
      }
    }

    if (IsKeyPressed(KEY_SPACE)) {
      g.basis.i    = (Vector2){1, 0};
      g.basis.j    = (Vector2){0, 1};
      g.line_count = 15;
    }

    if (shift_pressed() && IsKeyPressed(KEY_EQUAL)) {
      g.line_count += 2;
    }

    if (IsKeyPressed(KEY_MINUS)) {
      g.line_count -= 2;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    draw_grid(&g);
    update_circle_path(&c, &g);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
