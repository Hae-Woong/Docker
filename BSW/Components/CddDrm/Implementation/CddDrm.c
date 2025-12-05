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
/*!        \file  CddDrm.c
 *        \brief  CddDrm source file
 *
 *      \details
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

#define CDDDRM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "CddDrm.h"
#include "PduR_CddDrm.h"

/* critical sections handled by SchM */
#include "SchM_CddDrm.h"

#include "CddDrm_Cbk.h"

#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of CddDrm header file */
#if (  (CDDDRM_SW_MAJOR_VERSION != (7U)) \
    || (CDDDRM_SW_MINOR_VERSION != (0U)) \
    || (CDDDRM_SW_PATCH_VERSION != (1U)) )
# error "Vendor specific version numbers of CddDrm.c and CddDrm.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (CDDDRM_CFG_SW_MAJOR_VERSION != (7U)) \
    || (CDDDRM_CFG_SW_MINOR_VERSION != (0U)) )
# error "Version numbers of CddDrm.c and CddDrm_Cfg.h are inconsistent!"
#endif

/* Check the version of the type header file */
#if (  (CDDDRM_TYPES_MAJOR_VERSION != (7U)) \
    || (CDDDRM_TYPES_MINOR_VERSION != (0U)) \
    || (CDDDRM_TYPES_PATCH_VERSION != (1U)) )
# error "Version numbers of CddDrm.c and CddDrm_Types.h are inconsistent"
#endif

/* Check the version of the callback header file */
#if (  (CDDDRM_CBK_MAJOR_VERSION != (7U)) \
    || (CDDDRM_CBK_MINOR_VERSION != (0U)) \
    || (CDDDRM_CBK_PATCH_VERSION != (1U)) )
# error "Version numbers of CddDrm.c and CddDrm_Cbk.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined (CddDrm_MemCopy)                                                                                                                        /* COV_CDDDRM_USER_MACROS */
# define CddDrm_MemCopy(dst, src, length)                             CddDrm_LocalMemCopy((dst), (src), (length))                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

/* ----- Channel reference      ------------------------------------------------------------------------------------ */
#define CDDDRM_CHANNEL_NOT_ASSIGNED                                   (0xffU) /*<! Channel reference free */

/* ----- Channel States -------------------------------------------------------------------------------------------- */
#define CDDDRM_CHANNEL_STATE_INACTIVE                                 (0U)      /*<! Channel inactive */
#define CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL                            (1U)      /*<! Channel waits for closing */
#define CDDDRM_CHANNEL_STATE_WAIT_DELAY                               (2U)      /*<! Channel waits for delay elapsed */
#define CDDDRM_CHANNEL_STATE_REQUEST_START                            (3U)      /*<! Channel starts sending request */
#define CDDDRM_CHANNEL_STATE_WAIT_RESPONSE                            (4U)      /*<! Channel waits for response */
#define CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING                      (5U)      /*<! Channel waits for response processing */
#define CDDDRM_CHANNEL_STATE_REQUEST_PROCESSING                       (6U)      /*<! Channel waits for request processing */


/* ----- Channel Allocation -------------------------------------------------------------------------------------------- */
#define CDDDRM_CHANNEL_CLIENT_ECU_DETECTION                           (0U)
#define CDDDRM_CHANNEL_CLIENT_APPLICATION                             (1U)

/* ----- Drm States ------------------------------------------------------------------------------------------------ */
#define CDDDRM_STATE_RUN                                              (0U)      /*<! Normal run mode */
#define CDDDRM_STATE_TESTER_DETECTED                                  (1U)      /*<! External tester is present */
#define CDDDRM_STATE_FUNCTIONAL_REQUEST_ACTIVE                        (2U)      /*<! Functional request is active */

/* ----- CDDDRM Response validation value -------------------------------------------------------------------------- */
#define CDDDRM_RESPONSE_CODE_VALIDATION                               (0x40U)   /*<! Used for validate response */

/* ----- QUEUE ----------------------------------------------------------------------------------------------------- */
#define CDDDRM_QUEUE_EMPTY                                            (0xffU)
/* ----- QUEUE_ECUD ----- */
#define CDDDRM_QUEUE_ECUD_START                                       (0U)
#define CDDDRM_QUEUE_ECUD_STOP                                        (1U)
/* External tester queue */
#define CDDDRM_QUEUE_EXTERNAL_TESTER_CONNECTED                        (0U)
#define CDDDRM_QUEUE_EXTERNAL_TESTER_DISCONNECTED                     (1U)

/* ----- ECU Detection Parameters ---------------------------------------------------------------------------------- */
/* ----- Ecud_States ------ */
#define CDDDRM_ECUD_STATE_IDLE                                        (0U)
#define CDDDRM_ECUD_STATE_READY                                       (1U)
#define CDDDRM_ECUD_STATE_BUSY                                        (2U)
#define CDDDRM_ECUD_STATE_WRITE_NVM                                   (3U)
#define CDDDRM_ECUD_STATE_FINISH                                      (4U)
/* ----- Ecud result ----- */
#define CDDDRM_ECUD_RESULT_MASK                                       (0x03U)
/* ----- Dependent on configuration ------ */
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
# define CDDDRM_CFG_ECUD_INDEX_LIMIT                                  (CDDDRM_CFG_PHYS_CONNECTION_COUNT)
/* Define start index of channel iteration if ECU Detection is enabled */
/* ECU detection and functional request processing are using the same channel */
# define CDDDRM_CHANNEL_ECU_DETECTION_INDEX                           (0U)
#endif
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON || CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/* Define the channel index, that is used for application channels */
# define CDDDRM_CHANNEL_APPLICATION_INDEX                             (1U)
# define CDDDRM_APPL_CHANNEL_COUNT                                    (CDDDRM_CFG_CHANNEL_COUNT - 1U)
#else
/* define start index of channel iteration */
# define CDDDRM_CHANNEL_APPLICATION_INDEX                             (0U)
# define CDDDRM_APPL_CHANNEL_COUNT                                    (CDDDRM_CFG_CHANNEL_COUNT)
#endif
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/* ECU detection and functional request processing are using the same channel */
# define CDDDRM_CHANNEL_FUNCTIONAL_INDEX                              (0U)
/* Service index is always 0 for functional requests as they can not be configured as dynamic */
# define CDDDRM_CHANNEL_FUNCTIONAL_SVC_INDEX                          (0U)
#endif
/* ----- Dynamic Connection Parameters ----------------------------------------------------------------------------- */

#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
# define CDDDRM_SVCID_INDEX                                           (CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)
#else
# define CDDDRM_SVCID_INDEX                                           (0U)
#endif

/* ----- UDS Services ---------------------------------------------------------------------------------------------- */
#define CDDDRM_SERVICE_10                                             (0x10U)
#define CDDDRM_SERVICE_11                                             (0x11U)
#define CDDDRM_SERVICE_14                                             (0x14U)
#define CDDDRM_SERVICE_19                                             (0x19U)
#define CDDDRM_SERVICE_22                                             (0x22U)
#define CDDDRM_SERVICE_23                                             (0x23U)
#define CDDDRM_SERVICE_24                                             (0x24U)
#define CDDDRM_SERVICE_27                                             (0x27U)
#define CDDDRM_SERVICE_28                                             (0x28U)
#define CDDDRM_SERVICE_2A                                             (0x2aU)
#define CDDDRM_SERVICE_2C                                             (0x2cU)
#define CDDDRM_SERVICE_2E                                             (0x2eU)
#define CDDDRM_SERVICE_2F                                             (0x2fU)
#define CDDDRM_SERVICE_31                                             (0x31U)
#define CDDDRM_SERVICE_34                                             (0x34U)
#define CDDDRM_SERVICE_35                                             (0x35U)
#define CDDDRM_SERVICE_36                                             (0x36U)
#define CDDDRM_SERVICE_37                                             (0x37U)
#define CDDDRM_SERVICE_38                                             (0x38U)
#define CDDDRM_SERVICE_3D                                             (0x3dU)
#define CDDDRM_SERVICE_3E                                             (0x3eU)
#define CDDDRM_SERVICE_83                                             (0x83U)
#define CDDDRM_SERVICE_84                                             (0x84U)
#define CDDDRM_SERVICE_85                                             (0x85U)
#define CDDDRM_SERVICE_86                                             (0x86U)
#define CDDDRM_SERVICE_87                                             (0x87U)

/* ----- Dedicated Service Parameters ------------------------------------------------------------------------------ */
#if (CDDDRM_CFG_SUPPORT_SERVICE_10 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_10                                    (2U)
# define CDDDRM_SERVICE_10_MIN_RESPONSE_BUFFER                        (6U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_11 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_11                                    (2U)
# define CDDDRM_SERVICE_11_MIN_RESPONSE_BUFFER                        (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_1902 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_1902                                  (3U)
# define CDDDRM_SERVICE_1902_MIN_RESPONSE_BUFFER                      (3U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_1904 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_1904                                  (6U)
# define CDDDRM_SERVICE_1904_MIN_RESPONSE_BUFFER                      (3U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_22 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_22                                    (3U)
# define CDDDRM_SERVICE_22_MIN_RESPONSE_BUFFER                        (4U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_27 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_27                                    (2U)
# define CDDDRM_SERVICE_27_MIN_RESPONSE_BUFFER                        (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_28 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_28                                    (3U)
# define CDDDRM_SERVICE_28_MIN_RESPONSE_BUFFER                        (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_31 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_31                                    (4U)
# define CDDDRM_SERVICE_31_MIN_RESPONSE_BUFFER                        (4U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_34 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_34                                    (3U)
# define CDDDRM_SERVICE_34_MIN_RESPONSE_BUFFER                        (4U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_36 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_36                                    (2U)
# define CDDDRM_SERVICE_36_MIN_RESPONSE_BUFFER                        (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_37 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_37                                    (1U)
# define CDDDRM_SERVICE_37_MIN_RESPONSE_BUFFER                        (1U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_3E == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_3E                                    (2U)
# define CDDDRM_SERVICE_3E_MIN_RESPONSE_BUFFER                        (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_85 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_85                                    (2U)
# define CDDDRM_SERVICE_85_MIN_RESPONSE_BUFFER                        (2U)
#endif

/* ----- Local conversion of active firewall rules ----------------------------------------------------------------- */
#if (CDDDRM_CFG_BITPOS_SERVICE_10 != 0)
# define CDDDRM_FIREWALL_SERVICE_10                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_10                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_11 != 0)
# define CDDDRM_FIREWALL_SERVICE_11                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_11                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_14 != 0)
# define CDDDRM_FIREWALL_SERVICE_14                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_14                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_19 != 0)
# define CDDDRM_FIREWALL_SERVICE_19                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_19                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_22 != 0)
# define CDDDRM_FIREWALL_SERVICE_22                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_22                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_23 != 0)
# define CDDDRM_FIREWALL_SERVICE_23                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_23                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_24 != 0)
# define CDDDRM_FIREWALL_SERVICE_24                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_24                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_27 != 0)
# define CDDDRM_FIREWALL_SERVICE_27                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_27                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_28 != 0)
# define CDDDRM_FIREWALL_SERVICE_28                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_28                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2A != 0)
# define CDDDRM_FIREWALL_SERVICE_2A                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_2A                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2C != 0)
# define CDDDRM_FIREWALL_SERVICE_2C                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_2C                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2E != 0)
# define CDDDRM_FIREWALL_SERVICE_2E                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_2E                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2F != 0)
# define CDDDRM_FIREWALL_SERVICE_2F                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_2F                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_31 != 0)
# define CDDDRM_FIREWALL_SERVICE_31                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_31                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_34 != 0)
# define CDDDRM_FIREWALL_SERVICE_34                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_34                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_35 != 0)
# define CDDDRM_FIREWALL_SERVICE_35                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_35                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_36 != 0)
# define CDDDRM_FIREWALL_SERVICE_36                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_36                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_37 != 0)
# define CDDDRM_FIREWALL_SERVICE_37                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_37                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_38 != 0)
# define CDDDRM_FIREWALL_SERVICE_38                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_38                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_3D != 0)
# define CDDDRM_FIREWALL_SERVICE_3D                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_3D                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_3E != 0)
# define CDDDRM_FIREWALL_SERVICE_3E                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_3E                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_83 != 0)
# define CDDDRM_FIREWALL_SERVICE_83                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_83                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_84 != 0)
# define CDDDRM_FIREWALL_SERVICE_84                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_84                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_85 != 0)
# define CDDDRM_FIREWALL_SERVICE_85                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_85                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_86 != 0)
# define CDDDRM_FIREWALL_SERVICE_86                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_86                                   STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_87 != 0)
# define CDDDRM_FIREWALL_SERVICE_87                                   STD_ON
#else
# define CDDDRM_FIREWALL_SERVICE_87                                   STD_OFF
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define CddDrm_GetChannelMessageLength(channelIndex)                  (CddDrm_ChannelInfo[(channelIndex)].messageLength)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelMessageLength(channelIndex, val)             (CddDrm_ChannelInfo[(channelIndex)].messageLength = (val))                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelCurrentMessageIndex(channelIndex)            (CddDrm_ChannelInfo[(channelIndex)].currentMessageIndex)                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelCurrentMessageIndex(channelIndex, val)       (CddDrm_ChannelInfo[(channelIndex)].currentMessageIndex = (val))                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_AddChannelCurrentMessageIndex(channelIndex, addVal)    (CddDrm_ChannelInfo[(channelIndex)].currentMessageIndex += (addVal))                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelServiceId(channelIndex)                      (CddDrm_ChannelInfo[(channelIndex)].protocolData[0U])                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelServiceId(channelIndex, val)                 (CddDrm_ChannelInfo[(channelIndex)].protocolData[0U] = (val))                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelResponseCode(channelIndex)                   (CddDrm_ChannelInfo[(channelIndex)].responseCode)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelResponseCode(channelIndex, val)              (CddDrm_ChannelInfo[(channelIndex)].responseCode = (val))                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelSPRMIB(channelIndex)                         (CddDrm_ChannelInfo[(channelIndex)].useSPRMIB)                                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelSPRMIB(channelIndex, val)                    (CddDrm_ChannelInfo[(channelIndex)].useSPRMIB = (val))                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelTesterPresent(channelIndex)                  (CddDrm_ChannelInfo[(channelIndex)].testerPresent)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelTesterPresent(channelIndex, val)             (CddDrm_ChannelInfo[(channelIndex)].testerPresent = (val))                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* ----- request buffer macros ------------------------------------------------------------------------------------- */
#define CddDrm_GetChannelReqBufferInfoPtrVal(channelIndex, ptrIndex)  (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.requestBufferDataPtr[(ptrIndex)])     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelReqBufferInfoPtr(channelIndex, pointer)      (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.requestBufferDataPtr = (pointer))     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* ----- response buffer macros ------------------------------------------------------------------------------------ */
#define CddDrm_GetChannelRespBufferInfoPtrVal(channelIndex, ptrIndex) (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferDataPtr[(ptrIndex)])    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelRespBufferInfoPtr(channelIndex, pointer)     (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferDataPtr = (pointer))    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelRespBufferInfoSize(channelIndex)             (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferSize)                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelRespBufferInfoSize(channelIndex, val)        (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferSize = (val))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define CddDrm_GetChannelState(channelIndex)                          (CddDrm_ChannelInfo[(channelIndex)].channelState)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelState(channelIndex, val)                     (CddDrm_ChannelInfo[(channelIndex)].channelState = (val))                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelRCRRP(channelIndex)                          (CddDrm_ChannelInfo[(channelIndex)].rcrrpCounter)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelRCRRP(channelIndex, val)                     (CddDrm_ChannelInfo[(channelIndex)].rcrrpCounter = (val))                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_DecChannelRCRRP(channelIndex)                          (CddDrm_ChannelInfo[(channelIndex)].rcrrpCounter--)                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelP2Timer(channelIndex)                        (CddDrm_ChannelInfo[(channelIndex)].P2Timer)                                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelP2Timer(channelIndex, val)                   (CddDrm_ChannelInfo[(channelIndex)].P2Timer = (val))                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_DecChannelP2Timer(channelIndex)                        (CddDrm_ChannelInfo[(channelIndex)].P2Timer--)                                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetChannelProtocolLength(channelIndex)                 (CddDrm_ChannelInfo[(channelIndex)].protocolDataLength)                              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelProtocolLength(channelIndex, val)            (CddDrm_ChannelInfo[(channelIndex)].protocolDataLength = (val))                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetConnectionId(channelIndex)                          (CddDrm_ChannelInfo[(channelIndex)].connectionId)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetConnectionId(channelIndex, val)                     (CddDrm_ChannelInfo[(channelIndex)].connectionId = (val))                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetConnectionReqPduId(channelIndex)                    (CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[(channelIndex)].connectionId))  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetConnectionResPduId(channelIndex)                    (CddDrm_Cfg_GetConnectionResPduId(CddDrm_ChannelInfo[(channelIndex)].connectionId))  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetProtocolBufferActive(channelIndex)                  (CddDrm_ChannelInfo[(channelIndex)].useProtocolBuffer)                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetProtocolBufferActive(channelIndex)                  (CddDrm_ChannelInfo[(channelIndex)].useProtocolBuffer = TRUE)                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_ResetProtocolBufferActive(channelIndex)                (CddDrm_ChannelInfo[(channelIndex)].useProtocolBuffer = FALSE)                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_IsChannelDynamicConnection(channelIndex)               (CddDrm_ChannelInfo[(channelIndex)].useDynamicConnection == TRUE)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_SetChannelDynamicConnection(channelIndex)              (CddDrm_ChannelInfo[(channelIndex)].useDynamicConnection = TRUE)                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_ResetChannelDynamicConnection(channelIndex)            (CddDrm_ChannelInfo[(channelIndex)].useDynamicConnection = FALSE)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define CddDrm_IsChannelValid(channelIndex)                           ((channelIndex) < CDDDRM_CFG_CHANNEL_COUNT)                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_ActiveChannelExist()                                   (CddDrm_ActiveChannels > 0U)                                                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_IsChannelActive(channelId)                             ((CddDrm_GetChannelState(channelId) > CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
# define CddDrm_GetFuncRespBufferPtr(Idx)                             (&CddDrm_FuncReq_RespBuffer[(Idx)].responseBuffer[0])                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define CddDrm_IsFunctionalConnection(connectionId)                  ((connectionId) >= CDDDRM_CFG_PHYS_CONNECTION_COUNT)                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define CddDrm_GetTesterPresentChannelAssignIdx(connectionId)        (connectionId + CDDDRM_CFG_FUNC_CONNECTION_COUNT)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define CddDrm_GetFunctionalDelayRequestIdx(connectionId)            (connectionId - CDDDRM_CFG_PHYS_CONNECTION_COUNT)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define CddDrm_StateMgrIsFuncReqActive()                             (CddDrm_State == CDDDRM_STATE_FUNCTIONAL_REQUEST_ACTIVE)                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
# define CddDrm_SetChannelRequestMetaData(channelIndex, pointer)      (CddDrm_ChannelInfo[(channelIndex)].requestMetaDataPtr = (pointer))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define CddDrm_GetChannelRequestMetaData(channelIndex, ptrIdx)       (CddDrm_ChannelInfo[(channelIndex)].requestMetaDataPtr[(ptrIdx)])             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define CddDrm_SetChannelResponseMetaData(channelIndex, pointer)     (CddDrm_ChannelInfo[(channelIndex)].responseMetaDataPtr = (pointer))          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define CddDrm_GetChannelResponseMetaData(channelIndex, ptrIdx)      (CddDrm_ChannelInfo[(channelIndex)].responseMetaDataPtr[(ptrIdx)])            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define Cdddrm_SetChannelRxPduId(channelIndex, id)                   (CddDrm_ChannelInfo[(channelIndex)].rxPduId = (id))                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Cdddrm_GetChannelRxPduId(channelIndex)                       (CddDrm_ChannelInfo[(channelIndex)].rxPduId)                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

/* ----- decompose a 32 bit (value) into a byte stream ------------------------------------------------------------- */
#define CddDrm_GetHiHiByte(value)                                     ((uint8)((uint32)(value)>>24))                                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetHiLoByte(value)                                     ((uint8)((uint32)(value)>>16))                                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetLoHiByte(value)                                     ((uint8)((uint32)(value)>>8))                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetLoLoByte(value)                                     ((uint8)(value))                                                              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* ----- decompose a 16 bit (value) into a byte stream ------------------------------------------------------------- */
#define CddDrm_GetHiByte(value)                                       ((uint8)((uint16)(value)>>8))                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CddDrm_GetLoByte(value)                                       ((uint8)((value)))                                                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* ----- Suppress positive response macro functions ---------------------------------------------------------------- */
#define CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(value)                   ((uint8)((value) | (0x80U)))                                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(value)                 ((uint8)((value)& ~(0x80U)))                                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* ----- Access Macros for ECU Detection --------------------------------------------------------------------------- */
/* The 2bit ECU detection result is stored in a byte array, where each byte contains the result for 4 connections.
 * CDDDRM_ECUD_GET_CONNECTION_INDEX returns the index in the array for a connection.
 * CDDDRM_ECUD_GET_CONNECTION_POS returns the bit position within on byte of the array for the detection result of a connection. */
#define CDDDRM_ECUD_GET_CONNECTION_INDEX(connectionId)                ((connectionId) / 4U)                                                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/* get position of relevant two bits in a byte */
#define CDDDRM_ECUD_GET_CONNECTION_POS(connectionId)                  (((connectionId) % 4U) * 2U)                                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CDDDRM_ECUD_SET_CONNECTION_NOT_AVAILABLE(var, connectionId)   ((CddDrm_EcudStateType)((var) | (CDDDRM_ECUD_CONNECTION_NOT_AVAILABLE << CDDDRM_ECUD_GET_CONNECTION_POS(connectionId)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define CDDDRM_ECUD_SET_CONNECTION_AVAILABLE(var, connectionId)       ((CddDrm_EcudStateType)((var) | (CDDDRM_ECUD_CONNECTION_AVAILABLE << CDDDRM_ECUD_GET_CONNECTION_POS(connectionId))))     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define CddDrm_Ecud_GetRamBlockInitPattern()                          (CddDrm_Cfg_EcuDetectionData.InitPattern)
#define CddDrm_Ecud_SetRamBlockInitPattern(val)                       (CddDrm_Cfg_EcuDetectionData.InitPattern = (val))                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 * INCOMPATIBLE FEATURE CHECK
 *********************************************************************************************************************/
/* separation time & request delay */
#if ((CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON) && (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON))
# error "Global delay request and connection specific delay time cannot be enabled at the same time"
#endif
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Connection channel id iteration type */
typedef uint8_least CddDrm_ChannelIdIterType;

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
# if ((CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON))
typedef struct
{
  CddDrm_ChannelIdType      channelIdToClose;
  CddDrm_ChannelStateType   lastChannelState;
}CddDrm_CancelChannelsStructType;

typedef struct
{
#  if(CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  CddDrm_CancelChannelsStructType   cancelChannels[CDDDRM_CFG_CHANNEL_COUNT];
#  else
  CddDrm_CancelChannelsStructType   cancelChannels[1U];
#  endif
  CddDrm_ChannelIdType              lnoOfChannelsToClose;
}CddDrm_CancellationStructType;
# endif
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define CDDDRM_START_SEC_VAR_ZERO_INIT_8BIT
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_ZERO_INIT) CddDrm_ModuleInitialized = CDDDRM_UNINIT;

#define CDDDRM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CDDDRM_START_SEC_VAR_NOINIT_8BIT
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Drm state */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_State;

/*! Indicate that external tester was connected/detected */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Queue_TesterConnected;

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/*! Connection Scan State */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Ecud_State;
/*! Indicates that the application want to change the ECU Detection state */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Queue_Ecud;
/*! Current processing connection index */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Ecud_ConnectionIterationIndex;
#endif

