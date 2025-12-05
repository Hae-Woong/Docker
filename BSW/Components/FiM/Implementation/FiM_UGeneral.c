/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!        \file     FiM_UGeneral.c
 *         \unit     UGeneral
 *         \brief    Source for Autosar 4 module FiM - Unit General
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  
 *  see FiM.h
 *  
 **********************************************************************************************************************/

#define FIM_SOURCE

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Dem.h"
#include "FiM.h"
/* EcuM for ErrorHook */
#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
# include "EcuM_Error.h"
#endif
#include "FiM_Int.h"


#include "FiM_ErrorCheck.h"
#include "FiM_UEvHdl.h"
#include "FiM_UFidHdl.h"
#include "FiM_UGeneral.h"


/***********************************************************************************************************************
*  UNIT LOCAL DATA
***********************************************************************************************************************/

#define FIM_START_SEC_VAR_INIT_8
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Is the FiM master initialized? Don't use directly. Call FiM_UGeneral_GetInitializationStatusMaster() instead. */
VAR(boolean, FIM_VAR_INIT)                      FiM_UGeneral_MasterInitialized = FALSE;

#define FIM_STOP_SEC_VAR_INIT_8
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_VAR_INIT_8_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Is the FiM at least pre-initialized? */
VAR(boolean, FIM_VAR_INIT)                      FiM_UGeneral_PreInitialized = FALSE;
/*! Cumulated initialization status of all FiM Satellites. */
VAR(FiM_InitializationStatusType, FIM_VAR_INIT) FiM_UGeneral_InitializationStatus_SatCumulated = FIM_UNINITIALIZED;/* PRQA S 3218 */ /* MD_FiM_8.9 */

#define FIM_STOP_SEC_VAR_INIT_8_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/



/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/



/***********************************************************************************************************************
*  GLOBAL DATA
***********************************************************************************************************************/

#define FIM_START_SEC_VAR_NO_INIT_UNSPECIFIED_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
/*! Pointer to configuration data in case of PB loadable. */
P2CONST(FiM_ConfigType, FIM_VAR_NO_INIT, FIM_INIT_DATA)              FiM_ConfigDataPtr;
#endif

#define FIM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************/
/* FiM_UGeneral Start **********************************************************************************************/
/**********************************************************************************************************************/

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  private FiM_UGeneral_SetInitializationStatusSat_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to set initialization status of satellite.
 *
 *  \details                  This function sets the initialization status of a satellite.
 *                            If runtime checks are enabled:
 *                              It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  initStatus    Initialization status to be set.
 *  \param[in]  satId         Satellite Id that initialization status should be set for.
 *  \return                   E_OK if initialization status could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE) FiM_UGeneral_SetInitializationStatusSat_Checked
                                                  (FiM_InitializationStatusType initStatus, FiM_SatelliteIdType satId);

/***********************************************************************************************************************
 * private FiM_UGeneral_CalcInitSatusSatCumulated
 **********************************************************************************************************************/
/*! \brief                          Calculate the cumulated initialization status of all satellites.
 *
 *  \details                        see brief
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(void, FIM_CODE) FiM_UGeneral_CalcInitSatusSatCumulated(void);


/***********************************************************************************************************************
 *  FiM_UGeneral_GetNrSatellites
 **********************************************************************************************************************/
