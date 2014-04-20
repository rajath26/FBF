#include <iostream>
#include "bloom_filter.hpp"

int main() { 
  bloom_parameters parameter;
  parameter.compute_optimal_parameters(6250, 3);
  bloom_filter *newBF = new bloom_filter(parameter);

  std::cout<<" Inserting 10 into BF" <<std::endl;
  newBF->insert(10);

  std::cout<<" Check if 10 has been inserted" <<std::endl;
  if ( newBF->contains(10) ) { 
    std::cout<<" YEAH !!! " <<std::endl;
  }
  else {
    std::cout<<" OH NO !!! " <<std::endl;
  }

  return 0;
}




