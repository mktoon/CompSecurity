#include <stdint.h>
#include <stdio.h>
// Function declarations
uint64_t initial_permutation(uint64_t plaintext);
uint64_t final_permutation(uint64_t ciphertext);
uint64_t feistel_function(uint32_t block, uint64_t round_key);

typedef struct {
    uint32_t left;
    uint32_t right;
} uint48_t;

typedef struct {
    uint32_t left;
    uint32_t right;
} uint28_t;
// Define the PC-1 permutation table
const int pc1_table[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};
// Define the PC-2 permutation table
const int pc2_table[] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Permutation tables used in the encryption and decryption functions
const uint8_t initial_permutation_table[64] = { 
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
    };
    

const uint8_t final_permutation_table[64] = { 
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
 };
const uint8_t expansion_permutation_table[48] = { 
                        2, 1, 2, 3, 4, 5,
                        4, 5, 6, 7, 8, 9,
                        8, 9, 10, 11, 12, 13,
                        12, 13, 14, 15, 16, 17,
                        16, 17, 18, 19, 20, 21,
                        20, 21, 22, 23, 24, 25,
                        24, 25, 26, 27, 28, 29,
                        28, 29, 30, 31, 32, 1 
 };
const uint8_t permutation_table[32] = { 
                        16, 7, 20, 21, 29, 12, 28, 17,
                        1, 15, 23, 26, 5, 18, 31, 10,
                        2, 8, 24, 14, 32, 27, 3, 9,
                        19, 13, 30, 6, 22, 11, 4, 25
 };

uint64_t initial_permutation(uint64_t plaintext){
    uint64_t permuted_text = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t bit = ((plaintext >> (64 - initial_permutation_table[i])) & 1) << (63 - i);
        permuted_text |= bit;
    }
    return permuted_text;
}

uint64_t final_permutation(uint64_t ciphertext){
    uint64_t permuted_ciphertext = 0;
        for (int i = 0; i < 64; i++) {
        permuted_ciphertext |= ((ciphertext >> (64 - final_permutation_table[i])) & 1) << (63 - i);
    }
    return permuted_ciphertext;
}



    // S-Boxes used in the Feistel function
const uint8_t s_boxes[8][4][16] = {
	{{ 
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
    }, 
    { 
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10, 
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5, 
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15, 
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 
    }, 
    { 
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8, 
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1, 
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7, 
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 
    }, 
    { 
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15, 
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9, 
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4, 
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 
    }, 
    { 
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9, 
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, 
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14, 
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 
    }, 
    { 
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11, 
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8, 
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6, 
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 
    }, 
    { 
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1, 
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6, 
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2, 
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 
    }, 
    { 
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7, 
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2, 
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8, 
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 
    },}
};





uint64_t feistel_function(uint32_t block, uint64_t round_key){
    // Extract the relevant 48 bits from the 64-bit round key
    uint64_t expanded_key = 0;

    for (int i = 0; i < 48; i++) {
        uint64_t bit = ((round_key >> (64 - expansion_permutation_table[i])) & 1) << (47 - i);
        expanded_key |= bit;
    }
    
    // XOR the expanded key with the block
    uint64_t xored_block = expanded_key ^ block;
    
    // Apply the S-Boxes to the xored block
    uint64_t s_box_output = 0;
    for (int i = 0; i < 8; i++) {
        int s_box_index = ((xored_block >> (42 - i*6)) & 0x3f);
        uint8_t s_box_value = s_boxes[i][(s_box_index >> 4) & 0x0f][s_box_index & 0x0f];
        s_box_output |= ((uint64_t)s_box_value) << (32 - i*4);
    }
    
    // Apply the P-Box permutation to the S-Box output
    uint32_t permuted_s_box_output = 0;
    for (int i = 0; i < 32; i++) {
        uint32_t bit = ((s_box_output >> (32 - permutation_table[i])) & 1) << (31 - i);
        permuted_s_box_output |= bit;
    }
    
    // Return the permuted S-Box output
    return permuted_s_box_output;
}

// Helper function to perform a circular left shift
int circular_left_shift(int value, int shift) {
    return (value << shift) | (value >> (28 - shift));
}

int* key_schedule_generation(int key){
    static int round_keys[16];
    
    // Apply PC-1 permutation to the key
    int pc1_key = 0;
    for (int i = 0; i < 56; i++) {
        pc1_key |= ((key >> (64 - pc1_table[i])) & 0x1) << (55 - i);
    }
    
    // Split the permuted key into left and right halves
    int left_half = (pc1_key >> 28) & 0xFFFFFFF;
    int right_half = pc1_key & 0xFFFFFFF;
    
    // Generate 16 round keys
    int key_shifts[]={};
    for (int i = 0; i < 16; i++) {
        // Perform circular left shift on both halves
        left_half = circular_left_shift(left_half, key_shifts[i]);
        right_half = circular_left_shift(right_half, key_shifts[i]);
        
        // Combine the halves and apply PC-2 permutation to generate the round key
        int combined_half = ( left_half << 28) | right_half;
        int round_key = 0;
        for (int j = 0; j < 48; j++) {
            round_key = ((combined_half >> (56 - pc2_table[j])) & 0x1) << (47 - j);
        }
        round_keys[i] = round_key;
    }
    
    return round_keys;
}

