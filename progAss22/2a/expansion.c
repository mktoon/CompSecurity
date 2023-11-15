#include <stdio.h>
// expansion table
    int expansion_table[48] = {31, 0, 1, 2, 3, 4, 3, 4, 5, 6, 7, 8, 7, 8, 9, 10,
                                11, 12, 11, 12, 13, 14, 15, 16, 15, 16, 17, 18,
                                19, 20, 19, 20, 21, 22, 23, 24, 23, 24, 25, 26,
                                27, 28, 27, 28, 29, 30, 31, 0};

void expand(int* input_bits, int* output_bits) {
    
    for (int i = 0; i < 48; i++) {
        output_bits[i] = input_bits[expansion_table[i]];
    }
}

// Perform the expansion permutation
void expansion_permutation(int input[32], int output[48]) {
    for (int i = 0; i < 48; i++) {
        output[i] = input[expansion_table[i]];
    }
}

void encrypt(int* message_bits, int* key_bits, int* ciphertext_bits) {
    int message_expanded[48];
    int key_expanded[48];
    int message_xor_key[48];

    // expand message and key bits
    expand(message_bits, message_expanded);
    expand(key_bits, key_expanded);

    // xor message and key
    for (int i = 0; i < 48; i++) {
        message_xor_key[i] = message_expanded[i] ^ key_expanded[i];
    }

    // set ciphertext bits to result of xor operation
    for (int i = 0; i < 48; i++) {
        ciphertext_bits[i] = message_xor_key[i];
    }
}

void decrypt(int* ciphertext_bits, int* key_bits, int* decrypted_bits) {
    int ciphertext_expanded[48];
    int key_expanded[48];
    int ciphertext_xor_key[48];

    // expand ciphertext and key bits
    expand(ciphertext_bits, ciphertext_expanded);
    expand(key_bits, key_expanded);

    // xor ciphertext and key
    for (int i = 0; i < 48; i++) {
        ciphertext_xor_key[i] = ciphertext_expanded[i] ^ key_expanded[i];
    }

    // set decrypted bits to result of xor operation
    for (int i = 0; i < 48; i++) {
        decrypted_bits[i] = ciphertext_xor_key[i];
    }
}

int main() {
    int input[32] = {
        0, 1, 0, 1, 1, 0, 1, 0,  // first 8 bits
        1, 0, 1, 1, 0, 1, 0, 1,  // second 8 bits
        0, 1, 1, 0, 1, 0, 1, 1,  // third 8 bits
        1, 0, 1, 0, 0, 1, 1, 0   // fourth 8 bits
    };
    int output[48];
    expansion_permutation(input, output);
    
    printf("Output: ");
    for (int i = 0; i < 48; i++) {
        printf("%d", output[i]);
    }
    printf("\n");
    printf("using it to perform encryption and decryption\n");
    int message[32] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};
    int key[32] = {1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1,0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1};
int ciphertext[48];
int decrypted[48];
printf("Original message: ");
for (int i = 0; i < 32; i++) {
    printf("%d", message[i]);
}
printf("\n");

printf("Key: ");
for (int i = 0; i < 32; i++) {
    printf("%d", key[i]);
}
printf("\n");

// encrypt message using key
encrypt(message, key, ciphertext);

printf("Encrypted message: ");
for (int i = 0; i < 48; i++) {
    printf("%d", ciphertext[i]);
}
printf("\n");

// decrypt ciphertext using key
decrypt(ciphertext, key, decrypted);

printf("Decrypted message: ");
for (int i = 0; i < 32; i++) {
    printf("%d", decrypted[i+8]); // original message starts at bit 8
}
printf("\n");

return 0;
}