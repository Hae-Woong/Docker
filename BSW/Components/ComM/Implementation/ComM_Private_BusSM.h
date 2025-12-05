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
/*!         \file    ComM_Private_BusSM.h
 *          \unit    BusSM
 *          \brief   Communication Manager ASR4
 *
 *          \details  Private internal header of the Communication Manager for the Bus state manager.
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

#ifndef COMM_PRIVATE_BUSSM_H
# define COMM_PRIVATE_BUSSM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_BusSM.h"
# include "ComM_Private_Cfg.h"

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * ComM_BusSM_ProcessModeIndication()
 *********************************************************************************************************************/
/*! \brief       Indication of the actual bus mode by the corresponding Bus State Manager.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[in]   ComMode                 Valid pointer where the BusSM mode is stored
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-102897
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_BusSM_ProcessModeIndication( NetworkHandleType Channel, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode );

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
FUNC( void, COMM_CODE ) ComM_BusSM_ProcessBusSleepMode( NetworkHandleType  Channel );

# define COMM_STOP_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_PRIVATE_BUSSM_H */

/**********************************************************************************************************************
  END OF FILE: ComM_Private_BusSM.h
 **********************************************************************************************************************/
