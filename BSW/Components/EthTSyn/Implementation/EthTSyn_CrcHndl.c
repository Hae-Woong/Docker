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
/*!        \file  EthTSyn_CrcHndl.c
 *         \unit  CrcHndl
 *        \brief  EthTSyn CrcHndl source file
 *      \details  Implementation of the CrcHndl unit of the EthTSyn.
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

#define ETHTSYN_CRC_HNDL_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) /* COV_MSR_UT_OPTIONAL_UNIT */

# include "EthTSyn_CrcHndl_Int.h"
# include "EthTSyn_MsgDefs_Int.h"
# include "EthTSyn_Memory_Int.h"
# include "vstdlib.h"
# if (ETHTSYN_CRC_SUPPORT == STD_ON)
#  include "Crc.h"
# endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Size of the Crc Data for the different Sub-Tlvs (except time secured). */
# define ETHTSYN_CRC_STATUS_DATA_SIZE                                 (1u)
# define ETHTSYN_CRC_USER_DATA_DATA_SIZE                              (4u)
# define ETHTSYN_CRC_OFS_DATA_SIZE                                    (16u)
/* Size of the DatId in bytes */
# define ETHTSYN_CRC_DATA_ID_SIZE                                     (1u)
/* Maximum size of the Crc Data for the different Sub-Tlvs (except time secured). */
# define ETHTSYN_CRC_MAX_DATA_SIZE                                    (ETHTSYN_CRC_OFS_DATA_SIZE +\
                                                                       ETHTSYN_CRC_DATA_ID_SIZE)

 /* CRC_TIME_0 Max Size: TimeFlages (1) + DomainNumber (1) + SrcPortIdentity (10) + PreciseOriginTimestamp (10) +
 *                      DataId (1) = 23 */
# define ETHTSYN_CRC_TIME_0_MAX_DATA_SIZE                             (23u)
 /* CRC_TIME_1 Max Size: TimeFlages (1) + MsgLength (2) + CorrectionField (8) + SequenceId (2) + DataId (1) = 14 */
# define ETHTSYN_CRC_TIME_1_MAX_DATA_SIZE                             (14u)

/* Shift value required to transform the correction value in nanoseconds into the 64Bit correction field. */
# define ETHTSYN_CRC_CORR_FIELD_SHIFT                                 (16u)
/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
  ETHTSYN_CRC_HNDL_ACCEPT_MSG,
# if (ETHTSYN_CRC_SUPPORT == STD_ON)
  ETHTSYN_CRC_HNDL_VALIDATE_CRC,
# endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
  ETHTSYN_CRC_HNDL_DISCARD_MSG
} EthTSyn_CrcValidationActionEnumType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if !defined(ETHTSYN_NOUNIT_CRCHNDL)

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetTlvCrcSecuredInfo
 *********************************************************************************************************************/
/*! \brief        Determines if a sub-TLV is CRC secured, not secured or of unknown type.
 *  \details      -
 *  \param[in]    TlvStartPtr      Pointer to start of the sub-TLV in the FollowUp message buffer
 *  \return       ETHTSYN_TLV_SECURED - Sub-TLV is CRC secured
 *  \return       ETHTSYN_TLV_NOT_SECURED - Sub-TLV is not secured
 *  \return       ETHTSYN_TLV_UNKNOWN - TLV is of unknown type
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_TlvSecuredInfo, ETHTSYN_CODE) EthTSyn_CrcHndl_GetTlvCrcSecuredInfo(
  ETHTSYN_P2CONST(uint8) TlvStartPtr);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetValidationAction
 *********************************************************************************************************************/
/*! \brief        Determines, based on the sub-TLV type and the configuration of the slave port, whether the FollowUp
 *                message shall be accepted, discarded or if it depends on the CRC validation.
 *  \details      -
 *  \param[in]    RxCrcValidated   Configuration how a slave port shall handle a CRC secured sub-TLV.
 *  \param[in]    TlvSecuredInfo   Indication if sub-TLV is CRC secured, not secured or of unknown type.
 *                                 [range: ETHTSYN_TLV_SECURED, ETHTSYN_TLV_NOT_SECURED]
 *  \return       ETHTSYN_CRC_HNDL_ACCEPT_MSG - Accept FollowUp
 *  \return       ETHTSYN_CRC_HNDL_DISCARD_MSG - Discard FollowUp
 *  \return       ETHTSYN_CRC_HNDL_VALIDATE_CRC - CRC validation determines whether to accept or discard FollowUp
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_CrcValidationActionEnumType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetValidationAction(
  EthTSyn_RxCrcValidatedEnumType RxCrcValidated,
  EthTSyn_TlvSecuredInfo         TlvSecuredInfo);

#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetDataIdListEntry
 *********************************************************************************************************************/