/*!
 * \brief                     Return number of configured satellites.
 *
 * \details                   See brief.
 *
 * \return                    Number of satellites
 *
 * \pre                       -
 * \config                    FIM_DEV_ERROR_DETECT == STD_ON
 * \context                   TASK|ISR2
 * \synchronous               TRUE
 * \reentrant                 TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UGeneral_GetNrSatellites(void);


/***********************************************************************************************************************
 *  external public FiM_UGeneral::FiM_InitMemory
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FIM_CODE) FiM_InitMemory(void)
{
  FiM_SatelliteInfoTableIterType  satInfoIter = 0;

  FiM_UGeneral_PreInitialized    = FALSE;
  FiM_UGeneral_MasterInitialized = FALSE;

  for (satInfoIter = 0; satInfoIter < FiM_GetSizeOfSatelliteInfoTable(); satInfoIter++)
  {
    (void)FiM_UGeneral_SetInitializationStatusSat_Checked(FIM_UNINITIALIZED, (FiM_SatelliteIdType)satInfoIter);
  }
  FiM_UGeneral_CalcInitSatusSatCumulated();
}


/***********************************************************************************************************************
 *  external public FiM_UGeneral::FiM_Init
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
FUNC(void, FIM_CODE)
  FiM_Init(P2CONST(FiM_ConfigType, AUTOMATIC, FIM_INIT_DATA) FiMConfigPtr) /*lint -e{438} */ /*lint -e{550} */
{
  /* #10 if PB is enabled: do some checks on passed PB config */
#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
  uint8 errorId = FIM_E_INIT_FAILED;                                              /* PRQA S 2981 */ /* MD_FiM_2.2conf */
  /* #20 if FiMConfigPtr is NULL_PTR */
  if (FiMConfigPtr == NULL_PTR)
  {
    /* #25 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_NULLPTR);
  }
  /* #40 otherwise if there is a mismatch in the magic numbers of the PB config and the code */
  else if (FiMConfigPtr->FinalMagicNumberOfPBConfig != FIM_FINAL_MAGIC_NUMBER)
  {
    /* #45 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  /* #50 otherwise if there is a mismatch in compatibility versions of the generators used for PB config and code */
  else if (FiMConfigPtr->GeneratorCompatibilityVersionOfPBConfig != FIM_CFG_GEN_COMPATIBILITY_VERSION)
  {
    /* #55 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION);
  }
  /* #60 otherwise PB config is fine */
  else
  {
    /* #70 use PB config pointer */
    FiM_ConfigDataPtr = FiMConfigPtr;
    errorId           = FIM_E_NO_ERROR;                                           /* PRQA S 2983 */ /* MD_FiM_2.2conf */
#else
  {
    FIM_DUMMY_STATEMENT(FiMConfigPtr);                                   /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif

    /* #100 determine search algorithm for event id list */
    FiM_UEvHdl_SetSearchEventIdTableFct(FIM_EVENTID_TABLE);
# if (FIM_FEATURE_OBD == STD_ON)
    /* #110 in the case of OBD: determine search algorithm for event id pending list */
    FiM_UEvHdl_SetSearchEventIdTableFct(FIM_EVENTIDPENDING_TABLE);
#endif

    /* #120 initialize FID handling */
    FiM_UFidHdl_Init();

    /* #250 set component state to "pre-initialized" */
    FiM_UGeneral_PreInitialized = TRUE;
    FiM_UGeneral_CalcInitSatusSatCumulated();
  }

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
  /* #300 report error if needed */
  FiM_ErrorCheck_ReportError(FIM_SID_FIM_INIT, errorId);
#endif
}


/***********************************************************************************************************************
 *  external public FiM_UGeneral::FiM_DemInit
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
FUNC(void, FIM_CODE) FiM_DemInit(void)  /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                     /* PRQA S 2981, 2983 */ /* MD_FiM_2.2, MD_FiM_2.2conf */

  /* #10 check whether there's actually only one satellite configured                               */
  /*     if there's more than one satellite use FiM_DemInitMaster and FiM_DemInitSatellite instead  */
  if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetNrSatellites() > 1U) == TRUE) /* PRQA S 2991, 2992, 2995, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (FiM_UGeneral_GetNrSatellites() > 1U) */
    errorId = FIM_E_INIT_FAILED;                          /* PRQA S 2880, 2983 */ /* MD_FiM_ConfCheck, MD_FiM_2.2conf */
  }
  else
  {
    /* #20 call FiM_DemInit<...> for FIM Master and for the first (and only) satellite */
    FiM_DemInitSatellite(FiM_UGeneral_GetApplicationId_SatelliteInfoTable(0));/* PRQA S 2880 */ /* MD_FiM_ConfCheck */
    FiM_DemInitMaster();
    /* #30 there is only one satellite and it's initialized -> we can call FiM_PostInit ourselves */
    FiM_PostInit();
  }

  /* #40 report error if needed */
  FiM_ErrorCheck_ReportError(FIM_SID_FIM_DEMINIT, errorId);
}


