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
/*!        \file     FiM_Int.h
 *         \brief    Header for Autosar 4 module FiM - Component Internal Definitions
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
#if !defined(FIM_INT_H)
#define FIM_INT_H


/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Std_Types.h"
#include "Dem_Types.h"
#include "FiM_Cfg.h"
#include "FiM_Types.h"
#include "FiM_Lcfg.h"
#include "FiM_PBcfg.h"

#include "SchM_FiM.h"


/*! Enter critical section of FiM */
#define FiM_EnterCritical_CrossCore()   SchM_Enter_FiM_FIM_EXCLUSIVE_AREA_0()
#define FiM_EnterCritical_LocalCore()   SchM_Enter_FiM_FIM_EXCLUSIVE_AREA_1()
/*! Leave critical section of FiM */
#define FiM_LeaveCritical_CrossCore()   SchM_Exit_FiM_FIM_EXCLUSIVE_AREA_0()
#define FiM_LeaveCritical_LocalCore()   SchM_Exit_FiM_FIM_EXCLUSIVE_AREA_1()


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/

#if !defined (FIM_LOCAL)                                                                  /* COV_MSR_COMPATIBILITY XF */
/*! Default compiler abstraction for local functions */
# define FIM_LOCAL                 static
#endif

#if !defined (FIM_LOCAL_INLINE)                                                           /* COV_MSR_COMPATIBILITY XF */
/*! Default compiler abstraction for local inline functions */
# define FIM_LOCAL_INLINE          LOCAL_INLINE
#endif

/*! Minimum list length necessary to apply binary search algorithm. */
#if !defined FIM_BINSEARCH_MINLISTLENGTH                                                  /* COV_MSR_COMPATIBILITY TX */
# define FIM_BINSEARCH_MINLISTLENGTH                (5u)
#endif

/*! FiM is in uninitialized state */
#define FIM_UNINITIALIZED                           (0U)
/*! FiM is in initialized state (FiM_Init and FiM_DemInit were executed) */
#define FIM_INITIALIZED                             (1U)
/*! FiM is in pre-initialized state (FiM_Init was executed but not FiM_DemInit) */
#define FIM_PREINITIALIZED                          (2U)

/*! \defgroup  FiM_IncDecCodes  Internal codes for FID increment / decrement
 * Internal codes for FID increment / decrement
 * \{ */
#define FIM_FID_NOCHANGE            (0x00u) /*!< No change */
#define FIM_FID_INC                 (0x01u) /*!< Increment */
#define FIM_FID_DEC                 (0x02u) /*!< Decrement */
/*! \} */


/*! \defgroup  FiM_EventIdTableType  Internal codes which eventId table to use
 * Internal codes which eventId table to use
 * \{ */
#define FIM_EVENTID_TABLE           (0x00u) /*!< Main table for inhibition */
#define FIM_EVENTIDPENDING_TABLE    (0x01u) /*!< Table for pending locks   */
/*! \} */


/* status bits defined by DEM
DEM_MONITOR_STATUS_TF     0x01  bit 0: TestFailed 
DEM_MONITOR_STATUS_TNCTOC 0x02  bit 1: TestNotCompletedThisOperationCycle
DEM_UDS_STATUS_TF         0x01  bit 0: TestFailed 
DEM_UDS_STATUS_TFTOC      0x02  bit 1: TestFailedThisOperationCycle 
DEM_UDS_STATUS_PDTC       0x04  bit 2: PendingDTC 
DEM_UDS_STATUS_CDTC       0x08  bit 3: ConfirmedDTC 
DEM_UDS_STATUS_TNCSLC     0x10  bit 4: TestNotCompletedSinceLastClear 
DEM_UDS_STATUS_TFSLC      0x20  bit 5: TestFailedSinceLastClear 
DEM_UDS_STATUS_TNCTOC     0x40  bit 6: TestNotCompletedThisOperationCycle 
DEM_UDS_STATUS_WIR        0x80  bit 7: WarningIndicatorRequested          */

/*! * \defgroup  DefInhModTable Defines for Inhibition Mode Table
 * Defines for masks, patterns and whether result needs to be inverted that are used in calculation of effects of
 * monitor status changes.
 * \{ */
/*! \defgroup  InhCfgCodeLF Inhibition Config Code "Last Failed"
 * Inhibition configuration inhibits if test failed status bit is set. \n
 * Inhibits if: TF
 * \{ */
