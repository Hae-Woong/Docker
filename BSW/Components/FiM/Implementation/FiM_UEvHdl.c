/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!        \file     FiM_UEvHdl.c
 *         \unit     UEvHdl
 *         \brief    Source for Autosar 4 module FiM - Unit Event Handling
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  
 *  see FiM.h
 *  
 **********************************************************************************************************************/

#define FIM_SOURCE

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Dem.h"
#include "FiM.h"
#include "FiM_Int.h"

#include "FiM_ErrorCheck.h"
#include "FiM_UEvHdl.h"
#include "FiM_UFidHdl.h"
#include "FiM_UGeneral.h"
#include "FiM_UInhStatHdl.h"
#include "FiM_UPendCntHdl.h"

#include "FiM_Cfg_EvHdl.h"


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/

/*! Return event id at particular position in event id list (macro is easier to read).                                */
#define FiM_UEvHdl_GetEventId_EventIdTable                         FiM_GetEventIdOfEventIdTable
/*! Return event id at particular position in event id pending list (macro is easier to read).                        */
#define FiM_UEvHdl_GetEventId_EventIdPendingTable                  FiM_GetEventIdOfEventIdPendingTable
/*! Return index into inhibition code table at particular position in event id list (macro is easier to read).        */
#define FiM_UEvHdl_GetEventToInhCodeTableIndex_EventIdTable        FiM_GetEventToInhCodeTableIndexOfEventIdTable
/*! Return count of relevant inhibition code table entries at particular position in event id list (easier to read).  */
#define FiM_UEvHdl_GetEventToInhCodeTableCount_EventIdTable        FiM_GetInhCodeCountOfEventIdTable
/*! Return satellite id of event at particular position in event id list (macro is easier to read).                   */
#define FiM_UEvHdl_GetSatelliteId_EventIdTable        (FiM_SatelliteIdType)FiM_GetSatelliteInfoTableIdxOfEventIdTable


/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/


/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/

#define FIM_START_SEC_VAR_NO_INIT_16_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Offset into event id table.
    Used if direct table access is used, i.e. event ids are in continuously ascending order.*/
FIM_LOCAL VAR(FiM_EventIdTableIndexType, FIM_VAR_NO_INIT)  FiM_UEvHdl_SearchEventIdTableDirectOffset;
#if (FIM_FEATURE_OBD == STD_ON)
/*! Offset into event id pending table.
    Used if direct table access is used, i.e. event ids are in continuously ascending order.*/
FIM_LOCAL VAR(FiM_EventIdTableIndexType, FIM_VAR_NO_INIT)  FiM_UEvHdl_SearchEventIdPendingTableDirectOffset;
#endif

#define FIM_STOP_SEC_VAR_NO_INIT_16_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_VAR_NO_INIT_UNSPECIFIED_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer that holds the used function for searching an event id in EventIdTable.         */
FIM_LOCAL FUNC(boolean, FIM_CODE) (*FiM_UEvHdl_SearchEventIdTableFct)
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);
#if (FIM_FEATURE_OBD == STD_ON)
/*! Pointer that holds the used function for searching an event id in EventIdPendingTable.  */
FIM_LOCAL FUNC(boolean, FIM_CODE) (*FiM_UEvHdl_SearchEventIdPendingTableFct)
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);
#endif

