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
/*!        \file  EthTSyn_SwtMgmtQ.c
 *         \unit  SwtMgmtQ
 *        \brief  EthTSyn SwtMgmtQ source file
 *      \details  Implementation of the SwtMgmtQ unit.
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

#define ETHTSYN_SWT_MGMT_Q_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
# include "EthTSyn.h"
# include "EthTSyn_SwtMgmtQ_Int.h"
# include "EthTSyn_SwtMgmtRx_Int.h"
# include "EthTSyn_SwtMgmtTx_Int.h"
# include "EthTSyn_Timestamp_Int.h"
# include "SchM_EthTSyn.h"

# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif /* ETHTSYN_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_IncBufIdx
 *********************************************************************************************************************/
/*! \brief          Increments index of SwtMgmtQ ring buffer and handles overflow if necessary.
 *  \details        -
 *  \param[in]      SwtMgmtQIdx       SwtMgmtQ instance index
 *                                    [range: SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ()]
 *  \param[in,out]  SwtMgmtQBufIdxPtr SwtMgmtQ buffer index to be incremented
 *                                    [range: (*SwtMgmtQBufIdxPtr) < EthTSyn_GetSizeOfSwtMgmtQBuf()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ();
 *    requires (*SwtMgmtQBufIdxPtr) < EthTSyn_GetSizeOfSwtMgmtQBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_IncBufIdx(
                EthTSyn_SwtMgmtQIterType     SwtMgmtQIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmtQ_BufIdxType) SwtMgmtQBufIdxPtr);

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_GetIterBufIdx
 *********************************************************************************************************************/
/*! \brief          Gets the actual SwtMgmtQBuf index when iterating over the buffers using a virtual index ranging
 *                  from 0u to EthTSyn_GetSwtMgmtQBufLengthOfSwtMgmtQ(SwtMgmtQIdx).
 *  \details        -
 *  \param[in]      SwtMgmtQIdx    SwtMgmtQ instance index
 *                                 [range: SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ()]
 *  \param[in]      CurrProcBufIdx SwtMgmtQ buffer to be first processed during the iteration
 *                                 [range: CurrProcBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf()]
 *  \param[in]      BufIter        Virtual buffer iteration index
 *                                 [range: BufIter < EthTSyn_GetSizeOfSwtMgmtQBuf()]
 *  \return         SwtMgmtQ buffer index of the current iteration step
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ();
 *    requires CurrProcBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf();
 *    requires BufIter < EthTSyn_GetSizeOfSwtMgmtQBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SwtMgmtQ_BufIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_GetIterBufIdx(
  EthTSyn_SwtMgmtQIterType    SwtMgmtQIdx,
  EthTSyn_SwtMgmtQ_BufIdxType CurrProcBufIdx,
  EthTSyn_SwtMgmtQBufIterType BufIter);

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_Poll
 *********************************************************************************************************************/
/*! \brief          Checks for every requested switch management object if it is available and copies information
 *                  into internal buffer.
 *  \details        -
 *  \param[in]      SwtMgmtQIdx    SwtMgmtQ instance index
 *                                 [range: SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Poll(
  EthTSyn_SwtMgmtQIterType SwtMgmtQIdx);

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_Proc
 *********************************************************************************************************************/
/*! \brief          Processes information of all available SwtMgmtQ buffer in chronological order.
 *  \details        -
 *  \param[in]      SwtMgmtQIdx    SwtMgmtQ instance index
 *                                 [range: SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Proc(
  EthTSyn_SwtMgmtQIterType SwtMgmtQIdx);

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_CopyMgmtObj
 *********************************************************************************************************************/
/*! \brief          If referenced switch management object is in state OWNED_BY_UPPER_LAYER, copies all available
 *                  information into the internal SwtMgmtQ buffer and releases management object.
 *  \details        -
 *  \param[in]      SwtMgmtQBufIdx SwtMgmtQ buffer index
 *                                 [range: SwtMgmtQBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf()]
 *  \return         E_OK - Switch management object information were successfully copied
 *  \return         E_NOT_OK - Switch management object is not yet available
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SwtMgmtQBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_CopyMgmtObj(
  EthTSyn_SwtMgmtQBufIterType SwtMgmtQBufIdx);

# if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_CopyAddInfo
 *********************************************************************************************************************/