/***********************************************************************************************************************
 *  external public FiM_UGeneral::FiM_DemInitSatellite
 **********************************************************************************************************************/
/*!
 * Note: May only be called from the respective satellite partition.
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
FUNC(void, FIM_CODE) FiM_DemInitSatellite(ApplicationType applicationId)  /*lint -e{529} */
{
  uint8                           errorId = FIM_E_NO_ERROR;                                   /* PRQA S 2981 */ /* MD_FiM_2.2 */
  boolean                         found   = FALSE;
  FiM_SatelliteInfoTableIterType  satInfoIter = 0;

  /* #10 identify FiM satellite id from given OS application id */
  for (satInfoIter = 0; satInfoIter < FiM_GetSizeOfSatelliteInfoTable(); satInfoIter++)
  {
    if (FiM_UGeneral_GetApplicationId_SatelliteInfoTable(satInfoIter) == applicationId)
    {
      found = TRUE;
      break;
    }
  }

  /* #20 check if OS application id is known to FiM (otherwise ignore call), see DSGN-FiM23363    */
  /*     possible reasons if id is not found:                                                     */
  /*        - no FiM satellite is configured on the same OS application as calling DEM satellite  */
  /*        - DEM passed an invalid application id                                                */
  if (found == TRUE)
  {
    /* #30 OS application id is known to FiM */
    FiM_SatelliteIdType satId = (FiM_SatelliteIdType)satInfoIter;

    /* #40 check whether FIM is at least pre-initialized */
    if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusSat(satId) == FIM_UNINITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
    {
      /* (FIM_DEV_ERROR_DETECT == STD_ON) && (FiM_UGeneral_GetInitializationStatusSat(satId) == FIM_UNINITIALIZED) */
      errorId = FIM_E_INIT_FAILED;                                                /* PRQA S 2983 */ /* MD_FiM_2.2conf */
    }
    else
    {
      /* #50 re-calculate inhibition states of FIDs if it is initialized */
      FiM_UEvHdl_CalcInhStates(satId);
      /* #60 set component state to "initialized" */
      (void)FiM_UGeneral_SetInitializationStatusSat_Checked(FIM_INITIALIZED, satId);
    }

    /* #80 report error if needed */
    FiM_ErrorCheck_ReportError(FIM_SID_FIM_DEMINITSATELLITE, errorId);
  }
}


/***********************************************************************************************************************
 *  external public FiM_UGeneral::FiM_DemInitMaster
 **********************************************************************************************************************/
/*!
 * Note: May only be called from master partition.
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_DemInitMaster(void)  /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                                           /* PRQA S 2981 */ /* MD_FiM_2.2 */

  /* #10 check whether FIM Master is at least pre-initialized */
  if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusMaster() == FIM_UNINITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    errorId = FIM_E_INIT_FAILED;                                                  /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  else
  {
    /* #30 re-calculate pending lock states of FIDs if it is initialized */
    FiM_UEvHdl_CalcPendStates();
    /* #40 set master's state to "initialized" */
    FiM_UGeneral_MasterInitialized  = TRUE;
  }

  /* #50 report error if needed */
  FiM_ErrorCheck_ReportError(FIM_SID_FIM_DEMINITMASTER, errorId);
}