/*! Counter for active connection channels */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_ActiveChannels;

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/*! This was used for processing connections in CDDDRM_CHANNEL_STATE_WAIT_DELAY state, to prevent starving of connections */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_NextConnection;
#endif

#define CDDDRM_STOP_SEC_VAR_NOINIT_8BIT
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CDDDRM_START_SEC_VAR_NOINIT_16BIT
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/*! Timer for delay between requests, loaded to zero at startup */
CDDDRM_LOCAL VAR(CddDrm_TimerType, CDDDRM_VAR_NOINIT) CddDrm_RequestDelayTimer;
#endif

#define CDDDRM_STOP_SEC_VAR_NOINIT_16BIT
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CDDDRM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Dynamic mapping: connection id to channel index */
CDDDRM_LOCAL VAR(CddDrm_ChannelIdType, CDDDRM_VAR_NOINIT) CddDrm_ChannelAssign[CDDDRM_CFG_CONNECTION_COUNT + CDDDRM_CFG_FUNC_CONNECTION_COUNT];

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/*! ECU Detection response buffer with configurable size */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Ecud_ResponseBuffer[CDDDRM_CFG_ECUD_RESPONSE_BUFFER_SIZE];                                                /* PRQA S 3218 */ /* MD_CDDDRM_Rule8.9_3218 */
#endif

#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
/* Separation timer for each connection */
CDDDRM_LOCAL VAR(CddDrm_SeparationTimeOfConnectionTableType, CDDDRM_VAR_NOINIT) CddDrm_SeparationTimer[CDDDRM_CFG_CONNECTION_COUNT];
#endif

#if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
/* Separation timer for each functional connection */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_FuncRequestTimer[CDDDRM_CFG_FUNC_CONNECTION_COUNT];
#endif

/* Semaphores for concurrent closing */
CDDDRM_LOCAL VAR(boolean, CDDDRM_VAR_NOINIT) CddDrm_Closing_Semaphore[CDDDRM_CFG_CHANNEL_COUNT];

#if(CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/*! Flag indicating that functional TesterPresent(3E) request with SPRMIB is being processed. */
CDDDRM_LOCAL VAR(boolean, CDDDRM_VAR_NOINIT) CddDrm_FuncReq_TesterPresent[CDDDRM_CFG_FUNC_CONNECTION_COUNT];

/*! Response buffer for responses to a functional request */
CDDDRM_LOCAL VAR(CddDrm_FuncReqRespBufferType, CDDDRM_VAR_NOINIT) CddDrm_FuncReq_RespBuffer[CDDDRM_CFG_FUNC_RESP_BUFFER_NUM];

/* Flag for functional connection request processing */
CDDDRM_LOCAL VAR(boolean, CDDDRM_VAR_NOINIT) CddDrm_FuncRequestProcessing[CDDDRM_CFG_FUNC_CONNECTION_COUNT];
#endif

/*! Struct array with information about Connections */
CDDDRM_LOCAL VAR(CddDrm_ChannelStructType, CDDDRM_VAR_NOINIT) CddDrm_ChannelInfo[CDDDRM_CFG_CHANNEL_COUNT];

#define CDDDRM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

# define CDDDRM_FUNCTIONAL_INVALIDRESPONSEIDX                        (0xFFU)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CDDDRM_START_SEC_CODE
#include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */


 /**********************************************************************************************************************
 CddDrm_ReleaseSemaphore()
 *********************************************************************************************************************/
 /*! \brief    Release semaphore for the channel
 *  \details   Releases the semaphore for the channel id
 *  \param[in] channelId        Channel Id 
 *  \pre       Channel is valid
 *  \context   TASK|ISR2
 *  \reentrant TRUE for different channel IDs
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_ReleaseSemaphore(CddDrm_ChannelIdType channelId);

/**********************************************************************************************************************
CddDrm_AllocateSemaphore()
*********************************************************************************************************************/
/*! \brief    Allocate semaphore for the channel
*  \details   Allocating semaphore if the state of the channel is equal to the expected state
*  \param[in] channelId        Channel Id
*  \param[in] expectedState    Expected state of the channel
*  \pre       Channel is valid
*  \context   TASK|ISR2
*  \reentrant TRUE for different channel IDs
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(boolean, CDDDRM_CODE) CddDrm_AllocateSemaphore(CddDrm_ChannelIdType channelId,
                                                                 CddDrm_ChannelStateType expectedState);

/**********************************************************************************************************************
 CddDrm_LocalMemCopy()
*********************************************************************************************************************/
/*! \brief     Copy memory data
 *  \details   Copies the data from source memory location to the destination with the given length
 *  \param[in] destination  Destination memory location
 *  \param[in] source       Source memory location
 *  \param[in] length       Length of the memory to be copied
 *  \spec
 *    requires $lengthOf(destination) >= length;
 *  \endspec
 *  \pre       -
 *  \context   TASK|ISR
 *  \reentrant TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_LocalMemCopy(P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) destination, P2CONST(uint8, AUTOMATIC, CDDDRM_APPL_DATA) source, uint32 length);

#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
/**********************************************************************************************************************
 CddDrm_LocalMemCmp()
*********************************************************************************************************************/
/*! \brief    Compares memory data
 *  \details   Compares the data from two memory locations for the given length
 *  \param[in] source1  First source memory location
 *  \param[in] source2  Second source memory location
 *  \param[in] length   Length of the memory to be compared
 *  \pre       -
 *  \context   TASK|ISR
 *  \reentrant TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_LocalMemCmp(P2CONST(uint8, AUTOMATIC, CDDDRM_APPL_DATA) source1,
                                                                  P2CONST(uint8, AUTOMATIC, CDDDRM_APPL_DATA) source2,
                                                                  uint8 length);
#endif

/**********************************************************************************************************************
  CddDrm_RequestClosingOfAllChannels()
 *********************************************************************************************************************/
/*! \brief     Sets all open channels to state CLOSE.
 *  \details   This function sets all open channels to state CLOSE.
 *  \pre       Shall only be called if external tester is connected.
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_RequestClosingOfAllChannels(void);

/**********************************************************************************************************************
  CddDrm_Timer_Task()
 *********************************************************************************************************************/
/*! \brief     Handle channel timer
 *  \details   Handle DRM channel delay between requests and channel P2/P2* timeouts
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_Task(void);

#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
/**********************************************************************************************************************
  CddDrm_Timer_ConnectionDelay()
 *********************************************************************************************************************/
/*! \brief     Timer processing for the connection specific delay time
 *  \details   -
 *  \param[in] connectionId    unique handle of the connection
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_ConnectionDelay(CddDrm_ConnectionTableIterType connectionId);
#endif

/**********************************************************************************************************************
  CddDrm_Timer_P2()
 *********************************************************************************************************************/
/*! \brief     Timer processing for the P2 and P2 extended time
 *  \details   -
 *  \param[in] connectionId    unique handle of the connection
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_P2(CddDrm_ConnectionTableIterType connectionId);

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/**********************************************************************************************************************
  CddDrm_Timer_GlobalDelay()
 *********************************************************************************************************************/
/*! \brief     Timer processing for the global delay time
 *  \details   -
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_GlobalDelay(void);
#endif

#if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
/**********************************************************************************************************************
  CddDrm_Timer_FunctionalRequestDelay()
 *********************************************************************************************************************/
 /*! \brief     Timer processing for the functional connection request delay time
  *  \details   -
  *  \pre       -
  *  \context   TASK
  *  \reentrant FALSE
  **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_FunctionalRequestDelay(void);
#endif

#if((CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON) || (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON))
/**********************************************************************************************************************
  CddDrm_Timer_ReloadP2Timer()
 *********************************************************************************************************************/
/*! \brief     Timer processing for the P2 timer
 *  \details   -
 *  \param[in] channelId    The channel to reload P2 timer
 *  \param[in] rcrrp        TRUE    If RCRRP is received
 *                          FALSE   If RCRRP is not received
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_ReloadP2Timer(CddDrm_ChannelIdType channelId,
                                                                boolean              rcrrp);
#endif

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
  CddDrm_GetFreeFuncRespBuffer()
 *********************************************************************************************************************/
 /*! \brief     Retrieve a free functional response buffer
  *  \details   Iterate through functional response buffers and return the available buffer
  *  \pre       Functional reqeust is supported
  *  \context   TASK
  *  \reentrant TRUE
  *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(CddDrm_BufferIdxType, CDDDRM_CODE) CddDrm_GetFreeFuncRespBuffer(void);

/**********************************************************************************************************************
  CddDrm_GetFuncRespBufferIdx()
 *********************************************************************************************************************/
 /*! \brief     Retrieve the functional response buffer used for the connectionId
  *  \details   Iterate through functional response buffer and return the buffer used for the connection
  *  \pre       Functional reqeust is supported
  *  \context   TASK
  *  \reentrant TRUE
  *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(CddDrm_BufferIdxType, CDDDRM_CODE) CddDrm_GetFuncRespBufferIdx(CddDrm_ConnectionIdType connectionId);
#endif

/**********************************************************************************************************************
  CddDrm_Cm_StateTask()
 *********************************************************************************************************************/
/*! \brief     Process open connections
 *  \details   Open channel state will be handled, excluded timeouts
 *  \pre       Minimum one connection is active
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_StateTask(void);

#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
/**********************************************************************************************************************
  CddDrm_Cm_ProcessRCRRP()
 *********************************************************************************************************************/
/*! \brief       Processing RCRRP counter.
 *  \details     Processing RCRRP counter and returns resulting response code.
 *  \param[in]   channelId    the channel which has to be processed
 *  \return      CDDDRM_RESPONSE_RCRRP_LIMIT_REACHED   the maximum number of RCRRP were received
 *  \return      CDDDRM_RESPONSE_FORWARDED_RCRRP       RCRRP limit not reached yet.
 *  \pre         channel must be active
 *  \context     TASK
 *  \reentrant   FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ResponseCodeType, CDDDRM_CODE) CddDrm_Cm_ProcessRCRRP(CddDrm_ChannelIdType channelId);
#endif

/**********************************************************************************************************************
  CddDrm_Cm_RequestStart()
 *********************************************************************************************************************/
/*! \brief     DRM initiates the transmission to PduR.
 *  \details   This function initiates a transmission of a request.
 *  \param[in] channelId    which is allocated to a connection
 *  \pre       Channel must be in REQUEST_START state
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_RequestStart(CddDrm_ChannelIdType channelId);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelClose()
 *********************************************************************************************************************/
/*! \brief     Close channel and notify application
 *  \details   Notify application about success or failure
 *  \param[in] channelId    that will be used for application notification and closing the channel
 *  \pre       channel must be active
 *  \context   TASK
 *  \reentrant FALSE
 *  \spec
 *    requires channelId < CDDDRM_CFG_CHANNEL_COUNT;
 *  \endspec
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelClose(CddDrm_ChannelIdType channelId);

/**********************************************************************************************************************
CddDrm_HandleChannelClosing()
*********************************************************************************************************************/
/*! \brief    Close channels if it was requested.
*  \details   If channel shall be closed, this function will handle the actual closing.
*  \pre       -
*  \context   TASK
*  \reentrant FALSE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_HandleChannelClosing(void);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelInit()
 *********************************************************************************************************************/
/*! \brief     Initialize channel
 *  \details   Initialize channel to default values
 *  \param[in] channelId        Channel that is assigned to connection
 *  \param[in] serviceId        Service Id
 *  \param[in] callback         Internal reference to Notify Function
 *  \param[in] requestLength    Length of request that should send
 *  \param[in] sprmib           Indicates if positive response is expected or not
 *  \param[in] testerPresent    Channel is used for functional tester present (SPRMIB) request
 *  \param[in] dynamicChannel   Indicates if the channel is dynamic or not
 *  \param[in] bufferInfo       Information about application buffer
 *  \pre       Channel is allocated
 *  \context   TASK|ISR2
 *  \reentrant TRUE for different channel IDs
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelInit(CddDrm_ChannelIdType channelId,
                                                           uint8 serviceId,
                                                           CddDrm_ResponseNotificationPtr callback,
                                                           CddDrm_LengthType requestLength,
                                                           boolean sprmib,
                                                           boolean testerPresent,
                                                           boolean dynamicChannel,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_CONST) bufferInfo);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelGet()
 *********************************************************************************************************************/
/*! \brief      Look for free channel
 *  \details    Returns the first free channel that is found.
 *  \param[in]  connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]  channelClient                    The client that needs to allocate a channel, Application or ECU Detection
 *  \param[in]  testerPresent                    Functional TesterPresent(3E) request with SPRMIB
 *  \param[in]  serviceId                        The service id
 *  \param[out] reqReturn                        Response code to be reported in case of a failure
 *  \return     0-253:                           Possible channel IDs
 *  \return     CDDDRM_CHANNEL_NOT_ASSIGNED:    No channel found
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires connectionId < CDDDRM_CFG_CONNECTION_COUNT;
 *  \endspec
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_ChannelGet(
                                              CddDrm_ConnectionIdType connectionId,
                                              uint8 channelClient,
                                              boolean funcTesterPresent,
                                              uint8 serviceId,
                                              P2VAR(CddDrm_RequestReturnType, AUTOMATIC, CDDDRM_VAR_NOINIT) reqReturn);

/**********************************************************************************************************************
CddDrm_Cm_SetInitialChannelState()
*********************************************************************************************************************/
/*! \brief     Sets the initial state of a channel
*  \details    Initializes the channel to the correct state depending on wait delays.
*  \param[in]  channelId      channel which will be assigned and initialized
*  \pre        Channel must be allocated and should be initialized to obtain correct behavior if state machine is executed
*  \context    TASK|ISR2
*  \reentrant  FALSE
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_SetInitialChannelState(CddDrm_ChannelIdType channelId);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelAllocate()
 *********************************************************************************************************************/
/*! \brief       Assign a connection to a channel
 *  \details     Initialize the channel state and assign a connection to a channel.
 *  \param[in]   channelId           channel which will be assigned and initialized
 *  \param[in]   connectionId        connection related to the ECU to which the request shall be send
 *  \param[in]   funcTesterPresent   Channel will be used for a functional TesterPreset(3E) request with SPRMIB
 *  \pre         Interrupts must be disabled
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \spec
 *    requires channelId < CDDDRM_CFG_CHANNEL_COUNT;
 *    requires connectionId < CDDDRM_CFG_CONNECTION_COUNT;
 *  \endspec
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelAllocate(CddDrm_ChannelIdType channelId,
                                                                      CddDrm_ConnectionIdType connectionId,
                                                                      boolean                 funcTesterPresent);

/**********************************************************************************************************************
  CddDrm_Cm_GetApplChannel()
 *********************************************************************************************************************/
/*! \brief     Look for free application channel.
 *  \details   Returns the first free application channel that is found.
 *  \param[in] connectionId                     Connection related to the ECU to which the request shall be send
 *  \return    0-252:                           Possible channel IDs
 *  \return    CDDDRM_CHANNEL_NOT_ASSIGNED:    Invalid channel ID
 *  \pre       Interrupts must be disabled
 *  \context   TASK|ISR2
 *  \reentrant FALSE
 *  \spec
 *    requires connectionId < CDDDRM_CFG_CONNECTION_COUNT;
 *  \endspec
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetApplChannel(
                                                                                CddDrm_ConnectionIdType connectionId);


#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Cm_GetEcudChannel()
 *********************************************************************************************************************/
/*! \brief     Get ECU detection channel
 *  \details   Returns the reserved ECU detection channel.
 *  \param[in] connectionId                     Related connection
 *  \return    0:                               ECU detection channel ID
 *  \return    CDDDRM_CHANNEL_NOT_ASSIGNED:    Invalid channel ID
 *  \pre       Interrupts must be disabled
 *  \context   TASK|ISR2
 *  \reentrant FALSE
 *  \spec
 *    requires connectionId < CDDDRM_CFG_CONNECTION_COUNT;
 *  \endspec
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetEcudChannel(
                                                                                CddDrm_ConnectionIdType connectionId);
#endif

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
  CddDrm_Cm_GetFuncChannel()
 *********************************************************************************************************************/
/*! \brief     Get functional request channel
 *  \details   Returns the reserved functional request channel.
 *  \param[in] connectionId                     Related connection
 *  \param[in] funcTesterPresent                Forced functional request (TesterPresent(3E) with SPRMIB)
 *  \return    0-252:                           Possible channel IDs
 *  \return    CDDDRM_CHANNEL_NOT_ASSIGNED:     Invalid channel ID
 *  \pre       Interrupts must be disabled
 *  \context   TASK|ISR2
 *  \reentrant FALSE
 *  \spec
 *    requires connectionId < CDDDRM_CFG_CONNECTION_COUNT;
 *  \endspec
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetFuncChannel(
                                                                                  CddDrm_ConnectionIdType connectionId,
                                                                                  boolean funcTesterPresent);

/**********************************************************************************************************************
  CddDrm_Cm_GetFreeChannel()
 *********************************************************************************************************************/
/*! \brief     Look for free channel.
 *  \details   Returns the first free channel that is found.
 *  \return    0-252:                          Possible channel IDs
 *  \return    CDDDRM_CHANNEL_NOT_ASSIGNED:    Invalid channel ID
 *  \pre       Interrupts must be disabled
 *  \context   TASK|ISR2
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetFreeChannel(void);

/**********************************************************************************************************************
  CddDrm_Cm_ActiveChannelExists()
 *********************************************************************************************************************/
 /*! \brief     Check if there are any active channels.
  *  \details   TesterPresent channels are disregarded as active channels since they do not interfere with other requests.
  *  \return    TRUE:   Active channel exists
  *  \return    FALSE:  No active channel exist
  *  \pre       Interrupts must be disabled
  *  \context   TASK|ISR2
  *  \reentrant FALSE
  *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(boolean, CDDDRM_CODE) CddDrm_Cm_ActiveChannelExists(void);
#endif

#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
/**********************************************************************************************************************
  CddDrm_Cm_CheckFirewallRule()
 *********************************************************************************************************************/
/*! \brief     Check firewall rule for connection.
 *  \details   Check if service is allowed to the connection.
 *  \param[in] connectionId   connection related to the ECU to which the request shall be send
 *  \param[in] serviceId      Service Id, which has to be checked
 *  \return    TRUE:  The ServiceId is allowed for the given connection
 *             FALSE: The ServiceId is not allwoed for the given connection
 *  \pre       channel must be active
 *  \context   TASK
 *  \reentrant TRUE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(boolean, CDDDRM_CODE) CddDrm_Cm_CheckFirewallRule(CddDrm_ConnectionIdType connectionId, uint8 serviceId);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Ecud_SetupRequest()
 *********************************************************************************************************************/
/*! \brief     Setup a request
 *  \details   Allocate and initialize the current connection index
 *  \pre       External tester is not active. ECU Detection is running.
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_SetupRequest(void);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Ecud_ResponseNotification()
 *********************************************************************************************************************/
/*! \brief     Notification handler for ECU Detection
 *  \details   Drm internal response notification that is called as soon as a response for the ECU detection request
 *             was received -> Similar to the application response notification
 *  \param[in] response   contains the relevant information regarding the received response message.
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_ResponseNotification(P2CONST(CddDrm_RespInfoStructType, AUTOMATIC, CDDDRM_CONST) response);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Ecud_StateTask()
 *********************************************************************************************************************/
/*! \brief     Handle the ECU Detection
 *  \details   Automatically setup requests to connections that not been discovered.
 *  \pre       External tester is not active
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_StateTask(void);
#endif

/**********************************************************************************************************************
  CddDrm_Queue_ExternalTester()
 *********************************************************************************************************************/
/*! \brief     Handles the External Tester Queue
 *  \details   Closes open connections if an external tester is detected or set the Drm back to normal working mode.
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_ExternalTester(void);

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Queue_EcuDetection()
 *********************************************************************************************************************/
/*! \brief     Handles the ECU Detection Queue
 *  \details   Starts and stops the ECU Detection.
 *  \pre       -
 *  \context   TASK
 *  \reentrant FALSE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_EcuDetection(void);
#endif

/**********************************************************************************************************************
  CddDrm_StateMgrCheckSvcProcState()
 *********************************************************************************************************************/
