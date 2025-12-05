/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Lin_30_Generic.c
 *        \brief  AUTOSAR LIN Driver
 *
 *      \details  Implementation of the AUTOSAR LIN Driver
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  FILE REQUIRES USER MODIFICATIONS
*  Template Scope: sections marked with "START OF USER CODE" -> "END OF USER CODE"
*  -------------------------------------------------------------------------------------------------------------------
* This file includes template code that must be completed and/or adapted during BSW integration.
* The template code is incomplete and only intended for providing a signature and an empty implementation. It is neither intended nor qualified for use in series production without applying suitable quality measures.
* The template code must be completed as described in the instructions given within this file and/or in the Technical Reference.
* The completed implementation must be tested with diligent care and must comply with all quality requirements which are necessary according to the state of the art before its use.
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

#define LIN_30_GENERIC_SOURCE
/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Lin_30_Generic.h"
#include "EcuM_Cbk.h"
#include "LinIf_Cbk.h"

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif
#if ( LIN_30_GENERIC_E_TIMEOUT_TYPE_DEM == STD_ON )
# include "Dem.h"
#endif

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if ( LIN_30_GENERIC_SW_MAJOR_VERSION != 5 )
# error "Source and Header are inconsistent (LIN_30_GENERIC_SW_MAJOR_VERSION)!"
#endif
#if ( LIN_30_GENERIC_SW_MINOR_VERSION != 0 )
# error "Source and Header are inconsistent (LIN_30_GENERIC_SW_MINOR_VERSION)!"
#endif
#if ( LIN_30_GENERIC_SW_PATCH_VERSION != 0 )
# error "Source and Header are inconsistent (LIN_30_GENERIC_SW_PATCH_VERSION)!"
#endif


/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 **********************************************************************************************************************/

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/* DET Report Handling */
#define Lin_30_Generic_Det_ReportError( ApiId, ErrorCode) (Det_ReportError(LIN_30_GENERIC_MODULE_ID, 0, (ApiId), (ErrorCode)))


/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
#define Lin_30_Generic_ChannelConfigIndexType uint8

/* Define if not already defined */
#if !defined (LIN_30_GENERIC_LOCAL)
# define LIN_30_GENERIC_LOCAL static
#endif

#if !defined (LIN_30_GENERIC_LOCAL_INLINE)
# define LIN_30_GENERIC_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
#if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
# define LIN_30_GENERIC_DEV_UNINITVALUE                              ((uint8)0x00U)
# define LIN_30_GENERIC_DEV_INITVALUE                                ((uint8)0xA5U)

# define LIN_30_GENERIC_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"

/* Variable Lin_Dev_InitDetect must be initialized after reset for working debug check */
LIN_30_GENERIC_LOCAL VAR(uint8, LIN_30_GENERIC_VAR_ZERO_INIT) Lin_30_Generic_Dev_InitDetect = LIN_30_GENERIC_DEV_UNINITVALUE;
# define LIN_30_GENERIC_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"
#endif

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/**********************************************************************************************************************
 *  FUNCTIONS
 **********************************************************************************************************************/
