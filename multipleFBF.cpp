/*
 * Header files
 */
#include<iostream>
#include<time.h>
#include<unistd.h>
#include<sys/time.h>

/*
 * Bloom Filter library
 */ 
#include "bloom_filter.hpp"

/* 
 * Macros
 */
#define FAILURE -1
#define SUCCESS 0
#define NUM_ARGS 5
#define FP_CHECK_MULTIPLIER 5
#define SLEEP_TIME 2
#define NUM_OF_BF 10
#define PRESENT 1
#define PAST 0

/*
 * Timer Class
 */
class Timer {

private:
  struct timeval startTime;
  double startSeconds;

public:
  void start() {
    gettimeofday(&startTime, NULL);
    startSeconds = startTime.tv_sec + (startTime.tv_usec/1000000.0);
  }

  double elapsedTime() {
    struct timeval now;
    gettimeofday(&now, NULL);
    double nowSeconds = now.tv_sec + (now.tv_usec/1000000.0);
    return nowSeconds - startSeconds;
  }

}; // End of class Timer

/* 
 * Main function
 */
int main(int argc, char *argv[]) { 

  if ( argc != NUM_ARGS ) {
    std::cout<< "ERROR :: Invalid number of arguments" <<std::endl;
    std::cout<< "USAGE :: " <<argv[0] <<" <refresh time> <num_of_elements> <batch_ops> <num_of_bloom_filters>" <<std::endl;
    return FAILURE;
  }

  unsigned long seconds = (unsigned long) atoi(argv[1]);
  unsigned long long numElements = (unsigned long long) atoi(argv[2]);
  unsigned long long batchOps = (unsigned long long) atoi(argv[3]);
  unsigned int numBFs = (unsigned int) atoi(argv[4]);

  std::cout<<" INFO :: PROGRAM RUN PARAMETERS: " <<std::endl;
  std::cout<<" INFO :: REFRESH TIME: " <<seconds <<std::endl;
  std::cout<<" INFO :: NUMBER OF ELEMENTS: " <<numElements <<std::endl;
  std::cout<<" INFO :: BATCH OPERATIONS: " <<batchOps <<std::endl;

  unsigned long long FPCountDFBF = 0;
  unsigned long long FPCountSFBF = 0;

  double DFBFfalsePositiveRate = 0.0;
  double SFBFfalsePositiveRate = 0.0;

  Timer t;
  Timer s;

  int j;

  // Bloom filter parameters 
  // Common ground for both the BFs
  bloom_parameters parameters;

  // Set how many elements we expect to insert
  parameters.projected_element_count = numElements/10;

  // Maximum tolerable false positive probability ? (0,1)
  parameters.false_positive_probability = 0.0001;

  // Simple randomizer 
  parameters.random_seed = 0xA5A5A5A5;

  if (!parameters) {
    std::cout << "ERROR :: Invalid set of bloom filter parameters" << std::endl;
    return FAILURE;
  }

  // Determine optimal parameters based on the above settings
  parameters.compute_optimal_parameters();

  /////////////////////////////
  // The forgetful Bloom Filter is composed 
  // of three bloom filters namely: 
  // 1) Past BF
  // 2) Present BF
  // 3) Future BF
  ////////////////////////////

  bloom_filter baseBF(parameters);

  bloom_filter fbf[NUM_OF_BF];

  //create an array of bloom filters. Initially always 10
  for ( int counter = 0; counter < NUM_OF_BF; counter++ ) { 
    fbf[counter] = baseBF;
  }

  // Empty BF to do an intersection with the future BF 
  // and the resultant future BF becomes empty
  bloom_filter newBF(parameters);

  // Start the timer
  t.start();

  std::cout<<"Timer started..." <<std::endl;

  /*
   * Insert some numbers into the bloom filter 
   */
  /////////////////////////
  // When an insert is done it is done both into 
  // the present as well as the future BF
  // Numbers are inserted from 0 to n
  ////////////////////////
  s.start();
  for ( unsigned long long i = 0; i < numElements; i++ ) { 
    if (t.elapsedTime() >= seconds) {
      std::cout<<" INFO :: Timeout reached while inserting. Copying BFs " <<std::endl;
      // copy BFs
      for(j = 0; j < numBFs-1; j++) { 
       if (j+1 < numBFs) {
       	fbf[j] = fbf[j+1];
       }
      }
      fbf[numBFs - 1] &= newBF;
      t.start();
    } // End of if (t.elapsedTime() >= seconds)

    // For every batchOps number of inserts induce a sleep of 1s
    if ( 0 == i % batchOps ) { 
      //std::cout<< " INFO :: Batch number of ops reached. Sleeping " <<std::endl;
      sleep(SLEEP_TIME);
    }

    // Insert into the bloom filter i.e. into the present 
    // and the future bloom filters 
    for ( int k = PRESENT; k < numBFs; k++ ) { 
      fbf[k].insert(i);
    }

  } // End of for ( int i = 0; i < numElements; i++ )

  double elapTime = s.elapsedTime();
  std::cout<< " INFO :: Time elapses in for loop: " <<elapTime <<std::endl;
  std::cout<< " INFO :: Rate of insertion: " <<numElements/elapTime << "per second" <<std::endl;

  /* 
   * Check for false positives in the smart FBF
   */
  for ( unsigned long long i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ ){ 

    if ( fbf[PAST].contains(i) && fbf[numBFs - 1].contains(i) ) { 
      FPCountSFBF++;
    }

    int count = 0;
    for ( int j = 1; j <= (numBFs - 2); j++ ) {
      if ( fbf[numBFs - j].contains(i) ) { 
        count = 0;
	for ( int k = (numBFs - j - 1); k >= PRESENT; k-- ) {
	  if ( fbf[k].contains(i) )
	    count++;
	}
        if ( count == (numBFs - j - 1) ) 
	  FPCountSFBF++;
      }
    } 

    if ( fbf[PRESENT].contains(i) ) {
      if ( fbf[PAST].contains(i) ) 
	FPCountSFBF++;
      else {
	for ( int r = PRESENT + 1; r <= (numBFs - 1); r++ ) { 
	  if ( fbf[r].contains(i) ) { 
	    FPCountSFBF++;
	    break;
	  }
	} 
      }
    }

    int flagContains = 0;
    if ( fbf[PAST].contains(i) ) {
      for ( int s = PRESENT; s <= (numBFs - 1); s++ ) {
	if ( fbf[s].contains(i) ) { 
	  flagContains = 1;
          break;
	}
      }
      if ( 0 == flagContains ) 
	FPCountSFBF++;
    }

  } // End of for ( unsigned long long i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ )

  // Print results  
  std::cout<< " INFO :: The number of FPs in a Smart FBF : " <<FPCountSFBF <<std::endl;
  std::cout<< " INFO :: The FPR (False Positive Rate) of the SMART FBF is : " <<(double)FPCountSFBF/(FP_CHECK_MULTIPLIER*numElements) <<std::endl;
  
  return 0;

} // End of main()
