/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Can_30_Core_CanIf.vcaspecs.c
 *        \brief  CanIf stub for Vector Code Analyzer
 *      \details  Stub of CAN Interface functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/
#include "CanIf.h"

/* CanIf_30_Core_ControllerModeIndication can be modeled as an empty stub since function does not have a pointer parameter and no return value */
void CanIf_30_Core_ControllerModeIndication(uint8 ControllerId, CanIf_ControllerModeType ControllerMode) 
{
}

/* CanIf_30_Core_ErrorNotification can be modeled as an empty stub since function does not have a pointer parameter and no return value */
void CanIf_30_Core_ErrorNotification(uint8 Controller, Can_ErrorType CanError)
{
}
