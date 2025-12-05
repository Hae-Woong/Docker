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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dlt.h
 *      Project:  Diagnostic Log and Trace
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implements partly the functionality of the AUTOSAR 4.x module DLT based on XCP.
 *
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2011-05-12  visml   ESCAN00050129   Creation
 *  01.02.00  2013-03-26  visml   ESCAN00065961   Added DLT_APPL_VAR and DLT_PBCFG
 *                        visml   ESCAN00065958   Added DLT_XCP_DET_EVENT to the template
 *                        visml   ESCAN00066198   Added default XCP event to A2L file
 *  01.02.01  2013-05-21  visazb  ESCAN00067468   Corrected vesion mismatch
 *  01.03.00  2013-07-02  visore  ESCAN00068516   Compiler warning: parameter 'ConfigPtr' unused in function 'Dlt_Init'
 *                        visore  ESCAN00068517   The DET service and error IDs are not compliant to the AUTOSAR4 specification
 *                        visore  ESCAN00068518   The MISRA justifications are not implemented according WI_MISRAC2004_PES.pdf
 *                        visore  ESCAN00068274   AR4-292: Reporting of DET and DEM errors via DLT
 *                        visore  ESCAN00068519   Signature of Dlt_DetForwardErrorTrace() is not compliant to AUTOSAR4
 *                        visore  ESCAN00070450   No changes here
 *                        visore  ESCAN00070451   No changes here
 *  01.03.01  2013-11-20  visore  ESCAN00071890   No changes here
 *  01.03.02  2014-01-20  visore  ESCAN00072716   Improve DLT include structure
 *  01.03.03  2014-05-21  visore  ESCAN00074824   No changes here
 *  01.05.00  2014-02-25  viszda  ESCAN00081343   FEAT-1311: DLT with AUTOSAR functionality and DLT Transport layer
 *            2015-03-17  viszda  ESCAN00081868   No changes here
 *  01.06.00  2015-04-22  viszda  ESCAN00082261   FEAT-1312: Continue DLT development with AUTOSAR functionality and DLT Transport layer (based on FEAT-1311)
 *                        viszda  ESCAN00082468   Compiler error: error C2065: 'xyz' : undeclared identifier
 *                        viszda  ESCAN00082469   Compiler warning: warning C4244, warning C4101, warning C4189, warning C4100
 *                        viszda  ESCAN00082472   Missing filters, compiler abstractions and exclusive areas
 *                        viszda  ESCAN00082473   Reduce code redundancies and code complexity due to FEAT-1311
 *                        viszda  ESCAN00082474   Change erroneous compiler abstractions and use the correct initialization for global variables
 *                        viszda  ESCAN00083946   No changes here
 *  01.06.01  2015-12-17  viszda  ESCAN00087203   No changes here
 *                        viszda  ESCAN00087212   Wrong DLT module ID
 *  01.07.00  2015-02-01  viszda  ESCAN00087978   Correction of version number in DltCom messages
 *            2016-02-26  viszda  ESCAN00088121   Product release of ASR DLT
 *  01.07.01  2016-06-21  viszda  ESCAN00090043   Get the socket connection state via API
 *            2016-07-25  viszda  ESCAN00091129   Valid DEM Event forwarded to DLT triggers DET Error
 *  01.07.02  2016-11-18  viszda  ESCAN00092787   Control messages from Dlt client are rejected and no response is send.
 *                        viszda  ESCAN00081639   Correction of formal issues
 *                        viszda  ESCAN00092560   Compiler error: The include file 'Det.h' cannot be opened
 *            2016-11-22  viszda  ESCAN00092936   Reduce Stack consumption of Dlt
 *  01.07.03  2018-03-13  viszda  ESCAN00098621   Loss of DLT log data after socket reconnect
 *            2018-03-14  viszda  ESCAN00098730   DLT log message loss if a control request message is received by DLT
 *  01.07.04  2018-05-24  viszda  ESCAN00097255   DLT (XCP) forwards DET errors to uninitialized XCP
 *  02.00.00  2018-11-07  viszda  STORY-8329      Support disabled Rx path and control APIs.
 *  02.01.00  2019-01-29  viszda  STORY-9729      Change data types corresponding to ASR 4.3.1 and change the APIs using those data types.
 *                        viszda  ESCAN00101530   Wrong default values for Dlt_Context
 *                        viszda  ESCAN00102027   Wrong interpretation of application and context id on little endian platforms
 *  03.00.00  2020-03-02  viszda  SWAT-12         Update the BSWMD corresponding to AUTOSAR DLT 4.3.1 specification.
 *            2020-04-08  viszda  ESCAN00106077   Log messages with high log level are filtered
 *  03.01.00  2020-04-13  viszda  SWAT-62         Support the DLT service Store_Config (0x05).
 *                        viszda  SWAT-78         Support the DLT service SWC injection (0xFFF-0xFFFFFFFF).
 *            2020-04-28  viszda  ESCAN00106239   Compiler errors: Multiple elements are undefined
 *  03.02.00  2020-05-14  viszda  SWAT-35         Implement the multi log channel support
 *  03.03.00  2020-06-23  viszda  SWAT-1041       Support of bandwidth management
 *            2020-06-24  viszda  ESCAN00106674   Compiler error: Missing file Dlt_Lcfg.h
 *  03.04.00  2020-07-31  viszda  SWAT-1110       Provide separate exclusive areas for Dlt_MainFunction, Tx and Rx.
 *            2020-07-31  viszda  ESCAN00107008   Wrong length of GetLoginfo response
 *            2020-07-31  viszda  ESCAN00107010   Missing trace status in response of SetLogChannelThreshold
 *  04.00.00  2020-10-29  viszda  SWAT-1165       Use of VStdLib MemCopy instead of local copy loops
 *  04.01.00  2020-11-20  viszda  SWAT-1252       Provide the API to get the default LogLevel
 *  04.02.00  2021-01-11  viszda  SWAT-1296       Add an optional validator to support TCP via SoAd
 *  04.02.01  2021-01-18  viszda  ESCAN00108292   C/S calls cannot be queued for services belonging to DLT server ports
 *  05.00.00  2021-11-29  visore  SWAT-1831       Prepare SafeBSW Release for DLT
 *            2022-05-03  viszda  ESCAN00109334   Compile error due to wrong data types in callback function Dlt_CopyTxData, Dlt_CopyRxData, and Dlt_StartOfReception
 *            2022-05-30  visore  SWAT-1627       Usage of <Mip>_MemMap.h in Dlt
 *  05.01.00  2022-07-15  visore  SWAT-2205       Dlt: support fetching synchronized timestamps from Tsyn
 *  06.00.00  2022-12-19  viszda  SWAT-2420       Support of snapshot tracing
 *            2023-01-23  viszda  SWAT-2138       Tidy up
 *            2023-01-23  viszda  SWAT-2211       Improvements found during safety review
 *            2023-01-23  viszda  ESCAN00113408   Undefined behavior if Dlt protocol v2 service request is received
 *            2023-02-14  viszda  ESCAN00113902   NOAR of control message responses is not set to 0
 *  07.00.00  2023-03-16  visore  SWAT-2583       DLT AUTOSAR BufferOverflowNotification
 *            2023-03-16  visore  ESCAN00114167   Payload of control response of service SyncTimeStamp (0x24) in wrong endianness
 *            2023-03-16  visore  ESCAN00114166   Payload of cyclically send service SyncTimeStamp (0x24) reported with wrong endianness
 *            2023-04-05  visore  ESCAN00114221   Unexpected behavior of multiple APIs due to uninitialized Dlt_LogChannelInfo[]
 *  07.00.01  2023-10-16  vismjv  ESCAN00115640   Compiler error: Cannot open file 'Dem.h'
 *            2023-10-23  vismjv  ESCAN00114783   Compiler error: C4244: conversion from 'PduLengthType' to 'uint16' 'possible loss of data'
 *  07.01.00  2023-12-09  viszda  SWAT-2651       Os Software Trace Module Support - Impl, Generate, FeatureTest
 *            2023-11-18  viszda  ESCAN00116164   Control messages have no effect on DLT settings if received from TP other than SoAd
 *            2024-02-17  viszda  ESCAN00116699   The control service GetSoftwareVersion (0x13) reports the software version in incorrect formatting
 *            2024-02-17  viszda  ESCAN00116215   Reported number of lost messages in control response of buffer overflow notification is always 0
 *            2024-03-02  viszda  ESCAN00116851   The API Dlt_GetTraceStatus returns E_OK but does not set the output parameter TraceStatus
 *********************************************************************************************************************/

