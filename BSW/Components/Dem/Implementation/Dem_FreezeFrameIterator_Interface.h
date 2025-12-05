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
 *  \defgroup   Dem_FreezeFrameIterator FreezeFrame Iterator
 *  \{
 *  \file       Dem_FreezeFrameIterator_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) Interface declaration file
 *  \entity     FreezeFrameIterator
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_FREEZEFRAMEITERATOR_INTERFACE_H)
#define DEM_FREEZEFRAMEITERATOR_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_FreezeFrameIterator_Types.h"
#include "Dem_FaultMemory_Types.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DEM_START_SEC_CODE
# include "Dem_MemMap.h"                                                                                                         /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_FreezeFrameIterator_Public Public Methods
 * \{
 */

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) )
/* ****************************************************************************
 * Dem_FreezeFrameIterator_InitDefault
 *****************************************************************************/
/*!
 * \brief         Initialize a FreezeFrameIterator.
 *
 * \details       Initializes a FreezeFrameIterator, the resulting filter
 *                cannot be used by GetNextFilteredRecord without prior
 *                calling Dem_SetFreezeFrameRecordFilter.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON ||
 *                DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FreezeFrameIteratorId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_InitDefault(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );
#endif

# if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_TestFilterSet
 *****************************************************************************/
/*!
 * \brief         Test FreezeFrameIterator for containing valid filter setting.
 *
 * \details       Test that freeze frame filter has been set.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        TRUE
 *                The freeze frame filter can be used.
 * \return        FALSE
 *                The freeze frame filter is not initialized.
 *                
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FreezeFrameIteratorId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_TestFilterSet(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );
#endif

# if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_TestJ1939FreezeFrameFilterSet
 *****************************************************************************/
/*!
 * \brief         Test if a J1939 freeze frame filter has been set.
 *
 * \details       Test if a J1939 freeze frame filter has been set.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        TRUE
 *                A J1939 freeze frame filter has been set.
 * \return        FALSE
 *                No J1939 freeze frame filter has been set.
 *                
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FreezeFrameIteratorId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_TestJ1939FreezeFrameFilterSet(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 * Dem_FreezeFrameIterator_StartIterator
 *****************************************************************************/
/*!
 * \brief         Initialize (start) FreezeFrameIterator with given criteria.
 *
 * \details       Initialize (start) FreezeFrameIterator with given criteria.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[in]     FilterDTCFormat
 *                Defines the output-format of the requested DTC values.
 *
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 * \pre           -
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_StartIterator(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONST(Dem_FreezeFrameIterator_DTCFormatType, AUTOMATIC)  FilterDTCFormat
  );
# endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNext
 *****************************************************************************/
/*!
 * \brief         Initialize (start) FreezeFrameIterator with given criteria.
 *
 * \details       Initialize (start) FreezeFrameIterator with given criteria.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[out]    DTC
 *                With return value TRUE: the next filtered DTC number.
 * \param[out]    RecordNumber
 *                With return value TRUE: the next filtered RecordNumber
 *                number of the DTC.
 *
 * \return        TRUE
 *                Returned next filtered element.
 * \return        FALSE
 *                No further element (matching the filter criteria) found,
 *                iteration is complete.
 *
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                && ((DEM_CFG_SUPPORT_SRECS == STD_ON) 
 *                   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
 * \pre           -
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_GetNext(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  P2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                                   /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber                                                                            /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  );
# endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNumberOfFreezeFrameRecords
 *****************************************************************************/
/*!
 * \brief         Gets the total number of stored FreezeFrame records
 *
 * \details       Gets the number of stored FreezeFrame records
 *
 * \return        Number of stored records
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FreezeFrameIterator_GetNumberOfFreezeFrameRecords(
  void
  );
# endif

/* ****************************************************************************
 * Dem_FreezeFrameIterator_StartJ1939Iterator
 *****************************************************************************/
/*!
 * \brief         Initialize the J1939 freeze frame iterator with given criteria.
 *
 * \details       Initialize the J1939 freeze frame iterator with given criteria.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[in]     FreezeFrameKind
 *                Kind of freeze frames to be filtered.
 * \param[in]     NodeIndex
 *                The node Id.
 *
 * \config        -
 *
 * \pre           -
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_StartJ1939Iterator(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONST(Dem_J1939DcmSetFreezeFrameFilterType, AUTOMATIC)  FreezeFrameKind,
  CONST(uint8, AUTOMATIC)  NodeIndex
  );

# if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNextJ1939FreezeFrame
 *****************************************************************************/
/*!
 * \brief         Search and retrieve the next J1939 Freeze Frame.
 *
 * \details       Search and retrieve the next J1939 Freeze Frame.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[out]    J1939DTC
 *                Pointer to the J1939DTC .
 * \param[out]    OccurrenceCounter
 *                Pointer to the OccurrenceCounter buffer.
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
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 * \pre           A J1939 freeze frame filter has been set up by 
 *                Dem_FreezeFrameIterator_StartJ1939Iterator
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FreezeFrameIterator_GetNextJ1939FreezeFrame(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)    FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  DestBuffer,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  BufSize 
  );
#endif

/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNextSPNInFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Return the Identifier and Length of next SPN in freeze frame
 *
 * \details       Return the Identifier and Length of next SPN in freeze frame
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[out]    SPN
 *                Buffer to store Id of next SPN
 * \param[out]    SPNDataLength
 *                Buffer to store length of next SPN
 *
 * \return        E_OK
 *                SPN for the next J1939 Expanded Freeze Frame is 
 *                sucessfully retrieved
 *
 * \return        E_NOT_OK
 *                SPN for the next J1939 Expanded Freeze Frame was not
 *                sucessfully retrieved
 *
 * \pre           J1939 Expanded Freeze Frame is supported
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE for different FreezeFrameIteratorId
 * \reentrant     TRUE for different FreezeFrameIteratorId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FreezeFrameIterator_GetNextSPNInFreezeFrame(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA) SPN,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  SPNDataLength
  );

/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetFreezeFrameKind
 *****************************************************************************/
/*!
 * \brief         Get filter freeze frame kind of Freeze frame iterator handle.
 *
 * \details       Get filter freeze frame kind of Freeze frame iterator handle.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        FreezeFrameKind
 *                Freeze frame kind of the iterator handle
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmSetFreezeFrameFilterType, DEM_CODE)
Dem_FreezeFrameIterator_GetFreezeFrameKind(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId
  );


/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetFreezeFrameFilterData()
 *****************************************************************************/
/*!
 * \brief         Get the filter data of a freeze frame iterator.
 *
 * \details       Get the filter data of a freeze frame iterator.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.

 * \return        The filter data of the freeze frame iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_FreezeFrameIterator_FilterType, DEM_CODE) 
Dem_FreezeFrameIterator_GetFreezeFrameFilterData(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );

/* ****************************************************************************
 * Dem_FreezeFrameIterator_SetFreezeFrameFilterData()
 *****************************************************************************/
/*!
 * \brief         Set the filter data of a freeze frame iterator.
 *
 * \details       Set the filter data of a freeze frame iterator.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[in]     FreezeFrameFilterData
 *                The freeze frame filter data to be set.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_FreezeFrameIterator_SetFreezeFrameFilterData(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC)  FreezeFrameFilterData
  );


/*!
 * \}
 */
# define DEM_STOP_SEC_CODE
# include "Dem_MemMap.h"                                                                                                         /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_FREEZEFRAMEITERATOR_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_FreezeFrameIterator_Interface.h
 *********************************************************************************************************************/
