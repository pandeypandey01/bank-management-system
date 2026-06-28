#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct{
    char name[20];
    int Account_Number;
    float Total_Amount;
    int Id;
}account;

account acc;
int account_Created = 0;
float Withdraw_Amount;
int Target_account;
float Transfer_amount;

void menu(){
    printf("----- BANK MENU -----\n");
    printf("1.Create_account\n");
    printf("2.Deposite_money\n");
    printf("3.Withdraw_money\n");
    printf("4.Transfer_money\n");
    printf("5.Account_details\n");
    printf("6.Transfer_details\n");
    printf("7.Exit\n");
}
void Create_account(){
    FILE *ptr = fopen("account.txt","a+");
    if(ptr == NULL){
        printf("File error\n");
        return;
    }
    printf("Enter your name\n");
    scanf("%s",acc.name);
    printf("Enter your Account Number");
    scanf("%d",&acc.Account_Number);
    printf("Enter Costumer ID\n");
    scanf("%d",&acc.Id);
    acc.Total_Amount = 0.0;
    rewind(ptr);
    account temp;
    while(fscanf(ptr,"%s %d %d %f",temp.name,&temp.Account_Number,&temp.Id,&temp.Total_Amount) == 4){
        if(temp.Account_Number == acc.Account_Number || temp.Id == acc.Id){
            printf("Account or Customer ID already exists!\n");
            fclose(ptr);
            return;
        }
    }
   fprintf(ptr,"%s %d %d %.2f\n",acc.name,acc.Account_Number,acc.Id,acc.Total_Amount);
    printf("Account created successfully\n");
    account_Created = 1;
    fclose(ptr);
}

