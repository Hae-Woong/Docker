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
/*!        \file  EthIf_MacSec.c
 *        \brief  EthIf MACsec public API source file
 *
 *      \details  Implementation of the public API of the sub-module MACsec of EthIf.
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

#define ETHIF_MACSEC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1975
 *********************************************************************************************************/
#include "EthIf_MacSec.h"
#include "EthIf_MacSec_Cbk.h"
#include "EthIf_MacSec_Int.h"
#include "EthIf_Link_Int.h"

#if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
# include "Mka.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of Msn header file */
#if (  (ETHIF_SW_MAJOR_VERSION != (17u)) \
    || (ETHIF_SW_MINOR_VERSION != (2u)) \
    || (ETHIF_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of EthIf_EthTrcv.c and EthIf.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined(ETHIF_MACSEC_LOCAL)                                                                                      /* COV_ETHIF_LOCAL_INLINE */
# define ETHIF_MACSEC_LOCAL                                           static
#endif

#if !defined(ETHIF_MACSEC_LOCAL_INLINE)                                                                               /* COV_ETHIF_LOCAL_INLINE */
# define ETHIF_MACSEC_LOCAL_INLINE                                    LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
/**********************************************************************************************************************
 *  EthIf_Internal_SwitchMacSecOperational
 *********************************************************************************************************************/
/*! \brief          Updates the link state of the Switch Port after MACsec operational notification by MKA
 *  \details        -
 *  \param[in]      mgmtInfoPtr       Pointer to the management information within the context of an Ethernet Switch
 *                                    Driver. SwitchIdx in context of EthIf, PortIdx in context of EthSwt.
 *  \param[in]      macSecOperational Boolean to notify if MACsec is operational
 *  \return         E_OK              The request has been accepted
 *  \return         E_NOT_OK          The request has not been accepted
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE
 *********************************************************************************************************************/
ETHIF_MACSEC_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_MACSEC_INLINE_CODE) EthIf_Internal_SwitchMacSecOperational(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_VAR) mgmtInfoPtr,
  boolean                                                 macSecOperational);

/**********************************************************************************************************************
 *  EthIf_Internal_MacSecOperational
 *********************************************************************************************************************/
/*! \brief          Updates the link state of the Transceiver after MACsec operational notification by MKA
 *  \details        -
 *  \param[in]      ctrlIdx           EthIf Controller Index
 *  \param[in]      macSecOperational Boolean to notify if MACsec is operational
 *  \pre            -
 *  \context        ANY
 *  \reentrant      TRUE
 *********************************************************************************************************************/
ETHIF_MACSEC_LOCAL_INLINE FUNC(void, ETHIF_MACSEC_INLINE_CODE) EthIf_Internal_MacSecOperational(
  uint8   ctrlIdx,
  boolean macSecOperational);
# endif /* ETHIF_ENABLE_MACSEC_API */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS DEFINITIONS
 *********************************************************************************************************************/
