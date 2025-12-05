/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SoAd_EventQueue.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component EventQueue.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

/* PRQA S 4304 EOF */ /* MD_MSR_AutosarBoolean */

#define SOAD_EVENT_QUEUE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_EventQueue.h"
#include "SoAd_Util.h"
#include "SoAd_Priv.h"

#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (SOAD_LOCAL) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL static
#endif

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
  SoAd_EventQueue_GetEventQueueInfo()
**********************************************************************************************************************/
/*! \brief      Returns information on a specific event queue of a specific instance identified by name and
 *              EventQueueIdentIdx.
 *  \details    Refer to "Concept for EventQueue" in SoAd_EventQueue.c.
 *  \pre        -
 *  \param[in]  EventQueue          Name of the event queue.
 *                                  [range:  SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE .. SOAD_EVENT_QUEUE_SO_CON_STATE]
 *  \param[in]  EventQueueIdentIdx  Index of the event queue ident.
 *                                  [range: EventQueueIdentIdx < SoAd_GetSizeOfEventQueueIdent()]
 *  \param[in]  PartitionIdx        Partition index.
 *                                  [range: PartitionIdx < SoAd_GetSizeOfPartitionIdentifiers()]
 *  \param[out] EventQueueParamPtr  Pointer to the event queue parameter struct with all updated values.
 *  \return     E_OK                Event queue does exist.
 *  \return     E_NOT_OK            Event queue does not exist.
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_EventQueue_GetEventQueueInfo(
  SoAd_EventQueueEnumType EventQueue,
  SoAd_SizeOfEventQueueIdentType EventQueueIdentIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_EventQueueParamType, AUTOMATIC, SOAD_APPL_VAR) EventQueueParamPtr);

/**********************************************************************************************************************
  SoAd_EventQueue_AddHandleIdx()
**********************************************************************************************************************/
/*! \brief      Adds handle index to a specific event queue.
 *  \details    Refer to "Concept for EventQueue" in SoAd_EventQueue.c.
 *  \pre        -
 *  \param[in]  EventQueue    Name of the event queue.
 *                            [range:  SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE .. SOAD_EVENT_QUEUE_SO_CON_STATE]
 *  \param[in]  WriteIdx      Index of event queue to write handle index.
 *                            [range: WriteIdx <  SoAd_GetSizeOf<EventQueue>()]
 *  \param[in]  HandleIdx     Handle index (e.g. socket connection index).
 *                            [range: HandleIdx < SoAd_GetSizeOf<HandleType>()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < SoAd_GetSizeOfPartitionIdentifiers()]
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_EventQueue_AddHandleIdx(
  SoAd_EventQueueEnumType EventQueue,
  uint32 WriteIdx,
  uint32 HandleIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
  SoAd_EventQueue_GetHandleIdx()
**********************************************************************************************************************/
/*! \brief      Gets oldest handle index of a specific event queue.
 *  \details    Refer to "Concept for EventQueue" in SoAd_EventQueue.c.
 *  \pre        -
 *  \param[in]  EventQueue           Name of the event queue.
 *                                   [range:  SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE .. SOAD_EVENT_QUEUE_SO_CON_STATE]
 *  \param[in]  EventQueueMgmtIdx    Index of the event queue management structure.
 *  \param[in]  EventQueueStartIdx   First index of the event queue.
 *  \param[in]  PartitionIdx         Partition index.
 *                                   [range: PartitionIdx < SoAd_GetSizeOfPartitionIdentifiers()]
 *  \param[out] HandleIdxPtr         Pointer to the handle index of next element (e.g. socket connection index).
 *                                   [range: HandleIdx < SoAd_GetSizeOf<HandleType>()]
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_EventQueue_GetHandleIdx(
  SoAd_EventQueueEnumType EventQueue,
  uint32 EventQueueMgmtIdx,
  uint32 EventQueueStartIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_EventQueue_GetEventQueueInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_EventQueue_GetEventQueueInfo(
  SoAd_EventQueueEnumType EventQueue,
  SoAd_SizeOfEventQueueIdentType EventQueueIdentIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_EventQueueParamType, AUTOMATIC, SOAD_APPL_VAR) EventQueueParamPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get event queue information of a specific event queue in case it exists. */
  switch (EventQueue)
  {
    case SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE:
    {
      if ( SoAd_IsEventQueueIfUdpPduRouteConfigUsedOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx) )
      {
        SoAd_SizeOfEventQueueIfUdpPduRouteConfigType eveQueCfgIdx =
          SoAd_GetEventQueueIfUdpPduRouteConfigIdxOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx);

        EventQueueParamPtr->EventQueueStartIdx =                                                                       /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueIfUdpPduRouteStartIdxOfEventQueueIfUdpPduRouteConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueEndIdx =                                                                         /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueIfUdpPduRouteEndIdxOfEventQueueIfUdpPduRouteConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueFlagOffset = SoAd_GetFlagOffsetOfEventQueueIfUdpPduRouteConfig(eveQueCfgIdx,     /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->EventQueueMgmtIdx = SoAd_GetEventQueueMgmtIdxOfEventQueueIfUdpPduRouteConfig(eveQueCfgIdx, /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->BitPattern = SOAD_EVENT_QUEUE_BIT_PATTERN_IF_UDP_PDU_ROUTE;                                /* SBSW_SOAD_POINTER_WRITE */
        retVal = E_OK;
      }
      break;
    }
    case SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP:
    {
      if ( SoAd_IsEventQueueIfTxRouteGrpConfigUsedOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx) )
      {
        SoAd_SizeOfEventQueueIfTxRouteGrpConfigType eveQueCfgIdx =
          SoAd_GetEventQueueIfTxRouteGrpConfigIdxOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx);

        EventQueueParamPtr->EventQueueStartIdx =                                                                       /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueIfTxRouteGrpStartIdxOfEventQueueIfTxRouteGrpConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueEndIdx =                                                                         /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueIfTxRouteGrpEndIdxOfEventQueueIfTxRouteGrpConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueFlagOffset = SoAd_GetFlagOffsetOfEventQueueIfTxRouteGrpConfig(eveQueCfgIdx,      /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->EventQueueMgmtIdx = SoAd_GetEventQueueMgmtIdxOfEventQueueIfTxRouteGrpConfig(eveQueCfgIdx,  /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->BitPattern = SOAD_EVENT_QUEUE_BIT_PATTERN_IF_TX_ROUTE_GRP;                                 /* SBSW_SOAD_POINTER_WRITE */
        retVal = E_OK;
      }
      break;
    }
    case SOAD_EVENT_QUEUE_TP_TX_SO_CON:
    {
      if ( SoAd_IsEventQueueTpTxSoConConfigUsedOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx) )
      {
        SoAd_SizeOfEventQueueTpTxSoConConfigType eveQueCfgIdx =
          SoAd_GetEventQueueTpTxSoConConfigIdxOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx);

        EventQueueParamPtr->EventQueueStartIdx =                                                                       /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueTpTxSoConStartIdxOfEventQueueTpTxSoConConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueEndIdx =                                                                         /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueTpTxSoConEndIdxOfEventQueueTpTxSoConConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueFlagOffset = SoAd_GetFlagOffsetOfEventQueueTpTxSoConConfig(eveQueCfgIdx,         /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->EventQueueMgmtIdx = SoAd_GetEventQueueMgmtIdxOfEventQueueTpTxSoConConfig(eveQueCfgIdx,     /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->BitPattern = SOAD_EVENT_QUEUE_BIT_PATTERN_TP_TX_SO_CON;                                    /* SBSW_SOAD_POINTER_WRITE */
        retVal = E_OK;
      }
      break;
    }
    case SOAD_EVENT_QUEUE_TP_RX_SO_CON:
    {
      if ( SoAd_IsEventQueueTpRxSoConConfigUsedOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx) )
      {
        SoAd_SizeOfEventQueueTpRxSoConConfigType eveQueCfgIdx =
          SoAd_GetEventQueueTpRxSoConConfigIdxOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx);

        EventQueueParamPtr->EventQueueStartIdx =                                                                       /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueTpRxSoConStartIdxOfEventQueueTpRxSoConConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueEndIdx =                                                                         /* SBSW_SOAD_POINTER_WRITE */
          SoAd_GetEventQueueTpRxSoConEndIdxOfEventQueueTpRxSoConConfig(eveQueCfgIdx, PartitionIdx);
        EventQueueParamPtr->EventQueueFlagOffset = SoAd_GetFlagOffsetOfEventQueueTpRxSoConConfig(eveQueCfgIdx,         /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->EventQueueMgmtIdx = SoAd_GetEventQueueMgmtIdxOfEventQueueTpRxSoConConfig(eveQueCfgIdx,     /* SBSW_SOAD_POINTER_WRITE */
          PartitionIdx);
        EventQueueParamPtr->BitPattern = SOAD_EVENT_QUEUE_BIT_PATTERN_TP_RX_SO_CON;                                    /* SBSW_SOAD_POINTER_WRITE */
        retVal = E_OK;
      }
      break;
    }
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    case SOAD_EVENT_QUEUE_SO_CON_MODE_CHG:
    {
      SoAd_SizeOfEventQueueSoConModeChgConfigType eveQueCfgIdx =
        SoAd_GetEventQueueSoConModeChgConfigIdxOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx);

      EventQueueParamPtr->EventQueueStartIdx =                                                                         /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetEventQueueSoConModeChgStartIdxOfEventQueueSoConModeChgConfig(eveQueCfgIdx, PartitionIdx);
      EventQueueParamPtr->EventQueueEndIdx =                                                                           /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetEventQueueSoConModeChgEndIdxOfEventQueueSoConModeChgConfig(eveQueCfgIdx, PartitionIdx);
      EventQueueParamPtr->EventQueueFlagOffset = SoAd_GetFlagOffsetOfEventQueueSoConModeChgConfig(eveQueCfgIdx,        /* SBSW_SOAD_POINTER_WRITE */
        PartitionIdx);
      EventQueueParamPtr->EventQueueMgmtIdx = SoAd_GetEventQueueMgmtIdxOfEventQueueSoConModeChgConfig(eveQueCfgIdx,    /* SBSW_SOAD_POINTER_WRITE */
        PartitionIdx);
      EventQueueParamPtr->BitPattern = SOAD_EVENT_QUEUE_BIT_PATTERN_SO_CON_MODE_CHG;                                   /* SBSW_SOAD_POINTER_WRITE */
      retVal = E_OK;
      break;
    }
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */
    default: /* SOAD_EVENT_QUEUE_SO_CON_STATE */
    {
      SoAd_SizeOfEventQueueStateSoConConfigType eveQueCfgIdx =
        SoAd_GetEventQueueStateSoConConfigIdxOfEventQueueIdent(EventQueueIdentIdx, PartitionIdx);

      EventQueueParamPtr->EventQueueStartIdx =                                                                         /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetEventQueueStateSoConStartIdxOfEventQueueStateSoConConfig(eveQueCfgIdx, PartitionIdx);
      EventQueueParamPtr->EventQueueEndIdx =                                                                           /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetEventQueueStateSoConEndIdxOfEventQueueStateSoConConfig(eveQueCfgIdx, PartitionIdx);
      EventQueueParamPtr->EventQueueFlagOffset = SoAd_GetFlagOffsetOfEventQueueStateSoConConfig(eveQueCfgIdx,          /* SBSW_SOAD_POINTER_WRITE */
        PartitionIdx);
      EventQueueParamPtr->EventQueueMgmtIdx = SoAd_GetEventQueueMgmtIdxOfEventQueueStateSoConConfig(eveQueCfgIdx,      /* SBSW_SOAD_POINTER_WRITE */
        PartitionIdx);
      EventQueueParamPtr->BitPattern = SOAD_EVENT_QUEUE_BIT_PATTERN_SO_CON_STATE;                                      /* SBSW_SOAD_POINTER_WRITE */
      retVal = E_OK;
      break;
    }
  }

  return retVal;
} /* SoAd_EventQueue_GetEventQueueInfo() */