#if !defined(DLT_H)
# define DLT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Dlt_Types.h"
# include "Dlt_Lcfg.h"
# include "SchM_Dlt.h"

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  include "Dlt_Cbk.h"
#  include "PduR.h"
#  include "PduR_Dlt.h"
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
# if (DLT_IS_DEM_PRESENT == STD_ON)
# include "Dem.h" /* needed for Dem_EventIdType */
# endif

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* vendor and module identification */
# define DLT_VENDOR_ID                       (30u)
# define DLT_MODULE_ID                       (55u) /* ESCAN00087212 */

/* AUTOSAR Software specification version information */
# define DLT_AR_RELEASE_MAJOR_VERSION        (4u)
# define DLT_AR_RELEASE_MINOR_VERSION        (3u)
# define DLT_AR_RELEASE_REVISION_VERSION     (1u)

/* Component version information (decimal version of ALM implementation package) */
# define DLT_SW_MAJOR_VERSION                (7u)
# define DLT_SW_MINOR_VERSION                (1u)
# define DLT_SW_PATCH_VERSION                (0u)

# define DLT_INSTANCE_ID_DET                 (0x00u)

/* API service IDs */
# define DLT_API_ID_INIT                     ((uint8)0x01u) /*!< ASR4.0 Dlt_Init() service ID [Dlt239]. */
# define DLT_API_ID_GETVERSIONINFO           ((uint8)0x02u) /*!< ASR4.0 Dlt_GetVersionInfo() service ID [Dlt271]. */
# define DLT_API_ID_SENDLOGMESSAGE           ((uint8)0x03u) /*!< ASR4.0 Dlt_SendLogMessage() service ID [Dlt241]. */
# define DLT_API_ID_SENDTRACEMESSAGE         ((uint8)0x04u) /*!< ASR4.0 Dlt_SendTraceMessage() service ID [Dlt243]. */
# define DLT_API_ID_REGISTERCONTEXT          ((uint8)0x05u) /*!< ASR4.0 Dlt_RegisterContext() service ID [Dlt245]. */
# define DLT_API_ID_RESETTOFACTORYDEFAULT    ((uint8)0x06u) /*!< ASR4.0 Dlt_ResetToFactoryDefault() service ID [SWS_Dlt_00739]. */
# define DLT_API_ID_DETFORWARDERRORTRACE     ((uint8)0x07u) /*!< ASR4.0 Dlt_DetForwardErrorTrace() service ID [Dlt432]. */
# define DLT_API_ID_SETLOGLEVEL              ((uint8)0x08u) /*!< ASR4.0 Dlt_SetLogLevel() service ID [SWS_Dlt_00252]. */
# define DLT_API_ID_SETTRACESTATUS           ((uint8)0x09u) /*!< ASR4.0 Dlt_SetTraceStatus() service ID [SWS_Dlt_00254]. */
# define DLT_API_ID_GETLOGINFO               ((uint8)0x0Au) /*!< ASR4.0 Dlt_GetLogInfo() service ID [SWS_Dlt_00732]. */
# define DLT_API_ID_SETDEFAULTLOGLEVEL       ((uint8)0x11u) /*!< ASR4.0 Dlt_SetDefaultLogLevel() service ID [SWS_Dlt_00740]. */
# define DLT_API_ID_SETDEFAULTTRACESTATUS    ((uint8)0x12u) /*!< ASR4.0 Dlt_SetDefaultTraceStatus() service ID [SWS_Dlt_00743]. */
# define DLT_API_ID_INJECTCALL               ((uint8)0x14u) /*!< ASR4.0 Dlt_InjectCall() service ID [SWS_Dlt_00259]. */
# define DLT_API_ID_DEMTRIGGERONEVENTSTATUS  ((uint8)0x15u) /*!< ASR4.0 Dlt_DemTriggerOnEventStatus() service ID [Dlt470]. */
# define DLT_API_ID_GETLOGCHANNELNAMES       ((uint8)0x17u) /*!< ASR4.0 Dlt_GetLogChannelNames() service ID [SWS_Dlt_00749]. */
# define DLT_API_ID_GETDEFAULTLOGLEVEL       ((uint8)0x18u) /*!< ASR4.0 Dlt_GetDefaultLogLevel() service ID [SWS_Dlt_00733]. */
# define DLT_API_ID_GETDEFAULTTRACESTATUS    ((uint8)0x19u) /*!< ASR4.0 Dlt_GetDefaultTraceStatus() service ID [SWS_Dlt_00746]. */
# define DLT_API_ID_STORECONFIGURATION       ((uint8)0x1Au) /*!< ASR4.0 Dlt_StoreConfiguration() service ID [SWS_Dlt_00736]. */
# define DLT_API_ID_SETMESSAGEFILTERING      ((uint8)0x1Bu) /*!< ASR4.0 Dlt_SetMessageFiltering() service ID [SWS_Dlt_00770]. */
# define DLT_API_ID_GETTRACESTATUS           ((uint8)0x1Fu) /*!< ASR4.0 Dlt_GetTraceStatus() service ID [SWS_Dlt_00750]. */
# define DLT_API_ID_SETLOGCHANNELASSIGNMENT  ((uint8)0x20u) /*!< ASR4.0 Dlt_SetLogChannelAssignment() service ID [SWS_Dlt_00751]. */
# define DLT_API_ID_SETLOGCHANNELTHRESHOLD   ((uint8)0x21u) /*!< ASR4.0 Dlt_SetLogChannelThreshold() service ID [SWS_Dlt_00752]. */
# define DLT_API_ID_GETLOGCHANNELTHRESHOLD   ((uint8)0x22u) /*!< ASR4.0 Dlt_GetLogChannelThreshold() service ID [SWS_Dlt_00753]. */
# define DLT_API_ID_COPYTXDATA               ((uint8)0x43u) /*!< ASR4.0 Dlt_CopyTxData() service ID [SWS_Dlt_00516]. */
# define DLT_API_ID_COPYRXDATA               ((uint8)0x44u) /*!< ASR4.0 Dlt_CopyRxData() service ID [SWS_Dlt_91008]. */
# define DLT_API_ID_TPRXINDICATION           ((uint8)0x45u) /*!< ASR4.0 Dlt_TpRxIndication() service ID [SWS_Dlt_91007]. */
# define DLT_API_ID_STARTOFRECEPTION         ((uint8)0x46u) /*!< ASR4.0 Dlt_StartOfReception() service ID [SWS_Dlt_91006]. */
# define DLT_API_ID_TPTXCONFIRMATION         ((uint8)0x48u) /*!< ASR4.0 Dlt_TpTxConfirmation() service ID [SWS_Dlt_00756]. */
# define DLT_API_ID_MAINFUNCTION             ((uint8)0x50u) /*!< MSR4.0 Dlt_MainFunction() service ID. */
# define DLT_API_ID_SETSTATE                 ((uint8)0x51u) /*!< MSR4.0 Dlt_SetState() service ID. */
# define DLT_API_ID_GETSTATE                 ((uint8)0x52u) /*!< MSR4.0 Dlt_GetState() service ID. */
# define DLT_API_ID_SETDEBUGMODE             ((uint8)0x80u) /*!< MSR4.0 Dlt_SetLogChannelDebugMode() service ID. */
# define DLT_API_ID_GETDEBUGMODE             ((uint8)0x81u) /*!< MSR4.0 Dlt_GetLogChannelDebugMode() service ID. */
# define DLT_API_ID_TRIGGERDEBUGEVENT        ((uint8)0x82u) /*!< MSR4.0 Dlt_TriggerLogChannelDebugEvent() service ID. */
# define DLT_API_ID_OSVTHSCHEDULE            ((uint8)0x83u) /*!< MSR4.0 Dlt_OsVthSchedule() service ID. */
# define DLT_API_ID_OSVTHACTIVATION          ((uint8)0x84u) /*!< MSR4.0 Dlt_OsVthActivation() service ID. */
# define DLT_API_ID_OSVTHSETEVENT            ((uint8)0x85u) /*!< MSR4.0 Dlt_OsVthSetEvent() service ID. */


