/* Initialize the SVG file after opening */
void svg_header (FILE *f, unsigned int width, unsigned int height);

/* Finalize the SVG file before closing */
void svg_footer (FILE *f);

/* Change the current color (default: red) */
void set_svg_color (char *color);

/* Draw a line using current color between two points */
void svg_line (FILE *f, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);


/* Draw a rectangle */
void svg_rect (FILE *f, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
