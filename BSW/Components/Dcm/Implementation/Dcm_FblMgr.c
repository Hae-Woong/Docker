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
/**        \file  Dcm_FblMgr.c
 *         \unit  FblMgr
 *        \brief  Contains the implementation of Flashbootloader Manager unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
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
#define DCM_FBLMGR_SOURCE

#ifdef __PRQA__                                                                                                                                      /* COV_DCM_PRQA_UNREACHABLE XF */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetReqData" 2985 /* MD_Dcm_Redundant_2985 */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetResData" 2985 /* MD_Dcm_Redundant_2985 */
                                                                                                                                                     /* PRQA S 2991 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2992 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2995 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2996 EOF */ /* MD_Dcm_ConstExpr */
#endif
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_FblMgr.h"
#if (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Download control state */
struct DCM_FBLMGRSINGLETONCONTEXTTYPE_TAG
{
  Dcm_FblMgrMemBlockType              MemoryBlock;           /*! Address, length and MID */
  Dcm_FblMgrMemBlockType              PreviousMemoryBlock;   /*! Address, length and MID of previous MemoryBlock when it is necessary to write it again */
  Dcm_FblMgrBlockLengthType           MaxBlockLength;        /*!< Maximum number of block length */
  Dcm_FblMgrBlockLengthType           RemainingBlockLength;  /*!< Remaining number of block length */
  Dcm_FblMgrBlockSequenceCounterType  BlockSequenceCounter;  /*!< Old block sequence counter */
  Dcm_FblMgrDataTransferStateType     DataTransferState;     /*!< Current state of the dataTransfer process */
};
typedef struct DCM_FBLMGRSINGLETONCONTEXTTYPE_TAG Dcm_FblMgrSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of FblMgr unit */
DCM_LOCAL VAR(Dcm_FblMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_FblMgrSingletonContext;
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_FblMgrReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_FblMgrReset(void)
{
  Dcm_FblMgrSingletonContext.BlockSequenceCounter = 0u;
  Dcm_FblMgrSingletonContext.DataTransferState = DCM_FBL_DATATRANSFER_STATE_IDLE;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrGetMemoryBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Dcm_FblMgrMemBlockPtrType, DCM_CODE) Dcm_FblMgrGetMemoryBlock(void)
{
  return &Dcm_FblMgrSingletonContext.MemoryBlock;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrSetMemoryBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_FblMgrSetMemoryBlock(Dcm_FblMgrMemBlockConstPtrType memoryBlock)
{
  Dcm_FblMgrSingletonContext.MemoryBlock = *memoryBlock;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrGetPreviousMemoryBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Dcm_FblMgrMemBlockPtrType, DCM_CODE) Dcm_FblMgrGetPreviousMemoryBlock(void)
{
  return &Dcm_FblMgrSingletonContext.PreviousMemoryBlock;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrSetPreviousMemoryBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_FblMgrSetPreviousMemoryBlock(Dcm_FblMgrMemBlockConstPtrType memoryBlock)
{
  Dcm_FblMgrSingletonContext.PreviousMemoryBlock = *memoryBlock;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrGetMaxBlockLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Dcm_FblMgrBlockLengthType, DCM_CODE) Dcm_FblMgrGetMaxBlockLength(void)
{
  return Dcm_FblMgrSingletonContext.MaxBlockLength;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrSetMaxBlockLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_FblMgrSetMaxBlockLength(Dcm_FblMgrBlockLengthType blockLength)
{
  Dcm_FblMgrSingletonContext.MaxBlockLength = blockLength;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrGetRemainingBlockLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Dcm_FblMgrBlockLengthType, DCM_CODE) Dcm_FblMgrGetRemainingBlockLength(void)
{
  return Dcm_FblMgrSingletonContext.RemainingBlockLength;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrSetRemainingBlockLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_FblMgrSetRemainingBlockLength(Dcm_FblMgrBlockLengthType blockLength)
{
  Dcm_FblMgrSingletonContext.RemainingBlockLength = blockLength;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrGetBlockSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Dcm_FblMgrBlockSequenceCounterType, DCM_CODE) Dcm_FblMgrGetBlockSequenceCounter(void)
{
  return Dcm_FblMgrSingletonContext.BlockSequenceCounter;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrSetBlockSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_FblMgrSetBlockSequenceCounter(Dcm_FblMgrBlockSequenceCounterType blockSequenceCounter)
{
  Dcm_FblMgrSingletonContext.BlockSequenceCounter = blockSequenceCounter;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrGetDataTransferState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Dcm_FblMgrDataTransferStateType, DCM_CODE) Dcm_FblMgrGetDataTransferState(void)
{
  return Dcm_FblMgrSingletonContext.DataTransferState;
}

/**********************************************************************************************************************
 *  Dcm_FblMgrSetDataTransferState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_FblMgrSetDataTransferState(Dcm_FblMgrDataTransferStateType DataTransferState)
{
  Dcm_FblMgrSingletonContext.DataTransferState = DataTransferState;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_FblMgr.c
 *********************************************************************************************************************/
