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
/*! \addtogroup Dem_Odometer
 *  \{
 *  \file       Dem_OdometerData_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details
 *  \entity     OdometerData
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ODOMETERDATA_IMPLEMENTATION_H)
#define DEM_ODOMETERDATA_IMPLEMENTATION_H

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
#include "Dem_OdometerData_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_Cfg_Declarations.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Indicator_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"

                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_ODOMETERDATA_IMPLEMENTATION_FILENAME "Dem_OdometerData_Implementation.h"

/* Bit index of the update pending flag bit in 32 bit odometer values */
#define DEM_ODOMETERDATA_UPDATEFLAG_BIT_INDEX                          (31u)

/* Bit index of the initialization pending bit in 32 bit odometer activation distance values */
#define DEM_ODOMETERDATA_ACITVATIONDISTANCE_INITIALIZEDFLAG_BIT_INDEX  (31u)

/* Bitmask excluding the most significant bit of a 32 bit value, used to retrieve odometer value without flag bit */
#define DEM_ODOMETERDATA_ODOMETER_VALUE_MASK                           (0x7FFFFFFFu)

/* Bitmask excluding everything but the most significant bit of a 32 bit value, used to retrieve flag bit in odometer values */
#define DEM_ODOMETERDATA_ODOMETER_FLAG_MASK                            (0x80000000u)

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
 * \defgroup Dem_OdometerData_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_GetPermanentStorageActivationDistance
 *****************************************************************************/
/*!
 * \brief         Get the permanent storage activation distance
 *
 * \details       Get the permanent storage activation distance
 *
 * \return        The permanent storage activation distance
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_GetPermanentStorageActivationDistance(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_GetObdOdometerDataElementIndex
 *****************************************************************************/
/*!
 * \brief         Get the index for the OBD distance information data element
 *
 * \details       Get the index for the OBD distance information data element
 *
 * \return        Index of a data element descriptor
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_GetObdOdometerDataElementIndex(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether MIL related odometer data is supported.
 *
 * \details       Tests whether MIL related odometer data is supported.
 *
 * \return        TRUE
 *                MIL related odometer data is supported.
 * \return        FALSE
 *                MIL related odometer data is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Sets the update flag for mil related odometer data.
 *
 * \details       Sets the update flag for mil related odometer data indicating
 *                that new data needs to be polled.
 *
 * \param[in]     FlagValue
 *                The value to set the update flag to.
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *                Not relevant during initialization.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(
  boolean FlagValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_TestUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Tests the update flag for mil related odometer data.
 *
 * \details       Tests the update flag for mil related odometer data indicating
 *                that new data needs to be polled.
 *
 * \return        TRUE
 *                Update flag is set.
 *                FALSE
 *                Update flag is not set.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_TestUpdateFlag(
  void
);

/* ****************************************************************************
* Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceAtMilOn
*****************************************************************************/
/*!
 * \brief         Returns the mileage travelled since the MIL is on.
 *
 * \details       Returns the mileage travelled since the MIL is on
 *                stored in the NvM admin data block.
 *
 * \return        Mileage travelled with MIL on.
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceAtMilOn(
  void
);

 /* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn
 *****************************************************************************/
/*!
 * \brief         Sets the mileage since the MIL is on.
 *
 * \details       Sets the mileage since the MIL is on in the NvM admin data.
 *
 * \param[in]     OdometerAtMalfunctionValue
 *                The value the OdometerAtMalfunctionActive admin data value 
 *                is set to.
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *                Not relevant during initialization.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(
  uint32 OdometerAtMalfunctionValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether clear related odometer data is supported.
 *
 * \details       Tests whether clear related odometer data is supported.
 *
 * \return        TRUE
 *                Clear related odometer data is supported.
 * \return        FALSE
 *                Clear related odometer data is not supported.
 *
 * \pre           -
 * 
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Sets the update flag for clear related odometer data.
 *
 * \details       Sets the update flag for clear related odometer data indicating
 *                that new data needs to be polled.
 *
 * \param[in]     FlagValue
 *                The value to set the update flag to
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *                Not relevant during initialization.
 * 
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetUpdateFlag(
  boolean FlagValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_TestUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Tests the update flag for clear related odometer data.
 *
 * \details       Tests the update flag for clear related odometer data indicating
 *                that new data needs to be polled.
 *
 * \return        TRUE
 *                Update flag is set.
 *                FALSE
 *                Update flag is not set.
 *
 * \pre           -
 * 
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_TestUpdateFlag(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceAtClear
 *****************************************************************************/
