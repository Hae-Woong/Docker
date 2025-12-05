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
/**        \file  CanIf.h
 *        \brief  CAN Interface header file.
 *
 *      \details  This file contains the declaration of the public APIs and global data of the AUTOSAR 
 *                CAN Interface module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  05.00.00  2012-06-26  visrna  ESCAN00060132 Support ASR 4.0 Rev 3
 *                        visrna  ESCAN00058639 AR4-94: Wake-up validation optionally only via NM PDUs CanIfPublicWakeupCheckValidByNM
 *                        visrna  ESCAN00058642 AR4-95: Provide CanTrcv according ASR 4, Asynchronous Transceiver Mode Changes
 *  ...                                         see older versions for previous history
 *  09.00.00  2023-02-20  visgaz  CANCORE-1199  [CAN-IF] Remove configuration options for Tx-Buffer and Software filtering
 *                                ESCAN00112947 MISRA deviation: MISRA C:2012 Rule 9.1
 *  09.01.00  2023-03-23  visgaz  CANCORE-1833  [CAN-IF] [ASIL-D] Support CAN Driver combination 3rd party AR version 4.4.0 with MSRC
 *                                ESCAN00112356 Compiler error: Identifier "CAN_APPL_VAR" is undefined
 *  09.02.00  2023-04-11  visgaz  CANCORE-1835  [CAN-IF] [Implementation] Extend support of third party CAN Driver with different AR versions
 *                                CANCORE-493   [CAN-IF] Improve documentation in Impl and TechRef for CanIf_SetControllerMode/CANIF_EXCLUSIVE_AREA_4
 *  09.03.00  2023-06-06  visgaz  CANCORE-1771  [CAN-IF] Support dynamic Rx-PDUs
 *  11.00.00  2023-07-31  destler CANCORE-2096  [CAN-IF] Remove address translation inside ACKM / Add service functions to access the J1939 look up tables
 *                                ESCAN00115252 Multi-PG Container Tx-PDU with NULL SA (0xFE) is not transmitted
 *  11.01.00  2023-08-23  destler CANCORE-2088  [CAN-IF] Support CAN XL (Beta) - Tx path
 *            2023-10-06  destler CANCORE-1706  [CAN-IF] Support CAN XL (Beta) - Rx path
 *            2023-11-21  visgaz  CANCORE-2231  [CAN-IF] Support CAN XL (Beta) - Multiple CAN Driver support
 *  11.02.00  2024-03-13  visgaz  CANCORE-2527  [CAN-IF] Allow CAN transceiver drivers for multi-partition
 *********************************************************************************************************************/
 #if !defined (CANIF_H)
 # define CANIF_H

/* \trace SPEC-5285 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "CanIf_Cfg.h"

/* \trace SPEC-5204 */
/* CanIf_Types.h  is included by CanIf_Cfg.h */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Version of CAN Interface module */
# define CANIF_SW_MAJOR_VERSION                    0x11u
# define CANIF_SW_MINOR_VERSION                    0x02u
# define CANIF_SW_PATCH_VERSION                    0x00u

/* Autosar release version */
# define CANIF_AR_RELEASE_MAJOR_VERSION            0x04u
# define CANIF_AR_RELEASE_MINOR_VERSION            0x00u
# define CANIF_AR_RELEASE_REVISION_VERSION         0x03u

# define CANIF_API_MAJOR_VERSION                   CANIF_SW_MAJOR_VERSION
# define CANIF_API_MINOR_VERSION                   CANIF_SW_MINOR_VERSION
# define CANIF_API_PATCH_VERSION                   CANIF_SW_PATCH_VERSION

# define IF_ASRIFCAN_VERSION                       ((CANIF_SW_MAJOR_VERSION << 8u) | CANIF_SW_MINOR_VERSION)
# define IF_ASRIFCAN_RELEASE_VERSION               CANIF_SW_PATCH_VERSION

/* ----- GenData compatibility version ----- */
# define CANIF_GENDATA_COMPATIBILITY_VERSION       0x0212u

/* ----- Magic number ----- */
# define CANIF_MAGIC_NUMBER                        CANIF_FINAL_MAGIC_NUMBER

/* ----- CAN Id Mask ----- */
# define CANIF_EXT_CANID_MASK                      0x80000000u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# if (CANIF_DEV_ERROR_REPORT == STD_ON)
/* \trace SPEC-1967 */
#  define CanIf_Det_ReportError(APIID, ErrorCode)  ((void)Det_ReportError(CANIF_MODULE_ID, 0, (APIID), (ErrorCode))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 
# if !defined (CANIF_LOCAL) /* COV_CANIF_REDEFINITION_PROTECTION */
#  define CANIF_LOCAL static
# endif

# if !defined (CANIF_LOCAL_INLINE) /* COV_CANIF_REDEFINITION_PROTECTION */
#  define CANIF_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CANIF_START_SEC_CODE
# include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/************************************************************************************************************
 *  CanIf_InitMemory()
 ***********************************************************************************************************/
