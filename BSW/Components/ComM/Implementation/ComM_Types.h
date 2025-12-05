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
/**         \file  ComM_Types.h
 *         \brief  Communication Manager ASR4
 *
 *       \details  Implementation of the Autosar Communication Manager, type definitions
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

#ifndef COMM_TYPES_H
# define COMM_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"  /* Include standard types */
# include "ComM_GenTypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define COMM_TYPES_MAJOR_VERSION          (0x17u)
# define COMM_TYPES_MINOR_VERSION          (0x00u)
# define COMM_TYPES_PATCH_VERSION          (0x01u)

/*! Network bus types */
# define COMM_BUS_TYPE_CAN                 (0u)
# define COMM_BUS_TYPE_FR                  (1u)
# define COMM_BUS_TYPE_LIN                 (3u)
# define COMM_BUS_TYPE_ETH                 (4u)
# define COMM_BUS_TYPE_INTERNAL            (5u)

/*! ComM channel states */
# define COMM_NO_COM_NO_PENDING_REQUEST    (0u)
# define COMM_NO_COM_REQUEST_PENDING       (1u)
# define COMM_FULL_COM_NETWORK_REQUESTED   (2u)
# define COMM_FULL_COM_READY_SLEEP         (3u)
# define COMM_SILENT_COM                   (4u)
# define COMM_MAX_NUMBER_OF_STATES         (5u)

/*! ComM PNC states */
# define COMM_PNC_NO_COMMUNICATION         (0u)
# define COMM_PNC_PREPARE_SLEEP            (1u)
# define COMM_PNC_READY_SLEEP              (2u)
# define COMM_PNC_REQUESTED                (3u)
# define COMM_PNC_REQUESTED_PASSIVE        (4u) /*!< Internal state representing PNC requested via ERA, which is received on PASSIVE channels only */

/*! PNC gateway types */
# define COMM_GATEWAY_TYPE_ACTIVE          (0u)
# define COMM_GATEWAY_TYPE_PASSIVE         (1u)
# define COMM_GATEWAY_TYPE_NONE            (2u)

/*! Initialization status */
# define COMM_UNINIT                       (0u)
# define COMM_INIT                         (1u)

/* ----- Error codes ----- */
# define COMM_E_NO_ERROR                                (0x00u) /*!< used to check if no error occurred - use a value unequal to any error code */
# define COMM_E_NOT_INITED                              (0x01u) /*!< Error code: API service used without module initialization */
# define COMM_E_WRONG_PARAMETERS                        (0x02u) /*!< Error code: API service used with wrong parameters */
# define COMM_E_ERROR_IN_PROV_SERVICE                   (0x03u) /*!< Error code: Provided API services of other module returns with an error */
# define COMM_E_NOSUPPORTED_MODECHANGE                  (0x04u) /*!< Error code: ComM is trying to perform a mode change which is not supported */
# define COMM_E_DIAGNOSTIC_NOT_SUPPORTED                (0x05u) /*!< Error code: Diagnostic communication is requested or released on a channel where diagnostic is not supported */
# define COMM_E_ALREADY_INITIALIZED                     (0x06u) /*!< Error code: The service ComM_Init() is called while the module is already initialized  */
# define COMM_E_WRONG_GENERATED_DATA                    (0x07u) /*!< Error code: Check of generated data has failed */
# define COMM_E_NO_PREINIT                              (0x08u) /*!< Error code: API service is used without module pre-initialization */
# define COMM_E_INVALID_PARTITION                       (0x09u) /*!< Error code: The current partition index is out of range */
# define COMM_E_NO_POSTINIT                             (0x10u) /*!< Error code: API service is used without module post-initialization */

/* advanced return type of the communication manager */
# define COMM_E_MODE_LIMITATION                         (Std_ReturnType)(2u) /*!< Mode is limited and cannot be requested by a ComM User */
# define COMM_E_UNINIT                                  (Std_ReturnType)(5u) /*!< API called but ComM is not initialized */
# define E_MULTIPLE_PNC_ASSIGNED                        (Std_ReturnType)(3u) /*!< Multiple PNCs are assigned to the affected user */
# define E_NO_PNC_ASSIGNED                              (Std_ReturnType)(4u) /*!< No PNC is assigned to the affected user */

/*! Send PNC signals on channels with Gateway Type ACTIVE */
# define COMM_SEND_ACTIVE_SIG_ONLY                      (0x00u)
/*! Send PNC signals on channels with Gateway Type PASSIVE */
# define COMM_SEND_PASSIVE_SIG_ONLY                     (0x01u)
/*! Send PNC signals on all channels */
# define COMM_SEND_ALL_SIG                              (0x02u)

/*! Used to initialize and check the user request bit field */
# define COMM_NO_REQUESTS                               (0x00u)

/*! Default value of the state change status */
# define COMM_NO_STATE_CHANGE                           (0x10u)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 /*! Pre-Initialization status type of the ComM */
typedef boolean ComM_PreInitializedType;
 /*! Initialization status type of the ComM */
typedef uint8 ComM_InitStatusType;
/*! Bus type of a channel */
typedef uint8 ComM_BusType;
/*! State type of a channel */
typedef uint8 ComM_StateType;
/*! Mode type of a PNC */
typedef uint8 ComM_PncModeType;

#endif /* COMM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: ComM_Types.h
 *********************************************************************************************************************/