#define LIN_30_GENERIC_START_SEC_CODE_ISR
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************/
    /* START OF USER CODE */
    /* END OF USER CODE */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  Lin_30_Generic_Interrupt
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(void, LIN_30_GENERIC_CODE_ISR) Lin_30_Generic_Interrupt( uint8 ChannelHw )
{
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check hardware channel parameter validity against configuration */
  if ( Lin_30_Generic_GetSizeOfChannelHw() <= ChannelHw )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else if ( !Lin_30_Generic_GetChannelHwIsValidHandle( ChannelHw ) ) /* is not Valid channel */
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif /* LIN_30_GENERIC_DEV_ERROR_DETECT */
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelHwChannelConfigIndex( ChannelHw );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /*if(Lin_30_Generic_GetNodeType(ChannelConfigIdx) == LIN_30_GENERIC_MASTER_NODETYPE)
    {
           Do Master channel handling
    }
    else
    {
           Do Slave channel handlig
      LinIf_HeaderIndication( (NetworkHandleType) Lin_GetChannelIdOfChannelConfig(ChannelConfigIdx), &TmpPduPtr );
      LinIf_LinErrorIndication( (NetworkHandleType) Lin_GetChannelIdOfChannelConfig(ChannelConfigIdx), Error );
      LinIf_TxConfirmation((NetworkHandleType) Lin_GetChannelIdOfChannelConfig(ChannelConfigIdx));
      LinIf_RxIndication((NetworkHandleType) Lin_GetChannelIdOfChannelConfig(ChannelConfigIdx),(uint8*)&TmpDataBuffer);
    }

    if( Channel is in SLEEP) {
     If Wakeup Support enabled, call Ecu Check Wakeup
      if (Lin_30_Generic_GetWakeupSupport(ChannelConfigIdx) == TRUE) {
      EcuM_CheckWakeup(Lin_vi_ai_GetEcuMWakeupSourceOfChannelConfig(Lin_30_Generic_GetEcuMWakeupSource(ChannelConfigIdx)));
      }
    }*/

    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_INTERRUPT_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif /* LIN_30_GENERIC_DEV_ERROR_REPORT */
}

