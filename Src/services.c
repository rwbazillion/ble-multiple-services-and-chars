/*
Library:			Services.c
Written by:			Ronald Bazillion
Date written:		1/15/2020
Description:

Resources:

UUID generator - https://www.uuidgenerator.net/

Modifications:
	1/15/2020		Ronald Bazillion		First Draft
-----------------
*/

//***** Header files *****//
#include "bluenrg_gap.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "services.h"
#include <stdlib.h>

//***** typedefs enums *****//


//***** typedefs structs *****//


//***** static Library variables *****//
//****Remember "static" is used like "private" in C++*******//


//*********static function prototypes***********************//
//****Remember "static" is used like "private" in C++*******//

//***** API Functions definition *****//
//****Remember "API" is used like "public" in C++*******//

uint32_t connected = FALSE;
uint8_t set_connectable = 1;
uint16_t connection_handle = 0;
uint8_t notification_enabled = FALSE;

//Observe the 4th byte if different in all UUIDs

//d973f2e0-b19e-11e2-9e96-0800200c9a66
//Health Service and characteristic UUIDs
const uint8_t health_service_uuid[16]  = {0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe0, 0xf2, 0x73, 0xd9};
const uint8_t bpm_char_uuid[16]        = {0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe1, 0xf2, 0x73, 0xd9};
const uint8_t weight_char_uuid[16]     = {0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe2, 0xf2, 0x73, 0xd9};

//83a78570-6076-b885-a146-6986-fac12592
//Weather Service and characteristics UUIDs
const uint8_t weather_service_uuid[16] = {0x92, 0x25, 0xc1, 0xfa, 0x86, 0x69, 0x46, 0xa1, 0x85, 0xb8, 0x76, 0x60, 0x70, 0x85, 0xa7, 0x83};
const uint8_t temp_char_uuid[16]       = {0x92, 0x25, 0xc1, 0xfa, 0x86, 0x69, 0x46, 0xa1, 0x85, 0xb8, 0x76, 0x60, 0x71, 0x85, 0xa7, 0x83};
const uint8_t hum_char_uuid[16]        = {0x92, 0x25, 0xc1, 0xfa, 0x86, 0x69, 0x46, 0xa1, 0x85, 0xb8, 0x76, 0x60, 0x72, 0x85, 0xa7, 0x83};
const uint8_t temp_desc_uuid[2]        = {0x92, 0x11};

uint16_t healthServHandle;
uint16_t bpmCharHandle;
uint16_t weightCharHandle;

uint16_t weatherServHandle;
uint16_t tempCharHandle;
uint16_t humCharHandle;

uint16_t tempDescHandle;

tBleStatus addServices(void)
{
	tBleStatus ret;

	charactFormat charFormat;

	//Add service
	aci_gatt_add_serv(UUID_TYPE_128, health_service_uuid, PRIMARY_SERVICE, 7, &healthServHandle);
	aci_gatt_add_serv(UUID_TYPE_128, weather_service_uuid, PRIMARY_SERVICE, 7, &weatherServHandle);

	//Add characteristics for weather service
	ret = aci_gatt_add_char(weatherServHandle, UUID_TYPE_128, temp_char_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &tempCharHandle);
	ret = aci_gatt_add_char(weatherServHandle, UUID_TYPE_128, hum_char_uuid , 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &humCharHandle);

	charFormat.format = FORMAT_SINT16;
	charFormat.exp = -1;
	charFormat.unit = UNIT_TEMP_CELSIUS;
	charFormat.name_space = 0;
	charFormat.desc = 0;

	//Add Descriptor
	aci_gatt_add_char_desc(weatherServHandle, tempCharHandle, UUID_TYPE_16, (uint8_t*)temp_desc_uuid, 7, 7, (void*)&charFormat, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY, 0, 16, FALSE, &tempDescHandle);

	//Add characteristics for health service
	ret = aci_gatt_add_char(healthServHandle, UUID_TYPE_128, bpm_char_uuid,    2, CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &bpmCharHandle);
	ret = aci_gatt_add_char(healthServHandle, UUID_TYPE_128, weight_char_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &weightCharHandle);

	return ret;
}

//update bpm data
void updateBpmData(uint16_t new_data)
{
	aci_gatt_update_char_value(healthServHandle, bpmCharHandle, 0, 2, (uint8_t*)&new_data);
}

//update weight data
void updateWeightData(uint16_t new_data)
{
	aci_gatt_update_char_value(healthServHandle, weightCharHandle, 0, 2, (uint8_t*)&new_data);
}

//update humidity data
void updateHumData(uint16_t new_data)
{
	aci_gatt_update_char_value(weatherServHandle, humCharHandle, 0, 2, (uint8_t*)&new_data);
}

//update temperature data
void updateTempData(uint16_t new_data)
{
	aci_gatt_update_char_value(weatherServHandle, tempCharHandle, 0, 2, (uint8_t*)&new_data);
}

void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle)
{
	connected = TRUE;
	connection_handle = handle;
}

void GAP_DisconectionComplete_CB(void)
{
	//You can initialize a GPIO Pin to blink

}

//Temporary values
int bpm = 85;
int weight = 90;
int temp = 20;
int hum = 80;

void Read_Request_CB(uint16_t handle)
{
	if (handle == bpmCharHandle + 1)
	{
		bpm +=2;
		updateBpmData(bpm);
	}
	if (handle == weightCharHandle + 1)
	{
		weight-=5;
		updateWeightData(weight);
	}
	if (handle == tempCharHandle + 1)
	{
		temp-=1;
		updateTempData(temp);
	}
	if(handle == humCharHandle + 1)
	{
		hum+=2;
		updateHumData(hum);
	}

	if (connection_handle != 0)
	{
		aci_gatt_allow_read(connection_handle);
	}
}

//This is a state machine that when data is presented as an argument we check the event
//and call the correct callback function.
void user_notify(void* pData)
{
	hci_uart_pckt* hci_pckt = pData;

	hci_event_pckt* event_pckt = (hci_event_pckt*)hci_pckt->data;

	if(hci_pckt->type != HCI_EVENT_PKT)
		return;

	switch(event_pckt->evt)
	{
		case EVT_DISCONN_COMPLETE:
		{
			GAP_DisconectionComplete_CB();
		}
		break;
		case EVT_LE_META_EVENT:
		{
			evt_le_meta_event* evt = (void*)event_pckt->data;
			switch(evt->subevent)
			{
				case EVT_LE_CONN_COMPLETE:
				{
					evt_le_connection_complete* cc = (void*)evt->data;
					GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
				}
				break;
			}
		}
		break;
		case EVT_VENDOR:
		{
			evt_blue_aci* blue_evt = (void *) event_pckt->data;
			switch(blue_evt->ecode)
			{
				case EVT_BLUE_GATT_READ_PERMIT_REQ:
				{
					evt_gatt_read_permit_req* pr = (void *) blue_evt->data;
					Read_Request_CB(pr->attr_handle);
				}
				break;
			}
		}
		break;
	}
}


//************************************************** STATIC FUNCTIONS ***********************************************
//*******************************************************************************************************************
//*******************************************************************************************************************