/*! \brief        Determines the FollowUp data Id list entry which corresponds to the passed sequence Id.
 *  \details      -
 *  \param[in]    CrcHndlIdx       Index of the CRC handler
 *                                 [range: CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl()]
 *  \param[in]    SequenceId       Sequence id of FollowUp message
 *  \return       DataId list entry
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \spec
 *    requires CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_GetDataIdListEntry(
  EthTSyn_CrcHndl_IdxType CrcHndlIdx,
  uint16                  SequenceId);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_IsCrcValid
 *********************************************************************************************************************/
/*! \brief        Validates CRC of the Autosar sub-TLV.
 *  \details      -
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TimeSecuredFlags Configured time secured flags of slave port
 *                                 [range: ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED,
 *                                         ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED,
 *                                         ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED,
 *                                         ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED]
 *  \param[in]    TlvInfoPtr       Pointer to relevant information about the sub-TLV:
 *                                 - PTP message header
 *                                 - FollowUp message payload buffer
 *                                 - buffer offset in bytes referencing the beginning of the sub-TLV
 *  \return       TRUE - CRC is valid
 *  \return       FALSE - CRC is invalid
 *  \pre          Shall only be called for CRC secured Autosar sub-TLVs.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsCrcValid(
                  uint8                           DataId,
                  uint8                           TimeSecuredFlags,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_IsTimeCrcValid
 *********************************************************************************************************************/
/*! \brief        Validates CRC of a time secured sub-TLV.
 *  \details      -
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TlvInfoPtr       Pointer to relevant information about the sub-TLV:
 *                                 - PTP message header
 *                                 - FollowUp message payload buffer
 *                                 - buffer offset in bytes referencing the beginning of the sub-TLV
 *  \return       TRUE - CRC is valid
 *  \return       FALSE - CRC is invalid
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsTimeCrcValid(
                  uint8                           DataId,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_IsStatusCrcValid
 *********************************************************************************************************************/
/*! \brief        Validates CRC of a status secured sub-TLV.
 *  \details      -
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TlvStartPtr      Pointer to start of the sub-TLV in the FollowUp message buffer
 *  \return       TRUE - CRC is valid
 *  \return       FALSE - CRC is invalid
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsStatusCrcValid(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_IsUserDataCrcValid
 *********************************************************************************************************************/
/*! \brief        Validates CRC of a user data secured sub-TLV.
 *  \details      -
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TlvStartPtr      Pointer to start of the sub-TLV in the FollowUp message buffer
 *  \return       TRUE - CRC is valid
 *  \return       FALSE - CRC is invalid
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsUserDataCrcValid(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_IsOfsCrcValid
 *********************************************************************************************************************/
/*! \brief        Validates CRC of an offset timedomain secured sub-TLV.
 *  \details      -
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TlvStartPtr      Pointer to start of the sub-TLV in the FollowUp message buffer
 *  \return       TRUE - CRC is valid
 *  \return       FALSE - CRC is invalid
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsOfsCrcValid(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_ComputeTime0Crc
 *********************************************************************************************************************/
/*! \brief        Computes the CRC_TIME_0 for a time secured sub-TLV of a FollowUp message.
 *  \details      The Crc_CalculateCRC8H2F() algorithm is used for calculation.
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TimeSecuredFlags Flags determining which fields to use for the Crc calculation
 *  \param[in]    DomainNum        Timedomain number stored in FollowUp header
 *  \param[in]    SrcPortIdPtr     Source port identity stored in FollowUp header
 *  \param[in]    PotPtr           Precise origin timestamp stored in FollowUp payload
 *  \return       The computed CRC
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_ComputeTime0Crc(
                  uint8                        DataId,
                  uint8                        TimeSecuredFlags,
                  uint8                        DomainNum,
  ETHTSYN_P2CONST(EthTSyn_PortIdentityType)    SrcPortIdPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PotPtr);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_ComputeTime1Crc
 *********************************************************************************************************************/
/*! \brief        Computes the CRC_TIME_1 for a time secured sub-TLV of a FollowUp message.
 *  \details      The Crc_CalculateCRC8H2F() algorithm is used for calculation.
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TimeSecuredFlags Flags determining which fields to use for the Crc calculation
 *  \param[in]    MsgLength        Message length stored in FollowUp header
 *  \param[in]    CorrField        Correction field stored in FollowUp header
 *  \param[in]    SequenceId       Sequence id stored in FollowUp header
 *  \return       The computed CRC
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_ComputeTime1Crc(
  uint8  DataId,
  uint8  TimeSecuredFlags,
  uint16 MsgLength,
  uint64 CorrField,
  uint16 SequenceId);

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_ComputeSubTlvCrc
 *********************************************************************************************************************/