/*!
 * \brief         Returns the mileage the last clear occurred at.
 *
 * \details       Returns the mileage the last clear operation was performed at
 *                stored in the NvM admin data block.
 *
 * \return        Mileage during the last clear operation.
 *
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceAtClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetDistanceAtClear
 *****************************************************************************/
/*!
 * \brief         Sets the mileage at which the last clear occurred.
 *
 * \details       Sets the mileage at which the last clear occurred in the NvM
 *                admin data.
 *
 * \param[in]     OdometerAtClearValue
 *                The value the odometer at clear admin data value is set to.
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *                Not relevant during initialization.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetDistanceAtClear(
  uint32 OdometerAtClearValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Sets the update flag for Confirmed DTC related odometer data.
 *
 * \details       Sets the update flag for Confirmed DTC related odometer data
 *                indicating that new data needs to be polled.
 *
 * \param[in]     FlagValue
 *                The value to set the update flag to.
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *                Not relevant during initialization.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetUpdateFlag(
  boolean FlagValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_TestUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Tests the update flag for ConfirmedDTC related odometer data.
 *
 * \details       Tests the update flag for ConfirmedDTC related odometer data 
 *                indicating that new data needs to be polled.
 *
 * \return        TRUE
 *                Update flag is set.
 *                FALSE
 *                Update flag is not set.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_TestUpdateFlag(
  void
);

/* ****************************************************************************
* Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceAtConfirmedDTC
*****************************************************************************/
/*!
 * \brief         Returns the mileage travelled since first confirmed OBD DTC.
 *
 * \details       Returns the mileage travelled since first confirmed OBD DTC
 *                stored in the NvM admin data block.
 *
 * \return        Mileage travelled with first confirmed OBD DTC.
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceAtConfirmedDTC(
  void
);

 /* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC
 *****************************************************************************/
/*!
 * \brief         Sets the mileage since the first DTC was reported confirmed.
 *
 * \details       Sets the mileage since the first DTC was reported confirmed
 *                in the NvM admin data.
 *
 * \param[in]     OdometerAtMalfunctionValue
 *                The value the OdometerAtMalfunctionValue admin data value 
 *                is set to.
 *
 * \pre           Must be called within critical section NonAtomic32Bit.
 *                Not relevant during initialization.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC(
  uint32 OdometerAtMalfunctionValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ActivationDistance_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether permanent activation distance is supported
 *
 * \details       Tests whether permanent activation distance is supported
 *
 * \return        TRUE
 *                Permanent activation distance is supported
 * \return        FALSE
 *                Permanent activation distance is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ActivationDistance_SetInitializationPendingFlag
 *****************************************************************************/
/*!
 * \brief         Sets or resets the initialized flag for the activation distance
 *                value.
 *
 * \details       -
 * 
 * \param[in]     FlagValue
 *                The value to set the initialization flag to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_SetInitializationPendingFlag(
  boolean FlagValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ActivationDistance_TestInitializationPendingFlag
 *****************************************************************************/
/*!
 * \brief         Tests whether the activation distance value has been initialized.
 *
 * \details       Tests whether the activation distance value has been initialized
 *                by checking the initialized flag of the value.
 *
 * \return        TRUE
 *                Activation distance value is initialized.
 *                FALSE
 *                Activation distance value is not initialized.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_TestInitializationPendingFlag(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Set
 *****************************************************************************/
/*!
 * \brief         Sets the permanent activation distance admin data
 *
 * \details       Sets the permanent activation distance admin data
 *
 * \param[in]     PermanentActivationDistance
 *                The value to set the permanent activation distance to
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Set(
  uint32 PermanentActivationDistance
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Get
 *****************************************************************************/
/*!
 * \brief         Returns the permanent activation distance admin data
 *
 * \details       Returns the permanent activation distance admin data
 *
 * \return        Permanent activation distance
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Get(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_IsObdOdometerSupported
 *****************************************************************************/