/*! \brief         Initializes global RAM variables, which have to be available before any other API of the
 *                 CAN-Interface is called.
 *  \details       Sets the CAN-Interface to the state: uninitialized.
 *  \param[in]     -
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_InitMemory(void);


/************************************************************************************************************
 *  CanIf_Init()
 ***********************************************************************************************************/
/*! \brief         Initializes the CAN-Interface.
 *  \details       -
 *  \param[in]     ConfigPtr - Pointer to the CanIf_Config structure.
 *                 If multiple configurations are available, the active configuration can be selected by using
 *                 the related CanIf_Config_<IdentityName> structure.
 *  \pre           The function CanIf_InitMemory() must be called before the function CanIf_Init() is called.
 *                 This function must be called before any other service functionality of the CAN-Interface.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_Init(P2CONST(CanIf_ConfigType, AUTOMATIC, CANIF_XCFG) ConfigPtr);


/************************************************************************************************************
 *  CanIf_SetControllerMode()
 ***********************************************************************************************************/
/*! \brief         Requests a controller mode transition of a CAN controller.
 *  \details       Supported controller modes: CANIF_CS_SLEEP, CANIF_CS_STOPPED, CANIF_CS_STARTED
 *  \param[in]     ControllerId   : Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                                  which is requested for controller mode transition.
 *  \param[in]     ControllerMode : Requested controller mode transition.
 *  \return        E_OK     : The request to change the controller mode has been accepted.
 *  \return        E_NOT_OK : The request to change the controller mode has not been accepted.
 *  \pre           Possible preconditions (e.g. interrupt lock) for calling Can_SetControllerMode can be
 *                 found in the respective CAN Driver documentation.
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *  \config        -
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode);


/************************************************************************************************************
 *  CanIf_GetControllerMode()
 ***********************************************************************************************************/
/*! \brief         Returns the current controller mode of a CAN controller.
 *  \details       -
 *  \param[in]     ControllerId      : Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                                     which is requested for its current controller mode.
 *  \param[out]    ControllerModePtr : Pointer to memory location, where the current controller mode of the
 *                                     CAN controller will be stored.
 *  \return        E_OK     : Controller mode request has been accepted; current controller mode is stored
 *                            at ControllerModePtr.
 *  \return        E_NOT_OK : Controller mode request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerMode(uint8 ControllerId, P2VAR(CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_STATE_VAR) ControllerModePtr);


/* \trace SPEC-1831, SPEC-1228, SPEC-1931, SPEC-1235 */
/************************************************************************************************************
 *  CanIf_Transmit()
 ***********************************************************************************************************/
/*! \brief         Initiates a request for transmission of the specified Tx-PDU.
 *  \details       -
 *  \param[in]     CanTxPduId : Handle of Tx-PDU which shall be transmitted.
 *  \param[in]     PduInfoPtr : Pointer to a struct containing the properties of the Tx PDU.
 *  \return        E_OK     : Transmit request has been accepted.
 *  \return        E_NOT_OK : Transmit request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE (only for a different CanTxPduId)
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_Transmit(PduIdType CanTxPduId, P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_MSG_VAR) PduInfoPtr);


/************************************************************************************************************
 *  CanIf_SetPduMode()
 ***********************************************************************************************************/
/*! \brief         Requests a PDU mode transition of a CAN controller.
 *  \details       Supported PDU modes: CANIF_SET_OFFLINE, CANIF_SET_RX_OFFLINE, CANIF_SET_RX_ONLINE,
 *                                      CANIF_SET_TX_OFFLINE, CANIF_SET_TX_ONLINE, CANIF_SET_ONLINE,
 *                                      CANIF_SET_TX_OFFLINE_ACTIVE
 *  \param[in]     ControllerId : Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                                which is requested for PDU mode transition.
 *  \param[in]     PduModeRequest : Requested PDU mode transition.
 *  \return        E_OK     : The request to change the PDU mode has been accepted.
 *  \return        E_NOT_OK : The request to change the PDU mode has not been accepted.
 *  \pre           Controller has to be in state CANIF_CS_STARTED.
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest);


/************************************************************************************************************
 *  CanIf_GetPduMode()
 ***********************************************************************************************************/
/*! \brief         Returns the current PDU mode of a CAN controller.
 *  \details       -
 *  \param[in]     ControllerId : Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                                which is requested for its current PDU mode.
 *  \param[out]    PduModePtr   : Pointer to memory location, where the current PDU mode of the
 *                                CAN controller will be stored.
 *  \return        E_OK     : PDU mode request has been accepted; current PDU mode is stored at PduModePtr.
 *  \return        E_NOT_OK : PDU mode request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetPduMode(uint8 ControllerId, P2VAR(CanIf_PduGetModeType, AUTOMATIC, CANIF_APPL_STATE_VAR) PduModePtr);


# if (CANIF_VERSION_INFO_API == STD_ON)
/************************************************************************************************************
 *  CanIf_GetVersionInfo()
 ***********************************************************************************************************/
