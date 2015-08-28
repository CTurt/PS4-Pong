#include "ps4.h"

#include "framebuffer.h"
#include "controller.h"

void *pong(void *arg) {
	int ballX = ((WIDTH - 8) / 2) << 8;
	int ballY = ((HEIGHT - 8) / 2) << 8;
	int ballVX = 2 << 8;
	int ballVY = 2 << 8;
	
	int paddleY = (HEIGHT - 32) / 2;
	
	initController();
	
	getFramebuffer();
	
	while(1) {
		clearFramebuffer();
		
		drawPaddle(paddleY);
		drawBall(ballX >> 8, ballY >> 8);
		
		readController();
		
		if(buttons & (1 << 6) && paddleY > 0) {
			paddleY -= 4;
		}
		
		if(buttons & (1 << 7) && paddleY < HEIGHT - 32) {
			paddleY += 4;
		}
		
		if(ballX < -(8 << 8)) {
			ballX = ((WIDTH - 8) / 2) << 8;
			ballY = ((HEIGHT - 8) / 2) << 8;
			ballVX = 2 << 8;
			ballVY = 2 << 8;
			
			sceKernelSleep(1);
		}
		
		if((ballX >> 8 <= 16 && ballY >> 8 > paddleY - 4 && ballY >> 8 < paddleY + 32 - 4) || ballX >> 8 >= WIDTH - 8) {
			ballX = (ballX >> 8 <= 16) ? 16 << 8 : (WIDTH - 8) << 8;
			ballVX = -ballVX;
			ballVX += 16 * ((ballVX > 0) * 2 - 1);
		}
		
		if(ballY >> 8 <= 0 || ballY >> 8 >= HEIGHT - 8) {
			ballY = (ballY >> 8 <= 0) ? 0 : (HEIGHT - 8) << 8;
			ballVY = -ballVY;
			ballVY += 16 * ((ballVY > 0) * 2 - 1);
		}
		
		if(ballVX > 8 << 8) ballVX = 8 << 8;
		if(ballVY > 8 << 8) ballVY = 8 << 8;
		
		ballX += ballVX;
		ballY += ballVY;
		
		// ~60 FPS
		sceKernelUsleep(1000000 / 60);
	}
	
	deinitController();
	
	return NULL;
}

int _main(void) {
	// Init and resolve libraries
	initLibc();
	initKernel();
	initPthread();
	initNetwork();
	
	
	// Create our pong thread
	ScePthread thread;
	scePthreadCreate(&thread, NULL, pong, NULL, "pong");
	
	
	// Let this thread get back to updating the JS engine
	return 0;
}
