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
 * Timer class
 */
#include "Timer.cpp"

/*
 * Macros
 */
#define FAILURE -1
#define SUCCESS 0
#define DEF_TABLE_SIZE 6250
#define DEF_NUM_OF_HASH 3
#define DEF_REFRESH_RATE 5
#define DEF_BATCH_OPS 200
#define DEF_NUM_INVALIDS 5000

using namespace std;

/***********************************************************************
 * FUNCTION NAME: smartFBFvsDumbFBF
 *
 * This function compares the false positive rate (FPR) of the smart FBF 
 * with that of dumb FBF  
 *
 * PARAMETERS: 
 *            numElements: Number of elements to be inserted into the
 *                         FBF
 *            tableSize: constituent BFs size in FBF
 *            numOfHashes: Number of hashes in each constituent BFs in 
 *                         FBF
 *            refreshRate: time in seconds after which the FBF is 
 *                         refreshed
 *            batchOps: number of inserts after which a sleep should be 
 *                      induced to simulate real world scenario
 *            numberOfInvalids: number of invalid membership checks to 
 *                              be made
 *            
 * RETURNS: void
 ***********************************************************************/
void smartFBFvsDumbFBF(unsigned long long int numElements, 
                       unsigned long long int tableSize,
		       unsigned int numOfHashes,
		       unsigned long refreshRate,
		       unsigned long long int batchOps
		       unsigned long long int numberOfInvalids) { 

  cout<<" INFO :: Test Execution Info " <<endl;
  cout<<" INFO :: NUMBER OF ELEMENTS: " <<numElements <<endl;
  // Table size and number of hash functions printed by compute_optimal function
  // in FBF constructor. Dont put it here 
  cout<<" INFO :: REFRESH RATE: " <<refreshRate <<endl;
  cout<<" INFO :: BATCH OPERATIONS: " <<batchOps <<endl;

  // Timer to refresh the constituent BFs in FBF
  Timer t; 

  unsigned long long int i;

  /*
   * STEP 1: Create the FBF 
   */
  FBF simpleFBF(tableSize, numOfHashes);

  // Start the timer
  t.start();
  cout<<" INFO :: Timer started " <<endl;

  /* 
   * STEP 2: Insert some numbers into the FBF
   */
  for ( i = 0; i < numElements; i++ ) { 

    /* 
     * Check for elapsed time and refresh the FBF
     */
    if ( t.elapsedTime() >= refreshRate ) { 
      simpleFBF.refresh();
      // Restart the timer
      t.start();
    }
      
    /*
     * For every batch operations done induce some
     * sleep time
     */
    if ( 0 == i % batchOps ) {
      sleep(SLEEP_TIME);
    }

    /* 
     * Insert number into the FBF
     */
    simpleFBF.insert(i);

  } // End of for that inserts elements into the FBF

  /* 
   * STEP 3: Check for False Positives (FPs) using smart rules 
   */ 
  simpleFBF.checkSmartFBF_FPR(numberOfInvalids);

  /* 
   * STEP 4: Check for False Positives (FPs) using dumb rules
   */ 
  simpleFBF.checkDumbFBF_FPR(numberOfInvalids);

} // End of smartFBFvsDumbFBFvarNumElements()

/*
 * Main function
 */
int main(int argc, char *argv[]) { 

  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, 

} // End of main()

/* 
 * EOF
 */
