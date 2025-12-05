/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!        \file     FiM_LHelper.c
 *         \entity   LHelper
 *         \brief    Source for Autosar 4 module FiM - Logical Unit Helper
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  
 *  see FiM.h
 *  
 **********************************************************************************************************************/

#define FIM_SOURCE

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Det.h"

#include "FiM_Int.h"
#include "FiM_LHelper.h"
#include "FiM_ErrorCheck.h"


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/

/*!
 * Internal comment removed.
 *
 *
 */
#if !defined (FIM_UCMPSWP_LOCAL_INLINE)                                                    /* COV_MSR_COMPATIBILITY TX */
# define FIM_UCMPSWP_LOCAL_INLINE          FIM_LOCAL_INLINE
#endif

/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/



/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/



/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/



/***********************************************************************************************************************
 *  GLOBAL (UNIT'S PUBLIC) DATA
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* \unit UBitfieldHdl */

/***********************************************************************************************************************
 * private FiM_UBitfieldHdl_TestBitInWord
 **********************************************************************************************************************/
/*! \brief                        Test if the bit at a given position is set within a word.
 *
 *  \details                      see brief
 *  \param[in]  bitfieldWord      Word with bit to test.
 *  \param[in]  bitInWordPosition Specified bit position within word.
 *  \return                       TRUE if bit is set.
 *                                FALSE if bit is not set.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UBitfieldHdl_TestBitInWord
                                          (CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                           CONST(uint8, AUTOMATIC) bitInWordPosition);

/***********************************************************************************************************************
 * private FiM_UBitfieldHdl_GetBitfieldWordIndex
 **********************************************************************************************************************/
/*! \brief                    Calculate and return the word index in a bitfield array.
 *
 *  \details                  A bitfield array is composed of a word array.
 *                            Function calculates the index of the word array where a specified bit position is located.
 *  \param[in]  bitPosition   Specified bit position.
 *  \return                   Index in word array.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(uint32, FIM_CODE) FiM_UBitfieldHdl_GetBitfieldWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition /* in  */);

/***********************************************************************************************************************
 * private FiM_UBitfieldHdl_GetBitfieldBitInWordIndex
 **********************************************************************************************************************/
/*! \brief                    Calculate and return the index of a bit within the corresponding word in a bitfield array.
 *
 *  \details                  A bitfield array is composed of a word array.
 *                            Function calculates the index of the bit within the word where a specified bit position
 *                            is located.
 *  \param[in]  bitPosition   Specified bit position.
 *  \return                   Bit position within word.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UBitfieldHdl_GetBitfieldBitInWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition /* in  */);


/***********************************************************************************************************************
 *  private FiM_UBitfieldHdl_GetNextSetBitInWord()
 **********************************************************************************************************************/
/*! \brief                          Internal helper function to scan a range within a word for next bit that is set.
 *
 *  \details                        Scans a range (between startInWordIndex and stopInWordIndex - each including)
 *                                  within a word for the next bit that is set. Does not consider wrap around.
 *  
 *  \param[in]      bitfieldWord    Word to scan (part of bitfield).
 *  \param[in,out]  bitIndexPtr     Must point to a valid memory location.
 *                                  In:   Start of bitfield range (including) to scan.
 *                                  Out:  Index of bit that was found.
 *                                        If no bit was found, the index where the scan stopped (not plus 1).
 *  \param[in]      stopInWordIndex Stop of bitfield range (including) to scan. May not be smaller than start index.
 *  \return                         TRUE if bit was found
 *                                  FALSE if bit was not found
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \reentrant                      TRUE
 *  \spec                           requires bitIndexPtr != 0;  \endspec
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UBitfieldHdl_GetNextSetBitInWord(CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                       P2VAR(uint8, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                       CONST(uint8, AUTOMATIC) stopInWordIndex);

/* \unit UCmpSwp */

/***********************************************************************************************************************
 *  public FiM_UCmpSwp_CompareAndSwap
 **********************************************************************************************************************/