/* Error codes */
# define DLT_E_NO_ERROR                      ((uint8)0x00u) /*!< Error code: No error. */
# define DLT_E_PARAM                         ((uint8)0x01u) /*!< Error code: Development [SWS_Dlt_00727]. */
# define DLT_E_PARAM_POINTER                 ((uint8)0x02u) /*!< Error code: Development [SWS_Dlt_00727]. */
# define DLT_E_INIT_FAILED                   ((uint8)0x03u) /*!< Error code: Development [SWS_Dlt_00727]. */
# define DLT_E_REGISTRATION                  ((uint8)0x04u) /*!< Error code: Development [SWS_Dlt_00727]. */
# define DLT_E_MSG_LOOSE                     ((uint8)0x05u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_SKIPPED_TRANSMISSION          ((uint8)0x05u) /*!< Error code: Runtime [SWS_Dlt_00728]. */
# define DLT_E_DEPRECATED_PARAMETER          ((uint8)0x06u) /*!< Error code: Runtime [SWS_Dlt_00728]. */
# define DLT_E_MULTIPLE_REQUESTS             ((uint8)0x07u) /*!< Error code: Runtime [SWS_Dlt_00728]. */
# define DLT_E_UNINITIALIZED                 ((uint8)0x10u) /*!< Error code: Development; DLT is not initialized. */
# define DLT_E_INVALID_STATE                 ((uint8)0x11u) /*!< Error code: Runtime; DLT is in an invalid state. */

# define DLT_INVALID_LOG_CHANNEL_INDEX       ((uint16)0xFFFFu)

# ifndef DLT_DEM_EVENT_LOG_LEVEL /* COV_DLT_MSR_COMPATIBILITY */
#  define DLT_DEM_EVENT_LOG_LEVEL DLT_LOG_INFO
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

 /* Check the version of Dlt_Cfg.h */
# if (  (DLT_CFG_MAJOR_VERSION != (8u)) \
     || (DLT_CFG_MINOR_VERSION != (1u)) )
#  error "Version numbers of Dlt_Cfg.h are inconsistent"
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FUNCTIONLIKEMACRO */ /* MD_MSR_FctLikeMacro */

# if !defined(DLT_DUMMY_STATEMENT)     /* COV_DLT_MSR_COMPATIBILITY */
#  if defined (DLT_USE_DUMMY_STATEMENT)
# define DLT_DUMMY_STATEMENT(statement) ((void)(statement))
#  else
# define DLT_DUMMY_STATEMENT(statement)
#  endif
# endif

# if !defined(DLT_DUMMY_STATEMENT_CONST)     /* COV_DLT_MSR_COMPATIBILITY */
#  if defined (DLT_USE_DUMMY_STATEMENT_CONST)
# define DLT_DUMMY_STATEMENT_CONST(statement) ((void)(statement))
#  else
# define DLT_DUMMY_STATEMENT_CONST(statement)
#  endif
# endif

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  define Dlt_GetMaxNumberOfContexts() (uint32)(Dlt_GetMaxNumberOfApplications() * Dlt_GetMaxNumberOfContextsPerApplication())
# endif

/* PRQA L:FUNCTIONLIKEMACRO */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  if (DLT_NVRAM_SUPPORT == STD_ON)
typedef struct
{
  Dlt_LocalLogChannelIdType      logChannelId;                     /*!< log channel id */
  Dlt_LocalLogLevelType          logChannelThreshold;              /*!< log channel specific threshold (log level) */
  Dlt_LocalTraceStatusType       logChannelTraceStatus;            /*!< log channel specific trace status */
} Dlt_NvLogChannelInfoType;                                        /*!< data type: log channel info to be persisted */

