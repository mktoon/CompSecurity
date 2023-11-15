#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#define BLOCK_SIZE 64 // 64 bits

// Define the initial 56-bit key
const uint64_t initial_key = 0x133457799BBCDFF1;

// Define the left and right halves of the key
uint32_t left_half_key = 0;
uint32_t right_half_key = 0;

// Define the left and right halves of each subkey
uint32_t left_half_subkey = 0;
uint32_t right_half_subkey = 0;

// Define the circular shift amounts for each round
const int circular_shifts[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

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

// Define the initial message
const char* initial_message = "Micah";

// Define the padded message
char padded_message[16] = { 0 };


// Initial permutation table
const uint8_t IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                         60, 52, 44, 36, 28, 20, 12, 4,
                         62, 54, 46, 38, 30, 22, 14, 6,
                         64, 56, 48, 40, 32, 24, 16, 8,
                         57, 49, 41, 33, 25, 17, 9, 1,
                         59, 51, 43, 35, 27, 19, 11, 3,
                         61, 53, 45, 37, 29, 21, 13, 5,
                         63, 55, 47, 39, 31, 23, 15, 7 };

//final oermutation table
const uint8_t FP[] = {
    
};

// Expansion table
const uint8_t E[48] = { 3};

// Permutation table
const uint8_t P[32] = { 16, 7, 20, 21, 29, 12, 28, 17,
                        1, 15, 23, 26, 5, 18, 31, 10,
                        2, 8, 24, 14, 32, 27, 3, 9,
                        19, 13, 30, 6, 22, 11, 4, 25 };

// The substitution boxes. The should contain values
	// from 0 to 15 in any order.
	int substition_boxes[8][4][16]=  
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
    }};
	// The permutation table
	int permutation_tab[32] = { 
	16,7,20,21,29,12,28,17, 
	1,15,23,26,5,18,31,10, 
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25 
	}; 
	// The inverse permutation table
	int inverse_permutation[64]= { 
	40,8,48,16,56,24,64,32, 
	39,7,47,15,55,23,63,31, 
	38,6,46,14,54,22,62,30, 
	37,5,45,13,53,21,61,29, 
	36,4,44,12,52,20,60,28, 
	35,3,43,11,51,19,59,27, 
	34,2,42,10,50,18,58,26, 
	33,1,41,9,49,17,57,25 
	};
    //padding the message
void pad_message(uint8_t *message, size_t message_len, uint8_t *padded_message) {
    size_t padded_len = ((message_len + 7) / 8) * 8; // round up to nearest multiple of 8
    memcpy(padded_message, message, message_len);
    memset(padded_message + message_len, 0, padded_len - message_len);
}

