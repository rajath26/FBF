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
   * The vector can accommodate multiple past BFs as well
   */
  bloom_filter dyn_fbf[DEF_NUM_OF_BFS];

  /* 
   * New BF to create a new future BF 
   * after each refresh time
   */
  bloom_filter newBF;

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

    bloom_filter baseBF(parameters);
    cout<<" INFO :: NUMBER OF CONSTITUENT BFs initialized in the FBF: " <<numberBFs <<endl;

    for ( unsigned int counter = 0; counter < numberBFs; counter++ ) { 
      dyn_fbf[counter] = baseBF;
      dyn_fbf[counter].clear();
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

    unsigned int j;
    for ( j = (numberOfBFs - 1); j > 0; j-- ) {
      dyn_fbf[j].clear();
      dyn_fbf[j] = dyn_fbf[j - 1];
    }

    dyn_fbf[j].clear();
    dyn_fbf[j] = newBF;

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
    long long int i = -1;
    unsigned int j = 0;
    int found = 0;

    while ( counter != numberOfInvalids ) { 
      /*
      if ( (dyn_fbf[dfuture].contains(i) && dyn_fbf[dpresent].contains(i)) ) {
	      smartFP++;
      }
      else if ( (dyn_fbf[dpresent].contains(i) && dyn_fbf[pastStart].contains(i)) ) {
	      smartFP++;
      }
      else if ( pastStart < pastEnd ) { 
        //std::cout<<" INFO :: pastStart at this stage is : " <<pastStart <<std::endl;
        //std::cout<<" INFO :: pastEnd at this stage is : " <<pastEnd <<std::endl;
        j = pastStart;
        while ( j <= (pastEnd - 1) ) { 
            if ( (dyn_fbf[j].contains(i) && dyn_fbf[j+1].contains(i)) ) { 
              smartFP++;
              goto next;
            }
            j++;
        }
      }
      else if ( (dyn_fbf[pastEnd].contains(i)) ) {
        smartFP++;
      } 
      */
      /*
      else { 
        for ( j = pastStart; j < pastEnd; j++ ) {
          if ( (dyn_fbf[j].contains(i) && dyn_fbf[j + 1].contains(i)) ) {
            smartFP++;
            break;
          }
        }
      }
      */
      /*
      next: 
        i--;
        counter++;
      */

      /*
      std::cout<<" INFO :: PRESENT Value is : " <<dpresent <<std::endl;
      std::cout<<" INFO :: PAST END Value is : " <<pastEnd <<std::endl;

      for ( j = dpresent; j <= pastEnd; j++ )  {
        if ( (dyn_fbf[j-1].contains(i) && dyn_fbf[j].contains(i)) ) { 
          smartFP++;
          found = 1;
          break;
        }
      }

      if ( 0 == found ) { 
        if ( dyn_fbf[pastEnd].contains(i) ) { 
          smartFP++;
        }
      }

      i--;
      counter++;
      */

      if ( (dyn_fbf[dfuture].contains(i) && dyn_fbf[dpresent].contains(i)) ) {
        smartFP++;
      }
      else if ( (dyn_fbf[dpresent].contains(i) && dyn_fbf[pastStart].contains(i)) ) {
        smartFP++;
      }
      else if ( pastEnd > pastStart ) {
        /*
        j = pastStart;
        while ( j <= (pastEnd - 1) ) {
          if ( (dyn_fbf[j].contains(i) && dyn_fbf[j+1].contains(i)) ) {
            smartFP++;
            found = 1;
          }
          j++;
          if ( 1 == found ) {
            goto next;
          }
        }
        */
        for ( j = pastStart; j <= (pastEnd - 1); j++ ) {
          if ( (dyn_fbf[j].contains(i) && dyn_fbf[j+1].contains(i)) ) {
            smartFP++;
            found = 1;
          }
          if ( 1 == found ) {
            goto next;
          }
        }
      }
      else if ( dyn_fbf[pastEnd].contains(i) ) {
        smartFP++;
      }

      next:
        i--;
        counter++;

    }

    smartFPR = (double) smartFP/numberOfInvalids;

    cout<<" RESULT :: SMART FP = " <<smartFP <<endl;
    cout<<" RESULT :: SMART FPR = " <<smartFPR <<endl;
  }

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
  void checkDumbFBF_FPR(unsigned long long int numberOfInvalids) {
	unsigned long long int dumbFP = 0;
	double dumbFPR = 0.0;
	unsigned int counter = 0;
	long long int i = -1;
	//int found = 0;

	while ( counter != numberOfInvalids ) {
      for ( unsigned int j = dfuture; j <= pastEnd; j++ ) {
        if ( dyn_fbf[j].contains(i) ) {
          dumbFP++;
          //found = 1;
          break;
        }
        /*if ( 1 == found ) {
          goto next;
        }*/
      }
      //next:
        i--;
        counter++;
    }

	dumbFPR = (double) dumbFP/numberOfInvalids;

	cout<<" RESULT :: DUMB FP = " <<dumbFP <<endl;
	cout<<" RESULT :: DUMB FPR = " <<dumbFPR <<endl;
  }

  /************************************************************
   * FUNCTION NAME: checkEffectiveFPR
   *
   * This function checks the effective FPR of the FBF
   *
   * RETURNS: void
   ***********************************************************/
  void checkEffectiveFPR() {
    unsigned int counter = 0;
    unsigned int temp = 0;
    double effectiveFPR = 0.0;

    cout<<endl<<" INFO :: Individual FPP here: " <<endl;
    cout<<" INFO :: Future BF FPP: " <<dyn_fbf[dfuture].effective_fpp() <<endl;
    for ( unsigned int i = present; i <= pastEnd; i++ ) {
      cout<<" INFO :: " <<i <<"BF FPP: " <<dyn_fbf[i].effective_modified_fpp() <<endl;
    }

    effectiveFPR = dyn_fbf[dfuture].effective_fpp() * dyn_fbf[dpresent].effective_modified_fpp();
    for ( counter = dpresent; counter <= (pastEnd - 1); counter++ ) {
      temp = counter + 1;
      effectiveFPR += dyn_fbf[counter].effective_modified_fpp() * dyn_fbf[temp].effective_modified_fpp();
    }

    effectiveFPR += dyn_fbf[pastEnd].effective_modified_fpp();

    cout<<" RESULT :: The effective FPR of the FBF is: " <<effectiveFPR <<endl;
  }

}; // End of dynFBF class

/* 
 * EOF
 */
