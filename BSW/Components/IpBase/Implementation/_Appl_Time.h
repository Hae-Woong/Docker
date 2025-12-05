/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  _Appl_Time.h
 *        \brief  Demo time generator header file
 *      \details  Demo time generator (integration support example providing functions required by MICROSAR IP).
 *********************************************************************************************************************/
#if !defined (APPL_TIME_H)
# define APPL_TIME_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Platform_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void Appl_TlsGetTime32(uint32 *TgtDataPtr);  /* deprecated */
void ApplTime_GetTime32(uint32 *TgtDataPtr);

#endif 
  /* APPL_TIME_H */
/**********************************************************************************************************************
 *  END OF FILE: Appl_Time.h
 *********************************************************************************************************************/