/*! \brief      Checks service processing state
 *  \details    Checks if the service processing is allowed in current state.
 *  \param[in]  connectionId      Connection related to the ECU to which the request shall be send
 *  \param[in]  funcTesterPresent Force functional request (Tester Present with SPRMIB)
 *  \param[out] reqReturn         Response code to be reported in case of a failure
 *  \return     E_OK:             Service processing allowed in current state
 *  \return     E_NOT_OK:         Service processing not allowed
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StateMgrCheckSvcProcState(
                                            CddDrm_ConnectionIdType connectionId,
                                            boolean funcTesterPresent,
                                            P2VAR(CddDrm_RequestReturnType, AUTOMATIC, CDDDRM_VAR_NOINIT) reqReturn);

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
  CddDrm_FuncReqMgrInitResponseChannel()
*********************************************************************************************************************/
/*! \brief     Initiate a response to a functional request
 *  \details   Initialize a channel and its states to receive a response to a functional request.
 *  \param[in] rxPduId        Id of the TP CddPduRUpperLayerRxPdu.
 *  \param[in] responseLength  Length of the response data.
 *  \return    E_OK:          A channel to receive the response is initialized
 *  \return    E_NOT_OK:      The operation failed
 *  \pre       Interrupts must be disabled.
 *  \context   TASK|ISR2
 *  \reentrant FALSE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_FuncReqMgrInitResponseChannel(
                                                        PduIdType rxPduId,
                                                        CddDrm_LengthType responseLength);
#endif

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
  CddDrm_FuncReqMgrResetRespResult()
*********************************************************************************************************************/
/*! \brief     Reset functional request results
 *  \details   Reset the results of a functional request.
 *  \pre       -
 *  \context   TASK|ISR2
 *  \reentrant TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_FuncReqMgrResetRespResult(void);
#endif

/**********************************************************************************************************************
*  CddDrm_ProcessStartOfReception()
*********************************************************************************************************************/
/*! \brief        Processing for start of reception
*  \details       Function checks the request type and accordingly assigns channel id and process for buffer info
*  \param[in]     pduId             Identification of I-PDU
*  \param[in]     channelId         Indicates which channel that shall be used for processing
*  \param[in]     info              Pointer to a PduInfoType structure containing the metadata if available,
*                                   otherwise NULL_PTR.
*  \param[in]     TpSduLength       Total length of message to be received
*  \param[out]    bufferSizePtr     Available receive buffer in the receiving module
*  \return        BUFREQ_OK:        Connection has been accepted. Reception is continued
*                 BUFREQ_E_NOT_OK:  Connection has been rejected. Reception aborted
*                 BUFREQ_E_OVFL:    No buffer data of the required length can be provided. Reception aborted
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \spec
*    requires channelId < CDDDRM_CFG_CHANNEL_COUNT;
*  \endspec
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessStartOfReception(PduIdType pduId,
                                                  CddDrm_ChannelIdType channelId,
                                                  P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                  PduLengthType TpSduLength,
                                                  P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr);
/**********************************************************************************************************************
*  CddDrm_ProcessAllocatedChannel()
*********************************************************************************************************************/
/*! \brief        Processing for when a channel has been allocated
*  \details       Function verifies response and response buffer lengths and sets the correct channel state for\
                  further processing.
*  \param[in]     channelId             Channel Id for the Pdu that is processed.
*  \param[in]     info                  Pointer to a PduInfoType structure containing the metadata if available,
*                                       otherwise NULL_PTR.
*  \param[in]     TpSduLength           Total length of message to be received
*  \param[in]     isFunctionalResponse  Whether the response is on a functional request or not
*  \param[out]    bufferSizePtr         Available receive buffer in the receiving module
*  \return        BUFREQ_OK:            Connection has been accepted. Reception is continued
*                 BUFREQ_E_NOT_OK:      Connection has been rejected. Reception aborted
*                 BUFREQ_E_OVFL:        No buffer data of the required length can be provided. Reception aborted
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \spec
*    requires channelId < CDDDRM_CFG_CHANNEL_COUNT;
*  \endspec
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessAllocatedChannel(CddDrm_ChannelIdType channelId,
                                                    P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                    PduLengthType TpSduLength,
                                                    boolean isFunctionalResponse,
                                                    P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr);


/**********************************************************************************************************************
*  CddDrm_ProcessCopyRxData()
*********************************************************************************************************************/
/*! \brief        Copy the data for reception
*  \details       This function copies the data to the response buffer.
*  \param[in]     channelId         Channel Id for which the reception request is received
*                 info              Provides the destination buffer and number of bytes to be copied
*                 bufferSizePtr     Available response buffer after the data is copied
*  \return        BUFREQ_OK:        Data copied successfully
*                 BUFREQ_E_NOT_OK:  Data has not been copied because of an error
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessCopyRxData(CddDrm_ChannelIdType channelId,
                                                  P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                  P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
*  CddDrm_ProcessCopyTxData()
*********************************************************************************************************************/
/*! \brief        Copy the data for transmission
*  \details       This function copies the transmit data.
*  \param[in]     channelId         Channel Id for which the transmission request is to be sent
*                 info              Provides the destination buffer and number of bytes to be copied
*                 availableDataPtr  Available remaining number of bytes in the Tx buffer
*  \return        BUFREQ_OK:        Data copied successfully. Request completed
*                 BUFREQ_E_NOT_OK:  Data has not been copied. Request failed
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessCopyTxData(CddDrm_ChannelIdType channelId,
                                               P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                               P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) availableDataPtr);

/**********************************************************************************************************************
*  CddDrm_ProcessTpRxIndication()
*********************************************************************************************************************/
/*! \brief        Handling of the result for RxIndication
*  \details       Function checks for reception result and processes accordingly
*  \param[in]     channelId       Channel Id for which the RxIndicationis received
*                 result          Result of the reception
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_ProcessTpRxIndication(CddDrm_ChannelIdType channelId,
                                                                         Std_ReturnType result);



/**********************************************************************************************************************
*  CddDrm_HandleNegativeResponse()
*********************************************************************************************************************/
/*! \brief        Handling of the received negative response
*  \details       Derives the response code and returns it.
*  \param[in]     channelId             Channel Id for which the response id is received
*  \return        ResponseCode:         Returns the NRC of the response
*  \pre           This function shall only be called when channel state is CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ResponseCodeType, CDDDRM_CODE) CddDrm_HandleNegativeResponse(CddDrm_ChannelIdType channelId);

/**********************************************************************************************************************
*  CddDrm_ProcessTpTxConfirmation()
*********************************************************************************************************************/
/*! \brief        Handling of the result received for TxConfirmation
*  \details       Function checks for transmission result and processes accordingly
*  \param[in]     channelId       Channel Id for which the transmission confirmation is received
*                 result          Result of the transmission of the I-PDU
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_ProcessTpTxConfirmation(CddDrm_ChannelIdType channelId,
                                                                           Std_ReturnType result);
#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
# if ((CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON))
/**********************************************************************************************************************
*  CddDrm_InitializeCancellationStruct()
*********************************************************************************************************************/
/*! \brief        Initialization for the structure containing data for cancellation
*  \details       Function initializes structure containing the cancellations to be invoked
*  \param[in]     cancellationsToInvoke     List of cancellations to invoke
*                 channelCount              Amount of channels to close
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_InitializeCancellationStruct(
                                      P2VAR(CddDrm_CancellationStructType, AUTOMATIC, AUTOMATIC) cancellationsToInvoke,
                                      CddDrm_ChannelIdType channelCount);

/**********************************************************************************************************************
*  CddDrm_AddToCancellationList()
*********************************************************************************************************************/
/*! \brief        Sets a new cancel in list
*  \details       This function adds a new channel to be canceled and increments the counter of channels to be closed.
*  \param[in]     channelId
*                 cancellationsToInvoke   List of cancellations to invoke
*  \pre           -
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_AddToCancellationList(
                                CddDrm_ChannelIdType channelId,
                                P2VAR(CddDrm_CancellationStructType, AUTOMATIC, AUTOMATIC) cancellationsToInvoke);
# endif
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
# if ((CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON))
/**********************************************************************************************************************
*  CddDrm_CancellationToLowerLayer()
*********************************************************************************************************************/
/*! \brief        Invoke Cancellation to lower layer.
*  \details       Invokes Cancel Receive and/or Cancel Transmit to lower layer
*  \param[in]     cancellationToInvoke  Struct containing no of channels to close and a list of cancellations.
*  \pre           Size of noOfChannelsToClose must not be higher than the size of ChannelsToClose.
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_CancellationToLowerLayer(
                                    P2CONST(CddDrm_CancellationStructType, AUTOMATIC, AUTOMATIC) cancellationToInvoke);
# endif
#endif

#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetChannelIdFromMetaData()
*********************************************************************************************************************/
/*! \brief        Retrieve the channelId which has the requested metadata stored
 *  \details       Iterates through all channels and retrieves the channel with the requested metadata
 *  \param[in]     pduInfo    Contains the metadata
 *  \return        ChannelId: Returns the channelId
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_GetChannelIdFromMetaData(P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) pduInfo);

/**********************************************************************************************************************
 *  CddDrm_GetChannelIdFromRxPduId()
*********************************************************************************************************************/
/*! \brief        Retrieve the channelId which has the requested RxPduId stored
 *  \details       Iterates through all channels and retrieves the channel with the requested RxPduId
 *  \param[in]     rxPduId    requested Id
 *  \return        ChannelId: Returns the channelId
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_GetChannelIdFromRxPduId(PduIdType rxPduId);
#endif /* CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
 /**********************************************************************************************************************
 * CddDrm_ReleaseSemaphore()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_ReleaseSemaphore(CddDrm_ChannelIdType channelId)
{
  CddDrm_Closing_Semaphore[channelId] = FALSE;
}

/**********************************************************************************************************************
* CddDrm_AllocateSemaphore()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
CDDDRM_LOCAL FUNC(boolean, CDDDRM_CODE) CddDrm_AllocateSemaphore(CddDrm_ChannelIdType channelId, CddDrm_ChannelStateType expectedState)
{
  boolean lAllocationSuccessful = FALSE;

  /* #10 Allocate semaphore if the state is equal to the expected state and the semaphore was not allocated before. */
  CddDrm_EnterCritical_ChannelManager();
  if ((CddDrm_GetChannelState(channelId) == expectedState)
    && (CddDrm_Closing_Semaphore[channelId] == FALSE))                                                                                               /* COV_CDDDRM_ROBUSTNESS */
  {
    CddDrm_Closing_Semaphore[channelId] = TRUE;
    lAllocationSuccessful = TRUE;
  }
  CddDrm_LeaveCritical_ChannelManager();

  return lAllocationSuccessful;
}

/**********************************************************************************************************************
* CddDrm_LocalMemCopy()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_LocalMemCopy(P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) destination,
                                                         P2CONST(uint8, AUTOMATIC, CDDDRM_APPL_DATA) source,
                                                         uint32 length)
{
  uint32_least i;

  for (i = 0; i < length; i++)
  {
    /* #10 Copy data bytewise from source to destination */
    destination[i] = source[i];                                                                                                                      /* VCA_CDDDRM_MEMCPY_WRITE_DEST */
  }
}

#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
/**********************************************************************************************************************
* CddDrm_LocalMemCmp()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
CDDDRM_LOCAL FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_LocalMemCmp(P2CONST(uint8, AUTOMATIC, CDDDRM_APPL_DATA) source1,
                                                                  P2CONST(uint8, AUTOMATIC, CDDDRM_APPL_DATA) source2,
                                                                  uint8 length)
{
  Std_ReturnType retVal = E_OK;
  uint8_least i;

  for (i = 0; i < length; i++)
  {
    /* #10 compare data bytewise in the two memory locations */
    if (source1[i] != source2[i])
    {
      retVal = E_NOT_OK;
      break;
    }
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
* CddDrm_RequestClosingOfAllChannels()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_RequestClosingOfAllChannels(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If at least one channel active */
  if (CddDrm_ActiveChannelExist())
  {
    CddDrm_ChannelIdType lChannelId;
    /* #20 Close all active channels by iterating over all connections. As of the application needs to be notified
    *     with the reason of the forced channel closing the response code is also set here. */

    /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
    CddDrm_EnterCritical_ChannelManager();
    for (lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
    {
      if (CddDrm_IsChannelActive(lChannelId))
      {
        CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL);
        CddDrm_SetChannelResponseCode(lChannelId, CDDDRM_RESPONSE_TESTER_DETECTED);
      }
    }
    /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
    CddDrm_LeaveCritical_ChannelManager();
  }
}

/**********************************************************************************************************************
 * CddDrm_Timer_Task()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_Task(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ConnectionTableIterType lConnectionIndex;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate through all configured connections */
  for (lConnectionIndex = 0; lConnectionIndex < CDDDRM_CFG_CONNECTION_COUNT; lConnectionIndex++)
  {
#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
    CddDrm_Timer_ConnectionDelay(lConnectionIndex);
#endif
    CddDrm_Timer_P2(lConnectionIndex);
  } /* End Loop */

#if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
  CddDrm_Timer_FunctionalRequestDelay();
#endif

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
  CddDrm_Timer_GlobalDelay();
#endif
}

#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Timer_ConnectionDelay()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_ConnectionDelay(CddDrm_ConnectionTableIterType connectionId)
{
  CddDrm_ChannelIdType lAssignedChannel = CddDrm_ChannelAssign[connectionId];

  /* Process connection specific delay timer */
  if (CddDrm_SeparationTimer[connectionId] > 0U)
  {
    CddDrm_SeparationTimer[connectionId]--;
  }

  /* Update channel state of connections that wait for processing */
  if (lAssignedChannel == CDDDRM_CHANNEL_NOT_ASSIGNED)
  {
    /* no channel assigned to this connection */
  }
  else
  {
    /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
    CddDrm_EnterCritical_ChannelManager();
    if (CddDrm_GetChannelState(lAssignedChannel) == CDDDRM_CHANNEL_STATE_WAIT_DELAY)
    {
# if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
      if (!CddDrm_IsFunctionalConnection(connectionId))
# endif
      {
        if (CddDrm_SeparationTimer[connectionId] == 0U)
        {
          /* Set the next state, so connection can send the request */
          CddDrm_SetChannelState(lAssignedChannel, CDDDRM_CHANNEL_STATE_REQUEST_START);                                                                /* VCA_CDDDRM_WRITE_CHANNEL_STATE */
        }
      }
    }
    /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
    CddDrm_LeaveCritical_ChannelManager();
  }
}
#endif

/**********************************************************************************************************************
 * CddDrm_Timer_P2()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_P2(CddDrm_ConnectionTableIterType connectionId)
{
  CddDrm_ChannelIdType lChannelId = CddDrm_ChannelAssign[connectionId];

  /* Process channel specific P2 and P2* timeout */
  if(CddDrm_IsChannelValid(lChannelId))
  {
    /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
    CddDrm_EnterCritical_ChannelManager();

    /* Check channel for open connection and channel state, to decrease timeout counter */
    if (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_WAIT_RESPONSE)
    {
      /* no need to check for timeout > 0 as of in state wait response it is always greater than 0 */
      /* Timeout not elapsed */
      CddDrm_DecChannelP2Timer(lChannelId);

      /* If timeout has exceeded: set channel state to CLOSE */
      if (CddDrm_GetChannelP2Timer(lChannelId) == 0U)
      {
        /* Timeout elapsed, notify application and set channel state to CLOSE */
        if ((CddDrm_GetChannelSPRMIB(lChannelId) == TRUE)
#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
          && (CddDrm_GetChannelRCRRP(lChannelId) == CddDrm_Cfg_GetConnectionRCRRP(CddDrm_GetConnectionId(lChannelId)))
#endif
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
          && (!CddDrm_IsFunctionalConnection(connectionId))
#endif
          )
        { /* If SPRMIB bit is set no response is expected within P2 time. As of RCRRP counter has still its initial value the P2 timer has exceeded and not the P2 extended timer  */
          /* No need to set the positive response code as it was set during channel initialization */
        }
        else
        {
          CddDrm_SetChannelResponseCode(lChannelId, CDDDRM_RESPONSE_TIMEOUT);
        }
        CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL);
      }
    }
    /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
    CddDrm_LeaveCritical_ChannelManager();
  }
}

#if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Timer_FunctionalRequestDelay()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_FunctionalRequestDelay(void)
{
  CddDrm_ConnectionIdType lConnectionId;

  for (lConnectionId = 0; lConnectionId < CDDDRM_CFG_FUNC_CONNECTION_COUNT; lConnectionId++)
  {
    if (CddDrm_FuncRequestTimer[lConnectionId] > 0u)
    {
      CddDrm_FuncRequestTimer[lConnectionId]--;

      if (CddDrm_FuncRequestTimer[lConnectionId] == 0u)
      {
        CddDrm_ChannelIdType lFunctionalChannelId = CddDrm_ChannelAssign[lConnectionId + CDDDRM_CFG_PHYS_CONNECTION_COUNT];

        /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
        CddDrm_EnterCritical_ChannelManager();

        if (CddDrm_IsChannelValid(lFunctionalChannelId))
        {
          if (CddDrm_GetChannelState(lFunctionalChannelId) == CDDDRM_CHANNEL_STATE_WAIT_DELAY)
          {
            CddDrm_SetChannelState(lFunctionalChannelId, CDDDRM_CHANNEL_STATE_REQUEST_START);
          }
        }
        /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
        CddDrm_LeaveCritical_ChannelManager();
      }
    }
  }
}
#endif

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Timer_GlobalDelay()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_GlobalDelay(void)
{
  CddDrm_ConnectionTableIterType lConnectionIndex;

  /* Delay be reloaded when a request was sent */
  if (CddDrm_RequestDelayTimer > 0U)
  {
    /* Decrease global delay timer */
    CddDrm_RequestDelayTimer--;
    if (CddDrm_RequestDelayTimer == 0U)
    {
      if (CddDrm_ActiveChannelExist())
      {
        for (lConnectionIndex = 0; lConnectionIndex <= CDDDRM_CFG_CONNECTION_COUNT; lConnectionIndex++)
        {
          CddDrm_ChannelIdType lChannelId = CddDrm_ChannelAssign[CddDrm_NextConnection];
          if (CddDrm_IsChannelValid(lChannelId))
          {
            boolean foundNextConnection = FALSE;
            /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
            CddDrm_EnterCritical_ChannelManager();
            if (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_WAIT_DELAY)
            {
              /* Update channel state of connections that wait for processing */
              CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_REQUEST_START);
              foundNextConnection = TRUE;
            }
            /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
            CddDrm_LeaveCritical_ChannelManager();
            if (foundNextConnection)
            {
              break;
            }
          }
          CddDrm_NextConnection = (CddDrm_NextConnection + 1U) % CDDDRM_CFG_CONNECTION_COUNT;
        }
      }
    }
  }
}
#endif

#if((CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON) || (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON))
/**********************************************************************************************************************
 * CddDrm_Timer_ReloadP2Timer()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_ReloadP2Timer(CddDrm_ChannelIdType channelId,
                                                                boolean              rcrrp)
{

# if(CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  if (CddDrm_StateMgrIsFuncReqActive())
  {
    CddDrm_TimerType timerValue;
    CddDrm_ChannelIdIterType lChannelId;

    if (rcrrp == TRUE)
    {
      timerValue = CddDrm_Cfg_GetConnectionP2ExtendedTime(CddDrm_GetConnectionId(CDDDRM_CHANNEL_FUNCTIONAL_INDEX));
    }
    else
    {
      timerValue = CddDrm_Cfg_GetConnectionP2Time(CddDrm_GetConnectionId(CDDDRM_CHANNEL_FUNCTIONAL_INDEX));

      for (lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
      {
        if ((lChannelId != channelId)
          && CddDrm_IsChannelActive(lChannelId)
          && (CddDrm_GetChannelRCRRP(lChannelId) != CddDrm_Cfg_GetConnectionRCRRP(CddDrm_GetConnectionId(lChannelId))))
        {
          /* A RCRRP message received and channel is still active: use P2 extended timer value */
          timerValue = CddDrm_Cfg_GetConnectionP2ExtendedTime(CddDrm_GetConnectionId(CDDDRM_CHANNEL_FUNCTIONAL_INDEX));
          break;
        }
      }
    }

    /* Reload the timers of all active channels */
    for (lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
    {
      if (CddDrm_IsChannelActive(lChannelId))
      {
        CddDrm_SetChannelP2Timer(lChannelId, timerValue);
      }
    }
  }
  else
# endif
  {
    CddDrm_SetChannelP2Timer(channelId, CddDrm_Cfg_GetConnectionP2ExtendedTime(CddDrm_GetConnectionId(channelId)));
  }

# if(CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_OFF)
  CDDDRM_DUMMY_STATEMENT(rcrrp);                                                                                                                /* PRQA S 1338, 2983, 3206 */ /* MD_MSR_DummyStmt */
# endif
}
#endif
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
* CddDrm_GetFreeFuncRespBuffer()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
CDDDRM_LOCAL FUNC(CddDrm_BufferIdxType, CDDDRM_CODE) CddDrm_GetFreeFuncRespBuffer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_BufferIdxType lBufferIdx = CDDDRM_FUNCTIONAL_INVALIDRESPONSEIDX;
  uint8_least lIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Iterate through ConnectionId field of Request Response Buffer */
  for (lIndex = 0; lIndex < CDDDRM_CFG_FUNC_RESP_BUFFER_NUM; lIndex++)                                                                                  /* COV_CDDDRM_FUNCRESPONSEBUFFER */
  {
    /* Find an entry with invalid ConnectionId */
    if ((CddDrm_FuncReq_RespBuffer[lIndex].connectionId == 0xFFu) || (CddDrm_FuncReq_RespBuffer[lIndex].responseProvided == TRUE))
    {
      lBufferIdx = (CddDrm_BufferIdxType)lIndex;
      break;
    }
  }

  /* Return the buffer index */
  return lBufferIdx;
}

/**********************************************************************************************************************
* CddDrm_GetFuncRespBufferIdx()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
CDDDRM_LOCAL FUNC(CddDrm_BufferIdxType, CDDDRM_CODE) CddDrm_GetFuncRespBufferIdx(CddDrm_ConnectionIdType connectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_BufferIdxType lBufferIdx = CDDDRM_FUNCTIONAL_INVALIDRESPONSEIDX;
  uint8_least lIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Iterate through ConnectionId field of Request Response Buffer */
  for (lIndex = 0; lIndex < CDDDRM_CFG_FUNC_RESP_BUFFER_NUM; lIndex++)                                                                                  /* COV_CDDDRM_FUNCRESPONSEBUFFER */
  {
    /* Find an entry with invalid ConnectionId */
    if (CddDrm_FuncReq_RespBuffer[lIndex].connectionId == connectionId)
    {
      lBufferIdx = (CddDrm_BufferIdxType)lIndex;
      break;
    }
  }

  /* Return the buffer index */
  return lBufferIdx;
}
#endif

/**********************************************************************************************************************
 * CddDrm_Cm_StateTask()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_StateTask(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ChannelIdIterType lChannelId;
  /* ----- Implementation ----------------------------------------------- */
  /* Iterate through all configured channels */
  for(lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
  {
    /* Process request if channel state is CDDDRM_CHANNEL_STATE_REQUEST_START */
    if (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_REQUEST_START)
    {
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
      if ((CddDrm_IsFunctionalConnection(CddDrm_GetConnectionId(lChannelId))) &&                                                                     /* COV_CDDDRM_PARALLEL_FUNCREQUEST */
          (CddDrm_FuncRequestProcessing[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(lChannelId))] == TRUE))
      { 
        /* Request shall not be sent out due to active functional request processing. */
      }
      else
#endif
      {
        CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_REQUEST_PROCESSING);
        CddDrm_Cm_RequestStart((CddDrm_ChannelIdType)lChannelId);
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
        if (CddDrm_IsFunctionalConnection(CddDrm_GetConnectionId(lChannelId)))
        {
          CddDrm_FuncRequestProcessing[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(lChannelId))] = TRUE;                              /* VCA_CDDDRM_CONNECTIONID_CHANNELINFO */
        }
#endif
      }
    }
  }
}

#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Cm_ProcessRCRRP()
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
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ResponseCodeType, CDDDRM_CODE) CddDrm_Cm_ProcessRCRRP(CddDrm_ChannelIdType channelId)
{
  CddDrm_ResponseCodeType lResponseCode = CDDDRM_RESPONSE_RCRRP_LIMIT_REACHED;
# if (CDDDRM_CFG_SUPPORT_FORWARD_RCRRP == STD_ON)
  CddDrm_RespInfoStructType lNotificationInfo;
  CddDrm_ResponseNotificationPtr lNotificationFunctionPtr = CddDrm_ChannelInfo[channelId].notificationFunction;
# endif

  /* #10 If RCRRP limit not reached */
  if (CddDrm_GetChannelRCRRP(channelId) > 0U)
  {
    /* #20 If forwarding supported for RCRRP, forward the notification to the application */
# if (CDDDRM_CFG_SUPPORT_FORWARD_RCRRP == STD_ON)
    lNotificationInfo.connectionId = CddDrm_GetConnectionId(channelId);
    lNotificationInfo.responseCode = CDDDRM_RESPONSE_FORWARDED_RCRRP;
    lNotificationInfo.responseLength = 0U;
    lNotificationInfo.serviceId = CddDrm_GetChannelServiceId(channelId);

    lNotificationFunctionPtr(&lNotificationInfo);                                                                                                    /* VCA_CDDDRM_NOTIFICATIONFUNCPTR */
# endif

    /* #30 Decrement counter\
           Reload connection timer\
           Reset the channel message index\
           Set the response code */
    CddDrm_DecChannelRCRRP(channelId);
    CddDrm_Timer_ReloadP2Timer(channelId, TRUE);
    CddDrm_SetChannelCurrentMessageIndex(channelId, 0U);
    lResponseCode = CDDDRM_RESPONSE_FORWARDED_RCRRP;
  }

  return lResponseCode;
}
#endif

/**********************************************************************************************************************
 * CddDrm_Cm_RequestStart()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_RequestStart(CddDrm_ChannelIdType channelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  lStdResult;
  PduInfoType     lPduInfo;

#if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT)
  boolean lCancelTransmitNeeded = FALSE;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  /* #10 If request shall be sent on dynamic connection, transmit data shall reference the request metadata buffer  */
  if (CddDrm_IsChannelDynamicConnection(channelId))
  {
    lPduInfo.SduDataPtr = &CddDrm_GetChannelRequestMetaData(channelId, 0U);
  }
  else
#endif
  {
#if (CDDDRM_CFG_SUPPORT_DEDICATED_APIS == STD_ON)
    /* #20 Otherwise, choose if transmit data shall reference application or internal protocol data buffer */
    if (CddDrm_GetProtocolBufferActive(channelId) == TRUE)
    {
      lPduInfo.SduDataPtr = &CddDrm_ChannelInfo[channelId].protocolData[0U];
    }
    else
#endif
    {
      lPduInfo.SduDataPtr = &CddDrm_GetChannelReqBufferInfoPtrVal(channelId, 0U);
    }
  }
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  if (CddDrm_GetChannelTesterPresent(channelId) == TRUE)
  {
    CddDrm_FuncReq_TesterPresent[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(channelId))] = TRUE;                                     /* VCA_CDDDRM_CONNECTIONID_CHANNELINFO */
  }
