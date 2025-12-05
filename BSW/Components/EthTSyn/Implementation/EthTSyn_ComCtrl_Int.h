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
/*!        \file  EthTSyn_ComCtrl_Int.h
 *         \unit  ComCtrl
 *        \brief  EthTSyn internal header file of the ComCtrl unit.
 *      \details  Interface and type definitions of the ComCtrl unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_COM_CTRL_INT_H)
# define ETHTSYN_COM_CTRL_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the ComCtrl unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_MainFunction
 *********************************************************************************************************************/
 /*! \brief          Processes cyclic tasks of the ComCtrl unit.
  *  \details        -
  *  \pre            Must be called cyclically in context of the EthTSyn_MainFunction()
  *  \context        TASK
  *  \reentrant      FALSE
  *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetClockId
 *********************************************************************************************************************/
/*! \brief          Gets the ClockId belonging to the passed port.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         0 - ClockId unknown because trcv link state is inactive.
 *  \return         other values - The ClockId of the passed port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint64, ETHTSYN_CODE) EthTSyn_ComCtrl_GetClockId(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_IsLinkStateActive
 *********************************************************************************************************************/
/*! \brief          Checks if the link state of the passed ComCtrlEthIfCtrl is active.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         FALSE - TrcvLinkState of the underlying EthIfCtrl is down.
 *  \return         TRUE - TrcvLinkState of the underlying EthIfCtrl is active.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_ComCtrl_IsLinkStateActive(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_IsTxAllowed
 *********************************************************************************************************************/
/*! \brief          Checks if transmission on the passed port is allowed.
 *  \details        Transmission is allowed, when link state is active, transmission mode is TX_ON and the debounce
 *                  timer already expired.
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         FALSE - Transmission not allowed.
 *  \return         TRUE - Transmission allowed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_ComCtrl_IsTxAllowed(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_RestartDebounceTimer
 *********************************************************************************************************************/
/*! \brief          Restarts the debounce timer of the passed port.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           Has to be called after each successful transmission (not TxConfirmation).
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_RestartDebounceTimer(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_SetAsCapable
 *********************************************************************************************************************/
/*! \brief          Sets the asCapable state.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]      IsAsCapable      AsCapable information.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_SetAsCapable(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  boolean                     IsAsCapable);
# endif /* ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_IsAsCapable
 *********************************************************************************************************************/
/*! \brief          Gets current asCapable state.
 *  \details        Returns the current asCapable state, if pdelay initiator support is enabled. Otherwise, return TRUE
 *                  because port is configured to be always asCapable.
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         Current asCapable state.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_ComCtrl_IsAsCapable(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_SetPdelay
 *********************************************************************************************************************/
/*! \brief          Sets the pdelay value.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]      Pdelay           Pdelay value.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_SetPdelay(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  EthTSyn_PdelayType          Pdelay);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_ResetPdelay
 *********************************************************************************************************************/
/*! \brief          Resets delay to the configured initial value.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_ResetPdelay(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);
# endif /* ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetPdelay
 *********************************************************************************************************************/
/*! \brief          Gets current pdelay.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         Current pdelay.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_PdelayType, ETHTSYN_CODE) EthTSyn_ComCtrl_GetPdelay(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetEthIfCtrlIdx
 *********************************************************************************************************************/
/*! \brief          Gets the EthIfCtrl index of the passed port.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         The EthIfCtrl index.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint8, ETHTSYN_CODE) EthTSyn_ComCtrl_GetEthIfCtrlIdx(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_FindComCtrlEthIfCtrl
 *********************************************************************************************************************/
/*! \brief          Searches for a matching ComCtrlEthIfCtrl.
 *  \details        -
 *  \param[in]      EthIfCtrlIdx   Index of the EthIfCtrl.
 *  \return         EthTSyn_GetSizeOfComCtrlEthIfCtrl() - No matching ComCtrlEthIfCtrl found.
 *  \return         other values - Index of the ComCtrlEthIfCtrl.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(EthTSyn_ComCtrl_EthIfCtrlIdxType, ETHTSYN_CODE) EthTSyn_ComCtrl_FindComCtrlEthIfCtrl(
  uint8 EthIfCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_FindPort
 *********************************************************************************************************************/
/*! \brief          Searches for a matching port.
 *  \details        Checks if any port upon the passed EthIf controller is part of the passed time domain. If so,
 *                  the first matching port is returned (in an end station configuration, there is only 0..1 port per
 *                  time domain and EthIf controller).
 *  \param[in]      ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                        [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \param[in]      TimeDomainId          Id of the time domain.
 *  \return         EthTSyn_GetSizeOfComCtrlPort() - No matching port found.
 *  \return         other values - Index of the matching port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_ComCtrl_PortIdxType, ETHTSYN_CODE) EthTSyn_ComCtrl_FindPort(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  uint8                            TimeDomainId);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_FindTimeDomain
 *********************************************************************************************************************/
/*! \brief          Searches for a matching time domain.
 *  \details        -
 *  \param[in]      TimeDomainId          Id of the time domain.
 *  \return         EthTSyn_GetSizeOfComCtrlTimeDomain() - No matching time domain found.
 *  \return         other values - Index of the matching time domain.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(EthTSyn_ComCtrl_TimeDomainIdxType, ETHTSYN_CODE) EthTSyn_ComCtrl_FindTimeDomain(
  uint8 TimeDomainId);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetTimeDomainId
 *********************************************************************************************************************/
/*! \brief          Gets the time domain id of the passed port.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         The time domain Id of the passed port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint8, ETHTSYN_CODE) EthTSyn_ComCtrl_GetTimeDomainId(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetSynchronizedTimeBaseId
 *********************************************************************************************************************/
/*! \brief          Gets the synchronized time base id of the passed port.
 *  \details        -
 *  \param[in]      ComCtrlPortIdx   Index of the port.
 *                                   [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return         The synchronized time base Id (handle for StbM-APIs) of the passed port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(StbM_SynchronizedTimeBaseType, ETHTSYN_CODE) EthTSyn_ComCtrl_GetSynchronizedTimeBaseId(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_COM_CTRL_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_ComCtrl_Int.h
 *********************************************************************************************************************/
