#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int anum = 1670000;

typedef struct account{
  char fname[15];
  char lname[15];
  int accnum;
  double balc;
  double mob;
  int psswd;
} account;

void display_account_details(int accno);
void displall();
void get_balance(int accno);
int deposit(int accno,int accno2, int amount);
int withdraw(int accno,int accno2, int amount);
void create_account();
void delete_account(int accno, int psswd);
void minist(int accno);
int comame(const void*a,const void*b);
int combal(const void*a,const void*b);
void sort();
void save();

int main() {
    int choice, accno,accno2,ans, pass,xoice;
    int found = 0;
    int dele = 0,sure=0;
    double amount;
    FILE*file=fopen("bank.dat","rb");
    if (file == NULL) {
        printf("New file has been created.\n");
        FILE *ghj=fopen("bank.dat","wb");
        fclose(ghj);
    }
    account acc;
    while(!feof(file)){
        if(fread(&acc, sizeof(struct account), 1, file)){
            if(acc.accnum>=anum){
                anum = acc.accnum+1;
            }
        }     
    }

while(1){
    printf("\n1.FACULTY\n"
       "2.CUSTOMER\n"
      "0.Exit\n");
    printf("\n\nEnter your Choice(0 - Exit): ");
    scanf("%d", &xoice);

    if(xoice==1){
        printf("\n1.CREATE ACCOUNT\n"
               "2.DELETE ACCOUNT\n3.SORTING (ACCOUNT)\n"
                "4.SORTING (NAME)\n5.SORTING (BALANCE)\n"
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
                printf("For conformation re-type PIN: ");scanf("%d",&sure);
                if(pass==sure){
                    dele=1;
                    delete_account(accno, pass);
                }else{
                    printf("No Account Deleted!!!");
                }

            } else if (choice == 3) {
                displall();
            } else if(choice == 4){
                sort(0);
            } else if (choice==5){
                sort(1);
            } else if (choice == 0) {
                if(dele==1)
                    save();
                break;
            } else {
                printf("\nInvalid Choice!!!\n");
            }
        }
    }
    else if(xoice==2){
        printf("Enter your Account Number:");scanf("%d",&accno);
        printf("Enter Your PIN Number:");scanf("%d",&pass);
        rewind(file);
        while(1){
            if(fread(&acc, sizeof(struct account), 1, file)!=1){
                break;
            }
            if(accno==acc.accnum && pass==acc.psswd){
                found = 1;
                printf("\n1.DISPLAY ACCOUNT DETAILS\n"
                       "2.DEPOSIT\n3.WITHDRAW\n"
                    "4.GET BALANCE\n5.MONEY TRANSFER\n"
                    "6.MINI-STATEMENT\n" 
                    "0.Exit\n");
                while (1) {
                    printf("\n\nEnter your Choice(0 - Exit): ");
                    scanf("%d", &choice);
                    if (choice == 1) {
                        display_account_details(accno);
                    } else if (choice == 2) {
                        printf("Enter the Deposit Amount: ");scanf("%lf", &amount);
                        printf("For conformation re-type PIN: ");scanf("%d",&sure);
                        if(pass==sure){
                            ans=deposit(accno,accno, amount);
                            if(ans==0)
                                printf("\nAmount Deposited successfully!!!\n");
                        }else{
                            printf("Money Deposition failed!!!");
                        }
                    } else if (choice == 3) {
                        printf("Enter the Withdraw Amount: ");scanf("%lf", &amount);
                        printf("For conformation re-type PIN: ");scanf("%d",&sure);
                        if(pass==sure){
                        ans=withdraw(accno,accno, amount);
                            if(ans==0)
                                printf("\nAmount Withdrawn successfully!!!\n");
                        }else{
                            printf("Money Withdrawn Failed!!!");
                        }
                    } else if (choice == 4) {
                        get_balance(accno);
                    } else if (choice == 5) {
                        printf("Enter the Amount to be transfered: ");scanf("%lf", &amount);
                        printf("Enter the Recipient ACC No: ");scanf("%d",&accno2);
                        printf("For conformation re-type PIN: ");scanf("%d",&sure);
                        if(pass==sure){
                            withdraw(accno,accno2, amount);
                            ans=deposit(accno2,accno,amount);
                            if(ans==0)
                                printf("\nMoney Transfered Successfully!!!\n");
                        }else{
                            printf("Money Transfer Failed!!!");
                        }
                    } 
                    else if(choice==6){
                        minist(accno);                        
                    }
                    else if (choice == 0) {
                        break;
                    } else {
                        printf("\nInvalid Choice!!!\n");
                    }
                }//while
            }//if
        }//while
        if (!found)
            printf("Wrong (Account or PIN) Number!!!\n"); 
        fclose(file);
    }//x2
    else if(xoice==0){
        printf("\nThank You!!!\n");
        break;
    }
    else {
        printf("\nInvalid Choice!!!\n");
    }

}
    return 0;
}


void create_account(){
    FILE *f=fopen("bank.dat","ab+");
    account newacc;
    printf("Enter your First Name: ");
    scanf("%s", newacc.fname);
    printf("Enter your Last Name: ");
    scanf("%s",newacc.lname);
    printf("Enter Your Mobile Number: ");
    scanf("%lf", &newacc.mob);
    printf("Create a 4-digit Pin: ");
    scanf("%d", &newacc.psswd);
    newacc.accnum = anum++;
    newacc.balc = 500;
    fwrite(&newacc, sizeof(struct account), 1, f);
    fclose(f);

    char filename[25];
    sprintf(filename,"trans_%d.txt",newacc.accnum);
    FILE *mi =fopen(filename,"w");
    fclose(mi);

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
            printf("Name: \t\t%s %s\n", acc.fname,acc.lname);
            printf("Mobile Number: %.0lf\n", acc.mob);
            printf("Balance: \t%0.2lf\n", acc.balc);
            break;
        }
    }
    fclose(f);
    if (!found)
        printf("Account not found.\n");
}

