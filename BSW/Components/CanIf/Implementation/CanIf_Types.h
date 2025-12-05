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
/**        \file  CanIf_Types.h
 *        \brief  CAN Interface type definition header file.
 *
 *      \details  This file contains types which are required and used by the AUTOSAR CAN Interface module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  05.00.00  2012-07-02  visrna  ESCAN00060132 Support ASR 4.0 Rev 3
 *  ...                                         see older versions for previous history
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (CANIF_TYPES_H)
# define CANIF_TYPES_H

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Configuration variants */
# define CANIF_CFGVAR_PRECOMPILETIME           1u
# define CANIF_CFGVAR_POSTBUILDTIME            3u

/* Development Errors */
# define CANIF_E_PARAM_CANID                   10u
# define CANIF_E_PARAM_DLC                     11u
# define CANIF_E_PARAM_HRH                     12u
# define CANIF_E_PARAM_LPDU                    13u
# define CANIF_E_PARAM_CONTROLLER              14u
# define CANIF_E_PARAM_CONTROLLERID            15u
# define CANIF_E_PARAM_WAKEUPSOURCE            16u
# define CANIF_E_PARAM_TRCV                    17u
# define CANIF_E_PARAM_TRCVMODE                18u
# define CANIF_E_PARAM_TRCVWAKEUPMODE          19u
# define CANIF_E_PARAM_POINTER                 20u
# define CANIF_E_PARAM_CTRLMODE                21u
# define CANIF_E_PARAM_PDU_MODE                22u
# define CANIF_E_PARAM_CAN_ERROR               23u

# define CANIF_E_UNINIT                        30u

# define CANIF_E_INVALID_TXPDUID               50u
# define CANIF_E_INVALID_RXPDUID               60u
# define CANIF_E_INVALID_DLC                   61u
# define CANIF_E_DATA_LENGTH_MISMATCH          62u
# define CANIF_E_STOPPED                       70u
# define CANIF_E_NOT_SLEEP                     71u
# define CANIF_E_TXPDU_LENGTH_EXCEEDED         90u

/* Not defined by AUTOSAR */
/* \trace SPEC-1881 */
# define CANIF_E_NO_ERROR                      0u
# define CANIF_E_INVALID_PDURECEPTIONMODE      44u
# define CANIF_E_CONFIG                        45u
# define CANIF_E_FATAL                         46u
# define CANIF_E_INVALID_SA                    47u
# define CANIF_E_INVALID_DA                    48u
# define CANIF_E_INVALID_CANIDTYPESIZE         49u
# define CANIF_E_INVALID_DLC_METADATA          50u

# define CANIF_E_FULL_TX_BUFFER_FIFO           51u /* FEAT-1429 */
# define CANIF_E_INVALID_DOUBLEHASH_CALC       52u /* ESCAN00087755 */

# define CANIF_E_TX_BUFFER_TRANSMIT            53u /* CAN-1904 */

# define CANIF_E_INVALID_XLPARAM               54u

/* Module ID of CAN Interface */
# define CANIF_MODULE_ID                       60u

/* Vendor ID of CAN Interface */ /* CANIF726 */
# define CANIF_VENDOR_ID                       0x001Eu

/* Search algorithm */
# define CANIF_LINEAR                          1u
# define CANIF_DOUBLE_HASH                     2u


# define CanIf_TrcvIndex_INVALID               ((uint8)0xFFu)


/* API IDs for Error Reporting */
# define CANIF_INIT_API                        1u
/* # define CANIF_INITCONTROLLER_API           2u */
# define CANIF_SETCONTROLLERMODE_API           3u
# define CANIF_GETCONTROLLERMODE_API           4u
# define CANIF_TRANSMIT_API                    5u
/* # define CANIF_READRXDATA_API               6u */

/* # define CANIF_READTXNOTIFSTATUS_API        7u */
/* # define CANIF_READRXNOTIFSTATUS_API        8u */

# define CANIF_SETPDUMODE_API                  9u
# define CANIF_GETPDUMODE_API                  10u
# define CANIF_GETVERSIONINFO_API              11u

# define CANIF_SETDYNAMICTXID_API_ID           12u

# define CANIF_SETTRCVMODE_API                 13u
# define CANIF_GETTRCVMODE_API                 14u
# define CANIF_GETTRCVWAKEUPREASON_API         15u
# define CANIF_SETTRCVWAKEUPMODE_API           16u

# define CANIF_CHECKWAKEUP_API                 17u

# define CANIF_CHECKVALIDATIONUP_API           18u

# define CANIF_TXCONFIRMATION_API              19u
# define CANIF_RXINDICATION_API                20u

