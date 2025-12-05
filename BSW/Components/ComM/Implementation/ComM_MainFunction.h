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
/*!         \file    ComM_MainFunction.h
 *          \unit    MainFunction
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the MainFunction unit.
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

#ifndef COMM_MAINFUNCTION_H
# define COMM_MAINFUNCTION_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Private_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  INTERNAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 /********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 * ComM_MainFunction_ProcessCycle()
 *********************************************************************************************************************/
/*! \brief       Function to perform channel-specific state transitions and state change notifications.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-19840
 *********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_MainFunction_ProcessCycle(NetworkHandleType Channel);

#if ( COMM_MULTIPARTITION == STD_ON )
/**********************************************************************************************************************
 *  ComM_MainFunction_ProcessSatelliteCycle()
 *********************************************************************************************************************/
/*! \brief      Implements the synchronization process for master and satellite partitions.
 *  \details     Synchronizes the data and forwards corresponding call to <Bus>Nm.
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[in]   partitionIdx            partition Id where the channel resides
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      ( COMM_MULTIPARTITION == STD_ON )
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_MainFunction_ProcessSatelliteCycle( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );
#endif

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_MAINFUNCTION_H */

/**********************************************************************************************************************
  END OF FILE: ComM_MainFunction.h
**********************************************************************************************************************/

