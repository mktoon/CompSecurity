#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define keystream 32
#define c_key 7
//function to determine the majority (x,y,z)
int majority(int x ,int y, int z){
        int maj;
        if(x == 0){
                if(y == 0 || z == 0){//0xor0xor0 = 0
                        maj = 0;
                }else{//0x1x1
                        maj = 1;
                }
        }else{//0x1x1
                if(y == 1 || z == 1){
                        maj = 1;
                }else{
                        maj = 0;
                }
        }
        return maj;
}
//encryption
char* encrypt(char* input_str){
    char* output_str = (char*) malloc(sizeof(char) * (strlen(input_str) + 1));
    for(int i=0;i<strlen(input_str);i++){
        if(input_str[i]>='0' && input_str[i]<='9'){
            output_str[i] = (input_str[i]-'0'+c_key)%10+'0';
        }
        else if(input_str[i]>='A' && input_str[i]<='Z'){
            output_str[i] = (input_str[i]-'A'+c_key)%26+'A';
        }
        else if(input_str[i]>='a' && input_str[i]<='z'){
            output_str[i] = (input_str[i]-'a'+c_key)%26+'a';
        }
    }
    output_str[strlen(input_str)] = '\0';
    return output_str;
}
//decryption
char* decrypt(char* input_str){
    char* output_str = (char*) malloc(sizeof(char) * (strlen(input_str) + 1));
    for(int i=0;i<strlen(input_str);i++){
        if(input_str[i]>='0' && input_str[i]<='9'){
            output_str[i] = (input_str[i]-'0'-c_key+10)%10+'0';
        }
        else if(input_str[i]>='A' && input_str[i]<='Z'){
            output_str[i] = (input_str[i]-'A'-c_key+26)%26+'A';
        }
        else if(input_str[i]>='a' && input_str[i]<='z'){
            output_str[i] = (input_str[i]-'a'-c_key+26)%26+'a';
        }
    }
    output_str[strlen(input_str)] = '\0';
    return output_str;
}

int main(){
        int x[19] = {1,0,1,0,0,1,0,0,1,1,0,0,0,0,1,1,1,0,0};
        int y[22] = {0,0,1,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,1,0,1,1};
        int z[23] = {1,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,0};

        int i,j = 0;
        int m;

        printf("\n");
        for(i = 0; i < 32; i++){
                m  = majority(x[8], y[10], z[10]);

                printf("Register X: ");
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
            printf("\n");

            printf("Register Y: ");
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

        printf("\n");
		printf("register Z: ");
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
		//print the keystreams
		if(i < 32){
                	int key = x[18] ^ y[21] ^ z[22];
                	printf("\nKeystream generated= %d ^ %d ^ %d = %d",x[18],y[21],z[22],key);
                	printf("\n\n");
		}
    }

    //part B of the question 
    char input[100];
    int choice;
    //request string from user
    printf("Enter the string of 100 characters\n");
    scanf("%s", input);
    //prompt the user for a choice
    printf("Enter 1 to encrypt the above entered string\n");
    printf("Enter 2 to decrypt the above entered string\n");
    scanf("%d", &choice);
    //for encryption
    if(choice==1){
        char* output=encrypt(input);
        printf("Encrypted string: %s\n", output);
        free(output);
    }
    //for decryption
    if(choice==2){
        char* output=decrypt(input);
        printf("Decrypted string: %s\n", output);
        free(output);
    }
    return 0;
}
