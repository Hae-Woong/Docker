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
/*! \addtogroup Dem_J1939ExpandedFF
 *  \{
 *  \file       Dem_J1939ExpandedFF_Implementation.h
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

#if !defined (DEM_J1939EXPANDEDFF_IMPLEMENTATION_H)
#define DEM_J1939EXPANDEDFF_IMPLEMENTATION_H

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
#include "Dem_J1939ExpandedFF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Client_Interface.h"
#include "Dem_DataReport_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_FaultMemory_Interface.h"
                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_J1939EXPANDEDFF_IMPLEMENTATION_FILENAME "Dem_J1939ExpandedFF_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_J1939ExpandedFF_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_RecordReader_J1939ExpandedFF_GlobalObdIIFFRawSize
 *****************************************************************************/
/*!
 * \brief         Get the unformatted size of the OBD-II freeze frame.
 *
 * \details       Get the unformatted size of the OBD-II freeze frame.
 *
 * \return        The unformatted size of the OBD-II freeze frame
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_RecordReader_J1939ExpandedFF_GlobalObdIIFFRawSize(
  void
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_RecordReader_J1939ExpandedFF_GlobalObdIIFFRawSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_RecordReader_J1939ExpandedFF_GlobalObdIIFFRawSize(                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return (uint16)DEM_CFG_GLOBAL_OBD_IIFFRAW_SIZE;
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
 * \addtogroup Dem_J1939ExpandedFF_PublicProperties
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
 * \defgroup Dem_J1939ExpandedFF_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_J1939ExpandedFF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_RecordReader_J1939ExpandedFF_GetOBDFreezeFrame
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_RecordReader_J1939ExpandedFF_GetOBDFreezeFrame(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  DestBuffer,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  BufSize
  )
{
  Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;
  lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);

  if (lFreezeFrameFilterData.FilterState == DEM_FREEZEFRAMEITERATOR_J1939STATE_USED)
  {
    /* copy Mode02 index on the stack as it might change in the background */
    uint8  lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_GetFreezeFrameIndexMode02();
    boolean lModificationDetected = FALSE;

    /* first check if FF0 is available */
    if (   (lSelectedIndex != Dem_Memories_ObdFreezeFrameMemory_GetSize())
        && (Dem_DTC_TestDtcSuppression(Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lSelectedIndex)) == FALSE))                /* PRQA S 3415 */ /* MD_DEM_13.5_volatile */
    {
      /* Check if provided buffer is large enough */
      if (*BufSize >= Dem_RecordReader_J1939ExpandedFF_GlobalObdIIFFRawSize())
      {
        Dem_EventIdType lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lSelectedIndex);
        uint8 lUpdateState = Dem_Event_GetDataCommitState(lEventId);

        if (Dem_Event_TestDataUpdateInProgress(lUpdateState) == FALSE)
        {
          Dem_Data_DestinationBuffer lDestinationBuffer;
          Dem_DataReport_BufferIndexType lBufferIdx;                                                                             /* PRQA S 0759 */ /* MD_MSR_Union */
          lBufferIdx.FreezeFrameIndex = lSelectedIndex;
          /* Initialize a DestinationBuffer Context. */
          Dem_Data_InitDestinationBuffer(&lDestinationBuffer, DestBuffer, *BufSize);                                             /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */

          /* Copy the freeze frame data */
          Dem_RecordReader_DataReport_CopyJ1939ObdFreezeFrameData(lBufferIdx,
            &lDestinationBuffer);                                                                                                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

          if (   (lUpdateState != Dem_Event_GetDataCommitState(lEventId))
              || (Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lSelectedIndex) != lEventId))
          {
            /* concurrent access to the entry during copy process or Freeze frame eventId has changed */
            lModificationDetected = TRUE;
          }
          else
          {
            /* Only proceed if the freeze frame is still same and for the same event */
            *J1939DTC = Dem_Cfg_EventJ1939Dtc(lEventId);                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
            *BufSize = (uint8)Dem_Data_GetDestinationBufferWriteIndex(&lDestinationBuffer);                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */ /* SBSW_DEM_POINTER_FORWARD_STACK */
            *OccurrenceCounter = Dem_Client_FilterData_GetJ1939OccurrenceCounter(lEventId);                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
          }
        }
        else
        { /* concurrent access detected */
          lModificationDetected = TRUE;
        }


        if (lModificationDetected == TRUE)
        {
          /* Concurrent modification detected, return pending */
          lReturnValue = DEM_PENDING;
        }
        else
        {
          /* Set iterator to finished since only 1 OBD-II freeze frame shall be returned */
          lFreezeFrameFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_FINISHED;
          lReturnValue = E_OK;
        }
      }
      else
      {
        /* Not enough buffer available */
        lReturnValue = DEM_BUFFER_TOO_SMALL;
      }
    }
    else
    { /* OBD freezeFrame not available */
      lReturnValue = DEM_NO_SUCH_ELEMENT;
    }

    Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_J1939EXPANDEDFF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_J1939ExpandedFF_Implementation.h
 *********************************************************************************************************************/
