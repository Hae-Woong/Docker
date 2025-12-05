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
/*! \addtogroup Dem_UDSStatus
 *  \{
 *  \file       Dem_UDSStatus_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public types of UDSStatus subcomponent
 *  \entity     UDSStatus
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_UDSSTATUS_TYPES_H)
#define DEM_UDSSTATUS_TYPES_H

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
/*! UDS status after re-initialization */
#define DEM_UDS_STATUS_INITIALIZE                (0x50U)

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Type to store the UDS status of an event */
typedef uint8 Dem_UDSStatus_StatusType;

/*! Enum to encode UDS status bits */
enum Dem_UDSStatus_UDSStatusBits
{
  Dem_UdsStatus_TF = 0u,                        /*!< Test failed */
  Dem_UdsStatus_TFTOC = 1u,                     /*!< Test failed this operation cycle*/
  Dem_UdsStatus_PDTC = 2u,                      /*!< Pending DTC */
  Dem_UdsStatus_CDTC = 3u,                      /*!< Confirmed DTC */
  Dem_UdsStatus_TNCSLC = 4u,                    /*!< Test not completed since last clear */
  Dem_UdsStatus_TFSLC = 5u,                     /*!< Test failed since last clear */
  Dem_UdsStatus_TNCTOC = 6u,                    /*!< Test not completed this operation cycle */
  Dem_UdsStatus_WIR = 7u                        /*!< Warning indicator requested */
};

typedef enum Dem_UDSStatus_UDSStatusBits Dem_UDSStatus_UDSStatusBitsType;

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#endif /* DEM_UDSSTATUS_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_UDSStatus_Types.h
 *********************************************************************************************************************/
