#ifndef __UTILS_H
#define __UTILS_H
int max(int a, int b);
int min(int a, int b);
void exhaustive_delay(int t);
void exhaustive_delay_us(uint32_t xus);
char* str_slice(char src[], int src_start, int length);
int str_is_equal(char str1[], char str2[]);
int checkPositiveInteger(char str[]);
int checkPositiveInteger_2char(char ch1, char ch2);
void USART2_Init(void);

#endif
