#include <stdio.h>

// DES P-box permutation table
int P_box[32] = {15, 6, 19, 20, 28, 11, 27, 16,
                 0, 14, 22, 25, 4, 17, 30, 9,
                 1, 7, 23, 13, 31, 26, 2, 8,
                 18, 12, 29, 5, 21, 10, 3, 24};

// Function to perform the P-box permutation
void P_box_permutation(int *input, int *output) {
    int i;
    for (i = 0; i < 32; i++) {
        output[i] = input[P_box[i]];
    }
}

// Function to perform the DES encryption
void des_encrypt(int *message, int *key, int *encrypted_message) {
    // Perform the P-box permutation on the message
    int message_permuted[32];
    P_box_permutation(message, message_permuted);

    // Perform the XOR operation between the message and the key
    int i;
    for (i = 0; i < 32; i++) {
        encrypted_message[i] = message_permuted[i] ^ key[i];
    }
}

// Function to perform the DES decryption
void des_decrypt(int *encrypted_message, int *key, int *decrypted_message) {
    // Perform the XOR operation between the encrypted message and the key
    int i;
    for (i = 0; i < 32; i++) {
        decrypted_message[i] = encrypted_message[i] ^ key[i];
    }

    // Perform the inverse P-box permutation on the decrypted message
    int decrypted_message_permuted[32];
    P_box_permutation(decrypted_message, decrypted_message_permuted);
    for (i = 0; i < 32; i++) {
        decrypted_message[i] = decrypted_message_permuted[i];
    }
}

int main() {
    // Example usage of the DES encryption and decryption functions
    int message[32] = {0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1,
                       1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0};
    int key[32] = {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1,
                   0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1};
    int encrypted_message[32];
    int decrypted_message[32];

    // Encrypt the message using the key
    des_encrypt(message, key, encrypted_message);

    // Decrypt the encrypted message using the key
    des_decrypt(encrypted_message, key, decrypted_message);

    // Print the original message, the encrypted message, and the decrypted message
    printf("Original message: ");
    for (int i = 0; i < 32; i++) {
printf("%d", message[i]);
}
printf("\nEncrypted message: ");
for (int i = 0; i < 32; i++) {
printf("%d", encrypted_message[i]);
}
printf("\nDecrypted message: ");
for (int i = 0; i < 32; i++) {
printf("%d", decrypted_message[i]);
}
printf("\n");
return 0;
}
