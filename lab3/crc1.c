//CS22B1093
//ROHAN G

#include <stdio.h>
#include <string.h>

#define POLY_LENGTH strlen(generatorPolynomial)

char dataToSend[28];
char crcValue[28];
char generatorPolynomial[10];

int originalDataLength, i, j;

void performXOR();
void computeCRC();
void verifyReceivedData();

void performXOR() {
    for (j = 1; j < POLY_LENGTH; j++)
        crcValue[j] = ((crcValue[j] == generatorPolynomial[j]) ? '0' : '1');
}

void computeCRC() {
    for (i = 0; i < POLY_LENGTH; i++)
        crcValue[i] = dataToSend[i];

    do {
        if (crcValue[0] == '1')
            performXOR();

        for (j = 0; j < POLY_LENGTH - 1; j++)
            crcValue[j] = crcValue[j + 1];

        crcValue[j] = dataToSend[i++];
    } while (i <= originalDataLength + POLY_LENGTH - 1);
}

void verifyReceivedData() {
    printf("\nEnter the received data: ");
    scanf("%s", dataToSend);
    printf("Data received: %s", dataToSend);
    computeCRC();

    for (i = 0; (i < POLY_LENGTH - 1) && (crcValue[i] != '1'); i++);

    if (i < POLY_LENGTH - 1)
        printf("\nError detected\n\n");
    else
        printf("\nNo error detected\n\n");
}

int main() {
    printf("Enter data to be transmitted: ");
    scanf("%s", dataToSend);
    printf("\nEnter the Generator Polynomial: ");
    scanf("%s", generatorPolynomial);

    originalDataLength = strlen(dataToSend);

    for (i = originalDataLength; i < originalDataLength + POLY_LENGTH - 1; i++)
        dataToSend[i] = '0';
    dataToSend[i] = '\0';

    printf("Data padded with %ld zeros: %s", POLY_LENGTH - 1, dataToSend);

    computeCRC();
    printf("\nCRC : %s", crcValue);

    for (i = originalDataLength; i < originalDataLength + POLY_LENGTH - 1; i++)
        dataToSend[i] = crcValue[i - originalDataLength];
    dataToSend[i] = '\0';

    printf("\nFinal data to be sent: %s", dataToSend);

    verifyReceivedData();

    return 0;
}
