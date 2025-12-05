/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup Os_XSignalChannel
 *
 * \{
 *
 * \file
 * \brief       Contains the buffer interface.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#ifndef OS_XSIGNALCHANNEL_H
# define OS_XSIGNALCHANNEL_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_XSignalChannelInt.h"

/* Os kernel module dependencies */
# include "Os_XSignal.h"
# include "Os_Common.h"
# include "Os_Error.h"

/* Os hal dependencies */
# include "Os_Hal_Compiler.h"




/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_XSigChannelIsResultAvailable()
 **********************************************************************************************************************/
/*! \brief          Returns whether further result elements are available or not.
 *  \details        The function performs a load-acquire operation on the ReceiveIdx of the channel that synchronizes
 *                  with the increment on the receiver. If this function returns true, the reply at the current
 *                  ResultIdx has certainly been written by the receiver.
 *
 *  \param[in]      Channel  The Channel to query. Parameter must not be NULL.
 *  \param[out]     ResultIdxCopy  A pointer to store a local copy of the current result index. Parameter must not be NULL.
 *
 *  \retval         !0    Further result data is available.
 *  \retval         0     No more result data available.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_XSigChannelIsResultAvailable,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel,
  P2VAR(Os_XSigBufferIdxType, AUTOMATIC, OS_APPL_DATA) ResultIdxCopy
));


/***********************************************************************************************************************
 *  Os_XSigChannelIsReceiveElementAvailable()
 **********************************************************************************************************************/
/*! \brief          Returns whether the buffer contains receive elements.
 *  \details        Also checks whether the send index is valid.
 *
 *  \param[in]      Channel The Channel to query. Parameter must not be NULL.
 *  \param[out]     ReceiveIdxCopy A pointer to store a local copy of the receive index. Parameter must not be NULL.
 *
 *  \retval         !0    If receive data is available.
 *  \retval         0     If no receive data is available.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,                                                /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_XSigChannelIsReceiveElementAvailable,
(
  P2CONST(Os_XSigChannelRecvConfigType, AUTOMATIC, OS_APPL_DATA) Channel,
  P2VAR(Os_XSigBufferIdxType, AUTOMATIC, OS_APPL_DATA) ReceiveIdxCopy
));


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_XSigChannelIdxIncrement
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
Os_XSigChannelIdxIncrement,
(
  P2VAR(Os_XSigBufferIdxType volatile, AUTOMATIC, OS_APPL_DATA) Index,
  Os_XSigBufferIdxType Size,
  Os_XSigWriteVisibilityType Visibility
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  const Os_XSigBufferIdxType oldValue = *Index;
  Os_XSigBufferIdxType newValue;
  Os_Assert((Os_StdReturnType)(oldValue < Size));                                                                     /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

  /* #10  Compute new value and check for overflow. */
  newValue = oldValue + 1u;
  if(OS_UNLIKELY(newValue == Size))
  {
    newValue = 0u;
  }

  /* #20  Store new value. */
  if(Visibility == OS_XSIG_WRITE_LOCAL)
  {
    *Index = newValue;                                                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
  else
  {
    Os_Hal_StoreRelease16(Index, newValue);                                                                             /* SBSW_OS_FC_PRECONDITION */
  }
}


/***********************************************************************************************************************
 *  Channel Send Functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_XSigChannelSendInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
Os_XSigChannelSendInit,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  Os_XSigBufferIdx_IteratorType comDataIdx;

  /* #10  Initialize all indices to zero. */
  Channel->Dyn->SendIdx = 0;                                                                                            /* SBSW_OS_XSIG_XSIGCHANNELSEND_001 */
  Channel->Dyn->ResultIdx = 0;                                                                                          /* SBSW_OS_XSIG_XSIGCHANNELSEND_001 */

  /* #20 Mark each buffer element as unused. */
  for(comDataIdx = 0; comDataIdx < (Os_XSigBufferIdx_IteratorType) Channel->BufferSize; ++comDataIdx)                   /* FETA_OS_CONSTANTASFUNCTIONPARAMETER */
  {
    Channel->Buffer[comDataIdx].SendData->Acknowledge = OS_XSIGSTATE_ACK;                                               /* SBSW_OS_XSIG_CHANNEL_SENDDATA_001 */
  }
}


