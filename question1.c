#include <stdio.h>
#define SIZEX 19
#define SIZEY 22
#define SIZEZ 23
#define keystream 32

int maj(int, int, int);

int main(){
        int x[] = {1,0,1,0,0,1,0,0,1,1,0,0,0,0,1,1,1,0,0};
        int y[] = {0,0,1,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,1,0,1,1};
        int z[] = {1,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,0};

        int i,j = 0;
        int m;

        printf("\n");
        for(i = 0; i < 32; i++){
                m  = maj(x[8], y[10], z[10]);

                printf("X: ");
                if(x[8] == m){
                        int t = x[13] ^ x[16] ^ x[17] ^ x[18];
                        for(j = 18 ; j >= 0; j--){
                                if(j == 0){
                                        x[j] = t;
                                }else{
                                        x[j] = x[j-1];
                                }

                        }
                }

                for(j = 0; j < 19; j++){
                        printf("%d", x[j]);
                }


                printf("\nY: ");
                if(y[10] == m){
                        int t = y[20] ^ y[21];
                        for(j = 21 ; j >= 0; j--){
                                if(j == 0){
                                        y[j] = t;
                                }else{
                                        y[j] = y[j-1];
                                }
                        }

                }
                for(j = 0; j < 22; j++){
                        printf("%d",y[j]);
                	}

		printf("\nZ: ");
                	if(z[10] == m){
                        int t = z[7] ^ z[20] ^ z[21] ^ z[22];
                        for(j = 22 ; j >= 0; j--){
                                if(j == 0){
                                        z[j] = t;
                                }else{
                                        z[j] = z[j-1];
                                }
                        }
                }
                for(j = 0; j < 23; j++){
                        printf("%d",z[j]);
                }
		
		if(i < 31){
                	int key = x[18] ^ y[21] ^ z[22];
                	printf("\nKeystream bit = %d ^ %d ^ %d = %d",x[18],y[21],z[22],key);
                	printf("\n\n");
		}
    }
}
//function to determine the majority (x,y,z)
int majorty(X8 ,Y10, Z10){
        int maj;
        if(X8 == 0){
                if(Y10 == 0 || Z10 == 0){//0xor0xor0 = 0
                        maj = 0;
                }else{//0x1x1
                        maj = 1;
                }
        }else{//0x1x1
                if(Y10 == 1 || Z10 == 1){
                        maj = 1;
                }else{
                        maj = 0;
                }
        }
        return maj;
}





/*
    int X8 = 1010010011000011100;
    int Y10 = 0b0011011100100001111011;
    int Z10 = 0b11101010001110111000010;

    // Generate 32 keystream bits and print them
    printf("Keystream bits: ");
    for (int i = 0; i < 32; i++) {
        int bit = keyStream32(X8, Y10, Z10);
        printf("%u", bit);
    }
    printf("\n");
    // Print the contents of the registers after generating the 32 bits
    printRegisters(X8, Y10, Z10);

    // Encrypt the plaintext 7e5d7fff
    int P = 0x7e5d7fff;
    int keystream = 0;

    for (int i = 0; i < 32; i++) {
        keystream = (keystream << 1) | keyStream32(X8, Y10, Z10);
    }

    int C = P ^ keystream;
    printf("Ciphertext: %08x\n", C);

    // Decrypt the ciphertext and verify the plaintext
    keystream = 0;
    for (int i = 0; i < 32; i++) {
        keystream = (keystream << 1) | keyStream32(X8, Y10, Z10);
    }
    int decrypted = C ^ keystream;
    if (decrypted == P) {
        printf("Decrypted message: %08x\n", decrypted);
    } else {
        printf("Decryption failed\n");
    }

    return 0;
}
*/