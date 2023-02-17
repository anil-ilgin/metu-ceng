#include "the2.h"

// do not add extra libraries here


/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    size      : size of arr
    ascending : true for ascending, false for descending 
    
    you can use ceil function from cmath
    
    
*/

int partitioner(int*& arr, int small, int big, int & count, bool ascending) {
    int pivot = arr[big];
    int i = small - 1 ;
    if (ascending) {
        for (int j = small; j < big ; j ++ ) {
        if (arr[j] < pivot ) {
            i++;
            //swap(arr[i], arr[j])
            int k ;
            k = arr[i];
            arr[i] = arr[j];
            arr[j] = k ;
        }
        count++;
    }
        //swap(arr[i+1],big)
        int m;
        m = arr[i+1];
        arr[i+1] = arr[big];
        arr[big] = m ;
    }
    else {
        for (int j = small; j < big ; j ++ ) {
        if (arr[j] > pivot ) {
            i++;
            //swap(arr[i], arr[j])
            int k ;
            k = arr[i];
            arr[i] = arr[j];
            arr[j] = k ;
        }
        count++;
        }
        //swap(arr[i+1],big)
        int m;
        m = arr[i+1];
        arr[i+1] = arr[big];
        arr[big] = m ;
    }
    
    return i+1; 
}

void QuickSorter (int*& arr, int small, int big, bool ascending, int& count) {
    int index;
    if (small < big) {
        index = partitioner(arr, small, big, count, ascending);
        QuickSorter(arr, small, index - 1, ascending, count);
        QuickSorter(arr, index + 1, big, ascending, count);
    }
    
}


int QuickSort (int*& arr, int size, bool ascending)
{
    int count = 0;
    QuickSorter(arr, 0, size - 1, ascending, count);
    
    return count;
}


int QuickSorter3 (int*& arr, int small, int big, bool ascending) {
    
    if (big - small <= 0) return 0;
    int i = small; int k = small; int p = big;
    int r_val = 0;
    while (i < p) {
        bool checker ;
        if (ascending) checker = arr[i] < arr[big] ; 
        else checker = arr[i] > arr[big] ;
        if ( checker ) {
            int temp = arr[i];
            arr[i] = arr[k];
            arr[k] = temp;
            i++ ; k++;
        }
        else if (arr[i] == arr[big]) {
            p--;
            int temp = arr[i];
            arr[i] = arr[p];
            arr[p] = temp;
            r_val++;
        }
        else {
            i++;
            r_val++;
        }
        r_val++;
    }
      int m = std::min(p-k,big-p+1);
    for(int j = 0; j<m; j++){
        int temp = arr[k+j];
        arr[k+j] = arr[big-m+1+j];
        arr[big-m+1+j] = temp;
    } 
    return r_val +  QuickSorter3(arr,small,k-1,ascending) + QuickSorter3(arr,big-p+k+1,big, ascending);
}



int QuickSort3 (int*& arr, int size, bool ascending)
{
    return QuickSorter3 (arr, 0 , size - 1 , ascending);
}
