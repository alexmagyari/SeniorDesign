/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 *                                     INCLUDES
 ******************************************************************************/
#include <string.h>
#include <stdint.h>
#include <ti/display/Display.h>
#include <ti/sap/snp.h>
#include <ti/sap/snp_rpc.h>
#include <ti/sap/sap.h>

/* Local Includes */
#include "simple_gatt_profile.h"
#include "profile_util.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
/*******************************************************************************
 *                                  GLOBAL TYPEDEFS
 ******************************************************************************/
/* Simple GATT Profile Service UUID: 0xFFF0 */
uint8_t simpleProfileServUUID[SNP_16BIT_UUID_SIZE] =
{
    LO_UINT16(SIMPLEPROFILE_SERV_UUID),
    HI_UINT16(SIMPLEPROFILE_SERV_UUID)
};

/* Characteristic 1 UUID: 0xFFF1 */
uint8_t simpleProfilechar1UUID[SNP_16BIT_UUID_SIZE] =
{
    LO_UINT16(SIMPLEPROFILE_CHAR1_UUID),
    HI_UINT16(SIMPLEPROFILE_CHAR1_UUID)
};

/* Characteristic 2 UUID: 0xFFF2 */
uint8_t simpleProfilechar2UUID[SNP_16BIT_UUID_SIZE] =
{
    LO_UINT16(SIMPLEPROFILE_CHAR2_UUID),
    HI_UINT16(SIMPLEPROFILE_CHAR2_UUID)
};

/* Characteristic 3 UUID: 0xFFF3 */
uint8_t simpleProfilechar3UUID[SNP_16BIT_UUID_SIZE] =
{
    LO_UINT16(SIMPLEPROFILE_CHAR3_UUID),
    HI_UINT16(SIMPLEPROFILE_CHAR3_UUID)
};

/* Characteristic 4 UUID: 0xFFF4 */
uint8_t simpleProfilechar4UUID[SNP_16BIT_UUID_SIZE] =
{
    LO_UINT16(SIMPLEPROFILE_CHAR4_UUID),
    HI_UINT16(SIMPLEPROFILE_CHAR4_UUID)
};

/*******************************************************************************
 *                             LOCAL VARIABLES
 ******************************************************************************/
static CharacteristicChangeCB_t simpleProfile_AppWriteCB = NULL;
static CCCDUpdateCB_t simpleProfile_AppCccdCB = NULL;
static uint8_t cccdFlag = 0;
static uint16_t connHandle = 0;
extern Display_Handle displayOut;
int count = 0;
/*******************************************************************************
 *                              Profile Attributes - TYPEDEFS
 ******************************************************************************/
SAP_Service_t simpleService;
SAP_CharHandle_t simpleServiceCharHandles[4];

static UUIDType_t simpleServiceUUID =
{ SNP_16BIT_UUID_SIZE, simpleProfileServUUID };

/* Characteristic 1 Value */
static uint8_t simpleProfileChar1 = 0;

/* Simple Profile Characteristic 1 User Description */
static uint8_t simpleProfileChar1UserDesp[17] = "Characteristic 1";

/* Characteristic 2 Value */
static uint8_t simpleProfileChar2 = 0;

/* Simple Profile Characteristic 2 User Description */
static uint8_t simpleProfileChar2UserDesp[17] = "Characteristic 2";

/* Characteristic 3 Value */
static uint8_t simpleProfileChar3 = 0;

/* Simple Profile Characteristic 3 User Description */
static uint8_t simpleProfileChar3UserDesp[17] = "Characteristic 3";

/* Characteristic 4 Value */
static uint8_t simpleProfileChar4 = 0;

/* Simple Profile Characteristic 4 User Description */
static uint8_t simpleProfileChar4UserDesp[17] = "Characteristic 4";

/*******************************************************************************
 *                              Profile Attributes - TABLE
 ******************************************************************************/
SAP_UserDescAttr_t char1UserDesc =
{
    SNP_GATT_PERMIT_READ,
    sizeof(simpleProfileChar1UserDesp),
    sizeof(simpleProfileChar1UserDesp),
    simpleProfileChar1UserDesp
};

SAP_UserDescAttr_t char2UserDesc =
{
    SNP_GATT_PERMIT_READ,
    sizeof(simpleProfileChar2UserDesp),
    sizeof(simpleProfileChar2UserDesp),
    simpleProfileChar2UserDesp
};

