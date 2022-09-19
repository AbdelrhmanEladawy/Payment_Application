/*
*   Author        : Abdelrhman Ahmed Ealdawy
*   Created       : 08/08/2022
*
*/

#include "app.h"

static service_t c = Welcome; //enum

void appStart(void)
{

    while (true)
    {
        if (c == Welcome)
        {
            system("cls"); //for windows only
            printf("\n********************************************************************************\n");
            printf("*************** \t Welcome,What service do you want ? \t ***************\n");
            printf("********************************************************************************\n");
            printf("1. withdraw \t\t 2. getTransaction \t\t 3. check balance \t\t 4. Exit \n");
            c = getchar();
        }

        else if (c == Withdraw)
        {
            system("cls"); //for windows only
            printf("\t\t............... getting card info ...............\t\t\n");
            c = getchar();
            ST_cardData_t card = { 0 };
            printf(" please enter you Name : ");
            while (true)
            {
                if (getCardHolderName(&card) == WRONG_NAME)
                    printf("WRONG NAME format, please re-enter card Name : ");
                else
                    break;
            }

            printf(" please enter card Expiry Date (MM/YY): ");
            while (true)
            {
                if (getCardExpiryDate(&card) == WRONG_EXP_DATE)
                    printf("WRONG EXP DATE format, please re-enter Date : ");
                else
                    break;
            }

            printf(" please enter card PAN : ");
            while (true)
            {
                if (getCardPAN(&card) == WRONG_PAN)
                    printf("WRONG PAN format, please re-enter PAN : ");
                else
                    break;
            }

            if (isValidCardPAN(&card) == INVALID_CARD)
            {
                printf("Declined INVALID CARD!!\n");
                c = getchar();
                c = Welcome;
                break;
            }

            printf("\n\t\t ............... sending data to terminal ............... \t\t\n");

            ST_terminalData_t term_data = { 0 };
            printf(" set Max terminal Transaction Amount : ");
            while (true)
            {
                if (setMaxAmount(&term_data) == INVALID_MAX_AMOUNT)
                    printf("INVALID MAX AMOUNT, please re-enter it : ");
                else
                    break;
            }

            if (getTransactionDate(&term_data) == WRONG_DATE)
                printf("WRONG SYSTEM DATE, there are error in the system ...");

            if (isCardExpired(card, term_data) == EXPIRED_CARD)
            {
                printf("Declined EXPIRED CARD!!\n");
                c = getchar(); c = getchar();
                c = Welcome;
                break;
            }

            while (true)
            {
                if (getTransactionAmount(&term_data) == INVALID_AMOUNT)
                    printf("INVALID AMOUNT enter, please re-enter it : \n");
                else
                    break;
            }

            if (isBelowMaxAmount(&term_data) == EXCEED_MAX_AMOUNT)
            {
                printf("Declined Amount Exceeding Limit!!\n");
                c = getchar(); c = getchar();
                c = Welcome;
                break;
            }

            printf("\n\t\t ............... processing data in the server ............... \t\t\n");

            ST_transaction_t trans = { card , term_data };
            trans.transState = recieveTransactionData(&trans);

            switch (trans.transState)
            {
            case DECLINED_STOLEN_CARD:
                printf("DECLINED_STOLEN_CARD!\n");
                c = getchar(); c = getchar();
                c = Welcome;
                return;
                break;
            case DECLINED_INSUFFECIENT_FUND:
                printf("DECLINED_INSUFFECIENT_FUND\n");
                c = getchar(); c = getchar();
                c = Welcome;
                return;
                break;
            case INTERNAL_SERVER_ERROR:
                printf("INTERNAL_SERVER_ERROR\n");
                c = getchar(); c = getchar();
                c = Welcome;
                return;
                break;
            case APPROVED:
                printf("APPROVED\n");
                printf("Transaction number : %d\n", trans.transactionSequenceNumber);
                printf("click any key to return back to main screen ...");
                c = getchar(); c= getchar();
                c = Welcome;
                break;
            default:
                printf("error in the system ...\n");
                c = getchar(); c = getchar();
                c = Exit;
                break;
            }
        }

        else if (c == GetTransaction)
        {
            system("cls"); //for windows only
            uint32_t TSN = 0;
            printf("please enter Transaction Sequence Number : ");
            scanf_s("%ud", &TSN);
            ST_transaction_t trans = { 0 };
            if (getTransaction(TSN, &trans) == OK)
            {
                printf("\ttransaction found\t\n");
                printf("Name : %s\n",trans.cardHolderData.cardHolderName);
                printf("Transaction done on : %s\n", trans.terminalData.transactionDate);
                printf("Amount deducted : %f \n", trans.terminalData.transAmount);
                printf("click any key to return back to main screen ...");
                c = getchar();
                c = getchar();
                c = Welcome;
            }

            else
            {
                printf("sorry, transaction not found");
                printf("click any key to return back to main screen ...");
                c = getchar();
                c = getchar();
                c = Welcome;
            }
        }

        else if (c == CheckBalance)
        {
            system("cls"); //for windows only
            ST_cardData_t PAN = {0};
            printf("please enter the Primary Account Number : ");
            c = getchar();
            
            if (getCardPAN(&PAN) == WRONG_PAN )
            {
                printf("sorry, wrong PAN number entered\n");
                printf("click any key to return back to main screen ...");
                c = getchar();
                c = Welcome;
                break;
            }
            
            int8_t index = check_balance(PAN);
            (index == -1) ? (printf("sorry, account not found..\n")) : ( printf("you balance is : %f\n", accounts_database[index].balance));

            printf("click any key to return back to main screen ...");
            c = getchar();
            c = Welcome;
        }

        else if (c == Exit)
        {
            printf("thank you!\n");
            return;
        }

        else
        {
            printf("wrong input !! re-enter it :- ");
            c = getchar();
        }
    }
}

int main()
{
    srand(time(0));
    uint8_t i;

    //two accounts for test
    ST_cardData_t card1 = { "Abderlrhman Ahmed eladwy" , "2774393028816928" ,"10/24"};
    ST_cardData_t card2 = { "Mohamed Ahmed eladawy" , "6420779400743386" ,"07/21" };
    ForTEST_add_server_account(card1 , 7500.00);
    ForTEST_add_server_account(card2 , 4500.00);

    //add random server account in database
    for (i = 0; i < 100; i++)
        get_random_server_account();    

    c = Welcome; //start with welcome meesage
    system("cls"); //for windows only...
    while (c != Exit)
    {
        appStart();
    }
    return 0;
}