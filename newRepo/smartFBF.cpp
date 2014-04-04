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
 * FBF classes
 */
#include "FBF.cpp"

/*
 * Macros
 */
#define FAILURE -1
#define SUCCESS 0
#define NUM_OF_ARGS 2
#define DEFAULT_NUM_OF_HASH_FUNCTIONS 3
#define DEFAULT_TABLE_SIZE 6250

using namespace std;

void smartFBFvsDumbFBFvaryNumElements(unsigned long long int numElements) { 

  /*
   * Create the FBF 
   */
  FBF oFBF(DEFAULT_TABLE_SIZE, DEFAULT_NUM_OF_HASH_FUNCTIONS);

} // End of smartFBFvsDumbFBFvarNumElements()

/*
 * Main function
 */
int main(int argc, char *argv[]) { 

  if ( argc != NUM_OF_ARGS ) { 
    cout<<" ERROR :: Usage: " <<argv[0] <<" <number_of_elements> " <<endl;
    return FAILURE;
  }

  unsigned long long int numElements;
  numElements = (unsigned long long int) atoi(argv[1]);
  cout<<" INFO :: Number of elements to be inserted: " <<numElements <<endl;

  FBF oFBF(numElements, DEFAULT_NUM_OF_HASH_FUNCTIONS);

} // End of main()
