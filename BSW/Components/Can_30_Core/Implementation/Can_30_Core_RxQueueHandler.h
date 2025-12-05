/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  Can_30_Core_RxQueueHandler.h
 *        \brief  Can_30_Core_RxQueueHandler header file
 *        \details  Header of the Rx Queue Handler unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_RxQueueHandler
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_RXQUEUEHANDLER_H)
#define CAN_30_CORE_RXQUEUEHANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_Types.h"
#include "Can_30_Core_PrivateTypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Decrement Rx Queue Info members */
#define Can_30_Core_DecFillCountOfRxQueueInfo()       (Can_30_Core_GetAddrRxQueueInfo()->FillLevel--)
#define Can_30_Core_DecReadIndexOfRxQueueInfo()       (Can_30_Core_GetAddrRxQueueInfo()->ReadIndex--)
#define Can_30_Core_DecWriteIndexOfRxQueueInfo()      (Can_30_Core_GetAddrRxQueueInfo()->WriteIndex--)

/*! Increment Rx Queue Info members */
#define Can_30_Core_IncFillCountOfRxQueueInfo()       (Can_30_Core_GetAddrRxQueueInfo()->FillLevel++)
#define Can_30_Core_IncReadIndexOfRxQueueInfo()       (Can_30_Core_GetAddrRxQueueInfo()->ReadIndex++)
#define Can_30_Core_IncWriteIndexOfRxQueueInfo()      (Can_30_Core_GetAddrRxQueueInfo()->WriteIndex++)

/*! Read Rx Queue Info members */
#define Can_30_Core_GetReadIndexOfRxQueueInfo()       (Can_30_Core_GetAddrRxQueueInfo()->ReadIndex)
#define Can_30_Core_GetWriteIndexOfRxQueueInfo()      (Can_30_Core_GetAddrRxQueueInfo()->WriteIndex)
#define Can_30_Core_GetFillCountOfRxQueueInfo()       (Can_30_Core_GetAddrRxQueueInfo()->FillLevel)

/*! Set Rx Queue Info members */
#define Can_30_Core_SetWriteIndexOfRxQueueInfo(value) ((Can_30_Core_GetAddrRxQueueInfo()->WriteIndex) = (value))
#define Can_30_Core_SetReadIndexOfRxQueueInfo(value)  ((Can_30_Core_GetAddrRxQueueInfo()->ReadIndex) = (value))
#define Can_30_Core_SetFillCountOfRxQueueInfo(value)  ((Can_30_Core_GetAddrRxQueueInfo()->FillLevel) = (value))

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_RxQueueInit()
 *********************************************************************************************************************/
/*! \brief       RxQueue Initialization
 *  \details     Initializes the RxQueue Buffer
 *  \param[in]   -
 *  \pre         Module is uninitialized.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_RxQueueInit(void);

/**********************************************************************************************************************
 *  Can_30_Core_PutMessage()
 *********************************************************************************************************************/
/*! \brief       Rx Queue put message
 *  \details     Stores a message in the Rx Queue and updated the write index
 *  \param[in]   hrh              Handle to mailbox object
 *  \param[in]   PduInfo          Pointer to local PduInfo struct
 *  \pre         Rx Queue is initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(Can_30_Core_ErrorId, CAN_30_CORE_CODE) Can_30_Core_PutMessage(Can_HwHandleType hrh, Can_PduInfoPtrType PduInfo);

/**********************************************************************************************************************
 *  Can_30_Core_RxQueueExecution()
 *********************************************************************************************************************/
/*! \brief       Executes the Rx Queue
 *  \details     Executes the Rx Queue by popping all the stored messages and performs the indication to the upper layer
 *  \param[in]   -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_RxQueueExecution(void);

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_RXQUEUEHANDLER_H */
/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_RxQueueHandler.h
 *********************************************************************************************************************/
