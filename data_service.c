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
 *                                INCLUDES
 ******************************************************************************/
#include <ti/display/Display.h>
#include <ti/sap/snp.h>
#include <ti/sap/snp_rpc.h>
#include <ti/sap/snp_rpc_synchro.h>
#include <ti/sap/sap.h>
#include "profile_util.h"
#include "data_service.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

/*******************************************************************************
 *                                   MACROS
 ******************************************************************************/
#define DATA_NUM_ATTR_SUPPORTED 2

/*******************************************************************************
 *                                  GLOBAL TYPEDEFS
 ******************************************************************************/
/* Data_Service Service UUID */
static uint8_t DataServiceUUID[SNP_128BIT_UUID_SIZE] =
 { TI_BASE_UUID_128(DATA_SERVICE_UUID) };

/* String UUID */
static uint8_t ds_StringUUID[SNP_128BIT_UUID_SIZE] =
 { TI_BASE_UUID_128(DS_STRING_UUID) };

/* Stream UUID */
static uint8_t ds_StreamUUID[SNP_128BIT_UUID_SIZE] =
 { TI_BASE_UUID_128(DS_STREAM_UUID) };

/*******************************************************************************
 *                             VARIABLES
 ******************************************************************************/
static BLEProfileCallbacks_t *dataServiceCallback;
static uint8_t cccdFlag = 0;
static uint16_t connHandle = 0;
int count = 0;
/* Used for log messages */
extern Display_Handle displayOut;

/*******************************************************************************
 *                              Profile Attributes - TYPEDEFS
 ******************************************************************************/
static SAP_Service_t    DataService;
static SAP_CharHandle_t DataServiceCharHandles[DATA_NUM_ATTR_SUPPORTED];

/* Profile Service attribute */
static UUIDType_t DataServiceDecl =
{ SNP_128BIT_UUID_SIZE, DataServiceUUID };

/* Characteristic "String" Value variable */
static uint8_t ds_StringVal[DS_STRING_LEN] = {0};

/* Length of data in characteristic "String" Value variable,
 * initialized to minimal size.
 */
static uint16_t ds_StringValLen = DS_STRING_LEN_MIN;

/* Characteristic "Stream" Value variable */
static uint8_t ds_StreamVal[DS_STREAM_LEN] = {0};

/* Length of data in characteristic "Stream" Value variable,
 * initialized to minimal size.
 */
static uint16_t ds_StreamValLen = DS_STREAM_LEN_MIN;

/*******************************************************************************
 *                              Profile Attributes - TABLE
 ******************************************************************************/
static SAP_UserCCCDAttr_t DataCCCD =
{ SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE };

static SAP_Char_t DataAttrTable[DATA_NUM_ATTR_SUPPORTED] =
{
    /* String Characteristic */
    {
        { SNP_128BIT_UUID_SIZE, ds_StringUUID },        /* UUID */
        SNP_GATT_PROP_READ | SNP_GATT_PROP_WRITE,       /* Properties */
        SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE,   /* Permissions */
    },

    /* Stream Characteristic */
    {
        { SNP_128BIT_UUID_SIZE, ds_StreamUUID },        /* UUID */
        SNP_GATT_PROP_NOTIFICATION | SNP_GATT_PROP_WRITE_NORSP, /* Properties */
        SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE,          /* Permissions */
        0,                                              /* User Description */
        &DataCCCD                                       /* CCCD */
    },
};

/*******************************************************************************
 *                                  LOCAL FUNCTIONS
 ******************************************************************************/
static void Data_processSNPEventCB(uint16_t event, snpEventParam_t *param);
static uint8_t DataService_ReadAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t offset, uint16_t size, uint16_t * pLen,
        uint8_t *pData);
static uint8_t DataService_WriteAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t len, uint8_t *pData);
static uint8_t DataService_CCCDIndCB(void *context, uint16_t conn,
        uint16_t cccdHdl, uint8_t type, uint16_t value);

/******************* Functions for Drone ******************************************/

//void delay(int t){
//    int x, y;
//
//    for (x=0; x<t; x++){
//        for (y=0; y<7900; y++){
//
//        }
//    }
//
//}
//
typedef struct location{
    float lat;
    float longt;
    int hov;
} location;

