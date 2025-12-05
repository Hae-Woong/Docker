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
/*!         \file     ComM_BusSM.h
 *          \unit     BusSM
 *          \brief    Communication Manager ASR4
 *
 *          \details  Callback API services of the Communication Manager for the Bus state manager.
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

#ifndef COMM_BUSSM_H
# define COMM_BUSSM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define COMM_BUSSM_MAJOR_VERSION    (0x17u)
# define COMM_BUSSM_MINOR_VERSION    (0x00u)
# define COMM_BUSSM_PATCH_VERSION    (0x01u)

# define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  CALLBACK FUNCTIONS PROTOTYPES (called by appropriate bus state manager)
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * ComM_BusSM_ModeIndication()
 *********************************************************************************************************************/
/*! \brief       Indication of the actual bus mode by the corresponding Bus State Manager.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[in]   ComMode                 Valid pointer where the BusSM mode is stored
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_BusSM_ModeIndication( NetworkHandleType Channel, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode );

/**********************************************************************************************************************
 *  ComM_BusSM_BusSleepMode()
 *********************************************************************************************************************/
/*! \brief       Indication of the corresponding Bus State Manager that the actual bus mode is Bus-Sleep.
 *  \details     Only applicable for LIN slave nodes.
 *  \param[in]   Channel               Valid channel identifier (network handle)
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_BusSM_BusSleepMode( NetworkHandleType  Channel );


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  ComM_InternalBusSmModeIndication()
 **********************************************************************************************************************/
/*! \brief       Request the communication mode from the simulated BusSM (i.e. for channels with COMM_BUS_TYPE_INTERNAL).
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  ComMode                 Communication mode which shall be requested
 *  \return      E_OK                    Request is accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_BUS_TYPE_INTERNAL_PRESENT == STD_ON)
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_InternalBusSmModeIndication( NetworkHandleType Channel, VAR( ComM_ModeType, AUTOMATIC ) ComMode );

/***********************************************************************************************************************
 *  ComM_InternalGetCurrentBusSmMode()
 **********************************************************************************************************************/
/*! \brief       Query the communication mode from the simulated BusSM (i.e. for channels with COMM_BUS_TYPE_INTERNAL).
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  ComMode                 Valid pointer where the communication mode shall be stored
 *  \return      E_OK                    Request is accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_BUS_TYPE_INTERNAL_PRESENT == STD_ON)
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_InternalGetCurrentBusSmMode( NetworkHandleType Channel, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode );

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_BUSSM_H */

/**********************************************************************************************************************
  END OF FILE: ComM_BusSM.h
**********************************************************************************************************************/

