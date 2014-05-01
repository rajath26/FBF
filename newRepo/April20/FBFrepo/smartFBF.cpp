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
//#include "FBF.cpp"
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
//#define SLEEP_TIME 2 
#define SLEEP_TIME 1
#define DEF_NUM_INSERTS 2000
#define DEF_TABLE_SIZE 6250 
#define DEF_NUM_OF_HASH 3
#define DEF_REFRESH_RATE 3 
#define DEF_BATCH_OPS 200
#define DEF_NUM_INVALIDS 2500
#define SIMPLE_FBF 3
#define THRESHOLD_FRACTION 0.8
#define ADD_DEC_RR 1
#define MIN_RR 1
#define ADD_INC_RR 1

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
  t.printStartTime();

  /* 
   * STEP 2: Insert some numbers into the FBF
   */
  for ( i = 0; i < numElements; i++ ) { 

    /* 
     * Check for elapsed time and refresh the FBF
     */
    if ( t.getElapsedTime() >= refreshRate ) { 
      t.printElapsedTime();
      simpleFBF.refresh();
      // Restart the timer
      t.start();
      t.printStartTime();
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
  dynFBF simpleFBF(3, tableSize, numOfHashes);

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
  //simpleFBF.checkSmartFBF_FPR(numberOfInvalids);

  /*
   * STEP 5: Check for FPR using mathematical probability
   */
  simpleFBF.checkEffectiveFPR();

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
  //t.printStartTime();

  /* 
   * STEP 2: Insert some numbers in to the FBF
   */
  loopTime.start();
  for ( i = 0; i < numElements; i++ ) {

    /* 
     * Check for elapsed time and refresh the FBF
     */
    if ( t.getElapsedTime() >= refreshRate ) {
      //t.printElapsedTime();
      dyn_FBF.refresh();
      // Restart the timer after the refresh
      t.start();
      //t.printStartTime();
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
  //dyn_FBF.checkSmartFBF_FPR(numberOfInvalids);

  /*
   * STEP 5: Check for probabilistic FPR
   */
  //t.printElapsedTime();
  dyn_FBF.checkEffectiveFPR();

  cout<<" -----------------------------------------------------------" <<endl <<endl;

}

/***********************************************************************
 * FUNCTION NAME: dynamicResizing
 *
 * This function runs the dynamically resized FBF
 *
 * PARAMETERS:
 *            targetFPR: target false positive rate
 *
 * RETURNS: void
 ***********************************************************************/
void dynamicResizing(double targetFPR) {

  unsigned long long int numElements = 0;
  unsigned long long int tableSize = 12500;
  unsigned int numOfHashes = 3;
  unsigned long refreshRate = 20;
  unsigned long long int batchOps;
  unsigned long long int numberOfInvalids;
  double currentFPR = 0.0;

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
  dynFBF drFBF(SIMPLE_FBF, tableSize, numOfHashes);

  // Start the timer
  t.start();
  cout<<" INFO :: Timer started " <<endl;
  t.printStartTime();

  /*
   * STEP 2: Insert some numbers into the FBF
   */
  for ( i = 0; i < numElements; i++ ) {

	currentFPR = drFBF.checkEffectiveFPR();
	if ( currentFPR >= THRESHOLD_FRACTION * targetFPR ) {
      drFBF.triggerDynamicResizing();
      if ( refreshRate - ADD_DEC_RR >= MIN_RR ) {
        refreshRate -= ADD_DEC_RR;
      }
	}
	else if( currentFPR <= 0.5 * targetFPR ) {
      drFBF.triggerTrimDown();
      refreshRate += ADD_INC_RR;
	}

    /*
     * Check for elapsed time and refresh the FBF
     */
    if ( t.getElapsedTime() >= refreshRate ) {
      t.printElapsedTime();
      drFBF.refresh();
      // Restart the timer
      t.start();
      t.printStartTime();
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
    drFBF.insert(i);

  } // End of for that inserts elements into the FBF

  cout<<" -----------------------------------------------------------" <<endl <<endl;

} // End of smartFBFvsDumbFBFvarNumElements()

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
  unsigned long long int num = 3000;
  unsigned long long int bat = 750;
  unsigned long long int inv = 1500;
  for ( unsigned int i = 3; i < 10; i++ ) {
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
  unsigned long long int num = 6250;
  //unsigned long long int bat = 400;
  unsigned long long int inv = 3125;
  unsigned long long int tableSize = 6250;
  unsigned int numHashes = 3;

  unsigned long refreshRate = 20;
  for ( int i = 0; i < 5; i++ ) {
    refreshRateVsOpsPerSec(num, tableSize, numHashes, refreshRate, 30, inv);
    refreshRate /= 2;
  }
  /*
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 400, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 19, 400, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 18, 400, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 17, 400, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 400, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 400, 25000);
  */

  /* 
   * 1000 Ops per second 
   */
  refreshRate = 20;
  for ( int i = 0; i < 5; i++ ) {
    refreshRateVsOpsPerSec(num, tableSize, numHashes, refreshRate, 300, inv);
    refreshRate /= 2;
  }
  /*
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 4000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 10, 4000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 5, 4000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 3, 4000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 4000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 4000, 25000);
  */

  /* 
   * 5000 Ops per second
   */
  refreshRate = 20;
  for ( int i = 0; i < 5; i++ ) {
    refreshRateVsOpsPerSec(num, tableSize, numHashes, refreshRate, 3000, inv);
    refreshRate /= 2;
  }
  /*
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 26000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 10, 26000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 5, 26000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 3, 26000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 26000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 26000, 25000);
  */

  /* 
   * 10000 Ops per second 
   */
  refreshRate = 20;
  for ( int i = 0; i < 5; i++ ) {
    refreshRateVsOpsPerSec(num, tableSize, numHashes, refreshRate, 6000, inv);
    refreshRate /= 2;
  }
  /*
  refreshRateVsOpsPerSec(50000, 50000, 3, 20, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 10, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 5, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 3, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 2, 50000, 25000);
  refreshRateVsOpsPerSec(50000, 50000, 3, 1, 50000, 25000);
  */
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
  unsigned long long int num = 12500;
  //unsigned long long int bat = 400;
  unsigned long long int inv = 6250;
  unsigned long long int tableSize = 12500;
  unsigned int numHashes = 3;
  unsigned int refreshRate = 3;
  unsigned int bf = 3;

  bf = 3;
  for ( unsigned int counter = 0; counter < 4; counter++) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, 10, inv);
    bf *= 2;
  }

  bf = 3;
  for ( unsigned int counter = 0; counter < 4; counter++) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, 100, inv);
    bf *= 2;
  }  

  bf = 3;
  for ( unsigned int counter = 0; counter < 4; counter++) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, 1000, inv);
    bf *= 2;
  }

  bf = 3;
  for ( unsigned int counter = 0; counter < 4; counter++ ) {
    numberOfBFsVsOpsPerSec(bf, num, tableSize, numHashes, refreshRate, 10000, inv);
    bf *= 2;
  }

}

/******************************************************************************
 * FUNCTION NAME: dynamicResizingStart
 *
 * This function starts the iterative dynamic resizing FBF
 *
 * RETURNS: void
 ******************************************************************************/
void dynamicResizingStart() {
  dynamicResizing(0.01);
}

/*
 * Main function
 */
int main(int argc, char *argv[]) { 

  //varyNumElements();
  //varyBFsize();
  //varyHashes();
  //varyRefreshRate();
  varyConstituentBFNumbers();
  dynamicResizingStart();

  return SUCCESS;

} // End of main()

/* 
 * EOF
 */
