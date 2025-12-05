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
/*!        \file  _Appl_Time.c
 *        \brief  Demo time provision source file
 *      \details  Demo time generator (integration support example providing functions required by MICROSAR IP).
 *********************************************************************************************************************/
#define APPL_TIME_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Appl_Time.h"
  /* C_COMP_ANSI_CANOE */

/***********************************************************************************************************************
 *  ApplTime_GetTime32
 **********************************************************************************************************************/
/*! \brief      get seconds since 1980
 *  \context    TLS
 **********************************************************************************************************************/
void Appl_TlsGetTime32(uint32 *TgtDataPtr)
{
  ApplTime_GetTime32(TgtDataPtr);
}
void ApplTime_GetTime32(uint32 *TgtDataPtr)
{
# if !defined (APPLTIME_DEACTIVATE_ERROR_DIRECTIVE)
#  error "Appl_Time.c: Replace the code below with valid time stamp returning the time in seconds since 1970"
# endif
  *TgtDataPtr = 1644192000; /* i.e. 2022-02-07 00:00:00 */
}
/**********************************************************************************************************************
 *  END OF FILE: Appl_Time.c
 *********************************************************************************************************************/