/*!
 * \brief         Tests if the OBD odometer is supported.
 *
 * \details       Tests if the OBD odometer is supported.
 *
 * \return        TRUE
 *                OBD odometer is supported.
 * \return        FALSE
 *                OBD odometer is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_IsObdOdometerSupported(
  void
);



/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_GetPermanentStorageActivationDistance
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_GetPermanentStorageActivationDistance(                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
#if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
  return DEM_CFG_GLOBAL_PERMANENT_DTC_REL_ACTIVATION_DIST;
#else
  return 0u;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_GetObdOdometerDataElementIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_GetObdOdometerDataElementIndex(
  void
  )
{
  return Dem_Cfg_GetDataElementTableIdxOfGeneralObdInput(DEM_CFG_OBDDISTANCEINFORMATION_INDEX);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported(
  void
)
{
  return (boolean)((DEM_FEATURE_NEED_ODOMETER_MIL_ON == STD_ON)
    && (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag
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
Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(
  boolean FlagValue
)
{
#if (DEM_FEATURE_NEED_ODOMETER_MIL_ON == STD_ON)
  Dem_Cfg_AdminData.OdometerAtMalfunctionActive = Dem_Infrastructure_Utility_SetBitAtIndex(Dem_Cfg_AdminData.OdometerAtMalfunctionActive, DEM_ODOMETERDATA_UPDATEFLAG_BIT_INDEX, FlagValue);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FlagValue);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_TestUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_TestUpdateFlag(
  void
)
{
  boolean lReturnValue = FALSE;
#if (DEM_FEATURE_NEED_ODOMETER_MIL_ON == STD_ON)
  lReturnValue = Dem_Infrastructure_Utility_TestBitAtIndex(Dem_Cfg_AdminData.OdometerAtMalfunctionActive, DEM_ODOMETERDATA_UPDATEFLAG_BIT_INDEX);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceAtMilOn
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceAtMilOn(
  void
)
{
  uint32 lReturnValue = 0u;
#if (DEM_FEATURE_NEED_ODOMETER_MIL_ON == STD_ON)
  lReturnValue = (Dem_Cfg_AdminData.OdometerAtMalfunctionActive & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(
  uint32 OdometerAtMalfunctionValue
)
{
#if (DEM_FEATURE_NEED_ODOMETER_MIL_ON == STD_ON)
  Dem_Cfg_AdminData.OdometerAtMalfunctionActive &= DEM_ODOMETERDATA_ODOMETER_FLAG_MASK;
  Dem_Cfg_AdminData.OdometerAtMalfunctionActive |= (OdometerAtMalfunctionValue & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OdometerAtMalfunctionValue);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_ODOMETER_AT_CLEAR == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetUpdateFlag
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
Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetUpdateFlag(
  boolean FlagValue
)
{
#if (DEM_FEATURE_NEED_ODOMETER_AT_CLEAR == STD_ON)
  Dem_Cfg_AdminData.OdometerAtClear = Dem_Infrastructure_Utility_SetBitAtIndex(Dem_Cfg_AdminData.OdometerAtClear, DEM_ODOMETERDATA_UPDATEFLAG_BIT_INDEX, FlagValue);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FlagValue);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ClearRelated_TestUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_TestUpdateFlag(
  void
)
{
  boolean lReturnValue = FALSE;
#if (DEM_FEATURE_NEED_ODOMETER_AT_CLEAR == STD_ON)
   lReturnValue = Dem_Infrastructure_Utility_TestBitAtIndex(Dem_Cfg_AdminData.OdometerAtClear, DEM_ODOMETERDATA_UPDATEFLAG_BIT_INDEX);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceAtClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceAtClear(
  void
)
{
  uint32 lReturnValue = 0u;
#if (DEM_FEATURE_NEED_ODOMETER_AT_CLEAR == STD_ON)
  lReturnValue = (Dem_Cfg_AdminData.OdometerAtClear & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetDistanceAtClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetDistanceAtClear(
  uint32 OdometerAtClearValue
)
{
#if (DEM_FEATURE_NEED_ODOMETER_AT_CLEAR == STD_ON)
  Dem_Cfg_AdminData.OdometerAtClear &= DEM_ODOMETERDATA_ODOMETER_FLAG_MASK;
  Dem_Cfg_AdminData.OdometerAtClear |= (OdometerAtClearValue & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OdometerAtClearValue);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetUpdateFlag
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
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetUpdateFlag(
  boolean FlagValue
)
{
#if (DEM_FEATURE_NEED_ODOMETER_CONFIRMEDDTC == STD_ON)
  Dem_Cfg_AdminData.OdometerAtMalfunctionActive = Dem_Infrastructure_Utility_SetBitAtIndex(Dem_Cfg_AdminData.OdometerAtMalfunctionActive, DEM_ODOMETERDATA_UPDATEFLAG_BIT_INDEX, FlagValue);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FlagValue);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_TestUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_TestUpdateFlag(
  void
)
{
  boolean lReturnValue = FALSE;
#if (DEM_FEATURE_NEED_ODOMETER_CONFIRMEDDTC == STD_ON)
  lReturnValue = Dem_Infrastructure_Utility_TestBitAtIndex(Dem_Cfg_AdminData.OdometerAtMalfunctionActive, DEM_ODOMETERDATA_UPDATEFLAG_BIT_INDEX);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceAtConfirmedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceAtConfirmedDTC(
  void
)
{
  uint32 lReturnValue = 0u;
#if (DEM_FEATURE_NEED_ODOMETER_CONFIRMEDDTC == STD_ON)
  lReturnValue = (Dem_Cfg_AdminData.OdometerAtMalfunctionActive & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC(
  uint32 OdometerAtMalfunctionValue
)
{
#if (DEM_FEATURE_NEED_ODOMETER_CONFIRMEDDTC == STD_ON)
  Dem_Cfg_AdminData.OdometerAtMalfunctionActive &= DEM_ODOMETERDATA_ODOMETER_FLAG_MASK;
  Dem_Cfg_AdminData.OdometerAtMalfunctionActive |= (OdometerAtMalfunctionValue & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OdometerAtMalfunctionValue);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ActivationDistance_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_IsSupported(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ActivationDistance_SetInitializationPendingFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_SetInitializationPendingFlag(
  boolean FlagValue
)
{
#if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
  Dem_Cfg_AdminData.PermanentActivationDistance = Dem_Infrastructure_Utility_SetBitAtIndex(Dem_Cfg_AdminData.PermanentActivationDistance,
                                                                                           DEM_ODOMETERDATA_ACITVATIONDISTANCE_INITIALIZEDFLAG_BIT_INDEX,
                                                                                           FlagValue);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FlagValue);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ActivationDistance_TestInitializationPendingFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_TestInitializationPendingFlag(
  void
)
{
  boolean lReturnValue = FALSE;
#if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
  lReturnValue = Dem_Infrastructure_Utility_TestBitAtIndex(Dem_Cfg_AdminData.PermanentActivationDistance,
                                                           DEM_ODOMETERDATA_ACITVATIONDISTANCE_INITIALIZEDFLAG_BIT_INDEX);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Set
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Set(
  uint32 PermanentActivationDistance
)
{
#if DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON
  Dem_Cfg_AdminData.PermanentActivationDistance &= DEM_ODOMETERDATA_ODOMETER_FLAG_MASK;
  Dem_Cfg_AdminData.PermanentActivationDistance |= (PermanentActivationDistance & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(PermanentActivationDistance);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Get
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Get(
  void
)
{
  uint32 lPermanentActivationDistance = 0u;
#if DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON
  lPermanentActivationDistance = (Dem_Cfg_AdminData.PermanentActivationDistance & DEM_ODOMETERDATA_ODOMETER_VALUE_MASK);
#endif
  return lPermanentActivationDistance;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_IsObdOdometerSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_IsObdOdometerSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON);
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
 * \addtogroup Dem_OdometerData_PublicProperties
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
 * \defgroup Dem_OdometerData_Private Private Methods
 * \{
 */

 /* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_GetObdOdometer
 *****************************************************************************/
