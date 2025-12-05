/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  DoIPInt_TxQueue.c
 *        \brief  DoIPInt source file for sub-module DoIPInt_TxQueue.
 *      \details  Implementation for sub-module DoIPInt_TxQueue of component DoIPInt.
 *                Sub-module implements one TxRequest queue per connection.
 *                For each queue, the following management data is maintained:
 *                  - One double linked list for the used queue elements ("used list").
 *                  - One double linked list for the free elements ("unused list").
 *                  - Tail index of both lists and head index of used list.
 *                  - The number of free elements in the queue.
 *                  - A look up table with
 *                      - input = channel index and
 *                      - output = index of the queue element for that channel.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#define DOIPINT_TXQUEUE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Priv.h"
#include "DoIPInt_TxQueue.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if !defined (DOIPINT_LOCAL)
# define DOIPINT_LOCAL static
#endif /* !defined (DOIPINT_LOCAL) */

#if !defined (DOIPINT_LOCAL_INLINE)
# define DOIPINT_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (DOIPINT_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define DOIPINT_START_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_TxQueue_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_TxQueue_Init(
  DoIPInt_ConnectionIterType ConnectionIter)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_TxQueueIterType       queueIdxIter; /* Not initialized: For-loop counter. */
  DoIPInt_TxQueueLookUpIterType queuelookUpIter; /* Not initialized: For-loop counter. */
  DoIPInt_TxQueueIterType       queueStartIdx = DoIPInt_GetTxQueueStartIdxOfConnection(ConnectionIter);
  DoIPInt_TxQueueIterType       queueEndIdx = DoIPInt_GetTxQueueEndIdxOfConnection(ConnectionIter);
  DoIPInt_SizeOfTxQueueType     queueDepth = (DoIPInt_SizeOfTxQueueType)(queueEndIdx - queueStartIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Loop over all queue entries of connection. */
  for ( queueIdxIter = queueStartIdx; queueIdxIter < queueEndIdx; queueIdxIter++ )
  {
    /* #100 Initialize data of queue with invalid values. */
    DoIPInt_SetTxRequestHandleOfTxQueue(queueIdxIter, DOIPINT_MAX_UINT_32);
    DoIPInt_SetTxRequestLengthOfTxQueue(queueIdxIter, 0u);
    DoIPInt_SetMsgHandlerIdxOfTxQueue(queueIdxIter, DoIPInt_GetSizeOfMsgHandler());

    /* #101 Build linked list from unused entries. */
    DoIPInt_SetPrevListElementOfTxQueue(queueIdxIter,
      ((queueIdxIter == queueStartIdx) ? DoIPInt_GetSizeOfTxQueue() : (DoIPInt_SizeOfTxQueueType)(queueIdxIter - 1u)));
    DoIPInt_SetNextListElementOfTxQueue(queueIdxIter, ((queueIdxIter == (queueEndIdx - 1u)) ?
        DoIPInt_GetSizeOfTxQueue() : (DoIPInt_SizeOfTxQueueType)(queueIdxIter + 1u)));
  }

  /* #20 Set head/tail of used and unused lists and initialize number of free elements in queue. */
  DoIPInt_SetTxQueueUsedListStartIdxOfConnectionDyn(ConnectionIter, DoIPInt_GetSizeOfTxQueue());
  DoIPInt_SetTxQueueUsedListEndIdxOfConnectionDyn(ConnectionIter, DoIPInt_GetSizeOfTxQueue());
  DoIPInt_SetTxQueueUnusedListEndIdxOfConnectionDyn(ConnectionIter, (DoIPInt_SizeOfTxQueueType)(queueEndIdx - 1u));
  DoIPInt_SetTxQueueUnusedElementsOfConnectionDyn(ConnectionIter, queueDepth);

  /* #30 Initialize the Tx queue index lookup table. */
  for ( queuelookUpIter = 0u; queuelookUpIter < DoIPInt_GetSizeOfTxQueueLookUp(); queuelookUpIter++ )
  {
    DoIPInt_SetTxQueueIdxOfTxQueueLookUp(queuelookUpIter, DoIPInt_GetSizeOfTxQueue());
  }
} /* DoIPInt_TxQueue_Init() */

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_Add()
 *********************************************************************************************************************/
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
 *
 *
 *
  */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TxQueue_Add(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  PduLengthType Length,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent queue modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Ensure that the derived index for the lookup table is inside valid range. */
  if ( HandleIdx < ((uint32)DoIPInt_GetTxQueueLookUpEndIdxOfMsgHandler(MsgHandlerIdx) -
    (uint32)DoIPInt_GetTxQueueLookUpStartIdxOfMsgHandler(MsgHandlerIdx)) )
  {
    const DoIPInt_SizeOfTxQueueLookUpType txQueuelookUpIdx = (DoIPInt_SizeOfTxQueueLookUpType)HandleIdx +
      DoIPInt_GetTxQueueLookUpStartIdxOfMsgHandler(MsgHandlerIdx);

    /* #30 Ensure that no request for this handle exists yet. */
    if ( DoIPInt_GetTxQueueIdxOfTxQueueLookUp(txQueuelookUpIdx) == DoIPInt_GetSizeOfTxQueue() )
    {
      /* #40 Ensure that the queue has a free element. */
      if ( DoIPInt_GetTxQueueUnusedElementsOfConnectionDyn(ConnectionIdx) >= 1u )
      {
        /* #50 Get index of last element in unused list and index of predecessor. */
        DoIPInt_SizeOfTxQueueType elemIdxToInsert = DoIPInt_GetTxQueueUnusedListEndIdxOfConnectionDyn(ConnectionIdx);
        DoIPInt_SizeOfTxQueueType elemIdxPreviousUnused = DoIPInt_GetPrevListElementOfTxQueue(elemIdxToInsert);
        DoIPInt_SizeOfTxQueueType elemIdxUsed = DoIPInt_GetTxQueueUsedListEndIdxOfConnectionDyn(ConnectionIdx);

        /* #60 Store transmit request in queue element. */
        /*@ assert elemIdxToInsert < DoIPInt_GetSizeOfTxQueue(); */                                                    /* VCA_DOIPINT_QUEUE_IDX_VALID */
        DoIPInt_SetTxRequestHandleOfTxQueue(elemIdxToInsert, HandleIdx);
        DoIPInt_SetTxRequestLengthOfTxQueue(elemIdxToInsert, Length);
        DoIPInt_SetMsgHandlerIdxOfTxQueue(elemIdxToInsert, MsgHandlerIdx);

        /* #70 If unused list is bigger than one element, update link to remove element from end of unused list. */
        if ( elemIdxPreviousUnused < DoIPInt_GetSizeOfTxQueue() )
        {
          DoIPInt_SetNextListElementOfTxQueue(elemIdxPreviousUnused, DoIPInt_GetSizeOfTxQueue());
        }

        /* #80 Update links to append element to end of used list. */
        DoIPInt_SetPrevListElementOfTxQueue(elemIdxToInsert, elemIdxUsed);

        /* #90 If element is first in used list, adapt head of used list, otherwise link to last entry of used list. */
        if ( elemIdxUsed >= DoIPInt_GetSizeOfTxQueue() )
        {
          DoIPInt_SetTxQueueUsedListStartIdxOfConnectionDyn(ConnectionIdx, elemIdxToInsert);
        }
        else
        {
          DoIPInt_SetNextListElementOfTxQueue(elemIdxUsed, elemIdxToInsert);
        }

        /* #100 Update tail/head pointers of lists and the number of free elements in queue. */
        DoIPInt_SetTxQueueUnusedListEndIdxOfConnectionDyn(ConnectionIdx, elemIdxPreviousUnused);
        DoIPInt_SetTxQueueUsedListEndIdxOfConnectionDyn(ConnectionIdx, elemIdxToInsert);
        DoIPInt_DecTxQueueUnusedElementsOfConnectionDyn(ConnectionIdx);

        /* #110 Add new entry to OEM Tx buffer or channel lookup table. */
        DoIPInt_SetTxQueueIdxOfTxQueueLookUp(txQueuelookUpIdx, elemIdxToInsert);

        retVal = E_OK;
      }
    }
  }

  /* #120 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();

  return retVal;
} /* DoIPInt_TxQueue_Add() */

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_GetNext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TxQueue_GetNext(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint32, AUTOMATIC, DOIPINT_VAR) HandleIdxPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) LengthPtr,
  P2VAR(DoIPInt_SizeOfMsgHandlerType, AUTOMATIC, DOIPINT_VAR) MsgHandlerIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent queue modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Check if queue contains at least one element. */
  if ( DoIPInt_GetTxQueueUsedListStartIdxOfConnectionDyn(ConnectionIdx) != DoIPInt_GetSizeOfTxQueue() )
  {
    const DoIPInt_SizeOfTxQueueType txQueueIdx = DoIPInt_GetTxQueueUsedListStartIdxOfConnectionDyn(ConnectionIdx);

    /* #30 Return first element of queue. */
    *HandleIdxPtr = DoIPInt_GetTxRequestHandleOfTxQueue(txQueueIdx);
    *LengthPtr = DoIPInt_GetTxRequestLengthOfTxQueue(txQueueIdx);
    *MsgHandlerIdxPtr = DoIPInt_GetMsgHandlerIdxOfTxQueue(txQueueIdx);
    retVal = E_OK;
  }

  /* #40 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();

  return retVal;
} /* DoIPInt_TxQueue_GetNext() */

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_Remove()
 *********************************************************************************************************************/
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
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_TxQueue_Remove(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent queue modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Ensure that the derived index for the lookup table is inside valid range. */
  if ( HandleIdx < ((uint32)DoIPInt_GetTxQueueLookUpEndIdxOfMsgHandler(MsgHandlerIdx) -
    (uint32)DoIPInt_GetTxQueueLookUpStartIdxOfMsgHandler(MsgHandlerIdx)) )
  {
    const DoIPInt_SizeOfTxQueueLookUpType txQueuelookUpIdx = (DoIPInt_SizeOfTxQueueLookUpType)HandleIdx +
      DoIPInt_GetTxQueueLookUpStartIdxOfMsgHandler(MsgHandlerIdx);

    /* #30 Look up index of element to remove. */
    const DoIPInt_SizeOfTxQueueType elemIdxToRemove = DoIPInt_GetTxQueueIdxOfTxQueueLookUp(txQueuelookUpIdx);

    /* #40 Ensure that queue entry for requested handle (channel or OEM Tx buffer) exists. */
    if ( elemIdxToRemove < DoIPInt_GetSizeOfTxQueue() )
    {
      /* #50 Get index of next and previous element of the queue entry that shall be removed. */
      DoIPInt_SizeOfTxQueueType elemIdxNextUsed = DoIPInt_GetNextListElementOfTxQueue(elemIdxToRemove);
      DoIPInt_SizeOfTxQueueType elemIdxPrevUsed = DoIPInt_GetPrevListElementOfTxQueue(elemIdxToRemove);
      DoIPInt_SizeOfTxQueueType elemIdxUnused = DoIPInt_GetTxQueueUnusedListEndIdxOfConnectionDyn(ConnectionIdx);

      /* #60 If element is first in used list, adapt head of used list,
         otherwise link predecessor of element to following element. */
      if ( elemIdxPrevUsed >= DoIPInt_GetSizeOfTxQueue() )
      {
        DoIPInt_SetTxQueueUsedListStartIdxOfConnectionDyn(ConnectionIdx, elemIdxNextUsed);
      }
      else
      {
        DoIPInt_SetNextListElementOfTxQueue(elemIdxPrevUsed, elemIdxNextUsed);
      }

      /* #70 If element is last in used list, adapt tail of used list, otherwise link following element to
       *     predecessor. */
      if ( elemIdxNextUsed >= DoIPInt_GetSizeOfTxQueue() )
      {
        DoIPInt_SetTxQueueUsedListEndIdxOfConnectionDyn(ConnectionIdx, elemIdxPrevUsed);
      }
      else
      {
        DoIPInt_SetPrevListElementOfTxQueue(elemIdxNextUsed, elemIdxPrevUsed);
      }

      /* #80 Update links to append removed element to end of unused list. */
      if ( elemIdxUnused < DoIPInt_GetSizeOfTxQueue() )
      {
        DoIPInt_SetNextListElementOfTxQueue(elemIdxUnused, elemIdxToRemove);
      }
      DoIPInt_SetPrevListElementOfTxQueue(elemIdxToRemove, elemIdxUnused);
      DoIPInt_SetNextListElementOfTxQueue(elemIdxToRemove, DoIPInt_GetSizeOfTxQueue());

      /* #90 Set tail of unused list to removed element and increase number of free elements. */
      DoIPInt_SetTxQueueUnusedListEndIdxOfConnectionDyn(ConnectionIdx, elemIdxToRemove);
      DoIPInt_IncTxQueueUnusedElementsOfConnectionDyn(ConnectionIdx);

      /* #100 Invalidate entry of channel lookup table. */
      /*@ assert txQueuelookUpIdx < DoIPInt_GetSizeOfTxQueueLookUp(); */                                               /* VCA_DOIPINT_IDX_VALIDITY_CHECKED_CSL03 */
      DoIPInt_SetTxQueueIdxOfTxQueueLookUp(txQueuelookUpIdx, DoIPInt_GetSizeOfTxQueue());
    }
  }

  /* #110 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();
} /* DoIPInt_TxQueue_Remove() */

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_IsChannelInQueue()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DOIPINT_CODE) DoIPInt_TxQueue_IsChannelInQueue(
  DoIPInt_SizeOfChannelType ChannelIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get queue index of channel. */
  const DoIPInt_SizeOfTxQueueType queueIdx = DoIPInt_GetTxQueueIdxOfTxQueueLookUp(ChannelIdx);

  /* #20 Return true if channel is in queue, otherwise return false. */
  if ( queueIdx < DoIPInt_GetSizeOfTxQueue() )
  {
    retVal = TRUE;
  }

  return retVal;
} /* DoIPInt_TxQueue_IsChannelInQueue() */

#define DOIPINT_STOP_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_TxQueue.c
 *********************************************************************************************************************/