/***********************************************************************************************************************
 *  Os_XSigChannelGetSendElement()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE                                                                            /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_CONST), OS_CODE, OS_ALWAYS_INLINE, Os_XSigChannelGetSendElement,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_CONST) comData = NULL_PTR;
  Os_XSigBufferIdxType nextSendIdx;
  Os_XSigBufferIdxType sendIdx;
  Os_XSigBufferIdxType resultIdx;

  /* #10 Iterate, beginning with the first send element. */
  sendIdx = Channel->Dyn->SendIdx;
  nextSendIdx = sendIdx;
  Os_XSigChannelIdxIncrement(&nextSendIdx, Channel->BufferSize, OS_XSIG_WRITE_LOCAL);                                   /* SBSW_OS_FC_POINTER2LOCAL */

  /* #20 Iterate while there are further elements in the queue: */
  resultIdx = Channel->Dyn->ResultIdx;
  while(nextSendIdx != resultIdx)                                                                                       /* FETA_OS_XSIGCHANNELINDEX */
  {
    /* #30 If the result of this element has already been handled by the receiver: */
    if(Channel->Buffer[sendIdx].SendData->Acknowledge == OS_XSIGSTATE_ACK)
    {
      /* #40 Take the current send element and stop searching. */
      comData = &(Channel->Buffer[sendIdx]);
      break;
    }

    /* #50 Select the next element. */
    sendIdx = nextSendIdx;
    Os_XSigChannelIdxIncrement(&nextSendIdx, Channel->BufferSize, OS_XSIG_WRITE_LOCAL);                                 /* SBSW_OS_FC_POINTER2LOCAL */
  }
  /*!
   * Internal comment removed.
 *
 *
 *
 *
 *
   */
  Os_Hal_StoreRelease16(&(Channel->Dyn->SendIdx), sendIdx);                                                             /* SBSW_OS_XSIG_STORERELEASE_001 */

  /* #60 Update the send index. */
  return comData;
}


/***********************************************************************************************************************
 *  Os_XSigChannelSend()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
Os_XSigChannelSend,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  Os_XSigChannelIdxIncrement(&(Channel->Dyn->SendIdx), Channel->BufferSize, OS_XSIG_WRITE_SYNCRONIZED);                 /* SBSW_OS_XSIG_XSIGCHANNELIDXINCREMENT_001 */
}


/***********************************************************************************************************************
 *  Os_XSigChannelTryGetResult()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE                                                                            /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_CONST), OS_CODE, OS_ALWAYS_INLINE, Os_XSigChannelTryGetResult,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_CONST) result = NULL_PTR;
  Os_XSigBufferIdxType CurrentResultIdx;

  /* #10 If a result is availble: */
  if(Os_XSigChannelIsResultAvailable(Channel, &CurrentResultIdx) != 0u)                                                 /* SBSW_OS_XSIG_XSIGCHANNELISRESULTAVAILABLE_001 */
  {
    /* #20 Return the next result element. */
    result = &(Channel->Buffer[CurrentResultIdx]);
  }
  return result;
}


/***********************************************************************************************************************
 *  Os_XSigChannelFreeResult()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
Os_XSigChannelFreeResult,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10 Increment the result index counter. */
  /*!
   * Internal comment removed.
 *
 *
   */
  Os_XSigChannelIdxIncrement(&(Channel->Dyn->ResultIdx), Channel->BufferSize, OS_XSIG_WRITE_LOCAL);                     /* SBSW_OS_XSIG_XSIGCHANNELIDXINCREMENT_001 */
}


/***********************************************************************************************************************
 *  Os_XSigChannelAcknowledgeResult()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_XSigChannelAcknowledgeResult,
(
  P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_APPL_DATA) ComData
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10 Acknowledge the communication data element. */
  ComData->SendData->Acknowledge = OS_XSIGSTATE_ACK;                                                                    /* SBSW_OS_XSIG_XSIGCHANNELSENDDATA_001 */
}


/***********************************************************************************************************************
 *  Os_XSigChannelIsResultAvailable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CONST,                                                /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
OS_ALWAYS_INLINE, Os_XSigChannelIsResultAvailable,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel,
  P2VAR(Os_XSigBufferIdxType, AUTOMATIC, OS_APPL_DATA) ResultIdxCopy
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  Os_XSigBufferIdxType receiveIdx;
  Os_XSigBufferIdxType resultIdx;

  receiveIdx = Os_Hal_LoadAcquire16(&(Channel->Receive->ReceiveIdx));                                                   /* SBSW_OS_XSIG_LOADACQUIRE_001 */
  resultIdx = Channel->Dyn->ResultIdx;
  *ResultIdxCopy = resultIdx;                                                                                           /* SBSW_OS_PWA_PRECONDITION */
  return (receiveIdx != resultIdx);                                                                                     /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
}


