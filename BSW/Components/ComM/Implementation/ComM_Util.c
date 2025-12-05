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
/*!         \file     ComM_Util.c
 *          \unit     Util
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the Util unit
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define CCL_ASR_COMM_UTIL_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Util.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  ComM_GetInternalPartitionIdx()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( ComM_PartitionConfigIdxOfChannelType, COMM_CODE ) ComM_GetInternalPartitionIdx( CONST( ApplicationType, AUTOMATIC ) applicationId )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType partitionIdx = COMM_MASTER_PARTITION_IDX;

  /* ----- Implementation ----------------------------------------------- */
#if ( COMM_MULTIPARTITION == STD_ON )
  /* #10 Iterate over all ComM partition instances and return the partition index of the ComM partition instance which belongs to the current application. */
  for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    /* #100 If no ComM partition belongs to the current application, return SizeOfPartitionIdentifiers as an invalid partition index. */
    if ( applicationId == ComM_GetPartitionSNVOfPartitionIdentifiers( partitionIdx ) )
    {
      break;
    }
  }
#endif

  COMM_DUMMY_STATEMENT_CONST( applicationId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return (ComM_PartitionConfigIdxOfChannelType)partitionIdx;
}


#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
