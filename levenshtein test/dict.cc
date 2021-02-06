#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <queue>

std::vector<std::string> wordlist;

void load_dictionary(std::string file)
{
	std::ifstream fs(file);
	std::string line;
	
	while (std::getline(fs,line))
	{
		wordlist.push_back(line);
	}
	
	fs.close();
}


size_t uiLevenshteinDistance(const std::string &s1, const std::string &s2)
{
  const size_t
    m(s1.size()),
    n(s2.size());
 
  if( m==0 ) return n;
  if( n==0 ) return m;
 
  // allocation below is not ISO-compliant,
  // it won't work with -pedantic-errors.
  size_t costs[n + 1];
 
  for( size_t k=0; k<=n; k++ ) costs[k] = k;
 
  size_t i { 0 };
  for (char const &c1 : s1) 
  {
    costs[0] = i+1;
    size_t corner { i },
           j      { 0 };
    for (char const &c2 : s2)
    {
      size_t upper { costs[j+1] };
      if( c1 == c2 ) costs[j+1] = corner;
      else {
        size_t t(upper<corner? upper: corner);
        costs[j+1] = (costs[j]<t?costs[j]:t)+1;
      }
 
      corner = upper;
      j++;
    }
    i++;
  }
 
  return costs[n];
}

class entry{

	public:
	size_t d;
	long i;
	
	entry(size_t di, long ii)
	{
		d = di;
		i = ii;
	}

};

	bool operator< (const entry& e1,const entry& e2)
	{
		return e1.d > e2.d;
	}
	
	bool operator> (const entry& e1,const entry& e2)
	{
		return e1.d < e2.d;
	}


int main(){
	load_dictionary("d:\\downloads\\english3.txt");
	std::string input;
	size_t s;
	std::priority_queue<entry> q;
	
	std::cout << "Enter here: ";
	std::cin >> input;
	
	std::cout << "entered " << input << std::endl;
	long t0;
	
	long i=0;
	
	t0 = clock();
	
	for(std::string w : wordlist)
	{
		s = uiLevenshteinDistance(input,w);
		entry e(s,i);
		q.push(e);
		i++;
	}

 	std::cout << "It took " << clock()-t0 << " ms and " << i << " iterations." << std::endl;
	
	int count=0;

	while (!q.empty())
	{
		entry e = q.top();
		q.pop();
		std::cout << wordlist[e.i] << ", dist: " << e.d << std::endl;
		count++;
		if (count > 20) break;
	}

	return 0;
}