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
/*!       \file  Can_30_Core_RxQueueHandler.c
 *        \brief  Can_30_Core_RxQueueHandler source file
 *        \details  Contains the implementation of the Rx Queue Handler unit.
 *        \unit Can_30_Core_RxQueueHandler
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
#define CAN_30_CORE_RXQUEUEHANDLER_SOURCE
#define CAN_30_CORE_INTERNAL

/**********************************************************************************************************************
 * LOCAL MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2880, 2991, 2992, 2994, 2995, 2996 EOF */ /* MD_Can_30_Core_ConstValue */
/* PRQA  S 2741, 2742 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core.h"
#include "Can_30_Core_vCan.h"
#include "vstdlib.h"
#include "Can_30_Core_RxQueueHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "CanIf.h"
#include "CanIf_Cbk.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_UpdateReadIndex()
 *********************************************************************************************************************/
/*! \brief       RxQueue Read index update
 *  \details     Updates the read index and fill level of the RxQueue
 *  \param[in]   -
 *  \pre         RxQueue is initialized and Can_30_Core_GetMessage() is called in the same context.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateReadIndex(void);

/**********************************************************************************************************************
 *  Can_30_Core_UpdateWriteIndex()
 *********************************************************************************************************************/
/*! \brief       RxQueue Read write update
 *  \details     Updates the write index and fill level of the RxQueue
 *  \param[in]   -
 *  \pre         RxQueue is initialized and function is only called inside Can_30_Core_PutMessage()
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateWriteIndex(void);

/**********************************************************************************************************************
 *  Can_30_Core_IsQueueFull()
 *********************************************************************************************************************/
/*! \brief       Checks if RxQueue Is full
 *  \details     Compares the fill level with the buffer size
 *  \param[in]   -
 *  \return      TRUE     Queue is full
 *  \return      FALSE    Queue is not full
 *  \pre         RxQueue is initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsQueueFull(void);

/**********************************************************************************************************************
 *  Can_30_Core_ClearQueue()
 *********************************************************************************************************************/
/*! \brief       Clears the Rx Queue
 *  \details     Resets the read and write indexes and the fill level of the RxQueue
 *  \param[in]   -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ClearQueue(void);

/**********************************************************************************************************************
 *  Can_30_Core_GetMessage()
 *********************************************************************************************************************/
/*! \brief       Rx Queue Get message
 *  \details     Returns the next message of the Rx Queue and updates the read index
 *  \param[in]   -
 *  \return      Can_30_Core_RxQueueBufType      Pointer to RxQueue buffer containing data
 *  \pre         Can_30_Core_GetRxQueueFillLevel is not 0
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC_P2VAR(Can_30_Core_RxQueueBufType, AUTOMATIC, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetMessage(void);

/**********************************************************************************************************************
 *  Can_30_Core_GetRxQueueFillLevel()
 *********************************************************************************************************************/
/*! \brief       RxQueue Fill Level
 *  \details     Returns the fill level of the RxQueue
 *  \param[in]   -
 *  \return      uintx      Fill Level
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(uint16, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetRxQueueFillLevel(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_UpdateReadIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateReadIndex(void) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrement fill level */
  Can_30_Core_DecFillCountOfRxQueueInfo(); /* PRQA S 3387 */ /* MD_Can_30_Core_DecInc */
  /* #20 Increment read index */
  Can_30_Core_IncReadIndexOfRxQueueInfo(); /* PRQA S 3387 */ /* MD_Can_30_Core_DecInc */
  /* #30 Update read index if it has reached the size of buffer */
  Can_30_Core_SetReadIndexOfRxQueueInfo(Can_30_Core_GetReadIndexOfRxQueueInfo() % Can_30_Core_GetSizeOfRxQueueBuffer());
}

