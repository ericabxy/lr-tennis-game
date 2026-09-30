#include <stdint.h>

#ifndef __R_DRAW__
#define __R_DRAW__

void R_DrawChecks(uint32_t *buf, uint32_t rgba1, uint32_t rgba2);
void R_DrawQuarters(uint32_t *buf, uint32_t rgba1, uint32_t rgba2);
void R_DrawLine(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba);
void R_DrawPoint(uint32_t *buf, int x, int y, uint32_t rgba);
void R_DrawOval(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba);
void R_FillOval(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba);
void R_FillRect(uint32_t *buf, int rx, int ry, int width, int height, uint32_t rgba);

#endif
