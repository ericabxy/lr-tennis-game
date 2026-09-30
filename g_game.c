#include <stdint.h>
#include <stdlib.h>

#include "libretro.h"
#include "gamedef.h"
#include "g_game.h"
#include "r_draw.h"

int gstate = WAIT;

void G_InitGame(game_t *game)
{
    game->ball.x = WINDOW_WIDTH / 3;
    game->ball.y = WINDOW_HEIGHT / 4;
    game->ball.dx = 1.5;
    game->ball.dy = 2.3;
    game->ball.color = CGA_WHITE;
    game->ball.size = BALLSIZE;
    game->bounds.x = 0;
    game->bounds.y = 0;
    game->bounds.width = WINDOW_WIDTH;
    game->bounds.height = WINDOW_HEIGHT;
    game->players[0].x = WINDOW_WIDTH >> 1;
    game->players[0].y = 6;
    game->players[0].width = 25;
    game->players[0].height = 3;
    game->players[0].color = CGA_WHITE;
    game->players[1].x = WINDOW_WIDTH >> 1;
    game->players[1].y = WINDOW_HEIGHT - 6;
    game->players[1].width = 25;
    game->players[1].height = 3;
    game->players[1].color = CGA_WHITE;
}

void G_UpdateGame(game_t *game)
{
    G_BallMove(&game->ball, game->bounds);
    gstate = G_CheckReturn(game->players[1], &game->ball, true, SERVE, RETURN, PGUTTER);
    gstate = G_CheckReturn(game->players[0], &game->ball, false, RETURN, SERVE, GGUTTER);
}

void G_RenderGame(game_t *game, uint32_t *buf)
{
    ball_t ball = game->ball;
    paddle_t gPaddle = game->players[0];
    paddle_t pPaddle = game->players[1];

    R_FillOval(buf, ball.x, ball.y, ball.x + ball.size, ball.y + ball.size, ball.color);
    R_FillRect(buf, gPaddle.x - gPaddle.width, gPaddle.y - gPaddle.height, gPaddle.x + gPaddle.width, gPaddle.y + gPaddle.height, gPaddle.color);
    R_FillRect(buf, pPaddle.x - gPaddle.width, pPaddle.y - gPaddle.height, pPaddle.x + pPaddle.width, pPaddle.y + pPaddle.height, pPaddle.color);
}

void G_BallMove(ball_t *ball, rectangle_t bounds)
{
    ball->x += ball->dx;
    ball->y += ball->dy;
    // Check for collision with left edge
    if (ball->x < bounds.x && ball->dx < 0)
    {
        ball->dx = -ball->dx;
        ball->x -= 2 * (ball->x - bounds.x);
    }
    // Check for collision with right edge
    else if ((ball->x + ball->size) > (bounds.x + bounds.width) && ball->dx > 0)
    {
        ball->dx = -ball->dx;
        ball->x -= 2 * ((ball->x + ball->size) - (bounds.x + bounds.width));
    }
    // Check for collision with top edge
    if (ball->y < bounds.y && ball->dy < 0)
    {
        ball->dy = -ball->dy;
        ball->y -= 2 * (ball->y - bounds.y);
    }
    // Check for collision with bottom edge
    else if ((ball->y + ball->size) > (bounds.y + bounds.height) && ball->dy > 0)
    {
        ball->dy = -ball->dy;
        ball->y -= 2 * ((ball->y + ball->size) - (bounds.y + bounds.height));
    }
}

int G_CheckReturn(paddle_t paddle, ball_t *ball, bool plyr, int r1, int r2, int r3)
{
    if (plyr && ball->y > (paddle.y - ball->size) || !plyr && ball->y < (paddle.y + ball->size))
    {
        if (abs(ball->x - paddle.x) < (paddle.width / 2 + ball->size))
        {
            ball->dy = -ball->dy;
            // Put a little english on the ball
            ball->dx += (ball->dx * abs(ball->x - paddle.x) / (paddle.width / 2)) * 0.01;
            return r2;
        }
        else
            return r3;
    }
    return r1;
}
