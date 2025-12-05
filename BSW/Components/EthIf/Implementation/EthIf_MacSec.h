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
/*!        \file  EthIf_MacSec.h
 *        \brief  EthIf MACsec public API header
 *
 *      \details  Provides access to the public API of the sub-module MACsec of EthIf.
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

#if !defined(ETHIF_MACSEC_H)
# define ETHIF_MACSEC_H

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
 *  EthIf_SwitchMacSecUpdateSecY
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to update the SecY/PAC of the provided port with the provided parameters.
 *  \details        Requests the Ethernet Switch to update the SecY/PAC of the the provided port with the provided
 *                  parameters. A Transmission Secure Channel with the provided SCI shall be configured during the first
 *                  call. A pointer to a MACsec Basic Parameters Configuration file shall be provided to create the
 *                  Secure Channel.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      MACsecCfgPtr  Pointer to the structure to configure a MACsec Entity (SecY)
 *  \param[in]      TxSci         Secure Channel Identifier for the MACsec's Transmission Secure channel
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecUpdateSecY(
  P2CONST(EthSwt_MgmtInfoType,  AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
  P2CONST(Mka_MacSecConfigType, AUTOMATIC, ETHIF_APPL_DATA) MACsecCfgPtr,
          uint64                                            TxSci);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecInitRxSc
 **********************************************************************************************************************/
/*! \brief          Requests the EhtSwt to configure a Reception Secure Channel.
 *  \details        Requests the Ethernet Switch Driver to configure a Reception Secure Channel for the given
 *                  Secure Channel Identifier.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      Sci           Secure Channel Identifier for the MACsec's Reception Secure channel
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecInitRxSc(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint64                                           Sci);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecResetRxSc
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to reset to default the MACsec values of the Reception Secure Channel.
 *  \details        Requests the Ethernet Switch Driver to reset to default the MACsec values of the Reception
 *                  Secure Channel for the given Secure Channel Identifier.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      Sci           Secure Channel Identifier for the MACsec's Reception Secure channel
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecResetRxSc(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint64                                           Sci);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecAddTxSa
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to create a Transmission Secure Association.
 *  \details        Requests the Ethernet Switch Driver to create a Transmission Secure Association in the
 *                  provided port. The Short Secure Channel Identifier is included to support XPN configurations.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      An            Association Number to use in the MACsec's transmission secure association
 *  \param[in]      NextPn        Next accepted Packet Number in the MACsec's transmission secure association
 *  \param[in]      Ssci          Short Secure Channel Identifier used in the MACsec's transmission secure association
 *  \param[in]      KeysPtr       Pointer to the SAKs Key (and needed Key information) to use in
 *                                the MACsec's transmission secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's transmission secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecAddTxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           NextPn,
          uint32                                           Ssci,
  P2CONST(Mka_SakKeyPtrType,   AUTOMATIC, ETHIF_APPL_DATA) KeysPtr,
          boolean                                          Active);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecUpdateTxSa
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to update the Transmission Secure Association.
 *  \details        Requests the Ethernet Switch Driver to update the Transmission Secure Association with the
 *                  given Packet Number. The Active parameter is included to change the specified AN status.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      An            Association Number to use in the MACsec's transmission secure association
 *  \param[in]      NextPn        Next accepted Packet Number in the MACsec's transmission secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's transmission secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecUpdateTxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           NextPn,
          boolean                                          Active);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecDeleteTxSa
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to remove the Transmission Secure Association.
 *  \details        Requests the Ethernet Switch Driver to remove the Transmission Secure Association identified
 *                  by the provided Association Number.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      An            Association Number to use in the MACsec's transmission secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecDeleteTxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecAddRxSa
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to create a Reception Secure Association.
 *  \details        Requests the Ethernet Switch Driver to create a Reception Secure Association in the provided
 *                  Port. The Short Secure Channel Identifier is included to support XPN configurations.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \param[in]      LowestPn      Lowest accepted Packet Number in the MACsec's reception secure association
 *  \param[in]      Ssci          Short Secure Channel Identifier used in the MACsec's reception secure association
 *  \param[in]      KeysPtr       Pointer to the SAKs Key (and needed Key information) to use in
 *                                the MACsec's reception secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's reception secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecAddRxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           LowestPn,
          uint32                                           Ssci,
  P2CONST(Mka_SakKeyPtrType,   AUTOMATIC, ETHIF_APPL_DATA) KeysPtr,
          boolean                                          Active);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecUpdateRxSa
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to update the Reception Secure Association.
 *  \details        Requests the Ethernet Switch Driver to update the Reception Secure Association with the given
 *                  Packet Number. The Active parameter is included to change the specified AN status.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \param[in]      LowestPn      Lowest accepted Packet Number in the MACsec's reception secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's reception secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecUpdateRxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           LowestPn,
          boolean                                          Active);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecDeleteRxSa
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to remove the Reception Secure Association.
 *  \details        Requests the Ethernet Switch Driver to remove the Reception Secure Association identified by
 *                  the provided Association Number.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecDeleteRxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecGetTxSaNextPn
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to return the Packet Number of Transmission Secure Association.
 *  \details        Requests the Ethernet Switch Driver to return the Packet Number that is used for the next packet
 *                  in the given Transmission Secure Association.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \param[out]     NextPnPtr     Pointer to the Next Packet Number read out from the MACsec Entity (SecY)
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecGetTxSaNextPn(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
  P2VAR(  uint64,              AUTOMATIC, ETHIF_APPL_VAR)  NextPnPtr);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecGetMacSecStats
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to provide MACsec statistics.
 *  \details        Requests the Ethernet switch Driver to provide MACsec statistics. The result is returned through
 *                  EthIf_SwitchMacSecGetMacSecStatsNotification.
 *  \param[in]      MgmtInfoPtr   Pointer to the management information within the context of an
 *                                Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                context of EthSwt.
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecGetMacSecStats(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr);

/**********************************************************************************************************************
 *  EthIf_SwitchMacSecSetControlledPortEnabled
 **********************************************************************************************************************/
