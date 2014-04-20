#include<iostream>
using namespace std;
int main() {
  int i;
  for (i = 2; i > 0; i--) { 
    //if ( i-1 > 0) { 
      cout<<" INFO :: value of i inside for: " <<i <<std::endl;
      cout <<" INFO :: Copying something " <<endl;
    //}
  }
  cout<<" INFO :: value of i after for: " <<i <<std::endl;
  return 0;
}
