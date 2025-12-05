/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************
 *        \file   LinSM.c
 *        \brief  LIN State Manager ASR4
 *
 *      \details  Implementation of the LIN State Manager AUTOSAR Release 4.0
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

#define LINSM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "LinSM_Cbk.h"
#include "LinSM.h"
#include "BswM_LinSM.h"
#include "ComM_BusSM.h"
#include "SchM_LinSM.h"
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif
#if ( LINSM_USE_INIT_POINTER == STD_ON ) && ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
# include "EcuM_Error.h"
#endif


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Header version check.
 * \trace SPEC-17422 */
#if (  (LINSM_SW_MAJOR_VERSION != (0x07u)) || (LINSM_SW_MINOR_VERSION != (0x00u)) || (LINSM_SW_PATCH_VERSION != (0x03u)) )
# error "Vendor specific version numbers of LinSM.c and LinSM.h are inconsistent"
#endif

/* Callback header version check
 * \trace SPEC-17422 */
#if (  (LINSM_CBK_MAJOR_VERSION != (LINSM_SW_MAJOR_VERSION)) || (LINSM_CBK_MINOR_VERSION != (LINSM_SW_MINOR_VERSION)) || (LINSM_CBK_PATCH_VERSION != (LINSM_SW_PATCH_VERSION)) )
# error "Vendor specific version numbers of LinSM.c and LinSM_Cbk.h are inconsistent!"
#endif

/* LinSM Types header version check
 * \trace SPEC-17422 */
#if (  (LINSM_TYPES_MAJOR_VERSION != (LINSM_SW_MAJOR_VERSION)) || (LINSM_TYPES_MINOR_VERSION != (LINSM_SW_MINOR_VERSION)) || (LINSM_TYPES_PATCH_VERSION != (LINSM_SW_PATCH_VERSION)) )
# error "Vendor specific version numbers of LinSM.c and LinSM_Types.h are inconsistent!"
#endif


/* AUTOSAR version information check has to match definition in header file */
#if (  (LINSM_AR_RELEASE_MAJOR_VERSION != (0x04u)) || (LINSM_AR_RELEASE_MINOR_VERSION != (0x00u)) || (LINSM_AR_RELEASE_PATCH_VERSION != (0x03u)) )
# error "AUTOSAR Specification Version numbers of LinSM.c and LinSM.h are inconsistent!"
#endif


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Defines for LinSM_RequestedComMode */
#define LINSM_FULL_COM_REQUESTED          0x01u
#define LINSM_NO_COM_REQUESTED            0x02u
#define LINSM_NO_STATE_CHANGE_REQUESTED   0x03u

#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/* Defines for parameter trcvOnOff of function LinSM_SwitchTransceiver */
# define LINSM_SWITCH_TRCV_OFF            0x00u
# define LINSM_SWITCH_TRCV_ON             0x01u
#endif

/* DesiredConfirmation */
#define LINSM_NONE                        0x00u
#define LINSM_WAKEUP_PENDING              0x01u
#define LINSM_SLEEP_PENDING               0x02u

/* ESCAN00063508 */
#if !defined (LINSM_LOCAL)  /* COV_MSR_COMPATIBILITY */
# define LINSM_LOCAL static
#endif

#if !defined (LINSM_LOCAL_INLINE)  /* COV_MSR_COMPATIBILITY */
# define LINSM_LOCAL_INLINE LOCAL_INLINE
#endif

#if !defined LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG
/* For MISRA compliance LINSM_ISDEF_<param> needs to be defined in case LINSM_<param> is STD_OFF, i.e. param is not generated */
# define LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG STD_OFF
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/* Used parameter type for function LinSM_SwitchTransceiver */
typedef uint8 LinSM_SwitchTrcvOnOffType;
#endif


/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define LINSM_START_SEC_CONST_8BIT
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_MemMap */

/* LinSM version constants */
CONST(uint8, LINSM_CONST) LinSM_MainVersion      = LINSM_SW_MAJOR_VERSION;
CONST(uint8, LINSM_CONST) LinSM_SubVersion       = LINSM_SW_MINOR_VERSION;
CONST(uint8, LINSM_CONST) LinSM_ReleaseVersion   = LINSM_SW_PATCH_VERSION;

#define LINSM_STOP_SEC_CONST_8BIT
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_MemMap */


#define LINSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( LINSM_USE_INIT_POINTER == STD_ON )
P2CONST(LinSM_ConfigType, LINSM_VAR_ZERO_INIT, LINSM_INIT_DATA) LinSM_ConfigDataPtr = NULL_PTR;
#endif

#define LINSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define LINSM_START_SEC_CODE
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (   ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) && (  ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) || ( LINSM_MASTERNODE_POSSIBLE == STD_ON )  )   )
/**********************************************************************************************************************
 * LinSM_ConfirmationTimerTask()
 *********************************************************************************************************************/
/*! \brief      Handles the confirmation timer
 *  \details    In case of a confirmation timeout a development error is reported and further error handling is done.
 *  \pre        -
 *  \param[in]  channel       Local channel handle
 *  \param[in]  network       System channel handle
 *  \trace      SPEC-10447, SPEC-17343, SPEC-17370, SPEC-17393, SPEC-17415, SPEC-17418, SPEC-17440
**********************************************************************************************************************/
LINSM_LOCAL_INLINE FUNC( void, LINSM_CODE ) LinSM_ConfirmationTimerTask( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                         CONST( NetworkHandleType, AUTOMATIC ) network );
#endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON  LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON   LINSM_MASTERNODE_POSSIBLE == STD_ON */

/**********************************************************************************************************************
 * LinSM_TriggerFullCom()
 *********************************************************************************************************************/
/*! \brief      Triggers the transition to full communication mode
 *  \details    Transceiver is switched to normal mode and the full communication request is forwarded to LinIf.
 *  \pre        -
 *  \param[in]  channel         Local channel handle
 *  \param[in]  network         System channel handle
 *  \return     E_OK     - Request accepted
 *              E_NOT_OK - Wake-up is not accepted by LinIf, i.e. LinIf_Wakeup returned E_NOT_OK
 *  \trace      SPEC-10447, SPEC-17380
**********************************************************************************************************************/
LINSM_LOCAL_INLINE FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerFullCom( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                            CONST( NetworkHandleType, AUTOMATIC ) network );

