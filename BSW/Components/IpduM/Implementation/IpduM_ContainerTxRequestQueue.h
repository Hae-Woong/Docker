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
 *         \file  IpduM_ContainerTxRequestQueue.h
 *         \unit  ContainerTxRequestQueue
 *        \brief  Microsar IpduM Container Tx Request Queue header file
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
#ifndef IPDUM_CONTAINERTXREQUESTQUEUE_H
# define IPDUM_CONTAINERTXREQUESTQUEUE_H

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
 *  IpduM_ContainerTx_RequestQueue_Init()
*******************************************************************************************/
/*! \brief        Initializes the request queue data structures.
 *  \details      -
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Init(void);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_Remove
*******************************************************************************************/
/*! \brief        Removes the oldest ID from the request queue.
 *  \details      -
 *  \param[in]    requestQueueInfoIdx
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Remove(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_IsEmpty()
*******************************************************************************************/
/*! \brief        Checks if the request queue is empty.
 *  \details      -
 *  \param[in]    requestQueueInfoIdx
 *  \return       TRUE if the queue has no PDUs
 *  \return       FALSE if there are PDUs in the queue
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_IsEmpty(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_CalculateSize()
*******************************************************************************************/
/*! \brief        Calculates the size of data stored in the request queue (the DLCs + HeaderSizes combined)
 *  \details      -
 *  \param[in]    requestQueueInfoIdx
 *  \param[in]    headerSize
 *  \return       Size of data stored in the request queue
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_CalculateSize(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx, PduLengthType headerSize);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_Put()
*******************************************************************************************/
/*! \brief        Adds a Pdu to the request queue.
 *  \details      -
 *  \param[in]    requestQueueInfoIdx
 *  \param[in]    txContainedId
 *  \param[in]    sduLength       length of the received PDU for size estimation
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo();
 *    requires txContainedId < IpduM_GetSizeOfTxContainedPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Put(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx, IpduM_TxContainedPduIterType txContainedId, PduLengthType sduLength);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_RestoreReadIDs
*******************************************************************************************/
/*! \brief        Restores the read IDs
 *  \details      The restore point is set by IpduM_ContainerTx_RequestQueue_SetRestorePoint.
 *                Both APIs must be called in the same exclusive area and this sequence must not be interrupted by other APIs.
 *                FillLastIsBestContainer API can also be used without the restore feature of the request queue.
 *  \param[in]    requestQueueInfoIdx
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_RestoreReadIDs(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_SetRestorePoint
*******************************************************************************************/
/*! \brief        Marks the point from which the read operations from the queue can be restored.
 *  \details      -
 *  \param[in]    requestQueueInfoIdx
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_SetRestorePoint(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_Peek
*******************************************************************************************/
/*! \brief        Read and return the oldest ID in the request queue without removing it.
 *  \details      -
 *  \param[in]    requestQueueInfoIdx
 *  \param[out]   containedPduId  ID of the Pdu
 *  \param[out]   containedPduLength  Length of the Pdu in the Transmit call
 *  \return       E_OK        Pdu available. Out parameter can be used.
 *                E_NOT_OK    No Pdu available. Out parameter must NOT be used.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Peek(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx,
                                                                                        P2VAR(IpduM_TxContainedPduIdxOfRequestQueueBufferType, AUTOMATIC, IPDUM_APPL_DATA) containedPduId,
                                                                                        P2VAR(PduLengthType, AUTOMATIC, IPDUM_APPL_DATA) containedPduLength);


# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXREQUESTQUEUE_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxRequestQueue.h
 *********************************************************************************************************************/
