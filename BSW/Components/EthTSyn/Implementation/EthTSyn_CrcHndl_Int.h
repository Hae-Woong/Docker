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
/*!        \file  EthTSyn_CrcHndl_Int.h
 *         \unit  CrcHndl
 *        \brief  EthTSyn internal header file of the CrcHndl.
 *      \details  Interface and type definitions of the CrcHndl unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_CRC_HNDL_INT_H)
# define ETHTSYN_CRC_HNDL_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define ETHTSYN_CRC_FLAGS_MSG_LENGTH_MASK                            (0x01u)
# define ETHTSYN_CRC_FLAGS_DOMAIN_NUMBER_MASK                         (0x02u)
# define ETHTSYN_CRC_FLAGS_CORRECTION_FIELD_MASK                      (0x04u)
# define ETHTSYN_CRC_FLAGS_SRC_PORT_IDENTITY_MASK                     (0x08u)
# define ETHTSYN_CRC_FLAGS_SEQUENCE_ID_MASK                           (0x10u)
# define ETHTSYN_CRC_FLAGS_PRECISE_ORIGIN_TS_MASK                     (0x20u)

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

# if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) /* COV_MSR_UT_OPTIONAL_UNIT */
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetTimeSecuredCrc
 *********************************************************************************************************************/
/*! \brief      Computes the CRC for a time secured sub-TLV.
 *  \details    -
 *  \param[in]  CrcHndlIdx       Index of the CRC handler
 *  \param[in]  TimeSecuredFlags Configured time secured flags of master port
 *  \param[in]  TxBufPtr         Pointer to the Ethernet transmission buffer of the FollowUp message
 *  \return     The computed CRC (Time0 and Time1 CRC)
 *  \pre        The header and payload of the FollowUp message are already copied to the Tx-Buffer
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(EthTSyn_TimeSecuredCrcType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetTimeSecuredCrc(
                  EthTSyn_CrcHndl_IdxType CrcHndlIdx,
                  uint8                   TimeSecuredFlags,
  ETHTSYN_P2CONST(uint8)                  TxBufPtr);

#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetStatusTlvCrcInfo
 *********************************************************************************************************************/
/*! \brief      Gets the type and CRC of a status sub-TLV depending on the configuration of the master port.
 *  \details    Type can be "status secured" or "status not secured".
 *  \param[in]  CrcHndlIdx       Index of the CRC handler
 *  \param[in]  TxBufPtr         Pointer to the Ethernet transmission buffer of the FollowUp message
 *  \param[in]  TlvOfs           Offset of the sub-TLV within the FollowUp message
 *  \param[in]  CrcSecured       Configuration of master port whether to send CRC secured or not secured sub-TLVs
 *  \return     Type and CRC of status sub-TLV
 *  \pre        All sub-TLV information except the sub-TLV type and the Crc itself are already copied to the Tx-Buffer
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(EthTSyn_SubTlvCrcInfoType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetStatusTlvCrcInfo(
                  EthTSyn_CrcHndl_IdxType      CrcHndlIdx,
  ETHTSYN_P2CONST(uint8)                       TxBufPtr,
                  uint16                       TlvOfs,
                  EthTSyn_TxCrcSecuredEnumType CrcSecured);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetUserDataTlvCrcInfo
 *********************************************************************************************************************/
/*! \brief      Gets the type and CRC of a user data sub-TLV depending on the configuration of the master port.
 *  \details    Type can be "user data secured" or "user data not secured".
 *  \param[in]  CrcHndlIdx       Index of the CRC handler
 *  \param[in]  TxBufPtr         Pointer to the Ethernet transmission buffer of the FollowUp message
 *  \param[in]  TlvOfs           Offset of the sub-TLV within the FollowUp message
 *  \param[in]  CrcSecured       Configuration of master port whether to send CRC secured or not secured sub-TLVs
 *  \return     Type and CRC of user data sub-TLV
 *  \pre        All sub-TLV information except the sub-TLV type and the Crc itself are already copied to the Tx-Buffer
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(EthTSyn_SubTlvCrcInfoType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetUserDataTlvCrcInfo(
                  EthTSyn_CrcHndl_IdxType      CrcHndlIdx,
  ETHTSYN_P2CONST(uint8)                       TxBufPtr,
                  uint16                       TlvOfs,
                  EthTSyn_TxCrcSecuredEnumType CrcSecured);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetOfsTlvCrcInfo
 *********************************************************************************************************************/
/*! \brief      Gets the type and CRC of an offset sub-TLV depending on the configuration of the master port.
 *  \details    Type can be "offset secured" or "offset not secured".
 *  \param[in]  CrcHndlIdx       Index of the CRC handler
 *  \param[in]  TxBufPtr         Pointer to the Ethernet transmission buffer of the FollowUp message
 *  \param[in]  TlvOfs           Offset of the sub-TLV within the FollowUp message
 *  \param[in]  CrcSecured       Configuration of master port whether to send CRC secured or not secured sub-TLVs
 *  \return     Type and CRC of offset sub-TLV
 *  \pre        All sub-TLV information except the sub-TLV type and the Crc itself are already copied to the Tx-Buffer
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(EthTSyn_SubTlvCrcInfoType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetOfsTlvCrcInfo(
                  EthTSyn_CrcHndl_IdxType      CrcHndlIdx,
  ETHTSYN_P2CONST(uint8)                       TxBufPtr,
                  uint16                       TlvOfs,
                  EthTSyn_TxCrcSecuredEnumType CrcSecured);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_CheckSubTlvCrcState
 *********************************************************************************************************************/
/*! \brief      Checks if slave port accepts sub-TLV and validates CRC if required.
 *  \details    -
 *  \param[in]  CrcHndlIdx       Index of the CRC handler
 *  \param[in]  TlvInfoPtr       Pointer to relevant information about the sub-TLV:
 *                               - PTP message header
 *                               - FollowUp message payload buffer
 *                               - buffer offset in bytes referencing the beginning of the sub-TLV
 *  \param[in]  CrcCfgInfoPtr    Configuration information of slave port required for CRC validation:
 *                               - RxCrcValidated settings
 *                               - time secured flags
 *  \return     TRUE - Sub-TLV is accepted and CRC is valid (if it was required to be checked)
 *  \return     FALSE - Sub-TLV is not accepted or CRC is invalid
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_CheckSubTlvCrcState(
                  EthTSyn_CrcHndl_IdxType         CrcHndlIdx,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_CrcCfgInfoType)         CrcCfgInfoPtr);

#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_CheckTimeSecuredCrc
 *********************************************************************************************************************/
/*! \brief      Validates the CRC of a time secured sub-TLV of a FollowUp message.
 *  \details    Computes the expected CRC and validates the CRC value carried in the sub-TLV itself.
 *  \param[in]  CrcHndlIdx       Index of the CRC handler
 *  \param[in]  TlvInfoPtr       Pointer to relevant information about the sub-TLV:
 *                               - PTP message header
 *                               - FollowUp message payload buffer
 *                               - buffer offset in bytes referencing the beginning of the sub-TLV
 *  \return     TRUE - CRC is valid
 *  \return     FALSE - CRC is invalid
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_CheckTimeSecuredCrc(
                  EthTSyn_CrcHndl_IdxType         CrcHndlIdx,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr);
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_CRC_HNDL_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_CrcHndl_Int.h
 *********************************************************************************************************************/