#define FIM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED_RESTRICTED
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  GLOBAL (UNIT'S PUBLIC) DATA
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  private FiM_UEvHdl_CalcInhibStatusOfEvent()
 **********************************************************************************************************************/
/*! \brief                      Internal helper function to calculate the inhibition states connected to an event.
 *  \details                    Request Monitor Status from DEM. Set or reset corresponding inhibition status bit.
 *  \param[in]  evTableIdx      Index into event id table for event of which inhibition states are to be calculated.
 *  \pre                        -
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
*******************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UEvHdl_CalcInhibStatusOfEvent(FiM_EventIdTableIndexType evTableIdx);

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  private FiM_UEvHdl_CalcPendStatusOfEvent()
 **********************************************************************************************************************/
/*! \brief                      Internal helper function to calcuate the FID pending states connected to an event.
 *  \details                    Increment, decrement or leave the counters unchanged depending on passed modification
 *                              codes. Changes all FID pending counters of an event.
 *  \param[in]  evTableIdx      Index into event id table for event of which inhibition states are to be calculated.
 *  \param[in]  modCodePending  Code whether to increment, decrement or leave the counter unchanged.
 *  \pre                        Event Id Pending table must contain at least one element apart from last dummy element.
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
***********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE)
  FiM_UEvHdl_CalcPendStatusOfEvent(FiM_EventIdTableIndexType evTableIdx, uint8 modCodePending);
#endif

/***********************************************************************************************************************
 * private FiM_UEvHdl_GetCountOfEvents_ByTableType
 **********************************************************************************************************************/
/*! \brief                        Return the number of events (except dummy ones) depending on type of eventId
 *                                table.
 *
 *  \details                      There may be several eventId tables. This function accesses FiM_GetCountOfEvents of
 *                                the table that is specified in the parameter.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       Count of events in the specified table.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_EventIdTableIndexType, FIM_CODE)
  FiM_UEvHdl_GetCountOfEvents_ByTableType(uint8 eventIdTableType);

/***********************************************************************************************************************
 * private FiM_UEvHdl_GetEventId_EventIdTable_ByTableType
 **********************************************************************************************************************/
/*! \brief                        Return event id at index position depending on type of eventId table.
 *
 *  \details                      There may be several eventId tables. This function accesses
 *                                FiM_GetEventId_EventId<...>Table of the table type that is specified in the parameter.
 *  \param[in]  index             Index in table to obtain eventId from.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       EventId in the specified table.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Dem_EventIdType, FIM_CODE)
  FiM_UEvHdl_GetEventId_EventIdTable_ByTableType(FiM_EventIdTableIndexType index, uint8 eventIdTableType);

/***********************************************************************************************************************
 *  private FiM_UEvHdl_SearchEventIdTableDirect
 **********************************************************************************************************************/
/*! \brief                        Internal helper function to find a given event id in an event id table
 *                                using direct index access.
 *
 *  \details                      There is a direct relation between the event id and it's position in the event id
 *                                table of the kind: event id = position + offset
 *                                Algorithm is determined by FiM_UEvHdl_SetSearchEventIdTableFct.
 *                                Offset is stored in FiM_SearchEventId[Pending]TableDirectOffset and was determined in
 *                                FiM_UEvHdl_SetSearchEventIdTableFct.
 *  \param[in]  eventId           Event id to be searched in event id table.
 *                                May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx        Pointer to variable where index of found event is stored.
 *                                Must point to a variable of type FiM_EventIdTableIndexType.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       TRUE  event id was found
 *                                FALSE event id was not found
 *  \pre                          Event Ids in event id table must be in continuously ascending order without gaps.
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 *  \spec                         requires evTableIdx != 0; \endspec
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UEvHdl_SearchEventIdTableDirect
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);

/***********************************************************************************************************************
 *  private FiM_UEvHdl_SearchEventIdTableLinear
 **********************************************************************************************************************/
/*! \brief                        Internal helper function to find a given event id in an event id table using linear 
 *                                search.
 *
 *  \details                      Just a dumb linear search starting at the beginning of the event id table until either
 *                                end is reached or event id is found.
 *                                Algorithm is determined by FiM_UEvHdl_SetSearchEventIdTableFct.
 *  \param[in]  eventId           Event id to be searched in event id table.
 *                                May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx        Pointer to variable where index of found event is stored.
 *                                Must point to a variable of type FiM_EventIdTableIndexType.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       TRUE  event id was found
 *                                FALSE event id was not found
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 *  \spec                         requires evTableIdx != 0; \endspec
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UEvHdl_SearchEventIdTableLinear
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);

/***********************************************************************************************************************
 *  private FiM_UEvHdl_SearchEventIdTableBinary
 **********************************************************************************************************************/
/*! \brief                        Internal helper function to find a given event id in an event id table using binary 
 *                                search.
 *
 *  \details                      Just a standard half-interval search.
 *                                Algorithm is determined by FiM_UEvHdl_SetSearchEventIdTableFct.
 *  \param[in]  eventId           Event id to be searched in event id table.
 *                                May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx        Pointer to variable where index of found event is stored.
 *                                Must point to a variable of type FiM_EventIdTableIndexType.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       TRUE  event id was found
 *                                FALSE event id was not found
 *  \pre                          Event Ids in event id table must be in ascending order.
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 *  \spec                         requires evTableIdx != 0; \endspec
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UEvHdl_SearchEventIdTableBinary
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 * private FiM_UEvHdl_GetCountOfEvents_ByTableType
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_EventIdTableIndexType, FIM_CODE)
                                                      FiM_UEvHdl_GetCountOfEvents_ByTableType(uint8 eventIdTableType)
{
  FiM_EventIdTableIndexType retVal = 0;
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    retVal = (FiM_EventIdTableIndexType)(FiM_GetSizeOfEventIdPendingTable() - (FiM_EventIdTableIndexType)1U);
  }
  else
  {
    retVal = (FiM_EventIdTableIndexType)(FiM_GetSizeOfEventIdTable() - (FiM_EventIdTableIndexType)1U);
  }
  return retVal;
}


/**********************************************************************************************************************
 *  private FiM_UEvHdl_CalcInhibStatusOfEvent()
 *********************************************************************************************************************/
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
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UEvHdl_CalcInhibStatusOfEvent(FiM_EventIdTableIndexType evTableIdx)
{
  /* #10 identify the parts of the complete inhibition config tables that are relevant for this event                 */
  /*     do so by identifying start index and stop index (last relevant element plus 1) in the complete array         */
  uint16  inhCodeTableIdx_Start = FiM_UEvHdl_GetEventToInhCodeTableIndex_EventIdTable(evTableIdx);
  uint16  inhCodeTableIdx_Stop  = (uint16)((uint16)FiM_UEvHdl_GetEventToInhCodeTableCount_EventIdTable(evTableIdx) + /* PRQA S 2985 */ /* MD_FiM_2985 */
                          inhCodeTableIdx_Start);
  FiM_SatelliteIdType   satId                 = FiM_UEvHdl_GetSatelliteId_EventIdTable(evTableIdx);
  const Dem_EventIdType cEvId                 = FiM_UEvHdl_GetEventId_EventIdTable(evTableIdx);

  Dem_MonitorStatusType monitorStatusAtBegin  = 0x00;
  Dem_MonitorStatusType monitorStatusAtEnd    = 0x00;
  Std_ReturnType        retCodeAtBegin        = E_NOT_OK;
  /* #20 get monitor status for event from DEM                                                                        */
  Std_ReturnType  retCodeAtEnd =
    Dem_GetMonitorStatus(cEvId, &monitorStatusAtEnd); /* SBSW_FIM_DEMGETMONST_CALL */ /* VCA_FIM_PASSINGLOCALPTR_CALL */

  /* #30 repeat the following until both return code and monitor status didn't change during calculation              */
  /*     (in which case we were interrupted and the basis for our calculation changed -> we have to redo it           */
  do
  {
    uint16  inhCodeIdx = inhCodeTableIdx_Start;

    retCodeAtBegin        = retCodeAtEnd;
    monitorStatusAtBegin  = monitorStatusAtEnd;
    /* #100 loop through all inhibition configurations that are connected to the event */
    for(inhCodeIdx = inhCodeTableIdx_Start; inhCodeIdx < inhCodeTableIdx_Stop; inhCodeIdx++)
    {
      /* inhibition configuration table is always generated, no manual calibration here                               */
      /* -> it does not contain invalid inhibition configurations                                                     */
      /* even if it contained an invalid event id, first element of FiM_UInhStatHdl_InhibitionModeTable would         */
      /* neutralise it                                                                                                */

      /* #110 if monitor status could be obtained */
      if (retCodeAtBegin == E_OK)
      {
        /* #120 determine the inhibition status of this particular inhibition cfg                                     */
        /* satId is taken from eventId table; as long as event id table is generated correctly, satId is smaller      */
        /* than size of satellite info table -> pre-condition for FiM_UInhStatHdl_CalcInhibitionStatusFromMonitor     */
        /* is fulfilled                                                                                               */
        FiM_UInhStatHdl_CalcInhibitionStatusFromMonitor(monitorStatusAtBegin, inhCodeIdx, satId);
      }
      /* #130 otherwise */
      else
      {
        /* #140 event is not available in DEM -> it cannot inhibit the function                                       */
        /*      (it may not be available, e.g. because SetEventAvailable was called with parameter FALSE)             */
        /* SPEC-63513 */
        (void)FiM_UInhStatHdl_ClearInhibitionStatus_Checked(inhCodeIdx, satId);
      }
      
    } /* for(inhCodeIdx = inhCodeTableIdx_Start; inhCodeIdx < inhCodeTableIdx_Stop; inhCodeIdx++)                     */

    retCodeAtEnd =
     Dem_GetMonitorStatus(cEvId, &monitorStatusAtEnd);/* SBSW_FIM_DEMGETMONST_CALL */ /* VCA_FIM_PASSINGLOCALPTR_CALL */
  } while ((monitorStatusAtEnd != monitorStatusAtBegin) || (retCodeAtEnd != retCodeAtBegin));

  {
    /* #220 notify unit UFidHdl to set event changed flag of connected FIDs as required */
    FiM_UFidHdl_SetFidEventChanged(evTableIdx);
  }

  return;
}


#if (FIM_FEATURE_OBD == STD_ON)
/**********************************************************************************************************************
 *  private FiM_UEvHdl_CalcPendStatusOfEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UEvHdl_CalcPendStatusOfEvent(FiM_EventIdTableIndexType evTableIdx,
                                                                       uint8  modCodePending)
{
  FIM_DUMMY_STATEMENT(evTableIdx); /* column EventToFidPendingTableIdx in EventIdPendingTable is optimized to define */

  /* #10 check if a manipulation of FID Pending counters is necessary */
  if (modCodePending != FIM_FID_NOCHANGE)
  {
    /* #20 if yes */
    /* #30 identify the part of the complete FID Pending table that is relevant for this event and inhibition cfg   */
    /*     do so by identifying start index and stop index (last relevant element + 1) in the complete array        */
    /* #40 loop through relevant FIDs and manipulate their Pending counters */
    uint16  evToFidTableIdx_Start = FiM_GetEventToFidPendingTableIdxOfEventIdPendingTable(evTableIdx);
    uint16  evToFidTableIdx_Stop  = FiM_GetEventToFidPendingTableIdxOfEventIdPendingTable(evTableIdx+1U);/* PRQA S 2841 */ /* MD_FiM_18.1 */
    uint16  evToFidTableIter      = evToFidTableIdx_Start;
    for (evToFidTableIter = evToFidTableIdx_Start; evToFidTableIter < evToFidTableIdx_Stop; evToFidTableIter++)/* PRQA S 2994, 2996 */ /* MD_FiM_ConfCheck */
    {
      /* #50 modify FID Pending counters according to given modification code */
      FiM_UPendCntHdl_ModPendingCounter_Checked(FiM_GetEventToFidPendingTable(evToFidTableIter), modCodePending); /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
    }
  }
}
#endif