#if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
/**********************************************************************************************************************
 *  EthIf_Internal_SwitchMacSecOperational
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
ETHIF_MACSEC_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Internal_SwitchMacSecOperational(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_VAR) mgmtInfoPtr,
  boolean                                                 macSecOperational
)
{
    /* ----- Local Variables ---------------------------------------------- */
    Std_ReturnType retVal = E_NOT_OK;

    /* Get the SwitchIdx within the context of EthIf */
    EthSwt_SwitchIdxType switchIdx = mgmtInfoPtr->SwitchIdx;
    /* Get the PortIdx within the context of EthSwt */
    EthSwt_PortIdxType switchPortIdx = mgmtInfoPtr->PortIdx;
    /* Get the PortIdx within the context of EthIf */
    EthIf_EthSwtPortIterType portIdx = EthIf_GetPortIdxOfEthIfContext(switchIdx, switchPortIdx);

    /* ----- Implementation ----------------------------------------------- */
    /* #10 If the PortIdx is valid */
    if (portIdx < EthIf_GetSizeOfEthSwtPort())
    {
      /* #110 Enter exclusive area (Reason: Ensure consistency of link state on all EthIf users) */
      EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #120 If MACsec is operational */
      if (macSecOperational != FALSE)
      {
        /* #1210 If link state is in state PHY_ACTIVE */
        if (EthIf_GetEthSwtPortLinkState(portIdx) == ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE) {
          /* #12110 Set the link state of this switch port stored in the EthIf link state table as ACTIVE */
          EthIf_SetEthSwtPortLinkState(portIdx, ETHIF_PHYELEM_LINK_STATE_ACTIVE);                                       /* SBSW_ETHIF_COMMON_CSL01 */
        }
      }
      /* #130 If MACsec is not operational */
      else
      {
        /* #1310 If link state is in state ACTIVE */
        if (EthIf_GetEthSwtPortLinkState(portIdx) == ETHIF_PHYELEM_LINK_STATE_ACTIVE) {
          /* #13110 Set the link state of this switch port stored in the EthIf link state table as PHY_ACTIVE */
          EthIf_SetEthSwtPortLinkState(portIdx, ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE);                                   /* SBSW_ETHIF_COMMON_CSL01 */
        }
      }

      /* #140 Leave exclusive area -> processing of elements finished */
      EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      retVal = E_OK;
    }

    return retVal;
} /* EthIf_Internal_SwitchMacSecOperational() */


/**********************************************************************************************************************
 *  EthIf_Internal_MacSecOperational
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
ETHIF_MACSEC_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_Internal_MacSecOperational(
  uint8   ctrlIdx,
  boolean macSecOperational
)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(ctrlIdx);
  EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter exclusive area (Reason: Ensure consistency of link state on all EthIf users) */
  EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 If MACsec is operational */
  if (macSecOperational != FALSE)
  {
    /* #210 If link state is in state PHY_ACTIVE */
    if (EthIf_GetEthTrcvLinkState(ethTrcvIdx) == ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE) {
      /* #2110 Set the link state of this transceiver stored in the EthIf link state table as ACTIVE */
      EthIf_SetEthTrcvLinkState(ethTrcvIdx, ETHIF_PHYELEM_LINK_STATE_ACTIVE);                                           /* SBSW_ETHIF_COMMON_CSL01 */
    }
  }
  /* #30 If MACsec is not operational */
  else
  {
    /* #310 If link state is in state ACTIVE */
    if (EthIf_GetEthTrcvLinkState(ethTrcvIdx) == ETHIF_PHYELEM_LINK_STATE_ACTIVE) {
      /* #3110 Set the link state of this transceiver stored in the EthIf link state table as PHY_ACTIVE */
      EthIf_SetEthTrcvLinkState(ethTrcvIdx, ETHIF_PHYELEM_LINK_STATE_PHY_ACTIVE);                                       /* SBSW_ETHIF_COMMON_CSL01 */
    }
  }

  /* #40 Leave exclusive area -> processing of elements finished */
  EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_Internal_MacSecOperational() */
