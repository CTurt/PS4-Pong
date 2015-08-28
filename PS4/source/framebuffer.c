#include "ps4.h"

#include "framebuffer.h"

unsigned int *framebuffer = NULL;

void getFramebuffer(void) {
	struct memoryRegionInfo info;
	struct otherMemoryRegionInfo otherInfo;
	
	void *m = NULL;
	
	int i;
	for(i = 0; i < 16; i++) {
		// Find base of next mapping
		getOtherMemoryInfo(m, 1, &otherInfo);
		
		// Get more info about this mapping
		getMemoryInfo(otherInfo.base, &info);
		
		// Search mappings 14 and 15 if they are readable
		if((i == 14 || i == 15) && (info.flags & PROT_CPU_READ)) {
			framebuffer = info.base;
			
			// Search for the colour we filled the framebuffer with
			while(framebuffer < (unsigned int *)info.end) {
				if(
					framebuffer[0] == 0xffc2c2c2 &&
					framebuffer[1] == 0xffc2c2c2
				) break;
				
				framebuffer++;
			}
		}
		
		m = info.end;
	}
}

void clearFramebuffer(void) {
	unsigned int *f = framebuffer;
	while(f < framebuffer + WIDTH * HEIGHT) {
		*f = BLACK;
		f++;
	}
}

void drawBall(int x, int y) {
	if(x < 0) x = 0;
	if(x > WIDTH - 8) x = WIDTH - 8;
	if(y < 0) y = 0;
	if(y > HEIGHT - 8) y = HEIGHT - 8;
	
	int lx, ly;
	for(lx = x; lx < x + 8; lx++) {
		for(ly = y; ly < y + 8; ly++) {
			framebuffer[lx + ly * WIDTH] = WHITE;
		}
	}
}

void drawPaddle(int y) {
	int x = 8;
	
	int lx, ly;
	for(lx = x; lx < x + 8; lx++) {
		for(ly = y; ly < y + 32; ly++) {
			framebuffer[lx + ly * WIDTH] = WHITE;
		}
	}
}