/***********************************************************************************************************************
 *  Os_XSigChannelIsRequestHandled()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CONST,                                                /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
OS_ALWAYS_INLINE, Os_XSigChannelIsRequestHandled,
(
  P2CONST(Os_XSigChannelSendConfigType, AUTOMATIC, OS_APPL_DATA) Channel,
  P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_CONST) ComData
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  Os_StdReturnType result;
  Os_XSigBufferIdxType receiveIdx;
  Os_XSigBufferIdxType resultIdx;
  Os_XSigBufferIdxType acknowledge;

  /* Ensure that the index of ComData is in range. */
  Os_Assert((Os_StdReturnType)(Channel->BufferSize > ComData->Idx));                                                    /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

  /* Ensure that ComData is a member of the given Channel. */
  Os_Assert((Os_StdReturnType)(&(Channel->Buffer[ComData->Idx]) == ComData));                                           /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

  /*!
   * Internal comment removed.
 *
 *
 *
 *
   */
  acknowledge = Os_Hal_LoadAcquire16(&(ComData->SendData->Acknowledge));                                                /* SBSW_OS_XSIG_LOADACQUIRE_002 */
  /* #20 If the requested is marked as pending, it is available. */
  if(OS_UNLIKELY(acknowledge == OS_XSIGSTATE_SYNC_PENDING))
  {
    /* This case may occur, if the synchronous waiting loop is interrupted by a higher priority thread,
     * which sends a cross core request. */
    result = !0u;                                                                                                       /* PRQA S 4404, 4558 */  /* MD_Os_C90BooleanCompatibility, MD_Os_Rule10.1_4558 */
  }
  else
  {

    /* #25 Take a snapshot of the result and the receive indices. */
    receiveIdx = Os_Hal_LoadAcquire16(&(Channel->Receive->ReceiveIdx));                                                 /* SBSW_OS_XSIG_LOADACQUIRE_001 */
    resultIdx = Channel->Dyn->ResultIdx;

    /* #30 If the buffer is empty return '0'. */
    if(receiveIdx == resultIdx)
    {
      result = 0u;
    }
    /* #40 In case that there was no wrap around of the receive counter, check that the ComDataIdx is within both. */
    else if(receiveIdx > resultIdx)
    {
      /* There was no wrap around of the receiveIdx. */
      result = (ComData->Idx >= resultIdx) && (ComData->Idx < receiveIdx);                                              /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */ /* COV_OS_NOT_DETERMINISTICALLY_TESTABLE */
    }
    /* #50 Otherwise test whether the ComDataIdx is not within both indices. */
    else
    {
      /* There was a wrap around of the receiveIdx. */
      result = (ComData->Idx >= resultIdx) || (ComData->Idx < receiveIdx);                                              /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */ /* COV_OS_NOT_DETERMINISTICALLY_TESTABLE */
    }
  }

  return result;
}


/***********************************************************************************************************************
 *  Channel Receive Functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_XSigChannelReceiveInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
Os_XSigChannelReceiveInit,
(
  P2CONST(Os_XSigChannelRecvConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #10  Initialize all indices to zero. */
  Channel->Dyn->ReceiveIdx = 0u;                                                                                        /* SBSW_OS_XSIG_XSIGCHANNELRECV_001 */
}


/***********************************************************************************************************************
 *  Os_XSigChannelGetReceiveElement()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE                                                                            /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE, Os_XSigChannelGetReceiveElement,
(
  P2CONST(Os_XSigChannelRecvConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  Os_XSigBufferIdxType currentReceiveIdx;
  P2CONST(Os_XSigComDataConfigType, AUTOMATIC, OS_VAR_NOINIT) result = NULL_PTR;

  /* #10 If a receive element is available: */
  if(Os_XSigChannelIsReceiveElementAvailable(Channel, &currentReceiveIdx) != 0u)                                        /* SBSW_OS_XSIG_XSIGCHANNELISRECEIVEELEMENTAVAILABLE_001 */
  {
    /* #20 Return the current receive element. */
    result = &(Channel->Buffer[currentReceiveIdx]);
  }
  return result;
}


/***********************************************************************************************************************
 *  Os_XSigChannelReply()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
Os_XSigChannelReply,
(
  P2CONST(Os_XSigChannelRecvConfigType, AUTOMATIC, OS_APPL_DATA) Channel
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  /* #30  Increment the receive index counter. */
  Os_XSigChannelIdxIncrement(&(Channel->Dyn->ReceiveIdx), Channel->BufferSize, OS_XSIG_WRITE_SYNCRONIZED);              /* SBSW_OS_XSIG_XSIGCHANNELIDXINCREMENT_002 */
}


/***********************************************************************************************************************
 *  Os_XSigChannelIsReceiveElementAvailable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CONST,                                                /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */ /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
OS_ALWAYS_INLINE, Os_XSigChannelIsReceiveElementAvailable,
(
  P2CONST(Os_XSigChannelRecvConfigType, AUTOMATIC, OS_APPL_DATA) Channel,
  P2VAR(Os_XSigBufferIdxType, AUTOMATIC, OS_APPL_DATA) ReceiveIdxCopy
))                                                                                                                      /* COV_OS_HALPLATFORMMULTICOREUNSUPPORTED */
{
  const Os_XSigBufferIdxType sendIdx = Os_Hal_LoadAcquire16(&Channel->Send->SendIdx);                                   /* SBSW_OS_XSIG_LOADACQUIRE_001 */
  Os_XSigBufferIdxType receiveIdx = Channel->Dyn->ReceiveIdx;
  Os_StdReturnType result = 0u;

  /* #10 If the send index is valid: */
  if(Os_ErrIsValueLo(sendIdx, Channel->BufferSize) != 0u)
  {
    /* #20 Check if elements have been sent. */
    result = (receiveIdx != sendIdx);                                                                                   /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
  }

  /* #30 Store the current receive index and return. */
  *ReceiveIdxCopy = receiveIdx;                                                                                         /* SBSW_OS_PWA_PRECONDITION */
  return result;
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */




#endif /* OS_XSIGNALCHANNEL_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_XSignalChannel.h
 **********************************************************************************************************************/
