#include<iostream>

#include "bloom_filter.hpp" 

int main(int argc, char * argv[]) { 

  int falsePositiveCounts = 0;

  bloom_parameters parameters;

  // How many elements roughly do we expect to insert?
  parameters.projected_element_count = 1000;

  // Maximum tolerable false positive rate (FPR) (0,1)
  parameters.false_positive_probability = 0.0001; // 1 in 10000

  // Simple randomizer 
  parameters.random_seed = 0xA5A5A5A5;

  if ( !parameters ) {
    std::cout<< "ERROR :: Invalid set of bloom filter parameters! " << std::endl;
  } // End of if ( !parameters )

  parameters.compute_optimal_parameters();

  // Instantiate Bloom Filter
  bloom_filter filter(parameters);

  // Insert numbers starting from 1 to 1000 into the Bloom Filter

  for ( std::size_t i = 0; i < 1000; i++ ) { 
    filter.insert(i);
  }

  // Query the Bloom Filter and check for false positives
  for ( std::size_t i = 1000; i < 100000; i++ ) {
    if ( filter.contains(i) ) {
      falsePositiveCounts++; 
      std::cout<<std::endl<<"FOUND A FALSE POSITIVE"<<std::endl;
    }
  }

  std::cout<< "NO OF FALSE POSITIVES: " <<falsePositiveCounts  <<std::endl;

} // End of main()
