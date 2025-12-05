/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Dlt_StbM.vcaspecs.c
 *        \brief  StbM stub for Vector Code Analyzer
 *      \details  Stub of StbM functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2021-11-28  visore        SWAT-1831     Initial creation
 *  01.01.00  2022-07-21  visore        SWAT-2205     Dlt: support fetching synchronized timestamps from Tsyn
 *********************************************************************************************************************/

#include "StbM.h"

/*!
 * \spec
 *    requires timeStampPtr != NULL_PTR;
 *    requires userDataPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType StbM_GetCurrentTime(
  StbM_SynchronizedTimeBaseType timeBaseId,
  StbM_TimeStampType* timeStampPtr,
  StbM_UserDataType* userDataPtr)
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