location check[30];
//
//void getfloat(char *array){
//    char* pend;
//
//    // f1 variable to store float value
//    float f1 = strtof(array, &pend);
//    check[count].lat = f1;
//
//    // f2 variable to store float value
//    float f2 = strtof(pend, NULL);
//    check[count].longt = f2;
//
//    count = count + 1;
//}
//
void printdata(void){

    int i;
    for (i = 0; i < count; i++){

        Display_printf(displayOut, 0, 0, "%d location has latitude is: %.6f and longitude is %.6f and hover is %d\n", i+1, check[i].lat, check[i].longt, check[i].hov);

    }
}
//
//void flying(int arr[], int num){
//    int i;
//    for (i = 0; i < num; i++){
//        switch (arr[i]){
//            case 0:
//                Display_printf(displayOut, 0, 0, "Keep going straight \n");
//                delay(450);
//                break;
//            case 1:
//                Display_printf(displayOut, 0, 0, "Going north west\n");
//                delay(450);
//                break;
//            case 2:
//                Display_printf(displayOut, 0, 0, "Going north\n");
//                delay(450);
//                break;
//            case 3:
//                Display_printf(displayOut, 0, 0, "Going north east\n");
//                delay(450);
//                break;
//            case 4:
//                Display_printf(displayOut, 0, 0, "Turning left\n");
//                delay(450);
//                break;
//            case 5:
//                Display_printf(displayOut, 0, 0, "Turning right\n");
//                delay(450);
//                break;
//            case 6:
//                Display_printf(displayOut, 0, 0, "Going south west\n");
//                delay(450);
//                break;
//            case 7:
//                Display_printf(displayOut, 0, 0, "Going south\n");
//                delay(450);
//                break;
//            case 8:
//                Display_printf(displayOut, 0, 0, "Going south east\n");
//                delay(450);
//                break;
//            default:
//                Display_printf(displayOut, 0, 0, "No command\n");
//                break;
//        }
//
//
//    }
//
//}
//
//void nonumber(char *test3, uint16_t num){
//
//    char test2[] = " ";
//    char test[] = " ";
//    char test4[] = " ";
//    strncpy(test, test3, num);
//    int i, c = 0;
//    //printf("String is: %s \n", test);
//    //Display_printf(displayOut, 0, 0, "From User: %s", test);
//    for (i = 0; i < strlen(test); i++ ){
//
//        if (test[i] >= '0' && test[i] <= '9'){
//            //Display_printf(displayOut, 0, 0, "%c", test[i]);
//            test2[c] = test[i];
//            c = c + 1;
////          strncat(test2, &test[i], 1);
//        }
//
//    }
//
//    //Display_printf(displayOut, 0, 0, "Final String: %s has length is: %d\n", test2, c);
//    strncpy(test4, test2, c);
//    int final[c];
//    for (i = 0; i < c; i++){
//        final[i] = test4[i] - '0';
//    }
//
//    //Display_printf(displayOut, 0, 0, "\nFinal string 3 is: %s has length is: %d\n", test4, c);
//    Display_printf(displayOut, 0, 0, "\nCommand: ");
//    for (i = 0; i < c; i++){
//        Display_printf(displayOut, 0, i, "%d ", final[i]);
//    }
//    flying(final, c);
//}
//
//void select(char *test, int num){
//
//    Display_printf(displayOut, 0, 0, "===***===");
//    Display_printf(displayOut, 0, 0, "Input from user: %s", test);
//
//    char str1[100];
//    char str2[100];
//
//    strncpy(str1,test,sizeof(str1)-1);
//
//    //Display_printf(displayOut, 0, 0, "Converting %s\n", str1);
//
//    strncpy (str2, str1+1, strlen(str1)-1);
//
//    //Display_printf(displayOut, 0, 0, "After deleting %s\n", str2);
//    //Display_printf(displayOut, 0, 0, "***\n");
//
//    if (test[0] == '1'){
//        Display_printf(displayOut, 0, 0, "You choose command\n");
//        nonumber(str2, num);
//    }
//
//    else if (test[0] == '2'){
//        Display_printf(displayOut, 0, 0, "You choose add GPS location\n");
//        getfloat(str2);
//    }
//
//    else if (test[0] == '3'){
//        Display_printf(displayOut, 0, 0, "You choose print locations\n");
//        printdata();
//    }
//}



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
//    Display_printf(displayOut, 0, 0, "Lat is: %.2f", test);


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
//    Display_printf(displayOut, 0, 0, "Long is: %.4f", test);

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
//    Display_printf(displayOut, 0, 0, "Hov is: %d", decimal);

}