/*! \brief         Returns the version information of the called CAN Interface module.
 *  \details       Version information (BCD-coded): vendor ID, AUTOSAR module ID, SW version of the component.
 *  \param[out]    VersionInfo: Pointer to variable Pointer to memory location, where the version information
 *                              of this module will be stored.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_VERSION_INFO_API == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANIF_APPL_VAR) VersionInfo);
# endif


# if (CANIF_TRCV_HANDLING == STD_ON)
/************************************************************************************************************
 *  CanIf_SetTrcvMode()
 ***********************************************************************************************************/
/*! \brief         Requests a transceiver mode transition of a CAN transceiver.
 *  \details       Supported transceiver modes: CANTRCV_TRCVMODE_NORMAL, CANTRCV_TRCVMODE_SLEEP,
 *                                              CANTRCV_TRCVMODE_STANDBY
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                                which is requested for mode transition.
 *  \param[in]     TransceiverMode: Requested transceiver mode transition.
 *  \return        E_OK: Transceiver mode request has been accepted.
 *                 E_NOT_OK: Transceiver mode request has not been accepted.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE for each CAN transceiver which is configured synchronous else FALSE
 *  \config        CANIF_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);


/************************************************************************************************************
 *  CanIf_GetTrcvMode()
 ***********************************************************************************************************/
/*! \brief         Returns the current transceiver mode of a CAN transceiver.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                                which is requested for current transceiver mode.
 *  \param[out]    TransceiverModePtr: Pointer to memory location, where the current transceiver mode of the
*                                      CAN transceiver will be stored.
 *  \return        E_OK: Transceiver mode request has been accepted; current transceiver mode is stored
 *                       at TransceiverModePtr.
 *  \return        E_NOT_OK: Transceiver mode request has not been accepted.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetTrcvMode(P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_STATE_VAR) TransceiverModePtr, uint8 TransceiverId);


/************************************************************************************************************
 *  CanIf_GetTrcvWakeupReason()
 ***********************************************************************************************************/
/*! \brief         Returns the wakeup reason of a CAN transceiver.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                                which is requested for wakeup reason.
 *  \param[out]    TrcvWuReasonPtr: Pointer to memory location, where the wake-up reason of the 
 *                                  CAN transceiver will be stored.
 *  \return        E_OK: Transceiver wakeup reason request has been accepted; wakeup reason is stored
 *                       at TrcvWuReasonPtr.
 *  \return        E_NOT_OK: Transceiver wakeup reason request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetTrcvWakeupReason(uint8 TransceiverId, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_STATE_VAR) TrcvWuReasonPtr);


/************************************************************************************************************
 *  CanIf_SetTrcvWakeupMode()
 ***********************************************************************************************************/
/*! \brief         Requests a transceiver wakeup mode transition of a CAN transceiver.
 *  \details       Supported wakeup modes: CANTRCV_WUMODE_ENABLE, CANTRCV_WUMODE_DISABLE, CANTRCV_WUMODE_CLEAR
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                                which is requested for wakeup mode transition.
 *  \param[in]     TrcvWakeupMode: Requested transceiver wakeup mode transition.
 *  \return        E_OK: Transceiver wakeup mode request has been accepted.
 *  \return        E_NOT_OK: Transceiver wakeup mode request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);


/************************************************************************************************************
 *  CanIf_TrcvModeIndication()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Transceiver Driver to notify CAN Interface about a successful
 *                 transceiver mode transition at the specified CAN transceiver.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId which is assigned to a CAN transceiver,
 *                                which controller mode has been transitioned.
 *  \param[in]     TransceiverMode: Transceiver mode to which the CAN transceiver transitioned.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);


#  if (CANIF_PN_TRCV_HANDLING == STD_ON)
/************************************************************************************************************
 *  CanIf_ConfirmPnAvailability()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Transceiver Driver to notify CAN Interface that the specified
 *                 CAN transceiver is running in PN communication mode.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId which is assigned to a CAN transceiver,
 *                                which has confirmed the PN availability.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_TRCV_HANDLING == STD_ON and CANIF_PN_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ConfirmPnAvailability(uint8 TransceiverId);


/************************************************************************************************************
 *  CanIf_ClearTrcvWufFlagIndication()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Transceiver Driver to notify CAN Interface that the specified
 *                 CAN transceiver has cleared the WUF flag.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId which is assigned to a CAN transceiver,
 *                                which WUF flag was cleared.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_TRCV_HANDLING == STD_ON and CANIF_PN_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId);


/************************************************************************************************************
 *  CanIf_CheckTrcvWakeFlagIndication()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Transceiver Driver to notify CAN Interface that the specified
 *                 CAN transceiver has finished the check of its wakeup events.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId which is assigned to a CAN transceiver,
 *                                which has detected a wake-up.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_TRCV_HANDLING == STD_ON and CANIF_PN_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId);


/************************************************************************************************************
 *  CanIf_ClearTrcvWufFlag()
 ***********************************************************************************************************/
/*! \brief         Requests a CAN transceiver to clear its WUF flag.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                                which is requested to clear its WUF flag.
 *  \return        E_OK: Clear WUF flag request has been accepted.
 *  \return        E_NOT_OK: Clear WUF flag request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE for each CAN transceiver which is configured synchronous else FALSE
 *  \config        CANIF_TRCV_HANDLING == STD_ON and CANIF_PN_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_ClearTrcvWufFlag(uint8 TransceiverId);


/************************************************************************************************************
 *  CanIf_CheckTrcvWakeFlag()
 ***********************************************************************************************************/
