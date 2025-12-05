/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  KeyM_StbM.vcaspecs.c
 *        \brief  StbM stub for Vector Code Analyzer
 *      \details  Stub of StbM functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2021-09-29  vismxe                      Initial creation.
 *********************************************************************************************************************/
#include "KeyM_Cfg.h"

#if (KEYM_CERTTIMEBASEREFOFCERTIFICATE == STD_ON)
# include "StbM.h"

/*!
 * \spec
 *    requires timeStampPtr != NULL_PTR;
 *    requires userDataPtr != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTime(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr)
{
  timeStampPtr->timeBaseStatus = (StbM_TimeBaseStatusType)$undef();
  timeStampPtr->nanoseconds = (uint32)$undef();
  timeStampPtr->seconds = (uint32)$undef();
  timeStampPtr->secondsHi = (uint16)$undef();

  userDataPtr->userDataLength = (uint8)$undef();
  userDataPtr->userByte0 = (uint8)$undef();
  userDataPtr->userByte1 = (uint8)$undef();
  userDataPtr->userByte2 = (uint8)$undef();

  return (Std_ReturnType)$undef();
}
#endif