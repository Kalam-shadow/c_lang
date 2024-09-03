#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct account{
  char name[25];
  int accnum;
  double balc;
  double mob;
  int psswd;
} account;

void display_account_details(int accno);
void displall();
void get_balance(int accno);
int deposit(int accno, int amount);
int withdraw(int accno, int amount);
void create_account();
void delete_account(int accno, int psswd);


int main() {
    int choice, accno,accno2,ans, pass,xoice;
    double amount;
    FILE*file=fopen("bank.dat","rb");
    if (file == NULL) {
        printf("New file has been created.\n");
        FILE *ghj=fopen("bank.dat","wb");
        fclose(ghj);
    }
    account acc;

start:
while(1){
    printf("\n1.FACULTY\n"
       "2.CUSTOMER\n"
      "0.Exit\n");
    printf("\n\nEnter your Choice(0 - Exit): ");
    scanf("%d", &xoice);
    
    if(xoice==1){
        printf("\n1.CREATE ACCOUNT\n"
               "2.DELETE ACCOUNT\n3.DISPLAY ACCOUNT DETAILS\n"
              "0.Exit\n");
        while (1) {
            printf("\n\nEnter your Choice(0 - Exit): ");
            scanf("%d", &choice);
            if (choice == 1) {
                create_account();
            } else if (choice == 2) {
                printf("Enter your Account Number: ");
                scanf("%d", &accno);
                printf("Enter Your Pin Number: ");
                scanf("%d", &pass);
                delete_account(accno, pass);

            } else if (choice == 3) {
                displall();
            } else if (choice == 0) {
                //printf("Thank You!!!\n");
                break;
            } else {
                printf("Invalid Choice!!!\n");
            }
        }
    }
    else if(xoice==2){
        printf("Enter your Account Number:");scanf("%d",&accno);
        printf("Enter Your PIN Number:");scanf("%d",&pass);
        fseek(file,accno,SEEK_SET);
        fread(&acc, sizeof(struct account), 1, file);
        if(accno!=acc.accnum || pass!=acc.psswd){
            printf("Account not found.\n");
            goto start;
        }
        
        printf("\n1.DISPLAY ACCOUNT DETAILS\n"
               "2.DEPOSIT\n3.WITHDRAW\n4.GET BALANCE\n5.MONEY TRANSFER\n0.Exit\n");
        while (1) {
            printf("\n\nEnter your Choice(0 - Exit): ");
            scanf("%d", &choice);
            if (choice == 1) {
                display_account_details(accno);
            } else if (choice == 2) {
                printf("Enter the Deposit Amount: ");scanf("%lf", &amount);
                ans=deposit(accno, amount);
                if(ans==0)
                    printf("\nAmount Deposited successfully!!!\n");
            } else if (choice == 3) {
                printf("Enter the Withdraw Amount: ");scanf("%lf", &amount);
                ans=withdraw(accno, amount);
                if(ans==0)
                    printf("\nAmount Withdrawn successfully!!!\n");
            } else if (choice == 4) {
                get_balance(accno);
            } else if (choice == 5) {
                printf("Enter the Amount to be transfered: ");scanf("%lf", &amount);
                withdraw(accno, amount);
                printf("Enter the Recipient ACC No: ");scanf("%d",&accno2);
                ans=deposit(accno2,amount);
                if(ans==0)
                    printf("\nMoney Transfered Suddessfully!!!\n");
            } else if (choice == 0) {
                //printf("Thank You!!!\n");
                break;
            } else {
                printf("Invalid Choice!!!\n");
            }
        }
    }//x2
    else if(xoice==0){
        printf("Thank You!!!\n");
        break;
    }
    else {
        printf("Invalid Choice!!!\n");
    }
}

    fclose(file);
    return 0;
}


void create_account(){
    FILE *f=fopen("bank.dat","ab+");
    account newacc;
    fseek(f,0,SEEK_END);
    int fpos=ftell(f);
    printf("Enter your Name: ");
    scanf("%s", newacc.name);
    printf("Enter Your Mobile Number: ");
    scanf("%lf", &newacc.mob);
    printf("Create a 4-digit Pin: ");
    scanf("%d", &newacc.psswd);
    newacc.accnum = fpos;
    newacc.balc = 500;
    fwrite(&newacc, sizeof(struct account), 1, f);
    fclose(f);
    printf("Account created successfully with Amount 500 only!!!\nYour Account Number is %d", newacc.accnum);
}

void display_account_details(int accno) {
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    if (f==NULL){
        printf("Error opening in file!!!");
        return;
    }
    fseek(f,accno,SEEK_SET);
    fread(&acc, sizeof(struct account), 1, f);
    if(accno!=acc.accnum){
        printf("Account not found.\n");
        return;
    }     
    printf("Account Number: %d\n", acc.accnum);
    printf("Name: %s\n", acc.name);
    printf("Mobile Number: %.0lf\n", acc.mob);
    printf("Balance: %0.2lf\n", acc.balc);
    
    fclose(f);
    
}

int withdraw(int accno, int amount) {
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    if (f == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    fseek(f,accno,SEEK_SET);
    fread(&acc, sizeof(struct account), 1, f);
    if(accno!=acc.accnum){
        printf("Account not found.\n");
        return 1;
    }
    if (acc.balc >= (double)amount) {
        acc.balc -= (double)amount;
        fseek(f, accno, SEEK_SET);
        fwrite(&acc, sizeof(struct account), 1, f);
        
    }
    else {
        printf("Insufficient Balance!!!\n");
    }
    fclose(f);
    return 0;
}

int deposit(int accno, int amount) {
    FILE *f = fopen("bank.dat", "rb+");
    if (f == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    account acc;
    fseek(f,accno,SEEK_SET);
    fread(&acc, sizeof(struct account), 1, f);
    if(accno!=acc.accnum){
        printf("Account not found.\n");
        return 1;
    }
    acc.balc += (double)amount;
    fseek(f, accno, SEEK_SET); 
    fwrite(&acc, sizeof(struct account), 1, f);
   
    fclose(f);
    return 0;
}

void delete_account(int accno, int pass){
    FILE *f = fopen("bank.dat", "rb+");
    if (f == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    account acc;
    fseek(f,accno,SEEK_SET);
    fread(&acc, sizeof(struct account), 1, f);
    if(accno!=acc.accnum){
        printf("Account not found.\n");
        return;
    }
    acc.accnum=2;
    fseek(f,accno, SEEK_SET); 
    fwrite(&acc, sizeof(struct account), 1, f);
    printf("\nAccount Deleted successfully!!!\n");
    fclose(f);
}

void get_balance(int accno){
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    if (f==NULL){
        printf("Error opening in file!!!");
        return;
    }
    fseek(f,accno,SEEK_SET);
    fread(&acc, sizeof(struct account), 1, f);
    if(accno!=acc.accnum){
        printf("Account not found.\n");
        return;
    }     
    printf("Balance: %0.2lf\n", acc.balc);
    fclose(f);
}

void displall(){
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    if (f==NULL){
        printf("Error opening in file!!!");
        return;
    }
while( fread(&acc, sizeof(struct account), 1, f)==1){
    if(acc.accnum!=2){
        printf("%d\t%s\t\t%.0lf\t%0.2lf\t%d\n", acc.accnum, acc.name, acc.mob,acc.balc,acc.psswd);
    }
}
    fclose(f);
}