/*! \brief          Copies the additional information of a switch management object (if possible and required).
 *  \details        -
 *  \param[in]      SwtMgmtQBufIdx  SwtMgmtQ buffer index
 *                                  [range: SwtMgmtQBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf()]
 *  \param[in]      AddInfoPtr      The additional information to copy
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SwtMgmtQBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_CopyAddInfo(
                  EthTSyn_SwtMgmtQBufIterType    SwtMgmtQBufIdx,
  ETHTSYN_P2CONST(EthSwt_VMgmtObjectAddInfoType) AddInfoPtr);
# endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_ProcMgmtObj
 *********************************************************************************************************************/
/*! \brief          Forwards required information of SwtMgmtQ buffer to user of SwtMgmtQ instance.
 *  \details        -
 *  \param[in]      SwtMgmtQBufIdx SwtMgmtQ buffer index
 *                                 [range: SwtMgmtQBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf()]
 *  \pre            SwtMgmtQ buffer must be in state AVAILABLE
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SwtMgmtQBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_ProcMgmtObj(
  EthTSyn_SwtMgmtQBufIterType SwtMgmtQBufIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_IncBufIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_IncBufIdx(
                EthTSyn_SwtMgmtQIterType     SwtMgmtQIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmtQ_BufIdxType) SwtMgmtQBufIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtQ_BufIdxType incBufIdx = (*SwtMgmtQBufIdxPtr) + 1u;

  /* ----- Implementation ----------------------------------------------- */
  if (incBufIdx >= EthTSyn_GetSwtMgmtQBufEndIdxOfSwtMgmtQ(SwtMgmtQIdx))
  {
    incBufIdx = EthTSyn_GetSwtMgmtQBufStartIdxOfSwtMgmtQ(SwtMgmtQIdx);
  }

  (*SwtMgmtQBufIdxPtr) = incBufIdx;
} /* EthTSyn_SwtMgmtQ_IncBufIdx() */

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_GetIterBufIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SwtMgmtQ_BufIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_GetIterBufIdx(
  EthTSyn_SwtMgmtQIterType    SwtMgmtQIdx,
  EthTSyn_SwtMgmtQ_BufIdxType CurrProcBufIdx,
  EthTSyn_SwtMgmtQBufIterType BufIter)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Use a wider datatype to avoid overflow during addition of values. */
  uint16 bufIdx = (uint16)CurrProcBufIdx + (uint16)BufIter;

  /* ----- Implementation ----------------------------------------------- */
  if (bufIdx >= EthTSyn_GetSwtMgmtQBufEndIdxOfSwtMgmtQ(SwtMgmtQIdx))
  {
    bufIdx -= EthTSyn_GetSwtMgmtQBufLengthOfSwtMgmtQ(SwtMgmtQIdx);
  }