/**********************************************************************************************************************
 *  SoAd_EventQueue_AddHandleIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_EventQueue_AddHandleIdx(
  SoAd_EventQueueEnumType EventQueue,
  uint32 WriteIdx,
  uint32 HandleIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add handle index to corresponding event queue. */
  switch (EventQueue)
  {
    case SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE:
    {
      SoAd_SetPduRouteIdxOfEventQueueIfUdpPduRoute(WriteIdx, (SoAd_SizeOfPduRouteType)HandleIdx, PartitionIdx);        /* SBSW_SOAD_PARAMETER_IDX */
      break;
    }
    case SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP:
    {
      SoAd_SetRouteGrpIdxOfEventQueueIfTxRouteGrp(WriteIdx, (SoAd_SizeOfRouteGrpType)HandleIdx, PartitionIdx);         /* SBSW_SOAD_PARAMETER_IDX */
      break;
    }
    case SOAD_EVENT_QUEUE_TP_TX_SO_CON:
    {
      SoAd_SetSoConIdxOfEventQueueTpTxSoCon(WriteIdx, (SoAd_SizeOfSoConType)HandleIdx, PartitionIdx);                  /* SBSW_SOAD_PARAMETER_IDX */
      break;
    }
    case SOAD_EVENT_QUEUE_TP_RX_SO_CON:
    {
      SoAd_SetSoConIdxOfEventQueueTpRxSoCon(WriteIdx, (SoAd_SizeOfSoConType)HandleIdx, PartitionIdx);                  /* SBSW_SOAD_PARAMETER_IDX */
      break;
    }
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    case SOAD_EVENT_QUEUE_SO_CON_MODE_CHG:
    {
      SoAd_SetSoConIdxOfEventQueueSoConModeChg(WriteIdx, (SoAd_SizeOfSoConType)HandleIdx, PartitionIdx);               /* SBSW_SOAD_PARAMETER_IDX */
      break;
    }
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */
    default: /* SOAD_EVENT_QUEUE_SO_CON_STATE */
    {
      SoAd_SetSoConIdxOfEventQueueStateSoCon(WriteIdx, (SoAd_SizeOfSoConType)HandleIdx, PartitionIdx);                 /* SBSW_SOAD_PARAMETER_IDX */
      break;
    }
  }
} /* SoAd_EventQueue_AddHandleIdx() */

