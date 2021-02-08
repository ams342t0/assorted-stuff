#include <SDL2/SDL.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <fstream>
#include <string>
#include <cmath>

#define W 1368
#define H 720
#define C(R,G,B) (B) | ((G)<<8)&0X0000FF00 | ((R)<<16)&0X00FF0000

int *buffer;


void make_cos_table();
void do_plasma_c();

unsigned char cos_table[256];
int colors[256]={0};
unsigned char 	p1=0, p2=0, p3=0, p4=0,
				p1i=1, p2i=2, p3i=1, p4i=2;

//////////////////////////////////////////////////////////////////////////
void make_cos_table()
{
  for(int a=0; a<256; a++)
    cos_table[a]=(int)((cos(360.0*a/255*M_PI/180)*128)+128);
}

void make_palette()
{
  for(int a=0; a<256; a+=8)
  {
      colors[a+0] = C(a*2,0,0);
      colors[a+1] = C(63-a*2,a*2,0);
      colors[a+2] = C(0,63-a*2,a*2);
      colors[a+3] = C(a*2,a*2,63);
      colors[a+4] = C(63-a*2,63,63-a*2);
      colors[a+5] = C(a*2,63-a*2,0);
      colors[a+6] = C(63-a*2,0,a*2);
      colors[a+7] = C(0,0,63-a*2);
  }
}


//////////////////////////////////////////////////////////////////////////
void do_plasma_c()
  {
  int x=0, y=0;
  unsigned int where=0;
  unsigned char vptr1, vptr2, hptr1, hptr2;

  vptr1=p3;
  vptr2=p4;

  do
    {
    vptr1+=p2i;
    vptr2+=p1i;
    hptr1=p2;
    hptr2=p1;
    x=0;
    do
    {
      hptr1+=p4i;
      hptr2+=p3i;
      buffer[where++] = cos_table[hptr1] + cos_table[hptr2]+cos_table[vptr1] + cos_table[vptr2];
    }while(++x<W);
  }while(++y<H);

  p1-=1;
  p2+=2;
  p3-=3;
  p4+=4;
}


//////////////////////////////////////////////////////////////////////////
int main()
{
	SDL_Window  *w;
	SDL_Surface *ws, *s;
	SDL_Event e;
	
	SDL_Init(SDL_INIT_VIDEO);
	w = SDL_CreateWindow("SDL PLASMA",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_OPENGL);
	ws = SDL_GetWindowSurface(w);
	s =  SDL_CreateRGBSurface(0,W,H,32,0,0,0,0);
	buffer = (int*)s->pixels;


	// make_palette();
	make_cos_table();

	SDL_ShowWindow(w);
	
	while (1)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		do_plasma_c();

		SDL_BlitScaled(s,NULL,ws,NULL);
		SDL_UpdateWindowSurface(w);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto d = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
		auto hud = "FPS :" + std::to_string(1/d.count());
		std::cout << hud << "\r";
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) break;
		}
	}
	
	SDL_DestroyWindow(w);
	SDL_Quit();
	
	return 1;
}