/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 * @fn      DataService_addService
 *
 * @brief   Initializes the DataService service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 ******************************************************************************/
uint8_t DataService_addService(void)
{
    /* Register to receive Connection Established Events */
    SAP_registerEventCB(Data_processSNPEventCB,
            SNP_CONN_EST_EVT | SNP_CONN_TERM_EVT);

    /* Build Service to register with NP */
    DataService.serviceUUID = DataServiceDecl;
    DataService.serviceType = SNP_PRIMARY_SERVICE;
    DataService.charTableLen = DATA_NUM_ATTR_SUPPORTED;
    DataService.charTable = DataAttrTable;
    DataService.context = NULL;
    DataService.charReadCallback = DataService_ReadAttrCB;
    DataService.charWriteCallback = DataService_WriteAttrCB;
    DataService.cccdIndCallback = DataService_CCCDIndCB;
    DataService.charAttrHandles = DataServiceCharHandles;

//    Display_print1(displayOut, 0, 0,
//            "DataService: Registered service, %d attributes",
//            DATA_NUM_ATTR_SUPPORTED);

    /* Service is set up, register with GATT server on the SNP. */
    return SAP_registerService(&DataService);
}

/*******************************************************************************
 * @fn      DataService_registerAppCBs
 *
 * @brief   Registers the application callback function. Only call
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  BLE_PROFILE_SUCCESS or BLE_PROFILE_ALREADY_IN_REQ_MODE
 ******************************************************************************/
uint8_t DataService_registerAppCBs(BLEProfileCallbacks_t *appCallbacks)
{
    if (dataServiceCallback == NULL)
    {
        if (appCallbacks != NULL)
        {
            dataServiceCallback = appCallbacks;
//          Display_print0(displayOut,0,0,
//                  "DataService: Registered callbacks to application.");
        }
        else
        {
//            Display_print0(displayOut,0,0,
//                    "DataService: nullptr given for application callbacks.");
          return ( BLE_PROFILE_FAILURE );
        }
        return (BLE_PROFILE_SUCCESS);
    }

    return (BLE_PROFILE_ALREADY_IN_REQ_MODE);
}

/*******************************************************************************
 * @fn      DataService_setParameter
 *
 * @brief   Set a DataService parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  uint8_t
 ******************************************************************************/
uint8_t DataService_setParameter(uint8_t param, uint16_t len, void *value )
{
    uint8_t ret = BLE_PROFILE_SUCCESS;
    uint8_t  *pAttrVal;
    uint16_t *pValLen;
    uint16_t valMinLen;
    uint16_t valMaxLen;
    uint8_t   sendNotiInd = false;
    snpNotifIndReq_t localReq;

    switch (PROFILE_ID_CHAR(param))
    {
    case DATA_STRING_CHAR0:
        pAttrVal  =  ds_StringVal;
        pValLen   = &ds_StringValLen;
        valMinLen =  DS_STRING_LEN_MIN;
        valMaxLen =  DS_STRING_LEN;
        break;

    case DATA_STREAM_CHAR1:
        pAttrVal  =  ds_StreamVal;
        pValLen   = &ds_StreamValLen;
        valMinLen =  DS_STREAM_LEN_MIN;
        valMaxLen =  DS_STREAM_LEN;
        sendNotiInd = true;
        localReq.attrHandle = ProfileUtil_getHdlFromCharID(DS_STREAM_ID,
                        DataServiceCharHandles,
                        DATA_NUM_ATTR_SUPPORTED);
        break;

    default:
        ret = BLE_PROFILE_INVALIDPARAMETER;
        break;
    }

    /* Check bounds, update value and send notification or indication
     * if possible.
     */
    if ( len <= valMaxLen && len >= valMinLen )
    {
      memcpy(pAttrVal, value, len);
      *pValLen = len;

      if (sendNotiInd)
      {
        /* Initialize Request */
        localReq.connHandle = connHandle;
        localReq.pData = pAttrVal;
        localReq.authenticate = 0;

        /* Check for whether a notification or indication should be sent.
           Both flags should never be allowed to be set by NWP */
        if (cccdFlag & SNP_GATT_CLIENT_CFG_NOTIFY)
        {
           localReq.type = SNP_SEND_NOTIFICATION;
           SNP_RPC_sendNotifInd(&localReq, len);
         }
        else if (cccdFlag & SNP_GATT_CLIENT_CFG_INDICATE)
         {
           localReq.type = SNP_SEND_INDICATION;
           SNP_RPC_sendNotifInd(&localReq, len);
         }
      }
    }
    else
    {
        ret = BLE_PROFILE_INVALID_RANGE;
    }

    return ret;
}

