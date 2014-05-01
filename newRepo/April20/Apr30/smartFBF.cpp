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
#define SLEEP_TIME 2 
#define DEF_NUM_INSERTS 2000
#define DEF_TABLE_SIZE 6250 
#define DEF_NUM_OF_HASH 3
#define DEF_REFRESH_RATE 3 
#define DEF_BATCH_OPS 200
#define DEF_NUM_INVALIDS 2500
#define SIMPLE_FBF 3

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
  dynFBF simpleFBF(4, tableSize, numOfHashes);

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

  /*
   * STEP 5: Check the False Positives (FPs) using mathematical formula
   */
  simpleFBF.checkEffectiveFPR();

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
 * FUNCTION NAME: FPRComparison
 *
 * This function compares the FPR calculated emperically/mathematically
 * versus the FPR calculated practically
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
void FPRComparison(unsigned long numberOfBFs,
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

	  /*
	   *  STEP 5: Check effective FPR
	   */
	  dyn_FBF.checkEffectiveFPR();

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
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  smartFBFvsDumbFBF(5000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  smartFBFvsDumbFBF(8000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2000, 4000);
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
  unsigned long long int numE = 5000;
  unsigned long long int batO = 1250;
  unsigned long long int invT = 2500;
  smartFBFvsDumbFBF(numE, 3750, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, batO, invT);
  smartFBFvsDumbFBF(numE, 6250, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, batO, invT);
  smartFBFvsDumbFBF(numE, 8750, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, batO, invT);
  smartFBFvsDumbFBF(numE, 12500, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, batO, invT);
  smartFBFvsDumbFBF(numE, 15000, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, batO, invT);
  smartFBFvsDumbFBF(numE, 25000, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, batO, invT);
  smartFBFvsDumbFBF(numE, 37500, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, batO, invT);
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
  unsigned long long int num = 2000;
  unsigned long long int bat = 500;
  unsigned long long int inv = 2000;
  for ( unsigned int i = 3; i < 18; i++ ) {
    smartFBFvsDumbFBF(num, DEF_TABLE_SIZE, i, DEF_REFRESH_RATE, bat, inv);
  }
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
   * 100 ops per sec
   */
  /*
  for ( unsigned i = 0; i < 5; i++ ) { 
    numberOfBFsVsOpsPerSec(bf, num, 6250, 5, 3, bat, inv);
    bf += 3;
  }
  */

  /* 
   * Initial Tests
   */
  /*
  numberOfBFsVsOpsPerSec(3, 19250, 6250, 5, 3, 2000, 3125);
  //numberOfBFsVsOpsPerSec(4, 6250, 6250, 5, 3, 2000, 3125);
  //numberOfBFsVsOpsPerSec(5, 6250, 6250, 5, 3, 2000, 3125);
  numberOfBFsVsOpsPerSec(6, 19250, 6250, 5, 3, 2000, 3125);
  //numberOfBFsVsOpsPerSec(7, 6250, 6250, 5, 3, 2000, 3125);
  //numberOfBFsVsOpsPerSec(8, 6250, 6250, 5, 3, 2000, 3125);
  //numberOfBFsVsOpsPerSec(9, 6250, 6250, 5, 3, 2000, 3125);
  //numberOfBFsVsOpsPerSec(10, 6250, 6250, 5, 3, 2000, 3125);
  //numberOfBFsVsOpsPerSec(11, 6250, 6250, 5, 3, 2000, 3125);
  numberOfBFsVsOpsPerSec(12, 19250, 6250, 5, 3, 2000, 3125);
  numberOfBFsVsOpsPerSec(24, 19250, 6250, 5, 3, 2000, 3125);
  */

  /*
   * 100 ops per sec
   */
  unsigned long long int num = 50000;
  unsigned long long int bat = 200;
  unsigned long long int inv = 25000;
  unsigned long long int tableSize = 25000;
  unsigned int numHashes = 5;
  unsigned int refreshRate = 5;
  unsigned int bf = 3;

  /*
  for ( unsigned int counter = 0; counter < 4; counter++) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, bat, inv);
    bf *= 2;
  } 
  */ 

  bf = 3;
  for ( unsigned int counter = 0; counter < 4; counter++) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, 2000, inv);
    refreshRate -= 1;
    bf += 1;
  }  

  bf = 3;
  for ( unsigned int counter = 0; counter < 4; counter++) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, 10000, inv);
    bf += 1;
  }  

  bf = 3;
  for ( unsigned int counter = 0; counter < 4; counter++) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, 20000, inv);
    bf += 1;
  }  
  
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

/******************************************************************************
 * FUNCTION NAME: effectiveFPRvsActualFPR
 *
 * This function runs the test case where the effective FPR of the FBF
 * calculated mathematically is compared with the FPR calculated
 * practically
 *
 * RETURNS: void
 ******************************************************************************/
void effectiveFPRvsActualFPR() {
    FPRComparison(3, 1000, 6250, 3, 3, 200, 500);
    FPRComparison(3, 2000, 6250, 3, 3, 200, 1000);
    FPRComparison(3, 3000, 6250, 3, 3, 200, 1500);
    FPRComparison(3, 4000, 6250, 3, 3, 200, 2000);
    FPRComparison(3, 5000, 6250, 3, 3, 200, 2500);
	FPRComparison(3, 6250, 6250, 3, 3, 200, 3125);
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
  //effectiveFPRvsActualFPR();

  return SUCCESS;

} // End of main()

/* 
 * EOF
 */
