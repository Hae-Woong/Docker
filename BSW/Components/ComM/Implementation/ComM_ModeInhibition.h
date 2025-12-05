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
/*!         \file    ComM_ModeInhibition.h
 *          \unit    ModeInhibition
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the ModeInhibition unit.
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

#ifndef COMM_MODEINHIBITION_H
# define COMM_MODEINHIBITION_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Private_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
 /*! Bit mask to set the Prevent Wake-up status */
# define COMM_WAKEUP_INHIBITION_MASK                    (0x01u)
/*! Bit mask to clear the Prevent Wake-up status */
# define COMM_WAKEUP_INHIBITION_CLEAR_MASK              (0xFEu)
/*! Bit mask to set the Mode Limitation status */
# define COMM_MODE_LIMITATION_MASK                      (0x02u)
/*! Bit mask to clear the Mode Limitation status */
# define COMM_MODE_LIMITATION_CLEAR_MASK                (0xFDu)

/*! Bit mask to set PNC Routing Limitation status of a channel to activated (limited) */
# define COMM_PNCROUTING_LIMITED                        (0x00u)
/*! Bit mask to set PNC Routing Limitation status of a channel to de-activated (partly) because Nm is in NM_REPEAT_MESSAGE state */
# define COMM_PNCROUTING_NM_REPEAT_MESSAGE              (0x01u)
/*! Bit mask to set PNC Routing Limitation status of a channel to de-activated (default) */
# define COMM_PNCROUTING_NOT_LIMITED                    (0x02u)

/*! Bit mask to clear PNC Routing Limitation status de-activated (partly) */
# define COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR        (0xFEu)
/*! Bit mask to clear PNC Routing Limitation status de-activated */
# define COMM_PNCROUTING_NOT_LIMITED_CLEAR              (0xFDu)

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
  ComM_NoComLimitationActive()
 *********************************************************************************************************************/
/*! \brief       Determines whether Mode Limitation NO_COM is active on the channel.
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \return      TRUE                    Mode Limitation to NO_COM is active on the channel
 *  \return      FALSE                   otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( boolean, COMM_CODE ) ComM_NoComLimitationActive( NetworkHandleType channel );

/**********************************************************************************************************************
  ComM_ChannelIsLimited()
 *********************************************************************************************************************/
/*! \brief       Determines whether Mode Limitation NO_COM or Prevent Wake-up is active on the channel.
 *  \details     -
 *  \param[in]   channel                 local channel handle
 *  \return      TRUE                    Mode Limitation NO_COM or Prevent Wake-up is active on the channel
 *  \return      FALSE                   otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( boolean, COMM_CODE ) ComM_ChannelIsLimited( NetworkHandleType channel );

/**********************************************************************************************************************
  ComM_ChannelResetAfterForcingNoComm()
 *********************************************************************************************************************/
 /*! \brief       Initiates a reset after all channels entered NO_COM and at least one channel has an active Mode Limitation
  *               and Reset After Forcing NO_COM enabled.
  *  \details     -
  *  \param[in]  partitionIdx            identification of the internal partition index
  *  \pre         -
  *  \context     ANY
  *  \reentrant   TRUE
  *  \synchronous TRUE
  *  \trace       CREQ-116030
  *  \trace       SPEC-2818113
  *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ChannelResetAfterForcingNoComm( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
 * ComM_ModeInhibition_GetInhibitionStatus()
 *********************************************************************************************************************/
/*! \brief       This function process the request to get the current inhibition status of the given channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[in]   Status                  Valid pointer where the inhibition status shall be stored
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ModeInhibition_GetInhibitionStatus( NetworkHandleType Channel, P2VAR( ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_VAR ) Status );

/**********************************************************************************************************************
 * ComM_ModeInhibition_PreventWakeUp()
 *********************************************************************************************************************/
/*! \brief       Process Prevent Wake-up requests or releases for a ComM channel
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[in]   Status                 TRUE: request Wake-up Inhibition
 *                                      FALSE: release Wake-up Inhibition
 *  \return      E_OK                   Request is accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       SPEC-5113
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_ModeInhibition_PreventWakeUp( NetworkHandleType Channel, boolean Status );

/**********************************************************************************************************************
 * ComM_ModeInhibition_LimitChannelToNoComMode()
 *********************************************************************************************************************/
 /*! \brief       Process Mode Limitation requests or releases for a ComM channel
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[in]   Status                 TRUE: request Mode Limitation
 *                                      FALSE: release Mode Limitation
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               - the channel has Nm Variant NONE or
 *                                      - ECU Group Classification supports Mode Limitation
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       SPEC-5453, SPEC-5433
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_ModeInhibition_LimitChannelToNoComMode( NetworkHandleType Channel, boolean Status );

/**********************************************************************************************************************
 * ComM_ModeInhibition_LimitECUToNoComMode()
 *********************************************************************************************************************/
/*! \brief       Process Mode Limitation requests or releases for all ComM channels
 *  \details     -
 *  \param[in]   Status                 TRUE: request Mode Limitation
 *                                      FALSE: release Mode Limitation
 *  \return      E_OK                   Request is accepted
 *  \return      E_NOT_OK               Request is ignored if one of the following occurs
 *                                      - Mode Limitation to NO_COM is disabled or
 *                                      - ECU Group Classification does not support Mode Limitation or
 *                                      - The API ComM_LimitChannelToNoComMode returned E_NOT_OK for at least one channel
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_ModeInhibition_LimitECUToNoComMode( boolean Status );

/**********************************************************************************************************************
 * ComM_ModeInhibition_ReadInhibitCounter()
 *********************************************************************************************************************/
/*! \brief       Process the request to Query the inhibition counter.
 *  \details     -
 *  \param[out]  CounterValue           Valid pointer where the value of inhibition counter shall be stored
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ModeInhibition_ReadInhibitCounter( P2VAR( uint16, AUTOMATIC, COMM_APPL_VAR ) CounterValue );

/**********************************************************************************************************************
 * ComM_ModeInhibition_ResetInhibitCounter()
 *********************************************************************************************************************/
/*! \brief       Process the request to reset the inhibition counter.
 *  \details     -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-5099
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ModeInhibition_ResetInhibitCounter( void );

/**********************************************************************************************************************
 * ComM_SetECUGroupClassification()
 *********************************************************************************************************************/
/*! \brief       Sets the value of ECU group classification status during runtime. The value is stored non-volatile.
 *  \details     -
 *  \param[in]   Status                 The value of ECU group classification.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_ModeInhibition_SetECUGroupClassification( ComM_InhibitionStatusType Status );

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"


#endif  /* COMM_USER_H */

/**********************************************************************************************************************
  END OF FILE: ComM_ModeInhibition.h
**********************************************************************************************************************/