/**********************************************************************************************************************
 *  SoAd_EventQueue_GetHandleIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_EventQueue_GetHandleIdx(
  SoAd_EventQueueEnumType EventQueue,
  uint32 EventQueueMgmtIdx,
  uint32 EventQueueStartIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get handle index from corresponding event queue. */
  switch (EventQueue)
  {
    case SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE:
    {
      *HandleIdxPtr = SoAd_GetPduRouteIdxOfEventQueueIfUdpPduRoute(                                                    /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetIdxOfEventQueueMgmt(EventQueueMgmtIdx, PartitionIdx) + EventQueueStartIdx, PartitionIdx);
      break;
    }
    case SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP:
    {
      *HandleIdxPtr = SoAd_GetRouteGrpIdxOfEventQueueIfTxRouteGrp(                                                     /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetIdxOfEventQueueMgmt(EventQueueMgmtIdx, PartitionIdx) + EventQueueStartIdx, PartitionIdx);
      break;
    }
    case SOAD_EVENT_QUEUE_TP_TX_SO_CON:
    {
      *HandleIdxPtr = SoAd_GetSoConIdxOfEventQueueTpTxSoCon(                                                           /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetIdxOfEventQueueMgmt(EventQueueMgmtIdx, PartitionIdx) + EventQueueStartIdx, PartitionIdx);
      break;
    }
    case SOAD_EVENT_QUEUE_TP_RX_SO_CON:
    {
      *HandleIdxPtr = SoAd_GetSoConIdxOfEventQueueTpRxSoCon(                                                           /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetIdxOfEventQueueMgmt(EventQueueMgmtIdx, PartitionIdx) + EventQueueStartIdx, PartitionIdx);
      break;
    }
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    case SOAD_EVENT_QUEUE_SO_CON_MODE_CHG:
    {
      *HandleIdxPtr = SoAd_GetSoConIdxOfEventQueueSoConModeChg(                                                        /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetIdxOfEventQueueMgmt(EventQueueMgmtIdx, PartitionIdx) + EventQueueStartIdx, PartitionIdx);
      break;
    }
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */
    default: /* SOAD_EVENT_QUEUE_SO_CON_STATE */
    {
      *HandleIdxPtr = SoAd_GetSoConIdxOfEventQueueStateSoCon(                                                          /* SBSW_SOAD_POINTER_WRITE */
        SoAd_GetIdxOfEventQueueMgmt(EventQueueMgmtIdx, PartitionIdx) + EventQueueStartIdx, PartitionIdx);
      break;
    }
  }
} /* SoAd_EventQueue_GetHandleIdx() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_EventQueue_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_EventQueue_Init(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_EventQueueMgmtIterType eventQueueIter;
  SoAd_EventQueueFlagIterType eventQueueFlagIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize management struct of all event queues. */
  for ( eventQueueIter = 0u; eventQueueIter < SoAd_GetSizeOfEventQueueMgmt(PartitionIdx); eventQueueIter++ )
  {
    SoAd_SetIdxOfEventQueueMgmt(eventQueueIter, 0u, PartitionIdx);                                                     /* SBSW_SOAD_CSL01_CSL05 */
    SoAd_SetLvlOfEventQueueMgmt(eventQueueIter, 0u, PartitionIdx);                                                     /* SBSW_SOAD_CSL01_CSL05 */
  }
  /* #20 Initialize event queue flags. */
  for ( eventQueueFlagIter = 0u; eventQueueFlagIter < SoAd_GetSizeOfEventQueueFlag(PartitionIdx);
    eventQueueFlagIter++ )
  {
    SoAd_SetEventQueueFlag(eventQueueFlagIter, 0u, PartitionIdx);                                                      /* SBSW_SOAD_CSL01_CSL05 */
  }
} /* SoAd_EventQueue_Init() */

