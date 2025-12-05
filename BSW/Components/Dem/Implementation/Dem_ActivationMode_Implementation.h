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
/*! \addtogroup Dem_ActivationMode
 *  \{
 *  \file       Dem_ActivationMode_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     ActivationMode
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ACTIVATIONMODE_IMPLEMENTATION_H)
#define DEM_ACTIVATIONMODE_IMPLEMENTATION_H

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
#include "Dem_ActivationMode_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Indicator_Interface.h"

/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_ACTIVATIONMODE_IMPLEMENTATION_FILENAME "Dem_ActivationMode_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#  define DEM_ACTIVATIONMODE_NUMBER_DTCCLASS_ARRAY_SIZE (DEM_ACTIVATIONMODE_NUMBER_DTCCLASS)

/*! Number of qualified active and failed events per WWH-OBD class. Index 0 (no class) is unused. */
DEM_LOCAL VAR(Dem_EventIdType, DEM_VAR_NO_INIT)   Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount[DEM_ACTIVATIONMODE_NUMBER_DTCCLASS_ARRAY_SIZE];
/*! Number of MIL requests per WWH-OBD class. Index 0 (no class) is unused. */
DEM_LOCAL VAR(Dem_EventIdType, DEM_VAR_NO_INIT)   Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount[DEM_ACTIVATIONMODE_NUMBER_DTCCLASS_ARRAY_SIZE];
/*! Tracks if a Class B1 DTC malfunction was detected in the current DCY. */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)   Dem_GlobalDiagnostics_ActivationMode_B1Counter_AnyMalfunctionFailedThisDCY;
/*! Tracks if all Class B1 DTC malfunctions were tested in the current DCY. */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)   Dem_GlobalDiagnostics_ActivationMode_B1Counter_AllMalfunctionsTestedThisDCY;


#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ActivationMode_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue
 *****************************************************************************/
/*!
 * \brief         Get the number of MIL requests for the WWH-OBD DTC class
 *
 * \details       Get the number of MIL requests for the WWH-OBD DTC class
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \return        The number of MIL requests
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
  );
/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_SetValue
 *****************************************************************************/
/*!
 * \brief         Set the number of MIL requests for the WWH-OBD DTC class
 *
 * \details       Set the number of MIL requests for the WWH-OBD DTC class
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 * \param[in]     DtcClassMILCount
 *                The new number of MIL requests
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_SetValue(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass,
  CONST(Dem_EventIdType, AUTOMATIC)  DtcClassMILCount
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Increment
 *****************************************************************************/
/*!
 * \brief         Increment the number of MIL requests for the WWH-OBD DTC class
 *
 * \details       Increment the number of MIL requests for the WWH-OBD DTC class
 *                by one
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Increment(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Decrement
 *****************************************************************************/
/*!
 * \brief         Decrement the number of MIL requests for the WWH-OBD DTC class
 *
 * \details       Decrement the number of MIL requests for the WWH-OBD DTC class
 *                by one
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Decrement(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Init
 *****************************************************************************/
/*!
 * \brief         Set the number of MIL requests for the WWH-OBD DTC class to 0
 *
 * \details       Set the number of MIL requests for the WWH-OBD DTC class to 0
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Init(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_SetValue
 *****************************************************************************/
/*!
 * \brief         Set the number of MIL requests for the WWH-OBD DTC class
 *
 * \details       Set the number of MIL requests for the WWH-OBD DTC class
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 * \param[in]     QualifiedActiveEventCount
 *                The new number of MIL requests
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_SetValue(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass,
  CONST(Dem_EventIdType, AUTOMATIC)  QualifiedActiveEventCount
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Increment
 *****************************************************************************/
/*!
 * \brief         Increment the number of MIL requests for the WWH-OBD DTC class
 *
 * \details       Increment the number of MIL requests for the WWH-OBD DTC class
 *                by one
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \return        TRUE: Increment of the qualified active event count resulted 
 *                      in activation of activation mode 4
 *                FALSE: Increment of the qualified active event count did not 
 *                      result in activation of activation mode 4
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Increment(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Decrement
 *****************************************************************************/
/*!
 * \brief         Decrement the number of MIL requests for the WWH-OBD DTC class
 *
 * \details       Decrement the number of MIL requests for the WWH-OBD DTC class
 *                by one
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Decrement(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Init
 *****************************************************************************/
/*!
 * \brief         Set the number of MIL requests for the WWH-OBD DTC class to 0
 *
 * \details       Set the number of MIL requests for the WWH-OBD DTC class to 0
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Init(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue
 *****************************************************************************/
/*!
 * \brief         Get the number of qualified active and failed events for the
 *                WWH-OBD DTC class
 *
 * \details       Get the number of qualified active and failed events for the
 *                WWH-OBD DTC class
 *
 * \param[in]     WwhObdDtcClass
 *                Unique handle of the WWH-OBD DTC class
 *
 * \return        The number of qualified active and failed events
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue
 *****************************************************************************/
