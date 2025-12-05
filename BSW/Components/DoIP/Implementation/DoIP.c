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
/*!        \file  DoIP.c
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Implementation file of Diagnostic over IP
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

#define DOIP_SOURCE

/**********************************************************************************************************************
 * LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/* PRQA  S 1881 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA  S 4304 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA  S 4404 EOF */ /* MD_MSR_AutosarBoolean */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "DoIP.h" /* includes ComStack_Types.h */
#include "DoIP_Priv.h" /* includes SoAd.h */
#include "DoIP_Cbk.h"
#include "IpBase.h"
#include "PduR_DoIP.h"
#include "SoAd.h"
#include "vstdlib.h"
#if ( DOIP_CHANNEL_READY_ENABLED == STD_ON )
# include "BswM_DoIP.h"
#endif /* DOIP_CHANNEL_READY_ENABLED == STD_ON */
#if ( DOIP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE == DOIP_CONFIGURATION_VARIANT )
# include "EcuM_Error.h"
#endif /* DOIP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE == DOIP_CONFIGURATION_VARIANT */

#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check consistency of source and header file. */
#if ( (DOIP_SW_MAJOR_VERSION != 16u) || (DOIP_SW_MINOR_VERSION != 0u) || (DOIP_SW_PATCH_VERSION != 0u) )
# error "DoIP.c: Source and Header file are inconsistent!"
#endif /* (DOIP_SW_MAJOR_VERSION != 16u) || (DOIP_SW_MINOR_VERSION != 0u) || (DOIP_SW_PATCH_VERSION != 0u) */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if ( !defined (DOIP_LOCAL) ) /* COV_DOIP_COMPATIBILITY */
# define DOIP_LOCAL static
#endif /* !defined (DOIP_LOCAL) */

#if ( !defined (DOIP_LOCAL_INLINE) ) /* COV_DOIP_COMPATIBILITY */
# define DOIP_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (DOIP_LOCAL_INLINE) */

#define DOIP_ACK_VEH_IDENT_WO_SYNC_STAT_LEN_BYTE 32u
#define DOIP_ACK_VEH_IDENT_W_SYNC_STAT_LEN_BYTE  33u
#define DOIP_ACK_VEH_IDENT_MAX_LEN_BYTE          DOIP_ACK_VEH_IDENT_W_SYNC_STAT_LEN_BYTE

#if ( DOIP_ENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON )
# define DOIP_ACK_ENTITY_STATE_LEN_BYTE       7u
#else
# define DOIP_ACK_ENTITY_STATE_LEN_BYTE       3u
#endif /* DOIP_ENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON */

#define DOIP_VIN_LEN_BYTE                     17u
#define DOIP_GID_LEN_BYTE                     6u
#define DOIP_EID_LEN_BYTE                     6u
#define DOIP_ADDR_LEN_BYTE                    2u
#define DOIP_ADDR_ANY                         ((uint16)0xFFFFu)
#define DOIP_GEN_HDR_LEN_BYTE                 8u
#define DOIP_GEN_HDR_TYPE_IDX                 2u
#define DOIP_GEN_HDR_NACK_CODE_LEN_BYTE       1u
#define DOIP_ROUT_ACTIV_REQ_MIN_LEN_BYTE      7u
#define DOIP_ROUT_ACTIV_REQ_MAX_LEN_BYTE      11u
#define DOIP_ROUT_ACTIV_REQ_TYPE_LEN_BYTE     1u
#define DOIP_ROUT_ACTIV_REQ_RSVD_LEN_BYTE     4u
#define DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE      4u
#define DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE      9u
#define DOIP_ROUT_ACTIV_ACK_MAX_LEN_BYTE      13u
#define DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE      4u
#define DOIP_DIAG_HDR_LEN_BYTE                4u
#define DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE        4u
#define DOIP_DIAG_MSG_ACK_LEN_BYTE            5u

#define DOIP_HOST_NAME_MIN_SIZE               5u  /* Hostname option size minimum:              "DoIP-" */
#define DOIP_HOST_NAME_VIN_PREFIX_SIZE        8u  /* Hostname option size with VIN prefix only: "DoIP-VIN" */
#define DOIP_HOST_NAME_VIN_SIZE               25u /* Hostname option size with VIN:             "DoIP-VIN<VinValue>" */

#define DOIP_IF_API_ENABLED                   DOIP_IFTXBUF

#define DOIP_DHCP_IPADDRESS_LIST_MAX_SIZE     4u
#define DOIP_PORT_NUMBER                      13400u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
#define DOIP_MIN(a, b)                        (((a) <= (b)) ? (a) : (b))

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define DOIP_START_SEC_VAR_CLEARED_8
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( (DOIP_SUPPORT_SHUTDOWN == STD_ON) || (DOIP_DEV_ERROR_DETECT == STD_ON) ) /* COV_DOIP_SUPPORTED */
/*! Indicates current state of the module state machine (valid values: DOIP_STATE_*). */
DOIP_LOCAL VAR(DoIP_StateType, DOIP_VAR_CLEARED) DoIP_State = DOIP_STATE_UNINIT;
#endif /* (DOIP_SUPPORT_SHUTDOWN == STD_ON) || (DOIP_DEV_ERROR_DETECT == STD_ON) */

#define DOIP_STOP_SEC_VAR_CLEARED_8
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*****************************************************/

#define DOIP_START_SEC_VAR_CLEARED_32
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/*! Counter for the number of dropped TCP packets. */
DOIP_LOCAL VAR(DoIP_MeasurementDataType, DOIP_VAR_CLEARED) DoIP_Measure_DroppedTcpPacketsCounter = 0u;

/*! Counter for the number of dropped UDP packets. */
DOIP_LOCAL VAR(DoIP_MeasurementDataType, DOIP_VAR_CLEARED) DoIP_Measure_DroppedUdpPacketsCounter = 0u;
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#define DOIP_STOP_SEC_VAR_CLEARED_32
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*****************************************************/

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the configuration. */
P2CONST(DoIP_ConfigType, DOIP_VAR_CLEARED, DOIP_PBCFG) DoIP_ConfigDataPtr = NULL_PTR;

#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*****************************************************/

#define DOIP_START_SEC_VAR_NO_INIT_8
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
/*! Indicates if Pdu size routing is enabled. */
DOIP_LOCAL VAR(boolean, DOIP_VAR_NO_INIT) DoIP_IsPduSizeRoutingEnabled;
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
/*! Indicates if GID is synchronised. */
DOIP_LOCAL VAR(boolean, DOIP_VAR_NO_INIT) DoIP_GidInSync;
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

#define DOIP_STOP_SEC_VAR_NO_INIT_8
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*****************************************************/

#define DOIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "DoIP_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
/*! Number of connections with assigned local address. */
DOIP_LOCAL VAR(DoIP_SizeOfConnectionType, DOIP_VAR_NO_INIT) DoIP_LocalAddrAssignedCounter;
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

#define DOIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "DoIP_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIP_Util_InitCons()
 *********************************************************************************************************************/
/*! \brief          Initializes all connections.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitCons(void);

/**********************************************************************************************************************
 *  DoIP_Util_InitChannels()
 *********************************************************************************************************************/
/*! \brief          Initializes all channels.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitChannels(void);

/**********************************************************************************************************************
 *  DoIP_Util_InitTesters()
 *********************************************************************************************************************/
/*! \brief          Initializes all testers.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitTesters(void);

/**********************************************************************************************************************
 *  DoIP_Util_InitLocalAddrs()
 *********************************************************************************************************************/
/*! \brief          Initializes all IP address assignments.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitLocalAddrs(void);

/**********************************************************************************************************************
 *  DoIP_Util_InitRoutActivHandlers()
 *********************************************************************************************************************/
/*! \brief          Initializes all routing activation handlers.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitRoutActivHandlers(void);

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_InitOemPayloadTypes()
 *********************************************************************************************************************/
/*! \brief          Initializes all OemPayloadTypes variables.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitOemPayloadTypes(void);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Util_InitInterfaces()
 *********************************************************************************************************************/
/*! \brief          Initializes all interface variables.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitInterfaces(void);

#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_ValidateConfig()
 *********************************************************************************************************************/
/*! \brief          Validates the module configuration.
 *  \details        -
 *  \param[in]      DoIPConfigPtr   Pointer to the configuration data of the DoIP module.
 *  \return         E_OK            Module configuration is valid.
 *  \return         E_NOT_OK        Module configuration is invalid.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DEV_ERROR_DETECT
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_ValidateConfig(
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_CONST) DoIPConfigPtr);
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Util_Init()
 *********************************************************************************************************************/
/*! \brief          Initializes the module data structures.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_Init(void);

/**********************************************************************************************************************
 *  DoIP_Util_FillGenHdr()
 *********************************************************************************************************************/
/*! \brief          Fills generic header data into buffer.
 *  \details        -
 *  \param[in]      HdrTypeField  Header type field value.
 *                                [range: undefined]
 *  \param[in]      HdrLenField   Header length field value.
 *                                [range: undefined]
 *  \param[out]     TgtBufferPtr  Pointer to target buffer.
 *                                [range: POINTER != NULL_PTR && DOIP_GEN_HDR_LEN_BYTE <= sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGenHdr(
  uint16 HdrTypeField,
  uint32 HdrLenField,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Util_FillVin()
 *********************************************************************************************************************/
/*! \brief          Fills VIN data into buffer.
 *  \details        -
 *  \param[out]     TgtBufferPtr  Pointer to target buffer.
 *                                [range: POINTER != NULL_PTR && DOIP_VIN_LEN_BYTE <= sizeof(POINTER)]
 *  \return         E_OK          VIN successful written to buffer.
 *  \return         E_NOT_OK      VIN invalid pattern is written to buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPRetrieveVin
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillVin(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Util_FillLocalLogAddr()
 *********************************************************************************************************************/
/*! \brief          Fills logical address data into buffer.
 *  \details        -
 *  \param[out]     TgtBufferPtr  Pointer to target buffer.
 *                                [range: POINTER != NULL_PTR && DOIP_ADDR_LEN_BYTE <= sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillLocalLogAddr(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);

/**********************************************************************************************************************
 *  DoIP_Util_FillEid()
 *********************************************************************************************************************/
/*! \brief          Fills EID data into buffer.
 *  \details        -
 *  \param[out]     TgtBufferPtr    Pointer to target buffer.
 *                                  [range: POINTER != NULL_PTR && DOIP_EID_LEN_BYTE <= sizeof(POINTER)]
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \return         E_OK            Data successful written.
 *  \return         E_NOT_OK        Data not successful written.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPPhysicalAddressAsEid
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillEid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Util_FillGid()
 *********************************************************************************************************************/
/*! \brief          Fills GID data into buffer.
 *  \details        -
 *  \param[out]     TgtBufferPtr    Pointer to target buffer.
 *                                  [range: POINTER != NULL_PTR && DOIP_GID_LEN_BYTE <= sizeof(POINTER)]
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \return         E_OK            Data successful written.
 *  \return         E_NOT_OK        Data not successful written.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPRetrieveGid
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillGid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  DoIP_SizeOfConnectionType ConnectionIdx);

#if ( !((DOIP_VIN_GID_MASTER == STD_ON ) && (DOIP_USE_EID_AS_GID == STD_ON)) )
/**********************************************************************************************************************
 *  DoIP_Util_FillGidOrInv()
 *********************************************************************************************************************/
/*! \brief          Fills GID or invalid data into buffer.
 *  \details        -
 *  \param[out]     TgtBufferPtr  Pointer to target buffer.
 *                                [range: POINTER != NULL_PTR && DOIP_GID_LEN_BYTE <= sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPRetrieveGid
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGidOrInv(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr);
#endif /* !((DOIP_VIN_GID_MASTER == STD_ON ) && (DOIP_USE_EID_AS_GID == STD_ON)) */

/**********************************************************************************************************************
 *  DoIP_Util_FillFurtherAction()
 *********************************************************************************************************************/
/*! \brief          Fills Central Security data into buffer.
 *  \details        -
 *  \param[out]     TgtBufferPtr   Pointer to target buffer.
 *                                 [range: POINTER != NULL_PTR]
 *  \param[in]      ConnectionIdx  Connection Index.
 *                                 [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillFurtherAction(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Util_PutUInt16()
 *********************************************************************************************************************/
/*! \brief          Puts an uint16 into a byte buffer.
 *  \details        -
 *  \param[out]     Buffer  Target buffer.
 *                          [range: POINTER != NULL_PTR && (Offset + 2u) <= sizeof(POINTER)]
 *  \param[in]      Offset  Buffer write offset.
 *                          [range: undefined]
 *  \param[in]      Value   Value to put into buffer.
 *                          [range: undefined]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_Util_PutUInt16(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) Buffer,
  uint32 Offset,
  uint16 Value);

/**********************************************************************************************************************
 *  DoIP_Util_PutUInt32()
 *********************************************************************************************************************/
/*! \brief          Puts an uint32 into a byte buffer.
 *  \details        -
 *  \param[out]     Buffer  Target buffer.
 *                          [range: POINTER != NULL_PTR && (Offset + 4u) <= sizeof(POINTER)]
 *  \param[in]      Offset  Buffer write offset.
 *                          [range: undefined]
 *  \param[in]      Value   Value to put into buffer.
 *                          [range: undefined]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_Util_PutUInt32(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) Buffer,
  uint32 Offset,
  uint32 Value);

/**********************************************************************************************************************
 *  DoIP_Util_CheckBufferSizeAndModify()
 *********************************************************************************************************************/
/*! \brief          Saves the BufferSize in BufferSizePtr and modify it according to its data type size.
 *  \details        -
 *  \param[in]      BufferSize      Expected buffer size.
 *                                  [range: undefined]
 *  \param[in,out]  BufferSizePtr   Pointer to remaining size of message.
 *                                  [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_Util_CheckBufferSizeAndModify(
  uint32 BufferSize,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr);

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
# if ( DOIP_IPV6_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_CheckIpV6Addr()
 *********************************************************************************************************************/
/*! \brief          Validates the IPv6 address against the invalid IPv6 address types.
 *  \details        -
 *  \param[in]      IpAddr    IPv6 address.
 *                            [range: IPv6 address]
 *  \return         E_OK      IPv6 address is valid and as expected.
 *  \return         E_NOT_OK  Unexpected IPv6 address.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS & DOIP_IPV6_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_CheckIpV6Addr(
  SoAd_SockAddrInetXType IpAddr);
# endif /* DOIP_IPV6_ENABLED == STD_ON */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
# if ( DOIP_IPV4_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_CheckIpV4Addr()
 *********************************************************************************************************************/
/*! \brief          Validates the IPv4 address against the invalid IPv4 address types.
 *  \details        -
 *  \param[in]      IpAddr    IPv4 address.
 *                            [range: IPv4 address]
 *  \return         E_OK      IPv4 address is valid and as expected.
 *  \return         E_NOT_OK  Unexpected IPv4 address.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS & DOIP_IPV4_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_CheckIpV4Addr(
  SoAd_SockAddrInetXType IpAddr);
# endif /* DOIP_IPV4_ENABLED == STD_ON */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_CompareIpAddr()
 *********************************************************************************************************************/
/*! \brief          Compares two IP addresses.
 *  \details        -
 *  \param[in]      Domain      IP domain (IPv4/v6).
 *                              [range: SOAD_AF_INET, SOAD_AF_INET6]
 *  \param[in]      IpAddrAPtr  Pointer to first IP address.
 *                              [range: IPv4/v6 address]
 *  \param[in]      IpAddrBPtr  Pointer to second IP address.
 *                              [range: IPv4/v6 address]
 *  \return         TRUE        IP addresses are equal.
 *  \return         FALSE       IP addresses are not equal.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_CompareIpAddr(
  SoAd_DomainType Domain,
  P2CONST(uint32, AUTOMATIC, DOIP_APPL_VAR) IpAddrAPtr,
  P2CONST(uint32, AUTOMATIC, DOIP_APPL_VAR) IpAddrBPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Util_GetEcuAddr()
 *********************************************************************************************************************/
/*! \brief          Gets the logical ECU address either from configuration or the stored RAM value.
 *  \details        -
 *  \param[in]      ChannelIdx  Channel index.
 *                              [range: ChannelIdx < DoIP_GetSizeOfChannel()]
 *  \return         The ECU address of the Channel.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(uint16, DOIP_CODE) DoIP_Util_GetEcuAddr(
  DoIP_ChannelIterType ChannelIdx);

/**********************************************************************************************************************
 *  DoIP_Util_GetEntityAddr()
 *********************************************************************************************************************/
/*! \brief          Gets the entity address either from configuration or the stored RAM value.
 *  \details        -
 *  \return         The entity address.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(uint16, DOIP_CODE) DoIP_Util_GetEntityAddr(void);

/**********************************************************************************************************************
 *  DoIP_Util_IsTcpConnection()
 *********************************************************************************************************************/
/*! \brief          Checks if a connection is a TCP connection.
 *  \details        -
 *  \param[in]      ConnectionIdx  Connection Index.
 *                                 [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \return         TRUE           Connection is a TCP connection.
 *  \return         FALSE          Connection is no TCP connection.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_IsTcpConnection(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Util_IsUdpConnection()
 *********************************************************************************************************************/
/*! \brief          Checks if a connection is a UDP connection.
 *  \details        -
 *  \param[in]      ConnectionIdx  Connection Index.
 *                                 [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \return         TRUE           Connection is a UDP connection.
 *  \return         FALSE          Connection is no UDP connection.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_IsUdpConnection(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Util_IsUdpVehAnConnection()
 *********************************************************************************************************************/
/*! \brief          Checks if a connection is a UDP Vehicle Announcement connection.
 *  \details        -
 *  \param[in]      ConnectionIdx  Connection Index.
 *                                 [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \return         TRUE           Connection is a UDP Vehicle Announcement connection.
 *  \return         FALSE          Connection is no UDP Vehicle Announcement connection.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_IsUdpVehAnConnection(
  DoIP_SizeOfConnectionType ConnectionIdx);

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_GetLocalAddrIdxOfDhcpAssignmentByLocalIpAddrId()
 *********************************************************************************************************************/
/*! \brief          Checks that the local address is used by DoIP and that is has a DHCP assignment.
 *  \details        -
 *  \param[in]      LocalIpAddrId     TcpIp local address identifier.
 *                                    [range: undefined]
 *  \param[out]     LocalAddrIdxPtr   Pointer to the found DoIP LocalAddrIdx of the DoIP used local address or
 *                                    value of DoIP_GetSizeOfLocalAddr() if E_NOT_OK is returned.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         E_OK              If local address is used by DoIP and has DHCP assignment.
 *  \return         E_NOT_OK          If local address is not used by DoIP or address has no DHCP assignment.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_GetLocalAddrIdxOfDhcpAssignmentByLocalIpAddrId(
  SoAd_LocalAddrIdType LocalIpAddrId,
  P2VAR(DoIP_SizeOfLocalAddrType, AUTOMATIC, DOIP_APPL_VAR) LocalAddrIdxPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_PrepareVendorClassTxBuf()
 *********************************************************************************************************************/
/*! \brief          Writes DHCP vendor option data to buffer.
 *  \details        -
 *  \param[in]      OptionCode              Option code.
 *                                          [range: DOIP_DHCPV4_OPT_VENDOR_CLASS_OPT_CODE,
 *                                                  DOIP_DHCPV6_OPT_VENDOR_CLASS_OPT_CODE]
 *  \param[in,out]  OptionBufferLengthPtr   Pointer to the length of the option buffer
 *                                          (updated to length of option on return).
 *                                          [range: POINTER != NULL_PTR &&
 *                                          IPv4: *POINTER >= DOIP_DHCPV4_OPT_VENDOR_CLASS_MIN_LEN,
 *                                          IPv6: *POINTER >= DOIP_DHCPV6_OPT_VENDOR_CLASS_MIN_LEN]
 *  \param[out]     OptionBufferPtr         Pointer to option buffer to write the data.
 *                                          [range: POINTER != NULL_PTR && OptionBufferLength <= sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_PrepareVendorClassTxBuf(
  uint16 OptionCode,
  P2VAR(uint16, AUTOMATIC, DOIP_APPL_VAR) OptionBufferLengthPtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_HandleVendorOptsOption()
 *********************************************************************************************************************/
/*! \brief          Processes received DHCP option (vendor opts).
 *  \details        -
 *  \param[in]      LocalAddrIdx        Local address index.
 *                                      [range: LocalAddrIdx < DoIP_GetSizeOfLocalAddr()]
 *  \param[in]      OptionCode          Option code.
 *                                      [range: DOIP_DHCPV4_OPT_VENDOR_OPTS_OPT_CODE,
 *                                              DOIP_DHCPV6_OPT_VENDOR_OPTS_OPT_CODE]
 *  \param[in]      OptionBufferLength  Length of the option buffer.
 *                                      [range: undefined]
 *  \param[in]      OptionBufferPtr     Pointer to option buffer to read the data from.
 *                                      [range: POINTER != NULL_PTR && OptionBufferLength <= sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_HandleVendorOptsOption(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ValidateVendorOptsRxBuf()
 *********************************************************************************************************************/
/*! \brief          Validates received DHCP vendor opts for correctness regarding RFC.
 *  \details        -
 *  \param[in]      OptionCode          Option code.
 *                                      [range: DOIP_DHCPV4_OPT_VENDOR_OPTS_OPT_CODE,
 *                                              DOIP_DHCPV6_OPT_VENDOR_OPTS_OPT_CODE]
 *  \param[in]      OptionBufferLength  Length of the option buffer.
 *                                      [range: undefined]
 *  \param[in]      OptionBufferPtr     Pointer to option buffer to read the data from.
 *                                      [range: POINTER != NULL_PTR && OptionBufferLength <= sizeof(POINTER)]
 *  \return         E_OK                Data are valid. OptionBufferLength/OptionBufferPtr fulfills the requirements of
 *                                      the DHCP option regarding OptionCode.
 *  \return         E_NOT_OK            Data are not valid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_ValidateVendorOptsRxBuf(
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_HandleIpAddrListFromVendorOpts()
 *********************************************************************************************************************/
/*! \brief          Updates IP address list from DHCP vendor opts.
 *  \details        -
 *  \param[in]      LocalAddrIdx        Local address index.
 *                                      [range: LocalAddrIdx < DoIP_GetSizeOfLocalAddr()]
 *  \param[in]      OptionCode          Option code.
 *                                      [range: DOIP_DHCPV4_OPT_VENDOR_OPTS_OPT_CODE,
 *                                              DOIP_DHCPV6_OPT_VENDOR_OPTS_OPT_CODE]
 *  \param[in]      OptionBufferLength  Length of the option buffer.
 *                                      [range: IPv4: OptionBufferLength >= DOIP_DHCPV4_OPT_VENDOR_OPTS_MIN_LEN,
 *                                              IPv6: OptionBufferLength >= DOIP_DHCPV6_OPT_VENDOR_OPTS_MIN_LEN]
 *  \param[in]      OptionBufferPtr     Pointer to option buffer to read the data from.
 *                                      [range: POINTER != NULL_PTR && OptionBufferLength <= sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_HandleIpAddrListFromVendorOpts(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ReadAndFilterIpAddresses()
 *********************************************************************************************************************/
/*! \brief          Reads the DHCP supplied IP addresses from the buffer and filters them.
 *  \details        Reads, validates and filters IP addresses.
 *  \param[in]      OptionCode              Option code.
 *                                          [range: DOIP_DHCPV4_OPT_VENDOR_OPTS_OPT_CODE,
 *                                                  DOIP_DHCPV6_OPT_VENDOR_OPTS_OPT_CODE]
 *  \param[in]      OptionBufferLength      Length of the option buffer.
 *                                          [range: IPv4: OptionBufferLength >= DOIP_DHCPV4_OPT_VENDOR_OPTS_MIN_LEN,
 *                                                  IPv6: OptionBufferLength >= DOIP_DHCPV6_OPT_VENDOR_OPTS_MIN_LEN]
 *  \param[in]      OptionBufferPtr         Pointer to option buffer to read the data from.
 *                                          [range: POINTER != NULL_PTR && OptionBufferLength <= sizeof(POINTER)]
 *  \param[in,out]  VenOptIpAddrDynListPtr  Pointer to the list to copy valid DHCP supplied IP addresses to.
 *                                          [range: POINTER != NULL_PTR &&
 *                                                  DOIP_DHCP_IPADDRESS_LIST_MAX_SIZE == sizeof(POINTER)]
 *  \return         E_OK                    The VenOptIpAddrDynList contains at least one valid IP address.
 *  \return         E_NOT_OK                The VenOptIpAddrDynList contains no IP addresses.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_ReadAndFilterIpAddresses(
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr,
  P2VAR(DoIP_DhcpIpAddrDynType, AUTOMATIC, DOIP_APPL_VAR) VenOptIpAddrDynListPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ValidateIpAddress()
 *********************************************************************************************************************/
/*! \brief          Validates the additional IP addresses received from the DHCP server.
 *  \details        Compares received address against local IP address, unspecified address and loopback address.
 *  \param[in]      IpAddr        The IP address received from DHCP server.
 *                                [range: IPv4/v6 address]
 *  \return         E_OK          IP address is valid.
 *  \return         E_NOT_OK      IP address is not valid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_ValidateIpAddress(
  SoAd_SockAddrInetXType IpAddr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_CompareToLocalIpAddrList()
 *********************************************************************************************************************/
/*! \brief          Compares the received IP address from DHCP to the local address list and updates this info.
 *  \details        A flag is set in the dynamic list to indicate if the new address is same as a previously received
 *                  one.
 *  \param[in]      LocalAddrIdx            Local address index.
 *                                          [range: LocalAddrIdx < DoIP_GetSizeOfLocalAddr()]
 *  \param[in,out]  VenOptIpAddrDynListPtr  Pointer to the list that holds the valid DHCP supplied IP addresses and its
 *                                          info.
 *                                          [range: POINTER != NULL_PTR &&
 *                                                  DOIP_DHCP_IPADDRESS_LIST_MAX_SIZE == sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_CompareToLocalIpAddrList(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  P2VAR(DoIP_DhcpIpAddrDynType, AUTOMATIC, DOIP_APPL_VAR) VenOptIpAddrDynListPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_UpdateLocalIpAddrList()
 *********************************************************************************************************************/
/*! \brief          Updates the local list of DHCP IP addresses.
 *  \details        Updates the IP address as well as enables vehicle announcement to new IP addresses.
 *  \param[in]      LocalAddrIdx            Local address index.
 *                                          [range: LocalAddrIdx < DoIP_GetSizeOfLocalAddr()]
 *  \param[in]      VenOptIpAddrDynListPtr  Pointer to the list that holds the valid DHCP supplied IP addresses and its
 *                                          info.
 *                                          [range: POINTER != NULL_PTR &&
 *                                                  DOIP_DHCP_IPADDRESS_LIST_MAX_SIZE == sizeof(POINTER)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_UpdateLocalIpAddrList(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  P2CONST(DoIP_DhcpIpAddrDynType, AUTOMATIC, DOIP_APPL_VAR) VenOptIpAddrDynListPtr);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ProcessIpAddrListToTriggerVehAnnouncements()
 *********************************************************************************************************************/
/*! \brief          This function triggers vehicle announcements for all local addresses to newly received DHCP Vendor
 *                  Option IP addresses.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_ProcessIpAddrListToTriggerVehAnnouncements(
  DoIP_InterfaceIterType InterfaceIdx);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_HandleVehAnnounceTriggOfIpAddrList()
 *********************************************************************************************************************/
/*! \brief          Handle triggering of vehicle announcements to newly received DHCP Vendor Option IP addresses.
 *  \details        -
 *  \pre            -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*]
 *  \param[in]      IpAddrListIdx   Index of the DoIP_IpAddrList for the Local address.
 *                                  [range: IpAddrListIdx < DoIP_GetSizeOfIpAddrList()]
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_HandleVehAnnounceTriggOfIpAddrList(
  DoIP_SizeOfConnectionType ConnectionIdx,
  DoIP_IpAddrListIterType IpAddrListIdx);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_HandleSoConModeChg()
 *********************************************************************************************************************/
/*! \brief          Handles socket state change callback to online/offline.
 *  \details        Notification about a SoAd socket connection state change, e.g. socket connection gets online.
 *  \param[in]      SoConId         Socket connection index specifying the socket connection with the mode change.
 *                                  [range: undefined]
 *  \param[in]      Mode            New mode.
 *                                  [range: SOAD_SOCON_ONLINE, SOAD_SOCON_RECONNECT, SOAD_SOCON_OFFLINE]
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[out]     ErrorIdPtr      Pointer to DET error if one occurred.
 *                                  [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different SoConIds. Non reentrant for the same SoConId.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleSoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode,
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_Con_HandleOnlineSoConModeChg()
 *********************************************************************************************************************/
/*! \brief          Handles socket state change callback to online.
 *  \details        Notification about a SoAd socket connection state change, e.g. socket connection gets online.
 *  \param[in]      SoConId         Socket connection index specifying the socket connection with the mode change.
 *                                  [range: undefined]
 *  \param[in]      Mode            New mode.
 *                                  [range: SOAD_SOCON_ONLINE]
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[out]     ErrorIdPtr      Pointer to DET error if one occurred.
 *                                  [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different SoConIds/ConnectionIdxs. Non reentrant for the same
 *                          SoConId/ConnectionIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOnlineSoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode,
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_Con_IsTxVehAnRequired()
 *********************************************************************************************************************/
/*! \brief          Checks if a transmission of vehicle announcement is required after the socket connection mode
 *                  changed to SOAD_SOCON_ONLINE.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \return         TRUE            Transmission of vehicle announcement is required.
 *  \return         FALSE           Transmission of vehicle announcement is not required.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(boolean, DOIP_CODE) DoIP_Con_IsTxVehAnRequired(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineSoConModeChg()
 *********************************************************************************************************************/
/*! \brief          Handles socket state change callback to offline.
 *  \details        Notification about a SoAd socket connection state change, e.g. socket connection gets online.
 *  \param[in]      Mode            New mode.
 *                                  [range: SOAD_SOCON_RECONNECT, SOAD_SOCON_OFFLINE]
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineSoConModeChg(
  SoAd_SoConModeType Mode,
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineTcpSoConModeChg()
 *********************************************************************************************************************/
/*! \brief          Handles TCP socket state change callback to offline.
 *  \details        Notification about a SoAd socket connection state change, e.g. socket connection gets online.
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineTcpSoConModeChg(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineTcpAliveCheckSoConModeChg()
 *********************************************************************************************************************/
/*! \brief          Handles alive checks on TCP socket state change callback to offline.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineTcpAliveCheckSoConModeChg(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineUdpSoConModeChg()
 *********************************************************************************************************************/
/*! \brief          Handles UDP socket state change callback to offline.
 *  \details        Notification about a SoAd socket connection state change, e.g. socket connection gets online.
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineUdpSoConModeChg(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Con_SoConInitAll()
 *********************************************************************************************************************/
/*! \brief          Initiates connections by extracting socket connection identifier from SoAd if not done before.
 *  \details        -
 *  \param[out]     ErrorIdPtr  Pointer to DET error if one occurred.
 *                              [range: POINTER != NULL_PTR]
 *  \return         E_OK        All socket connections are initialized or already initialized.
 *  \return         E_NOT_OK    Not all socket connections are initzialized.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConInitAll(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_Con_SoConOpen()
 *********************************************************************************************************************/
/*! \brief          Opens a socket connection initially after IP address assignment.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConOpen(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Con_SoConCloseAll()
 *********************************************************************************************************************/
/*! \brief          Closes all socket connections.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \return         E_OK           All socket connections are already closed.
 *  \return         E_NOT_OK       Not all socket connections are closed. Closing is requested but not yet performed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConCloseAll(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_Con_SoConClose()
 *********************************************************************************************************************/
/*! \brief          Closes single socket connection.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[in]      Abort           Flag to close socket connection immediately.
 *                                  [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConClose(
  DoIP_SizeOfConnectionType ConnectionIdx,
  boolean Abort);

/**********************************************************************************************************************
 *  DoIP_Con_HandleLocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*! \brief          Handles IP address assignment change callback.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[in]      State           State of IP address assignment.
 *                                  [range: SOAD_IPADDR_STATE_*]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleLocalIpAddrAssignmentChg(
  DoIP_SizeOfConnectionType ConnectionIdx,
  SoAd_IpAddrStateType State);

/**********************************************************************************************************************
 *  DoIP_Con_RelAllIpAddrAssign()
 *********************************************************************************************************************/
/*! \brief          Releases all IP address assignments.
 *  \details        Iterate over all UDP connections and release IP address assignment on one UDP connection of each
 *                  local address and check if all local addresses are unassigned.
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelAllIpAddrAssign(
  DoIP_InterfaceIterType InterfaceIdx);

#if ( DOIP_CONTROL_IP_ASSIGNMENT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_RelIpAddrAssign()
 *********************************************************************************************************************/
/*! \brief          Releases single IP address assignment.
 *  \details        -
 *  \param[in]      LocalAddrIdx  Local address index.
 *                                [range: LocalAddrIdx < DoIP_GetSizeOfLocalAddr()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_CONTROL_IP_ASSIGNMENT
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelIpAddrAssign(
  DoIP_SizeOfLocalAddrType LocalAddrIdx);
#endif /* DOIP_CONTROL_IP_ASSIGNMENT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_GetConnectionIdxBySoConId()
 *********************************************************************************************************************/
/*! \brief          Gets the connection index of a socket connection.
 *  \details        -
 *  \param[in]      SoConId         Socket connection identifier.
 *                                  [range: undefined]
 *  \param[out]     ConnectionIdx   Connection Index.
 *                                  [range: POINTER != NULL_PTR]
 *  \return         E_OK            Corresponding Connection found.
 *  \return         E_NOT_OK        Corresponding Connection not found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_GetConnectionIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(DoIP_SizeOfConnectionType, AUTOMATIC, DOIP_APPL_VAR) ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_Con_HandleSoConStates()
 *********************************************************************************************************************/
/*! \brief          Handles socket connection states (i.e. reopens closed socket connections).
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different interfaces.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleSoConStates(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_Con_CloseConnection()
 *********************************************************************************************************************/
/*! \brief        Calls the Socket Adaptor to close a socket connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[in]    Abort                   Flag to close socket connection immediately.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_CloseConnection(
  DoIP_SizeOfConnectionType ConnectionIdx,
  boolean Abort);

#if ( (DOIP_CONTROL_IP_ASSIGNMENT == STD_ON) || (DOIP_IPADDR_ASSIGN_DHCP == STD_ON) )
/**********************************************************************************************************************
 *  DoIP_Con_HandleIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief          Handles IP address assignment requests and sets the DHCP hostname option.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_CONTROL_IP_ASSIGNMENT & DOIP_IPADDR_ASSIGN_DHCP
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleIpAddrAssignment(
  DoIP_InterfaceIterType InterfaceIdx);
#endif /* (DOIP_CONTROL_IP_ASSIGNMENT == STD_ON) || (DOIP_IPADDR_ASSIGN_DHCP == STD_ON) */

#if ( DOIP_CONTROL_IP_ASSIGNMENT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssign()
 *********************************************************************************************************************/
/*! \brief          Requests single IP address assignment and sets DHCP hostname option.
 *  \details        -
 *  \param[in]      LocalAddrIdx          Local address index.
 *                                        [range: LocalAddrIdx < DoIP_GetSizeOfLocalAddr()]
 *  \param[in]      IpAddrAssignmentIdx   IP address assignment index.
 *                                        [range: IpAddrAssignmentIdx < DoIP_GetSizeOfIpAddrAssignment()]
 *  \return         E_OK                  Address assignment is successfully requested.
 *  \return         E_NOT_OK              Address assignment request failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_CONTROL_IP_ASSIGNMENT
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssign(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  DoIP_SizeOfIpAddrAssignmentType IpAddrAssignmentIdx);
#endif /* DOIP_CONTROL_IP_ASSIGNMENT == STD_ON */

#if ( DOIP_IPADDR_ASSIGN_DHCP == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_SetDhcpHostname()
 *********************************************************************************************************************/
/*! \brief          Sets DHCP hostname option.
 *  \details        -
 *  \param[in]      LocalAddrIdx    Local address index.
 *                                  [range: LocalAddrIdx < DoIP_GetSizeOfLocalAddr()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_IPADDR_ASSIGN_DHCP
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SetDhcpHostname(
  DoIP_SizeOfLocalAddrType LocalAddrIdx);
#endif /* DOIP_IPADDR_ASSIGN_DHCP == STD_ON */

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_HandleGidSync()
 *********************************************************************************************************************/
/*! \brief          Handles GID synchronization.
 *  \details        Wait until GID is provided by application.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_GID_SYNC_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleGidSync(void);
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_HandleActLineStates()
 *********************************************************************************************************************/
/*! \brief          Handles activation line state in context of main function.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleActLineStates(void);

/**********************************************************************************************************************
 *  DoIP_Con_HandleActLineStateActive()
 *********************************************************************************************************************/
/*! \brief          Handles activation line state active in context of main function.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleActLineStateActive(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_Con_HandleActLineStateInactive()
 *********************************************************************************************************************/
/*! \brief          Handles activation line state inactive in context of main function.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleActLineStateInactive(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleRxIndication()
 *********************************************************************************************************************/
/*! \brief          Handles receive indication callback for IF.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR && POINTER->SduDataPtr != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleRxIndication(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  DoIP_RxUdp_CheckGenHdr()
 *********************************************************************************************************************/
/*! \brief          Checks generic header of received data.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR && POINTER->SduDataPtr != NULL_PTR]
 *  \param[out]     HdrLenField     Header length field value.
 *                                  [range: POINTER != NULL_PTR]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset < sizeof(POINTER->SduDataPtr)]
 *  \return         E_OK            Header check passed.
 *  \return         E_NOT_OK        Header check failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxUdp_CheckGenHdr(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_VAR) HdrLenField,
  uint32 DataOffset);

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleRxIndicationMessageTypes()
 *********************************************************************************************************************/
/*! \brief          Handles received message by there type.
 *  \details        If disabled, a Entity Status Request is handled as an unknown payload type
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR &&
 *                                          POINTER->SduDataPtr != NULL_PTR &&
 *                                          POINTER->SduLength >= DOIP_GEN_HDR_LEN_BYTE &&
 *                                          POINTER->SduLength == sizeof(POINTER->SduDataPtr)]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset <= sizeof(POINTER->SduDataPtr) - DOIP_GEN_HDR_LEN_BYTE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPSupportISO13400-2
 *  \trace          DSGN-DoIPEntityStatus
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleRxIndicationMessageTypes(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset);

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleVehicleIdentReq()
 *********************************************************************************************************************/
/*! \brief          Handles received Vehicle Ident Requests.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR &&
 *                                          POINTER->SduDataPtr != NULL_PTR &&
 *                                          POINTER->SduLength >= DOIP_GEN_HDR_LEN_BYTE &&
 *                                          POINTER->SduLength == sizeof(POINTER->SduDataPtr)]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset <= sizeof(POINTER->SduDataPtr) - DOIP_GEN_HDR_LEN_BYTE]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPVehicleIdentification
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleVehicleIdentReq(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleVehicleIdentReqVin()
 *********************************************************************************************************************/
/*! \brief          Handles received Vehicle Ident Requests with VIN.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR &&
 *                                          POINTER->SduDataPtr != NULL_PTR &&
 *                                          POINTER->SduLength >= DOIP_GEN_HDR_LEN_BYTE &&
 *                                          POINTER->SduLength == sizeof(POINTER->SduDataPtr)]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset <= sizeof(POINTER->SduDataPtr) - DOIP_GEN_HDR_LEN_BYTE]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPVehicleIdentification
 *********************************************************************************************************************/
/* PRQA S 0779 5 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleVehicleIdentReqVin(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket);

#if ( DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleVehicleIdentReqEid()
 *********************************************************************************************************************/
/*! \brief          Handles received Vehicle Ident Requests with EID.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR &&
 *                                          POINTER->SduDataPtr != NULL_PTR &&
 *                                          POINTER->SduLength >= DOIP_GEN_HDR_LEN_BYTE &&
 *                                          POINTER->SduLength == sizeof(POINTER->SduDataPtr)]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset <= sizeof(POINTER->SduDataPtr) - DOIP_GEN_HDR_LEN_BYTE]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *  \config         DOIP_VEHICLE_IDENT_EID_SUPPORT
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPVehicleIdentification
 *********************************************************************************************************************/
/* PRQA S 0779 5 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleVehicleIdentReqEid(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket);
#endif /* DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleDiagPowerModeInfoReq()
 *********************************************************************************************************************/
/*! \brief          Handles received Diagnostic Power Mode Requests.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR &&
 *                                          POINTER->SduDataPtr != NULL_PTR &&
 *                                          POINTER->SduLength >= DOIP_GEN_HDR_LEN_BYTE &&
 *                                          POINTER->SduLength == sizeof(POINTER->SduDataPtr)]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset <= sizeof(POINTER->SduDataPtr) - DOIP_GEN_HDR_LEN_BYTE]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPDiagnosticPowerMode, DSGN-DoIPSupportISO13400-2, DSGN-DoIPNodeInformation
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleDiagPowerModeInfoReq(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket);

#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleEntityStateReq()
 *********************************************************************************************************************/
/*! \brief          Handles received Entity State Requests.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR &&
 *                                          POINTER->SduDataPtr != NULL_PTR &&
 *                                          POINTER->SduLength >= DOIP_GEN_HDR_LEN_BYTE &&
 *                                          POINTER->SduLength == sizeof(POINTER->SduDataPtr)]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset <= sizeof(POINTER->SduDataPtr) - DOIP_GEN_HDR_LEN_BYTE]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *  \config         DOIP_ENTITY_STATE_SUPPORT
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPNodeInformation
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleEntityStateReq(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket);
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleUnknownHdrType()
 *********************************************************************************************************************/
/*! \brief          Handles received messages with unknown header type.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Pointer to destination buffer and length to be copied.
 *                                  [range: POINTER != NULL_PTR &&
 *                                          POINTER->SduDataPtr != NULL_PTR &&
 *                                          POINTER->SduLength >= DOIP_GEN_HDR_LEN_BYTE &&
 *                                          POINTER->SduLength == sizeof(POINTER->SduDataPtr)]
 *  \param[in]      DataOffset      Data offset to read from PduInfoPtr.
 *                                  [range: DataOffset <= sizeof(POINTER->SduDataPtr) - DOIP_GEN_HDR_LEN_BYTE]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPOemSpecificPayloadType
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleUnknownHdrType(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitVehAn()
 *********************************************************************************************************************/
/*! \brief          Requests the transmission of UDP vehicle announcement messages.
 *  \details        If the transmission request is successful, the pending vehicle announcement flag is reset.
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*]
 *  \param[out]     ErrorIdPtr      Pointer to DET error if one occurred.
 *                                  [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdp_TransmitVehAn(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_TxUdp_Transmit()
 *********************************************************************************************************************/
/*! \brief          Transmits UDP messages immediately or delayed.
 *  \details        Depending on message type the message is transmitted immediately or delayed. If transmit failed
 *                  message gets delayed by adding the transmit to the retry list.
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      HdrType         Message header type.
 *                                  [range: DOIP_HDR_TYPE_* (but only DoIP TX UDP header types)]
 *  \param[in]      Param           Parameter depending on header type (e.g. negative acknowledge code).
 *                                  [range: undefined]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \param[in]      RemoteAddress   Indicates the supposed remote address of this connection.
 *                                  [range: POINTER may be NULL_PTR]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_Transmit(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket,
  P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, DOIP_APPL_VAR) RemoteAddress);

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitImmediately()
 *********************************************************************************************************************/
/*! \brief          Transmits UDP messages immediately.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      HdrType         Message header type.
 *                                  [range: DOIP_HDR_TYPE_* (but only DoIP TX UDP header types)]
 *  \param[in]      Param1          Parameter depending on header type (e.g. OEM Tx buffer index).
 *                                  [range: undefined]
 *  \param[in]      Param2          Parameter depending on header type (e.g. negative acknowledge code).
 *                                  [range: undefined]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_TransmitImmediately(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint32 Param1,
  uint16 Param2,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitDelayed()
 *********************************************************************************************************************/
/*! \brief          Transmits UDP messages delayed.
 *  \details        Stores transmission request for DoIP messages (UDP) in a retry list.
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      HdrType         Message header type.
 *                                  [range: DOIP_HDR_TYPE_* (but only DoIP TX UDP header types)]
 *  \param[in]      Param1          Parameter depending on header type (e.g. OEM Tx buffer index).
 *                                  [range: undefined]
 *  \param[in]      Param2          Parameter depending on header type (e.g. negative acknowledge code).
 *                                  [range: undefined]
 *  \param[in]      CloseSocket     Indicates if a socket shall be closed after transmission.
 *                                  [range: TRUE, FALSE]
 *  \param[in]      RemoteAddress   Indicates the supposed remote address of this connection.
 *                                  [range: POINTER may be NULL_PTR]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_TransmitDelayed(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint32 Param1,
  uint16 Param2,
  boolean CloseSocket,
  P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, DOIP_APPL_VAR) RemoteAddress);

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitDelayedSetVehAnnSpecParams()
 *********************************************************************************************************************/
/*! \brief          Sets Vehicle Announcement and Vehicle Identification Response specific parameters in the UDP retry
 *                  list.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      TxUdpListIdx    List index of handled entry.
 *                                  [range: TxUdpListIdx < DoIP_GetSizeOfTxUdpMgtList()]
 *  \param[in]      RemoteAddress   Indicates the supposed remote address of this connection.
 *                                  [range: POINTER may be NULL_PTR]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdp_TransmitDelayedSetVehAnnSpecParams(
  DoIP_SizeOfConnectionType ConnectionIdx,
  DoIP_TxUdpMgtListIterType TxUdpListIdx,
  P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, DOIP_APPL_VAR) RemoteAddress);

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmit()
 *********************************************************************************************************************/
/*! \brief          Transmits single UDP messages immediately.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      HdrType         Message header type.
 *                                  [range: DOIP_HDR_TYPE_* (but only DoIP TX UDP header types)]
 *  \param[in]      Param1          Parameter depending on header type (e.g. OEM Tx buffer index).
 *                                  [range: undefined]
 *  \param[in]      Param2          Parameter depending on header type (e.g. negative acknowledge code).
 *                                  [range: undefined]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmit(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint32 Param1,
  uint16 Param2);

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitGenNack()
 *********************************************************************************************************************/
/*! \brief          Transmits Generic Negative Acknowledge UDP messages immediately.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      HdrType         Message header type.
 *                                  [range: DOIP_HDR_TYPE_* (but only DoIP TX UDP header types)]
 *  \param[in]      NackCode        Negative acknowledge code.
 *                                  [range: undefined]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPSupportISO13400-2
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitGenNack(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint8 NackCode);

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitVehicleAnnounceAndIdentRes()
 *********************************************************************************************************************/
/*! \brief          Transmits Vehicle Announcements and Vehicle Identification Responses immediately.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPVehicleIdentification
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitVehicleAnnounceAndIdentRes(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitDiagPowerModeInfoRes()
 *********************************************************************************************************************/
/*! \brief          Transmits Diagnostic Power Mode Info Response UDP messages immediately.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      DiagPowerMode   Diagnostic Power Mode.
 *                                  [range: undefined]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPNodeInformation
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitDiagPowerModeInfoRes(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint8 DiagPowerMode);

#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitEntityStateRes()
 *********************************************************************************************************************/
/*! \brief          Transmits Entity State Response UDP messages immediately.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_ENTITY_STATE_SUPPORT
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPNodeInformation
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitEntityStateRes(
  DoIP_SizeOfConnectionType ConnectionIdx);
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitOemSpecific()
 *********************************************************************************************************************/
/*! \brief          Transmits OEM Specific UDP messages immediately.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      OemTxBufCfgIdx  OEM Tx Buffer Configuration index.
 *                                  [range: OemTxBufCfgIdx < DoIP_GetSizeOfOemPayloadTxBufConfig()]
 *  \param[in]      HdrTypeOem      OEM specific message header type.
 *                                  [range: undefined]
 *  \return         E_OK            Transmission requested.
 *  \return         E_NOT_OK        Transmission request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *  \trace          DSGN-DoIPSupportISO13400-2, DSGN-DoIPOemSpecificPayloadType
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitOemSpecific(
  DoIP_SizeOfConnectionType ConnectionIdx,
  DoIP_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx,
  uint16 HdrTypeOem);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TxUdp_CheckAndSetTxActive()
 *********************************************************************************************************************/
/*! \brief          Checks if there is any ongoing transmission for the connection. If there is no active transmission
 *                  a flag is set to indicate that a transmission is active on the connection.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \return         E_OK            Transmission active flag could be set.
 *  \return         E_NOT_OK        Transmission is already active in another context.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_CheckAndSetTxActive(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_TxUdp_ResetTxActive()
 *********************************************************************************************************************/
/*! \brief          Resets the transmission active flag.
 *  \details        -
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_TxUdp_ResetTxActive(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_TxUdpList_CloseSocket()
 *********************************************************************************************************************/
/*! \brief          Handles socket connection closing considering remaining Tx UDP list entries if it is requested to
 *                  be closed already. If it is not requested to be closed already, parameter "CloseSocket" can be used
 *                  to request the closing of the socket.
 *  \details        -
 *  \param[in]      ConnectionIdx     Connection Index.
 *                                    [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[in]      CloseSocket       Indicates if a socket shall be closed after transmission.
 *                                    [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_CloseSocket(
  DoIP_SizeOfConnectionType ConnectionIdx,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_TxUdpList_CheckForPendingEntry()
 *********************************************************************************************************************/
/*! \brief          Checks for remaining Tx UDP list entries of the socket connection.
 *  \details        -
 *  \param[in]      ConnectionIdx     Connection Index.
 *                                    [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \return         TRUE              There is a pending entry for the socket connection in the Tx UDP list.
 *  \return         FALSE             There is no pending entry for the socket connection in the Tx UDP list.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(boolean, DOIP_CODE) DoIP_TxUdpList_CheckForPendingEntry(
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_TxUdpList_HandleEntries()
 *********************************************************************************************************************/
/*! \brief          Handles pending Tx UDP list entries.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_HandleEntries(void);

/**********************************************************************************************************************
 *  DoIP_TxUdpList_HandleEntriesVehicleAnnounceAndIdentRes()
 *********************************************************************************************************************/
/*! \brief          Handles pending Tx UDP list entries for Vehicle Announcements and Vehicle Identification Responses.
 *  \details        -
 *  \param[in]      TxUdpListIdx    List index of handled entry.
 *                                  [range: TxUdpListIdx < DoIP_GetSizeOfTxUdpMgtList()]
 *  \param[in]      ConnectionIdx   Connection Index.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_HandleEntriesVehicleAnnounceAndIdentRes(
  DoIP_SizeOfTxUdpMgtListType TxUdpListIdx,
  DoIP_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIP_TxUdpList_HandleEntriesOthers()
 *********************************************************************************************************************/
/*! \brief          Handles pending Tx UDP list entries for Generic Negative Acknowledge, Entity State Status and
 *                  Diagnostic Power Mode Responses.
 *  \details        -
 *  \param[in]      TxUdpListIdx      List index of handled entry.
 *                                    [range: TxUdpListIdx < DoIP_GetSizeOfTxUdpMgtList()]
 *  \param[in]      ConnectionIdx     Connection Index.
 *                                    [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                        (DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_* ||
 *                                         DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDPVEHAN_*)]
 *  \param[out]     ReleaseListEntry  Indicates if entry is handled and can be released from list.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_HandleEntriesOthers(
  DoIP_SizeOfTxUdpMgtListType TxUdpListIdx,
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) ReleaseListEntry);

/**********************************************************************************************************************
 *  DoIP_TxTcp_Transmit()
 *********************************************************************************************************************/
/*! \brief          Requests a transmission of a TCP message.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      HdrType           Message header type.
 *                                    [range: DOIP_HDR_TYPE_* (but only DoIP TX TCP header types)]
 *  \param[in]      Param1            First parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      Param2            Second parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      Param3            Third parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      DataPtr           Pointer to data depending on header type.
 *                                    [range: POINTER (may be NULL_PTR)]
 *  \param[in]      CloseSocket       Indicates if a socket shall be closed after transmission.
 *                                    [range: TRUE, FALSE]
 *  \return         E_OK              Request successful.
 *  \return         E_NOT_OK          Request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxTcp_Transmit(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrType,
  uint32 Param1,
  uint32 Param2,
  uint32 Param3,
  SduDataPtrType DataPtr,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_TxTcp_PrepareTransmit()
 *********************************************************************************************************************/
/*! \brief          Stores a transmit request for a message which is not ready yet.
 *  \details        In case of a received diagnostic message a corresponding negative acknowledge may be sent. If the
 *                  message is not yet received completely the transmission is prepared with this function but
 *                  performed when the message is received completely to append the diagnostic data to the negative
 *                  acknowledge message.
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      HdrType           Message header type.
 *                                    [range: DOIP_HDR_TYPE_* (but only DoIP TX TCP header types)]
 *  \param[in]      Param1            First parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      Param2            Second parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      Param3            Third parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      CloseSocket       Indicates if a socket shall be closed after transmission.
 *                                    [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcp_PrepareTransmit(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrType,
  uint16 Param1,
  uint32 Param2,
  uint8  Param3,
  boolean CloseSocket);

/**********************************************************************************************************************
 *  DoIP_TxTcp_FinishTransmit()
 *********************************************************************************************************************/
/*! \brief          Finish a pending transmission of a message which was requested by DoIP_TxTcp_PrepareTransmit().
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcp_FinishTransmit(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_TxTcp_ConfirmTransmit()
 *********************************************************************************************************************/
/*! \brief          Call a TxConfirmation depending on the API type.
 *  \details        -
 *  \param[in]      ChannelIdx        Channel Index.
 *                                    [range: ChannelIdx < DoIP_GetSizeOfChannel()]
 *  \param[in]      TxConfPduId       TxConfirmation Pdu Id.
 *                                    [range: TxConfPduId < size of DoIP_PduRTxConfPduIdOfChannelType]
 *  \param[in]      Result            Result of transmission.
 *                                    [range: E_OK, E_NOT_OK]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcp_ConfirmTransmit(
  DoIP_SizeOfChannelType ChannelIdx,
  DoIP_PduRTxConfPduIdOfChannelType TxConfPduId,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_Add()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP message to Tx TCP queue
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      HdrType           Message header type.
 *                                    [range: DOIP_HDR_TYPE_* (but only DoIP TX TCP header types)]
 *  \param[in]      Param1            First parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      Param2            Second parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      Param3            Third parameter depending on header type.
 *                                    [range: undefined]
 *  \param[in]      DataPtr           Pointer to data depending on header type.
 *                                    [range: POINTER (may be NULL_PTR)]
 *  \return         E_OK              The DoIP TCP message could be added.
 *  \return         E_NOT_OK          The DoIP TCP message could not be added.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_Add(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrType,
  uint32 Param1,
  uint32 Param2,
  uint32 Param3,
  SduDataPtrType DataPtr);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddGenNack()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Generic Negative Acknowledge message to Tx TCP queue.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first free element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      NackCode          Negative acknowledge code.
 *                                    [range: undefined]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddGenNack(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint8 NackCode);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddRoutingActivationRes()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Routing Activation Response message to Tx TCP queue.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first free element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterAddr        Tester logical Address.
 *                                    [range: undefined]
 *  \param[in]      AckCode           Acknowledge code.
 *                                    [range: undefined]
 *  \param[in]      TesterIdx         Tester Index.
 *                                    [range: undefined]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddRoutingActivationRes(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr,
  uint8 AckCode,
  DoIP_SizeOfTesterType TesterIdx);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddAliveCheckReq()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Alive Check Request message to Tx TCP queue.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first free element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddAliveCheckReq(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddDiagReq()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Diagnostic Reuqest message to Tx TCP queue.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first free element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      ChannelIdx        Channel Index.
 *                                    [range: undefined]
 *  \param[in]      Length            Pdu length.
 *                                    [range: undefined]
 *  \param[in]      DataPtr           Pointer to data.
 *                                    [range: POINTER (may be NULL_PTR)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddDiagReq(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfChannelType ChannelIdx,
  PduLengthType Length,
  SduDataPtrType DataPtr);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddDiagAck()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Diagnostic Positive Acknowledge message to Tx TCP queue.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first free element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      EcuAddr           ECU logical Address.
 *                                    [range: undefined]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddDiagAck(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 EcuAddr);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddDiagNack()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP Diagnostic Negative Acknowledge message to Tx TCP queue.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first free element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      EcuAddr           ECU logical Address.
 *                                    [range: undefined]
 *  \param[in]      TesterAddr        Tester logical Address.
 *                                    [range: undefined]
 *  \param[in]      NackCode          Negative acknowledge code.
 *                                    [range: undefined]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddDiagNack(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 EcuAddr,
  uint16 TesterAddr,
  uint8 NackCode);

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddOem()
 *********************************************************************************************************************/
/*! \brief          Adds a DoIP TCP OEM message to Tx TCP queue.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first free element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      HdrTypeOem        OEM message header type.
 *                                    [range: undefined]
 *  \param[in]      OemTxBufCfgIdx    OEM Tx Buffer Configuration index.
 *                                    [range: OemTxBufCfgIdx < DoIP_GetSizeOfOemPayloadTxBufConfig()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddOem(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrTypeOem,
  DoIP_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_Copy()
 *********************************************************************************************************************/
/*! \brief          Copies buffer stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_Copy(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyHeader()
 *********************************************************************************************************************/
/*! \brief          Copies header stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[out]     HdrCopied         Indicates if header is copied completely.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyHeader(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) HdrCopied);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyData()
 *********************************************************************************************************************/
/*! \brief          Copies data stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *                                    [range: TRUE, FALSE]
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyData(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagReq()
 *********************************************************************************************************************/
/*! \brief          Copies diagnostic request stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *                                    [range: TRUE, FALSE]
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagReq(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagReqTP()
 *********************************************************************************************************************/
/*! \brief          Copies TP diagnostic request stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *                                    [range: TRUE, FALSE]
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 6 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagReqTP(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);

#if ( DOIP_IF_API_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagReqIF()
 *********************************************************************************************************************/
/*! \brief          Copies IF diagnostic request stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *                                    [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 6 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagReqIF(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);
#endif /* DOIP_IF_API_ENABLED == STD_ON */

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagAck()
 *********************************************************************************************************************/
/*! \brief          Copies buffer stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *                                    [range: TRUE, FALSE]
 *  \return         BUFREQ_OK         Data has been copied sucessful.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_NACK_APPEND_REQ
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagAck(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyOemSpecific()
 *********************************************************************************************************************/
/*! \brief          Copies buffer stored in first Tx TCP queue element to a target buffer.
 *  \details        -
 *  \param[in]      QueueIdx          Index of the first element in the Tx TCP queue.
 *                                    [range: QueueIdx < DoIP_GetSizeOfTxTcpMgt() &&
                                              QueueIdx < (DoIP_GetSizeOfTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
                                                          DoIP_GetSizeOfTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx))]
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length to be copied.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Available transmission buffer size after data are copied to destination buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      HdrCopied         Indicates if header is copied completely.
 *                                    [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_CopyOemSpecific(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_TransmitElement()
 *********************************************************************************************************************/
/*! \brief          Triggers transmission of Tx TCP queue.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_TransmitElement(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_RemoveElement()
 *********************************************************************************************************************/
/*! \brief          Removes first Tx TCP queue element (used to confirm transmission of element).
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      Result            Result of transmission.
 *                                    [range: E_OK, E_NOT_OK]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_RemoveElement(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_HandleTransmit()
 *********************************************************************************************************************/
/*! \brief          Handles pending Tx TCP queue elements.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_HandleTransmit(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpGen_CopyGenHdr()
 *********************************************************************************************************************/
/*! \brief          Copies Generic Header to header buffer.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         E_OK              Header (already) copied completely.
 *  \return         E_NOT_OK          Header not copied completely.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpGen_CopyGenHdr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpGen_CopyDataToHeaderBuffer()
 *********************************************************************************************************************/
/*! \brief          Copies data to header buffer.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      BytesToCopy       Number of bytes to copy.
 *                                    [range: undefined]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpGen_CopyDataToHeaderBuffer(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  uint32 BytesToCopy);

/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxData()
 *********************************************************************************************************************/
/*! \brief          Handles SoAd copy calles.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in,out]  PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[out]     ErrorIdPtr        Pointer to DET error if one occurred.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         BUFREQ_OK         Data copied successfully.
 *  \return         BUFREQ_E_NOT_OK   Data was not copied because an error occurred.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_RxTcp_HandleCopyRxData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxDataCopy()
 *********************************************************************************************************************/
/*! \brief          Handles message copying.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in,out]  PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcp_HandleCopyRxDataCopy(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr);

/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxDataCopyBody()
 *********************************************************************************************************************/
/*! \brief          Handles message body copying.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in,out]  PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcp_HandleCopyRxDataCopyBody(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxDataAppendNack()
 *********************************************************************************************************************/
/*! \brief          Handles copying of the Nack data.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in,out]  PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcp_HandleCopyRxDataAppendNack(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2CONST(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleRoutingActivationReq()
 *********************************************************************************************************************/
/*! \brief          Processes Routing Activation Request messages.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPRoutingActivationAuthenticationConfirmation
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleRoutingActivationReq(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleCopiedRoutingActivationReq()
 *********************************************************************************************************************/
/*! \brief          Processes Routing Activation Request messages if copied to local buffer completely.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleCopiedRoutingActivationReq(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleCopiedRoutingActivationReqOem()
 *********************************************************************************************************************/
/*! \brief          Processes OEM part of a Routing Activation Request message.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleCopiedRoutingActivationReqOem(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckForRegisteredTester()
 *********************************************************************************************************************/
/*! \brief          Checks if a tester is already registered on the connection and if it is the same as in this routing
 *                  activation request.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterIdx         Tester Index.
 *                                    [range: TesterIdx < DoIP_GetSizeOfTester()]
 *  \return         E_OK              Tester is already registered for this connection or not registered at all and at
 *                                    least one other TCP connection is left.
 *  \return         E_NOT_OK          Tester is registered to another connection or no other TCP connection is left.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckForRegisteredTester(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_GetRoutingActivationIdxByActivationType()
 *********************************************************************************************************************/
/*! \brief          Returns the routing activation index of a routing activation type for a specific tester in case a
 *                  suitable routing activation index could be found.
 *  \details        Considers the required security of the activation type and the security level of the socket.
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterIdx         Tester Index.
 *                                    [range: TesterIdx < DoIP_GetSizeOfTester()]
 *  \param[in]      ActivationType    Activation type.
 *                                    [range: undefined]
 *  \param[out]     RoutActIdxPtr     Pointer to routing activation index.
 *                                    [range: IN:   POINTER != NULL_PTR,
 *                                            OUT:  RoutActIdxPtr <= DoIP_GetSizeOfRoutingActivation()]
 *  \return         E_OK              Routing activation index found/returned.
 *  \return         E_NOT_OK          Routing activation index not found/returned.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPRoutingActivationSecurityRequirement
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_GetRoutingActivationIdxByActivationType(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx,
  uint8 ActivationType,
  P2VAR(DoIP_SizeOfRoutingActivationType, AUTOMATIC, DOIP_APPL_VAR) RoutActIdxPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckConnectionNum()
 *********************************************************************************************************************/
/*! \brief          Checks if tester is not active on another connection and an additional connection is left. If there
 *                  is no free connection alive checks gets triggered.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterAddr        Tester logical Address.
 *                                    [range: undefined]
 *  \return         E_OK              Tester is not active and an additional connection is left.
 *  \return         E_NOT_OK          Tester is active or no additional connection is left.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckConnectionNum(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckSecurityReq()
 *********************************************************************************************************************/
/*! \brief          Checks if the security requirements of the connection and the routing activation match and sends
 *                  negative response if security is required by routing activation but connection is not secured.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterIdx         Tester index in configured tester array.
 *                                    [range: TesterIdx < DoIP_GetSizeOfTester()]
 *  \param[in]      TesterAddr        Tester logical Address.
 *                                    [range: undefined]
 *  \param[in]      RoutActIdx        Routing activation index.
 *                                    [range: RoutActIdx <= DoIP_GetSizeOfRoutingActivation()]
 *  \return         E_OK              Security requirements match.
 *  \return         E_NOT_OK          Security requirements do not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckSecurityReq(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx,
  uint16 TesterAddr,
  DoIP_SizeOfRoutingActivationType RoutActIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_TriggerAliveCheckSingleConnection()
 *********************************************************************************************************************/
/*! \brief          Performs alive check a single connection with registered tester.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_TriggerAliveCheckSingleConnection(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_TriggerAliveCheckAllConnections()
 *********************************************************************************************************************/
/*! \brief          Performs alive checks on all local address related connections with registered testers.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_TriggerAliveCheckAllConnections(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_GetTesterIdxByTesterAddr()
 *********************************************************************************************************************/
/*! \brief          Gets the tester index of a tester address.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterAddr        Tester logical address.
 *                                    [range: undefined]
 *  \param[out]     TesterIdx         Tester index in configured tester array.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         E_OK              Tester index found.
 *  \return         E_NOT_OK          No tester index found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_GetTesterIdxByTesterAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr,
  P2VAR(DoIP_SizeOfTesterType, AUTOMATIC, DOIP_APPL_VAR) TesterIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_GetConnectionIdxOfTesterAddr()
 *********************************************************************************************************************/
/*! \brief          Identifies a connection by tester address.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterAddr        Tester logical address.
 *                                    [range: undefined]
 *  \return         Connection TCP Index or DoIP_GetSizeOfConnectionTcp() if not tester was found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(DoIP_SizeOfConnectionTcpType, DOIP_CODE) DoIP_RxTcpRout_GetConnectionIdxOfTesterAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_IsSockAvailable()
 *********************************************************************************************************************/
/*! \brief          Indicates if a free TCP_DATA sockets for a new connection is available.
 *  \details        -
 *  \param[in]      ConnectionGroupIdx  Connection group index.
 *                                      [range: ConnectionGroupIdx < DoIP_GetSizeOfConnectionGroup()]
 *  \return         TRUE                A free socket is available.
 *  \return         FALSE               No socket is available.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(boolean, DOIP_CODE) DoIP_RxTcpRout_IsSockAvailable(
  DoIP_SizeOfConnectionGroupType ConnectionGroupIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_PerformRoutingActivation()
 *********************************************************************************************************************/
/*! \brief          Performs authentication and confirmation via OEM specific part and activates routing if successful.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \trace          DSGN-DoIPRoutingActivationAuthenticationConfirmation
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_PerformRoutingActivation(void);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_RegisterTester()
 *********************************************************************************************************************/
/*! \brief          Register authentified or confirmed tester.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterIdx         Tester index in configured tester array.
 *                                    [range: TesterIdx <= DoIP_GetSizeOfTester()]
 *  \param[in]      TesterAddr        Logical target address.
 *                                    [range: undefined]
 *  \param[in]      RoutActIdx        Routing activation index.
 *                                    [range: RoutActIdx <= DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      NextState         Next routing activation state.
 *                                    [range: DOIP_ROUT_ACTIV_STATE_*]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_RegisterTester(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx,
  uint16 TesterAddr,
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  DoIP_RoutActivStateType NextState);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_PerformRoutingActivationAuth()
 *********************************************************************************************************************/
/*! \brief          Performs routing activation authentication.
 *  \details        -
 *  \param[out]     NextStatePtr  Pointer to next routing activation state.
 *                                [range: POINTER != NULL_PTR]
 *  \return         E_OK          Authentication successfully performed.
 *  \return         E_NOT_OK      Authentication not successfully performed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \trace          DSGN-DoIPRoutingActivationAuthenticationConfirmation
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_PerformRoutingActivationAuth(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_PerformRoutingActivationConf()
 *********************************************************************************************************************/
/*! \brief          Performs routing activation confirmation.
 *  \details        -
 *  \param[in,out]  NextStatePtr  Indicates next state for main function retry handling.
 *                                [range: POINTER != NULL_PTR && *POINTER == DOIP_ROUT_ACTIV_STATE_*]
 *  \return         E_OK          Confirmation successfully performed.
 *  \return         E_NOT_OK      Confirmation not successfully performed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \trace          DSGN-DoIPRoutingActivationAuthenticationConfirmation
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_PerformRoutingActivationConf(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleRetryRoutingActivationAuthAndConf()
 *********************************************************************************************************************/
/*! \brief          Handles authentication and confirmation of routing activations dependent on the state.
 *  \details        Called in MainFunction after routing activation was processed in reception context
 *                  (DoIP_RxTcpRout_PerformRoutingActivation).
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPRoutingActivationAuthenticationConfirmation
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleRetryRoutingActivationAuthAndConf(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_NotifyChannelReady()
 *********************************************************************************************************************/
/*! \brief          Notify user when ComM channel is ready to transmit diagnostic messages.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_NotifyChannelReady(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleAliveCheckTimeoutOfTcpConnections()
 *********************************************************************************************************************/
/*! \brief          Handles alive check DoIP protocol timeout of TCP connections.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 1 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleAliveCheckTimeoutOfTcpConnections(
  DoIP_InterfaceIterType InterfaceIdx);
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleAliveCheckTimeoutOfRoutActHandler()
 *********************************************************************************************************************/
/*! \brief          Handles alive check DoIP protocol timeout of routing activation handler.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 1 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleAliveCheckTimeoutOfRoutActHandler(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleAliveCheckTimeoutsSocketsClose()
 *********************************************************************************************************************/
/*! \brief          Closes sockets if alive check timeout occurred and no alive check response has been received on it.
 *  \details        -
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 1 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleAliveCheckTimeoutsSocketsClose(void);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckConnectionAvailablility()
 *********************************************************************************************************************/
/*! \brief          Checks if tester is not active on another connection and an additional connection is left. If there
 *                  is no free connection request gets discarded.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      TesterAddr        Tester logical Address.
 *                                    [range: undefined]
 *  \return         E_OK              Tester is not active and an additional connection is left.
 *  \return         E_NOT_OK          Tester is active or no additional connection is left.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckConnectionAvailablility(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_TesterAddrOfRoutActivHandlerType TesterAddr);

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleInactivityTimeouts()
 *********************************************************************************************************************/
/*! \brief          Handles inactivity DoIP protocol timeouts.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleInactivityTimeouts(
  DoIP_InterfaceIterType InterfaceIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpAlive_HandleAliveCheckRes()
 *********************************************************************************************************************/
/*! \brief          Processes Alive Check Response messages.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the alive check response.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpAlive_HandleAliveCheckRes(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpAlive_HandleCopiedAliveCheckRes()
 *********************************************************************************************************************/
/*! \brief          Processes copied Alive Check Response messages.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpAlive_HandleCopiedAliveCheckRes(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpAlive_RemoveAliveCheckFromRoutActHandler()
 *********************************************************************************************************************/
/*! \brief          Removes an alive check entry in alive check list of routing activation handler.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpAlive_RemoveAliveCheckFromRoutActHandler(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleDiagnosticMessage()
 *********************************************************************************************************************/
/*! \brief          Processes Diagnostic messages.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_HandleDiagnosticMessage(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleValidDiagnosticMessage()
 *********************************************************************************************************************/
/*! \brief          Processes valid diagnostic message.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 5 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_HandleValidDiagnosticMessage(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyLogicalAddr()
 *********************************************************************************************************************/
/*! \brief          Copies logical address of diagnostic message.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         E_OK              Logical address (already) successfully copied.
 *  \return         E_NOT_OK          Logical address not successfully copied.
 *  \return         DOIP_E_PENDING    Logical address partly copied.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyLogicalAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleCopiedLogicalAddr()
 *********************************************************************************************************************/
/*! \brief          Handles copied logical address of diagnostic message.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \return         E_OK              Logical address valid.
 *  \return         E_NOT_OK          Logical address invalid.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_HandleCopiedLogicalAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleTargetLogicalAddress()
 *********************************************************************************************************************/
/*! \brief          Handles target logical address of diagnostic message.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \return         E_OK              Logical address valid.
 *  \return         E_NOT_OK          Logical address invalid.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace          DSGN-DoIPForwardReceivedTargetAddressWithTargetAddressVerification
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_HandleTargetLogicalAddress(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_AppendNackData()
 *********************************************************************************************************************/
/*! \brief          Appends Diagnostic data to Ack/Nack buffer.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_NACK_APPEND_REQ
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_AppendNackData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2CONST(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyUserDataToHeaderBuffer()
 *********************************************************************************************************************/
/*! \brief          Copies the required amount of received user data for Pdu verification and/or start of reception
 *                  procedure to the local Rx header buffer.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx     TCP Connection Index.
 *                                       [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr           Pointer to source buffer and length.
 *                                       [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr        Pointer to remaining size of the diagnostic message.
 *                                       [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr       Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                       [range: POINTER != NULL_PTR]
 *  \return         E_OK                 Rx Pdu header already successfully copied.
 *  \return         E_NOT_OK             Rx Pdu header not successfully copied.
 *  \return         DOIP_E_JUST_COPIED   Rx Pdu header successfully copied.
 *  \return         DOIP_E_PENDING       Rx Pdu header data still pending to be copied.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyUserDataToHeaderBuffer(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

#if ( DOIP_VERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_VerifyRxPdu()
 *********************************************************************************************************************/
/*! \brief          Verifies the Rx Pdu.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      HdrLenField       Header length field value.
 *                                    [range: HdrLenField >= DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE]
 *  \return         E_OK              Rx Pdu successfully verified.
 *  \return         E_NOT_OK          Rx Pdu not successfully verified.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_VERIFY_RX_PDU_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_VerifyRxPdu(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint32 HdrLenField);
#endif /* DOIP_VERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_StartOfReception()
 *********************************************************************************************************************/
/*! \brief          Starts reception of a message.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      ChannelIdx        Channel index.
 *                                    [range: ChannelIdx < DoIP_GetSizeOfChannel()]
 *  \param[in]      HdrLenField       Header length field value.
 *                                    [range: HdrLenField >= DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         E_OK              Rx Pdu reception successfully started.
 *  \return         E_NOT_OK          Rx Pdu reception not successfully started.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_StartOfReception(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfChannelType ChannelIdx,
  uint32 HdrLenField,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyUserData()
 *********************************************************************************************************************/
/*! \brief          Copies the buffered and unbuffered user data.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_CopyUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyBufferedUserData()
 *********************************************************************************************************************/
/*! \brief          Copies buffered user data.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      QueryBufSize      Indicates if buffer size has to be queried.
 *                                    [range: TRUE, FALSE]
 *  \return         E_OK              Buffered user data (already) successfully copied.
 *  \return         E_NOT_OK          Buffered user data not successfully copied.
 *  \return         DOIP_E_PENDING    Buffered user data copy pending.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyBufferedUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  boolean QueryBufSize);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleCopyBufferedUserData()
 *********************************************************************************************************************/
/*! \brief          Handles copying buffered user data.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      HdrLenField       Header length field value.
 *                                    [range: HdrLenField >= DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in]      QueryBufSize      Indicates if buffer size has to be queried.
 *                                    [range: TRUE, FALSE]
 *  \return         E_OK              Buffered user data (already) successfully copied.
 *  \return         E_NOT_OK          Buffered user data not successfully copied.
 *  \return         DOIP_E_PENDING    Buffered user data copy pending.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_HandleCopyBufferedUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint32 HdrLenField,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  boolean QueryBufSize);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyUnbufferedUserData()
 *********************************************************************************************************************/
/*! \brief          Copies unbuffered user data.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_CopyUnbufferedUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyDataToPduR()
 *********************************************************************************************************************/
/*! \brief          Copies data to PduR.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  LocalPduInfoPtr   Pointer to local buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         E_OK              Data successfully copied.
 *  \return         E_NOT_OK          Data not successfully copied.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyDataToPduR(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) LocalPduInfoPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutAct()
 *********************************************************************************************************************/
/*! \brief          Retrieves a channel by target address for a specific routing activation considering data length in
 *                  case of Pdu size routing.
 *  \details        -
 *  \param[in]      RoutActIdx      Routing activation index.
 *                                  [range: RoutActIdx < DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      TgtAddr         Logical target address.
 *                                  [range: undefined]
 *  \param[in]      DataLen         Data length.
 *                                  [range: 0 <= DataLen <= (DOIP_MAX_REQUEST_BYTES - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)]
 *  \param[in]      PduSizeRouting  Pdu size routing.
 *                                  [range: TRUE, FALSE]
 *  \return         Channel index or invalid channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutAct(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  boolean PduSizeRouting);

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseBinSearch()
 *********************************************************************************************************************/
/*! \brief          Retrieves a channel by target address for a specific routing activation considering data length in
 *                  case of Pdu size routing use binary search.
 *  \details        -
 *  \param[in]      RoutActIdx      Routing activation index.
 *                                  [range: RoutActIdx < DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      TgtAddr         Logical target address.
 *                                  [range: undefined]
 *  \param[in]      DataLen         Data length.
 *                                  [range: 0 <= DataLen <= (DOIP_MAX_REQUEST_BYTES - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)]
 *  \param[in]      PduSizeRouting  Pdu size routing.
 *                                  [range: TRUE, FALSE]
 *  \return         Channel index or invalid channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 4 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseBinSearch(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  boolean PduSizeRouting);
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF */

#if ( (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseIteration()
 *********************************************************************************************************************/
/*! \brief          Retrieves a channel by target address for a specific routing activation in case of bit masking use
 *                  iteration.
 *  \details        -
 *  \param[in]      RoutActIdx  Routing activation index.
 *                              [range: RoutActIdx < DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      TgtAddr     Logical target address.
 *                              [range: undefined]
 *  \return         Channel index or invalid channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 3 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseIteration(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr);
#endif /* (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddr()
 *********************************************************************************************************************/
/*! \brief          Retrieves a channel by target address.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \param[in]      TgtAddr        Logical target address.
 *                                 [range: undefined]
 *  \return         Channel index or invalid channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddr(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  uint16 TgtAddr);

#if ( (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_OFF) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrUseBinSearch()
 *********************************************************************************************************************/
/*! \brief          Retrieves a channel by target address use binary search.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \param[in]      TgtAddr        Logical target address.
 *                                 [range: undefined]
 *  \return         Channel index or invalid channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrUseBinSearch(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  uint16 TgtAddr);
#endif /* (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_OFF) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

#if ( (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrUseIteration()
 *********************************************************************************************************************/
/*! \brief          Retrieves a channel by target address use iteration.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \param[in]      TgtAddr        Logical target address.
 *                                 [range: undefined]
 *  \return         Channel index or invalid channel index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
/* PRQA S 0779 2 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrUseIteration(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  uint16 TgtAddr);
#endif /* (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) */

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_SearchChannel()
 *********************************************************************************************************************/
/*! \brief          Searches a channel by target address for a specific routing activation.
 *  \details        A binary search is used to find the channel.
 *  \param[in]      RoutActIdx  Routing activation index.
 *                              [range: RoutActIdx < DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      TgtAddr     Logical target address.
 *                              [range: undefined]
 *  \return         Found channel indirection index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_SearchChannel(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr);
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF */

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CheckChannel()
 *********************************************************************************************************************/
/*! \brief          Checks for a better fitting channel by target address for a specific routing activation considering
 *                  data length in case of Pdu size routing.
 *  \details        -
 *  \param[in]      RoutActIdx      Routing activation index.
 *                                  [range: RoutActIdx < DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      TgtAddr         Logical target address.
 *                                  [range: undefined]
 *  \param[in]      DataLen         Data length.
 *                                  [range: 0 <= DataLen <= (DOIP_MAX_REQUEST_BYTES - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)]
 *  \param[in]      ChannelIndIdx   Found channel index to check.
 *                                  [range: ChannelIndIdx < DoIP_GetSizeOfChannelInd()]
 *  \param[in]      PduSizeRouting  Pdu size routing.
 *                                  [range: TRUE, FALSE]
 *  \return         Found channel indirectionindex.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_CheckChannel(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  DoIP_SizeOfChannelIndType ChannelIndIdx,
  boolean PduSizeRouting);
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF */

#if ( (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_SearchPduSizeRoutingGroupStartIndex()
 *********************************************************************************************************************/
/*! \brief          Searchs for the (left side) start index of the pdu size routing group with the same target address
 *                  on the same routing activation.
 *  \details        -
 *  \param[in]      RoutActIdx      Routing activation index.
 *                                  [range: RoutActIdx < DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      TgtAddr         Logical target address.
 *                                  [range: undefined]
 *  \param[in]      ChannelIndIdx   Found channel indirection index to check.
 *                                  [range: ChannelIndIdx < DoIP_GetSizeOfChannelInd()]
 *  \return         Found channel indirection start index of pdu size routing group.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_SUPPORT_PDU_SIZE_ROUTING
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_SearchPduSizeRoutingGroupStartIndex(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  DoIP_SizeOfChannelIndType ChannelIndIdx);
#endif /* (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

#if ( (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_SearchPduSizeRoutingGroupBestIndex()
 *********************************************************************************************************************/
/*! \brief          Searches for the best matching channel by target address for a specific routing activation
 *                  considering data length. The search is performed from the (left side) start index to the right side.
 *  \details        -
 *  \param[in]      RoutActIdx          Routing activation index.
 *                                      [range: RoutActIdx < DoIP_GetSizeOfRoutingActivation()]
 *  \param[in]      TgtAddr             Logical target address.
 *                                      [range: undefined]
 *  \param[in]      DataLen             Data length.
 *                                      [range: 0 <= DataLen <= (DOIP_MAX_REQUEST_BYTES - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)]
 *  \param[in]      ChannelIndStartIdx  Found channel indirection start index to check.
 *                                      [range: ChannelIndIdx < DoIP_GetSizeOfChannelInd()]
 *  \param[in]      PduSizeRouting      Pdu size routing.
 *                                      [range: TRUE, FALSE]
 *  \return         Found channel indirection index of best matching channel.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_SUPPORT_PDU_SIZE_ROUTING
 *********************************************************************************************************************/
 /* PRQA S 0779 5 */ /* MD_MSR_Rule5.2_0779 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_SearchPduSizeRoutingGroupBestIndex(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  DoIP_SizeOfChannelIndType ChannelIndStartIdx,
  boolean PduSizeRouting);
#endif /* (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

#if ( (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_PduSizeRoutValidateChannel()
 *********************************************************************************************************************/
/*! \brief          Validates if a channel is the best fitting channel for a target address.
 *  \details        -
 *  \param[in]      ChannelIdx      Channel index.
 *                                  [range: ChannelIdx < DoIP_GetSizeOfChannel()]
 *  \param[in]      DataLen         Data length.
 *                                  [range: 0 <= DataLen <= (DOIP_MAX_REQUEST_BYTES - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE)]
 *  \param[in,out]  BestMaxPduSize  Configured length of best matching channel up to now.
 *                                  [range: POINTER != NULL_PTR]
 *  \param[in]      PduSizeRouting  Pdu size routing.
 *                                  [range: TRUE, FALSE]
 *  \return         E_OK            Best matching channel (up to now).
 *  \return         E_NOT_OK        Channel does not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_SUPPORT_PDU_SIZE_ROUTING
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_PduSizeRoutValidateChannel(
  DoIP_SizeOfChannelType ChannelIdx,
  uint32 DataLen,
  P2VAR(DoIP_MaxPduSizeOfChannelType, AUTOMATIC, DOIP_APPL_VAR) BestMaxPduSize,
  boolean PduSizeRouting);
#endif /* (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleRetryCopyBufferedUserData()
 *********************************************************************************************************************/
/*! \brief          Handles copying of buffered user data.
 *  \details        -
 *  \param[in]      InterfaceIdx   Interface index.
 *                                 [range: InterfaceIdx < DoIP_GetSizeOfInterface()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_HandleRetryCopyBufferedUserData(
  DoIP_InterfaceIterType InterfaceIdx);

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpOem_HandleOemPayloadTypes()
 *********************************************************************************************************************/
/*! \brief          Processes OEM specific messages.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to destination buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpOem_HandleOemPayloadTypes(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_ReserveBuf()
 *********************************************************************************************************************/
/*! \brief          Finds and reserves a reception buffer for the received OEM specific message.
 *  \details        -
 *  \param[in]      ConnectionIdx            Connection Index.
 *                                           [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[in]      UserDataLength           Length of user data.
 *  \return         DOIP_GEN_NACK_TYPE       No reception buffer configured on the connection.
 *  \return         DOIP_GEN_NACK_MAX_LEN    No large enough reception buffer configured on the connection.
 *  \return         DOIP_GEN_NACK_OUT_OF_MEM No reception buffer available for the connection.
 *  \return         DOIP_GEN_NACK_OK         A suiting buffer was reserved.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(uint8, DOIP_CODE) DoIP_RxTcpOem_ReserveBuf(
  DoIP_SizeOfConnectionType ConnectionIdx,
  PduLengthType UserDataLength);

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_ReleaseBuf()
 *********************************************************************************************************************/
/*! \brief          Releases the corresponding reception buffer.
 *  \details        -
 *  \param[in]      OemRxBufCfgIdx           OEM reception buffer configuration index.
 *                                           [range: OemRxBufCfgIdx < DoIP_GetSizeOfOemPayloadRxBufConfig()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpOem_ReleaseBuf(
  DoIP_SizeOfOemPayloadRxBufConfigType OemRxBufCfgIdx);

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_CopyPayload()
 *********************************************************************************************************************/
/*! \brief          Copy payload data to OEM Rx buffer and forward data via callback if all data has been provided.
 *  \details        -
 *  \param[in]      ConnectionTcpIdx  TCP Connection Index.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Pointer to source buffer and length.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  BufferSizePtr     Pointer to remaining size of the diagnostic message.
 *                                    [range: POINTER != NULL_PTR]
 *  \param[in,out]  ReadDataLenPtr    Pointer to data offset for read/write. Number of copied bytes will be added.
 *                                    [range: POINTER != NULL_PTR]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_RxTcpOem_CopyPayload(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr);

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_ForwardToUser()
 *********************************************************************************************************************/
/*! \brief          Forwards received data to the user via callback if configured.
 *  \details        -
 *  \param[in]      ConnectionIdx       Connection Index.
 *                                      [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[in]      HdrTypeField        Value of the received header type field.
 *                                      [range: undefined]
 *  \param[in]      Flags               Flags indicates protocol (TCP/UDP) and routing activation state.
 *                                      [range: DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_TCP,
 *                                      (DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_TCP | DOIP_OEM_PAYLOAD_TYPE_FLAG_ROUT_ACTIV)]
 *  \param[in]      OemPayloadRxBufPtr  PDU information structure which contains the length and the payload data of the
 *                                      received message.
 *                                      [range: POINTER != NULL_PTR]
 *  \return         E_OK                Known payload type.
 *  \return         E_NOT_OK            Unknown payload type.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpOem_ForwardToUser(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrTypeField,
  uint8 Flags,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) OemPayloadRxBufPtr);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_HandleTpTransmit()
 *********************************************************************************************************************/
/*! \brief          Handles transmission request of a specific TP Pdu
 *  \details        This service is called to request the transfer data from the PduRouter to the SoAd. It is used to
 *                  indicate the transmission which will be performed by SoAd or in the DoIP_Mainfunction. Within the
 *                  provided DoIPPduRTxInfoPtr only SduLength is valid (no data)! If this function returns E_OK then
 *                  the SoAd module will raise a subsequent call to PduR_DoIPCopyTxData via DoIP_SoAdTpCopyRxData in
 *                  order to get the data to send.
 *  \param[in]      DoIPPduRTxId        DoIP unique identifier of the Pdu to be transmitted by the PduR.
 *                                      [range: DoIPPduRTxId < DoIP_GetSizeOfPduRTxPduId()]
 *  \param[in]      DoIPPduRTxInfoPtr   Tx Pdu information structure which contains the length of the DoIPTxMessage.
 *                                      [range: POINTER != NULL_PTR && POINTER->SduLength > 0]
 *  \return         E_OK                The request has been accepted.
 *  \return         E_NOT_OK            The request has not been accepted, e.g. no resources are available for
 *                                      transmission.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE - Reentrant for different DoIPPduRTxIds. Non reentrant for the same DoIPPduRTxId.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleTpTransmit(
  DoIP_SizeOfPduRTxPduIdType DoIPPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) DoIPPduRTxInfoPtr);

#if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleTpCancelTransmit()
 *********************************************************************************************************************/
/*! \brief          Handles transmission request cancellation of a specific TP Pdu
 *  \details        This service primitive is used to cancel the transfer of pending DoIPPduRTxIds. The connection is
 *                  identified by DoIPPduRTxId. If the function returns the cancellation is requested but not yet
 *                  performed.
 *  \param[in]      DoIPPduRTxId  DoIP unique identifier of the Pdu to be canceled by the PduR.
 *                                [range: DoIPPduRTxId < DoIP_GetSizeOfPduRTxPduId()]
 *  \return         E_OK          Transmit cancellation request of the specified DoIPPduRTxId is accepted.
 *  \return         E_NOT_OK      The transmit cancellation request of the DoIPPduRTxId has been rejected.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleTpCancelTransmit(
  DoIP_SizeOfPduRTxPduIdType DoIPPduRTxId);
#endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */

#if ( DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleTpCancelReceive()
 *********************************************************************************************************************/
/*! \brief          Handles reception cancellation request of a specific TP Pdu
 *  \details        By calling this API with the corresponding DoIPPduRRxId the currently ongoing data reception is
 *                  terminated immediately. If the function returns the cancellation is requested but not yet
 *                  performed.
 *  \param[in]      DoIPPduRRxId  DoIP unique identifier of the Pdu for which reception shall be canceled by the PduR.
 *                                [range: DoIPPduRRxId < DoIP_GetSizeOfPduRRxCancelPduId()]
 *  \return         E_OK          Reception was canceled successfully.
 *  \return         E_NOT_OK      Reception was not canceled.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleTpCancelReceive(
  DoIP_SizeOfPduRRxCancelPduIdType DoIPPduRRxId);
#endif /* DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON */

#if ( DOIP_IF_API_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleIfTransmit()
 *********************************************************************************************************************/
/*! \brief          Handles transmission request of a specific IF Pdu
 *  \details        -
 *  \param[in]      PduRTxPduId   Identification of the IF Pdu.
 *                                [range: PduRTxPduId < DoIP_GetSizeOfPduRTxPduId()]
 *  \param[in]      PduInfoPtr    Length and pointer to the buffer of the IF Pdu.
 *                                [range: POINTER != NULL_PTR && POINTER->SduLength > 0]
 *  \return         E_OK          Request is accepted by the destination module.
 *  \return         E_NOT_OK      Request is not accepted by the destination module.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE - Reentrant for different ids. Non reentrant for the same id.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleIfTransmit(
  DoIP_SizeOfPduRTxPduIdType PduRTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr);
#endif /* DOIP_IF_API_ENABLED == STD_ON */

#if ( DOIP_IF_API_ENABLED == STD_ON )
# if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleIfCancelTransmit()
 *********************************************************************************************************************/
/*! \brief          Handles transmission cancellation request of a specific IF Pdu
 *  \details        -
 *  \param[in]      PduRTxPduId   Identification of the IF Pdu to be cancelled.
 *                                [range: PduRTxPduId < DoIP_GetSizeOfPduRTxPduId()]
 *  \return         E_OK          Cancellation was executed successfully by the destination module.
 *  \return         E_NOT_OK      Cancellation was rejected by the destination module.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleIfCancelTransmit(
  DoIP_SizeOfPduRTxPduIdType PduRTxPduId);
# endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */
#endif /* DOIP_IF_API_ENABLED == STD_ON */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxOem_Transmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission of a message with OEM specific payload type on a specific connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[in]    PayloadType             The payload type that shall be used for the message.
 *  \param[in]    PayloadDataPtr          PDU information structure which contains the length and the data of the
 *                                        message.
 *                                        [range: PayloadDataPtr != NULL_PTR]
 *  \return       E_OK                    Transmit request was accepted.
 *  \return       E_NOT_OK                Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous  TRUE
 *  \config       DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxOem_Transmit(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PayloadDataPtr);

/**********************************************************************************************************************
 *  DoIP_TxOem_ReserveBuf()
 *********************************************************************************************************************/
/*! \brief        Searches for a free transmission OEM buffer on a connection, reserves it and copies the transmission
 *                data to the buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIP_GetSizeOfConnection()]
 *  \param[out]   OemTxBufCfgIdxPtr       Pointer to index of reserved OEM Tx Buffer Configuration.
 *                                        [range: OemTxBufCfgIdxPtr != NULL_PTR]
 *  \param[in]    PayloadDataPtr          PDU information structure which contains the length and the data of the
 *                                        message.
 *                                        [range: PayloadDataPtr != NULL_PTR]
 *  \return       E_OK                    Buffer reserved and transmission data added to buffer.
 *  \return       E_NOT_OK                No buffer could be reserved.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE - Reentrant for different ConnectionIdxs. Non reentrant for the same ConnectionIdx.
 *  \synchronous  TRUE
 *  \config       DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxOem_ReserveBuf(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(DoIP_SizeOfOemPayloadTxBufConfigType, AUTOMATIC, DOIP_APPL_VAR) OemTxBufCfgIdxPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PayloadDataPtr);

/**********************************************************************************************************************
 *  DoIP_TxOem_ReleaseBuf()
 *********************************************************************************************************************/
/*! \brief        Releases a previously reserved OEM transmission buffer.
 *  \details      -
 *  \param[in]    OemTxBufCfgIdx          OEM Tx Buffer Configuration index.
 *                                        [range: OemTxBufCfgIdx < DoIP_GetSizeOfOemPayloadTxBufConfig()]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE - Reentrant for different OemTxBufCfgIdxs. Non reentrant for the same OemTxBufCfgIdx.
 *  \synchronous  TRUE
 *  \config       DOIP_OEM_PAYLOAD_TYPE_ENABLED
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxOem_ReleaseBuf(
  DoIP_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_HandleSoAdTpCopyTxData()
 *********************************************************************************************************************/
/*! \brief          Handles transmit data of a Pdu
 *  \details        This function is called to acquire the transmit data of an I-PDU segment (N-PDU). Each call to this
 *                  function provides the next part of the TP Pdu data. The size of the remaining data is written to
 *                  the position indicated by availableDataPtr.
 *  \param[in]      ConnectionTcpIdx  Identification of the transmitted TP Pdu.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      PduInfoPtr        Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 *                                    (SduLength). An SduLength of 0 can be used to query the current amount of
 *                                    available data in the upper layer module. In this case, the SduDataPtr may be a
 *                                    NULL_PTR.
 *                                    [range: POINTER != NULL_PTR, if POINTER->SduLength == 0 then POINTER->SduDataPtr
 *                                    may be NULL_PTR]
 *  \param[out]     AvailableDataPtr  Indicates the remaining number of bytes that are available in the upper layer
 *                                    module's Tx buffer.
 *                                    [range: POINTER != NULL_PTR]
 *  \return         BUFREQ_OK         Data has been copied to the transmit buffer completely as requested.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied. Request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleSoAdTpCopyTxData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIP_HandleSoAdTpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Handles transmission of confirmation callback for TP
 *  \details        This function is called after the TP Pdu has been transmitted on its network, the result indicates
 *                  whether the transmission was successful or not.
 *  \param[in]      ConnectionTcpIdx  Identification of the transmitted TP Pdu.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \param[in]      Result            Result of the transmission of the TP Pdu.
 *                                    [range: E_OK, E_NOT_OK]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different ConnectionTcpIdxs. Non reentrant for the same ConnectionTcpIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleSoAdTpTxConfirmation(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIP_HandleSoAdTpStartOfReception()
 *********************************************************************************************************************/
/*! \brief          Handles receive indication callback for TP
 *  \details        This function is called at the start of receiving an N-SDU. The N-SDU might be fragmented into
 *                  multiple N-PDUs or might consist of a single N-PDU.
 *  \param[in]      ConnectionTcpIdx  Identification of the TP Pdu.
 *                                    [range: ConnectionTcpIdx < DoIP_GetSizeOfConnectionTcp()]
 *  \return         BUFREQ_OK         Connection has been accepted. bufferSizePtr indicates the available receive
 *                                    buffer; reception is continued. If no buffer of the requested size is
 *                                    available, a receive buffer size of 0 shall be indicated by bufferSizePtr.
 *  \return         BUFREQ_E_NOT_OK   Connection has been rejected; reception is aborted. bufferSizePtr remains
 *                                    unchanged.
 *  \return         BUFREQ_E_OVFL     No buffer of the required length can be provided; reception is aborted.
 *                                    bufferSizePtr remains unchanged.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleSoAdTpStartOfReception(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx);

/**********************************************************************************************************************
 *  DoIP_HandleSoAdIfRxIndication()
 *********************************************************************************************************************/
/*! \brief          Handles receive indication callback for IF
 *  \details        Indication of a received IF Pdu from a lower layer communication interface module.
 *  \param[in]      ConnectionIdx   Id of the received IF Pdu.
 *                                  [range: ConnectionIdx < DoIP_GetSizeOfConnection() &&
 *                                          DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == *CONNECTIONUDP_*]
 *  \param[in]      PduInfoPtr      Contains the length (SduLength) of the received IF Pdu and a pointer to a buffer
 *                                  (SduDataPtr) containing the IF Pdu.
 *                                  [range: POINTER != NULL_PTR && POINTER->SduDataPtr != NULL_PTR]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different RxPduIds. Non reentrant for the same RxPduId.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleSoAdIfRxIndication(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr);

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleDhcpEvent()
 *********************************************************************************************************************/
/*! \brief          Handles DHCP event.
 *  \details        -
 *  \param[in]      LocalIpAddrId   IP address identifier.
 *                                  [range: undefined]
 *  \param[in]      Event           Indicates the received message type or the message type which will be send.
 *                                  [range: SOAD_DHCP_EVENT_*]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \config         DOIP_DHCP_OPT_VENDOR_CLASS
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleDhcpEvent(
  SoAd_LocalAddrIdType LocalIpAddrId,
  SoAd_DhcpEventType Event);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

/**********************************************************************************************************************
 *  DoIP_HandleActivationLineSwitch()
 *********************************************************************************************************************/
/*! \brief          Saves activation line state change requests for main function processing.
 *  \details        -
 *  \param[in]      InterfaceIdx  DoIP unique identifier of the interface.
 *                                [range: InterfaceId < DoIP_GetSizeOfInterface()]
 *  \param[in]      Active        Flag to indicate the state of the activation line.
 *                                [range: TRUE, FALSE]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleActivationLineSwitch(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  boolean Active);

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_InitDroppedPacketCounter()
 *********************************************************************************************************************/
/*! \brief          Initializes the count of dropped TCP/UDP packets to zero.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_GET_RESET_MEASUREMENT_DATA
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_InitDroppedPacketCounter(void);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounterTcp()
 *********************************************************************************************************************/
/*! \brief          Increments the count of dropped TCP packets.
 *  \details        -
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_GET_RESET_MEASUREMENT_DATA
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounterTcp(void);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounterUdp()
 *********************************************************************************************************************/
/*! \brief          Increments the count of dropped UDP packets.
 *  \details        -
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_GET_RESET_MEASUREMENT_DATA
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounterUdp(void);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounterType()
 *********************************************************************************************************************/
/*! \brief          Increments the count of dropped packets of the specified type.
 *  \details        -
 *  \param[in]      DroppedPacketCounterType  The counter type to be incremented.
 *                                            [range: DOIP_MEASURE_DROPPED_PACKET_COUNTER_*]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_GET_RESET_MEASUREMENT_DATA
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounterType(
  DoIP_Measure_DroppedPacketCounterType DroppedPacketCounterType);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounter()
 *********************************************************************************************************************/
/*! \brief          Increments the count of dropped TCP/UDP packets.
 *  \details        -
 *  \param[in]      DroppedPacketCounterType  The counter to be incremented.
 *                                            [range: DOIP_MEASURE_DROPPED_PACKET_COUNTER_*]
 *  \param[in]      HdrType                   The NACK Header type.
 *                                            [range: undefined]
 *  \param[in]      NackType                  The NACK type.
 *                                            [range: undefined]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_GET_RESET_MEASUREMENT_DATA
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounter(
  DoIP_Measure_DroppedPacketCounterType DroppedPacketCounterType,
  uint16 HdrType,
  uint8 NackType);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_GetAndResetDroppedPacketCounter()
 *********************************************************************************************************************/
/*! \brief          Gets and Resets(if requested) the dropped TCP/UDP packets counter.
 *  \details        Retrieve the dropped TCP/UDP packets counter value and if requested, reset the value.
 *  \param[in]      MeasurementIdx          The index to select specific measurement data.
 *                                          [range: undefined]
 *  \param[in]      MeasurementResetNeeded  Flag to indicate if the counter needs to be reset.
 *                                          [range: undefined]
 *  \param[in]      MeasurementDataPtr      Buffer where the value of the counter is to be copied into.
 *                                          [range: POINTER may be NULL_PTR]
 *  \pre            -
 *  \return         E_OK                    The operation was successful.
 *  \return         E_NOT_OK                The operation failed.
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         DOIP_GET_RESET_MEASUREMENT_DATA
 *********************************************************************************************************************/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Measure_GetAndResetDroppedPacketCounter(
  DoIP_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(DoIP_MeasurementDataType, AUTOMATIC, DOIP_APPL_VAR) MeasurementDataPtr);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  DoIP_Util_InitCons()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitCons(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionIterType connectionIdx;
  DoIP_TxUdpMgtListIterType txUdpMgtListIdx;
  DoIP_ConnectionTcpIterType connectionTcpIdx;
  DoIP_ConnectionUdpIterType connectionUdpIter;
  DoIP_ConnectionUdpVehAnIterType connectionUdpVehAnIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all connection data. */
  for ( connectionIdx = 0u;
        connectionIdx < DoIP_GetSizeOfConnection();
        connectionIdx++ )
  {
    DoIP_SetIpAddrStateOfConnectionDyn(connectionIdx, SOAD_IPADDR_STATE_UNASSIGNED);                                   /* SBSW_DOIP_CSL02 */
    DoIP_SetSoConIdOfConnectionDyn(connectionIdx, SOAD_INV_SO_CON_ID);                                                 /* SBSW_DOIP_CSL02 */
    DoIP_SetSoConStateOfConnectionDyn(connectionIdx, SOAD_SOCON_OFFLINE);                                              /* SBSW_DOIP_CSL02 */
    DoIP_SetSoConClosingOfConnectionDyn(connectionIdx, FALSE);                                                         /* SBSW_DOIP_CSL02 */
  }

  /* #20 Initialize all UDP management lists. */
  for ( txUdpMgtListIdx = 0u;
        txUdpMgtListIdx < DoIP_GetSizeOfTxUdpMgtList();
        txUdpMgtListIdx++ )
  {
    DoIP_SetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx, DoIP_GetSizeOfConnection());                                  /* SBSW_DOIP_CSL01 */
  }

  /* #30 Initialize all TCP connection data. */
  for ( connectionTcpIdx = 0u;
        connectionTcpIdx < DoIP_GetSizeOfConnectionTcp();
        connectionTcpIdx++ )
  {
    DoIP_TxTcpMgtIterType txTcpMgtIdx;

    DoIP_SetInactivCntOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                        /* SBSW_DOIP_CSL02 */
    DoIP_SetSkipRxMsgOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                                      /* SBSW_DOIP_CSL02 */
    DoIP_SetRxMsgOffsetOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                       /* SBSW_DOIP_CSL02 */
    DoIP_SetTxBytesCopiedOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                     /* SBSW_DOIP_CSL02 */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                     /* SBSW_DOIP_CSL02 */
    DoIP_SetLastTxBufSizeOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                     /* SBSW_DOIP_CSL02 */
    DoIP_SetChannelRxLastIdxOfConnectionTcpDyn(connectionTcpIdx, DoIP_GetSizeOfChannel());                             /* SBSW_DOIP_CSL02 */
    DoIP_SetChannelRxActiveOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                                /* SBSW_DOIP_CSL02 */
    DoIP_SetTxTcpMgtQueLvlOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                    /* SBSW_DOIP_CSL02 */
    DoIP_SetTxTcpMgtQueIdxOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                    /* SBSW_DOIP_CSL02 */
    DoIP_SetTxTpSessionActiveOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                              /* SBSW_DOIP_CSL02 */
    DoIP_SetRxTpSessionActiveOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                              /* SBSW_DOIP_CSL02 */
    DoIP_SetTpTxPreHdrTypeOfConnectionTcpDyn(connectionTcpIdx, DOIP_HDR_TYPE_INVALID);                                 /* SBSW_DOIP_CSL02 */
#if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
    DoIP_SetTxTpCancelRequestedOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                            /* SBSW_DOIP_CSL02 */
#endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */
#if ( DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON )
    DoIP_SetRxTpCancelRequestedOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                            /* SBSW_DOIP_CSL02 */
#endif /* DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON */
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
    DoIP_SetAliveCheckCntOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                     /* SBSW_DOIP_CSL02 */
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
    DoIP_RxTcpRout_RegisterTester((DoIP_SizeOfConnectionTcpType)connectionTcpIdx, DoIP_GetSizeOfTester(),
      DOIP_ADDR_ANY, DoIP_GetSizeOfRoutingActivation(), DOIP_ROUT_ACTIV_STATE_NONE);

    /* #40 Initialize all TCP management queues. */
    for ( txTcpMgtIdx = DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(connectionTcpIdx);
          txTcpMgtIdx < DoIP_GetTxTcpMgtEndIdxOfConnectionTcp(connectionTcpIdx);
          txTcpMgtIdx++ )
    {
      DoIP_SetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx, 0u);                                                           /* SBSW_DOIP_CSL02_CSL03 */
      DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, 0u);                                                               /* SBSW_DOIP_CSL02_CSL03 */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
      DoIP_SetOemSpecificOfTxTcpMgtDyn(txTcpMgtIdx, FALSE);                                                            /* SBSW_DOIP_CSL02_CSL03 */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
    }

#if ( DOIP_VERIFY_RX_PDU_ENABLED == STD_ON )
    {
      SoAd_SockAddrInetXType ipAddr = { SOAD_AF_INVALID, 0u, { 0u } };

      DoIP_SetRemIpAddrOfConnectionTcpDyn(connectionTcpIdx, ipAddr);                                                   /* SBSW_DOIP_CSL02 */
      DoIP_SetLocIpAddrOfConnectionTcpDyn(connectionTcpIdx, ipAddr);                                                   /* SBSW_DOIP_CSL02 */
    }
#endif /* DOIP_VERIFY_RX_PDU_ENABLED == STD_ON */
  }

  /* #50 Initialize all UDP connection data. */
  for ( connectionUdpIter = 0u;
        connectionUdpIter < DoIP_GetSizeOfConnectionUdp();
        connectionUdpIter++ )
  {
    DoIP_SetTxActiveOfConnectionUdpDyn(connectionUdpIter, FALSE);                                                      /* SBSW_DOIP_CSL02 */
  }

  /* #60 Initialize all UDP vehicle announcement connection data. */
  for ( connectionUdpVehAnIter = 0u;
        connectionUdpVehAnIter < DoIP_GetSizeOfConnectionUdpVehAn();
        connectionUdpVehAnIter++ )
  {
    DoIP_SetPendingVehAnOfConnectionUdpVehAnDyn(connectionUdpVehAnIter, FALSE);                                        /* SBSW_DOIP_CSL02 */
    DoIP_SetTxActiveOfConnectionUdpVehAnDyn(connectionUdpVehAnIter, FALSE);                                            /* SBSW_DOIP_CSL02 */
  }
} /* DoIP_Util_InitCons() */

/**********************************************************************************************************************
 *  DoIP_Util_InitChannels()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitChannels(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ChannelIterType channelIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all channel data. */
  for ( channelIdx = 0u;
        channelIdx < DoIP_GetSizeOfChannel();
        channelIdx++ )
  {
    DoIP_SetTxMsgLenOfChannelDyn(channelIdx, 0u);                                                                      /* SBSW_DOIP_CSL02 */
#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON )
    DoIP_SetEcuAddrOfChannelDyn(channelIdx, DoIP_GetEcuAddrOfChannel(channelIdx));                                     /* SBSW_DOIP_CSL02 */
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON */
  }
} /* DoIP_Util_InitChannels() */

/**********************************************************************************************************************
 *  DoIP_Util_InitTesters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitTesters(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_TesterIterType testerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all tester data. */
  for ( testerIdx = 0u;
        testerIdx < DoIP_GetSizeOfTester();
        testerIdx++ )
  {
    DoIP_SetConnectionTcpIdxOfTesterDyn(testerIdx, DoIP_GetSizeOfConnectionTcp());                                     /* SBSW_DOIP_CSL02 */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
    DoIP_SetDiagNAckOffsetOfTesterDyn(testerIdx, 0u);                                                                  /* SBSW_DOIP_CSL02 */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  }
} /* DoIP_Util_InitTesters() */

/**********************************************************************************************************************
 *  DoIP_Util_InitLocalAddrs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitLocalAddrs(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_IpAddrAssignmentIterType       ipAddrAssignmentIdx;
#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  const SoAd_SockAddrInetXType        ipAddr = { SOAD_AF_INVALID, 0u, { 0u } };
  DoIP_ConnectionUdpVehAnIterType     connectionUdpVehAnIter;
  DoIP_IpAddrListIterType             ipAddrListIdx;
  DoIP_SizeOfLocalAddrType            localAddrIdx;
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all IP address assignment states. */
  for ( ipAddrAssignmentIdx = 0u;
        ipAddrAssignmentIdx < DoIP_GetSizeOfIpAddrAssignment();
        ipAddrAssignmentIdx++ )
  {
    DoIP_SetIpAddrReqStateOfIpAddrAssignmentDyn(ipAddrAssignmentIdx, DOIP_IP_ADDR_REQ_STATE_NONE);                     /* SBSW_DOIP_CSL02 */
  }

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
  /* #20 Reset number of connections with assigned local addresses. */
  DoIP_LocalAddrAssignedCounter = 0u;
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  /* #30 Iterate over all DoIP Vehicle Announcement connections (if enabled). */
  for ( connectionUdpVehAnIter = 0u;
        connectionUdpVehAnIter < DoIP_GetSizeOfConnectionUdpVehAn();
        connectionUdpVehAnIter++ )
  {
    DoIP_ConnectionIdxOfConnectionUdpVehAnType connectionIdx =
      DoIP_GetConnectionIdxOfConnectionUdpVehAn(connectionUdpVehAnIter);

    /* #40 Reset DHCP vendor option IP address list for connection related local address. */
    localAddrIdx = DoIP_GetLocalAddrIdxOfConnection(connectionIdx);
    for ( ipAddrListIdx = DoIP_GetIpAddrListStartIdxOfLocalAddr(localAddrIdx);
          ipAddrListIdx < DoIP_GetIpAddrListEndIdxOfLocalAddr(localAddrIdx);
          ipAddrListIdx++ )
    {
      DoIP_SetTriggerVehAnnOfIpAddrList(ipAddrListIdx, FALSE);                                                         /* SBSW_DOIP_CSL03_CSL01 */
      DoIP_SetIpAddrOfIpAddrList(ipAddrListIdx, ipAddr);                                                               /* SBSW_DOIP_CSL03_CSL01 */
    }
  }
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */
} /* DoIP_Util_InitLocalAddrs() */

/**********************************************************************************************************************
 *  DoIP_Util_InitRoutActivHandlers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitRoutActivHandlers(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_AliveCheckListIterType aliveCheckListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all routing activation handler lists. */
  for ( aliveCheckListIdx = 0u;
        aliveCheckListIdx < DoIP_GetSizeOfAliveCheckList();
        aliveCheckListIdx++ )
  {
    DoIP_SetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx, DoIP_GetSizeOfConnectionTcp());                        /* SBSW_DOIP_CSL01 */
  }

  /* #20 Initialize routing activation handler data. */
  DoIP_SetOemSpecReqOfRoutActivHandler(0u, 0u);                                                                        /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetOemSpecResOfRoutActivHandler(0u, 0u);                                                                        /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetOemSpecUsedOfRoutActivHandler(0u, FALSE);                                                                    /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetRoutingActivationIdxOfRoutActivHandler(0u, DoIP_GetSizeOfRoutingActivation());                               /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetTesterAddrOfRoutActivHandler(0u, DOIP_ADDR_ANY);                                                             /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetTesterIdxOfRoutActivHandler(0u, DoIP_GetSizeOfTester());                                                     /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetAliveCheckCntOfRoutActivHandler(0u, 0u);                                                                     /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetAliveCheckElemNumOfRoutActivHandler(0u, 0u);                                                                 /* SBSW_DOIP_RoutActivHandler */
  DoIP_SetConnectionTcpIdxOfRoutActivHandler(0u, DoIP_GetSizeOfConnectionTcp());                                       /* SBSW_DOIP_RoutActivHandler */
} /* DoIP_Util_InitRoutActivHandlers() */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_InitOemPayloadTypes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitOemPayloadTypes(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionIterType connectionIter;
  DoIP_OemPayloadRxBufConfigIterType oemRxBufCfgIter;
  DoIP_OemPayloadTxBufConfigIterType oemTxBufCfgIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connection data. */
  for ( connectionIter = 0u;
        connectionIter < DoIP_GetSizeOfConnection();
        connectionIter++ )
  {
    /* #20 Check if an OEM specific configuration exists for the connection. */
    if ( DoIP_IsOemSpecificCfgUsedOfConnection(connectionIter) )
    {
      const DoIP_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIP_GetOemSpecificCfgIdxOfConnection(connectionIter);

      /* #30 Initialize the reserved buffer index. */
      DoIP_SetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx, DoIP_GetSizeOfOemPayloadRxBufConfig());   /* SBSW_DOIP_CSL02_CSL03 */
    }
  }

  /* #40 Iterate over all OEM specific Rx buffers to initialize them. */
  for ( oemRxBufCfgIter = 0u;
        oemRxBufCfgIter < DoIP_GetSizeOfOemPayloadRxBufConfig();
        oemRxBufCfgIter++ )
  {
    DoIP_RxTcpOem_ReleaseBuf((DoIP_SizeOfOemPayloadRxBufConfigType)oemRxBufCfgIter);
  }

  /* #50 Iterate over all OEM specific Tx buffers to initialize them. */
  for ( oemTxBufCfgIter = 0u;
        oemTxBufCfgIter < DoIP_GetSizeOfOemPayloadTxBufConfig();
        oemTxBufCfgIter++ )
  {
    DoIP_TxOem_ReleaseBuf((DoIP_SizeOfOemPayloadTxBufConfigType)oemTxBufCfgIter);
  }
} /* DoIP_Util_InitOemPayloadTypes() */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Util_InitInterfaces()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_InitInterfaces(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_InterfaceIterType interfaceIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all interfaces. */
  for ( interfaceIter = 0u;
        interfaceIter < DoIP_GetSizeOfInterface();
        interfaceIter++ )
  {
    /* #20 Initialize the activation line state. */
    DoIP_SetActLineActiveOfInterfaceDyn(interfaceIter, (boolean)!DoIP_IsActLineCtrlOfInterface(interfaceIter));        /* SBSW_DOIP_CSL02 */
    DoIP_SetActLineActiveRequestCounterOfInterfaceDyn(interfaceIter, 0u);                                              /* SBSW_DOIP_CSL02 */
    DoIP_SetActLineInactiveRequestCounterOfInterfaceDyn(interfaceIter, 0u);                                            /* SBSW_DOIP_CSL02 */
    DoIP_SetActLineInactiveTimeOfInterfaceDyn(interfaceIter, 0u);                                                      /* SBSW_DOIP_CSL02 */
  }

} /* DoIP_Util_InitInterfaces() */

#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_ValidateConfig()
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
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_ValidateConfig(
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_CONST) DoIPConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_CONST) tmpConfigPtr;
  DoIP_RoutingActivationIterType routingActivationIndex;
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize configuration pointer temporary to enable the following checks. */
  tmpConfigPtr = DoIP_ConfigDataPtr;
  DoIP_ConfigDataPtr = DoIPConfigPtr;

  /* #20 Iterate over all configured routing activations to check the configuration of the OEM specific part. */
  for ( routingActivationIndex = 0u;
        routingActivationIndex < DoIP_GetSizeOfRoutingActivation();
        routingActivationIndex++ )
  {
    /* #30 Mark config as invalid if the confirmation length for the request exceeds the maximum size. */
    if ( DoIP_GetConfReqLengthOfRoutingActivation(routingActivationIndex) > DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE )
    {
      retVal = E_NOT_OK;
    }
    /* #40 Mark config as invalid if the confirmation length for the response exceeds the maximum size otherwise. */
    else if ( DoIP_GetConfResLengthOfRoutingActivation(routingActivationIndex) > DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE )
    {
      retVal = E_NOT_OK;
    }
    /* #50 Assume that OEM specific part is configured correctly otherwise. */
    else
    {
      /* Nothing to do. */
    }

    /* #60 Stop iteration if at least one OEM specific part is configured invalidly. */
    if ( retVal != E_OK )
    {
      break;
    }
  }

  /* #70 Revert configuration pointer. */
  DoIP_ConfigDataPtr = tmpConfigPtr;

  return retVal;
} /* DoIP_Util_ValidateConfig() */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Util_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_Init(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the module data structures. */
  /* PRQA S 2987 9 */ /* MD_DoIP_Rule2.2_2987 */
  DoIP_Util_InitCons();
  DoIP_Util_InitChannels();
  DoIP_Util_InitTesters();
  DoIP_Util_InitLocalAddrs();
  DoIP_Util_InitRoutActivHandlers();
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
  DoIP_Util_InitOemPayloadTypes();
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
  DoIP_Util_InitInterfaces();
} /* DoIP_Util_Init() */

/**********************************************************************************************************************
 *  DoIP_Util_FillGenHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGenHdr(
  uint16 HdrTypeField,
  uint32 HdrLenField,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write data to target buffer. */
  /* header version */
  TgtBufferPtr[0u] = DOIP_PROTOCOL_VERSION;                                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  /* inverse header version */
  TgtBufferPtr[1u] = DOIP_INVERSE_PROTOCOL_VERSION;                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  /* header type */
  DoIP_Util_PutUInt16(&TgtBufferPtr[2u], 0u, HdrTypeField);                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  /* header length */
  DoIP_Util_PutUInt32(&TgtBufferPtr[4u], 0u, HdrLenField);                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
} /* DoIP_Util_FillGenHdr() */

/**********************************************************************************************************************
 *  DoIP_Util_FillVin()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillVin(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 vinBuffer[DOIP_VIN_LEN_BYTE] = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy VIN to target buffer if VIN retrieved successfully. */
  if ( DoIP_CallGetVinDirect(vinBuffer) == E_OK )                                                                      /* SBSW_DOIP_FUNCTION_POINTER_CALL */
  {
    /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)&TgtBufferPtr[0u], DOIP_A_P2CONST(void)vinBuffer, DOIP_VIN_LEN_BYTE);             /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */

    retVal = E_OK;
  }
  else
  {
    uint8_least tgtBufferIdx;

    /* #20 Otherwise write invalid pattern to target buffer. */
    for ( tgtBufferIdx = 0u;
          tgtBufferIdx < DOIP_VIN_LEN_BYTE;
          tgtBufferIdx++ )
    {
      TgtBufferPtr[tgtBufferIdx] = DOIP_VIN_INV_PATTERN;                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    }

    retVal = E_NOT_OK;
  }

  return retVal;
} /* DoIP_Util_FillVin() */

/**********************************************************************************************************************
 *  DoIP_Util_FillLocalLogAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillLocalLogAddr(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write logical address to target buffer. */
  DoIP_Util_PutUInt16(TgtBufferPtr, 0u, DoIP_Util_GetEntityAddr());                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
} /* DoIP_Util_FillLocalLogAddr() */

/**********************************************************************************************************************
 *  DoIP_Util_FillEid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillEid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 physAddrBuf[DOIP_EID_LEN_BYTE];

  /* ----- Implementation ----------------------------------------------- */

#if ( DOIP_EID == STD_ON )
  /* #10 Check if configured EID shall be used (if EID is available). */
  /* Use negative check for 'IsUseMacAddr..' for better readability with PreProcessor. */
  if ( !DoIP_IsUseMacAddrForVehIdentOfInterface(DoIP_GetInterfaceIdxOfConnection(ConnectionIdx)) )
  {
    /* #20 Copy the configured EID to the target buffer. */
    /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)&TgtBufferPtr[0u], DOIP_A_P2CONST(void)DoIP_GetAddrEId(0u), DOIP_EID_LEN_BYTE);   /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
    retVal = E_OK;
  }
  /* #30 Otherwise, use the MAC-Address as EID. */
  else
#endif /* DOIP_EID == STD_ON */
  {
    /* #40 Try to get the MAC-Address via SoAd_GetPhysAddr(). */
    if ( SoAd_GetPhysAddr(DoIP_GetSoConIdOfConnectionDyn(ConnectionIdx), &physAddrBuf[0u]) == E_OK )                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    {
      /* #50 Copy the MAC-Address to the target buffer. */
      /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
      VStdLib_MemCpy(DOIP_A_P2VAR(void)&TgtBufferPtr[0u], DOIP_A_P2CONST(void)&physAddrBuf[0u], DOIP_EID_LEN_BYTE);    /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
      retVal = E_OK;
    }
  }

  return retVal;
} /* DoIP_Util_FillEid() */

/**********************************************************************************************************************
 *  DoIP_Util_FillGid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_FillGid(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( (DOIP_VIN_GID_MASTER == STD_ON ) && (DOIP_USE_EID_AS_GID == STD_ON) )
  /* #10 Write EID to target buffer (if EID as GID enabled). */
  retVal = DoIP_Util_FillEid(TgtBufferPtr, ConnectionIdx);                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
#else
  /* #20 Write GID or invalid pattern to target buffer (if enabled). */
  DoIP_Util_FillGidOrInv(TgtBufferPtr);                                                                                /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

  DOIP_DUMMY_STATEMENT(ConnectionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  retVal = E_OK;
#endif /* (DOIP_VIN_GID_MASTER == STD_ON ) && (DOIP_USE_EID_AS_GID == STD_ON) */

  return retVal;
} /* DoIP_Util_FillGid() */

#if ( !((DOIP_VIN_GID_MASTER == STD_ON ) && (DOIP_USE_EID_AS_GID == STD_ON)) )
/**********************************************************************************************************************
 *  DoIP_Util_FillGidOrInv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillGidOrInv(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
# if ( DOIP_GID_AVAILABLE == STD_ON )
  /* #10 Write configured GID to target buffer (if enabled). */
  /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
  VStdLib_MemCpy(DOIP_A_P2VAR(void)&TgtBufferPtr[0u], DOIP_A_P2CONST(void)DoIP_GetAddrGId(0u), DOIP_GID_LEN_BYTE);     /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
# else
  uint8 gidBuffer[DOIP_GID_LEN_BYTE] = { 0u };

  if ( DoIP_CallGetGidDirect(gidBuffer) == E_OK )                                                                      /* SBSW_DOIP_FUNCTION_POINTER_CALL */
  {
    /* #20 Copy GID to target buffer if GID retieved sucessfully. */
    /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)&TgtBufferPtr[0u], DOIP_A_P2CONST(void)gidBuffer, DOIP_GID_LEN_BYTE);             /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
  }
  else
  {
    uint8_least tgtBufferIdx;

    /* #30 Otherwise write invalid pattern to target buffer. */
    for ( tgtBufferIdx = 0u;
          tgtBufferIdx < DOIP_GID_LEN_BYTE;
          tgtBufferIdx++ )
    {
      TgtBufferPtr[tgtBufferIdx] = DOIP_GID_INV_PATTERN;                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    }
  }
# endif /* DOIP_GID_ENABLED == STD_ON */
} /* DoIP_Util_FillGidOrInv() */
#endif /* !((DOIP_VIN_GID_MASTER == STD_ON ) && (DOIP_USE_EID_AS_GID == STD_ON)) */

/**********************************************************************************************************************
 *  DoIP_Util_FillFurtherAction()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Util_FillFurtherAction(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) TgtBufferPtr,
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_TESTER_WITH_CEN_SEC_AVAILABLE == STD_ON )
  DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(ConnectionIdx);
#endif /* DOIP_TESTER_WITH_CEN_SEC_AVAILABLE == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set further action byte to 0x00u. */
  TgtBufferPtr[0u] = 0x00u;                                                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#if ( DOIP_TESTER_WITH_CEN_SEC_AVAILABLE == STD_ON )
  {
    DoIP_TesterIterType testerIdx;

    for ( testerIdx = DoIP_GetTesterStartIdxOfInterface(interfaceIdx);
          testerIdx < DoIP_GetTesterEndIdxOfInterface(interfaceIdx);
          testerIdx++ )
    {
      /* #20 Iterate over all testers with central security (if enabled). */
      if ( DoIP_IsCentralSecurityOfTester(testerIdx) )
      {
        boolean furtherActionRequired = FALSE;
        const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfTesterDyn(testerIdx);

        /* #30 Check if tester is unconnected. */
        if ( connectionTcpIdx == DoIP_GetSizeOfConnectionTcp() )
        {
          /* #40 Set further action byte to 0x10u. */
          furtherActionRequired = TRUE;
        }
        /* #50 Otherwise check if tester is not connected to routing activation number 0xE0u. */
        else
        {
          if ( DoIP_GetRoutingActivationNumOfRoutingActivation(
                 DoIP_GetRoutingActivationIdxOfConnectionTcpDyn(connectionTcpIdx)) != 0xE0u )
          {
            /* #60 Set further action byte to 0x10u. */
            furtherActionRequired = TRUE;
          }
        }

        if ( furtherActionRequired == TRUE )
        {
          TgtBufferPtr[0u] = 0x10u;                                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
          break;
        }
      }
    }
  }
#else
  DOIP_DUMMY_STATEMENT(ConnectionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_TESTER_WITH_CEN_SEC_AVAILABLE == STD_ON */
} /* DoIP_Util_FillFurtherAction() */

/**********************************************************************************************************************
 *  DoIP_Util_PutUInt16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_Util_PutUInt16(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) Buffer,
  uint32 Offset,
  uint16 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Put value into buffer. */
  (Buffer)[(Offset)     ] = (uint8)((uint16_least)(Value) >> 8u);                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  (Buffer)[(Offset) + 1u] = (uint8)((uint16_least)(Value));                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_Util_PutUInt16() */

/**********************************************************************************************************************
 *  DoIP_Util_PutUInt32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_Util_PutUInt32(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) Buffer,
  uint32 Offset,
  uint32 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Put value into buffer. */
  (Buffer)[(Offset)     ] = (uint8)((uint32_least)(Value) >> 24u);                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  (Buffer)[(Offset) + 1u] = (uint8)((uint32_least)(Value) >> 16u);                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  (Buffer)[(Offset) + 2u] = (uint8)((uint32_least)(Value) >> 8u);                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  (Buffer)[(Offset) + 3u] = (uint8)((uint32_least)(Value));                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_Util_PutUInt32() */

/**********************************************************************************************************************
 *  DoIP_Util_CheckBufferSizeAndModify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_Util_CheckBufferSizeAndModify(
  uint32 BufferSize,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if BufferSize exceeds PduLengthType size. */
#if ( DOIP_MAX_PDU_LENGTH != 0xFFFFFFFFUL )
  if ( BufferSize > DOIP_MAX_PDU_LENGTH )
  {
    /* #20 Use max PduLengthType size. */
    *BufferSizePtr = (PduLengthType)DOIP_MAX_PDU_LENGTH;                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
  else
#endif /* DOIP_MAX_PDU_LENGTH != 0xFFFFFFFFUL */
  {
    /* #30 Use BufferSize size otherwise. */
    *BufferSizePtr = (PduLengthType)BufferSize;                                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
} /* DoIP_Util_CheckBufferSizeAndModify() */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
# if ( DOIP_IPV6_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_CheckIpV6Addr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_CheckIpV6Addr(
  SoAd_SockAddrInetXType IpAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the IPv6 address is unspecified. */
  if ( DoIP_Util_CompareIpAddr(SOAD_AF_INET6, DOIP_A_P2CONST(uint32)(IpAddr.addr),                                     /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
        DOIP_A_P2CONST(uint32)(DoIP_IpV6AddrUnspecified.addr)) == FALSE )
  {
    /* #20 Check if the IPv6 address is the loopback address. */
    if ( DoIP_Util_CompareIpAddr(SOAD_AF_INET6, DOIP_A_P2CONST(uint32)(IpAddr.addr),                                   /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
          DOIP_A_P2CONST(uint32)(DoIP_IpV6AddrLoopback.addr)) == FALSE )
    {
      /* #30 Check if the IPv6 address is the multicast address. */
      if ( DoIP_Util_CompareIpAddr(SOAD_AF_INET6, DOIP_A_P2CONST(uint32)(IpAddr.addr),                                 /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
            DOIP_A_P2CONST(uint32)(DoIP_IpV6AddrMulticast.addr)) == FALSE )
      {
        retVal = E_OK;
      }
    }
  }
  return retVal;
} /* DoIP_Util_CheckIpV6Addr() */
# endif /* DOIP_IPV6_ENABLED == STD_ON */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
# if ( DOIP_IPV4_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_CheckIpV4Addr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_Util_CheckIpV4Addr(
  SoAd_SockAddrInetXType IpAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint32 ipAddrMasked = IpAddr.addr[0] & DOIP_8_BIT_NET_MASK;

  /* ----- Implementation ----------------------------------------------- */
  /*  #10 Check if the IPv4 address is unspecified. */
  if ( DoIP_Util_CompareIpAddr(SOAD_AF_INET, DOIP_A_P2CONST(uint32)(&ipAddrMasked),                                    /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
        DOIP_A_P2CONST(uint32)(DoIP_IpV4AddrUnspecifiedNetmask.addr)) == FALSE )
  {
    /* #20 Check if the IPv4 address is the loopback address. */
    if ( DoIP_Util_CompareIpAddr(SOAD_AF_INET, DOIP_A_P2CONST(uint32)(&ipAddrMasked),                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
          DOIP_A_P2CONST(uint32)(DoIP_IpV4AddrLoopbackNetmask.addr)) == FALSE )
    {
      /* #30 Check if the IPv4 address is the broadcast address. */
      if ( DoIP_Util_CompareIpAddr(SOAD_AF_INET, DOIP_A_P2CONST(uint32)IpAddr.addr,                                    /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
            DOIP_A_P2CONST(uint32)(DoIP_IpV4AddrBroadcast.addr)) == FALSE )
      {
        retVal = E_OK;
      }
    }
  }
  return retVal;
} /* DoIP_Util_CheckIpV4Addr() */
# endif /* DOIP_IPV4_ENABLED == STD_ON */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Util_CompareIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_DoIP_3206 */
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_CompareIpAddr(
  SoAd_DomainType Domain,
  P2CONST(uint32, AUTOMATIC, DOIP_APPL_VAR) IpAddrAPtr,
  P2CONST(uint32, AUTOMATIC, DOIP_APPL_VAR) IpAddrBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isMatching;

  /* ----- Implementation ----------------------------------------------- */
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  /* #10 Check if it is an IPv6 IP address (if enabled). */
  if ( Domain == SOAD_AF_INET6 )
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
  {
    /* #20 Compare both IPv6 addresses. */
    if ( (IpAddrAPtr[0u] == IpAddrBPtr[0u]) &&
         (IpAddrAPtr[1u] == IpAddrBPtr[1u]) &&
         (IpAddrAPtr[2u] == IpAddrBPtr[2u]) &&
         (IpAddrAPtr[3u] == IpAddrBPtr[3u]) )
    {
      isMatching = TRUE;
    }
    else
    {
      isMatching = FALSE;
    }
  }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  else
#  else
  DOIP_DUMMY_STATEMENT(Domain); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
# endif /* DOIP_IPV6_ENABLED == STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
  {
    /* #30 Compare both IPv4 addresses. */
    if ( IpAddrAPtr[0u] == IpAddrBPtr[0u] )
    {
      isMatching = TRUE;
    }
    else
    {
      isMatching = FALSE;
    }
  }
#  if ( DOIP_IPV6_ENABLED == STD_OFF )
  DOIP_DUMMY_STATEMENT(Domain); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV6_ENABLED == STD_OFF */
# endif /* DOIP_IPV4_ENABLED == STD_ON */

  return isMatching;
} /* DoIP_Util_CompareIpAddr() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Util_GetEcuAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(uint16, DOIP_CODE) DoIP_Util_GetEcuAddr(
  DoIP_ChannelIterType ChannelIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 ecuAddr;

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON )
  /* #10 Return the logical ECU address stored in RAM (if OverwriteLogicalTargetAddr API is enabled). */
  ecuAddr = DoIP_GetEcuAddrOfChannelDyn(ChannelIdx);
#else
  /* #20 Return the logical ECU address stored in ROM (if OverwriteLogicalTargetAddr API is disabled). */
  ecuAddr = DoIP_GetEcuAddrOfChannel(ChannelIdx);
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON */

  return ecuAddr;
} /* DoIP_Util_GetEcuAddr() */

/**********************************************************************************************************************
 *  DoIP_Util_GetEntityAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(uint16, DOIP_CODE) DoIP_Util_GetEntityAddr(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 entityAddr;

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON )
  /* #10 Return the entity address stored in RAM (if OverwriteLogicalTargetAddr API is enabled). */
  entityAddr = DoIP_GetEntityAddr();
#else
  /* #20 Return the entity address provided as constant macro (if OverwriteLogicalTargetAddr API is disabled). */
  entityAddr = DoIP_GetLogicalAddr(0u);
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON */

  return entityAddr;
} /* DoIP_Util_GetEntityAddr() */

/**********************************************************************************************************************
 *  DoIP_Util_IsTcpConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_IsTcpConnection(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* #10 Return TRUE when connection type of the connection is DOIP_CONNECTIONTCP_CONNECTIONTYPEOFCONNECTION.*/
  /* #20 Otherwise, return FALSE.*/
  return (boolean)(DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == DOIP_CONNECTIONTCP_CONNECTIONTYPEOFCONNECTION);
} /* DoIP_Util_IsTcpConnection() */

/**********************************************************************************************************************
 *  DoIP_Util_IsUdpConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_IsUdpConnection(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* #10 Return TRUE when connection type of the connection is DOIP_CONNECTIONUDP_CONNECTIONTYPEOFCONNECTION.*/
  /* #20 Otherwise, return FALSE.*/
  return (boolean)(DoIP_GetConnectionTypeOfConnection(ConnectionIdx) == DOIP_CONNECTIONUDP_CONNECTIONTYPEOFCONNECTION);
} /* DoIP_Util_IsUdpConnection() */

/**********************************************************************************************************************
 *  DoIP_Util_IsUdpVehAnConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_Util_IsUdpVehAnConnection(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* #10 Return TRUE when connection type of the connection is DOIP_CONNECTIONUDPVEHAN_CONNECTIONTYPEOFCONNECTION.*/
  /* #20 Otherwise, return FALSE.*/
  return (boolean)(DoIP_GetConnectionTypeOfConnection(ConnectionIdx) ==
                   DOIP_CONNECTIONUDPVEHAN_CONNECTIONTYPEOFCONNECTION);
} /* DoIP_Util_IsUdpVehAnConnection() */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_GetLocalAddrIdxOfDhcpAssignmentByLocalIpAddrId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_GetLocalAddrIdxOfDhcpAssignmentByLocalIpAddrId(
  SoAd_LocalAddrIdType LocalIpAddrId,
  P2VAR(DoIP_SizeOfLocalAddrType, AUTOMATIC, DOIP_APPL_VAR) LocalAddrIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_LocalAddrIterType localAddrIdx;
  DoIP_IpAddrAssignmentIterType ipAddrAssignmentIdx;

  /* ----- Implementation ----------------------------------------------- */
  *LocalAddrIdxPtr = DoIP_GetSizeOfLocalAddr();                                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

  /* #10 Iterate over all local IP addresses. */
  for ( localAddrIdx = 0u;
        localAddrIdx < DoIP_GetSizeOfLocalAddr();
        localAddrIdx++ )
  {
    if ( DoIP_GetTcpIpLocalAddrIdOfLocalAddr(localAddrIdx) == LocalIpAddrId )
    {
      /* #20 Iterate over all related IP address assignments. */
      for ( ipAddrAssignmentIdx = DoIP_GetIpAddrAssignmentStartIdxOfLocalAddr(localAddrIdx);
            ipAddrAssignmentIdx < DoIP_GetIpAddrAssignmentEndIdxOfLocalAddr(localAddrIdx);
            ipAddrAssignmentIdx++ )
      {
        if ( DoIP_GetAssignmentTypeOfIpAddrAssignment(ipAddrAssignmentIdx) == DOIP_IPADDR_ASSIGN_TYPE_DHCP )
        {
          /* #30 Return found DoIP index if address has DHCP assignment. */
          *LocalAddrIdxPtr = (DoIP_SizeOfLocalAddrType)localAddrIdx;                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
          retVal = E_OK;
          break;
        }
      }
    }
    if ( retVal == E_OK )
    {
      break;
    }
  }

  return retVal;
} /* DoIP_Dhcp_GetLocalAddrIdxOfDhcpAssignmentByLocalIpAddrId() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_PrepareVendorClassTxBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_PrepareVendorClassTxBuf(
  uint16 OptionCode,
  P2VAR(uint16, AUTOMATIC, DOIP_APPL_VAR) OptionBufferLengthPtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 venEnterpriseNum = DOIP_DHCPVX_OPT_VENDOR_ENT_NUM_3210;

  /* ----- Implementation ----------------------------------------------- */
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  if ( OptionCode == DOIP_DHCPV6_OPT_VENDOR_CLASS_OPT_CODE )
#  endif /* DOIP_IPV4_ENABLED = STD_ON */
  {
    /* #10 Fill the option buffer with the data in case of DHCPv6 option code (if enabled). */
    *OptionBufferLengthPtr = DOIP_DHCPV6_OPT_VENDOR_CLASS_MIN_LEN;                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[0u] = (uint8)(0u);                                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[1u] = (uint8)(0u);                                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[2u] = (uint8)(venEnterpriseNum >> 8u);                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[3u] = (uint8)(venEnterpriseNum);                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  else
#  else
  DOIP_DUMMY_STATEMENT(OptionCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV4_ENABLED = STD_ON */
# endif /* DOIP_IPV6_ENABLED = STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
  {
    /* #20 Fill the option buffer with the data in case of DHCPv4 option code (if enabled). */
    *OptionBufferLengthPtr = DOIP_DHCPV4_OPT_VENDOR_CLASS_MIN_LEN;                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[0u] = (uint8)(0u);                                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[1u] = (uint8)(0u);                                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[2u] = (uint8)(venEnterpriseNum >> 8u);                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[3u] = (uint8)(venEnterpriseNum);                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    OptionBufferPtr[4u] = (uint8)(0u); /* Data Length = 0 */                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
#  if ( DOIP_IPV6_ENABLED == STD_OFF )
  DOIP_DUMMY_STATEMENT(OptionCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV6_ENABLED = STD_OFF */
# endif /* DOIP_IPV4_ENABLED = STD_ON */
} /* DoIP_Dhcp_PrepareVendorClassTxBuf() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_HandleVendorOptsOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_HandleVendorOptsOption(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Validate the received vendor option buffer data. */
  if ( DoIP_Dhcp_ValidateVendorOptsRxBuf(OptionCode, OptionBufferLength, OptionBufferPtr) == E_OK )                    /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    /* #20 Process the IP address list received from DHCP. */
    DoIP_Dhcp_HandleIpAddrListFromVendorOpts(LocalAddrIdx, OptionCode, OptionBufferLength, OptionBufferPtr);           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }
} /* DoIP_Util_HandleDhcpVendorOptsOption() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ValidateVendorOptsRxBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_ValidateVendorOptsRxBuf(
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint32         enterpriseNumber;
  uint16         optionDataCode;
  uint16         optionDataLen;
  uint16         bufIdx;
# if ( DOIP_IPV4_ENABLED == STD_ON )
  uint8          optionDataLenAll;
# endif /* DOIP_IPV4_ENABLED = STD_ON */

  /* ----- Implementation ----------------------------------------------- */
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  if ( OptionCode == DOIP_DHCPV6_OPT_VENDOR_OPTS_OPT_CODE )
#  endif /* DOIP_IPV4_ENABLED = STD_ON */
  {
    /* Option code is DHCPv6 type. */
    if ( OptionBufferLength >= (DOIP_DHCPV6_OPT_VENDOR_OPTS_MIN_LEN) )
    {
      /* #10 Read the buffer data if the buffer length meets the minimum value for DHCPv6 vendor option
       *     (if enabled). */
      bufIdx = 0u;
      IPBASE_GET_UINT32(OptionBufferPtr, bufIdx, enterpriseNumber);                                                    /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      bufIdx += 4u;
      /* enterprise specific data within the 'sub-option' */
      IPBASE_GET_UINT16(OptionBufferPtr, bufIdx, optionDataCode);                                                      /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      bufIdx += 2u;
      /* enterprise specific data within the 'sub-option' */
      IPBASE_GET_UINT16(OptionBufferPtr, bufIdx, optionDataLen);                                                       /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

      if ( /* Enterprise Number matches expected */
           (enterpriseNumber == DOIP_DHCPVX_OPT_VENDOR_ENT_NUM_3210) &&
           /* DHCP Vendor Option - 'sub-option' code matches DOIP 'sub-option' code regarding enterprise number */
           (optionDataCode == DOIP_DHCPVX_OPT_VENDOR_SUB_CODE) &&
           /* not more than 4 addresses */
           (optionDataLen <= (DOIP_DHCPV6_OPT_ADDR_SIZE * DOIP_DHCPVX_OPT_VENDOR_TABLE_SIZE)) &&
           /* at least one address */
           (optionDataLen >= (DOIP_DHCPV6_OPT_ADDR_SIZE)) &&
           /* no 'half' ip address is received */
           ((optionDataLen % DOIP_DHCPV6_OPT_ADDR_SIZE) == 0u) &&
           /* general length check of 'sub-option' length field to received overall length */
           /* 8u = enterNum (4u) within the option + code (2u) within the 'sub-option' + len (2u) within the
            * 'sub-option' */
           (optionDataLen == (OptionBufferLength - 8u)) )
      {
        /* #20 Return E_OK if the data read agrees to the DHCPv6 vendor option format. */
        retVal = E_OK;
      }
    }
  }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  else
#  else
  DOIP_DUMMY_STATEMENT(OptionCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV4_ENABLED = STD_ON */
# endif /* DOIP_IPV6_ENABLED = STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
  {
    /* Option code is DHCPv4 type. */
    if ( OptionBufferLength >= (DOIP_DHCPV4_OPT_VENDOR_OPTS_MIN_LEN) )
    {
      /* #30 Read the buffer data if the buffer length meets the minimum value for DHCPv4 vendor option
       *     (if enabled). */
      bufIdx = 0u;
      IPBASE_GET_UINT32(OptionBufferPtr, bufIdx, enterpriseNumber);                                                    /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      bufIdx += 4u;
      IPBASE_GET_UINT8(OptionBufferPtr, bufIdx, optionDataLenAll);                                                     /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      bufIdx += 1u;
      /* enterprise specific data within the 'sub-option' */
      IPBASE_GET_UINT8(OptionBufferPtr, bufIdx, optionDataCode);                                                       /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      bufIdx += 1u;
      /* enterprise specific data within the 'sub-option' */
      IPBASE_GET_UINT8(OptionBufferPtr, bufIdx, optionDataLen);                                                        /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

      if ( /* Enterprise Number matches expected */
           (enterpriseNumber == DOIP_DHCPVX_OPT_VENDOR_ENT_NUM_3210) &&
           /* DHCP Vendor Option - 'sub-option' code matches DOIP 'sub-option' code regarding enterprise number */
           (optionDataCode == DOIP_DHCPVX_OPT_VENDOR_SUB_CODE) &&
           /* not more than 4 addresses */
           (optionDataLen <= (DOIP_DHCPV4_OPT_ADDR_SIZE * DOIP_DHCPVX_OPT_VENDOR_TABLE_SIZE)) &&
           /* at least one address */
           (optionDataLen >= (DOIP_DHCPV4_OPT_ADDR_SIZE)) &&
           /* no 'half' ip address is received */
           ((optionDataLen % DOIP_DHCPV4_OPT_ADDR_SIZE) == 0u) &&
           /* vendor 'sub-option data' length is consistent */
           /* 2u = code (1u) within the 'sub-option' + len (1u) within the 'sub-option' */
           ((optionDataLen + 2u) == optionDataLenAll) &&
           /* general length check of 'sub-option' length field to received overall length */
           /* 5u = enterNum (4u) within the option + dataLen (1u) within the option */
           (optionDataLenAll == (OptionBufferLength - 5u)) )
      {
        /* #40 Return E_OK if the data read agrees to the DHCPv4 vendor option format. */
        retVal = E_OK;
      }
    }
  }
#  if ( DOIP_IPV6_ENABLED == STD_OFF )
  DOIP_DUMMY_STATEMENT(OptionCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV6_ENABLED = STD_OFF */
# endif /* DOIP_IPV4_ENABLED = STD_ON */

  return retVal;
} /* DoIP_Util_ValidateDhcpVendorOptsRxBuf() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_HandleIpAddrListFromVendorOpts()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_HandleIpAddrListFromVendorOpts(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_DhcpIpAddrDynType venOptIpAddrDynList[DOIP_DHCP_IPADDRESS_LIST_MAX_SIZE];
  DoIP_DhcpIpAddrIterType venOptsAddrListIter;

  /* ----- Implementation ----------------------------------------------- */
  for ( venOptsAddrListIter = 0u;
        venOptsAddrListIter < DOIP_DHCP_IPADDRESS_LIST_MAX_SIZE;
        venOptsAddrListIter++ )
  {
    /* Initialize the list with invalid IP address type. */
    venOptIpAddrDynList[venOptsAddrListIter].DhcpIpAddr.domain = SOAD_AF_INVALID;                                      /* SBSW_DOIP_ARRAY_WRITE */
  }

  /* #10 Check if IP addresses supplied from DHCP can be read and filtered. */
  if ( DoIP_Dhcp_ReadAndFilterIpAddresses(OptionCode, OptionBufferLength, OptionBufferPtr,                             /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
        &venOptIpAddrDynList[0]) == E_OK )
  {
    /* #20 Compare the received IP addresses to the locally stored IP addresses. */
    DoIP_Dhcp_CompareToLocalIpAddrList(LocalAddrIdx, &venOptIpAddrDynList[0]);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

    /* #30 Update the local list with the new set of IP addresses. */
    DoIP_Dhcp_UpdateLocalIpAddrList(LocalAddrIdx, &venOptIpAddrDynList[0]);                                            /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  }
} /* DoIP_Util_HandleIpAddrListFromDhcpVendorOpts() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ReadAndFilterIpAddresses()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 5 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_ReadAndFilterIpAddresses(
  uint16 OptionCode,
  uint16 OptionBufferLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_VAR) OptionBufferPtr,
  P2VAR(DoIP_DhcpIpAddrDynType, AUTOMATIC, DOIP_APPL_VAR) VenOptIpAddrDynListPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType          retVal = E_NOT_OK;
  SoAd_SockAddrInetXType  ipAddr = { SOAD_AF_INVALID, 0u, { 0u } };
  SoAd_SockAddrInetXType  venOptIpAddr = { SOAD_AF_INVALID, 0u, { 0u } };
  DoIP_DhcpIpAddrIterType venOptsAddrListIter = 0u;
  DoIP_DhcpIpAddrIterType ipAddrListIter;
  uint16                  dynAddrOffset;
  uint16                  addrSize;
  uint16                  staticAddrOffset;
  uint16                  optionDataLen;
  boolean                 isAddrRepeated;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize the DOIP port number - 13400. */
  ipAddr.port = IPBASE_HTON16(DOIP_PORT_NUMBER);

# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  if ( OptionCode == DOIP_DHCPV6_OPT_VENDOR_OPTS_OPT_CODE )
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
  {
    /* The optionDataLen field within the buffer was validated by DoIP_Dhcp_ValidateVendorOptsRxBuf() before. */
    IPBASE_GET_UINT16(OptionBufferPtr, DOIP_DHCPV6_OPT_VENDOR_OPTS_SUB_OPT_LEN_OFFSET, optionDataLen);                 /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    staticAddrOffset = DOIP_DHCPV6_OPT_VENDOR_OPTS_ADDR_OFFSET;
    addrSize = DOIP_DHCPV6_OPT_ADDR_SIZE;
    venOptIpAddr.domain = SOAD_AF_INET6;
    ipAddr.domain = SOAD_AF_INET6;
  }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  else
#  else
  DOIP_DUMMY_STATEMENT(OptionCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
# endif /* DOIP_IPV6_ENABLED == STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
  {
    /* The optionDataLen field within the buffer was validated by DoIP_Dhcp_ValidateVendorOptsRxBuf() before. */
    IPBASE_GET_UINT8(OptionBufferPtr, DOIP_DHCPV4_OPT_VENDOR_OPTS_SUB_OPT_LEN_OFFSET, optionDataLen);                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    staticAddrOffset = DOIP_DHCPV4_OPT_VENDOR_OPTS_ADDR_OFFSET;
    addrSize = DOIP_DHCPV4_OPT_ADDR_SIZE;
    venOptIpAddr.domain = SOAD_AF_INET;
    ipAddr.domain = SOAD_AF_INET;
  }
#  if ( DOIP_IPV6_ENABLED == STD_OFF )
  DOIP_DUMMY_STATEMENT(OptionCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV6_ENABLED = STD_OFF */
# endif /* DOIP_IPV4_ENABLED == STD_ON */

  /* #10 Iterate over the option buffer till the given buffer length, incrementing each time by the IP address size. */
  for ( dynAddrOffset = 0u;
        dynAddrOffset < optionDataLen;
        dynAddrOffset += addrSize )
  {
    /* Initialize the Vendor Options Dynamic IP Address List. */
    VenOptIpAddrDynListPtr[venOptsAddrListIter].DhcpIpAddr.port = ipAddr.port;                                         /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    VenOptIpAddrDynListPtr[venOptsAddrListIter].DhcpIpAddr.domain = ipAddr.domain;                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

    /* Retrieve the IP addresses from the buffer and store them in the temporary list. */
    /* PRQA S 0314 2 */ /* MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)venOptIpAddr.addr,                                                                /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
      DOIP_A_P2CONST(void)&OptionBufferPtr[staticAddrOffset + dynAddrOffset], addrSize);

    /* #20 Check if new IP Address provided by DHCP is valid. */
    if ( DoIP_Dhcp_ValidateIpAddress(venOptIpAddr) == E_OK )
    {
      /* Reset the flag for new the new address. */
      isAddrRepeated = FALSE;

      /* #30 Check if the IP address repeats in the DHCP supplied list. */
      for ( ipAddrListIter = 0u;
            ipAddrListIter < venOptsAddrListIter;
            ipAddrListIter++ )
      {
        if ( DoIP_Util_CompareIpAddr(ipAddr.domain, VenOptIpAddrDynListPtr[ipAddrListIter].DhcpIpAddr.addr,            /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
              venOptIpAddr.addr) == TRUE )
        {
          isAddrRepeated = TRUE;
          break;
        }
      }

      if ( isAddrRepeated == FALSE )
      {
        /* #40 Copy the IP address to the dynamic list of IP addresses. */
        /* PRQA S 0314 2 */ /* MD_MSR_VStdLibCopy */
        VStdLib_MemCpy(DOIP_A_P2VAR(void)VenOptIpAddrDynListPtr[venOptsAddrListIter].DhcpIpAddr.addr,                  /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
          DOIP_A_P2CONST(void)venOptIpAddr.addr, addrSize);

        /* Read the next DHCP address from the buffer. */
        venOptsAddrListIter++;
      }

      /* The received IP address list contains at least one valid IP address */
      retVal = E_OK;
    }
    else
    {
      /* Received DHCP IP address is not valid, overwrite the IP address in next loop */
    }
  }

  DOIP_DUMMY_STATEMENT(OptionBufferLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  return retVal;
} /* DoIP_Dhcp_ReadAndFilterIpAddresses() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ValidateIpAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Dhcp_ValidateIpAddress(
  SoAd_SockAddrInetXType IpAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                  retVal;
  uint8                           netmask = 0u;
  SoAd_SockAddrInetXType          localAddr = { SOAD_AF_INVALID, 0u, { 0u } };
  SoAd_SockAddrInetXType          defaultRouter = { SOAD_AF_INVALID, 0u, { 0u } };
  DoIP_ConnectionUdpVehAnIterType connectionUdpVehAnIter;

  /* ----- Implementation ----------------------------------------------- */
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  if ( IpAddr.domain == SOAD_AF_INET6 )
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
  {
    /* #10 Check if IPv6 address is an invalid address. */
    retVal = DoIP_Util_CheckIpV6Addr(IpAddr);
  }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
  else
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
# endif /* DOIP_IPV6_ENABLED == STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
  {
    /* #20 Check if IPv4 address is an invalid address. */
    retVal = DoIP_Util_CheckIpV4Addr(IpAddr);
  }
# endif /* DOIP_IPV4_ENABLED == STD_ON */

  if ( retVal == E_OK )
  {
    for ( connectionUdpVehAnIter = 0u;
          connectionUdpVehAnIter < DoIP_GetSizeOfConnectionUdpVehAn();
          connectionUdpVehAnIter++ )
    {
      DoIP_ConnectionIdxOfConnectionUdpVehAnType connectionIdx =
        DoIP_GetConnectionIdxOfConnectionUdpVehAn(connectionUdpVehAnIter);

      /* PRQA S 0310 3 */ /* MD_DoIP_11.4 */
      if ( SoAd_GetLocalAddr(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                                            /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
            DOIP_A_P2VAR(SoAd_SockAddrType)&localAddr, &netmask,
            DOIP_A_P2VAR(SoAd_SockAddrType)&defaultRouter) == E_OK )
      {
        /* #30 Validate the received IP address against all DoIP related local IP addresses. */
        if ( DoIP_Util_CompareIpAddr(IpAddr.domain, IpAddr.addr, localAddr.addr) == TRUE )                             /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
        {
          /* The received address matches the local address; exit return E_NOT_OK */
          retVal = E_NOT_OK;
          break;
        }
      }
    }
  }
  return retVal;
} /* DoIP_Dhcp_ValidateIpAddress() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_CompareToLocalIpAddrList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_CompareToLocalIpAddrList(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  P2VAR(DoIP_DhcpIpAddrDynType, AUTOMATIC, DOIP_APPL_VAR) VenOptIpAddrDynListPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_IpAddrListIterType ipAddrListIdx;
  DoIP_DhcpIpAddrIterType venOptsAddrListIter;

  /* ----- Implementation ----------------------------------------------- */
  for ( venOptsAddrListIter = 0u;
        venOptsAddrListIter < DOIP_DHCP_IPADDRESS_LIST_MAX_SIZE;
        venOptsAddrListIter++ )
  {
    /* #10 Iterate over all the received Vendor option IP address list until the ones with invalid address. */
    if ( VenOptIpAddrDynListPtr[venOptsAddrListIter].DhcpIpAddr.domain != SOAD_AF_INVALID )
    {
      /* Assumed no match ==> new DHCP address has to be added to the local list. */
      VenOptIpAddrDynListPtr[venOptsAddrListIter].IsAddedToList = FALSE;                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

      for ( ipAddrListIdx = DoIP_GetIpAddrListStartIdxOfLocalAddr(LocalAddrIdx);
            ipAddrListIdx < DoIP_GetIpAddrListEndIdxOfLocalAddr(LocalAddrIdx);
            ipAddrListIdx++ )
      {
        /* #20 Compare the locally stored IP addresses with the Vendor option IP address received from DHCP. */
        if ( DoIP_Util_CompareIpAddr(DoIP_GetIpAddrOfIpAddrList(ipAddrListIdx).domain,                                 /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
              DoIP_GetIpAddrOfIpAddrList(ipAddrListIdx).addr,
              VenOptIpAddrDynListPtr[venOptsAddrListIter].DhcpIpAddr.addr) == TRUE )
        {
          /* #30 Mark that the received IP address already exists in the local IP address list. */
          VenOptIpAddrDynListPtr[venOptsAddrListIter].IsAddedToList = TRUE;                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
          break;
        }
      }
    }
    else
    {
      /* Start of invalid addresses in the VenOptIpAddrDynList, hence skip the remaining. */
      break;
    }
  }
} /* DoIP_Dhcp_CompareToLocalIpAddrList() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_UpdateLocalIpAddrList()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_UpdateLocalIpAddrList(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  P2CONST(DoIP_DhcpIpAddrDynType, AUTOMATIC, DOIP_APPL_VAR) VenOptIpAddrDynListPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType unspecifiedAddress = { SOAD_AF_INVALID, 0u, { 0u } };
  DoIP_IpAddrListIterType ipAddrListIdx;
  DoIP_DhcpIpAddrIterType venOptsAddrListIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #10 Iterate over entire local vendor option IP address list. */
  for ( ipAddrListIdx = DoIP_GetIpAddrListStartIdxOfLocalAddr(LocalAddrIdx);
        ipAddrListIdx < DoIP_GetIpAddrListEndIdxOfLocalAddr(LocalAddrIdx);
        ipAddrListIdx++ )
  {
    /* #20 Check if corresponding IP address in received list is valid. */
    if ( VenOptIpAddrDynListPtr[venOptsAddrListIter].DhcpIpAddr.domain != SOAD_AF_INVALID  )
    {
      /* #30 Update the local IP address list entry with address from the received list. */
      DoIP_SetIpAddrOfIpAddrList(ipAddrListIdx, VenOptIpAddrDynListPtr[venOptsAddrListIter].DhcpIpAddr);               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */

      /* #40 Check if it is a newly received address. */
      if ( VenOptIpAddrDynListPtr[venOptsAddrListIter].IsAddedToList == FALSE )
      {
        /* #50 Mark entry to send vehicle announcements. */
        DoIP_SetTriggerVehAnnOfIpAddrList(ipAddrListIdx, TRUE);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
      }
      venOptsAddrListIter++;
    }
    /* #60 Set local IP address list entry as invalid otherwise. */
    else
    {
      DoIP_SetIpAddrOfIpAddrList(ipAddrListIdx, unspecifiedAddress);                                                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
      DoIP_SetTriggerVehAnnOfIpAddrList(ipAddrListIdx, FALSE);                                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
    }
  }

  DOIP_END_CRITICAL_SECTION();
} /* DoIP_Dhcp_UpdateLocalIpAddrList() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_ProcessIpAddrListToTriggerVehAnnouncements()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_ProcessIpAddrListToTriggerVehAnnouncements(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionUdpVehAnIterType connectionUdpVehAnIter;
  DoIP_IpAddrListIterType         ipAddrListIdx;
  DoIP_SizeOfLocalAddrType        localAddrIdx;

  /* ----- Implementation ----------------------------------------------- */
  for ( connectionUdpVehAnIter = DoIP_GetConnectionUdpVehAnStartIdxOfInterface(InterfaceIdx);
        connectionUdpVehAnIter < DoIP_GetConnectionUdpVehAnEndIdxOfInterface(InterfaceIdx);
        connectionUdpVehAnIter++ )
  {
    DoIP_ConnectionIdxOfConnectionUdpVehAnType connectionIdx =
      DoIP_GetConnectionIdxOfConnectionUdpVehAn(connectionUdpVehAnIter);

    /* #10 Iterate over all online socket connections (vehicle announcement) and whose IP addresses are assigned. */
    if ( (DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE) &&
         (DoIP_GetIpAddrStateOfConnectionDyn(connectionIdx) == SOAD_IPADDR_STATE_ASSIGNED) )
    {
      localAddrIdx = DoIP_GetLocalAddrIdxOfConnection(connectionIdx);
      for ( ipAddrListIdx = DoIP_GetIpAddrListStartIdxOfLocalAddr(localAddrIdx);
            ipAddrListIdx < DoIP_GetIpAddrListEndIdxOfLocalAddr(localAddrIdx);
            ipAddrListIdx++ )
      {
        /* #20 Handle vehicle announcements on connections. */
        DoIP_Dhcp_HandleVehAnnounceTriggOfIpAddrList(connectionIdx, ipAddrListIdx);
      }
    }
  }
} /* DoIP_Dhcp_ProcessIpAddrListToTriggerVehAnnouncements() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Dhcp_HandleVehAnnounceTriggOfIpAddrList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Dhcp_HandleVehAnnounceTriggOfIpAddrList(
  DoIP_SizeOfConnectionType ConnectionIdx,
  DoIP_IpAddrListIterType IpAddrListIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType ipAddr;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #10 Check if the trigger is enabled for the DHCP provided IP address. */
  if ( DoIP_IsTriggerVehAnnOfIpAddrList(IpAddrListIdx) )
  {
    ipAddr = DoIP_GetIpAddrOfIpAddrList(IpAddrListIdx);

    /* #20 Request transmission of vehicle announcement and check if it succeeds. */
    if ( DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT,                                        /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
          DoIP_GetVehAnCntOfInterface(DoIP_GetInterfaceIdxOfConnection(ConnectionIdx)), FALSE, &ipAddr) == E_OK )
    {
      /* #30 Reset the trigger only if the transmission request succeeded. */
      DoIP_SetTriggerVehAnnOfIpAddrList(IpAddrListIdx, FALSE);                                                         /* SBSW_DOIP_PARAM_VALID_INDEX */
    }
  }

  DOIP_END_CRITICAL_SECTION();
} /* DoIP_Dhcp_HandleVehAnnounceTriggOfIpAddrList() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_HandleSoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleSoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode,
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle socket connection mode change to online. */
  if ( Mode == SOAD_SOCON_ONLINE )
  {
    DoIP_Con_HandleOnlineSoConModeChg(SoConId, Mode, ConnectionIdx, ErrorIdPtr);                                       /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }
  /* #20 Handle socket connection mode change to offline or reconnect. */
  else
  {
    DoIP_Con_HandleOfflineSoConModeChg(Mode, ConnectionIdx);
  }
} /* DoIP_Con_HandleSoConModeChg() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleOnlineSoConModeChg()
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
/* PRQA S 3206 6 */ /* MD_DoIP_3206 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOnlineSoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode,
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr) /* PRQA S 3673 */ /* MD_DoIP_16.7 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IP address is assigned. */
  if ( DoIP_GetIpAddrStateOfConnectionDyn(ConnectionIdx) == SOAD_IPADDR_STATE_ASSIGNED )
  {
    DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(ConnectionIdx);

    /* #20 Check if activation line is active. */
    if ( DoIP_IsActLineActiveOfInterfaceDyn(interfaceIdx) &&
      (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(interfaceIdx) == 0u) )
    {
      DoIP_SetSoConStateOfConnectionDyn(ConnectionIdx, Mode);                                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx, FALSE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

      /* #30 Check if connection is UDP vehicle announcement and transmission is required. */
      DOIP_BEGIN_CRITICAL_SECTION();
      if ( DoIP_Con_IsTxVehAnRequired(ConnectionIdx) )
      {
        /* #40 If required, request transmission of vehicle announcement. */
        DoIP_TxUdp_TransmitVehAn(ConnectionIdx, ErrorIdPtr);                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        DOIP_END_CRITICAL_SECTION();
      }
#if ( DOIP_VERIFY_RX_PDU_ENABLED == STD_ON )
      /* #50 Otherwise, check if connection is TCP and save remote/local IP address (if enabled). */
      else if ( DoIP_Util_IsTcpConnection(ConnectionIdx) == TRUE )
      {
        uint8 netmask = 0u;
        SoAd_SockAddrInetXType remIpAddr = { SOAD_AF_INVALID, 0u, { 0u } };
        SoAd_SockAddrInetXType locIpAddr = { SOAD_AF_INVALID, 0u, { 0u } };
        SoAd_SockAddrInetXType defaultRouter = { SOAD_AF_INVALID, 0u, { 0u } };
        DoIP_ConnectionTypeIdxOfConnectionType connectionTcpIdx = DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

        DOIP_END_CRITICAL_SECTION();

        /* PRQA S 0310 6 */ /* MD_DoIP_11.4 */
        (void)SoAd_GetRemoteAddr(SoConId, DOIP_A_P2VAR(SoAd_SockAddrType)&remIpAddr);                                  /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        (void)SoAd_GetLocalAddr (SoConId, DOIP_A_P2VAR(SoAd_SockAddrType)&locIpAddr,                                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
          &netmask, DOIP_A_P2VAR(SoAd_SockAddrType)&defaultRouter);

        DoIP_SetRemIpAddrOfConnectionTcpDyn(connectionTcpIdx, remIpAddr);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL04 */
        DoIP_SetLocIpAddrOfConnectionTcpDyn(connectionTcpIdx, locIpAddr);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL04 */
      }
#endif /* DOIP_VERIFY_RX_PDU_ENABLED == STD_ON */
      /* #60 Otherwise, do nothing. */
      else
      {
        DOIP_END_CRITICAL_SECTION();
      }
    }
    /* #70 Otherwise, close the socket connection. */
    else
    {
      /* Socket connection was opened on reception. In case of inactive activation line, the 'open request' has to
       * discarded to ensure that no DoIP communication happens. */
      (void)SoAd_CloseSoCon(SoConId, TRUE);
    }
  }
  else
  {
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
    /* #80 Set error if unassigned (if enabled). */
    *ErrorIdPtr = DOIP_E_UNEXPECTED_ASSIGNMENT;                                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#else
    /* nothing to do here */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  }

#if ( DOIP_DEV_ERROR_DETECT == STD_OFF )
  DOIP_DUMMY_STATEMENT(ErrorIdPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_OFF */
} /* DoIP_Con_HandleOnlineSoConModeChg() */

/**********************************************************************************************************************
 *  DoIP_Con_IsTxVehAnRequired()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(boolean, DOIP_CODE) DoIP_Con_IsTxVehAnRequired(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean txRequired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if connection is a UDP vehicle announcement connection. */
  if ( DoIP_Util_IsUdpVehAnConnection(ConnectionIdx) )
  {
    DoIP_SizeOfInterfaceType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(ConnectionIdx);
    DoIP_SizeOfConnectionUdpVehAnType vehAnConnectionIdx = DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

    /* #20 Return TRUE if vehicle announcement start is automatic or if the pending announcement flag is set. */
    txRequired = (boolean)( (DoIP_GetVehAnStartOfInterface(interfaceIdx) == DOIP_AUTOMATIC_ANNOUNCE) ||
      ((DoIP_GetVehAnStartOfInterface(interfaceIdx) == DOIP_ONTRIGGER_ANNOUNCE) &&
      (DoIP_IsPendingVehAnOfConnectionUdpVehAnDyn(vehAnConnectionIdx))) );
  }

  return txRequired;
} /* DoIP_Con_IsTxVehAnRequired() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineSoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineSoConModeChg(
  SoAd_SoConModeType Mode,
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset socket connection on state change to offline. */
  if ( (Mode == SOAD_SOCON_OFFLINE) && (DoIP_GetSoConStateOfConnectionDyn(ConnectionIdx) != SOAD_SOCON_OFFLINE) )
  {
    (void)SoAd_CloseSoCon(DoIP_GetSoConIdOfConnectionDyn(ConnectionIdx), TRUE);
  }

  /* Do not check current state since it might be reset on call to SoAd_CloseSoCon already. */
  DoIP_SetSoConStateOfConnectionDyn(ConnectionIdx, Mode);                                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx, FALSE);                                                           /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

  if ( DoIP_Util_IsTcpConnection(ConnectionIdx) == TRUE )
  {
    /* #20 Handle TCP socket connection mode change to offline or reconnect. */
    DoIP_Con_HandleOfflineTcpSoConModeChg(DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx));
  }
  else
  {
    /* #30 Handle UDP socket connection mode change to offline or reconnect. */
    DoIP_Con_HandleOfflineUdpSoConModeChg(ConnectionIdx);
  }
} /* DoIP_Con_HandleOfflineSoConModeChg() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineTcpSoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineTcpSoConModeChg(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
  const DoIP_SizeOfConnectionType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset socket connection specific structs. */
  if ( testerIdx != DoIP_GetSizeOfTester() )
  {
    DoIP_SetConnectionTcpIdxOfTesterDyn(testerIdx, DoIP_GetSizeOfConnectionTcp());                                     /* SBSW_DOIP_RegisterTester */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
    DoIP_SetDiagNAckOffsetOfTesterDyn(testerIdx, 0u);                                                                  /* SBSW_DOIP_RegisterTester */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  }

  DoIP_SetInactivCntOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetTxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetRxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx, DOIP_HDR_TYPE_INVALID);                                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
#if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
  DoIP_SetTxTpCancelRequestedOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
#endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */
#if ( DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON )
  DoIP_SetRxTpCancelRequestedOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
#endif /* DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON */
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
  DoIP_SetAliveCheckCntOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
  DoIP_RxTcpRout_RegisterTester(ConnectionTcpIdx, DoIP_GetSizeOfTester(), DOIP_ADDR_ANY,
    DoIP_GetSizeOfRoutingActivation(), DOIP_ROUT_ACTIV_STATE_NONE);

  /* #20 Handle pending alive checks. */
  DoIP_Con_HandleOfflineTcpAliveCheckSoConModeChg(ConnectionTcpIdx);

  /* #30 Cancel ongoing reception if Rx channel is active. */
  if ( DoIP_IsChannelRxActiveOfConnectionTcpDyn(ConnectionTcpIdx) )
  {
    PduR_DoIPTpRxIndication(DoIP_GetPduRRxPduIdOfChannel(DoIP_GetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx)),
      E_NOT_OK);
    DoIP_SetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx, DoIP_GetSizeOfChannel());                             /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetChannelRxActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
  /* #40 Check if an OEM specific Rx buffer is currently used by the connection and release it. */
  if ( DoIP_IsOemSpecificCfgUsedOfConnection(connectionIdx) )
  {
    const DoIP_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIP_GetOemSpecificCfgIdxOfConnection(connectionIdx);
    const DoIP_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx = DoIP_GetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(
      oemSpecificCfgIdx);

    if ( oemRxBufCfgIdx != DoIP_GetSizeOfOemPayloadRxBufConfig() )
    {
      DoIP_RxTcpOem_ReleaseBuf(oemRxBufCfgIdx);
      DoIP_SetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx, DoIP_GetSizeOfOemPayloadRxBufConfig());   /* SBSW_DOIP_CSL02_CSL03 */
    }
  }
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

  /* #50 Remove all outstanding TCP transmissions from Tx queue. */
  while ( DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx) > 0u )
  {
    DoIP_TxTcpQueue_RemoveElement(ConnectionTcpIdx, E_NOT_OK);
  }
} /* DoIP_Con_HandleOfflineTcpSoConModeChg() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineTcpAliveCheckSoConModeChg()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineTcpAliveCheckSoConModeChg(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfConnectionTcpType connectionTcpIdxOfRoutActivHandler =
    DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if routing activation handler is used. */
  if ( connectionTcpIdxOfRoutActivHandler != DoIP_GetSizeOfConnectionTcp() )
  {
    /* #20 Check if alive checks are pending. */
    if ( DoIP_GetAliveCheckElemNumOfRoutActivHandler(0u) > 0u )
    {
      DoIP_AliveCheckListIterType aliveCheckListIdx;

      for ( aliveCheckListIdx = 0u;
            aliveCheckListIdx < DoIP_GetSizeOfAliveCheckList();
            aliveCheckListIdx++ )
      {
        /* #30 Check if alive checks are pending for current connection and remove element from list. */
        if ( ConnectionTcpIdx == DoIP_GetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx) )
        {
          DoIP_SetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx, DoIP_GetSizeOfConnectionTcp());                  /* SBSW_DOIP_CSL01 */
          DoIP_DecAliveCheckElemNumOfRoutActivHandler(0u);                                                             /* SBSW_DOIP_RoutActivHandler */
          break;
        }
      }
    }

    /* #40 Check if no alive checks are pending anymore. */
    if ( DoIP_GetAliveCheckElemNumOfRoutActivHandler(0u) == 0u )
    {
      DoIP_ConnectionIdxOfConnectionTcpType connectionIdx =
        DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdxOfRoutActivHandler);

      /* #50 Check if socket connection on which the routing activation request has been received is still online. */
      if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE )
      {
        /* #60 Trigger routing activation handling in next main function in case alive check has been started. */
        if ( DoIP_GetAliveCheckCntOfRoutActivHandler(0u) > 0u )
        {
          DoIP_SetAliveCheckCntOfRoutActivHandler(0u, DOIP_ALIVE_CHECK_WAIT_NEXT_MAIN);                                /* SBSW_DOIP_RoutActivHandler */
        }
      }
      /* #70 Release routing activation handler otherwise. */
      else
      {
        DoIP_SetConnectionTcpIdxOfRoutActivHandler(0u, DoIP_GetSizeOfConnectionTcp());                                 /* SBSW_DOIP_RoutActivHandler */
        DoIP_SetAliveCheckCntOfRoutActivHandler(0u, 0u);                                                               /* SBSW_DOIP_RoutActivHandler */
      }
    }
  }
} /* DoIP_Con_HandleOfflineTcpAliveCheckSoConModeChg() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleOfflineUdpSoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleOfflineUdpSoConModeChg(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_TxUdpMgtListIterType txUdpMgtListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over Tx UDP list. */
  for ( txUdpMgtListIdx = 0u;
        txUdpMgtListIdx < DoIP_GetSizeOfTxUdpMgtList();
        txUdpMgtListIdx++ )
  {
    /* #20 Invalidate UDP connection if element is in list. */
    if ( DoIP_GetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx) == ConnectionIdx )
    {
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
      if ( DoIP_GetHdrTypeOfTxUdpMgtList(txUdpMgtListIdx) == DOIP_HDR_TYPE_OEM_SPECIFIC )
      {
        DoIP_TxOem_ReleaseBuf((DoIP_SizeOfOemPayloadTxBufConfigType)DoIP_GetParam1OfTxUdpMgtList(txUdpMgtListIdx));
      }
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

      DoIP_SetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx, DoIP_GetSizeOfConnection());                                /* SBSW_DOIP_CSL01 */
    }
  }
} /* DoIP_Con_HandleOfflineUdpSoConModeChg() */

/**********************************************************************************************************************
 *  DoIP_Con_SoConInitAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConInitAll(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr) /* PRQA S 3673 */ /* MD_DoIP_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initiate whole socket connection struct if not done before. */
  if ( DoIP_GetSoConIdOfConnectionDyn(0u) == SOAD_INV_SO_CON_ID )
  {
    DoIP_ConnectionIterType connectionIdx;
    SoAd_SoConIdType soConId = 0u;

    /* #20 For all connections get socket connection identifier from SoAd. */
    for ( connectionIdx = 0u;
          connectionIdx < DoIP_GetSizeOfConnection();
          connectionIdx++ )
    {
      retVal |= SoAd_GetSoConId(DoIP_GetSoAdTxPduIdOfConnection(connectionIdx), &soConId);                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

      /* #30 Save socket connection identifier if successfully received from SoAd. */
      if ( retVal == E_OK )
      {
        DoIP_SetSoConIdOfConnectionDyn(connectionIdx, soConId);                                                        /* SBSW_DOIP_CSL02 */
      }
      else
      {
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
        /* #40 Otherwise set error (if enabled). */
        *ErrorIdPtr = DOIP_E_SOAD_CALL_FAILED;                                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#else
        DOIP_DUMMY_STATEMENT(ErrorIdPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
        break;
      }
    }
  }

  return retVal;
} /* DoIP_Con_SoConInitAll() */

/**********************************************************************************************************************
 *  DoIP_Con_SoConOpen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConOpen(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #10 Check if socket connection has to be opened. */
  if ( DoIP_GetSoConStateOfConnectionDyn(ConnectionIdx) == SOAD_SOCON_OFFLINE )
  {
    /* #20 Set socket connection state to reconnect to avoid multiple open requests and open socket connection. */
    DoIP_SetSoConStateOfConnectionDyn(ConnectionIdx, SOAD_SOCON_RECONNECT);                                            /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    DOIP_END_CRITICAL_SECTION();

    (void)SoAd_OpenSoCon(DoIP_GetSoConIdOfConnectionDyn(ConnectionIdx));
  }
  else
  {
    DOIP_END_CRITICAL_SECTION();
  }
} /* DoIP_Con_SoConOpen() */

/**********************************************************************************************************************
 *  DoIP_Con_SoConCloseAll()
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
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_SoConCloseAll(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  DoIP_ConnectionIterType connectionIdx;
  DoIP_SizeOfConnectionType connectionTcpIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all socket connections of the interface. */
  for ( connectionIdx = DoIP_GetConnectionStartIdxOfInterface(InterfaceIdx);
        connectionIdx < DoIP_GetConnectionEndIdxOfInterface(InterfaceIdx);
        connectionIdx++ )
  {
    /* #20 Check if socket connection is online. */
    if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) != SOAD_SOCON_OFFLINE )
    {
      retVal = E_NOT_OK;

      /* #30 Check if socket connection is not yet closing. */
      if ( !DoIP_IsSoConClosingOfConnectionDyn(connectionIdx) )
      {
        /* #40 Handle TCP close. */
        if ( DoIP_Util_IsTcpConnection((DoIP_SizeOfConnectionType)connectionIdx) == TRUE )
        {
          /* #50 Check if activation line inactive timeout is configured. */
          if ( DoIP_GetActLineInactiveTimeoutOfInterface(InterfaceIdx) > 0u )
          {
            connectionTcpIdx = DoIP_GetConnectionTypeIdxOfConnection(connectionIdx);

            /* #60 Close socket connection directly if no reception or transmission is pending. */
            if ( (DoIP_GetRxMsgOffsetOfConnectionTcpDyn(connectionTcpIdx) == 0u) &&
              (DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(connectionTcpIdx) == 0u) )
            {
              DoIP_Con_SoConClose((DoIP_SizeOfConnectionType)connectionIdx, FALSE);
            }
            /* #70 Close socket connection immediately if an activation line inactive request is pending. */
            else if ( (DoIP_GetActLineInactiveTimeOfInterfaceDyn(InterfaceIdx) + 1u) >=
              DoIP_GetActLineInactiveTimeoutOfInterface(InterfaceIdx) )
            {
              DoIP_Con_SoConClose((DoIP_SizeOfConnectionType)connectionIdx, TRUE);
            }
            else
            {
              /* nothing to do here */
            }
          }
          /* #80 Otherwise close socket directly. */
          else
          {
#if ( DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON )
            DoIP_Con_SoConClose((DoIP_SizeOfConnectionType)connectionIdx, FALSE);
#else
            DoIP_Con_SoConClose((DoIP_SizeOfConnectionType)connectionIdx, TRUE);
#endif /* DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON */
          }
        }
        /* #90 Handle UDP close. */
        else
        {
          /* #100 Close socket connection directly. */
          DoIP_Con_SoConClose((DoIP_SizeOfConnectionType)connectionIdx, FALSE);
        }
      }
    }
  }

  /* #110 Increment inactive time if not all socket connections were closed yet. */
  if ( (retVal != E_OK) && (DoIP_GetActLineInactiveTimeoutOfInterface(InterfaceIdx) > 0u) )
  {
    DoIP_IncActLineInactiveTimeOfInterfaceDyn(InterfaceIdx);                                                           /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

  return retVal;
} /* DoIP_Con_SoConCloseAll() */

/**********************************************************************************************************************
 *  DoIP_Con_SoConClose()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SoConClose(
  DoIP_SizeOfConnectionType ConnectionIdx,
  boolean Abort)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Mark socket connection as closing. */
  DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx, TRUE);                                                            /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

  /* #20 Request socket connection close. */
  (void)SoAd_CloseSoCon(DoIP_GetSoConIdOfConnectionDyn(ConnectionIdx), Abort);

  /* #30 Set state from reconnect to offline since it might be set to reconnect to avoid multiple open requests. */
  /* To set socket connection to offline here won't have side effects on the DoIP_SoConModeChg callback since socket
   * connection is reset there without checking current state. */
  if ( DoIP_GetSoConStateOfConnectionDyn(ConnectionIdx) == SOAD_SOCON_RECONNECT )
  {
    DoIP_SetSoConStateOfConnectionDyn(ConnectionIdx, SOAD_SOCON_OFFLINE);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
} /* DoIP_Con_SoConClose() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleLocalIpAddrAssignmentChg()
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
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleLocalIpAddrAssignmentChg(
  DoIP_SizeOfConnectionType ConnectionIdx,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfLocalAddrType localAddrIdx = DoIP_GetLocalAddrIdxOfConnection(ConnectionIdx);
  DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(ConnectionIdx);
#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  const SoAd_SockAddrInetXType ipAddr = { SOAD_AF_INVALID, 0u, { 0u } };
  DoIP_IpAddrListIterType ipAddrListIdx;
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store new assignment state. */
  DoIP_SetIpAddrStateOfConnectionDyn(ConnectionIdx, State);                                                            /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

  /* #20 Check if IP address assignment changed to unassigned. */
  if ( State == SOAD_IPADDR_STATE_UNASSIGNED )
  {
    DoIP_IpAddrAssignmentIterType ipAddrAssignmentIdx;

    /* #30 Iterate over all related IP address assignments. */
    for ( ipAddrAssignmentIdx = DoIP_GetIpAddrAssignmentStartIdxOfLocalAddr(localAddrIdx);
          ipAddrAssignmentIdx < DoIP_GetIpAddrAssignmentEndIdxOfLocalAddr(localAddrIdx);
          ipAddrAssignmentIdx++ )
    {
      /* #40 Reset IP address request state. */
      DoIP_SetIpAddrReqStateOfIpAddrAssignmentDyn(ipAddrAssignmentIdx, DOIP_IP_ADDR_REQ_STATE_NONE);                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
    }

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
    DOIP_BEGIN_CRITICAL_SECTION();

    /* #50 Iterate over related IP address list and reset IP addresses. */
    for ( ipAddrListIdx = DoIP_GetIpAddrListStartIdxOfLocalAddr(localAddrIdx);
          ipAddrListIdx < DoIP_GetIpAddrListEndIdxOfLocalAddr(localAddrIdx);
          ipAddrListIdx++ )
    {
      DoIP_SetTriggerVehAnnOfIpAddrList(ipAddrListIdx, FALSE);                                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
      DoIP_SetIpAddrOfIpAddrList(ipAddrListIdx, ipAddr);                                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
    }

    DOIP_END_CRITICAL_SECTION();
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

    /* #60 Request to close socket connection. */
    DoIP_Con_SoConClose(ConnectionIdx, TRUE);

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
    /* #70 Reset GID sync state if all IP addresses are unassigned (if GID sync is configured). */
    /* Check value before decrement since IP address unassigned is reported even if it has not been assigned before. */
    DOIP_BEGIN_CRITICAL_SECTION(); /* protects DoIP_LocalAddrAssignedCounter */
    if ( DoIP_LocalAddrAssignedCounter > 0u )
    {
      DoIP_LocalAddrAssignedCounter--;
      if ( DoIP_LocalAddrAssignedCounter == 0u )
      {
        DoIP_GidInSync = FALSE;
      }
    }
    DOIP_END_CRITICAL_SECTION();
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */
  }
  /* #80 Check if IP address assignment changed to assigned otherwise. */
  else if ( State == SOAD_IPADDR_STATE_ASSIGNED )
  {
    /* #90 Request to open socket connection if activation line state is active. */
    if ( DoIP_IsActLineActiveOfInterfaceDyn(interfaceIdx) &&
      (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(interfaceIdx) == 0u) )
    {
      DoIP_Con_SoConOpen(ConnectionIdx);
    }

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
    /* #100 Increment number of connections with assigned local addresses (if GID sync is configured). */
    DOIP_BEGIN_CRITICAL_SECTION(); /* protects DoIP_LocalAddrAssignedCounter */
    DoIP_LocalAddrAssignedCounter++;
    DOIP_END_CRITICAL_SECTION();
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */
  }
  /* #110 Ignore assignment state change otherwise. */
  else
  {
    /* nothing to do here */
  }
} /* DoIP_Con_HandleLocalIpAddrAssignmentChg() */

/**********************************************************************************************************************
 *  DoIP_Con_RelAllIpAddrAssign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelAllIpAddrAssign(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_LocalAddrIterType localAddrIdx;
  DoIP_IpAddrAssignmentIterType ipAddrAssignmentIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all local IP addresses of the interface. */
  for ( localAddrIdx = DoIP_GetLocalAddrStartIdxOfInterface(InterfaceIdx);
        localAddrIdx < DoIP_GetLocalAddrEndIdxOfInterface(InterfaceIdx);
        localAddrIdx++ )
  {
    /* #20 Iterate over all related IP address assignments. */
    for ( ipAddrAssignmentIdx = DoIP_GetIpAddrAssignmentStartIdxOfLocalAddr(localAddrIdx);
          ipAddrAssignmentIdx < DoIP_GetIpAddrAssignmentEndIdxOfLocalAddr(localAddrIdx);
          ipAddrAssignmentIdx++ )
    {
#if ( DOIP_CONTROL_IP_ASSIGNMENT == STD_ON )
      /* #30 Release IP address assignment if IP address assignment control is enabled (if configured). */
      if ( DoIP_IsControlIpAssignmentOfIpAddrAssignment(ipAddrAssignmentIdx) )
      {
        /* May be called multiple times in case multiple IP address assignments are controlled by DoIP. */
        DoIP_Con_RelIpAddrAssign((DoIP_SizeOfLocalAddrType)localAddrIdx);
      }
#endif /* DOIP_CONTROL_IP_ASSIGNMENT == STD_ON */

      /* #40 Reset IP address assignment request state. */
      DoIP_SetIpAddrReqStateOfIpAddrAssignmentDyn(ipAddrAssignmentIdx, DOIP_IP_ADDR_REQ_STATE_NONE);                   /* SBSW_DOIP_CSL02_CSL03 */
    }
  }
} /* DoIP_Con_RelAllIpAddrAssign() */

#if ( DOIP_CONTROL_IP_ASSIGNMENT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_RelIpAddrAssign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_RelIpAddrAssign(
  DoIP_SizeOfLocalAddrType LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfConnectionType connectionIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get first local address related connection. */
  connectionIdx = DoIP_GetConnectionInd(DoIP_GetConnectionIndStartIdxOfLocalAddr(LocalAddrIdx));

  /* #20 Release IP address on related connection. */
  /* Ignore return value of function since there is nothing DoIP can do if release fails (retry doesn't make sense). */
  (void)SoAd_ReleaseIpAddrAssignment(DoIP_GetSoConIdOfConnectionDyn(connectionIdx));
} /* DoIP_Con_RelIpAddrAssign() */
#endif /* DOIP_CONTROL_IP_ASSIGNMENT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_GetConnectionIdxBySoConId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_GetConnectionIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(DoIP_SizeOfConnectionType, AUTOMATIC, DOIP_APPL_VAR) ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_ConnectionIterType connectionIdx;

  /* ----- Implementation ----------------------------------------------- */
  *ConnectionIdx = DoIP_GetSizeOfConnection();                                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

  if ( SoConId != SOAD_INV_SO_CON_ID )
  {
    for ( connectionIdx = 0u;
          connectionIdx < DoIP_GetSizeOfConnection();
          connectionIdx++ )
    {
      /* #10 Look up connection index for current socket connection identifier. */
      if ( SoConId == DoIP_GetSoConIdOfConnectionDyn(connectionIdx) )
      {
        *ConnectionIdx = (DoIP_SizeOfConnectionType)connectionIdx;                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
        retVal = E_OK;
        break;
      }
    }
  }

  return retVal;
} /* DoIP_Con_GetConnectionIdxBySoConId() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleSoConStates()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleSoConStates(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionIterType connectionIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connections. */
  for ( connectionIdx = DoIP_GetConnectionStartIdxOfInterface(InterfaceIdx);
        connectionIdx < DoIP_GetConnectionEndIdxOfInterface(InterfaceIdx);
        connectionIdx++ )
  {
    /* #20 Check if socket connection state is offline and IP address is assigned to open connection. */
    if ( (DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_OFFLINE) &&
         (DoIP_GetIpAddrStateOfConnectionDyn(connectionIdx) == SOAD_IPADDR_STATE_ASSIGNED) )
    {
      /* #30 Set state to reconnect to avoid multiple open requests. */
      DoIP_SetSoConStateOfConnectionDyn(connectionIdx, SOAD_SOCON_RECONNECT);                                          /* SBSW_DOIP_CSL02_CSL03 */

      /* #40 Request to open socket connection. */
      (void)SoAd_OpenSoCon(DoIP_GetSoConIdOfConnectionDyn(connectionIdx));
    }
  }
} /* DoIP_Con_HandleSoConStates() */

/**********************************************************************************************************************
 *  DoIP_Con_CloseConnection()
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
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_CloseConnection(
  DoIP_SizeOfConnectionType ConnectionIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean closeRequired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent mode modifications from different context. */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #20 Check if socket connection is online. */
  if ( DoIP_GetSoConStateOfConnectionDyn(ConnectionIdx) != SOAD_SOCON_OFFLINE )
  {
    /* #30 Immediately close the connection when the abort flag is set or in case of a UDP connection. */
    if ( Abort == TRUE )
    {
      /* #300 Indicate that a call to SoAd to close the socket connection is required. */
      closeRequired = TRUE;
    }
    /* #31 Check for pending transmission otherwise. */
    else
    {
      /* #310 Check for pending transmission in case of a TCP connection. */
      if ( DoIP_Util_IsTcpConnection(ConnectionIdx) == TRUE )
      {
        const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

        /* #3100 Indicate that the connection shall be closed if no transmission is pending. */
        if ( DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(connectionTcpIdx) == 0u )
        {
          closeRequired = TRUE;
        }
        /* #3101 Mark socket connection as closing otherwise. */
        else
        {
          DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx, TRUE);                                                    /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        }
      }
      /* #311 Check for pending transmission in case of UDP and UDP vehicle announcement connections otherwise. */
      else
      {
        /* #3110 Indicate that the connection shall be closed if no transmission is pending. */
        if ( DoIP_TxUdpList_CheckForPendingEntry(ConnectionIdx) == FALSE )
        {
          closeRequired = TRUE;
        }
        /* #3111 Mark socket connection as closing otherwise. */
        else
        {
          DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx, TRUE);                                                    /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        }
      }
    }
  }

  /* #40 Leave critical section. */
  DOIP_END_CRITICAL_SECTION();

  /* #50 Forward call to SoAd to close the socket connection if required. */
  if ( closeRequired == TRUE )
  {
    DoIP_Con_SoConClose(ConnectionIdx, Abort);
  }
} /* DoIP_Con_CloseConnection() */

#if ( (DOIP_CONTROL_IP_ASSIGNMENT == STD_ON) || (DOIP_IPADDR_ASSIGN_DHCP == STD_ON) )
/**********************************************************************************************************************
 *  DoIP_Con_HandleIpAddrAssignment()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleIpAddrAssignment(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_LocalAddrIterType localAddrIdx;
  DoIP_IpAddrAssignmentIterType ipAddrAssignmentIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all local addresses of the interface. */
  for ( localAddrIdx = DoIP_GetLocalAddrStartIdxOfInterface(InterfaceIdx);
        localAddrIdx < DoIP_GetLocalAddrEndIdxOfInterface(InterfaceIdx);
        localAddrIdx++ )
  {
    /* #20 Iterate over all IP address assignments of the local address. */
    for ( ipAddrAssignmentIdx = DoIP_GetIpAddrAssignmentStartIdxOfLocalAddr(localAddrIdx);
          ipAddrAssignmentIdx < DoIP_GetIpAddrAssignmentEndIdxOfLocalAddr(localAddrIdx);
          ipAddrAssignmentIdx++ )
    {
      /* #30 Check if IP address assignment is not yet requested. */
      if ( DoIP_GetIpAddrReqStateOfIpAddrAssignmentDyn(ipAddrAssignmentIdx) == DOIP_IP_ADDR_REQ_STATE_NONE )
      {
        /* #40 Set IP address assignment to requested. */
        DoIP_SetIpAddrReqStateOfIpAddrAssignmentDyn(ipAddrAssignmentIdx, DOIP_IP_ADDR_REQ_STATE_REQUESTED);            /* SBSW_DOIP_CSL02_CSL03 */

# if ( DOIP_IPADDR_ASSIGN_DHCP == STD_ON )
        /* #50 Overwrite DHCP hostname option (if configured). */
        if ( DoIP_GetAssignmentTypeOfIpAddrAssignment(ipAddrAssignmentIdx) == DOIP_IPADDR_ASSIGN_TYPE_DHCP )
        {
          DoIP_Con_SetDhcpHostname((DoIP_SizeOfLocalAddrType)localAddrIdx);
        }
# endif /* DOIP_IPADDR_ASSIGN_DHCP == STD_ON */

# if ( DOIP_CONTROL_IP_ASSIGNMENT == STD_ON )
        /* #60 Check if IP address assignment control is enabled (if configured). */
        if ( DoIP_IsControlIpAssignmentOfIpAddrAssignment(ipAddrAssignmentIdx) )
        {
          /* #70 Request IP address assignment. */
          Std_ReturnType retVal = DoIP_Con_ReqIpAddrAssign((DoIP_SizeOfLocalAddrType)localAddrIdx,
            (DoIP_SizeOfIpAddrAssignmentType)ipAddrAssignmentIdx);

          /* #80 Reset IP address assignment request state if request failed. */
          if ( retVal != E_OK )
          {
            DoIP_SetIpAddrReqStateOfIpAddrAssignmentDyn(ipAddrAssignmentIdx, DOIP_IP_ADDR_REQ_STATE_NONE);             /* SBSW_DOIP_CSL02_CSL03 */
          }
        }
# endif /* DOIP_CONTROL_IP_ASSIGNMENT == STD_ON */
      }
    }
  }
} /* DoIP_Con_HandleIpAddrAssignment() */
#endif /* (DOIP_CONTROL_IP_ASSIGNMENT == STD_ON) || (DOIP_IPADDR_ASSIGN_DHCP == STD_ON) */

#if ( DOIP_CONTROL_IP_ASSIGNMENT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_ReqIpAddrAssign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Con_ReqIpAddrAssign(
  DoIP_SizeOfLocalAddrType LocalAddrIdx,
  DoIP_SizeOfIpAddrAssignmentType IpAddrAssignmentIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  DoIP_SizeOfConnectionType connectionIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get first connection related to local address. */
  connectionIdx = DoIP_GetConnectionInd(DoIP_GetConnectionIndStartIdxOfLocalAddr(LocalAddrIdx));

  /* #20 Request IP address assignment on the connection. */
  switch(DoIP_GetAssignmentTypeOfIpAddrAssignment(IpAddrAssignmentIdx))
  {
# if ( DOIP_IPADDR_ASSIGN_STATIC == STD_ON )
    case DOIP_IPADDR_ASSIGN_TYPE_STATIC:
    {
      SoAd_SockAddrInetXType localIpAddr = { SOAD_AF_INVALID, 0u, { 0u } };
      SoAd_SockAddrInetXType defaultRouter = { SOAD_AF_INVALID, 0u, { 0u } };

      localIpAddr.domain = DoIP_GetDomainTypeOfLocalAddr(LocalAddrIdx);
      defaultRouter.domain = DoIP_GetDomainTypeOfLocalAddr(LocalAddrIdx);

      /* PRQA S 0310 3 */ /* MD_DoIP_11.4 */
      retVal = SoAd_RequestIpAddrAssignment(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        SOAD_IPADDR_ASSIGNMENT_STATIC, DOIP_A_P2VAR(SoAd_SockAddrType)&localIpAddr, 0u,
        DOIP_A_P2VAR(SoAd_SockAddrType)&defaultRouter);
      break;
    }
# endif /* DOIP_IPADDR_ASSIGN_STATIC == STD_ON */
# if ( DOIP_IPADDR_ASSIGN_LINKLOCAL_DOIP == STD_ON )
    case DOIP_IPADDR_ASSIGN_TYPE_LINKLOCAL_DOIP:
    {
      retVal = SoAd_RequestIpAddrAssignment(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                             /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        SOAD_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP, NULL_PTR, 0u, NULL_PTR);
      break;
    }
# endif /* DOIP_IPADDR_ASSIGN_LINKLOCAL_DOIP == STD_ON */
# if ( DOIP_IPADDR_ASSIGN_LINKLOCAL == STD_ON )
    case DOIP_IPADDR_ASSIGN_TYPE_LINKLOCAL:
    {
      retVal = SoAd_RequestIpAddrAssignment(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                             /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        SOAD_IPADDR_ASSIGNMENT_LINKLOCAL, NULL_PTR, 0u, NULL_PTR);
      break;
    }
# endif /* DOIP_IPADDR_ASSIGN_LINKLOCAL == STD_ON */
# if ( DOIP_IPADDR_ASSIGN_DHCP == STD_ON )
    case DOIP_IPADDR_ASSIGN_TYPE_DHCP:
    {
      retVal = SoAd_RequestIpAddrAssignment(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                             /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        SOAD_IPADDR_ASSIGNMENT_DHCP, NULL_PTR, 0u, NULL_PTR);
      break;
    }
# endif /* DOIP_IPADDR_ASSIGN_DHCP == STD_ON */
# if ( DOIP_IPADDR_ASSIGN_IPV6_ROUTER == STD_ON )
    case DOIP_IPADDR_ASSIGN_TYPE_IPV6_ROUTER:
    {
      retVal = SoAd_RequestIpAddrAssignment(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                             /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        SOAD_IPADDR_ASSIGNMENT_IPV6_ROUTER, NULL_PTR, 0u, NULL_PTR);
      break;
    }
# endif /* DOIP_IPADDR_ASSIGN_IPV6_ROUTER == STD_ON */
    default: /* COV_DOIP_DEFAULT_CASE */
    {
      /* nothing to do here */
      break;
    }
  }

  return retVal;
} /* DoIP_Con_ReqIpAddrAssign() */
#endif /* DOIP_CONTROL_IP_ASSIGNMENT == STD_ON */

#if ( DOIP_IPADDR_ASSIGN_DHCP == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_SetDhcpHostname()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_SetDhcpHostname(
  DoIP_SizeOfLocalAddrType LocalAddrIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get first connection related to local address. */
  const DoIP_SizeOfConnectionType connectionIdx = DoIP_GetConnectionInd(
    DoIP_GetConnectionIndStartIdxOfLocalAddr(LocalAddrIdx));

# if ( DOIP_DHCP_OPTION_VIN_USE_ENABLED == STD_ON )
  /* #20 Create hostname string with VIN (if configured). */
  /* PRQA S 0686 3 */ /* MD_DoIP_0686 */
  const uint8 hostNameSize = DOIP_HOST_NAME_VIN_SIZE;
  /* PRQA S 4414 1 */ /* MD_DoIP_ImplicitCharCast */
  uint8 hostName[DOIP_HOST_NAME_VIN_SIZE] = { 'D', 'o', 'I', 'P', '-', 'V', 'I', 'N' };

  if ( DoIP_CallGetVinDirect(&hostName[DOIP_HOST_NAME_VIN_PREFIX_SIZE]) != E_OK )                                      /* SBSW_DOIP_FUNCTION_POINTER_CALL */
  {
    /* #30 Append VIN invalid pattern if VIN could not be retrieved from user. */
    uint8_least hostNameIdx;

    for ( hostNameIdx = (uint8)(DOIP_HOST_NAME_VIN_PREFIX_SIZE);
          hostNameIdx < DOIP_HOST_NAME_VIN_SIZE;
          hostNameIdx++ )
    {
      hostName[hostNameIdx] = DOIP_VIN_INV_PATTERN;                                                                    /* SBSW_DOIP_ARRAY_WRITE */
    }
  }
# else
  /* #40 Create minimum hostname string (otherwise). */
  const uint8 hostNameSize = DOIP_HOST_NAME_MIN_SIZE;
  /* PRQA S 4414 1 */ /* MD_DoIP_ImplicitCharCast */
  const uint8 hostName[DOIP_HOST_NAME_MIN_SIZE] = { 'D', 'o', 'I', 'P', '-' };
# endif /* DOIP_DHCP_OPTION_VIN_USE_ENABLED == STD_ON */

  /* #50 Overwrite DHCP hostname option on first related connection. */
  /* There is no handling specified in case hostname option could not be set. */
  (void)SoAd_WriteDhcpHostNameOption(DoIP_GetSoConIdOfConnectionDyn(connectionIdx), hostNameSize, hostName);           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
} /* DoIP_Con_SetDhcpHostname() */
#endif /* DOIP_IPADDR_ASSIGN_DHCP == STD_ON */

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Con_HandleGidSync()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleGidSync(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if GID is out of sync. */
  if ( DoIP_GidInSync != TRUE )
  {
    /* #20 Trigger GID sync if at least one IP address is assigned. */
    if ( DoIP_LocalAddrAssignedCounter > 0u )
    {
      if ( DoIP_CallTriggerGidSyncDirect() == E_OK )                                                                   /* SBSW_DOIP_FUNCTION_POINTER_CALL */
      {
        DoIP_GidInSync = TRUE;
      }
    }
  }
} /* DoIP_Con_HandleGidSync() */
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_Con_HandleActLineStates()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleActLineStates(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_InterfaceIterType interfaceIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all interfaces. */
  for ( interfaceIter = 0u;
        interfaceIter < DoIP_GetSizeOfInterface();
        interfaceIter++ )
  {
    /* #20 Check if activation line is active. */
    if ( DoIP_IsActLineActiveOfInterfaceDyn(interfaceIter) )
    {
      /* #30 Check if activation line state inactive request is pending. */
      if ( DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(interfaceIter) > 0u )
      {
        /* #40 Handle activation line state inactive. */
        DoIP_Con_HandleActLineStateInactive(interfaceIter);
      }
      else
      {
        /* #50 Handle activation line state active. */
        DoIP_Con_HandleActLineStateActive(interfaceIter);
      }
    }
    /* #60 Otherwise check if activation line can be activated. */
    else if ( DoIP_GetActLineActiveRequestCounterOfInterfaceDyn(interfaceIter) > 0u )
    {
      /* #70 Set activation line to active. */
      DoIP_SetActLineActiveOfInterfaceDyn(interfaceIter, TRUE);                                                        /* SBSW_DOIP_CSL02 */
      DoIP_DecActLineActiveRequestCounterOfInterfaceDyn(interfaceIter);                                                /* SBSW_DOIP_CSL02 */

#if ( DOIP_ACT_LINE_STATE_CHG_CBK_SUPPORT == STD_ON )
      /* #80 Notify user about activation line state change to active. */
      DoIP_CallActivationLineStateChgDirect((uint8)interfaceIter, TRUE);                                               /* SBSW_DOIP_FUNCTION_POINTER_CALL */
#endif /* DOIP_ACT_LINE_STATE_CHG_CBK_SUPPORT == STD_ON */
    }
    else
    {
      /* nothing to do here */
    }
  }
} /* DoIP_Con_HandleActLineStates() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleActLineStateActive()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleActLineStateActive(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( (DOIP_CONTROL_IP_ASSIGNMENT == STD_ON) || (DOIP_IPADDR_ASSIGN_DHCP == STD_ON) )
  /* #10 Handle IP address assignments (if enabled). */
  DoIP_Con_HandleIpAddrAssignment(InterfaceIdx);
#endif /* (DOIP_CONTROL_IP_ASSIGNMENT == STD_ON) || (DOIP_IPADDR_ASSIGN_DHCP == STD_ON) */

  /* #20 Handle SoCon states. */
  DoIP_Con_HandleSoConStates(InterfaceIdx);

#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
  /* #30 Handle GID sync state (if enabled). */
  DoIP_Con_HandleGidSync();
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

  /* #40 Handle outstanding routing activation authentifications and confirmations. */
  DoIP_RxTcpRout_HandleRetryRoutingActivationAuthAndConf(InterfaceIdx);

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  /* #50 Handle outstanding vehicle announcement in DHCP IP Address List (if enabled). */
  DoIP_Dhcp_ProcessIpAddrListToTriggerVehAnnouncements(InterfaceIdx);
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

  /* #60 Copy outstanding user data. */
  DoIP_RxTcpDiag_HandleRetryCopyBufferedUserData(InterfaceIdx);

  /* #70 Handle outstanding TCP actions. */
  DoIP_TxTcpQueue_HandleTransmit(InterfaceIdx);

  /* #80 Handle timings. */
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
  DoIP_RxTcpRout_HandleAliveCheckTimeoutOfTcpConnections(InterfaceIdx);
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
  DoIP_RxTcpRout_HandleAliveCheckTimeoutOfRoutActHandler(InterfaceIdx);
  DoIP_RxTcpRout_HandleInactivityTimeouts(InterfaceIdx);
} /* DoIP_Con_HandleActLineStateActive() */

/**********************************************************************************************************************
 *  DoIP_Con_HandleActLineStateInactive()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Con_HandleActLineStateInactive(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Release all IP address assignments if all socket connections could be closed. */
  if ( DoIP_Con_SoConCloseAll(InterfaceIdx) == E_OK )
  {
    DoIP_Con_RelAllIpAddrAssign(InterfaceIdx); /* PRQA S 2987 */ /* MD_DoIP_Rule2.2_2987 */

    /* #20 Reset activation line inactive time. */
    DoIP_SetActLineInactiveTimeOfInterfaceDyn(InterfaceIdx, 0u);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    /* #30 Set activation line to inactive. */
    DoIP_SetActLineActiveOfInterfaceDyn(InterfaceIdx, FALSE);                                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_DecActLineInactiveRequestCounterOfInterfaceDyn(InterfaceIdx);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

#if ( DOIP_ACT_LINE_STATE_CHG_CBK_SUPPORT == STD_ON )
    /* #40 Notify user about activation line state change to inactive. */
    DoIP_CallActivationLineStateChgDirect((uint8)InterfaceIdx, FALSE);                                                 /* SBSW_DOIP_FUNCTION_POINTER_CALL */
#endif /* DOIP_ACT_LINE_STATE_CHG_CBK_SUPPORT == STD_ON */

    /* #50 Check if activation line is requested to be active again. */
    if ( DoIP_GetActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx) > 0u)
    {
      DoIP_SetActLineActiveOfInterfaceDyn(InterfaceIdx, TRUE);                                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      DoIP_DecActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

#if ( DOIP_ACT_LINE_STATE_CHG_CBK_SUPPORT == STD_ON )
      /* #60 Notify user about activation line state change to active. */
      DoIP_CallActivationLineStateChgDirect((uint8)InterfaceIdx, TRUE);                                                /* SBSW_DOIP_FUNCTION_POINTER_CALL */
#endif /* DOIP_ACT_LINE_STATE_CHG_CBK_SUPPORT == STD_ON */
    }
  }
} /* DoIP_Con_HandleActLineStateInactive() */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleRxIndication()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleRxIndication(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint64 dataOffset = 0u;
  uint64 msgLength;
  uint32 hdrLenField = 0u;
  uint8 reqCnt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all DoIP messages in the UDP frame. */
  while ( PduInfoPtr->SduLength > dataOffset )
  {
    /* #20 Check if the DoIP message is valid. */
    if ( DoIP_RxUdp_CheckGenHdr(ConnectionIdx, PduInfoPtr, &hdrLenField, (uint32)dataOffset) != E_OK )                 /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    {
      break;
    }

    msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;

    reqCnt++;
    /* #30 Check if the DoIP message count per UDP frame does exceeded the configured maximum. */
    if ( reqCnt > DOIP_MAX_UDP_REQ_PER_MSG )
    {
      /* #40 Check if current request is the last one and send general DoIP nack message for each DoIP request. */
      if ( msgLength == (PduInfoPtr->SduLength - dataOffset) )
      { /* It is the last DoIP request. */
        /* #50 Close socket after sending if there is no futher DoIP message and no configured UDP alive timeout. */
        if ( !DoIP_IsUdpAliveTimeoutOfLocalAddr(DoIP_GetLocalAddrIdxOfConnection(ConnectionIdx)) )
        {
          (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_OUT_OF_MEM, TRUE, NULL_PTR);  /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        }
        else
        {
          (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_OUT_OF_MEM, FALSE, NULL_PTR); /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        }
      }
      else
      { /* There are further DoIP requests. */
        (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_OUT_OF_MEM, FALSE, NULL_PTR);   /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      }
    }
    else
    {
      /* #60 Otherwise handle DoIP message. */
      DoIP_RxUdp_HandleRxIndicationMessageTypes(ConnectionIdx, PduInfoPtr, (uint32)dataOffset);                        /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    }

    dataOffset += msgLength;
  }
} /* DoIP_RxUdp_HandleRxIndication() */

/**********************************************************************************************************************
 *  DoIP_RxUdp_CheckGenHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxUdp_CheckGenHdr(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_VAR) HdrLenField,
  uint32 DataOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 hdrVerField;
  uint8 hdrInvVerField;
  uint16 hdrTypeField;
  uint32 hdrLenField;
  uint32_least tmpDataOffset_least;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check message length and close socket connction if message is too short. */
  if ( (PduInfoPtr->SduLength - DataOffset) < DOIP_GEN_HDR_LEN_BYTE )
  {
    DoIP_Con_SoConClose(ConnectionIdx, FALSE);
  }
  else
  {
    /* #20 Otherwise extract generic header data. */
    tmpDataOffset_least = (uint32_least)DataOffset;
    IPBASE_GET_UINT8 (PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrVerField);                                       /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)1u;
    IPBASE_GET_UINT8 (PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrInvVerField);                                    /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)2u;
    IPBASE_GET_UINT16(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrTypeField);                                      /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
    IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                       /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    *HdrLenField = hdrLenField;                                                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

    if ( (((uint8)hdrVerField != DOIP_PROTOCOL_VERSION) ||
          ((uint8)(hdrInvVerField) != DOIP_INVERSE_PROTOCOL_VERSION)) &&
         (((uint8)hdrVerField != DOIP_PROTOCOL_VERSION_DEFAULT) ||
          ((uint8)(hdrInvVerField) != DOIP_INVERSE_PROTOCOL_VERSION_DEFAULT) ||
          (hdrTypeField < DOIP_HDR_TYPE_VEHICLE_IDENT_REQ) ||
          (hdrTypeField > DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_VIN)) )
    {
      /* #30 Check header and send Nack for DoIP version missmatch. */
      (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_FORMAT, TRUE, NULL_PTR);          /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
    }
    else if ( ((uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE) > ((uint64)PduInfoPtr->SduLength - (uint64)DataOffset) )
    {
      /* #40 Check header and send Nack for DoIP for invalid header length. */
      (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, TRUE, NULL_PTR);         /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
    }
    else
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* DoIP_RxUdp_CheckGenHdr() */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleRxIndicationMessageTypes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleRxIndicationMessageTypes(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean closeSocket = FALSE;
  uint32 hdrLenField;
  uint16 hdrTypeField;
  uint32_least tmpDataOffset_least;

  /* ----- Implementation ----------------------------------------------- */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)2u;
  IPBASE_GET_UINT16(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrTypeField);                                        /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
  IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Close socket after sending if there is no further DoIP message and no configured UDP alive timeout. */
  if ( (hdrLenField + DOIP_GEN_HDR_LEN_BYTE) == (PduInfoPtr->SduLength - DataOffset) )
  {
    if ( !DoIP_IsUdpAliveTimeoutOfLocalAddr(DoIP_GetLocalAddrIdxOfConnection(ConnectionIdx)) )
    {
      closeSocket = TRUE;
    }
  }

  /* #20 Handle message depending on the message type. */
  switch(hdrTypeField)
  {
    case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ:
    {
      DoIP_RxUdp_HandleVehicleIdentReq(ConnectionIdx, PduInfoPtr, DataOffset, closeSocket);                            /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      break;
    }
    case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_VIN:
    {
      DoIP_RxUdp_HandleVehicleIdentReqVin(ConnectionIdx, PduInfoPtr, DataOffset, closeSocket);                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      break;
    }
#if ( DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON )
    case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_EID:
    {
      DoIP_RxUdp_HandleVehicleIdentReqEid(ConnectionIdx, PduInfoPtr, DataOffset, closeSocket);                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      break;
    }
#endif /* DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON */
    case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_REQ:
    {
      DoIP_RxUdp_HandleDiagPowerModeInfoReq(ConnectionIdx, PduInfoPtr, DataOffset, closeSocket);                       /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      break;
    }
#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
    case DOIP_HDR_TYPE_ENTITY_STATE_REQ:
    {
      DoIP_RxUdp_HandleEntityStateReq(ConnectionIdx, PduInfoPtr, DataOffset, closeSocket);                             /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      break;
    }
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */
    case DOIP_HDR_TYPE_GEN_NACK:
    case DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT:
    case DOIP_HDR_TYPE_ROUT_ACTIV_REQ:
    case DOIP_HDR_TYPE_ROUT_ACTIV_RES:
    case DOIP_HDR_TYPE_ALIVE_CHECK_REQ:
    case DOIP_HDR_TYPE_ALIVE_CHECK_RES:
    case DOIP_HDR_TYPE_ENTITY_STATE_RES:
    case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES:
    case DOIP_HDR_TYPE_DIAG_REQ:
    case DOIP_HDR_TYPE_DIAG_ACK:
    case DOIP_HDR_TYPE_DIAG_NACK:
    { /* close socket if not relevant header type is received */
      DoIP_Con_SoConClose(ConnectionIdx, FALSE);
      break;
    }
    default:
    { /* TxNack: unknown header type */
      DoIP_RxUdp_HandleUnknownHdrType(ConnectionIdx, PduInfoPtr, DataOffset, closeSocket);                             /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      break;
    }
  }
} /* DoIP_RxUdp_HandleRxIndicationMessageTypes() */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleVehicleIdentReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleVehicleIdentReq(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;
  uint32_least tmpDataOffset_least;

  /* ----- Implementation ----------------------------------------------- */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
  IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    /* #10 Send Nack if max message length exceeded. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else if ( hdrLenField != 0u )
  {
    /* #20 Send Nack if header length check failed. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else
  {
    /* #30 Otherwise send vehicle identification response. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_IDENT_RES_NUM, CloseSocket,      /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      NULL_PTR);
  }
} /* DoIP_RxUdp_HandleVehicleIdentReq() */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleVehicleIdentReqVin()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleVehicleIdentReqVin(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;
  uint32_least tmpDataOffset_least;

  /* ----- Implementation ----------------------------------------------- */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
  IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Send Nack if max message length exceeded. */
  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  /* #20 Send Nack if header length check failed otherwise. */
  else if ( hdrLenField != DOIP_VIN_LEN_BYTE )
  {
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  /* #30 Assume that request is valid otherwise. */
  else
  {
    boolean identOk = TRUE;
    uint8_least identIdx;
    uint32_least tmpIdentIdx;
    uint8 vinBuffer[DOIP_VIN_LEN_BYTE] = { 0u };

    /* #40 Compare VIN to retrieved VIN if VIN could successfully retrieved. */
    if ( DoIP_CallGetVinDirect(vinBuffer) == E_OK )                                                                    /* SBSW_DOIP_FUNCTION_POINTER_CALL */
    {
      for ( identIdx = 0u;
            identIdx < DOIP_VIN_LEN_BYTE;
            identIdx++ )
      {
        tmpIdentIdx = (uint32_least)DataOffset + (uint32_least)DOIP_GEN_HDR_LEN_BYTE + (uint32_least)identIdx;
        if ( PduInfoPtr->SduDataPtr[tmpIdentIdx] != vinBuffer[identIdx] )
        {
          identOk = FALSE;
          break;
        }
      }
    }
    /* #50 Compare VIN to invalid pattern otherwise. */
    else
    {
      for ( identIdx = 0u;
            identIdx < DOIP_VIN_LEN_BYTE;
            identIdx++ )
      {
        tmpIdentIdx = (uint32_least)DataOffset + (uint32_least)DOIP_GEN_HDR_LEN_BYTE + (uint32_least)identIdx;
        if ( PduInfoPtr->SduDataPtr[tmpIdentIdx] != DOIP_VIN_INV_PATTERN )
        {
          identOk = FALSE;
          break;
        }
      }
    }

    /* #60 Send vehicle identification response if VIN check passed. */
    if ( identOk == TRUE )
    {
      (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_IDENT_RES_NUM, CloseSocket,    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        NULL_PTR);
    }
    /* #70 Close socket if requested otherwise. */
    else
    {
      DoIP_TxUdpList_CloseSocket(ConnectionIdx, CloseSocket);
    }
  }
} /* DoIP_RxUdp_HandleVehicleIdentReqVin() */

#if ( DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleVehicleIdentReqEid()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleVehicleIdentReqEid(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;
  uint32_least tmpDataOffset_least;

  /* ----- Implementation ----------------------------------------------- */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
  IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Send Nack if max message length exceeded. */
  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  /* #20 Send Nack if header length check failed otherwise. */
  else if ( hdrLenField != DOIP_EID_LEN_BYTE )
  {
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  /* #30 Assume that request is valid otherwise. */
  else
  {
    boolean identOk = TRUE;
    uint8_least identIdx;
    uint32_least tmpIdentIdx;
    uint8 locEidBuf[DOIP_EID_LEN_BYTE] = { 0u };

    /* #40 Try to get the local EID. */
    if ( DoIP_Util_FillEid(locEidBuf, ConnectionIdx) == E_OK )                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    {
      /* #50 Compare the local EID with the received data. */
      for ( identIdx = 0u;
            identIdx < DOIP_EID_LEN_BYTE;
            identIdx++ )
      {
        tmpIdentIdx = (uint32_least)DataOffset + (uint32_least)DOIP_GEN_HDR_LEN_BYTE + (uint32_least)identIdx;
        if ( PduInfoPtr->SduDataPtr[tmpIdentIdx] != locEidBuf[identIdx] )
        {
          identOk = FALSE;
          break;
        }
      }
    }
    else
    {
      identOk = FALSE;
    }

    /* #60 Send vehicle identification response if EID check passed. */
    if ( identOk == TRUE )
    {
      (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_IDENT_RES_NUM, CloseSocket,    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        NULL_PTR);
    }
    /* #70 Close socket if requested otherwise. */
    else
    {
      DoIP_TxUdpList_CloseSocket(ConnectionIdx, CloseSocket);
    }
  }
} /* DoIP_RxUdp_HandleVehicleIdentReqEid() */
#endif /* DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleDiagPowerModeInfoReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleDiagPowerModeInfoReq(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;
  uint32_least tmpDataOffset_least;

  /* ----- Implementation ----------------------------------------------- */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
  IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    /* #10 Send Nack if max message length exceeded. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else if ( hdrLenField != 0u )
  {
    /* #20 Send Nack if header length check failed. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else
  {
    DoIP_PowerModeType diagPowerMode = DOIP_POWER_MODE_NOT_SUPPORTED;

    /* #30 Otherwise try to retrieve power mode. */
    if ( DoIP_CallPowerModeDirect(&diagPowerMode) != E_OK )                                                            /* SBSW_DOIP_FUNCTION_POINTER_CALL */
    {
      diagPowerMode = DOIP_POWER_MODE_NOT_READY;
    }
    /* #40 Send diagnostic power mode response. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES, diagPowerMode, CloseSocket,       /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      NULL_PTR);
  }
} /* DoIP_RxUdp_HandleDiagPowerModeInfoReq() */

#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleEntityStateReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleEntityStateReq(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;
  uint32_least tmpDataOffset_least;

  /* ----- Implementation ----------------------------------------------- */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
  IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    /* #10 Send Nack if max message length exceeded. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else if ( hdrLenField != 0u )
  {
    /* #20 Send Nack if header length check failed. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, CloseSocket, NULL_PTR);    /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else
  {
    /* #30 Otherwise send entity state response. */
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_ENTITY_STATE_RES, DOIP_UNUSED_PARAM, CloseSocket, NULL_PTR);/* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }
} /* DoIP_RxUdp_HandleEntityStateReq() */
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxUdp_HandleUnknownHdrType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxUdp_HandleUnknownHdrType(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  uint32 DataOffset,
  boolean CloseSocket)
{
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;
  uint16 hdrTypeField;
  PduInfoType rxUserData;
  uint32_least tmpDataOffset_least;
  DoIP_SizeOfInterfaceType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(ConnectionIdx);
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)2u;
  IPBASE_GET_UINT16(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrTypeField);                                        /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  tmpDataOffset_least = (uint32_least)DataOffset + (uint32_least)4u;
  IPBASE_GET_UINT32(PduInfoPtr->SduDataPtr, tmpDataOffset_least, hdrLenField);                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  rxUserData.SduLength = (PduLengthType)hdrLenField;

  /* #10 Check header length and set payload data. */
  if ( hdrLenField == 0u )
  {
    rxUserData.SduDataPtr = NULL_PTR;
  }
  else
  {
    rxUserData.SduDataPtr = &PduInfoPtr->SduDataPtr[DataOffset + DOIP_GEN_HDR_LEN_BYTE];
  }

  /* #20 Check if Rx callback is configured. */
  if ( DoIP_IsOemPayloadRxCbkConfigUsedOfInterface(interfaceIdx) )
  {
    /* #200 Forward received data to the configured callback. */
    retVal = DoIP_GetOemPayloadRxCbkOfOemPayloadRxCbkConfig(DoIP_GetOemPayloadRxCbkConfigIdxOfInterface(interfaceIdx)) /* SBSW_DOIP_FUNCTION_POINTER_CALL */
      (DoIP_GetConnectionIdOfConnection(ConnectionIdx), hdrTypeField, &rxUserData,
        DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_UDP);
  }

  /* #30 Send a generic Nack in case no Rx callback is configured or in case it returned with E_NOT_OK. */
  if ( retVal != E_OK )
  {
    (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, CloseSocket, NULL_PTR);       /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
#else
  /* #40 Send a generic Nack (if OEM payload type is not enabled). */
  DOIP_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT(DataOffset); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  (void)DoIP_TxUdp_Transmit(ConnectionIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, CloseSocket, NULL_PTR);         /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
} /* DoIP_RxUdp_HandleUnknownHdrType() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitVehAn()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdp_TransmitVehAn(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)  /* PRQA S 3673 */ /* MD_DoIP_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(ConnectionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request transmission of vehicle announcement. */
  if ( DoIP_TxUdp_TransmitDelayed(ConnectionIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_UNUSED_PARAM,                /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
    DoIP_GetVehAnCntOfInterface(interfaceIdx), FALSE, NULL_PTR) == E_OK )
  {
    /* #20 If successful, reset pending vehicle announcement flag. */
    DoIP_SetPendingVehAnOfConnectionUdpVehAnDyn(DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx), FALSE);          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
  }
  else
  {
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
    /* buffer overflow after socket connection setup; should not happen */
    /* #30 Otherwise, set error Id (if enabled). */
    *ErrorIdPtr = DOIP_E_NOBUFS;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#else
    DOIP_DUMMY_STATEMENT(ErrorIdPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  }
} /* DoIP_TxUdp_TransmitVehAn() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_Transmit(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint16 Param,
  boolean CloseSocket,
  P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, DOIP_APPL_VAR) RemoteAddress)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket connection shall stay active. */
  if ( DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(DoIP_GetInterfaceIdxOfConnection(ConnectionIdx)) == 0u )
  {
    retVal = DoIP_TxUdp_TransmitImmediately(ConnectionIdx, HdrType, DOIP_UNUSED_PARAM, Param, CloseSocket);

    /* #20 Queue element if message could not be sent immediately. */
    if ( retVal != E_OK )
    {
      retVal = DoIP_TxUdp_TransmitDelayed(ConnectionIdx, HdrType, DOIP_UNUSED_PARAM, Param, CloseSocket,               /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
        RemoteAddress);
    }
  }

  return retVal;
} /* DoIP_TxUdp_Transmit() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitImmediately()
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
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_TransmitImmediately(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint32 Param1,
  uint16 Param2,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to send messages (except vehicle announcements) immediately. */
  if ( HdrType != DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT )
  {
    retVal = DoIP_TxUdp_SingleTransmit(ConnectionIdx, HdrType, Param1, Param2);
  }

  /* #20 Check if message has been sent immediately. */
  if ( retVal == E_OK )
  {
    boolean otherEntryFound = FALSE;
    DoIP_TxUdpMgtListIterType txUdpMgtListIdx;

    /* #30 check if response is pending for this socket connection. */
    for ( txUdpMgtListIdx = 0u;
          txUdpMgtListIdx < DoIP_GetSizeOfTxUdpMgtList();
          txUdpMgtListIdx++ )
    {
      if ( ConnectionIdx == DoIP_GetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx) )
      { /* response pending */
        otherEntryFound = TRUE;
        break;
      }
    }

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
    /* #40 Check if OEM specific payload type buffer has to be released (if enabled). */
    if ( HdrType == DOIP_HDR_TYPE_OEM_SPECIFIC )
    {
      DoIP_TxOem_ReleaseBuf((DoIP_SizeOfOemPayloadTxBufConfigType)Param1);
    }
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

    if ( otherEntryFound == FALSE )
    {
      /* #50 Close socket connection directly if there is no pending response and a close socket is indicated. */
      if ( CloseSocket == TRUE )
      {
        DoIP_Con_SoConClose(ConnectionIdx, FALSE);
      }
    }
    else
    {
      /* #60 Mark socket connection for closing if there are pending elements in the list. */
      DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx,                                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        (boolean)(DoIP_IsSoConClosingOfConnectionDyn(ConnectionIdx) || (CloseSocket == TRUE)));
    }
  }

  return retVal;
} /* DoIP_TxUdp_TransmitImmediately() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitDelayed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_TransmitDelayed(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint32 Param1,
  uint16 Param2,
  boolean CloseSocket,
  P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, DOIP_APPL_VAR) RemoteAddress)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_TxUdpMgtListIterType txUdpMgtListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to find an empty list element. */
  DOIP_BEGIN_CRITICAL_SECTION(); /* protects ConnectionIdxOfTxUdpMgtList */
  for ( txUdpMgtListIdx = 0u;
        txUdpMgtListIdx < DoIP_GetSizeOfTxUdpMgtList();
        txUdpMgtListIdx++ )
  {
    if ( DoIP_GetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx) == DoIP_GetSizeOfConnection() )
    {
      /* #20 Write element in Tx queue if an empty list element was found. */
      DoIP_SetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx, ConnectionIdx);                                             /* SBSW_DOIP_CSL01 */
      DoIP_SetHdrTypeOfTxUdpMgtList(txUdpMgtListIdx, HdrType);                                                         /* SBSW_DOIP_CSL01 */
      DoIP_SetRetryAttemptOfTxUdpMgtList(txUdpMgtListIdx, DOIP_UDP_MAX_RETRY_CNT);                                     /* SBSW_DOIP_CSL01 */

      /* #30 Add message type specific parameter to the Tx queue element. */
      if ( HdrType == DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT )
      {
        DoIP_TxUdp_TransmitDelayedSetVehAnnSpecParams(ConnectionIdx, txUdpMgtListIdx, RemoteAddress);                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      }
      else
      {
        /* OEM buffer index,... */
        DoIP_SetParam1OfTxUdpMgtList(txUdpMgtListIdx, Param1);                                                         /* SBSW_DOIP_CSL01 */
      }

      /* number of announcements, nack code, power mode, OEM specific header type... */
      DoIP_SetParam2OfTxUdpMgtList(txUdpMgtListIdx, Param2);                                                           /* SBSW_DOIP_CSL01 */

      DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx,                                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        (boolean)(DoIP_IsSoConClosingOfConnectionDyn(ConnectionIdx) || (CloseSocket == TRUE)));

      retVal = E_OK;
      break;
    }
  }
  DOIP_END_CRITICAL_SECTION();

  return retVal;
} /* DoIP_TxUdp_TransmitDelayed() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_TransmitDelayedSetVehAnnSpecParams()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdp_TransmitDelayedSetVehAnnSpecParams(
  DoIP_SizeOfConnectionType ConnectionIdx,
  DoIP_TxUdpMgtListIterType TxUdpListIdx,
  P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, DOIP_APPL_VAR) RemoteAddress)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the timer for vehicle announcement messages. */
  DoIP_SetParam1OfTxUdpMgtList(TxUdpListIdx,                                                                           /* SBSW_DOIP_PARAM_VALID_INDEX */
    DoIP_GetVehAnInitWaitOfInterface(DoIP_GetInterfaceIdxOfConnection(ConnectionIdx)));

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  /* #20 Check if the remote address is already known. */
  if ( RemoteAddress != NULL_PTR )
  {
    /* #30 Set the remote address for Tx, if it is already known. */
    DoIP_SetRemoteAddrOfTxUdpMgtList(TxUdpListIdx, *RemoteAddress);                                                    /* SBSW_DOIP_PARAM_VALID_INDEX */
  }
  else
  {
    /* #40 Set the remote address for Tx to general broadcast, if it is unknown. */
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
    if ( DoIP_GetDomainTypeOfLocalAddr(DoIP_GetLocalAddrIdxOfConnection(ConnectionIdx)) == SOAD_AF_INET6 )
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
    {
      DoIP_SetRemoteAddrOfTxUdpMgtList(TxUdpListIdx, DoIP_IpV6AddrMulticast);                                          /* SBSW_DOIP_PARAM_VALID_INDEX */
    }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
    else
#  else
    DOIP_DUMMY_STATEMENT(ConnectionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
# endif /* DOIP_IPV6_ENABLED == STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
    {
      DoIP_SetRemoteAddrOfTxUdpMgtList(TxUdpListIdx, DoIP_IpV4AddrBroadcast);                                          /* SBSW_DOIP_PARAM_VALID_INDEX */
    }
#  if ( DOIP_IPV6_ENABLED == STD_OFF )
    DOIP_DUMMY_STATEMENT(ConnectionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* DOIP_IPV6_ENABLED == STD_OFF */
# endif /* DOIP_IPV4_ENABLED == STD_ON */
  }
#else
  DOIP_DUMMY_STATEMENT(ConnectionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT(RemoteAddress); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */
} /* DoIP_TxUdp_TransmitDelayedSetVehAnnSpecParams() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmit(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint32 Param1,
  uint16 Param2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Assert that no other transmission context is currently active on the connection to avoid reentrant calls to
   *     SoAd_IfTransmit. */
  if ( DoIP_TxUdp_CheckAndSetTxActive(ConnectionIdx) == E_OK )
  {
    /* #20 Handle transmit depending on the header type. */
    switch(HdrType)
    {
      case DOIP_HDR_TYPE_GEN_NACK:
      {
        retVal = DoIP_TxUdp_SingleTransmitGenNack(ConnectionIdx, HdrType, (uint8)Param2);
        break;
      }
      case DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT:
      {
        retVal = DoIP_TxUdp_SingleTransmitVehicleAnnounceAndIdentRes(ConnectionIdx);
        break;
      }
      case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES:
      {
        retVal = DoIP_TxUdp_SingleTransmitDiagPowerModeInfoRes(ConnectionIdx, (uint8)Param2);
        break;
      }
#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
      case DOIP_HDR_TYPE_ENTITY_STATE_RES:
      {
        retVal = DoIP_TxUdp_SingleTransmitEntityStateRes(ConnectionIdx);
        break;
      }
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
      case DOIP_HDR_TYPE_OEM_SPECIFIC:
      {
        retVal = DoIP_TxUdp_SingleTransmitOemSpecific(ConnectionIdx, (DoIP_SizeOfOemPayloadTxBufConfigType)Param1,
          Param2);
        break;
      }
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
      default:
      { /* function does not support this header type */
        /* invalid state */
        /* nothing to do here */
        break;
      }
    }

    /* #30 Indicate that transmission is no longer active. */
    DoIP_TxUdp_ResetTxActive(ConnectionIdx);
  }

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_OFF )
  DOIP_DUMMY_STATEMENT(Param1); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_OFF */

  return retVal;
} /* DoIP_TxUdp_SingleTransmit() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitGenNack()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitGenNack(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrType,
  uint8 NackCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  PduInfoType msgStruct;
  uint8 data[9u] = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare message. */
  msgStruct.SduLength  = DOIP_GEN_HDR_LEN_BYTE + 1u;
  msgStruct.SduDataPtr = &data[0u];
  msgStruct.SduDataPtr[0u] = DOIP_PROTOCOL_VERSION;                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[1u] = DOIP_INVERSE_PROTOCOL_VERSION;                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  DoIP_Util_PutUInt16(&msgStruct.SduDataPtr[2u], 0u, HdrType);                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  msgStruct.SduDataPtr[4u] = 0u;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[5u] = 0u;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[6u] = 0u;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[7u] = 1u;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[8u] = NackCode;                                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
  /* #20 Increment the count of received UDP packets dropped by DoIP. */
  DoIP_Measure_IncDroppedPacketCounter(DOIP_MEASURE_DROPPED_PACKET_COUNTER_UDP, HdrType, NackCode);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

  /* #30 Send message. */
  retVal = SoAd_IfTransmit(DoIP_GetSoAdTxPduIdOfConnection(ConnectionIdx), &msgStruct);                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  return retVal;
} /* DoIP_TxUdp_SingleTransmitGenNack() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitVehicleAnnounceAndIdentRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitVehicleAnnounceAndIdentRes(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduInfoType msgStruct;
  uint16 ackIdx = DOIP_GEN_HDR_LEN_BYTE;
  uint8 data[DOIP_GEN_HDR_LEN_BYTE + DOIP_ACK_VEH_IDENT_MAX_LEN_BYTE];
  uint32 ackVehIdentLen;
#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
  Std_ReturnType retValVin;
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write VIN, logical address, EID, GID and central security to message buffer. */
  /* fill VIN */
#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
  retValVin = DoIP_Util_FillVin(&data[ackIdx]);                                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
#else
  (void)DoIP_Util_FillVin(&data[ackIdx]);                                                                              /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */
  ackIdx += DOIP_VIN_LEN_BYTE;
  /* fill Addr */
  DoIP_Util_FillLocalLogAddr(&data[ackIdx]);                                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  ackIdx += 2u;
  /* fill EID */
  retVal |= DoIP_Util_FillEid(&data[ackIdx], ConnectionIdx); /* PRQA S 2986 */ /* MD_DoIP_Rule2.2_2985_2986 */         /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  ackIdx += DOIP_EID_LEN_BYTE;
  /* fill GID */
  retVal |= DoIP_Util_FillGid(&data[ackIdx], ConnectionIdx); /* PRQA S 2985, 2986 */ /* MD_DoIP_Rule2.2_2985_2986 */   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  ackIdx += DOIP_GID_LEN_BYTE;
  /* fill Central Security */
  DoIP_Util_FillFurtherAction(&data[ackIdx], ConnectionIdx);                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

  /* #20 Write VinGidSync state to message buffer, if required. */
  if ( DoIP_IsUseVehIdentSyncStatusOfInterface(DoIP_GetInterfaceIdxOfConnection(ConnectionIdx)) )
  {
    ackVehIdentLen = DOIP_ACK_VEH_IDENT_W_SYNC_STAT_LEN_BYTE;
    ackIdx++;
#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
    if ( (retValVin != E_OK) && (DoIP_GidInSync != TRUE) )
    { /* VIN/GID sync not successful */
      data[ackIdx] = 0x10u;                                                                                            /* SBSW_DOIP_ARRAY_WRITE */
    }
    else
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */
    {
      data[ackIdx] = 0x00u;                                                                                            /* SBSW_DOIP_ARRAY_WRITE */
    }
  }
  else
  {
    ackVehIdentLen = DOIP_ACK_VEH_IDENT_WO_SYNC_STAT_LEN_BYTE;
  }

  /* #30 Check return values. */
  if ( retVal == E_OK )
  {
    /* #40 Write generic header to message buffer. */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, ackVehIdentLen, &data[0u]);                               /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

    /* #50 Send message. */
    msgStruct.SduDataPtr = &data[0u];
    msgStruct.SduLength = (PduLengthType)(DOIP_GEN_HDR_LEN_BYTE + ackVehIdentLen);
    /* PRQA S 2986 1 */ /* MD_DoIP_Rule2.2_2985_2986 */
    retVal |= SoAd_IfTransmit(DoIP_GetSoAdTxPduIdOfConnection(ConnectionIdx), &msgStruct);                             /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  }

  return retVal;
} /* DoIP_TxUdp_SingleTransmitVehicleAnnounceAndIdentRes() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitDiagPowerModeInfoRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitDiagPowerModeInfoRes(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint8 DiagPowerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint16 hdrType = DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES;
  PduInfoType msgStruct;
  uint8 data[9u] = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare message. */
  msgStruct.SduLength  = DOIP_GEN_HDR_LEN_BYTE + 1u;
  msgStruct.SduDataPtr = &data[0u];
  msgStruct.SduDataPtr[0u] = DOIP_PROTOCOL_VERSION;                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[1u] = DOIP_INVERSE_PROTOCOL_VERSION;                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  DoIP_Util_PutUInt16(&msgStruct.SduDataPtr[2u], 0u, hdrType);                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  msgStruct.SduDataPtr[4u] = 0x00u;                                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[5u] = 0x00u;                                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[6u] = 0x00u;                                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[7u] = 0x01u;                                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[8u] = DiagPowerMode;                                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

  /* #20 Send message. */
  retVal = SoAd_IfTransmit(DoIP_GetSoAdTxPduIdOfConnection(ConnectionIdx), &msgStruct);                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  return retVal;
} /* DoIP_TxUdp_SingleTransmitDiagPowerModeInfoRes() */

#if ( DOIP_ENTITY_STATE_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitEntityStateRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitEntityStateRes(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  PduInfoType msgStruct;
  DoIP_SizeOfLocalAddrType localAddrIdx = DoIP_GetLocalAddrIdxOfConnection(ConnectionIdx);
  DoIP_SizeOfConnectionType connectionIdx;
  DoIP_ConnectionIndIterType connectionIndIter;
# if ( DOIP_ENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON )
  uint8 data[15u] = { 0u };
# else
  uint8 data[11u] = { 0u };
# endif /* DOIP_ENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare message. */
  msgStruct.SduLength  = DOIP_GEN_HDR_LEN_BYTE + DOIP_ACK_ENTITY_STATE_LEN_BYTE;
  msgStruct.SduDataPtr = &data[0u];
  msgStruct.SduDataPtr[0u]  = DOIP_PROTOCOL_VERSION;                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[1u]  = DOIP_INVERSE_PROTOCOL_VERSION;                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  DoIP_Util_PutUInt16(&msgStruct.SduDataPtr[2u], 0u, DOIP_HDR_TYPE_ENTITY_STATE_RES);                                  /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  msgStruct.SduDataPtr[4u]  = 0x00u;                                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[5u]  = 0x00u;                                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[6u]  = 0x00u;                                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[7u]  = DOIP_ACK_ENTITY_STATE_LEN_BYTE;                                                          /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[8u]  = DOIP_NODE_TYPE;                                                                          /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[9u]  = DoIP_GetMaxTesterConnectionsOfLocalAddr(localAddrIdx);                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msgStruct.SduDataPtr[10u] = 0u;                                                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

  /* #20 Iterate over all connections of the current local address to count open TCP_DATA sockets. */
  for ( connectionIndIter = DoIP_GetConnectionIndStartIdxOfLocalAddr(localAddrIdx);
        connectionIndIter < DoIP_GetConnectionIndEndIdxOfLocalAddr(localAddrIdx);
        connectionIndIter++ )
  {
    connectionIdx = DoIP_GetConnectionInd(connectionIndIter);

    /* #30 Increase the count of open TCP_DATA sockets if the connection is an open TCP connection. */
    if ( (DoIP_Util_IsTcpConnection(connectionIdx) == TRUE) &&
      (DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE) )
    {
      msgStruct.SduDataPtr[10u]++;                                                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    }
  }

# if ( DOIP_ENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON )
  /* #40 Write max data size to message buffer (if enabled). */
  DoIP_Util_PutUInt32(&msgStruct.SduDataPtr[11u], 0u, DOIP_MAX_REQUEST_BYTES);                                         /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
# endif /* DOIP_ENTITY_STATUS_MAX_BYTE_FIELD_USE == STD_ON */

  /* #50 Send message. */
  retVal = SoAd_IfTransmit(DoIP_GetSoAdTxPduIdOfConnection(ConnectionIdx), &msgStruct);                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  return retVal;
} /* DoIP_TxUdp_SingleTransmitEntityStateRes() */
#endif /* DOIP_ENTITY_STATE_SUPPORT == STD_ON */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxUdp_SingleTransmitOemSpecific()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_SingleTransmitOemSpecific(
  DoIP_SizeOfConnectionType ConnectionIdx,
  DoIP_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx,
  uint16 HdrTypeOem)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  PduInfoType msgStruct;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add the generic header in front of the user data. */
  DoIP_Util_FillGenHdr(HdrTypeOem,                                                                                     /* SBSW_DOIP_OemPayloadTxBuf */
    ((uint32)DoIP_GetLengthOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx) - (uint32)DOIP_GEN_HDR_LEN_BYTE),
    DoIP_GetAddrOemPayloadTxBuf(DoIP_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(OemTxBufCfgIdx)));

  /* #20 Prepare message. */
  msgStruct.SduDataPtr = DoIP_GetAddrOemPayloadTxBuf(DoIP_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(
    OemTxBufCfgIdx));
  msgStruct.SduLength = DoIP_GetLengthOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx);

  /* #30 Send message. */
  retVal = SoAd_IfTransmit(DoIP_GetSoAdTxPduIdOfConnection(ConnectionIdx), &msgStruct);                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  return retVal;
} /* DoIP_TxUdp_SingleTransmitOemSpecific() */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TxUdp_CheckAndSetTxActive()
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
 *
 */
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxUdp_CheckAndSetTxActive(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent interruptions from another transmission context while accessing the flag to
   *     indicate if a transmission is active. */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #20 Check if the connection is a UDP connection. */
  if ( DoIP_Util_IsUdpConnection(ConnectionIdx) == TRUE )
  {
    const DoIP_SizeOfConnectionUdpType connectionUdpIdx = DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

    /* #200 Check if any transmission is currently active on the connection. */
    if ( DoIP_IsTxActiveOfConnectionUdpDyn(connectionUdpIdx) == FALSE )
    {
      /* #2000 Indicate that a transmission is now active on the connection. */
      DoIP_SetTxActiveOfConnectionUdpDyn(connectionUdpIdx, TRUE);                                                      /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL04 */
      retVal = E_OK;
    }
  }
  /* #21 The connection is a UDP vehicle announcement connection otherwise. */
  else
  {
    const DoIP_SizeOfConnectionUdpVehAnType connectionUdpVehAnIdx =
      DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

    /* #210 Check if any transmission is currently active on the connection. */
    if ( DoIP_IsTxActiveOfConnectionUdpVehAnDyn(connectionUdpVehAnIdx) == FALSE )
    {
      /* #2100 Indicate that a transmission is now active on the connection. */
      DoIP_SetTxActiveOfConnectionUdpVehAnDyn(connectionUdpVehAnIdx, TRUE);                                            /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL04 */
      retVal = E_OK;
    }
  }

  /* #30 Leave critical section. */
  DOIP_END_CRITICAL_SECTION();

  return retVal;
} /* DoIP_TxUdp_CheckAndSetTxActive() */

/**********************************************************************************************************************
 *  DoIP_TxUdp_ResetTxActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_TxUdp_ResetTxActive(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the connection is a UDP connection. */
  if ( DoIP_Util_IsUdpConnection(ConnectionIdx) == TRUE )
  {
    const DoIP_SizeOfConnectionUdpType connectionUdpIdx = DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

    /* #100 Indicate that a transmission is no longer active on the connection. */
    DoIP_SetTxActiveOfConnectionUdpDyn(connectionUdpIdx, FALSE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL03 */
  }
  /* #11 The connection is a UDP vehicle announcement connection otherwise. */
  else
  {
    const DoIP_SizeOfConnectionUdpVehAnType connectionUdpVehAnIdx =
      DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

    /* #110 Indicate that a transmission is no longer active on the connection. */
    DoIP_SetTxActiveOfConnectionUdpVehAnDyn(connectionUdpVehAnIdx, FALSE);                                             /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL03 */
  }
} /* DoIP_TxUdp_ResetTxActive() */

/**********************************************************************************************************************
 *  DoIP_TxUdpList_CloseSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_CloseSocket(
  DoIP_SizeOfConnectionType ConnectionIdx,
  boolean CloseSocket)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket connection shall be closed already or is requested to be closed now. */
  if ( DoIP_IsSoConClosingOfConnectionDyn(ConnectionIdx) || CloseSocket )
  {
    /* #20 Indicate that socket shall be closed after all pending Tx UDP list entries has been sent. */
    DoIP_SetSoConClosingOfConnectionDyn(ConnectionIdx, TRUE);                                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    /* #30 Check if other Tx UDP list entries for socket connection exist. */
    if ( DoIP_TxUdpList_CheckForPendingEntry(ConnectionIdx) == FALSE )
    {
      /* #40 Close socket connection if no other entry exists. */
      DoIP_Con_SoConClose(ConnectionIdx, FALSE);
    }
  }
} /* DoIP_TxUdpList_CloseSocket() */

/**********************************************************************************************************************
 *  DoIP_TxUdpList_CheckForPendingEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(boolean, DOIP_CODE) DoIP_TxUdpList_CheckForPendingEntry(
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_TxUdpMgtListIterType txUdpMgtListIdx;
  boolean otherEntryFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if any Tx UDP list entry for socket connection exists. */
  for ( txUdpMgtListIdx = 0u;
        txUdpMgtListIdx < DoIP_GetSizeOfTxUdpMgtList();
        txUdpMgtListIdx++ )
  {
    if ( ConnectionIdx == DoIP_GetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx) )
    {
      otherEntryFound = TRUE;
      break;
    }
  }

  return otherEntryFound;
} /* DoIP_TxUdpList_CheckForPendingEntry() */

/**********************************************************************************************************************
 *  DoIP_TxUdpList_HandleEntries()
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
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_HandleEntries(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_TxUdpMgtListIterType txUdpMgtListIdx;

  /* ----- Implementation ----------------------------------------------- */
  for ( txUdpMgtListIdx = 0u;
        txUdpMgtListIdx < DoIP_GetSizeOfTxUdpMgtList();
        txUdpMgtListIdx++ )
  {
    const DoIP_SizeOfConnectionType connectionIdx = DoIP_GetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx);

    /* #10 Check if there are pending UDP messages. */
    if ( connectionIdx != DoIP_GetSizeOfConnection() )
    {
      boolean releaseListEntry = FALSE;

      /* #20 Check socket connection state and header type. */
      if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE )
      {
        if ( DoIP_GetHdrTypeOfTxUdpMgtList(txUdpMgtListIdx) == DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT )
        {
          /* #30 Handle vehicle announcement and vehicle identification response. */
          DoIP_TxUdpList_HandleEntriesVehicleAnnounceAndIdentRes((DoIP_SizeOfTxUdpMgtListType)txUdpMgtListIdx,
            connectionIdx);
        }
        else
        {
          /* #40 Handle entity state status response, diagnostic power mode response, generic negative acknowledge. */
          /* #50 Mark entry for release if list reach max size or entry could be send. */
          DoIP_TxUdpList_HandleEntriesOthers((DoIP_SizeOfTxUdpMgtListType)txUdpMgtListIdx, connectionIdx,              /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
            &releaseListEntry);
        }
      }
      else
      {
        /* #60 Mark entry for release if socket connection is not online anymore. */
        releaseListEntry = TRUE;
      }

      /* #70 Check if entry is marked for release. */
      if ( releaseListEntry == TRUE )
      {
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
        /* #80 Check if OEM specific payload type buffer has to be released. */
        if ( DoIP_GetHdrTypeOfTxUdpMgtList(txUdpMgtListIdx) == DOIP_HDR_TYPE_OEM_SPECIFIC )
        {
          DoIP_TxOem_ReleaseBuf((DoIP_SizeOfOemPayloadTxBufConfigType)DoIP_GetParam1OfTxUdpMgtList(txUdpMgtListIdx));
        }
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

        /* #90 Release entry and close socket. */
        DoIP_SetConnectionIdxOfTxUdpMgtList(txUdpMgtListIdx, DoIP_GetSizeOfConnection());                              /* SBSW_DOIP_CSL01 */
        DoIP_TxUdpList_CloseSocket(connectionIdx, FALSE);
      }
    }
  }
} /* DoIP_TxUdpList_HandleEntries() */

/**********************************************************************************************************************
 *  DoIP_TxUdpList_HandleEntriesVehicleAnnounceAndIdentRes()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_HandleEntriesVehicleAnnounceAndIdentRes(
  DoIP_SizeOfTxUdpMgtListType TxUdpListIdx,
  DoIP_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  SoAd_SockAddrInetXType remAddressToAssign;
  Std_ReturnType retVal = E_OK;
#else
  Std_ReturnType retVal;
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  DoIP_DecParam1OfTxUdpMgtList(TxUdpListIdx);                                                                          /* SBSW_DOIP_PARAM_VALID_INDEX */

  /* #10 Try to send vehicle announcement if announce interval timer is expired. */
  if ( DoIP_GetParam1OfTxUdpMgtList(TxUdpListIdx) == 0u )
  {
#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
    /* #20 Set Remote Address if it is a vehicle announcement connection. */
    if ( (ConnectionIdx <  DoIP_GetSizeOfConnection()) &&
         (DoIP_Util_IsUdpVehAnConnection(ConnectionIdx) == TRUE) )
    {
      remAddressToAssign = DoIP_GetRemoteAddrOfTxUdpMgtList(TxUdpListIdx);

      /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
      retVal = SoAd_SetRemoteAddr(DoIP_GetSoConIdOfConnectionDyn(ConnectionIdx),                                       /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        DOIP_A_P2VAR(SoAd_SockAddrType)&remAddressToAssign);
    }

    if ( retVal == E_OK )
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */
    {
      /* #30 Try to send out the vehicle announcement. */
      retVal = DoIP_TxUdp_SingleTransmit(ConnectionIdx, DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT, DOIP_UNUSED_PARAM,
        DOIP_UNUSED_PARAM);
    }

    if ( retVal != E_OK )
    {
      /* #40 Requeue entry if transmission failed. */
      DoIP_IncParam1OfTxUdpMgtList(TxUdpListIdx);                                                                      /* SBSW_DOIP_PARAM_VALID_INDEX */
      DoIP_DecRetryAttemptOfTxUdpMgtList(TxUdpListIdx);                                                                /* SBSW_DOIP_PARAM_VALID_INDEX */
      if ( DoIP_GetRetryAttemptOfTxUdpMgtList(TxUdpListIdx) == 0u )
      {
        /* #50 Release entry if retry failed several times. */
        DoIP_SetConnectionIdxOfTxUdpMgtList(TxUdpListIdx, DoIP_GetSizeOfConnection());                                 /* SBSW_DOIP_PARAM_VALID_INDEX */
      }
    }
    else
    {
      DoIP_DecParam2OfTxUdpMgtList(TxUdpListIdx);                                                                      /* SBSW_DOIP_PARAM_VALID_INDEX */
      if ( DoIP_GetParam2OfTxUdpMgtList(TxUdpListIdx) > 0u )
      {
        /* #60 Otherwise reload announce interval timer if further vehicle announcement messages are pending. */
        DoIP_SetParam1OfTxUdpMgtList(TxUdpListIdx,                                                                     /* SBSW_DOIP_PARAM_VALID_INDEX */
          DoIP_GetVehAnIntervalOfInterface(DoIP_GetInterfaceIdxOfConnection(ConnectionIdx)));
      }
      else
      {
        /* #70 Release entry and close socket if all pending messages for this entry are sent. */
        DoIP_SetConnectionIdxOfTxUdpMgtList(TxUdpListIdx, DoIP_GetSizeOfConnection());                                 /* SBSW_DOIP_PARAM_VALID_INDEX */
        if ( DoIP_Util_IsUdpConnection(ConnectionIdx) == TRUE )
        {
          DoIP_TxUdpList_CloseSocket(ConnectionIdx, FALSE);
        }
      }
    }
  }
} /* DoIP_TxUdpList_HandleEntriesVehicleAnnounceAndIdentRes() */

/**********************************************************************************************************************
 *  DoIP_TxUdpList_HandleEntriesOthers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxUdpList_HandleEntriesOthers(
  DoIP_SizeOfTxUdpMgtListType TxUdpListIdx,
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) ReleaseListEntry)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to send message. */
  if ( DoIP_TxUdp_SingleTransmit(ConnectionIdx, DoIP_GetHdrTypeOfTxUdpMgtList(TxUdpListIdx),
        DoIP_GetParam1OfTxUdpMgtList(TxUdpListIdx), DoIP_GetParam2OfTxUdpMgtList(TxUdpListIdx)) != E_OK )
  {
    DoIP_DecRetryAttemptOfTxUdpMgtList(TxUdpListIdx);                                                                  /* SBSW_DOIP_PARAM_VALID_INDEX */
    if ( DoIP_GetRetryAttemptOfTxUdpMgtList(TxUdpListIdx) == 0u )
    {
      /* #20 Release entry if retry failed several times. */
      *ReleaseListEntry = TRUE;                                                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    }
  }
  else
  {
    /* #30 Release entry if successfully send. */
    *ReleaseListEntry = TRUE;                                                                                          /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
} /* DoIP_TxUdpList_HandleEntriesOthers() */

/**********************************************************************************************************************
 *  DoIP_TxTcp_Transmit()
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
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxTcp_Transmit(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrType,
  uint32 Param1,
  uint32 Param2,
  uint32 Param3,
  SduDataPtrType DataPtr,
  boolean CloseSocket)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #10 Check if socket connection is online. */
  if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE )
  {
    /* #20 Check if socket connection shall stay active. */
    if ( !DoIP_IsSoConClosingOfConnectionDyn(connectionIdx) &&
      (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(DoIP_GetInterfaceIdxOfConnection(connectionIdx)) == 0u) )
    {
#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
      /* #30 Increment the count of received TCP packets dropped by DoIP. */
      DoIP_Measure_IncDroppedPacketCounter(DOIP_MEASURE_DROPPED_PACKET_COUNTER_TCP, HdrType, (uint8)Param3);
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

      /* #40 Add element to Tx queue. */
      retVal = DoIP_TxTcpQueue_Add(ConnectionTcpIdx, HdrType, Param1, Param2, Param3, DataPtr);                        /* SBSW_DOIP_PARAM_VALID_POINTER_AND_NULL_POINTER_FORWARD */

      /* #50 Check if element was added successfully. */
      if ( retVal == E_OK )
      {
        if ( CloseSocket == TRUE )
        {
          /* #60 Mark socket for closing if close socket is indicated. */
          DoIP_SetSoConClosingOfConnectionDyn(connectionIdx, TRUE);                                                    /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
        }

        DOIP_END_CRITICAL_SECTION();

        /* #70 Start transmission of Tx queue. */
        DoIP_TxTcpQueue_TransmitElement(ConnectionTcpIdx);
      }
      else
      {
        DOIP_END_CRITICAL_SECTION();

#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
        /* no memory in TxQueue available */
        errorId = DOIP_E_NOBUFS;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
      }
    }
    else
    {
      DOIP_END_CRITICAL_SECTION();
    }
  }
  else
  {
    DOIP_END_CRITICAL_SECTION();
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_VTCP_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TxTcp_Transmit() */

/**********************************************************************************************************************
 *  DoIP_TxTcp_PrepareTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcp_PrepareTransmit(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrType,
  uint16 Param1,
  uint32 Param2,
  uint8  Param3,
  boolean CloseSocket)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_GetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_HDR_TYPE_INVALID )
  {
    /* #10 Store transmission parameter if there are no pending prepare transmit request. */
    DoIP_SetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx, HdrType);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetTpTxPreParam1OfConnectionTcpDyn(ConnectionTcpIdx, Param1);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetTpTxPreParam2OfConnectionTcpDyn(ConnectionTcpIdx, Param2);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetTpTxPreParam3OfConnectionTcpDyn(ConnectionTcpIdx, Param3);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetTpTxPreCloseSocketOfConnectionTcpDyn(ConnectionTcpIdx, CloseSocket);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
} /* DoIP_TxTcp_PrepareTransmit() */

/**********************************************************************************************************************
 *  DoIP_TxTcp_FinishTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcp_FinishTransmit(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( DoIP_GetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx) != DOIP_HDR_TYPE_INVALID )
  {
    /* #10 Transmit message if there is a pending transmit request. */
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx,                                                                        /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      DoIP_GetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx),
      DoIP_GetTpTxPreParam1OfConnectionTcpDyn(ConnectionTcpIdx),
      DoIP_GetTpTxPreParam2OfConnectionTcpDyn(ConnectionTcpIdx),
      DoIP_GetTpTxPreParam3OfConnectionTcpDyn(ConnectionTcpIdx),
      NULL_PTR,
      DoIP_GetTpTxPreCloseSocketOfConnectionTcpDyn(ConnectionTcpIdx));
    DoIP_SetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx, DOIP_HDR_TYPE_INVALID);                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
} /* DoIP_TxTcp_FinishTransmit() */

/**********************************************************************************************************************
 *  DoIP_TxTcp_ConfirmTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcp_ConfirmTransmit(
  DoIP_SizeOfChannelType ChannelIdx,
  DoIP_PduRTxConfPduIdOfChannelType TxConfPduId,
  Std_ReturnType Result)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IF API type is used on the channel. */
#if ( DOIP_IF_API_ENABLED == STD_ON )
  if ( DoIP_GetApiTypeOfChannel(ChannelIdx) == DOIP_API_TYPE_IF )
  {
# if ( DOIP_PDUR_IF_TXCONF_ENABLED == STD_ON )
    /* #20 Call IF TxConfirmation if it is enabled for IF channel. */
    if ( TxConfPduId != DOIP_NO_PDURTXCONFPDUIDOFCHANNEL )
    {
      PduR_DoIPIfTxConfirmation(TxConfPduId);
    }
# endif /* DOIP_PDUR_IF_TXCONF_ENABLED == STD_ON */
  }
  /* #11 Call TP TxConfirmation otherwise. */
  else
#endif /* DOIP_IF_API_ENABLED == STD_ON */
  {
    PduR_DoIPTpTxConfirmation(TxConfPduId, Result);
  }

#if ( DOIP_IF_API_ENABLED == STD_OFF )
  DOIP_DUMMY_STATEMENT(ChannelIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_IF_API_ENABLED == STD_OFF */
} /* DoIP_TxTcp_ConfirmTransmit() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_Add()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_Add(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrType,
  uint32 Param1,
  uint32 Param2,
  uint32 Param3,
  SduDataPtrType DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_SizeOfTxTcpMgtType txTcpMgtQueIdx = DoIP_GetTxTcpMgtQueIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_SizeOfTxTcpMgtType txTcpMgtQueLvl = DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_SizeOfTxTcpMgtType txTcpMgtSize = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Tx queue can handle this request. */
  if ( txTcpMgtQueLvl < txTcpMgtSize )
  {
    /* #20 Find next free entry in Tx queue. */
    if ( (txTcpMgtQueIdx + txTcpMgtQueLvl) >= txTcpMgtSize )
    {
      /* #30 Handle wrap around. */
      txTcpMgtQueIdx = (DoIP_SizeOfTxTcpMgtType)(txTcpMgtQueLvl - (txTcpMgtSize - txTcpMgtQueIdx));
    }
    else
    {
      txTcpMgtQueIdx = (DoIP_SizeOfTxTcpMgtType)(txTcpMgtQueIdx + txTcpMgtQueLvl);
    }

    /* #40 Add entry depending on the header type. */
    switch(HdrType)
    {
      case DOIP_HDR_TYPE_GEN_NACK:
      {
        DoIP_TxTcpQueue_AddGenNack(txTcpMgtQueIdx, ConnectionTcpIdx, (uint8)Param1);
        break;
      }
      case DOIP_HDR_TYPE_ROUT_ACTIV_RES:
      {
        DoIP_TxTcpQueue_AddRoutingActivationRes(txTcpMgtQueIdx, ConnectionTcpIdx, (uint16)Param1, (uint8)Param2,
          (DoIP_SizeOfTesterType)Param3);
        break;
      }
      case DOIP_HDR_TYPE_ALIVE_CHECK_REQ:
      {
        DoIP_TxTcpQueue_AddAliveCheckReq(txTcpMgtQueIdx, ConnectionTcpIdx);
        break;
      }
      case DOIP_HDR_TYPE_DIAG_REQ:
      {
        /* Param2 can be casted to PduLengthType because it was a PduLengthType before. */
        DoIP_TxTcpQueue_AddDiagReq(txTcpMgtQueIdx, ConnectionTcpIdx, (DoIP_SizeOfChannelType)Param1,                   /* SBSW_DOIP_PARAM_VALID_POINTER_AND_NULL_POINTER_FORWARD */
          (PduLengthType)Param2, DataPtr);
        break;
      }
      case DOIP_HDR_TYPE_DIAG_ACK:
      {
        DoIP_TxTcpQueue_AddDiagAck(txTcpMgtQueIdx, ConnectionTcpIdx, (uint16)Param1);
        break;
      }
      case DOIP_HDR_TYPE_DIAG_NACK:
      {
        DoIP_TxTcpQueue_AddDiagNack(txTcpMgtQueIdx, ConnectionTcpIdx, (uint16)Param1, (uint16)Param2, (uint8)Param3);
        break;
      }
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
      case DOIP_HDR_TYPE_OEM_SPECIFIC:
      {
        DoIP_TxTcpQueue_AddOem(txTcpMgtQueIdx, ConnectionTcpIdx, (uint16)Param1,
          (DoIP_SizeOfOemPayloadTxBufConfigType)Param2);
        break;
      }
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
      default:
      { /* function does not support this header type */
        /* invalid state */
        /* nothing to do here */
        break;
      }
    }

    /* #50 Update Tx queue. */
    DoIP_IncTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    retVal = E_OK;
  }

  return retVal;
} /* DoIP_TxTcpQueue_Add() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddGenNack()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddGenNack(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint8 NackCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
  PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write DoIP header to Tx buffer. */
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_HDR_NACK_CODE_LEN_BYTE, txHdrBufPtr);                          /* SBSW_DOIP_TxHdrBuf_FORWARD */

  /* #20 Write nack code to Tx buffer. */
  txHdrBufPtr[DOIP_GEN_HDR_LEN_BYTE] = NackCode;                                                                       /* SBSW_DOIP_TxHdrBuf */

  /* #30 Set Tx management struct. */
  DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, (DOIP_GEN_HDR_LEN_BYTE + DOIP_GEN_HDR_NACK_CODE_LEN_BYTE));            /* SBSW_DOIP_TxTcpMgt */
  msg->SduLength = DoIP_GetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx);                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msg->SduDataPtr = txHdrBufPtr;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_TxTcpQueue_AddGenNack() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddRoutingActivationRes()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddRoutingActivationRes(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr,
  uint8 AckCode,
  DoIP_SizeOfTesterType TesterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  const DoIP_RoutingActivationIdxOfConnectionTcpDynType routActIdx =
    DoIP_GetRoutingActivationIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
  PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write DoIP header to Tx buffer. */
  DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, TesterAddr);                                                           /* SBSW_DOIP_TxHdrBuf_FORWARD */
  bufIdx += DOIP_ADDR_LEN_BYTE;
  DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, DoIP_Util_GetEntityAddr());                                            /* SBSW_DOIP_TxHdrBuf_FORWARD */
  bufIdx += DOIP_ADDR_LEN_BYTE;
  txHdrBufPtr[bufIdx] = AckCode;                                                                                       /* SBSW_DOIP_TxHdrBuf */
  bufIdx++;
  txHdrBufPtr[bufIdx] = 0u;                                                                                            /* SBSW_DOIP_TxHdrBuf */
  bufIdx++;
  txHdrBufPtr[bufIdx] = 0u;                                                                                            /* SBSW_DOIP_TxHdrBuf */
  bufIdx++;
  txHdrBufPtr[bufIdx] = 0u;                                                                                            /* SBSW_DOIP_TxHdrBuf */
  bufIdx++;
  txHdrBufPtr[bufIdx] = 0u;                                                                                            /* SBSW_DOIP_TxHdrBuf */
  bufIdx++;

  if ( TesterIdx == DoIP_GetSizeOfTester() )
  {
    /* #20 Fill generic header (without OEM specific data) if tester is unknown. */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE, txHdrBufPtr);                 /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  else if ( routActIdx == DoIP_GetSizeOfRoutingActivation() )
  {
    /* #30 Otherwise fill generic header (without OEM specific data) if routing activation is unknown. */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE, txHdrBufPtr);                 /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  else if ( ((DoIP_GetAuthResLengthOfRoutingActivation(routActIdx) == 0u) &&
             (DoIP_GetConfResLengthOfRoutingActivation(routActIdx) == 0u)) ||
             (DoIP_GetActivStateOfConnectionTcpDyn(ConnectionTcpIdx) != DOIP_ROUT_ACTIV_STATE_ACTIVATED) )
  {
    /* #40 Otherwise fill generic header (without OEM specific data) if routing activation is needed and pending. */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE, txHdrBufPtr);                 /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  else if ( TesterAddr != DoIP_GetTesterAddrOfConnectionTcpDyn(ConnectionTcpIdx) )
  {
    /* #50 Otherwise fill generic header (without OEM specific data) if tester is different. */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MIN_LEN_BYTE, txHdrBufPtr);                 /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  else
  {
    uint32 oemSpecRes = DoIP_GetOemSpecResOfRoutActivHandler(0u);

    /* #60 Otherwise fill generic header with OEM specific data. */
    DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ROUT_ACTIV_RES, DOIP_ROUT_ACTIV_ACK_MAX_LEN_BYTE, txHdrBufPtr);                 /* SBSW_DOIP_TxHdrBuf_FORWARD */

    DoIP_Util_PutUInt32(&txHdrBufPtr[bufIdx], 0u, oemSpecRes);                                                         /* SBSW_DOIP_TxHdrBuf_FORWARD */
    bufIdx += 4u;
  }

  /* #70 Set Tx management struct. */
  DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, bufIdx);                                                               /* SBSW_DOIP_TxTcpMgt */
  msg->SduLength = bufIdx;                                                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msg->SduDataPtr = txHdrBufPtr;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_TxTcpQueue_AddRoutingActivationRes() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddAliveCheckReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddAliveCheckReq(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
  PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write DoIP header to Tx buffer. */
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_ALIVE_CHECK_REQ, 0u, txHdrBufPtr);                                                /* SBSW_DOIP_TxHdrBuf_FORWARD */

  /* #20 Set Tx management struct. */
  DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, DOIP_GEN_HDR_LEN_BYTE);                                                /* SBSW_DOIP_TxTcpMgt */
  msg->SduLength = DOIP_GEN_HDR_LEN_BYTE;                                                                              /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msg->SduDataPtr = txHdrBufPtr;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_TxTcpQueue_AddAliveCheckReq() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddDiagReq()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddDiagReq(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfChannelType ChannelIdx,
  PduLengthType Length,
  SduDataPtrType DataPtr) /* PRQA S 3673 */ /* MD_DoIP_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint16 ecuAddr = DoIP_Util_GetEcuAddr(ChannelIdx);
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_TesterAddrOfTesterType testerAddr = DoIP_GetTesterAddrOfTester(testerIdx);
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
  PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;
  uint16 payloadType = DOIP_HDR_TYPE_DIAG_REQ;

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_IF_API_ENABLED == STD_ON )
  if ( DoIP_GetApiTypeOfChannel(ChannelIdx) == DOIP_API_TYPE_IF )
  {
    /* #10 Copy data to IF Tx buffer if API type is IF. */
    /* PRQA S 0310, 0314 2 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)DoIP_GetAddrIfTxBuf(DoIP_GetIfTxBufStartIdxOfChannel(ChannelIdx)),                /* SBSW_DOIP_IfTxBuf_AND_CONST_POINTER_FORWARD */
      DOIP_A_P2CONST(void)DataPtr, Length);

    /* #20 Set the configured payload type if API type is IF. */
    payloadType = DOIP_PAYLOAD_TYPE_IF;
  }
#else
  DOIP_DUMMY_STATEMENT(DataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_IF_API_ENABLED == STD_ON */

  /* #30 Write DoIP header to Tx buffer. */
  DoIP_Util_FillGenHdr(payloadType, (DOIP_DIAG_HDR_LEN_BYTE + (uint32)Length), txHdrBufPtr);                           /* SBSW_DOIP_TxHdrBuf_FORWARD */

  DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, ecuAddr);                                                              /* SBSW_DOIP_TxHdrBuf_FORWARD */
  bufIdx += DOIP_ADDR_LEN_BYTE;
#if ( DOIP_DEFAULT_TESTER_ENABLED == STD_ON )
  if ( testerAddr == DOIP_ADDR_ANY )
  {
    /* #40 Add tester default address to Tx buffer if default address is used (if enabeld). */
    const DoIP_TesterAddrOfConnectionTcpDynType testerAddrOfConnection =
      DoIP_GetTesterAddrOfConnectionTcpDyn(ConnectionTcpIdx);
    DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, testerAddrOfConnection);                                             /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  else
#endif /* DOIP_DEFAULT_TESTER_ENABLED == STD_ON */
  {
    /* #50 Otherwise add tester address to Tx buffer. */
    DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, testerAddr);                                                         /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  bufIdx += DOIP_ADDR_LEN_BYTE;

  /* #60 Set Tx management struct. */
  DoIP_SetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx, (DoIP_TxRequestHandleType)ChannelIdx);                             /* SBSW_DOIP_TxTcpMgt */
  DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, bufIdx);                                                               /* SBSW_DOIP_TxTcpMgt */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
  DoIP_SetOemSpecificOfTxTcpMgtDyn(txTcpMgtIdx, FALSE);                                                                /* SBSW_DOIP_TxTcpMgt */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
  msg->SduLength = bufIdx + Length;                                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#if ( DOIP_IF_API_ENABLED == STD_ON )
  if ( DoIP_GetApiTypeOfChannel(ChannelIdx) == DOIP_API_TYPE_IF )
  {
    msg->SduDataPtr = DataPtr;                                                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
  else
#endif /* DOIP_IF_API_ENABLED == STD_ON */
  {
    msg->SduDataPtr = txHdrBufPtr;                                                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
} /* DoIP_TxTcpQueue_AddDiagReq() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddDiagAck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddDiagAck(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 EcuAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_TesterAddrOfTesterType testerAddr = DoIP_GetTesterAddrOfTester(testerIdx);
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  const DoIP_DiagNAckOffsetOfTesterDynType diagNAckOffset = DoIP_GetDiagNAckOffsetOfTesterDyn(testerIdx);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
  PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write DoIP header with nack offset to Tx buffer. */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_ACK, DOIP_DIAG_MSG_ACK_LEN_BYTE + diagNAckOffset, txHdrBufPtr);              /* SBSW_DOIP_TxHdrBuf_FORWARD */
#else
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_ACK, DOIP_DIAG_MSG_ACK_LEN_BYTE, txHdrBufPtr);                               /* SBSW_DOIP_TxHdrBuf_FORWARD */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

  DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, EcuAddr);                                                              /* SBSW_DOIP_TxHdrBuf_FORWARD */
  bufIdx += DOIP_ADDR_LEN_BYTE;
#if ( DOIP_DEFAULT_TESTER_ENABLED == STD_ON )
  if ( testerAddr == DOIP_ADDR_ANY )
  {
    /* #20 Add tester default address to Tx buffer if default address is used (if enabeld). */
    const DoIP_TesterAddrOfConnectionTcpDynType testerAddrOfConnection =
      DoIP_GetTesterAddrOfConnectionTcpDyn(ConnectionTcpIdx);
    DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, testerAddrOfConnection);                                             /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  else
#endif /* DOIP_DEFAULT_TESTER_ENABLED == STD_ON */
  {
    /* #30 Otherwise add tester address to Tx buffer. */
    DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, testerAddr);                                                         /* SBSW_DOIP_TxHdrBuf_FORWARD */
  }
  bufIdx += DOIP_ADDR_LEN_BYTE;
  txHdrBufPtr[bufIdx] = DOIP_DIAG_MSG_RES_CODE_ACK;                                                                    /* SBSW_DOIP_TxHdrBuf */
  bufIdx++;

  /* #40 Set Tx management struct. */
  DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, bufIdx);                                                               /* SBSW_DOIP_TxTcpMgt */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  msg->SduLength = (PduLengthType)(bufIdx + diagNAckOffset);                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#else
  msg->SduLength = bufIdx;                                                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  msg->SduDataPtr = txHdrBufPtr;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_TxTcpQueue_AddDiagAck() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddDiagNack()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddDiagNack(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 EcuAddr,
  uint16 TesterAddr,
  uint8 NackCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
  PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);
  PduLengthType bufIdx = DOIP_GEN_HDR_LEN_BYTE;
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  DoIP_DiagNAckOffsetOfTesterDynType diagNAckOffset;
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write DoIP header to Tx buffer. */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  /* Write DoIP header with nack offset to Tx buffer if tester is known. */
  diagNAckOffset = DoIP_GetDiagNAckOffsetOfTesterDyn(testerIdx);
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_NACK, DOIP_DIAG_MSG_ACK_LEN_BYTE + diagNAckOffset, txHdrBufPtr);             /* SBSW_DOIP_TxHdrBuf_FORWARD */
#else
  DoIP_Util_FillGenHdr(DOIP_HDR_TYPE_DIAG_NACK, DOIP_DIAG_MSG_ACK_LEN_BYTE, txHdrBufPtr);                              /* SBSW_DOIP_TxHdrBuf_FORWARD */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

  DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, EcuAddr);                                                              /* SBSW_DOIP_TxHdrBuf_FORWARD */
  bufIdx += DOIP_ADDR_LEN_BYTE;

  DoIP_Util_PutUInt16(&txHdrBufPtr[bufIdx], 0u, TesterAddr);                                                           /* SBSW_DOIP_TxHdrBuf_FORWARD */
  bufIdx += DOIP_ADDR_LEN_BYTE;

  txHdrBufPtr[bufIdx] = NackCode;                                                                                      /* SBSW_DOIP_TxHdrBuf */
  bufIdx += 1u;

  /* #20 Set Tx management struct. */
  DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, bufIdx);                                                               /* SBSW_DOIP_TxTcpMgt */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  /* Prepare diagnostic message negative ack with user data if tester is known. */
  diagNAckOffset = DoIP_GetDiagNAckOffsetOfTesterDyn(testerIdx);
  msg->SduLength = (PduLengthType)(bufIdx + diagNAckOffset);                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#else
  /* Prepare diagnostic message negative ack. */
  msg->SduLength = bufIdx;                                                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  msg->SduDataPtr = txHdrBufPtr;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_TxTcpQueue_AddDiagNack() */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_AddOem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_AddOem(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 HdrTypeOem,
  DoIP_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
  DoIP_LengthOfOemPayloadTxBufConfigDynType msgLength = DoIP_GetLengthOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx);
  PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write DoIP header to Tx buffer. */
  DoIP_Util_FillGenHdr(HdrTypeOem, msgLength, txHdrBufPtr);                                                            /* SBSW_DOIP_TxHdrBuf_FORWARD */

  /* #20 Set Tx management struct. */
  DoIP_SetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx, OemTxBufCfgIdx);                                                   /* SBSW_DOIP_TxTcpMgt */
  DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, DOIP_GEN_HDR_LEN_BYTE);                                                /* SBSW_DOIP_TxTcpMgt */
  DoIP_SetOemSpecificOfTxTcpMgtDyn(txTcpMgtIdx, TRUE);                                                                 /* SBSW_DOIP_TxTcpMgt */
  msg->SduLength = ((PduLengthType)DOIP_GEN_HDR_LEN_BYTE + (PduLengthType)msgLength);                                  /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  msg->SduDataPtr = txHdrBufPtr;                                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
} /* DoIP_TxTcpQueue_AddOem() */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_Copy()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_Copy(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  const DoIP_SizeOfTxTcpMgtType txTcpMgtQueIdx = DoIP_GetTxTcpMgtQueIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + txTcpMgtQueIdx);
  const DoIP_TxHdrBufLenOfTxTcpMgtDynType txHdrBufLen = DoIP_GetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx);
  const DoIP_TxBytesCopiedOfConnectionTcpDynType txBytesCopied =
    DoIP_GetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx);
  const PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);
  boolean hdrCopied = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #10 Check if TP session is online. */
  if ( !DoIP_IsTxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx) )
  { /* TP session is not online */
    /* nothing to do here */
  }
  else
#if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
  /* #20 Check if TP is not canceled (if enabled). */
  if ( DoIP_IsTxTpCancelRequestedOfConnectionTcpDyn(ConnectionTcpIdx) )
  { /* TP cancellation is ongoing */
    /* nothing to do here */
  }
  else
#endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */
  /* #30 Check length and if this is a subsequent call of CopyTxData. */
  if ( (PduInfoPtr->SduLength != 0u) &&
       (PduInfoPtr->SduLength > DoIP_GetLastTxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx)) )
  { /* unexpected length */
    /* nothing to do here */
  }
  else
  {
    if ( DoIP_GetActivStateOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
    {
      /* #40 Reset inactivity timer if routing activation is active. */
      DoIP_SetInactivCntOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                      /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }

    if ( txBytesCopied < txHdrBufLen )
    {
      /* #50 Check if header is not yet copied. */
      retVal = DoIP_TxTcpQueue_CopyHeader(txTcpMgtQueIdx, ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr, &hdrCopied); /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    }
    else
    {
      retVal = BUFREQ_OK;
    }

    if ( (retVal == BUFREQ_OK) &&
         (txHdrBufLen <= txBytesCopied) &&
         (txHdrBufLen < msg->SduLength) )
    {
      /* #60 Copy data if header was (already) copied successfuly and more data are available. */
      retVal = DoIP_TxTcpQueue_CopyData(txTcpMgtQueIdx, ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr, hdrCopied);    /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    }
  }

  DOIP_END_CRITICAL_SECTION();

  return retVal;
} /* DoIP_TxTcpQueue_Copy() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyHeader()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyHeader(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_OK;
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  const DoIP_TxHdrBufLenOfTxTcpMgtDynType txHdrBufLen = DoIP_GetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u);
  DoIP_TxBytesCopiedOfConnectionTcpDynType txBytesCopied = DoIP_GetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if this is a subsequent call of CopyTxData. */
  if ( PduInfoPtr->SduLength != 0u )
  {
    /* #20 Check requested length. */
    if ( PduInfoPtr->SduLength <= ((PduLengthType)(txHdrBufLen - txBytesCopied)) )
    {
      /* #30 Copy requested number of bytes to target buffer if length is valid. */
      /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
      VStdLib_MemCpy(DOIP_A_P2VAR(void)PduInfoPtr->SduDataPtr, DOIP_A_P2CONST(void)&txHdrBufPtr[txBytesCopied],        /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
         PduInfoPtr->SduLength);

      txBytesCopied += PduInfoPtr->SduLength;
      DoIP_SetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx, txBytesCopied);                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

      /* #40 Check if header was copied completely. */
      if ( txBytesCopied == txHdrBufLen )
      {
        uint16 hdrTypeField;

        IPBASE_GET_UINT16(&txHdrBufPtr[DOIP_GEN_HDR_TYPE_IDX], 0u, hdrTypeField);                                      /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

        /* #50 Check if Routing Activation Response shall be sent while confirmation is not pending. In case of
         *     confirmation a Routing Activation Response might be sent even if Routing Activation is not finished. */
        if ( (hdrTypeField == DOIP_HDR_TYPE_ROUT_ACTIV_RES) && (connectionTcpIdx != DoIP_GetSizeOfConnectionTcp()) &&
             (DoIP_GetActivStateOfConnectionTcpDyn(connectionTcpIdx) != DOIP_ROUT_ACTIV_STATE_CONF_PENDING) )
        {
          /* #60 Release Routing Activation Handler. */
          DoIP_SetConnectionTcpIdxOfRoutActivHandler(0u, DoIP_GetSizeOfConnectionTcp());                               /* SBSW_DOIP_RoutActivHandler */
        }

        *HdrCopied = TRUE;                                                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      }
    }
    else
    { /* invalid length */
      retVal = BUFREQ_E_NOT_OK;
    }
  }

  if ( retVal == BUFREQ_OK )
  {
    /* #70 Set available data length if header was (already) copied successfully. */
    *AvailableDataPtr = (PduLengthType)(txHdrBufLen - txBytesCopied);                                                  /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastTxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *AvailableDataPtr);                                      /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

  return retVal;
} /* DoIP_TxTcpQueue_CopyHeader() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyData(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  const DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
  /* #10 Copy Oem specific data in case the queued message is Oem specific. */
  if ( DoIP_IsOemSpecificOfTxTcpMgtDyn(txTcpMgtIdx) == TRUE )
  {
    DoIP_TxTcpQueue_CopyOemSpecific(QueueIdx, ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);              /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    retVal = BUFREQ_OK;
  }
  /* #20 Check the header type otherwise. */
  else
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
  {
    uint16 hdrTypeField;

    /* #30 Get the header type of the PDU and map it to the correct type in case of IF-API. */
    IPBASE_GET_UINT16(&txHdrBufPtr[DOIP_GEN_HDR_TYPE_IDX], 0u, hdrTypeField);                                          /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

#if ( DOIP_IF_API_ENABLED == STD_ON )
    if ( hdrTypeField == DOIP_PAYLOAD_TYPE_IF )
    {
      hdrTypeField = DOIP_HDR_TYPE_DIAG_REQ;
    }
#endif /* DOIP_IF_API_ENABLED == STD_ON */

    /* #40 Copy data depending on the header type. */
    switch(hdrTypeField)
    {
      case DOIP_HDR_TYPE_DIAG_REQ:
      {
        retVal = DoIP_TxTcpQueue_CopyDiagReq(QueueIdx, ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);     /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        break;
      }
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
      case DOIP_HDR_TYPE_DIAG_ACK:
      case DOIP_HDR_TYPE_DIAG_NACK:
      {
        retVal = DoIP_TxTcpQueue_CopyDiagAck(QueueIdx, ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);     /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        break;
      }
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
      default:
      {
        retVal = BUFREQ_E_NOT_OK;
        break;
      }
    }
  }

  return retVal;
} /* DoIP_TxTcpQueue_CopyData() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagReq(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;
#if ( DOIP_IF_API_ENABLED == STD_ON )
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
#endif /* DOIP_IF_API_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy data depending on there API type. */
#if ( DOIP_IF_API_ENABLED == STD_ON )
  if ( DoIP_GetApiTypeOfChannel((DoIP_SizeOfChannelType)DoIP_GetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx)) ==
    DOIP_API_TYPE_IF )
  {
    DoIP_TxTcpQueue_CopyDiagReqIF(QueueIdx, ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);                /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    retVal = BUFREQ_OK;
  }
  else
#endif /* DOIP_IF_API_ENABLED == STD_ON */
  {
    retVal = DoIP_TxTcpQueue_CopyDiagReqTP(QueueIdx, ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr, HdrCopied);       /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  return retVal;
} /* DoIP_TxTcpQueue_CopyDiagReq() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagReqTP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagReqTP(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;
  P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) locPduInfoPtr = PduInfoPtr;
  PduInfoType locPduInfoStruct;
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  const DoIP_TxBytesCopiedOfConnectionTcpDynType txBytesCopied =
    DoIP_GetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Query available upper layer buffer if header was copied successfully. */
  if ( HdrCopied == TRUE )
  {
    locPduInfoStruct.SduLength = 0u;
    locPduInfoStruct.SduDataPtr = NULL_PTR;
    locPduInfoPtr = &locPduInfoStruct;
  }

  /* #20 Call upper layer CopyTxData function to query available buffer or copying. */
  retVal = PduR_DoIPTpCopyTxData(DoIP_GetPduRTxConfPduIdOfChannel((DoIP_SizeOfChannelType)                             /* SBSW_DOIP_PARAM_VALID_POINTER_AND_NULL_POINTER_FORWARD */
    DoIP_GetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx)), locPduInfoPtr, NULL_PTR, AvailableDataPtr);

  /* #30 Indicate failure if Tx queue has been modifed. */
  if ( DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx) == 0u )
  {
    retVal = BUFREQ_E_NOT_OK;
  }

  if ( retVal == BUFREQ_OK )
  {
    /* #40 Update Tx management struct if buffer request was succesful. */
    DoIP_SetLastTxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *AvailableDataPtr);                                      /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx, (txBytesCopied + locPduInfoPtr->SduLength));             /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
  else
  {
    DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);

    /* #50 Close connection since generic header has already been sent if buffer request failed. */
#if ( DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON )
    DoIP_Con_SoConClose(connectionIdx, FALSE);
#else
    DoIP_Con_SoConClose(connectionIdx, TRUE);
#endif /* DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON */

    retVal = BUFREQ_E_NOT_OK;
  }

  return retVal;
} /* DoIP_TxTcpQueue_CopyDiagReqTP() */

#if ( DOIP_IF_API_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagReqIF()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagReqIF(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxBytesCopiedOfConnectionTcpDynType txBytesCopied = DoIP_GetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx);
  const PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if header is not yet copied. */
  if ( (PduInfoPtr->SduLength != 0u) && (HdrCopied == FALSE) )
  {
    const PduLengthType bufIdx = (PduLengthType)(txBytesCopied - DoIP_GetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx));

    /* #20 Copy data from internal buffer to target buffer. */
    /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)PduInfoPtr->SduDataPtr, DOIP_A_P2CONST(void)&(DoIP_GetAddrIfTxBuf(                /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
      DoIP_GetIfTxBufStartIdxOfChannel((DoIP_SizeOfChannelType)DoIP_GetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx))))
      [bufIdx], (uint32)PduInfoPtr->SduLength);

    txBytesCopied += PduInfoPtr->SduLength;
    DoIP_SetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx, txBytesCopied);                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

  /* #30 Set available data length. */
  *AvailableDataPtr = (PduLengthType)(msg->SduLength - txBytesCopied);                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  DoIP_SetLastTxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *AvailableDataPtr);                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
} /* DoIP_TxTcpQueue_CopyDiagReqIF() */
#endif /* DOIP_IF_API_ENABLED == STD_ON */

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyDiagAck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_TxTcpQueue_CopyDiagAck(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_OK;
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxBytesCopiedOfConnectionTcpDynType txBytesCopied = DoIP_GetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx);
  const PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if header is not yet copied. */
  if ( (PduInfoPtr->SduLength != 0u) && (HdrCopied == FALSE) )
  {
    /* #20 Copy data from internal buffer to target buffer. */
    /* PRQA S 0310, 0314 1 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)PduInfoPtr->SduDataPtr, DOIP_A_P2CONST(void)DoIP_GetAddrDiagNAckBuf(              /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
      DoIP_GetDiagNAckBufStartIdxOfTester(testerIdx) +
      (txBytesCopied - DoIP_GetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx))), PduInfoPtr->SduLength);

    txBytesCopied += PduInfoPtr->SduLength;
    DoIP_SetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx, txBytesCopied);                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    /* #30 Update control data structs if all data are copied to lower layer buffer. */
    if ( txBytesCopied == msg->SduLength )
    {
      DoIP_SetDiagNAckOffsetOfTesterDyn(testerIdx, 0u);                                                                /* SBSW_DOIP_RegisterTester */
    }
  }

  /* #40 Set available data length. */
  *AvailableDataPtr = (PduLengthType)(msg->SduLength - txBytesCopied);                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  DoIP_SetLastTxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *AvailableDataPtr);                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

  return retVal;
} /* DoIP_TxTcpQueue_CopyDiagAck() */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_CopyOemSpecific()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_CopyOemSpecific(
  DoIP_SizeOfTxTcpMgtType QueueIdx,
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr,
  boolean HdrCopied)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + QueueIdx);
  DoIP_TxBytesCopiedOfConnectionTcpDynType txBytesCopied = DoIP_GetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx);
  const PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if header is not yet copied. */
  if ( (PduInfoPtr->SduLength != 0u) && (HdrCopied == FALSE) )
  {
    const DoIP_SizeOfOemPayloadTxBufConfigType oemTxBufCfgIdx =
      (DoIP_SizeOfOemPayloadTxBufConfigType)DoIP_GetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx);
    const DoIP_SizeOfOemPayloadTxBufType oemTxBufIdx =
      (DoIP_SizeOfOemPayloadTxBufType)(DoIP_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(oemTxBufCfgIdx) +
      (DoIP_SizeOfOemPayloadTxBufType)(txBytesCopied - DoIP_GetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx)));                /* PRQA S 4391 */ /* MD_DoIP_GenericWiderTypeCast */

    /* #20 Copy data from internal buffer to target buffer. */
    /* PRQA S 0310, 0314 2 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)PduInfoPtr->SduDataPtr, DOIP_A_P2CONST(void)DoIP_GetAddrOemPayloadTxBuf(          /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
      oemTxBufIdx), PduInfoPtr->SduLength);

    txBytesCopied += PduInfoPtr->SduLength;
    DoIP_SetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx, txBytesCopied);                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    /* #30 Release the buffer. */
    if ( txBytesCopied == msg->SduLength )
    {
      DoIP_TxOem_ReleaseBuf(oemTxBufCfgIdx);
    }
  }

  /* #40 Set available data length. */
  *AvailableDataPtr = (PduLengthType)(msg->SduLength - txBytesCopied);                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  DoIP_SetLastTxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *AvailableDataPtr);                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
} /* DoIP_TxTcpQueue_CopyOemSpecific() */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_TransmitElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_TransmitElement(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTxTcpMgtType txTcpMgtQueIdx = DoIP_GetTxTcpMgtQueIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + txTcpMgtQueIdx);
  const PduInfoType *msg = DoIP_GetAddrMsgOfTxTcpMgtDyn(txTcpMgtIdx);

  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #10 Check if session is not yet active. */
  if ( !DoIP_IsTxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx) )
  {
    /* #20 Lock session since this function can be called reentrant. */
    DoIP_SetTxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    /* #30 Check if there are pendig elements. */
    if ( DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx) > 0u )
    {
      PduInfoType txTcpMgtMsg;
      DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);

      txTcpMgtMsg.SduDataPtr = msg->SduDataPtr;
      txTcpMgtMsg.SduLength = msg->SduLength;

      DOIP_END_CRITICAL_SECTION();

      if ( SoAd_TpTransmit(DoIP_GetSoAdTxPduIdOfConnection(connectionIdx), &txTcpMgtMsg) != E_OK )                     /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      {
        /* #40 Deaktivate session if transmission request failed. */
        DoIP_SetTxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }
    else
    {
      /* #50 Otherwise release lock. */
      DoIP_SetTxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                            /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

      DOIP_END_CRITICAL_SECTION();
    }
  }
  else
  {
    DOIP_END_CRITICAL_SECTION();
  }
} /* DoIP_TxTcpQueue_TransmitElement() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_RemoveElement()
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
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_RemoveElement(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  Std_ReturnType Result)
{
  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #10 Check if there are pending elements. */
  if ( DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx) > 0u )
  {
    const DoIP_SizeOfTxTcpMgtType txTcpMgtQueIdx = DoIP_GetTxTcpMgtQueIdxOfConnectionTcpDyn(ConnectionTcpIdx);
    const DoIP_SizeOfTxTcpMgtType txTcpMgtIdx = (DoIP_SizeOfTxTcpMgtType)(
      DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx) + txTcpMgtQueIdx);
    const uint32 handleIdx = DoIP_GetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx);
    const DoIP_TxHdrBufType *txHdrBufPtr = DoIP_GetAddrTxHdrBuf(DoIP_GetTxHdrBufStartIdxOfTxTcpMgt(txTcpMgtIdx));
    const DoIP_SizeOfTxTcpMgtType txTcpMgtSize = (DoIP_SizeOfTxTcpMgtType)(
      DoIP_GetTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
      DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx));
    uint16 hdrTypeField;

    /* #20 Reset Tx management structs. */
    DoIP_SetTxHdrBufLenOfTxTcpMgtDyn(txTcpMgtIdx, 0u);                                                                 /* SBSW_DOIP_TxTcpMgt */

    /* #30 Update Tx queue. */
    DoIP_DecTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_IncTxTcpMgtQueIdxOfConnectionTcpDyn(ConnectionTcpIdx);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    if ( DoIP_GetTxTcpMgtQueIdxOfConnectionTcpDyn(ConnectionTcpIdx) == txTcpMgtSize )
    {
      /* #40 Handle wrap around. */
      DoIP_SetTxTcpMgtQueIdxOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                  /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }

    /* #50 Finish TP session. */
    DoIP_SetTxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    DoIP_SetTxBytesCopiedOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    /* #60 Get the header type of the PDU. */
    IPBASE_GET_UINT16(&txHdrBufPtr[DOIP_GEN_HDR_TYPE_IDX], 0u, hdrTypeField);                                          /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

    /* #70 Check if the queued element is an OEM specific transmission request. */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
    if ( DoIP_IsOemSpecificOfTxTcpMgtDyn(txTcpMgtIdx) == TRUE )
    {
      /* #700 Release OEM specific payload type buffer. */
      DoIP_TxOem_ReleaseBuf((DoIP_SizeOfOemPayloadTxBufConfigType)handleIdx);

      DoIP_SetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx, 0u);                                                           /* SBSW_DOIP_TxTcpMgt */
      DoIP_SetOemSpecificOfTxTcpMgtDyn(txTcpMgtIdx, FALSE);                                                            /* SBSW_DOIP_TxTcpMgt */

      DOIP_END_CRITICAL_SECTION();
    }
    /* #71 Check if the queued element is a diagnostic request otherwise. */
    else
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
    if ( (hdrTypeField == DOIP_HDR_TYPE_DIAG_REQ)
#if ( DOIP_IF_API_ENABLED == STD_ON )
      || (hdrTypeField == DOIP_PAYLOAD_TYPE_IF)
#endif /* DOIP_IF_API_ENABLED == STD_ON */
      )
    {
      const DoIP_SizeOfChannelType channelIdx = (DoIP_SizeOfChannelType)handleIdx;
      const DoIP_PduRTxConfPduIdOfChannelType txConfPduId = DoIP_GetPduRTxConfPduIdOfChannel(channelIdx);

      DoIP_SetTxMsgLenOfChannelDyn(channelIdx, 0u);                                                                    /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      DoIP_SetTxRequestHandleOfTxTcpMgtDyn(txTcpMgtIdx, 0u);                                                           /* SBSW_DOIP_TxTcpMgt */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
      DoIP_SetOemSpecificOfTxTcpMgtDyn(txTcpMgtIdx, FALSE);                                                            /* SBSW_DOIP_TxTcpMgt */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

      DOIP_END_CRITICAL_SECTION();

      /* #710 Call TxConfirmation depending on the API type. */
      DoIP_TxTcp_ConfirmTransmit(channelIdx, txConfPduId, Result);
    }
    else
    {
      DOIP_END_CRITICAL_SECTION();
    }
  }
  else
  {
    DOIP_END_CRITICAL_SECTION();
  }
} /* DoIP_TxTcpQueue_RemoveElement() */

/**********************************************************************************************************************
 *  DoIP_TxTcpQueue_HandleTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxTcpQueue_HandleTransmit(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionTcpIterType connectionTcpIdx;

  /* ----- Implementation ----------------------------------------------- */
  for ( connectionTcpIdx = DoIP_GetConnectionTcpStartIdxOfInterface(InterfaceIdx);
        connectionTcpIdx < DoIP_GetConnectionTcpEndIdxOfInterface(InterfaceIdx);
        connectionTcpIdx++ )
  {
    DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx);

    /* #10 Check for all connections if there are online. */
    if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE )
    {
#if ( DOIP_TP_OPTIMIZED_ENABLED == STD_ON )
      uint8_least txIdx;

      /* #20 Handle configured number of transmission requests within one main function cycle (if enabled). */
      for ( txIdx = 0u;
            txIdx < DOIP_TCP_TX_QUEUE_PROCESS_LIMIT;
            txIdx++ )
#endif /* DOIP_TP_OPTIMIZED_ENABLED == STD_ON */
      {
        const DoIP_SizeOfTxTcpMgtType txTcpMgtQueLvl = DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(connectionTcpIdx);

        if ( txTcpMgtQueLvl > 0u )
        {
          /* #30 Try to transmit element if there are pending elements. */
          DoIP_TxTcpQueue_TransmitElement((DoIP_SizeOfConnectionTcpType)connectionTcpIdx);
        }

#if ( DOIP_TP_OPTIMIZED_ENABLED == STD_ON )
        if ( (DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(connectionTcpIdx) == 0u) ||
             (DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(connectionTcpIdx) == txTcpMgtQueLvl) )
        {
          /* #40 Break if queue is empty or queue level has not been changed (if enabled). */
          break;
        }
#endif /* DOIP_TP_OPTIMIZED_ENABLED == STD_ON */
      }
    }
  }
} /* DoIP_TxTcpQueue_HandleTransmit() */

/**********************************************************************************************************************
 *  DoIP_RxTcpGen_CopyGenHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpGen_CopyGenHdr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy generic header if not yet copied. */
  if ( rxMsgOffset < DOIP_GEN_HDR_LEN_BYTE )
  {
    const uint32 bytesToCopy = PduInfoPtr->SduLength;

    /* PRQA S 0310, 0314 2 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx) +  /* SBSW_DOIP_RxHdrBuf_AND_CONST_POINTER_FORWARD */
      rxMsgOffset), DOIP_A_P2CONST(void)PduInfoPtr->SduDataPtr, bytesToCopy);

    rxMsgOffset += bytesToCopy;
    *ReadDataLenPtr += (PduLengthType)bytesToCopy;                                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, rxMsgOffset);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

  /* #20 Check if generic header is received completly and set buffer size. */
  if ( rxMsgOffset >= DOIP_GEN_HDR_LEN_BYTE )
  {
    retVal = E_OK;
    *BufferSizePtr = 0u;                                                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
  else
  {
    *BufferSizePtr = (PduLengthType)(DOIP_GEN_HDR_LEN_BYTE - DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx)); /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

  return retVal;
} /* DoIP_RxTcpGen_CopyGenHdr() */

/**********************************************************************************************************************
 *  DoIP_RxTcpGen_CopyDataToHeaderBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpGen_CopyDataToHeaderBuffer(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr,
  uint32 BytesToCopy)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
  uint32 tmpBytesToCopy = BytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check and set max bytes to copy. */
  if ( tmpBytesToCopy > ((uint32)(PduInfoPtr->SduLength) - (uint32)(*ReadDataLenPtr)) )
  {
    tmpBytesToCopy = ((uint32)(PduInfoPtr->SduLength) - (uint32)(*ReadDataLenPtr));
  }

  /* #20 Copy data to header buffer if there are bytes to copy. */
  if ( tmpBytesToCopy > 0u )
  {
    /* PRQA S 0310, 0314 2 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx) +  /* SBSW_DOIP_RxHdrBuf_AND_CONST_POINTER_FORWARD */
      rxMsgOffset), DOIP_A_P2CONST(void)&PduInfoPtr->SduDataPtr[*ReadDataLenPtr], tmpBytesToCopy);

    *ReadDataLenPtr += (PduLengthType)tmpBytesToCopy;                                                                  /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, (rxMsgOffset + tmpBytesToCopy));                           /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
} /* DoIP_RxTcpGen_CopyDataToHeaderBuffer() */

/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_RxTcp_HandleCopyRxData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  const DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check plausibility of all input parameters and states. */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
  /* Check initialization state */
  /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
  if ( DoIP_State != DOIP_STATE_INIT )
  { /* DoIP_State is not INIT (it could be SHUTDOWN) */
    /* Continue if DoIP_State is shutdown (if enabled). */
    /* nothing to do here */
  }
  else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
  /* Check socket connection state */
  if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) != SOAD_SOCON_ONLINE )
  { /* socket connection is not online */
    /* nothing to do here */
  }
  /* Check TP session state */
  else if ( !DoIP_IsRxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx) )
  { /* TP session is not active */
    /* nothing to do here */
  }
  /* Check if socket connection is requested to be closed */
  else if ( DoIP_IsSoConClosingOfConnectionDyn(connectionIdx) )
  { /* socket connection closing is requested - do not handle more than pending Rx data of message */
    *BufferSizePtr = 0u;                                                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    retVal = BUFREQ_OK;
  }
#if ( DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON )
  /* Check TP cancellation */
  else if ( DoIP_IsRxTpCancelRequestedOfConnectionTcpDyn(ConnectionTcpIdx) )
  { /* TP cancellation is ongoing */
    /* nothing to do here */
  }
#endif /* DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON */
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
  /* the feature to send previously received user data cannot be used if tester could not be determined */
  else if ( (testerIdx != DoIP_GetSizeOfTester()) && (rxMsgOffset == 0u) &&
            (DoIP_GetDiagNAckOffsetOfTesterDyn(testerIdx) > 0u) )
  { /* diagnostic message acknowledge is pending - do not accept new data until acknowledge is sent */
    *BufferSizePtr = 0u;                                                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    retVal = BUFREQ_OK;
  }
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  /* SoAd queries Rx buffer size */
  else if ( (PduInfoPtr->SduLength == 0u) && (rxMsgOffset < DOIP_GEN_HDR_LEN_BYTE) )
  { /* generic header was not completely received */
    /* do only provide buffer for generic header */
    *BufferSizePtr = (PduLengthType)(DOIP_GEN_HDR_LEN_BYTE - rxMsgOffset);                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    retVal = BUFREQ_OK;
  }
  /* SoAd has Rx data */
  else if ( (PduInfoPtr->SduLength != 0u) &&
            (PduInfoPtr->SduDataPtr == NULL_PTR) )
  {
    *ErrorIdPtr = DOIP_E_INVALID_PARAMETER;                                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    /* nothing to do here */
  }
  /* SoAd has Rx data AND check length */
  else if ( (PduInfoPtr->SduLength != 0u) &&
            (PduInfoPtr->SduLength > DoIP_GetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx)) )
  { /* unexpected length */
    /* nothing to do here */
  }
  else
  {
    /* #20 Process request.*/
    DoIP_RxTcp_HandleCopyRxDataCopy(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr);                                      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    retVal = BUFREQ_OK;
  }

  return retVal;
} /* DoIP_RxTcp_HandleCopyRxData() */

/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxDataCopy()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcp_HandleCopyRxDataCopy(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType readDataLen = 0u;
  const DoIP_SizeOfTxTcpMgtType txTcpMgtSize = (DoIP_SizeOfTxTcpMgtType)(
    DoIP_GetTxTcpMgtEndIdxOfConnectionTcp(ConnectionTcpIdx) -
    DoIP_GetTxTcpMgtStartIdxOfConnectionTcp(ConnectionTcpIdx));
#if ( DOIP_TCP_RX_MULTI_MSG == STD_ON )
# if ( DOIP_NACK_APPEND_REQ == STD_ON )
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
# endif /* DOIP_NACK_APPEND_REQ == STD_ON */
#endif /* DOIP_TCP_RX_MULTI_MSG == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset inactivity timer if routing activation is active. */
  if ( DoIP_GetActivStateOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
  {
    DoIP_SetInactivCntOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

  /* #20 Copy header data. */
  if ( DoIP_RxTcpGen_CopyGenHdr(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, &readDataLen) == E_OK )                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    /* #30 Copy payload data. */
    DoIP_RxTcp_HandleCopyRxDataCopyBody(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, &readDataLen);                    /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  /* #40 Block buffer if message is not processed completely. */
  if ( DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx) == 0u )
  {
#if ( DOIP_TCP_RX_MULTI_MSG == STD_ON )
# if ( DOIP_NACK_APPEND_REQ == STD_ON )
    if ( testerIdx != DoIP_GetSizeOfTester() )
    {
      if ( DoIP_GetDiagNAckOffsetOfTesterDyn(testerIdx) > 0u )
      {
        /* #50 Do not accept new data until acknowledge is pending (if enabled). */
        *BufferSizePtr = 0u;                                                                                           /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
        DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }
    else
# endif /* DOIP_NACK_APPEND_REQ == STD_ON */
    {
      /* #60 Provide buffer size for previously received user data if tester could be determined (if enabled). */
      *BufferSizePtr = DOIP_GEN_HDR_LEN_BYTE;                                                                          /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, DOIP_GEN_HDR_LEN_BYTE);                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }
#else
    *BufferSizePtr = 0u;                                                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
#endif /* DOIP_TCP_RX_MULTI_MSG == STD_ON */
  }
  else if ( DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx) >= (txTcpMgtSize - 1u) )
  { /* high Tx queue fill level - slow down Rx path */
    *BufferSizePtr = 0u;                                                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
  else if ( (DoIP_GetActivStateOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_ROUT_ACTIV_STATE_AUTH_PENDING) ||
            (DoIP_GetActivStateOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_ROUT_ACTIV_STATE_CONF_PENDING) )
  { /* activation state is pending */
    /* stop reception until state changes */
    *BufferSizePtr = 0u;                                                                                               /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
  else
  {
    /* nothing to do here */
  }
} /* DoIP_RxTcp_HandleCopyRxDataCopy() */

/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxDataCopyBody()
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
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcp_HandleCopyRxDataCopyBody(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_RxHdrBufType *rxHdrBuf = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx));
  uint8  hdrVerField;
  uint8  hdrInvVerField;
  uint16 hdrTypeField;
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT8(rxHdrBuf, 0u, hdrVerField);                                                                         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  IPBASE_GET_UINT8(rxHdrBuf, 1u, hdrInvVerField);                                                                      /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Send Nack if header pattern is invalid. */
  if ( ((uint8)(hdrVerField) != DOIP_PROTOCOL_VERSION) ||
    ((uint8)(hdrInvVerField) != DOIP_INVERSE_PROTOCOL_VERSION) )
  {
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_FORMAT, 0u, 0u, NULL_PTR, TRUE); /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else
  {
    IPBASE_GET_UINT16(rxHdrBuf, 2u, hdrTypeField);                                                                     /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    IPBASE_GET_UINT32(rxHdrBuf, 4u, hdrLenField);                                                                      /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

    /* #20 Otherwise check if routing activation is busy then process routing activation later. */
    if ( (hdrTypeField == DOIP_HDR_TYPE_ROUT_ACTIV_REQ) &&
         (DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u) != DoIP_GetSizeOfConnectionTcp()) )
    {
      *BufferSizePtr = 0u;                                                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }
    else
    {
      const uint64 msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;

      /* #30 Otherwise check if message shall be skipped. */
      if ( DoIP_IsSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx) )
      {
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
        /* #40 Append Nack data (if enabled). */
        DoIP_RxTcp_HandleCopyRxDataAppendNack(ConnectionTcpIdx, PduInfoPtr, ReadDataLenPtr);                           /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */
        if ( (msgLength - rxMsgOffset) > ((uint64)(PduInfoPtr->SduLength) - (uint64)(*ReadDataLenPtr)) )
        {
          /* #50 Wait until message is received completely. */
          uint32 bufSize = (uint32)(msgLength - rxMsgOffset);                                                          /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */
          rxMsgOffset += ((DoIP_RxMsgOffsetOfConnectionTcpDynType)(PduInfoPtr->SduLength) -
                          (DoIP_RxMsgOffsetOfConnectionTcpDynType)(*ReadDataLenPtr));
          DoIP_Util_CheckBufferSizeAndModify(bufSize, BufferSizePtr);                                                  /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
          *ReadDataLenPtr = (PduLengthType)PduInfoPtr->SduLength;                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
          DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, rxMsgOffset);                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
          DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        }
        else
        {
          /* #60 Otherwise finish message. */
          DoIP_TxTcp_FinishTransmit(ConnectionTcpIdx);

          *BufferSizePtr = 0u;                                                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
          *ReadDataLenPtr = (PduLengthType)PduInfoPtr->SduLength;                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
          DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
          DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
          DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        }
      }
      else
      {
        /* #70 Otherwise handle message depending on the header type. */
        switch(hdrTypeField)
        {
          case DOIP_HDR_TYPE_ROUT_ACTIV_REQ:
          {
            DoIP_RxTcpRout_HandleRoutingActivationReq(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);    /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
            break;
          }
          case DOIP_HDR_TYPE_ALIVE_CHECK_RES:
          {
            DoIP_RxTcpAlive_HandleAliveCheckRes(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);          /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
            break;
          }
          case DOIP_HDR_TYPE_DIAG_REQ:
          {
            DoIP_RxTcpDiag_HandleDiagnosticMessage(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);       /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
            break;
          }
          case DOIP_HDR_TYPE_GEN_NACK:
          case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ:
#if ( DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON )
          case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_EID:
#endif /* DOIP_VEHICLE_IDENT_EID_SUPPORT == STD_ON */
          case DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_VIN:
          case DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT:
          case DOIP_HDR_TYPE_ROUT_ACTIV_RES:
          case DOIP_HDR_TYPE_ALIVE_CHECK_REQ:
          case DOIP_HDR_TYPE_ENTITY_STATE_REQ:
          case DOIP_HDR_TYPE_ENTITY_STATE_RES:
          case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_REQ:
          case DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES:
          case DOIP_HDR_TYPE_DIAG_ACK:
          case DOIP_HDR_TYPE_DIAG_NACK:
          { /* not relevant payload type */
            DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
            break;
          }
          default:
          { /* unknown payload type */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
            DoIP_RxTcpOem_HandleOemPayloadTypes(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);          /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
#else
            /* Send error if payload type are unknown. */
            DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
            (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx,                                                                /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
              DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, 0u, 0u, NULL_PTR, FALSE);
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
            break;
          }
        }
      }

      /* #80 Set buffer size to receive entire message in next function call if message shall be skipped. */
      if ( DoIP_IsSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx) )
      {
        uint32 bufSize = (uint32)(msgLength - DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx));                /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */
        DoIP_Util_CheckBufferSizeAndModify(bufSize, BufferSizePtr);                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }
  }
} /* DoIP_RxTcp_HandleCopyRxDataCopyBody() */

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcp_HandleCopyRxDataAppendNack()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcp_HandleCopyRxDataAppendNack(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2CONST(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 hdrTypeField;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT16(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 2u, hdrTypeField);/* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Check header type. */
  if ( hdrTypeField == DOIP_HDR_TYPE_DIAG_REQ )
  {
    if ( (DoIP_GetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_HDR_TYPE_DIAG_NACK) ||
         (DoIP_GetTpTxPreHdrTypeOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_HDR_TYPE_DIAG_ACK) )
    {
      DoIP_RxTcpDiag_AppendNackData(ConnectionTcpIdx, PduInfoPtr, ReadDataLenPtr);                                     /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
    }
  }
} /* DoIP_RxTcp_HandleCopyRxDataAppendNack() */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleRoutingActivationReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleRoutingActivationReq(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 bytesToCopy;
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u, hdrLenField); /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Check message lengths and send nack. */
  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR,       /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      FALSE);
  }
  else if ( (hdrLenField != (DOIP_ROUT_ACTIV_REQ_MIN_LEN_BYTE)) &&
            (hdrLenField != (DOIP_ROUT_ACTIV_REQ_MIN_LEN_BYTE + DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE)) )
  {
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, 0u, 0u, NULL_PTR, TRUE);/* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else
  {
    const uint64 msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;

    bytesToCopy = (uint32)(msgLength - DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx));

    /* #20 Otherwise copy data to header buffer. */
    DoIP_RxTcpGen_CopyDataToHeaderBuffer(ConnectionTcpIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);                   /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */

    if ( DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx) == msgLength )
    {
      /* #30 Handle message if message is now copied completely. */
      DoIP_RxTcpRout_HandleCopiedRoutingActivationReq(ConnectionTcpIdx);
    }
    else
    {
      /* #40 Otherwise set buffer size to length of pending message part. */
      uint32 bufSize = (uint32)(msgLength - DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx));                  /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */
      DoIP_Util_CheckBufferSizeAndModify(bufSize, BufferSizePtr);                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }
  }
} /* DoIP_RxTcpRout_HandleRoutingActivationReq() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleCopiedRoutingActivationReq()
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
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleCopiedRoutingActivationReq(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfTesterType testerIdx = DoIP_GetSizeOfTester();
  DoIP_SizeOfRoutingActivationType routActIdx = DoIP_GetSizeOfRoutingActivation();
  const DoIP_RxHdrBufType *rxHdrBuf = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx));
  uint16 testerAddr;
  uint8 activType;

  /* ----- Implementation ----------------------------------------------- */
  DoIP_SetInactivCntOfConnectionTcpDyn(ConnectionTcpIdx, 0u);       /* reset inactivity timer */                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);      /* finish message */                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetConnectionTcpIdxOfRoutActivHandler(0u, ConnectionTcpIdx); /* lock routing activation handler */              /* SBSW_DOIP_RoutActivHandler */

  IPBASE_GET_UINT16(rxHdrBuf, DOIP_GEN_HDR_LEN_BYTE, testerAddr);                                                      /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  IPBASE_GET_UINT8(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + DOIP_ADDR_LEN_BYTE), activType);                                 /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Send nack if no tester with received tester address is configured. */
  if ( DoIP_RxTcpRout_GetTesterIdxByTesterAddr(ConnectionTcpIdx, testerAddr, &testerIdx) != E_OK )                     /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                              /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      DOIP_ROUT_ACTIV_RES_CODE_UNKNOWN_TESTER, testerIdx, NULL_PTR, TRUE);
  }
  /* #20 Otherwise send nack if another tester is active on connection. */
  else if ( DoIP_RxTcpRout_CheckForRegisteredTester(ConnectionTcpIdx, testerIdx) != E_OK )
  {
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                              /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      DOIP_ROUT_ACTIV_RES_CODE_DIFFERENT_TESTER, testerIdx, NULL_PTR, TRUE);
  }
  /* #30 Otherwise send nack if no routing activation with received activation type is configured considering security
   *     required. */
  else if ( DoIP_RxTcpRout_GetRoutingActivationIdxByActivationType(ConnectionTcpIdx, testerIdx, activType,             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    &routActIdx) != E_OK )
  {
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                              /* SBSW_DOIP_PARAM_VALID_POINTER_AND_NULL_POINTER_FORWARD */
      DOIP_ROUT_ACTIV_RES_CODE_INV_ACTIV_TYPE, testerIdx, NULL_PTR, TRUE);
  }
  /* #40 Otherwise assume that the routing activation request is valid. */
  else
  {
    DoIP_SetTesterIdxOfRoutActivHandler(0u, testerIdx);                                                                /* SBSW_DOIP_RoutActivHandler */
    DoIP_SetTesterAddrOfRoutActivHandler(0u, testerAddr);                                                              /* SBSW_DOIP_RoutActivHandler */
    DoIP_SetRoutingActivationIdxOfRoutActivHandler(0u, routActIdx);                                                    /* SBSW_DOIP_RoutActivHandler */

    /* #50 Handle the OEM specific part. */
    DoIP_RxTcpRout_HandleCopiedRoutingActivationReqOem(ConnectionTcpIdx);

    /* #60 Trigger alive check if there is no free connection left or if tester is already connected. */
    if ( DoIP_RxTcpRout_CheckConnectionNum(ConnectionTcpIdx, testerAddr) != E_OK )
    {
      /* Nothing to do. */
    }
    /* #70 Otherwise send nack if the security requirement is violated. */
    else if ( DoIP_RxTcpRout_CheckSecurityReq(ConnectionTcpIdx, testerIdx, testerAddr, routActIdx) != E_OK )
    {
      /* Nothing to do. */
    }
    /* #80 Otherwise perform authentication and confirmation via OEM specific part and activate routing. */
    else
    {
      DoIP_RxTcpRout_PerformRoutingActivation();
    }
  }
} /* DoIP_RxTcpRout_HandleCopiedRoutingActivationReq() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleCopiedRoutingActivationReqOem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleCopiedRoutingActivationReqOem(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;
  uint32 oemSpecReq;
  const DoIP_RxHdrBufStartIdxOfConnectionTcpType rxHdrBufStartIdx =
    DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(rxHdrBufStartIdx), 4u, hdrLenField);                                          /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Check header length if the OEM specific part is available. */
  if ( hdrLenField == DOIP_ROUT_ACTIV_REQ_MAX_LEN_BYTE )
  {
    /* #20 Initialize OEM specific part for response. */
    DoIP_SetOemSpecResOfRoutActivHandler(0u, 0u);                                                                      /* SBSW_DOIP_RoutActivHandler */

    /* #30 Store OEM specific part for request. */
    IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(rxHdrBufStartIdx + (DOIP_GEN_HDR_LEN_BYTE + 7u)), 0u, oemSpecReq);          /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

    DoIP_SetOemSpecReqOfRoutActivHandler(0u, oemSpecReq);                                                              /* SBSW_DOIP_RoutActivHandler */

    DoIP_SetOemSpecUsedOfRoutActivHandler(0u, TRUE);                                                                   /* SBSW_DOIP_RoutActivHandler */
  }
  else
  {
    DoIP_SetOemSpecUsedOfRoutActivHandler(0u, FALSE);                                                                  /* SBSW_DOIP_RoutActivHandler */
  }
} /* DoIP_RxTcpRout_HandleCopiedRoutingActivationReqOem() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckForRegisteredTester()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckForRegisteredTester(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there is an active tester. */
  if ( testerIdx != DoIP_GetSizeOfTester() )
  {
    /* #20 Check if there is an other active tester. */
    if ( testerIdx != TesterIdx )
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* DoIP_RxTcpRout_CheckForRegisteredTester() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_GetRoutingActivationIdxByActivationType()
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
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_GetRoutingActivationIdxByActivationType(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx,
  uint8 ActivationType,
  P2VAR(DoIP_SizeOfRoutingActivationType, AUTOMATIC, DOIP_APPL_VAR) RoutActIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_RoutingActivationIndIterType routingActivationIndIdx;
  DoIP_SizeOfRoutingActivationType routingActivationIdxTmp;
  boolean stopIteration = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all routing activations of the requested tester. */
  for ( routingActivationIndIdx = DoIP_GetRoutingActivationIndStartIdxOfTester(TesterIdx);
    routingActivationIndIdx < DoIP_GetRoutingActivationIndEndIdxOfTester(TesterIdx);
    routingActivationIndIdx++ )
  {
    routingActivationIdxTmp = DoIP_GetRoutingActivationInd(routingActivationIndIdx);

    /* #20 Check if the routing activation number is equal to the received activation type. */
    if ( DoIP_GetRoutingActivationNumOfRoutingActivation(routingActivationIdxTmp) == ActivationType )
    {
      /* #30 Prepare to return the routing activation index. */
      *RoutActIdxPtr = routingActivationIdxTmp;                                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      retVal = E_OK;

      /* #40 Check if the connection is secured. */
      if ( DoIP_IsSecurityRequiredOfConnectionTcp(ConnectionTcpIdx) )
      {
        /* #50 Indicate to stop iteration if routing activation requires security. */
        if ( DoIP_IsSecurityRequiredOfRoutingActivation(routingActivationIdxTmp) )
        {
          stopIteration = TRUE;
        }
      }
      /* #60 Otherwise stop iteration if routing activation requires no security. */
      else
      {
        if ( !DoIP_IsSecurityRequiredOfRoutingActivation(routingActivationIdxTmp) )
        {
          stopIteration = TRUE;
        }
      }
    }

    /* #70 Stop iteration if required. */
    if ( stopIteration == TRUE )
    {
      break;
    }
  }

  return retVal;
} /* DoIP_RxTcpRout_GetRoutingActivationIdxByActivationType() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckConnectionNum()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckConnectionNum(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_SizeOfConnectionTcpType connectionTcpIdxOfTesterAddr =
    DoIP_RxTcpRout_GetConnectionIdxOfTesterAddr(ConnectionTcpIdx, TesterAddr);

  /* ----- Implementation ----------------------------------------------- */
  if ( connectionTcpIdxOfTesterAddr == ConnectionTcpIdx )
  {
    /* #10 Check if tester is already connected. */
    retVal = E_OK;
  }
  else if ( connectionTcpIdxOfTesterAddr != DoIP_GetSizeOfConnectionTcp() )
  {
    /* #20 Trigger alive check on single connection if connection is in use. */
    DoIP_RxTcpRout_TriggerAliveCheckSingleConnection(connectionTcpIdxOfTesterAddr);
  }
  else if ( !DoIP_RxTcpRout_IsSockAvailable(DoIP_GetConnectionGroupIdxOfConnectionTcp(ConnectionTcpIdx)) &&
    DoIP_IsPlusOneSocketUsedOfConnectionGroup(DoIP_GetConnectionGroupIdxOfConnectionTcp(ConnectionTcpIdx)) )
  {
    /* #30 Trigger alive check on all connections only if there is no free socket and the n+1 handling is used. */
    DoIP_RxTcpRout_TriggerAliveCheckAllConnections(ConnectionTcpIdx);
  }
  else
  { /* tester is not active on a different socket and a TCP_DATA socket is available */
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_RxTcpRout_CheckConnectionNum() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckSecurityReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckSecurityReq(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx,
  uint16 TesterAddr,
  DoIP_SizeOfRoutingActivationType RoutActIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the connection is not secured. */
  if ( !DoIP_IsSecurityRequiredOfConnectionTcp(ConnectionTcpIdx) )
  {
    /* #20 Check if routing activation requires a secured connection. */
    if ( DoIP_IsSecurityRequiredOfRoutingActivation(RoutActIdx) )
    {
      /* #30 Check if secure communication shall be used (if enabled). */
#if ( DOIP_USE_SECURE_COMMUNICATION_CBK == STD_ON )
      if ( DoIP_CallUseSecureCommunicationDirect() == TRUE )                                                           /* SBSW_DOIP_FUNCTION_POINTER_CALL */
#endif /* DOIP_USE_SECURE_COMMUNICATION_CBK == STD_ON */
      {
        /* #40 Send negative response. */
        (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, TesterAddr,                          /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
          DOIP_ROUT_ACTIV_RES_CODE_REQUIRE_SECURE, TesterIdx, NULL_PTR, TRUE);
        retVal = E_NOT_OK;
      }
    }
  }

  return retVal;
} /* DoIP_RxTcpRout_CheckSecurityReq() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_TriggerAliveCheckSingleConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_TriggerAliveCheckSingleConnection(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(connectionIdx);
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
  DoIP_AliveCheckCntOfConnectionTcpDynType aliveCheckCntOfConnectionTcp =
    DoIP_GetAliveCheckCntOfConnectionTcpDyn(ConnectionTcpIdx);
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
  if ( aliveCheckCntOfConnectionTcp != 0u )
  {
    /* #10 Load alive check timer for routing activation handler if alive check is already pending. */
    DoIP_SetAliveCheckCntOfRoutActivHandler(0u, aliveCheckCntOfConnectionTcp);                                         /* SBSW_DOIP_RoutActivHandler */
    DoIP_SetConnectionTcpIdxOfAliveCheckList(0u, ConnectionTcpIdx);                                                    /* SBSW_DOIP_AliveCheckList */
    DoIP_SetAliveCheckElemNumOfRoutActivHandler(0u, 1u);                                                               /* SBSW_DOIP_RoutActivHandler */
  }
  else
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
  /* #20 Otherwise try to send alive check request. */
  if ( DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_ALIVE_CHECK_REQ, 0u, 0u, 0u, NULL_PTR, FALSE) == E_OK )     /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  {
    /* #30 Load alive check timer for routing activation handler if alive check could be sent. */
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
    DoIP_SetAliveCheckCntOfConnectionTcpDyn(ConnectionTcpIdx, DoIP_GetAliveCheckResTimeOfInterface(interfaceIdx));     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
    DoIP_SetAliveCheckCntOfRoutActivHandler(0u, DoIP_GetAliveCheckResTimeOfInterface(interfaceIdx));                   /* SBSW_DOIP_RoutActivHandler */
    DoIP_SetConnectionTcpIdxOfAliveCheckList(0u, ConnectionTcpIdx);                                                    /* SBSW_DOIP_AliveCheckList */
    DoIP_SetAliveCheckElemNumOfRoutActivHandler(0u, 1u);                                                               /* SBSW_DOIP_RoutActivHandler */
  }
  else
  {
    /* nothing to do here */
  }
} /* DoIP_RxTcpRout_TriggerAliveCheckSingleConnection() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_TriggerAliveCheckAllConnections()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_TriggerAliveCheckAllConnections(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfAliveCheckListType aliveCheckListIdx = 0u;
  DoIP_ConnectionTcpIterType connectionTcpIter;
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(connectionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connections. */
  for ( connectionTcpIter = DoIP_GetConnectionTcpStartIdxOfInterface(interfaceIdx);
        connectionTcpIter < DoIP_GetConnectionTcpEndIdxOfInterface(interfaceIdx);
        connectionTcpIter++ )
  {
    DoIP_ConnectionIdxOfConnectionTcpType currConnectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIter);

    /* #20 Check if sockets are valid. */
    if ( (DoIP_GetLocalAddrIdxOfConnection(currConnectionIdx) == DoIP_GetLocalAddrIdxOfConnection(connectionIdx)) &&
         (DoIP_GetTesterIdxOfConnectionTcpDyn(connectionTcpIter) != DoIP_GetSizeOfTester()) )
    {
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
      DoIP_AliveCheckCntOfConnectionTcpDynType aliveCheckCnt =
        DoIP_GetAliveCheckCntOfConnectionTcpDyn(connectionTcpIter);

      if ( aliveCheckCnt != 0u )
      {
        /* #30 Load alive check timer for routing activation handler if alive check is already pending. */
        DoIP_SetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx, (DoIP_SizeOfConnectionTcpType)connectionTcpIter);  /* SBSW_DOIP_AliveCheckList */
        aliveCheckListIdx++;
      }
      else
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
      if ( DoIP_TxTcp_Transmit((DoIP_SizeOfConnectionTcpType)connectionTcpIter, DOIP_HDR_TYPE_ALIVE_CHECK_REQ, 0u, 0u, /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        0u, NULL_PTR, FALSE) == E_OK )
      {
        /* #40 Load alive check timer for routing activation handler if alive check could be sent. */
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
        DoIP_SetAliveCheckCntOfConnectionTcpDyn(connectionTcpIter, DoIP_GetAliveCheckResTimeOfInterface(interfaceIdx));/* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
        DoIP_SetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx, (DoIP_SizeOfConnectionTcpType)connectionTcpIter);  /* SBSW_DOIP_AliveCheckList */
        aliveCheckListIdx++;
      }
      else
      {
        /* nothing to do here */
      }
    }
  }

  /* #50 Load alive check timer. */
  DoIP_SetAliveCheckCntOfRoutActivHandler(0u, DoIP_GetAliveCheckResTimeOfInterface(interfaceIdx));                     /* SBSW_DOIP_RoutActivHandler */
  /* #60 Set number of sent alive checks. */
  DoIP_SetAliveCheckElemNumOfRoutActivHandler(0u, aliveCheckListIdx);                                                  /* SBSW_DOIP_RoutActivHandler */
} /* DoIP_RxTcpRout_TriggerAliveCheckAllConnections() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_GetTesterIdxByTesterAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_GetTesterIdxByTesterAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr,
  P2VAR(DoIP_SizeOfTesterType, AUTOMATIC, DOIP_APPL_VAR) TesterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  DoIP_TesterIterType testerIdx;
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(connectionIdx);

  /* ----- Implementation ----------------------------------------------- */
  *TesterIdx = DoIP_GetSizeOfTester();                                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

  for ( testerIdx = DoIP_GetTesterStartIdxOfInterface(interfaceIdx);
        testerIdx < DoIP_GetTesterEndIdxOfInterface(interfaceIdx);
        testerIdx++ )
  {
    /* #10 Look up testers index. */
    if ( DoIP_GetTesterAddrOfTester(testerIdx) == TesterAddr )
    {
      *TesterIdx = (DoIP_SizeOfTesterType)testerIdx;                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      retVal = E_OK;
      break;
    }
  }

#if ( DOIP_DEFAULT_TESTER_ENABLED == STD_ON )
  /* #20 Check if no tester is found and if interface uses a default tester (if enabled). */
  if ( (retVal != E_OK) && (DoIP_IsTesterDefaultUsedOfInterface(interfaceIdx)) )
  {
    DoIP_SizeOfConnectionTcpType connectionTcpIdx =
      DoIP_GetConnectionTcpIdxOfTesterDyn(DoIP_GetTesterDefaultIdxOfInterface(interfaceIdx));

    /* #30 Check if default tester is not used or used with same address. */
    if ( (connectionTcpIdx == DoIP_GetSizeOfConnectionTcp()) ||
         (DoIP_GetTesterIdxOfConnectionTcpDyn(connectionTcpIdx) == DoIP_GetSizeOfTester()) ||
         (TesterAddr == DoIP_GetTesterAddrOfConnectionTcpDyn(connectionTcpIdx)) )
    {
      *TesterIdx = DoIP_GetTesterDefaultIdxOfInterface(interfaceIdx);                                                  /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      retVal = E_OK;
    }
  }
#endif /* DOIP_DEFAULT_TESTER_ENABLED == STD_ON */

  return retVal;
} /* DoIP_RxTcpRout_GetTesterIdxByTesterAddr() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_GetConnectionIdxOfTesterAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_SizeOfConnectionTcpType, DOIP_CODE) DoIP_RxTcpRout_GetConnectionIdxOfTesterAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint16 TesterAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionTcpIterType connectionTcpIter;
  DoIP_SizeOfConnectionTcpType retConnectionTcpIdx = DoIP_GetSizeOfConnectionTcp();
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(connectionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Look up testers connection index. */
  for ( connectionTcpIter = DoIP_GetConnectionTcpStartIdxOfInterface(interfaceIdx);
        connectionTcpIter < DoIP_GetConnectionTcpEndIdxOfInterface(interfaceIdx);
        connectionTcpIter++ )
  {
    if ( (DoIP_GetTesterIdxOfConnectionTcpDyn(connectionTcpIter) != DoIP_GetSizeOfTester()) &&
         (DoIP_GetTesterAddrOfConnectionTcpDyn(connectionTcpIter) == TesterAddr) )
    {
      retConnectionTcpIdx = (DoIP_SizeOfConnectionTcpType)connectionTcpIter;
      break;
    }
  }

  return retConnectionTcpIdx;
} /* DoIP_RxTcpRout_GetConnectionIdxOfTesterAddr() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_GetAvailableSockNum()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(boolean, DOIP_CODE) DoIP_RxTcpRout_IsSockAvailable(
  DoIP_SizeOfConnectionGroupType ConnectionGroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionTcpIterType usedSockNum = 0u;
  DoIP_ConnectionTcpIterType availableSockNum;
  DoIP_ConnectionTcpIndIterType connectionTcpIndIdx;
  DoIP_ConnectionTcpIndIterType maximumSockNum =
    ((DoIP_ConnectionTcpIndIterType)DoIP_GetConnectionTcpIndEndIdxOfConnectionGroup(ConnectionGroupIdx) -
     (DoIP_ConnectionTcpIndIterType)DoIP_GetConnectionTcpIndStartIdxOfConnectionGroup(ConnectionGroupIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all TCP connections related to the connection group. */
  for ( connectionTcpIndIdx = DoIP_GetConnectionTcpIndStartIdxOfConnectionGroup(ConnectionGroupIdx);
        connectionTcpIndIdx < DoIP_GetConnectionTcpIndEndIdxOfConnectionGroup(ConnectionGroupIdx);
        connectionTcpIndIdx++ )
  {
    /* #20 Increment the number of used connections if a tester is active on the connection. */
    if ( DoIP_GetTesterIdxOfConnectionTcpDyn(DoIP_GetConnectionTcpInd(connectionTcpIndIdx)) != DoIP_GetSizeOfTester() )
    {
      usedSockNum++;
    }
  }

  /* #30 Calculate number of available sockets dependent on configuration settings. */
  /* PRQA S 2912 1 */ /* MD_DoIP_21.1_CFG */
  availableSockNum = (DoIP_ConnectionTcpIterType)((maximumSockNum - usedSockNum) -
    (DoIP_IsPlusOneSocketUsedOfConnectionGroup(ConnectionGroupIdx) ? 1u : 0u));

  /* #40 Indicate if free sockets are available. */
  return (boolean)(availableSockNum > 0u);
} /* DoIP_RxTcpRout_GetAvailableSockNum() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_PerformRoutingActivation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_PerformRoutingActivation(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  const uint16 testerAddr = DoIP_GetTesterAddrOfRoutActivHandler(0u);
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfRoutingActivationType routActIdx = DoIP_GetRoutingActivationIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfRoutingActivationFuncPtrType routActFuncPtrIdx =
    DoIP_GetRoutingActivationFuncPtrIdxOfRoutingActivation(routActIdx);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u);
  DoIP_RoutActivStateType nextState = DOIP_ROUT_ACTIV_STATE_NONE;

  /* ----- Implementation ----------------------------------------------- */
  if ( !DoIP_IsOemSpecUsedOfRoutActivHandler(0u) &&
       ((DoIP_GetAuthReqLengthOfRoutingActivation(routActIdx) != 0u) ||
        (DoIP_GetConfReqLengthOfRoutingActivation(routActIdx) != 0u)) )
  {
    DoIP_SetActivStateOfConnectionTcpDyn(connectionTcpIdx, DOIP_ROUT_ACTIV_STATE_NONE);                                /* SBSW_DOIP_RoutActivHandler_Data */

    /* #10 Send error if OEM specific part is not received but required. */
    (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                              /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      DOIP_ROUT_ACTIV_RES_CODE_MISSING_AUTHENT, testerIdx, NULL_PTR, FALSE);
  }
  else
  {
    /* #20 Otherwise call authentication callback (if configured). */
    if ( (DoIP_GetAuthFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx) != NULL_PTR) ||
         (DoIP_GetAuthWithRemAddrFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx) != NULL_PTR) )
    {
      retVal = DoIP_RxTcpRout_PerformRoutingActivationAuth(&nextState);                                                /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    }

    /* #30 Call confirmation callback (if configured). */
    if ( (retVal == E_OK) && (
           (DoIP_GetConfFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx) != NULL_PTR) ||
           (DoIP_GetConfWithRemAddrFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx) != NULL_PTR)) )
    {
      retVal = DoIP_RxTcpRout_PerformRoutingActivationConf(&nextState);                                                /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    }

    if ( retVal == E_OK )
    {
      /* #40 Register tester, activate route and send response if authentification and confirmation passed. */
      DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, testerIdx, testerAddr, routActIdx,
        DOIP_ROUT_ACTIV_STATE_ACTIVATED);

      DoIP_SetInactivCntOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                      /* SBSW_DOIP_RoutActivHandler_Data */

      DoIP_SetConnectionTcpIdxOfTesterDyn(testerIdx, connectionTcpIdx);                                                /* SBSW_DOIP_RoutActivHandler_Data */

      (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                            /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        DOIP_ROUT_ACTIV_RES_CODE_SUCCESS, testerIdx, NULL_PTR, FALSE);

      /* #50 Notify user that channel is ready to transmit diagnostic messages. */
      DoIP_RxTcpRout_NotifyChannelReady(connectionTcpIdx);
    }
  }
} /* DoIP_RxTcpRout_PerformRoutingActivation() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_RegisterTester()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_RegisterTester(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfTesterType TesterIdx,
  uint16 TesterAddr,
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  DoIP_RoutActivStateType NextState)
{
  /* #10 Save tester data. */
  DoIP_SetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx, TesterIdx);                                                    /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetTesterAddrOfConnectionTcpDyn(ConnectionTcpIdx, TesterAddr);                                                  /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetRoutingActivationIdxOfConnectionTcpDyn(ConnectionTcpIdx, RoutActIdx);                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetActivStateOfConnectionTcpDyn(ConnectionTcpIdx, NextState);                                                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
} /* DoIP_RxTcpRout_RegisterTester() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_PerformRoutingActivationAuth()
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
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_PerformRoutingActivationAuth(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  const uint16 testerAddr = DoIP_GetTesterAddrOfRoutActivHandler(0u);
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfRoutingActivationType routActIdx = DoIP_GetRoutingActivationIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfRoutingActivationFuncPtrType routActFuncPtrIdx =
    DoIP_GetRoutingActivationFuncPtrIdxOfRoutingActivation(routActIdx);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u);
  boolean authentified = FALSE;
  uint32 oemSpecReq32;
  uint32 oemSpecRes32;
  uint8 oemSpecReq[DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE] = { 0u };
  uint8 oemSpecRes[DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE] = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  oemSpecReq32 = DoIP_GetOemSpecReqOfRoutActivHandler(0u);
  DoIP_Util_PutUInt32(oemSpecReq, 0u, oemSpecReq32);                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

  oemSpecRes32 = DoIP_GetOemSpecResOfRoutActivHandler(0u);
  DoIP_Util_PutUInt32(oemSpecRes, 0u, oemSpecRes32);                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

#if ( DOIP_ROUT_ACT_AUTH_REM_ADDR_ENABLED == STD_ON )
  if ( DoIP_IsAuthFuncWithRemAddrOfRoutingActivation(routActIdx) )
  {
    const DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx);
    SoAd_SockAddrInetXType remAddr = { SOAD_AF_INVALID, 0u, { 0u } };

    /* #10 Try to get remote address for authentification (if enabled). */
    /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
    if ( SoAd_GetRemoteAddr(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
           DOIP_A_P2VAR(SoAd_SockAddrType)&remAddr) == E_OK )
    {
      /* #15 Call authentification with remote address on success (if enabled).*/
      /* PRQA S 0310 2 */ /* MD_DoIP_11.4 */
      retVal = DoIP_GetAuthWithRemAddrFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx)(&authentified,              /* SBSW_DOIP_FUNCTION_POINTER_CALL */
        &oemSpecReq[0u], &oemSpecRes[0u], DOIP_A_P2CONST(SoAd_SockAddrType)&remAddr);
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
#endif /* DOIP_ROUT_ACT_AUTH_REM_ADDR_ENABLED == STD_ON */
  {
    /* #20 Otherwise call authentification without remote address. */
    /* PRQA S 0310 3 */ /* MD_DoIP_11.4 */
    retVal = DoIP_GetAuthFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx)(&authentified, &oemSpecReq[0u],          /* SBSW_DOIP_FUNCTION_POINTER_CALL */
      &oemSpecRes[0u]);
  }

  IPBASE_GET_UINT32(&oemSpecReq[0u], 0u, oemSpecReq32);                                                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  DoIP_SetOemSpecReqOfRoutActivHandler(0u, oemSpecReq32);                                                              /* SBSW_DOIP_RoutActivHandler */

  IPBASE_GET_UINT32(&oemSpecRes[0u], 0u, oemSpecRes32);                                                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  DoIP_SetOemSpecResOfRoutActivHandler(0u, oemSpecRes32);                                                              /* SBSW_DOIP_RoutActivHandler */

  if ( retVal == E_OK )
  {
    /* #30 In case authentication succeeded. */
    if ( authentified == TRUE )
    {
      /* Hint: Registration of tester is done for this case in the calling function. */
      /* #40 In case authentication passed activate route. */
      *NextStatePtr = DOIP_ROUT_ACTIV_STATE_ACTIVATED;                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    }
    else
    {
      /* #50 Otherwise save state and send error response. */
      DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, testerIdx, testerAddr, routActIdx, DOIP_ROUT_ACTIV_STATE_NONE);

      (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                            /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        DOIP_ROUT_ACTIV_RES_CODE_MISSING_AUTHENT, testerIdx, NULL_PTR, FALSE);

      retVal = E_NOT_OK;
    }
  }
  else if ( retVal == DOIP_E_PENDING )
  {
    /* #60 In case authentication pending set state pending. */
    DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, testerIdx, testerAddr, routActIdx,
      DOIP_ROUT_ACTIV_STATE_AUTH_PENDING);
  }
  else
  {
    /* #70 Otherwise send error. */
    DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, testerIdx, testerAddr, routActIdx, DOIP_ROUT_ACTIV_STATE_NONE);

    (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                              /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      DOIP_ROUT_ACTIV_RES_CODE_MISSING_AUTHENT, testerIdx, NULL_PTR, FALSE);
  }

  return retVal;
} /* DoIP_RxTcpRout_PerformRoutingActivationAuth() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_PerformRoutingActivationConf()
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
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_PerformRoutingActivationConf(
  P2VAR(DoIP_RoutActivStateType, AUTOMATIC, DOIP_APPL_VAR) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  const uint16 testerAddr = DoIP_GetTesterAddrOfRoutActivHandler(0u);
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfRoutingActivationType routActIdx = DoIP_GetRoutingActivationIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfRoutingActivationFuncPtrType routActFuncPtrIdx =
    DoIP_GetRoutingActivationFuncPtrIdxOfRoutingActivation(routActIdx);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u);
  boolean confirmed = FALSE;
  uint32 oemSpecReq32;
  uint32 oemSpecRes32;
  uint8 oemSpecReq[DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE] = { 0u };
  uint8 oemSpecRes[DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE] = { 0u };
  uint8 offsetReq = 0u;
  uint8 offsetRes = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate request and response offset. */
  if ( DoIP_GetConfReqLengthOfRoutingActivation(routActIdx) > 0u )
  {
    offsetReq = (uint8)(DOIP_ROUT_ACTIV_REQ_OEM_LEN_BYTE - DoIP_GetConfReqLengthOfRoutingActivation(routActIdx));
  }
  if ( DoIP_GetConfResLengthOfRoutingActivation(routActIdx) > 0u )
  {
    offsetRes = (uint8)(DOIP_ROUT_ACTIV_ACK_OEM_LEN_BYTE - DoIP_GetConfResLengthOfRoutingActivation(routActIdx));
  }

  oemSpecReq32 = DoIP_GetOemSpecReqOfRoutActivHandler(0u);
  DoIP_Util_PutUInt32(oemSpecReq, 0u, oemSpecReq32);                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

  oemSpecRes32 = DoIP_GetOemSpecResOfRoutActivHandler(0u);
  DoIP_Util_PutUInt32(oemSpecRes, 0u, oemSpecRes32);                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

#if ( DOIP_ROUT_ACT_CONF_REM_ADDR_ENABLED == STD_ON )
  if ( DoIP_IsConfFuncWithRemAddrOfRoutingActivation(routActIdx) )
  {
    const DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx);
    SoAd_SockAddrInetXType remAddr = { SOAD_AF_INVALID, 0u, { 0u } };

    /* #20 Try to get remote address for confirmation (if enabled). */
    /* PRQA S 0310 8 */ /* MD_DoIP_11.4 */
    if ( SoAd_GetRemoteAddr(DoIP_GetSoConIdOfConnectionDyn(connectionIdx),                                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
           DOIP_A_P2VAR(SoAd_SockAddrType)&remAddr) == E_OK )
    {
      /* #25 Call confirmation with remote address on success (if enabled). */
      /* PRQA S 0313 2 */ /* MD_DoIP_11.1 */
      /* PRQA S 2842 2 */ /* MD_DoIP_21.1_IDX */
      retVal = DoIP_GetConfWithRemAddrFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx)                             /* SBSW_DOIP_FUNCTION_POINTER_CALL */
        (&confirmed, &oemSpecReq[offsetReq], &oemSpecRes[offsetRes], DOIP_A_P2CONST(SoAd_SockAddrType)&remAddr);
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
#endif /* DOIP_ROUT_ACT_CONF_REM_ADDR_ENABLED == STD_ON */
  {
    /* #30 Otherwise call confirmation without remote address. */
    /* PRQA S 2842 2 */ /* MD_DoIP_21.1_IDX */
    retVal = DoIP_GetConfFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx)                                          /* SBSW_DOIP_FUNCTION_POINTER_CALL */
      (&confirmed, &oemSpecReq[offsetReq], &oemSpecRes[offsetRes]);
  }

  IPBASE_GET_UINT32(&oemSpecReq[0u], 0u, oemSpecReq32);                                                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  DoIP_SetOemSpecReqOfRoutActivHandler(0u, oemSpecReq32);                                                              /* SBSW_DOIP_RoutActivHandler */

  IPBASE_GET_UINT32(&oemSpecRes[0u], 0u, oemSpecRes32);                                                                /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  DoIP_SetOemSpecResOfRoutActivHandler(0u, oemSpecRes32);                                                              /* SBSW_DOIP_RoutActivHandler */

  if ( retVal == E_OK )
  {
    /* #40 In case confirmation succeeded. */
    if ( confirmed == TRUE )
    {
      /* Hint: Registration of tester is done for this case in the calling function. */
      /* #50 In case confirmation passed activate route. */
      *NextStatePtr = DOIP_ROUT_ACTIV_STATE_ACTIVATED;                                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    }
    else
    {
      /* #60 Otherwise save state and send error response. */
      DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, testerIdx, testerAddr, routActIdx, DOIP_ROUT_ACTIV_STATE_NONE);

      (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                            /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REJECTED, testerIdx, NULL_PTR, TRUE);
    }
  }
  else if ( retVal == DOIP_E_PENDING )
  {
    /* #70 In case confirmation pending. */
    DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, testerIdx, testerAddr, routActIdx,
      DoIP_GetActivStateOfConnectionTcpDyn(connectionTcpIdx));

    if ( *NextStatePtr == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
    {
      DoIP_SetActivStateOfConnectionTcpDyn(connectionTcpIdx, DOIP_ROUT_ACTIV_STATE_CONF_PENDING);                      /* SBSW_DOIP_RoutActivHandler_Data */

      /* #80 Set state to confirmation pending and send response if authentication was performed in same context. */
      (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                            /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REQUIRED, testerIdx, NULL_PTR, FALSE);
    }
  }
  else
  {
    /* #90 Otherwise send error. */
    DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, testerIdx, testerAddr, routActIdx, DOIP_ROUT_ACTIV_STATE_NONE);

    (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                              /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REJECTED, testerIdx, NULL_PTR, TRUE);
  }

  return retVal;
} /* DoIP_RxTcpRout_PerformRoutingActivationConf() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleRetryRoutingActivationAuthAndConf()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleRetryRoutingActivationAuthAndConf(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if routing activation is pending on the interface. */
  if ( (connectionTcpIdx < DoIP_GetSizeOfConnectionTcp()) &&
       (DoIP_GetInterfaceIdxOfConnection(DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx)) == InterfaceIdx) )
  {
    /* #20 Check if no alive check is pending. */
    if ( DoIP_GetAliveCheckCntOfRoutActivHandler(0u) == 0u )
    {
      const uint16 testerAddr = DoIP_GetTesterAddrOfConnectionTcpDyn(connectionTcpIdx);
      const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(connectionTcpIdx);
      const DoIP_RoutActivStateType lastState = DoIP_GetActivStateOfConnectionTcpDyn(connectionTcpIdx);
      const DoIP_SizeOfRoutingActivationType routActIdx =
        DoIP_GetRoutingActivationIdxOfConnectionTcpDyn(connectionTcpIdx);
      const DoIP_SizeOfRoutingActivationFuncPtrType routActFuncPtrIdx =
        DoIP_GetRoutingActivationFuncPtrIdxOfRoutingActivation(routActIdx);
      DoIP_RoutActivStateType nextState = DOIP_ROUT_ACTIV_STATE_NONE;

      /* #30 Perform authentication if state is pending. */
      if ( lastState == DOIP_ROUT_ACTIV_STATE_AUTH_PENDING )
      {
        (void)DoIP_RxTcpRout_PerformRoutingActivationAuth(&nextState);                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      }

      /* #35 Perform confirmation if state is pending or callback is configured. */
      if ( (lastState == DOIP_ROUT_ACTIV_STATE_CONF_PENDING) ||
          ((nextState == DOIP_ROUT_ACTIV_STATE_ACTIVATED) &&
            ((DoIP_GetConfFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx) != NULL_PTR) ||
             (DoIP_GetConfWithRemAddrFuncPtrOfRoutingActivationFuncPtr(routActFuncPtrIdx) != NULL_PTR))) )
      {
        (void)DoIP_RxTcpRout_PerformRoutingActivationConf(&nextState);                                                 /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      }

      /* #40 Check if authentification and confirmation are performed successfully. */
      if ( nextState == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
      {
        DoIP_SetConnectionTcpIdxOfTesterDyn(testerIdx, connectionTcpIdx);                                              /* SBSW_DOIP_RegisterTester */

        DoIP_SetActivStateOfConnectionTcpDyn(connectionTcpIdx, DOIP_ROUT_ACTIV_STATE_ACTIVATED);                       /* SBSW_DOIP_RoutActivHandler_Data */

        if ( lastState != DOIP_ROUT_ACTIV_STATE_CONF_PENDING )
        {
          DoIP_SetInactivCntOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                  /* SBSW_DOIP_RoutActivHandler_Data */

          /* #50 Send routing activation response if not activated after confirmation was pending. */
          (void)DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, testerAddr,                        /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
            DOIP_ROUT_ACTIV_RES_CODE_SUCCESS, testerIdx, NULL_PTR, FALSE);

          /* #60 Notify user that channel is ready to transmit diagnostic messages. */
          DoIP_RxTcpRout_NotifyChannelReady(connectionTcpIdx);
        }
        else
        {
          /* #70 Release routing activation handler if activated after confirmation was pending. */
          DoIP_SetConnectionTcpIdxOfRoutActivHandler(0u, DoIP_GetSizeOfConnectionTcp());                               /* SBSW_DOIP_RoutActivHandler */
        }
      }
    }
  }
} /* DoIP_RxTcpRout_HandleRetryRoutingActivationAuthAndConf() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_NotifyChannelReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_NotifyChannelReady(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
#if ( DOIP_CHANNEL_READY_ENABLED == STD_ON )
  /* #10 Check if channel ready is used. */
  if (DoIP_IsChannelReadyUsedOfConnectionTcp(ConnectionTcpIdx) == TRUE)
  {
    /* #20 Notify user that ComM channel is ready to transmit diagnostic messages. */
    BswM_DoIP_SetChannelReady(DoIP_GetComMChannelIdOfConnectionGroup(
        DoIP_GetConnectionGroupIdxOfConnectionTcp(ConnectionTcpIdx)));
  }
#else
  DOIP_DUMMY_STATEMENT(ConnectionTcpIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_CHANNEL_READY_ENABLED == STD_ON */
} /* DoIP_RxTcpRout_NotifyChannelReady() */

#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleAliveCheckTimeoutOfTcpConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleAliveCheckTimeoutOfTcpConnections(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionTcpIterType connectionTcpIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check all timeouts. */
  for ( connectionTcpIdx = DoIP_GetConnectionTcpStartIdxOfInterface(InterfaceIdx);
        connectionTcpIdx < DoIP_GetConnectionTcpEndIdxOfInterface(InterfaceIdx);
        connectionTcpIdx++ )
  {
    DOIP_BEGIN_CRITICAL_SECTION();
    if ( DoIP_GetAliveCheckCntOfConnectionTcpDyn(connectionTcpIdx) > 0u )
    {
      DoIP_DecAliveCheckCntOfConnectionTcpDyn(connectionTcpIdx);                                                       /* SBSW_DOIP_CSL02 */
      if ( DoIP_GetAliveCheckCntOfConnectionTcpDyn(connectionTcpIdx) == 0u )
      {
        DOIP_END_CRITICAL_SECTION();

        /* #20 Close socket if elapsed. */
        DoIP_Con_SoConClose(DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx), TRUE);
        DoIP_RxTcpRout_RegisterTester((DoIP_SizeOfConnectionTcpType)connectionTcpIdx, DoIP_GetSizeOfTester(),
          DOIP_ADDR_ANY, DoIP_GetSizeOfRoutingActivation(), DOIP_ROUT_ACTIV_STATE_NONE);
      }
      else
      {
        DOIP_END_CRITICAL_SECTION();
      }
    }
    else
    {
      DOIP_END_CRITICAL_SECTION();
    }
  }
} /* DoIP_RxTcpRout_HandleAliveCheckTimeoutOfTcpConnections() */
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleAliveCheckTimeoutOfRoutActHandler()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleAliveCheckTimeoutOfRoutActHandler(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_TesterAddrOfRoutActivHandlerType testerAddr = DoIP_GetTesterAddrOfRoutActivHandler(0u);
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfRoutingActivationType routActIdx = DoIP_GetRoutingActivationIdxOfRoutActivHandler(0u);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfRoutActivHandler(0u);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if routing activation is pending on the interface. */
  if ( (connectionTcpIdx < DoIP_GetSizeOfConnectionTcp()) &&
       (DoIP_GetInterfaceIdxOfConnection(DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx)) == InterfaceIdx) )
  {
    /* #20 Handle alive check timeout counter. */
    DOIP_BEGIN_CRITICAL_SECTION();
    if ( DoIP_GetAliveCheckCntOfRoutActivHandler(0u) > 0u )
    {
      DoIP_DecAliveCheckCntOfRoutActivHandler(0u);                                                                     /* SBSW_DOIP_RoutActivHandler */
      if ( DoIP_GetAliveCheckCntOfRoutActivHandler(0u) == 0u )
      {
        DOIP_END_CRITICAL_SECTION();

        /* #30 Clean up all timed-out socket connections. */
        DoIP_RxTcpRout_HandleAliveCheckTimeoutsSocketsClose();

        /* #40 Check if tester is not active on another connection and an additional connection is available now. */
        if ( DoIP_RxTcpRout_CheckConnectionAvailablility(connectionTcpIdx, testerAddr) == E_OK )
        {
          /* #50 Check if security requirement is fulfilled. */
          if ( DoIP_RxTcpRout_CheckSecurityReq(connectionTcpIdx, testerIdx, testerAddr, routActIdx) == E_OK )
          {
            /* #60 Perform authentication and confirmation via OEM specific part and activate routing. */
            DoIP_RxTcpRout_PerformRoutingActivation();
          }
        }
      }
      else
      {
        DOIP_END_CRITICAL_SECTION();
      }
    }
    else
    {
      DOIP_END_CRITICAL_SECTION();
    }
  }
} /* DoIP_RxTcpRout_HandleAliveCheckTimeoutOfRoutActHandler() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleAliveCheckTimeoutsSocketsClose()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleAliveCheckTimeoutsSocketsClose(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_AliveCheckListIterType aliveCheckListIdx;
  DoIP_SizeOfConnectionTcpType connectionTcpIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all timeouted socket connections. */
  for ( aliveCheckListIdx = 0u;
        aliveCheckListIdx < DoIP_GetSizeOfAliveCheckList();
        aliveCheckListIdx++ )
  {
    if ( DoIP_GetAliveCheckElemNumOfRoutActivHandler(0u) == 0u )
    { /* all sockets closed or Alive Check Response received */
      break;
    }

    connectionTcpIdx = DoIP_GetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx);

    if ( connectionTcpIdx != DoIP_GetSizeOfConnectionTcp() )
    { /* alive check timeout -> close socket connection */

      DoIP_Con_SoConClose(DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx), TRUE);
      DoIP_RxTcpRout_RegisterTester(connectionTcpIdx, DoIP_GetSizeOfTester(), DOIP_ADDR_ANY,
        DoIP_GetSizeOfRoutingActivation(), DOIP_ROUT_ACTIV_STATE_NONE);

      DoIP_SetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx, DoIP_GetSizeOfConnectionTcp());                      /* SBSW_DOIP_CSL01 */
      DoIP_DecAliveCheckElemNumOfRoutActivHandler(0u);                                                                 /* SBSW_DOIP_RoutActivHandler */
    }
  }
} /* DoIP_RxTcpRout_HandleAliveCheckTimeoutsSocketsClose() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_CheckConnectionAvailablility()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpRout_CheckConnectionAvailablility(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_TesterAddrOfRoutActivHandlerType TesterAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if connection is still online. */
  if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE )
  {
    DoIP_SizeOfConnectionTcpType connectionTcpIdxofTesterAddr =
      DoIP_RxTcpRout_GetConnectionIdxOfTesterAddr(ConnectionTcpIdx, TesterAddr);

    /* #20 Check if routing is (still) active on other socket. */
    if ( connectionTcpIdxofTesterAddr != DoIP_GetSizeOfConnectionTcp() )
    {
      /* #30 Deny routing activation for active address on new socket connection. */
      (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, TesterAddr,                            /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        DOIP_ROUT_ACTIV_RES_CODE_ALREADY_ACTIV, 0u, NULL_PTR, TRUE);
    }
    else
    {
      if ( !DoIP_RxTcpRout_IsSockAvailable(DoIP_GetConnectionGroupIdxOfConnectionTcp(ConnectionTcpIdx)) )
      {
        /* #40 Deny routing activation if no free socket is available. */
        (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_ROUT_ACTIV_RES, TesterAddr,                          /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
          DOIP_ROUT_ACTIV_RES_CODE_SOCKETS_IN_USE, 0u, NULL_PTR, TRUE);
      }
      else
      {
        /* #50 Otherwise accept routing activation. */
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* DoIP_RxTcpRout_CheckConnectionAvailablility() */

/**********************************************************************************************************************
 *  DoIP_RxTcpRout_HandleInactivityTimeouts()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpRout_HandleInactivityTimeouts(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionTcpIterType connectionTcpIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connections. */
  for ( connectionTcpIdx = DoIP_GetConnectionTcpStartIdxOfInterface(InterfaceIdx);
        connectionTcpIdx < DoIP_GetConnectionTcpEndIdxOfInterface(InterfaceIdx);
        connectionTcpIdx++ )
  {
    const DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx);

    /* #20 Check if connection is online. */
    if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE )
    {
      /* #30 Check if routing is activated. */
      if ( DoIP_GetActivStateOfConnectionTcpDyn(connectionTcpIdx) == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
      {
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
        if ( DoIP_IsGeneralInactivityTimeWithAliveCheckOfInterface(InterfaceIdx) )
        {
          if ( DoIP_GetInactivCntOfConnectionTcpDyn(connectionTcpIdx) ==
            ((uint32)DoIP_GetGeneralInactivityTimeOfInterface(InterfaceIdx) -
              (uint32)DoIP_GetAliveCheckResTimeOfInterface(InterfaceIdx)) )
          {
            /* #40 Trigger alive check before general inactivity timeout (if enabled). */
            DoIP_RxTcpRout_TriggerAliveCheckSingleConnection((DoIP_SizeOfConnectionTcpType)connectionTcpIdx);
          }
        }
        else
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
        {
          if ( DoIP_GetInactivCntOfConnectionTcpDyn(connectionTcpIdx) ==
            DoIP_GetGeneralInactivityTimeOfInterface(InterfaceIdx) )
          {
            /* #50 Close socket after general inactivity timeout otherwise. */
            DoIP_Con_SoConClose(connectionIdx, TRUE);
          }
        }
      }
      else
      {
        if ( DoIP_GetInactivCntOfConnectionTcpDyn(connectionTcpIdx) ==
          DoIP_GetInitialInactivityTimeOfInterface(InterfaceIdx) )
        {
          /* #60 Otherwise close socket after inital timeout if no routing is activated. */
          DoIP_Con_SoConClose(connectionIdx, TRUE);
        }
      }
      DoIP_IncInactivCntOfConnectionTcpDyn(connectionTcpIdx);                                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
    }
  }
} /* DoIP_RxTcpRout_HandleInactivityTimeouts() */

/**********************************************************************************************************************
 *  DoIP_RxTcpAlive_HandleAliveCheckRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpAlive_HandleAliveCheckRes(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 bytesToCopy;
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u, hdrLenField); /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Check message lengths and send nack. */
  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR,       /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      FALSE);
  }
  else if ( hdrLenField != DOIP_ADDR_LEN_BYTE )
  {
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, 0u, 0u, NULL_PTR, TRUE); /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else
  {
    const uint64 msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;
    bytesToCopy = (uint32)(msgLength - DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx));                       /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */

    /* #20 Otherwise copy data to header buffer. */
    DoIP_RxTcpGen_CopyDataToHeaderBuffer(ConnectionTcpIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);                   /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */

    if ( DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx) == msgLength )
    {
      /* #30 Handle alive check response if message is now copied completely. */
      DoIP_RxTcpAlive_HandleCopiedAliveCheckRes(ConnectionTcpIdx);
    }
    else
    {
      /* #40 Otherwise set buffer size to length of pending message part. */
      uint32 bufSize = (uint32)(msgLength - DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx));                  /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */
      DoIP_Util_CheckBufferSizeAndModify(bufSize, BufferSizePtr);                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }
  }
} /* DoIP_RxTcpAlive_HandleAliveCheckRes() */

/**********************************************************************************************************************
 *  DoIP_RxTcpAlive_HandleCopiedAliveCheckRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpAlive_HandleCopiedAliveCheckRes(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  DoIP_SizeOfConnectionType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  uint16 testerAddr;
  uint16 testerAddrTmp;

  /* ----- Implementation ----------------------------------------------- */
  if ( testerIdx != DoIP_GetSizeOfTester() )
  {
    testerAddr = DoIP_GetTesterAddrOfTester(testerIdx);

#if ( DOIP_DEFAULT_TESTER_ENABLED == STD_ON )
    /* #10 Look up current address if default tester is used (if enabled). */
    if ( testerAddr == DOIP_ADDR_ANY )
    {
      testerAddr = DoIP_GetTesterAddrOfConnectionTcpDyn(ConnectionTcpIdx);
    }
#endif /* DOIP_DEFAULT_TESTER_ENABLED == STD_ON */

    IPBASE_GET_UINT16(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)),                 /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      DOIP_GEN_HDR_LEN_BYTE, testerAddrTmp);

    if ( testerAddrTmp == testerAddr )
    {
      /* #20 Remove alive check response if tester use expected address. */
      DoIP_RxTcpAlive_RemoveAliveCheckFromRoutActHandler(ConnectionTcpIdx);
    }
    else
    {
      /* #30 Otherwise close socket if tester use unexpected address. */
#if ( DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON )
      DoIP_Con_SoConClose(connectionIdx, FALSE);
#else
      DoIP_Con_SoConClose(connectionIdx, TRUE);
#endif /* DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON */
    }
  }

  /* finish message */
  DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
} /* DoIP_RxTcpAlive_HandleCopiedAliveCheckRes() */

/**********************************************************************************************************************
 *  DoIP_RxTcpAlive_RemoveAliveCheckFromRoutActHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpAlive_RemoveAliveCheckFromRoutActHandler(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_AliveCheckListIterType aliveCheckListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all alive check list elements. */
  for ( aliveCheckListIdx = 0u;
        aliveCheckListIdx < DoIP_GetSizeOfAliveCheckList();
        aliveCheckListIdx++ )
  {
    /* #20 Check if alive check list element belongs to the required connection. */
    if ( ConnectionTcpIdx == DoIP_GetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx) )
    {
      /* #30 Remove alive check from list. */
#if ( DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON )
      DoIP_SetAliveCheckCntOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
#endif /* DOIP_GEN_INACT_TIME_WITH_AC_ENABLED == STD_ON */
      DoIP_SetConnectionTcpIdxOfAliveCheckList(aliveCheckListIdx, DoIP_GetSizeOfConnectionTcp());                      /* SBSW_DOIP_CSL01 */
      DoIP_DecAliveCheckElemNumOfRoutActivHandler(0u);                                                                 /* SBSW_DOIP_RoutActivHandler */

      /* #40 Check if all expected alive check responses have been received. */
      if ( DoIP_GetAliveCheckElemNumOfRoutActivHandler(0u) == 0u )
      {
        /* #50 Trigger routing activation handling in next main function. */
        DoIP_SetAliveCheckCntOfRoutActivHandler(0u, DOIP_ALIVE_CHECK_WAIT_NEXT_MAIN);                                  /* SBSW_DOIP_RoutActivHandler */
      }

      break;
    }
  }
} /* DoIP_RxTcpAlive_RemoveAliveCheckFromRoutActHandler() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleDiagnosticMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_HandleDiagnosticMessage(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u, hdrLenField); /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Check message lengths and send nack. */
  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR,       /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      FALSE);
  }
  else if ( hdrLenField <= DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE )
  {
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_INV_LEN, 0u, 0u, NULL_PTR, TRUE);/* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
  else
  {
    /* #20 Otherwise handle diagnostic message. */
    DoIP_RxTcpDiag_HandleValidDiagnosticMessage(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);          /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }
} /* DoIP_RxTcpDiag_HandleDiagnosticMessage() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleValidDiagnosticMessage()
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
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_HandleValidDiagnosticMessage(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy logical address if not yet done. */
  retVal = DoIP_RxTcpDiag_CopyLogicalAddr(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);                /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */

  /* #20 Check if logical address could be copied. */
  if ( retVal == E_OK )
  {
#if ( DOIP_NACK_APPEND_REQ == STD_ON )
    /* #30 Check if acknowledge message shall contain user data and append them (if enabled). */
    DoIP_RxTcpDiag_AppendNackData(ConnectionTcpIdx, PduInfoPtr, ReadDataLenPtr);                                       /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

    /* #40 Copy the received user data to the local header buffer. */
    retVal = DoIP_RxTcpDiag_CopyUserDataToHeaderBuffer(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);   /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */

    /* #50 Check if copying has just succeeded. */
    if ( retVal == DOIP_E_JUST_COPIED )
    {
      const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx);

      /* #60 Check if channel is active on connection. */
      if ( channelIdx != DoIP_GetSizeOfChannel() )
      {
        uint32 hdrLenField;
        IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u,         /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
          hdrLenField);

#if ( DOIP_VERIFY_RX_PDU_ENABLED == STD_ON )
        /* #70 Verify Rx Pdu (if enabled). */
        if ( DoIP_RxTcpDiag_VerifyRxPdu(ConnectionTcpIdx, hdrLenField) == E_OK )
#endif /* DOIP_VERIFY_RX_PDU_ENABLED == STD_ON */
        {
          /* #80 Begin with StartOfReception procedure. */
          retVal = DoIP_RxTcpDiag_StartOfReception(ConnectionTcpIdx, channelIdx, hdrLenField, BufferSizePtr);          /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        }
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }

    /* #90 Copy user data on success. */
    if ( retVal == E_OK )
    {
      DoIP_RxTcpDiag_CopyUserData(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);                        /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    }
  }

  switch(retVal)
  {
    case E_NOT_OK:
      /* #100 Skip message if copying of data, verification of Rx Pdu or StartOfReception procedure failed. */
      DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      break;
    default: /* E_OK, DOIP_E_PENDING, DOIP_E_JUST_COPIED */
      /* nothing to do here */
      break;
  }
} /* DoIP_RxTcpDiag_HandleValidDiagnosticMessage() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyLogicalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyLogicalAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  const DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  if ( rxMsgOffset < (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) )
  {
    const uint32 bytesToCopy = (uint32)((DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) - rxMsgOffset);

    /* #10 Copy data to header buffer if there are bytes to copy. */
    DoIP_RxTcpGen_CopyDataToHeaderBuffer(ConnectionTcpIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);                   /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */

    if ( DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx) ==
          (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) )
    {
      /* #20 Handle logical addresses if message header was copied completely. */
      retVal = DoIP_RxTcpDiag_HandleCopiedLogicalAddr(ConnectionTcpIdx);
    }
    else
    {
      /* #30 Otherwise set buffer size to length of pending message part. */
      uint32 bufSize = (uint32)((DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_HDR_LEN_BYTE) -
        DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx));
      DoIP_Util_CheckBufferSizeAndModify(bufSize, BufferSizePtr);                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      retVal = DOIP_E_PENDING;
    }
  }

  return retVal;
} /* DoIP_RxTcpDiag_CopyLogicalAddr() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleCopiedLogicalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_HandleCopiedLogicalAddr(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_RxHdrBufType *rxHdrBuf = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx));
  uint16 srcAddr;
  uint16 tgtAddr;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + 0u), srcAddr);                                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + DOIP_ADDR_LEN_BYTE), tgtAddr);                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  if ( DoIP_GetActivStateOfConnectionTcpDyn(ConnectionTcpIdx) != DOIP_ROUT_ACTIV_STATE_ACTIVATED )
  {
    /* #10 Do nothing if routing activation is missing (message will be dropped). */
    /* This is the handling of the ISO FDIS 13400-2 (Ed2) DoIP-131 where the message is not responded */
  }
  else
#if ( DOIP_DEFAULT_TESTER_ENABLED == STD_ON )
  if ( (DoIP_GetTesterAddrOfTester(DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx)) == DOIP_ADDR_ANY) &&
       (DoIP_GetTesterAddrOfConnectionTcpDyn(ConnectionTcpIdx) != srcAddr) )
  {
    /* #20 Prepare to send error if default tester address is invalid (if enabled). */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_INVALID_SRC_ADDR, TRUE);
  }
  else if ( (DoIP_GetTesterAddrOfTester(DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx)) != DOIP_ADDR_ANY) &&
            (DoIP_GetTesterAddrOfTester(DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx)) != srcAddr) )
#else
  if ( DoIP_GetTesterAddrOfTester(DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx)) != srcAddr )
#endif /* DOIP_DEFAULT_TESTER_ENABLED == STD_ON */
  {
    /* #30 Prepare to send error if tester address is invalid. */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_INVALID_SRC_ADDR, TRUE);
  }
  else
  {
    /* #40 Otherwise handle target logical address. */
    retVal = DoIP_RxTcpDiag_HandleTargetLogicalAddress(ConnectionTcpIdx);
  }

  return retVal;
} /* DoIP_RxTcpDiag_HandleCopiedLogicalAddr() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleTargetLogicalAddress()
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
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_HandleTargetLogicalAddress(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_SizeOfRoutingActivationType routActIdx = DoIP_GetRoutingActivationIdxOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_RxHdrBufType *rxHdrBuf = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx));
  DoIP_SizeOfChannelType channelIdx;
  uint32 diagMsgLength;
  uint32 hdrLenField;
  uint16 srcAddr;
  uint16 tgtAddr;
#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
  /* use a local copy of the variable and pass them as function parameter so that they will not change during search */
  boolean pduSizeRouting = DoIP_IsPduSizeRoutingEnabled;
#else
  boolean pduSizeRouting = FALSE;
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(rxHdrBuf, 4u, hdrLenField);                                                                        /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + 0u), srcAddr);                                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + DOIP_ADDR_LEN_BYTE), tgtAddr);                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  diagMsgLength = (uint32)(hdrLenField - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE);

  /* #10 Look up routing activation channel. */
  /* diagMsgLength was already checked for valid length within DoIP_RxTcpDiag_HandleDiagnosticMessage() */
  channelIdx = DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutAct(routActIdx, tgtAddr, diagMsgLength, pduSizeRouting);

  if ( channelIdx == DoIP_GetSizeOfChannel() )
  {
    DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
    DoIP_InterfaceIdxOfConnectionType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(connectionIdx);

    channelIdx = DoIP_RxTcpDiag_GetChannelByTgtAddr(interfaceIdx, tgtAddr);

    /* #20 Prepare to send error depending on channel usage. */
    if ( channelIdx != DoIP_GetSizeOfChannel() )
    { /* target address is configured for an unreachable channel */
      DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
        DOIP_DIAG_MSG_RES_CODE_TGT_UNREACHABLE, FALSE);
    }
    else
    {
      /* target address is not configured */
      DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
        DOIP_DIAG_MSG_RES_CODE_UNKNOWN_TGT_ADDR, FALSE);
    }
  }
  else if ( DoIP_GetTpMaxLenOfChannel(channelIdx) < diagMsgLength )
  {
    /* #30 Prepare to send error if target protocol length exceeded. */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_MSG_TOO_LARGE, FALSE);
  }
#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
  else if ( (pduSizeRouting == TRUE) && (DoIP_GetMaxPduSizeOfChannel(channelIdx) < diagMsgLength) )
  {
    /* #40 Prepare to send error if Pdu length exceeded (if enabled). */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_MSG_TOO_LARGE, FALSE);
  }
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */
#if ( DOIP_VERIFY_TARGET_ADDR_ENABLED == STD_ON )
  else if ( DoIP_CallVerifyTargetAddrDirect(tgtAddr) != E_OK )                                                         /* SBSW_DOIP_FUNCTION_POINTER_CALL */
  {
    /* #50 Prepare to send error if target address is invalid (if enabled). */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_UNKNOWN_TGT_ADDR, FALSE);
  }
#endif /* DOIP_VERIFY_TARGET_ADDR_ENABLED == STD_ON */
  else
  {
    /* #60 Accept target address and message length. */
    DoIP_SetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx, channelIdx);                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetChannelRxActiveOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    /* #70 Reset buffered user data offset for new diagnostic message. */
    DoIP_SetBufferedUserDataOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                            /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_RxTcpDiag_HandleTargetLogicalAddress() */

#if ( DOIP_NACK_APPEND_REQ == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_AppendNackData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_AppendNackData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2CONST(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there are data to append. */
  if ( PduInfoPtr->SduLength > 0u )
  {
    const DoIP_SizeOfTesterType testerIdx = DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx);
    const DoIP_DiagNAckOffsetOfTesterDynType diagNAckOffset = DoIP_GetDiagNAckOffsetOfTesterDyn(testerIdx);
    const PduLengthType diagNAckNumByte = ((PduLengthType)DoIP_GetDiagNAckBufEndIdxOfTester(testerIdx) -
                                            (PduLengthType)DoIP_GetDiagNAckBufStartIdxOfTester(testerIdx));

    /* #20 Check if acknowledge message shall contain user data. */
    if ( diagNAckNumByte > 0u )
    {
      uint32 bytesToCopyTester = (diagNAckNumByte - diagNAckOffset);
      uint32 bytesToCopy = ((uint32)(PduInfoPtr->SduLength) - (uint32)(*ReadDataLenPtr));

      /* #30 Check and set max bytes to copy. */
      if ( bytesToCopy > bytesToCopyTester )
      {
        bytesToCopy = bytesToCopyTester;
      }

      /* #40 Copy data to ach/nack buffer if there are bytes to copy. */
      if ( bytesToCopy > 0u )
      {
        /* PRQA S 0310, 0314 2 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
        VStdLib_MemCpy(DOIP_A_P2VAR(void)DoIP_GetAddrDiagNAckBuf(DoIP_GetDiagNAckBufStartIdxOfTester(testerIdx) +      /* SBSW_DOIP_DiagNAckBuf_AND_CONST_POINTER_FORWARD */
          diagNAckOffset), DOIP_A_P2CONST(void)&PduInfoPtr->SduDataPtr[*ReadDataLenPtr], bytesToCopy);

        DoIP_SetDiagNAckOffsetOfTesterDyn(testerIdx, (diagNAckOffset + bytesToCopy));                                  /* SBSW_DOIP_RegisterTester */
      }
    }
  }
} /* DoIP_RxTcpDiag_AppendNackData() */
#endif /* DOIP_NACK_APPEND_REQ == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyUserDataToHeaderBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 5 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyUserDataToHeaderBuffer(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr) /* PRQA S 3673 */ /* MD_DoIP_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const uint32 msgReqMinLength = DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE;
  const uint32 msgBufferedMaxLength = msgReqMinLength + DOIP_BUFFERED_USER_DATA_LENGTH;
  DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
  boolean returnImmediately = FALSE;
  uint64 msgLength;
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u, hdrLenField); /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;

  /* #10 Check if all data are received to verify the Pdu and/or to send the requested data on start of reception. */
  if ( (rxMsgOffset >= msgReqMinLength) &&
       (rxMsgOffset <  msgLength) &&
       (rxMsgOffset <  msgBufferedMaxLength) )
  {
    const uint32 bytesToCopy = (uint32)DOIP_MIN(((uint64)msgBufferedMaxLength - rxMsgOffset), (msgLength -
        rxMsgOffset));

    /* #20 Copy data to header buffer if there are bytes to copy. */
    DoIP_RxTcpGen_CopyDataToHeaderBuffer(ConnectionTcpIdx, PduInfoPtr, ReadDataLenPtr, bytesToCopy);                   /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
    rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);

    if ( (rxMsgOffset == msgLength) ||
         (rxMsgOffset == msgBufferedMaxLength) )
    { /* message header copied completely */
      /* #30 Set return value to signalize that the data has just been copied. */
      retVal = DOIP_E_JUST_COPIED;
    }
    else
    {
      /* #40 Otherwise set the buffer size to the length of the pending message part. */
      uint32 bufSize = (uint32)DOIP_MIN(((uint64)msgBufferedMaxLength - rxMsgOffset), (msgLength - rxMsgOffset));      /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */
      DoIP_Util_CheckBufferSizeAndModify(bufSize, BufferSizePtr);                                                      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      retVal = DOIP_E_PENDING;
    }
    returnImmediately = TRUE;
  }

  /* #50 Check if message part is received completely. */
  if ( (returnImmediately == FALSE) &&
        ((rxMsgOffset >= msgBufferedMaxLength) ||
         (rxMsgOffset >= msgLength)) )
  {
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_RxTcpDiag_CopyUserDataToHeaderBuffer() */

#if ( DOIP_VERIFY_RX_PDU_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_VerifyRxPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_VerifyRxPdu(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint32 HdrLenField)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  const DoIP_RxHdrBufType *rxHdrBuf = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx));
  uint16 srcAddr;
  uint16 tgtAddr;
  const PduInfoType *pduInfoPtr;
# if ( DOIP_VERIFY_RX_PDU_MAX_DATA_LENGTH != 0u )
  PduInfoType pduInfo;
  pduInfo.SduDataPtr = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx) +
    (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE));
  pduInfo.SduLength =
    (PduLengthType)DOIP_MIN(DOIP_VERIFY_RX_PDU_MAX_DATA_LENGTH, (HdrLenField - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE));
  pduInfoPtr = &pduInfo;
# else
  pduInfoPtr = NULL_PTR;
# endif /* DOIP_VERIFY_RX_PDU_MAX_DATA_LENGTH != 0u */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Derive the source and target address from the header data. */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + 0u), srcAddr);                                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + DOIP_ADDR_LEN_BYTE), tgtAddr);                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #20 Verify Rx Pdu. */
  /* PRQA S 0310 3 */ /* MD_DoIP_11.4 */
  if ( DoIP_CallVerifyRxPduDirect(                                                                                     /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      (const SoAd_SockAddrType*)DoIP_GetAddrLocIpAddrOfConnectionTcpDyn(ConnectionTcpIdx),
      (const SoAd_SockAddrType*)DoIP_GetAddrRemIpAddrOfConnectionTcpDyn(ConnectionTcpIdx),
      srcAddr, tgtAddr, pduInfoPtr) != E_OK )
  {
    /* #30 Prepare to send an ack message and reject the diagnostic message if Rx Pdu is discarded. */
    /* For ack message srcAddr is not required. It is assigned when the message is added to the TxTcpQueue within the
     * DoIP_TxTcp_FinishTransmit() call. */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_ACK, tgtAddr, 0u, 0u, FALSE);
    retVal = E_NOT_OK;
  }
# if ( DOIP_VERIFY_RX_PDU_MAX_DATA_LENGTH == 0u )
  DOIP_DUMMY_STATEMENT(HdrLenField); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* DOIP_VERIFY_RX_PDU_MAX_DATA_LENGTH == 0u */

  return retVal;
} /* DoIP_RxTcpDiag_VerifyRxPdu() */
#endif /* DOIP_VERIFY_RX_PDU_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_StartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_StartOfReception(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  DoIP_SizeOfChannelType ChannelIdx,
  uint32 HdrLenField,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_RxHdrBufType *rxHdrBuf = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx));
  uint16 srcAddr;
  uint16 tgtAddr;
  const uint32 bytesLeft = (HdrLenField - DOIP_DIAG_HDR_LEN_BYTE);
  PduInfoType pduInfoStartOfReception;
  pduInfoStartOfReception.SduDataPtr = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx) +
      (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_HDR_LEN_BYTE));
  pduInfoStartOfReception.SduLength = (PduLengthType)DOIP_MIN(DOIP_USER_DATA_SIZE_ON_START_OF_RECEPTION, bytesLeft);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Derive the source and target address from the header data. */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + 0u), srcAddr);                                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  IPBASE_GET_UINT16(rxHdrBuf, (DOIP_GEN_HDR_LEN_BYTE + DOIP_ADDR_LEN_BYTE), tgtAddr);                                  /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #20 Call start of reception at PduR. */
  if ( PduR_DoIPTpStartOfReception(DoIP_GetPduRRxPduIdOfChannel(ChannelIdx), &pduInfoStartOfReception,                 /* SBSW_DOIP_PARAM_VALID_POINTER_AND_NULL_POINTER_FORWARD */
      (PduLengthType)bytesLeft, BufferSizePtr) != BUFREQ_OK )
  {
    /* #30 Prepare to send error if no buffer was obtained. */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, tgtAddr, srcAddr,
      DOIP_DIAG_MSG_RES_CODE_OUT_OF_MEMORY, FALSE);
  }
  else
  {
    PduInfoType pduInfoCopyRxData = { NULL_PTR, 0u };

    /* #40 Otherwise set target address for this socket connection and copy data to PduR. */
    DoIP_SetLastRxTgtAddrOfConnectionTcpDyn(ConnectionTcpIdx, tgtAddr);                                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    retVal = DoIP_RxTcpDiag_CopyDataToPduR(ConnectionTcpIdx, BufferSizePtr, &pduInfoCopyRxData);                       /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

    if ( retVal == E_OK )
    {
      if ( bytesLeft < *BufferSizePtr )
      { /* upper layer has more buffer than expected */
        *BufferSizePtr = (PduLengthType)bytesLeft;                                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      }
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }
  }

  return retVal;
} /* DoIP_RxTcpDiag_StartOfReception() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_CopyUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy buffered user data. */
  if ( DoIP_RxTcpDiag_CopyBufferedUserData(ConnectionTcpIdx, BufferSizePtr, FALSE) == E_OK )                           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    /* #20 Copy unbuffered user data on success. */
    DoIP_RxTcpDiag_CopyUnbufferedUserData(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);                /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }
  else
  {
    /* nothing to do on E_NOT_OK and DOIP_E_PENDING */
  }
} /* DoIP_RxTcpDiag_CopyUserData() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyBufferedUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyBufferedUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  boolean QueryBufSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const uint32 msgReqMinLength = DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE;
  const uint32 msgBufferedMaxLength = msgReqMinLength + DOIP_BUFFERED_USER_DATA_LENGTH;
  const DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
  const DoIP_BufferedUserDataOffsetOfConnectionTcpDynType bufferedUserDataOffset =
    DoIP_GetBufferedUserDataOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
  uint32 msgHdrLength;
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u, hdrLenField); /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  msgHdrLength = DOIP_GEN_HDR_LEN_BYTE + hdrLenField;

  if ( /* less than Pdu-max-data-length */
       ( (rxMsgOffset == msgHdrLength) &&
         (bufferedUserDataOffset < ((DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_BUFFERED_USER_DATA_LENGTH) - hdrLenField)) )
       ||
       /* at least Pdu-max-data-length */
       ( (rxMsgOffset == msgBufferedMaxLength) &&
         (bufferedUserDataOffset < DOIP_BUFFERED_USER_DATA_LENGTH) )
     )
  {
    /* #10 Copy buffered user data from header buffer to PduR if there are data to copy. */
    retVal = DoIP_RxTcpDiag_HandleCopyBufferedUserData(ConnectionTcpIdx, hdrLenField, BufferSizePtr, QueryBufSize);    /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  if ( (retVal != DOIP_E_PENDING) &&
       ( /* less than Pdu-max-data-length */
         ( (rxMsgOffset == msgHdrLength) &&
           (bufferedUserDataOffset >= ((DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_BUFFERED_USER_DATA_LENGTH)
               - hdrLenField)) )
         ||
         /* at least Pdu-max-data-length */
         ( (rxMsgOffset >= msgBufferedMaxLength) &&
           (bufferedUserDataOffset >= DOIP_BUFFERED_USER_DATA_LENGTH)
         )
       )
     )
  {
    /* #20 Check if all verified data are copied. */
    retVal = E_OK;
  }

  return retVal;
} /* DoIP_RxTcpDiag_CopyBufferedUserData() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleCopyBufferedUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_HandleCopyBufferedUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  uint32 HdrLenField,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  boolean QueryBufSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduInfoType pduInfo = { NULL_PTR, 0u };
  DoIP_BufferedUserDataOffsetOfConnectionTcpDynType bufferedUserDataOffset =
    DoIP_GetBufferedUserDataOffsetOfConnectionTcpDyn(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  pduInfo.SduDataPtr = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx) +
    (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + (DoIP_SizeOfRxHdrBufType)bufferedUserDataOffset));

  if ( QueryBufSize == TRUE )
  {
    /* #10 Query buffer size if not yet done. */
    pduInfo.SduLength = 0u;
    retVal = DoIP_RxTcpDiag_CopyDataToPduR(ConnectionTcpIdx, BufferSizePtr, &pduInfo);                                 /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  if ( retVal == E_OK )
  {
    pduInfo.SduLength =
      (PduLengthType)DOIP_MIN(DOIP_BUFFERED_USER_DATA_LENGTH, (HdrLenField - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE));
    if ( pduInfo.SduLength > *BufferSizePtr )
    {
      pduInfo.SduLength = *BufferSizePtr;
    }

    /* #20 Copy buffered user data to PduR. */
    retVal = DoIP_RxTcpDiag_CopyDataToPduR(ConnectionTcpIdx, BufferSizePtr, &pduInfo);                                 /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  if ( retVal == E_OK )
  {
    /* #30 Set calculate buffer size of pending message part if data could be copied. */
    uint32 bytesLeft;

    bufferedUserDataOffset += pduInfo.SduLength;
    DoIP_SetBufferedUserDataOffsetOfConnectionTcpDyn(ConnectionTcpIdx, bufferedUserDataOffset);                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    if ( HdrLenField < (DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_BUFFERED_USER_DATA_LENGTH) )
    {
      bytesLeft = (HdrLenField - bufferedUserDataOffset - DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE);
    }
    else if ( bufferedUserDataOffset < DOIP_BUFFERED_USER_DATA_LENGTH )
    {
      bytesLeft = (DOIP_BUFFERED_USER_DATA_LENGTH - (uint32)bufferedUserDataOffset);
    }
    else
    {
      bytesLeft = (HdrLenField - (DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_BUFFERED_USER_DATA_LENGTH));
    }

    if ( bytesLeft < *BufferSizePtr )
    { /* upper layer has more buffer than expected */
      *BufferSizePtr = (PduLengthType)bytesLeft;                                                                       /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    }
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    if ( bytesLeft == 0u )
    {
      /* #40 Send diagnostic message positive acknowledge and finish message if entire message is processed. */
      (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_ACK,                                              /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        DoIP_GetLastRxTgtAddrOfConnectionTcpDyn(ConnectionTcpIdx), 0u, 0u, NULL_PTR, FALSE);
      /* call PduR */
      PduR_DoIPTpRxIndication(DoIP_GetPduRRxPduIdOfChannel(
        DoIP_GetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx)), E_OK);
      /* finish message */
      DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      DoIP_SetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx, DoIP_GetSizeOfChannel());                           /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      DoIP_SetChannelRxActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }
    else
    {
      retVal = DOIP_E_PENDING;
    }
  }

  return retVal;
} /* DoIP_RxTcpDiag_HandleCopyBufferedUserData() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyUnbufferedUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_CopyUnbufferedUserData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there are bytes to copy. */
  if ( rxMsgOffset >= (DOIP_GEN_HDR_LEN_BYTE + DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_BUFFERED_USER_DATA_LENGTH) )
  {
    IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u,             /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      hdrLenField);

    if ( DoIP_IsChannelRxActiveOfConnectionTcpDyn(ConnectionTcpIdx) )
    {
      /* #20 Copy data to PduR if channel is still active. */
      retVal = DoIP_RxTcpDiag_CopyDataToPduR(ConnectionTcpIdx, BufferSizePtr, PduInfoPtr);                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    }

    if ( retVal == E_OK )
    {
      /* #30 Set calculate buffer size of pending message part if data could be copied. */
      const uint64 msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;
      const uint32 bytesLeft = (uint32)(msgLength - (rxMsgOffset + (uint64)(PduInfoPtr->SduLength)));                  /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */

      if ( bytesLeft < *BufferSizePtr )
      { /* upper layer has more buffer than expected */
        *BufferSizePtr = (PduLengthType)bytesLeft;                                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      }
      DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

      if ( bytesLeft == 0u )
      {
        /* #40 Send diagnostic message positive acknowledge and finish message if entire message is processed. */
        (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_ACK,                                            /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
          DoIP_GetLastRxTgtAddrOfConnectionTcpDyn(ConnectionTcpIdx), 0u, 0u, NULL_PTR, FALSE);
        /* call PduR */
        PduR_DoIPTpRxIndication(DoIP_GetPduRRxPduIdOfChannel(
          DoIP_GetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx)), E_OK);
        /* finish message */
        DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                   /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        DoIP_SetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx, DoIP_GetSizeOfChannel());                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        DoIP_SetChannelRxActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                            /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
      else
      {
        /* #50 Otherwise another part of the message is received so increment offset. */
        DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, (rxMsgOffset + PduInfoPtr->SduLength));                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }

    *ReadDataLenPtr += PduInfoPtr->SduLength;                                                                          /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }
} /* DoIP_RxTcpDiag_CopyUnbufferedUserData() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CopyDataToPduR()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_CopyDataToPduR(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) LocalPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  const DoIP_PduRRxPduIdOfChannelType rxPduId =
    DoIP_GetPduRRxPduIdOfChannel(DoIP_GetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to copy data to PduR. */
  if ( PduR_DoIPTpCopyRxData(rxPduId, LocalPduInfoPtr, BufferSizePtr) != BUFREQ_OK )                                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    /* #20 Look up current tester adress. */
    const DoIP_LastRxTgtAddrOfConnectionTcpDynType entityAddr =
      DoIP_GetLastRxTgtAddrOfConnectionTcpDyn(ConnectionTcpIdx);
    DoIP_TesterAddrOfTesterType testerAddr =
      DoIP_GetTesterAddrOfTester(DoIP_GetTesterIdxOfConnectionTcpDyn(ConnectionTcpIdx));

#if ( DOIP_DEFAULT_TESTER_ENABLED == STD_ON )
    if ( testerAddr == DOIP_ADDR_ANY )
    {
      /* #30 Look up current address if default tester is used (if enabled). */
      testerAddr = DoIP_GetTesterAddrOfConnectionTcpDyn(ConnectionTcpIdx);
    }
#endif /* DOIP_DEFAULT_TESTER_ENABLED == STD_ON */

    /* #40 Prepare to send error and skip message. */
    DoIP_TxTcp_PrepareTransmit(ConnectionTcpIdx, DOIP_HDR_TYPE_DIAG_NACK, entityAddr, testerAddr,
      DOIP_DIAG_MSG_RES_CODE_NETWORK_TRANSPROT, FALSE);
    /* call PduR */
    PduR_DoIPTpRxIndication(rxPduId, E_NOT_OK);
    DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetChannelRxLastIdxOfConnectionTcpDyn(ConnectionTcpIdx, DoIP_GetSizeOfChannel());                             /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    DoIP_SetChannelRxActiveOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

    retVal = E_NOT_OK;
  }

  return retVal;
} /* DoIP_RxTcpDiag_CopyDataToPduR() */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutAct()
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
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutAct(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  boolean PduSizeRouting)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_IF_API_ENABLED == STD_ON )
  DoIP_SizeOfChannelType channelIdx = DoIP_GetSizeOfChannel();
#else
  DoIP_SizeOfChannelType channelIdx;
#endif /* DOIP_IF_API_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_IF_API_ENABLED == STD_ON )
  /* #10 Check if at least one channel is referred by the routing activation (if enabled). */
  if ( DoIP_IsChannelIndUsedOfRoutingActivation(RoutActIdx) )
#endif /* DOIP_IF_API_ENABLED == STD_ON */
  {
    /* #20 Check if overwrite local target address API is enabled. */
#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON )
    {
      /* #200 Use linear search to find the correct channel. */
      channelIdx = DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseIteration(RoutActIdx, TgtAddr);
    }
#else
    /* #30 Otherwise. */
    {
# if ( DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON )
      /* #300 If routing activation refer to at least one channel with bitmasking iterate to find correct channel. */
      if ( DoIP_IsBitmaskingUsedOfRoutingActivation(RoutActIdx) )
      {
        channelIdx = DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseIteration(RoutActIdx, TgtAddr);
      }
      /* #301 Otherwise use binary search to find correct channel. */
      else
# endif /* DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON */
      {
        channelIdx = DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseBinSearch(RoutActIdx, TgtAddr, DataLen,
            PduSizeRouting);
      }
    }
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON */
  }

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON )
  DOIP_DUMMY_STATEMENT(DataLen);
  DOIP_DUMMY_STATEMENT(PduSizeRouting);
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON */

  return channelIdx;
} /* DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutAct() */

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseBinSearch()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseBinSearch(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  boolean PduSizeRouting)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelType channelIdx = DoIP_GetSizeOfChannel();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search channel with matching target address. */
  const DoIP_SizeOfChannelIndType indIdxFound = DoIP_RxTcpDiag_SearchChannel(RoutActIdx, TgtAddr);

  if ( indIdxFound != DoIP_GetSizeOfChannelInd() )
  {
    /* #20 Check if there is an other better fitting channel. */
    DoIP_SizeOfChannelIndType channelIndIdx = DoIP_RxTcpDiag_CheckChannel(RoutActIdx, TgtAddr, DataLen, indIdxFound,
      PduSizeRouting);
    channelIdx = DoIP_GetChannelInd(channelIndIdx);
  }

  return channelIdx;
} /* DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseBinSearch() */
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF */

#if ( (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseIteration()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseIteration(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelType channelIdx = DoIP_GetSizeOfChannel();
  DoIP_SizeOfChannelIndType channelIndIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all channels of the routing activation. */
  for ( channelIndIdx = DoIP_GetChannelIndStartIdxOfRoutingActivation(RoutActIdx);
        channelIndIdx < DoIP_GetChannelIndEndIdxOfRoutingActivation(RoutActIdx);
        channelIndIdx++ )
  {
    DoIP_SizeOfChannelType channelIdxTmp = DoIP_GetChannelInd(channelIndIdx);
    const uint16 ecuAddr = DoIP_Util_GetEcuAddr(channelIdxTmp);
    const DoIP_BitMaskOfChannelType bitMask = DoIP_GetBitMaskOfChannel(channelIdxTmp);

    /* #20 Check if bitmask match and choose matching channel. */
    if ( (ecuAddr & bitMask) == (TgtAddr & bitMask) )
    {
      channelIdx = channelIdxTmp;
      break;
    }
  }

  return channelIdx;
} /* DoIP_RxTcpDiag_GetChannelByTgtAddrForRoutActUseIteration() */
#endif /* (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddr(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  uint16 TgtAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelType channelIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get channel by target address. */
#if ( (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) )
  channelIdx = DoIP_RxTcpDiag_GetChannelByTgtAddrUseIteration(InterfaceIdx, TgtAddr);
#else
  channelIdx = DoIP_RxTcpDiag_GetChannelByTgtAddrUseBinSearch(InterfaceIdx, TgtAddr);
#endif /* (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) */

  return channelIdx;
} /* DoIP_RxTcpDiag_GetChannelByTgtAddr() */

#if ( (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_OFF) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrUseBinSearch()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrUseBinSearch(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  uint16 TgtAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelType channelIdx = DoIP_GetSizeOfChannel();
  DoIP_SizeOfChannelType channelIdxLeft = DoIP_GetChannelStartIdxOfInterface(InterfaceIdx);
  DoIP_SizeOfChannelType channelIdxRight =
    (DoIP_SizeOfChannelType)(DoIP_GetChannelEndIdxOfInterface(InterfaceIdx) - 1u);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all channels (binary search) until matching channel was found. */
  while ( channelIdxLeft <= channelIdxRight )
  {
    /* #20 Calculate middle index. */
    boolean searchFinished = FALSE;
    const DoIP_SizeOfChannelType channelIdxMiddle =
      (DoIP_SizeOfChannelType)(channelIdxLeft + ((DoIP_SizeOfChannelType)(channelIdxRight - channelIdxLeft) / 2u));
    const uint16 ecuAddr = DoIP_Util_GetEcuAddr(channelIdxMiddle);

    if ( ecuAddr == TgtAddr )
    {
      /* #30 Finish search if matching index was found. */
      channelIdx = channelIdxMiddle;
      searchFinished = TRUE;
    }
    else
    {
      /* #40 Otherwise recalculate indexes for next iteration. */
      if ( ecuAddr > TgtAddr )
      { /* target address of found channel is bigger */
        if ( channelIdxMiddle == DoIP_GetChannelStartIdxOfInterface(InterfaceIdx) )
        {
          searchFinished = TRUE;
        }
        else
        {
          channelIdxRight = (DoIP_SizeOfChannelType)(channelIdxMiddle - 1u);
        }
      }
      else
      { /* target address of found channel is smaller */
        if ( channelIdxMiddle == (DoIP_GetChannelEndIdxOfInterface(InterfaceIdx) - 1u) )
        {
          searchFinished = TRUE;
        }
        else
        {
          channelIdxLeft = (DoIP_SizeOfChannelType)(channelIdxMiddle + 1u);
        }
      }
    }

    /* #50 Finish search if index found or whole range was checked. */
    if ( searchFinished == TRUE )
    {
      break;
    }
  }

  return channelIdx;
} /* DoIP_RxTcpDiag_GetChannelByTgtAddrUseBinSearch() */
#endif /* (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_OFF) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

#if ( (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_GetChannelByTgtAddrUseIteration()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelType, DOIP_CODE) DoIP_RxTcpDiag_GetChannelByTgtAddrUseIteration(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  uint16 TgtAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelType channelIdx = DoIP_GetSizeOfChannel();
  DoIP_ChannelIterType idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all channels until matching channel was found. */
  for ( idx = DoIP_GetChannelStartIdxOfInterface(InterfaceIdx);
        idx < DoIP_GetChannelEndIdxOfInterface(InterfaceIdx);
        idx++ )
  {
    const uint16 ecuAddr = DoIP_Util_GetEcuAddr(idx);
    const DoIP_BitMaskOfChannelType bitMask = DoIP_GetBitMaskOfChannel(idx);

    if ( (ecuAddr & bitMask) == (TgtAddr & bitMask) )
    {
      channelIdx = (DoIP_SizeOfChannelType)idx;
      break;
    }
  }

  return channelIdx;
} /* DoIP_RxTcpDiag_GetChannelByTgtAddrUseIteration() */
#endif /* (DOIP_TARGET_ADDR_BIT_MASK_ENABLED == STD_ON) || (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON) */

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_SearchChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_SearchChannel(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelIndType indIdxFound = DoIP_GetSizeOfChannelInd();
  DoIP_SizeOfChannelIndType idxLeft = DoIP_GetChannelIndStartIdxOfRoutingActivation(RoutActIdx);
  DoIP_SizeOfChannelIndType idxRight =
    (DoIP_SizeOfChannelIndType)(DoIP_GetChannelIndEndIdxOfRoutingActivation(RoutActIdx) - 1u);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all channels (binary search) until matching channel was found. */
  while ( idxLeft <= idxRight )
  {
    /* #20 Calculate middle index to get corresponding channel. */
    boolean searchFinished = FALSE;
    const DoIP_SizeOfChannelIndType idxMiddle =
      (DoIP_SizeOfChannelIndType)(idxLeft + ((DoIP_SizeOfChannelIndType)(idxRight - idxLeft) / 2u));
    const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelInd(idxMiddle);
    const uint16 ecuAddr = DoIP_Util_GetEcuAddr(channelIdx);

    if ( ecuAddr == TgtAddr )
    {
      /* #30 Finish search if matching index was found. */
      indIdxFound = idxMiddle;
      searchFinished = TRUE;
    }
    else
    {
      /* #40 Otherwise recalculate indexes for next iteration. */
      if ( ecuAddr > TgtAddr )
      { /* target address of found channel is bigger */
        if ( idxMiddle == DoIP_GetChannelIndStartIdxOfRoutingActivation(RoutActIdx) )
        {
          searchFinished = TRUE;
        }
        else
        {
          idxRight = (DoIP_SizeOfChannelIndType)(idxMiddle - 1u);
        }
      }
      else
      { /* target address of found channel is smaller */
        if ( idxMiddle == (DoIP_SizeOfChannelIndType)(DoIP_GetChannelIndEndIdxOfRoutingActivation(RoutActIdx) - 1u) )
        {
          searchFinished = TRUE;
        }
        else
        {
          idxLeft = (DoIP_SizeOfChannelIndType)(idxMiddle + 1u);
        }
      }
    }

    /* #50 Finish search if index found or whole range was checked. */
    if ( searchFinished == TRUE )
    {
      break;
    }
  }

  return indIdxFound;
} /* DoIP_RxTcpDiag_SearchChannel() */
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF */

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_CheckChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 3206 5 */ /* MD_DoIP_3206 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_CheckChannel(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  DoIP_SizeOfChannelIndType ChannelIndIdx,
  boolean PduSizeRouting)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelIndType retChannelIndIdx = ChannelIndIdx;
# if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
  DoIP_MaxPduSizeOfChannelType bestMaxPduSize = 0u;
  boolean bestChannelFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if current channel is best. */
  if ( (DoIP_RxTcpDiag_PduSizeRoutValidateChannel(DoIP_GetChannelInd(retChannelIndIdx), DataLen, &bestMaxPduSize,      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    PduSizeRouting) == E_OK) && ((PduSizeRouting == FALSE) || (DataLen == (uint32)bestMaxPduSize)) )
  { /* channel is default channel and Pdu size dependent routing is disabled or exact match is found */
    bestChannelFound = TRUE;
  }

  /* #20 Otherwise search best channel from start index to the right. */
  if ( bestChannelFound == FALSE )
  {
    DoIP_SizeOfChannelIndType idxLeft =
      DoIP_RxTcpDiag_SearchPduSizeRoutingGroupStartIndex(RoutActIdx, TgtAddr, ChannelIndIdx);

    retChannelIndIdx = DoIP_RxTcpDiag_SearchPduSizeRoutingGroupBestIndex(RoutActIdx, TgtAddr, DataLen, idxLeft,
      PduSizeRouting);
  }
# else
  DOIP_DUMMY_STATEMENT(RoutActIdx);     /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT(TgtAddr);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT(DataLen);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT(PduSizeRouting); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */

  return retChannelIndIdx;
} /* DoIP_RxTcpDiag_CheckChannel() */
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF */

#if ( (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_SearchPduSizeRoutingGroupStartIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_SearchPduSizeRoutingGroupStartIndex(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  DoIP_SizeOfChannelIndType ChannelIndIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelIndType retChannelIndIdx = ChannelIndIdx;
  DoIP_SizeOfChannelIndType idx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Perform a search to find left index of pdu size routing group. */
  for ( idx = 0u;
        idx <= (ChannelIndIdx - DoIP_GetChannelIndStartIdxOfRoutingActivation(RoutActIdx));
        idx++ )
  {
    const DoIP_SizeOfChannelIndType idxTmp = (DoIP_SizeOfChannelIndType)(ChannelIndIdx - idx);
    const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelInd(idxTmp);
    const uint16 ecuAddr = DoIP_Util_GetEcuAddr(channelIdx);

    if ( ecuAddr == TgtAddr )
    {
      retChannelIndIdx = idxTmp;
    }
    else
    {
      /* #20 Stop search if ECU address does not match. */
      break;
    }
  }

  return retChannelIndIdx;
} /* DoIP_RxTcpDiag_SearchPduSizeRoutingGroupStartIndex() */
#endif /* (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

#if ( (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_SearchPduSizeRoutingGroupBestIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DOIP_LOCAL FUNC(DoIP_SizeOfChannelIndType, DOIP_CODE) DoIP_RxTcpDiag_SearchPduSizeRoutingGroupBestIndex(
  DoIP_SizeOfRoutingActivationType RoutActIdx,
  uint16 TgtAddr,
  uint32 DataLen,
  DoIP_SizeOfChannelIndType ChannelIndStartIdx,
  boolean PduSizeRouting)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfChannelIndType retChannelIndIdx = ChannelIndStartIdx;
  DoIP_SizeOfChannelIndType idx;
  DoIP_MaxPduSizeOfChannelType bestMaxPduSize = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for ( idx = ChannelIndStartIdx;
        idx < DoIP_GetChannelIndEndIdxOfRoutingActivation(RoutActIdx);
        idx++ )
  {
    boolean searchFinishedRight = FALSE;
    const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelInd(idx);
    const uint16 ecuAddr = DoIP_Util_GetEcuAddr(channelIdx);

    if ( ecuAddr == TgtAddr )
    {
      /* #10 Look for all match channels for the best fitting channel up to now. */
      if ( DoIP_RxTcpDiag_PduSizeRoutValidateChannel(channelIdx, DataLen, &bestMaxPduSize, PduSizeRouting) == E_OK )   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      {
        retChannelIndIdx = idx;
        if ( (PduSizeRouting == FALSE) || (DataLen == bestMaxPduSize) )
        {
          /* #20 Finish search if default channel or exact match is found. */
          searchFinishedRight = TRUE;
        }
      }
    }
    else
    {
      /* #30 Otherwise stop search if ECU address does not match. */
      searchFinishedRight = TRUE;
    }

    if ( searchFinishedRight == TRUE )
    {
      break;
    }
  }

  return retChannelIndIdx;
} /* DoIP_RxTcpDiag_SearchPduSizeRoutingGroupBestIndex() */
#endif /* (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

#if ( (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) )
/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_PduSizeRoutValidateChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpDiag_PduSizeRoutValidateChannel(
  DoIP_SizeOfChannelType ChannelIdx,
  uint32 DataLen,
  P2VAR(DoIP_MaxPduSizeOfChannelType, AUTOMATIC, DOIP_APPL_VAR) BestMaxPduSize,
  boolean PduSizeRouting)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_MaxPduSizeOfChannelType maxPduSize = DoIP_GetMaxPduSizeOfChannel(ChannelIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Pdu size depending routing is enabled. */
  if ( PduSizeRouting == TRUE )
  {
    if ( (maxPduSize >= DataLen) &&
          ((*BestMaxPduSize == 0u) ||
           (*BestMaxPduSize > maxPduSize)) )
    {
      /* #20 Check if max Pdu size is better fitting. */
      *BestMaxPduSize = maxPduSize;                                                                                    /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      retVal = E_OK;
    }
  }
  else
  {
    /* #30 Otherwise check if channel is the default channel. */
    if ( DoIP_IsDefaultChannelOfChannel(ChannelIdx) )
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* DoIP_RxTcpDiag_PduSizeRoutValidateChannel() */
#endif /* (DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON) && (DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_OFF) */

/**********************************************************************************************************************
 *  DoIP_RxTcpDiag_HandleRetryCopyBufferedUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpDiag_HandleRetryCopyBufferedUserData(
  DoIP_InterfaceIterType InterfaceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionTcpIterType connectionTcpIdx;
  uint16 hdrTypeField;
  uint32 hdrLenField;

  /* ----- Implementation ----------------------------------------------- */
  for ( connectionTcpIdx = DoIP_GetConnectionTcpStartIdxOfInterface(InterfaceIdx);
        connectionTcpIdx < DoIP_GetConnectionTcpEndIdxOfInterface(InterfaceIdx);
        connectionTcpIdx++ )
  {
    const DoIP_RxHdrBufType *rxHdrBuf = DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(
        connectionTcpIdx));

    IPBASE_GET_UINT16(rxHdrBuf, 2u, hdrTypeField);                                                                     /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

    /* #10 Check for all activated TCP connection if there is a pending diagnostic request. */
    if ( (DoIP_GetActivStateOfConnectionTcpDyn(connectionTcpIdx) == DOIP_ROUT_ACTIV_STATE_ACTIVATED) &&
         (hdrTypeField == DOIP_HDR_TYPE_DIAG_REQ) )
    {
      const DoIP_SkipRxMsgOfConnectionTcpDynType skipRxMsg =
        (DoIP_SkipRxMsgOfConnectionTcpDynType)DoIP_IsSkipRxMsgOfConnectionTcpDyn(connectionTcpIdx);
      const DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset =
        DoIP_GetRxMsgOffsetOfConnectionTcpDyn(connectionTcpIdx);
      uint64 msgLength;

      IPBASE_GET_UINT32(rxHdrBuf, 4u, hdrLenField);                                                                    /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

      msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;

      /* #20 Copy buffered user data if there is some data and message should not be skipped. */
      if ( (skipRxMsg == FALSE) && (hdrLenField >= DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE) )
      {
        PduLengthType bufferSize = DoIP_GetLastRxBufSizeOfConnectionTcpDyn(connectionTcpIdx);
        (void)DoIP_RxTcpDiag_CopyBufferedUserData((DoIP_SizeOfConnectionTcpType)connectionTcpIdx, &bufferSize, TRUE);  /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      }

      /* #30 Finish message if no user data is present or skipped message is fully received. */
      if ( /* skip message if all data are received */
           ( (skipRxMsg == TRUE) &&
             (rxMsgOffset >= msgLength) ) ||
           /* finish message if no user data present */
           ( (hdrLenField <= (DOIP_DIAG_MSG_REQ_MIN_LEN_BYTE + DOIP_BUFFERED_USER_DATA_LENGTH)) &&
             (rxMsgOffset == msgLength) ) )
      {
        DoIP_TxTcp_FinishTransmit((DoIP_SizeOfConnectionTcpType)connectionTcpIdx);

        DoIP_SetLastRxBufSizeOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                 /* SBSW_DOIP_CSL02 */
        DoIP_SetRxMsgOffsetOfConnectionTcpDyn(connectionTcpIdx, 0u);                                                   /* SBSW_DOIP_CSL02 */
        DoIP_SetSkipRxMsgOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                                  /* SBSW_DOIP_CSL02 */
      }
    }
  }
} /* DoIP_RxTcpDiag_HandleRetryCopyBufferedUserData() */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_RxTcpOem_HandleOemPayloadTypes()
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
/* PRQA S 3673 5 */ /* MD_DoIP_16.7 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpOem_HandleOemPayloadTypes(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfConnectionType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  uint32 hdrLenField;
  uint8 genNackCode = DOIP_GEN_NACK_OK;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u, hdrLenField); /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

  /* #10 Check message lengths and send nack if it is longer than supported. */
  if ( hdrLenField > DOIP_MAX_REQUEST_BYTES )
  {
    DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_MAX_LEN, 0u, 0u, NULL_PTR,       /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
      FALSE);
  }
  /* #20 Check if OEM specific reception is supported on the current connection otherwise. */
  else if ( DoIP_IsOemSpecificCfgUsedOfConnection(connectionIdx) &&
    DoIP_IsOemPayloadRxBufConfigIndUsedOfOemSpecificCfg(DoIP_GetOemSpecificCfgIdxOfConnection(connectionIdx)) )
  {
    const DoIP_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIP_GetOemSpecificCfgIdxOfConnection(connectionIdx);

    /* #30 Check if a buffer needs to be reserved. */
    if ( DoIP_GetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx) ==
      DoIP_GetSizeOfOemPayloadRxBufConfig() )
    {
      genNackCode = DoIP_RxTcpOem_ReserveBuf(connectionIdx, (PduLengthType)hdrLenField);
    }

    /* #40 Copy data in case a buffer has been reserved. */
    if ( genNackCode == DOIP_GEN_NACK_OK )
    {
      DoIP_RxTcpOem_CopyPayload(ConnectionTcpIdx, PduInfoPtr, BufferSizePtr, ReadDataLenPtr);                          /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
    }
    /* #41 Send generic nack with error code dependent on the reason why the buffer could not be reserved otherwise. */
    else
    {
      DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, genNackCode, 0u, 0u, NULL_PTR, FALSE);       /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
    }
  }
  /* #21 Send generic nack otherwise. */
  else
  {
    DoIP_SetSkipRxMsgOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, 0u, 0u, NULL_PTR, FALSE);  /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
  }
} /* DoIP_RxTcpOem_HandleOemPayloadTypes() */

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_ReserveBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(uint8, DOIP_CODE) DoIP_RxTcpOem_ReserveBuf(
  DoIP_SizeOfConnectionType ConnectionIdx,
  PduLengthType UserDataLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIP_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);
  DoIP_OemPayloadRxBufConfigIterType oemRxBufCfgIter;
  uint8 genNackCode = DOIP_GEN_NACK_TYPE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for an OEM Payload Rx buffer which can be used for the reception. */
  for ( oemRxBufCfgIter = DoIP_GetOemPayloadRxBufConfigIndStartIdxOfOemSpecificCfg(oemSpecificCfgIdx);
    oemRxBufCfgIter < DoIP_GetOemPayloadRxBufConfigIndEndIdxOfOemSpecificCfg(oemSpecificCfgIdx);
    oemRxBufCfgIter++ )
  {
    const DoIP_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx = DoIP_GetOemPayloadRxBufConfigInd(oemRxBufCfgIter);

    /* Set the next error level as there is at least one buffer configured for the connection. */
    genNackCode = DOIP_MAX(genNackCode, DOIP_GEN_NACK_MAX_LEN);

    /* #100 Check that the buffer is big enough for the received data. */
    if ( UserDataLength <=
      (PduLengthType)(DoIP_GetOemPayloadRxBufEndIdxOfOemPayloadRxBufConfig(oemRxBufCfgIdx) -                           /* PRQA S 4391 */ /* MD_DoIP_GenericWiderTypeCast */
        DoIP_GetOemPayloadRxBufStartIdxOfOemPayloadRxBufConfig(oemRxBufCfgIdx)) )
    {
      /* Set the next error level as there is at least one buffer that is big enough for the message. */
      genNackCode = DOIP_MAX(genNackCode, DOIP_GEN_NACK_OUT_OF_MEM);

      /* #1000 Check that the buffer is not yet used for any other reception. */
      if ( DoIP_IsUsedOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx) == FALSE )
      {
        /* #10000 Set the message length. */
        DoIP_SetLengthOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx, (DoIP_LengthOfOemPayloadRxBufConfigDynType)           /* SBSW_DOIP_CSL02_CSL03 */
          UserDataLength);

        /* #10001 Mark the buffer as used and store the buffer configuration. */
        DoIP_SetUsedOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx, TRUE);                                                  /* SBSW_DOIP_CSL02_CSL03 */
        DoIP_SetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx, oemRxBufCfgIdx);                        /* SBSW_DOIP_CSL02_CSL03 */

        genNackCode = DOIP_GEN_NACK_OK;

        break;
      }
    }
  }

  return genNackCode;
} /* DoIP_RxTcpOem_ReserveBuf() */

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_ReleaseBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_RxTcpOem_ReleaseBuf(
  DoIP_SizeOfOemPayloadRxBufConfigType OemRxBufCfgIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Release the buffer. */
  DoIP_SetLengthOfOemPayloadRxBufConfigDyn(OemRxBufCfgIdx, 0u);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  DoIP_SetUsedOfOemPayloadRxBufConfigDyn(OemRxBufCfgIdx, FALSE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
} /* DoIP_RxTcpOem_ReleaseBuf() */

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_CopyPayload()
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
 *
 */
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_RxTcpOem_CopyPayload(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) BufferSizePtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) ReadDataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfConnectionType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);
  const DoIP_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIP_GetOemSpecificCfgIdxOfConnection(connectionIdx);
  const DoIP_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx =
    DoIP_GetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx);
  const DoIP_SizeOfOemPayloadRxBufType oemRxBufStartIdx =
    DoIP_GetOemPayloadRxBufStartIdxOfOemPayloadRxBufConfig(oemRxBufCfgIdx);
  DoIP_RxMsgOffsetOfConnectionTcpDynType rxMsgOffset = DoIP_GetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx);
  uint32 hdrLenField;
  uint64 msgLength;
  uint32 bytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT32(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 4u, hdrLenField); /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  msgLength = (uint64)hdrLenField + DOIP_GEN_HDR_LEN_BYTE;

  /* #10 Check and set max bytes to copy. */
  bytesToCopy = (uint32)(msgLength - rxMsgOffset);                                                                     /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */

  if ( bytesToCopy > ((uint32)(PduInfoPtr->SduLength) - (uint32)(*ReadDataLenPtr)) )
  {
    bytesToCopy = ((uint32)(PduInfoPtr->SduLength) - (uint32)(*ReadDataLenPtr));
  }

  /* #20 Copy data to buffer if there are bytes to copy. */
  if ( bytesToCopy > 0u )
  {
    /* PRQA S 0310, 0314 2 */ /* MD_DoIP_11.4, MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIP_A_P2VAR(void)DoIP_GetAddrOemPayloadRxBuf(oemRxBufStartIdx + rxMsgOffset -                      /* SBSW_DOIP_PARAM_VALID_OEM_PAYLOAD_RX_BUF_CFG_INDEX_AS_POINTER_AND_CONST_POINTER_FORWARD */
        DOIP_GEN_HDR_LEN_BYTE), DOIP_A_P2CONST(void)&PduInfoPtr->SduDataPtr[*ReadDataLenPtr], bytesToCopy);

    rxMsgOffset += bytesToCopy;
    *ReadDataLenPtr += (PduLengthType)bytesToCopy;                                                                     /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, rxMsgOffset);                                              /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }

  /* #30 Check if all data has already been copied. */
  if ( rxMsgOffset == msgLength )
  {
    Std_ReturnType retVal;
    PduInfoType oemPayloadRxBuf;
    uint16 hdrTypeField;
    uint8 flags = DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_TCP;

    oemPayloadRxBuf.SduDataPtr = DoIP_GetAddrOemPayloadRxBuf(oemRxBufStartIdx);
    oemPayloadRxBuf.SduLength = (PduLengthType)hdrLenField;

    IPBASE_GET_UINT16(DoIP_GetAddrRxHdrBuf(DoIP_GetRxHdrBufStartIdxOfConnectionTcp(ConnectionTcpIdx)), 2u,             /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
      hdrTypeField);

    /* #40 Set flags for the callback. */
    if ( DoIP_GetActivStateOfConnectionTcpDyn(ConnectionTcpIdx) == DOIP_ROUT_ACTIV_STATE_ACTIVATED )
    {
      flags |= DOIP_OEM_PAYLOAD_TYPE_FLAG_ROUT_ACTIV;
    }

    /* #50 Forward the data to the callback (if configured). */
    retVal = DoIP_RxTcpOem_ForwardToUser(connectionIdx, hdrTypeField, flags, &oemPayloadRxBuf);                        /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */

    /* #60 Send a generic Nack in case no Rx callback is configured or in case it returned with E_NOT_OK. */
    if ( retVal != E_OK )
    {
      (void)DoIP_TxTcp_Transmit(ConnectionTcpIdx, DOIP_HDR_TYPE_GEN_NACK, DOIP_GEN_NACK_TYPE, 0u, 0u, NULL_PTR, FALSE);/* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
    }

    /* #70 Release the buffer and reset the reserved buffer index. */
    DoIP_RxTcpOem_ReleaseBuf(oemRxBufCfgIdx);
    DoIP_SetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx, DoIP_GetSizeOfOemPayloadRxBufConfig());     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL03 */

    /* #80 Reset the offset of the received message. */
    DoIP_SetRxMsgOffsetOfConnectionTcpDyn(ConnectionTcpIdx, 0u);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
  /* #31 Set buffer size to length of pending message part otherwise. */
  else
  {
    uint32 bufSize = (uint32)(msgLength - rxMsgOffset);                                                                /* INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED */
    DoIP_Util_CheckBufferSizeAndModify(bufSize, BufferSizePtr);                                                        /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    DoIP_SetLastRxBufSizeOfConnectionTcpDyn(ConnectionTcpIdx, *BufferSizePtr);                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
  }
} /* DoIP_RxTcpOem_CopyPayload() */

/**********************************************************************************************************************
 *  DoIP_RxTcpOem_ForwardToUser()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_RxTcpOem_ForwardToUser(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 HdrTypeField,
  uint8 Flags,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) OemPayloadRxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIP_SizeOfInterfaceType interfaceIdx = DoIP_GetInterfaceIdxOfConnection(ConnectionIdx);
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there is a callback configured. */
  if ( DoIP_IsOemPayloadRxCbkConfigUsedOfInterface(interfaceIdx) )
  {
    /* #20 Forward received data to the user via the configured callback. */
    retVal = DoIP_GetOemPayloadRxCbkOfOemPayloadRxCbkConfig(                                                           /* SBSW_DOIP_FUNCTION_POINTER_CALL */
        DoIP_GetOemPayloadRxCbkConfigIdxOfInterface(interfaceIdx))(DoIP_GetConnectionIdOfConnection(ConnectionIdx),
          HdrTypeField, OemPayloadRxBufPtr, Flags);
  }

  return retVal;
} /* DoIP_RxTcpOem_ForwardToUser() */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_HandleTpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleTpTransmit(
  DoIP_SizeOfPduRTxPduIdType DoIPPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) DoIPPduRTxInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelIdxOfPduRTxPduId(DoIPPduRTxId);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TP session is active or requested for this channel. */
  if ( DoIP_GetTxMsgLenOfChannelDyn(channelIdx) == 0u )
  {
    const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfTesterDyn(
      DoIP_GetTesterIdxOfChannel(channelIdx));

    /* #20 Check if tester is connected to channel. */
    if ( connectionTcpIdx != DoIP_GetSizeOfConnectionTcp() )
    {
      DoIP_SetTxMsgLenOfChannelDyn(channelIdx, DoIPPduRTxInfoPtr->SduLength);                                          /* SBSW_DOIP_CSL03 */

      /* #30 Transmit request. */
      /* Param2 of the transmit can be casted to uint32 because PduLengthType date type is less or equal to uint32. */
      retVal = DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_DIAG_REQ, channelIdx,                               /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        (uint32)DoIPPduRTxInfoPtr->SduLength, 0u, NULL_PTR, FALSE);

      if ( retVal != E_OK )
      {
        /* #40 Reset channel information on error. */
        DoIP_SetTxMsgLenOfChannelDyn(channelIdx, 0u);                                                                  /* SBSW_DOIP_CSL03 */
      }
    }
  }

  return retVal;
} /* DoIP_HandleTpTransmit() */

#if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleTpCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleTpCancelTransmit(
  DoIP_SizeOfPduRTxPduIdType DoIPPduRTxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelIdxOfPduRTxPduId(DoIPPduRTxId);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfTesterDyn(
    DoIP_GetTesterIdxOfChannel(channelIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check connection index. */
  if ( connectionTcpIdx != DoIP_GetSizeOfConnectionTcp() )
  {
    /* #20 Check if session is active. */
    if ( DoIP_IsTxTpSessionActiveOfConnectionTcpDyn(connectionTcpIdx) )
    {
      /* #30 Check if transmission is pending. */
      if ( DoIP_GetTxMsgLenOfChannelDyn(channelIdx) != 0u )
      {
        const DoIP_ConnectionIdxOfConnectionTcpType connectionIdx =
          DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx);

        /* #40 Cancel request if connection is active and if transmission is pending. */
        if ( SoAd_TpCancelTransmit(DoIP_GetSoAdTxPduIdOfConnection(connectionIdx)) == E_OK )
        {
          DoIP_SetTxTpCancelRequestedOfConnectionTcpDyn(connectionTcpIdx, TRUE);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
          retVal = E_OK;
        }
      }
    }
  }

  return retVal;
} /* DoIP_HandleTpCancelTransmit() */
#endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */

#if ( DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleTpCancelReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleTpCancelReceive(
  DoIP_SizeOfPduRRxCancelPduIdType DoIPPduRRxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelIdxOfPduRRxCancelPduId(DoIPPduRRxId);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfTesterDyn(
    DoIP_GetTesterIdxOfChannel(channelIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check connection index. */
  if ( connectionTcpIdx != DoIP_GetSizeOfConnectionTcp() )
  {
    /* #20 Check if session is active. */
    if ( DoIP_IsChannelRxActiveOfConnectionTcpDyn(connectionTcpIdx) )
    {
      /* #30 Cancel request if connection is active. */
      if ( SoAd_TpCancelReceive(DoIP_GetRxCancelPduIdOfConnectionTcp(connectionTcpIdx)) == E_OK )
      {
        DoIP_SetRxTpCancelRequestedOfConnectionTcpDyn(connectionTcpIdx, TRUE);                                         /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* DoIP_HandleTpCancelReceive() */
#endif /* DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON */

#if ( DOIP_IF_API_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleIfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleIfTransmit(
  DoIP_SizeOfPduRTxPduIdType PduRTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelIdxOfPduRTxPduId(PduRTxPduId);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission is pending. */
  if ( DoIP_GetTxMsgLenOfChannelDyn(channelIdx) == 0u )
  {
    const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfTesterDyn(
      DoIP_GetTesterIdxOfChannel(channelIdx));

    /* #20 Check connection index of connected tester. */
    if ( connectionTcpIdx != DoIP_GetSizeOfConnectionTcp() )
    {
      DoIP_SetTxMsgLenOfChannelDyn(channelIdx, PduInfoPtr->SduLength);                                                 /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */

      /* #30 Trigger transmit. */
      retVal = DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_DIAG_REQ, channelIdx, (uint16)PduInfoPtr->SduLength, /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        0u, PduInfoPtr->SduDataPtr, FALSE);

      if ( retVal != E_OK )
      {
        /* #40 Reset channel information on error. */
        DoIP_SetTxMsgLenOfChannelDyn(channelIdx, 0u);                                                                  /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03 */
      }
    }
  }

  return retVal;
} /* DoIP_HandleIfTransmit() */
#endif /* DOIP_IF_API_ENABLED == STD_ON */

#if ( DOIP_IF_API_ENABLED == STD_ON )
# if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleIfCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_HandleIfCancelTransmit(
  DoIP_SizeOfPduRTxPduIdType PduRTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelIdxOfPduRTxPduId(PduRTxPduId);
  const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTcpIdxOfTesterDyn(
    DoIP_GetTesterIdxOfChannel(channelIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check connection index of connected tester. */
  if ( connectionTcpIdx < DoIP_GetSizeOfConnectionTcp() )
  {
    /* #20 Check session state. */
    if ( DoIP_IsTxTpSessionActiveOfConnectionTcpDyn(connectionTcpIdx) )
    {
      /* #30 Check if transmission is pending. */
      if ( DoIP_GetTxMsgLenOfChannelDyn(channelIdx) != 0u )
      {
        const DoIP_ConnectionIdxOfConnectionTcpType connectionIdx =
          DoIP_GetConnectionIdxOfConnectionTcp(connectionTcpIdx);
        /* #40 Cancel IF transmit. */
        if ( SoAd_TpCancelTransmit(DoIP_GetSoAdTxPduIdOfConnection(connectionIdx)) == E_OK )
        {
          DoIP_SetTxTpCancelRequestedOfConnectionTcpDyn(connectionTcpIdx, TRUE);                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
          retVal = E_OK;
        }
      }
    }
  }

  return retVal;
} /* DoIP_HandleIfCancelTransmit() */
# endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */
#endif /* DOIP_IF_API_ENABLED == STD_ON */

#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  DoIP_TxOem_Transmit()
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
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxOem_Transmit(
  DoIP_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PayloadDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfOemPayloadTxBufConfigType oemTxBufCfgIdx = DoIP_GetSizeOfOemPayloadTxBufConfig();
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to add the transmission request to a transmission buffer. */
  if ( DoIP_TxOem_ReserveBuf(ConnectionIdx, &oemTxBufCfgIdx, PayloadDataPtr) == E_OK )                                 /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    /* #20 Prepare transmission in case of a TCP connection. */
    if ( DoIP_Util_IsTcpConnection(ConnectionIdx) == TRUE )
    {
      const DoIP_SizeOfConnectionTcpType connectionTcpIdx = DoIP_GetConnectionTypeIdxOfConnection(ConnectionIdx);

      retVal = DoIP_TxTcp_Transmit(connectionTcpIdx, DOIP_HDR_TYPE_OEM_SPECIFIC, PayloadType, oemTxBufCfgIdx, 0u,      /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
        NULL_PTR, FALSE);
    }
    /* #21 Prepare transmission in case of a UDP or UDP vehicle announcement connection otherwise. */
    else
    {
      /* #210 Check if socket connection is online and shall stay active. */
      if ( (DoIP_GetSoConStateOfConnectionDyn(ConnectionIdx) == SOAD_SOCON_ONLINE) &&
        (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(DoIP_GetInterfaceIdxOfConnection(ConnectionIdx)) == 0u) )
      {
        /* #2100 Try to transmit the PDU immediately. */
        retVal = DoIP_TxUdp_TransmitImmediately(ConnectionIdx, DOIP_HDR_TYPE_OEM_SPECIFIC, oemTxBufCfgIdx, PayloadType,
          FALSE);

        /* #2101 Queue element if message could not be sent immediately. */
        if ( retVal != E_OK )
        {
          retVal = DoIP_TxUdp_TransmitDelayed(ConnectionIdx, DOIP_HDR_TYPE_OEM_SPECIFIC, oemTxBufCfgIdx, PayloadType,  /* SBSW_DOIP_PARAM_NULL_POINTER_FORWARD */
            FALSE, NULL_PTR);
        }
      }
    }

    /* #30 Release the buffer if transmission request was not successful. */
    if ( retVal != E_OK )
    {
      DoIP_TxOem_ReleaseBuf(oemTxBufCfgIdx);
    }
  }

  return retVal;
} /* DoIP_TxOem_Transmit() */

/**********************************************************************************************************************
 *  DoIP_TxOem_ReserveBuf()
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
 *
 *
 *
 *
 */
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_TxOem_ReserveBuf(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2VAR(DoIP_SizeOfOemPayloadTxBufConfigType, AUTOMATIC, DOIP_APPL_VAR) OemTxBufCfgIdxPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PayloadDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent buffer modifications from different context. */
  DOIP_BEGIN_CRITICAL_SECTION();

  /* #20 Check if an OEM specific configuration is used for the connection. */
  if ( DoIP_IsOemSpecificCfgUsedOfConnection(ConnectionIdx) )
  {
    const DoIP_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIP_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);
    PduLengthType sduLength = PayloadDataPtr->SduLength;
    PduLengthType headerLength = 0u; /* No header needs to be stored in the buffer in case of TCP. */
    DoIP_OemPayloadTxBufConfigIterType oemTxBufCfgIter;

    /* #30 Set the header length to the generic header length in case of a UDP (vehicle announcement) connection. */
    if ( DoIP_Util_IsTcpConnection(ConnectionIdx) == FALSE )
    {
      headerLength = DOIP_GEN_HDR_LEN_BYTE;
    }

    /* #40 Search for an OEM Payload Tx buffer which is available for transmission. */
    for ( oemTxBufCfgIter = DoIP_GetOemPayloadTxBufConfigIndStartIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemTxBufCfgIter < DoIP_GetOemPayloadTxBufConfigIndEndIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemTxBufCfgIter++ )
    {
      const DoIP_SizeOfOemPayloadTxBufConfigType oemTxBufCfgIdx = DoIP_GetOemPayloadTxBufConfigInd(oemTxBufCfgIter);

      /* #50 Ensure that the required length does not exceed the buffer length. */
      if ( (sduLength + headerLength) <=
        ((PduLengthType)DoIP_GetOemPayloadTxBufEndIdxOfOemPayloadTxBufConfig(oemTxBufCfgIdx) -
        (PduLengthType)DoIP_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(oemTxBufCfgIdx)) )
      {
        /* #60 Ensure that the buffer is not yet used for any other transmission. */
        if ( DoIP_IsUsedOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx) == FALSE )
        {
          /* #70 Set the message length. */
          DoIP_SetLengthOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx,                                                     /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL03 */
            (DoIP_LengthOfOemPayloadTxBufConfigDynType)(sduLength + headerLength));

          /* #80 Mark the buffer as used to signalize a pending transmission. */
          DoIP_SetUsedOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx, TRUE);                                                /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL03 */

          /* #90 Leave critical section. */
          DOIP_END_CRITICAL_SECTION();

          /* #100 Copy data to OEM Payload Tx buffer. */
          /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
          VStdLib_MemCpy(DOIP_A_P2VAR(void)DoIP_GetAddrOemPayloadTxBuf(                                                /* SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD */
            DoIP_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(oemTxBufCfgIdx) + headerLength),
            DOIP_A_P2CONST(void)PayloadDataPtr->SduDataPtr, PayloadDataPtr->SduLength);

          /* #110 Return the reserved OEM Tx buffer configuration index. */
          *OemTxBufCfgIdxPtr = oemTxBufCfgIdx;                                                                         /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */

          retVal = E_OK;
          break;
        }
      }
    }
  }

  /* #120 Leave critical section in case no buffer was reserved. */
  if ( retVal != E_OK )
  {
    DOIP_END_CRITICAL_SECTION();
  }

  return retVal;
} /* DoIP_TxOem_ReserveBuf() */

/**********************************************************************************************************************
 *  DoIP_TxOem_ReleaseBuf()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_TxOem_ReleaseBuf(
  DoIP_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset message length. */
  DoIP_SetLengthOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx, 0u);                                                        /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */

  /* #20 Mark the buffer as not used to signalize that no transmission is pending. */
  DoIP_SetUsedOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx, FALSE);                                                       /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
} /* DoIP_TxOem_ReleaseBuf() */
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  DoIP_HandleSoAdTpCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleSoAdTpCopyTxData(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

/* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
  /* Check initialization state (if enabled). */
  /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
  if ( DoIP_State != DOIP_STATE_INIT )
  { /* DoIP_State is not INIT (it could be SHUTDOWN) */
    /* #10 Continue if DoIP_State is shutdown (if enabled). */
    /* nothing to do here */
  }
  else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
  /* check socket connection state */
  if ( DoIP_GetSoConStateOfConnectionDyn(DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx)) != SOAD_SOCON_ONLINE )
  { /* socket connection is not online */
    /* #20 Continue if socket connection is offline. */
    /* nothing to do here */
  }
  else
  {
    /* #30 Otherwise copy data if socket connection is online. */
    retVal = DoIP_TxTcpQueue_Copy(ConnectionTcpIdx, PduInfoPtr, AvailableDataPtr);                                     /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  return retVal;
} /* DoIP_HandleSoAdTpCopyTxData() */

/**********************************************************************************************************************
 *  DoIP_HandleSoAdTpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleSoAdTpTxConfirmation(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx,
  Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
  /* Check initialization state (if enabled). */
  /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
  if ( DoIP_State != DOIP_STATE_INIT )
  { /* DoIP_State is not INIT (it could be SHUTDOWN) */
    /* #10 Continue if DoIP_State is shutdown (if enabled). */
    /* nothing to do here */
  }
  else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
  {
    /* #20 Otherwise remove element from queue. */
    DoIP_TxTcpQueue_RemoveElement(ConnectionTcpIdx, Result);

#if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
    /* #30 Reset cancellation (if enabed). */
    if ( DoIP_IsTxTpCancelRequestedOfConnectionTcpDyn(ConnectionTcpIdx) )
    {
      DoIP_SetTxTpCancelRequestedOfConnectionTcpDyn(ConnectionTcpIdx, FALSE);                                          /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    }
#endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */

    if ( DoIP_GetTxTcpMgtQueLvlOfConnectionTcpDyn(ConnectionTcpIdx) > 0u )
    {
      /* #40 Transmit pending element (if enabled). */
#if ( DOIP_TP_OPTIMIZED_ENABLED == STD_OFF )
      DoIP_TxTcpQueue_TransmitElement(ConnectionTcpIdx);
#endif /* DOIP_TP_OPTIMIZED_ENABLED == STD_OFF */
    }
    else if ( DoIP_IsSoConClosingOfConnectionDyn(connectionIdx) )
    {
      /* #50 Otherwise close socket if requested. */
#if ( DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON )
      DoIP_Con_SoConClose(connectionIdx, FALSE);
#else
      DoIP_Con_SoConClose(connectionIdx, TRUE);
#endif /* DOIP_SUPPORT_GRACEFUL_CLOSE == STD_ON */
    }
    else
    {
      /* nothing to do here */
    }
  }
} /* DoIP_HandleSoAdTpTxConfirmation() */

/**********************************************************************************************************************
 *  DoIP_HandleSoAdTpStartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleSoAdTpStartOfReception(
  DoIP_SizeOfConnectionTcpType ConnectionTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  DoIP_ConnectionIdxOfConnectionTcpType connectionIdx = DoIP_GetConnectionIdxOfConnectionTcp(ConnectionTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* Check plausibility of parameters and states. */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
  /* Check initialization state */
  /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
  if ( DoIP_State != DOIP_STATE_INIT )
  { /* DoIP_State is not INIT (it could be SHUTDOWN) */
    /* #10 Continue if DoIP_State is shutdown (if enabled). */
    /* nothing to do here */
  }
  else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
  /* check socket connection state */
  if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) != SOAD_SOCON_ONLINE )
  { /* socket connection is not online */
    /* #20 Continue if socket connection is offline. */
    /* nothing to do here */
  }
  else if ( DoIP_IsRxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx) )
  { /* TP Routing is already active */
    /* #30 Continue if TP routing is already active. */
    /* nothing to do here */
  }
  else
  {
    /* #40 Otherwise activate TP session if TP routing is not active. */
    DoIP_SetRxTpSessionActiveOfConnectionTcpDyn(ConnectionTcpIdx, TRUE);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
    retVal = BUFREQ_OK;
  }

  return retVal;
} /* DoIP_HandleSoAdTpStartOfReception() */

/**********************************************************************************************************************
 *  DoIP_HandleSoAdIfRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleSoAdIfRxIndication(
  DoIP_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Check plausibility of all input parameters and states (if enabled). */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
  /* Check initialization state */
  /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
  if ( DoIP_State != DOIP_STATE_INIT )
  { /* DoIP_State is not INIT (it could be SHUTDOWN) */
    /* #10 Continue if DoIP_State is shutdown (if enabled). */
    /* nothing to do here */
  }
  else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
  if ( DoIP_GetIpAddrStateOfConnectionDyn(ConnectionIdx) != SOAD_IPADDR_STATE_ASSIGNED )
  { /* no IP is assigned */
    /* #20 Continue if ip address is not assigned. */
    /* nothing to do here */
  }
  else if ( (DoIP_GetSoConStateOfConnectionDyn(ConnectionIdx) != SOAD_SOCON_ONLINE) ||
            DoIP_IsSoConClosingOfConnectionDyn(ConnectionIdx) )
  { /* socket connection is not online or requested to be closed */
    /* #30 Continue if socket connection is offline or closing. */
    /* nothing to do here */
  }
  else
  {
    /* #40 Otherwise handle request. */
    DoIP_RxUdp_HandleRxIndication(ConnectionIdx, PduInfoPtr);                                                          /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  }
} /* DoIP_HandleSoAdIfRxIndication() */

#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
/**********************************************************************************************************************
 *  DoIP_HandleDhcpEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleDhcpEvent(
  SoAd_LocalAddrIdType LocalIpAddrId,
  SoAd_DhcpEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIP_SizeOfLocalAddrType localAddrIdx = DoIP_GetSizeOfLocalAddr();
  uint16 optionCode;
  uint16 optionBufferLength = DOIP_DHCPVX_OPT_VENDOR_BUF_MAX_LEN;
  uint8  optionBuffer[DOIP_DHCPVX_OPT_VENDOR_BUF_MAX_LEN] = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if LocalIpAddrId is used for DoIP assignment. */
  if ( DoIP_Dhcp_GetLocalAddrIdxOfDhcpAssignmentByLocalIpAddrId(LocalIpAddrId, &localAddrIdx) == E_OK )                /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
    SoAd_DomainType domain = DoIP_GetDomainTypeOfLocalAddr(localAddrIdx);
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
# endif /* DOIP_IPV6_ENABLED == STD_ON */

    switch(Event)
    {
      case SOAD_DHCP_EVENT_TX_DISCOVER_SOLICIT: /* fall through */
      case SOAD_DHCP_EVENT_TX_REQUEST:          /* fall through */
# if ( DOIP_IPV6_ENABLED == STD_ON )
      case SOAD_DHCP_EVENT_TX_V6_RENEW:         /* fall through */
      case SOAD_DHCP_EVENT_TX_V6_REBIND:
# endif /* DOIP_IPV6_ENABLED == STD_ON */
      {
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
        if ( domain == SOAD_AF_INET6 )
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
        {
          optionCode = DOIP_DHCPV6_OPT_VENDOR_CLASS_OPT_CODE;
        }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
        else
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
# endif /* DOIP_IPV6_ENABLED == STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
        {
          optionCode = DOIP_DHCPV4_OPT_VENDOR_CLASS_OPT_CODE;
        }
# endif /* DOIP_IPV4_ENABLED == STD_ON */

        /* #20 Write the Vendor Class data in case of tx event type. */
        DoIP_Dhcp_PrepareVendorClassTxBuf(optionCode, &optionBufferLength, optionBuffer);                              /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

        /* #30 Call the SoAd service function to write Vendor Option in case of tx event type. */
        (void)SoAd_WriteDhcpOption(LocalIpAddrId, optionCode, optionBufferLength, optionBuffer);                       /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */

        break;
      }
      case SOAD_DHCP_EVENT_RX_OFFER_ADVERTISE:
      {
        /* ignore SOAD_DHCP_EVENT_RX_OFFER_ADVERTISE and wait for SOAD_DHCP_EVENT_RX_ACK_REPLY */
        break;
      }
      case SOAD_DHCP_EVENT_RX_ACK_REPLY:
      {
# if ( DOIP_IPV6_ENABLED == STD_ON )
#  if ( DOIP_IPV4_ENABLED == STD_ON )
        if ( domain == SOAD_AF_INET6 )
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
        {
          optionCode = DOIP_DHCPV6_OPT_VENDOR_OPTS_OPT_CODE;
        }
#  if ( DOIP_IPV4_ENABLED == STD_ON )
        else
#  endif /* DOIP_IPV4_ENABLED == STD_ON */
# endif /* DOIP_IPV6_ENABLED == STD_ON */
# if ( DOIP_IPV4_ENABLED == STD_ON )
        {
          optionCode = DOIP_DHCPV4_OPT_VENDOR_OPTS_OPT_CODE;
        }
# endif /* DOIP_IPV4_ENABLED == STD_ON */

        /* #40 Read the supplied Vendor Opts data from the DHCP message if rx event type is ACK/REPLY. */
        if ( SoAd_ReadDhcpOption(LocalIpAddrId, optionCode, &optionBufferLength, optionBuffer) == E_OK )               /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        {
          /* #50 Handle Vender Opts. */
          DoIP_Dhcp_HandleVendorOptsOption(localAddrIdx, optionCode, optionBufferLength, optionBuffer);                /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
        }

        break;
      }
      default:
      {
        /* ignore event */
        /* nothing to do here */
        break;
      }
    }
  }
} /* DoIP_HandleDhcpEvent() */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

/**********************************************************************************************************************
 *  DoIP_HandleActivationLineSwitch()
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
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleActivationLineSwitch(
  DoIP_SizeOfInterfaceType InterfaceIdx,
  boolean Active)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if activation line state is active. */
  if ( DoIP_IsActLineActiveOfInterfaceDyn(InterfaceIdx) )
  {
    /* #20 Check if activation line is requested to be active. */
    if ( Active == TRUE )
    {
      /* #30 Save active request in state active only if an inactive request was received before. */
      if ( (DoIP_GetActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx) + 1u) ==
        (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(InterfaceIdx)) )
      {
        DoIP_IncActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }
    /* #40 Otherwise activation line is requested to be inactive. */
    else
    {
      /* #50 Save inactive request in state active only if a 'new' inactive request was received. */
      if ( (DoIP_GetActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx)) ==
        (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(InterfaceIdx)) )
      {
        DoIP_IncActLineInactiveRequestCounterOfInterfaceDyn(InterfaceIdx);                                             /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }
  }
  /* #60 Otherwise activation line state is inactive. */
  else
  {
    /* #70 Check if activation line is requested to be active. */
    if ( Active == TRUE )
    {
      /* #80 Save active request in state inactive only if a 'new' active request was received. */
      if ( (DoIP_GetActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx)) ==
        (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(InterfaceIdx)) )
      {
        DoIP_IncActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx);                                               /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }
    /* #90 Otherwise activation line is requested to be inactive. */
    else
    {
      /* #100 Save inactive request in state inactive only if an active request was received before. */
      if ( (DoIP_GetActLineActiveRequestCounterOfInterfaceDyn(InterfaceIdx)) ==
        (DoIP_GetActLineInactiveRequestCounterOfInterfaceDyn(InterfaceIdx) + 1u) )
      {
        DoIP_IncActLineInactiveRequestCounterOfInterfaceDyn(InterfaceIdx);                                             /* SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02 */
      }
    }
  }
} /* DoIP_HandleActivationLineSwitch() */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_InitDroppedPacketCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_InitDroppedPacketCounter(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the dropped packet counters to zero. */
  DoIP_Measure_DroppedTcpPacketsCounter = 0u;
  DoIP_Measure_DroppedUdpPacketsCounter = 0u;
} /* DoIP_Measure_InitDroppedPacketCounter() */
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounterTcp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounterTcp(void)
{
  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();
  /* #10 Check if the dropped TCP packet counter has not overflowed. */
  if ( DoIP_Measure_DroppedTcpPacketsCounter < DOIP_MEASURE_DROPPED_PACKET_COUNTER_MAX_VALUE )
  {
    /* #20 Increment the counter. */
    DoIP_Measure_DroppedTcpPacketsCounter++;
  }
  DOIP_END_CRITICAL_SECTION();
} /* DoIP_Measure_IncDroppedPacketCounterTcp() */
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounterUdp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounterUdp(void)
{
  /* ----- Implementation ----------------------------------------------- */
  DOIP_BEGIN_CRITICAL_SECTION();
  /* #10 Check if the dropped UDP packet counter has not overflowed. */
  if ( DoIP_Measure_DroppedUdpPacketsCounter < DOIP_MEASURE_DROPPED_PACKET_COUNTER_MAX_VALUE )
  {
    /* #20 Increment the counter. */
    DoIP_Measure_DroppedUdpPacketsCounter++;
  }
  DOIP_END_CRITICAL_SECTION();
} /* DoIP_Measure_IncDroppedPacketCounterUdp() */
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounterType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounterType(
  DoIP_Measure_DroppedPacketCounterType DroppedPacketCounterType)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the counter type to be incremented is TCP. */
  if ( DroppedPacketCounterType == DOIP_MEASURE_DROPPED_PACKET_COUNTER_TCP )
  {
    /* #20 Increment the dropped TCP packet counter. */
    DoIP_Measure_IncDroppedPacketCounterTcp();
  }
  /* #30 The counter type to be incremented is UDP. */
  else
  {
    /* #40 Increment the dropped UDP packet counter. */
    DoIP_Measure_IncDroppedPacketCounterUdp();
  }
} /* DoIP_Measure_IncDroppedPacketCounterType() */
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_IncDroppedPacketCounter()
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
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_Measure_IncDroppedPacketCounter(
  DoIP_Measure_DroppedPacketCounterType DroppedPacketCounterType,
  uint16 HdrType,
  uint8 NackType)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the header type is generic Nack. */
  if ( HdrType == DOIP_HDR_TYPE_GEN_NACK )
  {
    switch(NackType)
    {
      /* #20 Check if the GEN NACK type is to be considered for incrementing the packet drop counter. */
      case DOIP_GEN_NACK_FORMAT:
      case DOIP_GEN_NACK_MAX_LEN:
      case DOIP_GEN_NACK_INV_LEN:
      case DOIP_GEN_NACK_LEN_BYTE:
      {
        /* #30 Increment the dropped packet counter. */
        DoIP_Measure_IncDroppedPacketCounterType(DroppedPacketCounterType);
        break;
      }
      default:
      {
        /* Nothing to do. */
        break;
      }
    }
  }
  /* #40 Check if the header type is diagnostic NACK. */
  else if ( HdrType == DOIP_HDR_TYPE_DIAG_NACK )
  {
    switch(NackType)
    {
      /* #50 Check if the diagnostic NACK type is to be considered for incrementing the packet drop counter. */
      case DOIP_DIAG_MSG_RES_CODE_INVALID_SRC_ADDR:
      case DOIP_DIAG_MSG_RES_CODE_UNKNOWN_TGT_ADDR:
      case DOIP_DIAG_MSG_RES_CODE_MSG_TOO_LARGE:
      {
        /* #60 Increment the dropped packet counter. */
        DoIP_Measure_IncDroppedPacketCounterType(DroppedPacketCounterType);
        break;
      }
      default:
      {
        /* Nothing to do. */
        break;
      }
    }
  }
  else
  {
    /* Nothing to do. */
  }
} /* DoIP_Measure_IncDroppedPacketCounter() */
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_Measure_GetAndResetDroppedPacketCounter()
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
 *
 */
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE) DoIP_Measure_GetAndResetDroppedPacketCounter(
  DoIP_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(DoIP_MeasurementDataType, AUTOMATIC, DOIP_APPL_VAR) MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch(MeasurementIdx)
  {
    /* #10 Check if the measurement index is that of the TCP dropped packets counter. */
    case DOIP_MEAS_DROP_TCP:
    {
      DOIP_BEGIN_CRITICAL_SECTION();
      if ( MeasurementDataPtr != NULL_PTR )
      {
        /* #20 Copy the TCP dropped packets counter value to a buffer, if measurement data pointer is not NULL_PTR. */
        *MeasurementDataPtr = DoIP_Measure_DroppedTcpPacketsCounter;                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      }

      if ( MeasurementResetNeeded == TRUE )
      {
        /* #30 Reset the TCP dropped packets counter to zero if the reset flag is set to true. */
        DoIP_Measure_DroppedTcpPacketsCounter = 0u;
      }
      DOIP_END_CRITICAL_SECTION();

      retVal = E_OK;
      break;
    }

    /* #40 Check if the measurement index is that of the UDP dropped packets counter. */
    case DOIP_MEAS_DROP_UDP:
    {
      DOIP_BEGIN_CRITICAL_SECTION();
      if ( MeasurementDataPtr != NULL_PTR )
      {
        /* #50 Copy the UDP dropped packets counter value to a buffer, if measurement data pointer is not NULL_PTR. */
        *MeasurementDataPtr = DoIP_Measure_DroppedUdpPacketsCounter;                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
      }

      if ( MeasurementResetNeeded == TRUE )
      {
        /* #60 Reset the UDP dropped packets counter to zero if the reset flag is set to true. */
        DoIP_Measure_DroppedUdpPacketsCounter = 0u;
      }
      DOIP_END_CRITICAL_SECTION();

      retVal = E_OK;
      break;
    }

    /* #70 Check if the measurement index is that of ALL. */
    case DOIP_MEAS_ALL:
    {
      if ( MeasurementResetNeeded == TRUE )
      {
        /* #80 Reset the TCP and UDP dropped packets counter to zero if the reset flag is set to true. */
        DOIP_BEGIN_CRITICAL_SECTION();
        DoIP_Measure_DroppedTcpPacketsCounter = 0u;
        DoIP_Measure_DroppedUdpPacketsCounter = 0u;
        DOIP_END_CRITICAL_SECTION();
      }

      retVal = E_OK;
      break;
    }

    default:
    {
      /* #90 E_NOT_OK returned as the requested measurement index is not supported. */
      break;
    }
  }

  return retVal;
} /* DoIP_Measure_GetAndResetDroppedPacketCounter() */
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  DoIP_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( (DOIP_SUPPORT_SHUTDOWN == STD_ON) || (DOIP_DEV_ERROR_DETECT == STD_ON) ) /* COV_DOIP_SUPPORTED */
  /* #10 Initialize state variable (if enabled). */
  DoIP_State = DOIP_STATE_UNINIT;
#endif /* (DOIP_SUPPORT_SHUTDOWN == STD_ON) || (DOIP_DEV_ERROR_DETECT == STD_ON) */

  /* #20 Reset configuration pointer. */
  DoIP_ConfigDataPtr = NULL_PTR;
} /* DoIP_InitMemory() */

/**********************************************************************************************************************
 *  DoIP_Init()
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
FUNC(void, DOIP_CODE) DoIP_Init(
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_CONST) DoIPConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters (if enabled). */
  /* Check parameter 'DoIPConfigPtr' */
  if ( DoIPConfigPtr == NULL_PTR )
  {
# if ( DOIP_CONFIGURATION_VARIANT == DOIP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    EcuM_BswErrorHook((uint16) DOIP_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
# endif /* DOIP_CONFIGURATION_VARIANT == DOIP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    errorId = DOIP_E_INIT_FAILED;
  }
# if ( DOIP_CONFIGURATION_VARIANT == DOIP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  else if ( DoIPConfigPtr->FinalMagicNumberOfPBConfig != DOIP_FINAL_MAGIC_NUMBER )
  {
    EcuM_BswErrorHook((uint16) DOIP_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
    errorId = DOIP_E_INIT_FAILED;
  }
# endif /* DOIP_CONFIGURATION_VARIANT == DOIP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
  else if ( DoIP_Util_ValidateConfig(DoIPConfigPtr) != E_OK )                                                          /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    errorId = DOIP_E_INIT_FAILED;
  }
  else
#else
  DOIP_DUMMY_STATEMENT(DoIPConfigPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize configuration pointer. */
    DoIP_ConfigDataPtr = DoIPConfigPtr;

    /* #30 Initialize the module data structures. */
    DoIP_Util_Init();

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
    /* #40 Initialize the packet drop counters. (if enabled.) */
    DoIP_Measure_InitDroppedPacketCounter();
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

    /* #50 Initialize Pdu size routing data (if enabled). */
#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
    DoIP_IsPduSizeRoutingEnabled = FALSE;
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */

    /* #60 Initialize GID sync state (if enabled). */
#if ( DOIP_GID_SYNC_ENABLED == STD_ON )
    DoIP_GidInSync = FALSE;
#endif /* DOIP_GID_SYNC_ENABLED == STD_ON */

    /* #70 Initialize the entity address (if OverwriteLogicalTargetAddr API is enabled). */
#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON )
    DoIP_SetEntityAddr(DoIP_GetLogicalAddr(0u));                                                                                     /* SBSW_DOIP_EntityAddr */
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON */

#if ( (DOIP_SUPPORT_SHUTDOWN == STD_ON) || (DOIP_DEV_ERROR_DETECT == STD_ON) ) /* COV_DOIP_SUPPORTED */
    DoIP_State = DOIP_STATE_INIT;
#endif /* (DOIP_SUPPORT_SHUTDOWN == STD_ON) || (DOIP_DEV_ERROR_DETECT == STD_ON) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_INIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_Init() */

#if ( DOIP_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DOIP_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters (if enabled). */
  /* Check parameter 'versioninfo' */
  if ( versioninfo == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
# endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Write component data to versioninfo. */
    versioninfo->vendorID = DOIP_VENDOR_ID;                                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    versioninfo->moduleID = DOIP_MODULE_ID;                                                                            /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    versioninfo->sw_major_version = DOIP_SW_MAJOR_VERSION;                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    versioninfo->sw_minor_version = DOIP_SW_MINOR_VERSION;                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
    versioninfo->sw_patch_version = DOIP_SW_PATCH_VERSION;                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_WRITE */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_GET_VERSION_INFO, errorId);
  }
# else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_GetVersionInfo() */
#endif /* DOIP_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpTransmit(
  PduIdType DoIPPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) DoIPPduRTxInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
#else
  Std_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'DoIPPduRTxId' */
  else if ( DoIPPduRTxId >= DoIP_GetSizeOfPduRTxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'DoIPPduRTxInfoPtr' */
  else if ( DoIPPduRTxInfoPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'DoIPPduRTxInfoPtr' */
  else if ( DoIPPduRTxInfoPtr->SduLength == 0u )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
# if ( DOIP_IF_API_ENABLED == STD_ON )
  /* Check 'channel' */
  if ( DoIP_GetApiTypeOfChannel(DoIP_GetChannelIdxOfPduRTxPduId(DoIPPduRTxId)) != DOIP_API_TYPE_TP )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
# endif /* DOIP_IF_API_ENABLED == STD_ON */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Handle TP transmit. */
    retVal = DoIP_HandleTpTransmit((DoIP_SizeOfPduRTxPduIdType)DoIPPduRTxId, DoIPPduRTxInfoPtr);                       /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TpTransmit() */

/**********************************************************************************************************************
 *  DoIP_TpCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelTransmit(
  PduIdType DoIPPduRTxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
#else
  Std_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'DoIPPduRTxId' */
  else if ( DoIPPduRTxId >= DoIP_GetSizeOfPduRTxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
# if ( DOIP_IF_API_ENABLED == STD_ON )
  /* Check 'channel' */
  if ( DoIP_GetApiTypeOfChannel(DoIP_GetChannelIdxOfPduRTxPduId(DoIPPduRTxId)) != DOIP_API_TYPE_TP )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
# endif /* DOIP_IF_API_ENABLED == STD_ON */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON )
    /* #20 Handle cancelation of TP transmit (if enabled). */
    retVal = DoIP_HandleTpCancelTransmit((DoIP_SizeOfPduRTxPduIdType)DoIPPduRTxId);
#else
    DOIP_DUMMY_STATEMENT(DoIPPduRTxId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    retVal = E_NOT_OK;
#endif /* DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_CANCEL_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TpCancelTransmit() */

/**********************************************************************************************************************
 *  DoIP_TpCancelReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelReceive(
  PduIdType DoIPPduRRxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
#else
  Std_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'DoIPPduRRxId' */
  else if ( DoIPPduRRxId >= DoIP_GetSizeOfPduRRxCancelPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON )
    /* #20 Handle cancelation request (if enabled). */
    retVal = DoIP_HandleTpCancelReceive((DoIP_SizeOfPduRRxCancelPduIdType)DoIPPduRRxId);
#else
    DOIP_DUMMY_STATEMENT(DoIPPduRRxId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    retVal = E_NOT_OK;
#endif /* DOIP_PROVIDE_CANCEL_RECEIVE_API == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_CANCEL_RECEIVE, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TpCancelReceive() */

/**********************************************************************************************************************
 *  DoIP_IfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 3206 4 */ /* MD_DoIP_3206 */
/* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfTransmit(
  PduIdType id,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
#else
  Std_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DoIP_GetSizeOfPduRTxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'info' */
  else if ( info == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'info' */
  else if ( info->SduDataPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'info' */
  else if ( info->SduLength == 0u )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
# if ( DOIP_IF_API_ENABLED == STD_ON )
  /* Check 'channel' */
  if ( DoIP_GetApiTypeOfChannel(DoIP_GetChannelIdxOfPduRTxPduId(id)) != DOIP_API_TYPE_IF )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check 'channel' */
  else if ( info->SduLength > ((PduLengthType)DoIP_GetIfTxBufEndIdxOfChannel(DoIP_GetChannelIdxOfPduRTxPduId(id)) -
                               (PduLengthType)DoIP_GetIfTxBufStartIdxOfChannel(DoIP_GetChannelIdxOfPduRTxPduId(id))) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
# endif /* DOIP_IF_API_ENABLED == STD_ON */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_IF_API_ENABLED == STD_ON )
    /* #20 Handle IF transmit (if enabled). */
    retVal = DoIP_HandleIfTransmit((DoIP_SizeOfPduRTxPduIdType)id, info);                                              /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
#else
    DOIP_DUMMY_STATEMENT(id);   /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    DOIP_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    retVal = E_NOT_OK;
#endif /* DOIP_IF_API_ENABLED == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_IfTransmit() */

/**********************************************************************************************************************
 *  DoIP_IfCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_DoIP_3206 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfCancelTransmit(
  PduIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
#else
  Std_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DoIP_GetSizeOfPduRTxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
# if ( DOIP_IF_API_ENABLED == STD_ON )
  /* Check 'channel' */
  else if ( DoIP_GetApiTypeOfChannel(DoIP_GetChannelIdxOfPduRTxPduId(id)) != DOIP_API_TYPE_IF )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
# endif /* DOIP_IF_API_ENABLED == STD_ON */
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( (DOIP_IF_API_ENABLED == STD_ON) && (DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON) )
    /* #20 Handle IF cancelation request. */
    retVal = DoIP_HandleIfCancelTransmit((DoIP_SizeOfPduRTxPduIdType)id);
#else
    DOIP_DUMMY_STATEMENT(id); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    retVal = E_NOT_OK;
#endif /* (DOIP_IF_API_ENABLED == STD_ON) && (DOIP_PROVIDE_CANCEL_TRANSMIT_API == STD_ON) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_CANCEL_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_IfCancelTransmit() */

/**********************************************************************************************************************
 *  DoIP_TransmitOemSpecificPayloadType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TransmitOemSpecificPayloadType(
  DoIP_ConnectionIdType ConnectionId,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PayloadDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'ConnectionId'. */
  else if ( (ConnectionId >= DoIP_GetSizeOfConnectionIdMap()) || DoIP_IsInvalidHndOfConnectionIdMap(ConnectionId) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  /* Check parameter 'PayloadDataPtr'. */
  else if ( PayloadDataPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else if ( (PayloadDataPtr->SduDataPtr == NULL_PTR) && (PayloadDataPtr->SduLength != 0u) )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Try to transmit PDU. */
#if ( DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON )
    retVal = DoIP_TxOem_Transmit(DoIP_GetConnectionIdxOfConnectionIdMap(ConnectionId), PayloadType, PayloadDataPtr);   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
#else
    DOIP_DUMMY_STATEMENT(PayloadType); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# if ( DOIP_DEV_ERROR_DETECT == STD_OFF )
    DOIP_DUMMY_STATEMENT(ConnectionId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    DOIP_DUMMY_STATEMENT(PayloadDataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* DOIP_DEV_ERROR_DETECT == STD_OFF */
    retVal = E_NOT_OK;
#endif /* DOIP_OEM_PAYLOAD_TYPE_ENABLED == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_OEM_PAYLOAD_TRANSMIT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_TransmitOemSpecificPayloadType() */

/**********************************************************************************************************************
 *  DoIP_EnablePduSizeRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_EnablePduSizeRouting(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
  uint8 errorId = DOIP_E_NO_ERROR;
#else
  uint8 errorId = DOIP_E_PDU_SIZE_ROUTING; /* pdu size routing not supported */
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if enabled). */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
    /* #20 Activate Pdu size routing (if enabled). */
    DoIP_IsPduSizeRoutingEnabled = TRUE;
#else
  /* API disabled */
  /* nothing to do here */
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
# if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
# endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_ENABLE_PDU_SIZE_ROUTING, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_EnablePduSizeRouting() */

/**********************************************************************************************************************
 *  DoIP_DisablePduSizeRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_DisablePduSizeRouting(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
  uint8 errorId = DOIP_E_NO_ERROR;
#else
  uint8 errorId = DOIP_E_PDU_SIZE_ROUTING; /* pdu size routing not supported */
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if enabled). */
  if ( DoIP_State != DOIP_STATE_INIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
    /* #20 Disable Pdu size routing (if enabled). */
    DoIP_IsPduSizeRoutingEnabled = FALSE;
#else
  /* API disabled */
  /* nothing to do here */
#endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
# if ( DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
# endif /* DOIP_SUPPORT_PDU_SIZE_ROUTING == STD_ON */
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_DISABLE_PDU_SIZE_ROUTING, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_DisablePduSizeRouting() */

/**********************************************************************************************************************
 *  DoIP_Shutdown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_Shutdown(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call SoAd_Shutdown() (if enabled). */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
# if ( DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
  retVal = SoAd_Shutdown();
# else
  DoIP_State = DOIP_STATE_SHUTDOWN;
  retVal = SoAd_Shutdown();
# endif /* DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
#else
  /* API disabled */
  /* nothing to do here */
  retVal = E_NOT_OK;
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */

  return retVal;
} /* DoIP_Shutdown() */

#if ( DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON )
/**********************************************************************************************************************
 *  DoIP_GetAndResetMeasurementData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetAndResetMeasurementData(
  DoIP_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(DoIP_MeasurementDataType, AUTOMATIC, DOIP_APPL_VAR) MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
# if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
# else
  Std_ReturnType retVal;
# endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
# if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if DoIP is initialized. */
  /* Do not check 'MeasurementDataPtr' since it can be NULL_PTR. */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
# endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Handle Measurement Data Get-Reset request. */
    retVal = DoIP_Measure_GetAndResetDroppedPacketCounter(MeasurementIdx, MeasurementResetNeeded,
      MeasurementDataPtr);                                                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_AND_NULL_POINTER_FORWARD */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_GET_RESET_MEASURE_DATA, errorId);
  }
# else
  DOIP_DUMMY_STATEMENT(errorId);
# endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_GetAndResetMeasurementData() */
#endif /* DOIP_GET_RESET_MEASUREMENT_DATA == STD_ON */

#if ( DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_OverwriteLogicalTargetAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_OverwriteLogicalTargetAddress(
  DoIP_ChannelIdType ChannelId,
  uint16 LogicalTargetAddress)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
# if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
# else
  Std_ReturnType retVal;
# endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
# if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'ChannelId' */
  else if ( ChannelId >= DoIP_GetSizeOfChannelIdMap() )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
# endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DoIP_SizeOfChannelType channelIdx = DoIP_GetChannelIdxOfChannelIdMap(ChannelId);

    DOIP_BEGIN_CRITICAL_SECTION();

    /* #20 Overwrite the logical target address. */
    DoIP_SetEcuAddrOfChannelDyn(channelIdx, LogicalTargetAddress);                                                     /* SBSW_DOIP_CSL03 */

    /* #30 Overwrite the entity address if local address of the DoIP is overwritten. */
    if ( DoIP_GetEcuAddrOfChannel(channelIdx) == DoIP_GetLogicalAddr(0u) )
    {
      DoIP_SetEntityAddr(LogicalTargetAddress);                                                                        /* SBSW_DOIP_EntityAddr */
    }

    DOIP_END_CRITICAL_SECTION();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_OVERWRITE_LOGICAL_TGT_ADDR, errorId);
  }
# else
  DOIP_DUMMY_STATEMENT(errorId);
# endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_OverwriteLogicalTargetAddress() */
#endif /* DOIP_OVERWRITE_LOGICAL_TARGET_ADDR_API == STD_ON */

/**********************************************************************************************************************
 *  DoIP_ActivationLineSwitch()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitch(
  uint8 InterfaceId,
  boolean active)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else if ( InterfaceId >= DoIP_GetSizeOfInterface() )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else if ( !DoIP_IsActLineCtrlOfInterface(InterfaceId) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state (if enabled). */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* #20 Continue if DoIP_State is shutdown (if enabled). */
      /* nothing to do here */
    }
    else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
    {
      /* #30 Otherwise handle activation line state change request. */
      DoIP_HandleActivationLineSwitch(InterfaceId, active);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_ACTIVATION_LINE_SWITCH, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_ActivationLineSwitch() */

/**********************************************************************************************************************
 *  DoIP_CloseConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_CloseConnection(
  DoIP_ConnectionIdType ConnectionId,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'ConnectionId'. */
  else if ( (ConnectionId >= DoIP_GetSizeOfConnectionIdMap()) || DoIP_IsInvalidHndOfConnectionIdMap(ConnectionId) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Handle closing of socket connection. */
    DoIP_Con_CloseConnection(DoIP_GetConnectionIdxOfConnectionIdMap(ConnectionId), Abort);
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_CLOSE_CONNECTION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_CloseConnection() */

/**********************************************************************************************************************
 *  DoIP_TriggerVehicleAnnouncement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_TriggerVehicleAnnouncement(
  uint8 InterfaceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else if ( InterfaceId >= DoIP_GetSizeOfInterface() )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else if ( DoIP_IsActLineCtrlOfInterface(InterfaceId) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Local Variables ---------------------------------------------- */
    DoIP_ConnectionUdpVehAnIterType vehAnIter;
    DoIP_SizeOfConnectionType connectionIdx;

    /* ----- Implementation ----------------------------------------------- */
    /* #20 Iterate over all UDP vehicle announcement connections. */
    for ( vehAnIter = DoIP_GetConnectionUdpVehAnStartIdxOfInterface(InterfaceId);
          vehAnIter < DoIP_GetConnectionUdpVehAnEndIdxOfInterface(InterfaceId);
          vehAnIter++ )
    {
      connectionIdx = DoIP_GetConnectionIdxOfConnectionUdpVehAn(vehAnIter);
      /* #30 If socket connection is online, transmit vehicle announcement. */
      DOIP_BEGIN_CRITICAL_SECTION();
      if ( DoIP_GetSoConStateOfConnectionDyn(connectionIdx) == SOAD_SOCON_ONLINE )
      {
        DoIP_TxUdp_TransmitVehAn(connectionIdx, &errorId);                                                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      }
      /* #40 Set pending vehicle announcement flag otherwise. */
      else
      {
        DoIP_SetPendingVehAnOfConnectionUdpVehAnDyn(vehAnIter, TRUE);                                                  /* SBSW_DOIP_CSL03_CSL01 */
      }
      DOIP_END_CRITICAL_SECTION();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_LOC_TRIGGER_VEH_AN, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_TriggerVehicleAnnouncement() */

/**********************************************************************************************************************
 *  DoIP_MainFunction()
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
FUNC(void, DOIP_CODE) DoIP_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if enabled). */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state (if enabled). */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* #20 Continue if DoIP_State is shutdown (if enabled). */
      /* nothing to do here */
    }
    else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
    /* #30 Assume that module is initialized otherwise. */
    {
      /* #40 Initiates connections by extracting socket connection identifier from SoAd. */
      if ( DoIP_Con_SoConInitAll(&errorId) == E_OK )                                                                   /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
      {
        /* #50 Handle module depending on activation line state if socket connection identifier are extracted. */
        DoIP_Con_HandleActLineStates();

        /* #60 Handle outstanding UDP actions. */
        DoIP_TxUdpList_HandleEntries();
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_MAIN_FUNCTION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_MainFunction() */

/**********************************************************************************************************************
 *  DoIP_SoAdTpCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyTxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  P2VAR(RetryInfoType, AUTOMATIC, DOIP_APPL_VAR) retry, /* PRQA S 3673 */ /* MD_DoIP_16.7 */
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) availableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
#else
  BufReq_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DoIP_GetSizeOfSoAdTxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'id' */
  else if ( DoIP_Util_IsTcpConnection(DoIP_GetConnectionIdxOfSoAdTxPduId(id)) == FALSE )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'info' */
  else if ( info == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'info' */
  else if ( (info->SduLength != 0u) &&
            (info->SduDataPtr == NULL_PTR) )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'retry' */
  else if ( retry != NULL_PTR )
  {
    errorId = DOIP_E_INVALID_PARAMETER; /* AUTOSAR */
  }
  /* Check parameter 'availableDataPtr' */
  else if ( availableDataPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#else
  DOIP_DUMMY_STATEMENT(retry); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DoIP_ConnectionTypeIdxOfConnectionType connectionTcpIdx =
      DoIP_GetConnectionTypeIdxOfConnection(DoIP_GetConnectionIdxOfSoAdTxPduId(id));

    /* #20 Handle SoAd TP copy tx data call. */
    retVal = DoIP_HandleSoAdTpCopyTxData(connectionTcpIdx, info, availableDataPtr);                                    /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_COPY_TX_DATA, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_SoAdTpCopyTxData() */

/**********************************************************************************************************************
 *  DoIP_SoAdTpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6010, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_SoAdTpTxConfirmation(
  PduIdType id,
  Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DoIP_GetSizeOfSoAdTxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'id' */
  else if ( DoIP_Util_IsTcpConnection(DoIP_GetConnectionIdxOfSoAdTxPduId(id)) == FALSE )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'result' */
/* coding -v33 */
  else if ( (result != E_OK) &&
            (result != E_NOT_OK) )
/* coding +v33 */
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DoIP_ConnectionTypeIdxOfConnectionType connectionTcpIdx =
        DoIP_GetConnectionTypeIdxOfConnection(DoIP_GetConnectionIdxOfSoAdTxPduId(id));

    /* #20 Handle SoAd TP txconfirmation call. */
    DoIP_HandleSoAdTpTxConfirmation(connectionTcpIdx, result);
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_TX_CONFIRMATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdTpTxConfirmation() */

/**********************************************************************************************************************
 *  DoIP_SoAdTpCopyRxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyRxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
#else
  BufReq_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DoIP_GetSizeOfSoAdRxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'id' */
  else if ( DoIP_Util_IsTcpConnection(DoIP_GetConnectionIdxOfSoAdRxPduId(id)) == FALSE )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'info' */
  else if ( info == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'bufferSizePtr' */
  else if ( bufferSizePtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DoIP_ConnectionTypeIdxOfConnectionType connectionTcpIdx =
        DoIP_GetConnectionTypeIdxOfConnection(DoIP_GetConnectionIdxOfSoAdRxPduId(id));

    /* #20 Handle request. */
    retVal = DoIP_RxTcp_HandleCopyRxData(connectionTcpIdx, info, bufferSizePtr, &errorId);                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_COPY_RX_DATA, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_SoAdTpCopyRxData() */

/**********************************************************************************************************************
 *  DoIP_SoAdTpStartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 3206 7 */ /* MD_DoIP_3206 */
/* PRQA S 6080 2 */ /* MD_MSR_STMIF */
/* PRQA S 3673 5 */ /* MD_DoIP_16.7 */
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpStartOfReception(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
#else
  BufReq_ReturnType retVal;
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DoIP_GetSizeOfSoAdRxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'id' */
  else if ( DoIP_Util_IsTcpConnection(DoIP_GetConnectionIdxOfSoAdRxPduId(id)) == FALSE )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'bufferSizePtr' */
  else if ( bufferSizePtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'DoIPPduRTxInfoPtr' */
  else if ( TpSduLength != 0u )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  DOIP_DUMMY_STATEMENT(TpSduLength);   /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  DOIP_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    DoIP_ConnectionTypeIdxOfConnectionType connectionTcpIdx =
      DoIP_GetConnectionTypeIdxOfConnection(DoIP_GetConnectionIdxOfSoAdRxPduId(id));

    /* #20 Handle SoAd TP start of reception. */
    retVal = DoIP_HandleSoAdTpStartOfReception(connectionTcpIdx);

    DOIP_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_START_OF_RECEPTION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIP_SoAdTpStartOfReception() */

/**********************************************************************************************************************
 *  DoIP_SoAdTpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_SoAdTpRxIndication(
  PduIdType id,
  Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if ( id >= DoIP_GetSizeOfSoAdRxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'id' */
  else if ( DoIP_Util_IsTcpConnection(DoIP_GetConnectionIdxOfSoAdRxPduId(id)) == FALSE )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'result' */
/* coding -v33 */
  else if ( (result != E_OK) &&
            (result != E_NOT_OK) )
/* coding +v33 */
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  DOIP_DUMMY_STATEMENT(result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state (if enabled). */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* #20 Continue if DoIP_State is shutdown (if enabled). */
      /* nothing to do here */
    }
    else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
    {
      DoIP_ConnectionTypeIdxOfConnectionType connectionTcpIdx =
        DoIP_GetConnectionTypeIdxOfConnection(DoIP_GetConnectionIdxOfSoAdRxPduId(id));

      /* #30 Otherwise deactivate TP session. */
      DoIP_SetRxTpSessionActiveOfConnectionTcpDyn(connectionTcpIdx, FALSE);                                            /* SBSW_DOIP_CSL01_CSL02_CSL03_CSL04 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TP_RX_INDICATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdTpRxIndication() */

/**********************************************************************************************************************
 *  DoIP_SoAdIfRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, DOIP_CODE) DoIP_SoAdIfRxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'RxPduId' */
  else if ( RxPduId >=  DoIP_GetSizeOfSoAdRxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'RxPduId' */
  else if ( DoIP_Util_IsUdpConnection(DoIP_GetConnectionIdxOfSoAdRxPduId(RxPduId)) == FALSE )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'PduInfoPtr' */
  else if ( PduInfoPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  /* Check parameter 'PduInfoPtr' */
  else if ( PduInfoPtr->SduDataPtr == NULL_PTR )
  {
    errorId = DOIP_E_PARAM_POINTER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Handle SoAd IF rx indication call. */
    DoIP_HandleSoAdIfRxIndication(DoIP_GetConnectionIdxOfSoAdRxPduId(RxPduId), PduInfoPtr);                            /* SBSW_DOIP_PARAM_CONST_POINTER_FORWARD */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_RX_INDICATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdIfRxIndication() */

/**********************************************************************************************************************
 *  DoIP_SoAdIfTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_SoAdIfTxConfirmation(
  PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check parameter 'TxPduId' */
  else if ( TxPduId >= DoIP_GetSizeOfSoAdTxPduId() )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  /* Check parameter 'TxPduId' */
  else if ( DoIP_Util_IsTcpConnection(DoIP_GetConnectionIdxOfSoAdTxPduId(TxPduId)) == TRUE )
  {
    errorId = DOIP_E_INVALID_PDU_SDU_ID;
  }
  else
#else
  DOIP_DUMMY_STATEMENT(TxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Do nothing. */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IF_TX_CONFIRMATION, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoAdIfTxConfirmation() */

/**********************************************************************************************************************
 *  DoIP_SoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_SoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  DoIP_SizeOfConnectionType connectionIdx = 0u;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check 'SoConId' */
  else if ( DoIP_Con_GetConnectionIdxBySoConId(SoConId, &connectionIdx) != E_OK )                                      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  /* Check 'Mode' */
  else if ( (Mode != SOAD_SOCON_ONLINE) &&
            (Mode != SOAD_SOCON_RECONNECT) &&
            (Mode != SOAD_SOCON_OFFLINE) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  /* Check 'SoConId' */
  if ( DoIP_Con_GetConnectionIdxBySoConId(SoConId, &connectionIdx) == E_OK )                                           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state (if enabled). */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* #20 Continue if DoIP_State is shutdown (if enabled). */
      /* nothing to do here */
    }
    else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
    {
      /* #30 Otherwise handle request. */
      DoIP_Con_HandleSoConModeChg(SoConId, Mode, connectionIdx, &errorId);                                             /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SO_CON_MODE_CHG, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_SoConModeChg() */

/**********************************************************************************************************************
 *  DoIP_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_LocalIpAddrAssignmentChg(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIP_E_NO_ERROR;
  DoIP_SizeOfConnectionType connectionIdx = 0u;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of all input parameters and states (if enabled). */
  /* Check initialization state */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check 'State' */
  else if ( (State != SOAD_IPADDR_STATE_ASSIGNED) &&
            (State != SOAD_IPADDR_STATE_ONHOLD) &&
            (State != SOAD_IPADDR_STATE_UNASSIGNED) )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  /* Check 'SoConId' */
  else if ( DoIP_Con_GetConnectionIdxBySoConId(SoConId, &connectionIdx) != E_OK )                                      /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#else
  /* Check 'SoConId' */
  if ( DoIP_Con_GetConnectionIdxBySoConId(SoConId, &connectionIdx) == E_OK )                                           /* SBSW_DOIP_PARAM_VALID_POINTER_FORWARD */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state (if enabled). */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* #20 Continue if DoIP_State is shutdown (if enabled). */
      /* nothing to do here */
    }
    else
#endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
    {
      /* #30 Otherwise handle request. */
      DoIP_Con_HandleLocalIpAddrAssignmentChg(connectionIdx, State);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_LOC_IP_ADDR_ASSIGN_CHG, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_LocalIpAddrAssignmentChg() */

/**********************************************************************************************************************
 *  DoIP_ShutdownFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_ShutdownFinished(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
  uint8 errorId = DOIP_E_NO_ERROR;
#else
  uint8 errorId = DOIP_E_SHUTDOWN_FINISHED; /* shutdown finished not supported since callback is not configured */
#endif /* DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if enabled). */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
    /* Check initialization state (if enabled). */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* #20 Continue if DoIP_State is shutdown (if enabled). */
      /* nothing to do here */
    }
    else
    {
      /* #30 Handle shutdown finished (if enabled). */
      DoIP_State = DOIP_STATE_SHUTDOWN;
      DoIP_CallShutdownFinishedDirect();                                                                               /* SBSW_DOIP_FUNCTION_POINTER_CALL */
    }
#else
    /* API disabled */
    /* nothing to do here */
#endif /* DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
# if ( DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
# endif /* DOIP_SHUTDOWN_FINISHED_CBK_SUPPORT == STD_ON */
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SHUTDOWN_FINISHED, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_ShutdownFinished() */

/**********************************************************************************************************************
 *  DoIP_DhcpEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIP_CODE) DoIP_DhcpEvent(
  SoAd_LocalAddrIdType LocalIpAddrId,
  SoAd_DhcpEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  uint8 errorId = DOIP_E_NO_ERROR;
#else
  uint8 errorId = DOIP_E_DHCP_EVENT; /* dhcp event not supported since callback is not configured */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameter 'Event' and states (if enabled). */
  /* Do not check 'LocalIpAddrId' since its range is not constrained. */
  if ( DoIP_State == DOIP_STATE_UNINIT )
  {
    errorId = DOIP_E_UNINIT;
  }
  /* Check 'Event' */
  else if ( Event > SOAD_DHCP_EVENT_TX_V6_DECLINE )
  {
    errorId = DOIP_E_INVALID_PARAMETER;
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
# if ( DOIP_SUPPORT_SHUTDOWN == STD_ON )
    /* Check initialization state (if enabled). */
    /* with respect to SHUTDOWN state check DET only against UNINIT state and check callbacks against INIT state */
    if ( DoIP_State != DOIP_STATE_INIT )
    { /* DoIP_State is not INIT (it could be SHUTDOWN) */
      /* #20 Continue if DoIP_State is shutdown (if enabled). */
      /* nothing to do here */
    }
    else
# endif /* DOIP_SUPPORT_SHUTDOWN == STD_ON */
    {
      /* #30 Handle DHCP Event. */
      DoIP_HandleDhcpEvent(LocalIpAddrId, Event);
    }
#else
    /* API disabled */
    /* nothing to do here */
    DOIP_DUMMY_STATEMENT(LocalIpAddrId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    DOIP_DUMMY_STATEMENT(Event);         /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIP_DEV_ERROR_REPORT == STD_ON )
# if ( DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON )
  if ( errorId != DOIP_E_NO_ERROR )
# endif /* DOIP_DHCP_OPT_VENDOR_CLASS == STD_ON */
  {
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_DHCP_EVENT, errorId);
  }
#else
  DOIP_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIP_DEV_ERROR_REPORT == STD_ON */
} /* DoIP_DhcpEvent() */

#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* module specific MISRA deviations:

  MD_DoIP_11.1:
      Reason:     Casting to different function pointer type
      Risk:       Misinterpretation of data.
      Prevention: Code inspection.

  MD_DoIP_11.4:
      Reason:     Casting to different object pointer type
      Risk:       Misinterpretation of data.
      Prevention: Code inspection.

  MD_DoIP_16.7:
      Reason:     No pointer to const due to modification in a called function (maybe only in special variants) or
                  required by specification.
      Risk:       None.
      Prevention: None.

  MD_DoIP_21.1_CFG:
      Reason:     Error does not apply since prevention is given in configuration.
      Risk:       None.
      Prevention: None.

  MD_DoIP_21.1_IDX:
      Reason:     Indexing out of bounds prevented by complex mechanism (misinterpreted by test tool).
      Risk:       None.
      Prevention: None.

  MD_DoIP_0686:
      Reason:     All not initialized elements are overwritten later.
      Risk:       Uninitialized elements may remain.
      Prevention: Code inspection.

  MD_DoIP_3206:
      Reason:     Unused parameter due to configuration constraints in some variants.
      Risk:       None.
      Prevention: None.

  MD_DoIP_ObjectOnlyAccessedOnce:
      Reason:     There are generated structures and arrays which are sometimes only accessed by one function, in
                  either the translation unit where they are defined or in another translation unit. We have generated
                  and manually written source code and separate code and data structures. Due to preprocessing variants
                  the above situation can occur.
      Risk:       It isn't clear which code and data structures belong together.
      Prevention: Code inspection.

  MD_DoIP_ImplicitCharCast:
      Reason:     An expression of 'essentially character' type (char) is being converted to unsigned type, 'unsigned
                  char' on assignment. There are chars who get implicit converted to uint8 data. For readability there
                  is no explicit cast.
      Risk:       Misinterpretation of data.
      Prevention: Code inspection.

  MD_DoIP_Rule2.2_2985_2986:
      Reason:     This operation is redundant. The value of the result is always that of the left/right-hand operand.
                  In some configurations there are a redundant operation to keep the complexity low.
      Risk:       None.
      Prevention: None.

  MD_DoIP_Rule2.2_2987:
      Reason:     This function call produces no side effects and is redundant.
                  Global data is set inside the called function by using the ComStackLib. Therefore, the function call
                  is required.
      Risk:       None.
      Prevention: None.

  MD_DoIP_GenericWiderTypeCast:
      Reason:     A configuration dependent data type is cast to the widest expected data type to be
                  generic/configuration independent.
      Risk:       Type mismatch may indicate usage of a wrong data type or variable.
      Prevention: Code inspection.

*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_DOIP_CSL01
    \DESCRIPTION      Access of array element with index generated by ComStackLib.
    \COUNTERMEASURE   \N  Qualified use-case CSL01 of ComStackLib.

  \ID SBSW_DOIP_CSL02
    \DESCRIPTION      Access of array element with index checked against size of different array.
                      (Arrays Xyz[] and XyzDyn[] depend on ComStackLib size relation.)
    \COUNTERMEASURE   \N  Qualified use-case CSL02 of ComStackLib.

  \ID SBSW_DOIP_CSL03
    \DESCRIPTION      Access of array element with index modeled by indirection generated by ComStackLib.
    \COUNTERMEASURE   \N  Qualified use-case CSL03 of ComStackLib.

  \ID SBSW_DOIP_CSL03_CSL01
    \DESCRIPTION      Access of array element with index modeled by indirection generated by ComStackLib. The index to
                      access the indirection is a index generated by ComStackLib.
    \COUNTERMEASURE   \N  Qualified use-cases CSL01 and CSL03 of ComStackLib.

  \ID SBSW_DOIP_CSL02_CSL03
    \DESCRIPTION      Access of array element with a index checked against the size of different array generated by
                      ComStackLib. Additionally the index is modeled by indirection generated by ComStackLib.
    \COUNTERMEASURE   \N  Qualified use-cases CSL02 and CSL03 of ComStackLib.

  \ID SBSW_DOIP_CSL01_CSL02_CSL03_CSL04
    \DESCRIPTION      Access of array element with index modeled by multiple destinations indirection generated by
                      ComStackLib. The index which is used to access the indirection is another indirection.
    \COUNTERMEASURE   \N  The index used to get the first indirection is checked against the size of the corresponding
                          array (qualified use-case CSL01). The indirection index is used to get the multiple
                          destination indirection (qualified use-case CSL03). The destination of the multiple
                          destination indirection is checked and then used for array write access
                          (qualified use-case CSL04) to a different array modled by size relation
                          (qualified use-case CSL02).

  \ID SBSW_DOIP_ARRAY_WRITE
    \DESCRIPTION      Write access to array.
    \COUNTERMEASURE   \N  Array is a local variable/buffer which points to valid memory location and can be accessed
                          with index respecting write size and size of memory location. The array is either accessed
                          with static indexes within this range or with indexes checked against the variable/buffer
                          size.

  \ID SBSW_DOIP_EntityAddr
    \DESCRIPTION      Access of global element.
    \COUNTERMEASURE   \N  A global variable is accessed (without index).

  \ID SBSW_DOIP_RoutActivHandler
    \DESCRIPTION      Access of array element with index 0.
    \COUNTERMEASURE   \N  Routing Activation Handler is an array with the fix size of 1 and is always accessed with the
                          index 0.

  \ID SBSW_DOIP_RoutActivHandler_Data
    \DESCRIPTION      Access of array element with index from Routing Activation Handler. The index is checked against
                      size of different array.
    \COUNTERMEASURE   \N  The indexes used from DoIP_GetConnectionTcpIdxOfRoutActivHandler(),
                          DoIP_GetRoutingActivationIdxOfRoutActivHandler() and DoIP_GetTesterIdxOfRoutActivHandler()
                          are after initialization always valid or equals to DoIP_GetSizeOfXyz() of an array with same
                          size (qualified use-case CSL02 of ComStackLib).
                          These indexes are set to valid values in one single location in
                          DoIP_RxTcpRout_HandleCopiedRoutingActivationReq(). Since all values are set consistently it
                          is possible to use DoIP_GetConnectionTcpIdxOfRoutActivHandler() to check if the other valeus
                          are valid, too.
                          These indexes are set to DoIP_GetSizeOfXyz() in context of DoIP_SoConModeChg() or if the
                          routing activation response could be activated and sent successfully in context of
                          DoIP_SoAdTpCopyTxData() (--> DoIP_TxTcpQueue_CopyHeader()) or if routing activation is
                          pending and during succssfully retry in DoIP_MainFunction()
                          (--> DoIP_RxTcpRout_HandleRetryRoutingActivationAuthAndConf()).

  \ID SBSW_DOIP_TxTcpMgt
    \DESCRIPTION      Access of array element with calculated index used for a different array with same size generated
                      by ComStackLib. The calculation is based on function parameter and index stored in
                      DoIP_ConnectionTcpDyn.
    \COUNTERMEASURE   \N  The validity of the index passed as function parameter is defined at the function declaration
                          and has to be ensured by the caller. The index is calculated based on qualified use-cases
                          CSL01 and CSL03 of ComStackLib. The access to the array of different size is qualified
                          use-case CSL02.
                          After calling DoIP_Init() the queue is empty and all queue management elements are valid. New
                          elements are added within the DoIP_TxTcp_Transmit() function. The index is the index to the
                          first element (indicated by level). The index is relative between start and end
                          index of the tcp connection. The index passed to DoIP_TxTcpQueue_AddXyz() as function
                          parameter is calculated in DoIP_TxTcpQueue_Add() and is the index to the next free slot where
                          the new element can be addded. The data within the queue are valid after they get added by
                          DoIP_TxTcp_Transmit() and copied by CopyTxData call until they get removed within the
                          TxConfirmation call or they are removed if socon mode changes to offline.

  \ID SBSW_DOIP_RegisterTester
    \DESCRIPTION      Access of array element with index stored in DoIP_ConnectionTcpDyn for a different array with
                      same size.
    \COUNTERMEASURE   \N  The access to the array of different size is qualified use-case CSL02.
                          The data set with DoIP_RxTcpRout_RegisterTester() are DoIP_SetTesterIdxOfConnectionTcpDyn(),
                          DoIP_SetTesterAddrOfConnectionTcpDyn(), DoIP_RoutingActivationIdxOfConnectionTcpDyn() and
                          DoIP_ActivStateOfConnectionTcpDyn(). They are set to valid values if a routing activation
                          response is sent for a routing activation request or if the authentification or confirmation
                          is pending and they are reset to DoIP_GetSizeOfXyz() with the alive check timeout for routing
                          activations and they are reset if socon mode change to offline. At initialization time the
                          value is set to DoIP_GetSizeOfXyz().

  \ID SBSW_DOIP_RxHdrBuf_AND_CONST_POINTER_FORWARD
    \DESCRIPTION      The function forwards pointer to RxHdrBuf as function parameter with an index which could be
                      used to write to the corresponding memory location and the function forwards a const pointer.
    \COUNTERMEASURE   \N  The RxHdrBuf is used to store parts of different messages and has at least the size of
                          (buffer offset + bytes to copy). The messages which are stored within the buffer are the
                          received routing activation request (size 19 bytes) and the received alive check response
                          (size 10 bytes). A received diagnostic message is only stored partly within the buffer.
                          From the diagnostic message the generic header, the logical addresses and the configured
                          size of user data to be buffered (DOIP_BUFFERED_USER_DATA_LENGTH bytes) are stored in the
                          RxHdrBuf. The RxHdrBuf is generated respecting the largest required size. The offset and the
                          bytes to copy are checked against the received message type within
                          DoIP_RxTcp_HandleCopyRxData(). The offset is the length of the already received message
                          parts. The bytes to copy is the length of a message part which should be copied.
                          The const pointer which is forwarded can be used to read data, only.

  \ID SBSW_DOIP_OemPayloadTxBuf
    \DESCRIPTION      The function forwards pointer to DoIP_OemPayloadTxBuf as function parameter which could be used
                      to write to the corresponding memory location.
    \COUNTERMEASURE   \N  On reservation of the DoIP_OemPayloadTxBuf it is asserted that the buffer is big enough for
                          the requested payload length plus the generic header length. Pointers passed to function
                          are defined within the valid range at the function declaration and can be accessed with index
                          respecting write size and size of memory location. This is ensured by the caller.

  \ID SBSW_DOIP_TxHdrBuf
    \DESCRIPTION      Write access to pointer. The pointer may be accessed with an index.
    \COUNTERMEASURE   \N  It's a fixed size buffer of 21 bytes generated by ComStackLib. The array is accessed with
                          static indexes within this range.

  \ID SBSW_DOIP_TxHdrBuf_FORWARD
    \DESCRIPTION      The function forwards pointer as function parameter which could be used to write to the
                      corresponding memory location. The pointer may be forwarded with an offset.
    \COUNTERMEASURE   \N  Pointer passed as function parameter points to a fixed size buffer of 21 bytes and is
                          generated by ComStackLib. The offset is always within this range. The called function writes
                          a static number of data into this buffer or a separate function parameter is used to indicate
                          the range.

  \ID SBSW_DOIP_IfTxBuf_AND_CONST_POINTER_FORWARD
    \DESCRIPTION      The function forwards pointer as function parameter which could be used to write to the
                      corresponding memory location and forwards a const pointer.
    \COUNTERMEASURE   \N  It's a buffer generated by ComStackLib. The size depends on the configured size. The size of
                          the buffer is checked previously within the function DoIP_IfTransmit().
                          The const pointer which is forwarded can be used to read data, only.

  \ID SBSW_DOIP_DiagNAckBuf_AND_CONST_POINTER_FORWARD
    \DESCRIPTION      The function forwards pointer as function parameter which could be used to write to the
                      corresponding memory location and forwards a const pointer.
    \COUNTERMEASURE   \N  It's a buffer generated by ComStackLib. The size depends on the configured size. A separate
                          function parameter is used to indicate the range of the buffer. This parameter is set
                          according to the sizes provided by ComStackLib via indirection (qualified use-case CSL03 of
                          ComStackLib).
                          The const pointer which is forwarded can be used to read data, only.

  \ID SBSW_DOIP_AliveCheckList
    \DESCRIPTION      Access of array element with index.
    \COUNTERMEASURE   \N  Alive Check List is a array with a fix size of the maximum number of TCP connections per
                          local address and is accessed with an index. Index may be fixed size or the number of TCP
                          connections per local address is counted. The array contains at least one element because
                          there is always at least one connection configured.

  \ID SBSW_DOIP_PARAM_VALID_INDEX
    \DESCRIPTION      Access of array element with index passed as a function parameter.
    \COUNTERMEASURE   \N  The validity of the index passed as function parameter is defined at the function declaration
                          and has to be ensured by the caller.

  \ID SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02
    \DESCRIPTION      Access of array element with index passed as a function parameter. The index is specified for a
                      different array with same size generated by ComStackLib.
    \COUNTERMEASURE   \N  The validity of the index passed as function parameter is defined at the function declaration
                          and has to be ensured by the caller. To access with an index of a different array with same
                          size is qualified use-case CSL02 of ComStackLib.

  \ID SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL03
    \DESCRIPTION      Access of array element with an index which is modeled by an indirection generated by the
                      ComStackLib (CSL03) and is derived from an index passed as function parameter. The derived index
                      is specified for a different array with same size generated by ComStackLib (CSL02).
    \COUNTERMEASURE   \N  The validity of the index passed as function parameter is defined at the function declaration
                          and has to be ensured by the caller. The required index is derived by an indirection which is
                          qualified use-case CSL03 of ComStackLib. To access with an index of a different array with
                          same size is qualified use-case CSL02 of ComStackLib.

  \ID SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL03
    \DESCRIPTION      Access of array element with index modeled by indirection generated by ComStackLib. The index
                      which is used to access the indirection is passed as function parameter.
    \COUNTERMEASURE   \N  The validity of the index passed as function parameter is defined at the function declaration
                          and has to be ensured by the caller. The index modeled by indirection is qualified use-case
                          CSL03 of ComStackLib.

  \ID SBSW_DOIP_PARAM_VALID_INDEX_AND_CSL02_CSL04
    \DESCRIPTION      Access of array element with index modeled by multiple destinations indirection generated by
                      ComStackLib. The index which is used to access the indirection is passed as function parameter.
    \COUNTERMEASURE   \N  The validity of the index passed as function parameter is defined at the function declaration
                          (might include the allowed indirection target) and has to be ensured by the caller.
                          The indirection target is either checked during runtime or specified at the function
                          declaration. The index modeled by multiple destinations indirection is qualified use-case
                          CSL04 of ComStackLib and is used to access a different array modeled by size relation.
                          Qualified use-case CSLO02 of ComStackLib.

  \ID SBSW_DOIP_PARAM_VALID_POINTER_WRITE
    \DESCRIPTION      Write access to pointer. The pointer used to write may depend on a function parameter and may be
                      passed to other functions or is a reference to a local variable/buffer which could be used to
                      write to the corresponding memory location. The pointer may be accessed with an index.
    \COUNTERMEASURE   \N  Pointer passed as function parameter or pointer to a local variable/buffer points to valid
                          memory location. This includes struct members in case pointer points to a struct type.
                          Pointers passed to function are defined within the valid range at the function declaration
                          and can be accessed with index respecting write size and size of memory location. This is
                          ensured by the caller.

  \ID SBSW_DOIP_PARAM_VALID_POINTER_FORWARD
    \DESCRIPTION      The function forwards pointer as function parameter or a reference to a local variable/buffer
                      which could be used to write to the corresponding memory location. The pointer may be forwarded
                      with an offset.
    \COUNTERMEASURE   \N  Pointers passed to function point to a valid memory range considering type or an additional
                          function parameter is used to describe pointer validity. The validity of the local
                          variable/buffer is ensured due to the scope. Pointers passed to function is defined within
                          the valid range at the function declaration and can be accessed with index respecting write
                          size and size of memory location. This is ensured by the caller.

  \ID SBSW_DOIP_PARAM_CONST_POINTER_FORWARD
    \DESCRIPTION      The function forwards const pointer as function parameter or a reference to a local
                      variable/buffer which could be used to read the corresponding memory location. The pointer may be
                      forwarded with an offset or may be accessed with an index.
    \COUNTERMEASURE   \N  Pointers passed to function as const pointers can be used to read data only.

  \ID SBSW_DOIP_PARAM_NULL_POINTER_FORWARD
    \DESCRIPTION      The function forwards NULL_PTR as function parameter.
    \COUNTERMEASURE   \N  NULL_PTR is within the valid range of the called function.

  \ID SBSW_DOIP_PARAM_VALID_POINTER_AND_CONST_POINTER_FORWARD
    \DESCRIPTION      The function forwards pointer as function parameter or a reference to a local variable/buffer
                      which could be used to write to the corresponding memory location and the function forwards a
                      const pointer as function parameter. The pointer may be forwarded with an offset or may be
                      accessed with an index.
    \COUNTERMEASURE   \N  Pointers passed to function point to a valid memory range considering type or an additional
                          function parameter is used to describe pointer validity. The validity of the local
                          variable/buffer is ensured due to the scope. Pointers passed to function are defined within
                          the valid range at the function declaration and can be accessed with index respecting write
                          size and size of memory location. This is ensured by the caller. The const pointers can be
                          used to read data only.

  \ID SBSW_DOIP_PARAM_VALID_OEM_PAYLOAD_RX_BUF_CFG_INDEX_AS_POINTER_AND_CONST_POINTER_FORWARD
    \DESCRIPTION      Access to array element with index taken from a RAM array which has been previously stored inside
                      this array with a valid index. This index is used to get the address of a buffer which is
                      forwarded as function parameter and can be used to write to the corresponding memory location.
                      Additionally, the function forwards a const pointer as function parameter. The pointer may
                      be forwarded with an offset or may be accessed with an index.
    \COUNTERMEASURE   \R  The index stored in the RAM array will always be valid. It is asserted by runtime checks that
                          DoIP_RxTcpOem_ReserveBuf has been called successfully before deriving the index in this
                          context so that OemSpecificCfgDyn contains a valid OemPayloadRxBufConfigIdx. Therefore, the
                          accessed buffer must also be valid. This is asserted by qualified use-case CSL02 and CSL03
                          when writing the index to the array. The const pointer can be used to read data only.

  \ID SBSW_DOIP_PARAM_VALID_POINTER_AND_NULL_POINTER_FORWARD
    \DESCRIPTION      The function forwards pointer as function parameter or a reference to a local variable/buffer
                      which could be used to write to the corresponding memory location. The pointer may be forwarded
                      with an offset or may be accessed with an index. Additionally the function forwards NULL_PTR as
                      function parameter.
    \COUNTERMEASURE   \N  Pointers passed to function point to a valid memory range considering type or an additional
                          function parameter are used to describe pointer validity. The validity of the local
                          variable/buffer is ensured due to the scope. Pointers passed to function are defined within
                          the valid range at the function declaration and can be accessed with index respecting write
                          size and size of memory location. This is ensured by the caller.
                          NULL_PTR is within the valid range of the called function.

  \ID SBSW_DOIP_FUNCTION_POINTER_CALL
    \DESCRIPTION      Calling a function pointer generated by ComStackLib.
    \COUNTERMEASURE   \S The user of MICROSAR Safe shall perform the integration (ISO 26262:6-10) and verification
                         (ISO 26262:6-11) processes as required by ISO 26262.
                         SMI-4
                      \R Index validity is ensured by qualified use-case CSL03 of ComStackLib.
                         In case of optional indirection it is ensured that optional indirection is available according
                         to Safe BSW assumptions modeled in ComStackLib.

SBSW_JUSTIFICATION_END */

/* INTERNAL_NOTE_DOIP_JUSTIFICATION_BEGIN

  \ID INTERNAL_NOTE_DOIP_CAST_HEADER_ALREADY_RECEIVED
    \DESCRIPTION  The cast to uint32 can be done because the generic DoIP header is already fully received. Due to this
                  the remaining message size fits into uint32 data range. Note: the largest DoIP message has a size of
                  8 byte generic DoIP header and uint32.MAX_VALUE of payload bytes.

INTERNAL_NOTE_DOIP_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

  \ID COV_DOIP_COMPATIBILITY
    \ACCEPT   TX
    \REASON   [COV_MSR_COMPATIBILITY]

  \ID COV_DOIP_SUPPORTED
    \ACCEPT   TX tf tx
    \REASON   The feature is supported in SafeBSW. A MSSV Plugin enforces this configuration.

  \ID COV_DOIP_DEFAULT_CASE
    \ACCEPT   XX
    \REASON   The default case catchs the DOIP_IPADDR_ASSIGN_NONE assignment (link local address with IPv6 Router
              Advertisment assignment). In an IPv4 only configuration this case is not covered.

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: DoIP.c
 *********************************************************************************************************************/
