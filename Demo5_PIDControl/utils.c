#include "stm32f10x.h"                  // Device header
#include <cstring>
#include <cmath>
#include <stdlib.h>

int max(int a, int b){return (a>b)? a:b;}
int min(int a, int b){return (a<b)? a:b;}
int abs(int a){return (a>=0)? a:-a;}

void exhaustive_delay(int t) {           // delay by t ms
    int i; int j;
    for (i = 0; i < t; i++) {
        for (j=0;j<2666 ;j++)
        { GPIOA->BSRR ^= 0x00;} // do something to PA0 	 		
    }
}
void exhaustive_delay_us(uint32_t xus){
    int i; int j;
    for (i = 0; i < (xus/49); i++) {
        for (j=0;j<7 ;j++)
        { GPIOA->BSRR ^= 0x00;} // do something to PA0 	 		
    }
}


char* str_slice(char src[], int src_start, int length) {
    char* sliced = (char*) malloc((length + 1) * sizeof(char));
    memcpy(sliced, src + src_start, length);
    sliced[length] = '\0';
    return sliced;
}

int str_is_equal(char str1[], char str2[]){
    return strcmp(str1, str2) == 0;
}

int checkPositiveInteger(char str[]) {
	int num = 0;
	int length = strlen(str);
	if (length == 0){return -1;}
	for (int i=0; i<length; i++){
		if (str[length-i-1]<48 || str[length-i-1]>57) {return -1;}
		num += pow(10, i) * (str[length-i-1]-48);
	}
	return num;
}

int checkPositiveInteger_2char(char ch1, char ch2) {
	if (ch1<48 || ch1>57 || ch2<48 || ch2>57) {return -1;}
	int num = 10*(ch1-48) + (ch2-48);
	return num;
}


