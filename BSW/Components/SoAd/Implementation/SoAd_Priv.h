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
/*!        \file  SoAd_Priv.h
 *        \brief  Socket Adaptor private header file
 *
 *      \details  Vector static private header file for AUTOSAR Socket Adaptor module. This header file contains
 *                module internal declarations.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(SOAD_PRIV_H)
# define SOAD_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "SoAd_Lcfg.h"
# include "SoAd_PBcfg.h"
# include "SchM_SoAd.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# define SOAD_BEGIN_CRITICAL_SECTION()                  SchM_Enter_SoAd_SOAD_EXCLUSIVE_AREA_0()
# define SOAD_END_CRITICAL_SECTION()                    SchM_Exit_SoAd_SOAD_EXCLUSIVE_AREA_0()

# define SOAD_BEGIN_CRITICAL_SECTION_1()                SchM_Enter_SoAd_SOAD_EXCLUSIVE_AREA_1()
# define SOAD_END_CRITICAL_SECTION_1()                  SchM_Exit_SoAd_SOAD_EXCLUSIVE_AREA_1()

# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
#  define SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION() SchM_Enter_SoAd_SOAD_EXCLUSIVE_AREA_MULTI_PARTITION()
#  define SOAD_END_CRITICAL_SECTION_MULTI_PARTITION()   SchM_Exit_SoAd_SOAD_EXCLUSIVE_AREA_MULTI_PARTITION()
# else
#  define SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION() SchM_Enter_SoAd_SOAD_EXCLUSIVE_AREA_0()
#  define SOAD_END_CRITICAL_SECTION_MULTI_PARTITION()   SchM_Exit_SoAd_SOAD_EXCLUSIVE_AREA_0()
# endif /* SOAD_MULTI_PARTITION == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_MainFunctionInstanceRx()
 *********************************************************************************************************************/
/*! \brief       Schedules the asynchronous reception handling.
 *  \details     -
 *  \param[in]   InstMapIdx   Instance Map index.
 *                            [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_MainFunctionInstanceRx(SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_MainFunctionInstanceState()
 *********************************************************************************************************************/
/*! \brief       Schedules state handling.
 *  \details     -
 *  \param[in]   InstMapIdx   Instance Map index.
 *                            [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_MainFunctionInstanceState(SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_MainFunctionInstanceTx()
 *********************************************************************************************************************/
/*! \brief       Schedules the asynchronous transmission handling.
 *  \details     -
 *  \param[in]   InstMapIdx   Instance Map index.
 *                            [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_MainFunctionInstanceTx(SoAd_SizeOfInstanceMapType InstMapIdx);

# if ( SOAD_SINGLE_MAIN_FUNCTION == STD_ON )
/**********************************************************************************************************************
 *  SoAd_MainFunctionInstance()
 *********************************************************************************************************************/
/*! \brief       Schedules the asynchronous reception handling, the state handling and the asynchronous transmission
 *               handling.
 *  \details     -
 *  \param[in]   InstMapIdx   Instance Map index.
 *                            [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_MainFunctionInstance(SoAd_SizeOfInstanceMapType InstMapIdx);
# endif /* SOAD_SINGLE_MAIN_FUNCTION == STD_ON */

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* !defined(SOAD_PRIV_H) */

/*!
 * \exclusivearea SOAD_EXCLUSIVE_AREA_0
 * Ensures consistency while read, modify and write global variables which are accessed in multiple context (ISR/TASK).
 * \protects SoAd_EventQueueFlag, SoAd_EventQueueMgmt, SoAd_BestMatchSoConIdxList, SoAd_RouteGrpSoConDyn, SoAd_RxMgt,
 * SoAd_SocketDyn, SoAd_SoConDyn, SoAd_RcvRemAddr, SoAd_LocalAddrDyn, SoAd_TimeoutListUdpAlive, SoAd_TimeoutListMgmt,
 * SoAd_TimeoutListUdpAliveMap, SoAd_TimeoutListUdpAlive, SoAd_TimeoutListNPduUdpTxMap, SoAd_TimeoutListNPduUdpTx,
 * SoAd_TxMgt, SoAd_PduRouteDyn, SoAd_TcpTxQueueData, SoAd_TcpTxQueueMgt, SoAd_NPduUdpTxDyn, SoAd_NPduUdpTxBuffer,
 * SoAd_IpFragMgt, SoAd_IpFragBufConfigDyn, SoAd_TriggerBufConfigDyn, SoAd_MetaDataRxBufConfigDyn,
 * SoAd_MeasurementCounter, SoAd_ModeChgQueue
 * \usedin SoAd_CloseSoCon, SoAd_DisableSpecificRouting, SoAd_EnableSpecificRouting, SoAd_ForceReleaseRemoteAddr,
 * SoAd_GetAndResetMeasurementData, SoAd_GetRcvRemoteAddr, SoAd_IfRoutingGroupTransmit,
 * SoAd_IfSpecificRoutingGroupTransmit, SoAd_IfTransmit, SoAd_Init, SoAd_LocalIpAddrAssignmentChg,
 * SoAd_MainFunctionInstance, SoAd_MainFunctionInstanceRx, SoAd_MainFunctionInstanceState, SoAd_MainFunctionInstanceTx,
 * SoAd_OpenSoCon, SoAd_ReleaseRemoteAddr, SoAd_RxIndication, SoAd_SetRemoteAddr, SoAd_SetUniqueRemoteAddr,
 * SoAd_TcpAccepted, SoAd_TcpConnected, SoAd_TcpIpEvent, SoAd_TpCancelReceive, SoAd_TpCancelTransmit, SoAd_TpTransmit,
 * SoAd_TxConfirmation
 * \exclude All other functions provided by SoAd.
 * \length LONG A lot of statements and complex code. Calls to other BSW modules with an undefined maximum runtime.
 * \externalcall TcpIp_GetIpAddr
 * \endexclusivearea
 *
 * \exclusivearea SOAD_EXCLUSIVE_AREA_1
 * Ensures that transmission requests are not rejected when a transmission in main function context is interrupted by a
 * transmission request from a different context.
 * \protects Transmission path in main function context for nPdu and routing group (data consistency is already ensured
 * by SOAD_EXCLUSIVE_AREA_0).
 * \usedin SoAd_MainFunctionInstance, SoAd_MainFunctionInstanceTx
 * \exclude All other functions provided by SoAd.
 * \length LONG Calls to other BSW modules with an undefined maximum runtime.
 * \externalcall TcpIp_UdpTransmit, TcpIp_TcpTransmit, <Up>_[SoAd][If]TriggerTransmit
 * \endexclusivearea
 *
 * \exclusivearea SOAD_EXCLUSIVE_AREA_MULTI_PARTITION
 * Ensures consistency while read, modify and write global variables which are accessed by multiple SoAd instances.
 * \protects SoAd_InstanceDyn, SoAd_MeasurementCounter, SoAd_PartitionDyn, SoAd_ShutdownFinishedCnt
 * \usedin SoAd_GetAndResetMeasurementData, SoAd_MainFunctionInstance, SoAd_MainFunctionInstanceRx,
 * SoAd_MainFunctionInstanceState, SoAd_RxIndication, SoAd_TcpAccepted, SoAd_TcpIpEvent
 * \exclude All other functions provided by SoAd.
 * \length SHORT Only a few statements with a constant maximum runtime.
 * \endexclusivearea
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Priv.h
 *********************************************************************************************************************/
