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
 ** CLASS NAME: Timer 
 **
 ** NOTE: This class maintains the timer
 *******************************************************************/

class Timer {

private: 
  struct timeval startTime;
  double startSeconds;

public:
  void start() { 
    gettimeofday(&startTime, NULL);
    startSeconds = startTime.tv_sec + (startTime.tv_usec/1000000.0);
  }

  double getElapsedTime() {
    struct timeval now;
    gettimeofday(&now, NULL);
    double nowSeconds = now.tv_sec + (now.tv_usec/1000000.0);
    return (nowSeconds - startSeconds);
  }

}; // End of Timer class
