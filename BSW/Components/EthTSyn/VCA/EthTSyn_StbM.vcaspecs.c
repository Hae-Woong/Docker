/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_StbM.vcaspecs.c
 *        \brief  StbM stub for Vector Code Analyzer
 *      \details  Stub of StbM functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2022-05-09  vissem                      Initial creation.
 *********************************************************************************************************************/

#include "StbM.h"

/*! VCA Stub for StbM_GetTimeBaseStatus 
 * \spec
 *  requires syncTimeBaseStatus != NULL_PTR;
 *  requires offsetTimeBaseStatus != NULL_PTR;
 * \endspec
 */
Std_ReturnType StbM_GetTimeBaseStatus(
  StbM_SynchronizedTimeBaseType  timeBaseId,
  StbM_TimeBaseStatusType       *syncTimeBaseStatus,
  StbM_TimeBaseStatusType       *offsetTimeBaseStatus)
{
  return $range(0, 255);
}

/*! VCA Stub for StbM_BusGetCurrentTime 
 * \spec
 *  requires globalTimePtr != NULL_PTR;
 *  requires localTimePtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType StbM_BusGetCurrentTime(
  StbM_SynchronizedTimeBaseType  timeBaseId,
  StbM_TimeStampType            *globalTimePtr,
  StbM_VirtualLocalTimeType     *localTimePtr,
  StbM_UserDataType             *userData)
{
  return $range(0, 255);
}

/*! VCA Stub for StbM_GetCurrentVirtualLocalTime 
 * \spec
 *  requires localTimePtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType StbM_GetCurrentVirtualLocalTime(
  StbM_SynchronizedTimeBaseType  timeBaseId,
  StbM_VirtualLocalTimeType     *localTimePtr)
{
  return $range(0, 255);
}

/*! VCA Stub for StbM_GetCurrentTime 
 * \spec
 *  requires timeStampPtr != NULL_PTR;
 *  requires userDataPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType StbM_GetCurrentTime(
  StbM_SynchronizedTimeBaseType  timeBaseId,
  StbM_TimeStampType            *timeStampPtr,
  StbM_UserDataType             *userDataPtr)
{
  return $range(0, 255);
}

/*! VCA Stub for StbM_GetOffset 
 * \spec
 *  requires timeStampPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType StbM_GetOffset(
  StbM_SynchronizedTimeBaseType  timeBaseId,
  StbM_TimeStampType            *timeStampPtr)
{
  return $range(0, 255);
}
