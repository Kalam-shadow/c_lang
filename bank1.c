#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int anum = 1670000;

typedef struct account{
  // Basic details
  char name[50];
  int accnum;
  int balc;
  double mob;
  int psswd;
  // Account structure
  struct account *next;
} account;

account *head = NULL;

void display_account_details(int accno);
void get_balance(int accno);
void deposit(int accno, int amount, int psswd);
void withdraw(int accno, int amount, int psswd);
void create_account();
void delete_account(int accno, int psswd);
void load_accounts();
void save_accounts();

int main(void) {
    int choice, accno, amount, pass;
    load_accounts();

    printf("1.CREATE ACCOUNT\n"
           "2.DELETE ACCOUNT\n3.DISPLAY ACCOUNT DETAILS\n"
           "4.DEPOSIT\n5.WITHDRAW\n6.GET BALANCE\n0.Exit\n");

    while (1) {
        printf("\n\nEnter your Choice: ");
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
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            display_account_details(accno);
        } else if (choice == 4) {
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            printf("Enter the Deposit Amount: ");
            scanf("%d", &amount);
            printf("Enter Your Pin Number: ");
            scanf("%d", &pass);
            deposit(accno, amount, pass);
        } else if (choice == 5) {
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            printf("Enter the Withdraw Amount: ");
            scanf("%d", &amount);
            printf("Enter Your Pin Number: ");
            scanf("%d", &pass);
            withdraw(accno, amount, pass);
        } else if (choice == 6) {
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            get_balance(accno);
        } else if (choice == 0) {
            save_accounts();
            printf("Thank You!!!");
            break;
        } else {
            printf("Invalid Choice!!!\n");
        }
    }

    return 0;
}

void load_accounts() {
    FILE *fr = fopen("bank.txt", "r");
    if (fr == NULL) {
        printf("Error opening file\n");
        return;
    }
    account *newacc;
    while (!feof(fr)) {
        newacc = (struct account *)malloc(sizeof(struct account));
        if (fscanf(fr, "%d %s %lf %d %d", &newacc->accnum, newacc->name, &newacc->mob, &newacc->balc, &newacc->psswd) == 5) {
            newacc->next = head;
            head = newacc;
            if (newacc->accnum >= anum) {
                anum = newacc->accnum + 1;
            }
        } else {
            free(newacc);
        }
    }
    fclose(fr);
}

void save_accounts() {
    FILE *fw = fopen("bank.txt", "w");
    if (fw == NULL) {
        printf("Error opening file\n");
        return;
    }
    account *curr = head;
    while (curr != NULL) {
        fprintf(fw, "%d %s %lf %d %d\n", curr->accnum, curr->name, curr->mob, curr->balc, curr->psswd);
        curr = curr->next;
    }
    fclose(fw);
}

void create_account() {
    account *newacc = (struct account *)malloc(sizeof(struct account));
    printf("Enter your Name: ");
    scanf("%s", newacc->name);
    printf("Enter Your Mobile Number: ");
    scanf("%lf", &newacc->mob);
    printf("Create a 4-digit Pin: ");
    scanf("%d", &newacc->psswd);
    newacc->accnum = anum++;
    newacc->balc = 500;
    newacc->next = head;
    head = newacc;
    printf("Account created successfully with Amount 500 only!!!\nYour Account Number is %d", newacc->accnum);
    save_accounts();
}

void delete_account(int accno, int pass) {
    account *disp = head;
    account *prev = NULL;
    while (disp != NULL && accno != disp->accnum) {
        prev = disp;
        disp = disp->next;
    }
    if (disp == NULL) {
        printf("No Such Account Number!!!");
        return;
    }
    if (pass != disp->psswd) {
        printf("Wrong Pin Number!!!");
        return;
    }
    if (prev != NULL)
        prev->next = disp->next;
    else
        head = disp->next;
    free(disp);
    printf("\nAccount Deleted!!!\n");
    save_accounts();
}

void display_account_details(int accno) {
    account *disp = head;
    int found = 0;
    while (disp != NULL) {
        if (disp->accnum == accno) {
            found = 1;
            printf("\nAccount number: \t\t%d\n", disp->accnum);
            printf("Account Holder Name: \t%s\n", disp->name);
            printf("Account Balance: \t\t%d\n", disp->balc);
            printf("Mobile Number: \t\t\t%0.0lf\n", disp->mob);
            break;
        }
        disp = disp->next;
    }
    if (!found)
        printf("\nWrong Account Number!!!\n");
}

void get_balance(int accno) {
    account *disp = head;
    int found = 0;
    while (disp != NULL) {
        if (disp->accnum == accno) {
            found = 1;
            printf("\nAccount Balance: %d\n", disp->balc);
            break;
        }
        disp = disp->next;
    }
    if (!found)
        printf("Wrong Account Number!!!");
}

void deposit(int accno, int amount, int pass) {
    account *disp = head;
    int found = 0, pin_correct = 0;
    while (disp != NULL) {
        if (accno == disp->accnum) {
            found = 1;
            if (disp->psswd == pass) {
                pin_correct = 1;
                disp->balc += amount;
                printf("\nAmount Deposited successfully!!!\n");
                break;
            }
        }
        disp = disp->next;
    }
    if (!found)
        printf("Wrong Account Number!!!\n");
    else if (!pin_correct)
        printf("Wrong Pin Number!!!\n");
    save_accounts();
}


void withdraw(int accno, int amount, int pass) {
    account *disp = head;
    int found = 0, pin_correct = 0;
    while (disp != NULL) {
        if (accno == disp->accnum) {
            found = 1;
            if (disp->psswd == pass) {
                pin_correct = 1;
                if (disp->balc >= amount) {
                    disp->balc -= amount;
                    printf("\nAmount Withdrawn successfully!!!\n");
                } else {
                    printf("Insufficient Balance!!!\n");
                }
                break;
            }
        }
        disp = disp->next;
    }
    if (!found)
        printf("Wrong Account Number!!!\n");
    else if (!pin_correct)
        printf("Wrong Pin Number!!!\n");
    save_accounts();
}