#endif

  /* First time for this channel and no data has been copied */
  lPduInfo.SduLength = CddDrm_GetChannelMessageLength(channelId);

  /* #30 Transmit the request to PduR */
  lStdResult = PduR_CddDrmTransmit(CddDrm_GetConnectionReqPduId(channelId),                                                                          /* VCA_CDDDRM_PDURTRANSMIT_CALL */
                                   &lPduInfo);

  CddDrm_EnterCritical_ChannelManager();
  /* #40 Check if channel is not in active state (i.e. closed by an interrupt) */
  if (!CddDrm_IsChannelActive(channelId))
  {
    /* #50 If channel was set to state close and PduR_CddDrmTransmit returned OK Cancel Transmit shall be\
           invoked to Lower Layer */
#if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT)
    if ((lStdResult == E_OK)
      && (CddDrm_GetChannelState(channelId) == CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL))
    {
      lCancelTransmitNeeded = TRUE;
    }
#endif
  }
  /* #60 Otherwise, if the channel is in active state */
  else
  {
    /* #70 If the request is denied by the destination module, set response code to\
           CDDDRM_RESPONSE_PDUR_TX_ERROR and channel state to CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL */
    if(lStdResult != E_OK)
    {
      CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL);
      CddDrm_SetChannelResponseCode(channelId, CDDDRM_RESPONSE_PDUR_TX_ERROR);
    }
  }
  CddDrm_LeaveCritical_ChannelManager();

  /* #80 If cancel transmit is supported and cancel transmit flag is TRUE, cancel the transmission request */
#if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT)
  if (lCancelTransmitNeeded == TRUE)
  {
    (void)PduR_CddDrmCancelTransmit(CddDrm_GetConnectionReqPduId(channelId));
  }
#endif
}

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelClose()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelClose(CddDrm_ChannelIdType channelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ConnectionIdType lConnectionId;
  CddDrm_RespInfoStructType lNotificationInfo;
  CddDrm_ResponseNotificationPtr lNotificationFunctionPtr;
  /* ----- Implementation ----------------------------------------------- */
  lConnectionId = CddDrm_GetConnectionId(channelId);

  /* #10 Prepare application notification data */
  lNotificationFunctionPtr = CddDrm_ChannelInfo[channelId].notificationFunction;
  lNotificationInfo.connectionId = lConnectionId;
  lNotificationInfo.responseCode = CddDrm_GetChannelResponseCode(channelId);
  lNotificationInfo.serviceId = CddDrm_GetChannelServiceId(channelId);
  if (CddDrm_GetChannelResponseCode(channelId) == CDDDRM_RESPONSE_POSITIVE)
  {
    lNotificationInfo.responseLength = CddDrm_GetChannelMessageLength(channelId);
  }
  else
  {
    lNotificationInfo.responseLength = 0;
  }
  /*@ assert lConnectionId < CDDDRM_CFG_CONNECTION_COUNT; */ /* VCA_CDDDRM_CONNECTIONID_CHANNELINFO */
#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
  /* #20 If Separation timer is used, reload it */
  CddDrm_SeparationTimer[lConnectionId] = CddDrm_Cfg_GetConnectionSeparationTime(lConnectionId);
#endif

  /* Prevent that CddDrm_State is overriden */
  /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
  CddDrm_EnterCritical_ChannelManager();
  /* #30 Close the channel and update active channel counter */
  CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_INACTIVE);
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  if (CddDrm_GetChannelTesterPresent(channelId) == TRUE)
  {
    CddDrm_ChannelAssign[CddDrm_GetTesterPresentChannelAssignIdx(lConnectionId)] = CDDDRM_CHANNEL_NOT_ASSIGNED;
  }
  else
#endif
  {
    CddDrm_ChannelAssign[lConnectionId] = CDDDRM_CHANNEL_NOT_ASSIGNED;
  }
  CddDrm_ActiveChannels--;

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)

  /* #40 If forced functional request active and connection is functional, set forced functional request state to FALSE */
  if (CddDrm_GetChannelTesterPresent(channelId) == TRUE)
  {
    CddDrm_FuncReq_TesterPresent[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(channelId))] = FALSE;                                                        /* VCA_CDDDRM_CONNECTIONID_CHANNELINFO */
  }

  /* #50 If functional request active: */
  if (CddDrm_State == CDDDRM_STATE_FUNCTIONAL_REQUEST_ACTIVE)
  {
    /* #60 If not functional request connection closed: Store response length and response code in functional request\
           response buffer */
    if (!CddDrm_IsFunctionalConnection(lConnectionId))
    {
      CddDrm_BufferIdxType bufferIdx = CddDrm_GetFuncRespBufferIdx(lConnectionId);

      if (bufferIdx < CDDDRM_CFG_FUNC_RESP_BUFFER_NUM)                                                                                               /* COV_CDDDRM_ROBUSTNESS */
      {
        CddDrm_FuncReq_RespBuffer[bufferIdx].responseCode = lNotificationInfo.responseCode;
        CddDrm_FuncReq_RespBuffer[bufferIdx].responseBufferSize = lNotificationInfo.responseLength;
      }
      /* no need to copy response data here because the functional response data is directly written into the */
      /* functional response buffer via channel response buffer pointer */
    }
    /* #70 If no active channel exist, set CddDrm state to CDDDRM_STATE_RUN */
    if (CddDrm_Cm_ActiveChannelExists() == FALSE)
    {
      CddDrm_State = CDDDRM_STATE_RUN;
    }
  }
#endif

  /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
  CddDrm_LeaveCritical_ChannelManager();
  /* #80 Inform application or in case of ECU detection the DRM about finished request processing */
  lNotificationFunctionPtr(&lNotificationInfo);                                                                                                      /* VCA_CDDDRM_NOTIFICATIONFUNCPTR */

}

/**********************************************************************************************************************
* CddDrm_HandleChannelClosing()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_HandleChannelClosing(void)
{
  CddDrm_ChannelIdIterType lChannelId;
  /* #10 For all channels */

  for (lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
  {
    /* #20 Check whether closing was requested and no semaphore is set. */
    if ((CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL) &&
        (CddDrm_Closing_Semaphore[lChannelId] == FALSE))                                                                                             /* COV_CDDDRM_ROBUSTNESS */
    {
      CddDrm_Cm_ChannelClose((CddDrm_ChannelIdType)lChannelId);
    }
  }
}

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelInit(CddDrm_ChannelIdType channelId,
                                                           uint8 serviceId,
                                                           CddDrm_ResponseNotificationPtr callback,
                                                           CddDrm_LengthType requestLength,
                                                           boolean sprmib,
                                                           boolean testerPresent,
                                                           boolean dynamicChannel,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_CONST) bufferInfo)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize channel specific parameters */
  CddDrm_SetChannelP2Timer(channelId, CddDrm_Cfg_GetConnectionP2Time(CddDrm_GetConnectionId(channelId)));
  CddDrm_SetChannelRCRRP(channelId, CddDrm_Cfg_GetConnectionRCRRP(CddDrm_GetConnectionId(channelId)));
  CddDrm_SetChannelSPRMIB(channelId, sprmib);
  CddDrm_SetChannelTesterPresent(channelId, testerPresent);
  CddDrm_ResetChannelDynamicConnection(channelId);

  /* request buffer */
  CddDrm_SetChannelReqBufferInfoPtr(channelId, &bufferInfo->requestBufferDataPtr[0]);
  /* response buffer */
  CddDrm_SetChannelRespBufferInfoPtr(channelId, &bufferInfo->responseBufferDataPtr[0]);
  CddDrm_SetChannelRespBufferInfoSize(channelId, bufferInfo->responseBufferSize);

  CddDrm_SetChannelServiceId(channelId, serviceId);
  CddDrm_SetChannelMessageLength(channelId, requestLength);
  CddDrm_SetChannelCurrentMessageIndex(channelId, 0U);
  CddDrm_SetChannelResponseCode(channelId, CDDDRM_RESPONSE_POSITIVE);
  CddDrm_ResetProtocolBufferActive(channelId);
  CddDrm_ChannelInfo[channelId].notificationFunction = callback;

  /* #20 If channel is dynamic, set dynamic specific parameters */
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  if (dynamicChannel)
  {
    CddDrm_SetChannelDynamicConnection(channelId);

    CddDrm_SetChannelRequestMetaData(channelId, &bufferInfo->requestMetaDataBufferPtr[0]);
    CddDrm_SetChannelResponseMetaData(channelId, &bufferInfo->responseMetaDataBufferPtr[0]);

  }
#else
  CDDDRM_DUMMY_STATEMENT(dynamicChannel);                                                                                                            /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
# if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else
  {
    if (channelId == CDDDRM_CHANNEL_FUNCTIONAL_INDEX)
    {
      CddDrm_SetChannelResponseMetaData(channelId, &bufferInfo->responseMetaDataBufferPtr[0]);
    }
  }
# endif
#endif
}

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelGet()
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
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_ChannelGet(
                                              CddDrm_ConnectionIdType connectionId,
                                              uint8 channelClient,
                                              boolean funcTesterPresent,
                                              uint8 serviceId,
                                              P2VAR(CddDrm_RequestReturnType, AUTOMATIC, CDDDRM_VAR_NOINIT) reqReturn)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ChannelIdType lResultChannelId = CDDDRM_CHANNEL_NOT_ASSIGNED;

  /* ----- Implementation ----------------------------------------------- */
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_OFF)
  CDDDRM_DUMMY_STATEMENT(channelClient);                                                                                                             /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_OFF) && (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_OFF))
  CDDDRM_DUMMY_STATEMENT(serviceId);                                                                                                                 /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  /* Prevent that more applications allocate the same channel */
  /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
  CddDrm_EnterCritical_ChannelManager();
  /* Check if service is allowed in current state */
  if (CddDrm_StateMgrCheckSvcProcState(connectionId, funcTesterPresent, reqReturn) == E_OK)
  {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
    /* If requested service is allowed for this connection */
    if (CddDrm_Cm_CheckFirewallRule(connectionId, serviceId) == TRUE)
#endif
    {
      CddDrm_ConnectionIdType lConnectionId = connectionId;
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
      if (funcTesterPresent == TRUE)
      {
        lConnectionId = CddDrm_GetTesterPresentChannelAssignIdx(lConnectionId);
      }
#endif
      /* If connection is not assigned to channel */
      if ((CddDrm_ChannelAssign[lConnectionId] == CDDDRM_CHANNEL_NOT_ASSIGNED))
      {
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
        /* If ECU detection enabled and requesting the channel, then assign the connection to the reserved ECU
          *     detection channel */
        if (channelClient == CDDDRM_CHANNEL_CLIENT_ECU_DETECTION)
        {
          lResultChannelId = CddDrm_Cm_GetEcudChannel(connectionId);
        }
        else
#endif
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
          if (CddDrm_IsFunctionalConnection(connectionId))
          {
            lResultChannelId = CddDrm_Cm_GetFuncChannel(connectionId, funcTesterPresent);
          }
          else
#endif
          { /* Otherwise, application needs a channel, get a free channel for application */
            lResultChannelId = CddDrm_Cm_GetApplChannel(connectionId);
          }
      }
      else
      {
        *reqReturn = CDDDRM_REQUEST_CONNECTION_BUSY;
      }
    }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* Return that firewall blocked the requested service */
    else
    {
      *reqReturn = CDDDRM_REQUEST_FIREWALL_BLOCKED;
    }
#endif
  }
/* >>>> -------------------------------- Leave Critical Section: ChannelManager */
  CddDrm_LeaveCritical_ChannelManager();
  return lResultChannelId;
}

/**********************************************************************************************************************
* CddDrm_Cm_SetInitialChannelState()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_SetInitialChannelState(CddDrm_ChannelIdType channelId)
{

#if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
  if (CddDrm_IsFunctionalConnection(CddDrm_GetConnectionId(channelId)))
  {
    /* If channel is used for TesterPresent, process the request directly */
    if (CddDrm_GetChannelTesterPresent(channelId) == TRUE)
    {
      CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_REQUEST_START);
    }
    else
    {
      if (CddDrm_FuncRequestTimer[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(channelId))] == 0u)
      {
        CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_REQUEST_START);
        /* Set timer to indicate for next request that another functional request is active. */
        CddDrm_FuncRequestTimer[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(channelId))] = CDDDRM_P3_FUNCTIONAL_TIMER_REQUEST_DELAY;    /* VCA_CDDDRM_CONNECTIONID_CHANNELINFO */
      }
      else
      {
        CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_WAIT_DELAY);
      }
    }
  }
  else
#endif
  {
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
    CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_WAIT_DELAY);
    if (CddDrm_RequestDelayTimer == 0U)
    {
      CddDrm_RequestDelayTimer = 1U;
    }
#elif (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
    /* set channel state depend on seperation time */
    if (CddDrm_Cfg_GetConnectionSeparationTime(CddDrm_GetConnectionId(channelId)) == 0U)
    {
      CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_REQUEST_START);
    }
    else
    {
      CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_WAIT_DELAY);
    }
#else
    CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_REQUEST_START);
#endif
  }

}

/**********************************************************************************************************************
* CddDrm_Cm_ChannelAllocate()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelAllocate(CddDrm_ChannelIdType    channelId,
                                                                      CddDrm_ConnectionIdType connectionId,
                                                                      boolean                 funcTesterPresent)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Allocate the free channel */
  CddDrm_SetConnectionId(channelId, connectionId);
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  if (funcTesterPresent == TRUE)
  {
    CddDrm_ChannelAssign[CddDrm_GetTesterPresentChannelAssignIdx(connectionId)] = channelId;                                                         /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
  }
  else
#else
  CDDDRM_DUMMY_STATEMENT(funcTesterPresent);                                                                                                         /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  {
    CddDrm_ChannelAssign[connectionId] = channelId;
  }
  CddDrm_ActiveChannels++;
}

/**********************************************************************************************************************
 * CddDrm_Cm_GetApplChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetApplChannel(CddDrm_ConnectionIdType connectionId)
{
  CddDrm_ChannelIdType lResultChannelId = CDDDRM_CHANNEL_NOT_ASSIGNED;
  CddDrm_ChannelIdIterType lChannelId;

  for (lChannelId = CDDDRM_CHANNEL_APPLICATION_INDEX; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
  {
    if (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_INACTIVE)
    {
      lResultChannelId = (CddDrm_ChannelIdType)lChannelId;
      CddDrm_Cm_ChannelAllocate(lResultChannelId, connectionId, FALSE);
      break;
    }
  }
  return lResultChannelId;
}

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Cm_GetEcudChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetEcudChannel(CddDrm_ConnectionIdType connectionId)
{
  CddDrm_ChannelIdType lResultChannelId = CDDDRM_CHANNEL_NOT_ASSIGNED;

  if (CddDrm_GetChannelState(CDDDRM_CHANNEL_ECU_DETECTION_INDEX) == CDDDRM_CHANNEL_STATE_INACTIVE)                                                   /* COV_CDDDRM_ROBUSTNESS */
  {
    lResultChannelId = CDDDRM_CHANNEL_ECU_DETECTION_INDEX;

    /* allocate the channel */
    CddDrm_Cm_ChannelAllocate(lResultChannelId, connectionId, FALSE);
  }
  return lResultChannelId;
}
#endif

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Cm_GetFuncChannel()
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
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetFuncChannel(
                                                                                  CddDrm_ConnectionIdType connectionId,
                                                                                  boolean funcTesterPresent)
{
  CddDrm_ChannelIdType lResultChannelId = CDDDRM_CHANNEL_NOT_ASSIGNED;

  if ((!CddDrm_ActiveChannelExist()) || (funcTesterPresent == TRUE))
  {
    /* use reserved channel id */
    lResultChannelId = CDDDRM_CHANNEL_FUNCTIONAL_INDEX;

    /* If the call is for a functional 3E with SPRMIB, the Drm state shall not be changed */
    if (funcTesterPresent == TRUE)
    {
      /* If functional request is active, find free channel. */
      if (CddDrm_IsChannelActive(CDDDRM_CHANNEL_FUNCTIONAL_INDEX))
      {
        lResultChannelId = CddDrm_Cm_GetFreeChannel();
      }
    }
    else
    {
      /* Set CddDrm state to functional request active */
      CddDrm_State = CDDDRM_STATE_FUNCTIONAL_REQUEST_ACTIVE;
      CddDrm_FuncReqMgrResetRespResult();
    }

    /* allocate the channel */
    if (CddDrm_IsChannelValid(lResultChannelId))                                                                                 /* COV_CDDDRM_VALIDCHANNEL_FUNCREQUEST */
    {
      CddDrm_Cm_ChannelAllocate(lResultChannelId, connectionId, funcTesterPresent);
    }
  }
  return lResultChannelId;
}

/**********************************************************************************************************************
 * CddDrm_Cm_GetFreeChannel()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_GetFreeChannel(void)
{
  CddDrm_ChannelIdType lResultChannelId = CDDDRM_CHANNEL_NOT_ASSIGNED;
  CddDrm_ChannelIdIterType lChannelId;

  for (lChannelId = CDDDRM_CHANNEL_APPLICATION_INDEX; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)                       /* COV_CDDDRM_VALIDCHANNEL_FUNCREQUEST */
  {
    if (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_INACTIVE)                                                     /* COV_CDDDRM_VALIDCHANNEL_FUNCREQUEST */
    {
      lResultChannelId = (CddDrm_ChannelIdType)lChannelId;
      break;
    }
  }
  return lResultChannelId;
}

/**********************************************************************************************************************
 * CddDrm_Cm_ActiveChannelExists()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
CDDDRM_LOCAL_INLINE FUNC(boolean, CDDDRM_CODE) CddDrm_Cm_ActiveChannelExists(void)
{
  boolean lRetVal = FALSE;
  CddDrm_ChannelIdIterType lChannelId;

  for (lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
  {
    if (CddDrm_GetChannelState(lChannelId) != CDDDRM_CHANNEL_STATE_INACTIVE)
    {
      if (CddDrm_GetChannelTesterPresent(lChannelId) == FALSE)
      {
        lRetVal = TRUE;
        break;
      }
    }
  }
  return lRetVal;
}
#endif

#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
/**********************************************************************************************************************
 * CddDrm_Cm_CheckFirewallRule()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
CDDDRM_LOCAL FUNC(boolean, CDDDRM_CODE) CddDrm_Cm_CheckFirewallRule(CddDrm_ConnectionIdType connectionId, uint8 serviceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean lResult = TRUE;
  /* ----- Implementation ----------------------------------------------- */

  CDDDRM_DUMMY_STATEMENT(connectionId);                                                                                                              /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

# if (CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON)
  /* #10 If a rule is assigned to the connection */
  if (CddDrm_Cfg_GetFirewallRule(connectionId) > 0U)
  {
    /* #20 if the requested service id is blocked return FALSE, otherwise TRUE */
    switch (serviceId)
    {
#  if (CDDDRM_FIREWALL_SERVICE_10 == STD_ON)
      case CDDDRM_SERVICE_10:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_10) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_11 == STD_ON)
      case CDDDRM_SERVICE_11:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_11) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_14 == STD_ON)
      case CDDDRM_SERVICE_14:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_14) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_19 == STD_ON)
      case CDDDRM_SERVICE_19:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_19) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_22 == STD_ON)
      case CDDDRM_SERVICE_22:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_22) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_23 == STD_ON)
      case CDDDRM_SERVICE_23:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_23) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_24 == STD_ON)
      case CDDDRM_SERVICE_24:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_24) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_27 == STD_ON)
      case CDDDRM_SERVICE_27:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_27) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_28 == STD_ON)
      case CDDDRM_SERVICE_28:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_28) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2A == STD_ON)
      case CDDDRM_SERVICE_2A:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_2A) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2C == STD_ON)
      case CDDDRM_SERVICE_2C:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_2C) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2E == STD_ON)
      case CDDDRM_SERVICE_2E:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_2E) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2F == STD_ON)
      case CDDDRM_SERVICE_2F:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_2F) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_31 == STD_ON)
      case CDDDRM_SERVICE_31:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_31) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_34 == STD_ON)
      case CDDDRM_SERVICE_34:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_34) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_35 == STD_ON)
      case CDDDRM_SERVICE_35:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_35) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_36 == STD_ON)
      case CDDDRM_SERVICE_36:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_36) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_37 == STD_ON)
      case CDDDRM_SERVICE_37:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_37) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_38 == STD_ON)
      case CDDDRM_SERVICE_38:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_38) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_3D == STD_ON)
      case CDDDRM_SERVICE_3D:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_3D) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_3E == STD_ON)
      case CDDDRM_SERVICE_3E:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_3E) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_83 == STD_ON)
      case CDDDRM_SERVICE_83:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_83) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_84 == STD_ON)
      case CDDDRM_SERVICE_84:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_84) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_85 == STD_ON)
      case CDDDRM_SERVICE_85:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_85) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_86 == STD_ON)
      case CDDDRM_SERVICE_86:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_86) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_87 == STD_ON)
      case CDDDRM_SERVICE_87:
        if ((CddDrm_Cfg_GetFirewallRule(connectionId) & CDDDRM_CFG_BITPOS_SERVICE_87) > 0U)
        {
          lResult = FALSE;
        }
        break;
#  endif
      default:
        /* lResult is already TRUE */
        break;
    } /* End switch */
  } /* End if */
# endif
# if (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON)
  /* #30 Otherwise: return the result of the user firewall check (if configured) */
#  if (CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON)
  if (lResult == TRUE) 
