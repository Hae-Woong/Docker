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
/*!        \file  Com_DesGwCp.h
 *         \unit  DesGwCp
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_DESGWCP_H)
# define COM_DESGWCP_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_DesGwCP_ProcessQueuesTx
**********************************************************************************************************************/
/*! \brief        This function handles the processing of the cross partition routing queues leading to this partition and initiates the transmission if the conditions of
 *                the configured transfer properties are satisfied.
    \details      -
    \pre          -
    \param[in]    mainFunctionId                           ID of the calling route signals main function.
    \param[in]    gwDescriptionProcessingISRLockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
*****************************************************************************************/
FUNC(void, COM_CODE) Com_DesGwCp_ProcessQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_ISRThresholdInfoIterType gwDescriptionProcessingISRLockCounterIdx);

/**********************************************************************************************************************
  Com_DesGwCp_ProcessRoutingRx
**********************************************************************************************************************/
/*! \brief        This function handles the rx side of the cross partition gateway description events which are written into the cross partition routing queue.
    \details      -
    \pre          IsUsed check required: Com_IsCprQueueUsedOfGwTxPduDescriptionInfo()
    \param[in]    PduInfoPtr                contains the SDU buffer and the SDU length
    \param[in]    gwTxPduIdx                index of the GwTxPduDescriptionInfo struct
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires gwTxPduIdx < Com_GetSizeOfGwTxPduDescriptionInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
*****************************************************************************************/
FUNC(void, COM_CODE) Com_DesGwCp_ProcessRoutingRx(P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_GwTxPduDescriptionInfoIterType gwTxPduIdx);

# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_DESGWCP_H */
/**********************************************************************************************************************
  END OF FILE: Com_DesGwCp.h
**********************************************************************************************************************/
