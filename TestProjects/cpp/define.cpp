#include <stdio.h>
    #define HCI_INP_CODING_LINEAR 0x0000 /* 0000000000 */
    #define HCI_INP_DATA_FMT_2S_COMPLEMENT 0x0040  /* 0001000000 */
    #define HCI_INP_SAMPLE_SIZE_16BIT 0x0020 /* 0000100000 */
    #define HCI_AIR_CODING_FORMAT_CVSD 0x0000     /* 0000000000 */
    #define HCI_AIR_CODING_FORMAT_TRANSPNT 0x0003 /* 0000000011 */

#define BTM_VOICE_SETTING_CVSD                                         \
  ((HCI_INP_CODING_LINEAR | HCI_INP_DATA_FMT_2S_COMPLEMENT | \
              HCI_INP_SAMPLE_SIZE_16BIT | HCI_AIR_CODING_FORMAT_CVSD))

#define BTM_VOICE_SETTING_TRANS                                        \
  ((HCI_INP_CODING_LINEAR | HCI_INP_DATA_FMT_2S_COMPLEMENT | \
              HCI_INP_SAMPLE_SIZE_16BIT | HCI_AIR_CODING_FORMAT_TRANSPNT))

typedef double matrix[4][4];
matrix matrix_a;

// #define MATRIX double matrix[4][4]
// MATRIX matrix_a;
uintmax_t c;

int main()  
{   
    int a = BTM_VOICE_SETTING_CVSD;
    int b = BTM_VOICE_SETTING_TRANS;
    printf("[%x %x]", a, b);
    return 0;  
}  