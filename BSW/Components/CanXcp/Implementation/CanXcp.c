/******************************************************************************
 | Project Name:   XCP on CAN Transport Layer for AUTOSAR CanIf Release 3.0
 |    File Name:   CanXcp.c
 |
 |  Description:
 |    Adaption component for the XCP Protocol Layer interface
 |    and AUTOSAR CAN Interface.
 |
 |  Limitations:
 |    - Detection of all XCP slaves within a CAN network is not supported
 |        ( GET_SLAVE_ID )
 |    - Assignment of CAN identifiers to DAQ lists or STIM is not supported
 |        ( GET_DAQ_ID, SET_DAQ_ID )
 |
 |------------------------------------------------------------------------------
 |               C O P Y R I G H T
 |------------------------------------------------------------------------------
 | Copyright (c) 2023 by Vector Informatik GmbH.          All rights reserved.
 |
 |       This software is copyright protected and 
 |       proporietary to Vector Informatik GmbH.
 |       Vector Informatik GmbH grants to you only
 |       those rights as set out in the license conditions.
 |       All other rights remain with Vector Informatik GmbH.
 | 
 |       Diese Software ist urheberrechtlich geschuetzt. 
 |       Vector Informatik GmbH raeumt Ihnen an dieser Software nur 
 |       die in den Lizenzbedingungen ausdruecklich genannten Rechte ein.
 |       Alle anderen Rechte verbleiben bei Vector Informatik GmbH.
 |
 |------------------------------------------------------------------------------
 |  REVISION HISTORY
 |  -------------------------------------------------------------------------------------------------------------------
 |  Refer to the module's header file.
 | 
 |*****************************************************************************/

#define CANXCP_SOURCE

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "CanXcp.h"           /* Include header of XCP on CAN Transport Layer */
#if defined (CANXCP_CONFIGURATOR) && (CANXCP_CONFIGURATOR == 5)
  #include "Xcp.h"                    /* Include header of XCP Protocol Layer */
#else
  #include "Xcp.h"                    /* Include header of XCP Protocol Layer */
#endif
#include "CanIf.h"                                   /* Include CAN Interface */

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  #include "Det.h"                        /* Include Diagnostic Event Manager */
#endif

#if defined (CANXCP_CONFIGURATOR) && (CANXCP_CONFIGURATOR == 5)
  #include "SchM_Xcp.h"
#endif

#if defined ( CANXCP_AMD_RUNTIME_MEASUREMENT ) && ( CANXCP_AMD_RUNTIME_MEASUREMENT == STD_ON )
  #include "AmdRtm.h"
#endif


/******************************************************************************/
/* Version check                                                              */
/******************************************************************************/
#if ( CP_XCPONCANASR_VERSION != 0x0400u )
  #error "Source and Header file are inconsistent!"
#endif
#if ( CP_XCPONCANASR_RELEASE_VERSION != 0x01u )
  #error "Source and Header file are inconsistent!"
#endif

#if ( CANXCP_TPS_MAJOR_VERSION != 0x04u )
# error "Source and Header file CanXcp_Types.h are inconsistent (CANXCP_TPS_MAJOR_VERSION)!"
#endif
#if ( CANXCP_TPS_MINOR_VERSION != 0x00u )
# error "Source and Header file CanXcp_Types.h are inconsistent (CANXCP_TPS_MINOR_VERSION)!"
#endif
#if ( CANXCP_TPS_PATCH_VERSION != 0x01u )
# error "Source and Header file CanXcp_Types.h are inconsistent (CANXCP_TPS_PATCH_VERSION)!"
#endif


/******************************************************************************/
/* Defintions                                                                 */
/******************************************************************************/
/* States of CanXcp_SendWithoutQueueState */
#define CANXCP_MSG_IDLE           0
#define CANXCP_MSG_PENDING        1

