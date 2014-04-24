#include <iostream>
#include <list>

#include "bloom_filter.hpp"

using namespace std;

int main () {
	bloom_parameters parameters;
	parameters.compute_optimal_parameters(6250, 5);

    bloom_filter newBF;

	list<bloom_filter> fbf;
	list<bloom_filter>::iterator it;
	int counter;

	newBF = bloom_filter(parameters);

	for ( int counter = 0; counter < 5; counter++ ) {
		fbf.push_back(bloom_filter(parameters));
	}

    for ( int i = 0; i < 5; i++ ) { 
	  for ( it = fbf.begin(), counter = 0; counter < 2; counter++, it++ ) {
		(*it).insert(i);
		cout<<" INFO:: Inserting into the FBF " <<endl;
	  }
	}

	std::cout<<" INFO :: Number of constituent BFs in FBF is: 5" <<std::endl;

	return 0;
}