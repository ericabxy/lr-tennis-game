#include <stdint.h>
#include <stdbool.h>

#ifndef __G_GAME__
#define __G_GAME__

#define BALLSIZE 12
#define WAIT 1
#define SERVE 2
#define RETURN 4
#define PGUTTER 8
#define GGUTTER 16
#define PSCORE 32
#define GSCORE 64
#define PWON 128
#define GWON 256

// Game states
typedef enum
{
    STATE_WAIT,
    STATE_SERVE,
    STATE_RETURN,
    STATE_PGUTTER,
    STATE_GGUTTER,
    STATE_PSCORE,
    STATE_GSCORE,
    STATE_PWON,
    STATE_GWON,
} GameState;

typedef struct
{
    float x, y;
    int width, height;
} rectangle_t;

typedef struct
{
    float x, y, dx, dy;
    uint32_t color;
    int size;
} ball_t;

typedef struct
{
    float x, y, width, height;
    uint32_t color;
} paddle_t;

typedef struct
{
    ball_t ball;
    rectangle_t bounds;
    paddle_t players[2];
} game_t;

void G_InitGame(game_t *game);
void G_UpdateGame(game_t *game);
void G_RenderGame(game_t *game, uint32_t *buf);
void G_BallDraw(ball_t ball, uint32_t *buf);
void G_BallMove(ball_t *ball, rectangle_t bounds);
int G_CheckReturn(paddle_t paddle, ball_t *ball, bool plyr, int r1, int r2, int r3);

#endif
