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
 *         \file  IpduM_ContainerTxDataQueue.h
 *         \unit  ContainerTxDataQueue
 *        \brief  Microsar IpduM Container Tx Data Queue header file
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
#ifndef IPDUM_CONTAINERTXDATAQUEUE_H
# define IPDUM_CONTAINERTXDATAQUEUE_H

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
 *  IpduM_ContainerTx_DataQueue_QueueCurrContainerPdu()
*******************************************************************************************/
/*! \brief        Queues Tx container queue.
 *  \details      If the queue is full: clears oldest item, reports error, queue current containerPdu.
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_QueueCurrContainerPdu(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_AddPdu_DynamicLayout()
*******************************************************************************************/
/*! \brief        Adds contained PDU to current container PDU with dynamic layout.
 *  \details      -
 *  \param[in]    txContainedId
 *  \param[in]    PduInfoPtr      Pointer to the PDU data to be transmitted.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_AddPdu_DynamicLayout(IpduM_TxContainedPduIterType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_AddPdu_StaticLayout()
*******************************************************************************************/
/*! \brief        Adds contained PDU to current container PDU with static layout.
 *  \details      -
 *  \param[in]    txContainedId
 *  \param[in]    PduInfoPtr      Pointer to the PDU data to be transmitted.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_AddPdu_StaticLayout(IpduM_TxContainedPduIterType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty()
*******************************************************************************************/
/*! \brief        Checks if the FIFO for a given container is empty.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       TRUE if the FIFO is empty
 *  \return       FALSE if the FIFO it not empty
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_IsFifoQueueFull()
*******************************************************************************************/
/*! \brief        Checks if the FIFO for a given container is full.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       TRUE if the FIFO is full
 *  \return       FALSE if the FIFO it not full
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IsFifoQueueFull(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_FitsPduIntoCurrentOpenInstance()
*******************************************************************************************/
/*! \brief        Checks if the provided Pdu fits into the open container instance
 *  \details      -
 *  \param[in]    txContainerId
 *  \param[in]    pduLength
 *  \return       TRUE if it fits
 *  \return       FALSE if it doesn't fit
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_DataQueue_FitsPduIntoCurrentOpenInstance(IpduM_CTxContainerPduIterType txContainerId, PduLengthType pduLength);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_IncrementWritePtr
*******************************************************************************************/
/*! \brief        Increment the internal write pointer
 *  \details      -
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IncrementWritePtr(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_IncrementReadPtr
*******************************************************************************************/
/*! \brief        Increment the internal read pointer
 *  \details      -
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IncrementReadPtr(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_Init
*******************************************************************************************/
/*! \brief        Initializes all data queue related data structures
 *  \details      -
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_Init(void);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_AddPdu2IdBuffer()
*******************************************************************************************/
/*! \brief        Write contained PduID to TxConfirmation Buffer
 *  \details      -
 *  \param[in]    txContainedId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different contained IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_AddPdu2IdBuffer(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXDATAQUEUE_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxDataQueue.h
 *********************************************************************************************************************/