/**********************************************************************************************************************
 * LinSM_TriggerGotoSleep()
 *********************************************************************************************************************/
/*! \brief      Triggers the go to sleep process
 *  \details    Forwards the go to sleep request to the LinIf.
 *  \pre        -
 *  \param[in]  channel         Local channel handle
 *  \param[in]  network         System channel handle
 *  \return     E_OK     - Request accepted
 *              E_NOT_OK - Go to sleep is not accepted by LinIf, i.e. LinIf_GotoSleep returned E_NOT_OK.
 *  \trace      SPEC-10447, SPEC-17403, SPEC-17420
**********************************************************************************************************************/
LINSM_LOCAL FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerGotoSleep( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                       CONST( NetworkHandleType, AUTOMATIC ) network );

#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
 * LinSM_SwitchTransceiver()
 *********************************************************************************************************************/
/*! \brief      Switches the transceiver to Normal or Sleep/Standby state
 *  \details    -
 *  \pre        -
 *  \param[in]  channel       Local channel handle
 *  \param[in]  trcvOnOff     LINSM_SWITCH_TRCV_OFF or LINSM_SWITCH_TRCV_ON
**********************************************************************************************************************/
LINSM_LOCAL FUNC( void, LINSM_CODE ) LinSM_SwitchTransceiver( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                              CONST( LinSM_SwitchTrcvOnOffType, AUTOMATIC ) trcvOnOff );
#endif  /* LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * LinSM_TriggerGotoSleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
LINSM_LOCAL FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerGotoSleep( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                       CONST( NetworkHandleType, AUTOMATIC ) network )
{
  /* ----- Local Variables ---------------------------------------------- */


#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
  if( LinSM_IsSleepSupportOfChannelConfig( channel ) )
# endif /* ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) */
  {

    LinSM_SetNegativeConfirmation( channel, FALSE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
    LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif /* ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) */
    LinSM_SetDesiredConfirmation( channel, LINSM_SLEEP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
    if ( ( LinSM_GetRequestedComMode( channel ) == LINSM_NO_COM_REQUESTED )
#  if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
        &&  ( !LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
#  endif /* ( LINSM_MASTER_SLAVE_MIX == STD_ON )  */
        )
    { /* Slave */
      ComM_BusSM_BusSleepMode( network );
    }
# endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */

    (void)LinIf_GotoSleep( network );

  } /* if( LinSM_IsSleepSupportOfChannelConfig( channel ) ) */
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
  else  /* LinSM_IsSleepSupportOfChannelConfig( channel ) == FALSE */
# endif /* ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) */
#endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) || ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
  {

    LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
    LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
    LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_OFF );
# endif /* ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON ) */

    ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
    BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );

  }  /* LinSM_IsSleepSupportOfChannelConfig( channel )  */
#endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) || ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) */


  return E_OK;

} /* LinSM_TriggerGotoSleep() */


/**********************************************************************************************************************
 * LinSM_TriggerFullCom()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
LINSM_LOCAL_INLINE FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerFullCom( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                            CONST( NetworkHandleType, AUTOMATIC ) network )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
  if( LinSM_GetRequestedComMode( channel ) != LINSM_FULL_COM_REQUESTED )
#endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */
  {

#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
    /* Redundant requests are avoided inside via LinIf_GetTrcvMode */
    LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_ON );
#endif

#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
    if( LinSM_IsSleepSupportOfChannelConfig( channel ) )
# endif
    {

      LinSM_SetRequestedComMode( channel, LINSM_FULL_COM_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# if  ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
      LinSM_SetWakeUpRetryCounter( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif /* ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) */

      if ( LinSM_GetComState( channel ) != LINSM_STATE_NO_COM )
      { /* Wakeup/Restart during shutdown */
        LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );
      }

# if ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON )
      if( LinSM_GetSilenceAfterWakeupTimer( channel ) == 0u )
# endif /* ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON ) */
      {
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON */
        LinSM_SetDesiredConfirmation( channel, LINSM_WAKEUP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        retVal = LinIf_Wakeup( network );
      } /*  no silence time;  LinSM_GetSilenceAfterWakeupTimer */

    }
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
    else
# endif
#endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) || ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
    {
      LinSM_SetComState( channel, LINSM_STATE_FULL_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

      ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
      BswM_LinSM_CurrentState( network, LINSM_BSWM_FULL_COM );
    }
#endif
  }  /* LinSM_GetRequestedComMode( channel ) != LINSM_FULL_COM_REQUESTED ) */


  return retVal;

} /* LinSM_TriggerFullCom() */


#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
 * LinSM_SwitchTransceiver()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
LINSM_LOCAL FUNC( void, LINSM_CODE ) LinSM_SwitchTransceiver( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                              CONST( LinSM_SwitchTrcvOnOffType, AUTOMATIC ) trcvOnOff )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  NetworkHandleType network = LinSM_GetComMChannelHandleOfChannelConfig( channel );
#else
  NetworkHandleType network = channel;
# endif

# if ( LINSM_ISDEF_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_OFF )
  if( LinSM_GetTransceiverHandlingOfChannelConfig( channel ) != LINSM_TRCV_NONE )
# endif
  {
    LinTrcv_TrcvModeType curTrcvMode;
    LinTrcv_TrcvModeType newTrcvMode;

    if( trcvOnOff == LINSM_SWITCH_TRCV_OFF )
    {
# if ( LINSM_ISDEF_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
#  if ( LINSM_EQ2_TRANSCEIVERHANDLINGOFCHANNELCONFIG == LINSM_TRCV_STANDBY )
      newTrcvMode = LINTRCV_TRCV_MODE_STANDBY;
#  else
      newTrcvMode = LINTRCV_TRCV_MODE_SLEEP;
#  endif
# else
      if( LinSM_GetTransceiverHandlingOfChannelConfig( channel ) == LINSM_TRCV_STANDBY )
      {
        newTrcvMode = LINTRCV_TRCV_MODE_STANDBY;
      }
      else /* LinSM_GetTransceiverHandlingOfChannelConfig( channel ) == LINSM_TRCV_SLEEP */
      {
        newTrcvMode = LINTRCV_TRCV_MODE_SLEEP;
      }
# endif
    }    /* SWITCH_TRCV_OFF */
    else
    {  /* SWITCH_TRCV_ON  */
      newTrcvMode = LINTRCV_TRCV_MODE_NORMAL;
    }

# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
    if( LinIf_GetTrcvMode( network, &curTrcvMode ) == E_NOT_OK ) /* SBSW_LINSM_LINIFGETTRCVMODE_CALL */
    {
      errorId = LINSM_E_GETTRCVMODE_FAILED;
    }
# else
    (void)LinIf_GetTrcvMode( network, &curTrcvMode ); /* SBSW_LINSM_LINIFGETTRCVMODE_CALL */
# endif

    if( curTrcvMode != newTrcvMode )
    {
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
      if( LinIf_SetTrcvMode( network, newTrcvMode ) == E_NOT_OK )
      {
        errorId = LINSM_E_SETTRCVMODE_FAILED;
      }
# else
      (void)LinIf_SetTrcvMode( network, newTrcvMode );
# endif
    } /* if ( curTrcvMode != newTrcvMode ) */

  } /* if( LinSM_GetTransceiverHandlingOfChannelConfig( channel ) != LINSM_TRCV_NONE ) */

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SWITCH_TRANSCEIVER, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  LINSM_DUMMY_STATEMENT_CONST( channel ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

} /* LinSM_SwitchTransceiver() */
#endif /* ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON ) */



#if (   ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) && (  ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) || ( LINSM_MASTERNODE_POSSIBLE == STD_ON )  )   )
/**********************************************************************************************************************
 * LinSM_ConfirmationTimerTask()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
LINSM_LOCAL_INLINE FUNC( void, LINSM_CODE ) LinSM_ConfirmationTimerTask( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                         CONST( NetworkHandleType, AUTOMATIC ) network )
{

  if( LinSM_GetConfirmationTimer( channel ) != 0u )
  {
    LinSM_DecConfirmationTimer( channel ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
    if( LinSM_GetConfirmationTimer( channel ) == 0u )
    {
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
      (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_MAIN_FUNCTION, LINSM_E_CONFIRMATION_TIMEOUT );
# endif /* ( LINSM_DEV_ERROR_REPORT == STD_ON ) */

# if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )

#  if ( LINSM_MASTERNODE_POSSIBLE == STD_ON )
      if( LinSM_GetDesiredConfirmation( channel ) == LINSM_NONE )
      { /* Schedule */
        /* schedule request timeout Det error * */
      }
      else
#  endif /* LINSM_MASTERNODE_POSSIBLE; MASTER PURE or MIX; not LINSM_SLAVENODE_PURE */
      if( LinSM_GetDesiredConfirmation( channel ) == LINSM_WAKEUP_PENDING )
      {
#  if ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
        if( LinSM_GetWakeUpRetryCounter( channel ) < LinSM_GetModeRequestRepetitionMax() )
        {
#   if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
          if( LinSM_GetRequestedComMode( channel ) == LINSM_FULL_COM_REQUESTED )
#   endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */
          {
            LinSM_IncWakeUpRetryCounter( channel ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

            LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

            LinSM_SetDesiredConfirmation( channel, LINSM_WAKEUP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
            (void)LinIf_Wakeup( network );
          }
        }
        else
#  endif /* ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) */
        {
          /* RequestRepetitionMax */


#  if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
          if ( LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
#  endif /* ( LINSM_MASTER_SLAVE_MIX == STD_ON )  */
          {
#  if ( LINSM_MASTERNODE_POSSIBLE == STD_ON )
          /* Wake-up timeout -> trigger go to sleep */
            LinSM_SetRequestedComMode( channel, LINSM_NO_COM_REQUESTED );  /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
            (void)LinSM_TriggerGotoSleep( channel, network );
#  endif /* LINSM_MASTERNODE_POSSIBLE; MASTER PURE or MIX; not LINSM_SLAVENODE_PURE */
          }
#  if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
          else
#  endif /* ( LINSM_MASTER_SLAVE_MIX == STD_ON )  */
          { /* slave */
#  if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
#   if ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON )
            /*lint -e{506} */
            if ( LinSM_GetSilenceAfterWakeupTimeoutOfChannelConfig( channel ) > 0u )   /* PRQA S 2741 */  /* MD_LINSM_Rule14.3 */
            {
              LinSM_SetDesiredConfirmation( channel, LINSM_NONE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
              LinSM_SetSilenceAfterWakeupTimer( channel, LinSM_GetSilenceAfterWakeupTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
            }
            else
#   endif /* ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON ) */
            if( LinSM_GetRequestedComMode( channel ) == LINSM_FULL_COM_REQUESTED ) /* PRQA S 2004 */ /* MD_MSR_EmptyClause */
            {
              LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */  /* PRQA S 2880 */  /* MD_MSR_Unreachable */
              LinSM_SetDesiredConfirmation( channel, LINSM_WAKEUP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
              (void)LinIf_Wakeup( network );
            } /* else of ( LinSM_GetSilenceAfterWakeupTimeoutOfChannelConfig( channel ) > 0 ) */
#  endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */
          } /* node type */

        } /* RetryCounter Max */
      } /* LINSM_WAKEUP_PENDING */
      else /*   LinSM_GetDesiredConfirmation == LINSM_SLEEP_PENDING, alternative schedule timeout checked above */
#  if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
        if( LinSM_GetRequestedComMode( channel ) == LINSM_NO_COM_REQUESTED )
#  endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */
        {
          /* LINSM_SLEEP_PENDING */
          LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

#  if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
          LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_OFF );
#  endif /* LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG */
          ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );
        }
#  if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
        else /* LinSM_RequestedComMode == LINSM_FULL_COM_REQUESTED */
        {
          /* LINSM_SLEEP_PENDING with FullComReq */
          /* Wakeup/Restart after sleep timeout */
          LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );
          
          LinSM_SetRequestedComMode( channel, LINSM_FULL_COM_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

          LinSM_SetDesiredConfirmation( channel, LINSM_WAKEUP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          (void)LinIf_Wakeup( network );
        }
#  endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */

# else /* LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON */
      LINSM_DUMMY_STATEMENT_CONST( network ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif /* LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON */

    } /* LinSM_GetConfirmationTimer elapsed */
  } /* LinSM_GetConfirmationTimer running */


} /* LinSM_ConfirmationTimerTask() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON */



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * LinSM_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_InitMemory( void )
{
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  LinSM_SetInitialized( FALSE ); /* SBSW_LINSM_CSL_VAR_ACCESS */
#endif
} /* LinSM_InitMemory() */


/**********************************************************************************************************************
 * LinSM_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_Init( P2CONST(LinSM_ConfigType, AUTOMATIC, LINSM_INIT_DATA) ConfigPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;
  uint8 ecumErrorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( LinSM_IsInitialized() )
  {
    errorId = LINSM_E_INIT_FAILED;
  }
  else
#endif
#if ( LINSM_USE_INIT_POINTER == STD_ON )
  if ( ConfigPtr == NULL_PTR )
  {
# if ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
    ecumErrorId = ECUM_BSWERROR_NULLPTR;
# endif
    errorId = LINSM_E_PARAM_CONFIG;
  }
  else
#endif
  {
#if ( LINSM_USE_INIT_POINTER == STD_ON )
    LinSM_ConfigDataPtr = ConfigPtr;

# if ( LINSM_FINALMAGICNUMBER == STD_ON )
    /* Magic number check needs to be done after storing the config pointer, because local config pointer is used in access macro. */
    if( LinSM_GetFinalMagicNumber() != LINSM_FINAL_MAGIC_NUMBER )
    {
      ecumErrorId = ECUM_BSWERROR_MAGICNUMBER;
      errorId = LINSM_E_PARAM_CONFIG;
    }
    else
# endif
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      NetworkHandleType channel;

      for( channel = 0u; channel < LinSM_GetSizeOfChannelConfig(); channel++ )
      {
        LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
        /* ESCAN00060620: LinSM_RequestedComMode is only used if Sleep Support is enabled. */
        LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        LinSM_SetDesiredConfirmation( channel, LINSM_NONE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# if ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
        LinSM_SetWakeUpRetryCounter( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        LinSM_SetNegativeConfirmation( channel, FALSE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif /* ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) */
#endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */

#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#endif

#if ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON )
        LinSM_SetSilenceAfterWakeupTimer( channel, 0 ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#endif /* ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON ) */

      } /* for each channel */

#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
      LinSM_SetInitialized( TRUE ); /* SBSW_LINSM_CSL_VAR_ACCESS */
#endif

    }  /* MAGIC_NUMBER */
  } /*  errorId  LINSM_E_NO_ERROR */

  /* ----- EcuM Error Hook --------------------------------------- */
#if ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
  if ( ecumErrorId != LINSM_E_NO_ERROR )
  {
    (void)EcuM_BswErrorHook( LINSM_MODULE_ID, ecumErrorId );
  }
#else
  LINSM_DUMMY_STATEMENT( ecumErrorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_INIT, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

#if ( LINSM_USE_INIT_POINTER == STD_OFF )
  LINSM_DUMMY_STATEMENT_CONST( ConfigPtr ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

} /* LinSM_Init() */


/**********************************************************************************************************************
 * LinSM_MainFunction()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_MainFunction( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    NetworkHandleType channel;

    for( channel = 0u; channel < LinSM_GetSizeOfChannelConfig(); channel++ )
    {
#if (    (   ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) && (  ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) || ( LINSM_SLAVENODE_POSSIBLE == STD_ON )  )   ) ||  ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON ) || (   ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) && (  ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) || ( LINSM_MASTERNODE_POSSIBLE == STD_ON )  )   )    )
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
      NetworkHandleType network = LinSM_GetComMChannelHandleOfChannelConfig( channel );
# else
      NetworkHandleType network = channel;
# endif /* ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) */
#endif /* ( ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) && ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) ) || ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) || ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON ) ) */

      SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_2();

#if (   ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) && (  ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) || ( LINSM_SLAVENODE_POSSIBLE == STD_ON )  )   )
      if( LinSM_IsNegativeConfirmation( channel ) )
      {
# if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
        if( LinSM_GetRequestedComMode( channel ) == LINSM_FULL_COM_REQUESTED )
# endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */
        {
          LinSM_SetNegativeConfirmation( channel, FALSE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

          LinSM_IncWakeUpRetryCounter( channel ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
          LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif

          LinSM_SetDesiredConfirmation( channel, LINSM_WAKEUP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          (void)LinIf_Wakeup( network );
        }
      }
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
      else
# endif
#endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) && ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) */
      {

#if ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON )
        if( LinSM_GetSilenceAfterWakeupTimer( channel ) != 0u )
        {
          LinSM_DecSilenceAfterWakeupTimer( channel ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          if( LinSM_GetSilenceAfterWakeupTimer( channel ) == 0u )
          {
            if( LinSM_GetRequestedComMode( channel ) == LINSM_FULL_COM_REQUESTED )
            {
# if  ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
              /* reset WakeUpRetryCounter after SilenceAfterWakeupTime  */
              LinSM_SetWakeUpRetryCounter( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif /* ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) */
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
              LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif
              LinSM_SetDesiredConfirmation( channel, LINSM_WAKEUP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
              (void)LinIf_Wakeup( network );
            } /* LINSM_FULL_COM_REQUESTED */
          } /* LinSM_GetSilenceAfterWakeupTimer elapsed */
        } /* LinSM_GetSilenceAfterWakeupTimer running */
#endif /* ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON ) */


#if (   ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) && (  ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) || ( LINSM_MASTERNODE_POSSIBLE == STD_ON )  )   )
        LinSM_ConfirmationTimerTask( channel, network );
#endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) */

      } /* no LinSM_IsNegativeConfirmation, i.e. timer handling */

      SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_2();

    } /* for( channel = 0u; channel < LinSM_GetSizeOfChannelConfig(); channel++ ) */
  } /*  errorId  LINSM_E_NO_ERROR */

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_MAIN_FUNCTION, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

} /* LinSM_MainFunction() */  /* PRQA S 6080 */ /* MD_MSR_STMIF */


#if( LINSM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
* LinSM_GetVersionInfo()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
FUNC(void, LINSM_CODE) LinSM_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, LINSM_APPL_DATA) versionInfo )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if ( versionInfo == NULL_PTR )
  {
    errorId = LINSM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    versionInfo->vendorID         = (uint16)LINSM_VENDOR_ID; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->moduleID         = (uint16)LINSM_MODULE_ID; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->sw_major_version = (uint8) LINSM_SW_MAJOR_VERSION; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->sw_minor_version = (uint8) LINSM_SW_MINOR_VERSION; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->sw_patch_version = (uint8) LINSM_SW_PATCH_VERSION; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
  }

/* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
if ( errorId != LINSM_E_NO_ERROR)
{
  (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_GET_VERSION_INFO, errorId );
}
# else
LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

} /* LinSM_GetVersionInfo() */
#endif /* LINSM_VERSION_INFO_API == STD_ON */


/**********************************************************************************************************************
* LinSM_RequestComMode
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/
FUNC(Std_ReturnType, LINSM_CODE) LinSM_RequestComMode( VAR(NetworkHandleType, AUTOMATIC) network,
    VAR(ComM_ModeType, AUTOMATIC) mode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif /* ( LINSM_DEV_ERROR_DETECT == STD_ON ) */
    /* [SPEC-17383] If COMM_SILENT_COMMUNICATION is requested the function shall return E_NOT_OK directly without action. */
    if( ( mode != COMM_NO_COMMUNICATION ) && ( mode != COMM_FULL_COMMUNICATION ) )
    {
      errorId = LINSM_E_PARAMETER;
    }
    else
    {
      /* ----- Implementation ----------------------------------------------- */
#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
      NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
      NetworkHandleType channel = network;
#endif /* ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) */

      SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_0();

      if( ( LinSM_GetComState( channel ) == mode )
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
          && ( LinSM_GetRequestedComMode( channel ) == LINSM_NO_STATE_CHANGE_REQUESTED )
#endif
        )
      {
        /* [SPEC-17425] If the requested mode is the same as the current active mode,
         *             the function shall return E_NOT_OK directly without action. */
        errorId = LINSM_E_MODE_ALREADY_ACTIVE;
      }
      else /* LinSM_GetComState( channel ) != mode || LinSM_GetRequestedComMode( channel ) != LINSM_NO_STATE_CHANGE_REQUESTED */
      {

#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
        retVal = E_OK;
#endif

        if( mode == COMM_FULL_COMMUNICATION )
        {
          /* RequestedComMode LINSM_FULL_COM_REQUESTED checked inside */
          retVal = LinSM_TriggerFullCom( channel, network );
        }
        else
        {
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
          if( LinSM_GetRequestedComMode( channel ) != LINSM_NO_COM_REQUESTED )
#endif
          {

            LinSM_SetRequestedComMode( channel, LINSM_NO_COM_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

#if ( LINSM_MASTERNODE_POSSIBLE == STD_ON )
# if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
            if ( LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
# endif /* ( LINSM_MASTER_SLAVE_MIX == STD_ON )  */
            { /* Master  */
              retVal = LinSM_TriggerGotoSleep( channel, network );
            }
#endif /* LINSM_MASTERNODE_POSSIBLE; MASTER PURE or MIX; not LINSM_SLAVENODE_PURE */
          }  /* request changed */
        }   /* NO_COM */

      }  /* E_OK */

      SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_0();
    } /* no error */

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_REQUEST_COM_MODE, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;

} /* LinSM_RequestComMode() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */


/**********************************************************************************************************************
* LinSM_GetCurrentComMode()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
*/
FUNC(Std_ReturnType, LINSM_CODE) LinSM_GetCurrentComMode( VAR(NetworkHandleType, AUTOMATIC) network,
    P2VAR(ComM_ModeType, AUTOMATIC, LINSM_APPL_DATA) mode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  retVal = E_NOT_OK;
  if( mode == NULL_PTR )
  {
    errorId = LINSM_E_PARAM_POINTER;
  }
  else if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
    /* [SPEC-17432] If active state is LINSM_UNINIT the state COMM_NO_COMMUNICATION shall be returned */
    (*mode) = COMM_NO_COMMUNICATION; /* SBSW_LINSM_MODE_PTRWRITE */
  }
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
    NetworkHandleType channel = network;
#endif

    (*mode) = LinSM_GetComState( channel ); /* SBSW_LINSM_MODE_PTRWRITE */

    retVal = E_OK;

  } /* E_OK */

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_GET_CURRENT_COM_MODE, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;

} /* LinSM_GetCurrentComMode() */


#if ( LINSM_MASTERNODE_POSSIBLE == STD_ON )
/**********************************************************************************************************************
 * LinSM_ScheduleRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINSM_CODE) LinSM_ScheduleRequest( VAR(NetworkHandleType, AUTOMATIC) network,
    VAR(LinIf_SchHandleType, AUTOMATIC) schedule )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {

#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
    NetworkHandleType channel = network;
#endif

#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
    if( schedule >= LinSM_GetScheduleTableIdRangeOfChannelPostBuildConfig( channel ) )
    {
      errorId = LINSM_E_PARAMETER;
    }
# if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
    else if ( !LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
    { /* Slave, but Schedule is Master only */
      errorId = LINSM_E_NODE_TYPE;
    }
# endif /* LINSM_MASTER_SLAVE_MIX == STD_ON    */
    else
#endif /* ( LINSM_DEV_ERROR_DETECT == STD_ON ) */
    {
      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_3();

      if( LinSM_GetComState( channel ) != LINSM_STATE_FULL_COM )
      {
        /* [SPEC-17378] If the function LinSM_ScheduleRequest is called and the state is NOT LINSM_FULL_COM,
         *             the LinSM_ScheduleRequest shall return directly with E_NOT_OK. */
        errorId = LINSM_E_NOT_IN_FULL_COM;
      }
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
      /* ESCAN00060620: LinSM_RequestedComMode is always LINSM_NO_STATE_CHANGE_REQUESTED
       *                if Sleep Support is disabled. */
      else if( LinSM_GetRequestedComMode( channel ) != LINSM_NO_STATE_CHANGE_REQUESTED )
      {
        /* [SPEC-17423] If the function LinSM_ScheduleRequest is called and another request is in process
         *            on the same network, the LinSM_ScheduleRequest shall return directly with E_NOT_OK. */
        errorId = LINSM_E_REQUEST_IN_PROCESS;
      }
#endif
      else
      {

        retVal = LinIf_ScheduleRequest( network, schedule );

        if( retVal == E_OK )
        {
#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
          LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#endif
        }
      } /* no runtime error */
      
      SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_3();

    } /* Schedule within range, no Det */
  } /* no Det */

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_REQUEST, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif


  return retVal;

} /* LinSM_ScheduleRequest() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* LINSM_MASTERNODE_POSSIBLE; MASTER PURE or MIX LINSM_SLAVENODE_PURE */


/**********************************************************************************************************************
 * LinSM_WakeupConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_WakeupConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
    VAR(boolean, AUTOMATIC) success )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
    /* ESCAN00060620: LinSM_WakeupConfirmation will only be called if Sleep Support is enabled. */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
# else
    NetworkHandleType channel = network;
# endif

    SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_1();

    /* [SPEC-17342] If LinIf module calls the confirmation callback after the timer has
     *            elapsed, no further notification shall be made to the ComM module,
     *            i.e. the confirmation is ignored. */
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
    if(    ( LinSM_GetConfirmationTimer( channel ) != 0u )
#  if ( LINSM_ISDEF_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_OFF )
        /* Confirmation Timeout will be generated with 0, only if it is not equal on all channels. */
        || ( LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) == 0u )
#  endif
      )
# else
      /* Confirmation timeout is 0 on all channels. */
# endif
    {

      if( LinSM_GetDesiredConfirmation( channel ) == LINSM_WAKEUP_PENDING ) /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
      {

        LinSM_SetDesiredConfirmation( channel, LINSM_NONE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif

        if( success == TRUE )
        {
          LinSM_SetComState( channel, LINSM_STATE_FULL_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

          ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          BswM_LinSM_CurrentState( network, LINSM_BSWM_FULL_COM );
        }
        else /* ( success FALSE ) */
        {
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
          (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_WAKEUP_CONFIRMATION, LINSM_E_WAKEUPCONF_NO_SUCCESS );
# endif

# if ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
          if( LinSM_GetWakeUpRetryCounter( channel ) < LinSM_GetModeRequestRepetitionMax() )
          {  /* Set set flag to indicate negative confirmation and to force wake-up retry on next main function call. */
            LinSM_SetNegativeConfirmation( channel, TRUE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          }
          else
# endif
          { /* RequestRepetitionMax */

#  if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
            if ( LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
#  endif /* ( LINSM_MASTER_SLAVE_MIX == STD_ON )  */
            { /* Master */
#  if ( LINSM_MASTERNODE_POSSIBLE == STD_ON )
              /* Wake-up timeout -> trigger go to sleep */
              LinSM_SetRequestedComMode( channel, LINSM_NO_COM_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
              (void)LinSM_TriggerGotoSleep( channel, network );
#  endif /* LINSM_MASTERNODE_POSSIBLE; MASTER PURE or MIX; not LINSM_SLAVENODE_PURE */
            }
#  if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
            else
#  endif /* ( LINSM_MASTER_SLAVE_MIX == STD_ON )  */
            { /* slave */
#  if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
#   if ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON )
              /*lint -e{506} */
              if ( LinSM_GetSilenceAfterWakeupTimeoutOfChannelConfig( channel ) > 0u ) /* PRQA S 2741 */  /* MD_LINSM_Rule14.3 */
              {
                LinSM_SetSilenceAfterWakeupTimer( channel, LinSM_GetSilenceAfterWakeupTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
              }
              else
#   endif /* ( LINSM_SILENCEAFTERWAKEUPTIMEOUTOFCHANNELCONFIG == STD_ON ) */
              {

                LinSM_SetNegativeConfirmation( channel, TRUE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */  /* PRQA S 2880 */  /* MD_MSR_Unreachable */

              } /* else of ( LinSM_GetSilenceAfterWakeupTimeoutOfChannelConfig( channel ) > 0 ) */
#  endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */
            } /* slave */

          } /* WakeUpRetryCounter Max */
        } /* else of ( success == TRUE ) */

      } /* LinSM_GetDesiredConfirmation( == LINSM_WAKEUP_PENDING  */
    } /* ConfirmationTimeout; Timeout actions already done in main function * */

    SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_1();

#else /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */
    LINSM_DUMMY_STATEMENT( success ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
    LINSM_DUMMY_STATEMENT( network ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */

  } /*  errorId  LINSM_E_NO_ERROR */

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_WAKEUP_CONFIRMATION, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

} /* LinSM_WakeupConfirmation() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */



#if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
/**********************************************************************************************************************
 * LinSM_GotoSleepIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/*
   */
FUNC(void, LINSM_CODE) LinSM_GotoSleepIndication( VAR(NetworkHandleType, AUTOMATIC) network )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
#  if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
#  else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
#  endif /* ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) */
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
# endif /* ( LINSM_DEV_ERROR_DETECT == STD_ON ) */
  {

    /* ----- Implementation ----------------------------------------------- */
#  if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#  else
    NetworkHandleType channel = network;
#  endif /* ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) */

    SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_4();

#  if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
    if ( !LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
#  endif /* LINSM_MASTER_SLAVE_MIX == STD_ON  */
    { /* Slave */

# if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
      if (  ( LinSM_GetComState( channel ) != LINSM_STATE_NO_COM ) || ( LinSM_GetRequestedComMode( channel ) != LINSM_NO_STATE_CHANGE_REQUESTED ) )
# endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */
      { /* else of  NO_COM state without any pending request */
        (void)LinSM_TriggerGotoSleep( channel, network );
      }

    } /* not ( LinSM_IsMasterNodeTypeOfChannelConfig( channel ) ) */

    SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_4();

  } /* errorId  LINSM_E_NO_ERROR */


  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_GOTO_SLEEP_INDICATION, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

} /* LinSM_GotoSleepIndication()  */   /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE */



/**********************************************************************************************************************
 * LinSM_GotoSleepConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_GotoSleepConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
                                                    VAR(boolean, AUTOMATIC) success )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
#  if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
#  else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
#  endif /* ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) */
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
# endif /* ( LINSM_DEV_ERROR_DETECT == STD_ON ) */
  {

# if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )

    /* ----- Implementation ----------------------------------------------- */
#  if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#  else
    NetworkHandleType channel = network;
#  endif /* ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) */


    SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_1();

    /* [SPEC-17342] If LinIf module calls the confirmation callback after the timer has
     *            elapsed, no further notification shall be made to the ComM module,
     *            i.e. the confirmation is ignored. */
#  if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
    if(    ( LinSM_GetConfirmationTimer( channel ) != 0u )
#   if ( LINSM_ISDEF_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_OFF )
        /* Confirmation Timeout will be generated with 0, only if it is not equal on all channels. */
        || ( LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) == 0u )
#   endif /* ( LINSM_ISDEF_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_OFF ) */
      )
#  else /* ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG  */
      /* Confirmation timeout is configured to 0 on all channels. */
#  endif /* ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) */
    {

      if( LinSM_GetDesiredConfirmation( channel ) == LINSM_SLEEP_PENDING ) /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
      {

        LinSM_SetDesiredConfirmation( channel, LINSM_NONE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

#  if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
        if( LinSM_GetRequestedComMode( channel ) == LINSM_NO_COM_REQUESTED )
#  endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE */
        {

#  if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
          LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#  endif /* ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) */
          LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

#  if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
          LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_OFF );
#  endif /* ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON ) */
          ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );

        }
#  if ( LINSM_SLAVENODE_POSSIBLE == STD_ON )
        else
        { /* Slave, not LinSM_IsMasterNodeTypeOfChannelConfig */
          /* LINSM_FULL_COM_REQUESTED, FullComRequested, possible due to SLAVE LinSM_GotoSleepIndication; restart */

          /* enter NoCom even in case of following restart */
          LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );

#   if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
          LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig( channel ) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#   endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON */

          LinSM_SetRequestedComMode( channel, LINSM_FULL_COM_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          LinSM_SetDesiredConfirmation( channel, LINSM_WAKEUP_PENDING ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
          (void)LinIf_Wakeup( network );

        } /* FullComRequested, SLAVE */
#  endif /* LINSM_SLAVENODE_POSSIBLE; SLAVE PURE or MIX; not LINSM_MASTERNODE_PURE  */

      }  /* ( LinSM_GetDesiredConfirmation( channel ) == LINSM_SLEEP_PENDING ) */
      /* else ignore Sleep Confirmation e.g. in case set LinDrv back to Sleep to enable WakeUp;  */

    } /* ConfirmationTimer() != 0u  */


    SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_1();
# else /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */
    LINSM_DUMMY_STATEMENT( network ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif /* ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) */
  } /* errorId  LINSM_E_NO_ERROR; */

  /* LinIf will enter sleep state in any case independently from success value */
  LINSM_DUMMY_STATEMENT( success ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_GOTO_SLEEP_CONFIRMATION, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

} /* LinSM_GotoSleepConfirmation() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */


#if ( LINSM_MASTERNODE_POSSIBLE == STD_ON )
/**********************************************************************************************************************
 * LinSM_ScheduleRequestConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_ScheduleRequestConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
    VAR(LinIf_SchHandleType, AUTOMATIC) schedule )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {
#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
    NetworkHandleType channel = network;
#endif

#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
    if( schedule >= LinSM_GetScheduleTableIdRangeOfChannelPostBuildConfig( channel ) )
    {
      errorId = LINSM_E_PARAMETER;
    }
# if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
    else if ( !LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
    { /* Slave, but Schedule is Master only */
      errorId = LINSM_E_NODE_TYPE;
    }
# endif /* LINSM_MASTER_SLAVE_MIX == STD_ON    */
    else
#endif /* ( LINSM_DEV_ERROR_DETECT == STD_ON ) */
    {

      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_1();

      if( ( LinSM_GetComState( channel ) == LINSM_STATE_FULL_COM )
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
          && ( LinSM_GetRequestedComMode( channel ) == LINSM_NO_STATE_CHANGE_REQUESTED )
#endif
        )
      {
#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#endif
      }
      else /* LinSM_RequestComMode != LINSM_NO_STATE_CHANGE_REQUESTED || LinSM_GetComState( channel ) == LINSM_STATE_NO_COM */
      {
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
        (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_REQUEST_CONFIRMATION, LINSM_E_NOT_IN_FULL_COM );
#endif
      }

      SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_1();

      BswM_LinSM_CurrentSchedule( network, schedule );
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_REQUEST_CONFIRMATION, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

} /* LinSM_ScheduleRequestConfirmation() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* LINSM_MASTERNODE_POSSIBLE; MASTER PURE or MIX; not LINSM_SLAVENODE_PURE */


#if ( LINSM_SCHEDULEENDNOTIFICATIONOFCHANNELCONFIG == STD_ON ) /* LINSM_SLAVENODE_PURE considerd in validator */
/* FEAT-690: ESCAN00080029 */
/**********************************************************************************************************************
 * LinSM_ScheduleEndNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_ScheduleEndNotification( VAR(NetworkHandleType, AUTOMATIC) network, VAR(LinIf_SchHandleType, AUTOMATIC) schedule )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
#  if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
#  else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
#  endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
# endif
  {

# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
#  if ( LINSM_ISDEF_SCHEDULETABLEIDRANGEOFCHANNELPOSTBUILDCONFIG == STD_OFF )
#   if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#   else
    NetworkHandleType channel = network;
#   endif
#  endif /* ( LINSM_ISDEF_SCHEDULETABLEIDRANGEOFCHANNELPOSTBUILDCONFIG == STD_OFF ) */

#  if ( LINSM_MASTER_SLAVE_MIX == STD_ON )
    if ( !LinSM_IsMasterNodeTypeOfChannelConfig( channel ) )
    { /* Slave, but Schedule is Master only */
      errorId = LINSM_E_NODE_TYPE;
    }
    else
#   endif /* LINSM_MASTER_SLAVE_MIX == STD_ON    */
    if( schedule >= LinSM_GetScheduleTableIdRangeOfChannelPostBuildConfig( channel ) )
    {
      errorId = LINSM_E_PARAMETER;
    }
    else
#  endif /* ( LINSM_DEV_ERROR_DETECT == STD_ON ) */
    {
      /* ----- Implementation ----------------------------------------------- */
      BswM_LinSM_ScheduleEndNotification( network, schedule );
    }

  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_END_NOTIFICATION, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

} /* LinSM_ScheduleEndNotification() */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* LINSM_SCHEDULEENDNOTIFICATIONOFCHANNELCONFIG == STD_ON */


#define LINSM_STOP_SEC_CODE
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  Verify configuration
 *********************************************************************************************************************/

#if !( defined ( LINSM_CONFIGURATION_VARIANT ) ) /*    MSR_CONFIG */
# error "LinSM configuration issue: LINSM_CONFIGURATION_VARIANT must be defined."
#else
# if !( ( LINSM_CONFIGURATION_VARIANT == LINSM_CONFIGURATION_VARIANT_PRECOMPILE ) || ( LINSM_CONFIGURATION_VARIANT == LINSM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) )
#  error "LinSM configuration issue: Only configuration variants pre-compile and post-build-loadable are supported."
# endif
#endif

#if !( ( defined ( LINSM_POSTBUILD_VARIANT_SUPPORT ) ) && ( ( LINSM_POSTBUILD_VARIANT_SUPPORT == STD_ON ) || ( LINSM_POSTBUILD_VARIANT_SUPPORT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_POSTBUILD_VARIANT_SUPPORT has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_DEV_ERROR_DETECT ) ) && ( ( LINSM_DEV_ERROR_DETECT == STD_ON ) || ( LINSM_DEV_ERROR_DETECT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_DEV_ERROR_DETECT has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_DEV_ERROR_REPORT ) ) && ( ( LINSM_DEV_ERROR_REPORT == STD_ON ) || ( LINSM_DEV_ERROR_REPORT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_DEV_ERROR_REPORT has to be defined STD_OFF or STD_ON"
#endif

#if ( ( LINSM_DEV_ERROR_REPORT == STD_ON ) && ( LINSM_DEV_ERROR_DETECT == STD_OFF ) )
# error "LinSM configuration issue: LINSM_DEV_ERROR_DETECT has to be defined STD_ON if LINSM_DEV_ERROR_REPORT == STD_ON"
#endif

#if !( ( defined ( LINSM_VERSION_INFO_API ) ) && ( ( LINSM_VERSION_INFO_API == STD_ON ) || ( LINSM_VERSION_INFO_API == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_VERSION_INFO_API has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG ) ) && ( ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON ) || ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG ) ) && ( ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) || ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_SLEEPSUPPORTOFCHANNELCONFIG has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_USE_DUMMY_STATEMENT ) ) && ( ( LINSM_USE_DUMMY_STATEMENT == STD_ON ) || ( LINSM_USE_DUMMY_STATEMENT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_USE_DUMMY_STATEMENT has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_MODEREQUESTREPETITIONMAX ) ) && ( ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) || ( LINSM_MODEREQUESTREPETITIONMAX == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_MODEREQUESTREPETITIONMAX has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_USE_INIT_POINTER ) ) && ( ( LINSM_USE_INIT_POINTER == STD_ON ) || ( LINSM_USE_INIT_POINTER == STD_OFF ) ) )
# error "LinSM configuration issue:LINSM_USE_INIT_POINTER  has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_USE_ECUM_BSW_ERROR_HOOK ) ) && ( ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON ) || (LINSM_USE_ECUM_BSW_ERROR_HOOK  == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_USE_ECUM_BSW_ERROR_HOOK has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_FINALMAGICNUMBER ) ) && ( ( LINSM_FINALMAGICNUMBER == STD_ON ) || ( LINSM_FINALMAGICNUMBER == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_FINALMAGICNUMBER has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_COMMTOLINSMCHANNEL ) ) && ( ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) || ( LINSM_COMMTOLINSMCHANNEL == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_COMMTOLINSMCHANNEL has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG ) ) && ( ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) || ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_SCHEDULEENDNOTIFICATIONOFCHANNELCONFIG ) ) && ( ( LINSM_SCHEDULEENDNOTIFICATIONOFCHANNELCONFIG == STD_ON ) || ( LINSM_SCHEDULEENDNOTIFICATIONOFCHANNELCONFIG == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_SCHEDULEENDNOTIFICATIONOFCHANNELCONFIG has to be defined STD_OFF or STD_ON"
#endif




/* Justification for module-specific MISRA deviations:
 *
 * MD_LINSM_Rule14.3
 * Reason:     Configuation value is always greater zero
 * Risk:       None.
 * Prevention: Code inspection.
 *
*/




/* SBSW_JUSTIFICATION_BEGIN


\ID SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS
\DESCRIPTION Access of ConfirmationTimer, RequestedComMode, WakeUpRetryCounter, ComState and NegativeConfirmation via
             indirection over ChannelConfig structure.
             The 'channel' variable which is used as index is always checked.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID SBSW_LINSM_VERSIONINFO_PTRWRITE
\DESCRIPTION The function LinSM_GetVersionInfo writes to the object referenced by parameter versionInfo.
\COUNTERMEASURE \S The caller must ensure that the pointer passed to the parameter versionInfo references an object of
                   type Std_VersionInfoType. [SMI-16]
                \R A check of versionInfo against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINSM_MODE_PTRWRITE
\DESCRIPTION The function LinSM_GetCurrentComMode writes to the object referenced by parameter mode.
\COUNTERMEASURE \S The caller must ensure that the pointer passed to the parameter mode references an object of
                   type ComM_ModeType. [SMI-16]
                \R A check of mode against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINSM_LINIFGETTRCVMODE_CALL
\DESCRIPTION Call to LinIf_GetTrcvMode with network handle and pointer to local stack variable curTrcvMode as parameter.
\COUNTERMEASURE \N The passed pointer references the local stack variable curTrcvMode and is therefore always valid.

\ID SBSW_LINSM_COMMMODEINDICATION_CALL
\DESCRIPTION Call to ComM_BusSM_ModeIndication function with network handle and pointer to ComState variable.
\COUNTERMEASURE \N The passed pointer references the modules internal variable ComState via ComStackLib access macro and
                   is therefore always valid.

\ID SBSW_LINSM_CSL_VAR_ACCESS
\DESCRIPTION A ComStackLib generated variable is accessed.
\COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.


SBSW_JUSTIFICATION_END */


/**********************************************************************************************************************
 *  END OF FILE: LinSM.c
 *********************************************************************************************************************/
