#pragma once

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "matrix2.h"

typedef struct {
  Vector2 start, end;
  Color   color;
  float   thickness;
} line;

typedef struct {
  Vector2 origin;
  float   line_thickness;
  Color   line_color;
  matrix2 basis;
  int     line_count;
} grid;

typedef struct {
  float   radius;
  Vector2 center;
  float   theta;
  float   speed;
} circle_path;

void draw_line(const line l);
void draw_grid(const grid* const g);
void update_circle_path(circle_path* const circle, const grid* const g);
