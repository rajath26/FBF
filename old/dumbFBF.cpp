#include<iostream>
#include<string>

#include "bloom_filter.hpp"

#define FAILURE -1
#define SUCCESS 0

// Class for the FBF 
/////////////////////////////
// The forgetful Bloom Filter is composed 
// of three bloom filters namely: 
// 1) Past BF
// 2) Present BF
// 3) Future BF
////////////////////////////
class FBF{

public:

  // Constituent BF
  bloom_filter pastBF;
  bloom_filter presentBF;
  bloom_filter futureBF;

  FBF(bloom_parameters parameters) {
    pastBF(parameters);
    presentBF(parameters);
    futureBF(parameters);
  }

  // Function to instantiate the BFs
  void instantiateFBF() {
    pastBF(parameters);
    presentBF(parameters);
    futureBF(parameters);
  } // End of instantiateFBF()

  // Function to insert elements into teh BF
  /////////////////////////
  // When an insert is done it is done both into 
  // the present as well as the future BF
  // Numbers are inserted from 0 to n
  ////////////////////////
  void insertFBF(int n) {
    int i;
    for ( i = 0; i < n; i++ ) { 
      presentBF.insert(i);
      futureBF.insert(i);
    }
  } // End of insertFBF()

  // Check for False Positives
  //////////////////////////////////////////
  // The dumb FBF determines that it is a FP
  // if the number not inserted is present in
  // one of the constituent BFs
  //////////////////////////////////////////
  int checkFPForDFBF(int start, int end) {
    int i;
    int FPCount = 0;
    for ( i = start; i < end; i++ ) {
      if ( pastBF.contains(i) || presentBF.contains(i) || futureBF.contains(i) ) { 
	std::cout<<"FOUND FP"<<std::endl;
	FPCount++;
      }
    }
    return FPCount;
  } // End of checkFPForDFBF()

  /////////////////////////////////////////
  // The smart FBF uses the following rules. It is
  // a false positive if number not inserted is:
  // 1) contained in Past BF AND Future BF
  // 2) contained in Present BF AND (not contained in past BF OR future BF)
  /////////////////////////////////////////
  int checkFPForFBF(int start, int end) {
    int i;
    int FPCount = 0;
    for ( i = start; i < end; i++ ) {
      if ( (pastBF.contains(i) && futureBF.contains(i)) || (presentBF.contains(i) && (!pastBF.contains(i) || !futureBF.contains(i))) ) {
        std::cout<<"FOUND FP"<<std::endl;
	FPCount++;
      }
    }
    return FPCount;
  } // End of checkFPForFBF()

}; // End of class DumbFBF

int main() {

  bloom_parameters parameters;

  // Set how many elements we expect to insert
  parameters.projected_element_count = 1000;

  // Maximum tolerable false positive probability ? (0,1)
  parameters.false_positive_probability = 0.0001; // 1 in 10000

  // Simple randomizer 
  parameters.random_seed = 0xA5A5A5A5;

  if (!parameters) {
    std::cout << "ERROR :: Invalid set of bloom filter parameters" << std::endl;
    return FAILURE;
  }

  parameters.compute_optimal_parameters();
  parameters.compute_optimal_parameters();

  // Create two objects of the DFBF
  // One for the dumb FBF 
  // One for the usual FBF
  FBF dfbfObj(parameters); // Dumb forgetful bloom filter
  FBF fbfObj(parameters); // Smart forgetful bloom filter

  // False positive counts
  int dfbfFPCount = 0,
    fbfFPCount = 0;

  // Instantiate the FBFs
  //dfbfObj.instantiateFBF();
  //fbfObj.instantiateFBF();

  // Insert numbers into the FBFs
  dfbfObj.insertFBF(1000);
  fbfObj.insertFBF(1000);

  // Check for false positives
  dfbfFPCount = dfbfObj.checkFPForDFBF(1000, 100000);
  fbfFPCount = fbfObj.checkFPForFBF(1000, 100000);

  cout<<"FPR of DUMB FBF: " <<dfbfFPCount;
  cout<<"FPR of SMART FBF: " <<fbfFPCount;

  return SUCCESS;
  
} // End of main()


