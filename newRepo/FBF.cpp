/*
 * Header files
 */
#include <iostream>
#include <unistd.h>

/*
 * Bloom Filter Library
 */
#include "bloom_filter.hpp"

/*
 * Macros
 */
#define MINIMUM_NUM_OF_BFS 3
#define DEFAULT_NUM_OF_HASH_FUNCTIONS 3

using namespace std;

/*
 * FBF class
 */
/*******************************************************************
 ** CLASS NAME: FBF (Forgetful Bloom Filter)
 **
 ** NOTE: This class implements the basic FBF ie it contains the 
 **       minimum THREE constituent bloom filters (BF) namely:
 **       i) Past BF
 **       ii) Present BF
 **       iii) Future BF
 **
 **       The class is mainly used to compare a smart FBF v/s 
 **       dumb/naive FBF
 *******************************************************************/
class FBF { 

public:
  bloom_parameters parameters;
  bloom_filter fbf[MINIMUM_NUM_OF_BFS];

  FBF(unsigned long long int tableSize, unsigned int numOfHashes) { 
    parameters.projected_element_count = 10000;
    parameters.false_positive_probability = 0.0001;
    parameters.random_seed = 0xA5A5A5A5;
    if ( !parameters ) { 
      cout<<" ERROR :: Invalid set of bloom filter parameters " <<endl;
    }
    parameters.compute_optimal_parameters(tableSize, numOfHashes);
    bloom_filter baseBF(parameters);
    for ( int counter = 0; counter < MINIMUM_NUM_OF_BFS; counter++ ) { 
      fbf[counter] = baseBF;
    }

    cout<<" INFO :: FBFs initialized " <<endl;
  }

  ~FBF() {}

}; // End of FBF class