/*******************************************************************************
 * @fn      DataService_getParameter
 *
 * @brief   Get a DataService parameter
 *
 * @param   param - Profile parameter IDz
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  uint8_t
 ******************************************************************************/
uint8_t DataService_getParameter(uint8_t param, void *value )
{
    uint8_t ret = BLE_PROFILE_SUCCESS;
    switch (PROFILE_ID_CHAR(param))
    {
      case DATA_STRING_CHAR0:
        memcpy(value, ds_StringVal, ds_StringValLen);
        break;

      case DS_STREAM_ID:
        memcpy(value, ds_StreamVal, ds_StreamValLen);
        break;

      default:
        ret = BLE_PROFILE_INVALIDPARAMETER;
        break;
  }
  return ret;
}

/******************************************************************************
 * @fn      DataService_ReadAttrCB
 *
 * @brief   Read an attribute.
 *
 * @param   context - context used when registering service
 * @param   conn    - connection handle ID
 * @param   charHdl - characteristic value handle
 * @param   offset  - offset of data to be read
 * @param   size    - maximum size of data bytes to be read
 * @param   pLen    - amount of bytes copied into pData
 * @param   pData   - pointer to copy read data
 *
 * @return  BLE_PROFILE_SUCCESS, blePending or Failure
 ******************************************************************************/
static uint8_t DataService_ReadAttrCB(void *context, uint16_t connectionHandle,
        uint16_t charHdl, uint16_t offset, uint16_t size, uint16_t * pLen,
        uint8_t *pData)
{
    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(charHdl,
            DataServiceCharHandles,
            DATA_NUM_ATTR_SUPPORTED);
    uint8_t isValid = 0;

    /* Update connection handle (assumes one connection) */
    connHandle = connectionHandle;

    /* Find settings for the characteristic to be read. */
    switch (PROFILE_ID_CHAR(charID))
    {
    case DATA_STRING_CHAR0:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            *pLen = MIN(size, ds_StringValLen);
            memcpy(pData, ds_StringVal, *pLen);
            isValid = 1;
            break;

            /* Other considerations for String can be inserted here */
        default:
            break;
        }
        break;

    case DATA_STREAM_CHAR1:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            *pLen = MIN(size, ds_StreamValLen);
            memcpy(pData, ds_StreamVal, *pLen);
            isValid = 1;
            break;

            /* Other considerations for stream can be inserted here */
        default:
            break;
        }
        break;

    default:
        break;
    }

    if (isValid)
    {
        return SNP_SUCCESS;
    }

    /* Unable to find handle - set len to 0 and return error code */
    *pLen = 0;
    return SNP_UNKNOWN_ATTRIBUTE;
}