#  endif
  {
    if (CddDrm_Cfg_FirewallUserCallback(connectionId, serviceId) != TRUE)
    {
      lResult = FALSE;
    } /* else not needed, value can be set before to FALSE and is initially set to TRUE */
  }
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Ecud_SetupRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_SetupRequest(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ConnectionTableIterType lConnectionId;
  uint8 lNextConnectionId;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_OK;
  /* ----- Implementation ----------------------------------------------- */
  lNextConnectionId = CddDrm_Ecud_ConnectionIterationIndex;
  /* Iterate through all configured connections */
  for (lConnectionId = CddDrm_Ecud_ConnectionIterationIndex; lConnectionId < CDDDRM_CFG_ECUD_INDEX_LIMIT; lConnectionId++)
  {
    CddDrm_ChannelIdType lFreeChannel;
    CddDrm_BufferStructType lBufferInfo;

    lBufferInfo.requestBufferDataPtr = (CddDrm_BufferDataPtrType)&CddDrm_Cfg_GetEcuDetectionRequestByte(0U);                                          /* PRQA S 0311 */ /* MD_CDDDRM_Rule11.8_0311 */
    lBufferInfo.responseBufferDataPtr = &CddDrm_Ecud_ResponseBuffer[0U];
    lBufferInfo.responseBufferSize = CDDDRM_CFG_ECUD_RESPONSE_BUFFER_SIZE;
    /* Get the channel exclusive reserved for ECU detection and initialize the channel parameters */
    lFreeChannel = CddDrm_Cm_ChannelGet((CddDrm_ConnectionIdType)lConnectionId, CDDDRM_CHANNEL_CLIENT_ECU_DETECTION, FALSE, CddDrm_Cfg_GetEcuDetectionRequestByte(0U), &lResult);
    /* Check if channel is valid */
    if (CddDrm_IsChannelValid(lFreeChannel))
    {
      CddDrm_Ecud_State = CDDDRM_ECUD_STATE_BUSY;
      /* exclusive channel is free */
      CddDrm_Cm_ChannelInit(lFreeChannel,
                            lBufferInfo.requestBufferDataPtr[0U],
                            &CddDrm_Ecud_ResponseNotification,
                            CDDDRM_CFG_ECUD_REQUEST_LENGTH,
                            FALSE,
                            FALSE,
                            FALSE,
                            &lBufferInfo);
      CddDrm_Cm_SetInitialChannelState(lFreeChannel);
      lNextConnectionId = (uint8)lConnectionId + 1U;
      break;
    }
    else
    {
      lNextConnectionId = (uint8)lConnectionId + 1U;
    }
    
  }
    CddDrm_Ecud_ConnectionIterationIndex = lNextConnectionId;
}
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Ecud_ResponseNotification()
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
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_ResponseNotification(P2CONST(CddDrm_RespInfoStructType, AUTOMATIC, CDDDRM_CONST) response)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_EcudStateType lEcuState = CddDrm_Cfg_EcuDetectionData.EcuList[CDDDRM_ECUD_GET_CONNECTION_INDEX(response->connectionId)];
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if ECU Detection is actually running */
  if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_BUSY)
  {
    /* # Convert response code to respective ECU detection state */
    switch (response->responseCode)
    {
      case CDDDRM_RESPONSE_PDUR_TX_ERROR:
      case CDDDRM_RESPONSE_PDUR_RX_ERROR:
        /* ECU detection state initialized to not discovered -> nothing to do here */
        break;
      case CDDDRM_RESPONSE_TIMEOUT:
        /* ECU detection state: not available */
        lEcuState = CDDDRM_ECUD_SET_CONNECTION_NOT_AVAILABLE(lEcuState, response->connectionId);
        break;
      case CDDDRM_RESPONSE_BUFFER_TOO_SMALL:
      case CDDDRM_RESPONSE_INVALID_NRC_LENGTH:
      case CDDDRM_RESPONSE_POSITIVE:
# if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
#  if (CDDDRM_CFG_SUPPORT_FORWARD_RCRRP == STD_ON)
      case CDDDRM_RESPONSE_FORWARDED_RCRRP:
#  else
      case CDDDRM_RESPONSE_RCRRP_LIMIT_REACHED:
#  endif
# endif
      case CDDDRM_RESPONSE_WRONG_SERVICE:
      default:
        /* ECU detection state: available */
        lEcuState = CDDDRM_ECUD_SET_CONNECTION_AVAILABLE(lEcuState, response->connectionId);
        break;
    }
    /* #20 Write result to detection list */
    CddDrm_Cfg_EcuDetectionData.EcuList[CDDDRM_ECUD_GET_CONNECTION_INDEX(response->connectionId)] = lEcuState;
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_READY;
  }
}
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Ecud_StateTask()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_StateTask(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Proceed if ECU Detection is ready */
  if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_READY)
  {
    /* #20 If the end of configured connection list reached the ECU detection has finished */
    if (CddDrm_Ecud_ConnectionIterationIndex == CDDDRM_CFG_ECUD_INDEX_LIMIT)
    {
      /* #30 If ECU detection is finished write the detection list to NvRam */
# if (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON)
      if (NvM_WriteBlock((NvM_BlockIdType)CDDDRM_CFG_NVM_BLOCK_ECU_DETECTION, (uint8*)&CddDrm_Cfg_EcuDetectionData) == E_OK)
      {
        CddDrm_Ecud_State = CDDDRM_ECUD_STATE_WRITE_NVM;
      }
      else
      { /* End ECU detection by notifing the application and setting the state to idle */
#  if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
        CddDrm_Cfg_Ecud_Finished();
#  endif
        CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
      }
# else
#  if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
      CddDrm_Cfg_Ecud_Finished();
#  endif
      CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
# endif
    }
    else
    {
      /* #40 Otherwise: Setup the next request */
      CddDrm_Ecud_SetupRequest();
    }
  }
  else
# if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON) && (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON)
  /* #50 Otherwise: If configured notify the application that the ECU detection has finished and set the ECU detection state to idle */
  if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_FINISH)
  {
    CddDrm_Cfg_Ecud_Finished();
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
  }
  else
# endif
  {
    /* nothing to do */
  }
}
#endif

/**********************************************************************************************************************
 * CddDrm_Queue_ExternalTester()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_ExternalTester(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If external tester detected queue reports that an external tester was detected */
  if (CddDrm_Queue_TesterConnected == CDDDRM_QUEUE_EXTERNAL_TESTER_CONNECTED)
  {
    /* #20 Set DRM state to external tester mode, Stop ECU Detection, cancel all processing requests */
    /* first change state, so application is not allowed to send request anymore */
    CddDrm_State = CDDDRM_STATE_TESTER_DETECTED;
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_EMPTY;
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
# if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
    CddDrm_Cfg_Ecud_Finished();
# endif
#endif
    CddDrm_RequestClosingOfAllChannels();
  }
  /* #30 Else if external tester queue reports that external tester is disconnected: Set DRM to normal run mode */
  else if (CddDrm_Queue_TesterConnected == CDDDRM_QUEUE_EXTERNAL_TESTER_DISCONNECTED)
  {
    CddDrm_State = CDDDRM_STATE_RUN;
  }
  else
  {
    /* nothing to do */
  }
  CddDrm_Queue_TesterConnected = CDDDRM_QUEUE_EMPTY;
}

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Queue_EcuDetection()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_EcuDetection(void)
{
  uint8_least lElementIndex;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If application requests the start of ECU detection */
  if (CddDrm_Queue_Ecud == CDDDRM_QUEUE_ECUD_START)
  {
    /* Queue is only set to ECUD_START, if external tester is not present and
     * Ecu detection is not running. Handled in StartEcuDetection API, so it is
     * not necessary here.
     */
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_EMPTY;
    /* prevent that ecu detection was started, while external tester is active */
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_READY;
    CddDrm_Ecud_ConnectionIterationIndex = 0;
    /* reset ECU Detection states for all connections */
    /* #20 Initialize ECU detection list */
    for (lElementIndex = 0; lElementIndex < CDDDRM_CFG_ECUD_LIST_SIZE; lElementIndex++)
    {
      CddDrm_Cfg_EcuDetectionData.EcuList[lElementIndex] = 0;
    }
  }
  /* #30 If application requested stop of ECU detection */
  if (CddDrm_Queue_Ecud == CDDDRM_QUEUE_ECUD_STOP)
  {
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_EMPTY;
    /* #40 If detection is busy: close exclusive channel, stop detection */
    if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_BUSY)
    {
      /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
      CddDrm_EnterCritical_ChannelManager();

      CddDrm_SetChannelState(CDDDRM_CHANNEL_ECU_DETECTION_INDEX, CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL);

      /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
      CddDrm_LeaveCritical_ChannelManager();

      CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
    }
    /* #50 If detection is ready: stop detection */
    else if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_READY)
    {
      CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
    }
    else
    {
      /* nothing to do */
    }
  }
}
#endif


/**********************************************************************************************************************
 * CddDrm_StateMgrCheckSvcProcState()
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
CDDDRM_LOCAL_INLINE FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StateMgrCheckSvcProcState(
                                              CddDrm_ConnectionIdType connectionId,
                                              boolean funcTesterPresent,
                                              P2VAR(CddDrm_RequestReturnType, AUTOMATIC, CDDDRM_VAR_NOINIT) reqReturn)
{
  Std_ReturnType lStdReturn;

  switch (CddDrm_State)
  {
  case CDDDRM_STATE_TESTER_DETECTED:
    lStdReturn = E_NOT_OK;
    *reqReturn = CDDDRM_REQUEST_TESTER_ACTIVE;
    break;
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  case CDDDRM_STATE_FUNCTIONAL_REQUEST_ACTIVE:
    if (funcTesterPresent == TRUE)
    {
      lStdReturn = E_OK;
    }
    else
    {
      lStdReturn = E_NOT_OK;
      *reqReturn = CDDDRM_REQUEST_FUNCTIONAL_ACTIVE;
    }

    break;
#endif
  default:
    lStdReturn = E_OK;
    break;
  }

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON && CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  if (lStdReturn == E_OK)
  {
    if ((CddDrm_Ecud_State != CDDDRM_ECUD_STATE_IDLE)
      && CddDrm_IsFunctionalConnection(connectionId))
    {
      lStdReturn = E_NOT_OK;
      *reqReturn = CDDDRM_REQUEST_ECUD_ACTIVE;
    }
  }
#else
  CDDDRM_DUMMY_STATEMENT(connectionId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CDDDRM_DUMMY_STATEMENT(funcTesterPresent);                                                                                                             /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  return lStdReturn;
}

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
 * CddDrm_FuncReqMgrInitResponseChannel()
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
CDDDRM_LOCAL_INLINE FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_FuncReqMgrInitResponseChannel(
                                                          PduIdType rxPduId,
                                                          CddDrm_LengthType responseLength)
{
  Std_ReturnType lStdReturn = E_NOT_OK;
  boolean        lDynChannel = FALSE;
  /* If functional channel is waiting for response */
  if (CddDrm_GetChannelState(CDDDRM_CHANNEL_FUNCTIONAL_INDEX) == CDDDRM_CHANNEL_STATE_WAIT_RESPONSE)                                                 /* COV_CDDDRM_ROBUSTNESS */
  {
    CddDrm_ConnectionIdType lConnectionId = CddDrm_Cfg_GetConnectionIdByRxPdu(rxPduId);
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_GetApplChannel(lConnectionId);                                                                       /* VCA_CDDDRM_CONNECTIONID_RXPDU */

    if (CddDrm_IsChannelValid(lChannelId))                                                                                                           /* COV_CDDDRM_VALIDCHANNEL_FUNCRESPONSE */
    {
      CddDrm_BufferStructType lBufferInfo;
      CddDrm_BufferIdxType lBufferIdx = CddDrm_GetFreeFuncRespBuffer();

      if (lBufferIdx != CDDDRM_FUNCTIONAL_INVALIDRESPONSEIDX)                                                                                        /* COV_CDDDRM_FUNCRESPONSEBUFFER */
      {
        /* Initialize buffers for channel */
        lBufferInfo.requestBufferDataPtr = &CddDrm_GetChannelReqBufferInfoPtrVal(CDDDRM_CHANNEL_FUNCTIONAL_INDEX, 0U);
        lBufferInfo.responseBufferDataPtr = CddDrm_GetFuncRespBufferPtr(lBufferIdx);
        lBufferInfo.responseBufferSize = CDDDRM_CFG_FUNC_RESP_BUFFER_SIZE;

        /* Set the connectionId in the buffer object */
        CddDrm_FuncReq_RespBuffer[lBufferIdx].connectionId = lConnectionId;

# if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
        if (CddDrm_GetRxPduRefToMetaDataLength(rxPduId) != 0U)
        {
          lBufferInfo.responseMetaDataBufferPtr = &CddDrm_GetChannelReqBufferInfoPtrVal(CDDDRM_CHANNEL_FUNCTIONAL_INDEX, 0U);
          /* requestMetaData is set but it is not used during the functional responses. */
          lBufferInfo.requestMetaDataBufferPtr = &CddDrm_GetChannelRequestMetaData(CDDDRM_CHANNEL_FUNCTIONAL_INDEX, 0U);
          lDynChannel = TRUE;
        }
# endif

        CddDrm_Cm_ChannelInit(lChannelId,
          CddDrm_GetChannelServiceId(CDDDRM_CHANNEL_FUNCTIONAL_INDEX),
          CddDrm_ChannelInfo[CDDDRM_CHANNEL_FUNCTIONAL_INDEX].notificationFunction,
          responseLength,
          CddDrm_GetChannelSPRMIB(CDDDRM_CHANNEL_FUNCTIONAL_INDEX),
          FALSE,
          lDynChannel,
          &lBufferInfo);

        lStdReturn = E_OK;
      }
      else
      {
        CddDrm_RespInfoStructType lNotificationInfo;
        CddDrm_ResponseNotificationPtr lNotificationFunctionPtr;

        /* Enter Critical Section */
        CddDrm_EnterCritical_ChannelManager();

        /* Reset channel */
        CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_INACTIVE);
        CddDrm_ChannelAssign[lConnectionId] = CDDDRM_CHANNEL_NOT_ASSIGNED;
        CddDrm_ActiveChannels--;

        /* Leave Critical Section */
        CddDrm_LeaveCritical_ChannelManager();

        /* Notify user that all the buffers are in use. */
        lNotificationFunctionPtr = CddDrm_ChannelInfo[CDDDRM_CHANNEL_FUNCTIONAL_INDEX].notificationFunction;

        lNotificationInfo.connectionId = lConnectionId;
        lNotificationInfo.responseCode = CDDDRM_RESPONSE_BUFFER_FULL;
        lNotificationInfo.responseLength = 0;
        lNotificationInfo.serviceId = CddDrm_GetChannelServiceId(CDDDRM_CHANNEL_FUNCTIONAL_INDEX);

        lNotificationFunctionPtr(&lNotificationInfo);                                                                                                /* VCA_CDDDRM_NOTIFICATIONFUNCPTR */
      }
    }
  }
  return lStdReturn;
}
#endif

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
 * CddDrm_FuncReqMgrResetRespResult()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_FuncReqMgrResetRespResult(void)
{
  CddDrm_ConnectionTableIterType lResultBufferIndex;

  for (lResultBufferIndex = 0u; lResultBufferIndex < CDDDRM_CFG_FUNC_RESP_BUFFER_NUM; lResultBufferIndex++)
  {
    uint8_least bufferIdx;
    for (bufferIdx = 0; bufferIdx < CDDDRM_CFG_FUNC_RESP_BUFFER_SIZE; bufferIdx++)
    {
      CddDrm_FuncReq_RespBuffer[lResultBufferIndex].responseBuffer[bufferIdx] = 0u;
    }

    CddDrm_FuncReq_RespBuffer[lResultBufferIndex].responseCode = CDDDRM_RESPONSE_TIMEOUT;
    CddDrm_FuncReq_RespBuffer[lResultBufferIndex].responseBufferSize = 0u;
    CddDrm_FuncReq_RespBuffer[lResultBufferIndex].responseProvided = FALSE;
    CddDrm_FuncReq_RespBuffer[lResultBufferIndex].connectionId = 0xFFu;
  }
}
#endif

/**********************************************************************************************************************
*  CddDrm_ProcessStartOfReception()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessStartOfReception(PduIdType pduId,
                                                    CddDrm_ChannelIdType channelId,
                                                    P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                    PduLengthType TpSduLength,
                                                    P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType lResult       = BUFREQ_E_NOT_OK;
  CddDrm_ChannelIdType lChannelId = channelId;
  boolean lIsFunctionalResponse   = FALSE;

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  boolean lChannelAllocated = TRUE;

  /* #10 If a functional request is active, initialize a physical channel for the response. */
  if (CddDrm_StateMgrIsFuncReqActive())
  {
    lIsFunctionalResponse = TRUE;
    if ((CddDrm_FuncReqMgrInitResponseChannel(pduId, TpSduLength)) == E_OK)
    {
      lChannelId = CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByRxPdu(pduId)];
    }
    else
    {
      lChannelAllocated = FALSE;
    }
  }
  /* #20 Else, set the message length to TpSduLength.  */
  else
#endif
  {
    CddDrm_SetChannelMessageLength(lChannelId, TpSduLength);
  }

  /* #30 If channel is allocated, process the response for allocated channel */
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  if (lChannelAllocated == TRUE)
#endif
  {
    lResult = CddDrm_ProcessAllocatedChannel(lChannelId, info, TpSduLength, lIsFunctionalResponse, bufferSizePtr);                                   /* VCA_CDDDRM_FUNCTIONAL_RESPONSE_CHANNEL */
  }
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_OFF)
  CDDDRM_DUMMY_STATEMENT(pduId);                                                                                                                /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return lResult;
}

/**********************************************************************************************************************
*  CddDrm_ProcessAllocatedChannel()
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
 *
 *
 *
 *
 *
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessAllocatedChannel(CddDrm_ChannelIdType channelId,
                                                                                        P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,        /* PRQA S 3673 */ /* MD_CDDDRM_Rule8.13_3673 */
                                                                                        PduLengthType TpSduLength,
                                                                                        boolean isFunctionalResponse,
                                                                                        P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;
  CddDrm_ResponseCodeType lResponseCode = CDDDRM_RESPONSE_PDUR_RX_ERROR;
  boolean lCloseChannelNeeded = FALSE;
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  boolean lReloadP2Timer = FALSE;
#endif
  PduLengthType lMetaDataLength = 0;

#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  if (CddDrm_IsChannelDynamicConnection(channelId))
  {
    lMetaDataLength = info->SduLength;
  }
#endif

  /* #10 If TpSduLength is equal to 0 set channel close needed flag to TRUE */
  if (TpSduLength == 0U)
  {
    lCloseChannelNeeded = TRUE;
  }
  /* #20 Else If the local response buffer size is smaller than TpSduLength + optional metadata length\
         close the channel and report buffer too small error */
  else if (CddDrm_GetChannelRespBufferInfoSize(channelId) < (TpSduLength + lMetaDataLength))                                                         /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
  {
    lResult = BUFREQ_E_OVFL;
    lResponseCode = CDDDRM_RESPONSE_BUFFER_TOO_SMALL;
    lCloseChannelNeeded = TRUE;
  }
  /* #30 Otherwise */
  else
  {
    /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
    CddDrm_EnterCritical_ChannelManager();
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    /* #40 If a response is received from a functional request (if enabled) */
    if (isFunctionalResponse == TRUE)
    {
      /* #50 If functional channel is in active state (i.e.not closed by an interrupt/task),\
             Set result to BUFRREQ_OK\
             Available buffer size to Response buffer size\
             Channel state to CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING */
      if (CddDrm_IsChannelActive(CDDDRM_CHANNEL_FUNCTIONAL_INDEX))                                                                                   /* COV_CDDDRM_ROBUSTNESS */
      {
        lResult = BUFREQ_OK;
        lReloadP2Timer = TRUE;
        *bufferSizePtr = CddDrm_GetChannelRespBufferInfoSize(channelId);
        CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING);

      }
      /* #60 Otherwise,\
             Set channel close flag to TRUE\
             Response code to CDDDRM_RESPONSE_CHANNELS_CLOSED */
      else
      {
        lCloseChannelNeeded = TRUE;
        lResponseCode = CDDDRM_RESPONSE_CHANNELS_CLOSED;
      }
    }
    else
#endif
    /* #70 Otherwise, if the response is received from a physical channel */
    {
      /* #80 If the channel state is CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL\
             Set the result to BUFREQ_OK\
             Available buffer size to Response buffer size\
             Channel state to CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING */
      if (CddDrm_GetChannelState(channelId) != CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL)                                                                   /* COV_CDDDRM_ROBUSTNESS */
      {
        lResult = BUFREQ_OK;
        *bufferSizePtr = CddDrm_GetChannelRespBufferInfoSize(channelId);
        CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING);
      }
    }

    /* #90 If channel is a dynamic channel, copy metadata to response buffer and update the current message index */
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
    if (CddDrm_IsChannelDynamicConnection(channelId))
    {
      CddDrm_MemCopy(&CddDrm_GetChannelRespBufferInfoPtrVal(channelId, CddDrm_GetChannelCurrentMessageIndex(channelId)),                             /* VCA_CDDDRM_MEMCPY_CALL_RESPONSEBUFFER */
        info->SduDataPtr,
        info->SduLength);

      CddDrm_AddChannelCurrentMessageIndex(channelId, info->SduLength);
      CddDrm_SetChannelMessageLength(channelId, (TpSduLength + info->SduLength));
    }
#else
    CDDDRM_DUMMY_STATEMENT_CONST(info);
#endif
    /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
    CddDrm_LeaveCritical_ChannelManager();
  }

  /* #100 If close channel needed flag is set to TRUE, set the response code received and close the channel */
  if (lCloseChannelNeeded == TRUE)
  {
    CddDrm_SetChannelResponseCode(channelId, lResponseCode);
    CddDrm_Cm_ChannelClose(channelId);
  }
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else
  {
    /* #110 If response for functional request is received and chanel is active, reload P2 timer */
    if (lReloadP2Timer == TRUE)
    {
      CddDrm_Timer_ReloadP2Timer(channelId, FALSE);
    }
  }
#endif

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_OFF)
  CDDDRM_DUMMY_STATEMENT(isFunctionalResponse);                                                                                                                /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  return lResult;
}

/**********************************************************************************************************************
*  CddDrm_ProcessCopyRxData()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessCopyRxData(CddDrm_ChannelIdType channelId,
                                                   P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,                                             /* PRQA S 3673 */ /* MD_CDDDRM_Rule8.13_3673 */
                                                   P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;

  /* #10 If data length received to be copied is 0, update available receive buffer and return BUFREQ_OK */
  if (info->SduLength == 0U)
  {
    *bufferSizePtr = CddDrm_GetChannelRespBufferInfoSize(channelId) - CddDrm_GetChannelCurrentMessageIndex(channelId);
    lResult = BUFREQ_OK;
  }
  /* #20 Else If, data length received to be copied is not 0 */
  else
  {
    /* #30 If the remaining receive data buffer is larger than the data length received to be copied, copy the\
           data to response buffer, update the current message index and available buffer pointer */
    if ((CddDrm_GetChannelMessageLength(channelId) - CddDrm_GetChannelCurrentMessageIndex(channelId))
         >= info->SduLength)
    {
      CddDrm_MemCopy(&CddDrm_GetChannelRespBufferInfoPtrVal(channelId, CddDrm_GetChannelCurrentMessageIndex(channelId)),                             /* VCA_CDDDRM_MEMCPY_CALL_RESPONSEBUFFER */
                     info->SduDataPtr,
                     info->SduLength);

      CddDrm_AddChannelCurrentMessageIndex(channelId, info->SduLength);
      *bufferSizePtr = CddDrm_GetChannelRespBufferInfoSize(channelId) - CddDrm_GetChannelCurrentMessageIndex(channelId);
      lResult = BUFREQ_OK;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
*  CddDrm_ProcessCopyTxData()
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
CDDDRM_LOCAL_INLINE FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_ProcessCopyTxData(CddDrm_ChannelIdType channelId,
                                                 P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,                                               /* PRQA S 3673 */ /* MD_CDDDRM_Rule8.13_3673 */
                                                 P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) availableDataPtr)
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;
  uint8             lProtocolDataLength;

#if (CDDDRM_CFG_SUPPORT_DEDICATED_APIS == STD_ON)
  PduLengthType lRemainingProtocolDataToCopy;
  lProtocolDataLength = CddDrm_GetChannelProtocolLength(channelId);
#else
  lProtocolDataLength = 0U;
#endif

  /* #10 If the available transmit data buffer is larger than the number of requested bytes to be copied */
  if ((CddDrm_GetChannelMessageLength(channelId) - CddDrm_GetChannelCurrentMessageIndex(channelId))
       >= info->SduLength)
  {
#if (CDDDRM_CFG_SUPPORT_DEDICATED_APIS == STD_ON)
    /* #20 If protocol buffer is active and if the current message index is less than the protocol buffer length */
    /* Choose if application or internal buffer must be used. Internal buffer is only used by dedicated service APIs */
    if ((CddDrm_GetProtocolBufferActive(channelId) == TRUE) &&
        (CddDrm_GetChannelCurrentMessageIndex(channelId) < lProtocolDataLength))
    {
      lRemainingProtocolDataToCopy = (PduLengthType) (lProtocolDataLength - CddDrm_GetChannelCurrentMessageIndex(channelId));
      /* #30 If the data available to copied from the protocol buffer is less than the requested bytes to be copied */
      if ((lRemainingProtocolDataToCopy)
         < info->SduLength)
      {
        /* Copy the protocol data to the buffer */
        CddDrm_MemCopy(info->SduDataPtr,                                                                                                             /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */ /* VCA_CDDDRM_MEMCPY_CALL_REQUESTDATA */
                       &(CddDrm_ChannelInfo[channelId].protocolData[CddDrm_GetChannelCurrentMessageIndex(channelId)]),
                       lRemainingProtocolDataToCopy);

        CddDrm_MemCopy(&info->SduDataPtr[lRemainingProtocolDataToCopy],                                                                              /* VCA_CDDDRM_MEMCPY_CALL_REQUESTDATA */
                       &(CddDrm_GetChannelReqBufferInfoPtrVal(channelId, 0U)),
                       ((PduLengthType)(info->SduLength - lRemainingProtocolDataToCopy)));  
      }
      /* #40 Otherwise, copy the requested data from the protocol buffer */
      else
      {
        CddDrm_MemCopy(info->SduDataPtr,                                                                                                             /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */ /* VCA_CDDDRM_MEMCPY_CALL_REQUESTDATA */
                       &(CddDrm_ChannelInfo[channelId].protocolData[CddDrm_GetChannelCurrentMessageIndex(channelId)]),
                       info->SduLength);
      }
    }
    /* #50 Otherwise, copy the data at the current message index of the request buffer */
    else
#endif
    {
      CddDrm_MemCopy(info->SduDataPtr,                                                                                                               /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */ /* VCA_CDDDRM_MEMCPY_CALL_REQUESTDATA */
                     &(CddDrm_GetChannelReqBufferInfoPtrVal(channelId, (CddDrm_GetChannelCurrentMessageIndex(channelId) - lProtocolDataLength))),
                     info->SduLength);
    }
    /* #60 Update current message index for copying the data and remaining bytes to be copied */
    CddDrm_AddChannelCurrentMessageIndex(channelId, info->SduLength);
    *availableDataPtr = (CddDrm_GetChannelMessageLength(channelId)
                        - CddDrm_GetChannelCurrentMessageIndex(channelId));

    lResult = BUFREQ_OK;
  }
  return lResult;
}

