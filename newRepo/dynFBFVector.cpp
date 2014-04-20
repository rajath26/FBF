/*
 * Header files
 */
#include <iostream>
#include <unistd.h>
#include <vector>

/*
 * Bloom Filter Library
 */
#include "bloom_filter_vector.hpp"

/*
 * Macros
 */
#define DEF_NUM_OF_BFS 100 
#define DFUTURE 0

/* 
 * Global variables
 */ 
int dfuture = DFUTURE;
int dpresent = DFUTURE + 1;
int pastStart = DFUTURE + 2;

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
  vector<bloom_filter> dyn_fbf;

  /* 
   * New BF to create a new future BF 
   * after each refresh time
   */
  bloom_filter newBF;

  unsigned int numberOfBFs;
  unsigned int pastEnd;

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

    dyn_fbf.reserve(100);

    parameters.projected_element_count = 10000;
    parameters.false_positive_probability = 0.0001;
    parameters.random_seed = 0xA5A5A5A5;
    if ( !parameters ) { 
      cout<<" ERROR :: Invalid set of bloom filter parameters " <<endl;
    }
    parameters.compute_optimal_parameters(tableSize, numOfHashes);
    bloom_filter baseBF(parameters);
    cout<<" INFO :: NUMBER OF CONSTITUENT BFs initialized in the FBF: " <<numberBFs <<endl;
    for ( unsigned int counter = 0; counter < numberBFs; counter++ ) { 
      dyn_fbf.push_back(baseBF);
      //dyn_fbf[counter] = baseBF;
    }
    newBF = baseBF;

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
    //cout<<" DEBUG :: Just entered refresh function " <<endl;
    unsigned int j;
    for ( j = (numberOfBFs - 1); j > 0; j-- ) { 
      dyn_fbf[j] = dyn_fbf[j - 1];
    }
    dyn_fbf[j] &= newBF;

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
    dyn_fbf[dpresent].insert(element);
    dyn_fbf[dfuture].insert(element);
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
    long long int i = 1000;
    unsigned int j;

    while ( counter != numberOfInvalids ) { 
      if ( (dyn_fbf[dfuture].contains(i) && dyn_fbf[dpresent].contains(i)) ) {
	smartFP++;
      }
      else if ( (dyn_fbf[dpresent].contains(i) && dyn_fbf[pastStart].contains(i)) ) {
	smartFP++;
      }
      else if ( (dyn_fbf[pastEnd].contains(i)) ) {
	smartFP++;
      }
      else { 
        for ( j = pastStart; j < pastEnd; j++ ) {
          if ( (dyn_fbf[j].contains(i) && dyn_fbf[j + 1].contains(i)) ) {
            smartFP++;
            break;
          }
        }
      }

      i++;
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
