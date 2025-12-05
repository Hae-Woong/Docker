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
/*! \addtogroup Dem_EventInternalStatus
 *  \{
 *  \file       Dem_InternalStatus_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface of the EventInternalStatus subcomponent
 *  \entity     InternalStatus
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EVENTINTERNALSTATUS_TYPES_H)
#define DEM_EVENTINTERNALSTATUS_TYPES_H

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

/*! Required byte size of event internal status */
#define DEM_EVENT_INTERNAL_STATUS_BYTESIZE         DEM_1BYTE


/*! Extended DTC status after re-initialization */
#define DEM_EXT_STATUS_INITIALIZE                (0x00U)

/*! Qualification status 'all unqualified' */
#define DEM_QUALIFICATION_STATUS_ALL_UNQUALIFIED (0x77U)
/*! Qualification status 'all qualified' */
#define DEM_QUALIFICATION_STATUS_ALL_QUALIFIED   (0xFFU)


/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 /*! Type to store the internal status of an event */
typedef uint8 Dem_Event_InternalStatusType;

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/*!
 * \defgroup  Dem_Esm_StoredStatus  Encoding stored status states
 * \{
 */
 
/*! Enum to encode stored status states */
enum Dem_InternalStatus_StoredStatus
{
  Dem_InternalStatus_StoredStatus_None   = (0U),   /*!< Not stored */                                                            /* PRQA S 0779 */ /* MD_MSR_Rule5.2 */
  Dem_InternalStatus_StoredStatus_Active = (1U),   /*!< Active event entry exists */                                             /* PRQA S 0779 */ /* MD_MSR_Rule5.2 */
  Dem_InternalStatus_StoredStatus_Aging  = (2U),   /*!< Aging event entry exists */                                              /* PRQA S 0779 */ /* MD_MSR_Rule5.2 */
  Dem_InternalStatus_StoredStatus_Aged   = (3U)    /*!< Aged event entry exists */                                               /* PRQA S 0779 */ /* MD_MSR_Rule5.2 */
};
typedef enum Dem_InternalStatus_StoredStatus Dem_InternalStatus_StoredStatusType;

/*!
 * \}
 */


/*! Enum to encode event internal status bits */
/* For more details on how these bits are used, see DSGN-Dem-EventInternalStatus */
enum Dem_InternalStatus_InternalStatusBits
{
  /* Dem_InternalStatus_StoredStatus: Occupies bit 0 and 1. It is accessed using dedicated methods because it can not be accessed 
   * using the Test/Get/SetBit methods for single bit values */
  Dem_InternalStatus_SuppressedDTC = 2u,
  Dem_InternalStatus_Disconnected = 3u,
  Dem_InternalStatus_AvailabilityChanged = 4u,
  Dem_InternalStatus_FDCTOC = 5u,
  Dem_InternalStatus_AvailableInVariant = 6u,
  Dem_InternalStatus_UserControlledWIR = 7u
};

typedef enum Dem_InternalStatus_InternalStatusBits Dem_InternalStatus_InternalStatusBitsType;

/*! Enum to encode qualify status bits */
enum Dem_InternalStatus_QualifyStatusBits
{
  Dem_QualifyStatus_CDTC = 3u,    /*!< Confirmed DTC */
  Dem_QualifyStatus_WIR = 7u      /*!< Warning Indicator Requested */
};

typedef enum Dem_InternalStatus_QualifyStatusBits Dem_InternalStatus_QualifyStatusBitsType;

#endif /* DEM_EVENTINTERNALSTATUS_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_InternalStatus_Types.h
 *********************************************************************************************************************/