/*!
 * \brief        Set the value of the B1 Counter
 *
 * \details      Set the B1 Counter to given value
 *
 * \param[in]    B1 counter value

 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue(
  CONST(uint16, AUTOMATIC)  B1CounterValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetHealingCounter
 *****************************************************************************/
/*!
 * \brief         Get the value of the B1 healing counter
 *
 * \details       Get the value of the B1 healing counter
 *
 * \param[in]     -
 *
 * \return        B1 healing counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetHealingCounter(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter
 *****************************************************************************/
/*!
 * \brief        Set the value of the B1 healing counter
 *
 * \details      Set the B1 healing counter to given value
 *
 * \param[in]    B1 healing counter value
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter(
  CONST(uint8, AUTOMATIC)  B1HealingCounter
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue
 *****************************************************************************/
/*!
 * \brief          Get the WWH OBD B1 counter value
 *
 * \details        -
 *
 * \return         B1 counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue(
  void
);

/* ****************************************************************************
* Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAnyMalfunctionFailedThisDCY
*****************************************************************************/
/*!
 * \brief         Gets if any Class B1 DTC malfunction failed this DCY
 *
 * \details       Gets if any Class B1 DTC malfunction failed this DCY. 
 *                Flag is calculated when the end of the DCY is processed for
 *                each malfunction.
 *
 * \return        TRUE
 *                At least one Class B1 DTC malfunction failed in this DCY
 * \return        FALSE
 *                No Class B1 DTC malfunction failed in this DCY
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAnyMalfunctionFailedThisDCY(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY
 *****************************************************************************/
/*!
 * \brief         Sets if any Class B1 DTC malfunction failed this DCY
 *
 * \details       Sets if any Class B1 DTC malfunction failed this DCY.
 *
 * \param[in]     MalfunctionFailedThisDCYCycle
 *                TRUE
 *                At least one Class B1 DTC malfunction failed in this DCY
 *                FALSE
 *                No Class B1 DTC malfunction failed in this DCY
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY(
  CONST(boolean, AUTOMATIC)  MalfunctionFailedThisDCYCycle
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAllMalfunctionsTestedThisDCY
 *****************************************************************************/
/*!
 * \brief         Gets if all Class B1 DTC malfunctions were tested this DCY
 *
 * \details       Gets if all Class B1 DTC malfunctions were tested this DCY.
 *                Flag is calculated when the end of the DCY is processed for
 *                each malfunction.
 *
 *
 * \return        TRUE
 *                All Class B1 DTC Malfunctions tested in this DCY
 * \return        FALSE
 *                At least one Class B1 DTC malfunction not tested in this DCY
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAllMalfunctionsTestedThisDCY(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY
 *****************************************************************************/
/*!
 * \brief         Sets if all Class B1 DTC malfunctions were tested this DCY
 *
 * \details       Sets if all Class B1 DTC malfunctions were tested this DCY
 *
 * \param[in]     MalfunctionsTestedThisDCYCycle
 *                TRUE
 *                All Class B1 DTC Malfunctions were tested this DCY
 *                FALSE
 *                At least one Class B1 DTC Malfunction was not tested this DCY
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY(
  boolean MalfunctionsTestedThisDCYCycle
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetValue
 *****************************************************************************/
/*!
 * \brief        Set the value of the continuous Mi counter  
 *
 * \details      Set the continuous Mi counter to given value
 *
 * \param[in]    Continuous Mi counter value
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetValue(
  CONST(uint16, AUTOMATIC)  ContinuousMiCounterValue
);

/* ****************************************************************************
* Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValue
*****************************************************************************/
/*!
 * \brief        Get the WWH OBD continuous Mi counter value
 *
 * \details      -
 *
 * \return       Continuous Mi counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValue(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue
 *****************************************************************************/
/*!
 * \brief         Set the value of the continuous Mi counter to 0
 *
 * \details       Set the value of the continuous Mi counter to 0
 *
 * \param[in]     -
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue(
  void
);


/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter
 *****************************************************************************/
/*!
 * \brief         Get the value of the continuous Mi healing counter
 *
 * \details       Get the value of the continuous Mi healing counter
 *
 * \param[in]     -
 *
 * \return        Continuous Mi healing counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter
 *****************************************************************************/
/*!
 * \brief         Set the value of the continuous Mi healing counter
 *
 * \details       Set the continuous Mi healing counter to given value
 *
 * \param[in]     Continuous Mi healing counter value
 *      
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter(
  CONST(uint8, AUTOMATIC)  ContinuousMiHealingCounter
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingCounter
 *****************************************************************************/
