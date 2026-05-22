#ifndef BOX_LOGIC_CODE_H_
#define BOX_LOGIC_CODE_H_

#include "zf_common_headfile.h"
/********************
仓位结构体
********************/
typedef struct
{  
    int position;  
    int value;  
    int nums;  
} box_message;  

void swap(int* a, int* b);
void bubbleSortDescending(int arr[], int n);
int findMostFrequent(int arr[], int n);
int isValueInArray(box_message arr[], int size, int value);
void put_in_box(int position);
void put_out_box(int position);
int card_convert_small_to_big(int nums);
void card_sorting_in_straight(void);
void put_card_in_str(void);
void card_sorting_cir_cro(void);
void put_card_in_cir_cro(void);

extern int box_temp_pointer;
extern int box_location_in_str;
extern int box_location_in_cir_cro;
extern int box_location_in_str;
extern int box_location_in_cir_cro;
extern box_message boxs_in_str[6];

#endif 