# define CANIF_CANCELTXCONFIRMATION_API        21u

# define CANIF_CONTROLLERBUSOFF_API            22u

# define CANIF_CONTROLLERMODEINDICATION_API    23u
# define CANIF_TRCVMODEINDICATION_API          24u

# define CANIF_GETTXCONFIRMATIONSTATE_API      25u

# define CANIF_CONFIRMPNAVAILABILITY_API       26u  /* 0x1A */
# define CANIF_BAUDRATECHECK_API               28u  /* 0x1C */
# define CANIF_BAUDRATECHANGE_API              27u  /* 0x1B */
# define CANIF_BAUDRATESET_API                 39u  /* 0x27 */

# define CANIF_CLEARTRCVWUFFLAG_API            30u
# define CANIF_CHECKTRCVWAKEFLAG_API           31u
# define CANIF_CLEARTRCVWUFFLAGINDICATION_API  32u
# define CANIF_CHECKTRCVWAKEFLAGINDICATION_API 33u

# define CANIF_GETCONTROLLERERRORSTATE_API     75u
# define CANIF_ENABLEBUSMIRRORING_API          76u
# define CANIF_GETCONTROLLERRXERRORCOUNTER_API 77u
# define CANIF_GETCONTROLLERTXERRORCOUNTER_API 78u
# define CANIF_CONTROLLERERRORSTATEPASSIVE_API 79u
# define CANIF_ERRORNOTIFICATION_API           80u

# define CANIF_XLRXINDICATION_API              85u

/* Not defined by AUTOSAR */
# define CANIF_APPL_GENERICCONFIRMATION_API    243u

# define CANIF_RAMCHECKEXECUTE_API             249u  /* FEAT-598 */
# define CANIF_RAMCHECKENABLEMAILBOX_API       248u  /* FEAT-598 */
# define CANIF_RAMCHECKENABLECONTROLLER_API    247u  /* FEAT-598 */
# define CANIF_SETPDURECEPTIONMODE_API         246u  /* FEAT-2024 */
# define CANIF_RAMCHECKCORRUPTMAILBOX_API      245u  /* FEAT-598 */
# define CANIF_RAMCHECKCORRUPTCONTROLLER_API   244u  /* FEAT-598 */

# define CANIF_CANCELTRANSMIT_API              250u
# define CANIF_TXNOTIFICATION_API              251u

/* J1939 */
# define CANIF_SETADDRESSTABLEENTRY_API        252u
# define CANIF_RESETADDRESSTABLEENTRY_API      253u
# define CANIF_GETTXADDRESSTABLEENTRY_API      254u
# define CANIF_GETRXADDRESSTABLEENTRY_API      255u

# define CANIF_J1939_DYN_ADDR_DISABLED         0u
# define CANIF_J1939_DYN_ADDR_EXT_CANID        1u
# define CANIF_J1939_DYN_ADDR_MIXED_CANID      2u

/* hard coded array size */
# define CANIF_STATIC_BUFFER_SIZE              8u

/**********************************************************************************************************************
 *  INCLUDES (CanIf_Types.h is only allowed to be included by CanIf_Cfg.h)
 *********************************************************************************************************************/

# include "Can_GeneralTypes.h"
# if ((CANIF_WAKEUP_SUPPORT == STD_ON) || (CANIF_WAKEUP_VALIDATION == STD_ON) || (CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME))
#  include "EcuM_Cbk.h"
# endif

/**********************************************************************************************************************
 *  COMPATIBILITY / DEFAULTS
 *********************************************************************************************************************/
 
# if !defined (CANIF_PN_WU_TX_PDU_FILTER)
#  error "CANIF_PN_WU_TX_PDU_FILTER is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_TRCV_HANDLING)
#  error "CANIF_TRCV_HANDLING is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_PN_TRCV_HANDLING)
#  error "CANIF_PN_TRCV_HANDLING is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_WAKEUP_VALIDATION)
#  error "CANIF_WAKEUP_VALIDATION is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_SETDYNAMICTXID_API)
#  error "CANIF_SETDYNAMICTXID_API is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_META_DATA_RX_SUPPORT)
#  error "CANIF_META_DATA_RX_SUPPORT is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_META_DATA_TX_SUPPORT)
#  error "CANIF_META_DATA_TX_SUPPORT is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_STATIC_FD_TXQUEUE)
#  error "CANIF_STATIC_FD_TXQUEUE is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_MULTIPLE_CANDRV_SUPPORT)
#  error "CANIF_MULTIPLE_CANDRV_SUPPORT is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_CANCEL_SUPPORT_API)
#  error "CANIF_CANCEL_SUPPORT_API is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_WAKEUP_SUPPORT)
#  error "CANIF_WAKEUP_SUPPORT is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_CHANGE_BAUDRATE_SUPPORT)
#  error "CANIF_CHANGE_BAUDRATE_SUPPORT is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_SET_BAUDRATE_API)
#  error "CANIF_SET_BAUDRATE_API is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