/*!
 * \brief         Requests the OBD Odometer from application and composes the
 *                returned data into an uint32 element.
 *
 * \details       Requests the OBD Odometer from application and composes the
 *                returned data into an uint32 element.
 *
 * \param[out]    Odometer
 *                Pointer to receive the value
 *
 * \return        E_OK
 *                An odometer value was successfully retrieved.
 * \return        E_NOT_OK
 *                No odometer could be retrieved. The Odometer parameter is
 *                initialized to 0.
 *
 * \pre           RTE is started (if using RTE)
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_GetObdOdometer(
  CONSTP2VAR(uint32, AUTOMATIC, DEM_VAR_NO_INIT)  Odometer
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_NeedsUpdate
 *****************************************************************************/
/*!
 * \brief         Checks whether MIL related odometer data needs to be updated.
 *
 * \details       -
 *
 * \return        TRUE
 *                The odometer MIL on value needs to be updated.
 * \return        FALSE
 *                No update of the odometer MIL on value is needed.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_NeedsUpdate(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_UpdateDistanceAtMilOn
 *****************************************************************************/
/*!
 * \brief         Update the distance travelled at which the MIL turned on.
 *
 * \details       -
 *
 * \param[in]     OBDOdometer
 *                Distance value used to set the odometer MIL on value to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_UpdateDistanceAtMilOn(
  uint32 OBDOdometer
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_NeedsUpdate
 *****************************************************************************/
