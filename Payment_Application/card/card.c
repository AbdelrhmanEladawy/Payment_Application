/*
*   Author        : Abdelrhman Ahmed Ealdawy
*   Created       : 08/08/2022
*
*/

#include "card.h"
#include <string.h>

//buffer to get input and then copy it 
static uint8_t buffer[40]="";

//function to check if name input is correct or not, capital or small letters and spaces only
bool_t CheckNameFormat(uint8_t* CardName)
{
    uint8_t i; //for loop
    for (i = 0; i < strlen(CardName); i++)
    {   //if the input is not character
        if ( !( (CardName[i] >= 'A' && CardName[i]<='Z' ) || (CardName[i] >= 'a'-1 && CardName[i]<='z'-1 ) || (CardName[i] == ' ')|| (CardName[i] == '\n')) )
            return false;
    }

    return true;
}

//function to check if date input is correct or not
bool_t CheckDateFormat(uint8_t* Date)
{
    if (Date[0] != '0' && Date[0] != '1') //the first number is month's teens. so, it is always  0 or 1
        return false;

    else if (Date[0] == '0')
    {
        if (Date[1] < '1' || Date[1] > '9') //if the first digit is 0, so next number should be from 1 -> 9
            return false;
    }

    else if (Date[0] == '1') //if the first digit is 1, so next number should be 0 to 2
    {
        if (Date[1] < '0' || Date[1] > '2')
            return false;
    }

    if (Date[2] != '/') //the second digit always '/'
        return false;
    if (Date[3] < '0' || Date[3] > '9') //the year can be in range 0 -> 9
        return false;
    if (Date[4] < '0' || Date[4] > '9')
        return false;

    return true;
}

//function to check if PAN number input is correct or not
bool_t CheckPANFormat(uint8_t* CardPAN)
{
    uint8_t i; //for loop
    for (i = 0; i < strlen(CardPAN); i++)
    {
        if ( CardPAN[i] < '0' || CardPAN[i] > '9' ) //if it's not a digit 
            return false;
    }

    return true;
}

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    fflush(stdin); //clear input buffer
    gets(buffer); //get the input in the buffer then copy it
    strcpy_s(cardData->cardHolderName, sizeof(cardData->cardHolderName) / sizeof(cardData->cardHolderName[0]), buffer);
    
    //if input is NULL , or length is not in the range or wrong format
    if (cardData->cardHolderName == NULL || strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24
        || !CheckNameFormat(cardData->cardHolderName))
        return WRONG_NAME;
    else
        return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    fflush(stdin); //clear input buffer
    gets(buffer); //get the input in the buffer then copy it
    strcpy_s(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate) / sizeof(cardData->cardExpirationDate[0]), buffer);
    //if input is NULL , or length is not in the range or wrong format
    if (strlen(cardData->cardExpirationDate) != 5 || !CheckDateFormat(cardData->cardExpirationDate))
        return WRONG_EXP_DATE;
    else
        return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    gets(buffer); //get the input in the buffer then copy it
    strcpy_s(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber) / sizeof(cardData->primaryAccountNumber[0]), buffer);

    //initialized unused bit to 0 
    for (int i = strlen(cardData->primaryAccountNumber)+1 ; i < 20; i++)
        cardData->primaryAccountNumber[i] = '0'; //convert it to char
    
    //if input is NULL , or length is not in the range or wrong format
    if (cardData->primaryAccountNumber == NULL || strlen(cardData->primaryAccountNumber) < 16
        || strlen(cardData->primaryAccountNumber) > 19 || !CheckPANFormat(cardData->primaryAccountNumber))
        return WRONG_PAN;
    else
        return OK;
}