/* PRQA S 3453 27 */ /*  MD_MSR_FctLikeMacro */
#if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
  #define CanXcp_GetRxPduId()             CanXcp_RxPduId
  #if ( CanXcp_IsPostbuild() )
    #define CanXcp_GetTxPduIdIdx(idx)     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanIfTxPduId)
    #define CanXcp_GetTxConfPduIdIdx(idx) CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanXcpTxPduId)
    #define CanXcp_GetRxPduIdIdx(idx)     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanXcpRxPduId)
    #define CanXcp_GetMaxCto(idx)         CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].MaxCto)
    #define CanXcp_GetMaxDto(idx)         CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].MaxDto)
    #define CanXcp_GetPduSizeTx(idx)      CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].PduSizeTx)
    #define CanXcp_GetChHandle(NwH)       CanXcp_ConfigurationAccess(CanXcp_NwHMappingPtr[NwH])
  #else
    #define CanXcp_GetTxPduIdIdx(idx)     CanXcp_PduIdField[idx].CanIfTxPduId
    #define CanXcp_GetTxConfPduIdIdx(idx) CanXcp_PduIdField[idx].CanXcpTxPduId
    #define CanXcp_GetRxPduIdIdx(idx)     CanXcp_PduIdField[idx].CanXcpRxPduId
    #define CanXcp_GetMaxCto(idx)         CanXcp_PduIdField[idx].MaxCto
    #define CanXcp_GetMaxDto(idx)         CanXcp_PduIdField[idx].MaxDto
    #define CanXcp_GetPduSizeTx(idx)      CanXcp_PduIdField[idx].PduSizeTx
    #define CanXcp_GetChHandle(NwH)       CanXcp_NwHMapping[NwH]
  #endif
#else
  #if ( CanXcp_IsPostbuild() )
    #define CanXcp_GetTxPduIdIdx(idx)     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanIfTxPduId)
    #define CanXcp_GetTxConfPduIdIdx(idx) CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanXcpTxPduId)
    #define CanXcp_GetRxPduId()           CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[0].CanXcpRxPduId)
    #define CanXcp_GetRxPduIdIdx(idx)     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanXcpRxPduId)
    #define CanXcp_GetMaxCto(idx)         CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].MaxCto)
    #define CanXcp_GetMaxDto(idx)         CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].MaxDto)
    #define CanXcp_GetPduSizeTx(idx)      CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].PduSizeTx)
    #define CanXcp_GetChHandle(NwH)       CanXcp_ConfigurationAccess(CanXcp_NwHMappingPtr[NwH])
  #else
    #define CanXcp_GetTxPduIdIdx(idx)     CanXcp_PduIdField[idx].CanIfTxPduId
    #define CanXcp_GetTxConfPduIdIdx(idx) CanXcp_PduIdField[idx].CanXcpTxPduId
    #define CanXcp_GetRxPduId()           CanXcp_PduIdField[0].CanXcpRxPduId
    #define CanXcp_GetRxPduIdIdx(idx)     CanXcp_PduIdField[idx].CanXcpRxPduId
    #define CanXcp_GetMaxCto(idx)         CanXcp_PduIdField[idx].MaxCto
    #define CanXcp_GetMaxDto(idx)         CanXcp_PduIdField[idx].MaxDto
    #define CanXcp_GetPduSizeTx(idx)      CanXcp_PduIdField[idx].PduSizeTx
    #define CanXcp_GetChHandle(NwH)       CanXcp_NwHMapping[NwH]
  #endif
#endif

/******************************************************************************/
/* Macros                                                                     */
/******************************************************************************/
/***************************************************************************************************/
/* General definition of checking macros */
# define CanXcp_CheckErrorReturnVoid(cond, api, err)        \
{ if(!(cond)){                                           \
    CanXcp_CallDetReportError((api), (err));                \
    return;                                              \
}}

# define CanXcp_CheckErrorReturnValue(cond, api, err, ret)  \
{ if(!(cond)){                                           \
    CanXcp_CallDetReportError((api), (err));                \
    return(ret);                                         \
}}

# define CanXcp_CheckErrorContinue(cond, api, err)          \
{ if(!(cond)){                                           \
    CanXcp_CallDetReportError((api), (err));                \
}}

/***************************************************************************************************/
/* Macros for DET abstraction */
#if(CANXCP_DEV_ERROR_DETECT == STD_ON)
  # define CanXcp_CheckDetErrorReturnVoid(cond, api, err)       CanXcp_CheckErrorReturnVoid((cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
  # define CanXcp_CheckDetErrorReturnValue(cond, api, err, ret) CanXcp_CheckErrorReturnValue((cond), (api), (err), (ret)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
  # define CanXcp_CheckDetErrorContinue(cond, api, err)         CanXcp_CheckErrorContinue((cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
  # define CanXcp_CallDetReportError(api, err)                  (void)Det_ReportError(CANXCP_MODULE_ID, 0u, (api), (err)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
  # define CanXcp_CheckDetErrorReturnVoid(cond, api, err) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
  # define CanXcp_CheckDetErrorReturnValue(cond, api, err, ret) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
  # define CanXcp_CheckDetErrorContinue(cond, api, err) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
  # define CanXcp_CallDetReportError(api, err) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