/**********************************************************************************************************************
*  CddDrm_ProcessTpRxIndication()
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
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_ProcessTpRxIndication(CddDrm_ChannelIdType channelId,
                                                                         Std_ReturnType result)
{
  CddDrm_ResponseCodeType lResponseCode = CDDDRM_RESPONSE_PDUR_RX_ERROR;
  uint8                   lSvcIndex = 0;
#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
  boolean                 lChannelCloseNeeded = TRUE;
#endif
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  if (CddDrm_IsChannelDynamicConnection(channelId))
  {
    lSvcIndex = CDDDRM_SVCID_INDEX;
  }
#endif
  /* #10 If result is E_OK */
  /* Negative Response Message
  * | 0 | - Negative Response SID
  * | 1 | - <Service Name> Request SID
  * | 2 | - Response Code */
  if (result == E_OK)
  {
    /* #20 If negative response is received, call handle negative response function */
    if (CddDrm_GetChannelRespBufferInfoPtrVal(channelId, lSvcIndex) == 0x7fU)
    {
      lResponseCode = CddDrm_HandleNegativeResponse(channelId);

      /* #30 If the response code received is CDDDRM_RESPONSE_FORWARDED_RCRRP and Ecu detection is not running, set channel close not needed */
#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
      if ((lResponseCode == CDDDRM_RESPONSE_FORWARDED_RCRRP)
# if ((CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON) && (CDDDRM_CFG_SUPPORT_FORWARD_RCRRP == STD_ON))
        && (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_IDLE)
# endif
        )
      {
        lChannelCloseNeeded = FALSE;
      }
#endif
    }
    /* #40 Else if a positive response is received, return with response code CDDDRM_RESPONSE_POSITIVE */
    else if (CddDrm_GetChannelRespBufferInfoPtrVal(channelId, lSvcIndex)
         == (CddDrm_GetChannelServiceId(channelId) | CDDDRM_RESPONSE_CODE_VALIDATION))
    {
      lResponseCode = CDDDRM_RESPONSE_POSITIVE;
    }
    /* #50 Otherwise, return with response code as CDDDRM_RESPONSE_WRONG_SERVICE */
    else
    {
      lResponseCode = CDDDRM_RESPONSE_WRONG_SERVICE;
    }
  }

  CddDrm_EnterCritical_ChannelManager();
  /* #60 Check if channel is in active state (i.e.not closed by an interrupt/task) and channel close is not needed,\
         set the channel state to CDDDRM_CHANNEL_STATE_WAIT_RESPONSE */
  if (CddDrm_IsChannelActive(channelId))                                                                                                             /* COV_CDDDRM_ROBUSTNESS */
  {
#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
    if (lChannelCloseNeeded == FALSE)
    {
      CddDrm_SetChannelState(channelId, CDDDRM_CHANNEL_STATE_WAIT_RESPONSE);
    }
#endif
  }

  CddDrm_LeaveCritical_ChannelManager();

  /* #80 If channel close is needed, set the response code received and close the channel */
#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
  if (lChannelCloseNeeded == TRUE)
#endif
  {
    CddDrm_SetChannelResponseCode(channelId, lResponseCode);
    CddDrm_Cm_ChannelClose(channelId);
  }
}

/**********************************************************************************************************************
*  CddDrm_HandleNegativeResponse()
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
CDDDRM_LOCAL_INLINE FUNC(CddDrm_ResponseCodeType, CDDDRM_CODE) CddDrm_HandleNegativeResponse(CddDrm_ChannelIdType channelId)
{
  CddDrm_ResponseCodeType lResponseCode = CDDDRM_RESPONSE_INVALID_NRC_LENGTH;
  CddDrm_LengthType       lMetaDataLength = 0;
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  if (CddDrm_IsChannelDynamicConnection(channelId))
  {
    lMetaDataLength = CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN;
  }
#endif
  /* #10 Verify that response length has three bytes */
  if (CddDrm_GetChannelMessageLength(channelId) == (3U + lMetaDataLength))
  {
    /* #20 Check if service id is same as in request */
    if (CddDrm_GetChannelRespBufferInfoPtrVal(channelId, (1U + lMetaDataLength)) == CddDrm_GetChannelServiceId(channelId))
    {
#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
      /* #30 If RCRRP: Handle P2/P2* and RCRRP counter */
      if (CddDrm_GetChannelRespBufferInfoPtrVal(channelId, (2U + lMetaDataLength)) == 0x78U)
      {
        lResponseCode = CddDrm_Cm_ProcessRCRRP(channelId);
      }
      else
#endif
      {
        /* #40 Otherwise, return with the NRC received */
        lResponseCode = CddDrm_GetChannelRespBufferInfoPtrVal(channelId, (2U + lMetaDataLength));
      }
    }
    else
    {
      /* #50 Otherwise, return with response code as CDDDRM_RESPONSE_WRONG_SERVICE */
      lResponseCode = CDDDRM_RESPONSE_WRONG_SERVICE;
    }
  }

  return lResponseCode;
}

/**********************************************************************************************************************
*  CddDrm_ProcessTpTxConfirmation()
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
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_ProcessTpTxConfirmation(CddDrm_ChannelIdType channelId,
                                                                                Std_ReturnType result)
{
  CddDrm_ChannelStateType lNextState = CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL;
  boolean lCloseChannelNeeded = FALSE;

  /* #10 Check if the transmission was successful */
  if (result == E_OK)
  {
    /* #20 Send a TxConfirmation notification (if enabled) and reset the message length */
#if (CDDDRM_CFG_SUPPORT_TXCONFIRMATION == STD_ON)
    /* notify that DRM sent the request to PduR */
    CddDrm_Cfg_TxConfirmation(CddDrm_GetConnectionId(channelId));
#endif
    /* #30 Reset the channel length to 0 as the request has been processed, from this point and\
           on it is used for the response */
    CddDrm_SetChannelMessageLength(channelId, 0U);

    /* #40 Reset the Channel message index for proper response handling */
    CddDrm_SetChannelCurrentMessageIndex(channelId, 0U);

    /* #50 If any request delay is enabled, set the request delay timer */
#if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
    if ((CddDrm_IsFunctionalConnection(CddDrm_GetConnectionId(channelId))) && (CddDrm_GetChannelTesterPresent(channelId) == FALSE))
    {
      CddDrm_FuncRequestTimer[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(channelId))] = CDDDRM_P3_FUNCTIONAL_TIMER_REQUEST_DELAY;                                                             /* VCA_CDDDRM_CONNECTIONID_CHANNELINFO */
    }
#endif
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
    CddDrm_RequestDelayTimer = CDDDRM_TIMER_REQUEST_DELAY;
    CddDrm_NextConnection = (CddDrm_NextConnection + 1u) % CDDDRM_CFG_CONNECTION_COUNT;
#endif
    lNextState = CDDDRM_CHANNEL_STATE_WAIT_RESPONSE;
  }

  CddDrm_EnterCritical_ChannelManager();
  /* #60 Check if channel is in active state (i.e.not closed by an interrupt/task) */
  if (CddDrm_IsChannelActive(channelId))                                                                                                             /* COV_CDDDRM_ROBUSTNESS */
  {

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    /* #70 If functional request for Tester Present(0x3E) with SPRMIB, set close channel needed as TRUE to allow for another request */
    if ((CddDrm_IsFunctionalConnection(CddDrm_GetConnectionId(channelId)))
      && (CddDrm_GetChannelServiceId(channelId) == CDDDRM_SERVICE_3E)
      && (CddDrm_GetChannelSPRMIB(channelId) == TRUE))
    {
      CddDrm_SetChannelResponseCode(channelId, CDDDRM_RESPONSE_TIMEOUT);
      lCloseChannelNeeded = TRUE;
    }
    /* Set flag that functional request processing has been performed. */
    if (CddDrm_IsFunctionalConnection(CddDrm_GetConnectionId(channelId)))
    {
      CddDrm_FuncRequestProcessing[CddDrm_GetFunctionalDelayRequestIdx(CddDrm_GetConnectionId(channelId))] = FALSE;                                  /* VCA_CDDDRM_CONNECTIONID_CHANNELINFO */
    }
#endif
    /* #80 If next state is CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL, Set the response code as\ 
           CDDDRM_RESPONSE_PDUR_TX_ERROR and set close channel needed as TRUE */
    if (lNextState == CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL)
    {
      CddDrm_SetChannelResponseCode(channelId, CDDDRM_RESPONSE_PDUR_TX_ERROR);
      lCloseChannelNeeded = TRUE;
    }
    /* #90 Set the state to wait response so that the P2 timer is restarted */
    CddDrm_SetChannelState(channelId, lNextState);
  }
  CddDrm_LeaveCritical_ChannelManager();

  /* #100 If close channel needed is TRUE, close the channel */
  if (lCloseChannelNeeded == TRUE)
  {
    CddDrm_Cm_ChannelClose(channelId);
  }
}

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
# if ((CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON))
/**********************************************************************************************************************
* CddDrm_InitializeCancellationStruct()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_InitializeCancellationStruct(
                                      P2VAR(CddDrm_CancellationStructType, AUTOMATIC, AUTOMATIC) cancellationsToInvoke,
                                      CddDrm_ChannelIdType channelCount)
{
  CddDrm_ChannelIdIterType channelCountIndex;
  /* #10 For all the channels set the channel id to close as CDDDRM_CHANNEL_NOT_ASSIGNED and last state\
         as CDDDRM_CHANNEL_STATE_INACTIVE */
  for (channelCountIndex = 0U; channelCountIndex < channelCount; channelCountIndex++)
  {
    cancellationsToInvoke->cancelChannels[channelCountIndex].channelIdToClose = CDDDRM_CHANNEL_NOT_ASSIGNED;
    cancellationsToInvoke->cancelChannels[channelCountIndex].lastChannelState = CDDDRM_CHANNEL_STATE_INACTIVE;
  }
  cancellationsToInvoke->lnoOfChannelsToClose = 0;
}

/**********************************************************************************************************************
* CddDrm_AddToCancellationList()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_AddToCancellationList(
                                CddDrm_ChannelIdType channelId,
                                P2VAR(CddDrm_CancellationStructType, AUTOMATIC, AUTOMATIC) cancellationsToInvoke)
{
  /* #10 Set the invocation status for channel Id and last state and increments the number of channels to be closed. */
  cancellationsToInvoke->cancelChannels[cancellationsToInvoke->lnoOfChannelsToClose].channelIdToClose = (CddDrm_ChannelIdType)channelId;             /* VCA_CDDDRM_CANCELLATIONLIST_WRITE */
  cancellationsToInvoke->cancelChannels[cancellationsToInvoke->lnoOfChannelsToClose].lastChannelState = CddDrm_GetChannelState(channelId);           /* VCA_CDDDRM_CANCELLATIONLIST_WRITE */
  cancellationsToInvoke->lnoOfChannelsToClose++;
}

/**********************************************************************************************************************
* CddDrm_CancellationToLowerLayer()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_CancellationToLowerLayer(
                                   P2CONST(CddDrm_CancellationStructType, AUTOMATIC, AUTOMATIC) cancellationToInvoke)
{
  CddDrm_ChannelIdIterType lInvocationIndex;

  /* #10 Loop through the amount of channels to close */
  for (lInvocationIndex = 0; lInvocationIndex < cancellationToInvoke->lnoOfChannelsToClose; ++lInvocationIndex)
  {
    /* #20 If Cancel Receive is enabled send a cancellation of reception to Lower Layer */
#  if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
    if (cancellationToInvoke->cancelChannels[lInvocationIndex].lastChannelState == CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING)
    {
      (void)PduR_CddDrmCancelReceive(CddDrm_GetConnectionResPduId(cancellationToInvoke->cancelChannels[lInvocationIndex].channelIdToClose));
    }
#  endif

    /* #30 If Cancel Transmit is enabled send a cancellation of transmission to Lower Layer */
#  if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
    if (cancellationToInvoke->cancelChannels[lInvocationIndex].lastChannelState == CDDDRM_CHANNEL_STATE_REQUEST_PROCESSING)
    {
      (void)PduR_CddDrmCancelTransmit(CddDrm_GetConnectionReqPduId(cancellationToInvoke->cancelChannels[lInvocationIndex].channelIdToClose));
    }
#  endif
  }
}
# endif
#endif
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
/**********************************************************************************************************************
*  CddDrm_GetChannelIdFromMetaData()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_GetChannelIdFromMetaData(P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) pduInfo)
{
  CddDrm_ChannelIdType     lChannelId = 0xFFU;

  CddDrm_ChannelIdIterType lChannelIdIter;
  for (lChannelIdIter = 0U; lChannelIdIter < CDDDRM_CFG_CHANNEL_COUNT; lChannelIdIter++)
  {
    if (CddDrm_IsChannelActive(lChannelIdIter) && CddDrm_IsChannelDynamicConnection(lChannelIdIter))
    {
      Std_ReturnType isResponseMetaDataMatch = CddDrm_LocalMemCmp(pduInfo->SduDataPtr, &CddDrm_GetChannelResponseMetaData(lChannelIdIter, 0u), CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN);
      Std_ReturnType isRequestMetaDataMatch = CddDrm_LocalMemCmp(pduInfo->SduDataPtr, &CddDrm_GetChannelRequestMetaData(lChannelIdIter, 0u), CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN);

      if ((isResponseMetaDataMatch == E_OK)
        || (isRequestMetaDataMatch == E_OK))
      {
        lChannelId = (CddDrm_ChannelIdType)lChannelIdIter;
        break;
      }
    }
  }
  return lChannelId;
}

/**********************************************************************************************************************
*  CddDrm_GetChannelIdFromRxPduId()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_GetChannelIdFromRxPduId(PduIdType rxPduId)
{
  CddDrm_ChannelIdType lChannelId = 0xFFU;
  CddDrm_ChannelIdIterType lChannelIdIter;

# if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  lChannelIdIter = 1U;
# else
  lChannelIdIter = 0U;
# endif

  /* #10 Loop through physical channels */
  for ( ; lChannelIdIter < CDDDRM_CFG_CHANNEL_COUNT; lChannelIdIter++)
  {
    /* #20 When a matching channel has the RxPduId stored, break loop and return the channelId. */
    if ((CddDrm_IsChannelActive(lChannelIdIter)) && (Cdddrm_GetChannelRxPduId(lChannelIdIter) == rxPduId))
    {
      lChannelId = (CddDrm_ChannelIdType)lChannelIdIter;
      break;
    }
  }
  return lChannelId;
}
#endif /* CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  CALLBACK FUNCTIONS
*********************************************************************************************************************/

/**********************************************************************************************************************
 * CddDrm_StartOfReception()
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
/* PRQA S 6080, 6030 1 */ /* MD_MSR_STMIF, MD_MSR_STCYC */
FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_StartOfReception(PduIdType id,
                                                             P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,                                   /* PRQA S 3673 */ /* MD_CDDDRM_Rule8.13_3673 */
                                                             PduLengthType TpSduLength,
                                                             P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (id >= CDDDRM_CFG_RXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (bufferSizePtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
# if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_GetRxPduRefToMetaDataLength(id) != 0U) && (info == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByRxPdu(id)];

    /* #20 If functional requests are enabled and active, process the reception */
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    if ((CddDrm_State == CDDDRM_STATE_FUNCTIONAL_REQUEST_ACTIVE) &&
      (CddDrm_GetChannelState(CDDDRM_CHANNEL_FUNCTIONAL_INDEX) == CDDDRM_CHANNEL_STATE_WAIT_RESPONSE))                                               /* COV_CDDDRM_ROBUSTNESS */
    {
      lResult = CddDrm_ProcessStartOfReception(id, lChannelId, info, TpSduLength, bufferSizePtr);                                                    /* VCA_CDDDRM_FUNCTIONAL_RESPONSE_CHANNEL */
    }
    else
#endif
    {
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
      /* #30 Otherwise, if dynamic connection is enabled and metadata length is configured for RxPduId  */
      if (CddDrm_GetRxPduRefToMetaDataLength(id) != 0U)
      {
        /* #40 Retrieve the channel which has the same metadata stored */
        lChannelId = CddDrm_GetChannelIdFromMetaData(info);
        if (CddDrm_IsChannelValid(lChannelId))
        {
          Cdddrm_SetChannelRxPduId(lChannelId, id);
        }
      }
#endif
    /* #50 Check if the physical channel is valid */
      if (CddDrm_IsChannelValid(lChannelId))
      {
        /* #60 If semaphore is allocated, process the reception */
        if (CddDrm_AllocateSemaphore(lChannelId, CDDDRM_CHANNEL_STATE_WAIT_RESPONSE))
        {
          lResult = CddDrm_ProcessStartOfReception(id, lChannelId, info, TpSduLength, bufferSizePtr);
          CddDrm_ReleaseSemaphore(lChannelId);
        }
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
        else
        {
          lErrorId = CDDDRM_E_INVALID_STATE;
        }
#endif
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #70 Otherwise, return development error */
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_START_OF_RECEPTION, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  CDDDRM_DUMMY_STATEMENT_CONST(info);                                                                                                                /* PRQA S 3206 */ /* MD_MSR_DummyStmt */

  return lResult;
}

/**********************************************************************************************************************
 * CddDrm_CopyRxData()
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
FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_CopyRxData(PduIdType id,
                                                       P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                       P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (id >= CDDDRM_CFG_RXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if ((info == NULL_PTR) || (bufferSizePtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByRxPdu(id)];

    /* #20 If dynamic connection is enabled and metadata length is configured for RxPduId */
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
    /* If metadata is configured for RxPdu,  */
    if ((CddDrm_GetRxPduRefToMetaDataLength(id) != 0U)
# if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
       && (!CddDrm_StateMgrIsFuncReqActive())
# endif
      )
    {
      /* #30 Retrieve the channel which has the RxPduId stored. */
      lChannelId = CddDrm_GetChannelIdFromRxPduId(id);
    }
#endif

    /* #40 If channel of the Pdu id is valid */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      /* #50 Check if semaphore is allocated to the channel, copy the data and release the semaphore */
      if (CddDrm_AllocateSemaphore(lChannelId, CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING))
      {
        lResult = CddDrm_ProcessCopyRxData(lChannelId, info, bufferSizePtr);
        CddDrm_ReleaseSemaphore(lChannelId);
      }
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
      else
      {
        lErrorId = CDDDRM_E_INVALID_STATE;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #60 Otherwise, return development error */
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_COPY_RX_DATA, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return lResult;
}

/**********************************************************************************************************************
* CddDrm_TpRxIndication()
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
FUNC(void, CDDDRM_CODE) CddDrm_TpRxIndication(PduIdType id,
                                              Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (id >= CDDDRM_CFG_RXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PDU_ID_RX_OUT_OF_RANGE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByRxPdu(id)];

    /* #20 If dynamic connection is enabled and metadata length is configured for RxPduId */
#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
    /* If metadata is configured for RxPdu,  */
    if ((CddDrm_GetRxPduRefToMetaDataLength(id) != 0U)
# if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
       && (!CddDrm_StateMgrIsFuncReqActive())
# endif
      )
    {
      /* #30 Retrieve the channel which has the RxPduId stored. */
      lChannelId = CddDrm_GetChannelIdFromRxPduId(id);
    }
#endif

    /* #40 If the Pdu id is assigned to an active channel */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      /* #50 If semaphore is allocated to the channel, process the result and release semaphore */
      if (CddDrm_AllocateSemaphore(lChannelId, CDDDRM_CHANNEL_STATE_RESPONSE_PROCESSING))
      {
        CddDrm_ProcessTpRxIndication(lChannelId, result);
        CddDrm_ReleaseSemaphore(lChannelId);
      }
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
      else
      {
        lErrorId = CDDDRM_E_INVALID_STATE;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #60 Otherwise, return development error */
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_TP_RX_INDICATION, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 * CddDrm_CopyTxData()
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
FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_CopyTxData(PduIdType id,
                                                       P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                       P2VAR(RetryInfoType, AUTOMATIC, CDDDRM_APPL_DATA) retry,                                      /* PRQA S 3673 */ /* MD_CDDDRM_Rule8.13_3673 */
                                                       P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) availableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (id >= CDDDRM_CFG_TXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if ((retry != NULL_PTR) && (retry->TpDataState == TP_DATARETRY))
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if ((info == NULL_PTR) || (availableDataPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else
#endif
  {
    CddDrm_ChannelIdType lChannelId;
    CddDrm_ConnectionIdType lConnectionId = CddDrm_Cfg_GetConnectionIdByTxPdu(id);

    /* ----- Implementation ----------------------------------------------- */
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    if ((CddDrm_IsFunctionalConnection(lConnectionId)) && (CddDrm_FuncReq_TesterPresent[CddDrm_GetFunctionalDelayRequestIdx(lConnectionId)] == TRUE))
    {
      lConnectionId = CddDrm_GetTesterPresentChannelAssignIdx(lConnectionId);
    }
#endif

    lChannelId = CddDrm_ChannelAssign[lConnectionId];

    /* #20 If the Pdu id is assigned to an active channel */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      /* #30 If allocation of semaphore is successful, copy the data and release the semaphore */
      if (CddDrm_AllocateSemaphore(lChannelId, CDDDRM_CHANNEL_STATE_REQUEST_PROCESSING))
      {
        lResult = CddDrm_ProcessCopyTxData(lChannelId, info, availableDataPtr);
        CddDrm_ReleaseSemaphore(lChannelId);
      }
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
      else
      {
        lErrorId = CDDDRM_E_INVALID_STATE;
      }
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #40 Otherwise, return development error */
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_COPY_TX_DATA, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  CDDDRM_DUMMY_STATEMENT_CONST(retry);                                    /* PRQA S 3206 */ /* MD_MSR_DummyStmt */

  return lResult;
}

/**********************************************************************************************************************
* CddDrm_TpTxConfirmation()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
FUNC(void, CDDDRM_CODE) CddDrm_TpTxConfirmation(PduIdType id,
                                                Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (id >= CDDDRM_CFG_TXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else
#endif
  {
    CddDrm_ChannelIdType lChannelId;
    CddDrm_ConnectionIdType lConnectionId = CddDrm_Cfg_GetConnectionIdByTxPdu(id);

    /* ----- Implementation ----------------------------------------------- */
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    if ((CddDrm_IsFunctionalConnection(lConnectionId)) && (CddDrm_FuncReq_TesterPresent[CddDrm_GetFunctionalDelayRequestIdx(lConnectionId)] == TRUE))
    {
      lConnectionId = CddDrm_GetTesterPresentChannelAssignIdx(lConnectionId);
    }
#endif
    lChannelId = CddDrm_ChannelAssign[lConnectionId];

    /* #20 If Pdu id is assigned to an active channel */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      /* #30 If allocation of semaphore is successful, process tx confirmation and release the semaphore. */
      if (CddDrm_AllocateSemaphore(lChannelId, CDDDRM_CHANNEL_STATE_REQUEST_PROCESSING))
      {
        CddDrm_ProcessTpTxConfirmation(lChannelId, result);
        CddDrm_ReleaseSemaphore(lChannelId);
      }
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
      else
      {
        lErrorId = CDDDRM_E_INVALID_STATE;
      }
#endif
    }
  }
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #40 Otherwise, return development error */
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_TP_TX_CONFIRMATION, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

}

#if ((CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON) && (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON))
/**********************************************************************************************************************
* CddDrm_NvM_JobFinished()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_JobFinished(uint8 serviceId,
                                                         NvM_RequestResultType jobResult)
{
  /* ----- Implementation ----------------------------------------------- */
  /* JobResult is not used, because next time Ecu Detection is started,
   * Nvm is triggered to write again and before detection list was initialized again.
   */
  CDDDRM_DUMMY_STATEMENT(jobResult);                                                                                                                 /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  if ((serviceId == NVM_WRITE_BLOCK)
    && (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_WRITE_NVM) )
  {
# if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_FINISH;
# else
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
# endif
  }
  return E_OK;
}
#endif

/**********************************************************************************************************************
*  SERVICE FUNCTIONS
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  CddDrm_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  CddDrm_ModuleInitialized = CDDDRM_UNINIT;

#if (CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION == STD_ON)
  CddDrm_InitLowerLayerMemory();
#endif

} /* CddDrm_InitMemory() */

/**********************************************************************************************************************
 * CddDrm_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_Init(P2CONST(CddDrm_ConfigPtrType, AUTOMATIC, CDDDRM_PBCFG) configPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_ConnectionTableIterType lConnectionIndex;
  CddDrm_ChannelIdIterType lChannelId;

  CDDDRM_DUMMY_STATEMENT_CONST(configPtr);                                                                                                           /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_INIT)
  {
    lErrorId = CDDDRM_E_ALREADY_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ModuleInitialized        = (uint8)CDDDRM_INIT;
    /* Initial DRM State */
    CddDrm_State                    = CDDDRM_STATE_RUN;
    CddDrm_Queue_TesterConnected    = CDDDRM_QUEUE_EMPTY;
    CddDrm_ActiveChannels           = 0;
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
    CddDrm_Ecud_State               = CDDDRM_ECUD_STATE_IDLE;
    CddDrm_Queue_Ecud               = CDDDRM_QUEUE_EMPTY;

    if(CddDrm_Ecud_GetRamBlockInitPattern() != CDDDRM_CFG_ECUD_INIT_PATTERN)
    {
      (void)CddDrm_NvM_InitEcuDetectionData();
    }
#endif

    /* At startup there must be no delay between requests */
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
    CddDrm_RequestDelayTimer        = 0;
    CddDrm_NextConnection           = 0;
#endif

    /* Initialize list, so connection channels can be assigned */
    for (lConnectionIndex = 0; lConnectionIndex < CDDDRM_CFG_CONNECTION_COUNT; lConnectionIndex++)
    {
      CddDrm_ChannelAssign[lConnectionIndex] = CDDDRM_CHANNEL_NOT_ASSIGNED;
#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
      CddDrm_SeparationTimer[lConnectionIndex] = 0;
#endif
    }

    for(lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
    {
      CddDrm_Closing_Semaphore[lChannelId] = FALSE;
      CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_INACTIVE);
    }
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    for (lConnectionIndex = 0; lConnectionIndex < CDDDRM_CFG_FUNC_CONNECTION_COUNT; lConnectionIndex++)
    {
      CddDrm_ChannelAssign[lConnectionIndex + CDDDRM_CFG_CONNECTION_COUNT] = CDDDRM_CHANNEL_NOT_ASSIGNED;
      CddDrm_FuncReq_TesterPresent[lConnectionIndex] = FALSE;
      CddDrm_FuncRequestProcessing[lConnectionIndex] = FALSE;
# if (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON)
      CddDrm_FuncRequestTimer[lConnectionIndex] = 0;
# endif
    }
    CddDrm_FuncReqMgrResetRespResult();
#endif

#if (CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION == STD_ON)
    CddDrm_InitLowerLayer();
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_INIT, lErrorId);
  }
#else
    CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
} /* CddDrm_Init() */

/**********************************************************************************************************************
 * CddDrm_ExternalTesterConnected()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_ExternalTesterConnected(boolean present)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lReturnValue = E_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* Check if module is uninitialized */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
    lReturnValue = E_NOT_OK;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Drm have immediately go to TesterPresent mode and close all open connections */
    /* Check if external tester is connected or disconnected */
    if (present == TRUE)
    {
      CddDrm_Queue_TesterConnected = CDDDRM_QUEUE_EXTERNAL_TESTER_CONNECTED;
    }
    else
    {
      CddDrm_Queue_TesterConnected = CDDDRM_QUEUE_EXTERNAL_TESTER_DISCONNECTED;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_EXTERNAL_TESTER_CONNECTED, lErrorId);
  }
