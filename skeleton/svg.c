#include <stdio.h>
#include "svg.h"

char *svg_color = "red";

void set_svg_color(char *color)
{
  svg_color = color;
}

void svg_header(FILE *f, unsigned int width, unsigned int height)
{
  fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",
      width, height);
}

void svg_footer(FILE *f) {
  fprintf(f, "</svg>\n");
}

void svg_line (FILE *f, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  if ((x1 == x2)&&(y1==y2)) return;
  fprintf(f, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(0,0,0);stroke-width:0.2\"/>\n",
      x1, y1, x2, y2);
}



void svg_rect (FILE *f, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  if ((x1 == x2)&&(y1==y2)) return;
  fprintf(f, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"%s\" style=\"stroke:rgb(255,255,255);stroke-width:0;\"/>\n",
      x1, y1, x2-x1, y2-y1, svg_color);
}

