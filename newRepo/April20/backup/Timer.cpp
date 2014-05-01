/*
 * Header files
 */
#include<iostream>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>

/* 
 * Timer class
 */
/*******************************************************************
 *******************************************************************
 ** CLASS NAME: Timer 
 **
 ** NOTE: This class maintains the timer
 *******************************************************************
 *******************************************************************/
class Timer {

private: 
  struct timeval startTime;
  double startSeconds;

public:
  /********************************************************
   * FUNCTION NAME: start
   *
   * Start the timer
   *
   * RETURNS: void
   ********************************************************/
  void start() { 
    gettimeofday(&startTime, NULL);
    startSeconds = startTime.tv_sec + (startTime.tv_usec/1000000.0);
  }

  /*******************************************************
   * FUNCTION NAME: printStartTime
   *
   * Print the the start seconds
   *
   * RETURNS: void
   *******************************************************/
  void printStartTime() {
	  std::cout<<" INFO :: Start seconds: " <<startSeconds <<endl;
  }

  /*******************************************************
   * FUNCTION NAME: printElapsedTime
   *
   * Print the seconds elapsed since start time
   *
   * RETURNS: void
   *******************************************************/
  void printElapsedTime() {
	struct timeval now;
	gettimeofday(&now, NULL);
	double nowSeconds = now.tv_sec + (now.tv_usec/1000000.0);
	std::cout<<" INFO :: Elapsed seconds: " <<(nowSeconds - startSeconds) <<endl;
  }

  /*******************************************************
   * FUNCTION NAME: getElapsedTime
   * 
   * Get the elapsed time
   * 
   * RETURNS: (double) Elapsed Time
   *******************************************************/
  double getElapsedTime() {
    struct timeval now;
    gettimeofday(&now, NULL);
    double nowSeconds = now.tv_sec + (now.tv_usec/1000000.0);
    return (nowSeconds - startSeconds);
  }

}; // End of Timer class

/* 
 * EOF
 */