/*! \brief         Requests a CAN transceiver to check for Wake flag.
 *  \details       -
 *  \param[in]     TransceiverId: Abstracted CanIf TransceiverId, which is assigned to a CAN transceiver,
 *                                which is requested to check its Wake flag.
 *  \return        E_OK: Check Wake flag request has been accepted.
 *  \return        E_NOT_OK: Check Wake flag request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE for each CAN transceiver which is configured synchronous else FALSE
 *  \config        CANIF_TRCV_HANDLING == STD_ON and CANIF_PN_TRCV_HANDLING == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckTrcvWakeFlag(uint8 TransceiverId);
#  endif /* (CANIF_PN_TRCV_HANDLING == STD_ON) */
# endif /* (CANIF_TRCV_HANDLING == STD_ON) */


# if (CANIF_WAKEUP_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_CheckWakeup()
 ***********************************************************************************************************/
/*! \brief         Checks, whether an underlying CAN controller or CAN transceiver signals a wakeup event.
 *  \details       -
 *  \param[in]     WakeupSource: Source device, which initiated the wakeup event (CAN controller or
 *                               CAN transceiver).
 *  \return        E_OK: The specified source device signals the wakeup event.
 *  \return        E_NOT_OK: Check wakeup request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_WAKEUP_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource);
# endif


# if ((CANIF_WAKEUP_SUPPORT == STD_ON) && (CANIF_WAKEUP_VALIDATION == STD_ON))
/************************************************************************************************************
 *  CanIf_CheckValidation()
 ***********************************************************************************************************/
/*! \brief         Checks if a Rx PDU was received since the last wake-up event.
 *  \details       If a Rx PDU was received between the call of CanIf_CheckWakeup and CanIf_CheckValidation
 *                 the configurable EcuM callback function (default name "EcuM_ValidateWakeupEvent")
 *                 is called from the context of this function.
 *  \param[in]     WakeupSource: Source device which initiated the wakeup event and which has to be
 *                               validated (CAN controller or CAN transceiver).
 *  \return        E_OK: Check validation request has been accepted.
 *  \return        E_NOT_OK: Check validation request has not been accepted.
 *  \pre           CanIf_CheckWakeup has to be called before and a wakeup event has to be detected.
 *                 CAN Interface has to be set to CANIF_CS_STARTED mode before a validation is possible.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_WAKEUP_SUPPORT == STD_ON and CANIF_WAKEUP_VALIDATION == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource);
# endif


# if(CANIF_SETDYNAMICTXID_API == STD_ON)
/************************************************************************************************************
 *  CanIf_SetDynamicTxId()
 ***********************************************************************************************************/
/*! \brief         Reconfigures the CAN identifier of the specified Tx-PDU.
 *  \details       -
 *  \param[in]     CanTxPduId: Handle of Tx-PDU which CAN identifier shall be modified.
 *  \param[in]     CanId: CAN identifier which shall be set for the specified Tx-PDU.
 *  \pre           Shall not be interrupted by a call of CanIf_Transmit() for the same Tx PDU.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_SETDYNAMICTXID_API == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId);
# endif


/* NON AUTOSAR API functions */

/* Cancelation of TxPDUs */
/* Std_ReturnType CanIf_CancelTransmit(PduIdType CanTxPduId); */
# if (CANIF_CANCEL_SUPPORT_API == STD_ON)
/************************************************************************************************************
 *  CanIf_CancelTransmit()
 ***********************************************************************************************************/
/*! \brief         Initiates the cancellation / suppression of the confirmation of the specified Tx-PDU.
 *  \details       -
 *  \param[in]     CanTxPduId: Handle of Tx-PDU which shall be canceled.
 *  \return        E_OK: Transmit cancellation request has been accepted.
 *  \return        E_NOT_OK: Transmit cancellation request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_CANCEL_SUPPORT_API == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CancelTransmit(PduIdType CanTxPduId);
# endif


# if (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_GetTxConfirmationState()
 ***********************************************************************************************************/
/*! \brief         Reports if any TX confirmation has been done for the whole CAN controller since 
 *                 it's last start.
 *  \details       -
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller, 
 *                               which is requested for its Tx confirmation state.
 *  \return        CANIF_NO_NOTIFICATION:No transmit event occurred for the requested CAN controller.
 *  \return        CANIF_TX_RX_NOTIFICATION: The requested CAN controller has successfully transmitted 
 *                                           any message.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_GetTxConfirmationState(uint8 ControllerId);
# endif


# if (CANIF_CHANGE_BAUDRATE_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_CheckBaudrate()
 ***********************************************************************************************************/
