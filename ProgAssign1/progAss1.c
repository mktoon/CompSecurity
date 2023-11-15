#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define alphsize 26
char ciphertext[100000];
char key[26];
char plaintext[100000];
int freqTable[26][3];
int alphSize;

/*computing the frequency analysis of a string*/

int analyseString() {
    int count=0, position =0;
    char letter;
    //opening the file to access the ciphertext
    FILE *p = fopen("ciphertext.txt", "r");
    if (!p) {
        printf("Unable to open file\n");
        return 1;
    } 
    
    for (letter = fgetc(p); letter!= EOF; letter = fgetc(p)) {
        if (isalpha(letter)) {
            position = toupper(letter) - 'A';
            freqTable[position][1]++;
        }
        ciphertext[count++] = letter;
    }
    fclose(p);
    return 0;
}




/*permuting a key - swapp adjacent letters with corresponding frequency count*/
int permute() {
    int oldScore=0;
    char temp;
    int newScore;
    int i = 0, j, k;

    // swap permutations in groups 
    while (i < 6) {
        j = i * 4;

        while (j < i * 4 + 3) {
            k = j + 1;
            
            while (k < i * 4 + 4) {
        temp = freqTable[j][2];
        freqTable[j][2] = freqTable[k][2];
        freqTable[k][2] = temp;
        
        newScore = countDictionary();
    //check if new score is better than the oldscore
        if (newScore <= oldScore) {
            temp = freqTable[j][2];
            freqTable[j][2] = freqTable[k][2];
            freqTable[k][2] = temp;
        } 
        else {
            oldScore = newScore;
            }
            k++;
        }
       j++;
     }
    i++;
    }
    printf("Fnal permutation from the best score: \n");
    for (int i = 0; i < 26; i++) {
        
        printf("%c", freqTable[i][2]);
    }
    return 0;
}

/*decrypting a plaintext using a given key*/
int decrypt() {
   int i, j, position, temp,count =0;
   char newLetter;
   char key[26];
   printf("Enter Key: ");
   scanf("%s", key);
   strcpy(plaintext, ciphertext);

   for(int i =0; i<26; i++){
    freqTable[i][2] = ciphertext[i];
   } //end for loop

    while (i < 26) {
        while (j < 26 - i - 1) {
            if(freqTable[j][0] >freqTable[j + 1][0]){
                temp = freqTable[j][0];
                freqTable[j][0] = freqTable[j + 1][0];
                freqTable[j + 1][0] = temp;

                newLetter = freqTable[j][1];
                freqTable[j][1] = freqTable[j + 1][1];
                freqTable[j + 1][1] = newLetter;

                newLetter = freqTable[j][2];
                freqTable[j][2] = freqTable[j + 1][2];
                freqTable[j + 1][2] = newLetter;
            }//end if
            j++;
        }//end inner for
        i++;
}//end outer for

while(count <strlen(plaintext)){
 position = plaintext[count] -'A';
 //shifting position of b
 if(position >=0 && position <=26){
    plaintext[count]=freqTable[position][2];//shift position by 2 to decrypt plaintext
 }
 count++;
 
}
}//end decrypt

//converting lowercase string  to uppercase
void upperCase(char *ch){
     for (int i = 0; ch[i] != '\0'; ++i) {
        if (islower(ch[i])) {
            ch[i] = toupper(ch[i]);
        }
    }
}



    // Counting the number of dictionary words matched
int countDictionary() {
    int count = 0;
    char word[100000];
    int match =0;
    
    FILE* dict = fopen("dictionary.txt", "r");
    if(!dict){
        printf("Unable to open dictionary\n");
        return -1;
    }///end if
    
    while (fgets(word,100,dict)) {
            word[strcspn(word, "\r\n")] ='\0';
            upperCase(word);
        if (strstr(plaintext, word)) {
            match++;
        }
        count++;
    }
    fclose(dict);
    printf("Words matched in dictionary %d\n", match);
    return match;
}//end countDictionary

void sortString() {
char characters[26];
int counts[26];
int indices[26];

// Initialize the arrays
int i = 0;
while (i < 26) {
    characters[i] = 'A' + i;
    counts[i] = 0;
    indices[i] = i;
    i++;
}

// Count the frequency of each character in CipherText
int j = 0;
while (ciphertext[j] != '\0') {
    if (isalpha(ciphertext[j])) {
        counts[toupper(ciphertext[j]) - 'A']++;
    }
    j++;
}

// Sort the frequency array in descending order of count
int k = 0;
while (k < 26) {
    int l = k + 1;
    while (l < 26) {
        if (counts[l] > counts[k]) {
            // Swap counts
            int temp = counts[k];
            counts[k] = counts[l];
            counts[l] = temp;

            // Swap characters
            char temp_char = characters[k];
            characters[k] = characters[l];
            characters[l] = temp_char;

            // Swap indices
            temp = indices[k];
            indices[k] = indices[l];
            indices[l] = temp;
        }
        l++;
    }
    k++;
}

// Copy the sorted frequency array to the global variable FrequencyTable
int m = 0;
while (m < 26) {
    freqTable[m][0] = characters[m];
    freqTable[m][1] = counts[m];
    freqTable[m][2] = indices[m];
    m++;
  }

}

int main() {
    int table[26][3];

    if(!analyseString()){
        printf("cipertext from sender %s \n", ciphertext);
    }

while(1) {
    sortString();
    if(!decrypt()) {
        printf("%s\n", plaintext);
     }
     //call permute
    permute();

    printf("Frequency count in the table:\n\n");
    
    for (int i = 0; i < 26; i++) {
        printf("Letter:%c\t freqCount: %d\tswapped Letter: %c\n", freqTable[i][0], freqTable[i][1], freqTable[i][2]);
    }
    //count the number of wrds matched in the dictionary
    countDictionary();
  
}
 return 0; 
}//end main

