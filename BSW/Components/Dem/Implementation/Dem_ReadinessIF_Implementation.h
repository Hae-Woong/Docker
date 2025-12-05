/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_ReadinessIF
 *  \{
 *  \file       Dem_ReadinessIF_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     ReadinessIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_READINESSIF_IMPLEMENTATION_H)
#define DEM_READINESSIF_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_ReadinessIF_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_Cfg_Declarations.h"
#include "Dem_Core_Interface.h"
#include "Dem_Completion_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Indicator_Interface.h"
#include "Dem_Pto_Interface.h"
                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_READINESSIF_IMPLEMENTATION_FILENAME "Dem_ReadinessIF_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
                                                                                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ReadinessIF_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask
 *****************************************************************************/
/*!
 * \brief         Get readiness this driving cycle disabled mask state
 *
 * \details       Get readiness this driving cycle disabled mask state
 *
 * \return        Value of readiness this driving cycle disabled mask
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask(void);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_IsPID41CalculationSupported
 *****************************************************************************/
/*!
 * \brief         Tests if PID41 calculation is supported.
 *
 * \details       Tests if PID41 calculation is supported.
 *
 * \return        TRUE
 *                PID41 calculation is supported.
 * \return        FALSE
 *                PID41 calculation is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsPID41CalculationSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_IsDM26CalculationSupported
 *****************************************************************************/
/*!
 * \brief         Tests if DM26 calculation is supported.
 *
 * \details       Tests if DM26 calculation is supported.
 *
 * \return        TRUE
 *                DM26 calculation is supported.
 * \return        FALSE
 *                DM26 calculation is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsDM26CalculationSupported(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask(
  void
  )
{
#if (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) || (DEM_FEATURE_NEED_DM26_CALCULATION == STD_ON)
  return Dem_Cfg_AdminData.ReadinessThisDcyDisabledMask;
#else
  return 0;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsPID41CalculationSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsPID41CalculationSupported(
  void
)
{
  return DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsDM26CalculationSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsDM26CalculationSupported(
  void
)
{
  return DEM_FEATURE_NEED_DM26_CALCULATION == STD_ON;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_ReadinessIF_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_TestGroupCompleteThisDcy
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_TestGroupCompleteThisDcy(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 ReadinessGroupInfo,
  uint8 DataContext
)
{
  boolean lComplete = FALSE;
  if (DataContext == DEM_READINESS_DATACONTEXT_PID41)
  {
    lComplete = (((ReadinessGroupInfo & DEM_READINESS_INFO_ANYEVENTAVAILABLEMASK) == 0u)
      || ((ReadinessGroupInfo & DEM_READINESS_INFO_ANYEVENTFAILEDMASK) != 0u)
      || ((ReadinessGroupInfo & DEM_READINESS_INFO_ANYEVENTUNTESTEDMASK) == 0u));
  }
  else if (DataContext == DEM_READINESS_DATACONTEXT_DM26)                                                                        /* COV_DEM_MISRA_BRANCH */
  {
    lComplete = (((ReadinessGroupInfo & DEM_READINESS_INFO_ANYEVENTAVAILABLEMASK) == 0u)
      || ((ReadinessGroupInfo & DEM_READINESS_INFO_ANYEVENTUNTESTEDMASK) == 0u));
  }
  else
  {
    /* unused case */
  }
  return lComplete;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_TestGroupDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_TestGroupDisabled(
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  boolean lReturnValue;
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_MISF_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_FLSYS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_CMPRCMPT) != 0u);
    break;
  case DEM_CFG_READINESS_CAT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_CAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_HCCAT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_HCCAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_HTCAT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_HTCAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_NOXCAT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_NOXCAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_EVAP:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_EVAP_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_SECAIR:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_SECAIR_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_BOOSTPR_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_O2SENS:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_O2SENS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_EGSENS:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_EGSENS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_O2SENSHT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_PMFLT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_PMFLT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_EGR:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_EGR_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_AC:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_AC_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_EGHSENS:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_EGHSENS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_VVT:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_VVT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_CSAS:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_CSAS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_NOXADSORB:
    lReturnValue = (boolean)((Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask() & DEM_READINESS_DISABLED_NOXADSORB_MASK) != 0u);
    break;
  default:
    /* case DEM_CFG_READINESS_NONE: */
    lReturnValue = FALSE;
    break;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_Init(
  void
)
{
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    Dem_Readiness_Completion_TriggerCompletionRecalculationAll();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventOBDReadinessGroupType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_EventIdType, AUTOMATIC) EventId                                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  Dem_EventOBDReadinessGroupType lReadinessGroup = DEM_CFG_READINESS_NONE;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if ( (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON) \
   || (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) \
   || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON))
    if ((Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE) && (EventId != DEM_EVENT_INVALID))
    {
      lReadinessGroup = Dem_Cfg_GetReadinessGroupOfEventTable(EventId);
    }