/*! \brief         Checks if a CAN controller supports the specified baudrate.
 *  \details       -
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               whose baudrate support shall be checked.
 *  \param[in]     Baudrate: Baudrate to check [kbps].
 *  \return        E_OK: Baudrate is supported by the CAN controller.
 *  \return        E_NOT_OK: Baudrate is not supported / invalid by the CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_CHANGE_BAUDRATE_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckBaudrate(uint8 ControllerId, CONST(uint16, CANIF_CONST) Baudrate);


/************************************************************************************************************
 *  CanIf_ChangeBaudrate()
 ***********************************************************************************************************/
/*! \brief         Requests to change the baudrate of the specified CAN controller.
 *  \details       If possible please use the API CanIf_SetBaudrate() instead of this one.
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               whose baudrate shall be set.
 *  \param[in]     Baudrate: Requested baudrate [kbps].
 *  \return        E_OK: Change baudrate request has been accepted. Baudrate change started.
 *  \return        E_NOT_OK: Change baudrate request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_CHANGE_BAUDRATE_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_ChangeBaudrate(uint8 ControllerId, CONST(uint16, CANIF_CONST) Baudrate);
# endif


# if (CANIF_SET_BAUDRATE_API == STD_ON)
/************************************************************************************************************
 *  CanIf_SetBaudrate()
 ***********************************************************************************************************/
/*! \brief         Requests to set the baudrate configuration of the specified CAN controller.
 *  \details       -
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               whose baudrate shall be set.
 *  \param[in]     BaudRateConfigID: References a baudrate configuration by ID.
 *  \return        E_OK: Set baudrate request has been accepted. Baudrate change started.
 *  \return        E_NOT_OK: Set baudrate request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_SET_BAUDRATE_API == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID);
# endif


# if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
/************************************************************************************************************
 *  CanIf_SetAddressTableEntry()
 ***********************************************************************************************************/
/*! \brief         Sets up one relation between internal and external address.
 *  \details       Only used in J1939 environment.
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               at which a J1939 address shall be set.
 *  \param[in]     intAddr: J1939 internal address.
 *  \param[in]     busAddr: J1939 bus address.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_SetAddressTableEntry(uint8 ControllerId, uint8 intAddr, uint8 busAddr);


/************************************************************************************************************
 *  CanIf_ResetAddressTableEntry()
 ***********************************************************************************************************/
/*! \brief         Resets one relation between internal and external address.
 *  \details       Only used in J1939 environment.
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               at which a J1939 address shall be reset.
 *  \param[in]     intAddr: J1939 internal address.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ResetAddressTableEntry(uint8 ControllerId, uint8 intAddr);

/************************************************************************************************************
*  CanIf_GetTxAddressTableEntry()
***********************************************************************************************************/
/*! \brief        Returns the entry at the Tx lookup table position corresponding to the passed 
                  CAN Controller and address.
*  \details       Only used in J1939 environment.
*  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN Controller.
*  \param[in]     address: Used to retrieve table entry of Tx lookup table.
*  \return        Table entry value (input parameter "address" is returned if table entry value can not be
*                 determined).
*  \pre           -
*  \context       ANY
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \config        CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED
*  \trace         CREQ-CanIf-J1939GetAddressRelation
***********************************************************************************************************/
FUNC(uint8, CANIF_CODE) CanIf_GetTxAddressTableEntry(uint8 ControllerId, uint8 address);

/************************************************************************************************************
*  CanIf_GetRxAddressTableEntry()
***********************************************************************************************************/
/*! \brief        Returns the entry at the Rx lookup table position corresponding to the passed 
                  CAN Controller and address.
*  \details       Only used in J1939 environment.
*  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN Controller.
*  \param[in]     address: Used to retrieve table entry of Rx lookup table.
*  \param[out]    Table entry value (input parameter "address" is returned if entry value can not be
*                 determined).
*  \pre           -
*  \context       ANY
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \config        CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED
*  \trace         CREQ-CanIf-J1939GetAddressRelation
***********************************************************************************************************/
FUNC(uint8, CANIF_CODE) CanIf_GetRxAddressTableEntry(uint8 ControllerId, uint8 address);
# endif /* CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED */


# if (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_RamCheckCorruptMailbox()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Driver to notify CAN Interface that the specified mailbox is corrupt.
 *  \details       The specified corrupt mailbox will be notified to the application.
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                               with the corrupt mailbox.
 *  \param[in]     HwHandle: Handle of the corrupt mailbox.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_RamCheckCorruptMailbox(uint8 ControllerId, CanIf_HwHandleType HwHandle);

  
/************************************************************************************************************
 *  CanIf_RamCheckCorruptController()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Driver to notify CAN Interface that the specified CAN controller is corrupt.
 *  \details       The specified corrupt CAN controller will be notified to the application.
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                               which is corrupt.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_RamCheckCorruptController(uint8 ControllerId);
 
 
/************************************************************************************************************
 *  CanIf_RamCheckExecute()
 ***********************************************************************************************************/
/*! \brief         Requests the specified CAN controller to execute the RAM check of its CAN controller
 *                 HW registers.
 *  \details       -
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               for which the RAM-check shall be executed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_RamCheckExecute(uint8 ControllerId);
 
 
/************************************************************************************************************
 *  CanIf_RamCheckEnableMailbox()
 ***********************************************************************************************************/