#else
    CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  return lReturnValue;
} /* CddDrm_ExternalTesterConnected() */

/**********************************************************************************************************************
 * CddDrm_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STMIF */
FUNC(void, CDDDRM_CODE) CddDrm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
/**************************************************************
 * External tester detection Queue
 *************************************************************/
    CddDrm_Queue_ExternalTester();

/**************************************************************
 * Ecu Detection Start/Stop Queue
 *************************************************************/
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
    CddDrm_Queue_EcuDetection();
#endif

#if ((CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON) || (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON) || (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_ON))
/**************************************************************
 * [Separation time]/[global request delay] and P2/P2* handling
 *************************************************************/
    CddDrm_Timer_Task();
#endif

    if (CddDrm_ActiveChannelExist())
    {
#if ((CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_OFF) && (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_OFF) && (CDDDRM_CFG_SUPPORT_P3_FUNCTIONAL_DELAY_TIMER == STD_OFF))
/**************************************************************
 * Only P2/P2* handling
 *************************************************************/
      CddDrm_Timer_Task();
#endif
/**************************************************************
 * Channel state machine handler
 *************************************************************/
      CddDrm_Cm_StateTask();
    }

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**************************************************************
 * ECU Detection state machine handler
 *************************************************************/
    CddDrm_Ecud_StateTask();
#endif

/**************************************************************
 * Handle channel closing
 *************************************************************/
    CddDrm_HandleChannelClosing();


#if (CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION == STD_ON)
/**************************************************************
 * Main Function of the Lower Layer
 *************************************************************/
    CddDrm_MainFunctionLowerLayer();

#endif

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_MAINFUNCTION, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
} /* CddDrm_MainFunction() */

/**********************************************************************************************************************
 * CddDrm_SvcSend()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend(CddDrm_ConnectionIdType connectionId,
                                                           boolean sprmib,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo,
                                                           CddDrm_LengthType requestLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->requestBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < 3U)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (requestLength == 0U)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else if ((bufferInfo->requestBufferDataPtr[0U] & CDDDRM_RESPONSE_CODE_VALIDATION) > 0U) /* check for valid UDS Service (response bit not set) */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (bufferInfo->requestBufferDataPtr[0U] == (0x3fU)) /* 0x3F | 0x40 = 0x7F -> NRC */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
# if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
#  if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#  endif
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId)) && (bufferInfo->responseBufferSize < (3U + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId)) && (bufferInfo->requestMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType  lChannelId;
    boolean               lFunctionalTesterPresent = FALSE;

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    /* If the request is a functional 3E with SPRMIB set then lFunctionalTesterPresent shall be set to indicate it in ChannelGet functionality */
    if ((sprmib == TRUE)
      && (CddDrm_IsFunctionalConnection(connectionId))
      && (bufferInfo->requestBufferDataPtr[0U] == CDDDRM_SERVICE_3E))
    {
      lFunctionalTesterPresent = TRUE;
    }
#endif

    lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, lFunctionalTesterPresent, bufferInfo->requestBufferDataPtr[0U], &lResult);
    /* If a channel was allocated: initialize channel parameter */
    if (CddDrm_IsChannelValid(lChannelId))
    {                                                                                                                                                /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
      CddDrm_Cm_ChannelInit(lChannelId,
                            bufferInfo->requestBufferDataPtr[0U],
                            &CddDrm_Cfg_ResponseNotification,
                            requestLength,
                            sprmib,
                            lFunctionalTesterPresent,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

#if (CDDDRM_CFG_SUPPORT_DEDICATED_APIS == STD_ON)
      CddDrm_SetChannelProtocolLength(lChannelId, 0U);
#endif
      CddDrm_ResetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  return lResult;
}

#if (CDDDRM_CFG_SUPPORT_SERVICE_10 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_10()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_10(CddDrm_ConnectionIdType connectionId,
                                                              uint8 sessionType,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                     lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType  lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_10_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( (sessionType == 0x00U)
       || ( (sessionType > 0x04U) && (sessionType < 0x40U) )
       ||   (sessionType > 0x7eU) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_10_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_10, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     * message by setting index position
     * [0]: 0x10 (SID)
     * [1]: SessionType | SPRMIB (SubFunction)
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_10,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_10,
                            sprmib,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

      /* set protocol buffer data, service id at index 0 already set */
      if (sprmib == TRUE)
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(sessionType);
      }
      else
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(sessionType);                         /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
      }
      CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_10);

      /* set bit for using protocol buffer in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_10, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_11 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_11()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_11(CddDrm_ConnectionIdType connectionId,
                                                              uint8 resetType,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                     lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType  lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  /* If module is initialized and all parameter checks passed */
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_11_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( (resetType == 0x00U)
       || ( (resetType > 0x05U) && (resetType < 0x40U) )
       ||   (resetType > 0x7eU) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_11_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_11, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     * message by setting index position
     * [0]: 0x11 (SID)
     * [1]: ResetType | SPRMIB (SubFunction)
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_11,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_11,
                            sprmib,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

      /* set protocol buffer data, service id at index 0 already set */
      if (sprmib == TRUE)
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(resetType);
      }
      else
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(resetType);                           /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
      }

      CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_11);

      /* set bit for using protocol buffer in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_11, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_1902 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_1902()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1902(CddDrm_ConnectionIdType connectionId,
                                                                uint8 dtcStatusMask,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                     lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType  lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_1902_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_1902_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_19, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     * message by setting index position
     * [0]: 0x19 (SID)
     * [1]: 0x02 (SubFunction)
     * [2]: DTCStatusMask
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_19,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_1902,
                            FALSE,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);
      /* set protocol buffer data, service id at index 0 already set */
      CddDrm_ChannelInfo[lChannelId].protocolData[1U] = 0x02U; /* sub-function */
      CddDrm_ChannelInfo[lChannelId].protocolData[2U] = dtcStatusMask;
      CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_1902);
      /* set bit for using protocol buffer in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_1902, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_1904 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_1904()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1904(CddDrm_ConnectionIdType connectionId,
                                                                uint32 dtc,
                                                                uint8 recordNumber,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_1904_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (dtc >(0x00ffffffU))
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_1904_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_19, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     * message by setting index position
     * [0]: 0x19 (SID)
     * [1]: 0x04 (SubFunction)
     * [2]: DTC  (High Low Byte)
     * [3]: DTC  (Low High Byte)
     * [4]: DTC  (Low Low Byte)
     * [5]: RecordNumber
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_19,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_1904,
                            FALSE,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);
      /* set protocol buffer data, service id at index 0 already set */
      CddDrm_ChannelInfo[lChannelId].protocolData[1U] = 0x04U; /* sub-function */
      CddDrm_ChannelInfo[lChannelId].protocolData[2U] = CddDrm_GetHiLoByte(dtc);
      CddDrm_ChannelInfo[lChannelId].protocolData[3U] = CddDrm_GetLoHiByte(dtc);
      CddDrm_ChannelInfo[lChannelId].protocolData[4U] = CddDrm_GetLoLoByte(dtc);
      CddDrm_ChannelInfo[lChannelId].protocolData[5U] = recordNumber;
      CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_1904);
      /* set bit for using protocol buffer in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_1904, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_22 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_22()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_22(CddDrm_ConnectionIdType connectionId,
                                                              uint16 DID,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_22_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_22_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_22, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     *     message by setting index position
     *     [0]: 0x22 (SID)
     *     [1]: DID (High Byte)
     *     [2]: DID (Low Byte)
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_22,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_22,
                            FALSE,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);
      /* set protocol buffer data, service id at index 0 already set */
      CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CddDrm_GetHiByte(DID);
      CddDrm_ChannelInfo[lChannelId].protocolData[2U] = CddDrm_GetLoByte(DID);
      CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_22);
      /* set bit for using protocol buffer in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_22, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_27 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_27()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_27(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              CddDrm_LengthType dataLength,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_27_MIN_RESPONSE_BUFFER) /* minimum response length */
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ((bufferInfo->requestBufferDataPtr == NULL_PTR) && (dataLength > 0U))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ( (subFunction == 0x00U)
       || ( (subFunction > 0x42U) && (subFunction < 0x5fU) )
       ||   (subFunction > 0x7eU) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  /* if subfunction is a even number (security access send key) it must have a minimum request length of 3 byte */
  else if ( ((subFunction % 2U) == 0U) && ((CDDDRM_PROTOCOL_LENGTH_27 + dataLength) < 3U) )
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  } /* else: minimum length is protocol length */
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_27_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_27, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     *     message by setting index position
     *     [0]: 0x27 (SID)
     *     [1]: SubFunction | SPRMIB
     *     [2]-[x]: securityAccessDataRecord - application payload with given length
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_27,
                            &CddDrm_Cfg_ResponseNotification,
                            (CDDDRM_PROTOCOL_LENGTH_27 + dataLength),
                            sprmib,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);
      /* set protocol buffer data, service id at index 0 already set */
      if (sprmib == TRUE)                                                                                                                            /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(subFunction);
      }
      else
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(subFunction);                         /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
      }
      CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_27);
      /* set bit for using protocol buffer in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_27, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_28 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_28()
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
/* PRQA S 6030, 6060, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_28(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              uint8 communicationType,
                                                              uint16 nodeIdNumber,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_28_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( ( (subFunction > 0x05U)
           && (subFunction < 0x40U) )
           || (subFunction > 0x7eU)) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_28_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_28, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     *     message by setting index position
     *     [0]: 0x28 (SID)
     *     [1]: SubFunction | SPRMIB
     *     [2]: CommunicationType
     *     [3]: NodeNumber (High Byte) for SubFunction 0x04 or 0x05
     *     [4]: NodeNumber (Low Byte) for SubFunction 0x04 or 0x05
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      /* if sub-function is 0x04 or 0x05 NodeIdNumber must be set, so request length increases */
      if ( (subFunction == 0x04U)
        || (subFunction == 0x05U))
      {
        CddDrm_Cm_ChannelInit(lChannelId,
                              CDDDRM_SERVICE_28,
                              &CddDrm_Cfg_ResponseNotification,
                              CDDDRM_PROTOCOL_LENGTH_28 + 2u,
                              sprmib,
                              FALSE,
                              CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                             /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                              bufferInfo);
        CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_28 + 2u);
        CddDrm_ChannelInfo[lChannelId].protocolData[3U] = CddDrm_GetHiByte(nodeIdNumber);
        CddDrm_ChannelInfo[lChannelId].protocolData[4U] = CddDrm_GetLoByte(nodeIdNumber);
      }
      else
      {
        CddDrm_Cm_ChannelInit(lChannelId,
                              CDDDRM_SERVICE_28,
                              &CddDrm_Cfg_ResponseNotification,
                              CDDDRM_PROTOCOL_LENGTH_28,
                              sprmib,
                              FALSE,
                              CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                             /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                              bufferInfo);
        CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_28);
      }
      /* set protocol buffer data, service id at index 0 already set */
      if (sprmib == TRUE)
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(subFunction);
      }
      else
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(subFunction);                         /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
      }
      CddDrm_ChannelInfo[lChannelId].protocolData[2U] = communicationType;
      /* set bit in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_28, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_31 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_31()
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
/* PRQA S 6030, 6060, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_31(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              uint16 routineId,
                                                              CddDrm_LengthType routineOptionLength,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_31_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ((bufferInfo->requestBufferDataPtr == NULL_PTR) && (routineOptionLength > 0U))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ( (subFunction == 0x00U)
         || (subFunction > 0x03U) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_31_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_31, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     *     message by setting index position
     *     [0]: 0x31 (SID)
     *     [1]: SubFunction | SPRMIB
     *     [2]: RoutineId (High Byte)
     *     [3]: RoutineId (Low Byte)
     *     [4]-[x]: routineControlOptionRecord - application payload with given length
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      /* routine control options are optional if sub-function is start-/stop routine */
      if ((subFunction == 0x01U) || (subFunction == 0x02U))
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_Cm_ChannelInit(lChannelId,
                              CDDDRM_SERVICE_31,
                              &CddDrm_Cfg_ResponseNotification,
                              CDDDRM_PROTOCOL_LENGTH_31 + routineOptionLength,
                              sprmib,
                              FALSE,
                              CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                             /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                              bufferInfo);
      }
      else
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_Cm_ChannelInit(lChannelId,
                              CDDDRM_SERVICE_31,
                              &CddDrm_Cfg_ResponseNotification,
                              CDDDRM_PROTOCOL_LENGTH_31,
                              sprmib,
                              FALSE,
                              CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                             /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                              bufferInfo);
      }

      CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_31);
      /* set protocol buffer data, service id at index 0 already set */
      if (sprmib == TRUE)
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(subFunction);
      }
      else
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(subFunction);                         /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
      }
      CddDrm_ChannelInfo[lChannelId].protocolData[2U] = CddDrm_GetHiByte(routineId);
      CddDrm_ChannelInfo[lChannelId].protocolData[3U] = CddDrm_GetLoByte(routineId);
      /* set bit in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_31, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_34 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_34()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_34(CddDrm_ConnectionIdType connectionId,
                                                              uint8 dataFormatId,
                                                              uint8 addressAndLength,
                                                              CddDrm_LengthType dataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_34_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ((bufferInfo->requestBufferDataPtr == NULL_PTR) && (dataLength > 0U))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((dataLength + CDDDRM_PROTOCOL_LENGTH_34) < 5U)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_34_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_34, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     * message by setting index position
     *   [0]: 0x34 (SID)
     *   [1]: DataFormatId
     *   [2]: AdressAndLength
     *   [3]-[x]: MemoryAddress and MemorySize - application payload with given length
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_34,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_34 + dataLength,
                            FALSE,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

      CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_34);
      /* set protocol buffer data, service id at index 0 already set */
      CddDrm_ChannelInfo[lChannelId].protocolData[1U] = dataFormatId;
      CddDrm_ChannelInfo[lChannelId].protocolData[2U] = addressAndLength;
      /* set bit in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_34, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_36 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_36()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_36(CddDrm_ConnectionIdType connectionId,
                                                              uint8 blockSeqCounter,
                                                              CddDrm_LengthType transferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((bufferInfo->requestBufferDataPtr == NULL_PTR) && (transferDataLength > 0U))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_36_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_36_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_36, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     *     message by setting index position
     *     [0]: 0x36 (SID)
     *     [1]: BlockSequenceCounter
     *     [2]-[x]: TransferRequestParameterRecord - application payload with given length
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_36,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_36 + transferDataLength,
                            FALSE,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

      CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_36);
      CddDrm_ChannelInfo[lChannelId].protocolData[1U] = blockSeqCounter;
      /* set protocol buffer data, service id at index 0 already set */
      /* set bit in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_36, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_37 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_37()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_37(CddDrm_ConnectionIdType connectionId,
                                                              CddDrm_LengthType transferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((bufferInfo->requestBufferDataPtr == NULL_PTR) && (transferDataLength > 0U))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_37_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_37_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_37, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     *     message by setting index position
     *     [0]: 0x37 (SID)
     *     [1]-[x]: TransferRequestParameterRecord - application payload with given length
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_37,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_37 + transferDataLength,
                            FALSE,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

      /* service id at index 0 already set */
      CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_37);
      /* set bit in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_37, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_3E == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_3E()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_3E(CddDrm_ConnectionIdType connectionId,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                     lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType  lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_3E_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_3E_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    CddDrm_ChannelIdType  lChannelId;
    boolean               lFunctionalTesterPresent = FALSE;

    /* ----- Implementation ----------------------------------------------- */
# if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    if ((CddDrm_IsFunctionalConnection(connectionId)) && (sprmib == TRUE))
    {
      lFunctionalTesterPresent = TRUE;
    }
