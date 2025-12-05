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
/*! \addtogroup Dem_StatusIndicator
 *  \{
 *  \file       Dem_StatusIndicator_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \entity     StatusIndicator
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_STATUSINDICATOR_TYPES_H)
#define DEM_STATUSINDICATOR_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
/*! Handle for StatusIndicator actions set on satellite, read on master*/

/*!
 * \defgroup  Dem_StatusIndicator_QueueStates  Macros encoding StatusIndicator queue states
 * Macros encoding StatusIndicator queue state
 * \{
 */
# define DEM_STATUSINDICATOR_QUEUE_INITIAL                   ((uint8)(0x00U))   /*!< Initial state */
# define DEM_STATUSINDICATOR_QUEUE_SYMPTOM_REACHED           ((uint8)(0x04U))   /*!< Set StatusIndicator symptom shall be executed */
/*!
 * \}
 */

/*!
* \defgroup  Dem_StatusIndicator_ResetValue  Macros encoding StatusIndicator reset value
* Macros encoding StatusIndicator reset value
* \{
*/
# define DEM_STATUSINDICATOR_RESET_VALUE                   ((uint8)(0x00U))   /*!< Reset value */
/*!
* \}
*/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Type to store the StatusIndicator status of an event */
typedef uint8 Dem_StatusIndicator_StatusType;

/*! Type to store the StatusIndicator queued action */
typedef uint8 Dem_StatusIndicator_QueueActionType;

/*! Enum to encode status indocator status bits */
enum Dem_StatusIndicator_StatusIndicatorBits
{
  Dem_StatusIndicator_UDTC = 0u,       /*! Bit index in StatusIndicator byte indicating Unconfirmed DTC */
  Dem_StatusIndicator_UDTCTOC = 1u,    /*! Bit index in StatusIndicator byte indicating Unconfirmed DTC This Operation Cycle */
  Dem_StatusIndicator_UDTCSLC = 2u,    /*! Bit index in StatusIndicator byte indicating Unconfirmed DTC Since Last Clear */
  Dem_StatusIndicator_ADTC = 3u,       /*! Bit index in StatusIndicator byte indicating Aged DTC */
  Dem_StatusIndicator_SSLC = 4u,       /*! Bit index in StatusIndicator byte indicating Symptom Since Last Clear */
  Dem_StatusIndicator_WIRSLC = 5u,     /*! Bit index in StatusIndicator byte indicating Warning Indicator Since Last Clear */
  Dem_StatusIndicator_ERDTC = 6u,      /*! Bit index in StatusIndicator byte indicating Emission Related DTC */
  Dem_StatusIndicator_TFSLC = 7u       /*! Bit index in StatusIndicator byte indicating Test Failed Since Last Clear */
};

typedef enum Dem_StatusIndicator_StatusIndicatorBits Dem_StatusIndicator_StatusIndicatorBitsType;

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#endif /* DEM_STATUSINDICATOR_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_StatusIndicator_Types.h
 *********************************************************************************************************************/
