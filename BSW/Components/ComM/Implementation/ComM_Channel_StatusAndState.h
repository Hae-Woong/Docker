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
/*!         \file     ComM_Channel_StatusAndState.h
 *          \unit     Channel_StatusAndState
 *          \brief    Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the Channel_StatusAndState unit.
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

#ifndef COMM_CHANNEL_STATUSANDSTATE_H
# define COMM_CHANNEL_STATUSANDSTATE_H

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
 * ComM_Channel_GetMinFullComModeTimerStatus()
 *********************************************************************************************************************/
/*! \brief       Process the query of the status of Min Full Com Mode Timer from the corresponding channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  Status                  Valid pointer where the timer status shall be stored
 *                                       TRUE: Min Full Com Mode Timer is running
 *                                       FALSE: otherwise
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
 *  \trace       CREQ-1306
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_Channel_GetMinFullComModeTimerStatus( NetworkHandleType Channel, P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) Status );


# if (COMM_EXTENDED_RAM_CHECK == STD_ON)
/**********************************************************************************************************************
  ComM_ChannelGetMaxStateAfterExtRamCheck()
 *********************************************************************************************************************/
/*! \brief       Provides the highest possible state according to the result of Extended RAM Check.
 *  \details     -
 *  \param[in]   channel                 local channel handler
 *  \param[in]   targetState             target communication state
 *  \return      COMM_NO_COM_NO_PENDING_REQUEST if Extended RAM Check status was CANSM_DISABLE_COMMUNICATION
 *  \return      COMM_NO_COM_REQUEST_PENDING if Extended RAM Check status was CANSM_DISABLE_COMMUNICATION_REPEAT
 *  \return      targetState otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( ComM_StateType, COMM_CODE ) ComM_ChannelGetMaxStateAfterExtRamCheck( NetworkHandleType channel, ComM_StateType targetState );
# endif

/**********************************************************************************************************************
  ComM_Channel_HandleTimers()
 *********************************************************************************************************************/
/*! \brief       Handles Minimum FULL_COM mode timer and Nm Light timer of the channel
 *  \details     -
 *  \param[in]   channel                 local channel handler
 *  \param[in]   internalRequest         TRUE an internal request via user, Dcm or coordinated PNC exists on the channel
 *                                       FALSE otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-47932, SPEC-5395, SPEC-5396
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_Channel_HandleTimers( NetworkHandleType channel, boolean internalRequest );

/*********************************************************************************************************************
  ComM_CurrentChannelRequestUpdate()
 ********************************************************************************************************************/
/*! \brief       This function updates the set of ComM users currently requesting FULL_COM for the channel.
 *  \details     The set is stored in the static variable ComM_FullComRequesters[]. Then it calls
 *               ComM_CurrentChannelRequestNotification which detects if the set has changed and notifies RTE via the
 *               optional sender-receiver interface 'ComM_CurrentChannelRequest'.
 *  \param[in]   channel                 local channel handle
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_CurrentChannelRequestUpdate( NetworkHandleType channel );


/**********************************************************************************************************************
  ComM_StateChangeNotification()
 *********************************************************************************************************************/
/*! \brief       Called if BusSM mode has changed on the channel. It notifies users about a mode change if applicable.
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-1269
 *  \trace       SPEC-5098, SPEC-19802, SPEC-5266
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_StateChangeNotification( NetworkHandleType channel );


/**********************************************************************************************************************
 * ComM_Channel_GetState()
 *********************************************************************************************************************/
/*! \brief       Queries the current communication state of the corresponding channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[out]  State                   Valid pointer where the current ComM state shall be stored
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_Channel_GetState( NetworkHandleType Channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) State );

/**********************************************************************************************************************
  ComM_ProcessPassiveWakeUp()
 *********************************************************************************************************************/
/*! \brief       Stores the states requested by bus (externally) after a Passive Wake-up occurred
 *  \details     -
 *  \param[in]   channel                 local channel handle where the wake-up event occurred
 *  \param[in]   partitionIdx            identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ProcessPassiveWakeUp( NetworkHandleType channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_CalculateHighestComMode()
 *********************************************************************************************************************/
/*! \brief       Calculates the target highest communication mode of a channel by using the 'highest win' strategy
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \param[out]  internalRequest         set to TRUE if an internal request via user, Dcm or coordinated PNC exists on the channel
 *  \return      The target highest communication mode of a channel
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-5379, SPEC-5255, SPEC-5462, SPEC-19856, SPEC-19846, SPEC-5413, SPEC-5454, SPEC-5475, SPEC-5457,
 *  \trace       SPEC-5426, SPEC-5111, SPEC-5138, SPEC-5080, SPEC-5085
 *********************************************************************************************************************/
extern FUNC( ComM_StateType, COMM_CODE ) ComM_CalculateHighestComMode( NetworkHandleType channel, boolean *internalRequest );

/**********************************************************************************************************************
  ComM_ProcessSynchronousWakeUp()
 **********************************************************************************************************************/
/*! \brief       Handles synchronous wakeup for all possible passive wakeup events.
 *  \details     Calculates the bus-site requested communication mode and ensures that no passive wakeup event is
 *               overwritten by any triggered shutdown event.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ProcessSynchronousWakeUp( void );

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"


#endif  /* COMM_CHANNEL_STATUSANDSTATE_H */

/**********************************************************************************************************************
  END OF FILE: ComM_ChannelStatusAndState.h
**********************************************************************************************************************/