# if !defined (CANIF_J1939_DYN_ADDR_SUPPORT)
#  error "CANIF_J1939_DYN_ADDR_SUPPORT is undefined. Please check the reason why it is not generated and set the #define manually, if necessary!"
# endif

/* \trace SPEC-1836, changed to support non Vector CanDrv */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  define CanIf_HwHandleType uint16
# else
#  define CanIf_HwHandleType Can_HwHandleType
# endif

/* Not each PlatformTypes.h defines CPU_TYPE_64 (ESCAN00109109) */
# if !defined (CPU_TYPE_64) /* COV_CANIF_REDEFINITION_PROTECTION */
#  define CPU_TYPE_64  64u
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* \trace SPEC-5204 */

typedef enum
{
  CANIF_CS_UNINIT = 0u,
  CANIF_CS_STOPPED,
  CANIF_CS_STARTED,
  CANIF_CS_SLEEP
} CanIf_ControllerModeType;


typedef enum
{
  CANIF_SET_OFFLINE = 0u,
  CANIF_SET_RX_OFFLINE,
  CANIF_SET_RX_ONLINE,
  CANIF_SET_TX_OFFLINE,
  CANIF_SET_TX_ONLINE,
  CANIF_SET_ONLINE,
  CANIF_SET_TX_OFFLINE_ACTIVE,
# if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
  CANIF_SET_ONLINE_WAKF,
  CANIF_SET_TX_ONLINE_WAKF,
# endif
  CANIF_NO_MODE_PDU_SET_MODE_TYPE_END /* no PDU mode, is used to determine the size of CanIfState */
} CanIf_PduSetModeType;


typedef enum
{
  CANIF_GET_OFFLINE = 0u,
  CANIF_GET_RX_ONLINE,
  CANIF_GET_TX_ONLINE,
  CANIF_GET_ONLINE,
  CANIF_GET_OFFLINE_ACTIVE,
  CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE, 
# if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
  CANIF_GET_ONLINE_WAKF,              
  CANIF_GET_TX_ONLINE_WAKF,           
# endif
  CANIF_NO_MODE_PDU_GET_MODE_TYPE_END /* no PDU mode, is used to determine the size of CanIfState */
} CanIf_PduGetModeType;


typedef enum
{
  CANIF_RxBasicCANMailbox = 0u,
  CANIF_RxFullCANMailbox,
  CANIF_TxBasicCANMailbox,
  CANIF_TxFullCANMailbox,
  CANIF_UnusedCANMailbox
} CanIf_MailBoxTypeType;


typedef enum
{
  CanIf_SimpleRxIndicationLayout = 0u,
  CanIf_AdvancedRxIndicationLayout,
  CanIf_NmOsekRxIndicationLayout,
  CanIf_CddRxIndicationLayout
} CanIf_RxIndicationLayoutType;


typedef enum
{
  CANIF_NO_NOTIFICATION = 0u,
  CANIF_TX_RX_NOTIFICATION
} CanIf_NotifStatusType;

typedef enum 
{
  CANIF_WAKEUPREQUEST_NONE = 0u,
  CANIF_WAKEUPREQUEST_CAN  = 1u,
  CANIF_WAKEUPREQUEST_TRCV = 2u
} CanIf_WakeUpTargetType;

typedef enum
{
  CANIF_WUVALIDATION_IDLE     = 0u,
  CANIF_WUVALIDATION_ARMED    = 1u,
  CANIF_WUVALIDATION_DETECTED = 2u
} CanIf_WakeUpValidationStateType;

typedef enum
{
  CANIF_DO_TRANSMIT     = 0u,/*!< CANIF_DO_TRANSMIT */
  CANIF_DO_NOT_TRANSMIT = 1u /*!< CANIF_DO_NOT_TRANSMIT */
} CanIf_TransmitStateType;

typedef enum
{
  CANIF_MSG_TYPE_CAN = 0u,
  CANIF_MSG_TYPE_FD_CAN,
  CANIF_MSG_TYPE_NO_FD_CAN
} CanIf_MsgType;

typedef enum
{
  CANIF_RMT_RECEIVE_STOP      = 0u,
  CANIF_RMT_IGNORE_CONTINUE   = 1u
} CanIf_ReceptionModeType;

