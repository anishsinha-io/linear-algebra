#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "close_icon.h"
#include "grid.h"
#include "jetbrains_mono.h"
#include "moon.h"
#include "settings_icon.h"
#include "state.h"
#include "sun.h"

const int   SCREEN_HEIGHT = 800;
const int   SCREEN_WIDTH  = 1200;
const char* SCREEN_TITLE  = "Linear Algebra Visualized Geometrically";

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
  SetTargetFPS(60);

  Font jetbrains_mono =
      LoadFontFromMemory(".ttf", jb_mono_ttf, jb_mono_ttf_len, 192,
                         (int*)codepoints, codepoints_len);

  SetTextureFilter(jetbrains_mono.texture, TEXTURE_FILTER_BILINEAR);

  grid g = {
      .origin = {.x = (float)SCREEN_WIDTH / 2, .y = (float)SCREEN_HEIGHT / 2},
      .line_thickness = 1,
      .line_color     = RAYWHITE,
      .line_count     = 15,
      .basis          = {{3, 2}, {1, -1}},
      .num_paths      = 1,
      .paths =
          (path[]){
              (path){
                  .tag   = CIRCLE,
                  .color = BLUE,
                  .data =
                      {
                          .circle_path =
                              {.center = (Vector2){(float)SCREEN_WIDTH / 2,
                                                   (float)SCREEN_HEIGHT / 2},
                               .radius = 3,
                               .speed  = 0.1,
                               .theta  = 0},
                      },
              },
          },
      .style = BUILTIN_THEMES.dark,
  };

  state s = {
      .font          = jetbrains_mono,
      .show_settings = false,
      .scene =
          {
              .tag = SCENE_2D_INTERACTIVE_SIMUL,
              .scene_data =
                  {
                      .simul_2d =
                          {
                              .g        = &g,
                              .show_hud = true,
                          },

                  },
              .scene_howto =
                  "Keybinds (press s to close this dialog)\n"
                  "- Arrow keys to move the first basis\n  vector (i)\n"
                  "- Shift+Arrow keys to move the second\n  basis vector "
                  "(j)\n"
                  "- Hover over i, j, and the eigenvectors to\n  see their "
                  "exact values\n"
                  "- + to add more grid lines\n"
                  "- - to remove grid lines\n"
                  "- spacebar to reset the grid to a square\n  15x15 grid\n"
                  "- s to toggle the help dialog\n"
                  "- u to toggle the HUD\n"
                  "- t to toggle the "
                  "theme\n----------------------------------\n"
                  "About\n"
                  "This simulation\nillustrates linear algebra\ngeometrically."
                  "It showcases the concepts\nof basis, determinants,\n"
                  "eigenvalues, and eigenvectors.\n\nThis was really fun to "
                  "make. Enjoy! \n-A",
          },
      .theme = DARKMODE,
  };

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

  Image     sun_img = LoadImageFromMemory(".png", sun_png, sun_png_len);
  Texture2D sun     = LoadTextureFromImage(sun_img);

  Image     moon_img = LoadImageFromMemory(".png", moon_png, moon_png_len);
  Texture2D moon     = LoadTextureFromImage(moon_img);

  Rectangle theme_rec = {.x = 0.02F * SCREEN_WIDTH + 68,
                         .y = 0.02F * SCREEN_WIDTH - 3,
                         0.068F * SCREEN_HEIGHT,
                         0.068F * SCREEN_HEIGHT};

  SetTextureFilter(close, TEXTURE_FILTER_BILINEAR);

  while (!WindowShouldClose()) {
    g.origin.x = (float)GetScreenWidth() / 2;
    g.origin.y = (float)GetScreenHeight() / 2;

    Vector2 mouse_pos = GetMousePosition();
    if (CheckCollisionPointRec(mouse_pos, dest_rec)) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        toggle_show_settings(&s);
      }
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mouse_pos, theme_rec)) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        state_toggle_theme(&s);
      }
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    state_init_scene_keybinds(&s);

    BeginDrawing();
    ClearBackground(s.theme == DARKMODE ? BLACK : RAYWHITE);

    run_state(&s);
    if (s.show_settings) {
      state_show_scene_settings(&s);
      DrawTexturePro(close, source_rec, dest_rec, (Vector2){}, 0,
                     s.theme == DARKMODE ? RAYWHITE : BLACK);
    } else {
      DrawTexturePro(settings, source_rec, dest_rec, (Vector2){}, 0,
                     s.theme == DARKMODE ? RAYWHITE : BLACK);
    }

    if (s.theme == DARKMODE) {
      DrawTexturePro(sun, source_rec, theme_rec, (Vector2){}, 0, WHITE);
    } else {
      DrawTexturePro(moon, source_rec, theme_rec, (Vector2){}, 0, WHITE);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