typedef struct
{
  uint32                         logChannelIndexes[Dlt_GetNumberOfAllValidLogChannels()]; /*!< indexes to the assigned log channels. */
  Dlt_LocalApplicationIDType     appId;                            /*!< the application id */
  Dlt_LocalContextIDType         contextId;                        /*!< the context id */
  uint16                         appIndexInDltContext;             /*!< the index of application id in the Dlt_Context structure */
  uint16                         contextIndexInDltContext;         /*!< the index of context id in the Dlt_Context structure */
  sint8                          logLevel;                         /*!< the threshold (log level) of the Dlt user (tuple of application id and context id) */
  sint8                          traceStatus;                      /*!< the trace status of the Dlt user (tuple of application id and context id) */
} Dlt_NvLogInfoType;                                               /*!< data type: Dlt user specific data to be persisted */

typedef struct
{
  uint32                         numberOfLogChannel;               /*!< number of originally configured log channels: used to validate restored nv data */
  uint32                         numberOfSwcs;                     /*!< number of originally configured Dlt SWCs: used to validate restored nv data */
  uint32                         numberOfSwcContexts;              /*!< number of originally configured Dlt SWC Contexts: used to validate restored nv data */
  uint32                         defaultLogChannelIndex;           /*!< the index to the default log channel */
#   if (DLT_OS_SOFTWARE_TRACING == STD_ON)
  Dlt_NvLogInfoType              logInfo[Dlt_GetNumberOfAllValidSwcContexts() + 2u + DLT_OS_TRACING_NUMBER_OF_TYPES]; /*!< infos for all Dlt users inclusively the DEM, DET, and OS */
#   else
  Dlt_NvLogInfoType              logInfo[Dlt_GetNumberOfAllValidSwcContexts() + 2u]; /*!< infos for all Dlt users inclusively the DEM and DET */
#   endif
  Dlt_NvLogChannelInfoType       logChannelInfo[Dlt_GetNumberOfAllValidLogChannels()]; /*!< the log channel infos: Id, threshold, and trace status */
  uint8                          defaultLogLevel;                  /*!< the default log level */
  boolean                        defaultTraceStatus;               /*!< the default trace status */
} Dlt_NvDataType;                                                  /*!< data type: all data to be persisted */
#  endif

typedef struct
{
  uint8 argCount;                                                  /*!< number of arguments in the Dlt message payload */
  Dlt_MessageLogLevelType logLevel;                                /*!< the actual log level of this message */
  Dlt_MessageOptionsType options;                                  /*!< the message options: verbose/non-verbose and message type (log, network trace, application trace, or control) */
  Dlt_LocalContextIDType contextId;                                /*!< the context id to identify the source Dlt user */
  Dlt_LocalApplicationIDType appId;                                /*!< the application id to identify the source Dlt user */
} Dlt_LocalMessageLogInfoType;                                     /*!< data type: infos for log messages */

typedef struct
{
  Dlt_MessageTraceType traceInfo;                                  /*!< the type of the trace message (variable, function in, function out, state, or VFB) */
  Dlt_MessageOptionsType options;                                  /*!< the message options: verbose/non-verbose and message type (log, network trace, application trace, or control) */
  Dlt_LocalContextIDType context;                                  /*!< the context id to identify the source Dlt user */
  Dlt_LocalApplicationIDType appId;                                /*!< the application id to identify the source Dlt user */
} Dlt_LocalMessageTraceInfoType;                                   /*!< data type: infos for trace messages */

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
typedef struct
{
  Dlt_MessageTypeType           messageType;    /*!< Member of filter info: the message type. */
  Dlt_SessionIDType             sessionId;      /*!< Member of filter info: the session ID. */
  Dlt_LocalMessageLogInfoType   logInfo;        /*!< Member of filter info: if it is a log message, contains required data. */
  Dlt_LocalMessageTraceInfoType traceInfo;      /*!< Member of filter info: if it is a trace message, contains required data. */
  Dlt_UsedEndiannessType        usedEndianness; /*!< Member of filter info: the used endianness (forced to be little or big, or use the platform endianness). */
} Dlt_FilterInfoType;                           /*!< Filter info type. */

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define DLT_START_SEC_CONST_8
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
extern CONST(uint8, DLT_CONST)                     Dlt_StandardHeaderType_SoftwareVersion;
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

extern CONST(uint8, DLT_CONST)                     Dlt_ModuleTable[256];

# define DLT_STOP_SEC_CONST_8
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# define DLT_START_SEC_VAR_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Variables for Dlt runtime configuration */
extern VAR(boolean, DLT_VAR_INIT)                  Dlt_FilterMessages;
#  if (DLT_TIMESTAMP_SUPPORT == STD_ON)
extern VAR(boolean, DLT_VAR_INIT)                  Dlt_HeaderUseTimestamp;
#  endif

extern VAR(boolean, DLT_VAR_INIT)                  Dlt_HeaderUseEcuId;
extern VAR(boolean, DLT_VAR_INIT)                  Dlt_HeaderUseSessionId;
extern VAR(boolean, DLT_VAR_INIT)                  Dlt_DefaultTraceStatus;
extern VAR(Dlt_MessageLogLevelType, DLT_VAR_INIT)  Dlt_DefaultMaxLogLevel;

#  define DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
#  include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#  if (DLT_NVRAM_SUPPORT == STD_ON)
#   define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
#   include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

extern VAR(Dlt_NvDataType, DLT_VAR_CLEARED)        Dlt_NvData;

#   define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#   include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  endif

# else /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#  define DLT_START_SEC_VAR_NO_INIT_32
#  include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Variable is loaded with the last DET error code */
extern VAR(uint32, DLT_VAR_NO_INIT)                Dlt_DetErrorCode;

#  define DLT_STOP_SEC_VAR_NO_INIT_32
#  include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

# define DLT_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

extern VAR(SduDataPtrType, DLT_VAR_NO_INIT)        Dlt_SendPdu_DataPtr[Dlt_GetSizeOfLogChannelDescriptor()];
extern VAR(PduLengthType, DLT_VAR_NO_INIT)         Dlt_SendPdu_DataLength[Dlt_GetSizeOfLogChannelDescriptor()];
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

# if defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
/* Variable is loaded with DEM event status code. */
extern VAR(Dlt_DemEventStatusType, DLT_VAR_NO_INIT) Dlt_DemEventStatus;

/* Variable is loaded with message ID of non-verbose message . */
extern VAR(Dlt_MessageIDType, DLT_VAR_NO_INIT)     Dlt_NonVerboseMessageId;

