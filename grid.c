#include "grid.h"

#include <rlgl.h>

#include "quad.h"

void draw_line(const line l) {
  DrawLineEx(l.start, l.end, l.thickness, l.color);
}

Vector2 transform(matrix2 T, Vector2 v) {
  double x = (v.x * T.i.x) + (v.y * T.j.x);
  double y = (v.x * T.i.y) + (v.y * T.j.y);

  return (Vector2){.x = x, .y = y};
}

matrix2 rotation_matrix(double theta) {
  return (matrix2){.i = {cos(theta), -sin(theta)},
                   .j = {sin(theta), cos(theta)}};
}

static inline float det2(matrix2 m) { return m.i.x * m.j.y - m.j.x * m.i.y; }

static inline Vector2 translate_x(Vector2 v, double x) {
  return (Vector2){v.x + x, v.y};
}

static inline Vector2 translate_y(Vector2 v, double y) {
  return (Vector2){v.x, v.y + y};
}

static inline Vector2 translate_xy(Vector2 v, double x, double y) {
  return translate_y(translate_x(v, x), y);
}

static const Vector2 arrow_p1 = {10, 0};
static const Vector2 arrow_p2 = {-5, -5};
static const Vector2 arrow_p3 = {-5, 5};

static void draw_arrowhead(float x, float y, float scale, double angle_rad,
                           Color color) {
  matrix2 R = rotation_matrix(angle_rad);

  Vector2 p1 = translate_x(translate_y(transform(R, arrow_p1), y), x);
  Vector2 p2 = translate_x(translate_y(transform(R, arrow_p2), y), x);
  Vector2 p3 = translate_x(translate_y(transform(R, arrow_p3), y), x);

  DrawTriangle(p1, p2, p3, color);
}

static void draw_det2(const grid* const g) {
  int     scale       = 40;
  float   determinant = det2(g->basis);
  Vector2 i_hat = g->basis.i, j_hat = g->basis.j;
  i_hat.y *= -1;
  j_hat.y *= -1;
  QuadCoords coords = {0};

  coords.bottomLeft  = translate_xy((Vector2){0, 0}, g->origin.x, g->origin.y);
  coords.bottomRight = translate_xy((Vector2){i_hat.x * scale, i_hat.y * scale},
                                    g->origin.x, g->origin.y);
  coords.topLeft     = translate_xy((Vector2){j_hat.x * scale, j_hat.y * scale},
                                    g->origin.x, g->origin.y);
  coords.topRight    = translate_xy((Vector2){i_hat.x * scale + j_hat.x * scale,
                                              j_hat.y * scale + i_hat.y * scale},
                                    g->origin.x, g->origin.y);
  if (determinant < 0) {
    Vector2 tmp     = coords.topLeft;
    coords.topLeft  = coords.topRight;
    coords.topRight = tmp;

    tmp                = coords.bottomLeft;
    coords.bottomLeft  = coords.bottomRight;
    coords.bottomRight = tmp;
  }

  DrawQuad(coords, determinant > 0 ? BLUE : ORANGE);
}

void draw_grid(const grid* const g) {
  int scale            = 40;
  int line_lower_bound = g->line_count / 2;
  int line_upper_bound = line_lower_bound;
  if (g->line_count % 2 == 1) {
    line_upper_bound += 1;
  }

  Vector2 i_hat = g->basis.i;
  Vector2 j_hat = g->basis.j;
  matrix2 basis = {i_hat, j_hat};

  line i = {.color     = PINK,
            .start     = {.x = g->origin.x, .y = g->origin.y},
            .end       = {.x = g->origin.x + i_hat.x * scale,
                          .y = g->origin.y - i_hat.y * scale},
            .thickness = g->line_thickness};

  line j = {.color     = PINK,
            .start     = {.x = g->origin.x, .y = g->origin.y},
            .end       = {.x = g->origin.x + j_hat.x * scale,
                          .y = g->origin.y - j_hat.y * scale},
            .thickness = g->line_thickness};

  double basis_y_len = sqrtl(pow(g->basis.j.x, 2) + pow(g->basis.j.y, 2));
  double basis_x_len = sqrtl(pow(g->basis.i.x, 2) + pow(g->basis.i.y, 2));
  double len         = basis_x_len * line_lower_bound * scale;

  double theta = atan(i_hat.y / i_hat.x);
  double alpha = PI / 2 - atan(j_hat.x / j_hat.y);
  double x     = (len)*cos(theta);
  double y     = (len)*sin(theta);

  for (int i = -line_lower_bound; i < line_upper_bound; i++) {
    line l1 = {
        translate_xy((Vector2){g->origin.x - x, g->origin.y + y},
                     g->basis.j.x * scale * i, -g->basis.j.y * scale * i),
        translate_xy((Vector2){g->origin.x + x, g->origin.y - y},
                     g->basis.j.x * scale * i, -g->basis.j.y * scale * i),
        RAYWHITE, g->line_thickness};
    draw_line(l1);
  }

  double len_p = (basis_y_len * len) / basis_x_len;
  double x_p   = len_p * cos(alpha);
  double y_p   = len_p * sin(alpha);

  for (int i = -line_lower_bound; i < line_upper_bound; i++) {
    line l2 = {
        translate_xy((Vector2){g->origin.x - x_p, g->origin.y + y_p},
                     g->basis.i.x * scale * i, -g->basis.i.y * scale * i),
        translate_xy((Vector2){g->origin.x + x_p, g->origin.y - y_p},
                     g->basis.i.x * scale * i, -g->basis.i.y * scale * i),
        RAYWHITE, g->line_thickness};
    draw_line(l2);
  }

  draw_det2(g);

  draw_line(i);
  draw_line(j);

  double arrowhead1_angle = theta;
  if (i_hat.x < 0) {
    arrowhead1_angle = PI - theta;
    if (i_hat.y < 0) {
      arrowhead1_angle *= -1;
    }
    if (i_hat.y > 0) {
      arrowhead1_angle = PI + theta;
    }
  }

  double arrowhead2_angle = alpha;
  if (j_hat.y < 0) {
    arrowhead2_angle *= -1;
    if (j_hat.x > 0) {
      arrowhead2_angle = PI + alpha;
    }
    if (j_hat.x < 0) {
      arrowhead2_angle = PI + alpha;
    }
  }

  draw_arrowhead(g->origin.x + i_hat.x * scale, g->origin.y - i_hat.y * scale,
                 1, arrowhead1_angle, PINK);
  draw_arrowhead(g->origin.x + j_hat.x * scale, g->origin.y - j_hat.y * scale,
                 1, arrowhead2_angle, PINK);

  DrawCircle(g->origin.x, g->origin.y, 4, RED);
  DrawText("origin", g->origin.x + 5, g->origin.y + 10, 10, RAYWHITE);
}

void update_circle_path(circle_path* const c, const grid* const g) {
  c->theta += 0.1 * 20 * 0.01745329F;

  float   r   = c->radius * 40;
  float   x   = r * sin(c->theta);
  float   y   = r * cos(c->theta);
  Vector2 loc = {x, y};

  matrix2 R = g->basis;
  R.i.y *= -1;
  R.j.x *= -1;

  Vector2 new_loc = translate_xy(transform(R, loc), g->origin.x, g->origin.y);

  DrawCircle(new_loc.x, new_loc.y, 5, RED);
}
