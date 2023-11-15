#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define c_key 7

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