/***********************************************************************************************************************
 *  external public FiM_UGeneral::FiM_PostInit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_PostInit(void)  /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                                           /* PRQA S 2981 */ /* MD_FiM_2.2 */

  /* #10 calculate overall initialization status of all satellites        */
  /*     (should result in "initialized" if function was called correctly) */
  FiM_UGeneral_CalcInitSatusSatCumulated();

  /* #20 check whether FIM Master and all satellites are initialized */
  if (FiM_ErrorCheck_IsDevErrorDetected((FiM_UGeneral_GetInitializationStatusMaster()        != FIM_INITIALIZED) ||         /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
                                        (FiM_UGeneral_GetInitializationStatusSatCumulated()  != FIM_INITIALIZED)) == TRUE)  /* PRQA S 2992, 2996 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck */
  {
    errorId = FIM_E_INIT_FAILED;                                                  /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }

  /* #30 report error if needed */
  FiM_ErrorCheck_ReportError(FIM_SID_FIM_POSTINIT, errorId);
}


/***********************************************************************************************************************
  *  external public FiM_UGeneral::FiM_MainFunction()
  *********************************************************************************************************************/
/*!
 * Note:  May only be called from the master partition.
 *        While accessing it from a different partition may result in a memory protection exception, that's not relevant
 *        for silence because function only modifies its own data. Therefore no check for current application is done.
 * Note:  May only be called from master partition.
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_MainFunction(void)
{
  uint8               errorId = FIM_E_NO_ERROR;                                       /* PRQA S 2981 */ /* MD_FiM_2.2 */

  /* #020 if FIM satellites are not initialized set corresponding DET code */
  if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusSatCumulated() != FIM_INITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  else
  /* #030 otherwise calculate permission status of FIDs */
  {
    FiM_UFidHdl_CalcPermissionStates();
  }

  /* #100 report error if needed */
  FiM_ErrorCheck_ReportError(FIM_SID_FIM_MAINFUNCTION, errorId);

  return;
}


#if (FIM_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  external public FiM_UGeneral::FiM_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE)  FiM_GetVersionInfo
                       (P2VAR(Std_VersionInfoType, AUTOMATIC, FIM_APPL_DATA) versioninfo)

{
  uint8 errorId = FIM_E_NO_ERROR;

  if (FiM_ErrorCheck_IsDevErrorDetected(versioninfo == NULL_PTR) == TRUE) /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (versioninfo == NULL_PTR) */
    errorId = FIM_E_PARAM_POINTER;
  }
  else
  {
    versioninfo->vendorID         = FIM_VENDOR_ID;                                        /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->moduleID         = FIM_MODULE_ID;                                        /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_major_version = FIM_SW_MAJOR_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_minor_version = FIM_SW_MINOR_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_patch_version = FIM_SW_PATCH_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
  }

  FiM_ErrorCheck_ReportError(FIM_SID_FIM_GETVERSIONINFO, errorId);
}
#endif /* (FIM_VERSION_INFO_API == STD_ON) */



/***********************************************************************************************************************
 *  private FiM_UGeneral_SetInitializationStatusSat_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE)
  FiM_UGeneral_SetInitializationStatusSat_Checked(FiM_InitializationStatusType initStatus, FiM_SatelliteIdType satId)
{
  Std_ReturnType retVal = E_NOT_OK;                                                   /* PRQA S 2981 */ /* MD_FiM_2.2 */
  if (FiM_ErrorCheck_IsRuntimeErrorDetected(satId >= FiM_GetSizeOfSatelliteInfoTable()) == FALSE) /* PRQA S 2991, 2995, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_RUNTIME_CHECKS == STD_OFF) || (satId < FiM_GetSizeOfSatelliteInfoTable()) */
    /* Set initialization state to passed value. */
    *FiM_UGeneral_GetInitStatusPtr_SatelliteInfoTable(satId) = initStatus;            /* SBSW_FIM_ACCESS_INITSTATUSSAT_CHECKED */
    retVal = E_OK;
  }
  return retVal;
}