/*! \brief                    Atomic compare and swap operation.
 *
 *  \details                  Either uses internal default implementation or custom implementation.
 *  \param[in,out] addressPtr Memory location to be compared and swapped. Must point to a valid memory location.
 *  \param[in]     cmpValue   Value to be compared with.
 *  \param[in]     newValue   Value to be written.
 *  \return                   TRUE  swap has been performed
 *                            FALSE otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_UCMPSWP_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UCmpSwp_CompareAndSwap(volatile CONSTP2VAR(FiM_ProcessorWordType, FIM_VAR_UNCACHED, AUTOMATIC) addressPtr,
                             CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue,
                             CONST(FiM_ProcessorWordType, AUTOMATIC) newValue);

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************/
/* FiM_UBitfieldHdl Start *********************************************************************************************/
/**********************************************************************************************************************/
#if (!defined (FIM_NOUNIT_UBITFIELDHDL))                                                       /* COV_FIM_UNITTEST TX */
/* \unit     UBitfieldHdl */

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 * private FiM_UBitfieldHdl_TestBitInWord
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UBitfieldHdl_TestBitInWord
                                          (CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                           CONST(uint8, AUTOMATIC) bitInWordPosition)
{
  boolean               retVal = FALSE;
  
  /* #10 Set up bitmask for the word depending on passed bitPosition. */
  FiM_ProcessorWordType bitMask = 0x01;
                        bitMask = (bitMask << bitInWordPosition);
  /* #20 Mask out all other bits and test whether AND yields something different than 0. */
  if ((bitfieldWord & bitMask) != 0U)
  {
    /* #30 Return TRUE if yes (bit is set). */
    retVal = TRUE;
  }
  else
  {
    /* #40 Return FALSE if no (bit is not set). */
    retVal = FALSE;
  }

  return retVal;
}


/***********************************************************************************************************************
 * public FiM_UBitfieldHdl_TestBitInBitfield
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, FIM_CODE) FiM_UBitfieldHdl_TestBitInBitfield
                            (volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                             CONST(uint32, AUTOMATIC) bitPosition)
{
  /* ! bitfieldWordPtr may only be used for read access ! */

  uint32  bitfieldWordArrIndex  = FiM_UBitfieldHdl_GetBitfieldWordIndex(bitPosition);
  uint8   bitInWordPosition     = FiM_UBitfieldHdl_GetBitfieldBitInWordIndex(bitPosition);
  
  return FiM_UBitfieldHdl_TestBitInWord(bitfieldWordPtr[bitfieldWordArrIndex], bitInWordPosition);
}


/***********************************************************************************************************************
 * public FiM_UBitfieldHdl_SetBitInBitfield
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE)
  FiM_UBitfieldHdl_SetBitInBitfield(volatile CONSTP2VAR(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                    CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                    CONST(uint32, AUTOMATIC) bitPosition)
{
  /* #10 Identify the index into the word array where the bit is located. */
  uint32  bitfieldWordArrIndex = FiM_UBitfieldHdl_GetBitfieldWordIndex(bitPosition);

  /* #20 If runtime checks are enabled: Check that write access is not done beyond array bounds. */
  if ((FiM_ErrorCheck_IsRuntimeErrorDetected(bitfieldWordArrIndex >= bitfieldWordArrSize) == FALSE)) /* PRQA S 2991, 2995, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_RUNTIME_CHECKS == STD_OFF) || (bitfieldWordArrIndex < bitfieldWordArrSize) */
    FiM_ProcessorWordType oldValue  = 0;
    FiM_ProcessorWordType newValue  = 0;

    /* #30 Identify the word and the bitmask for the word. */
    volatile P2VAR(FiM_ProcessorWordType, FIM_VAR_UNCACHED, AUTOMATIC) wordPtr = &bitfieldWordPtr[bitfieldWordArrIndex];
    FiM_ProcessorWordType bitMask = 0x01U;
                          bitMask = bitMask << (FiM_UBitfieldHdl_GetBitfieldBitInWordIndex(bitPosition));

    /* #40 Repeat until CompareAndSwap succeeds. */
    do
    {
      /* #50 Set the correct bit in the bitfield word in a current stack copy. */
      oldValue = *wordPtr;
      newValue = (oldValue | bitMask);
      /* #60 Try to update (compare & swap) the RAM contents. */
    } while (FiM_UCmpSwp_CompareAndSwap(wordPtr, oldValue, newValue) == FALSE);        /* SBSW_FIM_BITMANIP_COMPSWAPFUNC_CALL */
  }

  return;
}


