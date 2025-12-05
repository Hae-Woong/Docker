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
/*! \ingroup    Dem_Master | Dem_Satellite
 *  \defgroup   Dem_Readout Readout
 *  \addtogroup Dem_Readout
 *  \{
 *  \file       Dem_Readout_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of the Readout Unit used for calculating DataContext specific (PID, DM, DID) readiness
 *              information and writing it to the destination buffer in the correct respective format.
 *  \entity     Readout
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_READOUT_IMPLEMENTATION_H_)
#define DEM_READOUT_IMPLEMENTATION_H_

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
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Readout_Interface.h"
#include "Dem_ReadinessIF_Interface.h"
#include "Dem_Completion_Interface.h"
#include "Dem_Indicator_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Readout_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Readout_IsCalculationForDataContextSupported
 *****************************************************************************/
/*!
 * \brief         Tests if readiness calculation for the requested
 *                DataContext is supported.
 *
 * \details       -
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05, DM26.
 *
 * \return        TRUE
 *                Readiness calculation is supported.
 * \return        FALSE
 *                Readiness calculation is not supported.
 *                Readiness can be derived from configuration.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Readout_IsCalculationForDataContextSupported(
  uint8 DataContext
);

/* ****************************************************************************
 * Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of supported state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of supported state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask of supported state for the corresponding PID01 or
 *                PID41 Data Block
 * \param[out]    BytePos
 *                Byte Position of supported state of the corresponding PID01
 *                or PID41 Data Block
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForPIDs(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDMs
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of support bits for
 *                a Readiness Group in DM05 or DM26 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of support bits for
 *                a Readiness Group in DM05 or DM26 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding DM05 or DM26 Data Block
 * \param[out]    BytePos
 *                Byte Position of the corresponding DM05 or DM26 Data
 *                Block
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDMs(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);


/* ****************************************************************************
 * Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDIDF501
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position for a Readiness Group in 
 *                DID F501 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position for a Readiness Group in 
 *                DID F501 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding DID F501 Data Block
 * \param[out]    BytePos
 *                Byte Position of the corresponding DID F501 Data Block
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDIDF501(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of completion state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of completion state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask of completion state for the corresponding PID01 or
 *                PID41 Data Block
 * \param[out]    BytePos
 *                Byte Position of completion state of the corresponding PID01
 *                or PID41 Data Block
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForPIDs(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDMs
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of status bits for
 *                a Readiness Group in DM05 or DM26 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of status bits for
 *                a Readiness Group in DM05 or DM26 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding DM05 or DM26 Data Block
 * \param[out]    BytePos
 *                Byte Position of the corresponding DM05 or DM26 Data Block
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDMs(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDataContext
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of completion status bits for
 *                a Readiness Group in the specified data context.
 *
 * \details       -
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05, DM26.
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding readiness group in the data context
 * \param[out]    BytePos
 *                Byte Mask for the corresponding readiness group in the data context
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDataContext(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDataContext
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of support status bits for
 *                a Readiness Group in the specified data context.
 *
 * \details       -
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05, DM26.
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding readiness group in the data context
 * \param[out]    BytePos
 *                Byte Mask for the corresponding readiness group in the data context
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDataContext(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) BytePos
);



/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
  % Dem_Readiness_Readout_IsCalculationForDataContextSupported
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Readout_IsCalculationForDataContextSupported(
  uint8 DataContext
)
{
  boolean lIsCalculationSwitchOn;

  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON);
    break;
  case DEM_READINESS_DATACONTEXT_DIDF501:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_PIDF501_CALCULATION == STD_ON);
    break;
  case DEM_READINESS_DATACONTEXT_DM05:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_DM05_CALCULATION == STD_ON);
    break;
  case DEM_READINESS_DATACONTEXT_PID41:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON);
    break;
  case DEM_READINESS_DATACONTEXT_DM26:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_DM26_CALCULATION == STD_ON);
    break;

  default:
    lIsCalculationSwitchOn = FALSE;
    break;
  }
  return lIsCalculationSwitchOn;
}

/* ****************************************************************************
 % Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForPIDs(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_PID_SUPPORT_MISF_MASK;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_MISF_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_FLSYS_MASK;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_FLSYS_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_CMPRCMPT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_CAT_MASK;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_CAT_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_HCCAT_MASK;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_HCCAT_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_HTCAT_MASK;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_HTCAT_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_NOXCAT_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_NOXCAT_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_PID_SUPPORT_EVAP_MASK;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_EVAP_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_PID_SUPPORT_SECAIR_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_SECAIR_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_PID_SUPPORT_BOOSTPR_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_BOOSTPR_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_O2SENS:
    *ByteMask = DEM_READINESS_PID_SUPPORT_O2SENS_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_O2SENS_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_EGSENS_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_SUPPORT_EGSENS_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_O2SENSHT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_O2SENSHT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_PMFLT_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_SUPPORT_PMFLT_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_EGR:
    *ByteMask = DEM_READINESS_PID_SUPPORT_EGR_MASK;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_EGR_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_MASK;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_NONE_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDMs
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDMs(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_DM_SUPPORT_MISF_MASK;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_MISF_BYTE;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_DM_SUPPORT_FLSYS_MASK;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_FLSYS_BYTE;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_DM_SUPPORT_CMPRCMPT_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_CMPRCMPT_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_DM_CAT_MASK;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_CAT_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_DM_HTCAT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_HTCAT_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_DM_EVAP_MASK;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_EVAP_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_DM_SECAIR_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_SECAIR_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_AC:
    *ByteMask = DEM_READINESS_DM_AC_MASK;                                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_AC_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    *ByteMask = DEM_READINESS_DM_EGSENS_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_EGSENS_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGHSENS:
    *ByteMask = DEM_READINESS_DM_EGHSENS_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_EGHSENS_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGR:
  case DEM_CFG_READINESS_VVT:
    *ByteMask = DEM_READINESS_DM_EGRVVT_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_EGRVVT_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CSAS:
    *ByteMask = DEM_READINESS_DM_CSAS_MASK;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_CSAS_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_DM_BOOSTPR_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_BOOSTPR_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    *ByteMask = DEM_READINESS_DM_PMFLT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_PMFLT_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
  case DEM_CFG_READINESS_NOXADSORB:
    *ByteMask = DEM_READINESS_DM_NOX_MASK;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_NOX_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_DM_HCCAT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_SUPPORT_HCCAT_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    /* invalid readiness group */
    *ByteMask = 0u;                                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = 0u;                                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */


