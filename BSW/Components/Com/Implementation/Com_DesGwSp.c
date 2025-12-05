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
/*!        \file  Com_DesGwSp.c
 *         \unit  DesGwSp
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_DESGWSP_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Com.h"
#include "Com_DesGwSp.h"
#include "Com_Cfg_DescriptionGatewayData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_DesGw.h"
#include "Com_GwTout.h"
#include "Com_CheckUpdateBit.h"
#include "Com_Transmission.h"

/**********************************************************************************************************************
 * LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_DesGwSp_ProcessEvent
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
 *
 *
 **********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGwSp_ProcessEvent(P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_GwTxPduDescriptionInfoIterType gwTxPduIdx)
{
  Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx = Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx);
  Com_TxPduInfoIterType ComTxPduId = Com_GetTxPduInfoIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx);

  boolean gwDescriptionUpdated = FALSE; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  boolean sendTrigger = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  boolean withoutRepetitionFlag = TRUE; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* Process all descriptions that correspond to the same destination txPdu */
  for(; gwDescrAccessIdx < Com_GetGwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfo(gwTxPduIdx); gwDescrAccessIdx++)  /* FETA_COM_01 */
  {
    /* Check if the source description is fully contained in received buffer */
    if(PduInfoPtr->SduLength >= Com_GetValidDlcOfGwDescriptionAccessInfo(gwDescrAccessIdx))
    {
      /* Check if the source description was updated */
      if(Com_CheckUpdateBit_GwDescription(gwDescrAccessIdx, PduInfoPtr))
      {
        /* Process Description Routing */
        CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) sourceBuffer = &(PduInfoPtr->SduDataPtr[Com_GetSourceMinStartBytePositionOfGwTxPduDescriptionInfo(gwTxPduIdx)]);
        switch (Com_DesGw_ProcessDestinationDescription(sourceBuffer, gwDescrAccessIdx))
        {
          case COM_TRANSMIT_TRIGGERWITHOUTREP:
            sendTrigger = TRUE; /* Set transmit flag if at least one description fulfills the transmit condition */
            break;
          case COM_TRANSMIT_TRIGGER:
            sendTrigger = TRUE;
            withoutRepetitionFlag = FALSE;      /* Set repetition flag if at least one description requires repetitive transmit */
            break;
          case COM_TRANSMIT_TRIGGER_NOTFULFILLED:
          default: /* COV_COM_MISRA */
          /* Intentionally Empty */
            break;
        }

        gwDescriptionUpdated = TRUE;
      }
    }
  }

  /* If any gateway description belonging to same source and destination Pdu was updated */
  if(gwDescriptionUpdated)
  {
    /* Handle Gw Timeout if configured */
    Com_GwTout_Event(ComTxPduId);
  }
  /* If transmit condition is fulfilled (transmit flag is set) */
  if(sendTrigger)
  {
    const Com_ImmediateProcessingOfGwDescriptionAccessInfoType isImmediate = Com_IsImmediateProcessingOfGwDescriptionAccessInfo(Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx));
    /* Trigger the transmission of the PDU */
    Com_Transmission_DesGwTransmit(ComTxPduId, isImmediate, withoutRepetitionFlag);
  }
}

#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  END OF FILE: Com_DesGwSp.c
 *********************************************************************************************************************/