uint64_t encrypt(uint64_t plaintext, uint64_t key){
     // Generate the round keys
    uint48_t* round_keys = key_schedule_generation(key);
    
    // Apply the initial permutation to the plaintext
    uint64_t permuted_plaintext = 0;
    for (int i = 0; i < 64; i++) {
        permuted_plaintext |= ((plaintext >> (64 - initial_permutation_table[i])) & 0x1) << (63 - i);
    }
    
    // Split the permuted plaintext into left and right halves
    uint32_t left_half = (permuted_plaintext >> 32) & 0xFFFFFFFF;
    uint32_t right_half = permuted_plaintext & 0xFFFFFFFF;
    
    // Perform 16 rounds of encryption
    for (int i = 0; i < 16; i++) {
        // Compute the Feistel function on the right half using the round key
        uint32_t feistel_output = feistel_function(right_half, round_keys);
        
        // XOR the left half with the Feistel output and swap the halves
        uint32_t temp = left_half;
        left_half = right_half ^ feistel_output;
        right_half = temp;
    }
    
    // Combine the left and right halves and apply the final permutation
    uint64_t combined_half = ((uint64_t) right_half << 32) | left_half;
    uint64_t ciphertext = 0;
    for (int i = 0; i < 64; i++) {
        ciphertext |= ((combined_half >> (64 - final_permutation_table[i])) & 0x1) << (63 - i);
    }
    
    return ciphertext;

}

uint64_t decrypt(uint64_t ciphertext, uint64_t key){
    // Generate the round keys using the key schedule generation function
    uint64_t* round_keys = key_schedule_generation(key);
    
    // Apply the initial permutation to the ciphertext
    uint64_t permuted_block = 0;
    for (int i = 0; i < 64; i++) {
        permuted_block |= ((ciphertext >> (64 - initial_permutation_table[i])) & 0x1) << (63 - i);
    }
    
    // Split the permuted block into left and right halves
    uint32_t left_half = (permuted_block >> 32) & 0xFFFFFFFF;
    uint32_t right_half = permuted_block & 0xFFFFFFFF;
    
    // Perform 16 rounds of the Feistel cipher
    for (int i = 15; i >= 0; i--) {
        uint32_t temp = left_half;
        left_half = feistel_function(right_half, round_keys[i]) ^ left_half;
        right_half = temp;
    }
    
    // Combine the left and right halves and apply the final permutation
    uint64_t combined_half = ((uint64_t) left_half << 32) | right_half;
    uint64_t plaintext = 0;
    for (int i = 0; i < 64; i++) {
        plaintext |= ((combined_half >> (64 - final_permutation_table[i])) & 0x1) << (63 - i);
    }
    
    return plaintext;
}
int main() {
    // Plaintext to encrypt
    uint64_t plaintext = 0x0123456789ABCDEF;

    // Key used for encryption
    uint64_t key = 0x133457799BBCDFF2;

    // Split the key into 16 round keys
    uint64_t round_keys[16];
    for (int i = 0; i < 16; i++) {
        uint64_t left = key >> 28;
        uint64_t right = key & 0xFFFFFFF;
        left = ((left << i) | (left >> (28 - i))) & 0xFFFFFFF;
        right = ((right << i) | (right >> (28 - i))) & 0xFFFFFFF;
        key = (left << 28) | right;
        round_keys[i] = ((uint64_t)left << 32) | (uint64_t)right;
    }

    // Perform the initial permutation on the plaintext
    uint64_t permuted_text = initial_permutation(plaintext);

    // Split the permuted text into two 32-bit blocks
    uint32_t left_block = (uint32_t)(permuted_text >> 32);
    uint32_t right_block = (uint32_t)(permuted_text & 0xFFFFFFFF);

    // Perform 16 rounds of encryption using the Feistel cipher
    for (int i = 0; i < 16; i++) {
        uint32_t new_left_block = right_block;
        uint32_t feistel_output = feistel_function(right_block, round_keys[i]);
        new_left_block ^= feistel_output;
        right_block = left_block;
        left_block = new_left_block;
    }

    // Combine the left and right blocks and perform the final permutation
    uint64_t ciphertext = ((uint64_t)left_block << 32) | (uint64_t)right_block;
    ciphertext = final_permutation(ciphertext);

    // Print the plaintext and ciphertext
    printf("Plaintext: 0x%016lX\n", plaintext);
    printf("Ciphertext: 0x%016lX\n", ciphertext);

    return 0;
}