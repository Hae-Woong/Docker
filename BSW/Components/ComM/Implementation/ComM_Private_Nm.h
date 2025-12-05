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
/*!         \file    ComM_Private_Nm.h
 *          \unit    Nm
 *          \brief   Communication Manager ASR4
 *
 *          \details  Private internal header of the Autosar Communication Manager for the Network Management unit
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

#ifndef COMM_PRIVATE_NM_H
# define COMM_PRIVATE_NM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Nm.h"
# include "ComM_Private_Cfg.h"

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  ComM_Nm_ProcessNetworkStartIndication()
 *********************************************************************************************************************/
/*! \brief       Process the network start indication
 *  \details     -
 *  \param[in]   Channel               Valid channel identifier (network handle)
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_Nm_ProcessNetworkStartIndication( NetworkHandleType  Channel );

/**********************************************************************************************************************
 *  ComM_Nm_ProcessNetworkMode()
 *********************************************************************************************************************/
/*! \brief       Process the indication that the Network Management has entered Network Mode.
 *  \details     -
 *  \param[in]   Channel               Valid channel identifier (network handle)
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_Nm_ProcessNetworkMode( NetworkHandleType  Channel );

/**********************************************************************************************************************
 *  ComM_Nm_ProcessPrepareBusSleepMode()
 *********************************************************************************************************************/
/*! \brief       Process the indication that the Network Management has entered Prepare Bus-Sleep Mode.
 *  \details     -
 *  \param[in]   Channel               Valid channel identifier (network handle)
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_Nm_ProcessPrepareBusSleepMode( NetworkHandleType  Channel );

/**********************************************************************************************************************
 *  ComM_Nm_ProcessBusSleepMode()
 *********************************************************************************************************************/
/*! \brief       Process the indication that the Network Management has entered Bus-Sleep Mode.
 *  \details     -
 *  \param[in]   Channel               Valid channel identifier (network handle)
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_Nm_ProcessBusSleepMode( NetworkHandleType  Channel );

/**********************************************************************************************************************
 *  ComM_Nm_ProcessRestartIndication()
 *********************************************************************************************************************/
/*! \brief       Process the Network restart Indication on at least one of the coordinated busses.
 *  \details     -
 *  \param[in]   Channel               Valid channel identifier (network handle)
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_Nm_ProcessRestartIndication( NetworkHandleType  Channel );
#endif

#if ( COMM_PNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  ComM_Nm_ProcessStateChangeNotification()
 *********************************************************************************************************************/
/*! \brief       Process the state change notification from Nm.
 *  \details      -
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \param[in]   NmPreviousState        Previous state of Nm
 *  \param[in]   NmCurrentState         Current state of Nm
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_Nm_ProcessStateChangeNotification( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONST( Nm_StateType, AUTOMATIC ) NmPreviousState, CONST( Nm_StateType, AUTOMATIC ) NmCurrentState );

# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  ComM_Nm_ProcessForwardSynchronizedPncShutdown ()
 *********************************************************************************************************************/
/*! \brief       Process the Indication that a synchronized PNC shutdown was requested and needs to be forwarded for the affected PNCs.
 *  \details      -
 *  \param[in]   Channel               Valid channel identifier (network handle)
 *  \param[in]   PncBitVectorPtr       Pointer to PNC bit vector
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON
 *  \trace       SPEC-17918823
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_Nm_ProcessForwardSynchronizedPncShutdown( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONSTP2CONST( uint8, AUTOMATIC, COMM_APPL_VAR ) PncBitVectorPtr );
# endif
#endif

/**********************************************************************************************************************
 * ComM_Nm_ServiceHandler()
 *********************************************************************************************************************/
/*! \brief       Handler for the Nm services.
 *  \details     Calls the respective Nm service, which is indicated by the serviceIndex.
 *  \param[in]   serviceIndex            Internal index of the NM service to be called
 *  \param[in]   channel                 Valid channel identifier (network handle)
 *  \param[in]   pncId                   Valid PNC identifier
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *  \pre         -
 *  \context     TASK|ISR1
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_Nm_ServiceHandler( uint8 serviceIndex, CONST( NetworkHandleType, AUTOMATIC ) channel, CONST( PNCHandleType, AUTOMATIC ) pncId );

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_PRVATE_NM_H */

/**********************************************************************************************************************
  END OF FILE: ComM_Private_Nm.h
 **********************************************************************************************************************/
