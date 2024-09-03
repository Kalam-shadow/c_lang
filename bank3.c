#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int anum = 1670000;

typedef struct account{

  char name[25];
  int accnum;
  double balc;
  double mob;
  int psswd;

} account;


void display_account_details(int accno);
void get_balance(int accno);
void deposit(int accno, int amount, int psswd);
void withdraw(int accno, int amount, int psswd);
void create_account();
void delete_account(int accno, int psswd);
void save();


int main() {
    int choice, accno, pass;
    int dele=0;
    double amount;
    FILE*file=fopen("bank.dat","rb");
    if (file == NULL) {
        printf("New file has been created.\n");
        FILE *ghj=fopen("bank.dat","wb");
        fclose(ghj);
    }
    account acc;
    while(!feof(file)){
        //if(fscanf(file, "%d %s %lf %lf %d\n", &acc.accnum,acc.name,&acc.mob,&acc.balc,&acc.psswd)){
        if(fread(&acc, sizeof(struct account), 1, file)){
            if(acc.accnum>=anum){
                anum = acc.accnum+1;
            }
        }     
    }

    printf("1.CREATE ACCOUNT\n"
           "2.DELETE ACCOUNT\n3.DISPLAY ACCOUNT DETAILS\n"
           "4.DEPOSIT\n5.WITHDRAW\n6.GET BALANCE\n0.Exit\n");

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
            dele=1;
            delete_account(accno, pass);

        } else if (choice == 3) {
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            display_account_details(accno);
        } else if (choice == 4) {
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            printf("Enter the Deposit Amount: ");
            scanf("%lf", &amount);
            printf("Enter Your Pin Number: ");
            scanf("%d", &pass);
            deposit(accno, amount, pass);
        } else if (choice == 5) {
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            printf("Enter the Withdraw Amount: ");
            scanf("%lf", &amount);
            printf("Enter Your Pin Number: ");
            scanf("%d", &pass);
            withdraw(accno, amount, pass);
        } else if (choice == 6) {
            printf("Enter your Account Number: ");
            scanf("%d", &accno);
            get_balance(accno);
        } else if (choice == 0) {
            if(dele == 1)
                save();
            printf("Thank You!!!");
            break;
        } else {
            printf("Invalid Choice!!!\n");
        }

    }

    fclose(file);
    return 0;
}


void create_account(){
    FILE *f=fopen("bank.dat","ab+");
    account newacc;
    printf("Enter your Name: ");
    scanf("%s", newacc.name);
    printf("Enter Your Mobile Number: ");
    scanf("%lf", &newacc.mob);
    printf("Create a 4-digit Pin: ");
    scanf("%d", &newacc.psswd);
    newacc.accnum = anum++;
    newacc.balc = 500;

    //fprintf(f, "%-10d %-20s %-15lf %-20lf %-5d\n", newacc.accnum, newacc.name, newacc.mob, newacc.balc, newacc.psswd); 
    fwrite(&newacc, sizeof(struct account), 1, f);
    fclose(f);
    printf("Account created successfully with Amount 500 only!!!\nYour Account Number is %d", newacc.accnum);
}

void display_account_details(int accno) {
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    int found = 0;
    if (f==NULL){
        printf("Error opening in file!!!");
        return;
    }

    while (1){
        if(fread(&acc, sizeof(struct account), 1, f)!=1){
            break;
        } 
        if (acc.accnum == accno) {
            found = 1;
            printf("Account Number: %d\n", acc.accnum);
            printf("Name: %s\n", acc.name);
            printf("Mobile Number: %.0lf\n", acc.mob);
            printf("Balance: %0.2lf\n", acc.balc);
            break;
        }
    }
    fclose(f);
    if (!found)
        printf("Account not found.\n");
}