SAP_UserDescAttr_t char3UserDesc =
{
    SNP_GATT_PERMIT_READ,
    sizeof(simpleProfileChar3UserDesp),
    sizeof(simpleProfileChar3UserDesp),
    simpleProfileChar3UserDesp
};

SAP_UserDescAttr_t char4UserDesc =
{
    SNP_GATT_PERMIT_READ,
    sizeof(simpleProfileChar4UserDesp),
    sizeof(simpleProfileChar4UserDesp),
    simpleProfileChar4UserDesp
};

SAP_UserCCCDAttr_t char4CCCD =
{
    SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE
};

#define SERVAPP_NUM_ATTR_SUPPORTED 4

static SAP_Char_t simpleProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] =
{
    /* Characteristic 1 Value Declaration */
    {
        {SNP_16BIT_UUID_SIZE, simpleProfilechar1UUID }, /* UUID */
        SNP_GATT_PROP_READ | SNP_GATT_PROP_WRITE,       /* Properties */
        SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE,   /* Permissions */
        &char1UserDesc                                  /* User Description */
    },

    /* Characteristic 2 Value Declaration */
    {
        {SNP_16BIT_UUID_SIZE, simpleProfilechar2UUID }, /* UUID */
        SNP_GATT_PROP_READ,                             /* Properties */
        SNP_GATT_PERMIT_READ,                           /* Permissions */
        &char2UserDesc                                  /* User Description */
    },

    /* Characteristic 3 Value Declaration */
    {
        {SNP_16BIT_UUID_SIZE, simpleProfilechar3UUID }, /* UUID */
        SNP_GATT_PROP_WRITE,                            /* Properties */
        SNP_GATT_PERMIT_WRITE,                          /* Permissions */
        &char3UserDesc                                  /* User Description */
    },

    /* Characteristic 4 Value Declaration */
    {
        {SNP_16BIT_UUID_SIZE, simpleProfilechar4UUID }, /* UUID */
        SNP_GATT_PROP_NOTIFICATION,                     /* Properties */
        0,                                              /* Permissions */
        &char4UserDesc,                                 /* User Description */
        &char4CCCD                                      /* CCCD */
    },
};

/*******************************************************************************
 *                                  LOCAL FUNCTIONS
 ******************************************************************************/
uint8_t simpleProfile_ReadAttrCB(void *context,
        uint16_t connectionHandle, uint16_t charHdl, uint16_t offset,
        uint16_t size, uint16_t *len, uint8_t *pData);

uint8_t simpleProfile_WriteAttrCB(void *context, uint16_t connectionHandle,
        uint16_t charHdl, uint16_t len, uint8_t *pData);

uint8_t simpleProfile_CCCDIndCB(void *context, uint16_t connectionHandle,
        uint16_t cccdHdl, uint8_t type, uint16_t value);


/******************* Functions for Drone ************************/

typedef struct location{
    float lat;
    float longt;
    int hov;
} location;

location check[30];

void printdata(void){

    int i;
    for (i = 0; i < count; i++){

        Display_printf(displayOut, 0, 0, "%d location has latitude is: %.6f and longitude is %.6f and hover is %d\n", i+1, check[i].lat, check[i].longt, check[i].hov);

    }
}

double  mantissa(char *str){

    //printf("Input is %s has length is: %d\n", str, strlen(str));
    int x, i;
    double total = 0.0;
    for (i = 0; i < strlen(str); i++){
        x = str[i] - '0';
        //e = x*pow(2, -(i+1));
        total = total + x*pow(2, -(i+1));
    }

    return total;
}


double exponent(char *str) {

//  printf("\nInput is %s has length is: %d\n", str, strlen(str));
    int x, i;
    double total = 0.0;

//  for (i = 0; i < strlen(str); i++){
//      x = str[i] - '0';
//      if ( x == 1 || x == 0){
//          len = len + 1;
//      }
//  }
//
//  printf("Lenght is %d\n", len);
    for (i = 0; i < 8; i++){
        x = str[i] - '0';
        //e = x*pow(2, (8-i)-1);
        //printf("Test: %d and %f\n",x, e);
        total = total + x*pow(2, (8-i-1));
        //printf("Total is: %f\n", total);
    }


    return total;

}