#define LIN_30_GENERIC_STOP_SEC_CODE_ISR
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define LIN_30_GENERIC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Lin_30_Generic_SendFrame
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(Std_ReturnType, LIN_30_GENERIC_CODE) Lin_30_Generic_SendFrame( uint8 Channel,
                                              P2VAR(Lin_PduType, AUTOMATIC, LIN_30_GENERIC_APPL_VAR) PduInfoPtr )
{
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect != LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_UNINIT;
  }
  /* Check if this is a master channel */
  else if ( Lin_30_Generic_GetNodeType( Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel ) ) == LIN_30_GENERIC_SLAVE_NODETYPE )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check PduInfoPtr parameter for NULL pointer */
  else if ( (void*)PduInfoPtr == NULL_PTR )
  {
    ErrorId = LIN_30_GENERIC_E_PARAM_POINTER;
  }
  /* Check if data length parameter is in range (1-8 data bytes) */
  else if( (PduInfoPtr->Dl == 0) || (PduInfoPtr->Dl > 8)  )
  {
    ErrorId = LIN_30_GENERIC_E_PARAM_VALUE;
  }
  /* Check Channel parameter validity against configuration */
  else if ( Lin_30_Generic_GetSizeOfChannelID() <= Channel )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check validity of channel handle */
  else if ( !Lin_30_Generic_GetChannelIDIsValidHandle( Channel ) )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /*RetTmp = E_OK;*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }
#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_SENDFRAME_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif

  return(RetTmp);
}


/***********************************************************************************************************************
 *  Lin_30_Generic_GetStatus
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(Lin_StatusType, LIN_30_GENERIC_CODE) Lin_30_Generic_GetStatus( uint8 Channel,
                                     P2VAR(Lin_u8PtrType, AUTOMATIC, LIN_30_GENERIC_APPL_VAR) SduPtr )
{
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(Lin_StatusType, AUTOMATIC) RetTmp = LIN_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect != LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_UNINIT;
  }
  /* Check if this is a master channel */
  else if ( Lin_30_Generic_GetNodeType( Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel ) ) == LIN_30_GENERIC_SLAVE_NODETYPE )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check SduPtr parameter for NULL pointer */
  else if ( (void*)SduPtr == NULL_PTR )
  {
    ErrorId = LIN_30_GENERIC_E_PARAM_POINTER;
  }
  /* Check Channel parameter validity against configuration */
  else if ( Lin_30_Generic_GetSizeOfChannelID() <= Channel )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check validity of channel handle */
  else if ( !Lin_30_Generic_GetChannelIDIsValidHandle( Channel ) )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /*RetTmp = E_OK;*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_GETSTATUS_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif

  return(RetTmp);
}


/***********************************************************************************************************************
 *  Lin_30_Generic_CheckWakeup
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(Std_ReturnType, LIN_30_GENERIC_CODE) Lin_30_Generic_CheckWakeup( uint8 Channel )
{
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect != LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_UNINIT;
  }
  /* Check Channel parameter validity against configuration */
  else if ( Lin_30_Generic_GetSizeOfChannelID() <= Channel )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check validity of channel handle */
  else if ( !Lin_30_Generic_GetChannelIDIsValidHandle( Channel ) ) /* PRQA S 3325 */ /* MD_MSR_14.1 */
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /* Inform EcuM and LinIf about wakeup
    EcuM_SetWakeupEvent( Lin_30_Generic_GetEcuMWakeupSource( ChannelConfigIdx ) );
    LinIf_WakeupConfirmation( Lin_30_Generic_GetEcuMWakeupSource( ChannelConfigIdx ) );
    RetTmp = E_OK;*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_CHECKWAKEUP_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif

  return (RetTmp);
}


/***********************************************************************************************************************
 *  Lin_30_Generic_Wakeup
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(Std_ReturnType, LIN_30_GENERIC_CODE) Lin_30_Generic_Wakeup( uint8 Channel )
{
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect != LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_UNINIT;
  }
  /* Check Channel parameter validity against configuration */
  else if ( Lin_30_Generic_GetSizeOfChannelID() <= Channel )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check validity of channel handle */
  else if ( !Lin_30_Generic_GetChannelIDIsValidHandle( Channel ) ) /* PRQA S 3325 */ /* MD_MSR_14.1 */
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /*RetTmp = E_OK;*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_WAKEUP_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif

  return (RetTmp);
}


/***********************************************************************************************************************
 *  Lin_30_Generic_GoToSleep
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(Std_ReturnType, LIN_30_GENERIC_CODE) Lin_30_Generic_GoToSleep( uint8 Channel )
{
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect != LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_UNINIT;
  }
  /* Check if this is a master channel */
  else if ( Lin_30_Generic_GetNodeType( Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel ) ) == LIN_30_GENERIC_SLAVE_NODETYPE )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check Channel parameter validity against configuration */
  else if ( Lin_30_Generic_GetSizeOfChannelID() <= Channel )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check validity of channel handle */
  else if ( !Lin_30_Generic_GetChannelIDIsValidHandle( Channel ) ) /* PRQA S 3325 */ /* MD_MSR_14.1 */
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /*RetTmp = E_OK;*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_GOTOSLEEP_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif

  return (RetTmp);
}


/***********************************************************************************************************************
 *  Lin_30_Generic_GoToSleepInternal
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(Std_ReturnType, LIN_30_GENERIC_CODE) Lin_30_Generic_GoToSleepInternal( uint8 Channel )
{
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect != LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_UNINIT;
  }
  /* Check Channel parameter validity against configuration */
  else if ( Lin_30_Generic_GetSizeOfChannelID() <= Channel )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check validity of channel handle */
  else if ( !Lin_30_Generic_GetChannelIDIsValidHandle( Channel ) ) /* PRQA S 3325 */ /* MD_MSR_14.1 */
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /*RetTmp = E_OK;*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_GOTOSLEEPINTERNAL_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif

  return (RetTmp);
}


/***********************************************************************************************************************
 *  Lin_30_Generic_WakeupInternal
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(Std_ReturnType, LIN_30_GENERIC_CODE) Lin_30_Generic_WakeupInternal( uint8 Channel )
{
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(Lin_30_Generic_ChannelConfigIndexType, AUTOMATIC) ChannelConfigIdx;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect != LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_UNINIT;
  }
  /* Check Channel parameter validity against configuration */
  else if ( Lin_30_Generic_GetSizeOfChannelID() <= Channel )
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  /* Check validity of channel handle */
  else if ( !Lin_30_Generic_GetChannelIDIsValidHandle( Channel ) ) /* PRQA S 3325 */ /* MD_MSR_14.1 */
  {
    ErrorId = LIN_30_GENERIC_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    ChannelConfigIdx = Lin_30_Generic_GetChannelIDChannelConfigIndex( Channel );
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /*RetTmp = E_OK;*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_WAKEUPINTERNAL_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif

  return (RetTmp);
}


#if ( LIN_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  Lin_30_Generic_GetVersionInfo
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(void, LIN_30_GENERIC_CODE) Lin_30_Generic_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, LIN_30_GENERIC_APPL_VAR) versioninfo )
{
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

# if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check versioninfo for NULL pointer */
  if ( versioninfo == (P2VAR(Std_VersionInfoType, AUTOMATIC, LIN_30_GENERIC_APPL_VAR))NULL_PTR )
  {
    ErrorId = LIN_30_GENERIC_E_PARAM_POINTER;
  }
  else
# endif
  {
/**********************************************************************************************************************/
    /* START OF USER CODE */

    /* Set versioninfo with corresponding macros from component header */
    versioninfo->vendorID           = LIN_30_GENERIC_VENDOR_ID;
    versioninfo->moduleID           = LIN_30_GENERIC_MODULE_ID;
    versioninfo->sw_major_version   = LIN_30_GENERIC_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version   = LIN_30_GENERIC_SW_MINOR_VERSION;
    versioninfo->sw_patch_version   = LIN_30_GENERIC_SW_PATCH_VERSION;
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_GETVERSIONINFO_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif
}
#endif


/***********************************************************************************************************************
 *  Lin_30_Generic_InitMemory
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(void, LIN_30_GENERIC_CODE) Lin_30_Generic_InitMemory( void )
{
#if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Set state to uninitialized */
  Lin_30_Generic_Dev_InitDetect = LIN_30_GENERIC_DEV_UNINITVALUE;
#endif
}


