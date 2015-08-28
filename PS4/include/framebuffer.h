#pragma once

#define WIDTH 160
#define HEIGHT 144

extern unsigned int *framebuffer;

void getFramebuffer(void);
void clearFramebuffer(void);

void drawBall(int x, int y);
void drawPaddle(int y);
