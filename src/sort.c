/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include "sort_private.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void upo_insertion_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    size_t i,j;
    unsigned char *ubase = (unsigned char*) base;
    unsigned char *a=NULL, *b=NULL, *tmp=NULL;
    
    for (i=1; i<n; i++)
    {
        j = i;
        while (j>0 && cmp((void*)(ubase+j*size), (void*)(ubase+(j*size)-(1*size)))<0)
        {
            a = (unsigned char*) (ubase+j*size);
            b = (unsigned char*) (ubase+(j*size)-(1*size));
            tmp = malloc(size);
            memmove(tmp,a,size);
            memmove(a,b,size);
            memmove(b,tmp,size);
            free(tmp);
            j = j-1;
        }
    }
}

void upo_merge_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    upo_merge_sort_rec(base,0,n-1,size,cmp);
}

void upo_merge_sort_rec(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    if (lo >= hi) return;
    size_t mid = (lo+hi)/2;
    upo_merge_sort_rec(base,lo,mid,size,cmp);
    upo_merge_sort_rec(base,mid+1,hi,size,cmp);
    upo_merge(base,lo,mid,hi,size,cmp);
}

void upo_merge(void *base, size_t lo, size_t mid, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    size_t i=0, j=0, x=0, k=0;
    unsigned char *ubase = (unsigned char*) base;
    unsigned char *aux = malloc(size * (hi-lo+1));

    j = mid+1-lo;

    for (x=lo; x<=hi; x++)
    {
        memmove(aux+(x-lo)*size, ubase+x*size, size);
    }
    for (k=lo; k<=hi; k++)
    {
        if (i > (mid-lo))
        {
            memmove(ubase+k*size, aux+j*size, size);
            j = j+1;
        }
        else if (j > (hi-lo))
        {
            memmove(ubase+k*size, aux+i*size, size);
            i = i+1;
        }
        else if (cmp(aux+j*size, aux+i*size)<0)
        {
            memmove(ubase+k*size, aux+j*size, size);
            j = j+1;
        }
        else
        {
            memmove(ubase+k*size, aux+i*size, size);
            i = i+1;
        }
    }
    free(aux);
}

void upo_quick_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    upo_quick_sort_rec(base,0,n-1,size,cmp);
}

void upo_quick_sort_rec(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    if (lo >= hi) return;

    size_t j = upo_quick_partition(base,lo,hi,size,cmp);

    if (j>0) upo_quick_sort_rec(base,lo,j-1,size,cmp);
    upo_quick_sort_rec(base,j+1,hi,size,cmp);
}

size_t upo_quick_partition(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    size_t p=lo, i=lo, j=hi+1;
    unsigned char *ubase = (unsigned char*) base;
    unsigned char *tmp = malloc(size);

    while (1)
    {
        do
        {
            i = i+1;
        } while (i<hi && cmp(ubase+i*size, ubase+p*size)<0);

        do
        {
            j = j-1;
        } while (j>lo && cmp(ubase+j*size, ubase+p*size)>0);
        
        if (i >= j) break;

        memmove(tmp, ubase+i*size, size);
        memmove(ubase+i*size, ubase+j*size, size);
        memmove(ubase+j*size, tmp, size);
    }

    memmove(tmp, ubase+p*size, size);
    memmove(ubase+p*size, ubase+j*size, size);
    memmove(ubase+j*size, tmp, size);
    free(tmp);

    return j;
}