return (EthTSyn_SwtMgmtQ_BufIdxType)bufIdx;
} /* EthTSyn_SwtMgmtQ_GetIterBufIdx() */

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_Poll
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
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Poll(
  EthTSyn_SwtMgmtQIterType SwtMgmtQIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtQ_BufIdxType currProcBufIdx = 0u;
  EthTSyn_SwtMgmtQ_BufIdxType nextFreeBufIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  currProcBufIdx = EthTSyn_GetSwtMgmtQBufCurrProcIdxOfSwtMgmtQDyn(SwtMgmtQIdx);
  nextFreeBufIdx = EthTSyn_GetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #10 Check that queue is not empty.
   *       Iterate over the whole SwtMgmtQ buffer range using a virtual iteration index ranging from 0 to
   *       EthTSyn_GetSwtMgmtQBufLengthOfSwtMgmtQ().
   *         Calculate the actual SwtMgmtQ buffer index using current processed index.
   *         Stop iteration after last requested buffer. */
  if (currProcBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf())
  {
    EthTSyn_SwtMgmtQBufIterType bufIter = 0u; /* Must not be used to access SwtMgmtQBuf arrays. */
    for (bufIter = 0u; bufIter < EthTSyn_GetSwtMgmtQBufLengthOfSwtMgmtQ(SwtMgmtQIdx); bufIter++)
    {
      const EthTSyn_SwtMgmtQ_BufIdxType swtMgmtQBufIdx = EthTSyn_SwtMgmtQ_GetIterBufIdx(SwtMgmtQIdx, currProcBufIdx,
        bufIter);
      /*@ assert swtMgmtQBufIdx < EthTSyn_GetSwtMgmtQBufEndIdxOfSwtMgmtQ(SwtMgmtQIdx); */                              /* VCA_ETHTSYN_SWT_MGMT_Q_BUF_ITER_IDX */

      if (swtMgmtQBufIdx == nextFreeBufIdx)
      {
        /* If queue is full, nextFreeBufIdx is invalid and this breaking condition is never met. */
        break;
      }

      /* #20 Check if SwtMgmtQ buffer is in state REQUESTED.
       *       Try to copy all required information of switch management object.
       *         Set state of SwtMgmtQ buffer to to AVAILABLE. */
      if (EthTSyn_GetStateOfSwtMgmtQBufDyn(swtMgmtQBufIdx) == ETHTSYN_REQUESTED_STATEOFSWTMGMTQBUFDYN)
      {
        if (EthTSyn_SwtMgmtQ_CopyMgmtObj(swtMgmtQBufIdx) == E_OK)
        {
          EthTSyn_SetStateOfSwtMgmtQBufDyn(swtMgmtQBufIdx, ETHTSYN_AVAILABLE_STATEOFSWTMGMTQBUFDYN);
        }
      }
    }
  }
} /* EthTSyn_SwtMgmtQ_Poll() */

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_Proc
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
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Proc(
  EthTSyn_SwtMgmtQIterType SwtMgmtQIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtQ_BufIdxType currProcBufIdx = 0u;
  EthTSyn_SwtMgmtQ_BufIdxType nextFreeBufIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  currProcBufIdx = EthTSyn_GetSwtMgmtQBufCurrProcIdxOfSwtMgmtQDyn(SwtMgmtQIdx);
  nextFreeBufIdx = EthTSyn_GetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #10 Check that queue is not empty.
   *       Iterate over the whole SwtMgmtQ buffer range using a virtual iteration index ranging from 0 to
   *       EthTSyn_GetSwtMgmtQBufLengthOfSwtMgmtQ().
   *         Calculate the actual SwtMgmtQ buffer index using current processed index. */
  if (currProcBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf())
  {
    EthTSyn_SwtMgmtQ_BufIdxType lastProcBufIdx = EthTSyn_GetSizeOfSwtMgmtQBuf();
    EthTSyn_SwtMgmtQBufIterType bufIter = 0u; /* Must not be used to access SwtMgmtQBuf arrays. */

    for (bufIter = 0u; bufIter < EthTSyn_GetSwtMgmtQBufLengthOfSwtMgmtQ(SwtMgmtQIdx); bufIter++)
    {
      boolean stopIteration = FALSE;

      const EthTSyn_SwtMgmtQ_BufIdxType swtMgmtQBufIdx = EthTSyn_SwtMgmtQ_GetIterBufIdx(SwtMgmtQIdx, currProcBufIdx,
        bufIter);
      /*@ assert swtMgmtQBufIdx < EthTSyn_GetSwtMgmtQBufEndIdxOfSwtMgmtQ(SwtMgmtQIdx); */                              /* VCA_ETHTSYN_SWT_MGMT_Q_BUF_ITER_IDX */

      if (swtMgmtQBufIdx == nextFreeBufIdx) /* Check if there are anymore buffer to process. */
      {
        /* If queue is full, nextFreeBufIdx is invalid and this breaking condition is never met. */
        stopIteration = TRUE;
      }
      else
      {
        /* #20 If buffer is available, process stored information.
         *     Otherwise, stop iteration to enforce chronological processing. */
        if (EthTSyn_GetStateOfSwtMgmtQBufDyn(swtMgmtQBufIdx) == ETHTSYN_AVAILABLE_STATEOFSWTMGMTQBUFDYN)
        {
          EthTSyn_SwtMgmtQ_ProcMgmtObj(swtMgmtQBufIdx);
          lastProcBufIdx = swtMgmtQBufIdx;

          EthTSyn_SetStateOfSwtMgmtQBufDyn(swtMgmtQBufIdx, ETHTSYN_UNUSED_STATEOFSWTMGMTQBUFDYN);
        }
        else
        {
          /* Iteration is stopped here, to ensure that buffers are processed in the same order as they were queued. */
          stopIteration = TRUE;
        }
      }

      if (stopIteration == TRUE)
      {
        break;
      }
    }

    /* #30 Store new current processed and next free buffer indices while protected against interrupts. */
    if (lastProcBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf()) /* Checks if any buffer was processed. */
    {
      EthTSyn_SwtMgmtQ_BufIdxType newCurrProcBufIdx = lastProcBufIdx;
      EthTSyn_SwtMgmtQ_IncBufIdx(SwtMgmtQIdx, &newCurrProcBufIdx);

      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      /* Checks if queue was full at the beginning of processing. */
      if (nextFreeBufIdx >= EthTSyn_GetSizeOfSwtMgmtQBuf())
      {
        EthTSyn_SetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx, currProcBufIdx);
      }

      if (newCurrProcBufIdx == EthTSyn_GetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx))
      {
        /* Store invalid index if all requested buffer were processed. */
        EthTSyn_SetSwtMgmtQBufCurrProcIdxOfSwtMgmtQDyn(SwtMgmtQIdx, EthTSyn_GetSizeOfSwtMgmtQBuf());
      }
      else
      {
        EthTSyn_SetSwtMgmtQBufCurrProcIdxOfSwtMgmtQDyn(SwtMgmtQIdx, newCurrProcBufIdx);
      }
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    }
  }
} /* EthTSyn_SwtMgmtQ_Proc() */

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_CopyMgmtObj
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_CopyMgmtObj(
  EthTSyn_SwtMgmtQBufIterType SwtMgmtQBufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_SwtMgmt_EthSwtMgmtObjPtrType ethSwtMgmtObjPtr = EthTSyn_GetEthSwtMgmtObjPtrOfSwtMgmtQBufDyn(SwtMgmtQBufIdx);
  /*@ assert $external(ethSwtMgmtObjPtr); */                                                                           /* VCA_ETHTSYN_ETH_SWT_MGMT_OBJ */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if switch management info management object is in state OWNED_BY_UPPER_LAYER. */
  if (ethSwtMgmtObjPtr->Ownership == ETHSWT_MGMT_OBJ_OWNED_BY_UPPER_LAYER)
  {
    EthTSyn_SwtMgmt_MgmtInfoMgmtStructType mgmtInfoMgmt = {0u};
    EthTSyn_TsMgmtType swtEgrTsMgmt = {0u};
    EthTSyn_TsMgmtType swtIngrTsMgmt = {0u};

    /* #20 Copy switch management info management object. */
    if (ethSwtMgmtObjPtr->Validation.MgmtInfoValid == E_OK)
    {
      mgmtInfoMgmt.MgmtInfo.SwitchIdx = ethSwtMgmtObjPtr->MgmtInfo.SwitchIdx;
      mgmtInfoMgmt.MgmtInfo.PortIdx = ethSwtMgmtObjPtr->MgmtInfo.PortIdx;
      mgmtInfoMgmt.IsValid = TRUE;
    }
    else
    {
      mgmtInfoMgmt.IsValid = FALSE;
    }
    EthTSyn_SetSwtMgmtInfoOfSwtMgmtQBufDyn(SwtMgmtQBufIdx, mgmtInfoMgmt);

    /* #30 Copy switch ingress and egress timestamp management object. */
    if (ethSwtMgmtObjPtr->Validation.IngressTimestampValid == E_OK)
    {
      swtIngrTsMgmt.Ts.secondsHi = ethSwtMgmtObjPtr->IngressTimestamp.secondsHi;
      swtIngrTsMgmt.Ts.seconds = ethSwtMgmtObjPtr->IngressTimestamp.seconds;
      swtIngrTsMgmt.Ts.nanoseconds = ethSwtMgmtObjPtr->IngressTimestamp.nanoseconds;
      swtIngrTsMgmt.TsValid = EthTSyn_Ts_IsTsValid(&swtIngrTsMgmt.Ts);
    }
    else
    {
      swtIngrTsMgmt.TsValid = FALSE;
    }
    EthTSyn_SetSwtIngressTsMgmtOfSwtMgmtQBufDyn(SwtMgmtQBufIdx, swtIngrTsMgmt);

    if (ethSwtMgmtObjPtr->Validation.EgressTimestampValid == E_OK)
    {
      swtEgrTsMgmt.Ts.secondsHi = ethSwtMgmtObjPtr->EgressTimestamp.secondsHi;
      swtEgrTsMgmt.Ts.seconds = ethSwtMgmtObjPtr->EgressTimestamp.seconds;
      swtEgrTsMgmt.Ts.nanoseconds = ethSwtMgmtObjPtr->EgressTimestamp.nanoseconds;
      swtEgrTsMgmt.TsValid = EthTSyn_Ts_IsTsValid(&swtEgrTsMgmt.Ts);
    }
    else
    {
      swtEgrTsMgmt.TsValid = FALSE;
    }
    EthTSyn_SetSwtEgressTsMgmtOfSwtMgmtQBufDyn(SwtMgmtQBufIdx, swtEgrTsMgmt);

# if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
    /* #40 Copy additional information, i.e. switch egress timestamps (if bridge mode is asymmetric transparent clock). */
    EthTSyn_SwtMgmtQ_CopyAddInfo(SwtMgmtQBufIdx, &ethSwtMgmtObjPtr->AddInfo);
# endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */

    /* #50 Set state of switch management info management object to UNUSED. */
    ethSwtMgmtObjPtr->Ownership = ETHSWT_MGMT_OBJ_UNUSED;
    EthTSyn_SetEthSwtMgmtObjPtrOfSwtMgmtQBufDyn(SwtMgmtQBufIdx, NULL_PTR);
    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_SwtMgmtQ_CopyMgmtObj() */

# if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_CopyAddInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_CopyAddInfo(
                  EthTSyn_SwtMgmtQBufIterType    SwtMgmtQBufIdx,
  ETHTSYN_P2CONST(EthSwt_VMgmtObjectAddInfoType) AddInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 copiedAddInfo = 0;

  /* ----- Implementation ----------------------------------------------- */
  if (AddInfoPtr->NumEgrTsInfos <= EthTSyn_GetSwtMgmtQEgrTsBufDynLengthOfSwtMgmtQBuf(SwtMgmtQBufIdx))
  {
    for (uint8_least egrTsInfoIter = 0u; egrTsInfoIter < AddInfoPtr->NumEgrTsInfos; egrTsInfoIter++)
    {
      const uint8_least swtMgmtQEgrTsBufDynIdx =
        EthTSyn_GetSwtMgmtQEgrTsBufDynStartIdxOfSwtMgmtQBuf(SwtMgmtQBufIdx) + (uint8_least)copiedAddInfo;
      /*@ assert swtMgmtQEgrTsBufDynIdx < EthTSyn_GetSizeOfSwtMgmtQEgrTsBufDyn(); */                                   /* VCA_ETHTSYN_EGR_TS_BUF_DYN_IDX_CALC_VALID */
      const EthTSyn_GlobalTimestampType egrTs = AddInfoPtr->EgrTsInfoPtr[egrTsInfoIter].EgressTimestamp;

      if (EthTSyn_Ts_IsTsValid(&egrTs) == TRUE)
      {
        EthTSyn_SetSwtMgmtQEgrTsBufDyn(swtMgmtQEgrTsBufDynIdx, AddInfoPtr->EgrTsInfoPtr[egrTsInfoIter]);
        copiedAddInfo++;
      }
    }
  }

  EthTSyn_SetEgrTsBufUsedElemsOfSwtMgmtQBufDyn(SwtMgmtQBufIdx, copiedAddInfo);
} /* EthTSyn_SwtMgmtQ_CopyAddInfo() */
# endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtQ_ProcMgmtObj
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_ProcMgmtObj(
  EthTSyn_SwtMgmtQBufIterType SwtMgmtQBufIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If SwtMgmtQ instance is used by SwtMgmtRx unit
   *       forward switch timestamp management objects and switch management info object */
  if ( EthTSyn_GetSwtMgmtObjUserOfSwtMgmtQBuf(SwtMgmtQBufIdx) == ETHTSYN_SWTMGMTRX_SWTMGMTOBJUSEROFSWTMGMTQBUF )
  {
    const EthTSyn_SwtMgmtObjUserIdxOfSwtMgmtQBufType swtMgmtRxIdx =
      EthTSyn_GetSwtMgmtObjUserIdxOfSwtMgmtQBuf(SwtMgmtQBufIdx);
    EthTSyn_SwtMgmtRxTsInfoType swtMgmtRxTsInfo = {0};
    EthTSyn_SwtMgmt_MgmtInfoMgmtStructType swtMgmtInfoMgmt = {0u};

    swtMgmtInfoMgmt = EthTSyn_GetSwtMgmtInfoOfSwtMgmtQBufDyn(SwtMgmtQBufIdx);
    swtMgmtRxTsInfo.SwtIngrTsMgmt = EthTSyn_GetSwtIngressTsMgmtOfSwtMgmtQBufDyn(SwtMgmtQBufIdx);
    swtMgmtRxTsInfo.SwtEgrTsMgmt = EthTSyn_GetSwtEgressTsMgmtOfSwtMgmtQBufDyn(SwtMgmtQBufIdx);

# if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
    swtMgmtRxTsInfo.AddInfo.NumEgrTsInfos = EthTSyn_GetEgrTsBufUsedElemsOfSwtMgmtQBufDyn(SwtMgmtQBufIdx);
    swtMgmtRxTsInfo.AddInfo.EgrTsInfoPtr = EthTSyn_GetAddrSwtMgmtQEgrTsBufDyn(
      EthTSyn_GetSwtMgmtQEgrTsBufDynStartIdxOfSwtMgmtQBuf(SwtMgmtQBufIdx));
# endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */

    /*@ assert swtMgmtRxIdx < EthTSyn_GetSizeOfSwtMgmtRx(); */                                                         /* VCA_ETHTSYN_MANDATORY_CHOICE_IND_CSL04 */
    EthTSyn_SwtMgmtRx_ProcMsg(swtMgmtRxIdx, &swtMgmtInfoMgmt, &swtMgmtRxTsInfo);
  }
  /* #20 If SwtMgmtQ instance is used by SwtMgmtTx unit
   *       forward switch timestamp management objects */
  else
  {
    const EthTSyn_SwtMgmtObjUserIdxOfSwtMgmtQBufType swtMgmtTxIdx =
      EthTSyn_GetSwtMgmtObjUserIdxOfSwtMgmtQBuf(SwtMgmtQBufIdx);
    EthTSyn_TsMgmtType swtIngrTsMgmt = {0u};
    EthTSyn_TsMgmtType swtEgrTsMgmt = {0u};

    swtIngrTsMgmt = EthTSyn_GetSwtIngressTsMgmtOfSwtMgmtQBufDyn(SwtMgmtQBufIdx);
    swtEgrTsMgmt = EthTSyn_GetSwtEgressTsMgmtOfSwtMgmtQBufDyn(SwtMgmtQBufIdx);

    /*@ assert swtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn(); */                                                      /* VCA_ETHTSYN_MANDATORY_CHOICE_IND_CSL04 */
    EthTSyn_SwtMgmtTx_ProcTs(swtMgmtTxIdx, &swtIngrTsMgmt, &swtEgrTsMgmt);
  }
} /* EthTSyn_SwtMgmtQ_ProcMgmtObj() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/* ----- Public interfaces of SwtMgmtQ -------------------------- */
# if !defined(ETHTSYN_NOUNIT_SWTMGMTQ)
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtQIterType swtMgmtQIter = 0u;
  EthTSyn_SwtMgmtQBufIterType swtMgmtQBufIter = 0u;
  EthTSyn_TsMgmtType invTsMgmt = {0u};
  EthTSyn_SwtMgmt_MgmtInfoMgmtStructType invMgmtInfoMgmt = {0u};

  /* ----- Implementation ----------------------------------------------- */
  for (swtMgmtQIter = 0u; swtMgmtQIter < EthTSyn_GetSizeOfSwtMgmtQ(); swtMgmtQIter++)
  {
    EthTSyn_SetInUseOfSwtMgmtQDyn(swtMgmtQIter, FALSE);
    EthTSyn_SetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(swtMgmtQIter,
      EthTSyn_GetSwtMgmtQBufStartIdxOfSwtMgmtQ(swtMgmtQIter));
    EthTSyn_SetSwtMgmtQBufCurrProcIdxOfSwtMgmtQDyn(swtMgmtQIter, EthTSyn_GetSizeOfSwtMgmtQBuf());
  }

  invTsMgmt.TsValid = FALSE;
  invMgmtInfoMgmt.IsValid = FALSE;

  for (swtMgmtQBufIter = 0u; swtMgmtQBufIter < EthTSyn_GetSizeOfSwtMgmtQBuf(); swtMgmtQBufIter++)
  {
    EthTSyn_SetStateOfSwtMgmtQBufDyn(swtMgmtQBufIter, ETHTSYN_UNUSED_STATEOFSWTMGMTQBUFDYN);
    EthTSyn_SetSwtMgmtInfoOfSwtMgmtQBufDyn(swtMgmtQBufIter, invMgmtInfoMgmt);
    EthTSyn_SetEthSwtMgmtObjPtrOfSwtMgmtQBufDyn(swtMgmtQBufIter, NULL_PTR);
    EthTSyn_SetSwtIngressTsMgmtOfSwtMgmtQBufDyn(swtMgmtQBufIter, invTsMgmt);
    EthTSyn_SetSwtEgressTsMgmtOfSwtMgmtQBufDyn(swtMgmtQBufIter, invTsMgmt);
  }
} /* EthTSyn_SwtMgmtQ_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtQIterType swtMgmtQIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (swtMgmtQIter = 0u; swtMgmtQIter < EthTSyn_GetSizeOfSwtMgmtQ(); swtMgmtQIter++)
  {
    EthTSyn_SwtMgmtQ_Poll(swtMgmtQIter);
  }

  for (swtMgmtQIter = 0u; swtMgmtQIter < EthTSyn_GetSizeOfSwtMgmtQ(); swtMgmtQIter++)
  {
    EthTSyn_SwtMgmtQ_Proc(swtMgmtQIter);
  }
} /* EthTSyn_SwtMgmtQ_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_PreparePush
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
 */