/***************************************************************************************************/
/* Macros for SBSW check abstraction */
#if(CANXCP_SILENT_CHECK == STD_ON)
  # define CanXcp_SilentBswCheckErrorReturnVoid(cond, api, err)       CanXcp_CheckErrorReturnVoid( (cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
  # define CanXcp_SilentBswCheckErrorReturnValue(cond, api, err, ret) CanXcp_CheckErrorReturnValue( (cond), (api), (err), (ret)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
  # define CanXcp_SilentBswCheckErrorReturnVoid(cond, api, err)                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */ \
  CanXcp_CheckDetErrorReturnVoid( (cond), (api), (err) )
  # define CanXcp_SilentBswCheckErrorReturnValue(cond, api, err, ret)          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */ \
  CanXcp_CheckDetErrorReturnValue( (cond), (api), (err), (ret) )
#endif 


/*******************************************************************************
 * Types
*******************************************************************************/
#define CANXCP_MAX(a, b)   (((a) > (b)) ? (a) : (b)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

typedef union { /* PRQA S 0750 */ /* MD_MSR_Union */
  uint8  b[CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO)]; /* PRQA S 3494 */ /* MD_CANXCP_3494 */
  uint16 w[CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO)/2u]; /* PRQA S 3494 */ /* MD_CANXCP_3494 */
  uint32 dw[CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO)/4u]; /* PRQA S 3494 */ /* MD_CANXCP_3494 */
} tCanXcpXto;

/* Define AUTOSAR types if not defined yet */
#if defined ( STATIC )
#else
  #define STATIC static
#endif

/*******************************************************************************
 * Data definitions
*******************************************************************************/
/* 
  State of CAN message, required to send without any queue.
  (requested to send via CanXcp_Send).
  These variables are initialized inCanXcp_Init().
*/
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
# define CANXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* TL Enable/Disable state */
VAR(uint8, CANXCP_VAR_INIT) CanXcp_ControlState = kXcponCan_Control_Enable;

# define CANXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

#define CANXCP_START_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
STATIC volatile VAR(uint8, CANXCP_VAR_INIT) CanXcp_InitializationState = CANXCP_UNINIT;
#endif

#if ( CANXCP_ENABLE_CONTROL == STD_ON )
VAR(uint8, CANXCP_VAR_INIT) CanXcp_ControlDeniedCount = 0u;
#endif

#define CANXCP_STOP_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CANXCP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

STATIC VAR(uint8, CANXCP_VAR_NOINIT) CanXcp_SendWithoutQueueState[CANXCP_NUMBER_OF_CHANNELS];

#define CANXCP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
STATIC VAR(CanXcp_PduSetModeType, CANXCP_VAR_NOINIT) CanXcp_PduState[CANXCP_NUMBER_OF_CHANNELS];
#endif
STATIC VAR(PduInfoType, CANXCP_VAR_NOINIT) CanXcp_QueuedPduInfo[CANXCP_NUMBER_OF_CHANNELS];
STATIC VAR(CanXcp_TimeoutTimerType, CANXCP_VAR_NOINIT) CanXcp_ConfTimeoutTimer[CANXCP_NUMBER_OF_CHANNELS];
STATIC VAR(tCanXcpXto, CANXCP_VAR_NOINIT) CanXcp_PduBuffer; /* PRQA S 0759, 3218 */ /* MD_MSR_Union, MD_CANXCP_3218 */

#define CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
/* global variables */
# define CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( XCP_NUMBER_OF_CHANNELS <= 1 )
STATIC VAR(PduIdType, CANXCP_VAR_NOINIT) CanXcp_RxPduId;
#endif

# define CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

#if ( CanXcp_IsPostbuild() )
# define CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

STATIC P2CONST(CanXcp_ConfigType, CANXCP_VAR_NOINIT, CANXCP_PBCFG) CanXcp_Configuration;