#  if (DLT_USE_VERBOSE_MODE == STD_ON)
/* Buffer is loaded with payload of verbose message. */
extern VAR(uint8, DLT_VAR_NO_INIT)                 Dlt_VerboseMessageData[DLT_MAX_MESSAGE_LENGTH];
#  endif
# endif /* defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) */

# define DLT_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define DLT_START_SEC_CODE
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dlt_InitMemory
**********************************************************************************************************************/
/*! \brief       Power-up memory initialization
 *  \details     Initializes component variables in *_INIT_* sections at power up.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106553
 *  \note        Use this function in case these variables are not initialized by the startup code.
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_InitMemory(void);

/**********************************************************************************************************************
 *  Dlt_Init
**********************************************************************************************************************/
/*! \brief       Initializes component
 *  \details     Initializes all component variables and sets the component state to initialized.
 *  \param[in]   ConfigPtr        Component configuration structure
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         Dlt_InitMemory has been called unless Dlt_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \spec        requires ConfigPtr != NULL_PTR;
 *  \endspec
 *  \synchronous TRUE
 *  \trace       CREQ-106553
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_Init(P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_PBCFG) ConfigPtr);

# if(DLT_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Dlt_GetVersionInfo()
**********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo      Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec        requires VersionInfo != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_VERSION_INFO_API = STD_ON).
 *  \trace       CREQ-106554
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DLT_APPL_VAR) VersionInfo);
# endif

/**********************************************************************************************************************
 *  Dlt_DetForwardErrorTrace()
**********************************************************************************************************************/
/*! \brief       Service to forward error reports from Det to Dlt.
 *  \details     Builds and sends a log message containing all passed data.
 *  \param[in]   ModuleId         Module ID of calling module.
 *  \param[in]   InstanceId       The identifier of the index based instance of a module.
 *  \param[in]   ApiId            ID of API service in which error is detected.
 *  \param[in]   ErrorId          ID of detected development error.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_DetForwardErrorTrace(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);

# if (DLT_IS_DEM_PRESENT == STD_ON)
/**********************************************************************************************************************
 *  Dlt_DemTriggerOnEventStatus()
**********************************************************************************************************************/
/*! \brief       This service is provided by the Dem in order to call Dlt upon status changes.
 *  \details     Builds and sends a log message containing the passed data.
 *  \param[in]   EventId          Module ID of calling module.
 *  \param[in]   EventStatusOld   The identifier of the index based instance of a module.
 *  \param[in]   EventStatusNew   ID of API service in which error is detected.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires EventId < DLT_NUM_DEM_EVENTS && EventId > 0;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_DemTriggerOnEventStatus(Dem_EventIdType EventId,
                                                 Dem_EventStatusExtendedType EventStatusOld,
                                                 Dem_EventStatusExtendedType EventStatusNew);
# endif /* (DLT_IS_DEM_PRESENT == STD_ON) */

/**********************************************************************************************************************
 *  Dlt_SendLogMessage()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SendLogMessage(Dlt_SessionIDType parg0, const Dlt_MessageLogInfoType* LogInfo, const uint8* LogData, uint16 LogDataLength)
 *  \brief       The service represents the interface to be used by software components to send log messages.
 *  \details     Builds and sends a log message containing the passed data.
 *  \param[in]   parg0            The session id.
 *  \param[in]   LogInfo          Structure containing the relevant information for filtering the message.
 *  \param[in]   LogData          Pointer to log data. This will be copied to payload of the log message.
 *  \param[in]   LogDataLength    Length of the log data.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, DLT_CODE) Dlt_SendLogMessage(
  Dlt_SessionIDType parg0,
  P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_VAR) LogInfo,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogData,
  uint16 LogDataLength);*/

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
/**********************************************************************************************************************
 *  Dlt_SendTraceMessage()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SendTraceMessage(Dlt_SessionIDType parg0, const Dlt_MessageTraceInfoType* TraceInfo, const uint8* TraceData, uint16 TraceDataLength)
 *  \brief       The service represents the interface to be used by software components to send trace messages.
 *  \details     Builds and sends a log message containing the passed data.
 *  \param[in]   parg0            The session id.
 *  \param[in]   TraceInfo        Structure containing the relevant information for filtering the message.
 *  \param[in]   TraceData        Pointer to trace data. This will be copied to payload of the log message.
 *  \param[in]   TraceDataLength  Length of the trace data.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, DLT_CODE) Dlt_SendTraceMessage(
  Dlt_SessionIDType parg0,
  P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_VAR) TraceInfo,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) TraceData,
  uint16 TraceDataLength);*/

/**********************************************************************************************************************
 *  Dlt_RegisterContext()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_RegisterContext(Dlt_SessionIDType parg0, const uint8* AppId, const uint8* ContextId, const uint8* AppDescription, uint8 LenAppDescription, const uint8* ContextDescription, uint8 LenContextDescription)
 *  \brief       Notifies the DLT to register a new DLT user.
 *  \details     The DLT user is specified by the couple of application id and context id. After registration, this
 *               DLT user may send log and trace messages.
 *  \param[in]   parg0                 The sesion id (usually set by RTE) to identify the SWC.
 *  \param[in]   AppId                 The Application id indentifies a DLT user (in combination with the Context id).
 *  \param[in]   ContextId             The Context id indentifies a DLT user (in combination with the Application id).
 *  \param[in]   AppDescription        Pointer to the application description.
 *  \param[in]   LenAppDescription     Length of the application description.
 *  \param[in]   ContextDescription    Pointer to the context description.
 *  \param[in]   LenContextDescription Length of the context description.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, DLT_CODE) Dlt_RegisterContext(
  Dlt_SessionIDType parg0,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppDescription,
  uint8 LenAppDescription,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextDescription,
  uint8 LenContextDescription);*/

#  if (DLT_OS_SOFTWARE_TRACING == STD_ON)
/**********************************************************************************************************************
 * Dlt_OsVthActivation
 *********************************************************************************************************************/
/*! \brief       Starts runtime measurement for task response time MPs.
 *  \details     -
 *  \param[in]   TaskId                  The task which is started.
 *  \param[in]   DestCoreId              The core on which the task is activated.
 *  \param[in]   CallerCoreId            The core on which the task activation is triggered.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different CallerCoreId
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON) && (DLT_OS_SOFTWARE_TRACING ==
 *               STD_ON).
 *  \ingroup     osTracing
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_OsVthActivation(
  uint32 TaskId,
  uint16 DestCoreId,
  uint16 CallerCoreId);

/**********************************************************************************************************************
 * Dlt_OsVthSetEvent
 *********************************************************************************************************************/
