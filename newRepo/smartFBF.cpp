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
#include "dynFBF.cpp"

/* 
 * Timer class
 */
#include "Timer.cpp"

/*
 * Macros
 */
#define FAILURE -1
#define SUCCESS 0
#define SLEEP_TIME 3 
#define DEF_NUM_INSERTS 2000
#define DEF_TABLE_SIZE 6250 
#define DEF_NUM_OF_HASH 3
#define DEF_REFRESH_RATE 3 
#define DEF_BATCH_OPS 200
#define DEF_NUM_INVALIDS 2500

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
 *            tableSize: constituent BFs size i.e. number of bits 
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
		       unsigned long long int batchOps,
		       unsigned long long int numberOfInvalids) { 

  cout<<" ----------------------------------------------------------- " <<endl;
  cout<<" INFO :: Test Execution Info " <<endl;
  cout<<" INFO :: NUMBER OF ELEMENTS: " <<numElements <<endl;
  // Table size and number of hash functions printed by compute_optimal function
  // in FBF constructor. Dont print here 
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
    if ( t.getElapsedTime() >= refreshRate ) { 
      //cout<<" DEBUG :: Yes I am inside the refresh if condition " <<endl;
      //cout<<" DEBUG :: Calling refresh function " <<endl;
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

  cout<<" -----------------------------------------------------------" <<endl <<endl;
  
} // End of smartFBFvsDumbFBFvarNumElements()

/******************************************************************************
 * FUNCTION NAME: refreshRateVsOpsPerSec
 * 
 * This function maps the false positive rate for a given refresh rate and 
 * also keeps a tab on the operations per second
 * 
 * PARAMETERS: 
 *            numElements: Number of elements to be inserted into the
 *                         FBF
 *            tableSize: constituent BFs size i.e. number of bits 
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
 ******************************************************************************/
void refreshRateVsOpsPerSec(unsigned long long int numElements, 
                            unsigned long long int tableSize,
		            unsigned int numOfHashes,
		            unsigned long refreshRate,
		            unsigned long long int batchOps,
		            unsigned long long int numberOfInvalids) { 

  cout<<" ----------------------------------------------------------- " <<endl;
  cout<<" INFO :: Test Execution Info " <<endl;
  cout<<" INFO :: NUMBER OF ELEMENTS: " <<numElements <<endl;
  // Table size and number of hash functions printed by compute_optimal function
  // in FBF constructor. Dont print here nnnnnn
  cout<<" INFO :: REFRESH RATE: " <<refreshRate <<endl;
  cout<<" INFO :: BATCH OPERATIONS: " <<batchOps <<endl;

  // Timer to refresh the constituent BFs in FBF
  Timer t;
  // Timer to keep a tab on the operations per second
  Timer loopTime;

  unsigned long long int i;

  /* 
   * STEP 1: CREATE THE FBF
   */
  FBF simpleFBF(tableSize, numOfHashes);

  // Start the timer
  t.start();
  cout<<" INFO :: Timer started " <<endl;

  /* 
   * STEP 2: Insert some numbers into the FBF
   */
  loopTime.start();
  for ( i = 0; i < numElements; i++ ) {
    
    /* 
     * Check for elapsed time and refresh the FBF
     */
    if ( t.getElapsedTime() >= refreshRate ) {
      simpleFBF.refresh();
      // Restart the timer after the refresh
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
   * STEP 3: Measure the operations per second done
   */
  double elapsedLoopTime = loopTime.getElapsedTime();
  cout<<" INFO :: Time elapsed in for loop: " <<elapsedLoopTime <<endl;
  cout<<" INFO :: Rate of insertion: " <<(double)numElements/elapsedLoopTime <<"per second" <<endl;

  /* 
   * STEP 4: Check for FPR using smart rules
   */
  simpleFBF.checkSmartFBF_FPR(numberOfInvalids);

  cout<<" -----------------------------------------------------------" <<endl <<endl;

}

/******************************************************************************
 * FUNCTION NAME: numberOfBFsVsOpsPerSec
 * 
 * This function maps the false positive rate for a given number of constituent
 * number of BFs in the FBF and also keeps a tab on the operations per second
 * 
 * PARAMETERS: 
 *            numberOfBFs: Number of constituent BFs in the FBF
 *            numElements: Number of elements to be inserted into the
 *                         FBF
 *            tableSize: constituent BFs size i.e. number of bits 
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
 ******************************************************************************/
void numberOfBFsVsOpsPerSec(unsigned long numberOfBFs,
                            unsigned long long int numElements, 
                            unsigned long long int tableSize,
		            unsigned int numOfHashes,
		            unsigned long refreshRate,
		            unsigned long long int batchOps,
		            unsigned long long int numberOfInvalids) { 

  cout<<" ----------------------------------------------------------- " <<endl;
  cout<<" INFO :: Test Execution Info " <<endl;
  cout<<" INFO :: NUMBER OF ELEMENTS: " <<numElements <<endl;
  // Table size and number of hash functions printed by compute_optimal function
  // in FBF constructor. Number of BFs in FBF printed in constructor.
  // Dont print here 
  cout<<" INFO :: REFRESH RATE: " <<refreshRate <<endl;
  cout<<" INFO :: BATCH OPERATIONS: " <<batchOps <<endl;

  // Timer to refresh the constituent BFs in FBF
  Timer t;
  // Timer to keep a tab on the operations per second
  Timer loopTime;

  unsigned long long int i;

  /*
   * STEP 1: CREATE THE DYNAMIC FBF
   */
  dynFBF dyn_FBF(numberOfBFs, tableSize, numOfHashes);

  // Start the timer
  t.start();
  cout<<" INFO :: Timer started " <<endl;

  /* 
   * STEP 2: Insert some numbers in to the FBF
   */
  loopTime.start();
  for ( i = 0; i < numElements; i++ ) {

    /* 
     * Check for elapsed time and refresh the FBF
     */
    if ( t.getElapsedTime() >= refreshRate ) {
      dyn_FBF.refresh();
      // Restart the timer after the refresh
      t.start();
    }

    /*
     * For every batch operations done induce some 
     * sleep time
     */
    if ( 0 == i% batchOps ) { 
      sleep(SLEEP_TIME);
    }

    /* 
     * Insert number into the FBF
     */
    dyn_FBF.insert(i);

  } // End of for that inserts elements into the FBF

  /* 
   * STEP 3: Measure the operations per second done 
   */
  double elapsedLoopTime = loopTime.getElapsedTime();
  cout<<" INFO :: Time elapsed in for loop: " <<elapsedLoopTime <<endl;
  cout<<" INFO :: Rate of insertion: " <<(double)numElements/elapsedLoopTime <<"per second" <<endl;

  /*
   * STEP 4: Check for FPR using smart rules
   */
  dyn_FBF.checkSmartFBF_FPR(numberOfInvalids);

  cout<<" -----------------------------------------------------------" <<endl <<endl;

}

/******************************************************************************
 * FUNCTION NAME: varyNumElements
 * 
 * This function runs the test case where the number of elements are varied 
 * ie increased while the constituent BF size and the number of hash functions
 * are kept constant. The insertion rate is also kept constant. 
 * 
 * RETURNS: void
 ******************************************************************************/
void varyNumElements() {
  smartFBFvsDumbFBF(100, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 25, 500);
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 250, 1000);
  //smartFBFvsDumbFBF(1500, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 375, 750);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  //smartFBFvsDumbFBF(3000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 750, 1500);
  //smartFBFvsDumbFBF(3500, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 875, 1750);
  //smartFBFvsDumbFBF(4000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1000, 2000);
  smartFBFvsDumbFBF(5000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  //smartFBFvsDumbFBF(6000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1500, 3000);
  //smartFBFvsDumbFBF(7000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1750, 3500);
  smartFBFvsDumbFBF(8000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2000, 4000);
  //smartFBFvsDumbFBF(9000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2250, 1500);
  smartFBFvsDumbFBF(10000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2500, 5000);
}

/******************************************************************************
 * FUNCTION NAME: varyBFsize
 * 
 * This function runs the test case where the constituent BF size are varied 
 * ie increased while the number of elements inserted and the number of hash
 * functions are kept constant. The insertion rate is also kept constant. 
 * 
 * RETURNS: void
 ******************************************************************************/
void varyBFsize() {
  smartFBFvsDumbFBF(5000, 1250, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  smartFBFvsDumbFBF(5000, 2500, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  //smartFBFvsDumbFBF(2000, 3750, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  //smartFBFvsDumbFBF(2000, 5000, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  smartFBFvsDumbFBF(5000, 6250, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  //smartFBFvsDumbFBF(2000, 8750, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  //smartFBFvsDumbFBF(2000, 10000, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  //smartFBFvsDumbFBF(2000, 11250, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  smartFBFvsDumbFBF(5000, 12500, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  //smartFBFvsDumbFBF(2000, 18750, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  //smartFBFvsDumbFBF(2000, 37500, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  smartFBFvsDumbFBF(5000, 37500, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  smartFBFvsDumbFBF(5000, 62500, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
}

/******************************************************************************
 * FUNCTION NAME: varyHashes
 * 
 * This function runs the test case where the number of hashes are varied 
 * ie increased while the number of elements inserted and the constituent
 * BF size are kept constant. The insertion rate is also kept constant. 
 * 
 * RETURNS: void
 ******************************************************************************/
void varyHashes() {
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, 3, DEF_REFRESH_RATE, 250, 500);
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, 6, DEF_REFRESH_RATE, 250, 500);
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, 9, DEF_REFRESH_RATE, 250, 500);
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, 12, DEF_REFRESH_RATE, 250, 500);
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, 15, DEF_REFRESH_RATE, 250, 500);
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, 18, DEF_REFRESH_RATE, 250, 500);
}