int withdraw(int accno,int accno2, int amount) {
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    int found = 0,pos=0;  
    char filename[25];
    sprintf(filename,"trans_%d.txt",accno);
    FILE *mi =fopen(filename,"a");
    if (f == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    while (1) {
        pos=ftell(f);
        if(fread(&acc, sizeof(struct account), 1, f)!=1){
            break;
        }
        if (acc.accnum == accno) {
            found = 1;
            if (acc.balc >= (double)amount) {
                acc.balc -= (double)amount;
                fseek(f, pos, SEEK_SET);
                fwrite(&acc, sizeof(struct account), 1, f);
                fprintf(mi,"%-10d %-10d %-7s %-15lf\n",acc.accnum,accno2,"DEBIT",(double)amount);
            }
            else {
                printf("Insufficient Balance!!!\n");
            }
        break;

        }
    }

    fclose(f);
    fclose(mi);
    if (!found)
        printf("Wrong Account Number!!!\n");
    return  0;
}

int deposit(int accno,int accno2, int amount) {
    FILE *f = fopen("bank.dat", "rb+");
    if (f == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    account acc;
    char filename[25];
    sprintf(filename,"trans_%d.txt",accno);
    FILE *mi =fopen(filename,"a");
    int found = 0;
    long pos;

    while (1) {
        pos = ftell(f); 
        if (fread(&acc, sizeof(struct account), 1, f)!=1) {
            break;
        }

        if (acc.accnum == accno) {
            found = 1;
            acc.balc += (double)amount;
            fseek(f, pos, SEEK_SET); 
            fwrite(&acc, sizeof(struct account), 1, f); 
            fprintf(mi,"%-10d %-10d %-7s %-15lf\n",acc.accnum,accno2,"CREDIT",(double)amount);
            break;       
        }
    }

    fclose(f);
    fclose(mi);
    if (!found)
        printf("Wrong Account Number!!!\n");
    return 0;
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
    char filename[25];
    sprintf(filename,"trans_%d.txt",accno);

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
                printf("\nAccount Deleted successfully!!!\n");
                remove(filename);
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


void displall(){
    FILE *f = fopen("bank.dat", "rb+");
    account acc;
    if (f==NULL){
        printf("Error opening in file!!!");
        return;
    }
while( fread(&acc, sizeof(struct account), 1, f)==1){
    if(acc.accnum!=0){
        printf("%d\t%s %s\t%.0lf\t%0.2lf\t%d\n", acc.accnum, acc.fname,acc.lname,acc.mob,acc.balc,acc.psswd);
    }
}
    fclose(f);
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
void minist(int accno) {
  int accno2;
  account acc;
  char filename[25];
  sprintf(filename, "trans_%d.txt", accno);
  FILE *mi = fopen(filename, "r+");

  if (mi == NULL) {
    printf("Error opening mini-statement file.\n");
    return;
  }

  fseek(mi, 0, SEEK_END);
  int pos = ftell(mi);
  //printf("%d",pos);
  if (pos == 0) {
    printf("No transactions found in mini-statement.\n");
    fclose(mi);
    return;
  }

  if (pos < (5 * 46)) {
    fseek(mi, 0, SEEK_SET);
  } else {
    pos -= (5 * 46);
    fseek(mi, pos, SEEK_SET);
  }
    printf("%-10s|%-10s|%-7s|%-15s\n","USER","BENEFICIER","STATUS","AMOUNT");
  while (1) {
    if (fscanf(mi, "%d %d %s %lf\n", &acc.accnum, &accno2, acc.fname, &acc.balc) != 4) {
      break;
    }
    printf("%-10d|%-10d|%-7s|%-15lf\n", acc.accnum, accno2, acc.fname, acc.balc);
  }
  fclose(mi);
}

int comame(const void *a, const void *b) {
    int val;
    const account *acc1 = (const account *)a;
    const account *acc2 = (const account *)b;
    val = strcmp(acc1->fname, acc2->fname);
    if(val == 0)
        val=strcmp(acc1->lname, acc2->lname);
    return val;
}

int combal(const void *a, const void *b) {
    const account *acc1 = (const account *)a;
    const account *acc2 = (const account *)b;

    if (acc1->balc > acc2->balc) return -1;
    if (acc1->balc < acc2->balc) return 1;
    return 0;
}

void sort(int choice) {
    FILE *f = fopen("bank.dat", "rb");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fseek(f, 0, SEEK_END);
    int num_accounts = ftell(f) / sizeof(struct account);
    rewind(f);

    account *acc = (account *)malloc(num_accounts * sizeof(account));
    if (acc == NULL) {
        printf("Memory allocation failed!\n");
        fclose(f);
        return;
    }

    fread(acc, sizeof(struct account), num_accounts, f);
    fclose(f);

    if (choice == 0) {
        qsort(acc, num_accounts, sizeof(struct account), comame);
    } else if (choice == 1) {
        qsort(acc, num_accounts, sizeof(struct account), combal);
    }

    printf("\nSorted Accounts:\n");
    for (int i = 0; i < num_accounts; i++) {
        if (acc[i].accnum != 0) {
            printf("%d\t%s %s\t%.0lf\t%0.2lf\t%d\n", acc[i].accnum, acc[i].fname,acc[i].lname, acc[i].mob, acc[i].balc, acc[i].psswd);
        }
    }

    free(acc);
}