/*! \brief        Computes the CRC for a status secured, user data secured or offset secured sub-TLV.
 *  \details      The Crc_CalculateCRC8H2F() algorithm is used for calculation.
 *  \param[in]    DataId           The DataId for the FollowUp
 *  \param[in]    TlvStartPtr      Pointer to start of the sub-TLV in the FollowUp message buffer
 *  \param[in]    SubTlvDataSize   Data size of the sub-TLV
 *                                 [range: SubTlvDataSize < ETHTSYN_CRC_MAX_DATA_SIZE]
 *  \return       The computed CRC
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \spec
 *    requires SubTlvDataSize < ETHTSYN_CRC_MAX_DATA_SIZE;
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_ComputeSubTlvCrc(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr,
                  uint8  SubTlvDataSize);
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * EthTSyn_CrcHndl_GetTlvCrcSecuredInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_TlvSecuredInfo, ETHTSYN_CODE) EthTSyn_CrcHndl_GetTlvCrcSecuredInfo(
  ETHTSYN_P2CONST(uint8) TlvStartPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TlvSecuredInfo subTlvSecured = ETHTSYN_TLV_UNKNOWN_TYPE;
  const uint8 subTlvType = EthTSyn_Mem_GetUint8(TlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TYPE_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine whether TLV is CRC secured, not secured or of unknown type. */
  switch (subTlvType)
  {
  case ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED:
    {
      subTlvSecured = ETHTSYN_TLV_SECURED;
      break;
    }

  case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED:
    {
      subTlvSecured = ETHTSYN_TLV_NOT_SECURED;
      break;
    }
  default:
    {
      subTlvSecured = ETHTSYN_TLV_UNKNOWN_TYPE;
      break;
    }
  }

  return subTlvSecured;
} /* EthTSyn_CrcHndl_GetTlvCrcSecuredInfo() */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_GetValidationAction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_CrcValidationActionEnumType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetValidationAction(
  EthTSyn_RxCrcValidatedEnumType RxCrcValidated,
  EthTSyn_TlvSecuredInfo         TlvSecuredInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_CrcValidationActionEnumType validationAction = ETHTSYN_CRC_HNDL_DISCARD_MSG;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if sub-TLV is CRC secured. */
  if (TlvSecuredInfo == ETHTSYN_TLV_SECURED)
  {
    /* #100 If slave port is configured to ignore CRC, accept FollowUp message. */
    if (RxCrcValidated == ETHTSYN_RX_CRC_VALIDATED_CRC_IGNORED)
    {
      validationAction = ETHTSYN_CRC_HNDL_ACCEPT_MSG;
    }
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
    /* #101 If slave port is configured to validate CRC, do so (in case that CRC support is enabled). */
    else if ((RxCrcValidated == ETHTSYN_RX_CRC_VALIDATED_CRC_VALIDATED) ||
             (RxCrcValidated == ETHTSYN_RX_CRC_VALIDATED_CRC_OPTIONAL))
    {
      validationAction = ETHTSYN_CRC_HNDL_VALIDATE_CRC;
    }
    /* #102 Otherwise, discard message. */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
    else
    {
      validationAction = ETHTSYN_CRC_HNDL_DISCARD_MSG;
    }
  }
  /* #11 Otherwise. */
  else
  {
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
    /* #110 If slave port is configured to validate CRC, discard FollowUp message (in case that CRC support is
     *      enabled). */
    if (RxCrcValidated == ETHTSYN_RX_CRC_VALIDATED_CRC_VALIDATED)
    {
      validationAction = ETHTSYN_CRC_HNDL_DISCARD_MSG;
    }
    /* #111 Otherwise, accept FollowUp message. */
    else
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
    {
      validationAction = ETHTSYN_CRC_HNDL_ACCEPT_MSG;
    }
  }

  return validationAction;
} /* EthTSyn_CrcHndl_GetValidationAction() */

