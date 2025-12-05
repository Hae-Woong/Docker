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
/*!        \file  Com_Initialization.c
 *         \unit  Initialization
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_INITIALIZATION_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_Initialization.h"
#include "Com_Cfg_Initialization.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_IPduGroupHdlr.h"
#include "Com_CprQueue.h"
#include "Com_Timer.h"
#include "Com_ISRThreshold.h"
#include "Com_EventCache.h"
#include "Com_RxDlMon.h"
#include "Com_Transmission.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_Initialization_InitRx
**********************************************************************************************************************/
/*! \brief        This function initializes Rx IPdus and related parameters.
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Initialization_InitRx(void);

/**********************************************************************************************************************
  Com_Initialization_DeactivateRxPdu
**********************************************************************************************************************/
/*! \brief        This function deactivates the given Rx I-PDU.
    \details      -
    \pre          -
    \param[in]    rxPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Initialization_DeactivateRxPdu(Com_RxPduInfoIterType rxPduId);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Initialization_InitRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Initialization_InitRx(void)
{
  /* Iterate over all Rx ComIPdus and initialize all related parameters */
  for(Com_RxPduInfoIterType rxPduId = 0u; rxPduId < Com_GetSizeOfRxPduInfo(); rxPduId++)  /* FETA_COM_01 */
  {
    if(!Com_IsInvalidHndOfRxPduInfo(rxPduId))
    {
      Com_IPduGroupHdlr_InitRxIpdu(rxPduId);

      /* The IPdu must be deactivated after all initialization tasks of this IPdu! */
      Com_Initialization_DeactivateRxPdu(rxPduId);
    }
  }

  /* Activate Rx ComIPdus without assigned ComIPduGroups */
  Com_IPduGroupHdlr_ActivateAlwaysOnRxComIPdus();
}

/**********************************************************************************************************************
  Com_Initialization_DeactivateRxPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Initialization_DeactivateRxPdu(Com_RxPduInfoIterType rxPduId)
{
  /* Iterate over all related timeout handling state machines and deactivate timeout handling */
  for(Com_RxTOutInfoIndIterType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
      idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++)  /* FETA_COM_01 */
  {
    /*@ assert idxRxTOutInfoInd < Com_GetSizeOfRxTOutInfoInd(); */ /* VCA_COM_CSL03 */
    Com_RxDlMon_DeactivateRxIpduDm(Com_GetRxTOutInfoInd(idxRxTOutInfoInd));
  }

  /* Deactivate the Pdu Group state of the passed ComIPdu */
  Com_IPduGroupHdlr_DeactivateRxPduActive(rxPduId);
}


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
Com_Initialization_Init
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Initialization_Init(void)
{
 /* Initialize all Rx ComIPdus and related parameters */
  Com_Initialization_InitRx();

  /* Initialize all Tx ComIPdus and related parameters */
  Com_Transmission_InitTx();

  /* Initialize all ComIPduGroup related parameters */
  Com_IPduGroupHdlr_IPduGroupsInit();

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  /* Initialize the CrossPartitionRoutingQueue */
  Com_CprQueue_Init();
#endif

  /* Initialize the Timer */
  Com_Timer_Init();

  /* Initialize the ISR thresholds */
  Com_ISRThreshold_Init();

  /* Initialize event caches */
  Com_EventCache_Init();

  /* Set the component state to INITIALIZED */
  /* Com must be enabled after all subcomponents are initialized */
  Com_SetInitialized(TRUE);
}

/**********************************************************************************************************************
Com_Initialization_InitMemory
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Initialization_InitMemory(void)
{
  /* Initialize all component _INIT_ variables and set component state to initialized */
  Com_SetInitialized(FALSE);
}

/**********************************************************************************************************************
Com_Initialization_DeInit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Initialization_DeInit(void)
{
  /* Deactivate Pdus and set component state to uninitialized */
  Com_IPduGroupHdlr_IpduGroupsDeInit();

  Com_SetInitialized(FALSE);
}

/**********************************************************************************************************************
Com_Initialization_GetInitialized
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Initialization_GetInitialized(void)
{
  return Com_IsInitialized();
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */


/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_Initialization.c
 *********************************************************************************************************************/
