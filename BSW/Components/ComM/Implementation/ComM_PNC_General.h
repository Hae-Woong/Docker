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
/*!         \file    ComM_PNC_General.h
 *          \unit    PNC_General
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the PNC_General unit.
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

#ifndef COMM_PNC_GENERAL_H
# define COMM_PNC_GENERAL_H

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

# if (COMM_PNC_SUPPORT == STD_ON)

/* Macros for PNC to channel routing limitation status */

/* Evaluate if the bit for InternalPncId is set in the routing limitation status byte of the given ChannelId and InternalPncId */
#define ComM_IsPncLimitation(ChannelId, InternalPncId, partitionIdx) (ComM_IsPncLimitationUsedOfChannelPb(ChannelId) && ((ComM_GetPncLimitation((uint8)(ComM_GetPncLimitationStartIdxOfChannelPb(ChannelId) + ComM_GetSignalByteIndexOfPncPb(InternalPncId)), partitionIdx) & ComM_GetSetMaskOfPnc(InternalPncId)) > 0u)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
  ComM_PncGetGwChannelRequest()
 *********************************************************************************************************************/
/*! \brief       Calculates the requested state of the PN gateway channel depending on its Gateway Type and the highest requested mode of the PNCs
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \return      The requested state of the PN gateway channel
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelRequest( NetworkHandleType channel );

/**********************************************************************************************************************
  ComM_PncGetGwChannelState()
 *********************************************************************************************************************/
/*! \brief       Calculates the target state of a PN gateway channel depending on its Gateway Type and the requested PNC mode
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \param[in]   highestPNComModeReq     the requested PNC mode
 *  \return      The target state of the PN gateway channel
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelState( NetworkHandleType channel, ComM_PncModeType highestPNComModeReq );

/**********************************************************************************************************************
  ComM_PncInFullComWakesUpTheChannel()
 *********************************************************************************************************************/
/*! \brief       Determines if at least one PNC on the channel is in PNC_FULL_COMMUNICATION mode.
 *  \details     The function shall not be used if Partial Network Coordination Limitation is enabled, it does not take
 *               the limitation status of the channel into account.
 *  \param[in]   channel                 local channel handle
 *  \param[in]   highestRequest          the current highest channel request
 *  \return      COMM_FULL_COM_READY_SLEEP if the current highest request is COMM_NO_COM_NO_PENDING_REQUEST and at least
 *               one PNC on this channel is in PNC_FULL_COMMUNICATION mode. Otherwise highestRequest (no changes).
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( ComM_StateType, COMM_CODE ) ComM_PncInFullComWakesUpTheChannel( NetworkHandleType channel, ComM_StateType highestRequest );

/**********************************************************************************************************************
  ComM_HandlePncPrepareSleepTimer()
 *********************************************************************************************************************/
/*! \brief       Decrements the PNC Prepare Sleep Timer for all PNCs and updates the bus-side request if the timer has expired.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-109081
 *  \trace       SPEC-19889
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_HandlePncPrepareSleepTimer( void );

/**********************************************************************************************************************
  ComM_PncModeArbitration()
 *********************************************************************************************************************/
/*! \brief       Determines if a PNC mapped to the channel has to perform a state transition or not and initiates
 *               a state transition if so.
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-19808
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_PncModeArbitration( NetworkHandleType channel );

/**********************************************************************************************************************
  ComM_PncModeArbitrationWithoutChannels()
 *********************************************************************************************************************/
/*! \brief       Determines if a PNC without channels has to perform a state transition or not and initiates
 *               a state transition if so.
 *  \details     -
 *  \param[in]   -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_PncModeArbitrationWithoutChannels( void );

/***********************************************************************************************************************
 *  ComM_PncStateChangeNotification()
 *********************************************************************************************************************/
/*! \brief       Notifies ComM user about a PNC main state change. If more than one PNC is assigned to a user, notify the lowest
 *               PNC state if it has changed since the last notification.
 *  \details     -
 *  \return      E_OK                    user notification is successful
 *  \return      E_NOT_OK                otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-1269
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_PncStateChangeNotification( void );

/**********************************************************************************************************************
  ComM_ActivateAllPncs()
 *********************************************************************************************************************/
/*! \brief       Activates all PNCs at least for the duration of the PNC Prepare Sleep Timer.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ActivateAllPncs( void );

/**********************************************************************************************************************
  ComM_PncNmRequestHandling()
 *********************************************************************************************************************/
/*! \brief       Calls Nm_NetworkRequest() if the affected PNC is mapped to the channel,
 *               and if routing limitation is not active on the channel.
 *  \details     -
 *  \param[in]   channel                  local channel handle
 *  \param[in]   PncHandle                local PNC handle
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_PncNmRequestHandling( NetworkHandleType channel, ComM_PncIterType PncHandle );

/**********************************************************************************************************************
 * ComM_Pnc_LimitPncToChannelRouting()
 *********************************************************************************************************************/
 /*! \brief      Service to set the status of PNC to Channel Routing Limitation
 *  \details     The function stores the limitation status for the given PNC and Channel. The status will be used in combination with
                 some other conditions (current Nm state, receiving of ERA signal) to decide whether the routing of PNC information on
                 the channel is active or not. The decision and corresponding actions are taken in the next ComM_MainFunction().
 *  \param[in]   Pnc                    Valid PNC identifier
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \param[in]   Status                 TRUE: limitation of the PNC on the Channel shall be activated
 *                                      FALSE: limitation of the PNC on the Channel shall be de-activated (default status after initialization)
 *  \return      E_OK                   parameters are correct, ComM accepted the request
 *               E_NOT_OK               parameters are incorrect, ComM discarded the request
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_Pnc_LimitPncToChannelRouting( PNCHandleType Pnc, NetworkHandleType Channel, boolean Status );
# endif


# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"


#endif  /* COMM_PNC_GENERAL_H */

/**********************************************************************************************************************
  END OF FILE: ComM_PNC_General.h
**********************************************************************************************************************/


