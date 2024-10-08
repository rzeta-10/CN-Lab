//CS22B1093 ROHAN G
#include <stdio.h>
#include <string.h>

#define FLAG 0x7E   // Define the flag byte
#define ESCAPE 0x7D // Define the escape byte

void byteStuffing(char *input, int length, char *stuffed, int *stuffedLength) {
    int i, j = 0;

    stuffed[j++] = FLAG;

    // Stuff the data
    for (i = 0; i < length; i++) {
        if (input[i] == FLAG) {
            stuffed[j++] = ESCAPE;
            stuffed[j++] = FLAG ^ 0x20; // XOR with 0x20 = 0x5E
        } else if (input[i] == ESCAPE) {
            stuffed[j++] = ESCAPE;
            stuffed[j++] = ESCAPE ^ 0x20; // XOR with 0x20 = 0x5D
        } else {
            stuffed[j++] = input[i];
        }
    }

    // Add the ending flag byte
    stuffed[j++] = FLAG;

    *stuffedLength = j;
}

// Function to perform byte unstuffing
void byteUnstuffing(char *stuffed, int stuffedLength,char *unstuffed, int *unstuffedLength) {
    int i = 1, j = 0; 

    while (i < stuffedLength - 1) {
        if (stuffed[i] == ESCAPE) {
            i++; 
            if (stuffed[i] == (FLAG ^ 0x20)) { // XOR with 0x20 = 0x5E
                unstuffed[j++] = FLAG;
            } else if (stuffed[i] == (ESCAPE ^ 0x20)) { // XOR with 0x20 = 0x5D
                unstuffed[j++] = ESCAPE;
            }
        } else {
            unstuffed[j++] = stuffed[i];
        }
        i++;
    }
    *unstuffedLength = j;
}

int main() {
    char input[100]; 
    char stuffed[200]; 
    char unstuffed[100];
    int unstuffedLength;

    // Get the user input
    int inputLength;
    printf("Enter the number of bytes in the data: ");
    scanf("%d", &inputLength);

    printf("Enter the data bytes in hexadecimal (e.g., 45 7E 56 7D 78):\n");
    for (int i = 0; i < inputLength; i++) {
        int byte;
        scanf("%x", &byte);
        input[i] = (char)byte;
    }

    int stuffedLength = 0;

    byteStuffing(input, inputLength, stuffed, &stuffedLength);

    printf("Stuffed Data (with flags): ");
    for (int i = 0; i < stuffedLength; i++) {
        printf("0x%X ", stuffed[i]);
    }
    printf("\n");

    byteUnstuffing(stuffed, stuffedLength, unstuffed, &unstuffedLength);

    printf("Unstuffed Data (original): ");
    for (int i = 0; i < unstuffedLength; i++) {
        printf("0x%X ", unstuffed[i]);
    }
    printf("\n");

    return 0;
}