typedef struct sCanIf_RxSearchParamsType
{
  Can_IdType    eCanId;
  PduIdType     eRxPduId;
  PduIdType     eRxPduIdMatch;
# if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON) || (CANIF_META_DATA_RX_SUPPORT == STD_ON)
  CanIf_MsgType eRxMsgType;
# endif
  CanIf_HwHandleType eHrh;
  boolean       eContinueSearch;
} CanIf_RxSearchParamsType;

/* Type of confirmation function */
/* typedef void (*CanIf_TxConfirmationFctType)(PduIdType CanTxPduId); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_TxConfirmationFctType) (VAR(PduIdType, AUTOMATIC) CanTxPduId);


/* Type of indication function */
/* Simple type for CanNm, PduR */
/* typedef void (*CanIf_SimpleRxIndicationFctType)(PduIdType CanRxPduId, const uint8 *CanSduPtr); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_SimpleRxIndicationFctType) (VAR(PduIdType, AUTOMATIC) CanRxPduId, P2CONST(uint8, AUTOMATIC, CANIF_UL_STANDARD_VAR) CanSduPtr);

/* Advanced type for CanTP */
/* typedef void (*CanIf_AdvancedRxIndicationFctType)(PduIdType CanRxPduId, const PduInfoType *PduInfoPtr); */
/* PduInfoPtr is created on stack -> AUTOMATIC */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_AdvancedRxIndicationFctType) (VAR(PduIdType, AUTOMATIC) CanRxPduId, P2CONST(PduInfoType, AUTOMATIC, CANIF_UL_ADVANCED_VAR) PduInfoPtr);

/* NmOsek type */
/* typedef void (*CanIf_NmOsekRxIndicationFctType)(PduIdType CanRxPduId, const uint8 *CanSduPtr, Can_IdType CanId); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_NmOsekRxIndicationFctType) (VAR(PduIdType, AUTOMATIC) CanRxPduId, P2CONST(uint8, AUTOMATIC, CANIF_UL_OSEKNM_VAR) CanSduPtr, VAR(Can_IdType, AUTOMATIC) CanId);

/* CDD type */
/* typedef void (*CanIf_CddRxIndicationFctType)(PduIdType CanRxPduId, const PduInfoType *PduInfoPtr, Can_IdType CanId); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_CddRxIndicationFctType) (VAR(PduIdType, AUTOMATIC) CanRxPduId, P2CONST(PduInfoType, AUTOMATIC, CANIF_UL_ADVANCED_VAR) PduInfoPtr, VAR(Can_IdType, AUTOMATIC) CanId);

/* CANIF687 */
/* typedef void (*CanIf_CtrlModeIndicationFctType) (uint8 ControllerId, CanIf_ControllerModeType ControllerMode); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_CtrlModeIndicationFctType) (VAR(uint8, AUTOMATIC) ControllerId, VAR(CanIf_ControllerModeType, AUTOMATIC) ControllerMode);

/* \trace SPEC-1839 */
/* typedef void (*CanIf_BusOffNotificationFctType)(uint8 ControllerId); */
typedef P2FUNC(void, CANIF_APPL_STATE_CODE, CanIf_BusOffNotificationFctType) (VAR(uint8, AUTOMATIC) ControllerId);

# if (CANIF_WAKEUP_VALIDATION == STD_ON)
/* WakeUp Validation */
/* \trace SPEC-5053 */
/* typedef void (*CanIf_WakeUpValidationFctType)(EcuM_WakeupSourceType, CanWakeupEvents); */
typedef P2FUNC(void, CANIF_APPL_STATE_CODE, CanIf_WakeUpValidationFctType) (VAR(EcuM_WakeupSourceType, AUTOMATIC) CanWakeupEvents);
# endif


# if (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON)
/* typedef void (*CanIf_RamCheckCorruptMailboxIndFctType)(uint8 ControllerId, CanIf_HwHandleType HwHandle); */
typedef P2FUNC(void, CANIF_APPL_STATE_CODE, CanIf_RamCheckCorruptMailboxIndFctType) (VAR(uint8, AUTOMATIC) ControllerId, VAR(CanIf_HwHandleType, AUTOMATIC) HwHandle);

/* typedef void (*CanIf_RamCheckCorruptControllerIndFctType)(uint8 ControllerId); */
typedef P2FUNC(void, CANIF_APPL_STATE_CODE, CanIf_RamCheckCorruptControllerIndFctType) (VAR(uint8, AUTOMATIC) ControllerId);
# endif /* (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON) */


