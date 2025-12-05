/***********************************************************************************************************************
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
/*!       \file  Can_30_Core_Types.h
 *        \brief  Can_30_Core types header file
 *        \details  Contains public type definitions and constants used in component's API.
 *        \unit Can_30_Core
 **********************************************************************************************************************/
#ifndef CAN_30_CORE_TYPES_H
#define CAN_30_CORE_TYPES_H

#include "ComStack_Types.h"

/* Vendor and module identification */
#define CAN_30_CORE_VENDOR_ID (30u) /*! Vendor ID */
#define CAN_30_CORE_MODULE_ID (80u) /*! Module ID */

/* ----- Magic number ----- */
#define CAN_30_CORE_MAGIC_NUMBER 0x501Eu

/*! Mode transition requests */
#define CAN_30_COREMODESLEEP ((uint8)0x01u)
#define CAN_30_COREMODESTOPREINITFAST ((uint8)0x02u)
#define CAN_30_COREMODESTOPREINIT ((uint8)0x03u)
#define CAN_30_COREMODESTART ((uint8)0x04u)
#define CAN_30_COREMODESTARTREINIT ((uint8)0x05u)
#define CAN_30_COREMODEWAKEUPSTOPREINIT ((uint8)0x06u)
#define CAN_30_COREMODESILENT ((uint8)0x07u)
#define CAN_30_COREMODERESETBUSOFFSTART ((uint8)0x08u)
#define CAN_30_COREMODERESETBUSOFFEND ((uint8)0x09u)
#define CAN_30_COREMODENONE ((uint8)0x0Au)

/*! States of a CAN controller */
#define CAN_30_CORE_STATUS_UNINIT ((uint8)0x00u)       /*! ASR extension */
#define CAN_30_CORE_STATUS_START ((uint8)0x01u)        /*! ASR extension */
#define CAN_30_CORE_STATUS_STOP ((uint8)0x02u)         /*! STOP */
#define CAN_30_CORE_STATUS_INIT ((uint8)0x04u)         /*! Initialized */
#define CAN_30_CORE_STATUS_INCONSISTENT ((uint8)0x08u) /*! Inconsistent */
#define CAN_30_CORE_STATUS_WARNING ((uint8)0x10u)      /*! WARNING */
#define CAN_30_CORE_STATUS_PASSIVE ((uint8)0x20u)      /*! PASSIVE */
#define CAN_30_CORE_STATUS_BUSOFF ((uint8)0x40u)       /*! BUSOFF */
#define CAN_30_CORE_STATUS_SLEEP ((uint8)0x80u)        /*! SLEEP */
#define CAN_30_CORE_STATUS_MASK_NOMODE ((uint8)0x0Cu)  /*! Initialized & Inconsistent/DEACTIVATED */
/*! Results of CAN RAM check */
#define CAN_30_CORE_DEACTIVATE_CONTROLLER CAN_30_CORE_STATUS_INCONSISTENT
#define CAN_30_CORE_ACTIVATE_CONTROLLER ((uint8)0x00u)