/*!
 * \brief         Get the value of the continuous Mi aging counter
 *
 * \details       Get the value of the continuous Mi aging counter
 *
 * \param[in]     -
 *
 * \return        Continuous Mi aging counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingCounter(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter
 *****************************************************************************/
/*!
 * \brief         Set the value of the continuous Mi aging counter
 *
 * \details       Set the continuous Mi aging counter to given value
 *
 * \param[in]     Continuous Mi aging counter value
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter(
  CONST(uint8, AUTOMATIC)  ContinuousMiAgingCounter
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingTimer
 *****************************************************************************/
/*!
 * \brief         Get the value of the continuous Mi aging timer
 *
 * \details       Get the value of the continuous Mi aging timer
 *
 * \param[in]     -
 *
 * \return        Continuous Mi aging timer value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingTimer(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer
 *****************************************************************************/
/*!
 * \brief         Set the value of the continuous Mi aging timer
 *
 * \details       Set the continuous Mi aging timer to given value
 *
 * \param[in]     Continuous Mi aging timer value
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer(
  CONST(uint16, AUTOMATIC)  ContinuousMiAgingTimer
);


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_SetValue
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_SetValue(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass,
  CONST(Dem_EventIdType, AUTOMATIC)  DtcClassMILCount
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (WwhObdDtcClass >= DEM_ACTIVATIONMODE_NUMBER_DTCCLASS)
  {
    Dem_Error_RunTimeCheckFailed(DEM_ACTIVATIONMODE_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount[WwhObdDtcClass] = DtcClassMILCount;                                    /* SBSW_DEM_ARRAY_WRITE_DTCCLASSMILCOUNT */ /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Increment
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Increment(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  Dem_EventIdType lNewDtcClassMilCount = Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(WwhObdDtcClass) + 1u;
  Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_SetValue(WwhObdDtcClass, lNewDtcClassMilCount);
}
/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Decrement
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Decrement(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  Dem_EventIdType lNewDtcClassMilCount = Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(WwhObdDtcClass) - 1u;
  Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_SetValue(WwhObdDtcClass, lNewDtcClassMilCount);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Init
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Init(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_SetValue(WwhObdDtcClass, 0);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  return Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount[WwhObdDtcClass];
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue(
  void
)
{
  uint16 lB1CounterValue = 0;
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)                                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    lB1CounterValue = Dem_Cfg_AdminData.B1Counter;
#endif
  }
  return lB1CounterValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAnyMalfunctionFailedThisDCY
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAnyMalfunctionFailedThisDCY(
  void
)
{
  return Dem_GlobalDiagnostics_ActivationMode_B1Counter_AnyMalfunctionFailedThisDCY;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY(
  CONST(boolean, AUTOMATIC)  MalfunctionFailedThisDCYCycle
)
{
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_AnyMalfunctionFailedThisDCY
  = MalfunctionFailedThisDCYCycle;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAllMalfunctionsTestedThisDCY
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAllMalfunctionsTestedThisDCY(
  void
)
{
  return Dem_GlobalDiagnostics_ActivationMode_B1Counter_AllMalfunctionsTestedThisDCY;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY(
  boolean MalfunctionsTestedThisDCYCycle
)
{
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_AllMalfunctionsTestedThisDCY
    = MalfunctionsTestedThisDCYCycle;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_SetValue
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_SetValue(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass,
  CONST(Dem_EventIdType, AUTOMATIC)  QualifiedActiveEventCount
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (WwhObdDtcClass >= DEM_ACTIVATIONMODE_NUMBER_DTCCLASS)
  {
    Dem_Error_RunTimeCheckFailed(DEM_ACTIVATIONMODE_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount[WwhObdDtcClass] = QualifiedActiveEventCount;                  /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_QUALIFIEDACTIVEEVENTCOUNT */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Increment
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Increment(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  boolean lTransitionToActivationMode4 = FALSE;
  Dem_EventIdType lOldQualifiedActiveEventCount = Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue(WwhObdDtcClass);
  Dem_EventIdType lNewQualifiedActiveEventCount = lOldQualifiedActiveEventCount +1u;
   
  Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_SetValue(WwhObdDtcClass, lNewQualifiedActiveEventCount);


  if ( (WwhObdDtcClass == DEM_ACTIVATIONMODE_DTCCLASS_A)
    && (lOldQualifiedActiveEventCount == 0u))
  {
    /* lNewQualifiedActiveEventCount == 1 now*/
    lTransitionToActivationMode4 = TRUE;
  }
  return lTransitionToActivationMode4;
}
/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Decrement
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Decrement(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  Dem_EventIdType lNewQualifiedActiveEventCount = Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue(WwhObdDtcClass) - 1u;
  Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_SetValue(WwhObdDtcClass, lNewQualifiedActiveEventCount);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Init
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Init(
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_SetValue(WwhObdDtcClass, 0);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue( 
  CONST(uint8, AUTOMATIC)  WwhObdDtcClass
)
{
  return Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount[WwhObdDtcClass];
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue(
  CONST(uint16, AUTOMATIC)  B1CounterValue
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(B1CounterValue)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    Dem_Cfg_AdminData.B1Counter = B1CounterValue;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetHealingCounter
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetHealingCounter(
  void
)
{
  uint8 lB1HealingCounter;
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  lB1HealingCounter = Dem_Cfg_AdminData.B1HealingCounter;
#else
  lB1HealingCounter = 0;
#endif

  return lB1HealingCounter;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter(
  CONST(uint8, AUTOMATIC)  B1HealingCounter
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(B1HealingCounter)                                                                             /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    Dem_Cfg_AdminData.B1HealingCounter = B1HealingCounter;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetValue
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetValue(
  CONST(uint16, AUTOMATIC)  ContinuousMiCounterValue
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ContinuousMiCounterValue)                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    Dem_Cfg_AdminData.ContinuousMiCounter = ContinuousMiCounterValue;
#endif

}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValue
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValue(
  void
)
{
  uint16 lContinuousMiCounter = 0;
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)                                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    lContinuousMiCounter = Dem_Cfg_AdminData.ContinuousMiCounter;
#endif
  }
  return lContinuousMiCounter;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter(
  CONST(uint8, AUTOMATIC)  ContinuousMiHealingCounter
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ContinuousMiHealingCounter)                                                                   /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    Dem_Cfg_AdminData.ContinuousMiHealingCounter = ContinuousMiHealingCounter;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter(
  void
)
{
  uint8 lContinuousMiHealingCounter = 0;

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    lContinuousMiHealingCounter = Dem_Cfg_AdminData.ContinuousMiHealingCounter;
#endif
  return lContinuousMiHealingCounter;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter(
  CONST(uint8, AUTOMATIC)  ContinuousMiAgingCounter
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ContinuousMiAgingCounter)                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    Dem_Cfg_AdminData.ContinuousMiAgingCounter = ContinuousMiAgingCounter;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingCounter
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingCounter(
  void
)
{
  uint8 lContinuousMiAgingCounter;
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  lContinuousMiAgingCounter = Dem_Cfg_AdminData.ContinuousMiAgingCounter;
#else
  lContinuousMiAgingCounter = 0;
#endif

  return lContinuousMiAgingCounter;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer(
  CONST(uint16, AUTOMATIC)  ContinuousMiAgingTimer
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ContinuousMiAgingTimer)                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    Dem_Cfg_AdminData.ContinuousMiAgingTimer = ContinuousMiAgingTimer;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingTimer
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingTimer(
  void
)
{
  uint16 lContinuousMiAgingTimer = 0;
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    lContinuousMiAgingTimer = Dem_Cfg_AdminData.ContinuousMiAgingTimer;
#endif
  return lContinuousMiAgingTimer;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue(
  void
)
{
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetValue(0u);
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
 * \addtogroup Dem_GlobalDiagnostics_ActivationMode_PublicProperties
 * \{
 */

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValueInHours
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValueInHours(                                                                  /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  return Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue() / 60u;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValueInHours
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValueInHours(                                                        /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  return Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValue() / 60u;
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
 * \defgroup Dem_GlobalDiagnostics_ActivationMode_Private Private Methods
 * \{
 */
/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_InitCounters
 *****************************************************************************/
/*!
 * \brief         Reset B1 counter related counters
 *
 * \details       Reset B1 counter and B1 healing counter
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_InitCounters(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_LatchValue
 *****************************************************************************/
/*!
 * \brief         Set B1 Counter value to 190h
 *
 * \details       Set B1 Counter value to 190h 
 *                - if the B1 counter value exceeds 200h and 
 *                - if no class B1 event is confirmed and active 
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_LatchValue(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_Increment
 *****************************************************************************/
/*!
 * \brief         Increment the B1 counter
 *
 * \details       Increment the B1 counter and latch value if necessary
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \return        TRUE: Increment of the B1 counter resulted
 *                      in activation of activation mode 4
 *                FALSE: Increment of the B1 counter did not
 *                      result in activation of activation mode 4
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_Increment(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessEngineRunTime
 *****************************************************************************/
/*!
 * \brief         Process a new EngineRunTime value for B1 counter
 *
 * \details       Update the B1 counter and trigger permanent memory
 *                update on exceeding 200h.
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessEngineRunTime(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetToRunState
 *****************************************************************************/
/*!
 * \brief         Stop healing and aging of ContinuousMiCounter
 *
 * \details       Set ContinuousMi healing and aging counters to invalid
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetToRunState(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_Increment
 *****************************************************************************/
/*!
 * \brief         Increment the Continuous Mi counter
 *
 * \details       -
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_Increment(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_PrepareHealing
 *****************************************************************************/
/*!
 * \brief         Prepare healing of ContinuousMiCounter
 *
 * \details       Prepare healing of ContinuousMiCounter if activation mode
 *                4 is not active
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_PrepareHealing(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsHealingPrepared
 *****************************************************************************/
/*!
 * \brief         Check if healing of Continuous Mi Counter is prepared
 *
 * \details       Check if healing of Continuous Mi Counter will start with end
 *                of DCY because activation mode 4 got inactive during the current
 *                DCY
 *
 * \return        TRUE: Healing is prepared
 *                FALSE: Continuous Mi Counter is currently active or currently
 *                       healing or has already healed
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsHealingPrepared(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_HasHealed
 *****************************************************************************/
/*!
 * \brief         Check if Continuous Mi Counter has healed
 *
 * \details       Check if healing of Continuous Mi Counter is ongoing
 *
 * \return        TRUE: Continuous Mi counter is currently healing
 *                FALSE: Continuous Mi Counter is currently active or healing
 *                will start with next DCY or has already healed
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_HasHealed(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsCurrentlyAging
 *****************************************************************************/
/*!
 * \brief         Check if Continuous Mi Counter is currently aging
 *
 * \details       Check aging counter of Continuous Mi counter to check if 
 *                aging of Continuous Mi Counter is currently ongoing
 *
 * \return        TRUE: Continuous Mi counter is currently aging
 *                FALSE: Aging Counter does not indicate an ongoing aging process
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsCurrentlyAging(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingTimer
 *****************************************************************************/
/*!
 * \brief         Process aging of Continuous Mi counter
 *
 * \details       Process aging of Continuous Mi counter depending on engine 
 *                runtime
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingTimer(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessEngineRunTime
 *****************************************************************************/
/*!
 * \brief         Process EngineRunTime update for continuous Mi counter
 *
 * \details       Process EngineRunTime update depending on current activation mode 
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessEngineRunTime(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessEventOfDtcClassNoLongerQualifiedActive
 *****************************************************************************/
/*!
 * \brief         Process the effects of an event not being qualified active &
 *                failed anymore.
 *
 * \details       Process the effects of an event not being qualified active &
 *                failed anymore. Internally checks if healing conditions are met
 *                for b1 counter and continuous mi counter and starts it.
 *
 * \param[in]     WwhObdDtcClass
 *                DTC Class of the event not being qualified active anymore.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEventOfDtcClassNoLongerQualifiedActive(
  CONST(uint8, AUTOMATIC) WwhObdDtcClass
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active
 *****************************************************************************/
/*!
 * \brief         Tests if activation mode 4 is currently active
 *
 * \details       -
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_InitEventCounters
 *****************************************************************************/
/*!
 * \brief         Initializes the WWHOBD Dtc class related statistics
 *
 * \details       Initializes the WWHOBD Dtc class related statistics
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_InitEventCounters(
  void
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_InitCounters
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_InitCounters(
  void
)
{
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue(0u);
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter(0u);
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY(FALSE);
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY(TRUE);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_LatchValue
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_LatchValue(
  void
)
{
  if (Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue() >= DEM_ACTIVATIONMODE_B1COUNTER_200H)
  {
    Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue(DEM_ACTIVATIONMODE_B1COUNTER_190H);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetToRunState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetToRunState(
  void
)
{
  /* Run state */
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter(DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_INVALID);
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer(DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_TIMER_INVALID);
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter(DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_INVALID);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_PrepareHealing
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_PrepareHealing(
  void
  )
{
  if (Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter() == DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_INVALID)
  {
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter(DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_LOCKED);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsHealingPrepared
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsHealingPrepared(
  void
)
{
    return (boolean)(Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter() == DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_LOCKED);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_HasHealed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_HasHealed(
  void
)
{
  return (boolean)(Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter() == DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_HEALED);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsCurrentlyAging
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsCurrentlyAging(
  void
)
{
  boolean IsCurrentlyAging = FALSE;
  uint8 lContinuousMiAgingCounter = Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingCounter();
  if (lContinuousMiAgingCounter  < DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_AGED)
  {
    IsCurrentlyAging = TRUE;
  }
  return IsCurrentlyAging;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingTimer
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingTimer(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
)
{
  uint32 lContinuousMiCounterAgingTime;

  lContinuousMiCounterAgingTime = Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingTimer();

  if (lContinuousMiCounterAgingTime < DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_TIMER_AGED)
  {/* Aging is active: process aging timer */
    lContinuousMiCounterAgingTime = lContinuousMiCounterAgingTime + EngineRunTimeTicks;

    if (lContinuousMiCounterAgingTime >= DEM_ACTIVATIONMODE_CONTINUOUS_MI_200H)
    { /* Aging time exceeded, reset Continuous MI counter */
      lContinuousMiCounterAgingTime = DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_TIMER_AGED;
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter(DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_AGED);
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue();
    }
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer((uint16)lContinuousMiCounterAgingTime);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_Increment
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_Increment(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
)
{

  uint32 lOldB1Counter;
  uint32 lNewB1Counter;
  boolean lTransitionToActivationMode4 = FALSE;

  lOldB1Counter = Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue();

  if (lOldB1Counter < DEM_ACTIVATIONMODE_B1COUNTER_LATCHED)
  { /* Latch B1Counter if >= 255h (15300min) */
    lNewB1Counter = lOldB1Counter + EngineRunTimeTicks;

    if (lNewB1Counter > DEM_ACTIVATIONMODE_B1COUNTER_LATCHED)
    { /* latch at 255h */
      lNewB1Counter = DEM_ACTIVATIONMODE_B1COUNTER_LATCHED;
    }

    Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue((uint16)lNewB1Counter);

    if ((lOldB1Counter <= DEM_ACTIVATIONMODE_B1COUNTER_200H)
      && (lNewB1Counter > DEM_ACTIVATIONMODE_B1COUNTER_200H))
    {
      lTransitionToActivationMode4 = TRUE;
    }
  }

  return lTransitionToActivationMode4;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_Increment
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_Increment(                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
)
{
  uint32 lContinuousMiCounter;

  lContinuousMiCounter = Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValue();

  if (lContinuousMiCounter < DEM_ACTIVATIONMODE_CONTINUOUS_MI_LATCHED)
  { /* Latch ContinuousMiCounter if >= 255h (15300min) */
    lContinuousMiCounter = lContinuousMiCounter + EngineRunTimeTicks;

    if (lContinuousMiCounter > DEM_ACTIVATIONMODE_CONTINUOUS_MI_LATCHED)
    { /* latch at 255h */
      lContinuousMiCounter = DEM_ACTIVATIONMODE_CONTINUOUS_MI_LATCHED;
    }

    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetValue((uint16)lContinuousMiCounter);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessEngineRunTime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessEngineRunTime(                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
)
{
  boolean lTransitionToActivationMode4 = FALSE;
  /* increment B1Counter if at least one B1 event is confirmed and failed */
  if (Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue(DEM_ACTIVATIONMODE_DTCCLASS_B1) != 0u)
  {
    lTransitionToActivationMode4 = Dem_GlobalDiagnostics_ActivationMode_B1Counter_Increment(EngineRunTimeTicks);

    if (lTransitionToActivationMode4 == TRUE)
    {
      Dem_Memories_PermanentMemory_ProcessActivationMode4Enabled();
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessEngineRunTime
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessEngineRunTime(                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
)
{
  /* increment continuous MI counter as long as activation mode 4 is active */
  if (Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active() == TRUE)
  {
    if (Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_HasHealed() == TRUE)
    {
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue();
    }

    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetToRunState();
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_Increment(EngineRunTimeTicks);
  }
  else
  {
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingTimer(EngineRunTimeTicks);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ProcessEventOfDtcClassNoLongerQualifiedActive
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEventOfDtcClassNoLongerQualifiedActive(
  CONST(uint8, AUTOMATIC) WwhObdDtcClass
)
{
  Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Decrement(WwhObdDtcClass);
  if (WwhObdDtcClass == DEM_ACTIVATIONMODE_DTCCLASS_B1)
  { 
    if (Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue(DEM_ACTIVATIONMODE_DTCCLASS_B1) == 0u)
    {
      Dem_GlobalDiagnostics_ActivationMode_B1Counter_LatchValue();
      Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter(0u);
    }
  }

  if (Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active() == FALSE)
  {
    /* prepare continuous MI Healing if activation mode 4 is not active */
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_PrepareHealing();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active(
  void
)
{
  boolean lIsActivationMode4Active;

  lIsActivationMode4Active = FALSE;

  lIsActivationMode4Active = (boolean)
    ((Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_GetValue(DEM_ACTIVATIONMODE_DTCCLASS_A) > 0u)
      || (Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue() >= DEM_ACTIVATIONMODE_B1COUNTER_200H));
  return lIsActivationMode4Active;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_InitEventCounters
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_InitEventCounters(
  void
)
{
  uint8 lDtcClassIndex;

  for (lDtcClassIndex = 0; lDtcClassIndex < DEM_ACTIVATIONMODE_NUMBER_DTCCLASS; lDtcClassIndex++)
  {
    Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Init(lDtcClassIndex);
    Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Init(lDtcClassIndex);
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
 * \addtogroup Dem_GlobalDiagnostics_ActivationMode_Public
 * \{
 */ 


/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_Init
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_Init(
  void
  )
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_EventIdType lEventId;

    Dem_GlobalDiagnostics_ActivationMode_InitEventCounters();

    Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY(FALSE);
    Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY(TRUE);

    for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
    {
      if (Dem_Event_TestEventAvailable(lEventId) == TRUE)
      {
        if (Dem_Core_TestExternalEventUdsStatusBit(lEventId, Dem_UdsStatus_WIR) == TRUE)
        {
          Dem_GlobalDiagnostics_ActivationMode_ProcessWirEnabled(lEventId);
        }
        if ((Dem_Core_TestExternalEventUdsStatusBit(lEventId, Dem_UdsStatus_CDTC) == TRUE)
          && (Dem_Core_TestExternalEventUdsStatusBit(lEventId, Dem_UdsStatus_TF) == TRUE))                                       /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          Dem_GlobalDiagnostics_ActivationMode_ProcessQualifiedActiveEvent(lEventId);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_InitAdminData
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_InitAdminData(
  void
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_GlobalDiagnostics_ActivationMode_B1Counter_InitCounters();
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitCounters();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitCounters
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitCounters(
  void
)
{
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue();
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter(DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_AGED);
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter(DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_HEALED);
  Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer(DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_TIMER_AGED);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingCounter
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
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingCounter(
  void
)
{
  uint8 lContinuousMiAgingCounter;

  lContinuousMiAgingCounter = Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetAgingCounter();

  if (lContinuousMiAgingCounter < DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_AGED)
  { /* Aging is active, process MI aging counter */
    ++lContinuousMiAgingCounter;

    if (lContinuousMiAgingCounter == DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_THRESHOLD)
    {
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue();
      lContinuousMiAgingCounter = DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_AGED;
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer(DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_TIMER_AGED);
    }
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter(lContinuousMiAgingCounter);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessHealing
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
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessHealing(
  void
)
{
  uint8 lContinuousMiHealingCounter;

  lContinuousMiHealingCounter = Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetHealingCounter();

  if (lContinuousMiHealingCounter == DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_LOCKED)
  { /* Activation mode 4 not active, start Healing */
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter(0);
  }
  else if (lContinuousMiHealingCounter < DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_HEALED)
  {  /* Healing is active */
    ++lContinuousMiHealingCounter;

    if (lContinuousMiHealingCounter == DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_THRESHOLD)
    { /* Healing is complete, start aging */
      lContinuousMiHealingCounter = DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_HEALED;
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingCounter(0);
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetAgingTimer(0);
    }
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetHealingCounter(lContinuousMiHealingCounter);
  }
  else
  {
    /* nothing to do here */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_RestoreCounters
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_RestoreCounters(
  void
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    /* recovery */
    if ((Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsHealingPrepared() == TRUE)
      && (Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active() == TRUE))
    { /* If continuous MI counter is not processed, reset states so it will be processed later */
      Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetToRunState();
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ProcessEngineRunTime
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEngineRunTime(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)                                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessEngineRunTime(EngineRunTimeTicks);
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessEngineRunTime(EngineRunTimeTicks);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_IsEventAndActivationMode4Active
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_IsEventAndActivationMode4Active(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lIsActivationMode4Active;

  lIsActivationMode4Active = FALSE;
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)                                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    uint8 lWwhObdDtcClass;
    lWwhObdDtcClass = Dem_Core_DTC_GetWwhObdDtcClass(EventId);

    lIsActivationMode4Active = (boolean)
      ((Dem_Event_InternalStatus_QualifyStatus_TestBit(Dem_DTC_GetQualifyStatus(EventId), Dem_QualifyStatus_CDTC) == TRUE)
        && (Dem_Core_TestInternalDTCUdsStatusBit(EventId, Dem_UdsStatus_TF) == TRUE)                                             /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        && ((lWwhObdDtcClass == DEM_ACTIVATIONMODE_DTCCLASS_A) || (Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValue() > DEM_ACTIVATIONMODE_B1COUNTER_200H)));
  }
  return lIsActivationMode4Active;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_GetMILStatus
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
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_GetMILStatus(
  void
  )
{
  Dem_IndicatorStatusType lReturnValue = DEM_INDICATOR_OFF;
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)                                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    /* calculate the current activation mode */
    if (Dem_GlobalDiagnostics_ActivationMode_IsActivationMode4Active() == TRUE)
    { /* Class A event currently active or B1 counter exceeds threshold */
      lReturnValue = DEM_INDICATOR_CONTINUOUS;
    }
    else if ((Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(DEM_ACTIVATIONMODE_DTCCLASS_A) != 0u)
      || (Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(DEM_ACTIVATIONMODE_DTCCLASS_B1) != 0u)
      || (Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(DEM_ACTIVATIONMODE_DTCCLASS_B2) != 0u))
    {
      lReturnValue = DEM_INDICATOR_SHORT;
    }
    else if (Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_GetValue(DEM_ACTIVATIONMODE_DTCCLASS_C) != 0u)
    {
      lReturnValue = DEM_INDICATOR_ON_DEMAND;
    }
    else /* DEM_ACTIVATIONMODE_DTCCLASS_NO */
    {
      /* nothing to do, lReturnValue = DEM_INDICATOR_OFF already set */
    }
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ProcessWirEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessWirEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    if (Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(EventId))
    {
      uint8 lWwhObdDtcClass;
      lWwhObdDtcClass = Dem_Core_DTC_GetWwhObdDtcClass(EventId);
      Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Increment(lWwhObdDtcClass);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ProcessWirDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessWirDisabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    if (Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(EventId))
    {
      uint8 lWwhObdDtcClass;
      lWwhObdDtcClass = Dem_Core_DTC_GetWwhObdDtcClass(EventId);
      Dem_GlobalDiagnostics_ActivationMode_DtcClassMILCount_Decrement(lWwhObdDtcClass);
    }
  }
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ProcessQualifiedActiveEvent
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessQualifiedActiveEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    if (Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(EventId) == TRUE)
    {
      uint8 lWwhObdDtcClass;
      boolean lTransitionToActivationMode4 = FALSE;

      lWwhObdDtcClass = Dem_Core_DTC_GetWwhObdDtcClass(EventId);

      lTransitionToActivationMode4 = Dem_GlobalDiagnostics_ActivationMode_QualifiedActiveEventCount_Increment(lWwhObdDtcClass);

      /* if continuous MI counter is not active yet and activation mode 4 becomes active , set to Run state */
      if (lTransitionToActivationMode4 == TRUE)
      {
        /* if continuous MI counter was healed and is currently aging, reset continuous MI counter */
        if (Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_IsCurrentlyAging() == TRUE)
        {
          Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitValue();
        }
        Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_SetToRunState();
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ProcessEventPassed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEventPassed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  )
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    if (Dem_Core_Event_UpdateTrigger_TestPassed(EventContext->EventEffects) == TRUE)
    {
      if ( (Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(EventContext->EventId) == TRUE)
        && (Dem_Event_InternalStatus_QualifyStatus_TestBit(EventContext->QualifyStatus, Dem_QualifyStatus_CDTC) == TRUE))
      {
        uint8 lDTCClass = Dem_Core_DTC_GetWwhObdDtcClass(EventContext->EventId);
        Dem_GlobalDiagnostics_ActivationMode_ProcessEventOfDtcClassNoLongerQualifiedActive(lDTCClass);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_ProcessEventCleared
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEventCleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    if ((Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(EventId) == TRUE)
      && (Dem_Event_InternalStatus_QualifyStatus_TestBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_CDTC) == TRUE))
    {
      uint8 lDTCClass = Dem_Core_DTC_GetWwhObdDtcClass(EventId);
      Dem_GlobalDiagnostics_ActivationMode_ProcessEventOfDtcClassNoLongerQualifiedActive(lDTCClass);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessHealing
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessHealing(
  void
)
{
  if (Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAnyMalfunctionFailedThisDCY() == TRUE)
  {
    Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter(0);
  }
  else if (Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetAllMalfunctionsTestedThisDCY() == TRUE)
  {
    uint8 lB1HealingCounter = Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetHealingCounter();

    if (lB1HealingCounter < DEM_ACTIVATIONMODE_B1_HEALING_COUNTER_THRESHOLD)
    {
      ++lB1HealingCounter;
      Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter(lB1HealingCounter);
    }

    /* clear the B1 counter if it has healed */
    if (lB1HealingCounter >= DEM_ACTIVATIONMODE_B1_HEALING_COUNTER_THRESHOLD)
    {
      Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetValue(0);
    }
  }
  else
  {
    /* Do nothing: B1 healing counter remains unchanged */
  }

  Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY(FALSE);
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY(TRUE);
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessMalfunctionStatusAtOpCycleEnd
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
Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessMalfunctionStatusAtOpCycleEnd(
  Dem_EventIdType EventId
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    if ( (Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(EventId) == TRUE)
      && (Dem_Core_DTC_IsClassB1(EventId) == TRUE) )                                                                             /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      if (Dem_Core_TestInternalEventUdsStatusBit(EventId, Dem_UdsStatus_TFTOC) == TRUE)
      {
        Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAnyMalfunctionFailedThisDCY(TRUE);
      }

      if (Dem_Core_TestInternalEventUdsStatusBit(EventId, Dem_UdsStatus_TNCTOC) == TRUE)
      {
        Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetAllMalfunctionsTestedThisDCY(FALSE);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
% Dem_GlobalDiagnostics_ActivationMode_ProcessAfterClearAll
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
Dem_GlobalDiagnostics_ActivationMode_ProcessAfterClearAll(
  void
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_GlobalDiagnostics_ActivationMode_B1Counter_SetHealingCounter(0);

    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitCounters();
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_ACTIVATIONMODE_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ActivationMode_Implementation.h
 *********************************************************************************************************************/
