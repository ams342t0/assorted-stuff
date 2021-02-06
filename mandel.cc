#include <SDL2\SDL.h>
#include <chrono>
#include <omp.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <immintrin.h>


#define W 320
#define H 240
#define MAXITER 32


int main(int argc, char ** argv)
{
	SDL_Event e;
	SDL_Window *w;
	SDL_Renderer *r;
	SDL_Texture *t;
	int *pixmem = (int*)malloc(320*240*4);
	
	std::chrono::high_resolution_clock::time_point t1,t2;
	std::chrono::duration<double> ts;
	
	std::vector<long> pixbuffer(W*H,0);
	bool loop=true;
	
	double cx = -0.5, 
	cy = 0.0,
	cr = 1.0,cd = 0.001,
	minx = cx - cr,
	maxx = cx + cr,
	miny = cy - cr,
	maxy = cy + cr;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(W*3,H*3,0,&w,&r);
	t = SDL_CreateTexture(r,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STATIC,320,240);

	while (1)
	{

		t1 = std::chrono::high_resolution_clock::now();
		cr -= cd;
		minx = cx - cr;
		maxx = cx + cr;
		miny = cy - cr;
		maxy = cy + cr;
		
		#pragma omp parallel
		{
			int ki= omp_get_thread_num();
			int h1 = ki*60;
			int h2 = h1+59;
			double cfx = (maxx-minx)/W;
			double cfy = (maxy-miny)/H;
		
			#pragma omp master
			if (omp_get_num_threads() == 1) { h1 = 0; h2 = H-1;	}
			
			for (int h=h1;h<=h2;++h)
			{
				double c2 = miny + cfy*h;

				for(int w=0;w<W;++w)
				{
					double c1 = minx + cfx*w;
					double z1 = 0.0, z2 = 0.0;

					int i=0;
					
					while (i<=MAXITER)
					{
						double n1 = z1*z1-z2*z2 + c1;
						double n2 = 2.0*z1*z2+c2;
						z1 = n1;
						z2 = n2;
						if ((z1*z1+z2*z2) >= 4.0) break;
						++i;
					}
					// if (i < MAXITER)  
					pixbuffer[h*W+w] = 255*((double)i/MAXITER);
				}
			}
		}
		
		if (cr <= 0.5 || cr >= 1.0) cd = -cd;
		
		for (int y=0;y<H;y++)
			for(int x=0;x<W;x++)
			{
					int c = pixbuffer[y*W+x];
					// SDL_SetRenderDrawColor(r,c*0.25,c*0.45,c*0.3,0);
					// SDL_RenderDrawPoint(r,x+p2,y+p1);
					*(pixmem+y*W+x) = c;
			}
		
		SDL_UpdateTexture(t,NULL,pixmem,1280);
		SDL_RenderCopy(r,t,NULL,NULL);
		SDL_RenderPresent(r);

		t2 = std::chrono::high_resolution_clock::now();
		ts = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
		std::cout << 1000.0*ts.count() << std::endl;

		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) break;
		}
	}
	
	free(t);
	SDL_DestroyTexture(t);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	
	
	return EXIT_SUCCESS;
}