/**********************************************************************************************************************
 *  SoAd_EventQueue_AddElement()
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
FUNC(void, SOAD_CODE) SoAd_EventQueue_AddElement(
  SoAd_EventQueueEnumType EventQueue,
  uint32 HandleIdx,
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                         writeIdx;
  SoAd_EventQueueParamType       eveQueParam;
  SoAd_SizeOfEventQueueIdentType eveQueIdentIdx = SoAd_GetEventQueueIdentIdxOfInstance(InstanceIdx, PartitionIdx);
  SoAd_SizeOfEventQueueFlagType  eveQueFlagIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that event queue is written inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Get event queue information if event queue exists. */
  if ( SoAd_EventQueue_GetEventQueueInfo(EventQueue, eveQueIdentIdx, PartitionIdx, &eveQueParam) == E_OK )             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* #30 Calculate the event queue flag index. */
    eveQueFlagIdx = (SoAd_SizeOfEventQueueFlagType)(HandleIdx + eveQueParam.EventQueueFlagOffset);

    /* #40 Check if the event queue flag index is in a valid range for the related event queue. */
    if ( (eveQueFlagIdx >= SoAd_GetEventQueueFlagStartIdxOfEventQueueIdent(eveQueIdentIdx, PartitionIdx)) &&
      (eveQueFlagIdx < SoAd_GetEventQueueFlagEndIdxOfEventQueueIdent(eveQueIdentIdx, PartitionIdx)) )
    {
      /* #50 Check if event is not yet in queue. */
      if ( (SoAd_GetEventQueueFlag(eveQueFlagIdx, PartitionIdx) & eveQueParam.BitPattern) == 0u )
      {
        /* #60 Get next queue index. */
        writeIdx = SoAd_Util_GetNextArrayElement(SoAd_GetIdxOfEventQueueMgmt(eveQueParam.EventQueueMgmtIdx,
            PartitionIdx) + eveQueParam.EventQueueStartIdx, SoAd_GetLvlOfEventQueueMgmt(
            eveQueParam.EventQueueMgmtIdx, PartitionIdx), eveQueParam.EventQueueStartIdx,
          eveQueParam.EventQueueEndIdx);

        /* #70 Add new queue entry. */
        SoAd_EventQueue_AddHandleIdx(EventQueue, writeIdx, HandleIdx, PartitionIdx);
        SoAd_IncLvlOfEventQueueMgmt(eveQueParam.EventQueueMgmtIdx, PartitionIdx);                                      /* SBSW_SOAD_INDEX_BY_FUNCTION */

        /* #80 Set flag to indicate that handle index is in queue. */
        SoAd_SetEventQueueFlag(eveQueFlagIdx, (SoAd_EventQueueFlagType)(                                               /* SBSW_SOAD_CALCULATED_CHECKED_CSL03_CSL05 */
          SoAd_GetEventQueueFlag(eveQueFlagIdx, PartitionIdx) | eveQueParam.BitPattern), PartitionIdx);
      }
    }
  }

  /* #90 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_EventQueue_AddElement() */