# define CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# define CanXcp_ConfigurationAccess(a)  (CanXcp_Configuration->a) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define CanXcp_ConfigurationAccess(a)  (CanXcp_Config.a) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

/****************************************************************************/
/* External ROM Data                                                        */
/****************************************************************************/

#define CANXCP_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Global constants with XCP Transport Layer for CAN main and subversion */
CONST(uint8, CANXCP_CONST) kXcpOnCanAsrMainVersion    = (uint8)(CP_XCPONCANASR_VERSION >> 8);
CONST(uint8, CANXCP_CONST) kXcpOnCanSubAsrVersion     = (uint8)(CP_XCPONCANASR_VERSION & 0x00ffu);
CONST(uint8, CANXCP_CONST) kXcpOnCanAsrReleaseVersion = (uint8)(CP_XCPONCANASR_RELEASE_VERSION);

#define CANXCP_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/******************************************************************************
*
*
*           Code Part for Cp_XcpOnCanAsr
*
*
*******************************************************************************/

#define CANXCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
| NAME:             CanXcp_InitMemory()
| SID:              none
| CALLED BY:        Upper layer
| PRECONDITIONS:    
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Initializes global variables. This function has to be called
|                   before any other calls to the CanXcp API
****************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) CanXcp_InitMemory( void )
{
#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  CanXcp_InitializationState = CANXCP_UNINIT;
#endif

#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  /* TL Enable/Disable state */
  CanXcp_ControlState = kXcponCan_Control_Enable;
  CanXcp_ControlDeniedCount = 0u;
#endif
}