void Deposite_money() {
    int search_acc;
    float amount;
    int found = 0;
    account temp;
    
    FILE *ptr = fopen("account.txt", "r");
    FILE *tmpPtr = fopen("temp.txt", "w");

    if (ptr == NULL|| tmpPtr == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("Enter Account Number to Deposit into: ");
    scanf("%d", &search_acc);
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    
    while (fscanf(ptr, "%s %d %d %f", temp.name, &temp.Account_Number, &temp.Id, &temp.Total_Amount) == 4) {
        if (temp.Account_Number == search_acc) {
            temp.Total_Amount += amount; // Update the balance
            found = 1;
            printf("Deposit successful! New Balance for %s: %.2f\n", temp.name, temp.Total_Amount);
        }
        
        fprintf(tmpPtr, "%s %d %d %.2f\n", temp.name, temp.Account_Number, temp.Id, temp.Total_Amount);
    }

    fclose(ptr);
    fclose(tmpPtr);

   
    remove("account.txt");
    rename("temp.txt", "account.txt");

    if (!found) {
        printf("Account Number %d not found.\n", search_acc);
    }
}
void Withdraw_money(){
      int search_acc;
    float amount;
    int found = 0;
    account temp;

    FILE *ptr = fopen("account.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");

    if (ptr == NULL || tmp == NULL) {
        printf("File error\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &search_acc);

    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    while (fscanf(ptr, "%s %d %d %f",temp.name,&temp.Account_Number,&temp.Id,&temp.Total_Amount) == 4) {

        if (temp.Account_Number == search_acc) {
            found = 1;
            if (amount > temp.Total_Amount) {
                printf("Insufficient balance\n");
            } else {
                temp.Total_Amount -= amount;
                printf("Amount withdrawn successfully! New Balance for %s: %.2f\n", temp.name, temp.Total_Amount);
            }
            acc = temp; // Update global acc
        }
        fprintf(tmp, "%s %d %d %.2f\n", temp.name, temp.Account_Number, temp.Id, temp.Total_Amount);
    }

    fclose(ptr);
    fclose(tmp);

    remove("account.txt");
    rename("temp.txt", "account.txt");

    if (!found) {
        printf("Account Number%d not found.\n", search_acc);
    }
}
void Transfer_money() {
    int sender_acc_num, target_acc_num;
    float amount;
    int found_sender = 0, found_target = 0;
    account temp;
    account sender;

    FILE *ptr = fopen("account.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");
    if (ptr == NULL || tmp == NULL) {
        printf("File error\n");
        return;
    }

    
    printf("Enter Your Account Number: ");
    scanf("%d", &sender_acc_num);

    printf("Enter Target Account Number: ");
    scanf("%d", &target_acc_num);

    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    while (fscanf(ptr, "%s %d %d %f", temp.name, &temp.Account_Number, &temp.Id, &temp.Total_Amount) == 4) {
        if (temp.Account_Number == sender_acc_num) {
            found_sender = 1;
            if (amount > temp.Total_Amount) {
                printf("Insufficient balance\n");
                fprintf(tmp, "%s %d %d %.2f\n", temp.name, temp.Account_Number, temp.Id, temp.Total_Amount);
                continue; // skip transfer
            }
            temp.Total_Amount -= amount;
            sender = temp; // store sender info
        }

        if (temp.Account_Number == target_acc_num) {
            found_target = 1;
            temp.Total_Amount += amount;
        }

        fprintf(tmp, "%s %d %d %.2f\n", temp.name, temp.Account_Number, temp.Id, temp.Total_Amount);
    }

    fclose(ptr);
    fclose(tmp);

    if (!found_sender) {
        printf("Sender account not found\n");
        remove("temp.txt");
        return;
    }

    if (!found_target) {
        printf("Target account not found\n");
        remove("temp.txt");
        return;
    }

    remove("account.txt");
    rename("temp.txt", "account.txt");

    acc = sender;         
    Target_account = target_acc_num;
    Transfer_amount = amount;

    printf("Amount %.2f transferred to account %d successfully\n", amount, target_acc_num);
    printf("Your Remaining Balance: %.2f\n", acc.Total_Amount);
}


void Account_details() {
    int search_acc;
    account temp;
    int found = 0;

    printf("Enter Account Number: ");
    scanf("%d", &search_acc);

    FILE *ptr = fopen("account.txt", "r");
    if (!ptr) {
        printf("File not found.\n");
        return;
    }

    while(fscanf(ptr, "%s %d %d %f", temp.name, &temp.Account_Number, &temp.Id, &temp.Total_Amount) == 4) {
        if(temp.Account_Number == search_acc) {
            found = 1;
            printf("Account Holder Name: %s\n", temp.name);
            printf("Account Number: %d\n", temp.Account_Number);
            printf("Total Amount: %.2f\n", temp.Total_Amount);
            break;
        }
    }

    if(!found) printf("Account not found.\n");

    fclose(ptr);
}



void Transfer_details() {
    int search_acc;
    account temp;
    int found = 0;

    printf("Enter Account Number: ");
    scanf("%d", &search_acc);

    FILE *ptr = fopen("account.txt", "r");
    if (!ptr) {
        printf("File not found.\n");
        return;
    }

    while(fscanf(ptr, "%s %d %d %f", temp.name, &temp.Account_Number, &temp.Id, &temp.Total_Amount) == 4) {
        if(temp.Account_Number == search_acc) {
            found = 1;
            printf("Transferred Amount: %.2f\n", Transfer_amount);
            printf("Target Account Number: %d\n", Target_account);
            break;
        }
    }
    if(!found) printf("Account not found.\n");
    fclose(ptr);
}

int main(){
 int choice;
    acc.Total_Amount = 0.0;
    while(1){
    menu();
    printf("Enter choice");
    scanf("%d",&choice);
    switch (choice)
{
case 1:
    Create_account();
    break;
case 2:
    Deposite_money();
    break;
case 3:
    Withdraw_money();
    break;
case 4:
    Transfer_money();
    break;
case 5:
    Account_details();
    break;
case 6:
    Transfer_details();
    break;
case 7:
    printf("Exit");
    return 0;
    break;
default:
printf("INVALID CHOICE");
    break;
}
    }
    return 0;
}
