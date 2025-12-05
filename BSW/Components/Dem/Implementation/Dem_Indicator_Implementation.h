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
/*! \addtogroup Dem_Indicator
 *  \{
 *  \file       Dem_Indicator_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     Indicator
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_INDICATOR_IMPLEMENTATION_H)
#define DEM_INDICATOR_IMPLEMENTATION_H

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

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Indicator_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_OdometerData_Interface.h"
#include "Dem_EngineRuntime_Interface.h"
#include "Dem_Core_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_INDICATOR_IMPLEMENTATION_FILENAME "Dem_Indicator_Implementation.h"

/*!
 * \defgroup  Dem_Indicator_LampStates Macros encoding J1939 lamp states and lamp flash states
 * \{
 */
#define DEM_J1939_LAMP_MIL_BITINDEX       (6u)     /*!< Bitmask for MIL indicator */
#define DEM_J1939_LAMP_RSL_BITINDEX       (4u)     /*!< Bitmask for RSL indicator */
#define DEM_J1939_LAMP_AWL_BITINDEX       (2u)     /*!< Bitmask for AWL indicator */
#define DEM_J1939_LAMP_PL_BITINDEX        (0u)     /*!< Bitmask for PL indicator */

#define DEM_J1939_LAMP_OFF                (0x00U)    /*!< Indicator off */
#define DEM_J1939_LAMP_ON                 (0x01U)    /*!< Indicator on */
#define DEM_J1939_LAMP_NA                 (0x03U)    /*!< Indicator not configured */
#define DEM_J1939_LAMP_ALL_NA              (0xFFU)  /*!< All Indicators not configured */

#define DEM_J1939_LAMP_FLASH_FAST_FLASH    (0x01U)  /*!< Indicator fast flashing */
#define DEM_J1939_LAMP_FLASH_SLOW_FLASH    (0x00U)  /*!< Indicator slow flashing */
#define DEM_J1939_LAMP_FLASH_NO_FLASH      (0x03U)  /*!< Indicator not flashing (either not configured, off or continuous) */
#define DEM_J1939_LAMP_FLASH_ALL_NO_FLASH  (0xFFU)  /*!< All Indicators not flashing (either not configured, off or continuous) */

#define DEM_J1939_LAMP_INVALID_BITINDEX    (0xFFU)
/*!
 * \}
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 /* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Indicator_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable
 *****************************************************************************/
/*!
 * \brief         Get the indicator ID of an indicator
 *
 * \details       Get the indicator ID for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table
 *
 * \return        Indicator ID
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
  uint16 Index
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex
 *****************************************************************************/
/*!
 * \brief         Get the event counter index from event indicator iterator
 *
 * \details       Get the index in IndicatorTable, which is the same as in 
 *                IndicatorEventCounters table, the iterator is pointing to
 *
 * \param[in]     IterPtr
 *                
 * \param[out]    IndicatorIndex
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_IndicatorIndexType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                           /* PRQA S 3206 */ /* MD_DEM_3206 */
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetGlobalPlIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Protection Lamp'
 *
 * \details       Get the unique handle of indicator 'Protection Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalPlIndicatorId(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetGlobalAwlIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Amber Warning Lamp'
 *
 * \details       Get the unique handle of indicator 'Amber Warning Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalAwlIndicatorId(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetGlobalRslIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Red Stop Lamp'
 *
 * \details       Get the unique handle of indicator 'Red Stop Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalRslIndicatorId(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable
 *****************************************************************************/
/*!
 * \brief         Get the behavior of an indicator
 *
 * \details       Get the behavior for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table
 *
 * \return        Indicator behavior
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable(
  uint16 Index
);

 /* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled
 *****************************************************************************/
 /*!
 * \brief         Tests if indicators are supported.
 *
 * \details       Tests if indicators are supported.
 *
 * \return        TRUE
 *                Indicators are supported.
 * \return        FALSE
 *                Indicators are not supported.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters
 *****************************************************************************/
/*!
 * \brief         Get the current event counter for indicator
 *
 * \details       Read the stored value from IndicatorEventCounters table
 *
 * \param[in]     Index in IndicatorEventCounters table
 *
 * \return        Value at given index
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters(
  uint16 Index
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IncrementIndicatorEventCounter
 *****************************************************************************/
/*!
 * \brief         Increment the event counter for an indicator
 *
 * \details       Increment the event counter for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table and 
 *                IndicatorEventCounter table
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IncrementIndicatorEventCounter(
  uint16 Index
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_DecrementIndicatorEventCounter
 *****************************************************************************/