/* ****************************************************************************
 % Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDIDF501
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDIDF501(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_MISF_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_MISF_BYTE;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_FLSYS_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_FLSYS_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_CMPRCMPT_MASK;                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_CMPRCMPT_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_ECS:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_ECS_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_ECS_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_CAT_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_CAT_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_HCCAT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_HCCAT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_HTCAT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_HTCAT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_NOXCAT_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_NOXCAT_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_EVAP_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_EVAP_BYTE;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_SECAIR_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_SECAIR_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_BOOSTPR_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_BOOSTPR_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_PMFLT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_PMFLT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_EGSENS_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_EGSENS_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PCV:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_PCV_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_PCV_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_EGR_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_EGR_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_VVT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_VVT_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_VVT_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_DOR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_DOR_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_DOR_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CSER:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_CSER_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_CSER_BYTE;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXADSORB:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_NOXADSORB_MASK;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_NOXADSORB_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_OTHER:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_OTHER_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_OTHER_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    *ByteMask = 0u;                                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = 0u;                                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */


/* ****************************************************************************
 % Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForPIDs(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_PID_COMPLETION_MISF_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_MISF_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:                                                                                                  /* COV_DEM_ROBUSTNESS XX */
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_FLSYS_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_FLSYS_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_CMPRCMPT_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_CMPRCMPT_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_CAT_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_CAT_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_HCCAT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_HCCAT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_HTCAT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_HTCAT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_NOXCAT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_NOXCAT_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_PID_COMPLETION_EVAP_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_EVAP_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_PID_COMPLETION_SECAIR_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_SECAIR_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_PID_COMPLETION_BOOSTPR_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_BOOSTPR_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_O2SENS:
    *ByteMask = DEM_READINESS_PID_COMPLETION_O2SENS_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_O2SENS_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_EGSENS_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_COMPLETION_EGSENS_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_O2SENSHT_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_O2SENSHT_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_PMFLT_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_COMPLETION_PMFLT_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_EGR:
    *ByteMask = DEM_READINESS_PID_COMPLETION_EGR_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_EGR_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_NONE_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDMs
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDMs(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_DM_COMPLETENESS_MISF_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_MISF_BYTE;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:                                                                                                  /* COV_DEM_ROBUSTNESS XX */
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_DM_COMPLETENESS_FLSYS_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_FLSYS_BYTE;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_DM_COMPLETENESS_CMPRCMPT_MASK;                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_CMPRCMPT_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_DM_CAT_MASK;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_CAT_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_DM_HTCAT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_HTCAT_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_DM_EVAP_MASK;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_EVAP_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_DM_SECAIR_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_SECAIR_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_AC:
    *ByteMask = DEM_READINESS_DM_AC_MASK;                                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_AC_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    *ByteMask = DEM_READINESS_DM_EGSENS_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_EGSENS_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGHSENS:
    *ByteMask = DEM_READINESS_DM_EGHSENS_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_EGHSENS_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGR:
  case DEM_CFG_READINESS_VVT:
    *ByteMask = DEM_READINESS_DM_EGRVVT_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_EGRVVT_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CSAS:
    *ByteMask = DEM_READINESS_DM_CSAS_MASK;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_CSAS_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_DM_BOOSTPR_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_BOOSTPR_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    *ByteMask = DEM_READINESS_DM_PMFLT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_PMFLT_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
  case DEM_CFG_READINESS_NOXADSORB:
    *ByteMask = DEM_READINESS_DM_NOX_MASK;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_NOX_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_DM_HCCAT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM_COMPLETENESS_HCCAT_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    /* invalid readiness group */
    *ByteMask = 0u;                                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = 0u;                                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDataContext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDataContext(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) BytePos
)
{
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
  case DEM_READINESS_DATACONTEXT_PID41:
    Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForPIDs(ReadinessGroup, ByteMask, BytePos);                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  case DEM_READINESS_DATACONTEXT_DIDF501:
  {
    uint8 lByteMask = 0u;
    Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDIDF501(ReadinessGroup, &lByteMask, BytePos);                   /* SBSW_DEM_POINTER_FORWARD_STACK */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    /* Not Completed Bits of a Readiness Group in F501 Format are always 4 bits to the left, of the supported bit*/
    *ByteMask = (uint8)((uint8)lByteMask << 4u);                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  break;
  case DEM_READINESS_DATACONTEXT_DM05:
  case DEM_READINESS_DATACONTEXT_DM26:
    Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDMs(ReadinessGroup, ByteMask, BytePos);                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDataContext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDataContext(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) BytePos
)
{
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
  case DEM_READINESS_DATACONTEXT_PID41:
    Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForPIDs(ReadinessGroup, ByteMask, BytePos);                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  case DEM_READINESS_DATACONTEXT_DIDF501:
    Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDIDF501(ReadinessGroup, ByteMask, BytePos);                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  case DEM_READINESS_DATACONTEXT_DM05:
  case DEM_READINESS_DATACONTEXT_DM26:
    Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDMs(ReadinessGroup, ByteMask, BytePos);                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
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
 * \addtogroup Dem_Readout_Properties
 * \{
 */

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
 * \defgroup Dem_Readout_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Readout_WriteReadinessThisDcyForSingleGroup
 *****************************************************************************/
/*!
 * \brief         Calculates the readiness this DCY for a single
 *                readiness group and writes the result into the destination buffer.
 *
 * \details       Calculates the readiness this DCY for a single
 *                readiness group and writes the result into the destination buffer.
 *                This includes support, completion and all reserved bits and
 *                bytes.
 *
 * \param[in]     ReadinessGroupId
 *                Unique indentifier of the readiness group
 * \param[in]     DataContext
 *                Specifies the context of the readiness calculation which 
 *                is needed for correct calculation and formatting inside
 *                the destination buffer.
 * \param[in,out] DestinationBuffer
 *                Buffer into which the result of the readiness calculation is
 *                written.
 * 
 * \pre           DataContext refers PID41 or DM26.
 * \pre           Readiness calculation is supported for the given data context.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessThisDcyForSingleGroup(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC) DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Readout_WriteReadinessThisDcy
 *****************************************************************************/
/*!
 * \brief         Calculates the readiness this DCY
 *                and writes the result into the destination buffer.
 *
 * \details       Calculates the readiness this DCY
 *                and writes the result into the destination buffer.
 *                This includes support, completion and all reserved bits and
 *                bytes.
 *
 * \param[in]     DataContext
 *                Specifies the context of the readiness calculation which 
 *                is needed for correct calculation and formatting inside
 *                the destination buffer.
 * \param[in,out] DestinationBuffer
 *                Buffer into which the result of the readiness calculation is
 *                written.
 * 
 * \pre           DataContext refers PID41 or DM26.
 * \pre           Readiness calculation is supported for the given data context.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessThisDcy(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC) DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Readout_WriteReadinessSinceLastClearForSingleGroup
 *****************************************************************************/
/*!
 * \brief         Calculates the readiness since last clear for a single readiness
 *                group and writes the result into the destination buffer.
 *
 * \details       Calculates the readiness since last clear for a single readiness
 *                group and writes the result into the destination buffer.
 *                This includes support, completion and all reserved bits and
 *                bytes.
 *
 * \param[in]     DataContext
 *                Specifies the context of the readiness calculation which
 *                is needed for correct calculation and formatting inside
 *                the destination buffer.
 * \param[in]     ReadinessGroupId
 *                Unique identifier of the readiness group
 * \param[in,out] DestinationBuffer
 *                Buffer into which the result of the readiness calculation is
 *                written.
 * 
 * \pre           DataContext refers PID01, DM05 or DIDF501.
 * \pre           Readiness calculation is supported for the given data context.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessSinceLastClearForSingleGroup(
  uint8 DataContext,
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Readout_WriteReadinessSinceLastClear
 *****************************************************************************/
/*!
 * \brief         Calculates the readiness since last clear for all readiness
 *                groups and writes the result into the destination buffer.
 *
 * \details       Writes completion and support state for each readiness group
 *                into the destination buffer, formatted according to the respective
 *                data context
 *
 * \param[in]     DataContext
 *                Data Context for 'readiness since last clear' for which the function
 *                is invoked.
 * \param[in,out] DestinationBuffer
 *                Buffer into which the result of the readiness calculation is
 *                written.
 * 
 * \pre           This function may only be called from a data context
 *                refering to readiness since last clear (DIDF501, DM05 or PID01)
 * \pre           Readiness calculation is supported for the given data context.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessSinceLastClear(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Readout_CalculateOBDByteA
 *****************************************************************************/
/*!
 * \brief         Calculates Byte A value for PID 01 and DID F501.
 *
 * \details       Calculates Byte A value for PID 01 and DID F501.
 *
 * \return        Formatted PID01 or DID501 Byte A data
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Readiness_Readout_CalculateOBDByteA(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Readout_SetEngineTypeInByteB
 *****************************************************************************/
/*!
 * \brief         Sets the engine type in Byte B for PID data context.
 *
 * \details       -
 *
 * \param[in,out] DestinationBuffer
 *                Destination buffer
 *
 * \pre           Function is called for PID01 or PID41
 * \pre           Byte B in DestinationBuffer (DestinationBuffer->Buffer[1])
 *                is initialized to 0. No other values have been written yet.
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_SetEngineTypeInByteB(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Readout_SetGroupSupported
 *****************************************************************************/
/*!
 * \brief         Set the readiness group supported in the
 *                given buffer for the respective data context.
 *
 * \details       Set the readiness group supported in the
 *                given buffer for the respective data context.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 * \param[in,out] DestinationBuffer
 *                Destination buffer
 *
 * \pre           DataContext must be one of: PID01, PID41, DID F501, DM05, DM26.
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_SetGroupSupported(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);


/* ****************************************************************************
 * Dem_Readiness_Readout_SetGroupNotCompleted
 *****************************************************************************/
/*!
 * \brief         Set the readiness group not completed in the
 *                given buffer for the respective data context.
 *
 * \details       Set the readiness group not completed in the
 *                given buffer for the respective data context.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 * \param[in,out] DestinationBuffer
 *                Destination buffer
 *
 * \pre           DataContext must be one of: PID01, PID41, DID F501, DM05, DM26.
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_SetGroupNotCompleted(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Readout_Private
 * \{
 */

/* ****************************************************************************
 % Dem_Readiness_Readout_WriteReadinessThisDcyForSingleGroup
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessThisDcyForSingleGroup(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC) DestinationBuffer
)
{
  uint8 lEventInfo = Dem_GlobalDiagnostics_Readiness_CalculateCurrentEventInfo(ReadinessGroupId);
  if ((Dem_GlobalDiagnostics_Readiness_TestGroupDisabled(ReadinessGroupId) == FALSE)
  && ((lEventInfo & DEM_READINESS_INFO_ANYEVENTAVAILABLEMASK) != 0u))
  {
    Dem_Readiness_Readout_SetGroupSupported(ReadinessGroupId, DataContext, DestinationBuffer);                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  if ((Dem_Readiness_Completion_TestGroupSupportsStaticCompletion(ReadinessGroupId, DataContext) == FALSE)
    && (Dem_GlobalDiagnostics_Readiness_TestGroupCompleteThisDcy(lEventInfo, DataContext) == FALSE))
  {
    /* Default static completion can only be changed by a readiness group which is calculated */
    /* Implicitly covers special group fuelsystem: Continuous fuelsystem always statically complete, but will be overwritten
        by non-continuous fuelsystem if calculation is supported */
    Dem_Readiness_Readout_SetGroupNotCompleted(ReadinessGroupId, DataContext, DestinationBuffer);                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}


/* ****************************************************************************
 % Dem_Readiness_Readout_WriteReadinessThisDcy
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessThisDcy(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC) DestinationBuffer
)
{
  Dem_Cfg_ComplexIterType lReadinessGroupIter;
  for (Dem_GlobalDiagnostics_Readiness_GroupIterInit(&lReadinessGroupIter);                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterExists(&lReadinessGroupIter) == TRUE;                                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lReadinessGroupIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventOBDReadinessGroupType lReadinessGroupId = Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId(&lReadinessGroupIter); /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Readiness_Readout_WriteReadinessThisDcyForSingleGroup(lReadinessGroupId, DataContext, DestinationBuffer);                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_WriteReadinessSinceLastClearForSingleGroup
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessSinceLastClearForSingleGroup(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 DataContext,
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lEventInfo = Dem_GlobalDiagnostics_Readiness_CalculateCurrentEventInfo(ReadinessGroupId);
  boolean lAnyEventAvailable = (lEventInfo & DEM_READINESS_INFO_ANYEVENTAVAILABLEMASK) != 0u;
  if (lAnyEventAvailable == TRUE)
  {
    Dem_Readiness_Readout_SetGroupSupported(ReadinessGroupId, DataContext, DestinationBuffer);                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    if ((Dem_Readiness_Completion_TestGroupSupportsStaticCompletion(ReadinessGroupId, DataContext) == FALSE)
      && (Dem_Readiness_Completion_TestGroupCalculatedCompleted(ReadinessGroupId) == FALSE))                                     /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      Dem_Readiness_Readout_SetGroupNotCompleted(ReadinessGroupId, DataContext, DestinationBuffer);                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_WriteReadinessSinceLastClear
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_WriteReadinessSinceLastClear(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  Dem_Cfg_ComplexIterType lReadinessGroupIter;
  for (Dem_GlobalDiagnostics_Readiness_GroupIterInit(&lReadinessGroupIter);                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterExists(&lReadinessGroupIter) == TRUE;                                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lReadinessGroupIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventOBDReadinessGroupType lReadinessGroupId = Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId(&lReadinessGroupIter); /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Readiness_Readout_WriteReadinessSinceLastClearForSingleGroup(DataContext, lReadinessGroupId, DestinationBuffer);         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_CalculateOBDByteA
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
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Readiness_Readout_CalculateOBDByteA(
  void
)
{
  uint8 ByteA;
  uint16 lNumConfirmedEvents = Dem_GlobalDiagnostics_CalculateNumberOfObdRelevantConfirmedDTCs();

  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant())
  {
    /* Number of OBD relevant confirmed events */
    uint8 lGlobalMilState;
    if (lNumConfirmedEvents > 0x7fU)
    {
      lNumConfirmedEvents = 0x7fU;
    }

    /* Get MIL state */
    if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == TRUE)
    {
      lGlobalMilState = 0x80U;
    }
    else
    {
      lGlobalMilState = 0x00U;
    }

    /* Combine into the output variable */
    ByteA = (uint8)((lGlobalMilState | (uint8)lNumConfirmedEvents));
  }
  else
  {
    ByteA = 0x00u;
    }

  return ByteA;
}

