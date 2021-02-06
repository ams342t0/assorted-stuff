#include <SDL2/SDL.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <fstream>
#include <string>


#define W 256
#define H 224
#define C(R,G,B) (B) | ((G)<<8)&0X0000FF00 | ((R)<<16)&0X00FF0000
#define GC(i)	(buffer+i*16)
char *buf;
int *buffer;

void loadfont()
{
	std::ifstream ifs;
	int s;
	
	ifs.open("d:\\downloads\\vga\\fonts\\FM-T-437.F08",std::ios::binary|std::ios::in);
	ifs.seekg(0,std::ios::end);
	s = ifs.tellg();
	std::cout << s << std::endl;
	buf = (char*)malloc(s);
	ifs.seekg(0,std::ios::beg);
	ifs.read(buf,s);
	ifs.close();
}

void drawchar(int a,int b,int c)
{
	const char *p = buf+(c*8);
	char cc;
		
		for (int y=b;y<b+8;y++)
		{
			cc = *p;
			for (int x=a;x<a+8;x++)
			{
				if (cc & 0x80)
				{
					*(buffer+y*W+x) = C(255,255,255);
				}
				cc<<=1;
			}
			p++;
		}
}

void drawstring(int x,int y,const char *s)
{
	int sx;
	const char *p=s;
	sx = x;
	while (*p)
	{
		drawchar(sx,y,(int)*p);
		++p;
		sx+=8;
	}
}

int main()
{
	SDL_Window  *w;
	SDL_Surface *ws, *s;
	SDL_Texture *t;
	SDL_Renderer *re;
	SDL_Event e;
	SDL_Rect r;
	r.x=0; r.y=0; r.w=W,r.h=H;
	std::string hud;

	int fc=0;


	loadfont();

	std::chrono::high_resolution_clock::time_point t1,t2;
	std::chrono::duration<double> d;

	
	SDL_Init(SDL_INIT_VIDEO);
	w = SDL_CreateWindow("SDL SURFACE TEST",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W*3,H*3,SDL_WINDOW_OPENGL);
	// SDL_CreateWindowAndRenderer(W*3,H*3,SDL_WINDOW_OPENGL,&w,&re);
	// t =  SDL_CreateTexture(re,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STATIC,W,H);
	ws = SDL_GetWindowSurface(w);
	s =  SDL_CreateRGBSurface(0,W,H,32,0,0,0,0);
	buffer = (int*)s->pixels;
	// buffer = (int*)malloc(W*H*4);
	SDL_ShowWindow(w);
	
	while (1)
	{
		t1 = std::chrono::high_resolution_clock::now();


		for(int y=0;y<H;++y)
			for(int x=0;x<W;++x)
				*(buffer+y*W+x)=C(rand()%255,rand()%255,rand()%255);

		// SDL_UpdateTexture(t,NULL,buffer,W*4);
		// SDL_RenderCopy(re,t,NULL,NULL);
		// SDL_RenderPresent(re);
		drawstring(0,0,hud.c_str());

		SDL_BlitScaled(s,NULL,ws,NULL);
		SDL_UpdateWindowSurface(w);
		
		t2 = std::chrono::high_resolution_clock::now();
		d = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
		hud = "FPS: " + std::to_string((long)(1.0/d.count()));
		fc++;

		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) break;
		}

		
	}
	SDL_DestroyWindow(w);
	SDL_Quit();
//	free(buffer);

	// SDL_DestroyTexture(t);
	// SDL_DestroyRenderer(re);
	
	return 1;
}