#if ( CANXCP_VERSION_INFO_API == STD_ON )
/****************************************************************************
| NAME:             CanXcp_GetVersionInfo()
| SID:              0x0A
| CALLED BY:        Upper layer
| PRECONDITIONS:
| INPUT PARAMETERS: versioninfo - buffer to copy version info to
| RETURN VALUES:    Std_ReturnType
| DESCRIPTION:      Provide DLC and data of the specified Pdu to upper layers
****************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) CanXcp_GetVersionInfo /* PRQA S 2889 */ /* MD_CANXCP_2889 */
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CANXCP_APPL_VAR) Versioninfo
)
{
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

  CanXcp_SilentBswCheckErrorReturnVoid( Versioninfo != (P2VAR(Std_VersionInfoType, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR,
                                        CANXCP_GETVERSIONINFO_ID, CANXCP_E_NULL_POINTER )

  Versioninfo->vendorID         = CANXCP_VENDOR_ID;
  Versioninfo->moduleID         = CANXCP_MODULE_ID;
  #if ( defined CANXCP_AUTOSARVERSION ) && ( CANXCP_AUTOSARVERSION == 4u )
  #else
  Versioninfo->instanceID       = 0x00u;
  #endif
  Versioninfo->sw_major_version = (uint8)CANXCP_SW_MAJOR_VERSION;
  Versioninfo->sw_minor_version = (uint8)CANXCP_SW_MINOR_VERSION;
  Versioninfo->sw_patch_version = (uint8)CANXCP_SW_PATCH_VERSION;
}
#endif


/*******************************************************************************
** Function:    CanXcp_Init
**
** Purpose:     Initializations of the XCP on CAN Transport Layer.
**
**  Called by:  Xcp_SendCrm, Xcp_SendEv, Xcp_SendDto
**
**  Parameter:  - 
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) CanXcp_Init /* PRQA S 2889 */ /* MD_CANXCP_2889 */
(
  P2CONST(CanXcp_ConfigType, AUTOMATIC, CANXCP_PBCFG) ConfigPtr
)
{
  uint8_least i;

#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  CanXcp_InitializationState = CANXCP_UNINIT;
#endif

#if ( CanXcp_IsPostbuild() )
  CanXcp_Configuration = ConfigPtr;
#else
  XCP_DUMMY_STATEMENT (ConfigPtr); /* PRQA S 3112, 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

#if defined ( CANXCP_MULTI_CHANNEL_ENABLED ) && ( XCP_NUMBER_OF_CHANNELS <= 1 )
  /* Initialize static variables. */
  CanXcp_RxPduId = CanXcp_GetRxPduIdIdx(0);
#endif
  for( i = 0u; i < CANXCP_NUMBER_OF_CHANNELS; i++ )
  {
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
    CanXcp_PduState[i] = CANXCP_SET_OFFLINE;
#endif
    CanXcp_SendWithoutQueueState[i] = (uint8)CANXCP_MSG_IDLE;
    CanXcp_ConfTimeoutTimer[i] = (CanXcp_TimeoutTimerType)0u;
    /* Initialize variables for transmisson without any queue. */
    CanXcp_QueuedPduInfo[i].SduDataPtr = (P2VAR(uint8, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR;
    CanXcp_QueuedPduInfo[i].SduLength  = (PduLengthType)8u;
  }

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  CanXcp_InitializationState = CANXCP_INIT;
#endif
}


/*******************************************************************************
** Function:    CanXcp_MainFunction
**
** Purpose:     Main function of XCP on CAN Transport Layer
**
**  Called by:  BSW Scheduler, OS
**
**  Parameter:  - 
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) CanXcp_MainFunction( void ) /* PRQA S 2889 */ /* MD_CANXCP_2889 */
{
  uint8_least i;
  /* Activation control */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

  for( i = 0u; i < CANXCP_NUMBER_OF_CHANNELS; i++ )
  {
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
    if ( CANXCP_SET_ONLINE == CanXcp_PduState[i] )
#endif
    {
      /* If a CAN message is pending, try to send this */
      if ( CanXcp_SendWithoutQueueState[i] == (uint8)CANXCP_MSG_PENDING )
      {
        /* Try to transmit the XCP Packet */
        if ( CanIf_Transmit( CanXcp_GetTxPduIdIdx(i), &CanXcp_QueuedPduInfo[i] ) == (Std_ReturnType)E_OK )
        {
          CanXcp_ConfTimeoutTimer[i] = (CanXcp_TimeoutTimerType)CANXCP_CONF_TIMEOUT;
          /* The message was sent successfully. Clear the 'message pending' bit. */
          CanXcp_SendWithoutQueueState[i] = (uint8)CANXCP_MSG_IDLE;
        }
        else
        {
          /* The message is still pending. */
        }
      }
    }

    CanXcp_EnterCritical();
    if( CanXcp_ConfTimeoutTimer[i] > 0u)
    {
      CanXcp_ConfTimeoutTimer[i]--;
      if( 0u == CanXcp_ConfTimeoutTimer[i] )
      {
        CanXcp_LeaveCritical();
        /* Timeout happened, release PDU */
        (void)Xcp_SendCallBack((uint8)(XCP_TRANSPORT_LAYER_CAN + i)); /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
      }
      else
      {
        CanXcp_LeaveCritical();
      }
    }
    else
    {
      CanXcp_LeaveCritical();
    }
  
    /* Trigger pending DTOs */
    Xcp_TlMainFunction((uint8)(XCP_TRANSPORT_LAYER_CAN + i)); /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
  }
}


/*******************************************************************************
** Function:    CanXcp_RxIndication
**
** Purpose:     Rx Indication for reception of CTO and DTO Packets
**              This function is configured in the generation tool.
**
**  Called by:  CanIf
**
**  Parameter:  
**              CanCanXcpRxPduId : Target PDU ID of CAN L-PDU that has been received.
**              CanSduPtr        : Pointer to received L-SDU (payload).
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 2 */ /* MD_MSR_STPTH */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, CANXCP_CODE) Xcp_CanIfRxIndication /* PRQA S 2889 */ /* MD_CANXCP_2889 */
(
  VAR(PduIdType, AUTOMATIC) CanCanXcpRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, CANXCP_APPL_VAR) PduInfoPtr
)
{
  uint8_least i;
  PduLengthType sduLen;
#if ( XCP_NUMBER_OF_CHANNELS > 1u ) || !defined ( CANXCP_MULTI_CHANNEL_ENABLED )
  PduIdType CanXcp_RxPduId;
#endif

  /* Activation control */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_RXINDICATION_SERVICE_ID, CANXCP_E_NOT_INITIALIZED )
  CanXcp_SilentBswCheckErrorReturnVoid( PduInfoPtr != (P2CONST(PduInfoType, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR,
                                        CANXCP_RXINDICATION_SERVICE_ID, CANXCP_E_NULL_POINTER )
  CanXcp_SilentBswCheckErrorReturnVoid( PduInfoPtr->SduDataPtr != (SduDataPtrType)NULL_PTR,
                                        CANXCP_RXINDICATION_SERVICE_ID, CANXCP_E_NULL_POINTER )

#if ( XCP_NUMBER_OF_CHANNELS > 1u )
  /* Store the Rx PDU Id to determine the reception channel. */
  CanXcp_RxPduId = CanCanXcpRxPduId;
#else
# if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
  if ( CanXcp_RxPduId != CanCanXcpRxPduId )
  {
#  if defined ( CANXCP_MULTI_CONN_PROT_ENABLED )
    /* Ensure that no XCP connection is interrupt via another channel. */
    if ((Xcp_GetSessionStatus((Xcp_ChannelType)(XCP_TRANSPORT_LAYER_CAN + CanXcp_RxPduId)) & (SessionStatusType)SS_CONNECTED) != 0u) /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
    {
      /* Do not transmit an error packet due to XcpConfirmation() will be called on the wrong channel. */
      return;
    }
    else
#  endif
    {
      /* Store the Rx PDU Id to determine the reception channel. */
      CanXcp_RxPduId = CanCanXcpRxPduId;
    }
  }
# else
  /* Verifiy RX Pdu Id */
  if(CanCanXcpRxPduId == CanXcp_GetRxPduId())
  {
    /* Store the Rx PDU Id for the reception. */
    CanXcp_RxPduId = CanCanXcpRxPduId;
# endif
#endif

    if(0xffu == PduInfoPtr->SduDataPtr[0])
    {
      /* In case of connect we set this as active TL */
      Xcp_SetActiveTl((Xcp_ChannelType)(XCP_TRANSPORT_LAYER_CAN + CanXcp_RxPduId), /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
        CanXcp_GetMaxCto(CanXcp_RxPduId),
        CanXcp_GetMaxDto(CanXcp_RxPduId),
        (uint8)(XCP_TRANSPORT_LAYER_CAN + CanXcp_RxPduId)); /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
    }

    if((XCP_TRANSPORT_LAYER_CAN + CanXcp_RxPduId) == Xcp_GetActiveTl((Xcp_ChannelType)(XCP_TRANSPORT_LAYER_CAN + CanXcp_RxPduId))) /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
    {
      /*
        A XCP Packet has been received.
        Copy the whole received SDU to an 32-Bit aligned buffer.
      */
      sduLen = CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO); /* PRQA S 3494 */ /* MD_CANXCP_3494 */
      if( PduInfoPtr->SduLength < sduLen )
      {
        sduLen = PduInfoPtr->SduLength;
      }
      for(i=0u; i<sduLen; i++)
      {
        CanXcp_PduBuffer.b[i] = PduInfoPtr->SduDataPtr[i];
      }
      /* Execution of Command Interpreter (evaluation of the XCP Packet) */
      Xcp_Command((uint8)(XCP_TRANSPORT_LAYER_CAN + CanXcp_RxPduId), CanXcp_PduBuffer.dw ); /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
    }
#if ( XCP_NUMBER_OF_CHANNELS > 1u )
#else
# if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
# else
  }
# endif
#endif
}


/*******************************************************************************
** Function:    CanXcp_TxConfirmation
**
** Purpose:     Tx Confirmation for successful transmission of CTO and DTO Packets
**              This function is configured in the generation tool.
**
**  Called by:  CanIf
**
**  Parameter:  
**              CanCanXcpTxPduId : PDU ID of CAN L-PDU transmitted successfully.
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) Xcp_CanIfTxConfirmation( VAR(PduIdType, AUTOMATIC) CanTxPduId ) /* PRQA S 2889 */ /* MD_CANXCP_2889 */
{
  uint8_least i;

  /* Activation control */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_TXCONFIRMATION_SERVICE_ID, CANXCP_E_NOT_INITIALIZED )

  /* Search respective channel handle */
  for(i=0u; i<CANXCP_NUMBER_OF_CHANNELS; i++)
  {
    if( CanXcp_GetTxConfPduIdIdx(i) == CanTxPduId )
    {
      break;
    }
  }

  if( i < CANXCP_NUMBER_OF_CHANNELS )
  {
    CanXcp_ConfTimeoutTimer[i] = (CanXcp_TimeoutTimerType)0u;
    /*
      Notify the XCP protocol driver of the successful transmission of a message.
      The XCP driver will not call ApplXcpSend() again, until Xcp_SendCallBack has
      indicated the successful transmission of the previous message.
    */
    (void)Xcp_SendCallBack((uint8)(XCP_TRANSPORT_LAYER_CAN + i)); /* PRQA S 2986 */ /* MD_CANXCP_2985_6 */
  }
}


/*******************************************************************************
** Function:    CanXcp_Send
**
** Purpose:     Request for the transmission of a DTO or CTO message
**
**  Called by:  Xcp_SendCrm, Xcp_SendEv, Xcp_SendDto
**
**  Parameter:  
**              len : length of PDU data
**              msg : pointer to PDU data
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) CanXcp_Send /* PRQA S 2889 */ /* MD_CANXCP_2889 */
(
  uint8 Xcp_Channel,
  uint16 len,
  P2CONST(uint8, AUTOMATIC, CANXCP_APPL_VAR) msg
) /* ESCAN00071289 */
{
  PduInfoType PduInfo;
  uint8 actualChannel;

  /* Activation control */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_SEND_ID, CANXCP_E_NOT_INITIALIZED )
  CanXcp_SilentBswCheckErrorReturnVoid( msg != (P2CONST(uint8, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR,
                                        CANXCP_SEND_ID, CANXCP_E_NULL_POINTER )

  /* Set correct channel index */
#if defined ( CANXCP_MULTI_CHANNEL_ENABLED ) && ( XCP_NUMBER_OF_CHANNELS <= 1u )
  actualChannel = (uint8)CanXcp_RxPduId;
  XCP_DUMMY_STATEMENT (Xcp_Channel); /* PRQA S 3112, 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#else
  actualChannel = Xcp_Channel - XCP_TRANSPORT_LAYER_CAN; /* PRQA S 2985 */ /* MD_CANXCP_2985_6 */
#endif

  PduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, CANXCP_APPL_VAR))msg; /* PRQA S 0311 */ /* MD_CANXCP_0311 */
#if defined ( CANXCP_VARIABLE_DLC_ENABLED )
  PduInfo.SduLength = (PduLengthType)len;
#else
  PduInfo.SduLength = CanXcp_GetPduSizeTx(actualChannel);
  XCP_DUMMY_STATEMENT(len); /* PRQA S 3112, 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
  if ( CANXCP_SET_ONLINE == CanXcp_PduState[actualChannel] )
  {
#endif
    /* Transmit the XCP Packet */
    if ( CanIf_Transmit( CanXcp_GetTxPduIdIdx(actualChannel), &PduInfo ) != (Std_ReturnType)E_OK )
    {
#if ( CANIF_TRANSMIT_BUFFER == STD_ON )

      /* Fatal Error. XCP Packet cannot be transmitted. */
      Xcp_Init();

#else /* if ( CANIF_TRANSMIT_BUFFER == STD_ON ) */
      /*
        Transmission via CanIf_Transmit has failed.
        Set a flag in order to re-start transmission of the XCP Packet in CanXcp_MainFunction
      */
      CanXcp_SendWithoutQueueState[actualChannel] = (uint8)CANXCP_MSG_PENDING;
      CanXcp_QueuedPduInfo[actualChannel]         = PduInfo;
#endif /* if ( CANIF_TRANSMIT_BUFFER == STD_ON ) */
    }
    else
    {
      CanXcp_ConfTimeoutTimer[actualChannel] = (CanXcp_TimeoutTimerType)CANXCP_CONF_TIMEOUT;
    }
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
  }
  else
  {
    CanXcp_SendWithoutQueueState[actualChannel] = (uint8)CANXCP_MSG_PENDING;
    CanXcp_QueuedPduInfo[actualChannel]         = PduInfo;
  }