/* ****************************************************************************
 % Dem_Readiness_Readout_SetEngineTypeInByteB
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_SetEngineTypeInByteB(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  if ((Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported() == TRUE)
    && (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE))
  {
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID_SUPPORT_COMPRESSION_BYTE, DEM_READINESS_PID_SUPPORT_COMPRESSION_MASK);
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_SetGroupSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_SetGroupSupported(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lSupportedByteMask = 0u;
  uint8 lSupportedBytePos = 0u;
  Dem_Readiness_Readout_ByteFormat_GetSupportMaskAndPositionForDataContext(ReadinessGroup, DataContext, &lSupportedByteMask, &lSupportedBytePos); /* SBSW_DEM_POINTER_FORWARD_STACK */
  if (lSupportedByteMask != 0u)
  {
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, lSupportedBytePos, DestinationBuffer->Buffer[lSupportedBytePos] | lSupportedByteMask); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_SetGroupNotCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Readout_SetGroupNotCompleted(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lCompletionByteMask = 0u;
  uint8 lCompletionBytePos = 0u;
  Dem_Readiness_Readout_ByteFormat_GetCompletionMaskAndPositionForDataContext(ReadinessGroup, DataContext, &lCompletionByteMask, &lCompletionBytePos); /* SBSW_DEM_POINTER_FORWARD_STACK */
  if (lCompletionByteMask != 0u)
  {
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, lCompletionBytePos,
      (DestinationBuffer->Buffer[lCompletionBytePos] | lCompletionByteMask));                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
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
 * \addtogroup Dem_Readout_Public
 * \{
 */

 /* ****************************************************************************
 % Dem_Readiness_Readout_ReadDataOfPID01
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
 *
 *
 */
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfPID01(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_SHARED_DATA)  PID01value,
  uint8 BufferSize
)
{
  Std_ReturnType lReturn;
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    Dem_Data_DestinationBuffer lDestinationBuffer;
    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID01value[0]), BufferSize);                                           /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
    
    /* Set values to static defaults - if calculation is supported the respective values will be overwritten */
    Dem_MemSet(lDestinationBuffer.Buffer, 0u, lDestinationBuffer.BufferSize);                                                    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_DEM_MEMSET_LOCAL_DESTINATIONBUFFER */

    Dem_Data_WriteDestinationBufferAtIndex(&lDestinationBuffer, DEM_READINESS_PID_BYTEA,
      Dem_Readiness_Readout_CalculateOBDByteA());                                                                                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    Dem_Readiness_Readout_SetEngineTypeInByteB(&lDestinationBuffer);                                                             /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    if (Dem_Readiness_Readout_IsCalculationForDataContextSupported(DEM_READINESS_DATACONTEXT_PID01) == TRUE)
    {
      Dem_Readiness_Readout_WriteReadinessSinceLastClear(DEM_READINESS_DATACONTEXT_PID01, &lDestinationBuffer);                  /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    }
    else
    {
      Dem_Readiness_Readout_SetGroupSupported(DEM_CFG_READINESS_CMPRCMPT, DEM_READINESS_DATACONTEXT_PID01,
        &lDestinationBuffer);                                                                                                    /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    }
    lReturn = E_OK;
  }
  else
  {
    lReturn = E_NOT_OK;
  }
  return lReturn;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_Readiness_Readout_ReadDataOfDM05
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfDM05(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)                                                                    /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_MemSet(DestinationBuffer->Buffer, 0u, DestinationBuffer->BufferSize);                                                    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_DEM_MEMSET_LOCAL_DESTINATIONBUFFER */

    if (Dem_Readiness_Readout_IsCalculationForDataContextSupported(DEM_READINESS_DATACONTEXT_DM05) == TRUE)
    {
      Dem_Readiness_Readout_WriteReadinessSinceLastClear(DEM_READINESS_DATACONTEXT_DM05, DestinationBuffer);                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Readiness_Readout_SetGroupSupported(DEM_CFG_READINESS_CMPRCMPT, DEM_READINESS_DATACONTEXT_DM05,
        DestinationBuffer);                                                                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_ReadDataOfDIDF501
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfDIDF501(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  DIDF501value,
  uint8 BufferSize
)
{
  Std_ReturnType lReturn;
  if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_Data_DestinationBuffer lDestinationBuffer;
    
    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(DIDF501value[0]), BufferSize);                                         /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
    Dem_MemSet(lDestinationBuffer.Buffer, 0u, lDestinationBuffer.BufferSize);                                                    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_DEM_MEMSET_LOCAL_DESTINATIONBUFFER */

    Dem_Data_WriteDestinationBufferAtIndex(&lDestinationBuffer, DEM_READINESS_DIDF501_BYTEA,
      Dem_Readiness_Readout_CalculateOBDByteA());                                                                                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    if (Dem_Readiness_Readout_IsCalculationForDataContextSupported(DEM_READINESS_DATACONTEXT_DIDF501) == TRUE)
    {
      Dem_Readiness_Readout_WriteReadinessSinceLastClear(DEM_READINESS_DATACONTEXT_DIDF501, &lDestinationBuffer);                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    }
    else
    {
      Dem_Readiness_Readout_SetGroupSupported(DEM_CFG_READINESS_CMPRCMPT, DEM_READINESS_DATACONTEXT_DIDF501,
        &lDestinationBuffer);                                                                                                    /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    }
    lReturn = E_OK;
  }
  else
  {
    lReturn = E_NOT_OK;
  }
  return lReturn;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_Readiness_Readout_ReadDataOfDM26
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfDM26(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC) DestinationBuffer
)
{
  if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)                                                                    /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_MemSet(DestinationBuffer->Buffer, 0u, DestinationBuffer->BufferSize);                                                    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_DEM_MEMSET_LOCAL_DESTINATIONBUFFER */

    if (Dem_Readiness_Readout_IsCalculationForDataContextSupported(DEM_READINESS_DATACONTEXT_DM26) == TRUE)
    {
      Dem_Readiness_Readout_WriteReadinessThisDcy(DEM_READINESS_DATACONTEXT_DM26, DestinationBuffer);                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Readiness_Readout_SetGroupSupported(DEM_CFG_READINESS_CMPRCMPT, DEM_READINESS_DATACONTEXT_DM26,
        DestinationBuffer);                                                                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Readout_ReadDataOfPID41
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
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfPID41(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID41value,
  uint8 BufferSize
)
{
  Std_ReturnType lReturnValue;
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    Dem_Data_DestinationBuffer lDestinationBuffer;
    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID41value[0]), BufferSize);                                           /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */

    Dem_MemSet(lDestinationBuffer.Buffer, 0u, lDestinationBuffer.BufferSize);                                                    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_DEM_MEMSET_LOCAL_DESTINATIONBUFFER */

    Dem_Readiness_Readout_SetEngineTypeInByteB(&lDestinationBuffer);                                                             /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    if (Dem_Readiness_Readout_IsCalculationForDataContextSupported(DEM_READINESS_DATACONTEXT_PID41) == TRUE)
    {
      Dem_Readiness_Readout_WriteReadinessThisDcy(DEM_READINESS_DATACONTEXT_PID41, &lDestinationBuffer);                         /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    }
    else
    {
      Dem_Readiness_Readout_SetGroupSupported(DEM_CFG_READINESS_CMPRCMPT, DEM_READINESS_DATACONTEXT_PID41,
        &lDestinationBuffer);                                                                                                    /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    }
    lReturnValue = E_OK;
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }
  return lReturnValue;
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_READOUT_IMPLEMENTATION_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Readout_Implementation.h
 *********************************************************************************************************************/