/***********************************************************************************************************************
 * private FiM_UEvHdl_GetEventId_EventIdTable_ByTableType
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Dem_EventIdType, FIM_CODE)
  FiM_UEvHdl_GetEventId_EventIdTable_ByTableType(FiM_EventIdTableIndexType index, uint8 eventIdTableType)
{
  Dem_EventIdType eventId = FIM_DEM_EVENT_INVALID;
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    eventId = FiM_UEvHdl_GetEventId_EventIdPendingTable(index);
  }
  else
  {
    eventId = FiM_UEvHdl_GetEventId_EventIdTable(index);
  }
  return eventId;
}


/***********************************************************************************************************************
 *  private FiM_UEvHdl_SearchEventIdTableDirect
 **********************************************************************************************************************/
/*!
 *  Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UEvHdl_SearchEventIdTableDirect
                                   (Dem_EventIdType eventId /* in */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */)
{
  boolean                   retVal        = FALSE;
  FiM_EventIdTableIndexType countOfEvents = FiM_UEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);
  FiM_EventIdTableIndexType searchEventIdTableDirectOffset = 0;
# if (FIM_FEATURE_OBD == STD_OFF)
  FIM_DUMMY_STATEMENT(eventIdTableType);                                 /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#else
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    /* we're searching in event id pending list                                               */
    searchEventIdTableDirectOffset = FiM_UEvHdl_SearchEventIdPendingTableDirectOffset;
  }
  else
