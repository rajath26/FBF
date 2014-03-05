/*
 * Header files
 */
#include<iostream>
#include<time.h>
#include<unistd.h>


/*
 * Bloom Filter library
 */ 
#include "bloom_filter.hpp"


/* 
 * Macros
 */
#define FAILURE -1
#define SUCCESS 0
#define NUM_ARGS 3
#define FP_CHECK_MULTIPLIER 5


/* 
 * Timer class
 */
class Timer {

private: 
  unsigned long startTime;

public:
  void start() {
    startTime = clock();
  }

  unsigned long elapsedTime() {
    return ((unsigned long) clock() - startTime) / CLOCKS_PER_SEC;
  }

  bool isTimeOut(unsigned long seconds) { 
    return seconds >= elapsedTime();
  }

}; // End of class Timer


/* 
 * Main function
 */
int main(int argc, char *argv[]) { 

  if ( argc != NUM_ARGS ) {
    std::cout<< "ERROR :: Invalid number of arguments" <<std::endl;
    std::cout<< "USAGE :: " <<argv[0] <<" <refresh time> <num_of_elements>" <<std::endl;
    return FAILURE;
  }

  unsigned long seconds = (unsigned long) atoi(argv[1]);
  unsigned long long numElements = (unsigned long long) atoi(argv[2]);

  std::cout<<" INFO :: REFRESH TIME: " <<seconds <<std::endl;
  std::cout<<" INFO :: NUMBER OF ELEMENTS: " <<numElements <<std::endl;

  unsigned long long FPCountDFBF = 0;
  unsigned long long FPCountSFBF = 0;

  double DFBFfalsePositiveRate = 0.0;
  double SFBFfalsePositiveRate = 0.0;

  Timer t;

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
  bloom_filter pastBF(parameters);
  bloom_filter presentBF(parameters);
  bloom_filter futureBF(parameters);

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
  for ( unsigned long long i = 0; i < numElements; i++ ) { 
    if (t.elapsedTime() >= seconds) {
      std::cout<<" INFO :: Timeout reached. Copying BFs " <<std::endl;
      // copy BFs
      pastBF = presentBF;
      presentBF = futureBF;
      futureBF &= newBF;
      t.start();
    } // End of if (t.elapsedTime() >= seconds)

    presentBF.insert(i);
    futureBF.insert(i);

    // For every 1000th element induce a sleep(1)
    /*if (0 == i%1000) {
      sleep(1);
    }*/

  } // End of for ( int i = 0; i < numElements; i++ )

  /* 
   * Check for false positives in the smart FBF
   */
  for ( unsigned long long i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ ) {
    if ( (presentBF.contains(i) && (pastBF.contains(i) || futureBF.contains(i))) ) {
      //std::cout<<"FOUND FP"<<std::endl;
      FPCountSFBF++;
    }
    else if ( (pastBF.contains(i) && !presentBF.contains(i) && !futureBF.contains(i)) ) {
      FPCountSFBF++;
    }
    else if ( (futureBF.contains(i) && presentBF.contains(i)) ) {
      FPCountSFBF++;
    }

  }

  // Print results  
  std::cout<<"The number of FPs in a Smart FBF : " <<FPCountSFBF <<std::endl;
  //SFBFfalsePositiveRate = FPCountSFBF/(FP_CHECK_MULTIPLIER*numElements;
  //std::cout<<"The FPR of the SMART FBF is : " <<SFBFfalsePositiveRate <<std::endl;

  /* 
   * Check for false positives in the dumb FBF
   */
  for ( unsigned long long i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ ) {
    if ( pastBF.contains(i) || presentBF.contains(i) || futureBF.contains(i) ) {
        //std::cout<<"FOUND FP"<<std::endl;
        FPCountDFBF++;
    }
  }

  // Print results
  std::cout<<"The number of FPs in a Dumb FBF : " <<FPCountDFBF <<std::endl;
  //DFBFfalsePositiveRate = FPCountDFBF/(FP_CHECK_MULTIPLIER*numElements);
  //std::cout<<"The FPR of the DUMB FBF is : " <<DFBFfalsePositiveRate <<std::endl;

} // End of main()
