//CS22B1093
//ROHAN G

#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT 50
#define MAX 200

char *string_to_stuffed(char *arr)
{
    char *stuffed = (char *)malloc(sizeof(char) * MAX);
    int count = 0;
    int i = 0, j = 0;

    stuffed[j++] = '0';
    for (int k = 0; k < 6; k++) {
        stuffed[j++] = '1';
    }
    stuffed[j++] = '0';

    for (int z = 0; arr[z] != '\0'; z++) {
        if (arr[z] == '1') {
            count++;
            stuffed[j++] = '1';
            if (count == 5) {
                stuffed[j++] = '0'; 
                count = 0;
            }
        } else {
            stuffed[j++] = '0';
            count = 0;
        }
    }

    stuffed[j++] = '0';
    for (int k = 0; k < 6; k++) {
        stuffed[j++] = '1';
    }
    stuffed[j++] = '0';

    stuffed[j] = '\0';
    printf("Stuffed string: %s\n", stuffed);
    return stuffed;
}

char *stuffed_to_string(char *stuffed)
{
    char *output = (char *)malloc(sizeof(char) * MAX_INPUT);
    int count = 0;
    int j = 0;

    int i = 8;

    while (stuffed[i] != '\0') {
        if (stuffed[i] == '1') {
            count++;
            output[j++] = '1';
            if (count == 5) {
                i++;
                count = 0;
            }
        } else {
            output[j++] = '0';
            count = 0;
        }
        i++;
    }

    // output[j] = '\0';

    output[j - 7] = '\0';
    
    printf("Unstuffed string: %s\n", output);
    return output;
}

int main()
{
    char arr[MAX_INPUT];
    printf("Enter the string: ");
    scanf("%s", arr);

    printf("Input string: %s\n", arr);

    char *stuffed = string_to_stuffed(arr);
    char *output = stuffed_to_string(stuffed);
    
    free(stuffed);
    free(output);
    return 0;
}