/*!
 * \brief         Decrement the event counter for an indicator
 *
 * \details       Decrement the event counter for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table and 
 *                IndicatorEventCounter table
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_DecrementIndicatorEventCounter(
  uint16 Index
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorCounterTable
 *****************************************************************************/
/*!
 * \brief         Get the indicator ID of an indicator
 *
 * \details       Get the indicator ID for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator counter table
 *
 * \return        Indicator ID
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorCounterTable(
  Dem_Indicator_IndicatorCounterTableIndexType Index
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetNodeIndexOfIndicatorTable
 *****************************************************************************/
/*!
 * \brief         Get the J1939 node of an indicator
 *
 * \details       Get the index of the J1939 node for which the indicator at 
 *                given index is configured
 *
 * \param[in]     Index of indicator in Indicator table
 *
 * \return        Index in J1939NodeId array
 *
 * \pre           -           
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetNodeIndexOfIndicatorTable(
  uint16_least Index
);

 /* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
  uint16 Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetIdOfIndicatorTable(Index);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_IndicatorIndexType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                           /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (Dem_Cfg_IndicatorIndexType)Dem_Cfg_GetIndicatorTableInd(IterPtr->mIdx);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetGlobalAwlIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalAwlIndicatorId(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{
# if (DEM_CFG_SUPPORT_AWL == STD_ON)
  return DEM_CFG_GLOBAL_AWL_INDICATOR;
# else
  return DEM_CFG_GLOBAL_INDICATOR_COUNT;
# endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetGlobalPlIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalPlIndicatorId(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{
# if (DEM_CFG_SUPPORT_PL == STD_ON)
  return DEM_CFG_GLOBAL_PL_INDICATOR;
# else
  return DEM_CFG_GLOBAL_INDICATOR_COUNT;
# endif
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetGlobalRslIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalRslIndicatorId(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{
# if (DEM_CFG_SUPPORT_RSL == STD_ON)
  return DEM_CFG_GLOBAL_RSL_INDICATOR;
# else
  return DEM_CFG_GLOBAL_INDICATOR_COUNT;
# endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable(
  uint16 Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetBehaviorOfIndicatorTable(Index);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled(
  void
)
{
  return (DEM_CFG_SUPPORT_INDICATORS == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters(
  uint16 Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetIndicatorEventCounters(Index);
}

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IncrementIndicatorEventCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IncrementIndicatorEventCounter(
  uint16 Index
  )
{
  if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
    && (Index >= Dem_Cfg_GetSizeOfIndicatorEventCounters()))                                                                                     
  {
    Dem_Error_RunTimeCheckFailed(DEM_INDICATOR_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
    Dem_Cfg_IncIndicatorEventCounters(Index);                                                                                    /* SBSW_DEM_ARRAY_WRITE_INDICATOREVENTCOUNTERS */
  }
}

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_DecrementIndicatorEventCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_DecrementIndicatorEventCounter(
  uint16 Index
  )
{
  if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
    && (Index >= Dem_Cfg_GetSizeOfIndicatorEventCounters()))                                                                                     
  {
    Dem_Error_RunTimeCheckFailed(DEM_INDICATOR_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
    Dem_Cfg_DecIndicatorEventCounters(Index);                                                                                    /* SBSW_DEM_ARRAY_WRITE_INDICATOREVENTCOUNTERS */
  }
}

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorCounterTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorCounterTable(
  Dem_Indicator_IndicatorCounterTableIndexType Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetIndicatorIdOfIndicatorCounterTable(Index);
}

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetNodeIndexOfIndicatorTable
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetNodeIndexOfIndicatorTable(
  uint16_least Index
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
  return Dem_Cfg_FindJ1939NodeIndexFromNmNodeId(Dem_Cfg_GetJ1939NodeIdOfIndicatorTable(Index));
#else
  return Dem_Cfg_GetInvalidJ1939NodeIndex();
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Indicator_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  uint8 lResult;

#if (DEM_CFG_SUPPORT_MIL == STD_ON)
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    lResult = DEM_CFG_GLOBAL_MIL_INDICATOR;
  }
  else
