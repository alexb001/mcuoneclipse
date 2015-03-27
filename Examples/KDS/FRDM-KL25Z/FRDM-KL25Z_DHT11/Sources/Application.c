/**
 * \file
 * \brief Implements the application
 * \author Erich Styger
 */

#include "Application.h"
#include "DHT11.h"
#include "WAIT1.h"
#include "LEDR.h"
#include "LEDG.h"
#include "CLS1.h"
#include "UTIL1.h"

void APP_Run(void) {
  DHT11_ErrorCode res;
  uint16_t temperature, humidity;
  CLS1_ConstStdIOType *io = CLS1_GetStdio();
  uint8_t buf[48];

  CLS1_SendStr("DHT11 Sensor Demo:\r\n", io->stdErr);
  WAIT1_Waitms(1000); /* wait one second after power-up to get the sensor stable */
  for(;;) {
    res = DHT11_Read(&temperature, &humidity);
    if (res!=DHT11_OK) { /* error */
      LEDR_Neg(); /* indicate error with red LED */
      /* write error message */
      CLS1_SendStr("ERROR: ", io->stdErr);
      CLS1_SendStr(DHT1_GetReturnCodeString(res), io->stdErr);
      CLS1_SendStr("\r\n", io->stdErr);
    } else { /* ok! */
      LEDG_Neg();
      /* write data values */
      UTIL1_strcpy(buf, sizeof(buf), "Temperature ");
      UTIL1_strcatNum32sDotValue100(buf, sizeof(buf), (int32_t)temperature);
      UTIL1_strcat(buf, sizeof(buf), "�C, Humidity ");
      UTIL1_strcatNum32sDotValue100(buf, sizeof(buf), (int32_t)humidity);
      UTIL1_strcat(buf, sizeof(buf), "%\r\n");
      CLS1_SendStr(buf, io->stdOut);
    }
    WAIT1_Waitms(1000); /* can only read sensor values with 1 Hz! */
  }
}
