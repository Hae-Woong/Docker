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
/*!         \file    ComM_Util.h
 *          \unit    Util
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the Util unit
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

#ifndef COMM_UTIL_H
# define COMM_UTIL_H

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Private_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

# define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
  ComM_Channel_HasNmTypeFullOrPncChannel()
 *********************************************************************************************************************/
/*! \brief       Evaluates if a channel of Nm type Full is present or if Pnc Channel Mapping is supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmTypeFullOrPncChannel( void );

/**********************************************************************************************************************
  ComM_Channel_HasNmTypeFullAndMinFullComTime()
 *********************************************************************************************************************/
/*! \brief       Evaluates if a channel of Nm type Full is present and if MinFullComTime is enabled.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmTypeFullAndMinFullComTime( void );

/**********************************************************************************************************************
  ComM_Channel_HasNmSupportOrBusFrOrLinslave()
 *********************************************************************************************************************/
/*! \brief       Evaluates if a channel of Bus type Fr or nm variant Linslave is present, or if Nm support is enabled.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmSupportOrBusFrOrLinslave( void );

/**********************************************************************************************************************
  ComM_Channel_HasNmSupportOrBusFrOrSynchronousWakeUp()
 *********************************************************************************************************************/
/*! \brief       Evaluates if a channel of Bus type Fr is present, or if Nm support or Synchronous Wake-up is supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmSupportOrBusFrOrSynchronousWakeUp( void );

/**********************************************************************************************************************
  ComM_User_HasModeNotificationOfChannel()
 *********************************************************************************************************************/
/*! \brief       Evaluates if User Mode Notification for Channels is supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_User_HasModeNotificationOfChannel( void );

/**********************************************************************************************************************
  ComM_Channel_HasDcmIndicationAndKeepAwakeChannels()
 *********************************************************************************************************************/
/*! \brief       Evaluates if Dcm Indication and Keep Awake channels are enabled.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasDcmIndicationAndKeepAwakeChannels( void );

/**********************************************************************************************************************
  ComM_Channel_HasModeLimitationNoNoneNmTypeOrWakeupInhibition()
 *********************************************************************************************************************/
/*! \brief       Evaluates if Mode limitation and no channel of nm type None or Wake-up Inhibition is supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasModeLimitationNoNoneNmTypeOrWakeupInhibition( void );

/**********************************************************************************************************************
  ComM_Channel_HasModeLimitationAndKeepAwakeAndNoNoneNmType()
 *********************************************************************************************************************/
/*! \brief       Evaluates if Mode limitation and keep awake channels and no channel of nm type None are supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasModeLimitationAndKeepAwakeAndNoNoneNmType( void );

/**********************************************************************************************************************
  ComM_Pnc_HasCoordinatedGwTypePassiveOrActive()
 *********************************************************************************************************************/
/*! \brief       Evaluates if Pnc coordinated channels of Gw type active or passive is present.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Pnc_HasCoordinatedGwTypePassiveOrActive( void );

/**********************************************************************************************************************
  ComM_Pnc_HasGwTypePassiveOrActive()
 *********************************************************************************************************************/
/*! \brief       Evaluates if Pnc channels of Gw type active or passive is present.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Pnc_HasGwTypePassiveOrActive( void );

/**********************************************************************************************************************
  ComM_Channel_HasModeLimitationOrWakeUpInhibition()
 *********************************************************************************************************************/
 /*! \brief       Evaluates if Mode Limitation or Wake-up Inhibition is enabled.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasModeLimitationOrWakeUpInhibition( void );

/**********************************************************************************************************************
  ComM_Pnc_HasSynchronousWakeUpOrPncWakeupEnabled()
 *********************************************************************************************************************/
/*! \brief       Evaluates if Synchronous Wake-up or Pnc Wake-up is supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Pnc_HasSynchronousWakeUpOrPncWakeupEnabled( void );

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
  ComM_GetInternalPartitionIdx()
 *********************************************************************************************************************/
