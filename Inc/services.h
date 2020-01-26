/*
Library:			Services.h
Written by:			Ronald Bazillion
Date written:		1/15/2020
Description:

Resources:

Modifications:
	1/15/2020		Ronald Bazillion		First Draft
-----------------
*/

#ifndef __SERVICES_H_
#define __SERVICES_H_

//Header files
#include "bluenrg_aci_const.h"

//*******Defines***************//

//*******Enums*****************//


//*******Flags****************//


//***** Constant variables and typedefs *****//

//*******Structures****************//

//***** Global Variables ************//


//***** API Functions prototype *****//

tBleStatus addServices(void);
void user_notify(void* pData);

#endif
