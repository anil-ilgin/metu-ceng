/********************************************************
 * Kernels to be optimized for the Metu Ceng Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "Kadersizler",                     /* Team name */
    
    "Anıl Utku Ilgın",             /* First member full name */
    "e2260073",                 /* First member id */
    
    "Ozan Adar Erol",                         /* Second member full name (leave blank if none) */
    "e2380400",                         /* Second member id (leave blank if none) */
    
    "",                         /* Third member full name (leave blank if none) */
    ""                          /* Third member id (leave blank if none) */
};

/****************
 * BOKEH KERNEL *
 ****************/

/****************************************************************
 * Various typedefs and helper functions for the bokeh function
 * You may modify these any way you like.
 ****************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) {
    if (a < b) return a;
    else return b;
}
static int max(int a, int b) {
    if (a > b ) return a;
    else return b;
}

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = 0;
    sum->green = 0;
    sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red +=  p.red;
    sum->green +=  p.green;
    sum->blue +=  p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;
    
    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
        for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
    
    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/*
 * naive_bokeh - The naive baseline version of bokeh effect with filter
 */
char naive_bokeh_descr[] = "naive_bokeh";
void naive_bokeh(int dim, pixel *src, short *flt, pixel *dst) {
    
    int i, j;
    
    for(i = 0; i < dim; i++) {
        for(j = 0; j < dim; j++) {
            if ( !flt[RIDX(i, j, dim)] )
                dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
            else
                dst[RIDX(i, j, dim)] = src[RIDX(i, j, dim)];
        }
    }
}

typedef struct {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
} edited_pixel_sum;

typedef struct {
    int dim;
    int i;
    int j;
    pixel *src;
} Args;

static pixel editedAvg(Args* args)
{
    
    edited_pixel_sum sum;
    pixel current_pixel;
    sum.red = 0;
    sum.green = 0;
    sum.blue = 0;
    
    int dim = args->dim;
    int i = args->i;
    int j = args->j;
    pixel* src = args->src;
    
    if(j == 0) {
        if(i == 0) {
            int loc = RIDX(i, j, dim) ;
            int loc2 = RIDX(i+1,j,dim) ;
            sum.red = src[loc].red + src[loc+1].red + src[loc2].red + src[loc2+1].red ;
            sum.green =  src[loc].green + src[loc+1].green + src[loc2].green + src[loc2+1].green ;
            sum.blue = src[loc].blue + src[loc+1].blue + src[loc2].blue + src[loc2+1].blue ;
            
            current_pixel.red = (sum.red >> 2);
            current_pixel.green = (sum.green >> 2);
            current_pixel.blue = (sum.blue >> 2);
        }
        else if (i == dim - 1) {
            int loc = RIDX(i,j,dim) ;
            int loc2 = RIDX(i-1,j,dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc2].red +  src[loc2+1].red ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc2].green +  src[loc2+1].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc2].blue +  src[loc2+1].blue ;
            
            current_pixel.red = (sum.red >> 2);
            current_pixel.green = (sum.green >> 2);
            current_pixel.blue = (sum.blue >> 2);
        }
        else {
            int loc = RIDX(i, j, dim) ;
            int loc2 = RIDX(i+1,j,dim) ;
            int loc3 = RIDX(i-1,j,dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc2].red +  src[loc2+1].red +  src[loc3].red +  src[loc3+1].red  ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc2].green +  src[loc2+1].green +  src[loc3].green +  src[loc3+1].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc2].blue +  src[loc2+1].blue +  src[loc3].blue +  src[loc3+1].blue  ;
            
            current_pixel.red = (sum.red/6);
            current_pixel.green = (sum.green/6);
            current_pixel.blue = (sum.blue/6);
        }
    }
    
    else if (j == dim - 1) {
        if (i == 0) {
            int loc = RIDX(i, j-1, dim) ;
            int loc2 = RIDX(i+1,j-1,dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc2].red +  src[loc2+1].red ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc2].green +  src[loc2+1].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc2].blue +  src[loc2+1].blue ;
            
            current_pixel.red = (sum.red >> 2);
            current_pixel.green = (sum.green >> 2);
            current_pixel.blue = (sum.blue >> 2);
            
        }
        else if (i == dim -1) {
            int loc = RIDX(i,j-1,dim) ;
            int loc2 = RIDX(i-1,j-1,dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc2].red +  src[loc2+1].red ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc2].green +  src[loc2+1].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc2].blue +  src[loc2+1].blue ;
            
            current_pixel.red = (sum.red >> 2);
            current_pixel.green = (sum.green >> 2);
            current_pixel.blue = (sum.blue >> 2);
        }
        else {
            int loc = RIDX(i, j-1, dim) ;
            int loc2 = RIDX(i+1,j-1,dim) ;
            int loc3 = RIDX(i-1,j-1,dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc2].red +  src[loc2+1].red +  src[loc3].red +  src[loc3+1].red  ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc2].green +  src[loc2+1].green +  src[loc3].green +  src[loc3+1].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc2].blue +  src[loc2+1].blue +  src[loc3].blue +  src[loc3+1].blue  ;
            
            current_pixel.red = (sum.red/6);
            current_pixel.green = (sum.green/6);
            current_pixel.blue = (sum.blue/6);
        }
    }
    
    else {
        if (i==0) {
            int loc = RIDX(i,j-1,dim) ;
            int loc2 = RIDX(i+1,j-1,dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc+2].red +  src[loc2].red +  src[loc2+1].red +  src[loc2+2].red ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc+2].green +  src[loc2].green +  src[loc2+1].green +  src[loc2+2].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc+2].blue +  src[loc2].blue +  src[loc2+1].blue +  src[loc2+2].blue ;
            
            current_pixel.red = (sum.red/6);
            current_pixel.green = (sum.green/6);
            current_pixel.blue = (sum.blue/6);
            
        }
        else if(i == dim-1) {
            int loc = RIDX(i-1,j-1,dim) ;
            int loc2 = RIDX(i,j-1,dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc+2].red +  src[loc2].red +  src[loc2+1].red +  src[loc2+2].red ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc+2].green +  src[loc2].green +  src[loc2+1].green +  src[loc2+2].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc+2].blue +  src[loc2].blue +  src[loc2+1].blue +  src[loc2+2].blue ;
            
            current_pixel.red = (sum.red/6);
            current_pixel.green = (sum.green/6);
            current_pixel.blue = (sum.blue/6);
        }
        else {
            int loc = RIDX(i,j-1,dim) ;
            int loc2 = RIDX(i+1,j-1,dim) ;
            int loc3 = RIDX(i-1, j-1, dim) ;
            sum.red =  src[loc].red +  src[loc+1].red +  src[loc+2].red +  src[loc2].red +  src[loc2+1].red +  src[loc2+2].red +  src[loc3].red +  src[loc3+1].red +  src[loc3+2].red  ;
            sum.green =  src[loc].green +  src[loc+1].green +  src[loc+2].green +  src[loc2].green +  src[loc2+1].green +  src[loc2+2].green +  src[loc3].green +  src[loc3+1].green +  src[loc3+2].green ;
            sum.blue =  src[loc].blue +  src[loc+1].blue +  src[loc+2].blue +  src[loc2].blue +  src[loc2+1].blue +  src[loc2+2].blue +  src[loc3].blue +  src[loc3+1].blue +  src[loc3+2].blue  ;
            
            current_pixel.red = (sum.red/9);
            current_pixel.green = (sum.green/9);
            current_pixel.blue = (sum.blue/9);
        }
    }
    
    return current_pixel;
}