/*!
 * \brief         Checks whether clear related odometer data needs to updated.
 *
 * \details       -
 *
 * \return        TRUE
 *                Clear related odometer data needs to be updated.
 * \return        FALSE
 *                Clear related odometer data doesn't need to be updated.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_NeedsUpdate(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_UpdateDistanceAtClear
 *****************************************************************************/
/*!
 * \brief         Update the distance travelled at which the last clear occurred.
 *
 * \details       -
 *
 * \param[in]     OBDOdometer
 *                Distance value used to set the odometer MIL on value to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_UpdateDistanceAtClear(
  uint32 OBDOdometer
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ActivationDistance_NeedsUpdate
 *****************************************************************************/
/*!
 * \brief         Checks whether the permanent activation distance needs to
 *                be updated.
 *
 * \details       -
 *
 * \return        TRUE
 *                Permanent activation distance handling needs to be updated.
 * \return        FALSE
 *                No updates are needed for the permanent activation distance.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_NeedsUpdate(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Update
 *****************************************************************************/
/*!
 * \brief         Updates the permanent activation distance and handles permanent
 *                memory activation.
 *
 * \details       Updates the permanent activation distance and handles the
 *                permanent memory, activating it if the activation distance
 *                has been reached.
 *
 * \param[in]     OBDOdometer
 *                The OBD odometer value used to update activation distance.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Update(
  uint32 OBDOdometer
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_NeedsUpdate
 *****************************************************************************/
/*!
 * \brief         Checks whether Confirmed DTC related odometer data needs to
 *                be updated.
 *
 * \details       -
 *
 * \return        TRUE
 *                The Confirmed DTC related odometer value needs to be updated.
 * \return        FALSE
 *                No update of the Confirmed DTC related odometer value is needed.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_NeedsUpdate(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_UpdateDistanceAtConfirmedDTC
 *****************************************************************************/
