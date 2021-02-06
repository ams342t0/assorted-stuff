/*
a very simple use case of parallelism implemented in C/C++
*/

#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <omp.h>
#include <stdio.h>
#include <pthread.h>

#define ASIZE 1000000000

using namespace std;
using namespace std::chrono;


float *vla;	//very large 32-bit float array

high_resolution_clock::time_point t1,t2;
duration<double> d;
__m256 v,vm = _mm256_set1_ps(0.777f);


int init_vla()
{
	vla = (float*)malloc(sizeof(float)*ASIZE);
	
	if (!vla)	
	{	
		cout << "No more memory" << endl;
		return 0;
	}

	for (long i=0;i<ASIZE;++i)
		*(vla+i) = (float)rand()*1.0339;

	cout << ASIZE << " floats ok." << endl;
	return 1;
}

void print_some()
{
	cout << endl;

	for(int i=0;i<ASIZE;i+=(ASIZE/10))
		cout << *(vla+i) << endl;
	cout << endl;
}



double use_plainloop()
{
	// normal
	t1 = high_resolution_clock::now();
	
	for (int i=0;i<ASIZE;++i)
		*(vla+i) *= 0.777f;
	
	t2 = high_resolution_clock::now();
	
	d = duration_cast<duration<double>>(t2-t1);
	
	return d.count();
}


struct boundary{
	int lo, hi,id;
};


//use threads
double use_threads()
{
	#define THREADS 16
	pthread_t threads[THREADS];
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	auto tproc = [](void* d) -> void*{
		struct boundary b;
		b = *((struct boundary*)d);

		for (int i=b.lo;i<=b.hi;++i)
		{
			*(vla+i) *= 0.777f;
		}
		
		pthread_exit(0);
	};
	
	pthread_attr_destroy(&attr);
	
	
	t1 = high_resolution_clock::now();
	
	// run threads

	int inter = ASIZE/THREADS;
	struct boundary b[THREADS];
	
	for(int i=0;i<THREADS;++i)
	{
		b[i].id = i;
		b[i].lo = i*inter;
		b[i].hi = b[i].lo + inter - 1;
		pthread_create(&threads[i],&attr,tproc,(void*)&b[i]);
	}
	
	// join threads
	for (int i=0;i<THREADS;++i)
	{
		pthread_join(threads[i],NULL);
	}
	
	
	t2 = high_resolution_clock::now();
	
	d = duration_cast<duration<double>>(t2-t1);

	return d.count();
	#undef THREADS
}


//use threads + avx
double use_threads_avx()
{
	#define THREADS 4
	pthread_t threads[THREADS];
	pthread_attr_t attr;
	static pthread_mutex_t m;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	auto tproc = [](void* d) -> void*{
		struct boundary b = *((struct boundary*)d);
		__m256 v;
		for (int i=b.lo;i<=b.hi;i+=8)
		{
			v = _mm256_load_ps(vla+i);
			v = _mm256_mul_ps(v,vm);
			_mm256_store_ps(vla+i,v);
		}
		pthread_exit(0);
	};
	
	pthread_attr_destroy(&attr);
	
	
	t1 = high_resolution_clock::now();
	
	// run threads
	int inter = ASIZE/THREADS;
	struct boundary b[THREADS];
	
	for(int i=0;i<THREADS;++i)
	{
		b[i].id = i;
		b[i].lo = i*inter;
		b[i].hi = b[i].lo + inter - 1;
		pthread_create(&threads[i],&attr,tproc,(void*)&b[i]);
	}
	
	// join threads
	for (int i=0;i<THREADS;++i)
	{
		pthread_join(threads[i],NULL);
	}
	
	
	t2 = high_resolution_clock::now();
	
	d = duration_cast<duration<double>>(t2-t1);

	return d.count();
	#undef THREADS
}


// openmp

double use_omp()
{
	t1 = high_resolution_clock::now();
	
	#pragma omp parallel
	{
		int c = omp_get_num_threads();
		int ival = ASIZE/c, rem=ASIZE%c;
		int id = omp_get_thread_num();
		int lo = id*ival;
		int hi = lo + ival - 1;
		if (id == c-1) hi += rem;
		for (int i=lo;i<=hi;++i)
			*(vla+i) *= 0.777f;
	}
	
	t2 = high_resolution_clock::now();
	
	d = duration_cast<duration<double>>(t2-t1);
	return d.count();
}

// using intrinsics

double use_avx()
{
	t1 = high_resolution_clock::now();

	
	for (int i=0;i<ASIZE;i+=8)
	{
		v = _mm256_load_ps(vla+i);
		v =_mm256_mul_ps(v,vm);
		_mm256_store_ps(vla+i,v);
	}
	
	t2 = high_resolution_clock::now();
	
	d = duration_cast<duration<double>>(t2-t1);
	return d.count();
}


// using intrinsics & omp

double use_omp_avx()
{
	t1 = high_resolution_clock::now();

	#pragma omp parallel
	{
		int c = omp_get_num_threads();
		int ival = ASIZE/c, rem=ASIZE%c;
		int id = omp_get_thread_num();
		int lo = id*ival;
		int hi = lo + ival - 1;
		if (id == c-1) hi += rem;
		__m256 v;
		for (int i=lo;i<=hi;i+=8)
		{
			v = _mm256_load_ps(vla+i);
			v = _mm256_mul_ps(v,vm);
			_mm256_store_ps(vla+i,v);
			
		}
	}
	
	t2 = high_resolution_clock::now();
	
	d = duration_cast<duration<double>>(t2-t1);
	return d.count();
}

struct g_: numpunct<char>
{
	string do_grouping() const override {return "\3";}
};

int main()
{
	cout.imbue(locale(cout.getloc(),new g_));
	
	if (!init_vla()) return 0;  //just in case out of memory
	double b,r;

	b = use_plainloop();   // use as time baseline
	cout << "Plain loop    : " << b << " seconds" << endl;

	r = use_threads();                                               
	cout << "Using threads : " << r << " seconds, " <<  100.0*b/r << "% of base" <<endl;		

	r = use_threads_avx();                                               
	cout << "Threads+avx   : " << r << " seconds, " <<  100.0*b/r << "% of base" <<endl;		
	
	r = use_omp();
	cout << "Using OMP     : " << r << " seconds, " <<  100.0*b/r << "% of base" <<endl;
																
	r = use_avx(); 	                                                 
	cout << "Using AVX     : " << r << " seconds, " <<  100.0*b/r << "% of base" <<endl;
																
	r = use_omp_avx();                                               
	cout << "Using AVX+OMP : " << r << " seconds, " <<  100.0*b/r << "% of base" <<endl;	
																	   
	free(vla);
	return 1;
}