#endif
  {
    /* we're searching in event id list                                                       */
    searchEventIdTableDirectOffset = FiM_UEvHdl_SearchEventIdTableDirectOffset;
  }
  /* #10 if event id is within range limits */
  if ((eventId >= searchEventIdTableDirectOffset)                             &&
      (eventId <  (countOfEvents + searchEventIdTableDirectOffset)))
  {
    /* #20 event id found: subtract offset from event id, return this as index into event id table */
    (*evTableIdx) =                                                           /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
      (FiM_EventIdTableIndexType)((FiM_EventIdTableIndexType)eventId - searchEventIdTableDirectOffset);
    /* #30 set return value to TRUE */
    retVal = TRUE;
  }
  /* #40 otherwise */
  else
  {
    /* #50 event id not found: set return value to FALSE */
    retVal = FALSE;
  }
  return retVal;
}


/***********************************************************************************************************************
 *  private FiM_UEvHdl_SearchEventIdTableLinear
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UEvHdl_SearchEventIdTableLinear
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */)
{
  /* #10 set return value to FALSE */
  boolean                   retVal        = FALSE;
  FiM_EventIdTableIndexType countOfEvents = FiM_UEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);
  /* #30 loop through all indexes in event id table until end is reached or event id was found */
  for(*evTableIdx = 0;                                                        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
      *evTableIdx < countOfEvents;
      (*evTableIdx)++)                                                        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
  {
    /* #40 check whether event id at current probe index matches */
    Dem_EventIdType  currEventId  = FiM_UEvHdl_GetEventId_EventIdTable_ByTableType(*evTableIdx, eventIdTableType);
    if (currEventId == eventId)
    {
      /* #50 it matches -> event id found; return this as index into event id table and set return value to TRUE */
      retVal = TRUE;
      break;
    }
  }
  return retVal;
}


