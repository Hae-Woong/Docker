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
/*!        \file  Gw_AsrPduRCfg5_Appl.vcaspecs.c
 *        \brief  Appl stub for Vector Code Analyzer
 *      \details  Stub of Cdd functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2022-06-27  visms         COM-1184      Replace RH by VCA
 *  02.00.01  2023-08-22  visfrm        ESCAN00114950 VCA erros when analyzing PduR
 *********************************************************************************************************************/
#include "ComStack_Types.h"

Std_ReturnType Appl_TryToGetSpinlock(uint32* lockVar)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  /* The values of Std_ReturnType are 0 (E_OK) and 1 (E_NOT_OK) */
  return $range(0,1);
}

Std_ReturnType Appl_ReleaseSpinlock(uint32* lockVar)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  /* The values of Std_ReturnType are 0 (E_OK) and 1 (E_NOT_OK) */
  return $range(0,1);
}
