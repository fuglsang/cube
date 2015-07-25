#include <gb/gb.h>
#include <gb/drawing.h>

#define WX GRAPHICS_WIDTH
#define WY GRAPHICS_HEIGHT

/* vertex indices

      7           6
       +---------+
      /|        /|
   3 / |     2 / |
    +---------+  |
    |  |      |  |
    |  +------|--+
    | / 4     | / 5
    |/        |/
    +---------+
   0           1
*/

typedef struct
{
  INT8 x;
  INT8 y;
  INT8 z;
} vertex_t;

typedef struct
{
  UINT8 i;
  UINT8 j;
} edge_t;

vertex_t vertices[8] = {
  { -1, -1,  1 },
  {  1, -1,  1 },
  {  1,  1,  1 },
  { -1,  1,  1 },
  { -1, -1, -1 },
  {  1, -1, -1 },
  {  1,  1, -1 },
  { -1,  1, -1 },
};

edge_t edges[12] = {
  { 0, 1 },
  { 0, 3 },
  { 0, 4 },
  { 1, 2 },
  { 1, 5 },
  { 2, 3 },
  { 2, 6 },
  { 3, 7 },
  { 4, 5 },
  { 4, 7 },
  { 4, 5 },
  { 5, 6 },
  { 6, 7 },  
};

void main(void)
{
  UINT8 x = 0;
  UINT8 y = 0;

  disable_interrupts();
  cpu_fast();
  enable_interrupts();
  
  color (BLACK, WHITE, SOLID);
  
  while (1)
  {
    x = (x + 1) % WX;
    y = (y + 1) % WY;
    
    plot_point(x, y);
  }
}