# if(CANIF_SETDYNAMICRXID_API == STD_ON)
typedef P2FUNC(void, CANIF_APPL_STATE_CODE, CanIf_SetDynamicRxIdFctType) (VAR(PduIdType, AUTOMATIC) RxPduId, P2VAR(Can_IdType, AUTOMATIC, CANIF_APPL_STATE_VAR) CanId);
# endif


# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON) /* function pointer definition needed if multiple CanDrivers used */
/* function pointer types needed for generated CanDriver function calls */

#  if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_OFF)
/* Enabled for: MSRC, AR403, AR421, MSRC/AR403, MSRC/AR421 */
/* typedef Can_ReturnType (*CanIf_Can_SetControllerModeFctType) (Controller, Transition); */
typedef P2FUNC(Can_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_SetControllerModeFctType) (VAR(uint8, AUTOMATIC) Controller, VAR(Can_StateTransitionType, AUTOMATIC) Transition);

/* typedef Can_ReturnType (*CanIf_Can_WriteFctType) (Hth, PduInfo); */
typedef P2FUNC(Can_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_WriteFctType) (VAR(Can_HwHandleType, AUTOMATIC) Hth, P2CONST(Can_PduType, AUTOMATIC, CANIF_VAR_STACK) PduInfo);

#  elif (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_OFF)
/* Enabled for: AR431 */
/* typedef Std_ReturnType (*CanIf_Can_SetControllerModeFctType) (Controller, Transition); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_SetControllerModeFctType) (VAR(uint8, AUTOMATIC) Controller, VAR(Can_ControllerStateType, AUTOMATIC) Transition);

/* typedef Std_ReturnType (*CanIf_Can_WriteFctType) (Hth, PduInfo); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_WriteFctType) (VAR(Can_HwHandleType, AUTOMATIC) Hth, P2CONST(Can_PduType, AUTOMATIC, CANIF_VAR_STACK) PduInfo);

#  else
/* Enabled for: MSRC/AR431 */
/* CanIf_Can_SetControllerModeAr403FctType/CanIf_Can_WriteAr403FctType is used for MSRC
   CanIf_Can_SetControllerModeAr431FctType/CanIf_Can_WriteAr431FctType is used for AR431 */

/* typedef Can_ReturnType (*CanIf_Can_SetControllerModeFctType) (Controller, Transition); */
typedef P2FUNC(Can_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_SetControllerModeAr403FctType) (VAR(uint8, AUTOMATIC) Controller, VAR(Can_StateTransitionType, AUTOMATIC) Transition);

/* typedef Std_ReturnType (*CanIf_Can_SetControllerModeFctType) (Controller, Transition); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_SetControllerModeAr431FctType) (VAR(uint8, AUTOMATIC) Controller, VAR(Can_ControllerStateType, AUTOMATIC) Transition);

typedef union
{ /* PRQA S 0750 1 */ /* MD_MSR_Union */
  CanIf_Can_SetControllerModeAr403FctType  CanIf_Can_SetControllerModeAr403Fct;
  CanIf_Can_SetControllerModeAr431FctType  CanIf_Can_SetControllerModeAr431Fct;
} CanIf_Can_SetControllerModeFctType;

/* typedef Can_ReturnType (*CanIf_Can_WriteFctType) (Hth, PduInfo); */
typedef P2FUNC(Can_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_WriteAr403FctType) (VAR(Can_HwHandleType, AUTOMATIC) Hth, P2CONST(Can_PduType, AUTOMATIC, CANIF_VAR_STACK) PduInfo);

/* typedef Std_ReturnType (*CanIf_Can_WriteFctType) (Hth, PduInfo); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_WriteAr431FctType) (VAR(Can_HwHandleType, AUTOMATIC) Hth, P2CONST(Can_PduType, AUTOMATIC, CANIF_VAR_STACK) PduInfo);

typedef union
{ /* PRQA S 0750 1 */ /* MD_MSR_Union */
  CanIf_Can_WriteAr403FctType  CanIf_Can_WriteAr403Fct;
  CanIf_Can_WriteAr431FctType  CanIf_Can_WriteAr431Fct;
} CanIf_Can_WriteFctType;
#  endif

#  if (CANIF_CANCEL_SUPPORT_API == STD_ON)
/* typedef void (*CanIf_Can_CancelTxFctType) (Hth, PduId); */
typedef P2FUNC(void, CANIF_CBK_CANDRV_CODE, CanIf_Can_CancelTxFctType) (VAR(Can_HwHandleType, AUTOMATIC) Hth, VAR(PduIdType, AUTOMATIC) PduId);
#  endif