/*! \brief       Starts runtime measurement for task response time MPs.
 *  \details     Activates the extended tasks.
 *  \param[in]   TaskId                  The task which is started.
 *  \param[in]   StateChanged            Indicates whether the task is actually activated or if it is already activated.
 *  \param[in]   DestCoreId              The core on which the task is activated.
 *  \param[in]   CallerCoreId            The core on which the task activation is triggered.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different CallerCoreId
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON) && (DLT_OS_SOFTWARE_TRACING ==
 *               STD_ON).
 *  \ingroup     osTracing
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_OsVthSetEvent(
  uint32 TaskId,
  boolean StateChanged,
  uint16 DestCoreId,
  uint16 CallerCoreId);

/**********************************************************************************************************************
 * Dlt_OsVthSchedule
 *********************************************************************************************************************/
/*! \brief       Handles runtime measurement of task response time MPs and CPU load MPs.
 *  \details     Calls Rtm_Schedule if any MP is of type flat or net execution time.
 *  \param[in]   FromThreadId            The thread which is stopped/preempted.
 *  \param[in]   FromThreadReason        The reason why the thread is stopped/preempted.
 *  \param[in]   ToThreadId              The thread which is started.
 *  \param[in]   ToThreadReason          The reason why the thread is started.
 *  \param[in]   CallerCoreId            The core on which the context switch is executed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different CallerCoreId
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON) && (DLT_OS_SOFTWARE_TRACING ==
 *               STD_ON).
 *  \ingroup     osTracing
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_OsVthSchedule(
  uint32 FromThreadId,
  uint32 FromThreadReason,
  uint32 ToThreadId,
  uint32 ToThreadReason,
  uint16 CallerCoreId);
#  endif /* (DLT_OS_SOFTWARE_TRACING == STD_ON) */

/**********************************************************************************************************************
 *  Dlt_GetLogInfo()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_GetLogInfo(uint8 Options, const uint8* AppId, const uint8* ContextId, uint8 Status, Dlt_LogInfoType* LogInfo)
 *  \brief       Returns log level and trace status of requested Dlt user.
 *  \details     The Dlt user is identified by the tuple of application id and context id. The application id and the
 *               context id may be set to 0x00000000 which is a wildcard value. In case that context id is 0, all
 *               registered context infos of the given application id are returned in LogInfo. If the application id
 *               is 0, all context infos of all registered application ids are returned.
 *  \param[in]   Options               Valid values are 6 and 7:
 *                                       6 means all Dlt context data without application description and context
 *                                         description is returned.
 *                                       7 means all Dlt context data including application description and context
 *                                         description is returned.
 *                                       But because the Dlt does not provide application and context descriptions,
 *                                         both values are handled equally.
 *  \param[in]   AppId                 The Application id indentifies a DLT user (in combination with the Context id).
 *  \param[in]   ContextId             The Context id indentifies a DLT user (in combination with the Application id).
 *  \param[in]   Status                Not used parameter.
 *  \param[out]  LogInfo               Pointer to Dlt_LogInfoType, provided by caller. There the request result is
 *                                       copied to.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetLogInfo(
  uint8 Options,
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) AppId,
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) ContextId,
  uint8 Status,
  P2VAR(Dlt_LogInfoType, AUTOMATIC, RTE_DLT_APPL_VAR) LogInfo);*/

/**********************************************************************************************************************
 *  Dlt_SetLogLevel()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetLogLevel(const uint8* AppId, const uint8*  ContextId, Dlt_MessageLogLevelType NewLogLevel)
 *  \brief       Notifies the DLT to change the log level of the specified DLT user.
 *  \details     The DLT user is specified by the couple of application id and context id. If the application id is 0,
 *               the log level of all registered users is changed. Otherwise if the context id is 0, the log level of
 *               all registered contexts of the specified application id is changed. Otherwise, only the log level of
 *               the specified user is changed.
 *  \param[in]   AppId            The application id specifying the SWC.
 *  \param[in]   ContextId        The context id specifying the runnable of the SWC.
 *  \param[in]   NewLogLevel      The new log level to be set for the specified DLT users.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetLogLevel(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  Dlt_MessageLogLevelType NewLogLevel);*/

/**********************************************************************************************************************
 *  Dlt_GetTraceStatus()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_GetTraceStatus(const uint8* AppId, const uint8* ContextId, boolean* TraceStatus)
 *  \brief       Returns the trace status of requested Dlt user.
 *  \details     The Dlt user is identified by the tuple of application id and context id.
 *  \param[in]   AppId                 The Application id indentifies a DLT user (in combination with the Context id).
 *  \param[in]   ContextId             The Context id indentifies a DLT user (in combination with the Application id).
 *  \param[out]  TraceStatus           The trace status of the requested Dlt user.
 *  \return      E_OK                  The request succeeded.
 *  \return      E_NOT_OK              The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetTraceStatus(
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) AppId,
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) ContextId,
  P2VAR(boolean, AUTOMATIC, RTE_DLT_APPL_VAR) TraceStatus);*/

/**********************************************************************************************************************
 *  Dlt_SetTraceStatus()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetTraceStatus(const uint8* AppId, const uint8* ContextId, boolean NewTraceStatus)
 *  \brief       Notifies the DLT to change the trace status of the specified DLT user.
 *  \details     The DLT user is specified by the couple of application id and context id. If the application id is 0,
 *               the trace status of all registered users is changed. Otherwise if the context id is 0, the trace
 *               status of all registered contexts of the specified application id is changed. Otherwise, only the
 *               trace status of the specified user is changed.
 *  \param[in]   AppId            The application id specifying the SWC.
 *  \param[in]   ContextId        The context id specifying the runnable of the SWC.
 *  \param[in]   NewTraceStatus   The new trace status to be set for the specified DLT users.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetTraceStatus(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  boolean NewTraceStatus);*/

/**********************************************************************************************************************
 *  Dlt_SetDefaultLogLevel()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetDefaultLogLevel(Dlt_MessageLogLevelType NewLogLevel)
 *  \brief       Notifies the DLT to change the default log level.
 *  \details     The default log level is used to set an initial log level of new registered contexts. This default
 *               value may be overwritten by a call to Dlt_SetLogLevel.
 *  \param[in]   NewLogLevel      The new default log level to be set for DLT users which register to DLT.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetDefaultLogLevel(Dlt_MessageLogLevelType NewLogLevel);*/

/**********************************************************************************************************************
 *  Dlt_GetDefaultLogLevel()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_GetDefaultLogLevel(Dlt_MessageLogLevelType* DefaultLogLevel)
 *  \brief       Returns the current default log level.
 *  \details     The default log level is used to set an initial log level of new registered contexts.
 *  \param[out]  DefaultLogLevel  The current default log level.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetDefaultLogLevel(
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) DefaultLogLevel);*/

