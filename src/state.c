#include "state.h"

#include "grid.h"

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

Rectangle calc_settings_rec(float screen_width, float screen_height) {
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
        .x      = (screen_width - 320) / 2,
        .y      = 200,
        .width  = 320,
        .height = 300,
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
      g->basis.j.y += 0.05;
    } else {
      g->basis.i.y += 0.05;
    }
  }

  if (IsKeyDown(KEY_DOWN) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.y -= 0.05;
    } else {
      g->basis.i.y -= 0.05;
    }
  }

  if (IsKeyDown(KEY_LEFT) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.x -= 0.05;
    } else {
      g->basis.i.x -= 0.05;
    }
  }

  if (IsKeyDown(KEY_RIGHT) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.x += 0.05;
    } else {
      g->basis.i.x += 0.05;
    }
  }

  if (IsKeyPressed(KEY_SPACE) && !s->show_settings) {
    g->basis.i    = (Vector2){1, 0};
    g->basis.j    = (Vector2){0, 1};
    g->line_count = 15;
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