FUNC(EthTSyn_SwtMgmtQ_BufUserIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_PreparePush(
  EthTSyn_SwtMgmtQ_IdxType        SwtMgmtQIdx,
  EthTSyn_SwtMgmtQ_BufUserType    SwtMgmtQUserType,
  EthTSyn_SwtMgmtQ_BufUserIdxType InvSwtMgmtQUserIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtQ_BufUserIdxType swtMgmtQUserIdx = InvSwtMgmtQUserIdx;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  if (!EthTSyn_IsInUseOfSwtMgmtQDyn(SwtMgmtQIdx))
  {
    const EthTSyn_SwtMgmtQ_BufIdxType nextFreeIdx = EthTSyn_GetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx);

    if (nextFreeIdx < EthTSyn_GetSizeOfSwtMgmtQBuf())
    {
      if (EthTSyn_GetSwtMgmtObjUserOfSwtMgmtQBuf(nextFreeIdx) == SwtMgmtQUserType)
      {
        EthTSyn_SetInUseOfSwtMgmtQDyn(SwtMgmtQIdx, TRUE);
        swtMgmtQUserIdx = EthTSyn_GetSwtMgmtObjUserIdxOfSwtMgmtQBuf(nextFreeIdx);
      }
    }
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  return swtMgmtQUserIdx;
} /* EthTSyn_SwtMgmtQ_PreparePush() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_Push
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Push(
  EthTSyn_SwtMgmtQ_IdxType             SwtMgmtQIdx,
  EthTSyn_SwtMgmt_EthSwtMgmtObjPtrType EthSwtMgmtObjPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtQ_BufIdxType nextFreeBufIdx = 0u;
  EthTSyn_SwtMgmtQ_BufIdxType newNextFreeBufIdx = 0u;
  EthTSyn_SwtMgmtQ_BufIdxType currProcBufIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  nextFreeBufIdx = EthTSyn_GetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx);
  currProcBufIdx = EthTSyn_GetSwtMgmtQBufCurrProcIdxOfSwtMgmtQDyn(SwtMgmtQIdx);

  if (nextFreeBufIdx < EthTSyn_GetSizeOfSwtMgmtQBuf())
  {
    EthTSyn_SetEthSwtMgmtObjPtrOfSwtMgmtQBufDyn(nextFreeBufIdx, EthSwtMgmtObjPtr);
    EthTSyn_SetStateOfSwtMgmtQBufDyn(nextFreeBufIdx, ETHTSYN_REQUESTED_STATEOFSWTMGMTQBUFDYN);

    /* Checks if there is currently any switch management object requested. */
    if (currProcBufIdx >= EthTSyn_GetSizeOfSwtMgmtQBuf())
    {
      EthTSyn_SetSwtMgmtQBufCurrProcIdxOfSwtMgmtQDyn(SwtMgmtQIdx, nextFreeBufIdx);
      currProcBufIdx = nextFreeBufIdx;
    }

    newNextFreeBufIdx = nextFreeBufIdx;
    EthTSyn_SwtMgmtQ_IncBufIdx(SwtMgmtQIdx, &newNextFreeBufIdx);
    /* Checks if there is no free buffer remaining -> set invalid index */
    if (newNextFreeBufIdx == currProcBufIdx)
    {
      EthTSyn_SetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx, EthTSyn_GetSizeOfSwtMgmtQBuf());
    }
    else
    {
      EthTSyn_SetSwtMgmtQBufNextFreeIdxOfSwtMgmtQDyn(SwtMgmtQIdx, newNextFreeBufIdx);
    }

    EthTSyn_SetInUseOfSwtMgmtQDyn(SwtMgmtQIdx, FALSE);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
  else
  {
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

#  if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SWT_MGMT_Q_PUSH,
      ETHTSYN_SWT_MGMT_E_SWT_MGMT_Q_INV_FREE_BUF_IDX);
#  endif /* ETHTSYN_DEV_ERROR_REPORT == STD_ON */
  }
} /* EthTSyn_SwtMgmtQ_Push() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_CancelPush
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_CancelPush(
  EthTSyn_SwtMgmtQ_IdxType SwtMgmtQIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  EthTSyn_SetInUseOfSwtMgmtQDyn(SwtMgmtQIdx, FALSE);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_SwtMgmtQ_CancelPush() */

# endif /* ETHTSYN_NOUNIT_SWTMGMTQ */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwitchManagementRx.c
 *********************************************************************************************************************/
