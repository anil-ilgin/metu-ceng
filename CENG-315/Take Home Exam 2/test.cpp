// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include "the2.h"

using namespace std;


void randomArray(int*& array, int size, int min, int interval) 
{
  
  for (int i = 0; i < size; i++) 
  {
    array[i] = min + (random() % interval);
  }
}

void measureTime(){

  clock_t begin;
  clock_t end;
  double duration;

  // maybe data generation here

  if ((begin = clock ()) == -1) 
    std::cerr << "clock err" << std::endl; 

  // call your function here instead of the loop
  for (int i = 0; i < 1<<30; i++)
    ;
  //

  if ((end = clock ()) == -1) 
    std::cerr << "clock err" << std::endl;
  duration = ((double)end-begin) / CLOCKS_PER_SEC * 1000000;

  std::cerr << "Times: " << begin <<" "<< end << " " << end-begin << " "<< duration <<" microsecs " << std::endl;
}

void printArr(int*& arr, int len)
{
     for(int i = 0; i < len; i++){
        std::cout << arr[i] <<" ";
    }std::cout<< std::endl;
}





int main()
{
    
    int case12[] = { 4 ,3 ,4 ,4 ,4 ,0 ,4 ,3 ,4, 3, 3, 3, 4, 0 ,4  };
  	int size12 = 15;
  	int *case12arr = new int[size12];
  	  for(int i = 0 ; i < size12; i++){
     case12arr[i] = case12[i];
    }
    std::cout <<"Quick : "<< QuickSort3(case12arr,size12,false) << std::endl;
    printArr(case12arr,size12);
  return 0;
}
