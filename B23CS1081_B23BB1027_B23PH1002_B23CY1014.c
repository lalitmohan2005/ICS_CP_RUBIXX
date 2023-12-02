#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

int star() {
    for (int i = 0; i < 50; i++) {
        printf("*");
    }
    printf("\n");
}

void writeDatabase(int pin, double balance) {
    FILE *file = fopen("atm_database.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d %.2lf", pin, balance);
        fclose(file);
    }
}

void writeTransactionLog(int transactionNumber, int choice, double amount) {
    FILE *file = fopen("atm_transactions.txt", "a");
    if (file != NULL) {
        time_t t;
        struct tm *tm_info;

        time(&t);
        tm_info = localtime(&t);

        fprintf(file, "Transaction %d: %02d-%02d-%04d %02d:%02d:%02d - ", transactionNumber,
                tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900,
                tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);

        switch (choice) {
            case 1:
                fprintf(file, "Balance Checking\n");
                break;
            case 2:
                fprintf(file, "Cash Withdrawal: $%.2lf\n", amount);
                break;
            case 3:
                fprintf(file, "Cash Deposition: $%.2lf\n", amount);
                break;
            default:
                break;
        }
        fclose(file);
    }
}

void showTransactions() {
    FILE *file = fopen("atm_transactions.txt", "r");
    if (file != NULL) {
        char ch;
        int isEmpty = 1;  

        while ((ch = fgetc(file)) != EOF) {
            isEmpty = 0;
            putchar(ch);
        }

        fclose(file);

        if (isEmpty) {
            printf("No transactions done yet.\n");
        }
    } else {
        printf("Error opening transaction log file.\n");
    }
}

int main() {
    int initializedPin, pin = 0, choice, transactionNumber = 1;
    double balance, amount;
    FILE *file = fopen("atm_database.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d %lf", &initializedPin, &balance);
        fclose(file);
    }

    star();
    printf("\t\tWelcome to ODBG BANK  ATM\n");
    star();

    while (true) {
        pin = 0;
        printf("Please enter your ATM pin: ");
        int i = 0;
        char ch;

        for (i = 0; i < 10; i++) {
            ch = getch();

            if (ch == 13) { 
                break;
            } else if (ch == 8) { 
                if (i > 0) {
                    i--;
                    pin /= 10;
                    printf("\b \b");
                }
            } else if (ch >= '0' && ch <= '9' && i < 9) { 
                pin = pin * 10 + (ch - '0');
                i++;
                printf("#");
            }
        }
        printf("\n");

        if (pin == initializedPin) {
            printf("Pin is correct. Choose your transaction:\n");
            printf("1. Balance checking\n2. Cash withdrawal\n3. Cash deposition\n4. Show Transactions\n5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Your balance is: $%.2lf\n", balance);
                    break;
                case 2:
                    printf("Enter the amount to withdraw: $");
                    scanf("%lf", &amount);
                    if (amount > 0 && amount <= balance) {
                        balance -= amount;
                        printf("Withdrawal successful. Remaining balance: $%.2lf\n", balance);
                        writeTransactionLog(transactionNumber, choice, amount);
                        writeDatabase(initializedPin, balance);
                        transactionNumber++;
                    } else {
                        printf("Invalid amount or insufficient balance.\n");
                    }
                    break;
                case 3:
                    printf("Enter the amount to deposit: $");
                    scanf("%lf", &amount);
                    if (amount > 0) {
                        balance += amount;
                        printf("Deposit successful. New balance: $%.2lf\n", balance);
                        writeTransactionLog(transactionNumber, choice, amount);
                        writeDatabase(initializedPin, balance);
                        transactionNumber++;
                    } else {
                        printf("Invalid amount.\n");
                    }
                    break;
                case 4:
                    showTransactions();
                    break;
                case 5:
                   
                    writeDatabase(initializedPin, balance);
                    printf("Thank you for using the ATM. \n");
                    return 0;
                default:
                    printf("Invalid choice. Please try again.\n");
                    break;
            }
        } else {
            printf("Incorrect PIN. Please try again.\n");
        }
    }

    return 0;
}
