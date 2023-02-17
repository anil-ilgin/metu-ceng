#include "the4.h"

// do not add extra libraries here


#define DEBUG 
#define DEBUG_STDERR(x) do { std::cerr << (x) << endl; } while(0)
#define DEBUG_ARRAY(a, n) do { for (int i = 0; i < n; i++) std::cerr << a[i] << " "; std::cerr << endl; } while(0)

// for example usage of DEBUG macros check test.cpp

int min_calc (int x, int y) {
    if (x < y) return x;
    else return y;
}


//l means line, n means where we're 
int Assembly_Rec_Helper(int*& a1, int*& a2, int n, int*&t1, int*& t2, int e1, int e2, int l, int& c_count ) {
    if (n == 0) {
        c_count++;
        if (l == 0) return e1 + a1[0] ;
        else return e2 + a2[0] ;
    }

    int fin_ret;
    if (l == 0) {
        c_count++;
        fin_ret = min_calc(Assembly_Rec_Helper(a1,a2,n-1,t1,t2,e1,e2,0,c_count) + a1[n], Assembly_Rec_Helper(a1,a2,n-1,t1,t2,e1,e2,1,c_count) + a1[n] + t2[n-1] ) ;
    }
    else {
        c_count++;
        fin_ret = min_calc(Assembly_Rec_Helper(a1,a2,n-1,t1,t2,e1,e2,1,c_count) + a2[n], Assembly_Rec_Helper(a1,a2,n-1,t1,t2,e1,e2,0,c_count) + a2[n] + t1[n-1] ) ;
    }
    return fin_ret ;
}

int Assembly_Memo_Helper(int*& a1, int*& a2, int n, int *& t1, int *& t2, int e1, int e2, int l, int& c_count, int** & matrix) {
    if (n == 0) {
        c_count++;
        if (l == 0) return e1 + a1[0] ;
        else return e2 + a2[0] ;
    }
    int fin_ret;
    if (l == 0) {
        c_count++;
        if(matrix[0][n] == -1) {
            
            fin_ret = min_calc(Assembly_Memo_Helper(a1,a2,n-1,t1,t2,e1,e2,0,c_count,matrix) + a1[n], Assembly_Memo_Helper(a1,a2,n-1,t1,t2,e1,e2,1,c_count,matrix) + a1[n] + t2[n-1] ) ;
            matrix[0][n] = fin_ret ;
        }
        else fin_ret = matrix[0][n] ;
        
    }
    else {
        c_count++;
        if (matrix[1][n] == -1) {
            
            fin_ret = min_calc(Assembly_Memo_Helper(a1,a2,n-1,t1,t2,e1,e2,1,c_count,matrix) + a2[n], Assembly_Memo_Helper(a1,a2,n-1,t1,t2,e1,e2,0,c_count,matrix) + a2[n] + t1[n-1] ) ;
            matrix[1][n] = fin_ret ;
        }
        else {
            fin_ret = matrix[1][n] ;
        }
        
    }
    return fin_ret ;
    
} 




std::pair<int,int> Assembly_Rec (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    // how to make pair
    std::pair<int,int> retVal;
    retVal.first = -1;
    retVal.second = -1;
    int c_count = 0;
    retVal.first =  min_calc (Assembly_Rec_Helper(a1,a2,n-1,t1,t2,e1,e2,0, c_count) + x1, Assembly_Rec_Helper(a1,a2,n-1,t1,t2,e1,e2,1, c_count) + x2  ) ;
    retVal.second = c_count;
    return retVal ;
    
}

std::pair<int,int> Assembly_Memo (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    
    std::pair<int,int> retVal;
    retVal.first = -1;
    retVal.second = -1;
    int** matrix = new int* [2];
    matrix[0] = new int[n];
    matrix[1] = new int[n];
    for (int i=0; i < n; i++) {
        matrix[0][i] = -1;
        matrix[1][i] = -1;
    }
    int c_count = 0;
    retVal.first = min_calc(Assembly_Memo_Helper(a1,a2,n-1,t1,t2,e1,e2,0,c_count,matrix) + x1, Assembly_Memo_Helper(a1,a2,n-1,t1,t2,e1,e2,1,c_count,matrix) + x2) ;
    retVal.second = c_count;
    delete [] matrix[1];
    delete [] matrix[0];
    delete [] matrix;
    return retVal;

}

std::pair<int,int> Assembly_Tab (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    std::pair<int,int> retVal;
    retVal.first = -1;
    retVal.second = -1;
    int f1[n];
    int f2[n];
    f1[0] = e1 + a1[0];
    f2[0] = e2 + a2[0];
    for(int i=1; i <= n; i++) {
        if (f1[i-1]+a1[i] <= f2[i-1]+t2[i-1]+a1[i]) f1[i] = f1[i-1]+a1[i] ;
        else f1[i] = f2[i-1] + t2[i-1] + a1[i] ;
        if (f2[i-1]+a2[i] <= f1[i-1]+t1[i-1]+a2[i]) f2[i] = f2[i-1] + a2[i] ;
        else f2[i] = f1[i-1]+t1[i-1]+a2[i]; 
    }
    if(f1[n-1]+x1 < f2[n-1]+x2) retVal.first = f1[n-1] + x1;
    else retVal.first = f2[n-1]+x2;
    retVal.second = n;
    return retVal;

}