void withdraw(int accno, int amount, int pass) {
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    int found = 0, pin_correct = 0,pos=0;  
    if (f == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    while (1) {
        pos=ftell(f);
        if(fread(&acc, sizeof(struct account), 1, f)!=1){
            break;
        }
        if (acc.accnum == accno) {
            found = 1;
            if (acc.psswd == pass) {
                pin_correct = 1;
                if (acc.balc >= (double)amount) {
                    acc.balc -= (double)amount;
                    fseek(f, pos, SEEK_SET);
                    fwrite(&acc, sizeof(struct account), 1, f);
                    //fprintf(f,"%-10d %-20s %-15lf %-20lf %-5d\n",acc.accnum,acc.name,acc.mob,acc.balc,acc.psswd); 
                    printf("\nAmount Withdrawn successfully!!!\n");
                }
                else {
                    printf("Insufficient Balance!!!\n");
                }
            break;
            }
        }
    }

    fclose(f);

    if (!found)
        printf("Wrong Account Number!!!\n");
    else if (!pin_correct)
        printf("Wrong Pin Number!!!\n");
}


void deposit(int accno, int amount, int pass) {
    FILE *f = fopen("bank.dat", "rb+");
    if (f == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    account acc;
    int found = 0, pin_correct = 0;
    long pos;

    while (1) {
        pos = ftell(f); 
        if (fread(&acc, sizeof(struct account), 1, f)!=1) {
            break;
        }

        if (acc.accnum == accno) {
            found = 1;
            if (acc.psswd == pass) {
                pin_correct = 1;
                acc.balc += (double)amount;
                fseek(f, pos, SEEK_SET); 
                fwrite(&acc, sizeof(struct account), 1, f);
                //fprintf(f,"%-10d %-20s %-15lf %-20lf %-5d\n", acc.accnum, acc.name, acc.mob, acc.balc, acc.psswd);  
                printf("\nAmount Deposited successfully!!!\n");
                break;
            }
        }
    }

    fclose(f);

    if (!found)
        printf("Wrong Account Number!!!\n");
    else if (!pin_correct)
        printf("Wrong Pin Number!!!\n");
}

void delete_account(int accno, int pass){
    FILE *f = fopen("bank.dat", "rb+");
    if (f == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    account acc;
    int found = 0, pin_correct = 0;
    long pos;

    while (1) {
        pos = ftell(f); 
        if (fread(&acc, sizeof(struct account), 1, f)!=1) {
            break;
        }

        if (acc.accnum == accno) {
            found = 1;
            if (acc.psswd == pass) {
                pin_correct = 1;
                acc.accnum=0;
                fseek(f,pos, SEEK_SET); 
                fwrite(&acc, sizeof(struct account), 1, f);
                //fprintf(f,"%-10d %-20s %-15lf %-20lf %-5d\n", acc.accnum, acc.name, acc.mob, acc.balc, acc.psswd); 
                printf("\nAccount Deleted successfully!!!\n");
                break;
            }
        }
    }

    fclose(f);

    if (!found)
        printf("Wrong Account Number!!!\n");
    else if (!pin_correct)
        printf("Wrong Pin Number!!!\n");

}

void get_balance(int accno){
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    int found = 0;
    if (f==NULL){
        printf("Error opening in file!!!");
        return;
    }

    while (1){
        if(fread(&acc, sizeof(struct account), 1, f)!=1){
            break;
        } 
        if (acc.accnum == accno) {
            found = 1;
            printf("Balance: %0.2lf\n", acc.balc);
            break;
        }
    }
    fclose(f);
    if (!found)
        printf("Account not found.\n");

}

void save(){
    FILE *f = fopen("bank.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (f == NULL || temp == NULL) {
        printf("Error opening the file.\n");
        if (f) fclose(f);
        if (temp) fclose(temp);
        return;
    }

    account acc;
    while (1){
        if(fread(&acc, sizeof(struct account), 1, f)!=1) {
            break;
        }
        if (acc.accnum != 0) {
            fwrite(&acc, sizeof(struct account), 1, temp);
         
        }
    }
    fclose(f);
    fclose(temp);

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

}