/*! \brief          Requests the EthSwt to set the Controlled Port enabled parameter of a PAE.
 *  \details        -
 *  \param[in]      MgmtInfoPtr               Pointer to the management information within the context of an
 *                                            Ethernet Switch Driver. SwitchIdx in context of EthIf, PortIdx in
 *                                            context of EthSwt.
 *  \param[in]      ControlledPortEnabled     Boolean to activate the Controlled Port of the PAE
 *  \return         E_OK                      The request has been accepted
 *  \return         E_NOT_OK                  The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different MgmtInfoPtr, FALSE for the same MgmtInfoPtr
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecSetControlledPortEnabled(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          boolean                                          ControlledPortEnabled);

/**********************************************************************************************************************
 *  EthIf_MacSecUpdateSecY
 **********************************************************************************************************************/
/*! \brief          Requests to update the SecY/PAC of the PHY with the provided parameters.
 *  \details        Requests the Ethernet Transceiver to update the SecY/PAC of the PHY with the provided parameters.
 *                  A Transmission Secure Channel with the provided SCI shall be configured during the first call. A
 *                  pointer to a MACsec Basic Parameters Configuration file shall be provided to create the Secure
 *                  Channel.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      MACsecCfgPtr  Pointer to the structure to configure a MACsec Entity (SecY)
 *  \param[in]      TxSci         Secure Channel Identifier for the MACsec's Transmission Secure channel
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecUpdateSecY(
          uint8                                             CtrlIdx,
  P2CONST(Mka_MacSecConfigType, AUTOMATIC, ETHIF_APPL_DATA) MACsecCfgPtr,
          uint64                                            TxSci);

/**********************************************************************************************************************
 *  EthIf_MacSecInitRxSc
 **********************************************************************************************************************/
/*! \brief          Requests to configure a Reception Secure Channel.
 *  \details        Requests the Ethernet Transceiver Driver to configure a Reception Secure Channel for the given
 *                  Secure Channel Identifier.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      Sci           Secure Channel Identifier for the MACsec's Reception Secure channel
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecInitRxSc(
  uint8   CtrlIdx,
  uint64  Sci);

/**********************************************************************************************************************
 *  EthIf_MacSecResetRxSc
 **********************************************************************************************************************/
/*! \brief          Requests to reset to default the MACsec values of the Reception Secure Channel.
 *  \details        Requests the Ethernet Transceiver Driver to reset to default the MACsec values of the Reception Secure
 *                  Channel for the given Secure Channel Identifier.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      Sci           Secure Channel Identifier for the MACsec's Reception Secure channel
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecResetRxSc(
  uint8   CtrlIdx,
  uint64  Sci);

/**********************************************************************************************************************
 *  EthIf_MacSecAddTxSa
 **********************************************************************************************************************/
/*! \brief          Requests to create a Transmission Secure Association.
 *  \details        Requests the Ethernet Transceiver Driver to create a Transmission Secure Association in the
 *                  Transceiver. The Short Secure Channel Identifier is included to support XPN configurations.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      An            Association Number to use in the MACsec's transmission secure association
 *  \param[in]      NextPn        Next accepted Packet Number in the MACsec's transmission secure association
 *  \param[in]      Ssci          Short Secure Channel Identifier used in the MACsec's transmission secure association
 *  \param[in]      KeysPtr       Pointer to the SAKs Key (and needed Key information) to use in
 *                                the MACsec's transmission secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's transmission secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecAddTxSa(
          uint8                                           CtrlIdx,
          uint8                                           An,
          uint64                                          NextPn,
          uint32                                          Ssci,
  P2CONST(Mka_SakKeyPtrType, AUTOMATIC, ETHIF_APPL_DATA)  KeysPtr,
          boolean                                         Active);

/**********************************************************************************************************************
 *  EthIf_MacSecUpdateTxSa
 **********************************************************************************************************************/