/*! \brief         Reactivates the specified mailbox from a CAN controller after it was deactivated
 *                 by RAM check.
 *  \details       -
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               for which the mailbox shall be enabled.
 *  \param[in]     HwHandle: Handle of the mailbox, which shall be enabled.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_RamCheckEnableMailbox(uint8 ControllerId, CanIf_HwHandleType HwHandle);  
 
 
/************************************************************************************************************
 *  CanIf_RamCheckEnableController()
 ***********************************************************************************************************/
/*! \brief         Reactivates a CAN controller after it was deactivated by RAM check.
 *  \details       -
 *  \param[in]     ControllerId: Abstract CanIf ControllerId which is assigned to a CAN controller,
 *                               which shall be enabled.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_RamCheckEnableController(uint8 ControllerId);
# endif  /* (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON) */


# if (CANIF_DATA_CHECKSUM_TX_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_TransmitSubDataChecksumTxAppend()
 ***********************************************************************************************************/
/*! \brief         Is called by the CAN Interface before transmission of a data checksum Tx-PDU in order to
 *                 add a checksum to its data.
 *  \details       This API is called in context in which the consistency of all passed parameters is ensured.
 *                 Hence no further protection is required within.
 *  \param[in]     txPduInfoPtr: Pointer to Tx-PDU-parameters: CAN identifier, data length, data.
 *  \param[out]    txPduDataWithChecksumPtr: Pointer to data buffer where the data of Tx-PDU incl. the checksum
 *                                           shall be stored in. The data checksum PDU is transmitted with data
 *                                           stored in this buffer.
 *                                           Note: Parameter "txPduDataWithChecksumPtr" may only be written with
 *                                                 index >= 0 and < CANIF_CFG_MAXTXDLC_PLUS_DATACHECKSUM
 *                                                 (see file CanIf_Cfg.h). The length of data can not be changed
 *                                                 hence the checksum must only be added within valid data-length
 *                                                 of the Tx-PDU which is given by range:
 *                                                 0 - (txPduInfoPtr->length - 1).
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_DATA_CHECKSUM_TX_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_TransmitSubDataChecksumTxAppend(P2CONST(Can_PduType, AUTOMATIC, CANIF_VAR_STACK) txPduInfoPtr, P2VAR(uint8, AUTOMATIC, CANIF_VAR_STACK) txPduDataWithChecksumPtr);
# endif /* (CANIF_DATA_CHECKSUM_TX_SUPPORT == STD_ON) */


# if (CANIF_DATA_CHECKSUM_RX_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_RxIndicationSubDataChecksumRxVerify()
 ***********************************************************************************************************/
/*! \brief         Is called by the CAN Interface to verify the data checksum from a received Rx-PDU that
 *                 contains a data checksum.
 *  \details       This API is called in context in which the consistency of all passed parameters is ensured.
 *                 Hence no further protection is required within.
 *  \param[in]     CanIfRxPduId: Handle of the Rx-PDU, which checksum shall be verifed.
 *  \param[in]     CanId: CAN identifier of received Rx-PDU.
 *  \param[in]     CanDlc: Data length of received Rx-PDU.
 *  \param[in]     CanSduPtr: Pointer to data of received Rx-PDU.
 *  \return        E_OK: Verification of checksum passed. In this case the Rx-PDU is forwarded to upper layer.
 *  \return        E_NOT_OK: Verification of checksum failed. In this case the Rx-PDU is discarded and
 *                           NOT forwarded to upper layer.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_DATA_CHECKSUM_RX_SUPPORT == STD_ON
***********************************************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_RxIndicationSubDataChecksumRxVerify(PduIdType CanIfRxPduId, Can_IdType CanId, uint8 CanDlc, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR)CanSduPtr);
# endif /* (CANIF_DATA_CHECKSUM_RX_SUPPORT == STD_ON) */


# if (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_SetPduReceptionMode()
 ***********************************************************************************************************/
/*! \brief         Sets the reception mode of a Rx-PDU.
 *  \details       With this API the upper layer may influence on which PDU ID a CAN ID is received,
 *                 respective if a CAN ID is received at all. During the initialization the reception mode
 *                 of all affected Rx-PDUs is set to CANIF_RMT_RECEIVE_STOP.
 *  \param[in]     id: The handle of Rx-PDU whose reception mode shall be changed.
 *  \param[in]     mode: The reception mode which shall be set. Following reception modes are possible:
 *                 CANIF_RMT_IGNORE_CONTINUE: In case of a match the received Rx-PDU is not forwarded
 *                                            to configured upper layer and the search for a potential
 *                                            match continues.
 *                 CANIF_RMT_RECEIVE_STOP: In case of a match the received Rx-PDU is forwarded to
 *                                         configured upperlayer.
 *  \return        E_OK: Set PDU reception mode request has been accepted. PDU reception mode was changed.
 *  \return        E_NOT_OK: Set PDU reception mode request has not been accepted. PDU reception mode was 
 *                           not changed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different RX-PDU handles
 *  \synchronous   TRUE
 *  \config        CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON
***********************************************************************************************************/  
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPduReceptionMode(PduIdType id, CanIf_ReceptionModeType mode);
# endif