#endif
}


/*******************************************************************************
** Function:    CanXcp_SendFlush
**
** Purpose:     Flush function of XCP on CAN Transport Layer
**
**  Called by:  Upper Layer
**
**  Parameter:  FlushType: 
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) CanXcp_SendFlush( uint8 Xcp_Channel, uint8 FlushType ) /* PRQA S 2889 */ /* MD_CANXCP_2889 */
{
  /* Activation control */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_SENDFLUSH_ID, CANXCP_E_NOT_INITIALIZED )

  XCP_DUMMY_STATEMENT (Xcp_Channel); /* PRQA S 3112, 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */  
  XCP_DUMMY_STATEMENT (FlushType); /* PRQA S 3112, 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */  
}


/*******************************************************************************
** Function:    CanXcp_TLService
**
** Purpose:     CAN Transport Layer Command
**
**  Called by:  Upper Layer
**
**  Parameter:  pCmd: pointer to cmd string
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, CANXCP_CODE) CanXcp_TLService( uint8 Xcp_Channel, P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) pCmd ) /* PRQA S 2889 */ /* MD_CANXCP_2889 */
{
  /* Activation control */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return(XCP_CMD_UNKNOWN);
  }
#endif

  CanXcp_CheckDetErrorReturnValue( CANXCP_UNINIT != CanXcp_InitializationState,
                                   CANXCP_TLSERVICE_ID, CANXCP_E_NOT_INITIALIZED, XCP_CMD_UNKNOWN )

  XCP_DUMMY_STATEMENT (Xcp_Channel); /* PRQA S 3112, 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */    
  XCP_DUMMY_STATEMENT (pCmd); /* PRQA S 3112, 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */    
  return(XCP_CMD_UNKNOWN);
}


