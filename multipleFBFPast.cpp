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
  int future = numBFs - 1;
  int present = numBFs - 2;
  int pastStart = 0;
  int pastEnd = present - 1;

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
      fbf[j] &= newBF;
      t.start();
    } // End of if (t.elapsedTime() >= seconds)

    // For every batchOps number of inserts induce a sleep of 1s
    if ( 0 == i % batchOps ) { 
      //std::cout<< " INFO :: Batch number of ops reached. Sleeping " <<std::endl;
      sleep(SLEEP_TIME);
    }

    // Insert into the bloom filter i.e. into the present 
    // and the future bloom filters 
    fbf[present].insert(i);
    fbf[future].insert(i);

  } // End of for ( int i = 0; i < numElements; i++ )

  double elapTime = s.elapsedTime();
  std::cout<< " INFO :: Time elapses in for loop: " <<elapTime <<std::endl;
  std::cout<< " INFO :: Rate of insertion: " <<numElements/elapTime << "per second" <<std::endl;

  /* 
   * Check for false positives in the smart FBF
   */
  for ( unsigned long long i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ ){ 
 
    if (fbf[future].contains(i)) {
      int flag = 0;
      for ( int j = pastStart ; j <= pastEnd; j++ ) {
	if ( fbf[j].contains(i) ) { 
	  flag = 1;
	  break;
	}
      }
      if ( 1 == flag ) 
	FPCountSFBF++;
    }  

    else if ( fbf[present].contains(i) ) { 
      if ( fbf[future].contains(i) || fbf[pastEnd].contains(i) ) 
	FPCountSFBF++;
    }

    else if ( fbf[future].contains(i) && fbf[present].contains(i) ) 
      FPCountSFBF++;

    else { 
      int found = 0;
      for ( int k = pastStart; k <= pastEnd; k++ ) { 
	if ( fbf[k].contains(i) && (k+1) < numBFs ) { 
	  if ( fbf[k+1].contains(i) ) {
	    FPCountSFBF++;
            found = 1;
	    break;
	  }
	}
        if ( 0 == found && k == pastStart )  { 
	  FPCountSFBF++;
	  break;
	}
      }
    } 

  } // End of for ( unsigned long long i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ )

  // Print results  
  std::cout<< " INFO :: The number of FPs in a Smart FBF : " <<FPCountSFBF <<std::endl;
  std::cout<< " INFO :: The FPR (False Positive Rate) of the SMART FBF is : " <<(double)FPCountSFBF/(FP_CHECK_MULTIPLIER*numElements) <<std::endl;
  
  return 0;

} // End of main()