/***********************************************************************************************************************
 *  private FiM_UGeneral_CalcInitSatusSatCumulated
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
FIM_LOCAL FUNC(void, FIM_CODE) FiM_UGeneral_CalcInitSatusSatCumulated(void)
{
  FiM_SatelliteInfoTableIterType  satInfoIter       = 0;
  FiM_InitializationStatusType    initStatCumulated = FIM_INITIALIZED;

  for (satInfoIter = 0; satInfoIter < FiM_GetSizeOfSatelliteInfoTable(); satInfoIter++)
  {
    FiM_InitializationStatusType  initStatSat
      = FiM_UGeneral_GetInitializationStatusSat((FiM_SatelliteIdType)satInfoIter);
    if (initStatSat == FIM_PREINITIALIZED)
    {
      initStatCumulated = FIM_PREINITIALIZED;
    }
    if (initStatSat == FIM_UNINITIALIZED)
    {
      initStatCumulated = FIM_UNINITIALIZED;
      break;
    }
  }
  FiM_UGeneral_InitializationStatus_SatCumulated = initStatCumulated;
}


/***********************************************************************************************************************
 * FiM_UGeneral_GetNrSatellites
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UGeneral_GetNrSatellites(void)
{
  return FiM_GetSizeOfSatelliteInfoTable();
}


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************/
/* FiM_UGeneral End ************************************************************************************************/
/**********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_FIM_COMPSWAPFUNC_ADDRESS_PTR
    \DESCRIPTION    Pointer write access to parameter addressPtr.
    \COUNTERMEASURE \N Compare and Swap function is internal and caller makes sure that a
                       valid pointer 'addressPtr' is passed.

\ID SBSW_FIM_BITINDEX_ADDRESS_PTR
    \DESCRIPTION    Pointer write access to parameter bitIndexPtr.
    \COUNTERMEASURE \N Function is internal and caller makes sure that a valid pointer 'bitIndexPtr' is passed.

\ID SBSW_FIM_BITMANIP_COMPSWAPFUNC_CALL
    \DESCRIPTION    Call of CompareAndSwap with pointer parameter.
    \COUNTERMEASURE \R A Runtime check ensures that the passed pointer is within array bounds.
                       Bit manipulation functions are internal and caller makes sure that a
                       valid pointer to word array is passed and that 'bitfieldWordArrSize'
                       does not exceed length of this word array.

\ID SBSW_FIM_GETNEXTSETBIT_CALL
    \DESCRIPTION Call of FiM_UBitfieldHdl_GetNextSetBit/..._GetNextSetBitInWord with pointer parameter.
    \COUNTERMEASURE \N Pointer to local variable of correct type uint32/uint8 is passed.

\ID SBSW_FIM_PTR_FWD_ARG
    \DESCRIPTION    A pointer parameter is received as argument and forwarded to another function expecting a valid
                    pointer. The called function does not store its pointer parameters.
    \COUNTERMEASURE \N The pointer is used unmodified, as such it is still valid.

\ID SBSW_FIM_EVCHANGED_ACCESS
    \DESCRIPTION    Access of EventChangedTable using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_PERMSTATUS_ACCESS
    \DESCRIPTION    Access of PermStatusTable using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_BITMANIP_CALL
    \DESCRIPTION    Call of bit manipulation function with pointer to array and length of array as parameters.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_BITTEST_CALL
    \DESCRIPTION    Call of bit test function with pointer to array.
    \COUNTERMEASURE \N Bit test function is internal and pointer is only used for read access.

\ID SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR
    \DESCRIPTION    Pointer write access to out parameter evTableIdx.
    \COUNTERMEASURE \N Search functions are internal and pointer passed by caller always points to a local (stack)
                       variable. See also SBSW_FIM_SEARCHIDFUNC_PTR.

\ID SBSW_FIM_SEARCHIDFUNC_PTR
    \DESCRIPTION Call of function via function pointer.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function.

\ID SBSW_FIM_SEARCHPENDINGIDFUNC_PTR
    \DESCRIPTION Call of function via function pointer.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed when FIM_FEATURE_OBD == STD_ON.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function when FIM_FEATURE_OBD == STD_ON.
                       Finally, the function containing the pointer access is only present
                       when FIM_FEATURE_OBD == STD_ON

\ID SBSW_FIM_DEMGETMONST_CALL
    \DESCRIPTION Call of Dem_GetMonitorStatus with pointer parameter.
    \COUNTERMEASURE \N Pointer to local variable of type Dem_MonitorStatusType is passed.

\ID SBSW_FIM_DEMGETEVENTUDSSTATUS_CALL
    \DESCRIPTION Call of Dem_GetEventUdsStatus with pointer parameter.
    \COUNTERMEASURE \N Pointer to local variable of type Dem_UdsStatusByteType is passed.

\ID SBSW_FIM_ACCESS_INITSTATUSSAT_CHECKED
    \DESCRIPTION    Access to Initialization Status variable via pointer in satellite info table.
                    Countermeasure CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS for ensuring that pointer is taken from table.
                    Countermeasure SMI-159210 for ensuring that this pointer points to a valid address.
                    Countermeasure SMI-232888 for ensuring that satellite does not overwrite another satellite's data:
                    The information in one line of FiM_SatelliteInfoTable belongs to the same satellite and all lines
                    in the table belong to different satellites.
                    Countermeasure SMI-438945 together with memory mapping rules in Technical Reference ensures that
                    initialization status bits of satellites are not overwritten by non-trusted parts of software.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]
                    \S SMI-159210
                    \S SMI-232888
                    \S SMI-438945

\ID SBSW_FIM_BITMANIP_INHSTATUSSAT_CALL
    \DESCRIPTION    Call of bit manipulation function with pointer to array and length of array as parameters.
                    Countermeasure CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS for ensuring that pointer and length are taken
                    from table.
                    Countermeasure SMI-159209 for ensuring that this pointer points to a valid address and
                    the length is the length of this array.
                    Countermeasure SMI-232888 for ensuring that satellite does not overwrite another satellite's data:
                    The information in one line of FiM_SatelliteInfoTable belongs to the same satellite and all lines
                    in the table belong to different satellites.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]
                    \S SMI-159209
                    \S SMI-232888

\ID SBSW_FIM_ACCESS_INHSTATUSSAT
    \DESCRIPTION    Access to Inhibition Status Array via pointer in satellite info table.
                    Countermeasure CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS for ensuring that pointer and length are taken
                    from table.
                    Countermeasure SMI-159209 for ensuring that this pointer points to the start of an array and
                    the length is the length of this array.
                    Countermeasure SMI-232888 for ensuring that satellite does not overwrite another satellite's data:
                    The information in one line of FiM_SatelliteInfoTable belongs to the same satellite and all lines
                    in the table belong to different satellites.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]
                    \S SMI-159209
                    \S SMI-232888

\ID SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED
    \DESCRIPTION    Access of Fim_FidPendingCounter using ComStackLib.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]

\ID SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CALCINHPENDSTATES
    \DESCRIPTION    Access of Fim_FidPendingCounter using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_VERSIONINFO_PTR
    \DESCRIPTION Pointer write access to versioninfo pointer variable in FiM_GetVersionInfo().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to the parameter
                       versioninfo is passed.

\ID SBSW_FIM_PERMISSION_PTR
    \DESCRIPTION Pointer write access to permission pointer variable in FiM_GetFunctionPermission().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to a variable
                       that can hold a boolean type is passed.

\ID SBSW_FIM_PENDINGSTATUS_PTR
    \DESCRIPTION Pointer write access to pending status pointer variable in FiM_GetFunctionPendingStatus().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to a variable
                       that can hold a boolean type is passed.

SBSW_JUSTIFICATION_END */

/*

\CM CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS
       Verify that FIM_DEV_RUNTIME_CHECKS == STD_ON (CheckQMDefines).
       Then it's Qualified use case CSL01 of ComStackLib.

*/


/***********************************************************************************************************************
*  END OF FILE: FiM_UGeneral.c
***********************************************************************************************************************/