/***********************************************************************************************************************
 *  private FiM_UEvHdl_SearchEventIdTableBinary
 **********************************************************************************************************************/
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
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UEvHdl_SearchEventIdTableBinary
                                   (Dem_EventIdType eventId                                           /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx /* out */,
                                    uint8 eventIdTableType                                            /* in  */)
{
  /* #05 set return value to FALSE */
  boolean                   retVal        = FALSE;
  FiM_EventIdTableIndexType countOfEvents = FiM_UEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);

  /* #10 init BOTTOM (loIdx) and TOP (hiIdx) boundaries to first and last element in event id table */
  FiM_EventIdTableIndexType loIdx = 0;
  FiM_EventIdTableIndexType hiIdx = FiM_UEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);
  if (hiIdx != 0U)                                                        /* PRQA S 2991, 2995 */ /* MD_FiM_ConfCheck */
  {
    /* indices start at 0 */
    hiIdx--;
  }
  else
  {
    /* only the last dummy element is contained in the array -> there's no valid event id present */
    /* set loIdx to value that immediately ends function */
    loIdx = hiIdx; /* + 1U; avoid GCC warning */                                /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
    loIdx++;
  }

  /* #20 loop while bottom index smaller than top index, element not found and search index is still within bounds */
  while (loIdx <= hiIdx)
  {
    boolean         notAvail      = FALSE;
    Dem_EventIdType probeEventId  = FIM_DEM_EVENT_INVALID;

    /* #30 calculate the probe reference (in the middle of loIdx and hiIdx) */
    (*evTableIdx) = (FiM_EventIdTableIndexType)((hiIdx + loIdx) >> 1);        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */

    /* #40 if event id found */
    probeEventId  = FiM_UEvHdl_GetEventId_EventIdTable_ByTableType(*evTableIdx, eventIdTableType);
    if (probeEventId == eventId)
    {
      /* found a matching element */
      /* #50 set return value to TRUE (found) */
      retVal = TRUE;
    }
    /* #60 otherwise */
    else
    {
      /* not found a matching element */
      /* #70 determine whether lower or higher half shall be taken as interval for next try */
      /* #80 if event id found is higher than the id we're looking for */
      if (probeEventId > eventId)
      {
        /* id we're looking for is lower than the current reference */
        /* #90 Use lower half of interval for next loop, i.e. set new top index to current probe index  */
        /* #100 Make sure new index stays within array bounds, remember if it wants to get beyond       */
        if((*evTableIdx) > 0U)
        {
          hiIdx = (FiM_EventIdTableIndexType)((*evTableIdx) - 1U);
        }
        else
        { /* not available */
          notAvail = TRUE; /* found no matching element */
        }
      }
      /* #110 otherwise */
      else
      {
        /* id we're looking for is greater than the current reference */
        /* #120 Use upper half of interval for next loop, i.e. set new bottom index to current probe index */
        /* #130 Make sure new index stays within array bounds, remember if it wants to get beyond          */
        if((*evTableIdx) < (countOfEvents - 1U))                          /* PRQA S 2992, 2996 */ /* MD_FiM_ConfCheck */
        {
          loIdx = (FiM_EventIdTableIndexType)((*evTableIdx) + 1U);              /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
        }
        else
        { /* not available */
          notAvail = TRUE; /* found no matching element */
        }
      }
    }
    /* #140 break loop if element was found or index went beyond array bounds */
    /* #150 check whether bottom index is still lower than top index is done in loop condition */
    if ((retVal == TRUE) || (notAvail == TRUE))
    {
      break;
    }
  }

  return retVal;
}


/***********************************************************************************************************************
 *  public FiM_UEvHdl_SetSearchEventIdTableFct
 **********************************************************************************************************************/
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
 */