/*******************************************************
 * Your different versions of the bokeh kernel go here
 *******************************************************/

char edited_bokeh_descr[] = "edited_bokeh";
void edited_bokeh(int dim, pixel *src, short *flt, pixel *dst) {
    
    int i1, i, j1, j, A = dim >> 1, pxl;
    Args a;
    a.dim = dim;
    a.src = src;
    
    for (i1 = 0; i1 < dim; i1 += A)
        for (i = i1; i < i1 + A; i++)
            for (j1 = 0; j1 < dim; j1 += A)
                for (j = j1; j < j1 + A; j++) {
                    pxl = RIDX(i,j,dim);
                    if ( !flt[pxl] ) {
                        a.i = i;
                        a.j = j;
                        dst[pxl] = editedAvg(&a);
                    }
                    else dst[pxl] = src[pxl];
                }
    
}

/*
 * bokeh - Your current working version of bokeh
 * IMPORTANT: This is the version you will be graded on
 */
char bokeh_descr[] = "bokeh: Current working version";
void bokeh(int dim, pixel *src, short *flt, pixel *dst)
{

}

/*********************************************************************
 * register_bokeh_functions - Register all of your different versions
 *     of the bokeh kernel with the driver by calling the
 *     add_bokeh_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/


void register_bokeh_functions()
{
    add_bokeh_function(&naive_bokeh, naive_bokeh_descr);
    add_bokeh_function(&edited_bokeh, edited_bokeh_descr);
    /* ... Register additional test functions here */
}

/***************************
 * HADAMARD PRODUCT KERNEL *
 ***************************/

/******************************************************
 * Your different versions of the hadamard product functions go here
 ******************************************************/


char edited_hadamard_descr[] = "edited_hadamard";
void edited_hadamard(int dim, int *src1, int *src2, int *dst) {
    
    int i, i1, A = dim >> 2;
    int len = dim * dim;
    if (dim < 1024) {
        for (i = 0; i < len; i += 4) {
            dst[i] = src1[i] * src2[i];
            dst[i+1] = src1[i+1] * src2[i+1];
            dst[i+2] = src1[i+2] * src2[i+2];
            dst[i+3] = src1[i+3] * src2[i+3];
        }
    }
    
    else {
        for(i1 = 0; i1 < len; i1 += A)
            for (i = i1; i < i1 + A; i++)
                dst[i] = src1[i] * src2[i];
    }
    
}

/*
 * naive_hadamard - The naive baseline version of hadamard product of two matrices
 */
char naive_hadamard_descr[] = "naive_hadamard";
void naive_hadamard(int dim, int *src1, int *src2, int *dst) {
    
    int i, j;
    
    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {
            dst[RIDX(i, j, dim)] = src1[RIDX(i, j, dim)] * src2[RIDX(i, j, dim)];
        }
}

/*
 * hadamard - Your current working version of hadamard product
 * IMPORTANT: This is the version you will be graded on
 */
char hadamard_descr[] = "hadamard: Current working version";
void hadamard(int dim, int *src1, int *src2, int *dst)
{

}
/*********************************************************************
 * register_hadamard_functions - Register all of your different versions
 *     of the hadamard kernel with the driver by calling the
 *     add_hadamard_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

// Sonuçlarını görmek istediğimiz fonksiyonların isimlerini ve descriptionlarını add_hadamard_function içine yazıyoruz.

void register_hadamard_functions()
{
    add_hadamard_function(&naive_hadamard, naive_hadamard_descr);
    add_hadamard_function(&edited_hadamard, edited_hadamard_descr);
    /* ... Register additional test functions here */
}