/**********************************************************************************************************************
 *  Can_30_Core_UpdateWriteIndex()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_UpdateWriteIndex
  Relation_Context:
DevErrorDetect, RxQueue, RxFullCan
DevErrorDetect, RxQueue, RxBasicCan
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateWriteIndex(void) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Increment fill level */
  Can_30_Core_IncFillCountOfRxQueueInfo(); /* PRQA S 3387 */ /* MD_Can_30_Core_DecInc */
  /* #20 Increment write index */
  Can_30_Core_IncWriteIndexOfRxQueueInfo(); /* PRQA S 3387 */ /* MD_Can_30_Core_DecInc */
  /* #30 Update write index if it has reached the size of buffer */
  Can_30_Core_SetWriteIndexOfRxQueueInfo(Can_30_Core_GetWriteIndexOfRxQueueInfo() % Can_30_Core_GetSizeOfRxQueueBuffer());
}

/**********************************************************************************************************************
 *  Can_30_Core_IsQueueFull()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsQueueFull
  Relation_Context:
DevErrorDetect, RxQueue, RxFullCan
DevErrorDetect, RxQueue, RxBasicCan
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsQueueFull(void) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* #10 Check if the fill level has reached the buffer size */
  return (Can_30_Core_GetFillCountOfRxQueueInfo() >= Can_30_Core_GetSizeOfRxQueueBuffer());
}

/**********************************************************************************************************************
 *  Can_30_Core_ClearQueue()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ClearQueue
  Relation_Context:
RxQueue
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ClearQueue(void) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Development Error Checks ------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */

  /* CAN_30_CORE_EXCLUSIVE_AREA_4 is intentionally not used here, because this function is only called during Can_30_Core_Init()
     and therefore concurrent access to the RxQueueInfo is not possible. */

  /* #10 Reset the Fill Level */
  Can_30_Core_SetFillCountOfRxQueueInfo(0u);
  /* #20 Reset the Read Index */
  Can_30_Core_SetReadIndexOfRxQueueInfo(0u);
  /* #30 Reset the Read Write Index */
  Can_30_Core_SetWriteIndexOfRxQueueInfo(0u);
}

/**********************************************************************************************************************
 *  Can_30_Core_GetRxQueueFillLevel()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetRxQueueFillLevel
  Relation_Context:
RxQueue
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(uint16, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetRxQueueFillLevel(void) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the fill level of the RxQueue */
  return Can_30_Core_GetFillCountOfRxQueueInfo();
}

/**********************************************************************************************************************
 *  Can_30_Core_GetMessage()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetMessage
  Relation_Context:
DevErrorDetect, RxQueue
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL FUNC_P2VAR(Can_30_Core_RxQueueBufType, AUTOMATIC, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetMessage(void) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the current read index */
  uint16 readIndex = Can_30_Core_GetReadIndexOfRxQueueInfo();
  return Can_30_Core_GetAddrRxQueueBuffer(readIndex);
}
#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_PutMessage()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_PutMessage
  Relation_Context:
DevErrorDetect, RxQueue, RxFullCan
DevErrorDetect, RxQueue, RxBasicCan
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
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
 */
CAN_30_CORE_LOCAL_API FUNC(Can_30_Core_ErrorId, CAN_30_CORE_CODE) Can_30_Core_PutMessage(Can_HwHandleType hrh, Can_PduInfoPtrType PduInfo) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;

  if (Can_30_Core_DevCheck_IsConditionTrue(((uint8)sizeof(Can_30_Core_GetAddrRxQueueBuffer(0)->data) / sizeof(Can_30_Core_GetAddrRxQueueBuffer(0)->data[0])) < PduInfo->length))
  { /* #95 Check if the message is of the expected length */
    errorId = CAN_30_CORE_E_PARAM_DATA_LENGTH;
  } 
  else /* ----- Implementation ----------------------------------------------- */ 
  {
    /* #100 Secure RxQueue Handling */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_4);
    /* #110 Check if fifo is not full */
    if (!Can_30_Core_IsQueueFull())
    { 
      /* #120 Get the current write index */
      uint16 writeIndex = Can_30_Core_GetWriteIndexOfRxQueueInfo();
      /* #130 Copy the message to the buffer */
      Can_30_Core_GetAddrRxQueueBuffer(writeIndex)->id = PduInfo->id; /* VCA_CAN_30_CORE_RXQUEUEBUFFER */
      Can_30_Core_GetAddrRxQueueBuffer(writeIndex)->dlc = PduInfo->length; /* VCA_CAN_30_CORE_RXQUEUEBUFFER */
      Can_30_Core_GetAddrRxQueueBuffer(writeIndex)->Hrh = hrh; /* VCA_CAN_30_CORE_RXQUEUEBUFFER */
      VStdMemCpy(Can_30_Core_GetAddrRxQueueBuffer(writeIndex)->data, PduInfo->sdu, PduInfo->length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_CAN_30_CORE_RXQUEUEBUFFERDATA */
      /* #140 Update the write index */
      Can_30_Core_UpdateWriteIndex();
    }
    else
    { /* #150 Rx Queue Full */
      errorId = CAN_30_CORE_E_RXQUEUE;
    }
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_4);
  }
  return errorId;
}

