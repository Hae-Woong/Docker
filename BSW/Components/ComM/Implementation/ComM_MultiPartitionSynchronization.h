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
/*!         \file    ComM_MultiPartitionSynchronization.h
 *          \unit    MultiPartitionSynchronization
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the MultiPartitionSynchronization unit.
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

#ifndef COMM_MULTIPARTITIONSYNCHRONIZATION_H
# define COMM_MULTIPARTITIONSYNCHRONIZATION_H

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

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
  ComM_CheckSyncCounterAndForwardFunction()
 *********************************************************************************************************************/
/*! \brief       Checks the synchronization counters and forwards the function calls, if needed.
 *  \details     -
 *  \param[in]   syncCounterPtr            pointer to the synchronization counter on the master partition
 *  \param[in]   syncCounterCurrPartPtr    pointer to the synchronization counter on the current satellite partition
 *  \param[in]   forwardingFunction        pointer to the function to be executed
 *  \param[in]   networkHandle             local channel handle
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for the different partitions
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction_Satellite()
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_CheckSyncCounterAndForwardFunction( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) syncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) syncCounterCurrPartPtr,
                                                                        ComM_ForwardingNmServicePtrType forwardingFunction, uint8 serviceIndex, CONST( NetworkHandleType, AUTOMATIC ) networkHandle, CONST( PNCHandleType, AUTOMATIC ) pncId );

/**********************************************************************************************************************
  ComM_UpdateDataFromSatellites()
 *********************************************************************************************************************/
/*! \brief       Update and synchronize data from satellite partitions
 *  \details     Checks if an event occured on any satellite partition since the last synchronization.
 *               Update the new data from the satellite partitions and synchronize the synchronization counters.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_UpdateDataFromSatellites( void );

/**********************************************************************************************************************
  ComM_IsRequestPendingOnSatellite()
 *********************************************************************************************************************/
/*! \brief       Checks if a request for the given channel is pending on any satellite partition.
 *  \details     -
 *  \param[in]   channel                     local channel handle
 *  \param[out]  busComModeReq               calculated requested bus communication mode
 *  \param[out]  busComModeReqFrShutdown     calculated requested bus communication mode for FR
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        Called by ComM_GetState()
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_IsRequestPendingOnSatellite( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) busComModeReq
                                                                          , P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) busComModeReqFrShutdown);
#endif /* COMM_MULTIPARTITION == STD_ON */

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"


#endif  /* COMM_USER_H */

/**********************************************************************************************************************
  END OF FILE: ComM_ModeInhibition.h
**********************************************************************************************************************/
