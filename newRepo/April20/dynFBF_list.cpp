/*
 * Header files
 */
#include <iostream>
#include <unistd.h>
#include <list>

/*
 * Bloom Filter Library
 */
#include "bloom_filter.hpp"

/*
 * Macros
 */
#define DEF_NUM_OF_BFS 100 
#define DFUTURE 0

/* 
 * Global variables
 */ 
unsigned int dfuture = DFUTURE;
unsigned int dpresent = DFUTURE + 1;
unsigned int pastStart = DFUTURE + 2;
unsigned int numberOfBFs;
unsigned int pastEnd;

using namespace std;

/*
 * Dynamic FBF class
 */
/*******************************************************************
 *******************************************************************
 ** CLASS NAME: dynFBF (Forgetful Bloom Filter)
 **
 ** NOTE: This class implements the dynamic FBF ie it contains the 
 **       minimum THREE constituent bloom filters (BF) namely:
 **       i) Past BF
 **       ii) Present BF
 **       iii) Future BF
 **       but can dynamically expand by adding multiple past BFs as
 **       the load increases
 **
 ** The class is mainly used to compare to run dynamic resizing tests 
 *******************************************************************
 *******************************************************************/
class dynFBF { 

public:
  /* 
   * Bloom Filter parameters 
   */
  bloom_parameters parameters;
  /* 
   * Constituent BFs of the FBF
   * Past, Present and Future BFs
   * The vector can accomodate multiple past BFs as well
   */
  //bloom_filter dyn_fbf[DEF_NUM_OF_BFS];
  list<bloom_filter *> dyn_fbf;
  /* 
   * New BF to create a new future BF 
   * after each refresh time
   */
  //bloom_filter newBF;

  /************************************************************ 
   * FUNCTION NAME: dynFBF 
   *
   * Constructor of the FBF class
   * 
   * PARAMETERS: 
   *            numberOfBFs: gives the number of constituent BFs
   *                         to be initialized in the FBF
   *            tableSize: gives the number of bits in each of the 
   *                       constituent BFs in the FBF
   *            numOfHashes: gives the number of hashes to be used 
   *                         by the constituent BFs in the FBF
   * 
   * RETURNS: NA 
   ************************************************************/
  dynFBF(unsigned long numberBFs, 
         unsigned long long int tableSize, 
         unsigned int numOfHashes) { 

    parameters.projected_element_count = 10000;
    parameters.false_positive_probability = 0.0001;
    parameters.random_seed = 0xA5A5A5A5;
    if ( !parameters ) { 
      cout<<" ERROR :: Invalid set of bloom filter parameters " <<endl;
    }

    parameters.compute_optimal_parameters(tableSize, numOfHashes);

    //bloom_filter baseBF(parameters);

    cout<<" INFO :: NUMBER OF CONSTITUENT BFs initialized in the FBF: " <<numberBFs <<endl;

    for ( unsigned int counter = 0; counter < numberBFs; counter++ ) { 
      dyn_fbf.push_back(new bloom_filter(parameters));
    }
    //newBF = bloom_filter(parameters);

    // Update the class members
    numberOfBFs = numberBFs;
    pastEnd = numberBFs - 1;

    cout<<" INFO :: dfuture: " <<dfuture <<endl;
    cout<<" INFO :: dpresent: " <<dpresent <<endl;
    cout<<" INFO :: pastStart: " <<pastStart <<endl;
    cout<<" INFO :: pastEnd: " <<pastEnd <<endl;

    cout<<" INFO :: FBFs initialized " <<endl;
  }

  /************************************************************
   * FUNCTION NAME: ~FBF
   * 
   * Destructor of the FBF class
   *
   * RETURNS: NA
   ************************************************************/
  ~dynFBF() {}

  /************************************************************
   * FUNCTION NAME: refresh
   * 
   * This function refreshes the FBF
   * 
   * RETURNS: void 
   ************************************************************/
  void refresh() { 
    dyn_fbf.pop_back();
    dyn_fbf.push_front(new bloom_filter(parameters));

    cout<<" INFO :: Refreshed FBF" <<endl;
  }

  /************************************************************
   * FUNCTION NAME: insert
   * 
   * This function inserts into the FBF
   * NOTE: When elements are inserted into the FBF they are 
   *       inserted into the following constituent BFs:
   *       i) present BF
   *       ii) future BF
   *
   * PARAMETERS: 
   *            element: element to be inserted into the FBF
   * 
   * RETURNS: void
   ************************************************************/
  void insert(unsigned long long int element) { 
    list<bloom_filter *>::iterator it;
    int counter;
    for ( it = dyn_fbf.begin(), counter = 0; counter < 2; it++, counter++ ) {
    	(*it)->insert(element);
    }
  }

  /************************************************************
   * FUNCTION NAME: checkSmartFBF_FPR
   * 
   * This function checks the False Positives (FPs) and the 
   * False Positive Rate (FPR) of the FBF using SMART RULES
   * 
   * PARAMETERS: 
   *            numberOfInvalids: Number of invalid membership 
   *                              checks to be made
   * 
   * RETURNS: void
   ***********************************************************/
  void checkSmartFBF_FPR(unsigned long long int numberOfInvalids) { 
    unsigned long long int smartFP = 0;
    double smartFPR = 0.0;
    unsigned int counter = 0;
    long long int i = -1;
    unsigned int j = 0;
    int found = 0;

    list<bloom_filter *>::iterator it;
    list<bloom_filter *>::iterator temp;

    while ( counter != numberOfInvalids ) { 

      it = dyn_fbf.begin();
      while ( it != dyn_fbf.end() ) {
    	  temp = it;
    	  temp++;
    	  if ( (*it)->contains(i) && (*temp)->contains(i) ) {
    		  smartFP++;
    		  found = 1;
    	  }
    	  if ( 1 == found ) {
    		  goto next;
    	  }
    	  it++;
      }

      if ( found != 1 ) {
    	  if ( (*it)->contains(i) ) {
    		  smartFP++;
    	  }
      }


      next:
        i--;
        counter++;

    }

    smartFPR = (double) smartFP/numberOfInvalids;

    cout<<" RESULT :: SMART FP = " <<smartFP <<endl;
    cout<<" RESULT :: SMART FPR = " <<smartFPR <<endl;
  }

}; // End of dynFBF class

/* 
 * EOF
 */
