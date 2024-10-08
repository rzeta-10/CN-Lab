#include <stdio.h>

#define MAX 100

// Global variables
int rows, cols;
int data[MAX][MAX], checksum[MAX];

// Function to compute checksum
void computeChecksum() {
    int sum[MAX] = {0};  // Array to store the column-wise sum
    int carry = 0;

    // Sum up the bits column by column
    for (int i = cols-1; i >=0 ; i--) {
        int columnSum = carry;
        for (int j = 0; j < rows; j++) {
            columnSum += data[j][i];  // Directly sum integers
        }

        // Store the column result and manage carry for overflow
        sum[i] = columnSum % 2;  // Store the bit (0 or 1)
        carry = columnSum / 2;   // Store carry
    } 

    // Add carry to the least significant bit if carry exists
    while (carry > 0) {
        for (int i = cols - 1; i >= 0 && carry > 0; i--) {
            int temp = sum[i] + carry;
            sum[i] = temp % 2;
            carry = temp / 2;
        }
    }

    // Compute 1's complement of the sum
    for (int i = 0; i < cols; i++) {
        checksum[i] = (sum[i] == 0) ? 1 : 0;  // Invert bits (0 to 1 and 1 to 0)
    }
}

// Function to validate receiver data using the checksum method
void validateReceiverData() {
    int sum[MAX] = {0};  // Array to store column-wise sum
    int carry = 0;

    // Sum up the bits column by column, including the checksum
    for (int i = cols-1; i >=0 ; i--) {
        int columnSum = carry;
        for (int j = 0; j < rows; j++) {
            columnSum += data[j][i];  // Directly sum integers
        }
        columnSum += checksum[i];  // Add the checksum bit

        // Store the column result and manage carry for overflow
        sum[i] = columnSum % 2;  // Store the bit (0 or 1)
        carry = columnSum / 2;   // Store carry
    }

    // Add carry to the least significant bit if carry exists
    while (carry > 0) {
        for (int i = cols - 1; i >= 0 && carry > 0; i--) {
            int temp = sum[i] + carry;
            sum[i] = temp % 2;
            carry = temp / 2;
        }
    }

    // Check if the final sum is all 1s
    int checksumValid = 1;
    for (int i = 0; i < cols; i++) {
        if (sum[i] != 1) {
            checksumValid = 0;
            break;
        }
    }

    printf("\nChecksum validation: %s\n", checksumValid ? "Valid" : "Invalid");
}

int main() {
    // Input rows and cols
    printf("Enter the number of rows and columns: ");
    scanf("%d %d", &rows, &cols);

    // Input data bits from the sender
    printf("Enter the data (each row with %d bits):\n", cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &data[i][j]);  // Read integer values (0 or 1)
        }
    }

    // Compute checksum for sender's data
    computeChecksum();

    // Print computed checksum
    printf("\nComputed Checksum: ");
    for (int i = 0; i < cols; i++)
        printf("%d", checksum[i]);

    // Input data bits from the receiver
    printf("\n\nEnter the received data (each row with %d bits):\n", cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &data[i][j]);  // Read integer values (0 or 1)
        }
    }

    // Validate the received data using the checksum method
    validateReceiverData();

    return 0;
}