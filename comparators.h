#ifndef COMPARATORS_H
#define COMPARATORS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define EPSYLON 1e-9

int CompareIntAscend(const void* elem1, const void* elem2);
int CompareIntDescend(const void* elem1, const void* elem2);
int CompareStrAscend(const void* elem1, const void* elem2);
int CompareStrDescend(const void* elem1, const void* elem2);
int CompareDoubAscend(const void* elem1, const void* elem2);
int CompareDoubDescend(const void* elem1, const void* elem2);
int CompareChrAscend(const void* elem1, const void* elem2);
int CompareChrDescend(const void* elem1, const void* elem2);
int CompareStrOneginBeginning(const void *elem1, const void *elem2);
int CompareStrOneginEnding(const void *elem1, const void *elem2);

int IsEqual(const double number1, const double number2);

int CompareIntAscend(const void* elem1, const void* elem2){
    assert(elem1 != 0);
    assert(elem2 != 0);

    const int val1 = *(const int *)elem1;
    const int val2 = *(const int *)elem2;
    if (val1 > val2) {return 1;}
    if (val1 < val2) {return -1;}
    else {return 0;}
}

int CompareIntDescend(const void* elem1, const void* elem2){
    return -CompareIntAscend(elem1, elem2);
}

int CompareStrAscend(const void* elem1, const void* elem2){
    assert(elem1 != 0);
    assert(elem2 != 0);

    const char * const *str1 = (const char * const*)elem1;
    const char * const *str2 = (const char * const*)elem2;
    int compare = strcmp(*str1, *str2);
    if(compare > 0) {return 1;}
    if(compare < 0) {return -1;}
    else {return 0;}
}

int CompareStrDescend(const void* elem1, const void* elem2){
    return -CompareStrAscend(elem1, elem2);
}

int CompareDoubAscend(const void* elem1, const void* elem2){
    assert(elem1 != 0);
    assert(elem2 != 0);

    const double val1 = *(const double *)elem1;
    const double val2 = *(const double *)elem2;
    if (IsEqual(val1, val2) > 0) {return 1;}
    if (IsEqual(val1, val2) < 0) {return -1;}
    else {return 0;}
}

int CompareDoubDescend(const void* elem1, const void* elem2){
    return -CompareDoubAscend(elem1, elem2);
}

int IsEqual(const double number1, const double number2){

    double delta = number1 - number2;
    if (delta > EPSYLON){
        return 1;
    }
    if (delta < -EPSYLON){
        return -1;
    }
    else{
        return 0;
    }
}

int CompareChrAscend(const void* elem1, const void* elem2){
    assert(elem1 != 0);
    assert(elem2 != 0);

    const char val1 = *(const char *)elem1;
    const char val2 = *(const char *)elem2;
    if (val1 > val2) {return 1;}
    if (val1 < val2) {return -1;}
    else {return 0;}
}

int CompareChrDescend(const void* elem1, const void* elem2){
    return -CompareChrAscend(elem1, elem2);
}


int CompareStrOneginBeginning(const void *elem1, const void *elem2){
    assert(elem1 != NULL);
    assert(elem2 != NULL);

    const char *str1 =  ((Line *)elem1)->line;
    const char *str2 =  ((Line *)elem2)->line;

    assert(str1 != NULL);
    assert(str2 != NULL);

    while (1) {
        if(!isalpha(*str1)){
            str1++;
            continue;
        }
        if(!isalpha(*str2)){
            str2++;
            continue;
        }
        int c1 = tolower((unsigned char)*str1);
        int c2 = tolower((unsigned char)*str2);
        if (c1 != c2) {return c1 - c2;}
        str1++; str2++;
    }
    return 0;
}

int CompareStrOneginEnding(const void *elem1, const void *elem2){
    assert(elem1 != NULL);
    assert(elem2 != NULL);

    const char *str1 =  ((Line *)elem1)->line;
    ssize_t len1 = ((Line *)elem1)->len;

    const char *str2 =  ((Line *)elem2)->line;
    ssize_t len2 = ((Line *)elem2)->len;

    assert(str1 != NULL);
    assert(str2 != NULL);

    while (len1 >= 0 && len2 >= 0){

        if(!isalpha(*(str1 + (len1 - 1)))){
            len1--;
            continue;
        }
        if(!isalpha(*(str2 + (len2 - 1)))){
            len2--;
            continue;
        }
        
        int int_c1 = tolower((unsigned char)*(str1 + (len1 - 1)));
        int int_c2 = tolower((unsigned char)*(str2 + (len2 - 1)));
        if (int_c1 != int_c2) {
            return int_c1 - int_c2;
        }
        else {
            len1--; len2--;
        }
    }
    return 0;
}

#endif