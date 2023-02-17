#include "the1.h"

// do not add extra libraries here


/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    size      : size of arr
    ascending : true for ascending, false for descending 
    
    you can use ceil function from cmath
    
*/
void Merger_descend(int*& arr, int l_index, int m_index, int r_index, int& count) {
     int n_1 = m_index - l_index + 1;
    int n_2 = r_index - m_index;
    int arr_1 [n_1 + 1];
    int arr_2 [n_2 + 1];
    int i,j,k;
    for(i=1; i < n_1 + 1; i++) arr_1[i] = arr[l_index + i - 1];
    for(j=1; j < n_2 + 1; j++) arr_2[j] = arr[m_index + j];
    arr_1[n_1 + 1] = INT_MIN;
    arr_2[n_2 + 1] = INT_MIN;
    i = 1; j = 1;
    for (k=l_index; k <r_index +1 ; k++ ) {
        if(i<=n_1 && j<=n_2) {
                count++;
            }
        if (arr_1[i] >= arr_2[j]) {
            arr[k] = arr_1[i];
            i++;
        }
        else {
            arr[k] = arr_2[j];
            j++;
        }

    }

}



void Merger_ascend(int*& arr, int l_index, int m_index, int r_index, int& count) {
    int n_1 = m_index - l_index + 1;
    int n_2 = r_index - m_index;
    int arr_1 [n_1 + 1];
    int arr_2 [n_2 + 1];
    int i,j,k;
    for(i=1; i < n_1 + 1; i++) arr_1[i] = arr[l_index + i - 1];
    for(j=1; j < n_2 + 1; j++) arr_2[j] = arr[m_index + j];
    arr_1[n_1 + 1] = INT_MAX;
    arr_2[n_2 + 1] = INT_MAX;
    i = 1; j = 1;
    for (k=l_index; k <r_index +1 ; k++ ) {
        if(i<=n_1 && j<=n_2) {
                count++;
            }
        if (arr_1[i] <= arr_2[j]) {
            arr[k] = arr_1[i];
            i++;
            
        }
        else {
            arr[k] = arr_2[j];
            j++;
        }

        
    }
}

void MergeSort_descend(int*& arr, int l_index, int r_index, int& count) {
    int m_index;
    if (l_index < r_index) {
        m_index = (l_index + r_index)/2 ;
        MergeSort_descend(arr,l_index,m_index, count);
        MergeSort_descend(arr,m_index +1, r_index, count);
        Merger_descend(arr,l_index,m_index,r_index, count);
    }
    return;
}


void MergeSort_ascend(int*& arr, int l_index, int r_index, int& count) {
    int m_index;
    if (l_index < r_index) {
        m_index = (l_index + r_index)/2 ;
        MergeSort_ascend(arr,l_index,m_index, count);
        MergeSort_ascend(arr,m_index +1, r_index, count);
        Merger_ascend(arr,l_index,m_index,r_index, count);
    }
    return;
}

int MergeSort (int*& arr, int size, bool ascending)
{
    int count = 0;
    if(ascending) {
        MergeSort_ascend(arr, 0, size - 1, count);
    }
    else {
        MergeSort_descend(arr, 0, size -1, count);
    }
    
    return count;   
}


void FunkySorter (int*& arr, int l_index,int r_index, bool ascending, int& count) {
    if ( (l_index == r_index ) || r_index == 0 ) return;
    if (ascending) {
        if (arr[l_index] > arr[r_index]) {
            int dummy = arr[l_index];
            arr[l_index] = arr[r_index];
            arr[r_index] = dummy;
            count++;
        }
        else {
            count++;
        }
        if (r_index - l_index >= 3) {
            FunkySorter(arr, l_index, floor( (float)r_index * (2.0/3.0)  ) , ascending,count);
            FunkySorter(arr, l_index + ceil((float)r_index * (1.0/3.0) ), r_index, ascending,count);
            FunkySorter(arr, l_index, floor( (float)r_index * (2.0/3.0)  ) , ascending,count);
        }
        else {
            return;
        }
    }
    
    
    else {
        
        if (arr[l_index] < arr[r_index]) {
            int dummy = arr[l_index];
            arr[l_index] = arr[r_index];
            arr[r_index] = dummy;
            count++;
        }
        else {
            count++;
        }
        if (r_index - l_index >= 3) {
            FunkySorter(arr, l_index, floor( (float)r_index * (2.0/3.0)  ) , ascending,count);
            FunkySorter(arr, l_index + ceil((float)r_index * (1.0/3.0) ), r_index, ascending,count);
            FunkySorter(arr, l_index, floor( (float)r_index * (2.0/3.0)  ) , ascending,count);
        }
        else {
            return;
        }
    }
}



int FunkySort (int*& arr, int size, bool ascending)
{
    int count = 0;
    FunkySorter(arr, 0, size - 1, ascending, count);
    return count;
}
