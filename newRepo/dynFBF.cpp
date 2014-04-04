/*
 * Header files
 */
#include <iostream>
#include <unistd.h>
#include <vector>

/*
 * Bloom Filter Library
 */
#include "bloom_filter.hpp"

/*
 * Macros
 */

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
  unsigned int future = 0;
  unsigned int present = future + 1;
  unsigned int pastStart = present + 1; // or present + 1
  unsigned int pastEnd;

  /************************************************************ 
   * FUNCTION NAME: FBF 
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
  dynFBF(unsigned int numberBFs, 
      unsigned long long int tableSize, 
      unsigned int numOfHashes) { 
    numberOfBFs = numberBFs;
    parameters.projected_element_count = 10000;
    parameters.false_positive_probability = 0.0001;
    parameters.random_seed = 0xA5A5A5A5;
    if ( !parameters ) { 
      cout<<" ERROR :: Invalid set of bloom filter parameters " <<endl;
    }
    parameters.compute_optimal_parameters(tableSize, numOfHashes);
    bloom_filter baseBF(parameters);
    cout<<" INFO :: NUMBER OF CONSTITUENT BFs initialized in the FBF: " <<numberBFs <<endl;
    for ( int counter = 0; counter < numberBFs; counter++ ) { 
      dyn_fbf.push_back(baseBF); 
    }
    newBF = baseBF;

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
  /*
  void refresh() { 
    //cout<<" DEBUG :: Just entered refresh function " <<endl;
    unsigned int j;
    for ( j = (MINIMUM_NUM_OF_BFS - 1); j > 0; j-- ) { 
      //cout<<" DEBUG :: Inside for loop. Iteration number: " <<j <<endl;
      fbf[j] = fbf[j - 1];
    }
    fbf[j] &= newBF;

    cout<<" INFO :: Refreshed FBF" <<endl;
  }
  */

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
  /*
  void insert(unsigned long long int element) { 
    fbf[present].insert(element);
    fbf[future].insert(element);
  }
  */

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
  /*
  void checkSmartFBF_FPR(unsigned long long int numberOfInvalids) { 
    unsigned long long int smartFP = 0;
    double smartFPR = 0.0;
    unsigned int counter = 0;
    long long int i = -1;
    unsigned int j;

    while ( counter != numberOfInvalids ) { 
      if ( (fbf[future].contains(i) && fbf[present].contains(i)) ) {
	smartFP++;
      }
      else if ( (fbf[present].contains(i) && fbf[past].contains(i)) ) {
	smartFP++;
      }
      else if ( (fbf[past].contains(i)) ) {
	smartFP++;
      }

      i--;
      counter++;
    }

    smartFPR = (double) smartFP/numberOfInvalids;

    cout<<" RESULT :: SMART FP = " <<smartFP <<endl;
    cout<<" RESULT :: SMART FPR = " <<smartFPR <<endl;
  }
  */

  /************************************************************
   * FUNCTION NAME: checkDumbFBF_FPR
   * 
   * This function checks the False Positives (FPs) and the 
   * False Positive Rate (FPR) of the FBF using NAIVE RULES
   * 
   * PARAMETERS: 
   *            numberOfInvalids: Number of invalid membership 
   *                              checks to be made
   * 
   * RETURNS: void
   ***********************************************************/
  /*
  void checkDumbFBF_FPR(unsigned long long int numberOfInvalids) { 
    unsigned long long int dumbFP = 0;
    double dumbFPR = 0.0;
    unsigned int counter = 0;
    long long int i = -1;
    unsigned int j;

    while ( counter != numberOfInvalids ) { 
      if ( (fbf[past].contains(i) || fbf[present].contains(i) || fbf[future].contains(i)) ) {
	dumbFP++;
      }

      i--;
      counter++;
    }

    dumbFPR = (double) dumbFP/numberOfInvalids;

    cout<<" RESULT :: DUMB FP = " <<dumbFP <<endl;
    cout<<" RESULT :: DUMB FPR = " <<dumbFPR <<endl;
  }
  */

}; // End of FBF class

/* 
 * EOF
 */
