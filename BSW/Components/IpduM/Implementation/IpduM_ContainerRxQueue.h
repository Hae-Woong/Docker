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
 *         \file  IpduM_ContainerRxQueue.h
 *         \unit  ContainerRxQueue
 *        \brief  Microsar IpduM Container Rx Queue header file
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
#ifndef IPDUM_CONTAINERRXQUEUE_H
# define IPDUM_CONTAINERRXQUEUE_H

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
 *  IpduM_ContainerRxQueue_Put()
*******************************************************************************************/
/*! \brief        Enqueue the rx Container Pdu
 *  \details      -
 *  \param[in]    containerRxQueueInfoIdx  internal ID of the containerRxQueue.
 *  \param[in]    dataPtr           data pointer
 *  \param[in]    pduLength         actual length of the data
 *  \return       E_OK              Pdu was written to the queue
 *                E_NOT_OK          Pdu was NOT written to the queue. The queue is full.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires containerRxQueueInfoIdx < IpduM_GetSizeOfCContainerRxQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerRxQueue_Put(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx, P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength);

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_Get()
*******************************************************************************************/
/*! \brief        Dequeue the rx Container Pdu
 *  \details      -
 *  \param[in]    containerRxQueueInfoIdx  internal ID of the containerRxQueue.
 *  \param[out]   dataPtr           data pointer
 *  \param[in,out]pduLength         in: size of buffer; out: actual length of the data
 *  \return       E_OK              Pdu was read from the queue
 *                E_NOT_OK          No Pdu was read. Queue is empty.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires containerRxQueueInfoIdx < IpduM_GetSizeOfCContainerRxQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerRxQueue_Get(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, P2VAR(PduLengthType, AUTOMATIC, IPDUM_APPL_DATA) pduLength);

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_Remove
*******************************************************************************************/
/*! \brief        Remove the oldest element from the Container Rx Queue.
 *  \details      -
 *  \param[in]    containerRxQueueInfoIdx  internal ID of the containerRxQueue.
 *  \return       E_OK              Oldest Pdu was removed from the queue.
 *                E_NOT_OK          No Pdu was removed. Queue is empty.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires containerRxQueueInfoIdx < IpduM_GetSizeOfCContainerRxQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerRxQueue_Remove(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx);

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_Init
*******************************************************************************************/
/*! \brief        Initialize the container rx queue
 *  \details      -
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerRxQueue_Init(void);

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_IsFull
*******************************************************************************************/
/*! \brief        Returns if the queue is full.
 *  \details      -
 *  \param[in]    containerRxQueueInfoIdx  internal ID of the containerRxQueue.
 *  \return       TRUE  queue is full
 *                FALSE queue is not full
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires containerRxQueueInfoIdx < IpduM_GetSizeOfCContainerRxQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerRxQueue_IsFull(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx);

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_IsEmpty
*******************************************************************************************/
/*! \brief        Returns if the queue is empty.
 *  \details      -
 *  \param[in]    containerRxQueueInfoIdx  internal ID of the containerRxQueue.
 *  \return       TRUE  queue is empty
 *                FALSE queue is not empty
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires containerRxQueueInfoIdx < IpduM_GetSizeOfCContainerRxQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerRxQueue_IsEmpty(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx);

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_GetQueueDepth
*******************************************************************************************/
/*! \brief        Returns the queue depth.
 *  \details      -
 *  \param[in]    containerRxQueueInfoIdx  internal ID of the containerRxQueue.
 *  \return       queue depth
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires containerRxQueueInfoIdx < IpduM_GetSizeOfCContainerRxQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(uint8, IPDUM_CODE) IpduM_ContainerRxQueue_GetQueueDepth(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERRXQUEUE_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerRxQueue.h
 *********************************************************************************************************************/
