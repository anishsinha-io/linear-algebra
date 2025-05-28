#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "close_icon.h"
#include "grid.h"
#include "matrix2.h"
#include "settings.h"
#include "settings_icon.h"

const int   SCREEN_HEIGHT = 800;
const int   SCREEN_WIDTH  = 1200;
const char* SCREEN_TITLE  = "Linear Algebra Visualized Geometrically";

static inline bool shift_pressed() {
  return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
}

bool show_settings = false;

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
  SetTargetFPS(60);

  grid g = {
      .origin         = {.x = 600, .y = 400},
      .line_thickness = 1,
      .line_color     = RAYWHITE,
      .line_count     = 15,

      // .basis = {.i = {2, -1}, .j = {-1, 1}},
      .basis = {{0, 2}, {1, -3}},
  };

  circle_path c = {3, g.origin, 0, 0.1};

  Image settings_img =
      LoadImageFromMemory(".png", settings_png, settings_png_len);

  Texture2D settings   = LoadTextureFromImage(settings_img);
  Rectangle source_rec = {0.0f, 0.0f, (float)settings.width,
                          (float)settings.height};
  Rectangle dest_rec   = {.x = 0.02F * SCREEN_WIDTH,
                          .y = 0.02F * SCREEN_WIDTH,
                          0.06F * SCREEN_HEIGHT,
                          0.06F * SCREEN_HEIGHT};

  SetTextureFilter(settings, TEXTURE_FILTER_BILINEAR);

  Image     close_img = LoadImageFromMemory(".png", close_png, close_png_len);
  Texture2D close     = LoadTextureFromImage(close_img);

  SetTextureFilter(close, TEXTURE_FILTER_BILINEAR);

  while (!WindowShouldClose()) {
    g.origin.x = (float)GetScreenWidth() / 2;
    g.origin.y = (float)GetScreenHeight() / 2;

    Vector2 mouse_pos = GetMousePosition();
    if (CheckCollisionPointRec(mouse_pos, dest_rec)) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        show_settings = !show_settings;
        c.speed       = show_settings ? 0 : 0.1;
      }
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (IsKeyPressed(KEY_S)) {
      show_settings = !show_settings;

      if (show_settings) {
        c.speed = 0;
      } else {
        c.speed = 0.1;
      }
    }

    if (IsKeyDown(KEY_UP) && !show_settings) {
      if (shift_pressed()) {
        g.basis.j.y += 0.05;
      } else {
        g.basis.i.y += 0.05;
      }
    }

    if (IsKeyDown(KEY_DOWN) && !show_settings) {
      if (shift_pressed()) {
        g.basis.j.y -= 0.05;
      } else {
        g.basis.i.y -= 0.05;
      }
    }

    if (IsKeyDown(KEY_LEFT) && !show_settings) {
      if (shift_pressed()) {
        g.basis.j.x -= 0.05;
      } else {
        g.basis.i.x -= 0.05;
      }
    }

    if (IsKeyDown(KEY_RIGHT) && !show_settings) {
      if (shift_pressed()) {
        g.basis.j.x += 0.05;
      } else {
        g.basis.i.x += 0.05;
      }
    }

    if (IsKeyPressed(KEY_SPACE) && !show_settings) {
      g.basis.i    = (Vector2){1, 0};
      g.basis.j    = (Vector2){0, 1};
      g.line_count = 15;
    }

    if (shift_pressed() && IsKeyPressed(KEY_EQUAL) && !show_settings) {
      g.line_count += 2;
    }

    if (IsKeyPressed(KEY_MINUS) && !show_settings) {
      g.line_count -= 2;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    draw_grid(&g);
    update_circle_path(&c, &g);
    if (show_settings) {
      draw_settings(GetScreenWidth(), GetScreenHeight());
      DrawTexturePro(close, source_rec, dest_rec, (Vector2){}, 0, WHITE);
    } else {
      DrawTexturePro(settings, source_rec, dest_rec, (Vector2){}, 0, WHITE);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
