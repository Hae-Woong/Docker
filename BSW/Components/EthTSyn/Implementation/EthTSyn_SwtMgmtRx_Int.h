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
/*!        \file  EthTSyn_SwtMgmtRx_Int.h
 *         \unit  SwtMgmtRx
 *        \brief  EthTSyn internal header file of the SwtMgmtRxMsgHndl unit.
 *      \details  Interface and type definitions of the SwtMgmtRxMsgHndl unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SWT_MGMT_RX_INT_H)
# define ETHTSYN_SWT_MGMT_RX_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define ETHTSYN_START_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_Init
 *********************************************************************************************************************/
/*! \brief        Initializes the SwtMgmtRx unit.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_Receive
 *********************************************************************************************************************/
/*! \brief        Notification about a received EthTSyn frame.
 *  \details      -
 *  \param[in]    ComCtrlEthIfCtrlIdx  ComCtrlEthIf controller index
 *                                     [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \param[in]    DataPtr              Pointer to payload of the received Ethernet frame
 *                                     (i.e. Ethernet header is not provided)
 *  \param[in]    IngrTimingInfoPtr    Ingress timestamp as well as EthIf and StbM time after reception as internal
 *                                     representation with validity information
 *  \return       ETHTSYN_E_NO_ERROR - No DET error detected
 *  \return       other values - DET error code in case a DET error was detected
 *  \pre          Must be called in context of EthTSyn_RxIndication()
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint8, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_Receive(
                  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8)                           DataPtr,
  ETHTSYN_P2CONST(EthTSyn_IngrTimingInfoType)      IngrTimingInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ProcMsg
 *********************************************************************************************************************/
/*! \brief        Processes information of switch management object.
 *  \details      Checks if port is valid and forwards received message and timestamping information to Rx unit.
 *  \param[in]    SwtMgmtRxIdx        SwtMgmtRx instance index
 *                                    [range: SwtMgmtRxIdx < EthTSyn_GetSizeOfSwtMgmtRx()]
 *  \param[in]    SwtMgmtInfoMgmtPtr  Switch management information and validity information
 *  \param[in]    SwtRxTsInfoPtr      Information about the switch reception timestamps
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtMgmtRxIdx < EthTSyn_GetSizeOfSwtMgmtRx();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ProcMsg(
                  EthTSyn_SwtMgmtRx_IdxType               SwtMgmtRxIdx,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MgmtInfoMgmtStructType) SwtMgmtInfoMgmtPtr,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmtRxTsInfoType)            SwtRxTsInfoPtr);

#  if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_GetSyncIngrTsAndResTime
 *********************************************************************************************************************/
/*! \brief        Gets the Sync ingress timestamp and switch residence time.
 *  \details      If switch and host are one time aware system, the switch ingress timestamp is considered as Sync
 *                ingress timestamp and the residence time is set to zero.
 *                Otherwise, the host ingress timestamp is considered as Sync ingress timestamp.
 *  \param[in]    SyncIngrTsInfoPtr  Sync ingress timestamp info
 *  \param[out]   SyncIngrTsPtr      The actual sync ingress timestamp as internal VLT
 *  \param[out]   SwtResTimePtr      The switch residence time of the Sync message
 *  \return       E_OK - Sync ingress timestamp and switch residence time are valid
 *  \return       E_NOT_OK - Sync ingress timestamp or switch residence time are invalid
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_GetSyncIngrTsAndResTime(
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)    SyncIngrTsInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType)    SyncIngrTsPtr,
    ETHTSYN_P2VAR(EthTSyn_UTimediffType) SwtResTimePtr);
#  endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ReleasePendingMgmtObjsOfCtrl
 *********************************************************************************************************************/
/*! \brief      Releases all pending switch management objects of the passed EthIf controller.
 *  \details    -
 *  \param[in]  ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                    [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ReleasePendingMgmtObjsOfCtrl(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

#endif /* ETHTSYN_SWT_MGMT_RX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwtMgmtRx_Int.h
 *********************************************************************************************************************/