# if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/************************************************************************************************************
 *  CanIf_GetControllerErrorState()
 ***********************************************************************************************************/
/*! \brief         Returns the current error state of a CAN controller.
 *  \details       -
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId assigned to a CAN controller,
 *                               from which the error state is requested.
 *  \param[out]    ErrorStatePtr: Pointer to a memory location, where the error state of the CAN controller
 *                                will be stored.
 *  \return        E_OK: Error state request has been accepted; current error state is stored at ErrorStatePtr.
 *  \return        E_NOT_OK: Error state request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE only for different ControllerId
 *  \synchronous   TRUE
 *  \config        CANIF_BUS_MIRRORING_SUPPORT == STD_ON
***********************************************************************************************************/  
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerErrorState(uint8 ControllerId, P2VAR(Can_ErrorStateType, AUTOMATIC, CANIF_APPL_STATE_VAR) ErrorStatePtr);


/************************************************************************************************************
 *  CanIf_GetControllerRxErrorCounter()
 ***********************************************************************************************************/
/*! \brief         Returns the current Rx error counter of a CAN controller.
 *  \details       -
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId assigned to a CAN controller,
 *                               from which the Rx error counter is requested.
 *  \param[out]    RxErrorCounterPtr: Pointer to a memory location, where the current Rx error counter of the
 *                                    CAN controller will be stored.
 *  \return        E_OK: Rx error counter request has been accepted; current Rx error counter is 
 *                       stored at RxErrorCounterPtr.
 *  \return        E_NOT_OK: Rx error counter request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE only for different ControllerId
 *  \synchronous   TRUE
 *  \config        CANIF_BUS_MIRRORING_SUPPORT == STD_ON
***********************************************************************************************************/  
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerRxErrorCounter(uint8 ControllerId, P2VAR(uint8, AUTOMATIC, CANIF_APPL_STATE_VAR) RxErrorCounterPtr);


/************************************************************************************************************
 *  CanIf_GetControllerTxErrorCounter()
 ***********************************************************************************************************/
/*! \brief         Returns the current Tx error counter of a CAN controller.
 *  \details       -
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId assigned to a CAN controller,
 *                               from which the Tx error counter is requested.
 *  \param[out]    TxErrorCounterPtr: Pointer to a memory location, where the current Tx error counter of the
 *                                    CAN controller will be stored.
 *  \return        E_OK: Tx error counter request has been accepted; current Tx error counter is 
 *                       stored at TxErrorCounterPtr.
 *  \return        E_NOT_OK: Tx error counter request has not been accepted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE only for different ControllerId
 *  \synchronous   TRUE
 *  \config        CANIF_BUS_MIRRORING_SUPPORT == STD_ON
***********************************************************************************************************/  
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerTxErrorCounter(uint8 ControllerId, P2VAR(uint8, AUTOMATIC, CANIF_APPL_STATE_VAR) TxErrorCounterPtr);


/************************************************************************************************************
 *  CanIf_EnableBusMirroring()
 ***********************************************************************************************************/
/*! \brief         Enables or disables mirroring for a CAN controller.
 *  \details       During the initialization mirroring of all configured CAN controller is set to disabled.
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId assigned to a CAN controller,
 *                               for which mirroring shall be enabled or disabled.
 *  \param[in]     MirroringActive: TRUE: Mirroring will be enabled to report each successful received or
 *                                        transmitted CAN frame on the given CAN controller to the Mirror module.
 *                                  FALSE: Mirroring will be disabled to not report received or transmitted
 *                                         CAN frame on the given CAN controller to the Mirror module.
 *  \return        E_OK: Mirroring change request has been accepted; mirroring was changed for the 
 *                       given CAN controller.
 *  \return        E_NOT_OK: Mirroring change request has not been accepted; mirroring was not changed for the 
 *                           given CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CANIF_BUS_MIRRORING_SUPPORT == STD_ON
***********************************************************************************************************/  
FUNC(Std_ReturnType, CANIF_CODE) CanIf_EnableBusMirroring(uint8 ControllerId, boolean MirroringActive);
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */


# define CANIF_STOP_SEC_CODE
# include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
 
# define CANIF_START_SEC_CONST_8BIT
# include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

extern CONST(uint8, CANIF_CONST) CanIf_MainVersion;
extern CONST(uint8, CANIF_CONST) CanIf_SubVersion;
extern CONST(uint8, CANIF_CONST) CanIf_ReleaseVersion;

# define CANIF_STOP_SEC_CONST_8BIT
# include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */



# if (CANIF_USE_INIT_POINTER == STD_ON)

#  define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#  include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

extern P2CONST(CanIf_ConfigType, CANIF_VAR_INIT, CANIF_XCFG) CanIf_ConfigDataPtr; /* UREQ00035484 */

#  define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#  include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# endif /* CANIF_USE_INIT_POINTER == STD_ON */

#endif /* CANIF_H */