/**********************************************************************************************************************
 *  SoAd_EventQueue_GetNextElement()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_EventQueue_GetNextElement(
  SoAd_EventQueueEnumType EventQueue,
  SoAd_SizeOfInstanceMapType InstMapIdx,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_EventQueueParamType       eveQueParam;
  SoAd_SizeOfInstanceType        instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);
  SoAd_PartitionConfigIdxType    partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);
  SoAd_SizeOfEventQueueIdentType eveQueIdentIdx = SoAd_GetEventQueueIdentIdxOfInstance(instanceIdx, partitionIdx);
  SoAd_SizeOfEventQueueFlagType  eveQueFlagIdx;
  Std_ReturnType                 retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that event queue is written inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Try to get event queue parameters. */
  retVal = SoAd_EventQueue_GetEventQueueInfo(EventQueue, eveQueIdentIdx, partitionIdx, &eveQueParam);                  /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

  /* #30 Check if event queue exists and if at least one event is in queue. */
  if ( (retVal == E_OK) && (SoAd_GetLvlOfEventQueueMgmt(eveQueParam.EventQueueMgmtIdx, partitionIdx) > 0u) )
  {
    /* #40 Get next element. */
    SoAd_EventQueue_GetHandleIdx(EventQueue, eveQueParam.EventQueueMgmtIdx, eveQueParam.EventQueueStartIdx,
      partitionIdx, HandleIdxPtr);                                                                                     /* SBSW_SOAD_POINTER_FORWARD */
    
    /* #50 Calculate the event queue flag index. */
    eveQueFlagIdx = (SoAd_SizeOfEventQueueFlagType)(*HandleIdxPtr + eveQueParam.EventQueueFlagOffset);

    /* #60 Check if the event queue flag index is in a valid range for the related event queue. */
    if ( (eveQueFlagIdx >= SoAd_GetEventQueueFlagStartIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx)) &&
      (eveQueFlagIdx < SoAd_GetEventQueueFlagEndIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx)) )
    {
      /* #70 Remove element from queue. */
      SoAd_IdxOfEventQueueMgmtType nextIdx = (SoAd_IdxOfEventQueueMgmtType)(SoAd_Util_GetNextArrayElement(
          SoAd_GetIdxOfEventQueueMgmt(eveQueParam.EventQueueMgmtIdx, partitionIdx) + eveQueParam.EventQueueStartIdx,
          1u, eveQueParam.EventQueueStartIdx, eveQueParam.EventQueueEndIdx) - eveQueParam.EventQueueStartIdx);

      SoAd_SetIdxOfEventQueueMgmt(eveQueParam.EventQueueMgmtIdx, nextIdx, partitionIdx);                               /* SBSW_SOAD_INDEX_BY_FUNCTION */
      SoAd_DecLvlOfEventQueueMgmt(eveQueParam.EventQueueMgmtIdx, partitionIdx);                                        /* SBSW_SOAD_INDEX_BY_FUNCTION */

      /* #80 Reset event flag for handle index. */
      SoAd_SetEventQueueFlag(eveQueFlagIdx, (SoAd_EventQueueFlagType)(SoAd_GetEventQueueFlag(eveQueFlagIdx,            /* SBSW_SOAD_CALCULATED_CHECKED_CSL03_CSL05 */
          partitionIdx) & ((SoAd_EventQueueFlagType)~eveQueParam.BitPattern)), partitionIdx);
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }

  /* #90 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_EventQueue_GetNextElement() */

