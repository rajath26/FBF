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
#define SLEEP_TIME 2
#define DEF_NUM_INSERTS 1000
#define DEF_TABLE_SIZE 12500 
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
 * FUNCTION NAME: varyNumElements
 * 
 * This function runs the test case where the number of elements are varied 
 * ie increased while the constituent BF size and the number of hash functions
 * are kept constant. The insertion rate is also kept constant. 
 * 
 * RETURNS: void
 ******************************************************************************/
void varyNumElements() {
  smartFBFvsDumbFBF(500, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 125, 250);
  smartFBFvsDumbFBF(1000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 250, 500);
  smartFBFvsDumbFBF(1500, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 375, 750);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 1000);
  smartFBFvsDumbFBF(3000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 750, 1500);
  smartFBFvsDumbFBF(3500, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 875, 1750);
  smartFBFvsDumbFBF(4000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1000, 2000);
  smartFBFvsDumbFBF(5000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 2500);
  smartFBFvsDumbFBF(6000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1500, 3000);
  smartFBFvsDumbFBF(7000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1750, 3500);
  smartFBFvsDumbFBF(8000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2000, 4000);
  smartFBFvsDumbFBF(9000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2250, 1500);
  smartFBFvsDumbFBF(10000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2500, 5000);
}

/******************************************************************************
 * FUNCTION NAME: varyBFsize
 * 
 * This function runs the test case where the the constituent BF size are varied 
 * ie increased while the number of elements inserted and the number of hash
 * functions are kept constant. The insertion rate is also kept constant. 
 * 
 * RETURNS: void
 ******************************************************************************/
void varyBFsize() {
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 125, 500);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 250, 1000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 375, 1500);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 500, 2000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 750, 3000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 875, 3500);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1000, 4000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1250, 5000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1500, 6000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 1750, 7000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2000, 8000);
  smartFBFvsDumbFBF(2000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2250, 9000);
  smartFBFvsDumbFBF(20000, DEF_TABLE_SIZE, DEF_NUM_OF_HASH, DEF_REFRESH_RATE, 2500, 10000);
}

/*
 * Main function
 */
int main(int argc, char *argv[]) { 

  varyNumElements();
  //varyBFsize();

  return SUCCESS;

} // End of main()

/* 
 * EOF
 */