/******************************************************************************
 * FUNCTION NAME: varyRefreshRate 
 *
 * This function runs the test case where the operations per second is varied 
 * while the rest is kept constant. This is to check how false positive rate 
 * reacts for different refresh rates
 * 
 * RETURNS: void
 ******************************************************************************/
void varyRefreshRate() {
  /*
   * 50 Ops per second
   */
  /*
  refreshRateVsOpsPerSec(10000, 25000, 3, 20, 250, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 10, 250, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 5, 250, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 3, 250, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 2, 250, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 1, 300, 5000);
  */
  /*
   * 100 Ops per second
   */
  /*
  refreshRateVsOpsPerSec(10000, 25000, 3, 20, 500, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 10, 500, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 5, 500, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 3, 500, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 2, 500, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 1, 500, 5000);
  */
  /* 
   * 200 
   */ 
  /*
  refreshRateVsOpsPerSec(10000, 25000, 3, 20, 1000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 10, 1000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 5, 1000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 3, 1000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 2, 1000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 1, 1000, 5000);
  */
  /*
   * 400 
   */
  /*
  refreshRateVsOpsPerSec(10000, 25000, 3, 20, 2000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 10, 2000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 5, 2000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 3, 2000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 2, 2000, 5000);
  refreshRateVsOpsPerSec(10000, 25000, 3, 1, 2000, 5000);
  */
  /*
   * 1000
   */
  /*
  refreshRateVsOpsPerSec(40000, 250000, 3, 20, 5500, 40000);
  refreshRateVsOpsPerSec(40000, 250000, 3, 10, 5500, 40000);
  refreshRateVsOpsPerSec(40000, 250000, 3, 5, 5500, 40000);
  refreshRateVsOpsPerSec(40000, 250000, 3, 3, 5500, 40000);
  refreshRateVsOpsPerSec(40000, 250000, 3, 2, 5500, 40000);
  refreshRateVsOpsPerSec(40000, 250000, 3, 1, 5500, 40000);
  */
  /*
   * 10000
   */
  /*
  refreshRateVsOpsPerSec(200000, 250000, 3, 20, 50000, 100000);
  refreshRateVsOpsPerSec(200000, 250000, 3, 10, 50000, 100000);
  refreshRateVsOpsPerSec(200000, 250000, 3, 5, 50000, 100000);
  refreshRateVsOpsPerSec(200000, 250000, 3, 3, 50000, 100000);
  refreshRateVsOpsPerSec(200000, 250000, 3, 2, 50000, 100000);
  refreshRateVsOpsPerSec(200000, 250000, 3, 1, 50000, 100000);
  */
  /*
   * 100 Ops per second
   */
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 500, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 10, 500, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 5, 500, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 3, 500, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 500, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 500, 25000);

  /* 
   * 1000 Ops per second 
   */
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 5000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 10, 5000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 5, 5000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 3, 5000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 5000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 5000, 25000);

  /* 
   * 5000 Ops per second
   */
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 25000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 10, 25000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 5, 25000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 3, 25000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 25000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 25000, 25000);

  /* 
   * 10000 Ops per second 
   */
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 10, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 5, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 3, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 50000, 25000);
}