#  if ((CANIF_WAKEUP_SUPPORT == STD_ON) && (CANIF_CANDRV_WAKEUP_HANDLING))
#   if (CANIF_SUPPORT_CANDRV_EQC_AR_431_OR_MSRC == STD_OFF)
/* Enabled for: AR403, AR421 */
/* typedef Can_ReturnType (*CanIf_Can_CheckWakeupFctType) (Controller); */
typedef P2FUNC(Can_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_CheckWakeupFctType) (VAR(uint8, AUTOMATIC) Controller);

#   elif (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_OFF)
/* Enabled for: MSRC, AR431, MSRC/AR431 */
/* typedef Std_ReturnType (*CanIf_Can_CheckWakeupFctType) (Controller); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_CheckWakeupFctType) (VAR(uint8, AUTOMATIC) Controller);

#   else
/* Enabled for: MSRC/AR403, MSRC/AR421 */
/* CanIf_Can_CheckWakeupAr403FctType is used for EQC AR403/AR421
   CanIf_Can_CheckWakeupAr431FctType is used for EQC MSRC */

/* typedef Can_ReturnType (*CanIf_Can_CheckWakeupFctType) (Controller); */
typedef P2FUNC(Can_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_CheckWakeupAr403FctType) (VAR(uint8, AUTOMATIC) Controller);

/* typedef Std_ReturnType (*CanIf_Can_CheckWakeupFctType) (Controller); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_CheckWakeupAr431FctType) (VAR(uint8, AUTOMATIC) Controller);

typedef union
{ /* PRQA S 0750 1 */ /* MD_MSR_Union */
  CanIf_Can_CheckWakeupAr403FctType  CanIf_Can_CheckWakeupAr403Fct;
  CanIf_Can_CheckWakeupAr431FctType  CanIf_Can_CheckWakeupAr431Fct;
} CanIf_Can_CheckWakeupFctType;

#   endif
#  endif /* CANIF_WAKEUP_SUPPORT == STD_ON */

#  if (CANIF_CHANGE_BAUDRATE_SUPPORT == STD_ON)
/* typedef Std_ReturnType (*CanIf_Can_CheckBaudrateFctType) (Controller, Baudrate); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_CheckBaudrateFctType) (VAR(uint8, AUTOMATIC) Controller, CONST(uint16, AUTOMATIC) Baudrate);

/* typedef Std_ReturnType (*CanIf_Can_ChangeBaudrateFctType) (Controller, Baudrate); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_ChangeBaudrateFctType) (VAR(uint8, AUTOMATIC) Controller, CONST(uint16, AUTOMATIC) Baudrate);
#  endif

#  if (CANIF_SET_BAUDRATE_API == STD_ON)
/* typedef Std_ReturnType (*CanIf_Can_SetBaudrateFctType) (Controller, BaudRateConfigID); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_SetBaudrateFctType) (VAR(uint8, AUTOMATIC) Controller, VAR(uint16, AUTOMATIC) BaudRateConfigID);
#  endif

#  if (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON)
/* typedef void (*CanIf_Can_RamCheckExecuteFctType) (ControllerId); */
typedef P2FUNC(void, CANIF_CBK_CANDRV_CODE, CanIf_Can_RamCheckExecuteFctType) (VAR(uint8, AUTOMATIC) Controller);

/* typedef void (*CanIf_Can_RamCheckEnableMailboxFctType) (ControllerId, HwHandle); */
typedef P2FUNC(void, CANIF_CBK_CANDRV_CODE, CanIf_Can_RamCheckEnableMailboxFctType) (VAR(Can_HwHandleType, AUTOMATIC) htrh);

/* typedef void (*CanIf_Can_RamCheckEnableControllerFctType) (ControllerId); */
typedef P2FUNC(void, CANIF_CBK_CANDRV_CODE, CanIf_Can_RamCheckEnableControllerFctType) (VAR(uint8, AUTOMATIC) Controller);
#  endif /* (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON) */

#  if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/* typedef Std_ReturnType (*CanIf_Can_GetControllerErrorStateFctType) (Controller, ErrorStatePtr); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_GetControllerErrorStateFctType) (VAR(uint8, AUTOMATIC) Controller, P2VAR(Can_ErrorStateType, AUTOMATIC, CANIF_VAR_STACK) ErrorStatePtr);