/*******************************************************************************
 * @fn      DataService_WriteAttrCB
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
static uint8_t DataService_WriteAttrCB(void *context, uint16_t connectionHandle,
        uint16_t charHdl, uint16_t len, uint8_t *pData)
{
    uint8_t status = SNP_UNKNOWN_ATTRIBUTE;
    uint8_t notifyApp = PROFILE_UNKNOWN_CHAR;
    char str1[8], str2[8], str3[4], str7[20];
    int i, c = 0, d = 0, e = 0, f = 0, number, temp = 0;
    char* test;
//    char input[1000];
    /* Update connection handle (assumes one connection) */
    connHandle = connectionHandle;

    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(charHdl,
                                DataServiceCharHandles,
                                DATA_NUM_ATTR_SUPPORTED);

    /* Find settings for the characteristic to be read. */
    switch (PROFILE_ID_CHAR(charID))
    {
      case DATA_STRING_CHAR0:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            memcpy(ds_StringVal, pData, len);
            test  = (char*) pData;
            status = SNP_SUCCESS;
            notifyApp = DS_STRING_ID;
            ds_StringValLen = len;
            //            nonumber(test, len);
            //            getfloat(test);
            //select(test, len);
            Display_printf(displayOut, 0, 0, "\nInput is %s\n", test);
            number = strlen(test) / 20;
            for (i = 0; i < number; i++){

                    while(d < 8){
                        str1[d] = test[temp+d];
                        //Display_printf(displayOut, 0, 0, "%c ", str7[0+d]);
                        d++;
                    }
                    str1[d] = '\0';
                   // Display_printf(displayOut, 0, 0, "Str1 is %s\n", str1);
                    ishexa1(str1);
////                    Display_printf(displayOut, 0, 0, "\n");
                    while(e < 8){
                        str2[e] = test[temp+8+e];
//                      Display_printf(displayOut, 0, 0, "%c ", str7[8+e]);
                        e++;
                    }
                    str2[e] = '\0';
                   // Display_printf(displayOut, 0, 0, "Str2 is %s\n", str2);
                    ishexa2(str2);
//                    //Display_printf(displayOut, 0, 0, "\n");
//
                    while(f < 4){
                        str3[f] = test[temp+16+f];
                        //Display_printf(displayOut, 0, 0, "%c ", str7[16+f]);
                        f++;
                    }
                    str3[f] = '\0';
                    //Display_printf(displayOut, 0, 0, "Str3 is %s\n", str3);
                    convert(str3);
                    c = 0;
                    d = 0;
                    e = 0;
                    f = 0;
                    temp = temp + 20;
                    count = count + 1;

            }
            printdata();

            break;

        /* Other considerations for LED0 can be inserted here */
        default:
            break;
        }
        break;

    case DATA_STREAM_CHAR1:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            DataService_setParameter(DS_STREAM_ID, len, pData);
            break;

        /* Other considerations for LED1 can be inserted here */
        default:
            break;
        }
        break;

      default:
        break;
    }

    /* If a characteristic value changed then callback function to notify
     * application of change
     */
    if ((notifyApp != PROFILE_UNKNOWN_CHAR) && dataServiceCallback
            && dataServiceCallback->charChangeCB)
    {
        dataServiceCallback->charChangeCB(notifyApp);
    }

    return status;
}

/*******************************************************************************
 * @fn      DataService_CCCDIndCB
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
uint8_t DataService_CCCDIndCB(void *context, uint16_t connectionHandle,
        uint16_t cccdHdl, uint8_t type, uint16_t value)
{
    uint8_t status = SNP_UNKNOWN_ATTRIBUTE;
    uint8_t notifyApp = PROFILE_UNKNOWN_CHAR;

    /* Update connection handle (assumes one connection) */
    connHandle = connectionHandle;

    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(cccdHdl,
                            DataServiceCharHandles,
                            DATA_NUM_ATTR_SUPPORTED);

    switch (PROFILE_ID_CHAR(charID))
    {
    case DATA_STREAM_CHAR1:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_CCCD:
            /* Set global cccd flag which will be used to to gate indications
             or notifications when SetParameter() is called */
            cccdFlag = value;
            notifyApp = charID;
            status = SNP_SUCCESS;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    /* If a characteristic value changed then callback function to notify
     * application of change
     */
    if ((notifyApp != PROFILE_UNKNOWN_CHAR) && dataServiceCallback
            && dataServiceCallback->cccdUpdateCB)
    {
        dataServiceCallback->cccdUpdateCB(notifyApp, value);
    }

    return status;
}

/*******************************************************************************
 * @fn      Data_processSNPEventCB
 *
 * @brief   This is a callback operating in the NPI task. It will be invoked
 *          whenever an event is received from the SNP that this profile has
 *          registered for
 *
 * @param   event  - event mask
 * @param   pValue - pointer event struct
 *
 * @return  status
 ******************************************************************************/
static void Data_processSNPEventCB(uint16_t event, snpEventParam_t *param)
{
    switch (event)
    {
    case SNP_CONN_EST_EVT:
    {
    }
        break;

    case SNP_CONN_TERM_EVT:
    {
    }
        break;
    default:
        break;
    }
}