#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
/*******************************************************************************
** Function:    CanXcp_SetPduMode
**
** Purpose:     Enable or disable the component
**
**  Called by:  State Manager
**
**  Parameter:  command : enable/disable
**
**  Returncode: -
*******************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANXCP_CODE) CanXcp_SetPduMode( NetworkHandleType XcpNwH, CanXcp_PduSetModeType PduMode ) /* PRQA S 2889 */ /* MD_CANXCP_2889 */
{
  /* Activation control */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  if ( CanXcp_ControlState == kXcponCan_Control_Disable )
  {
    CanXcp_ControlDeniedCount++;
    return;
  }
#endif

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_SETPDUMODE_ID, CANXCP_E_NOT_INITIALIZED )

  CanXcp_SilentBswCheckErrorReturnVoid(XcpNwH < CANXCP_NUMBER_OF_CHANNELS, CANXCP_SETPDUMODE_ID,
                                       CANXCP_E_INVALID_NW_HANDLE);

  CanXcp_PduState[XcpNwH] = PduMode;
}
#endif


#define CANXCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*******************************************************************************
  Consistency checks
*******************************************************************************/

/* Check configuration of Transport Layer */
#if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN_ASR )
#else
  #error "Configuration for XCP Transport Layer for AUTOSAR CAN Interface doesn't exist!"