/**********************************************************************************************************************
 *  EXCLUSIVE AREAS
 **********************************************************************************************************************/
 
/*! 
* \exclusivearea CANIF_EXCLUSIVE_AREA_0
* Ensures consistency while modifying/changing the CAN-Interface controller mode.
* \protects the status variable for the CanIf controller mode.
* \usedin CanIf_SetControllerMode().
* \exclude call of: CanIf_Init, CanIf_InitMemory, CanIf_Transmit, CanIf_CancelTransmit, CanIf_SetControllerMode, CanIf_CancelTxNotification, CanIf_CancelTxConfirmation,
*                   CanIf_SetPduMode, CanIf_TxConfirmation, CanIf_ControllerBusOff.
* \length LONG Calls to Can_SetControllerMode and to several sub-functions.
* \endexclusivearea
*
* \exclusivearea CANIF_EXCLUSIVE_AREA_1
* Ensures the consistency of transmit queue.
* \protects the access to Tx-queue at all.
* \usedin CanIf_CancelTxConfirmation, CanIf_CancelTransmit, CanIf_Init, CanIf_SetControllerMode, CanIf_ControllerBusOff, CanIf_SetPduMode.
* \exclude call of CanIf_Init, CanIf_Transmit, CanIf_CancelTransmit, CanIf_SetControllerMode, CanIf_CancelTxNotification, CanIf_CancelTxConfirmation, CanIf_SetPduMode,      
*          CanIf_TxConfirmation, CanIf_ControllerBusOff.
* \length MEDIUM Calls to several sub-functions.
* \endexclusivearea
*
* \exclusivearea CANIF_EXCLUSIVE_AREA_2
* Ensures the consistency of transmit queue and mode handling.
* \protects the access to Tx-queue at all, PDU mode and controller mode.
* \usedin CanIf_TxConfirmation, CanIf_CancelTxConfirmation, CanIf_CancelTxNotification. 
* \exclude call of: CanIf_Init, CanIf_Transmit, CanIf_CancelTransmit, CanIf_SetControllerMode, CanIf_CancelTxNotification, CanIf_CancelTxConfirmation, CanIf_SetPduMode,
*                   CanIf_TxConfirmation, CanIf_ControllerBusOff.
* \length LONG Calls to Can_Write and to several sub-functions.
* \endexclusivearea
*
* \exclusivearea CANIF_EXCLUSIVE_AREA_3
* Ensures the consistency of transmit queue and mode handling.
* \protects the access to Tx-queue at all and controller mode.
* \usedin CanIf_SetPduMode.
* \exclude call of CanIf_Init, CanIf_Transmit, CanIf_CancelTransmit, CanIf_SetControllerMode, CanIf_CancelTxNotification, CanIf_CancelTxConfirmation, CanIf_SetPduMode, 
*                  CanIf_TxConfirmation, CanIf_ControllerBusOff.
* \length MEDIUM Calls to several sub-functions.
* \endexclusivearea
*
* \exclusivearea CANIF_EXCLUSIVE_AREA_4
* Ensures the consistency while transmission.
* \protects the access to Tx-queue at all.
* \usedin CanIf_Transmit.
* \exclude call of CanIf_Init, CanIf_Transmit, CanIf_CancelTransmit, CanIf_SetControllerMode, CanIf_CancelTxNotification, CanIf_CancelTxConfirmation, CanIf_SetPduMode,
*                  CanIf_TxConfirmation, CanIf_ControllerBusOff, CanIf_SetAddressTableEntry, CanIf_ResetAddressTableEntry.
* \length LONG Calls to Can_Write and to several sub-functions.
* \endexclusivearea
*
* \exclusivearea CANIF_EXCLUSIVE_AREA_5
* Ensures the consistency of the dynamic CAN identifier.
* \protects the variable with the dynamic Tx ID.
* \usedin CanIf_SetDynamicTxId.
* \exclude call of CanIf_Init, CanIf_Transmit.
* \length SHORT No calls inside.
* \endexclusivearea
*
* \exclusivearea CANIF_EXCLUSIVE_AREA_6
* Ensures the consistency of the dynamic J1939 addressing indirection.
* \protects J1939RXADDR_LOOKUP, J1939TXADDR_LOOKUP.
* \usedin CanIf_SetAddressTableEntry, CanIf_ResetAddressTableEntry.
* \exclude CanIf_Init, CanIf_Transmit, CanIf_RxIndication, CanIf_SetAddressTableEntry, CanIf_ResetAddressTableEntry.
* \length SHORT No calls inside.
* \endexclusivearea
*
* \exclusivearea CANIF_EXCLUSIVE_AREA_7
* Ensures the consistent usage of the dynamic J1939 addressing informations.
* \protects the source and destination addresses if J1939 addressing is used.
* \usedin CanIf_RxIndication.
* \exclude CanIf_Init, CanIf_SetAddressTableEntry, CanIf_ResetAddressTableEntry.
* \length SHORT No calls inside.
* \endexclusivearea
*/

/**********************************************************************************************************************
 *  END OF FILE: CanIf.h
 *********************************************************************************************************************/
