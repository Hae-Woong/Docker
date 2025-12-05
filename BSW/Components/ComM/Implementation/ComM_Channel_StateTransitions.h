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
/**         \file     ComM_Channel_StateTransitions.h
 *          \unit     Channel_StateTransitions
 *          \brief    Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the Channel_StateTransitions unit.
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

#ifndef COMM_CHANNEL_STATETRANSITIONS_H
# define COMM_CHANNEL_STATETRANSITIONS_H

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
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
  ComM_ChannelStateTransition()
 *********************************************************************************************************************/
/*! \brief       Performs a state transition on the given channel according to the target (highest requested) mode.
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \param[in]   highestComModeReq       the target state of the transition, the highest requested mode
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-5483, SPEC-5472, SPEC-5479, SPEC-5463, SPEC-5469, SPEC-5474, SPEC-5397
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ChannelStateTransition( NetworkHandleType channel, ComM_StateType highestComModeReq );

/**********************************************************************************************************************
  ComM_ExecuteTransitionFct()
 *********************************************************************************************************************/
/*! \brief       Executes the transition function for the given targetState and sourceState in order to perform the channel
 *               state transition.
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \param[in]   sourceState             the source state of the transition
 *  \param[in]   targetState             the target state of the transition
 *  \param[in]   partitionIdx            partition Id where the channel resides
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ExecuteTransitionFct( NetworkHandleType channel, ComM_StateType sourceState, ComM_StateType targetState, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_CHANNEL_STATETRANSITIONS_H */

/**********************************************************************************************************************
  END OF FILE: ComM_Channel_StateTransitions.h
**********************************************************************************************************************/