# endif /* ETHIF_ENABLE_MACSEC_API */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if ( ETHIF_ENABLE_MACSEC_API == STD_ON )
/***********************************************************************************************************************
 *  EthIf_SwitchMacSecUpdateSecY
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecUpdateSecY( /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(EthSwt_MgmtInfoType,  AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
  P2CONST(Mka_MacSecConfigType, AUTOMATIC, ETHIF_APPL_DATA) MACsecCfgPtr,
          uint64                                            TxSci)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input pointer for plausibility */
  else if (MACsecCfgPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #50 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #510 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #520 Check if driver provides the API */
    if (EthIf_GetMacSecUpdateSecYOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #5210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecUpdateSecYOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                      MACsecCfgPtr,
                                                                      TxSci);                                           /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_UPDATE_SECY, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecUpdateSecY() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecUpdateSecYNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecUpdateSecYNotification(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Get the SwitchIdx within the context of EthIf */
    EthSwt_SwitchIdxType switchIdx = MgmtInfoPtr->SwitchIdx;
    /* Get the PortIdx within the context of EthSwt */
    EthSwt_PortIdxType switchPortIdx = MgmtInfoPtr->PortIdx;
    /* Get the PortIdx within the context of EthIf */
    EthIf_EthSwtPortIterType portIdx = EthIf_GetPortIdxOfEthIfContext(switchIdx, switchPortIdx);

    /* #410 If the port index is valid and the port is secured */
    if ((portIdx < EthIf_GetSizeOfEthSwtPort()) && (EthIf_IsMacSecSecuredOfEthSwtPort(portIdx) == TRUE) )
    {
      /* #4110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthSwtPort(portIdx);
      /* #4120 Redirect the call to MKA module */
      Mka_MacSecUpdateSecYNotification(paeIdx);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_UPDATE_SECY_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_SwitchMacSecUpdateSecYNotification() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecInitRxSc
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecInitRxSc(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint64                                           Sci)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt()){
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecInitRxScOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecInitRxScOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                    Sci);                                               /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_INIT_RX_SC, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecInitRxSc() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecResetRxSc
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecResetRxSc(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint64                                           Sci)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecResetRxScOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecResetRxScOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                     Sci);                                              /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_RESET_RX_SC, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecResetRxSc() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecAddTxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecAddTxSa( /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           NextPn,
          uint32                                           Ssci,
  P2CONST(Mka_SakKeyPtrType,   AUTOMATIC, ETHIF_APPL_DATA) KeysPtr,
          boolean                                          Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input pointer for plausibility */
  else if (KeysPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #50 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #510 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #520 Check if driver provides the API */
    if (EthIf_GetMacSecAddTxSaOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #5210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecAddTxSaOfEthSwtDrvApi(ethSwtDrvIdx)(MgmtInfoPtr,
                                                                  An,
                                                                  NextPn,
                                                                  Ssci,
                                                                  KeysPtr,
                                                                  Active);                                              /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_ADD_TX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecAddTxSa() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecAddTxSaNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecAddTxSaNotification(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if ((MgmtInfoPtr->SwitchIdx) >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Get the SwitchIdx within the context of EthIf */
    EthSwt_SwitchIdxType switchIdx = MgmtInfoPtr->SwitchIdx;
    /* Get the PortIdx within the context of EthSwt */
    EthSwt_PortIdxType switchPortIdx = MgmtInfoPtr->PortIdx;
    /* Get the PortIdx within the context of EthIf */
    EthIf_EthSwtPortIterType portIdx = EthIf_GetPortIdxOfEthIfContext(switchIdx, switchPortIdx);

    /* #410 If the port index is valid and the port is secured */
    if ((portIdx < EthIf_GetSizeOfEthSwtPort()) && (EthIf_IsMacSecSecuredOfEthSwtPort(portIdx) == TRUE) )
    {
      /* #4110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthSwtPort(portIdx);
      /* #4120 Redirect the call to MKA module */
      Mka_MacSecAddTxSaNotification(paeIdx);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_ADD_TX_SA_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_SwitchMacSecAddTxSaNotification() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecUpdateTxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecUpdateTxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           NextPn,
          boolean                                          Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecUpdateTxSaOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecUpdateTxSaOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                      An,
                                                                      NextPn,
                                                                      Active);                                          /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_UPDATE_TX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecUpdateTxSa() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecDeleteTxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecDeleteTxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecDeleteTxSaOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecDeleteTxSaOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                      An);                                              /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_DELETE_TX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecDeleteTxSa() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecAddRxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecAddRxSa( /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           LowestPn,
          uint32                                           Ssci,
  P2CONST(Mka_SakKeyPtrType,   AUTOMATIC, ETHIF_APPL_DATA) KeysPtr,
          boolean                                          Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input pointer for plausibility */
  else if (KeysPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #50 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #510 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #520 Check if driver provides the API */
    if (EthIf_GetMacSecAddRxSaOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #5210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecAddRxSaOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                   An,
                                                                   LowestPn,
                                                                   Ssci,
                                                                   KeysPtr,
                                                                   Active);                                             /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_ADD_RX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecAddRxSa() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecAddRxSaNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecAddRxSaNotification(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if ((MgmtInfoPtr->SwitchIdx) >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Get the SwitchIdx within the context of EthIf */
    EthSwt_SwitchIdxType switchIdx = MgmtInfoPtr->SwitchIdx;
    /* Get the PortIdx within the context of EthSwt */
    EthSwt_PortIdxType switchPortIdx = MgmtInfoPtr->PortIdx;
    /* Get the PortIdx within the context of EthIf */
    EthIf_EthSwtPortIterType portIdx = EthIf_GetPortIdxOfEthIfContext(switchIdx, switchPortIdx);

    /* #410 If the port index is valid and the port is secured */
    if ((portIdx < EthIf_GetSizeOfEthSwtPort()) && (EthIf_IsMacSecSecuredOfEthSwtPort(portIdx) == TRUE) )
    {
      /* #4110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthSwtPort(portIdx);
      /* #4120 Redirect the call to MKA module */
      Mka_MacSecAddRxSaNotification(paeIdx);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_ADD_RX_SA_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_SwitchMacSecAddRxSaNotification() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecUpdateRxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecUpdateRxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
          uint64                                           LowestPn,
          boolean                                          Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecUpdateRxSaOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecUpdateRxSaOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                      An,
                                                                      LowestPn,
                                                                      Active);                                          /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_UPDATE_RX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecUpdateRxSa() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecDeleteRxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecDeleteRxSa(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecDeleteRxSaOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecDeleteRxSaOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                      An);                                              /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_DELETE_RX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecDeleteRxSa() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecGetTxSaNextPn
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecGetTxSaNextPn( /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          uint8                                            An,
  P2VAR(  uint64,              AUTOMATIC, ETHIF_APPL_VAR)  NextPnPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input pointer for plausibility */
  else if (NextPnPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #50 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #510 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #520 Check if driver provides the API */
    if (EthIf_GetMacSecGetTxSaNextPnOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #5210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecGetTxSaNextPnOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                         An,
                                                                         NextPnPtr);                                    /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_GET_TX_SA_NEXT_PN, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecGetTxSaNextPn() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecGetMacSecStats
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecGetMacSecStats(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecGetMacSecStatsOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecGetMacSecStatsOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr);                                 /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_GET_MAC_SEC_STATS, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecGetMacSecStats() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecGetMacSecStatsNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_SwitchMacSecGetMacSecStatsNotification( /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
  P2CONST(Mka_Stats_SecYType,  AUTOMATIC, ETHIF_APPL_DATA) MacSecStatsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input pointer for plausibility */
  else if (MacSecStatsPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Check input switch index for plausibility */
  else if ((MgmtInfoPtr->SwitchIdx) >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #50 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Get the SwitchIdx within the context of EthIf */
    EthSwt_SwitchIdxType switchIdx = MgmtInfoPtr->SwitchIdx;
    /* Get the PortIdx within the context of EthSwt */
    EthSwt_PortIdxType switchPortIdx = MgmtInfoPtr->PortIdx;
    /* Get the PortIdx within the context of EthIf */
    EthIf_EthSwtPortIterType portIdx = EthIf_GetPortIdxOfEthIfContext(switchIdx, switchPortIdx);

    /* #510 If the port index is valid and the port is secured */
    if ((portIdx < EthIf_GetSizeOfEthSwtPort()) && (EthIf_IsMacSecSecuredOfEthSwtPort(portIdx) == TRUE) )
    {
      /* #5110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthSwtPort(portIdx);
      /* #5120 Redirect the call to MKA module */
      Mka_GetMacSecStatisticsNotification(paeIdx, MacSecStatsPtr);                                                      /* SBSW_ETHIF_PUB_FCT_PUB_PARAM_PTR_PASS */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_GET_MAC_SEC_STATS_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_SwitchMacSecGetMacSecStatsNotification() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecSetControlledPortEnabled
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecSetControlledPortEnabled(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr,
          boolean                                          ControlledPortEnabled)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if (MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet Switch index to API index */
    EthIf_EthSwtDrvApiIdxOfEthSwtType ethSwtDrvIdx = EthIf_GetEthSwtDrvApiIdxOfEthSwt(MgmtInfoPtr->SwitchIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecSetControlledPortEnabledOfEthSwtDrvApi(ethSwtDrvIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Switch driver */
      retVal = EthIf_GetMacSecSetControlledPortEnabledOfEthSwtDrvApi(ethSwtDrvIdx)( MgmtInfoPtr,
                                                                                    ControlledPortEnabled);             /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_SET_CONTROLLED_PORT_ENABLED, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_SwitchMacSecSetControlledPortEnabled() */

/***********************************************************************************************************************
 *  EthIf_SwitchMacSecOperational
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchMacSecOperational( /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_VAR)  MgmtInfoPtr,
  boolean                                                  MacSecOperational
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check input pointer for plausibility */
  else if( MgmtInfoPtr == NULL_PTR )
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #30 Check input switch index for plausibility */
  else if (MgmtInfoPtr->SwitchIdx >= EthIf_GetSizeOfEthSwt())
  {
    errorId = ETHIF_E_INV_SWT_IDX;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* #410 Call the internal function to update the switch port link state after MACsec state reported by MKA */
    retVal = EthIf_Internal_SwitchMacSecOperational(MgmtInfoPtr, MacSecOperational);                                    /* SBSW_ETHIF_INT_FCT_PUB_PARAM_PTR_PASS */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MACSEC_OPERATIONAL, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif /* ETHIF_DEV_ERROR_REPORT */

  return retVal;
} /* EthIf_SwitchMacSecOperational() */

/***********************************************************************************************************************
 *  EthIf_MacSecUpdateSecY
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecUpdateSecY(
          uint8                                             CtrlIdx,
  P2CONST(Mka_MacSecConfigType, AUTOMATIC, ETHIF_APPL_DATA) MACsecCfgPtr,
          uint64                                            TxSci)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Check input pointer for plausibility */
  else if (MACsecCfgPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecUpdateSecYOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecUpdateSecYOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                          MACsecCfgPtr,
                                                                          TxSci);                                       /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_UPDATE_SECY, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecUpdateSecY() */

/***********************************************************************************************************************
 *  EthIf_MacSecUpdateSecYNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_MacSecUpdateSecYNotification(
  uint8 CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);

    /* #310 If the transceiver is secured */
    if ( EthIf_IsMacSecSecuredOfEthTrcv(ethTrcvIdx) == TRUE )                                                           /* SBSW_ETHIF_REF_OF_STACK_VAR */
    {
      /* #3110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthTrcv(ethTrcvIdx);
      /* #3120 Redirect the call to MKA module */
      Mka_MacSecUpdateSecYNotification(paeIdx);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_UPDATE_SECY_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_MacSecUpdateSecYNotification() */

/***********************************************************************************************************************
 *  EthIf_MacSecInitRxSc
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecInitRxSc(
  uint8   CtrlIdx,
  uint64  Sci)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecInitRxScOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecInitRxScOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx), Sci); /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }

# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_INIT_RX_SC, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecInitRxSc() */

/***********************************************************************************************************************
 *  EthIf_MacSecResetRxSc
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecResetRxSc(
  uint8   CtrlIdx,
  uint64  Sci)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecResetRxScOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecResetRxScOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx), Sci);/* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_RESET_RX_SC, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecResetRxSc() */

/***********************************************************************************************************************
 *  EthIf_MacSecAddTxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecAddTxSa(
          uint8                                           CtrlIdx,
          uint8                                           An,
          uint64                                          NextPn,
          uint32                                          Ssci,
  P2CONST(Mka_SakKeyPtrType, AUTOMATIC, ETHIF_APPL_DATA)  KeysPtr,
          boolean                                         Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Check input pointer for plausibility */
  else if (KeysPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecAddTxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecAddTxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                       An,
                                                                       NextPn,
                                                                       Ssci,
                                                                       KeysPtr,
                                                                       Active);                                         /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_ADD_TX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecAddTxSa() */

/***********************************************************************************************************************
 *  EthIf_MacSecAddTxSaNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_MacSecAddTxSaNotification(
  uint8 CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);

    /* #310 If the transceiver is secured */
    if ( EthIf_IsMacSecSecuredOfEthTrcv(ethTrcvIdx) == TRUE )                                                           /* SBSW_ETHIF_REF_OF_STACK_VAR */
    {
      /* #3110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthTrcv(ethTrcvIdx);
      /* #3120 Redirect the call to MKA module */
      Mka_MacSecAddTxSaNotification(paeIdx);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_ADD_TX_SA_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_MacSecAddTxSaNotification() */

/***********************************************************************************************************************
 *  EthIf_MacSecUpdateTxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecUpdateTxSa(
  uint8   CtrlIdx,
  uint8   An,
  uint64  NextPn,
  boolean Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecUpdateTxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecUpdateTxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                          An,
                                                                          NextPn,
                                                                          Active);                                      /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_UPDATE_TX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecUpdateTxSa() */

/***********************************************************************************************************************
 *  EthIf_MacSecDeleteTxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecDeleteTxSa(
  uint8 CtrlIdx,
  uint8 An)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecDeleteTxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecDeleteTxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                          An);                                          /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_DELETE_TX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecDeleteTxSa() */

/***********************************************************************************************************************
 *  EthIf_MacSecAddRxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecAddRxSa(
          uint8                                           CtrlIdx,
          uint8                                           An,
          uint64                                          LowestPn,
          uint32                                          Ssci,
  P2CONST(Mka_SakKeyPtrType, AUTOMATIC, ETHIF_APPL_DATA)  KeysPtr,
          boolean                                         Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Check input pointer for plausibility */
  else if (KeysPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecAddRxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecAddRxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                       An,
                                                                       LowestPn,
                                                                       Ssci,
                                                                       KeysPtr,
                                                                       Active);                                         /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_ADD_RX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecAddRxSa() */

/***********************************************************************************************************************
 *  EthIf_MacSecAddRxSaNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_MacSecAddRxSaNotification(
  uint8 CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);

    /* #310 If the transceiver is secured */
    if ( EthIf_IsMacSecSecuredOfEthTrcv(ethTrcvIdx) == TRUE )                                                           /* SBSW_ETHIF_REF_OF_STACK_VAR */
    {
      /* #3110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthTrcv(ethTrcvIdx);
      /* #3120 Redirect the call to MKA module */
      Mka_MacSecAddRxSaNotification(paeIdx);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_ADD_RX_SA_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_MacSecAddRxSaNotification() */

/***********************************************************************************************************************
 *  EthIf_MacSecUpdateRxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecUpdateRxSa(
  uint8   CtrlIdx,
  uint8   An,
  uint64  LowestPn,
  boolean Active)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecUpdateRxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecUpdateRxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                          An,
                                                                          LowestPn,
                                                                          Active);                                      /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_UPDATE_RX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecUpdateRxSa() */

/***********************************************************************************************************************
 *  EthIf_MacSecDeleteRxSa
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecDeleteRxSa(
  uint8 CtrlIdx,
  uint8 An)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecDeleteRxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecDeleteRxSaOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                          An);                                          /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_DELETE_RX_SA, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecDeleteRxSa() */

/***********************************************************************************************************************
 *  EthIf_MacSecGetTxSaNextPn
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecGetTxSaNextPn(
        uint8                               CtrlIdx,
        uint8                               An,
  P2VAR(uint64, AUTOMATIC, ETHIF_APPL_VAR)  NextPnPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Check input pointer for plausibility */
  else if (NextPnPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #410 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #420 Check if driver provides the API */
    if (EthIf_GetMacSecGetTxSaNextPnOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #4210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecGetTxSaNextPnOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx),
                                                                             An,
                                                                             NextPnPtr);                                /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_GET_TX_SA_NEXT_PN, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecGetTxSaNextPn() */

/***********************************************************************************************************************
 *  EthIf_MacSecGetMacSecStats
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecGetMacSecStats(
  uint8 CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecGetMacSecStatsOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecGetMacSecStatsOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx));/* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_GET_MAC_SEC_STATS, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecGetMacSecStats() */

/***********************************************************************************************************************
 *  EthIf_MacSecGetMacSecStatsNotification
 **********************************************************************************************************************/
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
FUNC(void, ETHIF_CODE) EthIf_MacSecGetMacSecStatsNotification(
          uint8                                           CtrlIdx,
  P2CONST(Mka_Stats_SecYType, AUTOMATIC, ETHIF_APPL_DATA) MacSecStatsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Check input pointer for plausibility */
  else if (MacSecStatsPtr == NULL_PTR)
  {
    errorId = ETHIF_E_PARAM_POINTER;
  }
  /* #40 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);

    /* #410 If the transceiver is secured */
    if ( EthIf_IsMacSecSecuredOfEthTrcv(ethTrcvIdx) == TRUE )                                                           /* SBSW_ETHIF_REF_OF_STACK_VAR */
    {
      /* #4110 Retrieve the pae index */
      uint8 paeIdx = EthIf_GetMkaPaeIdxOfEthTrcv(ethTrcvIdx);
      /* #4120 Redirect the call to MKA module */
      Mka_GetMacSecStatisticsNotification(paeIdx, MacSecStatsPtr);                                                      /* SBSW_ETHIF_PUB_FCT_PUB_PARAM_PTR_PASS */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_GET_MAC_SEC_STATS_NOTIFICATION, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif
} /* EthIf_MacSecGetMacSecStatsNotification() */

/***********************************************************************************************************************
 *  EthIf_MacSecOperational
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecOperational(
  uint8   CtrlIdx,
  boolean MacSecOperational
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* #310 Call the internal function to update the transceiver link state after MACsec state reported by MKA */
    EthIf_Internal_MacSecOperational(CtrlIdx, MacSecOperational);

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_OPERATIONAL, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif /* ETHIF_DEV_ERROR_REPORT */

  return retVal;
} /* EthIf_MacSecOperational() */

/***********************************************************************************************************************
 *  EthIf_MacSecSetControlledPortEnabled
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_MacSecSetControlledPortEnabled(
  uint8   CtrlIdx,
  boolean ControlledPortEnabled)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check module initialization state */
  if (EthIf_ModuleInitialized == ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_UNINIT;
  }
  /* #20 Check controller index for plausibility */
  else if (CtrlIdx >= EthIf_GetSizeOfEthIfCtrl())
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  /* #30 Otherwise */
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #310 Map Ethernet controller index to Ethernet Transceiver Index and corresponding API index */
    EthIf_PhysLayerModeElemsIterType physLayerModeElemsIdx = EthIf_GetPhysLayerModeElemsIdxOfEthIfCtrl(CtrlIdx);
    EthIf_EthTrcvIterType ethTrcvIdx = EthIf_GetEthTrcvIdxOfPhysLayerModeElems(physLayerModeElemsIdx);
    EthIf_EthTrcvDrvApiIdxOfEthTrcvType ethTrcvDrvApiIdx = EthIf_GetEthTrcvDrvApiIdxOfEthTrcv(ethTrcvIdx);

    /* #320 Check if driver provides the API */
    if (EthIf_GetMacSecSetControlledPortEnabledOfEthTrcvDrvApi(ethTrcvDrvApiIdx) != NULL_PTR)
    {
      /* #3210 Redirect the call to the addressed Ethernet Transceiver driver */
      retVal = EthIf_GetMacSecSetControlledPortEnabledOfEthTrcvDrvApi(ethTrcvDrvApiIdx)((uint8)EthIf_GetSnvOfEthTrcv(ethTrcvIdx), ControlledPortEnabled); /* SBSW_ETHIF_DRVAPI_FCT_PTR_OPT_CSL03 */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_MACSEC_SET_CONTROLLED_PORT_ENABLED, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* EthIf_MacSecSetControlledPortEnabled() */

#endif /* ETHIF_ENABLE_MACSEC_API */

#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_MacSec.c
 *********************************************************************************************************************/
