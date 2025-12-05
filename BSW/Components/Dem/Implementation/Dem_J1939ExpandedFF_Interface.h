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
/*!
 *  \defgroup   Dem_J1939ExpandedFF J1939ExpandedFF
 *  \{
 *  \file       Dem_J1939ExpandedFF_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     J1939ExpandedFF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_J1939EXPANDEDFF_INTERFACE_H)
#define DEM_J1939EXPANDEDFF_INTERFACE_H

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
#include "Dem_FilterData_Types.h"
#include "Dem_DTCSelector_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_J1939ExpandedFF_PublicProperties Public Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_J1939ExpandedFF_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_RecordReader_J1939ExpandedFF_GetOBDFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Return the next OBD freeze frame as J1939 Expanded freeze frame
 *
 * \details       Return the next OBD freeze frame as J1939 Expanded freeze frame
 *
 * \param[in]     FreezeFrameIteratorId
 *                The FreezeFrameIteratorId to iterate.
 * \param[out]    J1939DTC
 *                Pointer to the J1939 DTC.
 * \param[out]    DestBuffer
 *                Buffer to receive the J1939 freeze frame data.
 * \param[in,out] BufSize
 *                In:  Buffer size
 *                Out: Number of bytes written
 *
 * \return        E_OK
 *                Matching element returned in output parameters.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No more matching elements, output parameters not modified.
 * \return        DEM_PENDING
 *                The operation was started and is currently processed.
 * \return        DEM_BUFFER_TOO_SMALL
 *                The provided buffer is not large enough.
 *
 * \config        -
 * \pre           A J1939 freeze frame filter has been set up by 
 *                Dem_FreezeFrameIterator_StartJ1939Iterator
 *                OBDII must be supported in the variant
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for FreezeFrameIteratorId of different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_RecordReader_J1939ExpandedFF_GetOBDFreezeFrame(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  DestBuffer,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  BufSize
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_J1939EXPANDEDFF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_J1939ExpandedFF_Interface.h
 *********************************************************************************************************************/