/**********************************************************************************************************************
 *  Dlt_SetDefaultTraceStatus()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetDefaultTraceStatus(boolean NewTraceStatus, const uint8* LogChannelName)
 *  \brief       Notifies the DLT to change the default trace status.
 *  \details     The default trace status is used to set an initial trace status of new registered contexts. This
 *               default value may be overwritten by a call to Dlt_SetTraceStatus.
 *  \param[in]   NewTraceStatus   The new default trace status to be set for DLT users which register to DLT.
 *  \param[in]   LogChannelName   Optional and not supported parameter.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetDefaultTraceStatus(
  boolean NewTraceStatus,
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) LogChannelName);*/

/**********************************************************************************************************************
 *  Dlt_GetDefaultTraceStatus()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_GetDefaultTraceStatus(const uint8* LogChannelName, boolean* TraceStatus)
 *  \brief       Returns the current default trace status.
 *  \details     The default trace status is used to set an initial trace status of new registered contexts.
 *  \param[in]   LogChannelName   Optional and not supported parameter.
 *  \param[out]  TraceStatus      The current default trace status.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetDefaultTraceStatus(
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) LogChannelName
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus);*/

/**********************************************************************************************************************
 *  Dlt_ResetToFactoryDefault()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_ResetToFactoryDefault(void)
 *  \brief       Resets the DLT configuration to configured values.
 *  \details     -
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_ResetToFactoryDefault(void);*/

/**********************************************************************************************************************
 *  Dlt_SetMessageFiltering()
**********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetMessageFiltering(boolean Status)
 *  \brief       Enables or disables the message filtering.
 *  \details     -
 *  \param[in]   Status           TRUE: filtering is enabled; FALSE: filtering is disabled.
 *  \return      E_OK             The request succeeded.
 *  \return      E_NOT_OK         The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetMessageFiltering(boolean Status);*/

/**********************************************************************************************************************
 *  Dlt_GetLogChannelNames()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_GetLogChannelNames(uint8* NumberOfLogChannels, uint8* LogChannelNames)
 *  \brief         Returns the names of the configured log channels and their number.
 *  \details       The log channel names are always four bytes, therefore the number of log channels times four is the
 *                 length of LogChannelNames.
 *  \param[in,out] NumberOfLogChannels  In: Number of requested log channel names. May be 0.
 *                                      Out: Number of the configured log channels.
 *  \param[out]    LogChannelNames      Names of the configured log channels.
 *  \return        E_OK                 The request succeeded.
 *  \return        E_NOT_OK             The request failed.
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetLogChannelNames(
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) NumberOfLogChannels,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelNames);*/

/**********************************************************************************************************************
 *  Dlt_SetLogChannelAssignment()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetLogChannelAssignment(const uint8* AppId, const uint8*  ContextId, const uint8*  LogChannelName, Dlt_AssignmentOperation AddRemoveOp)
 *  \brief         Adds or removes the assignment of a Dlt user to a specific log channel.
 *  \details       -
 *  \param[in]     AppId                The application id to identify the Dlt user.
 *  \param[in]     ContextId            The context id to identify the Dlt user.
 *  \param[in]     LogChannelName       Name of the requested log channel.
 *  \param[in]     AddRemoveOp          Indicates if assignment is added or removed. Valid range:
 *                                      DLT_ASSIGN_ADD = 1, DLT_ASSIGN_REMOVE = 2.
 *  \return        E_OK                 The log channel could be successfully added or removed.
 *  \return        E_NOT_OK             Adding or removing failed.
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetLogChannelAssignment(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  Dlt_AssignmentOperation AddRemoveOp);*/

/**********************************************************************************************************************
 *  Dlt_SetLogChannelThreshold()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetLogChannelThreshold(const uint8* LogChannelName, Dlt_MessageLogLevelType NewThreshold, boolean NewTraceStatus)
 *  \brief         Sets the threshold and the trace status of a specific log channel to new values.
 *  \details       -
 *  \param[in]     LogChannelName       Name of the requested log channel.
 *  \param[in]     NewThreshold         New threshold value for the specified log channel.
 *  \param[in]     NewTraceStatus       New trace status for the specified log channel.
 *  \return        E_OK                 The request succeeded.
 *  \return        E_NOT_OK             The request failed.
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetLogChannelThreshold(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  Dlt_MessageLogLevelType NewThreshold,
  boolean NewTraceStatus);*/

/**********************************************************************************************************************
 *  Dlt_GetLogChannelThreshold()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_GetLogChannelThreshold(const uint8* LogChannelName, Dlt_MessageLogLevelType* Threshold, boolean* TraceStatus)
 *  \brief         Gets the current threshold and the trace status of a specific log channel.
 *  \details       -
 *  \param[in]     LogChannelName       Name of the requested log channel.
 *  \param[out]    NewThreshold         Current threshold value of the specified log channel.
 *  \param[out]    NewTraceStatus       Current trace status of the specified log channel.
 *  \return        E_OK                 The request succeeded.
 *  \return        E_NOT_OK             The request failed.
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetLogChannelThreshold(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) Threshold,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus);*/

/**********************************************************************************************************************
 *  Dlt_StoreConfiguration()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_StoreConfiguration(void)
 *  \brief       Stores the current Dlt configuration in non-volatile memory.
 *  \details     -
 *  \return      E_OK                The request succeeded.
 *  \return      E_NOT_OK            The request failed.
 *  \return      DLT_E_NOT_SUPPORTED The request is not supported.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, DLT_CODE) Dlt_StoreConfiguration(void);*/

/**********************************************************************************************************************
 *  Dlt_SetLogChannelDebugMode()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_SetLogChannelDebugMode(const uint8* LogChannelName, Dlt_DebugModeType NewDebugMode)
 *  \brief       Requests to set a new debug mode.
 *  \details     This is rejected if the given log channel is invalid, or the requested debug mode is already active,
 *               or a debug mode change is still executed.
 *               The debug mode change is only requested, the actual change is done in Dlt_MainFunction.
 *               A debug mode change results in emptying the currently buffered Dlt messages!
 *  \param[in]   LogChannelName      Name of the requested log channel.
 *  \param[in]   NewDebugMode        The requested new debug mode.
 *  \return      E_OK                               The request succeeded.
 *  \return      E_NOT_OK                           The request failed.
 *  \return      DLT_E_DEBUG_MODE_ALREADY_ACTIVE    The requested debug mode is already active.
 *  \return      DLT_E_DEBUG_MODE_ALREADY_REQUESTED The requested debug mode was already requested before, but is not active yet.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE (for different log channels)
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, DLT_CODE) Dlt_SetLogChannelDebugMode(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  Dlt_DebugModeType NewDebugMode);*/