#endif
  {
    lResult = DEM_CFG_GLOBAL_INDICATOR_COUNT;
  }
  return lResult;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IsGlobalMilStateSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsGlobalMilStateSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IsIndicatorIdValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsIndicatorIdValid(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 IndicatorId
  )
{
  boolean lIsIndicatorIdValid = FALSE;
  if ( Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled() 
   && (IndicatorId < DEM_CFG_GLOBAL_INDICATOR_COUNT))
  {
    lIsIndicatorIdValid = TRUE;
  }
  return lIsIndicatorIdValid;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Indicator_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_TestGlobalSpecialIndicatorSupportedByNode
 *****************************************************************************/
/*!
 * \brief         Tests whether an indicator is supported by the given node.
 *
 * \details       -
 *
 * \param[in]     NodeIndex
 *                Handle to a Dem J1939 node
 * \param[in]     IndicatorId
 *                Index of the indicator to check
 *
 * \return        TRUE
 *                At least one event assigned to the node has the indicator
 *                configured.
 * \return        FALSE
 *                The indicator is not configured for any event assigned to
 *                the node.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_TestGlobalSpecialIndicatorSupportedByNode(
  uint8 NodeIndex,
  uint8 IndicatorId
  );


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus
 *****************************************************************************/
/*!
 * \brief         Calculates the node dependent (flash) lamp status of an indicator.
 *
 * \details       Calculates the node dependent (flash) lamp status of an indicator
 *                and stores the resulting status at the correct bit position
 *                in the lamp status struct passed as parameter.
 *
 * \param[in]     NodeIndex
 *                Handle to a Dem J1939 node
 * \param[in]     IndicatorId
 *                Index of the indicator to calculate the status of
 * \param[in,out] LampStatusToUpdate
 *                in:  Lamp status struct with the bits representing the lamp
 *                     status of the given indicator id as 0b11 (N/A)
 *                out: Lamp status struct with the bits representing the lamp
 *                     status of the given indicator set to the calculated
 *                     value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus(
  uint8 NodeIndex,
  uint8 IndicatorId,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, AUTOMATIC) LampStatusToUpdate
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_GetLampFlashStatusFromIndicatorState
 *****************************************************************************/
/*!
 * \brief         Maps indicator behavior to J1939 flash lamp status
 *
 * \details       This function maps the internal indicator behavior to the
 *                external flash lamp bit mask used by J1939.
 *
 *                The J1939 representation is not bit-shifted for a specific
 *                indicator. To use the returned value in DM messages the
 *                caller needs to shift the result to the correct position
 *                first.
 *
 *                The state map only includes states used by J1939 indicators,
 *                other indicators states will not work with this function.
 *
 * \param[in]     IndicatorState
 *                The state of a J1939 related indicator.
 *
 * \return        J1939 flash amp status of the passed indicator state.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_GetLampFlashStatusFromIndicatorState(
  Dem_IndicatorStatusType IndicatorState
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_GetLampStatusFromIndicatorState
 *****************************************************************************/
/*!
 * \brief         Maps indicator behavior to J1939 lamp status
 *
 * \details       This function maps the internal indicator behavior to the
 *                external lamp status bit mask used by J1939.
 *
 *                The J1939 representation is not bit-shifted for a specific
 *                indicator. To use the returned value in DM messages the
 *                caller needs to shift the result to the correct position
 *                first.
 *
 *                The state map only includes states used by J1939 indicators,
 *                other indicators states will not work with this function.
 *
 * \param[in]     IndicatorState
 *                The state of a J1939 related indicator.
 *
 * \return        J1939 lamp status of the passed indicator state.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_GetLampStatusFromIndicatorState(
  Dem_IndicatorStatusType IndicatorState
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IsSpecialIndicator
 *****************************************************************************/
/*!
 * \brief         Decide whether the indicator Id belongs to a special indicator
 *
 * \details       Test whether the indicator Id belongs to 
 *                - Malfunction Indicator Lamp
 *                - Red stop lamp
 *                - Amber Warning lamp
 *                - Protect lamp
 *
 * \param[in]     Indicator ID
 *                Must be in range [0, DEM_CFG_GLOBAL_INDICATOR_COUNT[
 *
 * \return        TRUE: The indicator Id denotes a special indicator
 *                FALSE: The indicator Id denotes no special indicator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsSpecialIndicator(
  uint8 IndicatorId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_GetBitIdexInLampStatusByte
 *****************************************************************************/
/*!
 * \brief         Returns the bit index for the bits representing the given
 *                indicator in the J1939 lamp status byte.
 *
 * \details       Returns the bit index for the bits representing the given
 *                indicator in the J1939 lamp status byte.
 *
 * \param[in]     IndicatorId
 *                Unique handle of the user defined indicator
 *
 * \return        Bit index for the respective indicator in the lamp status
 *                byte or invalid bit index, if indicator is not a special 
 *                indicator
 *
 * \pre           -
 * 
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_GetBitIdexInLampStatusByte(
  uint8 IndicatorId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_UpdateLampStatusForIndicator
 *****************************************************************************/
/*!
 * \brief         Calculate J1939 LampStatus from indicator specific lamp status
 *
 * \details       Integrates the indicator specific lamp status and
 *                flash lamp status into the J1939 LampStatus
 *
 * \param[in]     IndicatorId
 *                Unique handle of the user defined indicator
 * \param[in]     LampStatusOfIndicator
 *                Indicator specific lamp status to set
 * \param[in]     FlashLampStatusOfIndicator
 *                Indicator specific lamp status to set
 * \param[in,out] LampStatusToUpdate
 *                This lamp status is updated for the current indicator id
 * 
 * \pre           IndicatorId must be a special indicator
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_UpdateLampStatusForIndicator(
  uint8 IndicatorId,
  Dem_IndicatorStatusType LampStatusOfIndicator,
  Dem_IndicatorStatusType FlashLampStatusOfIndicator,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, AUTOMATIC) LampStatusToUpdate
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetCounterIndexFromId
 *****************************************************************************/
/*!
 * \brief         Get index in indicator counter table 
 *
 * \details       Get index in indicator counter table for given indicator Id
 *
 * \param[in]     Indicator Id
 *
 * \param[in,out] CounterIndex
 *                Index in IndicatorCounter table referring to this indicator Id
 *
 * \return        TRUE
 *                Indicator Id was found in IndicatorCounter table
 *                
 * \return        FALSE
 *                No entry was found in IndicatorCounter table for this 
 *                Indicator Id
 *                
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetCounterIndexFromId(
  uint8 IndicatorId,
  CONSTP2VAR(Dem_Indicator_IndicatorCounterTableIndexType, AUTOMATIC, AUTOMATIC) CounterIndex
  );


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IndicatorCounterIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate special indicator descriptors
 *                configured for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IndicatorCounterIterInit(
  uint8 IndicatorId,                                                                                                             /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                             /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IsMilStateActive
 *****************************************************************************/
/*!
 * \brief         Decide whether Mil state is considered as active
 *
 * \details       Decide whether a given Mil state is considered as active
 *                according to configuration option
 *
 * \param[in]     MilState
 *                
 * \param[out]    TRUE: Mil state is considered as active
 *                FALSE: Mil state is considered as inactive
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsMilStateActive(
  Dem_IndicatorStatusType MilState
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IncrementAllCountersAssignedToEvent
 *****************************************************************************/
/*!
 * \brief        Increment all counters assigned to the event
 *
 * \details      -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IncrementAllCountersAssignedToEvent(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_ResetAllEventCounters
 *****************************************************************************/
/*!
 * \brief         Reset all event counters
 *
 * \details       Reset all event counters
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_ResetAllEventCounters(void);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_TestGlobalSpecialIndicatorSupportedByNode
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_TestGlobalSpecialIndicatorSupportedByNode(
  uint8 NodeIndex,                                                                                                               /* PRQA S 3206 */ /* MD_DEM_3206 */
  uint8 IndicatorId
  )
{
  boolean lSpecialIndicatorIsSupported = FALSE;
  uint8 lIndicatorBitIndex = Dem_GlobalDiagnostics_Indicator_J1939_GetBitIdexInLampStatusByte(IndicatorId);
  if (lIndicatorBitIndex != DEM_J1939_LAMP_INVALID_BITINDEX)                                                                     /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_Cfg_J1939NodeIndicatorAvailabilityType lIndicatorAvailability = Dem_Cfg_GetJ1939NodeIndicatorAvailability(NodeIndex);
    lSpecialIndicatorIsSupported = (((lIndicatorAvailability >> lIndicatorBitIndex) & DEM_J1939_LAMP_NA) != DEM_J1939_LAMP_NA);
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(NodeIndex)                                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lSpecialIndicatorIsSupported;
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus
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
Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus(
  uint8 NodeIndex,
  uint8 IndicatorId,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, AUTOMATIC) LampStatusToUpdate
)
{
  if (Dem_GlobalDiagnostics_Indicator_J1939_TestGlobalSpecialIndicatorSupportedByNode(NodeIndex, IndicatorId) == TRUE)
  {
    Dem_IndicatorStatusType lIndicatorState = Dem_GlobalDiagnostics_Indicator_GetIndicatorState(IndicatorId, NodeIndex, FALSE);
    Dem_IndicatorStatusType lCurrentIndicatorLampStatus = Dem_GlobalDiagnostics_Indicator_J1939_GetLampStatusFromIndicatorState(lIndicatorState);
    Dem_IndicatorStatusType lCurrentIndicatorLampFlashStatus = Dem_GlobalDiagnostics_Indicator_J1939_GetLampFlashStatusFromIndicatorState(lIndicatorState);

    Dem_GlobalDiagnostics_Indicator_J1939_UpdateLampStatusForIndicator(IndicatorId, lCurrentIndicatorLampStatus,
      lCurrentIndicatorLampFlashStatus, LampStatusToUpdate);                                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_GetLampFlashStatusFromIndicatorState
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
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_GetLampFlashStatusFromIndicatorState(
  Dem_IndicatorStatusType IndicatorState
  )
{
  Dem_IndicatorStatusType lReturnValue;
  switch (IndicatorState)
  {
  case DEM_INDICATOR_OFF:
    lReturnValue = DEM_J1939_LAMP_FLASH_NO_FLASH;
    break;
  case DEM_INDICATOR_FAST_FLASH:
    lReturnValue = DEM_J1939_LAMP_FLASH_FAST_FLASH;
    break;
  case DEM_INDICATOR_SLOW_FLASH:
    lReturnValue = DEM_J1939_LAMP_FLASH_SLOW_FLASH;
    break;
  case DEM_INDICATOR_CONTINUOUS:
    lReturnValue = DEM_J1939_LAMP_FLASH_NO_FLASH;
    break;
  default:                                                                                                                       /* COV_DEM_MISRA_BRANCH */
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = DEM_J1939_LAMP_FLASH_NO_FLASH;
    break;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_GetLampStatusFromIndicatorState
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
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_GetLampStatusFromIndicatorState(
  Dem_IndicatorStatusType IndicatorState
  )
{
  Dem_IndicatorStatusType lReturnValue;
  switch (IndicatorState)
  {
  case DEM_INDICATOR_OFF:
    lReturnValue = DEM_J1939_LAMP_OFF;
    break;
  case DEM_INDICATOR_FAST_FLASH:
  case DEM_INDICATOR_SLOW_FLASH:
  case DEM_INDICATOR_CONTINUOUS:
    lReturnValue = DEM_J1939_LAMP_ON;
    break;
  default:                                                                                                                       /* COV_DEM_MISRA_BRANCH */
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = DEM_J1939_LAMP_OFF;
    break;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IsSpecialIndicator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsSpecialIndicator(
  uint8 IndicatorId
  )
{
  boolean lIsSpecialIndicatorId;
  lIsSpecialIndicatorId = FALSE;

  if (   (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId())
      || (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalRslIndicatorId())
      || (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalAwlIndicatorId())
      || (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalPlIndicatorId()))
  {
    lIsSpecialIndicatorId = TRUE;
  }
  return lIsSpecialIndicatorId;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_GetBitIdexInLampStatusByte
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_GetBitIdexInLampStatusByte(
  uint8 IndicatorId
)
{
  uint8 lBitIndex = DEM_J1939_LAMP_INVALID_BITINDEX;
  if (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId())
  {
    lBitIndex = DEM_J1939_LAMP_MIL_BITINDEX;
  }
  else if (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalRslIndicatorId())
  {
    lBitIndex = DEM_J1939_LAMP_RSL_BITINDEX;
  }
  else if (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalAwlIndicatorId())
  {
    lBitIndex = DEM_J1939_LAMP_AWL_BITINDEX;
  }
  else if (IndicatorId == Dem_GlobalDiagnostics_Indicator_GetGlobalPlIndicatorId())
  {
    lBitIndex = DEM_J1939_LAMP_PL_BITINDEX;
  }
  else
  {
    /* Indicator is no special indicator, return invalid bit index */
  }
  return lBitIndex;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_UpdateLampStatusForIndicator
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
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_UpdateLampStatusForIndicator(                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 IndicatorId,
  Dem_IndicatorStatusType LampStatusOfIndicator,
  Dem_IndicatorStatusType FlashLampStatusOfIndicator,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, AUTOMATIC) LampStatusToUpdate
)
{
  uint8 lIndicatorBitIndex = Dem_GlobalDiagnostics_Indicator_J1939_GetBitIdexInLampStatusByte(IndicatorId);

  if (lIndicatorBitIndex != DEM_J1939_LAMP_INVALID_BITINDEX)
  {
    /* Create bit masks in which all bits are set except for the bits representing the current indicator */
    uint8 lLampStatusBitMask = DEM_J1939_LAMP_ALL_NA & ~(DEM_J1939_LAMP_NA << lIndicatorBitIndex);
    uint8 lLampFlashStatusBitMask = DEM_J1939_LAMP_FLASH_ALL_NO_FLASH & ~(DEM_J1939_LAMP_FLASH_NO_FLASH << lIndicatorBitIndex);

    /* Set the bits representing the current indicator status which were set to 0 in the previous step */
    lLampStatusBitMask |= (LampStatusOfIndicator << lIndicatorBitIndex);
    lLampFlashStatusBitMask |= (FlashLampStatusOfIndicator << lIndicatorBitIndex);

    /* Use the bit mask to only set the desired bits in the resulting lamp status */
    LampStatusToUpdate->LampStatus &= lLampStatusBitMask;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    LampStatusToUpdate->FlashLampStatus &= lLampFlashStatusBitMask;                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetCounterIndexFromId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetCounterIndexFromId(
  uint8 IndicatorId,
  CONSTP2VAR(Dem_Indicator_IndicatorCounterTableIndexType, AUTOMATIC, AUTOMATIC) CounterIndex
  )
{
  boolean lIndicatorIdFound = FALSE;
  Dem_Indicator_IndicatorCounterTableIndexType lIndex;

  for (lIndex = 0; lIndex < Dem_Cfg_GetSizeOfIndicatorCounterTable(); lIndex++)                                                  /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    if (Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorCounterTable(lIndex) == IndicatorId)
    {
      *CounterIndex = lIndex;                                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lIndicatorIdFound = TRUE;
      break;
    }
  }
  return lIndicatorIdFound;
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IndicatorCounterIterInit
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
Dem_GlobalDiagnostics_Indicator_IndicatorCounterIterInit(
  uint8 IndicatorId,                                                                                         
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                            
  )
{
  Dem_Indicator_IndicatorCounterTableIndexType lIndicatorIndex = 0;
  if (Dem_GlobalDiagnostics_Indicator_GetCounterIndexFromId(IndicatorId, &lIndicatorIndex) == TRUE)                              /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
    Dem_Cfg_ComplexIterInit(IterPtr, Dem_Cfg_GetIndicatorTableStartIdxOfIndicatorCounterTable(lIndicatorIndex),                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_Cfg_GetIndicatorTableEndIdxOfIndicatorCounterTable(lIndicatorIndex));
  }
  else
  {
    Dem_Cfg_ComplexIterInit(IterPtr, 0,0);                                                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IsMilStateActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsMilStateActive(
  Dem_IndicatorStatusType MilState
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;

# if (DEM_CFG_REPORT_BLINKING_MIL_AS_ACTIVE == STD_ON)
  if ((MilState != DEM_INDICATOR_OFF))
#else
  if ((MilState == DEM_INDICATOR_CONTINUOUS) || (MilState == DEM_INDICATOR_BLINK_CONT))
# endif
  {
    lReturnValue = TRUE;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_IncrementAllCountersAssignedToEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IncrementAllCountersAssignedToEvent(
  Dem_EventIdType EventId
  )
{
  Dem_Cfg_ComplexIterType lIndicatorIter;

  for (Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE;                                                                          /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lIndicatorIter))                                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_GlobalDiagnostics_Indicator_IncrementIndicatorEventCounter(
      Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter));                                              /* SBSW_DEM_POINTER_FORWARD_STACK */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_ResetAllEventCounters
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_ResetAllEventCounters(
  void
  )
{
  /* If indicators are configured, initialize all indicator state counters */
  Dem_Cfg_IndicatorTableIndType lIndicatorIndex;
  for (lIndicatorIndex = 0u; lIndicatorIndex < Dem_Cfg_GetSizeOfIndicatorTable(); ++lIndicatorIndex)                             /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    Dem_Cfg_SetIndicatorEventCounters(lIndicatorIndex, 0);                                                                       /* SBSW_DEM_ARRAY_WRITE_INDICATOREVENTCOUNTERS */
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Indicator_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeRelatedLampStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeRelatedLampStatus(
  uint8 NodeIndex
)
{
  Dem_J1939DcmLampStatusType lLampStatus = { DEM_J1939_LAMP_ALL_NA , DEM_J1939_LAMP_FLASH_ALL_NO_FLASH };
  uint8 lGlobalMilId = Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId();
  uint8 lGlobalRslId = Dem_GlobalDiagnostics_Indicator_GetGlobalRslIndicatorId();
  uint8 lGlobalAwlId = Dem_GlobalDiagnostics_Indicator_GetGlobalAwlIndicatorId();
  uint8 lGlobalPlId = Dem_GlobalDiagnostics_Indicator_GetGlobalPlIndicatorId();

  Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus(NodeIndex, lGlobalMilId, &lLampStatus);         /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus(NodeIndex, lGlobalRslId, &lLampStatus);         /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus(NodeIndex, lGlobalAwlId, &lLampStatus);         /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeSpecificIndicatorLampStatus(NodeIndex, lGlobalPlId, &lLampStatus);          /* SBSW_DEM_POINTER_FORWARD_STACK */
  return lLampStatus;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_J1939_CalculateLampStatusOfEvent
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
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_CalculateLampStatusOfEvent(
  Dem_EventIdType EventId
)
{
  Dem_J1939DcmLampStatusType lLampStatus = { DEM_J1939_LAMP_ALL_NA , DEM_J1939_LAMP_FLASH_ALL_NO_FLASH };
  Dem_Cfg_ComplexIterType lEventIndicatorIter;

  /* Iterate over all special indicators for the event */
  for (Dem_Core_Event_IndicatorIterInit(EventId, &lEventIndicatorIter);                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterExists(&lEventIndicatorIter) == TRUE;                                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lEventIndicatorIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_IndicatorStatusType lCurrentIndicatorLampStatus = DEM_J1939_LAMP_OFF;
    Dem_IndicatorStatusType lCurrentIndicatorLampFlashStatus = DEM_J1939_LAMP_FLASH_NO_FLASH;

    uint8 lIndicatorId = Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
          Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex((&lEventIndicatorIter)));                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if ((Dem_GlobalDiagnostics_Indicator_IsSpecialIndicator(lIndicatorId) == TRUE) &&
      (Dem_Core_TestExternalDTCUdsStatusBit(EventId, Dem_UdsStatus_WIR) == TRUE))                                                /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      /* indicator requested/active */
      lCurrentIndicatorLampStatus = Dem_GlobalDiagnostics_Indicator_J1939_GetLampStatusFromIndicatorState(Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable
                        (Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lEventIndicatorIter)));                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
      lCurrentIndicatorLampFlashStatus = Dem_GlobalDiagnostics_Indicator_J1939_GetLampFlashStatusFromIndicatorState(Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable
                        (Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lEventIndicatorIter)));                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    }
    Dem_GlobalDiagnostics_Indicator_J1939_UpdateLampStatusForIndicator(lIndicatorId, lCurrentIndicatorLampStatus,
                                                                        lCurrentIndicatorLampFlashStatus, &lLampStatus);         /* SBSW_DEM_POINTER_FORWARD_STACK */
  }
  return lLampStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_GetIndicatorState
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
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorState(
  uint8 IndicatorId,
  uint8 NodeIndex,
  boolean NodeIndependent
  )
{
  Dem_IndicatorStatusType lIndicatorState = DEM_INDICATOR_OFF;

  if (Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled())
  {
    Dem_Cfg_ComplexIterType lCounterIter;

    for (Dem_GlobalDiagnostics_Indicator_IndicatorCounterIterInit(IndicatorId, &lCounterIter);                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterExists(&lCounterIter) == TRUE;                                                                          /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lCounterIter))                                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if ((Dem_GlobalDiagnostics_Indicator_GetNodeIndexOfIndicatorTable(lCounterIter.mIdx) == NodeIndex) || (NodeIndependent == TRUE))
      {
        if (Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters((uint16)(lCounterIter.mIdx)) > 0u)
        {
          if (lIndicatorState == DEM_INDICATOR_OFF)
          {
            lIndicatorState = Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable((uint16)(lCounterIter.mIdx));
          }
          else if ((lIndicatorState == DEM_INDICATOR_CONTINUOUS)                                                                 /* COV_DEM_GETINDICATORSTATE_ROBUSTNESS */
                && (Dem_GlobalDiagnostics_Indicator_GetBehaviorOfIndicatorTable((uint16)(lCounterIter.mIdx)) == DEM_INDICATOR_BLINKING))
          {
            lIndicatorState = DEM_INDICATOR_BLINK_CONT;
          }
          else
          {
            /* Do nothing, Misra case */
          }

          if (lIndicatorState != DEM_INDICATOR_CONTINUOUS)
          {
            /* The counters are sorted by the gravity of their behavior, so the first
              positive counter found determines the behavior of the indicator lamp,
              unless it is CONTINUOUS, in this case we need to continue the search to make
              sure there is no counter with BLINKING behavior, because this results in
              BLINK_CONT. */
            break;
          }
        }
      }
    }

  } /* IsIndicatorSupportEnabled */
  return lIndicatorState;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(
  Dem_EventIdType EventId
  )
{
  Dem_Cfg_ComplexIterType lIndicatorIter;
  boolean lMilAssignedForEvent = FALSE;

  for (Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE;                                                                          /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lIndicatorIter))                                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    if (Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
      Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter)))                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      == Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId())
    {
      lMilAssignedForEvent = TRUE;
      break;
    }
  }
  return lMilAssignedForEvent;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_ProcessEventWirEnabled
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_ProcessEventWirEnabled(
  Dem_EventIdType EventId
  )
{
  if (Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled())                                                               /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_Cfg_ComplexIterType lIndicatorIter;
    boolean lOldMilIsActive;
    lOldMilIsActive = FALSE;

    for (Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE;                                                                        /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lIndicatorIter))                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
           Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter)))                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
          == Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId())
      {
        if (Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters(
              Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 0u)                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          /* counter will increment from 0->1, this is a possible state change */
          lOldMilIsActive = Dem_GlobalDiagnostics_Indicator_TestMilReportedActive();
        }
      }

      Dem_GlobalDiagnostics_Indicator_IncrementIndicatorEventCounter(
        Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter));                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if (Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
           Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter))) 
          == Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId())                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        if ((Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters(
             Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 1u)                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
          && (lOldMilIsActive != Dem_GlobalDiagnostics_Indicator_TestMilReportedActive()))
        {
          /* counter has incremented from 0->1, process the state change */
          Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedEnabled();
          Dem_GlobalDiagnostics_EngineRuntime_ProcessMilStateChangedEnabled();
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_ProcessEventWirDisabled
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_ProcessEventWirDisabled(
  Dem_EventIdType EventId
  )
{
  if (Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled())
  {
    Dem_Cfg_ComplexIterType lIndicatorIter;
    boolean lOldMilIsActive;
    lOldMilIsActive = FALSE;

    for (Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE;                                                                        /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lIndicatorIter))                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
           Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter)))                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
          == Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId())
      {
        if (Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters(
            Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 1u)                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          /* counter will increment from 1->0, this is a possible state change */
          lOldMilIsActive = Dem_GlobalDiagnostics_Indicator_TestMilReportedActive();
        }
      }
      
      
      Dem_GlobalDiagnostics_Indicator_DecrementIndicatorEventCounter(
        Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter));                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if (Dem_GlobalDiagnostics_Indicator_GetIndicatorIdOfIndicatorTable(
        Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex((&lIndicatorIter)))                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
        == Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId())
      {
        if ((Dem_GlobalDiagnostics_Indicator_GetIndicatorEventCounters(
          Dem_GlobalDiagnostics_Indicator_EventIndicatorIterGetIndex(&lIndicatorIter)) == 0u)                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
          && (lOldMilIsActive != Dem_GlobalDiagnostics_Indicator_TestMilReportedActive()))
        {
          /* counter has incremented from 0->1, process the state change */
          Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedDisabled();
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_Init
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
Dem_GlobalDiagnostics_Indicator_Init(
  void
  )
{
  if (Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled())
  {
    Dem_EventIdType lEventId;

    Dem_GlobalDiagnostics_Indicator_ResetAllEventCounters();

    for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
    {
      if (Dem_Event_TestEventAvailable(lEventId) == TRUE)
      {
        Dem_UDSStatus_StatusType lExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(lEventId, Dem_Event_UDSStatus_GetInternalEventUdsStatus(lEventId));

        if (Dem_Event_UDSStatus_TestBit(lExternalEventStatus, Dem_UdsStatus_WIR) == TRUE)
        {
          Dem_GlobalDiagnostics_Indicator_IncrementAllCountersAssignedToEvent(lEventId);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Indicator_TestMilReportedActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_TestMilReportedActive(                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;

  if (Dem_GlobalDiagnostics_Indicator_IsIndicatorSupportEnabled())                                                               /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    lReturnValue = Dem_GlobalDiagnostics_Indicator_IsMilStateActive(Dem_GlobalDiagnostics_Indicator_GetIndicatorState(Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId(), 0, TRUE));
  }

  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_INDICATOR_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Indicator_Implementation.h
 *********************************************************************************************************************/