FUNC(void, FIM_CODE) FiM_UEvHdl_SetSearchEventIdTableFct(uint8 eventIdTableType)
{
  Dem_EventIdType           prevEventId  /* previous event id */
    = FiM_UEvHdl_GetEventId_EventIdTable_ByTableType(0, eventIdTableType);
  FUNC(boolean, FIM_CODE)   (*searchEventIdTableFct)
                              (Dem_EventIdType eventId                                            /* in  */,
                               P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                               uint8 eventIdTableType                                             /* in  */)
    = FiM_UEvHdl_SearchEventIdTableDirect;
  FiM_EventIdTableIndexType searchEventIdTableDirectOffset
    = prevEventId;
  FiM_EventIdTableIndexType countOfEvents = FiM_UEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);

  FiM_EventIdTableIndexType evTableIdx = 1;
  for (evTableIdx = 1; evTableIdx < countOfEvents; evTableIdx++)         /* PRQA S 2994, 2996 */ /* MD_FiM_ConfCheck */
  {
    /* current event id  */
    Dem_EventIdType currEventId = FiM_UEvHdl_GetEventId_EventIdTable_ByTableType(evTableIdx, /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
                                                                 eventIdTableType);
    /* are we still hoping for direct access? if so, check whether next id is still continuous */
    if ((searchEventIdTableFct == FiM_UEvHdl_SearchEventIdTableDirect) &&
        (currEventId != (prevEventId + 1U)))
    {
      /* not continuous anymore -> switch to binary search if list is not too small */
      /*lint -save -e506 */
      if (countOfEvents >= FIM_BINSEARCH_MINLISTLENGTH) /* PRQA S 2991, 2992, 2995, 2996, 2880 */ /* MD_FiM_ConfCheck */
      /*lint -restore */
      {
        searchEventIdTableFct = FiM_UEvHdl_SearchEventIdTableBinary;         /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
      }
      else
      {
        /* use linear algorithm for small lists (if direct access cannot be used) */
        searchEventIdTableFct = FiM_UEvHdl_SearchEventIdTableLinear;         /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
      }
    }
    /* are we hoping for binary algorithm, i.e. at least ordered event ids?                                           */
    /* no check if (FiM_UEvHdl_SearchEventIdTableFct == FiM_UEvHdl_SearchEventIdTableBinary) necessary because: */
    /* - if we're still in direct access mode (current event id <= previous event id)                                 */
    /*   always returns FALSE                                                                                         */
    /* - if we're already in linear search mode it won't do any harm; we may or may not enter the                     */
    /*   block but afterwards it will still be linear mode and we will stop                                           */
    /* Note: the next two decisions should only be true if event ids were re-assigned after                           */
    /*       FIM generation                                                                                           */
    if (currEventId <= prevEventId)
    {
      /* not ordered anymore -> switch to linear search and stop */
      searchEventIdTableFct = FiM_UEvHdl_SearchEventIdTableLinear;
    }
    prevEventId = currEventId;
    /* are we down to linear search? */
    if (searchEventIdTableFct == FiM_UEvHdl_SearchEventIdTableLinear)
    {
      /* we're down to linear search -> stop the search for the optimal algorithm - it won't get any better */
      break;
    }
  }
#if (FIM_FEATURE_OBD == STD_ON)
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    /* set search parameters for pending event id list                                            */
    FiM_UEvHdl_SearchEventIdPendingTableFct          = searchEventIdTableFct;
    FiM_UEvHdl_SearchEventIdPendingTableDirectOffset = searchEventIdTableDirectOffset;
  }
  else
