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
/*!        \file  EthIf_MacSec_Cbk.h
 *        \brief  EthIf MacSec public callback API header
 *
 *      \details  Provides access to the public callback API of the sub-module MacSec of EthIf.
 *
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

#if !defined (ETHIF_MACSEC_CBK_H)
# define ETHIF_MACSEC_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1975
 *********************************************************************************************************/
# include "EthIf_MacSec_Types.h"

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

# define ETHIF_START_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
/**********************************************************************************************************************
 *  EthIf_SwitchMacSecUpdateSecYNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module that requested Security Entity (SecY) update has been performed by EthSwt.
 *  \details        -
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_SecY-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecUpdateSecYNotification(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecAddTxSaNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module that requested Tx Secure Association has been created by EthSwt.
 *  \details        -
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Sa-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecAddTxSaNotification(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecAddRxSaNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module that requested Rx Secure Association has been created by EthSwt.
 *  \details        -
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Sa-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecAddRxSaNotification(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecGetMacSecStatsNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module that requested statistics are updated by EthSwt.
 *  \details        -
 *  \param[in]      MgmtInfoPtr     Pointer to the management information within the context of an
 *                                  Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                  context of EthSwt.
 *  \param[in]      MacSecStatsPtr  Pointer to a structure including the MACsec statistics of an MKA participant
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Stats-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecGetMacSecStatsNotification(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
  P2CONST(Mka_Stats_SecYType,  AUTOMATIC, ETHIF_APPL_DATA) MacSecStatsPtr);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecOperational
 **********************************************************************************************************************/
/*! \brief          To inform EthIf that MACsec is operational and update the link state stored in EthIf.
 *  \details        -
 *  \param[in]      MgmtInfoPtr       Pointer to the management information within the context of an Ethernet Switch
 *                                    Driver. SwitchIdx in context of EthIf, PortIdx in context of EthSwt.
 *  \param[in]      MacSecOperational Boolean to notify if MACsec is operational
 *  \return         E_OK              The request has been accepted
 *  \return         E_NOT_OK          The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_MacSec-Operational
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecOperational(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_VAR)  MgmtInfoPtr,
  boolean                                                  MacSecOperational);

/**********************************************************************************************************************
 *  EthIf_MacSecUpdateSecYNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module that requested Security Entity (SecY) update has been performed.
 *  \details        -
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_SecY-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_MacSecUpdateSecYNotification(
  uint8 CtrlIdx);

/**********************************************************************************************************************
 *  EthIf_MacSecAddTxSaNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module that requested Tx Secure Association has been created.
 *  \details        -
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Sa-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_MacSecAddTxSaNotification(
  uint8 CtrlIdx);

/**********************************************************************************************************************
 *  EthIf_MacSecAddRxSaNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module that requested Rx Secure Association has been created.
 *  \details        -
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Sa-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_MacSecAddRxSaNotification(
  uint8 CtrlIdx);

/**********************************************************************************************************************
 *  EthIf_MacSecGetMacSecStatsNotification
 **********************************************************************************************************************/
/*! \brief          Informs MKA module  that requested statistics are updated.
 *  \details        -
 *  \param[in]      CtrlIdx         EthIf Controller Index
 *  \param[in]      MacSecStatsPtr  Pointer to a structure including the MACsec statistics of an MKA participant
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Stats-Notification_MKA
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_MacSecGetMacSecStatsNotification(
          uint8                                           CtrlIdx,
  P2CONST(Mka_Stats_SecYType, AUTOMATIC, ETHIF_APPL_DATA) MacSecStatsPtr);

/**********************************************************************************************************************
 *  EthIf_MacSecOperational
 **********************************************************************************************************************/
/*! \brief          To inform EthIf that MACsec is operational and update the link state stored in EthIf.
 *  \details        -
 *  \param[in]      CtrlIdx           EthIf Controller Index
 *  \param[in]      MacSecOperational Boolean to notify if MACsec is operational
 *  \return         E_OK              The request has been accepted
 *  \return         E_NOT_OK          The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_MacSec-Operational
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecOperational(
  uint8   CtrlIdx,
  boolean MacSecOperational);

# endif /* ETHIF_ENABLE_MACSEC_API */

# define ETHIF_STOP_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* ETHIF_MACSEC_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_MacSec_Cbk.h
 *********************************************************************************************************************/