#endif
  return lReadinessGroup;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_TestReadinessGroupIsHighPriority
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_TestReadinessGroupIsHighPriority(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventOBDReadinessGroupType  ReadinessGroup
)
{
  boolean lIsHighPriorityReadinessGroup = FALSE;

  if (Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported() == TRUE)
  {
    if ((ReadinessGroup == DEM_CFG_READINESS_FLSYS_NONCONT)
      || (ReadinessGroup == DEM_CFG_READINESS_FLSYS)
      || (ReadinessGroup == DEM_CFG_READINESS_MISF))
    {
      lIsHighPriorityReadinessGroup = TRUE;
    }
  }
  return lIsHighPriorityReadinessGroup;
}

 /* ****************************************************************************
  % Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported
  *****************************************************************************/
  /*!
   * Internal comment removed.
 *
 *
   */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_MAJOR_MONITORS == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported(
  void
)
{
  return DEM_CFG_SUPPORT_IGNITION_COMPRESSION == STD_ON;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported(
  void
)
{
  return DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInRdyThisDcy
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInRdyThisDcy(
  void
)
{
  return DEM_CFG_PROCESS_RDY_THIS_DCY_COMPREHENSIVE_COMPLETE == STD_ON;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501(                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  boolean lReturnValue = TRUE;

  if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
  {
    lReturnValue = (boolean)(DEM_CFG_PROCESS_PIDF501_COMPREHENSIVE_COMPLETE == STD_ON);
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_SetRdyThisDcyDisabledMask
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetRdyThisDcyDisabledMask(
  uint32 ReadinessThisDcyDisabledMaskValue
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadinessThisDcyDisabledMaskValue)                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) || (DEM_FEATURE_NEED_DM26_CALCULATION == STD_ON)
    Dem_Cfg_AdminData.ReadinessThisDcyDisabledMask = ReadinessThisDcyDisabledMaskValue;
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ReadinessIF_Private Private Methods
 * \{
 */


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_FindReadinessTableIndexForReadinessGroup
 *****************************************************************************/
/*!
 * \brief         Find a certain readiness group identifier in the readiness group table.
 *
 * \details       Find a certain readiness group identifier in the readiness group table.
 *
 * \param[in]     ReadinessGroup
 *                Readiness group identifier to find in readiness group table
 * 
 * \return        Index of the readiness group in readiness group table
 *
 * \pre           -
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_FindReadinessTableIndexForReadinessGroup(
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_FindReadinessTableIndexForReadinessGroup
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_FindReadinessTableIndexForReadinessGroup(
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  Dem_Cfg_ComplexIterType lReadinessGroupTableIter;
  uint8 lReadinessGroupTableIndex = 0u;

  /* Find the requested record */
  for (Dem_GlobalDiagnostics_Readiness_GroupIterInit(&lReadinessGroupTableIter);                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterExists(&lReadinessGroupTableIter) == TRUE;                                                                /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lReadinessGroupTableIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    if (ReadinessGroup == Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId(&lReadinessGroupTableIter))                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      lReadinessGroupTableIndex = (uint8)lReadinessGroupTableIter.mIdx;
      break;
    }
  }
  return lReadinessGroupTableIndex;
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_ReadinessIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_SetGroupDisabled
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetGroupDisabled(
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  if ( (Dem_GlobalDiagnostics_Readiness_IsPID41CalculationSupported() == TRUE)
    || (Dem_GlobalDiagnostics_Readiness_IsDM26CalculationSupported() == TRUE) )
  {
    uint32 lRdyThisDcyDisabledMask;
    uint32 lSetRdyGroupDisabledMask;

    switch (ReadinessGroup)
    {
    case DEM_CFG_READINESS_MISF:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_MISF_MASK;
      break;
    case DEM_CFG_READINESS_FLSYS:
    case DEM_CFG_READINESS_FLSYS_NONCONT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_FLSYS_MASK;
      break;
    case DEM_CFG_READINESS_CMPRCMPT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_CMPRCMPT;
      break;
    case DEM_CFG_READINESS_CAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_CAT_MASK;
      break;
    case DEM_CFG_READINESS_HCCAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_HCCAT_MASK;
      break;
    case DEM_CFG_READINESS_HTCAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_HTCAT_MASK;
      break;
    case DEM_CFG_READINESS_NOXCAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_NOXCAT_MASK;
      break;
    case DEM_CFG_READINESS_EVAP:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_EVAP_MASK;
      break;
    case DEM_CFG_READINESS_SECAIR:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_SECAIR_MASK;
      break;
    case DEM_CFG_READINESS_BOOSTPR:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_BOOSTPR_MASK;
      break;
    case DEM_CFG_READINESS_O2SENS:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_O2SENS_MASK;
      break;
    case DEM_CFG_READINESS_EGSENS:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_EGSENS_MASK;
      break;
    case DEM_CFG_READINESS_O2SENSHT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_O2SENSHT_MASK;
      break;
    case DEM_CFG_READINESS_PMFLT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_PMFLT_MASK;
      break;
    case DEM_CFG_READINESS_EGR:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_EGR_MASK;
      break;
    case DEM_CFG_READINESS_AC:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_AC_MASK;
      break;
    case DEM_CFG_READINESS_EGHSENS:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_EGHSENS_MASK;
      break;
    case DEM_CFG_READINESS_VVT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_VVT_MASK;
      break;
    case DEM_CFG_READINESS_CSAS:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_CSAS_MASK;
      break;
    case DEM_CFG_READINESS_NOXADSORB:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_NOXADSORB_MASK;
      break;
    default:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_NONE_MASK;
      break;
    }

    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    lRdyThisDcyDisabledMask = Dem_GlobalDiagnostics_Readiness_GetReadinessThisDcyDisabledMask();
    lRdyThisDcyDisabledMask |= lSetRdyGroupDisabledMask;
    Dem_GlobalDiagnostics_Readiness_SetRdyThisDcyDisabledMask(lRdyThisDcyDisabledMask);

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_CompletionRecalculationOnActivationTimerLatch
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CompletionRecalculationOnActivationTimerLatch(
  Dem_EventIdType EventId
  )
{
  Dem_EventOBDReadinessGroupType lReadinessGroup = Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId);
  if (Dem_Readiness_Completion_TestGroupCalculatedCompleted(lReadinessGroup) == TRUE)
  {
    Dem_EventIdType lEventIdIter;
    boolean lConfirmedEventPresent = FALSE;

    /* Iterate over all other events belonging to this group */
    for (lEventIdIter = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
      lEventIdIter <= Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
      lEventIdIter++)
    {
      if ((Dem_Event_TestEventAvailable(lEventIdIter) == TRUE)
        && (EventId != lEventIdIter)
        && (lReadinessGroup == Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(lEventIdIter)))
      {
        if ((Dem_Event_ExtendedStatus_TestBit(Dem_Event_ExtendedStatus_GetEventStatus(lEventIdIter), Dem_ExtendedStatus_QCSLC) == TRUE)
            && (Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(Dem_Core_Event_GetActivationTimerIndex(lEventIdIter)) == FALSE))
        {
          /* Atleast one event has been qualified confirmed since last clear */
          lConfirmedEventPresent = TRUE;
          break;
        }
      }
    }

    /* If no other event is causing completion, reset completion of the group */
    if (lConfirmedEventPresent == FALSE)
    {
      Dem_Readiness_Completion_UpdateCompletionStatus(lReadinessGroup, FALSE);
    }
  }
  else
  {
    /* Completion need not be recalculated since it is not set */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_RecalculateCompletionOnActivationTimerReset
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_RecalculateCompletionOnActivationTimerReset(
  Dem_EventIdType  EventId
  )
{
  Dem_EventOBDReadinessGroupType lReadinessGroup = Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId);
  /* Check event is confirmed(externally visible) and readiness for group is not complete */
  if ( (Dem_Core_TestExternalEventUdsStatusBit(EventId, Dem_UdsStatus_CDTC) == TRUE)
    && (Dem_Readiness_Completion_TestGroupCalculatedCompleted(lReadinessGroup) == FALSE))
  {
    /* Set completion to true since this event causes completion */
    Dem_Readiness_Completion_UpdateCompletionStatus(lReadinessGroup, TRUE);
  }
  else
  {
    /* Otherwise trigger completion recalculation later */
    Dem_Readiness_Completion_TriggerCompletionRecalculation(lReadinessGroup);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_CalculateCurrentEventInfo
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CalculateCurrentEventInfo(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  uint8 lEventInfo = 0u;
  Dem_Cfg_ComplexIterType lEventsOfReadinessGroupIter;
  for (Dem_GlobalDiagnostics_Readiness_EventIterInit(ReadinessGroup, &lEventsOfReadinessGroupIter);                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
    (Dem_Cfg_ComplexIterExists(&lEventsOfReadinessGroupIter) == TRUE);                                                           /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lEventsOfReadinessGroupIter))                                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lEventId = Dem_GlobalDiagnostics_Readiness_EventIterGetEventId(&lEventsOfReadinessGroupIter);                /* SBSW_DEM_CALL_ITERATOR_POINTER */

    /* If event availability is not supported by configuration, Dem_Event_TestEventUnavailable will always be FALSE */
    if (Dem_Event_TestEventUnavailable(lEventId) == FALSE)
    {
      Dem_UDSStatus_StatusType lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(lEventId);
      lEventInfo |= DEM_READINESS_INFO_ANYEVENTAVAILABLEMASK;

      if (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_TFTOC) == TRUE)
      {
        lEventInfo |= DEM_READINESS_INFO_ANYEVENTFAILEDMASK;
      }
      if (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_TNCTOC) == TRUE)
      {
        lEventInfo |= DEM_READINESS_INFO_ANYEVENTUNTESTEDMASK;
      }
    }
  }
  return lEventInfo;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_GroupIterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GroupIterInit(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  IterPtr->mIdx = DEM_READINESS_EVENTTABLE_FIRST_VALID_INDEX;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetSizeOfReadinessGroupTable();                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventOBDReadinessGroupType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId(                                                                             /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetReadinessGroupOfReadinessGroupTable(IterPtr->mIdx);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_EventIterGetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_EventIterGetEventId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetEventTable_ObdRdyGroup2EventIdInd(IterPtr->mIdx);
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_EventIterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_EventIterInit(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  uint8 lReadinessGroupTableIndex = Dem_GlobalDiagnostics_Readiness_FindReadinessTableIndexForReadinessGroup(ReadinessGroupId);
  IterPtr->mIdx =    Dem_Cfg_GetEventTable_ObdRdyGroup2EventIdIndStartIdxOfReadinessGroupTable(lReadinessGroupTableIndex);       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetEventTable_ObdRdyGroup2EventIdIndEndIdxOfReadinessGroupTable(lReadinessGroupTableIndex);         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(lReadinessGroupTableIndex);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_READINESSIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ReadinessIF_Implementation.h
 *********************************************************************************************************************/