// Applying the initial permutation
  	void initial_permutation(uint8_t *message, uint8_t *permuted_message) {
    for (int i = 0; i < 64; i++) {
        uint8_t bit = message[(IP[i] - 1) / 8] >> (7 - ((IP[i] - 1) % 8)) & 1;
        permuted_message[i / 8] |= bit << (7 - (i % 8));
    }
}
    //applying the final permutation 
    void final_permutation(uint8_t *output, const uint8_t *input) {
    for (int i = 0; i < FP_SIZE; i++) {
        int index = FP[i] - 1;
        int byte_index = index / 8;
        int bit_index = index % 8;
        uint8_t bit = (input[byte_index] >> (7 - bit_index)) & 0x01;
        output[i / 8] |= (bit << (7 - (i % 8)));
    }
}

	// Dividing the result into two equal halves 
	string left = perm.substr(0, 32); 
	string right = perm.substr(32, 32);
	string right_expanded = ""; 
	string res = ""; 
	string perm2 =""; 
	// The plain text is encrypted 16 times  
	// The right half of the plain text is expanded
	for(int i=0; i<16; i++) { 
    	for(int i = 0; i < 48; i++) { 
      		right_expanded += right[expansion_table[i]-1]; 
    }  // The result is xored with a key
		string xored = Xor(round_keys[i], right_expanded);  
		// The result is divided into 8 equal parts and passed 
		// through 8 s boxes. After passing through a s box, each box is reduced from 6 to 4 bits.
		for(int i=0;i<8; i++){ 
			// Finding row and column indices to lookup the s box
      		string row1= xored.substr(i*6,1) + xored.substr(i*6 + 5,1);
      		int row = convertBinaryToDecimal(row1);
      		string col1 = xored.substr(i*6 + 1,1) + xored.substr(i*6 + 2,1) + xored.substr(i*6 + 3,1) + xored.substr(i*6 + 4,1);;
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[i][row][col];
			res += convertDecimalToBinary(val);  
		} 
		// p box is applied
		for(int i = 0; i < 32; i++){ 
			perm2 += res[permutation_tab[i]-1]; 
		}
		// The result is xored with the left half
		xored = Xor(perm2, left);
		// The left and the right parts of the plain text are swapped 
		left = xored; 
		if(i < 15){ 
			string temp = right;
			right = xored;
			left = temp;
		} 
	} 
	// The halves of the plain text are applied
	string combined_text = left + right;   
	string ciphertext =""; 
	// The inverse of the initial permutation is applied
	for(int i = 0; i < 64; i++){ 
		ciphertext+= combined_text[inverse_permutation[i]-1]; 
	}
	//And we finally get the cipher text
	return ciphertext; 


void key_schedule(uint64_t initial_key, uint64_t* subkeys) {
    // Apply the PC-1 permutation to the initial key
    uint64_t permuted_key = 0;
    for (int i = 0; i < 56; i++) {
        uint64_t bit = (initial_key >> (64 - pc1_table[i])) & 0x1;
        permuted_key |= (bit << (55 - i));
    }

    // Split the permuted key into left and right halves
    uint32_t left_half_key = permuted_key >> 28;
    uint32_t right_half_key = permuted_key & 0xFFFFFFF;

    // Generate the 16 subkeys
    for (int i = 0; i < 16; i++) {
        // Apply the circular shift to the left and right halves
        if (circular_shifts[i] == 1) {
            left_half_key = ((left_half_key << 1) & 0xFFFFFFF) | (left_half_key >> 27);
            right_half_key = ((right_half_key << 1) & 0xFFFFFFF) | (right_half_key >> 27);
        } else {
            left_half_key = ((left_half_key << 2) & 0xFFFFFFF) | (left_half_key >> 26);
            right_half_key = ((right_half_key << 2) & 0xFFFFFFF) | (right_half_key >> 26);
        }

        // Combine the left and right halves
        uint64_t combined_key = ((uint64_t) left_half_key << 28) | right_half_key;

        // Apply the PC-2 permutation to the combined key
        uint64_t subkey = 0;
        for (int j = 0; j < 48; j++) {
            uint64_t bit = (combined_key >> (56 - pc2_table[j])) & 0x1;
            subkey |= (bit << (47 - j));
        }

        // Save the subkey
        subkeys[i] = subkey;
    }
}


