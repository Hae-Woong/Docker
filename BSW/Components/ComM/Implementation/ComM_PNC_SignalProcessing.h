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
/*!         \file    ComM_PNC_SignalProcessing.h
 *          \unit    PNC_SignalProcessing
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the PNC_SignalProcessing unit.
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

#ifndef COMM_PNC_SIGNALPROCESSING_H
# define COMM_PNC_SIGNALPROCESSING_H

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

#if (COMM_PNC_SUPPORT == STD_ON)

/* Macros for stored PNC signal data manipulation:*/

/* Evaluate if the bit for InternalPncId is set in the stored data for signal specified by InternalSignalId */
#define ComM_IsSignalStoredPncBitSet(InternalSignalId, InternalPncId, partitionIdx) ((ComM_GetPncSignalValues((uint8)(ComM_GetPncSignalValuesStartIdxOfPncSignal(InternalSignalId) + ComM_GetSignalByteIndexOfPncPb(InternalPncId)), partitionIdx) & ComM_GetSetMaskOfPnc(InternalPncId)) > 0u) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
  ComM_PncModeSendSignal()
 *********************************************************************************************************************/
/*! \brief       Processing of EIRA_TX transmission requests. The function is available if at least one EIRA_TX signal is present.
 *  \details     -
 *  \param[in]   PncHandle               internal PNC index to send EIRA TX signals for
 *  \param[in]   activeState             Defines the value of PNC bit in the COM signal to be sent:
 *                                       TRUE - PNC entered PNC_REQUESTED state, PNC bit = 1
 *                                       FALSE - PNC left PNC_REQUESTED state, PNC bit = 0
 *  \param[in]   SigSendType             Defines the target GW type of channels where COM signal shall be sent:
 *                                       COMM_SEND_ACTIVE_SIG_ONLY - send only on channels with GW type ACTIVE
 *                                       COMM_SEND_PASSIVE_SIG_ONLY - send only on channels with GW type PASSIVE
 *                                       COMM_SEND_ALL_SIG - send on all channels, this is to be used if PNC Gateway feature is disabled.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-19783, SPEC-19817, SPEC-19801
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_PncModeSendSignal( ComM_PncIterType PncHandle, boolean activeState, uint8 SigSendType );

/**********************************************************************************************************************
  ComM_PncClearBitInSignal()
 *********************************************************************************************************************/
/*! \brief       Sets PNC bit in the signal to 0.
 *  \details     -
 *  \param[in]   InternalSignalId        internal signal handle where to set the PNC bit
 *  \param[in]   InternalPncId           internal PNC handle
 *  \param[in]   partitionIdx            identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_PncClearBitInSignal( ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_PncSetBitInSignal()
 *********************************************************************************************************************/
/*! \brief       Sets PNC bit in the signal to 1.
 *  \details     -
 *  \param[in]   InternalSignalId        internal signal handle where to set the PNC bit
 *  \param[in]   InternalPncId           internal PNC handle
 *  \param[in]   partitionIdx            identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_PncSetBitInSignal( ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_PncIsEraReceivedOnActiveChannel()
 *********************************************************************************************************************/
/*! \brief       Determines whether a request is received via ERA on a channel with GW type ACTIVE. This is needed to
 *               calculate the target PNC state.
 *  \details     -
 *  \param[in]   pncIndex                local PNC handle
 *  \return      TRUE                    if at least one request is received via ERA on a channel with GW type ACTIVE
 *  \return      FALSE                   otherwise.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( boolean, COMM_CODE ) ComM_PncIsEraReceivedOnActiveChannel( ComM_PncIterType pncIndex );

/**********************************************************************************************************************
  ComM_PncIsEraRequestReceivedOnChannel()
 *********************************************************************************************************************/
/*! \brief       Determines if at least one PNC is requested via ERA_RX signal received on the given channel
 *  \details     -
 *  \param[in]   Channel                 local channel handle
 *  \return      TRUE                    if at least one PNC is requested via ERA_RX signal received on the given channel
 *  \return      FALSE                   otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( boolean, COMM_CODE ) ComM_PncIsEraRequestReceivedOnChannel( NetworkHandleType Channel );
#endif


# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"


#endif  /* COMM_PNC_SIGNALPROCESSING_H */

/**********************************************************************************************************************
  END OF FILE: ComM_PNC_SignalProcessing.h
**********************************************************************************************************************/