/******************************************************************************
 * FUNCTION NAME: varyBFsize
 *
 * This function runs the test case where the operations per second is varied 
 * while the rest is kept constant. This is to check how false positive rate 
 * reacts for different number of constituent BFs in the FBF
 * 
 * RETURNS: void
 ******************************************************************************/
void varyConstituentBFNumbers() { 
  /*
   * 100 Ops per second
   */
  numberOfBFsVsOpsPerSec(3, 1000, 6250, 5, 3, 200, 1000);
  numberOfBFsVsOpsPerSec(5, 1000, 6250, 5, 3, 300, 1000);
  //numberOfBFsVsOpsPerSec(5, 50000, 50000, 3, 3, 500, 25000);
  //numberOfBFsVsOpsPerSec(7, 50000, 50000, 3, 3, 500, 25000);
  //numberOfBFsVsOpsPerSec(9, 50000, 50000, 3, 3, 500, 25000);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 500, 25000);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 500, 25000);

  /* 
   * 1000 Ops per second 
   */
   /*
  numberOfBFsVsOpsPerSec(3, 2500, 6250, 3, 5, 3000, 1250);
  numberOfBFsVsOpsPerSec(6, 2500, 6250, 3, 5, 3000, 1250);
  numberOfBFsVsOpsPerSec(12, 2500, 6250, 3, 5, 3000, 1250);
  numberOfBFsVsOpsPerSec(24, 2500, 6250, 3, 5, 3000, 1250);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 5000, 25000);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 5000, 25000);
  */

  /* 
   * 5000 Ops per second
   */
   /*
  numberOfBFsVsOpsPerSec(3, 2500, 6250, 3, 5, 9000, 1250);
  numberOfBFsVsOpsPerSec(6, 2500, 6250, 3, 5, 9000, 1250);
  numberOfBFsVsOpsPerSec(12, 2500, 6250, 3, 5, 9000, 1250);
  numberOfBFsVsOpsPerSec(24, 2500, 6250, 3, 5, 9000, 1250);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 25000, 25000);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 25000, 25000);
  */

  /* 
   * 10000 Ops per second 
   */
  /*
  numberOfBFsVsOpsPerSec(3, 50000, 50000, 3, 5, 50000, 25000);
  numberOfBFsVsOpsPerSec(6, 50000, 50000, 3, 5, 50000, 25000);
  numberOfBFsVsOpsPerSec(12, 50000, 50000, 3, 5, 50000, 25000);
  numberOfBFsVsOpsPerSec(24, 50000, 50000, 3, 5, 50000, 25000);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 50000, 25000);
  //numberOfBFsVsOpsPerSec(50000, 50000, 3, 5, 50000, 25000);
  */
}

/*
 * Main function
 */
int main(int argc, char *argv[]) { 

  varyNumElements();
  //varyBFsize();
  //varyHashes();
  //varyRefreshRate();
  //varyConstituentBFNumbers();

  return SUCCESS;

} // End of main()

/* 
 * EOF
 */