/*! \brief          Requests to update the Transmission Secure Association.
 *  \details        Requests the Ethernet Transceiver Driver to update the Transmission Secure Association with the
 *                  given Packet Number. The Active parameter is included to change the specified AN status.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      An            Association Number to use in the MACsec's transmission secure association
 *  \param[in]      NextPn        Next accepted Packet Number in the MACsec's transmission secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's transmission secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecUpdateTxSa(
  uint8   CtrlIdx,
  uint8   An,
  uint64  NextPn,
  boolean Active);

/**********************************************************************************************************************
 *  EthIf_MacSecDeleteTxSa
 **********************************************************************************************************************/
/*! \brief          Requests to remove the Transmission Secure Association.
 *  \details        Requests the Ethernet Transceiver Driver to remove the Transmission Secure Association identified
 *                  by the provided Association Number.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      An            Association Number to use in the MACsec's transmission secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecDeleteTxSa(
  uint8 CtrlIdx,
  uint8 An);

/**********************************************************************************************************************
 *  EthIf_MacSecAddRxSa
 **********************************************************************************************************************/
/*! \brief          Requests to create a Reception Secure Association.
 *  \details        Requests the Ethernet Transceiver Driver to create a Reception Secure Association in the
 *                  Transceiver. The Short Secure Channel Identifier is included to support XPN configurations.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \param[in]      LowestPn      Lowest accepted Packet Number in the MACsec's reception secure association
 *  \param[in]      Ssci          Short Secure Channel Identifier used in the MACsec's reception secure association
 *  \param[in]      KeysPtr       Pointer to the SAKs Key (and needed Key information) to use in
 *                                the MACsec's reception secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's reception secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecAddRxSa(
          uint8                                           CtrlIdx,
          uint8                                           An,
          uint64                                          LowestPn,
          uint32                                          Ssci,
  P2CONST(Mka_SakKeyPtrType, AUTOMATIC, ETHIF_APPL_DATA)  KeysPtr,
          boolean                                         Active);

/**********************************************************************************************************************
 *  EthIf_MacSecUpdateRxSa
 **********************************************************************************************************************/
/*! \brief          Requests to update the Reception Secure Association.
 *  \details        Requests the Ethernet Transceiver Driver to update the Reception Secure Association with the given
 *                  Packet Number. The Active parameter is included to change the specified AN status.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \param[in]      LowestPn      Lowest accepted Packet Number in the MACsec's reception secure association
 *  \param[in]      Active        Boolean to enable/disable the MACsec's reception secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecUpdateRxSa(
  uint8   CtrlIdx,
  uint8   An,
  uint64  LowestPn,
  boolean Active);

/**********************************************************************************************************************
 *  EthIf_MacSecDeleteRxSa
 **********************************************************************************************************************/
/*! \brief          Requests to remove the Reception Secure Association.
 *  \details        Requests the Ethernet Transceiver Driver to remove the Reception Secure Association identified by
 *                  the provided Association Number.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecDeleteRxSa(
  uint8 CtrlIdx,
  uint8 An);

/**********************************************************************************************************************
 *  EthIf_MacSecGetTxSaNextPn
 **********************************************************************************************************************/
/*! \brief          Requests to return the Packet Number of Transmission Secure Association.
 *  \details        Requests the Ethernet Transceiver Driver to return the Packet Number that is used for the next
 *                  packet in the given Transmission Secure Association.
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \param[in]      An            Association Number to use in the MACsec's reception secure association
 *  \param[out]     NextPnPtr     Pointer to the Next Packet Number read out from the MACsec Entity (SecY)
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecGetTxSaNextPn(
        uint8                               CtrlIdx,
        uint8                               An,
  P2VAR(uint64, AUTOMATIC, ETHIF_APPL_VAR)  NextPnPtr);

/**********************************************************************************************************************
 *  EthIf_MacSecGetMacSecStats
 **********************************************************************************************************************/
/*! \brief          Requests the Ethernet Transceiver Driver to provide MACsec statistics.
 *  \details        -
 *  \param[in]      CtrlIdx       EthIf Controller Index
 *  \return         E_OK          The request has been accepted
 *  \return         E_NOT_OK      The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    FALSE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecGetMacSecStats(
  uint8 CtrlIdx);

/**********************************************************************************************************************
 *  EthIf_MacSecSetControlledPortEnabled
 **********************************************************************************************************************/
/*! \brief          Requests to set the Controlled Port enabled parameter of a Port Access Entity (PAE).
 *  \details        -
 *  \param[in]      CtrlIdx                   EthIf Controller Index
 *  \param[in]      ControlledPortEnabled     Boolean to activate the Controlled Port of the PAE
 *  \return         E_OK                      The request has been accepted
 *  \return         E_NOT_OK                  The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE for different CtrlIdx, FALSE for the same CtrlIdx
 *  \synchronous    TRUE
 *  \trace          CREQ-EthIf-MacSec_Interface_PhyLayer
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecSetControlledPortEnabled(
  uint8   CtrlIdx,
  boolean ControlledPortEnabled);

# endif /* ETHIF_ENABLE_MACSEC_API */

# define ETHIF_STOP_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* ETHIF_MACSEC_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_MacSec.h
 *********************************************************************************************************************/