/*! \brief       Retrieves the current internal partition index
 *  \details     This function retrieves the current application context from the OS and translates it to the internal
 *               partition index. Returns the internal partition index related to the current application context. If
 *               no matching partition index was found, SizeOfPartitionIdentifiers is returned.
 *  \param[in]   applicationId   Current application ID from the OS
 *  \return      internal partition index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( ComM_PartitionConfigIdxOfChannelType, COMM_CODE ) ComM_GetInternalPartitionIdx( CONST( ApplicationType, AUTOMATIC ) applicationId );


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if !defined(COMM_NOUNIT_UTIL)

/**********************************************************************************************************************
 ComM_Channel_HasNmTypeFullOrPncChannel
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmTypeFullOrPncChannel( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasFullNmTypeOfChannel() || ComM_HasPncChannelMapping());
}

/**********************************************************************************************************************
  ComM_Channel_HasNmTypeFullAndMinFullComTime
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmTypeFullAndMinFullComTime( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasFullNmTypeOfChannel() && ComM_HasMinFullComTimeOfChannel());
}

/**********************************************************************************************************************
  ComM_Channel_HasNmSupportOrBusFrOrLinslave
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmSupportOrBusFrOrLinslave( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasNmSupportOfChannel() || ComM_HasBusTypeFr() || ComM_HasLinslaveNmTypeOfChannel());
}

/**********************************************************************************************************************
  ComM_Channel_HasNmSupportOrBusFrOrSynchronousWakeUp
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasNmSupportOrBusFrOrSynchronousWakeUp( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasNmSupportOfChannel() || ComM_HasBusTypeFr() || ComM_HasSynchronousWakeUp()); /* PRQA S 3415 1 */ /* MD_ComM_3415 */
}

/**********************************************************************************************************************
  ComM_Channel_HasDcmIndicationAndKeepAwakeChannels
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasDcmIndicationAndKeepAwakeChannels( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasDcmIndication() && ComM_HasKeepAwakeChannels());
}

/**********************************************************************************************************************
  ComM_Channel_HasModeLimitationOrWakeUpInhibition
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasModeLimitationOrWakeUpInhibition( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasModeLimitation() || ComM_HasWakeupInhibition());
}

/**********************************************************************************************************************
  ComM_Channel_HasModeLimitationNoNoneNmTypeOrWakeupInhibition
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasModeLimitationNoNoneNmTypeOrWakeupInhibition( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return ((ComM_HasModeLimitation() && !ComM_HasOnlyNoneNmTypeOfChannel()) || ComM_HasWakeupInhibition());
}

/**********************************************************************************************************************
  ComM_Channel_HasModeLimitationAndKeepAwakeAndNoNoneNmType
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Channel_HasModeLimitationAndKeepAwakeAndNoNoneNmType( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return ((ComM_HasModeLimitation()) && (ComM_HasKeepAwakeChannels()) && (!ComM_HasOnlyNoneNmTypeOfChannel()));
}

/**********************************************************************************************************************
  ComM_User_HasModeNotificationOfChannel
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_User_HasModeNotificationOfChannel( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasUserModeNotiFunc() && !(ComM_HasUserModeNotificationOnlyForPncUsers()));
}

/**********************************************************************************************************************
  ComM_Pnc_HasGwTypePassiveOrActive
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Pnc_HasGwTypePassiveOrActive( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasPncGwTypeActive() || ComM_HasPncGwTypePassive());
}

/**********************************************************************************************************************
  ComM_Pnc_HasCoordinatedGwTypePassiveOrActive
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Pnc_HasCoordinatedGwTypePassiveOrActive( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasCoordinatedOfPnc() && ComM_Pnc_HasGwTypePassiveOrActive());
}

/**********************************************************************************************************************
  ComM_Pnc_HasSynchronousWakeUpOrPncWakeupEnabled
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_Pnc_HasSynchronousWakeUpOrPncWakeupEnabled( void )  /* PRQA S 3219 */  /* MD_MSR_Unreachable */
{
  /* ------------------------ Implementation ------------------------------- */
  /* PRQA S 3415 1 */ /* MD_ComM_3415 */
  return (ComM_HasSynchronousWakeUp() || ComM_HasWakeupEnabledOfPnc());
}

#endif

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_UTIL_H */