float isfloat(char *str){
    //printf("\nInput is %s has length is: %d\n", str, strlen(str));
    char str3[8], str4[100];
    int sign;
    double expo, mat;
    float final;

    if (str[0] - '0' == 0){
        sign = 1;
    }
    else {
        sign = -1;
    }

    strncpy(str3, str + 1,8);
    strncpy(str4, str + 9,strlen(str));

    expo = exponent(str3);
    mat = mantissa(str4) + 1;
//  printf("\nSign: %d\n", sign);
//  printf("Exponent now is %f\n", expo);
//  printf("Mantissa now is %f\n", mat);
    final = sign * pow(2, expo - 127) * mat;
    //printf("Float in function is: %.4f\n", final);
    return final;

}


void ishexa1(char *str){
//    Display_printf(displayOut, 0, 0, "Is Hexa 1: %s\n",str);
    char example[100] = "";
    int i = 0;
//  while (str[i])
    for (i = 0; i < strlen(str); i++)
    {
        //printf("\nCase is %c: ", str[i]);
        switch (str[i])
        {

        case '0':
            //printf("0000");
            strcat(example, "0000");
//          printf("\nExample is: %s\n", example);
            break;
        case '1':
            //printf("0001");
            strcat(example, "0001");
//          printf("Example is: %s\n", example);
            break;
        case '2':
            //printf("0010");
            strcat(example, "0010");
//          printf("Example is: %s\n", example);
            break;
        case '3':
            //printf("0011");
            strcat(example, "0011");
//          printf("Example is: %s\n", example);
            break;
        case '4':
            //printf("0100");
            strcat(example, "0100");
            break;
        case '5':
            //printf("0101");
            strcat(example, "0101");
            break;
        case '6':
            //printf("0110");
            strcat(example, "0110");
            break;
        case '7':
            //printf("0111");
            strcat(example, "0111");
            break;
        case '8':
            //printf("1000");
            strcat(example, "1000");
            break;
        case '9':
            //printf("1001");
            strcat(example, "1001");
            break;
        case 'A':
            //printf("1010");
            strcat(example, "1010");
            break;
        case 'B':
            //printf("1011");
            strcat(example, "1011");
            break;
        case 'C':
            //printf("1100");
            strcat(example, "1100");
            break;
        case 'D':
            //printf("1101");
            strcat(example, "1101");
            break;
        case 'E':
            //printf("1110");
            strcat(example, "1110");
            break;
        case 'F':
            //printf("1111");
            strcat(example, "1111");
            break;
        case 'a':
            //printf("1010");
            strcat(example, "1010");
            break;
        case 'b':
            //printf("1011");
            strcat(example, "1011");
            break;
        case 'c':
            //printf("1100");
            strcat(example, "1100");
            break;
        case 'd':
            //printf("1101");
            strcat(example, "1101");
            break;
        case 'e':
            //printf("1110");
            strcat(example, "1110");
            break;
        case 'f':
            //printf("1111");
            strcat(example, "1111");
            break;
        default:
           // printf("\n Invalid hexa digit %c\n", str[i]);
//            return 0;
            break;
        }
//        i++;
    }

    //printf("\nInput first is: %s\n", example);
//    printf("%c", example[1]);
    float test;
    test = isfloat(example);
//  printf("Lat is: %.4f\n", test);

    check[count].lat = test;
   // Display_printf(displayOut, 0, 0, "Lat is: %.2f", test);


}

void ishexa2(char *str){
//    Display_printf(displayOut, 0, 0, "Is Hexa 2: %s\n",str);
    char example[100] = "";
    int i = 0;
//  while (str[i])
    for (i = 0; i < strlen(str); i++)
    {
        //printf("\nCase is %c: ", str[i]);
        switch (str[i])
        {

        case '0':
            //printf("0000");
            strcat(example, "0000");
//          printf("\nExample is: %s\n", example);
            break;
        case '1':
            //printf("0001");
            strcat(example, "0001");
//          printf("Example is: %s\n", example);
            break;
        case '2':
            //printf("0010");
            strcat(example, "0010");
//          printf("Example is: %s\n", example);
            break;
        case '3':
            //printf("0011");
            strcat(example, "0011");
//          printf("Example is: %s\n", example);
            break;
        case '4':
            //printf("0100");
            strcat(example, "0100");
            break;
        case '5':
            //printf("0101");
            strcat(example, "0101");
            break;
        case '6':
            //printf("0110");
            strcat(example, "0110");
            break;
        case '7':
            //printf("0111");
            strcat(example, "0111");
            break;
        case '8':
            //printf("1000");
            strcat(example, "1000");
            break;
        case '9':
            //printf("1001");
            strcat(example, "1001");
            break;
        case 'A':
            //printf("1010");
            strcat(example, "1010");
            break;
        case 'B':
            //printf("1011");
            strcat(example, "1011");
            break;
        case 'C':
            //printf("1100");
            strcat(example, "1100");
            break;
        case 'D':
            //printf("1101");
            strcat(example, "1101");
            break;
        case 'E':
            //printf("1110");
            strcat(example, "1110");
            break;
        case 'F':
            //printf("1111");
            strcat(example, "1111");
            break;
        case 'a':
            //printf("1010");
            strcat(example, "1010");
            break;
        case 'b':
            //printf("1011");
            strcat(example, "1011");
            break;
        case 'c':
            //printf("1100");
            strcat(example, "1100");
            break;
        case 'd':
            //printf("1101");
            strcat(example, "1101");
            break;
        case 'e':
            //printf("1110");
            strcat(example, "1110");
            break;
        case 'f':
            //printf("1111");
            strcat(example, "1111");
            break;
        default:
           // printf("\n Invalid hexa digit %c\n", str[i]);
//            return 0;
            break;
        }
//        i++;
    }

    //printf("\nInput first is: %s\n", example);
//    printf("%c", example[1]);
    float test;
    test = isfloat(example);
//  printf("Longt is: %.4f\n", test);

    check[count].longt = test;
   // Display_printf(displayOut, 0, 0, "Long is: %.4f", test);

}