/***********************************************************************************************************************
 * public FiM_UBitfieldHdl_ResetBitInBitfield
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE)
  FiM_UBitfieldHdl_ResetBitInBitfield(volatile CONSTP2VAR(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                      CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                      CONST(uint32, AUTOMATIC) bitPosition)
{
  /* #10 Identify the index into the word array where the bit is located. */
  uint32  bitfieldWordArrIndex = FiM_UBitfieldHdl_GetBitfieldWordIndex(bitPosition);

  /* #20 If runtime checks are enabled: Check that write access is not done beyond array bounds. */
  if ((FiM_ErrorCheck_IsRuntimeErrorDetected(bitfieldWordArrIndex >= bitfieldWordArrSize) == FALSE)) /* PRQA S 2991, 2995, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_RUNTIME_CHECKS == STD_OFF) || (bitfieldWordArrIndex < bitfieldWordArrSize) */
    FiM_ProcessorWordType oldValue  = 0;
    FiM_ProcessorWordType newValue  = 0;

    /* #30 Identify the word and the bitmask for the word. */
    volatile P2VAR(FiM_ProcessorWordType, FIM_VAR_UNCACHED, AUTOMATIC) wordPtr = &bitfieldWordPtr[bitfieldWordArrIndex];
    FiM_ProcessorWordType bitMask = 0x01U;
                          bitMask = ~(bitMask << (FiM_UBitfieldHdl_GetBitfieldBitInWordIndex(bitPosition)));

    /* #40 Repeat until CompareAndSwap succeeds. */
    do
    {
      /* #50 Reset the correct bit in the bitfield word in a current stack copy. */
      oldValue = *wordPtr;
      newValue = (oldValue & bitMask);
      /* #60 Try to update (compare & swap) the RAM contents. */
    } while (FiM_UCmpSwp_CompareAndSwap(wordPtr, oldValue, newValue) == FALSE);        /* SBSW_FIM_BITMANIP_COMPSWAPFUNC_CALL */
  }
  return;
}


/**********************************************************************************************************************
 *  private FiM_UBitfieldHdl_GetNextSetBitInWord()
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
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UBitfieldHdl_GetNextSetBitInWord(CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                       P2VAR(uint8, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                       CONST(uint8, AUTOMATIC) stopInWordIndex)
{
  /* save start index */
  uint8   iterInWordIndex = *bitIndexPtr;
  boolean found           = FALSE;
  /* if no set bit is found, we return the stop position */
          *bitIndexPtr    = stopInWordIndex;                                         /* SBSW_FIM_BITINDEX_ADDRESS_PTR */
  /* #040   check whether this word is 0 (i.e. no bit is set) */
  /*        if it's not 0 we need to take a closer look (set bit could be before startBit or after bitfield end */
  if (bitfieldWord != 0U)
  {
    /* #050   if word is not 0, traverse through bits in word up to calculated local stop */
    for (; iterInWordIndex <= stopInWordIndex; iterInWordIndex++)
    {
      /* #060   as soon as a set bit was found in this area, then break and return found position */
      found = FiM_UBitfieldHdl_TestBitInWord(bitfieldWord, iterInWordIndex);
      if (found == TRUE)
      {
        *bitIndexPtr = iterInWordIndex;                                              /* SBSW_FIM_BITINDEX_ADDRESS_PTR */
        break;
      }
    }
  }
  else
  {
    /* #070   if word is 0 there's nothing to do: it's sure that there is no set bit in the local scan area */
  }
  return found;
}