/* typedef Std_ReturnType (*CanIf_Can_GetControllerRxErrorCounterFctType) (Controller, RxErrorCounterPtr); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_GetControllerRxErrorCounterFctType) (VAR(uint8, AUTOMATIC) Controller, P2VAR(uint8, AUTOMATIC, CANIF_VAR_STACK) RxErrorCounterPtr);

/* typedef Std_ReturnType (*CanIf_Can_GetControllerTxErrorCounterFctType) (Controller, TxErrorCounterPtr); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_Can_GetControllerTxErrorCounterFctType) (VAR(uint8, AUTOMATIC) Controller, P2VAR(uint8, AUTOMATIC, CANIF_VAR_STACK) TxErrorCounterPtr);
#  endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

#  if (CANIF_SUPPORT_CAN_XL == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_CANDRV_CODE, CanIf_CanXL_WriteFctType) (VAR(Can_HwHandleType, AUTOMATIC) Mailbox, P2CONST(CanXL_PduType, AUTOMATIC, CANIF_VAR_STACK) PduInfo);
#  endif

# endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */


# if (CANIF_TRCV_HANDLING == STD_ON)
/* \trace SPEC-5279 */
/* typedef void (*CanIf_TrcvModeIndicationFctType) (uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_TrcvModeIndicationFctType) (VAR(uint8, AUTOMATIC) TransceiverId, VAR(CanTrcv_TrcvModeType, AUTOMATIC) TransceiverMode);

/* \trace SPEC-1967 */
/* typedef Std_ReturnType (*CanIf_CanTrcv_CheckWakeupFctType)(uint8 Transceiver); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_TRCV_CODE, CanIf_CanTrcv_CheckWakeupFctType) (VAR(uint8, AUTOMATIC) CanTrcvIndex);

/* \trace SPEC-1967 */
/* typedef Std_ReturnType (*CanIf_CanTrcv_SetOpModeFctType)(uint8 Transceiver, CanTrcv_TrcvModeType OpMode); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_TRCV_CODE, CanIf_CanTrcv_SetOpModeFctType) (VAR(uint8, AUTOMATIC) CanTrcvIndex, VAR(CanTrcv_TrcvModeType, AUTOMATIC) OpMode);

/* \trace SPEC-1967 */
/* typedef Std_ReturnType (*CanIf_CanTrcv_GetOpModeFctType)(uint8 Transceiver, CanTrcv_TrcvModeType *OpMode); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_TRCV_CODE, CanIf_CanTrcv_GetOpModeFctType) (VAR(uint8, AUTOMATIC) CanTrcvIndex, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_STATE_VAR) OpMode);

/* \trace SPEC-1967 */
/* typedef Std_ReturnType (*CanIf_CanTrcv_GetBusWuReasonFctType)(uint8 Transceiver, CanTrcv_TrcvWakeupReasonType *Reason); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_TRCV_CODE, CanIf_CanTrcv_GetBusWuReasonFctType) (VAR(uint8, AUTOMATIC) CanTrcvIndex, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_STATE_VAR)Reason);

/* \trace SPEC-1967 */
/* typedef Std_ReturnType (*CanIf_CanTrcv_SetWakeupModeFctType)(uint8 Transceiver, CanTrcv_TrcvWakeupModeType TrcvWakeupMode); */
typedef P2FUNC(Std_ReturnType, CANIF_CBK_TRCV_CODE, CanIf_CanTrcv_SetWakeupModeFctType) (VAR(uint8, AUTOMATIC) CanTrcvIndex, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);

#  if (CANIF_PN_TRCV_HANDLING == STD_ON)
/* CANIF821 */
/* typedef void (*CanIf_ConfirmPnAvailabilityFctType) (uint8 TransceiverId); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_ConfirmPnAvailabilityFctType) (VAR(uint8, AUTOMATIC) TransceiverId);

/* CANIF788 */
/* typedef void (*CanIf_ClearTrcvWufFlagIndicationFctType) (uint8 TransceiverId); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_ClearTrcvWufFlagIndicationFctType) (VAR(uint8, AUTOMATIC) TransceiverId);

/* CANIF814 */
/* typedef void (*CanIf_CheckTrcvWakeFlagIndicationFctType) (uint8 TransceiverId); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_CheckTrcvWakeFlagIndicationFctType) (VAR(uint8, AUTOMATIC) TransceiverId);

#   if(CANIF_TRCV_MAPPING == STD_ON)
/* typedef void (*CanIf_CanTrcv_ClearTrcvWufFlagFctType) (uint8 TransceiverId)*/
typedef P2FUNC(Std_ReturnType, CANIF_CBK_TRCV_CODE, CanIf_CanTrcv_ClearTrcvWufFlagFctType) (VAR(uint8, AUTOMATIC) CanTrcvIndex);

