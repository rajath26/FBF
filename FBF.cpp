
#include<iostream>
#include<time.h>

#include "bloom_filter.hpp"

#define FAILURE -1
#define SUCCESS 0
#define NUM_ARGS 3
#define FP_CHECK_MULTIPLIER 5

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

};

int main(int argc, char *argv[]) { 

  if ( argc != NUM_ARGS ) {
    std::cout<< "ERROR :: Invalid number of arguments" <<std::endl;
    std::cout<< "USAGE :: " <<argv[0] <<" <refresh time> <num_of_elements>" <<std::endl;
    return FAILURE;
  }

  unsigned long seconds = (long) atoi(argv[1]);
  unsigned long numElements = (long) atoi(argv[2]);

  int FPCountDFBF = 0;
  int FPCountSFBF = 0;

  Timer t;

  bloom_parameters parameters;

  // Set how many elements we expect to insert
  parameters.projected_element_count = 1000;

  // Maximum tolerable false positive probability ? (0,1)
  parameters.false_positive_probability = 0.0001;

  // Simple randomizer 
  parameters.random_seed = 0xA5A5A5A5;

  if (!parameters) {
    std::cout << "ERROR :: Invalid set of bloom filter parameters" << std::endl;
    return FAILURE;
  }

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
  bloom_filter newBF(parameters);

  // Start the timer
  t.start();

  std::cout<<"Timer started..." <<std::endl;

  // Insert some numbers into the bloom filter 
  /////////////////////////
  // When an insert is done it is done both into 
  // the present as well as the future BF
  // Numbers are inserted from 0 to n
  ////////////////////////
  for ( int i = 0; i < numElements; i++ ) {
    if (t.elapsedTime() >= seconds) {
      std::cout<<" INFO :: Timeout reached. Copying BFs " <<std::endl;
      // copy BFs
      pastBF = presentBF;
      presentBF = futureBF;
      futureBF &= newBF;
      t.start();
    }
    presentBF.insert(i);
    futureBF.insert(i);
  }

  for ( int i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ ) {
    if ( (pastBF.contains(i) && futureBF.contains(i)) || (presentBF.contains(i) && (pastBF.contains(i) || futureBF.contains(i))) ) {
      //std::cout<<"FOUND FP"<<std::endl;
      FPCountSFBF++;
    }
  }

  std::cout<<"The number of FPs in a Smart FBF : " <<FPCountSFBF <<std::endl;

  for ( int i = numElements; i < FP_CHECK_MULTIPLIER*numElements; i++ ) {
    if ( pastBF.contains(i) || presentBF.contains(i) || futureBF.contains(i) ) {
        //std::cout<<"FOUND FP"<<std::endl;
        FPCountDFBF++;
    }
  }

  std::cout<<"The number of FPs in a Dumb FBF : " <<FPCountDFBF <<std::endl;

} // End of main()