/**********************************************************************************************************************
 *  public FiM_UBitfieldHdl_GetNextSetBit()
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
 *
 *
 *
 *
 *
*/
FUNC(boolean, FIM_CODE)
  FiM_UBitfieldHdl_GetNextSetBit(volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                 CONST(uint32, AUTOMATIC) bitfieldLength, /* in bits */
                                 P2VAR(uint32, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                 CONST(uint32, AUTOMATIC) stopBit)
{
  /* do some preparational stuff so we can use indices (including) for the remainder of the function */
  /* last index in bitfield (including)                   */
  uint32  bitfieldEndIndex  = bitfieldLength - 1UL;
  /* overall index from which scan starts (including)     */
  uint32  startIndex        = *bitIndexPtr;
  /* overall index at which scan stops (including)        */
  /*    if (stopBit == 0UL)                               */
  /*      stopIndex = bitfieldEndIndex;                   */
  /*    else                                              */
  /*      stopIndex = stopBit - 1UL;                      */
  uint32  stopIndex = (stopBit == 0UL) ? bitfieldEndIndex : (stopBit - 1UL);
  /* overall index at which we found a set bit (if any)   */
  /* otherwise index where scan stopped (excluding)       */
  uint32  foundIndex        = stopBit;                                 /* PRQA S 2981 */ /* MD_FiM_2981_GetNextSetBit */
  /* have we found a bit that is set?                     */
  boolean found             = FALSE;

  /* #010 set iterator to start of scan */
  uint32 iterIndex = startIndex;
  /* #020 start loop (from startIndex till stopIndex while considering wrap around; stop loop if a set bit was found) */
  for (;;)
  {
    /* #030   get word index in which iterator is located */
    uint32  bitfieldWordArrIndex  = FiM_UBitfieldHdl_GetBitfieldWordIndex(iterIndex);
    /* #040   calculate the index at which to stop the next part of the scan; stop at (whichever comes first): */
    /*        - last bit in word (including) */
    uint32 localStopIndex = ((bitfieldWordArrIndex + 1U) * FIM_BITS_PER_PROCESSORWORD) - 1U;
    /*        - or highest bit in bitfield (including) */
    if (localStopIndex > bitfieldEndIndex)
    {
      localStopIndex = bitfieldEndIndex;
    }
    /*        - or stop bit (excluding) - we calculated stopIndex (including) for that                                */
    /*          only if there is no upcoming wrap around (in which case there are upcoming bits at start of bitfield) */
    if ((localStopIndex > stopIndex)  &&
                                          ((stopIndex >= startIndex)    ||  /* wrap around not needed   */
                                           (iterIndex < startIndex)))       /* wrap around already done */
    {
      localStopIndex = stopIndex;
    }

    /* #050   look for a set bit in word (within calculated limits) */
    {
      uint8   foundInWordIndex      = FiM_UBitfieldHdl_GetBitfieldBitInWordIndex(iterIndex);
      found = FiM_UBitfieldHdl_GetNextSetBitInWord(bitfieldWordPtr[bitfieldWordArrIndex],
                                                      &foundInWordIndex,               /* SBSW_FIM_GETNEXTSETBIT_CALL */
                                                      FiM_UBitfieldHdl_GetBitfieldBitInWordIndex(localStopIndex));
      /* transform the local index within the word to the found index in the global bitfield  */
      if (found == TRUE)
      {
        foundIndex = foundInWordIndex + (bitfieldWordArrIndex * FIM_BITS_PER_PROCESSORWORD);
      }
    }
    /* #060   advance iterator to last (virtually) checked bit */
    iterIndex = localStopIndex;
    /* #070   stop overall scan if set bit was found or global stop of scan is reached */
    if ((found == TRUE)  ||         /* set bit found */
        (iterIndex == stopIndex))   /* reached requested end of global scan */
    {
      break;
    }
    /* #080     increment iterator to next bit to check (considering potential wrap around)                           */
    iterIndex = (iterIndex == bitfieldEndIndex) ? 0UL : (iterIndex + 1UL);
  }
  /*   #100 end loop and return found index */
  *bitIndexPtr = foundIndex;                                                         /* SBSW_FIM_BITINDEX_ADDRESS_PTR */

  return found;
}


/***********************************************************************************************************************
 * private FiM_UBitfieldHdl_GetBitfieldWordIndex
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(uint32, FIM_CODE) FiM_UBitfieldHdl_GetBitfieldWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition)
{
  uint32  retVal = (uint32)(bitPosition / FIM_BITS_PER_PROCESSORWORD);

  return retVal;
}


/***********************************************************************************************************************
 * private FiM_UBitfieldHdl_GetBitfieldBitInWordIndex
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UBitfieldHdl_GetBitfieldBitInWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition)
{
  uint8 retVal = (uint8)(bitPosition % FIM_BITS_PER_PROCESSORWORD);

  return retVal;
}

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* FIM_NOUNIT_UBITFIELDHDL */

/**********************************************************************************************************************/
/* FiM_UBitfieldHdl End ***********************************************************************************************/
/**********************************************************************************************************************/


/**********************************************************************************************************************/
/* FiM_UCmpSwp Start **************************************************************************************************/
/**********************************************************************************************************************/
#if (!defined (FIM_NOUNIT_UCMPSWP))                                                            /* COV_FIM_UNITTEST TX */
/* \unit     UCmpSwp */

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (FIM_SUPPORT_USER_CAS == STD_OFF)
/***********************************************************************************************************************
 *  private FiM_UCmpSwp_CompareAndSwapDefault
 **********************************************************************************************************************/
/*! \brief                    Internal default version of an atomic compare and swap operation.
 *
 *  \details                  Compares the contents of a memory location (identified by addressPtr) to a given
 *                            value (cmpValue) and, only if they are the same, modifies the contents of that memory
 *                            location to a given new value (newValue).
 *                            This is done as a single atomic operation.
 *                              - Implemented here with a critical section.
 *                              - Can be replaced by more efficient version for multicore systems.
 *  \param[in,out] addressPtr Memory location to be compared and swapped. Must point to a valid memory location.
 *  \param[in]     cmpValue   Value to be compared with.
 *  \param[in]     newValue   Value to be written.
 *  \return                   TRUE  swap has been performed
 *                            FALSE otherwise
 *  \pre                      -
 *  \config                   FIM_SUPPORT_USER_CAS == STD_OFF
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UCmpSwp_CompareAndSwapDefault(volatile CONSTP2VAR(FiM_ProcessorWordType, FIM_VAR_UNCACHED, AUTOMATIC) addressPtr,
                                    CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue,
                                    CONST(FiM_ProcessorWordType, AUTOMATIC) newValue);
#endif


/***********************************************************************************************************************
 * public FiM_UCmpSwp_CompareAndSwap
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_UCMPSWP_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UCmpSwp_CompareAndSwap(volatile CONSTP2VAR(FiM_ProcessorWordType, FIM_VAR_UNCACHED, AUTOMATIC) addressPtr,
                             CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue,
                             CONST(FiM_ProcessorWordType, AUTOMATIC) newValue)
{
#if (FIM_SUPPORT_USER_CAS == STD_ON)
  return ApplFiM_SyncCompareAndSwap(addressPtr, cmpValue, newValue);/* SBSW_FIM_PTR_FWD_ARG */ /* VCA_FIM_PTR_FWD_ARG */
#else
  return FiM_UCmpSwp_CompareAndSwapDefault(addressPtr, cmpValue, newValue);                /* SBSW_FIM_PTR_FWD_ARG */
#endif
}


