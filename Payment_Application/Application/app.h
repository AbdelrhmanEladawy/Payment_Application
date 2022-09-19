/*
*   Author        : Abdelrhman Ahmed Ealdawy
*   Created       : 08/08/2022
*   last modfiled :
*
*/

#ifndef _APP
#define _APP

#include "../Server/server.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"

typedef enum service_t
{
	Withdraw = '1', GetTransaction ,CheckBalance, Exit ,Welcome, Approve = '1'
}service_t;

void appStart(void);

#endif/* _APP.H */