#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_CrcHndl_GetDataIdListEntry
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_GetDataIdListEntry(
  EthTSyn_CrcHndl_IdxType CrcHndlIdx,
  uint16                  SequenceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 dataListSize = EthTSyn_GetFupDataIdListEndIdxOfCrcHndl(CrcHndlIdx) -
      EthTSyn_GetFupDataIdListStartIdxOfCrcHndl(CrcHndlIdx);
  uint8 dataListIdx = (uint8)(SequenceId % dataListSize) +
    EthTSyn_GetFupDataIdListStartIdxOfCrcHndl(CrcHndlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return FollowUp data Id list entry depending on the sequence Id. */
  return EthTSyn_GetFupDataIdList(dataListIdx);
} /* EthTSyn_CrcHndl_GetDataIdListEntry() */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_IsCrcValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsCrcValid(
                  uint8                           DataId,
                  uint8                           TimeSecuredFlags,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isCrcValid = FALSE;
  ETHTSYN_P2CONST(uint8) tlvStartPtr = &TlvInfoPtr->FupPayloadPtr[TlvInfoPtr->SubTlvOfs];
  const uint8 subTlvType = EthTSyn_Mem_GetUint8(tlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TYPE_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check sub-TLV type. */
  switch (subTlvType)
  {
  case ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED:
    /* #100 If type is time secured, Check if TimeSecuredFlags of FollowUp message are identical to the configured
     *      ones of the slave port. */
    {
      if (TimeSecuredFlags == EthTSyn_Mem_GetUint8(tlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_FLAGS_OFS))
      {
        /* #1000 Validate CRC. */
        isCrcValid = EthTSyn_CrcHndl_IsTimeCrcValid(DataId, TlvInfoPtr);
      }
      break;
    }
  case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED:
    /* #101 If type is status secured, validate CRC. */
    {
      isCrcValid = EthTSyn_CrcHndl_IsStatusCrcValid(DataId, tlvStartPtr);
      break;
    }
  case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED:
    /* #102 If type is user data secured, validate CRC. */
    {
      isCrcValid = EthTSyn_CrcHndl_IsUserDataCrcValid(DataId, tlvStartPtr);
      break;
    }
  default: /* ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED */
    /* #103 If type is offset timedomain secured, validate CRC. */
    {
      isCrcValid = EthTSyn_CrcHndl_IsOfsCrcValid(DataId, tlvStartPtr);
      break;
    }
  }

  return isCrcValid;
} /* EthTSyn_CrcHndl_IsCrcValid() */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_IsTimeCrcValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsTimeCrcValid(
                  uint8                           DataId,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2CONST(uint8) tlvStartPtr = &TlvInfoPtr->FupPayloadPtr[TlvInfoPtr->SubTlvOfs];
  const uint8 timeSecuredFlags = EthTSyn_Mem_GetUint8(tlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_FLAGS_OFS);
  const uint8 crcTime0OfFup = EthTSyn_Mem_GetUint8(tlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_0_OFS);
  const uint8 crcTime1OfFup = EthTSyn_Mem_GetUint8(tlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_1_OFS);
  /* Correction field of header struct only contains the first 48 bit of the serialized field. */
  const uint64 correctionField = (TlvInfoPtr->PtpHdrPtr->CorrectionFieldNs << ETHTSYN_CRC_CORR_FIELD_SHIFT) |
    (uint64)TlvInfoPtr->PtpHdrPtr->CorrectionFieldSubNs;
  uint8 crcTime0Computed = 0u;
  uint8 crcTime1Computed = 0u;
  EthTSyn_PortIdentityType srcPortId = {0u};
  EthTSyn_GlobalTimestampType pot = {0u};

  /* ----- Implementation ----------------------------------------------- */
  srcPortId.ClockIdentity = TlvInfoPtr->PtpHdrPtr->SourceClockId;
  srcPortId.PortNumber = TlvInfoPtr->PtpHdrPtr->SourcePortNumber;
  pot.secondsHi = EthTSyn_Mem_GetUint16(TlvInfoPtr->FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_HI_PL_REL_OFS);
  pot.seconds = EthTSyn_Mem_GetUint32(TlvInfoPtr->FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_PL_REL_OFS);
  pot.nanoseconds = EthTSyn_Mem_GetUint32(TlvInfoPtr->FupPayloadPtr, ETHTSYN_MSG_FUP_POT_NSEC_PL_REL_OFS);

  /* #10 Compute CRC and compare it with values of FollowUp message. */
  crcTime0Computed = EthTSyn_CrcHndl_ComputeTime0Crc(DataId, timeSecuredFlags, TlvInfoPtr->PtpHdrPtr->DomainNumber,
    &srcPortId, &pot);
  crcTime1Computed = EthTSyn_CrcHndl_ComputeTime1Crc(DataId, timeSecuredFlags, TlvInfoPtr->PtpHdrPtr->MessageLength,
    correctionField, TlvInfoPtr->PtpHdrPtr->SequenceId);

  return (boolean)((crcTime0Computed == crcTime0OfFup) && (crcTime1Computed == crcTime1OfFup));
} /* EthTSyn_CrcHndl_IsTimeCrcValid() */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_IsStatusCrcValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsStatusCrcValid(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 crc = EthTSyn_Mem_GetUint8(TlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_STATUS_CRC_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compute CRC and compare it with value of FollowUp message. */
  return (boolean)(crc == EthTSyn_CrcHndl_ComputeSubTlvCrc(DataId, TlvStartPtr, ETHTSYN_CRC_STATUS_DATA_SIZE));
} /* EthTSyn_CrcHndl_IsStatusCrcValid() */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_IsUserDataCrcValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsUserDataCrcValid(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 crc = EthTSyn_Mem_GetUint8(TlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_CRC_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compute CRC and compare it with value of FollowUp message. */
  return (boolean)(crc == EthTSyn_CrcHndl_ComputeSubTlvCrc(DataId, TlvStartPtr, ETHTSYN_CRC_USER_DATA_DATA_SIZE));
} /* EthTSyn_CrcHndl_IsUserDataCrcValid() */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_IsOfsCrcValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_IsOfsCrcValid(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 crc = EthTSyn_Mem_GetUint8(TlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_CRC_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compute CRC and compare it with value of FollowUp message. */
  return (boolean)(crc == EthTSyn_CrcHndl_ComputeSubTlvCrc(DataId, TlvStartPtr, ETHTSYN_CRC_OFS_DATA_SIZE));
} /* EthTSyn_CrcHndl_IsOfsCrcValid() */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_ComputeTime0Crc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_ComputeTime0Crc(
                  uint8                        DataId,
                  uint8                        TimeSecuredFlags,
                  uint8                        DomainNum,
  ETHTSYN_P2CONST(EthTSyn_PortIdentityType)    SrcPortIdPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PotPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_TIME_0_MAX_DATA_SIZE] = {0};
  uint16 crcDataLength = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add the value of time secured flags to CRC data. */
  crcData[crcDataLength] = TimeSecuredFlags;
  crcDataLength += 1u;

  /* #20 Add Domain Number to CRC data if applicable. */
  if ((TimeSecuredFlags & ETHTSYN_CRC_FLAGS_DOMAIN_NUMBER_MASK) != 0u)
  {
    EthTSyn_Mem_PutUint8(crcData, crcDataLength, DomainNum);
    crcDataLength += ETHTSYN_MSG_HDR_DOMAIN_NUM_SIZE;
  }

  /* #30 Add Source Port Identity to CRC data if applicable. */
  if ((TimeSecuredFlags & ETHTSYN_CRC_FLAGS_SRC_PORT_IDENTITY_MASK) != 0u)
  {
    EthTSyn_Mem_PutUint64(crcData, crcDataLength, SrcPortIdPtr->ClockIdentity);
    crcDataLength += ETHTSYN_MSG_HDR_SRC_PORT_IDENT_CLOCK_IDENT_SIZE;
    EthTSyn_Mem_PutUint16(crcData, crcDataLength, SrcPortIdPtr->PortNumber);
    crcDataLength += ETHTSYN_MSG_HDR_SRC_PORT_IDENT_PORT_NUM_SIZE;
  }

  /* #40 Add Precise Origin Timestamp to CRC data if applicable. */
  if ((TimeSecuredFlags & ETHTSYN_CRC_FLAGS_PRECISE_ORIGIN_TS_MASK) != 0u)
  {
    EthTSyn_Mem_PutUint16(crcData, crcDataLength, PotPtr->secondsHi);
    crcDataLength += ETHTSYN_MSG_TS_SEC_HI_SIZE;
    EthTSyn_Mem_PutUint32(crcData, crcDataLength, PotPtr->seconds);
    crcDataLength += ETHTSYN_MSG_TS_SEC_LOW_SIZE;
    EthTSyn_Mem_PutUint32(crcData, crcDataLength, PotPtr->nanoseconds);
    crcDataLength += ETHTSYN_MSG_TS_NSEC_SIZE;
  }

  /* #50 Add DataId to CRC data. */
  crcData[crcDataLength] = DataId;
  crcDataLength += ETHTSYN_CRC_DATA_ID_SIZE;

  /* #60 Calculate and return the CRC value. */
  return Crc_CalculateCRC8H2F(crcData, crcDataLength, 0u, TRUE);
} /* EthTSyn_CrcHndl_ComputeTime0Crc */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_ComputeTime1Crc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_ComputeTime1Crc(
  uint8  DataId,
  uint8  TimeSecuredFlags,
  uint16 MsgLength,
  uint64 CorrField,
  uint16 SequenceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_TIME_1_MAX_DATA_SIZE] = {0};
  uint16 crcDataLength = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add the value of time secured flags to CRC data. */
  crcData[crcDataLength] = TimeSecuredFlags;
  crcDataLength += 1u;

  /* #20 Add Message Length to CRC data if applicable. */
  if ((TimeSecuredFlags & ETHTSYN_CRC_FLAGS_MSG_LENGTH_MASK) != 0u)
  {
    EthTSyn_Mem_PutUint16(crcData, crcDataLength, MsgLength);
    crcDataLength += ETHTSYN_MSG_HDR_MSG_LENGTH_SIZE;
  }

  /* #30 Add Correction field to CRC data if applicable. */
  if ((TimeSecuredFlags & ETHTSYN_CRC_FLAGS_CORRECTION_FIELD_MASK) != 0u)
  {
    EthTSyn_Mem_PutUint64(crcData, crcDataLength, CorrField);
    crcDataLength += ETHTSYN_MSG_HDR_CORR_FIELD_SIZE;
  }

  /* #40 Add Sequence Id to CRC data if applicable. */
  if ((TimeSecuredFlags & ETHTSYN_CRC_FLAGS_SEQUENCE_ID_MASK) != 0u)
  {
    EthTSyn_Mem_PutUint16(crcData, crcDataLength, SequenceId);
    crcDataLength += ETHTSYN_MSG_HDR_SEQUENCE_ID_SIZE;
  }

  /* #50 Add DataId to CRC data. */
  crcData[crcDataLength] = DataId;
  crcDataLength += ETHTSYN_CRC_DATA_ID_SIZE;

  /* #60 Calculate and return the CRC value. */
  return Crc_CalculateCRC8H2F(crcData, crcDataLength, 0, TRUE);
} /* EthTSyn_CrcHndl_ComputeTime1Crc */

/**********************************************************************************************************************
 * EthTSyn_CrcHndl_ComputeSubTlvCrc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CrcHndl_ComputeSubTlvCrc(
                  uint8  DataId,
  ETHTSYN_P2CONST(uint8) TlvStartPtr,
                  uint8  SubTlvDataSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_MAX_DATA_SIZE] = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add sub-TLV data to CRC data. */
  /* PRQA S 0315 2 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemCpy_s(crcData, ETHTSYN_CRC_MAX_DATA_SIZE, &TlvStartPtr[ETHTSYN_MSG_FUP_AR_SUB_TLV_DATA_OFS],
    SubTlvDataSize);

  /* #20 Add DataId to CRC data. */
  crcData[SubTlvDataSize] = DataId;

  /* #30 Calculate and return the CRC value. */
  return Crc_CalculateCRC8H2F(crcData, (uint32)SubTlvDataSize + (uint32)ETHTSYN_CRC_DATA_ID_SIZE, 0, TRUE);
} /* EthTSyn_CrcHndl_ComputeSubTlvCrc */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetTimeSecuredCrc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(EthTSyn_TimeSecuredCrcType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetTimeSecuredCrc(
                  EthTSyn_CrcHndl_IdxType CrcHndlIdx,
                  uint8                   TimeSecuredFlags,
  ETHTSYN_P2CONST(uint8)                  TxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeSecuredCrcType crc = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if CRC handler index is valid. */
  if (CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl())
  {
    const uint8 domainNum = EthTSyn_Mem_GetUint8(TxBufPtr, ETHTSYN_MSG_HDR_DOMAIN_NUM_OFS);
    const uint16 msgLength = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_MSG_LENGTH_OFS);
    const uint64 corrField = EthTSyn_Mem_GetUint64(TxBufPtr, ETHTSYN_MSG_HDR_CORR_FIELD_OFS);
    const uint16 sequenceId = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS);
    const uint8 dataId = EthTSyn_CrcHndl_GetDataIdListEntry(CrcHndlIdx, sequenceId);
    EthTSyn_PortIdentityType srcPortId = {0u};
    EthTSyn_GlobalTimestampType pot = {0u};

    srcPortId.ClockIdentity = EthTSyn_Mem_GetUint64(TxBufPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_CLOCK_IDENT_OFS);
    srcPortId.PortNumber = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_PORT_NUM_OFS);
    pot.secondsHi = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_FUP_POT_SEC_HI_OFS);
    pot.seconds = EthTSyn_Mem_GetUint32(TxBufPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_OFS);
    pot.nanoseconds = EthTSyn_Mem_GetUint32(TxBufPtr, ETHTSYN_MSG_FUP_POT_NSEC_OFS);

    /* #20 Compute and return CRC. */
    crc.Time0Crc = EthTSyn_CrcHndl_ComputeTime0Crc(dataId, TimeSecuredFlags, domainNum, &srcPortId, &pot);
    crc.Time1Crc = EthTSyn_CrcHndl_ComputeTime1Crc(dataId, TimeSecuredFlags, msgLength, corrField, sequenceId);
  }

  return crc;
} /* EthTSyn_CrcHndl_GetTimeSecuredCrc() */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetStatusTlvCrcInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(EthTSyn_SubTlvCrcInfoType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetStatusTlvCrcInfo(
                  EthTSyn_CrcHndl_IdxType      CrcHndlIdx,
  ETHTSYN_P2CONST(uint8)                       TxBufPtr,
                  uint16                       TlvOfs,
                  EthTSyn_TxCrcSecuredEnumType CrcSecured)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SubTlvCrcInfoType crcInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If CRC support is enabled, CRC handler index is valid and master port sends secured sub-TLV, set type to
   *     "status secured" and calculate CRC. */
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
  if ((CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl()) && (CrcSecured == ETHTSYN_TX_CRC_SECURED_CRC_SUPPORTED))
  {
    const uint16 sequenceId = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS);
    const uint8 dataId = EthTSyn_CrcHndl_GetDataIdListEntry(CrcHndlIdx, sequenceId);
    crcInfo.SubTlvType = ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED;
    crcInfo.SubTlvCrc = EthTSyn_CrcHndl_ComputeSubTlvCrc(dataId, &TxBufPtr[TlvOfs], ETHTSYN_CRC_STATUS_DATA_SIZE);
  }
  else