/**********************************************************************************************************************
 *  SoAd_EventQueue_GetElementNumToHandle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(uint32, SOAD_CODE) SoAd_EventQueue_GetElementNumToHandle(
  SoAd_EventQueueEnumType EventQueue,
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                          elementNumToHandle;
  uint32                          limit = 0u;
  SoAd_LvlOfEventQueueMgmtType    lvl = 0u;
  SoAd_SizeOfInstanceType         instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);
  SoAd_PartitionConfigIdxType     partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(
    InstMapIdx);
  SoAd_SizeOfEventQueueIdentType  eveQueIdentIdx = SoAd_GetEventQueueIdentIdxOfInstance(instanceIdx, partitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get limit of successively handled events and current level of event queue. */
  switch (EventQueue)
  {
    case SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE:
    {
      if ( SoAd_IsEventQueueIfUdpPduRouteConfigUsedOfEventQueueIdent(eveQueIdentIdx, partitionIdx) )
      {
        SoAd_SizeOfEventQueueIfUdpPduRouteConfigType eveQueCfgIdx =
          SoAd_GetEventQueueIfUdpPduRouteConfigIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx);

        lvl = SoAd_GetLvlOfEventQueueMgmt(SoAd_GetEventQueueMgmtIdxOfEventQueueIfUdpPduRouteConfig(eveQueCfgIdx,
            partitionIdx), partitionIdx);
        limit = SoAd_GetLimitOfEventQueueIfUdpPduRouteConfig(eveQueCfgIdx, partitionIdx);
      }
      break;
    }
    case SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP:
    {
      if ( SoAd_IsEventQueueIfTxRouteGrpConfigUsedOfEventQueueIdent(eveQueIdentIdx, partitionIdx) )
      {
        SoAd_SizeOfEventQueueIfTxRouteGrpConfigType eveQueCfgIdx =
          SoAd_GetEventQueueIfTxRouteGrpConfigIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx);

        lvl = SoAd_GetLvlOfEventQueueMgmt(SoAd_GetEventQueueMgmtIdxOfEventQueueIfTxRouteGrpConfig(eveQueCfgIdx,
            partitionIdx), partitionIdx);
        limit = SoAd_GetLimitOfEventQueueIfTxRouteGrpConfig(eveQueCfgIdx, partitionIdx);
      }
      break;
    }
    case SOAD_EVENT_QUEUE_TP_TX_SO_CON:
    {
      if ( SoAd_IsEventQueueTpTxSoConConfigUsedOfEventQueueIdent(eveQueIdentIdx, partitionIdx) )
      {
        SoAd_SizeOfEventQueueTpTxSoConConfigType eveQueCfgIdx =
          SoAd_GetEventQueueTpTxSoConConfigIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx);

        lvl = SoAd_GetLvlOfEventQueueMgmt(SoAd_GetEventQueueMgmtIdxOfEventQueueTpTxSoConConfig(eveQueCfgIdx,
            partitionIdx), partitionIdx);
        limit = SoAd_GetLimitOfEventQueueTpTxSoConConfig(eveQueCfgIdx, partitionIdx);
      }
      break;
    }
    case SOAD_EVENT_QUEUE_TP_RX_SO_CON:
    {
      if ( SoAd_IsEventQueueTpRxSoConConfigUsedOfEventQueueIdent(eveQueIdentIdx, partitionIdx) )
      {
        SoAd_SizeOfEventQueueTpRxSoConConfigType eveQueCfgIdx =
          SoAd_GetEventQueueTpRxSoConConfigIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx);

        lvl = SoAd_GetLvlOfEventQueueMgmt(SoAd_GetEventQueueMgmtIdxOfEventQueueTpRxSoConConfig(eveQueCfgIdx,
            partitionIdx), partitionIdx);
        limit = SoAd_GetLimitOfEventQueueTpRxSoConConfig(eveQueCfgIdx, partitionIdx);
      }
      break;
    }
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    case SOAD_EVENT_QUEUE_SO_CON_MODE_CHG:
    {
      SoAd_SizeOfEventQueueSoConModeChgConfigType eveQueCfgIdx =
        SoAd_GetEventQueueSoConModeChgConfigIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx);

      lvl = SoAd_GetLvlOfEventQueueMgmt(SoAd_GetEventQueueMgmtIdxOfEventQueueSoConModeChgConfig(eveQueCfgIdx,
          partitionIdx), partitionIdx);
      limit = SoAd_GetLimitOfEventQueueSoConModeChgConfig(eveQueCfgIdx, partitionIdx);
      break;
    }
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */
    default: /* SOAD_EVENT_QUEUE_SO_CON_STATE */
    {
      SoAd_SizeOfEventQueueStateSoConConfigType eveQueCfgIdx =
        SoAd_GetEventQueueStateSoConConfigIdxOfEventQueueIdent(eveQueIdentIdx, partitionIdx);

      lvl = SoAd_GetLvlOfEventQueueMgmt(SoAd_GetEventQueueMgmtIdxOfEventQueueStateSoConConfig(eveQueCfgIdx,
          partitionIdx), partitionIdx);
      limit = SoAd_GetLimitOfEventQueueStateSoConConfig(eveQueCfgIdx, partitionIdx);
      break;
    }
  }

  /* #20 Return limit if current level exceeds the limit. */
  if ( lvl > limit )
  {
    elementNumToHandle = limit;
  }
  /* #21 Return current level otherwise. */
  else
  {
    elementNumToHandle = lvl;
  }

  return elementNumToHandle;
} /* SoAd_EventQueue_GetElementNumToHandle() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/* Concept for EventQueue:

  All events handled in a queue use the same mechanism. Therefore, there is a generic handling for all event
  queues. Each event queue is identified by an enum value used in the API functions and by the instance map index which
  can be used to access the event queue ident array.
  The event queue ident array contains indexes to access the different event queue configuration arrays (e.g.
  "EventQueueStateSoConConfig") as well as start and end indexes for the event queue flag array.
  The event queue configuration arrays contain further event queue specific information as the event queue management
  index, the start and end indexes inside the corresponding event queue (e.g. "EventQueueStateSoCon") and the
  configured limit (all those parameters are instance specific).
  A generic parameter name "HandleIdx" is used in the API functions to identify the object which generates an event
  (e.g. event queue: StateSoCon -> handle: SoConIdx). The HandleIdx is used to access the "EventQueueFlag".

  The events are stored and processed as FIFO in "SoAd_EventQueue<EventQueue>" array and managed by
  "SoAd_EventQueueMgmt". To identify the event the object index is stored in the queue. Since there is just one event
  queue entry for an object it has to be checked whether the object is already stored in the event queue. To check and
  set this "SoAd_GetEventQueueFlag" and "SoAd_SetEventQueueFlag" is used. To save RAM "SoAd_GetEventQueueFlag" is
  accessed via a bit pattern for each event queue so just one bit is used to indicate if the object is already stored
  in the queue. The bit pattern is derived in "SoAd_EventQueue_GetEventQueueInfo" from different Macros.

*/

/**********************************************************************************************************************
 *  END OF FILE: SoAd_EventQueue.c
 *********************************************************************************************************************/
