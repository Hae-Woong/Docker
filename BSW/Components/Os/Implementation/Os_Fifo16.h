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
 * \addtogroup Os_Fifo16
 *
 * \{
 *
 * \file          Os_Fifo16.h
 * \brief         Contains the FIFO interface implementation.
 * \details       --no details--
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#ifndef OS_FIFO16_H
# define OS_FIFO16_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Fifo16Int.h"
# include "Os_Hal_CoreInt.h"

/* Os kernel module dependencies */
# include "Os_Fifo.h"
# include "Os_Error.h"
# include "Os_Common.h"

/* Os Hal dependencies */
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Core.h"


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
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  FIFO Write Functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_Fifo16WriteInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Fifo16WriteInit,
(
  P2CONST(Os_Fifo16WriteConfigType, AUTOMATIC, OS_APPL_DATA) Fifo
))
{
  /* #10  Initialize all indices to zero. */
  Fifo->Dyn->WriteIdx = 0;                                                                                              /* SBSW_OS_FF16_FIFOWRITE_DYN_001 */
}


/***********************************************************************************************************************
 *  Os_Fifo16Enqueue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Fifo16Enqueue,
(
  P2CONST(Os_Fifo16WriteConfigType, AUTOMATIC, OS_APPL_DATA) Fifo,
  uint16 Value
))
{
  /* Hint: As a foreign core may read the new Value concurrently, perform the write access before the writeIdx is
   * updated. */
  /* #20 Write the value. */
  Fifo->Data[Fifo->Dyn->WriteIdx].Value = Value;                                                                        /* SBSW_OS_FF16_FIFOWRITE_DATA_001 */

  /* #30 Increment the write index counter. */
  Os_FifoIdxIncrement(&(Fifo->Dyn->WriteIdx), Fifo->Size, OS_FIFO_WRITE_SYNCHRONIZED);                                  /* SBSW_OS_FF16_FIFOIDXINCREMENT_001 */
}


/***********************************************************************************************************************
 *  Os_Fifo16HasFreeSpaceAvailable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_FifoNodeAvailabilityType, OS_CODE, OS_ALWAYS_INLINE,                    /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Fifo16HasFreeSpaceAvailable,
(
  P2CONST(Os_Fifo16WriteConfigType, AUTOMATIC, OS_APPL_DATA) Fifo
))
{
  Os_FifoIdxType readIdx;
  Os_FifoIdxType writeIdx;
  Os_FifoNodeAvailabilityType result = OS_FIFO_INVALID_STATE;

  writeIdx = Fifo->Dyn->WriteIdx;
  readIdx = Os_Hal_LoadAcquire16(&(Fifo->Read->ReadIdx));                                                               /* SBSW_OS_FF16_LOADACQUIRE_001 */

  /* #10 Check for valid indices. */
  if(Os_FifoIndicesInRange(writeIdx, readIdx, Fifo->Size) != 0u)                                                        /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
  {
    /* #20 Check for full FIFO. */
    if(Os_FifoIsFull(writeIdx, readIdx, Fifo->Size) != 0u)                                                              /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
    {
      result = OS_FIFO_UNAVAILABLE;
    }
    else
    {
      result = OS_FIFO_AVAILABLE;
    }
  }

  return result;
}


/***********************************************************************************************************************
 *  FIFO Read Functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_Fifo16ReadInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Fifo16ReadInit,
(
  P2CONST(Os_Fifo16ReadConfigType, AUTOMATIC, OS_APPL_DATA) Fifo
))
{
  /* #10  Initialize all indices to zero. */
  Fifo->Dyn->ReadIdx = 0;                                                                                               /* SBSW_OS_FF16_FIFOREAD_DYN_001 */
}


/***********************************************************************************************************************
 *  Os_Fifo16Dequeue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint16, OS_CODE, OS_ALWAYS_INLINE,                                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Fifo16Dequeue,
(
  P2CONST(Os_Fifo16ReadConfigType, AUTOMATIC, OS_APPL_DATA) Fifo
))
{
  uint16 result;

  /* #20 Read the result value. */
  result = Fifo->Data[Fifo->Dyn->ReadIdx].Value;

  /* #30 Increment the front index counter. */
  Os_FifoIdxIncrement(&(Fifo->Dyn->ReadIdx), Fifo->Size, OS_FIFO_WRITE_SYNCHRONIZED);                                   /* SBSW_OS_FF16_FIFOIDXINCREMENT_001 */

  return result;
}


/***********************************************************************************************************************
 *  Os_Fifo16HasDataAvailable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_FifoNodeAvailabilityType, OS_CODE, OS_ALWAYS_INLINE,                    /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Fifo16HasDataAvailable,
(
  P2CONST(Os_Fifo16ReadConfigType, AUTOMATIC, OS_APPL_DATA) Fifo
))
{
  Os_FifoIdxType readIdx;
  Os_FifoIdxType writeIdx;
  Os_FifoNodeAvailabilityType result = OS_FIFO_INVALID_STATE;

  readIdx = Fifo->Dyn->ReadIdx;
  writeIdx = Os_Hal_LoadAcquire16(&(Fifo->Write->WriteIdx));                                                            /* SBSW_OS_FF16_LOADACQUIRE_001 */

  /* #10 Check for valid indices. */
  if(Os_FifoIndicesInRange(writeIdx, readIdx, Fifo->Size) != 0u)                                                        /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
  {
    /* #20 Check for empty FIFO. */
    if(Os_FifoIsEmpty(writeIdx, readIdx) != 0u)                                                                         /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
    {
      result = OS_FIFO_UNAVAILABLE;
    }
    else
    {
      result = OS_FIFO_AVAILABLE;
    }
  }

  return result;
}


/***********************************************************************************************************************
 *  Os_Fifo16EmptyQueue()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_Fifo16EmptyQueue,
(
  P2CONST(Os_Fifo16ReadConfigType, AUTOMATIC, OS_APPL_DATA) Fifo
))
{
  /* #10 Use base class method. */
  Os_FifoEmptyQueue(&(Fifo->Write->WriteIdx), &(Fifo->Dyn->ReadIdx));                                                   /* SBSW_OS_FF16_FIFOEMPTYQUEUE_001 */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_FIFO16_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Fifo16.h
 **********************************************************************************************************************/
