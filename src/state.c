#include "state.h"

#include <string.h>

#include "grid.h"
#include "raylib.h"

void run_state(state* s) {
  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:
      run_2d_interactive_simul(s->scene.scene_data.simul_2d.g,
                               s->scene.scene_data.simul_2d.show_hud, s->font);
      break;
    default:
      break;
  }
}

void toggle_show_settings(state* const s) {
  s->show_settings = !s->show_settings;
}

static inline bool shift_pressed() {
  return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
}

static int round_to_multiple(int n, int multiple) {
  int a = (n / multiple) * multiple;

  int b = a + multiple;

  return (n - a >= b - n) ? b : a;
}

Rectangle calc_settings_rec(float screen_width, float screen_height) {
  if (screen_width > 1000) {
    return (Rectangle){
        .x      = (screen_width - 550) / 2,
        .y      = 50,
        .width  = 500,
        .height = screen_height * (3.0f / 4.0f),
    };
  } else if (screen_width > 800) {
    return (Rectangle){
        .x      = (screen_width - 550) / 2,
        .y      = 50,
        .width  = 550,
        .height = screen_height * (3.0F / 4.0F),
    };
  } else if (screen_width > 600) {
    return (Rectangle){
        .x      = (screen_width - 450) / 2,
        .y      = 50,
        .width  = 450,
        .height = screen_height * (3.0F / 4.0F),
    };
  } else if (screen_width > 400) {
    return (Rectangle){
        .x      = (screen_width - 350) / 2,
        .y      = 50,
        .width  = 350,
        .height = screen_height * (3.0F / 4.0F),
    };
  } else {
    return (Rectangle){
        .x      = (screen_width - 320) / 2,
        .y      = 50,
        .width  = 320,
        .height = screen_height * (3.0F / 4.0F),
    };
  }
}

void state_show_scene_settings(state* const s) {
  Rectangle menu_rec = calc_settings_rec(GetScreenWidth(), GetScreenHeight());

  DrawRectangleRec(menu_rec, WHITE);

  DrawTextEx(s->font, s->scene.scene_howto,
             (Vector2){menu_rec.x + 10, menu_rec.y + 10}, 16, 0, BLACK);
}

void init_simul_2d_keybinds(state* const s) {
  if (IsKeyPressed(KEY_S)) {
    toggle_show_settings(s);
  }

  grid* g = s->scene.scene_data.simul_2d.g;
  if (IsKeyDown(KEY_UP) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.y += g->step;
    } else {
      g->basis.i.y += g->step;
    }
  }

  if (IsKeyDown(KEY_DOWN) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.y -= g->step;
    } else {
      g->basis.i.y -= g->step;
    }
  }

  if (IsKeyDown(KEY_LEFT) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.x -= g->step;
    } else {
      g->basis.i.x -= g->step;
    }
  }

  if (IsKeyDown(KEY_RIGHT) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.x += g->step;
    } else {
      g->basis.i.x += g->step;
    }
  }

  if (IsKeyPressed(KEY_SPACE) && !s->show_settings) {
    g->basis.i = (Vector2){1, 0};
    g->basis.j = (Vector2){0, 1};
  }

  if (shift_pressed() && IsKeyPressed(KEY_EQUAL) && !s->show_settings) {
    g->line_count += 2;
  }

  if (IsKeyPressed(KEY_MINUS) && !s->show_settings) {
    g->line_count -= 2;
  }

  if (IsKeyPressed(KEY_U)) {
    s->scene.scene_data.simul_2d.show_hud =
        !s->scene.scene_data.simul_2d.show_hud;
  }

  if (IsKeyPressed(KEY_T)) {
    state_toggle_theme(s);
  }

  if (IsKeyPressed(KEY_P)) {
    grid* g      = s->scene.scene_data.simul_2d.g;
    float radius = 5;

    Vector2 pos = GetMousePosition();
    if (shift_pressed()) {
      pos.x = round_to_multiple(pos.x, 40);
      pos.y = round_to_multiple(pos.y, 40);
    }

    for (int i = 0; i < g->num_points; i++) {
      Vector2 point = g->points[i];
      if (CheckCollisionPointCircle(pos, point, 5)) {
        memmove(&g->points[i], &g->points[i + 1],
                (g->num_points - i - 1) * sizeof(Vector2));
      }
    }

    if (g->num_points == 25) {
      memmove(&g->points[1], &g->points[0], 24 * sizeof(Vector2));

      g->points[0] = pos;

    } else {
      g->points[g->num_points++] = pos;
    }
  }

  if (IsKeyPressed(KEY_C)) {
    g->num_points = 0;
  }

  if (shift_pressed() && IsKeyPressed(KEY_COMMA)) {
    g->step -= 0.05;
  }
  if (shift_pressed() && IsKeyPressed(KEY_PERIOD)) {
    g->step += 0.05;
  }
}

void state_update(state* const s) {
  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:
      s->scene.scene_data.simul_2d.g->origin.x = (float)GetScreenWidth() / 2;
      s->scene.scene_data.simul_2d.g->origin.y = (float)GetScreenHeight() / 2;
      break;
    default:
      break;
  }
}

void state_init_scene_keybinds(state* const s) {
  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:
      init_simul_2d_keybinds(s);
      break;
    default:
      break;
  }
}

void state_toggle_theme(state* const s) {
  if (s->theme == DARKMODE) {
    s->theme = LIGHTMODE;
  } else {
    s->theme = DARKMODE;
  }

  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:
      if (s->theme == LIGHTMODE) {
        s->scene.scene_data.simul_2d.g->style = BUILTIN_THEMES.light;
      } else {
        s->scene.scene_data.simul_2d.g->style = BUILTIN_THEMES.dark;
      }
      break;
    default:
      break;
  }
}
