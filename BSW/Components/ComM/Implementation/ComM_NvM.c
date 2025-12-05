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
/*!         \file    ComM_NvM.c
 *          \unit    NvM
 *          \brief   Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the NvM unit.
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

#define CCL_ASR_COMM_NVM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
 /* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1_0777 */
 /* PRQA S 3453 EOF */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_NvM.h"
#include "ComM_ModeInhibition.h"
#if (COMM_NVM_SUPPORT == STD_ON)
# include "NvM.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

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


/***********************************************************************************************************************
 *  ComM_NvM_GetErrorStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3673 1 */ /* MD_ComM_3673 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_NvM_GetErrorStatus( P2VAR(boolean, AUTOMATIC, COMM_APPL_VAR) isNvMDataRestored )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
#if (COMM_NVM_SUPPORT == STD_ON)
  NvM_RequestResultType requestResult = NVM_REQ_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If NvM support is enabled, query the NvM error status in order to restore the NvM data. */
  retVal = NvM_GetErrorStatus( (NvM_BlockIdType)COMM_NVM_BLOCK_ID, &requestResult ); /* VCA_COMM_CALL_UNDEFINED_EXTERNAL_FUNCTION */

  *isNvMDataRestored = (requestResult == NVM_REQ_OK) ? TRUE : FALSE;

#else
  COMM_DUMMY_STATEMENT( *isNvMDataRestored ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}

/***********************************************************************************************************************
 *  ComM_NvM_StoreRequest()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_NvM_StoreRequest( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
#if (COMM_NVM_SUPPORT == STD_ON)
  ComM_ChannelIterType channelIndex;

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_IsNvMStoreReq( COMM_MASTER_PARTITION_IDX ) )
  {
    /* #10 Trigger storage of non-volatile values. Only store the Wake-up Inhibition bit, clear the No Com Mode Limitation bit,
     *     it shall not be stored to NvM.*/
    ComM_InhibitionPtrType ComMInhibition = ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX );

    for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      ComMInhibition->ComM_InhibitionStatus[channelIndex] &= COMM_WAKEUP_INHIBITION_MASK;
    }

    retVal = NvM_SetRamBlockStatus( (NvM_BlockIdType)COMM_NVM_BLOCK_ID, TRUE );

    ComM_SetNvMStoreReq( FALSE, COMM_MASTER_PARTITION_IDX );
  }
#endif

  return retVal;
}


#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
