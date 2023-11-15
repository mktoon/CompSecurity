#include <stdio.h>
//function to implement the P boxes
void p_box(const int input_bits[32], int output_bits[32]) {
    static const int mapping[32] = { 15, 6, 19, 20, 28, 11, 27, 16, 0, 14, 22, 25, 4, 17, 30, 9, 1, 7, 23, 13, 31, 26, 2, 8, 18, 12, 29, 5, 21, 10, 3, 24};
    for (int i = 0; i < 32; i++) {
        output_bits[i] = input_bits[mapping[i]];
    }
}

int main() {
    int input[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    int output[32];
    p_box(input, output);
	//print out the outputs
    printf("Output bits: ");
    for (int i = 0; i < 32; i++) {
        printf("%d ", output[i]);
    }
    printf("\n");
    return 0;}