/*!
 * \brief         Update the distance travelled after first OBD DTC was reported
 *                as confirmed.
 *
 * \details       -
 *
 * \param[in]     OBDOdometer
 *                Distance value used to set the odometer value to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_UpdateDistanceAtConfirmedDTC(
  uint32 OBDOdometer
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_GetObdOdometer
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
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_GetObdOdometer(
  CONSTP2VAR(uint32, AUTOMATIC, DEM_VAR_NO_INIT)  Odometer
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;
  uint32 lOdometerValue = 0u;
  if (Dem_GlobalDiagnostics_OdometerData_IsObdOdometerSupported() == TRUE)                                                       /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_Data_DestinationBuffer lDestinationBuffer;
    Dem_Cfg_DataIndexType lDataIndex = Dem_GlobalDiagnostics_OdometerData_GetObdOdometerDataElementIndex();
    uint8 lOdometerValueBuffer[4] = { 0 };

    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(lOdometerValueBuffer[0]), 0x04);                                       /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */

    lReturnValue = Dem_FaultMemory_ExtDataElement_CollectOBDRelevantUserData(                                                    /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
      &lDestinationBuffer,
      lDataIndex);

    if (lReturnValue == E_OK)
    {
      lOdometerValue = Dem_Infrastructure_Utility_ExtractValueUint32(&lOdometerValueBuffer[0]);                                  /* SBSW_DEM_POINTER_FORWARD_STACK */
    }
  }
  *Odometer = lOdometerValue;                                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_NeedsUpdate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_NeedsUpdate(
  void
)
{
  return (boolean)((Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported() == TRUE)
    && (Dem_GlobalDiagnostics_OdometerData_MilRelated_TestUpdateFlag() == TRUE));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_UpdateDistanceAtMilOn
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
Dem_GlobalDiagnostics_OdometerData_MilRelated_UpdateDistanceAtMilOn(
  uint32 OBDOdometer
)
{
  if (Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported() == TRUE)                                                       /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_EnterCritical_NonAtomic32bit();
    if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == TRUE)
    {
      Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(OBDOdometer);
    }
    else
    {
      Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(OBDOdometer - Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceAtMilOn());
    }
    Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(FALSE);
    Dem_LeaveCritical_NonAtomic32bit();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ClearRelated_NeedsUpdate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_NeedsUpdate(
  void
)
{
  return (boolean)((Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported() == TRUE)
    && (Dem_GlobalDiagnostics_OdometerData_ClearRelated_TestUpdateFlag() == TRUE));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ClearRelated_UpdateDistanceAtClear
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
Dem_GlobalDiagnostics_OdometerData_ClearRelated_UpdateDistanceAtClear(
  uint32 OBDOdometer
)
{
  if (Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported() == TRUE)                                                     /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_EnterCritical_NonAtomic32bit();
    Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetDistanceAtClear(OBDOdometer);
    Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetUpdateFlag(FALSE);
    Dem_LeaveCritical_NonAtomic32bit();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ActivationDistance_NeedsUpdate
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_NeedsUpdate(
  void
)
{
  return (boolean)((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    && (Dem_GlobalDiagnostics_OdometerData_ActivationDistance_IsSupported() == TRUE)
    && (Dem_Memories_PermanentMemory_TestEnabled() == FALSE));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_UpdateActivationDistance
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Update(
  uint32 OBDOdometer
)
{
  uint32 lPermanentActivationDistance = Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Get();
  if (Dem_GlobalDiagnostics_OdometerData_ActivationDistance_TestInitializationPendingFlag() == TRUE)
  {
    lPermanentActivationDistance = (uint32)(OBDOdometer + Dem_GlobalDiagnostics_OdometerData_GetPermanentStorageActivationDistance());
    Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Set(lPermanentActivationDistance);
    Dem_GlobalDiagnostics_OdometerData_ActivationDistance_SetInitializationPendingFlag(FALSE);
  }
  else if (OBDOdometer > lPermanentActivationDistance)
  {
    (void)Dem_Memories_PermanentMemory_EnableStorage();
  }
  else
  {
    /* MISRA Case */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_NeedsUpdate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_NeedsUpdate(
  void
)
{
  return (boolean)((Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported() == TRUE)
    && (Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_TestUpdateFlag() == TRUE));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_UpdateDistanceAtConfirmedDTC
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
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_UpdateDistanceAtConfirmedDTC(
  uint32 OBDOdometer
)
{
  if (Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported() == TRUE)                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_EnterCritical_NonAtomic32bit();
    if (Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed())  
    {
      /* when an update is needed and there are confirmed events, it means that the first event has become confirmed */
      Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC(OBDOdometer);
    }
    else
    {
      /* when an update is needed and there are no confirmed events, the distance should be set to 0u */
      Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC(0u);
    }
    Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetUpdateFlag(FALSE);
    Dem_LeaveCritical_NonAtomic32bit();
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
 * \addtogroup Dem_OdometerData_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceSinceMilOn
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
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceSinceMilOn(                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  uint32 lOdometerSinceMilOn = DEM_DATA_MAX_OBD_ODOMETER;
  if (Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported())                                                               /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    if (Dem_GlobalDiagnostics_OdometerData_MilRelated_TestUpdateFlag() == FALSE)
    {
      uint32 lCurrentObdOdometer = 0;
      uint32 lStoredOdometerAtMilOn = 0;
  
      /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
      Dem_EnterCritical_NonAtomic32bit();
      lStoredOdometerAtMilOn = Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceAtMilOn();
      Dem_LeaveCritical_NonAtomic32bit();
      /* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */

      if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == TRUE)
      {
        if (Dem_GlobalDiagnostics_OdometerData_GetObdOdometer(&lCurrentObdOdometer) == E_OK)                                     /* SBSW_DEM_POINTER_FORWARD_STACK */
        {
          lOdometerSinceMilOn = lCurrentObdOdometer - lStoredOdometerAtMilOn;
        }
      }
      else
      {
        lOdometerSinceMilOn = lStoredOdometerAtMilOn;
      }
      if (lOdometerSinceMilOn > DEM_DATA_MAX_OBD_ODOMETER)
      {
        lOdometerSinceMilOn = DEM_DATA_MAX_OBD_ODOMETER;
      }
    }
  }
  return (uint16)lOdometerSinceMilOn;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_MilRelated_Reset
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_Reset(
  void
)
{
  /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
  Dem_EnterCritical_NonAtomic32bit();
  Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(0u);
  Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(FALSE);
  Dem_LeaveCritical_NonAtomic32bit();
  /* >>>> -------------------------------- Leave Critical Section: NonAtomic32bit */
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_GetDistanceTravelledSinceLastClear
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
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceSinceLastClear(
  void
)
{
  uint32 lOdometerSinceLastClear = DEM_DATA_MAX_OBD_ODOMETER;
  if (Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported())                                                             /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    if (Dem_GlobalDiagnostics_OdometerData_ClearRelated_TestUpdateFlag() == FALSE)
    {
      uint32 lCurrentObdOdometer = 0u;
      uint32 lOdometerAtLastClear = 0u;

      /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
      Dem_EnterCritical_NonAtomic32bit();
      lOdometerAtLastClear = Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceAtClear();
      Dem_LeaveCritical_NonAtomic32bit();
      /* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */

      if (Dem_GlobalDiagnostics_OdometerData_GetObdOdometer(&lCurrentObdOdometer) == E_OK)                                       /* SBSW_DEM_POINTER_FORWARD_STACK */
      {
        lOdometerSinceLastClear = lCurrentObdOdometer - lOdometerAtLastClear;
        if (lOdometerSinceLastClear > DEM_DATA_MAX_OBD_ODOMETER)
        {
          lOdometerSinceLastClear = DEM_DATA_MAX_OBD_ODOMETER;
        }
      }
    }
  }
  return (uint16)lOdometerSinceLastClear;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported(
  void
)
{
  return (boolean)((DEM_FEATURE_NEED_ODOMETER_CONFIRMEDDTC == STD_ON)
    && (Dem_Cfg_IsZeroEmissionObdSupportedInVariant() == TRUE));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceConfirmedDTC
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
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceConfirmedDTC(                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  uint32 lOdometerSinceConfirmedDTC = DEM_DATA_MAX_OBD_ODOMETER;
  if (Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported())                                                      /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    if (Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_TestUpdateFlag() == FALSE)
    {
      uint32 lCurrentObdOdometer = 0;
      uint32 lStoredOdometerAtConfirmedDTC = 0;
  
      /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
      Dem_EnterCritical_NonAtomic32bit();
      lStoredOdometerAtConfirmedDTC = Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceAtConfirmedDTC();
      Dem_LeaveCritical_NonAtomic32bit();
      /* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */

      if (Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed() == TRUE)
      {
        if (Dem_GlobalDiagnostics_OdometerData_GetObdOdometer(&lCurrentObdOdometer) == E_OK)                                     /* SBSW_DEM_POINTER_FORWARD_STACK */
        {
          lOdometerSinceConfirmedDTC = lCurrentObdOdometer - lStoredOdometerAtConfirmedDTC;
        }
      }
      else
      {
        lOdometerSinceConfirmedDTC = lStoredOdometerAtConfirmedDTC;
      }
      if (lOdometerSinceConfirmedDTC > DEM_DATA_MAX_OBD_ODOMETER)
      {
        lOdometerSinceConfirmedDTC = DEM_DATA_MAX_OBD_ODOMETER;
      }
    }
  }
  return (uint16)lOdometerSinceConfirmedDTC;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_Init(
  void
)
{
  if (Dem_GlobalDiagnostics_OdometerData_IsObdOdometerSupported())
  {
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_Odometer);
  }
  Dem_GlobalDiagnostics_CountConfirmedEvents();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_InitAdminData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_InitAdminData(
  void
)
{
  Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Set(0u);
  Dem_GlobalDiagnostics_OdometerData_ActivationDistance_SetInitializationPendingFlag(TRUE);

  Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(0u);
  Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(FALSE);

  Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC(0u);
  Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetUpdateFlag(FALSE);

  Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetDistanceAtClear(0u);
  Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetUpdateFlag(TRUE);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ProcessAfterClear
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
Dem_GlobalDiagnostics_OdometerData_ProcessAfterClear(
  void
)
{
  if ((Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported() == TRUE) ||
      (Dem_GlobalDiagnostics_OdometerData_ClearRelated_IsSupported() == TRUE))
  {
    /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
    Dem_EnterCritical_NonAtomic32bit();
    Dem_GlobalDiagnostics_OdometerData_ClearRelated_SetUpdateFlag(TRUE);

    if (Dem_GlobalDiagnostics_Indicator_IsGlobalMilStateSupported() == TRUE)
    {
      if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == FALSE)
      {
        Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(0);
        Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(FALSE);
      }
    }
    Dem_LeaveCritical_NonAtomic32bit();
    /* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ProcessOnMainfunction
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
Dem_GlobalDiagnostics_OdometerData_ProcessOnMainfunction(
  void
)
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    uint32 lCurrentOdometer = 0;
    boolean lUpdateOdometerAtMilOn = Dem_GlobalDiagnostics_OdometerData_MilRelated_NeedsUpdate();
    boolean lUpdateOdometerAtConfirmedDTC = Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_NeedsUpdate();
    boolean lUpdateOdometerAtClear = Dem_GlobalDiagnostics_OdometerData_ClearRelated_NeedsUpdate();
    boolean lUpdateActivationDistance = Dem_GlobalDiagnostics_OdometerData_ActivationDistance_NeedsUpdate();

    if ((lUpdateOdometerAtMilOn == TRUE) || (lUpdateOdometerAtConfirmedDTC == TRUE) || (lUpdateOdometerAtClear == TRUE)
       || (lUpdateActivationDistance == TRUE))
    {
      if (Dem_GlobalDiagnostics_OdometerData_GetObdOdometer(&lCurrentOdometer) == E_OK)                                          /* SBSW_DEM_POINTER_FORWARD_STACK */
      {
        if (lUpdateOdometerAtMilOn == TRUE)
        {
          Dem_GlobalDiagnostics_OdometerData_MilRelated_UpdateDistanceAtMilOn(lCurrentOdometer);
        }
        if (lUpdateOdometerAtConfirmedDTC == TRUE)
        {
          Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_UpdateDistanceAtConfirmedDTC(lCurrentOdometer);
        }
        if (lUpdateOdometerAtClear == TRUE)
        {
          Dem_GlobalDiagnostics_OdometerData_ClearRelated_UpdateDistanceAtClear(lCurrentOdometer);
        }
        if (lUpdateActivationDistance == TRUE)
        {
          Dem_GlobalDiagnostics_OdometerData_ActivationDistance_Update(lCurrentOdometer);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedEnabled
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
Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedEnabled(
  void
)
{
  if (Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported() == TRUE)
  {
    /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
    Dem_EnterCritical_NonAtomic32bit();
    Dem_GlobalDiagnostics_OdometerData_MilRelated_SetDistanceAtMilOn(0u);
    Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(TRUE);
    Dem_LeaveCritical_NonAtomic32bit();
    /* >>>> -------------------------------- Leave Critical Section: NonAtomic32bit */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedDisabled
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
Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedDisabled(
  void
)
{
  if (Dem_GlobalDiagnostics_OdometerData_MilRelated_IsSupported() == TRUE)
  {
    /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
    Dem_EnterCritical_NonAtomic32bit();
    Dem_GlobalDiagnostics_OdometerData_MilRelated_SetUpdateFlag(TRUE);
    Dem_LeaveCritical_NonAtomic32bit();
    /* >>>> -------------------------------- Leave Critical Section: NonAtomic32bit */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged
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
Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged(
  void
)
{
  if (Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported() == TRUE)
  {
    /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
    Dem_EnterCritical_NonAtomic32bit();
    if (Dem_GlobalDiagnostics_TestNumOfConfirmedTransitionFlag() == TRUE)
    {
      if (Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed() == FALSE)
      {
        Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetDistanceAtConfirmedDTC(0);
      }
      else
      {
        Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_SetUpdateFlag(TRUE);
      }
    }

    Dem_LeaveCritical_NonAtomic32bit();
    /* >>>> -------------------------------- Leave Critical Section: NonAtomic32bit */
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_ODOMETERDATA_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_OdometerData_Implementation.h
 *********************************************************************************************************************/
