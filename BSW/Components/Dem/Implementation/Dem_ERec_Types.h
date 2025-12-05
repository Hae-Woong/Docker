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
/*! \addtogroup Dem_ERec
 *  \{
 *  \file       Dem_ERec_Types.h
 *  \brief      Diagnostic Event Manager (Dem) Type and Macro definition file
 *  \entity     ERec
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EREC_TYPES_H)
#define DEM_EREC_TYPES_H

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
#define  DEM_DCM_EREC_DATACOLLECTIONTABLE_INVALID  0u    /*!< Value for an invalid DataCollectionTable index */
#define  DEM_EREC_OBDONUDS_ERECNUM_IUMPR           0x91u /*!< Extended data record number to request DTC-based IUMPR */
#define  DEM_EREC_OBDONUDS_ERECNUM_DTR             0x92u /*!< Extended data record number to request DTR data*/
#define  DEM_EREC_OBDONUDS_ERECNUM_MAR             0x93u /*!< Extended data record number to request MAR data*/

/*!
 * \defgroup  Dem_ERec_ExtendedDataRecord_Value Extended Data Records
 * List of special extended data record values
 * \{
 */
#define DEM_DCM_EXTENDEDDATARECORD_INVALID         (0x00u)     /*! Invalid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST           (0x01u)     /*! The first valid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST_OBD       (0x90u)     /*! The first valid OBD extended record */
#define DEM_DCM_EXTENDEDDATARECORD_LAST            (0xEFu)     /*! The last valid extended record */
#define DEM_DCM_EXTENDEDDATARECORD_LAST_OBD        (0x9Fu)     /*! The last valid OBD extended record */
#define DEM_DCM_EXTENDEDDATARECORD_FIRST_RESERVED  (0xF0u)     /*! The first reserved extended record */
#define DEM_DCM_EXTENDEDDATARECORD_OBD             (0xFEu)     /*! Select all OBD records */
#define DEM_DCM_EXTENDEDDATARECORD_ALL             (0xFFu)     /*! Select all extended records */
/*!
 * \}
 */
/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

 /*! This used for ExtendedDataIterator handles. */
typedef uint8  Dem_Dcm_ERec_0x1916_RecordNumberFilter_HandleType;

/*! This used for iteration over ERecs, upon the EventTable_ERec2EventIdInd table. */
typedef Dem_Cfg_ComplexIterType  Dem_Dcm_ERec_ERecNumberIterType;

/*! This used for iteration over ERecs, upon the DataCollection table. */
typedef Dem_Cfg_ComplexIterType  Dem_Dcm_ERec_DataCollectionERecIterType;

#endif /* DEM_EREC_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ERec_Types.h
 *********************************************************************************************************************/
