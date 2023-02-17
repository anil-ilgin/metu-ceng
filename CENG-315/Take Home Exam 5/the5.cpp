#include "the5.h"

// do not add extra libraries here


#define DEBUG 
#define DEBUG_STDERR(x) do { std::cerr << (x) << endl; } while(0)
#define DEBUG_ARRAY(a, n) do { for (int i = 0; i < n; i++) std::cerr << a[i] << " "; std::cerr << endl; } while(0)

// for example usage of DEBUG macros check test.cpp

int calc_max(int x, int y) {
    if(x > y) return x;
    else return y;
}

int Buy_calc (int **& market, int n1, int n2, int i, int j, vector<vector<int> >& cache) {
    if (cache[i][j] != INT_MIN) {
        return cache[i][j];
    }
    else {
        if (i == n1 - 1) {
            cache[i][j] = market[i][j];
            return cache[i][j];
        }
        int val = INT_MIN;
        for(int a=j+1; a < n2-n1+i+2; a++) {
            val = calc_max(val, Buy_calc(market,n1,n2,i+1,a,cache));
        }
        cache[i][j] = val + market[i][j];
    }
}


int BuyStocks (int**& market, int n1, int n2, vector<int>& solution)
{   
    vector<vector<int> > cache;
    vector<int> sample;
    for(int i=0; i < n2; i++) {
        sample.push_back(INT_MIN);
    }
    for(int i=0; i < n1; i++) {
        cache.push_back(sample);
    }
    for(int i=0; i < n2-n1+1; i++) {
        Buy_calc(market,n1,n2,0,i,cache);
    }
    int prev = -1;
    int cursor;
    for(int i=0; i < n1; i++) {
        cursor = prev+1;
        for(int j=cursor+1; j < n2; j++) {
            if(cache[i][j] > cache[i][cursor] ) cursor = j;
        }
        solution.push_back(cursor);
        prev = cursor;
    }
    
    int ret_val = cache[0][solution[0]];

    return ret_val;
}