#  else
  ETHTSYN_DUMMY_STATEMENT(CrcHndlIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(TxBufPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(TlvOfs); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(CrcSecured); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
  /* #11 Otherwise set type to "status not secured" and CRC to "0". */
  {
    crcInfo.SubTlvType = ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED;
    crcInfo.SubTlvCrc = 0u;
  }

  return crcInfo;
} /* EthTSyn_CrcHndl_GetStatusTlvCrcInfo() */

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetUserDataTlvCrcInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(EthTSyn_SubTlvCrcInfoType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetUserDataTlvCrcInfo(
                  EthTSyn_CrcHndl_IdxType      CrcHndlIdx,
  ETHTSYN_P2CONST(uint8)                       TxBufPtr,
                  uint16                       TlvOfs,
                  EthTSyn_TxCrcSecuredEnumType CrcSecured)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SubTlvCrcInfoType crcInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If CRC support is enabled, CRC handler index is valid and master port sends secured sub-TLV, set type to
   *     "user data secured" and calculate CRC. */
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
  if ((CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl()) && (CrcSecured == ETHTSYN_TX_CRC_SECURED_CRC_SUPPORTED))
  {
    const uint16 sequenceId = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS);
    const uint8 dataId = EthTSyn_CrcHndl_GetDataIdListEntry(CrcHndlIdx, sequenceId);
    crcInfo.SubTlvType = ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED;
    crcInfo.SubTlvCrc = EthTSyn_CrcHndl_ComputeSubTlvCrc(dataId, &TxBufPtr[TlvOfs], ETHTSYN_CRC_USER_DATA_DATA_SIZE);
  }
  else
#  else
  ETHTSYN_DUMMY_STATEMENT(CrcHndlIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(TxBufPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(TlvOfs); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(CrcSecured); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
  /* #11 Otherwise set type to "user data not secured" and CRC to "0". */
  {
    crcInfo.SubTlvType = ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED;
    crcInfo.SubTlvCrc = 0u;
  }

  return crcInfo;
} /* EthTSyn_CrcHndl_GetUserDataTlvCrcInfo() */

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_GetOfsTlvCrcInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(EthTSyn_SubTlvCrcInfoType, ETHTSYN_CODE) EthTSyn_CrcHndl_GetOfsTlvCrcInfo(
                  EthTSyn_CrcHndl_IdxType      CrcHndlIdx,
  ETHTSYN_P2CONST(uint8)                       TxBufPtr,
                  uint16                       TlvOfs,
                  EthTSyn_TxCrcSecuredEnumType CrcSecured)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SubTlvCrcInfoType crcInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If CRC support is enabled, CRC handler index is valid and master port sends secured sub-TLV, set type to
   *     "offset secured" and calculate CRC. */
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
  if ((CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl()) && (CrcSecured == ETHTSYN_TX_CRC_SECURED_CRC_SUPPORTED))
  {
    const uint16 sequenceId = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS);
    const uint8 dataId = EthTSyn_CrcHndl_GetDataIdListEntry(CrcHndlIdx, sequenceId);
    crcInfo.SubTlvType = ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED;
    crcInfo.SubTlvCrc = EthTSyn_CrcHndl_ComputeSubTlvCrc(dataId, &TxBufPtr[TlvOfs], ETHTSYN_CRC_OFS_DATA_SIZE);
  }
  else
#  else
  ETHTSYN_DUMMY_STATEMENT(CrcHndlIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(TxBufPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(TlvOfs); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(CrcSecured); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
  /* #11 Otherwise set type to "offset not secured" and CRC to "0". */
  {
    crcInfo.SubTlvType = ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED;
    crcInfo.SubTlvCrc = 0u;
  }

  return crcInfo;
} /* EthTSyn_CrcHndl_GetOfsTlvCrcInfo() */

/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_CheckSubTlvCrcState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_CheckSubTlvCrcState(
                  EthTSyn_CrcHndl_IdxType         CrcHndlIdx,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_CrcCfgInfoType)         CrcCfgInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isSubTlvValid = FALSE;
  EthTSyn_TlvSecuredInfo tlvSecuredInfo = EthTSyn_CrcHndl_GetTlvCrcSecuredInfo(
    &TlvInfoPtr->FupPayloadPtr[TlvInfoPtr->SubTlvOfs]);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If TLV type is unknown, regard TLV as valid. */
  if (tlvSecuredInfo == ETHTSYN_TLV_UNKNOWN_TYPE)
  {
    isSubTlvValid = TRUE;
  }
  /* #20 Otherwise accept or discard message or validate CRC, depending on the sub-TLV type and slave port
   *     configuration. */
  else
  {
    EthTSyn_CrcValidationActionEnumType validationAction = EthTSyn_CrcHndl_GetValidationAction(
      CrcCfgInfoPtr->RxCrcValidated, tlvSecuredInfo);

    if (validationAction == ETHTSYN_CRC_HNDL_ACCEPT_MSG)
    {
      isSubTlvValid = TRUE;
    }
#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
    else if (validationAction == ETHTSYN_CRC_HNDL_VALIDATE_CRC)
    {
      /* #30 Check if Crc handler index is valid in case Crc needs to be validated and validate Crc. */
      if (CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl())
      {
        const uint8 dataId = EthTSyn_CrcHndl_GetDataIdListEntry(CrcHndlIdx, TlvInfoPtr->PtpHdrPtr->SequenceId);
        isSubTlvValid = EthTSyn_CrcHndl_IsCrcValid(dataId, CrcCfgInfoPtr->TimeSecuredFlags, TlvInfoPtr);
      }
    }
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */
    else /* validationAction == ETHTSYN_CRC_HNDL_DISCARD_MSG */
    {
      /* Discard message -> return FALSE. */
    }
  }

#  if (ETHTSYN_CRC_SUPPORT == STD_OFF)
  ETHTSYN_DUMMY_STATEMENT(CrcHndlIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_OFF) */

  return isSubTlvValid;
} /* EthTSyn_CrcHndl_CheckSubTlvCrcState() */

#  if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_CrcHndl_CheckTimeSecuredCrc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CrcHndl_CheckTimeSecuredCrc(
                  EthTSyn_CrcHndl_IdxType         CrcHndlIdx,
  ETHTSYN_P2CONST(EthTSyn_ValidateSubTlvInfoType) TlvInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean crcIsValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if CRC handler index is valid. */
  if (CrcHndlIdx < EthTSyn_GetSizeOfCrcHndl())
  {
    const uint8 dataId = EthTSyn_CrcHndl_GetDataIdListEntry(CrcHndlIdx, TlvInfoPtr->PtpHdrPtr->SequenceId);

    /* #20 Validate CRC. */
    crcIsValid = EthTSyn_CrcHndl_IsTimeCrcValid(dataId, TlvInfoPtr);
  }

  return crcIsValid;
} /* EthTSyn_CrcHndl_CheckTimeSecuredCrc() */
#  endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

# endif /* ETHTSYN_NOUNIT_CRCHNDL */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_CrcHndl.c
 *********************************************************************************************************************/