/* typedef void (*CanIf_CanTrcv_CheckWakeFlagFctType) (uint8 TransceiverId)*/
typedef P2FUNC(Std_ReturnType, CANIF_CBK_TRCV_CODE, CanIf_CanTrcv_CheckWakeFlagFctType) (VAR(uint8, AUTOMATIC) CanTrcvIndex);
#   endif

#  endif /* (CANIF_PN_TRCV_HANDLING == STD_ON) */
# endif /* (CANIF_TRCV_HANDLING == STD_ON) */

typedef union
{ /* PRQA S 0750 1 */ /* MD_MSR_Union */
  CanIf_SimpleRxIndicationFctType     eSimpleRxIndicationType;
  CanIf_AdvancedRxIndicationFctType   eAdvancedRxIndicationType;
  CanIf_NmOsekRxIndicationFctType     eNmOsekRxIndicationType;
  CanIf_CddRxIndicationFctType        eCddRxIndicationType;
} CanIf_RxIndicationFctType;

/* typedef boolean CanIf_CancelResultType; */
typedef boolean CanIf_CancelResultType;

/*** START - Tx-buffer-types - START ***/
typedef struct sCanIf_TxQueueBaseType
{
# if((CANIF_SETDYNAMICTXID_API == STD_ON) || (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) || (CANIF_META_DATA_TX_SUPPORT == STD_ON))
  Can_IdType    eCanId;     /* CAN-ID will be buffered */
# endif
  uint8         eSduLength; /* uint8: Is sufficient: 8-64 bytes */
} CanIf_TxQueueBaseType;


/* Base data type for "CanIfBufferCfg" with "CanIfTxBufferHandlingType == PRIO_BY_CANID". */
/* This type is to be instantiated "CanIfBufferSize"-times. */
typedef struct sCanIf_TxPrioByCanIdType
{
# if (CANIF_STATIC_FD_TXQUEUE == STD_OFF)
  uint8                 eSdu[CANIF_STATIC_BUFFER_SIZE]; /* const 64 bit */
# endif
  CanIf_TxQueueBaseType eBaseParams;                    /* CanId, SduLength */
} CanIf_TxPrioByCanIdType;

/* This type is to be instantiated one per each "CanIfBufferCfg" with "CanIfTxBufferHandlingType == PRIO_BY_CANID" is configured */
typedef struct sCanIf_TxBufferPrioByCanIdBaseType
{
  CanIf_TxBufferSizeType  eQueueCounter; /* How many elements are queued within the Tx-buffer. */
} CanIf_TxBufferPrioByCanIdBaseType;

/* Base data type for "CanIfBufferCfg" with "CanIfTxBufferHandlingType == FIFO". */
/* This type is to be instantiated "CanIfBufferSize"-times.  */
typedef struct sCanIf_TxFifoQueueBaseType
{
  CanIf_TxQueueBaseType eBaseParams; /* CanId, SduLength */
  PduIdType             eTxPduId;
# if (CANIF_CANCEL_SUPPORT_API == STD_ON)
  uint8                 eQueued;     /* Validity flag: 0: invalid, 1: valid */
# endif
} CanIf_TxFifoQueueBaseType;

/* This type is to be instantiated one per each "CanIfBufferCfg" with "CanIfTxBufferHandlingType == FIFO" */
typedef struct sCanIf_TxBufferFifoBaseType
{
  CanIf_TxBufferSizeType  eWriteIdx;     /* Next position to be written into FIFO */
  CanIf_TxBufferSizeType  eReadIdx;      /* Next position to be read from FIFO */
  CanIf_TxBufferSizeType  eQueueCounter; /* How many elements are queued within the Tx-buffer. */
} CanIf_TxBufferFifoBaseType;


/*** END - Tx-buffer-types (FEAT-1429, CANCORE-1199) - END ***/

/*** BEGIN - FEAT-1914 Data checksum - BEGIN ***/

/* Type for Data checksum Rx error indication function */
/* typedef void (*CanIf_DataChecksumRxErrFctType) (PduIdType CanIfRxPduId); */
typedef P2FUNC(void, CANIF_APPL_MSG_CODE, CanIf_DataChecksumRxErrFctType) (VAR(PduIdType, AUTOMATIC) CanIfRxPduId);

/*** END - FEAT-1914 Data checksum - END ***/

# if (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
#  if ((CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_ON) || (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON))

typedef enum
{
  CAN_DRV_EQC_MSRC,
  CAN_DRV_EQC_AR_403, /* Also used for EQC AR 4.2.1 */
  CAN_DRV_EQC_AR_431
} CanIf_CanDrvEqcArVersionType;

#  endif
# endif

#endif /* CANIF_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: CanIf_Types.h
 *********************************************************************************************************************/