/* ----- API service IDs ----- */
typedef enum Can_30_Core_ServiceIds_enum
{
    /*!< Service ID: Can_30_Core_Init() */
    CAN_30_CORE_SID_INIT = 0x00u,
    /*!< Service ID: Can_30_Core_MainFunction_Write() */
    CAN_30_CORE_SID_MAINFCT_WRITE = 0x01u,
    /*!< Service ID: Can_30_Core_InitController() */
    CAN_30_CORE_SID_INITCTR = 0x02u,
    /*!< Service ID: Can_30_Core_SetControllerMode() */
    CAN_30_CORE_SID_SET_CTR_MODE = 0x03u,
    /*!< Service ID: Can_30_Core_DisableControllerInterrupts() */
    CAN_30_CORE_SID_CTR_INT_DISABLE = 0x04u,
    /*!< Service ID: Can_30_Core_EnableControllerInterrupts() */
    CAN_30_CORE_SID_CTR_INT_ENABLE = 0x05u,
    /*!< Service ID: Can_30_Core_Write() */
    CAN_30_CORE_SID_WRITE = 0x06u,
    /*!< Service ID: Can_30_Core_GetVersionInfo() */
    CAN_30_CORE_SID_VERSION = 0x07u,
    /*!< Service ID: Can_30_Core_MainFunction_Read() */
    CAN_30_CORE_SID_MAINFCT_READ = 0x08u,
    /*!< Service ID: Can_30_Core_MainFunction_BusOff() */
    CAN_30_CORE_SID_MAINFCT_BO = 0x09u,
    /*!< Service ID: Can_30_Core_MainFunction_Wakeup() */
    CAN_30_CORE_SID_MAINFCT_WU = 0x0Au,
    /*!< Service ID: Can_30_Core_CheckWakeup() */
    CAN_30_CORE_SID_CHECK_WU = 0x0Bu,
    /*!< Service ID: Can_30_Core_MainFunction_Mode() */
    CAN_30_CORE_SID_MAINFCT_MODE = 0x0Cu,
    /*!< Service ID: Can_30_Core_ChangeBaudrate() */
    CAN_30_CORE_SID_CHANGE_BR = 0x0Du,
    /*!< Service ID: Can_30_Core_CheckBaudrate() */
    CAN_30_CORE_SID_CHECK_BR = 0x0Eu,
    /*!< Service ID: Can_30_Core_SetBaudrate() */
    CAN_30_CORE_SID_SET_BR = 0x0Fu,
    /*!< Service ID: unused() - Can_30_Core_DeInit() */
    CAN_30_CORE_SID_UNUSED = 0x10u,
    /*!< Service ID: Can_30_Core_GetControllerErrorState() */
    CAN_30_CORE_SID_GET_CTR_ERR_STATE = 0x11u,
    /*!< Service ID: Can_30_Core_GetControllerMode() */
    CAN_30_CORE_SID_GET_CTR_MODE = 0x12u,
    /*!< Service ID: Can_30_Core_GetStatus() */
    CAN_30_CORE_SID_GET_STATUS = 0x13u,
    /*!< Service ID: Can_30_Core_ModeTransition() */
    /* Deprecated, not used by Core anymore, Extensions should use CAN_30_CORE_SID_SET_CTR_MODE instead */
    CAN_30_CORE_SID_MODE_TRANS = CAN_30_CORE_SID_SET_CTR_MODE,
    /*!< Service ID: Can_30_Core_IsFdTxBrs() */
    CAN_30_CORE_SID_CTR_TX_BRS = 0x19u,
    /*!< Service ID: Can_30_Core_IsFdBrs() */
    CAN_30_CORE_SID_CTR_BRS = 0x1Au,
    /* ----- Initialization ----- */
    /*!< Service ID: Can_30_Core_IsControllerInitialized() */
    CAN_30_CORE_SID_IS_CTRL_INIT = 0x20u,
    /* ----- Error Handling ----- */
    /*!< Service ID: Can_30_Core_GetControllerRxErrorCounter() */
    CAN_30_CORE_SID_GET_CTR_RX_ERR_CNT = 0x30u,
    /*!< Service ID: Can_30_Core_GetControllerTxErrorCounter() */
    CAN_30_CORE_SID_GET_CTR_TX_ERR_CNT = 0x31u,
    /*!< Service ID: Can_30_Core_ErrorHandling() */
    CAN_30_CORE_SID_CTR_ERR_HANDLING = 0x32u,
    /*!< Service ID: Can_30_Core_ErrorNotification() */
    CAN_30_CORE_SID_CTR_ERR_NOTIFICATION = 0x33u,
    /* ----- Mode Handling and Requests ----- */
    /*!< Service ID: Can_30_Core_GetSilentMode() */
    CAN_30_CORE_SID_GET_MODE_SILENT = 0x5Au,
    /*!< Service ID: Can_30_Core_SetSilentMode() */
    CAN_30_CORE_SID_SET_MODE_SILENT = 0x5Bu,
    /*!< Service ID: Can_30_Core_GetMirrorMode() */
    CAN_30_CORE_SID_GET_MODE_MIRROR = 0x5Cu,
    /*!< Service ID: Can_30_Core_SetMirrorMode() */
    CAN_30_CORE_SID_SET_MODE_MIRROR = 0x5Du,
    /*!< Service ID: Can_30_Core_WakeupHandling() */
    CAN_30_CORE_SID_CTR_WAKEUP_HANDLING = 0x60u,
    /* ----- Tx Handling ----- */
    /*!< Service ID: Can_30_Core_Confirmation() */
    CAN_30_CORE_SID_TX_CONFIRMATION = 0x70u,
    /*!< Service ID: Can_30_Core_TxBufferHandlerClear() */
    CAN_30_CORE_SID_CLEAR_TX_BUFFERS = 0x72u,
    /*!< Service ID: Can_30_Core_CancelTx() */
    CAN_30_CORE_SID_CANCEL_MSG_TX = 0x73u,
    /* ----- Rx Handling ----- */
    /*!< Service ID: Can_30_Core_Indication() */
    CAN_30_CORE_SID_RX_INDICATION = 0x80u,
    /*!< Service ID: Can_30_Core_Overrun() */
    CAN_30_CORE_SID_RX_OVERRUN = 0x83u,
    /* ----- Timers Handling ----- */
    /*!< Service ID: Can_30_Core_TimerUpdate() */
    CAN_30_CORE_SID_TIMER_UPDATE = 0x84u,
    /*!< Service ID: Can_30_Core_TimerLoop() */
    CAN_30_CORE_SID_TIMER_LOOP = 0x85u,
    /* ----- ISR Handling ----- */
    /*!< Service ID: InterruptService */
    CAN_30_CORE_SID_ISR = 0x90u
} Can_30_Core_ServiceId;

