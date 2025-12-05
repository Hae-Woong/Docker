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
/**        \file  Dcm_Svc2CDefMgr.h
 *         \unit  Svc2CDefMgr
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x2C Definition Manager unit.
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
#if !defined(DCM_SVC2CDEFMGR_H)
# define DCM_SVC2CDEFMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc2CDefMgrTypes.h"
# if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
#  include "Dcm_DidMgr.h"
#  include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON) && \
      (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_COND_ENABLED == STD_ON)
#   define DCM_SVC_2C_NUM_PROCESS_CONTEXTS                           2u
#   define DCM_SVC_2C_PROCESS_CONTEXT_ID_EXT_ONLY                    1u
#  else
#   define DCM_SVC_2C_NUM_PROCESS_CONTEXTS                           1u
#   define DCM_SVC_2C_PROCESS_CONTEXT_ID_EXT_ONLY                    0u
#  endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#  define Dcm_Svc2CDefMgrIsDidSrcItem(srcItemIdx)                    (Dcm_UtiGenericBitSetTestBit(Dcm_Svc2CNvMData.Sequencer,(srcItemIdx)))          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc2CDefMgrSetDidSrcItem(srcItemIdx)                   (Dcm_UtiGenericBitSetSetBit(Dcm_Svc2CNvMData.Sequencer,(srcItemIdx)))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc2CDefMgrSetMemSrcItem(srcItemIdx)                   (Dcm_UtiGenericBitSetClrBit(Dcm_Svc2CNvMData.Sequencer,(srcItemIdx)))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_Svc2CDefMgrGetSrcItemIndex(dynDidHandle, offset)       ((Dcm_CfgDidMgrDynDidSrcItemIdxMemType)(Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle) + (offset))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Context for the processing of a specific DynDID */
struct DCM_SVC2CDYNDIDPROCESSCONTEXTTYPE_TAG
{
  Dcm_DidMgrDidInfoContextType         SrcDidContext;    /*!< Info context of the currently requested source DID */
  Dcm_DidMgrDidOpTypeContextType       DidOpTypeContext; /*!< Operation type context */
  Dcm_CfgDidMgrDynDidSrcItemIdxMemType ItemInProgress;   /*!< Index of the currently processed source item */
  Dcm_CfgDidMgrDynDidSrcItemIdxMemType ItemToStop;       /*!< Index of the item after the last source item */
};
typedef struct DCM_SVC2CDYNDIDPROCESSCONTEXTTYPE_TAG Dcm_Svc2CDynDidProcessContextType;

typedef P2VAR(Dcm_Svc2CDynDidProcessContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2CDynDidProcessContextPtrType;

/*! Context for the handling of the read access to a DynDID */
struct DCM_SVC2CDYNDIDACCESSCONTEXT_TAG
{
  Dcm_Svc2CDynDidProcessContextType ProcessContext[DCM_SVC_2C_NUM_PROCESS_CONTEXTS]; /*!< Process contexts for DynDIDs */
  Dcm_CfgDidMgrDynDidHandleMemType  DynDidHandleInUse;                               /*!< Handle of currently processed DynDID */
#  if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
  Dcm_MsgItemType                   Buffer[DCM_SVC_2C_READ_BUFFER_SIZE];             /*!< DynDID read buffer */
#  endif
};
typedef struct DCM_SVC2CDYNDIDACCESSCONTEXT_TAG Dcm_Svc2CDynDidReadAccessContextType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrInit()
 *********************************************************************************************************************/
/*! \brief          Initialization function.
 *  \details        Service 0x2C DID definition manager initializer.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrGetSrcItem()
 *********************************************************************************************************************/
/*! \brief          Return the requested source item
 *  \details        -
 *  \param[in]      srcItemIndex    Index of the source item
 *  \return         The requested source item or any valid source item if the provided index is invalid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc2CDynDidSrcItemPtrType, DCM_CODE) Dcm_Svc2CDefMgrGetSrcItem(
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType srcItemIndex
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrGetItem()
 *********************************************************************************************************************/
/*! \brief          Return the requested item
 *  \details        -
 *  \param[in]      itemIndex    Index of the item
 *  \return         The requested item or any valid item if the provided index is invalid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc2CDynDidItemPtrType, DCM_CODE) Dcm_Svc2CDefMgrGetItem(
  Dcm_CfgDidMgrDynDidHandleOptType itemIndex
  );

#  if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_STATE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrStateCheckSrcItems()
 *********************************************************************************************************************/
/*! \brief          Check state and mode group preconditions of each source item.
 *  \details        -
 *  \param[in]      connHdl         The connection identifier
 *  \param[in]      dynDidHandle    DynDID reference to be checked
 *  \param[out]     ErrorCode       The NRC
 *  \return         DCM_E_OK        Check was successful
 *  \return         DCM_E_NOT_OK    Check was not successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrStateCheckSrcItems(
  Dcm_NetConnRefMemType connHdl,
  Dcm_CfgDidMgrDynDidHandleOptType dynDidHandle,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrIsDidDefined()
 *********************************************************************************************************************/
/*! \brief          Check whether DynDid is defined.
 *  \details        -
 *  \param[in]      pDidInfo        The DID information
 *  \return         true            Did is a DynDid
 *  \return         false           DID is not a DynDid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_Svc2CDefMgrIsDidDefined(
  Dcm_CfgDidMgrDidInfoConstPtrType pDidInfo
  );

#  if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrOnStateChanged()
 *********************************************************************************************************************/
/*! \brief          Clears all no more allowed DynDIDs in the new states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrOnStateChanged(
  void
  );
#  endif

#  if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrClear()
 *********************************************************************************************************************/
/*! \brief          Clear all DDID definitions.
 *  \details        Clears all previously defined DDIDs.
 *  \param[in]      dynDidHandle    DynDID reference to be cleared
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrClear(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle
  );
#  endif
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC2CDEFMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2CDefMgr.h
 *********************************************************************************************************************/