#endif

/* Check range of kXcpMaxCTO and kXcpMaxDTO */
#if ( kCanXcpMaxCTO <= CANXCP_PDU_SIZE )
#else
  #error "XCP consistency error: MAX_CTO must be less or equal to the PDU size on CAN."
#endif
#if ( kCanXcpMaxDTO <= CANXCP_PDU_SIZE )
#else
  #error "XCP consistency error: MAX_DTO must be less or equal to the PDU size on CAN."
#endif


/*******************************************************************************
* Organi check
*******************************************************************************/


/* module specific MISRA deviations:
  MD_CANXCP_0311: Rule 11.8
      Reason:     Std Type from ASR does not use const, casting required to avoid compiler warning.
      Risk:       There is no risk as the pointers do only point to local component memory.
      Prevention: Not required.

  MD_CANXCP_2889: Rule 15.5
      Reason:     API pattern not applied in this component yet.
      Risk:       Code understandability and maintainability reduced.
      Prevention: Covered by code review.

  MD_CANXCP_2985_6: Rule 2.2
      Reason:     generated code might have 0 as value which leads to this warning.
      Risk:       No Risk.
      Prevention: Not required.

  MD_CANXCP_3218: Rule 8.9
      Reason:     For proper use of AUTOSAR memory mapping the variable is kept global even though
                  it is only used in one function.
      Risk:       There is no risk as there is no prohibition of use.
      Prevention: Not required.

  MD_CANXCP_3494: Rule 14.3
      Reason:     A pre-compile macro is used to determine parameters based on configuration.
      Risk:       There is no risk as this is a valid operation and performed by pre-processor.
      Prevention: Not required.
*/

/**********************************************************************************************************************
 *  END OF FILE: CanXcp.c
 *********************************************************************************************************************/
