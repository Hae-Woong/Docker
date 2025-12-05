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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  IpduM_ContainerTxRequestQueueHandling.h
 *         \unit  ContainerTxRequestQueueHandling
 *        \brief  Microsar IpduM Container Tx Request Queue Handling header file
 *      \details  This is the implementation of the MICROSAR IpduM module.
 *                The basic software module is based on the AUTOSAR IpduM specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#ifndef IPDUM_CONTAINERTXREQUESTQUEUEHANDLING_H
# define IPDUM_CONTAINERTXREQUESTQUEUEHANDLING_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "IpduM_Cfg.h"
# include "IpduM_Lcfg.h"
# include "IpduM_PBcfg.h"
# include "ComStack_Types.h"

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
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define IPDUM_START_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/*******************************************************************************************
*  IpduM_ContainerTx_RequestQueueHandling_TriggerTransmit()
*******************************************************************************************/
/*! \brief          Requests the PDU data before transmission
 *  \details        -
 *  \param[in]      txContainerId
 *  \param[in,out]  PduInfoPtr    Pointer to lower layer buffer
 *  \return         E_OK          Data was copied correctly
 *  \return         E_NOT_OK      Data could not be copied correctly, PduInfoPtr does not contain valid data
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different handle IDs
 *  \synchronous    TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *    requires IpduM_IsRequestQueuePrioIndirectionUsedOfTxContainerPdu(txContainerId);
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueueHandling_TriggerTransmit(IpduM_TxContainerPduIdxOfCTxLoInfoType txContainerId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
*  IpduM_ContainerTx_RequestQueueHandling_Transmit()
*******************************************************************************************/
/*! \brief        Requests the transmission of a contained PDU
 *  \details      -
 *  \param[in]    txContainedId
 *  \param[in]    PduInfoPtr    Pointer to payload buffer
 *  \return       E_OK          Transmit request was accepted by IpduM
 *  \return       E_NOT_OK      Transmit request was not accepted
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *    requires IpduM_IsRequestQueuePrioIndirectionUsedOfTxContainerPdu(IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId));
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueueHandling_Transmit(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueHandling_MainFunction()
*******************************************************************************************/
/*! \brief        Performs cyclic actions for request queue Pdus.
 *  \details      -
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *    requires IpduM_IsRequestQueuePrioIndirectionUsedOfTxContainerPdu(txContainerId);
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueueHandling_MainFunction(IpduM_CTxContainerPduIterType txContainerId);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXREQUESTQUEUEHANDLING_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxRequestQueueHandling.h
 *********************************************************************************************************************/