#endif
  {
    /* set search parameters for inhibition event id list                                         */
    FiM_UEvHdl_SearchEventIdTableFct           = searchEventIdTableFct;
    FiM_UEvHdl_SearchEventIdTableDirectOffset  = searchEventIdTableDirectOffset;
  }
}

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  GLOBAL (UNIT'S PUBLIC) FUNCTIONS
 *********************************************************************************************************************/

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  external public FiM_UEvHdl::FiM_DemTriggerOnMonitorStatus
 **********************************************************************************************************************/
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
*/
FUNC(void, FIM_CODE)  FiM_DemTriggerOnMonitorStatus
                       (Dem_EventIdType EventId
                       )                                                                               /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                                           /* PRQA S 2981 */ /* MD_FiM_2.2 */

  /* #10 assert that FIM is at least pre-initialized otherwise search functions are not yet determined */
  /* SilentBSW: check is silence relevant */
  if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusSatCumulated() == FIM_UNINITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (FiM_UGeneral_GetInitializationStatusSatCumulated() == FIM_UNINITIALIZED) */
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #20 FIM is at least pre-initialized (or error detection is not configured) */
  else
  {
    boolean                   evFound     = FALSE;
    FiM_EventIdTableIndexType evTableIdx  = 0;
    /* #30 if passed event id is not FIM_DEM_EVENT_INVALID */
    if (EventId != FIM_DEM_EVENT_INVALID)
    {
      /* #40 search for event (i.e. is event known/configured in FIM?) */
      /* Silent:
          SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR:
                    - pointer to local stack variale of type FiM_EventIdTableIndexType
          SBSW_FIM_SEARCHIDFUNC_PTR:
                    - initialization status is checked above
                    - in FiM_Init function pointer is always set */
      evFound = (*FiM_UEvHdl_SearchEventIdTableFct)    /* SBSW_FIM_SEARCHIDFUNC_PTR */ /* VCA_FIM_SEARCHIDFUNC_PTR */
                         (EventId, &evTableIdx, FIM_EVENTID_TABLE);
    }
    /* #50 if event was found */
    if (evFound == TRUE) /* not invalid and found */
    {
      /* #60 now we need to check if this FIM satellite is initialized otherwise skip rest of function (SPEC-31918) */
      FiM_SatelliteIdType satId = FiM_UEvHdl_GetSatelliteId_EventIdTable(evTableIdx);
      if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusSat(satId) != FIM_INITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
      {
        /* (FIM_DEV_ERROR_DETECT == STD_ON) && (FiM_UGeneral_GetInitializationStatusSat(satId) != FIM_INITIALIZED) */
        errorId = FIM_E_UNINIT;                                                   /* PRQA S 2983 */ /* MD_FiM_2.2conf */
      }
      /* #70 FIM satellite is initialized (or error detection is not configured) */
      else
      {
        /* #90 calculate the inhibition states of this event */
        FiM_UEvHdl_CalcInhibStatusOfEvent(evTableIdx);
      }
    } /* if (evFound == TRUE) */
  }

  FiM_ErrorCheck_ReportError(FIM_SID_FIM_DEMTRIGGERONMONITORSTATUS, errorId);

  return;
}


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  external public FiM_UEvHdl::FiM_DemTriggerOnEventStatus
 **********************************************************************************************************************/
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
FUNC(void, FIM_CODE)  FiM_DemTriggerOnEventStatus
                       (Dem_EventIdType EventId,
                        Dem_UdsStatusByteType EventStatusOld,
                        Dem_UdsStatusByteType EventStatusNew
                       )                                                                               /*lint -e{529} */
{
  uint8 errorId = FIM_E_NO_ERROR;                                                     /* PRQA S 2981 */ /* MD_FiM_2.2 */
  
  /* #10 if FIM is not initialized skip rest of function */
  /* SlientBSW: check is silence relevant */
  if (FiM_ErrorCheck_IsDevErrorDetected(FiM_UGeneral_GetInitializationStatusMaster() != FIM_INITIALIZED) == TRUE) /* PRQA S 2992, 2996, 4404 */ /* MD_FiM_ConfCheck, MD_FiM_ConfCheck, MD_MSR_AutosarBoolean */
  {
    /* (FIM_DEV_ERROR_DETECT == STD_ON) && (FiM_UGeneral_GetInitializationStatusMaster != FIM_INITIALIZED) */
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #20 otherwise */
  else
  {
    boolean                   pendingStatusNew = FALSE;
    boolean                   pendingStatusOld = FALSE;
    uint8                     modCode          = FIM_FID_DEC;
    /* #30 if passed event id is not FIM_DEM_EVENT_INVALID */
    if (EventId != FIM_DEM_EVENT_INVALID)
    {
      if((EventStatusNew & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
      {
        pendingStatusNew = TRUE;
        modCode          = FIM_FID_INC;
      }
      if((EventStatusOld & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
      {
        pendingStatusOld = TRUE;
      }
    }

    /* #40 if the pending status bit changed (we're only interested in changes of this status bit)  */
    /* for FIM_DEM_EVENT_INVALID pendingStatusNew and pendingStatusOld are both FALSE               */
    if (pendingStatusNew != pendingStatusOld)
    {
      FiM_EventIdTableIndexType evTableIdx  = 0;
      /* #50 search for event (i.e. is event known/configured in FIM?) */
      /* Silent:
          SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR:
                    - pointer to local stack variale of type FiM_EventIdTableIndexType
          SBSW_FIM_SEARCHPENDINGIDFUNC_PTR, VCA_FIM_SEARCHPENDINGIDFUNC_PTR:
                    - initialization status is checked above
                    - in FiM_Init function pointer is always set when FIM_FEATURE_OBD == STD_ON 
                    - this function (FiM_DemTriggerOnEventStatus) is only present when FIM_FEATURE_OBD == STD_ON */
      boolean evFound = (*FiM_UEvHdl_SearchEventIdPendingTableFct) /* SBSW_FIM_SEARCHPENDINGIDFUNC_PTR */ /* VCA_FIM_SEARCHPENDINGIDFUNC_PTR */
                         (EventId, &evTableIdx, FIM_EVENTIDPENDING_TABLE);
      /* #60 if event was found */
      if (evFound == TRUE)
      {
        /* #80 calculate the pending status FIDs for this event */
        /*     (does not depend on inhibition cfg, just linked to dummy inhibition cfg) */
        FiM_UEvHdl_CalcPendStatusOfEvent(evTableIdx, modCode);
      } /* if (evFound == TRUE) */
    } /* if (pendingStatusNew != pendingStatusOld) */
  }

  FiM_ErrorCheck_ReportError(FIM_SID_FIM_DEMTRIGGERONEVENTSTATUS, errorId);

  return;
}
#endif


/**********************************************************************************************************************
  *  public FiM_UEvHdl_CalcInhStates()
  *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
FUNC(void, FIM_CODE)  FiM_UEvHdl_CalcInhStates(FiM_SatelliteIdType satId)
{
  FiM_EventIdTableIndexType     evTableIdx  = 0;

  /* #10 reset all inhibition states of satellite to 0 */
  (void)FiM_UInhStatHdl_ClearInhibitionStatusAll_Checked(satId);

  /* #30 loop through all events in FiM_EventIdTable starting at first one until the end of the table is reached */
  for(evTableIdx = 0; evTableIdx < FiM_UEvHdl_GetCountOfEvents_ByTableType(FIM_EVENTID_TABLE); evTableIdx++)
  {
    /* event id table is always generated, no manual calibration here -> it does not contain invalid event ids */

    /* #40 calculate the inhibition states of this event if it is mapped to the satellite */
    if (FiM_UEvHdl_GetSatelliteId_EventIdTable(evTableIdx) == satId) /* only for events of our own satellite */
    {
      FiM_UEvHdl_CalcInhibStatusOfEvent(evTableIdx);
    }
  } /* for(...; evTableIdx < FiM_UEvHdl_GetCountOfEvents_ByTableType(); ...) */

  return;
}


/**********************************************************************************************************************
 *  public FiM_UEvHdl_CalcPendStates()
 *********************************************************************************************************************/
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
FUNC(void, FIM_CODE)  FiM_UEvHdl_CalcPendStates(void)
{
  /* #10 if OBD is configured */
#if (FIM_FEATURE_OBD == STD_ON)
  FiM_EventIdTableIndexType     evTableIdx  = 0;

  /* #20 reset all pending counters to 0 */
  FiM_UPendCntHdl_ClearPendingCounterAll();

  /* #30 loop through all events in FiM_EventIdPendingTable starting at the first one until the end of table is reached */
  for(evTableIdx = 0;
      evTableIdx < FiM_UEvHdl_GetCountOfEvents_ByTableType(FIM_EVENTIDPENDING_TABLE); /* PRQA S 2994, 2996 */ /* MD_FiM_ConfCheck */
      evTableIdx++)
  {
    /* event id table is always generated, no manual calibration here -> it does not contain invalid event ids */
    {
      Dem_UdsStatusByteType       eventStatus = 0x00;
      /* #50 get event status from DEM for current event id */
      Std_ReturnType  retValue = Dem_GetEventUdsStatus(FiM_UEvHdl_GetEventId_EventIdPendingTable(evTableIdx), &eventStatus);/* PRQA S 2880 */ /* MD_FiM_ConfCheck */   /* SBSW_FIM_DEMGETEVENTUDSSTATUS_CALL */ /* VCA_FIM_PASSINGLOCALPTR_CALL */
      /* #60 if event status could be obtained (Dem_GetEventUdsStatus returned E_OK) */
      if(retValue == E_OK)
      {
        uint8   modCode = FIM_FID_NOCHANGE;
        /* #70 determine whether FID pending counter has to be incremented */
        if((eventStatus & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
        {
           modCode = FIM_FID_INC;
        }
        /* #80 calculate the pending status FIDs for this event */
        FiM_UEvHdl_CalcPendStatusOfEvent(evTableIdx, modCode);
      } /* if(retValue == E_OK) */
      /* #210 otherwise (Dem_GetEventUdsStatus did not return E_OK) */
      else
      {
        /* #220 Dem_GetEventUdsStatus returned an error: ignore event for calculation */
      } /* else if(returnValue == E_OK) */
    }
  } /* for(...; evTableIdx < FiM_GetCountOfEvents_ByTableType(); ...) */
#endif

  return;
}


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
*  END OF FILE: FiM_UEvHdl.c
***********************************************************************************************************************/