/*! DET error IDs */
typedef enum Can_30_Core_ErrorIds_enum
{
    /*! used to check if no error occurred - use a value unequal to any error code */
    CAN_30_CORE_E_NO_ERROR = 0u,
    /* ----- Error codes ----- */
    /*! Error code: API service called with with invalid pointer parameter  */
    CAN_30_CORE_E_PARAM_POINTER = 0x01u,
    /*! Error code: API service called with invalid handle parameter */
    CAN_30_CORE_E_PARAM_HANDLE = 0x02u,
    /*! Error code: API service called with invalid data length parameter */
    CAN_30_CORE_E_PARAM_DATA_LENGTH = 0x03u,
    /*! Error code: API service called with invalid or unused controller indentifier */
    CAN_30_CORE_E_PARAM_CONTROLLER = 0x04u,
    /*! Error code: API service used without module initialization */
    CAN_30_CORE_E_UNINIT = 0x05u,
    /*! Error code: Invalid transition for the current mode */
    CAN_30_CORE_E_TRANSITION = 0x06u,
    /*! Error code: Parameter Baudrate has an invalid value */
    CAN_30_CORE_E_PARAM_BAUDRATE = 0x07u,
    /*! Error code: Data lost due to data overflow or message overruns */
    CAN_30_CORE_E_DATALOST = 0x08u,
    /*! Error code: Invalid configuration set selection */
    CAN_30_CORE_E_INIT_FAILED = 0x09u,
    /*! Error code: Invalid call of Can_30_Core_PutMessage(). Message will be lost */
    CAN_30_CORE_E_RXQUEUE = 0x0Au,
    /*! Error code: Timeout while waiting for hardware loops */
    CAN_30_CORE_E_TIMEOUT_DET = 0x0Bu,
    /*! Error code: Invalid/Inconsistent generated data */
    CAN_30_CORE_E_GENDATA = 0x0Cu,
    /*! Error code: Hardware issue */
    CAN_30_CORE_E_HARDWARE = 0x0Du 
} Can_30_Core_ErrorId;

/*! Pointer to the version info */
typedef P2VAR(Std_VersionInfoType, AUTOMATIC, CAN_30_CORE_VAR_NO_INIT) Can_30_Core_VersionInfoPtrType;

/*! Pointer to the error counter */
typedef P2VAR(uint8, AUTOMATIC, CAN_30_CORE_APPL_VAR) Can_30_Core_ErrorCounterPtrType;

/*! Pointer to the controller state */
typedef P2VAR(Can_ControllerStateType, AUTOMATIC, CAN_30_CORE_APPL_VAR) Can_30_Core_ControllerStatePtrType;


#endif /* CAN_30_CORE_TYPES_H */