#if (FIM_SUPPORT_USER_CAS == STD_OFF)
/***********************************************************************************************************************
 * FiM_UCmpSwp_CompareAndSwapDefault
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
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UCmpSwp_CompareAndSwapDefault(volatile CONSTP2VAR(FiM_ProcessorWordType, FIM_VAR_UNCACHED, AUTOMATIC) addressPtr,
                                    CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue,
                                    CONST(FiM_ProcessorWordType, AUTOMATIC) newValue)
{
  boolean retVal  = FALSE;

  /* #10 Enter critical section. */
  FiM_EnterCritical_CrossCore();

  /* #20 Assert that the currently stored value is the same as the passed compare value. */
  if (*addressPtr == cmpValue)
  {
    /* #30 if so: write new value. */
    *addressPtr = newValue;                                                      /* SBSW_FIM_COMPSWAPFUNC_ADDRESS_PTR */
    retVal = TRUE;
  }
  else
  {
    /* #40 otherwise: return FALSE. */
    retVal = FALSE;
  }

  /* #50 Leave critical section. */
  FiM_LeaveCritical_CrossCore();

  return retVal;
}
#endif


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* FIM_NOUNIT_UCMPSWP */


/***********************************************************************************************************************
*  END OF FILE: FiM_LHelper.c
***********************************************************************************************************************/
