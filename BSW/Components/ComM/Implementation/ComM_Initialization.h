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
/*!         \file    ComM_Initialization.h
 *          \unit    Initialization
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the Initialization unit.
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

#ifndef COMM_INITIALIZATION_H
# define COMM_INITIALIZATION_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Private_Cfg.h"

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
 *  INTERNAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 * ComM_Initialization_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Resets the pre-initialization status of ComM
 *  \details     This function can be used to initialize the variables in *_INIT_* sections
 *               in case they are not initialized by the startup code.
 *               This function has to be called once during start-up and before ComM_Init() is called.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_Initialization_InitMemory( void );

/**********************************************************************************************************************
 * ComM_Initialization_PreInit()
 *********************************************************************************************************************/
/*! \brief       Pre-Initialization function of the ComM
 *  \details     -
 *  \pre         ComM must not be initialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_Initialization_PreInit( void );

/**********************************************************************************************************************
 * ComM_Initialization_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes all ComM variables and sets the InitStatus to TRUE.
 *  \details     -
 *  \param[in]   partitionIdx                       identification of the internal partition index
 *  \return      COMM_E_NO_ERROR                    NvM data restored successfully
 *               COMM_E_ERROR_IN_PROV_SERVICE       NvM data not restored successfully
 *  \pre         ComM must not be initialized
 *  \pre         The NVRAM Manager module has to be initialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-5254, SPEC-5139
 *********************************************************************************************************************/
extern FUNC( uint8, COMM_CODE ) ComM_Initialization_Init( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
 * ComM_Initialization_PostInit()
 *********************************************************************************************************************/
/*! \brief       Processes the Post-Initialization of the ComM
 *  \details     -
 *  \param[in]   -
 *  \pre         ComM must be initialized on each partition.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( uint8, COMM_CODE ) ComM_Initialization_PostInit( void );

/**********************************************************************************************************************
 * ComM_Initialization_DeInit()
 *********************************************************************************************************************/
/*! \brief       This function stores non-volatile values in NVRAM. Additionally it de-initializes ComM and sets the
 *               initialization status to COMM_UNINIT if all channels are in state COMM_NO_COM_NO_PENDING_REQUEST.
 *  \details     -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
extern FUNC( uint8, COMM_CODE ) ComM_Initialization_DeInit( void );

/**********************************************************************************************************************
 * ComM_Initialization_GetStatus()
 *********************************************************************************************************************/
 /*! \brief       Queries the initialization status of the module.
 *  \details     -
 *  \param[in]   Status                  Valid pointer where the current ComM state shall be stored
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *  \pre         -
  *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_Initialization_GetStatus( P2VAR( ComM_InitStatusType, AUTOMATIC, COMM_APPL_VAR ) Status );

/**********************************************************************************************************************
 * ComM_Initialization_IsPostInitialized()
 *********************************************************************************************************************/
/*! \brief       Getter for the Post-Initialized flag
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( ComM_PostInitializedType, COMM_CODE ) ComM_Initialization_IsPostInitialized( void );

/**********************************************************************************************************************
 * ComM_Initialization_IsPreInitialized()
 *********************************************************************************************************************/
/*! \brief       Getter for the Pre-Initialized flag
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
*********************************************************************************************************************/
extern FUNC( ComM_PreInitializedType, COMM_CODE ) ComM_Initialization_IsPreInitialized( void );

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_INITIALIZATION_H */

/**********************************************************************************************************************
  END OF FILE: ComM_Initialization.h
**********************************************************************************************************************/
