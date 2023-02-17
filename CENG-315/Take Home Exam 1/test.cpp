// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include "the1.h"

using namespace std;


void printArr(int*& arr, int len)
{
     for(int i = 0; i < len; i++){
        std::cout << arr[i] <<" ";
    }std::cout<< std::endl;
}

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

   
 // GIVEN TEST CASES SETUP
  int case3[] = { 7, 1, -2, 5, 17, 8, -5, 3, 2, -3, 0};
  int size3 = 11;
  int *case3arr = new int[size3];
  
 int case2[] = { -11, 2, 6, 7, -1, 5 };
 int size2 = 6;
 int *case2arr= new int[size2];
 
 int case1[] = {3,2,7,10};
 int size1= 4;
 int *case1arr = new int[size1];
 
 for(int i = 0 ; i < size3; i++){
     case3arr[i] = case3[i];
 }
 for(int i = 0 ; i < size2; i++){
     case2arr[i] = case2[i];
 }
 for(int i = 0 ; i < size1; i++){
     case1arr[i] = case1[i];
 }
// GIVEN TEST CASES SETUP


  
    // MERGE SORT TEST CASES
    std::cout <<"Merge 1:"<< MergeSort(case1arr,size1,false) << std::endl;
    printArr(case1arr,size1);
    std::cout <<"Merge 2:"<< MergeSort(case2arr,size2,true) << std::endl;
    printArr(case2arr,size2);
    std::cout <<"Merge 3:"<< MergeSort(case3arr,size3,true) << std::endl;
    printArr(case3arr,size3);
    // MERGE SORT TEST CASES
    
    // RESET ARRAYS FOR REUSE
     for(int i = 0 ; i < size3; i++){
         case3arr[i] = case3[i];
     }
     for(int i = 0 ; i < size2; i++){
         case2arr[i] = case2[i];
     }
     for(int i = 0 ; i < size1; i++){
         case1arr[i] = case1[i];
     }
    // RESET ARRAYS FOR REUSE
    
    // FUNKY SORT TEST CASES
    std::cout <<"Count 1:"<< FunkySort(case1arr,size1,false) << std::endl;
    printArr(case1arr,size1);
    std::cout <<"Count 2:"<< FunkySort(case2arr,size2,true) << std::endl;
    printArr(case2arr,size2);
    std::cout <<"Count 3:"<< FunkySort(case3arr,size3,true) << std::endl;
    printArr(case3arr,size3);
    // FUNKY SORT TEST CASES

    std::cout<< std::endl;


// RANDOM LARGE INPUT 
    int size = 100000; //
    int* randArr = new int[size];
    randomArray(randArr, size, -2000,2000);
// RANDOM LARGE INPUT 
    
 // TIMED TEST ZONE
  if ((begin = clock ()) == -1) 
    std::cerr << "clock err" << std::endl; 
    
        // std::cout <<"Count large:  "<< FunkySort(randArr,size,false) << std::endl;
        std::cout <<"Merge large: "<< MergeSort(randArr,size,false) << std::endl;

  if ((end = clock ()) == -1) 
    std::cerr << "clock err" << std::endl;
  duration = ((double)end-begin) / CLOCKS_PER_SEC; //* 1000000;

  std::cerr << "Time: " << duration <<" secs " << std::endl;
  
 // TIMED TEST ZONE
}




int main()
{
    srandom(time(0));
    measureTime();
  return 0;
}
