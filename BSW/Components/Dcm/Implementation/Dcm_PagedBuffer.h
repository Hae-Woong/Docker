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
/**        \file  Dcm_PagedBuffer.h
 *         \unit  PagedBuffer
 *        \brief  Contains public types, function declarations and inline function definitions of PagedBuffer unit.
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
#if !defined(DCM_PAGEDBUFFER_H)
# define DCM_PAGEDBUFFER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_PagedBufferTypes.h"
# if (DCM_PAGED_BUFFER_ENABLED == STD_ON)                                                                                                            /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define DCM_PAGEDBUFFER_STATE_INACTIVE                             (Dcm_PagedBufferStateType)0x00u /*!< No paged-buffer transmission in progress */
#  define DCM_PAGEDBUFFER_STATE_ACTIVATED                            (Dcm_PagedBufferStateType)0x01u /*!< Paged-buffer transmission activated, but not yet passed to the PduR */
#  define DCM_PAGEDBUFFER_STATE_CANCELED                             (Dcm_PagedBufferStateType)0x02u /*!< Paged-buffer transmission terminated prior passing it to the PduR */
#  define DCM_PAGEDBUFFER_STATE_ONTX                                 (Dcm_PagedBufferStateType)0x03u /*!< Paged-buffer transmission ongoing (passed to the PduR) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_PagedBufferInit()
 *********************************************************************************************************************/
/*! \brief          Initialize paged-buffer any time.
 *  \details        Initialization of the paged-buffer.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_PagedBufferInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferGetState()
 *********************************************************************************************************************/
/*! \brief          Returns the paged-buffer state
 *  \details        -
 *  \param[in]      threadId    Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(Dcm_PagedBufferStateType, DCM_CODE) Dcm_PagedBufferGetState(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferStart()
 *********************************************************************************************************************/
/*! \brief          Activate paged-buffer write process.
 *  \details        -
 *  \param[in]      threadId      Active thread
 *  \param[in]      resDataLen    Specifies how many bytes will be copied using the update page function.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_PagedBufferStart(
  Dcm_ThreadIdOptType threadId,
  Dcm_MsgLenType resDataLen
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferCancel()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CancelPage call.
 *  \details        Dispatches the CancelPage call regarding the paged-buffer.
 *  \param[in]      pContext        Pointer to the context
 *  \param[out]     ErrorCode       The NRC
 *  \return         DCM_E_OK        Operation finished with success
 *  \return         DCM_E_NOT_OK    Operation failed, ErrorCode contains the NRC
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferCancel(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferDataPadding()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of readDtcInfo.
 *  \details        This function is used for data padding.
 *  \param[in]      pContext              Pointer to the context
 *  \param[in]      opStatus              The operation status
 *  \param[in,out]  pDataContext          Pointer to the data context
 *  \param[out]     ErrorCode             The NRC
 *  \return         DCM_E_BUFFERTOOLOW    Enforce transmission of the current part
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_PagedBufferDataPadding(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferCopyData()
 *********************************************************************************************************************/
/*! \brief          Copy from paged-buffer to the TP layer.
 *  \details        -
 *  \param[in]      threadId            Active thread
 *  \param[in]      info                Message context (data and length of the portion)
 *  \param[in]      availableDataPtr    Remaining Tx data after completion of this call
 *  \return         BUFREQ_OK           A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK     No free buffer is available - ignore request
 *  \return         BUFREQ_E_BUSY       No free buffer at this time - try later again
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_PagedBufferCopyData(
  Dcm_ThreadIdOptType threadId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_PAGED_BUFFER_ENABLED == STD_ON) */
#endif /* !defined(DCM_PAGEDBUFFER_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_PagedBuffer.h
 *********************************************************************************************************************/