/**********************************************************************************************************************
 *  Dlt_GetLogChannelDebugMode()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_GetLogChannelDebugMode(const uint8* LogChannelName, Dlt_DebugModeType* DebugModePtr)
 *  \brief       Requests the current debug mode.
 *  \details     This is rejected if the given log channel is invalid.
 *  \param[in]   LogChannelName      Name of the requested log channel.
 *  \param[in]   DebugModePtr        The currently active debug mode.
 *  \return      E_OK                The request succeeded.
 *  \return      E_NOT_OK            The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE (for different log channels)
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, DLT_CODE) Dlt_GetLogChannelDebugMode(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  P2VAR(Dlt_DebugModeType, AUTOMATIC, DLT_APPL_VAR) DebugModePtr);*/

/**********************************************************************************************************************
 *  Dlt_TriggerLogChannelDebugEvent()
 **********************************************************************************************************************/
/*! \fn Std_ReturnType Dlt_TriggerLogChannelDebugEvent(const uint8* LogChannelName)
 *  \brief       Triggers a debug event.
 *  \details     This is rejected if the given log channel is invalid, or the requested debug mode is already active,
 *               or a debug mode change is still executed.
 *  \param[in]   LogChannelName      Name of the requested log channel.
 *  \return      E_OK                The request succeeded.
 *  \return      E_NOT_OK            The request failed.
 *  \return      DLT_E_PENDING       The request cannot be executed now, try again later. Reasons are that the debug
 *                                   mode currently changes, or that a debug event is already executed.
 *  \return      DLT_E_DEBUG_MODE_WRONG The currently active debug mode does not support debug events.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE (for different log channels)
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
/* FUNC(Std_ReturnType, DLT_CODE) Dlt_TriggerLogChannelDebugEvent(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName);*/

#  if (DLT_INJECTION_CALL_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  Dlt_InjectCall()
 **********************************************************************************************************************/
/*! \brief       Forwards injection call to application.
 *  \details     -
 *  \param[in]   SessionId        Identifies the SW-C to which this request should be forwarded.
 *  \param[in]   AppId            Identifies together with context id the targeted Dlt user.
 *  \param[in]   ContextId        Identifies together with application id the targeted Dlt user.
 *  \param[in]   ServiceId        The service which is requested to be executed. Valid range is 0xFFF - 0xFFFFFFFF.
 *  \param[in]   DataLength       The length of service specific data.
 *  \param[in]   Data             The data required for the service.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires AppId != NULL_PTR;
 *               requires ContextId != NULL_PTR;
 *               requires Data != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON) and (DLT_INJECTION_CALL_SUPPORT
 *               == STD_ON).
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_InjectCall(
  Dlt_SessionIDType SessionId,
  P2CONST(uint8, AUTOMATIC, DLT_CONST) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_CONST) ContextId,
  uint32 ServiceId,
  uint32 DataLength,
  P2CONST(uint8, AUTOMATIC, DLT_CONST) Data);
#  endif
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

# if (DLT_IS_DEM_PRESENT == STD_ON)
#  if (DLT_DEM_EVENT_FILTERING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Appl_DltDemEventFilterCbk()
**********************************************************************************************************************/
/*! \brief       Checks if DEM event status has changed.
 *  \details     Must be implemented by user and should be used to compare old event status and new event status. The
 *               result should be returned.
 *  \param[in]   EventId          Identification of an Event by assigned event number. The Event number is configured
 *                                in the Dem:
*                                   Min.: 1 (0: Indication of no Event or Failure).
*                                   Max.: Result of configuration of Event Numbers in Dem (Max is either 255 or 65535).
 *  \param[in]   EventStatusOld   Extended event status before change.
 *  \param[in]   EventStatusNew   Detected / reported of event status.
 *  \return      TRUE             The DEM event has passed the filter (log message will be generated by the DLT).
 *  \return      FALSE            The DEM event has not passed the filter (no log message will be generated).
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_IS_DEM_PRESENT == STD_ON) && (DLT_DEM_EVENT_FILTERING_ENABLED
 *               = STD_ON).
 *********************************************************************************************************************/
FUNC(boolean, DLT_CODE) Appl_DltDemEventFilterCbk(
  Dem_EventIdType EventId,
  Dem_EventStatusExtendedType EventStatusOld,
  Dem_EventStatusExtendedType EventStatusNew);

#  endif
# endif

/**********************************************************************************************************************
 *  Dlt_MainFunction()
**********************************************************************************************************************/
/*! \fn          void Dlt_MainFunction(void)
 *  \brief       Checks periodically the Dlt status.
 *  \details     Handles cyclically the state machine(s).
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
/* FUNC(void, DLT_CODE) Dlt_MainFunction(void); */

# define DLT_STOP_SEC_CODE
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  EXCLUSIVE AREA DEFINITION
 **********************************************************************************************************************/
/*!
 * \exclusivearea DLT_EXCLUSIVE_AREA_APIS
 * Protects the global variables against concurrent modifications in case of reentrant calls.
 * \protects Buffers, states, and global variables
 * \usedin Dlt_MainFunction, Dlt_SetState, Dlt_RegisterContext, Dlt_DetForwardErrorTrace, Dlt_DemTriggerOnEventStatus, Dlt_SendLogMessage, Dlt_SendTraceMessage
 * \exclude All other APIs and callback functions.
 * \length LONG This exclusive area covers calls to several sub-functions.
 * \endexclusivearea
 */

/*!
 * \exclusivearea DLT_EXCLUSIVE_AREA_TX
 * Protects the global buffer against concurrent modifications in case of reentrant calls.
 * \protects Dlt_SendPdu_DataLength, Dlt_SendPdu_DataPtr
 * \usedin Dlt_CopyTxData
 * \exclude All APIs and other callback functions.
 * \length MEDIUM Copy of tx data to lower layer's buffer.
 * \endexclusivearea
 */

/*!
 * \exclusivearea DLT_EXCLUSIVE_AREA_RX
 * Protects the global buffer against concurrent modifications in case of reentrant calls.
 * \protects Dlt_ReceiveBuffer, Dlt_RxInfo.DataLengthToBeReceived, Dlt_RxInfo.CopyRxDataCounter, Dlt_RxInfo.RxIndicationOccured
 * \usedin Dlt_StartOfReception, Dlt_CopyRxData, Dlt_TpRxIndication
 * \exclude All APIs and other callback functions.
 * \length MEDIUM Copy of rx data from lower layer's buffer.
 * \endexclusivearea
 */

#endif

/**********************************************************************************************************************
 *  END OF FILE: Dlt.h
 *********************************************************************************************************************/