# endif

    lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, lFunctionalTesterPresent, CDDDRM_SERVICE_3E, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     * message by setting index position
     * [0]: 0x3E (SID)
     * [1]: SPRMIB
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_3E,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_3E,
                            sprmib,
                            lFunctionalTesterPresent,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

      CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_3E);

      /* set bit in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;

      /* service id at index 0 already set */
      if (TRUE == sprmib)
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(0U);
      }
      else
      {
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(0U);
      }
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_3E, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_85 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_85()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_85(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              CddDrm_LengthType recordLength,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (bufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((bufferInfo->requestBufferDataPtr == NULL_PTR) && (recordLength > 0u))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (bufferInfo->responseBufferSize < CDDDRM_SERVICE_85_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( (subFunction == 0U)
       || ( (subFunction > 0x02U) && (subFunction < 0x40U) ) /* ISOSAE Reserved sub-functions */
       ||   (subFunction > 0x7eU) )
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
#  if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && ((bufferInfo->responseMetaDataBufferPtr == NULL_PTR)
    || (bufferInfo->requestMetaDataBufferPtr == NULL_PTR)))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((CddDrm_IsDynamicConnectionOfConnectionTable(connectionId))
    && (bufferInfo->responseBufferSize < (CDDDRM_SERVICE_85_MIN_RESPONSE_BUFFER + CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)))
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
#   if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  else if ((CddDrm_IsFunctionalConnection(connectionId)) && (bufferInfo->responseMetaDataBufferPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(connectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION, FALSE, CDDDRM_SERVICE_85, &lResult);

    /* If a channel was allocated: initialize channel parameter, set request length and assemble the request
     * message by setting index position
     * [0]: 0x85 (SID)
     * [1]: SubFunction | SPRMIB
     * [2]-[x]: DTCSettingControlOptionRecord - application payload with given length
     */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_Cm_ChannelInit(lChannelId,
                            CDDDRM_SERVICE_85,
                            &CddDrm_Cfg_ResponseNotification,
                            CDDDRM_PROTOCOL_LENGTH_85 + recordLength,
                            sprmib,
                            FALSE,
                            CddDrm_IsDynamicConnectionOfConnectionTable(connectionId),                                                               /* PRQA S 2996 */ /* MD_CDDDRM_Rule2.2_2996 */
                            bufferInfo);

      CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_85);
      /* service id at index 0 already set */
      if (sprmib == TRUE)
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(subFunction);
      }
      else
      {                                                                                                                                              /* PRQA S 0715 */ /* MD_MSR_Dir1.1 */
        CddDrm_ChannelInfo[lChannelId].protocolData[1U] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(subFunction);                         /* PRQA S 2985 */ /* MD_CDDDRM_Rule2.2_2985 */
      }
      /* set bit in channel state */
      CddDrm_SetProtocolBufferActive(lChannelId);
      lResult = CDDDRM_REQUEST_OK;
      CddDrm_Cm_SetInitialChannelState(lChannelId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_85, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_CancelRequest()
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
/* PRQA S 6080, 6030 1 */ /* MD_MSR_STMIF, MD_MSR_STCYC */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelRequest(CddDrm_ConnectionIdType connectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  Std_ReturnType lReturnValue = E_NOT_OK;
  CddDrm_ChannelIdIterType lChannelId;

# if ((CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON))
  CddDrm_CancellationStructType lCancellationsToInvoke;
#  if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
  CddDrm_InitializeCancellationStruct(&lCancellationsToInvoke, CDDDRM_CFG_CHANNEL_COUNT);
#  else
  /* 1U is used for physical channel cancellations since only one channel will be affected */
  CddDrm_InitializeCancellationStruct(&lCancellationsToInvoke, 1U);
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (connectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
# endif
  {
    CddDrm_ConnectionIdType lConnectionId = connectionId;
    /* ----- Implementation ----------------------------------------------- */
    /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
    CddDrm_EnterCritical_ChannelManager();

# if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    if ((CddDrm_IsFunctionalConnection(lConnectionId)) && (CddDrm_FuncReq_TesterPresent[CddDrm_GetFunctionalDelayRequestIdx(lConnectionId)] == TRUE))
    {
      lConnectionId = CddDrm_GetTesterPresentChannelAssignIdx(lConnectionId);
    }
# endif

    lChannelId = CddDrm_ChannelAssign[lConnectionId];

    /* #20 If connection is assigned to a channel: queue that the channel linked to the given connection\
           shall be cancelled */
    if (CddDrm_IsChannelValid(lChannelId))
    {
# if(CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
      /* #30 If the connectionId is a functional connection */
      if (CddDrm_IsFunctionalConnection(connectionId))
      {
        /* #40 If forced functional request is active, only cancel the functional channel.\
               Otherwise, cancel all active channels */
        if (CddDrm_GetChannelTesterPresent(lChannelId) == TRUE)
        {
#  if((CDDDRM_CFG_SUPPORT_CANCELRECEIVE) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT))
          CddDrm_AddToCancellationList((CddDrm_ChannelIdType)lChannelId,
                                                             &lCancellationsToInvoke);
#  endif
          CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL);
          CddDrm_SetChannelResponseCode(lChannelId, CDDDRM_RESPONSE_CHANNELS_CLOSED);
        }
        else
        {
          for (lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
          {
            if (CddDrm_IsChannelActive(lChannelId))
            {
#  if((CDDDRM_CFG_SUPPORT_CANCELRECEIVE) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT))
              CddDrm_AddToCancellationList((CddDrm_ChannelIdType)lChannelId,
                                                                 &lCancellationsToInvoke);
#  endif
              CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL);
              CddDrm_SetChannelResponseCode(lChannelId, CDDDRM_RESPONSE_CHANNELS_CLOSED);
            }
          }
          CddDrm_State = CDDDRM_STATE_RUN;
        }
      }
      else
# endif
      /* #50 Otherwise, cancel the physical channel */
      {
# if ((CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON))
        CddDrm_AddToCancellationList((CddDrm_ChannelIdType)lChannelId,
                                                           &lCancellationsToInvoke);
# endif
        CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_CLOSE_CHANNEL);
        CddDrm_SetChannelResponseCode(lChannelId, CDDDRM_RESPONSE_CHANNELS_CLOSED);
      }

      lReturnValue = E_OK;
    }
    /* #60 Otherwise, return E_NOT_OK */
    /* >>>> -------------------------------- Leave Critical Section: ChannelManager */
    CddDrm_LeaveCritical_ChannelManager();

# if ((CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON) || (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON))

    lChannelId = CddDrm_ChannelAssign[connectionId];

    /* #70 If channel for the connection id is valid, invoke cancellation to lower layer */
    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_CancellationToLowerLayer(&lCancellationsToInvoke);                                                                 /* PRQA S 2972, 2973 */ /* MD_CDDDRM_CONSTPOINTERUSAGE */
    }
# endif
}
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #80 Otherwise, return development error */
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANCEL_REQUEST, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lReturnValue;
}
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_NvM_InitEcuDetectionData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_InitEcuDetectionData(void)                                                                              /* PRQA S 1505 */ /* MD_MSR_Rule8.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lReturnValue = E_OK;
  uint8_least lElementIndex;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
    lReturnValue = E_NOT_OK;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* initialize ECU Detection data */
    for (lElementIndex = 0; lElementIndex < CDDDRM_CFG_ECUD_LIST_SIZE; lElementIndex++)
    {
      CddDrm_Cfg_EcuDetectionData.EcuList[lElementIndex] = 0;
    }
    CddDrm_Ecud_SetRamBlockInitPattern(CDDDRM_CFG_ECUD_INIT_PATTERN);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_INIT_ECU_DETECTION_DATA, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lReturnValue;
}
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_GetEcuDetectionResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(CddDrm_EcudStateType, CDDDRM_CODE) CddDrm_GetEcuDetectionResult(CddDrm_ConnectionIdType connectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_EcudStateType lResult;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  lResult = CDDDRM_ECUD_CONNECTION_NOT_DISCOVERED;
  /* If module is initialized */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (connectionId >= CDDDRM_CFG_PHYS_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* 20 Get detection result for given connection */
    /* read a byte from ECU Detection data array and extract relevant ECU state */
    lResult = CddDrm_Cfg_EcuDetectionData.EcuList[CDDDRM_ECUD_GET_CONNECTION_INDEX(connectionId)];
    lResult = (CddDrm_EcudStateType)((lResult >> CDDDRM_ECUD_GET_CONNECTION_POS(connectionId)) & CDDDRM_ECUD_RESULT_MASK);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_ECU_DETECTION_RESULT, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  /* Return connection specific detection result */
  return lResult;
} /* CddDrm_GetEcuDetectionResult() */
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_StartEcuDetection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StartEcuDetection(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lResult = E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else
# endif
  /* ----- Implementation ----------------------------------------------- */
  {
    /* Ecu detection is not running and external tester is not present */
    if ( (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_IDLE)
      && (CddDrm_State == CDDDRM_STATE_RUN) )
    {
      CddDrm_Queue_Ecud = CDDDRM_QUEUE_ECUD_START;
      lResult = E_OK;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_ECU_DETECTION_START, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
} /* CddDrm_StartEcuDetection() */
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_StopEcuDetection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StopEcuDetection(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_ECUD_STOP;
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_ECU_DETECTION_STOP, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return E_OK;
} /* CddDrm_StopEcuDetection() */
#endif

#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
 * CddDrm_GetFuncRequestResult()
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
FUNC(CddDrm_FuncReqResultStateType, CDDDRM_CODE) CddDrm_GetFuncRequestResult(
                                CddDrm_ConnectionIdType connectionId,
                                P2VAR(CddDrm_FuncReqRespInfoStructType, AUTOMATIC, CDDDRM_APPL_DATA) respInfoStruct)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_FuncReqResultStateType lResult = CDDDRM_FUNC_REQ_RESULT_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (connectionId >= CDDDRM_CFG_PHYS_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else if (respInfoStruct == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (respInfoStruct->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_BufferIdxType lBufferIdx = CddDrm_GetFuncRespBufferIdx(connectionId);
    if (lBufferIdx != CDDDRM_FUNCTIONAL_INVALIDRESPONSEIDX)                                                                                           /* COV_CDDDRM_FUNCRESPONSEBUFFER */
    {
      if (respInfoStruct->responseBufferSize < CddDrm_FuncReq_RespBuffer[lBufferIdx].responseBufferSize)
      {
        lResult = CDDDRM_FUNC_REQ_RESULT_BUFFER_TOO_SMALL;
      }
      else
      {
        respInfoStruct->responseCode = CddDrm_FuncReq_RespBuffer[lBufferIdx].responseCode;
        respInfoStruct->responseBufferSize = CddDrm_FuncReq_RespBuffer[lBufferIdx].responseBufferSize;
        CddDrm_MemCopy(respInfoStruct->responseBufferDataPtr,                                                                                          /* VCA_CDDDRM_MEMCPY_CALL_FUNCRESPONSEBUFFER */
          &CddDrm_FuncReq_RespBuffer[lBufferIdx].responseBuffer[0],
          CddDrm_FuncReq_RespBuffer[lBufferIdx].responseBufferSize);
        lResult = CDDDRM_FUNC_REQ_RESULT_OK;

        /* Set flag that buffer has been read out */
        CddDrm_FuncReq_RespBuffer[lBufferIdx].responseProvided = TRUE;
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_GET_FUNC_REQUEST_RESULT, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return lResult;
}
#endif

#if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_CanTpRxMetaData()
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
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CanTpRxMetaData(P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) NPduPayLoad,
                                                         P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) NPduMetaData,
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfNSduOutPtr,                       /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                         P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfFcNPduOutPtr,
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) PayloadOfFcNPduInOutPtr)                    /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  Std_ReturnType lRetVal = E_NOT_OK;

# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if ((NPduMetaData == NULL_PTR)
    || (NPduMetaData->SduDataPtr == NULL_PTR)
    || (MetaDataOfNSduOutPtr == NULL_PTR)
    || (MetaDataOfNSduOutPtr->SduDataPtr == NULL_PTR)
    || (MetaDataOfFcNPduOutPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((NPduMetaData->SduLength != CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)
    || (MetaDataOfNSduOutPtr->SduLength != CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN))
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
    if (CddDrm_StateMgrIsFuncReqActive())
    {
      uint8 lMetaDataOffset = CddDrm_GetChannelResponseMetaData(CDDDRM_CHANNEL_FUNCTIONAL_INDEX, CDDDRM_CHANNEL_FUNCTIONAL_SVC_INDEX);
      uint16 lRequestMetaData = ((uint16)NPduMetaData->SduDataPtr[1U] << 8) | NPduMetaData->SduDataPtr[0U];
      lRequestMetaData -= lMetaDataOffset;

      CddDrm_MemCopy(MetaDataOfNSduOutPtr->SduDataPtr, NPduMetaData->SduDataPtr, MetaDataOfNSduOutPtr->SduLength);                                   /* VCA_CDDDRM_MEMCPY_CALL_METADATA */

      MetaDataOfFcNPduOutPtr[0U] = CddDrm_GetLoByte(lRequestMetaData);
      MetaDataOfFcNPduOutPtr[1U] = CddDrm_GetHiByte(lRequestMetaData);

      lRetVal = E_OK;
    }
    else
# endif
    {
      CddDrm_ChannelIdType lChannelId = CddDrm_GetChannelIdFromMetaData(NPduMetaData);

      if (CddDrm_IsChannelValid(lChannelId))
      {
        CddDrm_MemCopy(MetaDataOfNSduOutPtr->SduDataPtr, &CddDrm_GetChannelResponseMetaData(lChannelId, 0U), MetaDataOfNSduOutPtr->SduLength);       /* VCA_CDDDRM_MEMCPY_CALL_METADATA */

        CddDrm_MemCopy(MetaDataOfFcNPduOutPtr, &CddDrm_GetChannelRequestMetaData(lChannelId, 0U), MetaDataOfNSduOutPtr->SduLength);                  /* VCA_CDDDRM_MEMCPY_CALL_METADATA */

        lRetVal = E_OK;
      }
    }
    CDDDRM_DUMMY_STATEMENT_CONST(NPduPayLoad);
    CDDDRM_DUMMY_STATEMENT_CONST(PayloadOfFcNPduInOutPtr);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANTP_RX_METADATA, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return lRetVal;
}

/**********************************************************************************************************************
 * CddDrm_CanTpTxMetaData()
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
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CanTpTxMetaData(P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfNSduPtr,                        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfDataNPduOutPtr,                   /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                         P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfFcNPduOutPtr,
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) PayloadOfDataNPduInOutPtr,                 /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
                                                         P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) PayloadOfFcNPduInOutPtr)                         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  Std_ReturnType lRetVal = E_NOT_OK;

# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* If module is initialized */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if ((MetaDataOfNSduPtr == NULL_PTR)
    || (MetaDataOfNSduPtr->SduDataPtr == NULL_PTR)
    || (MetaDataOfDataNPduOutPtr == NULL_PTR)
    || (MetaDataOfDataNPduOutPtr->SduDataPtr == NULL_PTR)
    || (MetaDataOfFcNPduOutPtr == NULL_PTR))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((MetaDataOfNSduPtr->SduLength != CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN)
    || (MetaDataOfDataNPduOutPtr->SduLength != CDDDRM_DYNAMIC_CONNECTION_SUPPORTED_METADATA_LEN))
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else
# endif
  {
    CddDrm_ChannelIdType lChannelId = CddDrm_GetChannelIdFromMetaData(MetaDataOfNSduPtr);

    if (CddDrm_IsChannelValid(lChannelId))
    {
      CddDrm_MemCopy(MetaDataOfDataNPduOutPtr->SduDataPtr, &CddDrm_GetChannelRequestMetaData(lChannelId, 0U), MetaDataOfDataNPduOutPtr->SduLength);  /* VCA_CDDDRM_MEMCPY_CALL_METADATA */

      CddDrm_MemCopy(MetaDataOfFcNPduOutPtr, &CddDrm_GetChannelResponseMetaData(lChannelId, 0U), MetaDataOfDataNPduOutPtr->SduLength);               /* VCA_CDDDRM_MEMCPY_CALL_METADATA */

      lRetVal = E_OK;
    }

    CDDDRM_DUMMY_STATEMENT_CONST(PayloadOfDataNPduInOutPtr);
    CDDDRM_DUMMY_STATEMENT_CONST(PayloadOfFcNPduInOutPtr);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANTP_TX_METADATA, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return lRetVal;
}
#endif /* CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON */

#if (CDDDRM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CDDDRM_APPL_DATA) versionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (versionInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versionInfo->vendorID = (CDDDRM_VENDOR_ID);
    versionInfo->moduleID = (CDDDRM_MODULE_ID);
    versionInfo->sw_major_version = (CDDDRM_SW_MAJOR_VERSION);
    versionInfo->sw_minor_version = (CDDDRM_SW_MINOR_VERSION);
    versionInfo->sw_patch_version = (CDDDRM_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_GET_VERSION_INFO, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#define CDDDRM_STOP_SEC_CODE
#include "CddDrm_MemMap.h"                                                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
   MD_CDDDRM_Rule8.9_3218:
     Description: An object should be defined at block scope if its identifier only appears in a single function.
     Reason:      Vector style guide prevents usage of static variables in function scope.
     Risk:        None.
     Prevention:  None.

   MD_CDDDRM_Rule11.8_0311:
     Description: A cast shall not remove any const or volatile qualification from the type pointed to by a pointer.
     Reason:      Common pointer type used that needs to be not constant for other usage.
     Risk:        None. Pointer to ROM value.
     Prevention:  Safe access of shared data is verified during code review.

   MD_CDDDRM_Rule2.2_2985:
     Description: This operation is redundant. The value of the result is always that of the left-hand operand.
     Reason:      Apparent redundant-like operations are needed for better code readability and maintainability.
     Risk:        None.
     Prevention:  None.

   MD_CDDDRM_Rule2.1_2880:
    Description:  Code is unreachable for a particular configuration.
    Reason:       Keeping the code simple to handle several variants.
    Risk:         None
    Prevention:   None

   MD_CDDDRM_Rule14.3_2991:
    Description:  If clause is always true for a particular configuration.
    Reason:       Keeping the code simple to handle several variants.
    Risk:         None
    Prevention:   None

   MD_CDDDRM_Rule2.2_2996:
    Description:  Logical operation is always false for a particular configuration.
    Reason:       Keeping the code simple to handle several variants.
    Risk:         None
    Prevention:   None

   MD_CDDDRM_CONSTPOINTERUSAGE:
    Description:  Passing address of uninitialized object 'lCancellationsToInvoke' to a function parameter declared as a pointer to const
    Reason:       Avoiding unnecessary initialization of object.
    Risk:         None
    Prevention:   This array is only initialized to the number of elements which shall be checked. Since the number is passed to the underlying function there is no risk of accessing uninitialized memory.

   MD_CDDDRM_Rule8.13_3673:
     Description: A pointer should point to a const-qualified type whenever possible.
     Reason:      The API is defined to be compatible with the MICROSAR stack.
     Risk:        None.
     Prevention:  None.
 */

/* START_COVERAGE_JUSTIFICATION

  \ID COV_CDDDRM_USER_MACROS
    \ACCEPT TX
    \REASON The condition checks whether a macro is defined and conditionally assigns a default implementation.
            When using an own macro implementation, the customer is responsible to ensure its safety.

  \ID COV_CDDDRM_ROBUSTNESS
    \ACCEPT TF tx tf
    \ACCEPT TX tx tx
    \ACCEPT TF tf tx
    \ACCEPT XF
    \ACCEPT TX
    \ACCEPT TF tf tx tf
    \ACCEPT TF tx tf tf
    \ACCEPT TF tx tx tf
    \REASON This check increases robustness. Under normal circumstances it is never false/true.

   
  \ID COV_CDDDRM_VALIDCHANNEL_FUNCRESPONSE
    \ACCEPT TX
    \REASON The condition checks if a valid channel is available.The condition will never be fulfilled due to the 
            number of active channels will be the same as functional response buffers during active functional request.
            Based on the configuration, the condition check might be fulfilled.

  \ID COV_CDDDRM_FUNCRESPONSEBUFFER
    \ACCEPT TX
    \REASON The condition checks if a valid functional response buffer can be retrived. The condition will never be fulfilled in the case when
            the number of functional response buffers is the same as the number of total possible responses.

  \ID COV_CDDDRM_VALIDCHANNEL_FUNCREQUEST
    \ACCEPT TX
    \REASON The condition checks if a valid channel is available. The condition will never be fulfilled when the 
            number of functional connections is one. Based on the configuration, 
            the condition check might be fulfilled.

  \ID COV_CDDDRM_PARALLEL_FUNCREQUEST
    \ACCEPT XF tf xf
    \REASON The condition will never be fulfilled when any of the delay mechanisms are configured. Based on the configuration,
            the condition check might be fulfilled.


END_COVERAGE_JUSTIFICATION
*/

/**********************************************************************************************************************
*  SILENTBSW JUSTIFICATIONS
*********************************************************************************************************************/
/* VCA_JUSTIFICATION_BEGIN

\ID VCA_CDDDRM_PDURTRANSMIT_CALL
  \DESCRIPTION    PduR_CddDrmTransmit is called with PduInfo pointer which is taken from a local variable, as such it is always valid.
  \COUNTERMEASURE \N Only read access will be done on the PduInfo pointer therefore no risk with regards to overwriting memory shall be present.

\ID VCA_CDDDRM_PDURCOPYTXDATA_CALL
  \DESCRIPTION    PduR_CddDrmCopyTxData is called with several pointer parameters.
                   - The PduInfo pointer is a local variable, as such it is always valid.
                   - The retry pointer is not supported so it is always NULL.
                   - The availableDataPtr is a local variable, as such it is always valid.
  \COUNTERMEASURE \N Only read access will be done on the PduInfo pointer therefore no risk with regards to overwriting memory shall be present.

\ID VCA_CDDDRM_PDURSTARTOFRECEPTION_CALL
  \DESCRIPTION    PduR_CddDrmStartOfReception is called with several pointer parameters.
                   - The PduInfo pointer is always NULL.
                   - The bufferSizePtr is a local variable, as such it is always valid.
  \COUNTERMEASURE \N No countermeasure is required.

\ID VCA_CDDDRM_PDURCOPYRXDATA_CALL
  \DESCRIPTION    PduR_CddDrmCopyRxData is called with several pointer parameters.
                   - The PduInfo pointer is a local variable, as such it is always valid.
                   - The bufferSizePtr is a local variable, as such it is always valid.
  \COUNTERMEASURE \N Length of the data copied into the PduInfo->SduDataPtr buffer will always be the maximum size of the local buffer due to check in CddDrm_DetermineSduLength.
                     The same define is used for both the check and the initialization of the local buffer so any inconsistency from generator will not be any risk.

\ID VCA_CDDDRM_MEMCPY_WRITE_DEST
  \DESCRIPTION    The pointer destination and the upper limit for the used index (length) are parameters of the function and expected
                  to be valid and consistent. This shall be checked by the caller of this function.
  \COUNTERMEASURE \N No countermeasure is required due to the validity of the parameters checked by the caller of the function.

\ID VCA_CDDDRM_MEMCPY_CALL_RESPONSEBUFFER
  \DESCRIPTION    Call to MemCopy to copy receiving data from Lower Layer component into response buffer. 
                  RespBufferInfoPtr(destination) is only initialized in ChannelInit and it is always called before any data processing is done within the CddDrm.
                  SduLength(length) is checked against the remaining buffer length.
  \COUNTERMEASURE \N RespBufferInfoPtr(destination) is received from the user/application, therefore is always expected to be valid and consistent.

\ID VCA_CDDDRM_MEMCPY_CALL_FUNCRESPONSEBUFFER
  \DESCRIPTION    Call to MemCopy to copy stored data from internal buffer into response buffer. 
                  responseBufferDataPtr(destination) is valid as it is verified by DET.
                  responseBufferSize(length) is checked against the buffer length.
  \COUNTERMEASURE \N responseBufferDataPtr(destination) is received from the user/application, therefore is always expected to be valid and consistent.

\ID VCA_CDDDRM_MEMCPY_CALL_REQUESTDATA
  \DESCRIPTION    Call to MemCopy to copy diagnostic request data into buffer provided by Lower Layer component. 
                  SduDataPtr(destination) is valid as it is verified by DET.
                  length is checked against the received SduLength which shall be the buffer size.
  \COUNTERMEASURE \N SduDataPtr(destination) is received from the user/application, there is always expected to be valid and consistent.

\ID VCA_CDDDRM_MEMCPY_CALL_METADATA
  \DESCRIPTION    Call to MemCopy to copy diagnostic request data into buffer provided by Lower Layer component. 
                  destination is valid as it is verified by DET.
                  length is valid as it is verified by DET.
  \COUNTERMEASURE \N destination is verified by DET.
                  \N length is verified by DET.
                  \N Destination buffer is received from CanTp and is expected to be valid and consistent.

\ID VCA_CDDDRM_WRITE_CHANNEL_STATE
  \DESCRIPTION    ChannelId retrieved from CddDrm_ChannelAssign always either have CDDDRM_CHANNEL_NOT_ASSIGNED or a valid value. 
                  A check for CDDDRM_CHANNEL_NOT_ASSIGNED shall always be present before using channelId retrieved from CddDrm_ChannelAssign is used.
  \COUNTERMEASURE \N Check for CDDDRM_CHANNEL_NOT_ASSIGNED

\ID VCA_CDDDRM_CONNECTIONID_CHANNELINFO
  \DESCRIPTION    ChannelInfo data structure can only be initialized with valid connection IDs, therefore any access to connection id through the ChannelInfo data structure is deemed valid.
  \COUNTERMEASURE \N As the connection id is always considered valid, it is within the bounds of the arrays initialized by CDDDRM_CFG_CONNECTION_COUNT.

\ID VCA_CDDDRM_NOTIFICATIONFUNCPTR
  \DESCRIPTION    Notification function pointer will be used in the ChannelInfo data structure, therefore it is required that the user ensures that the signature 
                  of the function matches the expected signature. This is enforced by the safety manual entry.
  \COUNTERMEASURE \S SMI-1019002

\ID VCA_CDDDRM_CONNECTIONID_RXPDU
  \DESCRIPTION    Relation between RxPdu and ConnectionId is 1:1. Only valid connectionIds will be placed into the table CddDrm_RxPduRefToConnId, therefore it is consider to always be valid and consistent.
  \COUNTERMEASURE \N No countermeasure is required

\ID VCA_CDDDRM_CANCELLATIONLIST_WRITE
  \DESCRIPTION    Write channel information to cancellation list. Cancellation list is a parameter of the function and is expected to be valid.
                  This shall be ensured by the caller of the function.
  \COUNTERMEASURE \N No countermeasure is required

\ID VCA_CDDDRM_FUNCTIONAL_RESPONSE_CHANNEL
  \DESCRIPTION    When receiving a response for a functional request, a channel has to be initialized to handle the response. 
                  This is the only usecase where a channelId is allowed to be forwarded when it is not valid.
  \COUNTERMEASURE \N The specific usecase for functional request/response is handled in CddDrm_ProcessStartOfReception and ensures that the channel is initialized correctly and therefore will be valid.

 VCA_JUSTIFICATION_END */


/**********************************************************************************************************************
 *  END OF FILE: CddDrm.c
 *********************************************************************************************************************/