#define FIM_LAST_FAILED_MASK                  (Dem_MonitorStatusType)0x01u  /*!<DEM_MONITOR_STATUS_TF */
#define FIM_LAST_FAILED_PATTERN               (Dem_MonitorStatusType)0x01u  /*!<DEM_MONITOR_STATUS_TF set */
#define FIM_LAST_FAILED_INVERTRESULT          (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeTst Inhibition Config Code "Tested"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is NOT set. \n
 * Inhibits if: /TNCTOC
 * \{ */
#define FIM_TESTED_MASK                       (Dem_MonitorStatusType)0x02u  /*!<DEM_MONITOR_STATUS_TNCTOC */
#define FIM_TESTED_PATTERN                    (Dem_MonitorStatusType)0x00u  /*!<DEM_MONITOR_STATUS_TNCTOC not set */
#define FIM_TESTED_INVERTRESULT               (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeNTst Inhibition Config Code "Not Tested"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is set. \n
 * Inhibits if: TNCTOC
 * \{ */
#define FIM_NOT_TESTED_MASK                   (Dem_MonitorStatusType)0x02u  /*!<DEM_MONITOR_STATUS_TNCTOC */
#define FIM_NOT_TESTED_PATTERN                (Dem_MonitorStatusType)0x02u  /*!<DEM_MONITOR_STATUS_TNCTOC set */
#define FIM_NOT_TESTED_INVERTRESULT           (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeTstLF Inhibition Config Code "Tested AND Failed"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is NOT set AND LastFailed status
 * bit is set. \n
 * Inhibits if: /TNCTOC && TF
 * \{ */
#define FIM_TESTED_AND_FAILED_MASK            (Dem_MonitorStatusType)0x03u  /*!<DEM_MONITOR_STATUS_TNCTOC | DEM_MONITOR_STATUS_TF */
#define FIM_TESTED_AND_FAILED_PATTERN         (Dem_MonitorStatusType)0x01u  /*!<DEM_MONITOR_STATUS_TNCTOC not set, DEM_MONITOR_STATUS_TF set */
#define FIM_TESTED_AND_FAILED_INVERTRESULT    (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeNTstOrLF Inhibition Config Code "Not Tested OR Failed" (Autosar extension)
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is set OR LastFailed status
 * bit is NOT set. \n
 * Inhibits if: TNCTOC || TF -> /(/TNCTOC && /TF)
 * \{ */
#define FIM_NOT_TESTED_OR_FAILED_MASK         (Dem_MonitorStatusType)0x03u  /*!<DEM_MONITOR_STATUS_TNCTOC | DEM_MONITOR_STATUS_TF */
#define FIM_NOT_TESTED_OR_FAILED_PATTERN      (Dem_MonitorStatusType)0x00u  /*!<DEM_MONITOR_STATUS_TNCTOC not set, DEM_MONITOR_STATUS_TF not set */
#define FIM_NOT_TESTED_OR_FAILED_INVERTRESULT (boolean)TRUE                 /*!<invert result */
/*! \} */
/*! \} */


/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_VAR_NO_INIT_UNSPECIFIED_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
/*! Pointer to configuration data in case of PB loadable. */
extern P2CONST(FiM_ConfigType, FIM_VAR_NO_INIT, FIM_INIT_DATA)  FiM_ConfigDataPtr;
#endif

#define FIM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/

typedef uint8   FiM_SatelliteIdType;


/*! Struct that holds info for the calculation of a FID depending on the inhibition configuration. */
typedef struct
{
  Dem_MonitorStatusType       Mask;
  Dem_MonitorStatusType       Pattern;
  boolean                     InvertResult;
} FiM_InhibitionModeType;

/*! Typedef for access to event id table. */
typedef uint16 FiM_EventIdTableIndexType;



/* module specific MISRA deviations:
   MD_FiM_2.2:
     Description: Rule 2.2 (2012)
                  There shall be no dead code.
     Reason:      Necessary for API pattern.
     Risk:        None.
     Prevention:  None.

   MD_FiM_2.2conf:
     Description: Rule 2.2 (2012)
                  This initialization is redundant. The value of this object is never used before being modified.
                  This assignment is redundant. The value of this object is never subsequently used.
                  Static function '%s()' is not used within this translation unit.
     Reason:      Dependent on configuration, code parts that modify or use the value may be switched off.
     Risk:        Untested code that's activated by a different configuration.
     Prevention:  Test runs with different configurations so that the expression is evaluated to all possible results.
                  Guaranteed by test coverage analysis.

   MD_FiM_2.3:
     Description: Rule 2.3 (2012)
                  A project should not contain unused type declarations.
     Reason:      Avoid warning about empty translation unit in some configurations.
     Risk:        None.
     Prevention:  None.

   MD_FiM_2981_GetNextSetBit:
     Description: Rule 2.2
                  This initialization is redundant. The value of this object is never used before being modified.
     Reason:      This is a false positive. Depending on the return value of a called function, the initialization value
                  may be returned or not.
     Risk:        None.
     Prevention:  None.

   MD_FiM_2985:
     Description: Rule 2.2
                  This operation is redundant. The value of the result is always that of the left-hand operand.
     Reason:      Dependent on configuration, CSL may optimize identical table entries to defines.
     Risk:        Untested code (e.g., entering a for loop) that's activated by a different configuration.
     Prevention:  Test runs with different configurations so that parameters are used in some of them.

   MD_FiM_3219:
     Description: Rule 14.1
                  Static function '%s()' is not used within this translation unit.
     Reason:      Definition of an inline function is done in header file. However, not all units that include this
                  header file, actually use the inline function.
     Risk:        None.
     Prevention:  None.

   MD_FiM_8.9:
     Description: Rule 8.9 (2012)
                  An object should be defined at block scope if its identifier only appears in a single function.
     Reason:      Vector style guide prevents usage of static variables in function scope and partly
                  object is used in function parts that are not active in certain configurations.
     Risk:        None.
     Prevention:  None.

   MD_FiM_8.3:
     Description: Rule 8.3 (2012)
                  All declarations of an object or function shall use the same names and type qualifiers.
     Reason:      Conflicting prototype is generated by RTE. Currently there is no way to tell RTE the parameter name.
     Risk:        None.
     Prevention:  None.

   MD_FiM_18.1:
     Description: Rule 18.1 (2012)
                  A pointer resulting from arithmetic on a pointer operand shall address an element of the same array
                  as that pointer operand.
     Reason:      Caller must ensure that array contains at least 2 elements.
     Risk:        Caller does not obey this rule.
     Prevention:  Precondition is documented at function declaration. Review is performed.

   MD_FiM_ConfCheck:
     Description: Check always evaluates to same result. This results in MISRA warnings, e.g.
                  - The value of this 'if' controlling expression is always 'false'.
                  - The result of this logical operation is always 'false'.
                  - This code is unreachable.
                  - Controlling expressions shall not be invariant.
     Reason:      Since the configuration is constant during one test run, the expression always results to the same
                  result.
     Risk:        Untested code that's activated by a different configuration.
     Prevention:  Test runs with different configurations so that the expression is evaluated to all possible results.
                  Guaranteed by test coverage analysis.
*/


/* COV_JUSTIFICATION_BEGIN

\ID COV_FIM_DET_DETECT
  \ACCEPT TX
  \REASON For SafeBSW Error Detection is always switched on.

\ID COV_FIM_RUNTIME_CHECKS
  \ACCEPT TX
  \REASON For SafeBSW Runtime Checks are always switched on.

\ID COV_FIM_UNITTEST
  \ACCEPT TX
  \REASON Define is only used to switch off unit's code during unit testing.

COV_JUSTIFICATION_END */ 




/* VCA_JUSTIFICATION_BEGIN

\ID VCA_FIM_SEARCHIDFUNC_PTR
    \DESCRIPTION Possibly calling null pointer. Failed to determine points-to set.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function.

\ID VCA_FIM_SEARCHPENDINGIDFUNC_PTR
    \DESCRIPTION Possibly calling null pointer. Failed to determine points-to set.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed when FIM_FEATURE_OBD == STD_ON.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function when FIM_FEATURE_OBD == STD_ON.
                       Finally, the function containing the pointer access is only present
                       when FIM_FEATURE_OBD == STD_ON

\ID VCA_FIM_PASSINGLOCALPTR_CALL
  \DESCRIPTION  Calling undefined function ...
                A function with pointer parameters is directly called, but the function is not
                defined within the analyzed sources. VCA is unable to determine the
                behavior of the function.
  \COUNTERMEASURE \N  Pointer to local variable of appropriate type is passed.
                      The Compiler ensures the correct type of the pointer.
                      The called function uses the pointer only while being called.

\ID VCA_FIM_PTR_FWD_ARG
  \DESCRIPTION  Calling undefined function ...
                A function with pointer parameters is directly called, but the function is not
                defined within the analyzed sources. VCA is unable to determine the
                behavior of the function.
                A pointer parameter is received as argument and forwarded to another function expecting a valid
                pointer. The called function does not store its pointer parameters.
  \COUNTERMEASURE \N  The pointer is used unmodified, as such it is still valid.
                      The Compiler ensures the correct type of the pointer.
                      The called function uses the pointer only while being called.

VCA_JUSTIFICATION_END */


#endif /* FIM_INT_H */

/***********************************************************************************************************************
*  END OF FILE: FiM_Int.h
***********************************************************************************************************************/