uint64_t feistel_cipher(uint64_t block, uint64_t subkey) {
    const int num_rounds = 16;
    const int block_size = 64;
    const int half_block_size = block_size / 2;
    const int expanded_size = 48;
    const int sbox_size = 4;

    // Split the block into left and right halves
    uint32_t left_half = block >> half_block_size;
    uint32_t right_half = block & ((1 << half_block_size) - 1);

    // Perform the Feistel rounds
    for (int i = 0; i < num_rounds; i++) {
        // Compute the subkey for this round
        uint64_t round_key = subkey >> (block_size - expanded_size);
        
        // Expand the right half
        uint64_t expanded_right = 0;
        for (int j = 0; j < expanded_size; j++) {
            uint64_t bit = (right_half >> (half_block_size - E[j])) & 0x1;
            expanded_right |= (bit << (expanded_size - j - 1));
        }
        
        // XOR the expanded right half with the subkey
        uint64_t xored = expanded_right ^ round_key;
        
        // Apply the S-boxes
        uint32_t sboxed = 0;
        for (int j = 0; j < sbox_size; j++) {
            uint8_t row = ((xored >> (sbox_size * 6 - 6 - j * 6)) & 0x20) |
                          ((xored >> (sbox_size * 6 - 1 - j * 6)) & 0x01);
            uint8_t col = (xored >> (sbox_size * 6 - 5 - j * 6)) & 0x0F;
            uint8_t val = S[j][row][col];
            sboxed |= (val << (sbox_size * 4 - 4 - j * 4));
        }
        
        // Swap the left and right halves
        uint32_t temp = left_half;
        left_half = right_half;
        right_half = temp ^ sboxed;
    }
    
    // Combine the left and right halves into a single block
    uint64_t result = ((uint64_t)right_half << half_block_size) | left_half;
    return result;
}//end feistel _cipher


void pad_message(unsigned char *message, int *message_length) {
    int original_length = *message_length;
    int padding_length = BLOCK_SIZE - (*message_length % BLOCK_SIZE);

    unsigned char *padded_message = (unsigned char *) malloc(original_length + padding_length);
    memcpy(padded_message, message, original_length);

    padded_message[original_length] = 0x80; // append a '1' bit
    for (int i = original_length + 1; i < original_length + padding_length; i++) {
        padded_message[i] = 0x00; // append '0' bits
    }

    *message_length = original_length + padding_length;
    memcpy(message, padded_message, *message_length);

    free(padded_message);
}



int main()
{
    // Determine the number of blocks needed to pad the message
    int message_length = strlen(initial_message);
    int blocks_needed = (message_length + 7) / 8;

    // Pad the message with PKCS#5 padding
    memcpy(padded_message, initial_message, message_length);
    int padding_length = blocks_needed * 8 - message_length;
    memset(padded_message + message_length, padding_length, padding_length);

    // Print the padded message
    printf("Padded message: %s\n", padded_message);

    // Perform the PC-1 permutation on the initial key
    // Perform the PC-1 permutation on the initial key
for (int i = 0; i < 56; i++)
{
    uint64_t bit = (initial_key >> (63 - pc1_table[i])) & 1;
    if (i < 28)
    {
        left_half_key |= (bit << (31 - i));
    }
    else
    {
        right_half_key |= (bit << (63 - i));
    }
}

// Print the left and right halves of the key
printf("Left half of key: 0x%08X\n", left_half_key);
printf("Right half of key: 0x%08X\n", right_half_key);

// Generate the subkeys
for (int round = 0; round < 16; round++)
{
    // Apply the circular shift to the left and right halves of the key
    int shift = circular_shifts[round];
    uint32_t new_left_half_key = (left_half_key << shift) | (left_half_key >> (28 - shift));
    uint32_t new_right_half_key = (right_half_key << shift) | (right_half_key >> (28 - shift));

    // Combine the left and right halves of the key
    uint64_t shifted_key = ((uint64_t)new_left_half_key << 28) | new_right_half_key;

    // Apply the PC-2 permutation to generate the subkey
    for (int i = 0; i < 48; i++)
    {
        uint64_t bit = (shifted_key >> (63 - pc2_table[i])) & 1;
        if (i < 24)
        {
            left_half_subkey |= (bit << (23 - i));
        }
        else
        {
            right_half_subkey |= (bit << (47 - i));
        }
    }

    // Print the subkey
    printf("Subkey for round %d: 0x%08X%08X\n", round + 1, left_half_subkey, right_half_subkey);

    // Reset the left and right halves of the subkey
    left_half_subkey = 0;
    right_half_subkey = 0;

    // Update the left and right halves of the key for the next round
    left_half_key = new_left_half_key;
    right_half_key = new_right_half_key;
}

return 0;
}