void convert(char *hex){
    int i = 0, val, len;
    int decimal;

    decimal = 0;


    len = strlen(hex);
    len--;

    /*
     * Iterate over each hex digit
     */
    for(i=0; hex[i]!='\0'; i++)
    {

        /* Find the decimal representation of hex[i] */
        if(hex[i]>='0' && hex[i]<='9')
        {
            val = hex[i] - 48;
        }
        else if(hex[i]>='a' && hex[i]<='f')
        {
            val = hex[i] - 97 + 10;
        }
        else if(hex[i]>='A' && hex[i]<='F')
        {
            val = hex[i] - 65 + 10;
        }

        decimal += val * pow(16, len);
        len--;
    }

//    printf("Hexadecimal number = %s\n", hex);
//    printf("Hover = %d", decimal);
    check[count].hov = decimal;
   // Display_printf(displayOut, 0, 0, "Hov is: %d", decimal);

}

void btox(char *xp, const char *bb, int n)
{
    const char xx[]= "0123456789ABCDEF";
    while (--n >= 0) xp[n] = xx[(bb[n>>1] >> ((1 - (n&1)) << 2)) & 0xF];
}
/*******************************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   none
 *
 * @return  Success or Failure
 ******************************************************************************/
uint32_t SimpleProfile_AddService(void)
{
    /* Reads through table, adding attributes to the NP. */
    simpleService.serviceUUID = simpleServiceUUID;
    simpleService.serviceType = SNP_PRIMARY_SERVICE;
    simpleService.charTableLen = SERVAPP_NUM_ATTR_SUPPORTED;
    simpleService.charTable = simpleProfileAttrTbl;
    simpleService.context = NULL;
    simpleService.charReadCallback = simpleProfile_ReadAttrCB;
    simpleService.charWriteCallback = simpleProfile_WriteAttrCB;
    simpleService.cccdIndCallback = simpleProfile_CCCDIndCB;
    simpleService.charAttrHandles = simpleServiceCharHandles;

    /* Service is setup, register with GATT server on the SNP. */
    SAP_registerService(&simpleService);

    return BLE_PROFILE_SUCCESS;
}

/*******************************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  BLE_PROFILE_SUCCESS or BLE_PROFILE_ALREADY_IN_REQ_MODE
 ******************************************************************************/
uint32_t SimpleProfile_RegisterAppCB(BLEProfileCallbacks_t *callbacks)
{
    simpleProfile_AppWriteCB = callbacks->charChangeCB;
    simpleProfile_AppCccdCB = callbacks->cccdUpdateCB;

    return (BLE_PROFILE_SUCCESS);
}

/*******************************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  uint_fast8_t
 ******************************************************************************/