/**********************************************************************************************************************
 *  Can_30_Core_RxQueueExecution()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_RxQueueExecution
  Relation_Context:
DevErrorDetect, RxQueue
  Relation:
GenericPreCopy, DevErrorDetect, MirrorMode
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_RxQueueExecution(void) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 loopTimeout = 0u;
  /* ----- Implementation ----------------------------------------------- */
  while ((Can_30_Core_GetRxQueueFillLevel() != 0u) && (loopTimeout < Can_30_Core_GetSizeOfRxQueueBuffer())) /* FETA_CAN_30_CORE_3 */
  {
    Can_30_Core_RxQueueBufType *rxBufferElement = Can_30_Core_GetMessage(); /* PRQA S 3678 */ /* MD_Can_30_Core_ConstValue */
    uint8 controller = Can_30_Core_GetControllerConfigIdxOfMailbox(rxBufferElement->Hrh);
    /* #100 Check if the controller parameter is valid */
    if (Can_30_Core_IsControllerValid(controller))
    {
      /* #110 Check if the controller is in Start - only pop the message from the queue */
      if (Can_30_Core_IsControllerInStart(controller))
      {
        Can_ReturnType genericRetval = CAN_NOT_OK;
        if (Can_30_Core_IsGenericPrecopyOfGeneralFeature(0))
        {
          if ((Can_30_Core_IsMirrorModeOfGeneralFeature(0)) &&
              (Can_30_Core_GetMirrorMode(controller) == CDDMIRROR_INACTIVE)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
          { /* #120 MirrorMode Enabled but inactive */
            genericRetval = CAN_OK;
          }
          else
          {
            /* #130 call GenericPrecopy() */
            genericRetval = Can_30_Core_CallApplGenericPrecopy(controller, (rxBufferElement->id & CAN_30_CORE_ID_MASK_IN_GENERIC_CALLOUT), rxBufferElement->dlc, rxBufferElement->data); /* VCA_CAN_30_CORE_CALLAPPLGENERICPRECOPY */
          }
        }
        if ((!Can_30_Core_IsGenericPrecopyOfGeneralFeature(0)) || (genericRetval == CAN_OK))
        { /* #140 call CanIf_RxIndication() if Can_30_Core_CallApplGenericPrecopy() return OK or no generic precopy is used (SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687) */
          CanIf_30_Core_RxIndication(rxBufferElement->Hrh, rxBufferElement->id, rxBufferElement->dlc, rxBufferElement->data);
        }
        CAN_30_CORE_DUMMY_STATEMENT(genericRetval); /* PRQA S 1338, 2982, 2983 */ /* MD_MSR_DummyStmt */
      }
    }
    /* #150 Secure RxQueue Handling */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_4);
    /* #160 Update read index */
    Can_30_Core_UpdateReadIndex();
    /* #170 Update counter to keep amount of iterations to a finite number */
    loopTimeout++;
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_4);
  }
}

/**********************************************************************************************************************
 *  Can_30_Core_RxQueueInit()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_RxQueueExecution
  Relation_Context:
  Relation:
RxQueue
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_RxQueueInit(void)
{
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Core_IsRxQueueOfGeneralFeature(0))
  {
    /* #10 Clear RxQueue */
    Can_30_Core_ClearQueue();
  }
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_RxQueueHandler.c
 *********************************************************************************************************************/