/***********************************************************************************************************************
 *  Lin_30_Generic_Init
 **********************************************************************************************************************/
/*!
 *
 */
FUNC(void, LIN_30_GENERIC_CODE) Lin_30_Generic_Init( P2CONST(Lin_30_Generic_ConfigType, AUTOMATIC, LIN_30_GENERIC_PBCFG) Config )
{
  VAR(uint8, AUTOMATIC) ErrorId = LIN_30_GENERIC_E_NO_ERROR;

#if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if ( Lin_30_Generic_Dev_InitDetect == LIN_30_GENERIC_DEV_INITVALUE )
  {
    ErrorId = LIN_30_GENERIC_E_STATE_TRANSITION;
  }
  else
#endif
  {
#if ( LIN_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
      /* Set state to initialized */
      Lin_30_Generic_Dev_InitDetect = LIN_30_GENERIC_DEV_INITVALUE;
#endif
/**********************************************************************************************************************/
    /* START OF USER CODE */

  /*for(int i = 0; i < Lin_30_Generic_GetNumberOfChannels(); i++) {
      Lin_30_Generic_GetChannelId(i);                               get ChannelID
      Lin_30_Generic_GetHWChannel(i);                               get Hardware Channel
      Lin_30_Generic_GetBaudRate(i);                                get baud rate of this Channel
      Lin_30_Generic_GetClock(i);                                   get Clock of this channel
      Lin_30_Generic_GetWakeupSupport(i);                           get Wakeup suppor enabled
      Lin_30_Generic_GetEcuMWakeupSource(i);                        get Wakeup source
    }*/
    /* END OF USER CODE */
/**********************************************************************************************************************/
  }

#if ( LIN_30_GENERIC_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_30_GENERIC_E_NO_ERROR )
  {
    Lin_30_Generic_Det_ReportError( LIN_30_GENERIC_SID_INIT_ID, ErrorId );
  }
#else
  LIN_30_GENERIC_DUMMY_STATEMENT(ErrorId);
#endif
}


#define LIN_30_GENERIC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Lin_30_Generic.c
 *********************************************************************************************************************/