uint32_t SimpleProfile_SetParameter(uint8_t param, uint8_t len, void *value)
{
    uint_fast8_t ret = BLE_PROFILE_SUCCESS;
    switch (PROFILE_ID_CHAR(param))
    {
    case SP_CHAR1:
        if (len == sizeof(uint8_t))
        {
            simpleProfileChar1 = *((uint8*) value);
        } else
        {
            ret = BLE_PROFILE_INVALID_RANGE;
        }
        break;

    case SP_CHAR2:
        if (len == sizeof(uint8_t))
        {
            simpleProfileChar2 = *((uint8*) value);
        } else
        {
            ret = BLE_PROFILE_INVALID_RANGE;
        }
        break;

    case SP_CHAR3:
        if (len == sizeof(uint8_t))
        {
            simpleProfileChar3 = *((uint8*) value);
        } else
        {
            ret = BLE_PROFILE_INVALID_RANGE;
        }
        break;

    case SP_CHAR4:
        if (len == sizeof(uint8_t))
        {
            snpNotifIndReq_t localReq;
            simpleProfileChar4 = *((uint8*) value);

            /* Initialize Request */
            localReq.connHandle = connHandle;
            localReq.attrHandle = ProfileUtil_getHdlFromCharID(
                    PROFILE_ID_CREATE(SP_CHAR4, PROFILE_VALUE),
                    simpleServiceCharHandles, SERVAPP_NUM_ATTR_SUPPORTED);
            localReq.pData = (uint8_t *) &simpleProfileChar4;
            localReq.authenticate = 0;

            /* Check for whether a notification or indication should be sent.
               Both flags should never be allowed to be set by NWP */
            if (cccdFlag & SNP_GATT_CLIENT_CFG_NOTIFY)
            {
                localReq.type = SNP_SEND_NOTIFICATION;
                SNP_RPC_sendNotifInd(&localReq, sizeof(simpleProfileChar4));
            }
            else if (cccdFlag & SNP_GATT_CLIENT_CFG_INDICATE)
            {
                localReq.type = SNP_SEND_INDICATION;
                SNP_RPC_sendNotifInd(&localReq, sizeof(simpleProfileChar4));
            }
        } else
        {
            ret = BLE_PROFILE_INVALID_RANGE;
        }
        break;

    default:
        ret = BLE_PROFILE_INVALIDPARAMETER;
        break;
    }

    return (ret);
}

/*******************************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  uint_fast8_t
 ******************************************************************************/
uint32_t SimpleProfile_GetParameter(uint8_t param, void *value)
{
    uint_fast8_t ret = BLE_PROFILE_SUCCESS;
    switch (PROFILE_ID_CHAR(param))
    {
    case SP_CHAR1:
        *((uint8*) value) = simpleProfileChar1;
        break;

    case SP_CHAR2:
        *((uint8*) value) = simpleProfileChar2;
        break;

    case SP_CHAR3:
        *((uint8*) value) = simpleProfileChar3;
        break;

    case SP_CHAR4:
        *((uint8*) value) = simpleProfileChar4;
        break;

    default:
        ret = BLE_PROFILE_INVALIDPARAMETER;
        break;
    }

    return (ret);
}

/*******************************************************************************
 * @fn          simpleProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      BLE_PROFILE_SUCCESS, blePending or Failure
 ******************************************************************************/
uint8_t simpleProfile_ReadAttrCB(void *context, uint16_t connectionHandle,
        uint16_t charHdl, uint16_t offset, uint16_t size, uint16_t * pLen,
        uint8_t *pData)
{
    Display_printf(displayOut, 0, 0, "This is Read characteristic!");
    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(charHdl, simpleServiceCharHandles,
            SERVAPP_NUM_ATTR_SUPPORTED);
    uint8_t isValid = 0;

    /* Update connection handle (assumes one connection) */
    connHandle = connectionHandle;

    switch (PROFILE_ID_CHAR(charID))
    {
    case SP_CHAR1:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            *pLen = sizeof(simpleProfileChar1);
            memcpy(pData, &simpleProfileChar1, sizeof(simpleProfileChar1));
            isValid = 1;
            break;

        default:
            /* Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD */
            break;
        }
        break;
    case SP_CHAR2:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            *pLen = sizeof(simpleProfileChar2);
            memcpy(pData, &simpleProfileChar2, sizeof(simpleProfileChar2));
            isValid = 1;
            break;

        default:
            /* Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD */
            break;
        }
        break;
    default:
        /* Should not receive SP_CHAR3 || SP_CHAR4 reads */
        break;
    }

    if (isValid)
    {
        return (SNP_SUCCESS);
    }

    /* Unable to find handle - set len to 0 and return error code */
    *pLen = 0;
    return (SNP_UNKNOWN_ATTRIBUTE);
}

/*******************************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  BLE_PROFILE_SUCCESS, blePending or Failure
 ******************************************************************************/
