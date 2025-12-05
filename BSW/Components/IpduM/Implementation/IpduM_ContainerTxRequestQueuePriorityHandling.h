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
 *         \file  IpduM_ContainerTxRequestQueuePriorityHandling.h
 *         \unit  ContainerTxRequestQueuePriorityHandling
 *        \brief  Microsar IpduM Container Tx Request Queue Priority Handling header file
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
#ifndef IPDUM_CONTAINERTXREQUESTQUEUEPRIORITYHANDLING_H
# define IPDUM_CONTAINERTXREQUESTQUEUEPRIORITYHANDLING_H

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
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty()
*******************************************************************************************/
/*! \brief        Checks if the request queues for a given container is empty.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       TRUE if the queue has no PDUs
 *  \return       FALSE if there are PDUs in the queue
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_CalculateSize()
*******************************************************************************************/
/*! \brief        Calculates the size of data stored in the request queues (the DLCs combined)
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       Size of data stored in the request queue
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_CalculateSize(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_RestoreReadIDs
*******************************************************************************************/
/*! \brief        Restores the read IDs inside the FillLastIsBestContainer API.
 *  \details      The restore point is set by IpduM_ContainerTx_RequestQueue_SetRestorePoint.
 *                Both APIs must be called in the same exclusive area and this sequence must not be interrupted by other APIs.
 *                FillLastIsBestContainer API can also be used without the restore feature of the request queue.
 *  \param[in]    txContainerId   ID of the container PDU
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_RestoreReadIDs(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_SetRestorePoint
*******************************************************************************************/
/*! \brief        Marks the point from which the read operations from the queue can be restored.
 *  \details      -
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_SetRestorePoint(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_FillLastIsBestContainer()
*******************************************************************************************/
/*! \brief         Fills the provided buffer with the data of the read containedPdu IDs via the trigger transmit API.
 *  \details       -
 *  \param[in]     txContainerId   ID of the container PDU
 *  \param[in,out] bufferInfo.SduDataPtr[in] pointer to the buffer where the data of the contained Pdus is stored
 *                 bufferInfo.SduLength[in]  length of the buffer
 *                 bufferInfo.SduLength[out] amount of data written to the buffer
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_FillLastIsBestContainer(IpduM_CTxContainerPduIterType txContainerId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) bufferInfo);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXREQUESTQUEUEPRIORITYHANDLING_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxRequestQueuePriorityHandling.h
 *********************************************************************************************************************/
