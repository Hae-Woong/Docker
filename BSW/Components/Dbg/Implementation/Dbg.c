/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dbg.c
 *      Project:  Debugging
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implements partly the functionality of the AUTOSAR 4.x module DBG based on XCP.
 *
 *
 *  
 *********************************************************************************************************************/

#define DBG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dbg.h"

#include "SchM_Dbg.h"

#if(DBG_DEV_ERROR_REPORT == STD_ON)
# include "Det.h" /* Include Development Error Tracer */
#endif

#if(DBG_USE_COMLAYER_XCP == STD_ON)
# include "Xcp.h"
#endif

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
/* SWS_BSW_00036 */
#if((DBG_SW_MAJOR_VERSION != (0x02u)) || (DBG_SW_MINOR_VERSION != (0x00u)) || (DBG_SW_PATCH_VERSION != (0x00u)))
# error "Vendor specific version numbers of Dbg.c and Dbg.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#if(DBG_USE_COMLAYER_XCP == STD_ON) && !defined (Dbg_TransmitData)
/* PRQA S 3453 4 */ /*  MD_MSR_19.7 */
# define Dbg_TransmitData(event)                Xcp_Event(event)
#endif

# define DBG_BEGIN_CRITICAL_SECTION_0() SchM_Enter_Dbg_DBG_EXCLUSIVE_AREA_0()
# define DBG_END_CRITICAL_SECTION_0()   SchM_Exit_Dbg_DBG_EXCLUSIVE_AREA_0()

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#define DBG_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Dbg_Init()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, DBG_CODE) Dbg_Init(void)
{
	Dbg_EnableDidCollection(TRUE);
} /* Dbg_Init */

/**********************************************************************************************************************
 * Dbg_DeInit()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, DBG_CODE) Dbg_DeInit(void)
{
  /* ----- Implementation ---------------------------------------------------------------------- */
	Dbg_EnableDidCollection(FALSE);
} /* Dbg_DeInit */

#if defined (DBG_VERSION_INFO_API) && (DBG_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 * Dbg_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DBG_CODE) Dbg_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DBG_APPL_VAR) VersionInfo)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint8 errorCode = DBG_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------------------------------ */
  /* #10 If the parameter is invalid, indicate an error. */
# if (DBG_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfo == NULL_PTR)
  {
    errorCode = DBG_E_PARAM_POINTER;
  }
  else
# endif
  /* #20 Otherwise, return version info. */
  {
    /* ----- Implementation -------------------------------------------------------------------- */
    VersionInfo->vendorID         = (DBG_VENDOR_ID);
    VersionInfo->moduleID         = (DBG_MODULE_ID);
    VersionInfo->sw_major_version = (uint8)(DBG_SW_MAJOR_VERSION);
    VersionInfo->sw_minor_version = (uint8)(DBG_SW_MINOR_VERSION);
    VersionInfo->sw_patch_version = (uint8)(DBG_SW_PATCH_VERSION);
  }
  /* ----- Development Error Report ------------------------------------------------------------ */
# if (DBG_DEV_ERROR_REPORT == STD_ON)
  if (errorCode != DBG_E_NO_ERROR)
  {
    Dbg_DetReportError(DBG_API_ID_GETVERSIONINFO, errorCode);
  }
# else
  DBG_DUMMY_STATEMENT(errorCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e438 */
# endif
} /* Dbg_GetVersionInfo */
#endif /* (DBG_VERSION_INFO_API) && (DBG_VERSION_INFO_API == STD_ON) */

#define DBG_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:

  MD_Dbg_15.5:    message id 2889
      Reason:     This function has more than one 'return' path.
      Risk:       Minimal risk to missunderstand the code.
      Prevention: Not required.
*/

/* COV_JUSTIFICATION_BEGIN

\ID COV_DBG_MSR_COMPATIBILITY
\ACCEPT TX 
\ACCEPT XF 
\REASON [COV_MSR_COMPATIBILITY]

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Dbg.c
 *********************************************************************************************************************/
