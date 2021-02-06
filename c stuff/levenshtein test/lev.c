#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MIN3(a,b,c) MIN(MIN((a),(b)),(c))

int min(int a,int b,int c)
{
	return MIN(MIN(a,b),c);
}


size_t lev2(const char *s1, const char *s2)
{
  const size_t m=strlen(s1), n=strlen(s2);
 
  if( m==0 ) return n;
  if( n==0 ) return m;
 
  // allocation below is not ISO-compliant,
  // it won't work with -pedantic-errors.
  size_t costs[n + 1];
 
  for( size_t k=0; k<=n; k++ ) costs[k] = k;
 
  for (int k=0;k<m;k++) 
  {
    costs[0] = k+1;
    size_t corner=k;
    for (int u=0;u<n;u++)
    {
      size_t upper=costs[u+1];

      if( s1[k] == s2[u]) costs[u+1] = corner;
      else {
		costs[u+1] = MIN3(upper,corner,costs[u])+1;
      }
 
      corner = upper;
    }
  }
 
  return costs[n];
}


int levdist(const char *s1,const char *s2)
{
	static long i = 0;
	
	i++;
	//if (i>10000000) return 10000000;
	
	if (strlen(s1) == 0) return strlen(s2);
	if (strlen(s2) == 0) return strlen(s1);
	
	if (*s1 == *s2) levdist(s1+1,s2+1);
	else
		return 1+min(levdist(s1+1,s2),levdist(s1,s2+1),levdist(s1+1,s2+1));
}

int main()
{
	char i1[256],i2[256],p;
	int done=0;

	while (!done)
	{
		printf("\nText 1: ");
		scanf("%s",i1);
		printf("Text 2: ");
		scanf("%s",i2);
		printf("\nEdit distance: %d",lev2(i1,i2));
		printf("\nContinue? ");
		scanf("%c%c",&p,&p);
		if (p == 'N' || p == 'n') done = 1;
	}

	return 0;
}