int count2 = 1;
uint8_t simpleProfile_WriteAttrCB(void *context, uint16_t connectionHandle,
        uint16_t charHdl, uint16_t len, uint8_t *pData)
{
    //Display_printf(displayOut, 0, 0, "This is Write characteristic!");
    uint_fast8_t status = SNP_UNKNOWN_ATTRIBUTE;
    uint8_t notifyApp = PROFILE_UNKNOWN_CHAR;
    char *test;
    //char str1[8], str2[8], str3[4], str7[20];

    /* Update connection Handle (assumes one connection) */
    connHandle = connectionHandle;

    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(charHdl, simpleServiceCharHandles,
                            SERVAPP_NUM_ATTR_SUPPORTED);

    test = (char*) pData;

    //Display_printf(displayOut, 0, 0, "\nSizeof Char3 is %d\n", sizeof(pData));
    int n = sizeof pData << 1;
    char hexstr[n + 1];
    switch (PROFILE_ID_CHAR(charID))
    {
    case SP_CHAR1:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            test = (char*) pData;
            Display_printf(displayOut, 0, 0, "\nChar1 is %s\n", test);
            if (len == sizeof(simpleProfileChar1))
            {
                simpleProfileChar1 = pData[0];
                status = SNP_SUCCESS;
                notifyApp = SP_CHAR1_ID;
            }
            break;
        default:
            /* Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD */
            break;
        }
        break;
    case SP_CHAR3:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            test = (char*) pData;
            //Display_printf(displayOut, 0, 0, "\nChar3 is %s\n", test);

            btox(hexstr, test, n);
            //strcpy(hexstr, test);
            hexstr[n] = 0; /* Terminate! */
            Display_printf(displayOut, 0, 0,"Command: %s\n", hexstr);
//            Display_printf(displayOut, 0, 0,"Count2 Before: %d\n", count2);
//            Display_printf(displayOut, 0, 0,"Count: %d\n", count);
            if (count2 == 1){

                ishexa1(hexstr);
                count2 = count2 + 1;
            }

            else if (count2 == 2){
                ishexa2(hexstr);
                count2 = count2 + 1;
            }

            else if (count2 == 3){
                convert(hexstr);
                count2 = 1;
                count = count + 1;
                printdata();
            }

//            if (count2 == 3){
//                printdata();
//                count2 = 1;
//                count = count + 1;
//            }


           // Display_printf(displayOut, 0, 0,"Count2 After: %d\n", count2);




            if (len == sizeof(simpleProfileChar3))
            {
                simpleProfileChar3 = pData[0];
                status = SNP_SUCCESS;
                notifyApp = SP_CHAR3_ID;
            }
            break;
        default:
            /* Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD */
            break;
        }
        break;
    default:
        /* Should not receive SP_CHAR2 || SP_CHAR4 writes */
        break;
    }

    /* If a characteristic value changed then callback function to notify
     * application of change */
    if ((notifyApp != PROFILE_UNKNOWN_CHAR) && simpleProfile_AppWriteCB)
    {
        simpleProfile_AppWriteCB(notifyApp);
    }

    return (status);
}

/*******************************************************************************
 * @fn      simpleProfile_CCCDIndCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  BLE_PROFILE_SUCCESS, blePending or Failure
 ******************************************************************************/
uint8_t simpleProfile_CCCDIndCB(void *context, uint16_t connectionHandle,
        uint16_t cccdHdl, uint8_t type, uint16_t value)
{
    uint_fast8_t status = SNP_UNKNOWN_ATTRIBUTE;
    uint_fast8_t notifyApp = PROFILE_UNKNOWN_CHAR;

    /* Update connection handle (assumes one connection) */
    connHandle = connectionHandle;

    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(cccdHdl, simpleServiceCharHandles,
            SERVAPP_NUM_ATTR_SUPPORTED);

    switch (PROFILE_ID_CHAR(charID))
    {
    case SP_CHAR4:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_CCCD:
            /* Set Global cccd Flag which will be used to to gate Indications
               or notifications when SetParameter() is called */
            cccdFlag = value;
            notifyApp = charID;
            status = SNP_SUCCESS;
            break;
        default:
            /* Should not receive SP_VALUE || SP_USERDESC || SP_FORMAT */
            break;
        }
        break;
    default:
        /* No other Characteristics have CCCB attributes */
        break;
    }

    /* If a characteristic value changed then callback function to notify
     * application of change.
     */
    if ((notifyApp != PROFILE_UNKNOWN_CHAR) && simpleProfile_AppCccdCB)
    {
        simpleProfile_AppCccdCB(notifyApp, value);
    }

    return (status);
}
