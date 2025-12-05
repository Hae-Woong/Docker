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
/*!        \file  SoAd_TimeoutList.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component TimeoutList.
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

/* PRQA S 1881 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4304 EOF */ /* MD_MSR_AutosarBoolean */

#define SOAD_TIMEOUT_LIST_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_TimeoutList.h"
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
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_TimeoutList_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TimeoutList_Init(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_TimeoutListMgmtIterType               timeoutListIter;
  SoAd_TimeoutListUdpAliveMapIterType        timeoutListUdpAliveMapIter;
  SoAd_TimeoutListNPduUdpTxMapIterType       timeoutListNPduUdpTxMapIter;
#if ( SOAD_TCP == STD_ON )
  SoAd_TimeoutListTcpAutoConnectMapIterType  timeoutListTcpAutoConnectMapIter;
#endif /* SOAD_TCP == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all timeout lists and reset management struct. */
  for ( timeoutListIter = 0u; timeoutListIter < SoAd_GetSizeOfTimeoutListMgmt(PartitionIdx); timeoutListIter++ )
  {
    SoAd_SetLvlOfTimeoutListMgmt(timeoutListIter, 0u, PartitionIdx);                                                   /* SBSW_SOAD_CSL01_CSL05 */
  }

  /* #20 Reset timeout list for UDP alive supervision timeout. */
  for ( timeoutListUdpAliveMapIter = 0u;
    timeoutListUdpAliveMapIter < SoAd_GetSizeOfTimeoutListUdpAliveMap(PartitionIdx);
    timeoutListUdpAliveMapIter++ )
  {
    SoAd_SetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(timeoutListUdpAliveMapIter,                                 /* SBSW_SOAD_CSL01_CSL05 */
      SoAd_GetSizeOfTimeoutListUdpAlive(PartitionIdx), PartitionIdx);
  }

  /* #30 Reset timeout list for nPdu. */
  for ( timeoutListNPduUdpTxMapIter = 0u;
    timeoutListNPduUdpTxMapIter < SoAd_GetSizeOfTimeoutListNPduUdpTxMap(PartitionIdx);
    timeoutListNPduUdpTxMapIter++ )
  {
    SoAd_SetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(timeoutListNPduUdpTxMapIter,                              /* SBSW_SOAD_CSL01_CSL05 */
      SoAd_GetSizeOfTimeoutListNPduUdpTx(PartitionIdx), PartitionIdx);
  }

#if ( SOAD_TCP == STD_ON )
  /* #40 Reset timeout list for TCP auto connect. */
  for ( timeoutListTcpAutoConnectMapIter = 0u;
    timeoutListTcpAutoConnectMapIter < SoAd_GetSizeOfTimeoutListTcpAutoConnectMap(PartitionIdx);
    timeoutListTcpAutoConnectMapIter++ )
  {
    SoAd_SetTimeoutListTcpAutoConnectIdxOfTimeoutListTcpAutoConnectMap(timeoutListTcpAutoConnectMapIter,               /* SBSW_SOAD_CSL01_CSL05 */
      SoAd_GetSizeOfTimeoutListTcpAutoConnect(PartitionIdx), PartitionIdx);
  }
#endif /* SOAD_TCP == STD_ON */
} /* SoAd_TimeoutList_Init() */

/**********************************************************************************************************************
 *  SoAd_TimeoutList_SetElement()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_SetElement(
  SoAd_TimeoutListEnumType TimeoutList,
  uint32 HandleIdx,
  uint32 Timeout,
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfTimeoutListMgmtType  timeListMgmtIdx;
  /* Optional indirection: Refer to precondition in function description. */
  SoAd_SizeOfTimeoutListIdentType timeoutListIdentIdx = SoAd_GetTimeoutListIdentIdxOfInstance(InstanceIdx,
    PartitionIdx);
  uint32                          timerCounter = SoAd_GetCounterOfInstanceDyn(InstanceIdx, PartitionIdx);
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that timeout list is written inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Set element for the requested timeout list. */
  switch ( TimeoutList )
  {
    /* #30 Set element for UDP alive supervision timeout. */
    case SOAD_TIMEOUT_LIST_UDP_ALIVE:
    {
      SoAd_SizeOfSoConType soConIdx = (SoAd_SizeOfSoConType)HandleIdx;

      /* Optional indirection: Refer to precondition in function description. */
      timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListUdpAliveConfig(
        SoAd_GetTimeoutListUdpAliveConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, PartitionIdx), PartitionIdx);

      /* #300 Check if handle is not yet in list. */
      if ( SoAd_GetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(soConIdx, PartitionIdx) ==
        SoAd_GetSizeOfTimeoutListUdpAlive(PartitionIdx) )
      {
        /* #3000 Add handle to list. */
        SoAd_SetSoConIdxOfTimeoutListUdpAlive(SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx), soConIdx,   /* SBSW_SOAD_CALCULATED_TimeoutListUdpAlive */
          PartitionIdx);
        SoAd_SetTimeoutOfTimeoutListUdpAlive(SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx),              /* SBSW_SOAD_CALCULATED_TimeoutListUdpAlive */
          (uint32)(timerCounter + Timeout), PartitionIdx);

        /* #3001 Add used list index to socket connection to list index map. */
        SoAd_SetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(soConIdx,                                               /* SBSW_SOAD_PARAMETER_IDX */
          (SoAd_SizeOfTimeoutListUdpAliveType)SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx),
          PartitionIdx);

        /* #3002 Increment timeout list level. */
        SoAd_IncLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx);                                                   /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

        retVal = E_OK;
      }
      /* #301 Update timeout in list entry otherwise. */
      else
      {
        SoAd_SetTimeoutOfTimeoutListUdpAlive(SoAd_GetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(soConIdx,          /* SBSW_SOAD_CALCULATED_TimeoutListUdpAlive */
          PartitionIdx), (uint32)(timerCounter + Timeout), PartitionIdx);

        retVal = E_OK;
      }
      break;
    }
    /* #31 Set element for nPdu timeout otherwise. */
#if ( SOAD_TCP == STD_ON )
    case SOAD_TIMEOUT_LIST_N_PDU:
#else
    default: /* SOAD_TIMEOUT_LIST_N_PDU */
#endif /* SOAD_TCP == STD_ON */
    {
      SoAd_SizeOfNPduUdpTxType                  nPduUdpTxIdx = (SoAd_SizeOfNPduUdpTxType)HandleIdx;
      /* Optional indirection: Refer to precondition in function description. */
      SoAd_SizeOfTimeoutListNPduUdpTxConfigType timeoutListNPduUdpTxConfigIdx =
        SoAd_GetTimeoutListNPduUdpTxConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, PartitionIdx);

      timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListNPduUdpTxConfig(timeoutListNPduUdpTxConfigIdx,
        PartitionIdx);

      /* #310 Check if handle is not yet in list. */
      if ( SoAd_GetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(nPduUdpTxIdx, PartitionIdx) ==
        SoAd_GetSizeOfTimeoutListNPduUdpTx(PartitionIdx) )
      {
        /* #3100 Check if list size is sufficient for new element. */
        if ( SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx) < (SoAd_LvlOfTimeoutListMgmtType) /* PRQA S 4391 */ /* MD_SoAd_GenericWiderTypeCast */
          (SoAd_GetTimeoutListNPduUdpTxEndIdxOfTimeoutListNPduUdpTxConfig(timeoutListNPduUdpTxConfigIdx,
              PartitionIdx) -
            SoAd_GetTimeoutListNPduUdpTxStartIdxOfTimeoutListNPduUdpTxConfig(timeoutListNPduUdpTxConfigIdx,
              PartitionIdx)) )
        {
          /* #31000 Add handle to list. */
          SoAd_SetNPduUdpTxIdxOfTimeoutListNPduUdpTx(SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx),      /* SBSW_SOAD_CALCULATED_TimeoutListNPduUdpTx */
            nPduUdpTxIdx, PartitionIdx);
          SoAd_SetTimeoutOfTimeoutListNPduUdpTx(SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx),           /* SBSW_SOAD_CALCULATED_TimeoutListNPduUdpTx */
            (uint32)(timerCounter + Timeout), PartitionIdx);

          /* #31001 Add used list index to nPdu to list index map. */
          SoAd_SetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(nPduUdpTxIdx, (SoAd_SizeOfTimeoutListNPduUdpTxType) /* SBSW_SOAD_PARAMETER_IDX */
            SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx), PartitionIdx);

          /* #31002 Increment timeout list level. */
          SoAd_IncLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx);                                                 /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

          retVal = E_OK;
        }
      }
      /* #311 Update timeout in list entry otherwise. */
      else
      {
        SoAd_SetTimeoutOfTimeoutListNPduUdpTx(SoAd_GetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(nPduUdpTxIdx,   /* SBSW_SOAD_CALCULATED_TimeoutListNPduUdpTx */
          PartitionIdx), (uint32)(timerCounter + Timeout), PartitionIdx);

        retVal = E_OK;
      }
      break;
    }
    /* #32 Set element for TCP auto connect timeout otherwise. */
#if ( SOAD_TCP == STD_ON )
    default: /* SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT */
    {
      SoAd_SizeOfSoConType                           soConIdx = (SoAd_SizeOfSoConType)HandleIdx;
      /* Optional indirection: Refer to precondition in function description. */
      SoAd_SizeOfTimeoutListTcpAutoConnectConfigType timeoutListTcpAutoConnectConfigIdx =
        SoAd_GetTimeoutListTcpAutoConnectConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, PartitionIdx);

      timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListTcpAutoConnectConfig(timeoutListTcpAutoConnectConfigIdx,
        PartitionIdx);

      /* #320 Check if handle is not yet in list. */
      if ( SoAd_GetTimeoutListTcpAutoConnectIdxOfTimeoutListTcpAutoConnectMap(soConIdx, PartitionIdx) ==
        SoAd_GetSizeOfTimeoutListTcpAutoConnect(PartitionIdx) )
      {
        /* #3200 Add handle to list. */
        SoAd_SetSoConIdxOfTimeoutListTcpAutoConnect(SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx),       /* SBSW_SOAD_CALCULATED_TimeoutListTcpAutoConnect */
          soConIdx, PartitionIdx);
        SoAd_SetTimeoutOfTimeoutListTcpAutoConnect(SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx),        /* SBSW_SOAD_CALCULATED_TimeoutListTcpAutoConnect */
          (uint32)(timerCounter + Timeout), PartitionIdx);

        /* #3201 Add used list index to socket connection to list index map. */
        SoAd_SetTimeoutListTcpAutoConnectIdxOfTimeoutListTcpAutoConnectMap(soConIdx,                                   /* SBSW_SOAD_PARAMETER_IDX */
          (SoAd_SizeOfTimeoutListTcpAutoConnectType)SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx),
          PartitionIdx);

        /* #3202 Increment timeout list level. */
        SoAd_IncLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx);                                                   /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

        retVal = E_OK;
      }
      break;
    }
#endif /* SOAD_TCP == STD_ON */
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TimeoutList_SetElement() */

/**********************************************************************************************************************
 *  SoAd_TimeoutList_GetCurrentTimeout()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_GetCurrentTimeout(
  SoAd_TimeoutListEnumType TimeoutList,
  uint32 HandleIdx,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) TimeoutPtr,
  uint32 TimerCounter,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that timeout list is written inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Get value of element from the requested timeout list. */
  switch ( TimeoutList )
  {
    /* #30 Get current value for UDP alive supervision timeout. */
    case SOAD_TIMEOUT_LIST_UDP_ALIVE:
    {
      SoAd_SizeOfSoConType                soConIdx;
      SoAd_SizeOfTimeoutListUdpAliveType  timeListUdpAliveIdx;

      soConIdx = (SoAd_SizeOfSoConType)HandleIdx;
      timeListUdpAliveIdx = SoAd_GetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(soConIdx, PartitionIdx);

      /* #300 Check if handle is in list. */
      if ( timeListUdpAliveIdx != SoAd_GetSizeOfTimeoutListUdpAlive(PartitionIdx) )
      {
        /* #3000 Get current timeout value. */
        uint32 currentTimeout = SoAd_GetTimeoutOfTimeoutListUdpAlive(timeListUdpAliveIdx, PartitionIdx);

        if ( currentTimeout >= TimerCounter )
        { /* no timer counter overflow */
          *TimeoutPtr = (uint32)(currentTimeout - TimerCounter);                                                       /* SBSW_SOAD_POINTER_WRITE */
        }
        else
        { /* timer counter overflow */
          *TimeoutPtr = (uint32)(currentTimeout + (SOAD_MAX_TIMEOUT_32 - TimerCounter));                               /* SBSW_SOAD_POINTER_WRITE */
        }

        retVal = E_OK;
      }
      break;
    }
    /* #31 Get current value for nPdu timeout otherwise. */
    case SOAD_TIMEOUT_LIST_N_PDU:
    {
      SoAd_SizeOfNPduUdpTxType            nPduUdpTxIdx;
      SoAd_SizeOfTimeoutListNPduUdpTxType timeListnPduUdpTxIdx;

      nPduUdpTxIdx = (SoAd_SizeOfNPduUdpTxType)HandleIdx;
      timeListnPduUdpTxIdx = SoAd_GetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(nPduUdpTxIdx, PartitionIdx);

      /* #310 Check if handle is in list. */
      if ( timeListnPduUdpTxIdx != SoAd_GetSizeOfTimeoutListNPduUdpTx(PartitionIdx) )
      {
        /* #3100 Get current timeout value. */
        uint32 currentTimeout = SoAd_GetTimeoutOfTimeoutListNPduUdpTx(timeListnPduUdpTxIdx, PartitionIdx);

        if ( currentTimeout >= TimerCounter )
        { /* no timer counter overflow */
          *TimeoutPtr = (uint32)(currentTimeout - TimerCounter);                                                       /* SBSW_SOAD_POINTER_WRITE */
        }
        else
        { /* timer counter overflow */
          *TimeoutPtr = (uint32)(currentTimeout + (SOAD_MAX_TIMEOUT_32 - TimerCounter));                               /* SBSW_SOAD_POINTER_WRITE */
        }

        retVal = E_OK;
      }
      break;
    }
    /* #32 Return a failure otherwise. */
    default:
    {
      /* nothing to do */
      break;
    }
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TimeoutList_GetCurrentTimeout() */

/**********************************************************************************************************************
 *  SoAd_TimeoutList_CheckElements()
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
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_CheckElements(
  SoAd_TimeoutListEnumType TimeoutList,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) ElementIdxPtr,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr,
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfTimeoutListMgmtType  timeListMgmtIdx;
  SoAd_SizeOfInstanceType         instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);
  SoAd_PartitionConfigIdxType     partitionIdx =
    SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);
  SoAd_SizeOfTimeoutListIdentType timeoutListIdentIdx = SoAd_GetTimeoutListIdentIdxOfInstance(instanceIdx,
    partitionIdx);
  uint32                          timerCounter = SoAd_GetCounterOfInstanceDyn(instanceIdx, partitionIdx);
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that timeout list is written and read inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check elements for the requested timeout list. */
  switch ( TimeoutList )
  {
    /* #30 Check elements of UDP alive supervision timeout. */
    case SOAD_TIMEOUT_LIST_UDP_ALIVE:
    {
      SoAd_TimeoutListUdpAliveIterType timeListUdpAliveIter;

      /* Optional indirection with constant index: Refer to precondition in function description. */
      timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListUdpAliveConfig(
        SoAd_GetTimeoutListUdpAliveConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, partitionIdx), partitionIdx);

      /* #300 Search for timeout. */
      for ( timeListUdpAliveIter = (SoAd_SizeOfTimeoutListUdpAliveType)*ElementIdxPtr;
        timeListUdpAliveIter < SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, partitionIdx);
        timeListUdpAliveIter++ )
      {
        if ( SoAd_GetTimeoutOfTimeoutListUdpAlive(timeListUdpAliveIter, partitionIdx) == timerCounter )
        {
          /* #3000 Return first occurred timeout. */
          *ElementIdxPtr = (uint32)timeListUdpAliveIter;                                                               /* SBSW_SOAD_POINTER_WRITE */
          *HandleIdxPtr = SoAd_GetSoConIdxOfTimeoutListUdpAlive(timeListUdpAliveIter, partitionIdx);                   /* SBSW_SOAD_POINTER_WRITE */
          retVal = E_OK;
          break;
        }
      }
      break;
    }
    /* #31 Check elements of nPdu timeout. */
#if ( SOAD_TCP == STD_ON )
    case SOAD_TIMEOUT_LIST_N_PDU:
#else
    default: /* SOAD_TIMEOUT_LIST_N_PDU */
#endif /* SOAD_TCP == STD_ON */
    {
      SoAd_TimeoutListNPduUdpTxIterType timeListNPduUdpTxIter;

      /* Optional indirection with constant index: Refer to precondition in function description. */
      timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListNPduUdpTxConfig(
        SoAd_GetTimeoutListNPduUdpTxConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, partitionIdx), partitionIdx);

      /* #310 Search for timeout. */
      for ( timeListNPduUdpTxIter = (SoAd_SizeOfTimeoutListNPduUdpTxType)*ElementIdxPtr;
        timeListNPduUdpTxIter < SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, partitionIdx);
        timeListNPduUdpTxIter++ )
      {
        if ( SoAd_GetTimeoutOfTimeoutListNPduUdpTx(timeListNPduUdpTxIter, partitionIdx) == timerCounter )
        {
          /* #3100 Return first occurred timeout. */
          *ElementIdxPtr = (uint32)timeListNPduUdpTxIter;                                                              /* SBSW_SOAD_POINTER_WRITE */
          *HandleIdxPtr = SoAd_GetNPduUdpTxIdxOfTimeoutListNPduUdpTx(timeListNPduUdpTxIter, partitionIdx);             /* SBSW_SOAD_POINTER_WRITE */
          retVal = E_OK;
          break;
        }
      }
      break;
    }
    /* #32 Check elements of TCP auto connect timeout. */
#if ( SOAD_TCP == STD_ON )
    default: /* SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT */
    {
      SoAd_TimeoutListTcpAutoConnectIterType timeListTcpAutoConnectIter;

      /* Optional indirection with constant index: Refer to precondition in function description. */
      timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListTcpAutoConnectConfig(
        SoAd_GetTimeoutListTcpAutoConnectConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, partitionIdx), partitionIdx);

      /* #320 Search for timeout. */
      for ( timeListTcpAutoConnectIter = (SoAd_SizeOfTimeoutListTcpAutoConnectType)*ElementIdxPtr;
        timeListTcpAutoConnectIter < SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx, partitionIdx);
        timeListTcpAutoConnectIter++ )
      {
        if ( SoAd_GetTimeoutOfTimeoutListTcpAutoConnect(timeListTcpAutoConnectIter, partitionIdx) == timerCounter )
        {
          /* #3200 Return first occurred timeout. */
          *ElementIdxPtr = (uint32)timeListTcpAutoConnectIter;                                                         /* SBSW_SOAD_POINTER_WRITE */
          *HandleIdxPtr = SoAd_GetSoConIdxOfTimeoutListTcpAutoConnect(timeListTcpAutoConnectIter, partitionIdx);       /* SBSW_SOAD_POINTER_WRITE */
          retVal = E_OK;
          break;
        }
      }
      break;
    }
#endif /* SOAD_TCP == STD_ON */
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TimeoutList_CheckElements() */

/**********************************************************************************************************************
 *  SoAd_TimeoutList_RemoveElement()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TimeoutList_RemoveElement(
  SoAd_TimeoutListEnumType TimeoutList,
  uint32 HandleIdx,
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfTimeoutListIdentType timeoutListIdentIdx = SoAd_GetTimeoutListIdentIdxOfInstance(InstanceIdx,
    PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that timeout list is written inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Remove element from the requested timeout list. */
  switch ( TimeoutList )
  {
    /* #30 Remove element from UDP alive supervision timeout. */
    case SOAD_TIMEOUT_LIST_UDP_ALIVE:
    {
      SoAd_SizeOfSoConType                soConIdx;
      SoAd_SizeOfTimeoutListUdpAliveType  timeListUdpAliveIdx;
      SoAd_SizeOfTimeoutListUdpAliveType  lastTimeListUdpAliveIdx;

      soConIdx = (SoAd_SizeOfSoConType)HandleIdx;
      timeListUdpAliveIdx = SoAd_GetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(soConIdx, PartitionIdx);

      /* #300 Check if handle index has an element. */
      if ( timeListUdpAliveIdx != SoAd_GetSizeOfTimeoutListUdpAlive(PartitionIdx) )
      {
        /* Optional indirection with constant index: Refer to precondition in function description. */
        SoAd_SizeOfTimeoutListMgmtType timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListUdpAliveConfig(
          SoAd_GetTimeoutListUdpAliveConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, PartitionIdx), PartitionIdx);

        /* #3000 Remove element of handle index. */
        SoAd_DecLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx);                                                   /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(soConIdx, SoAd_GetSizeOfTimeoutListUdpAlive(            /* SBSW_SOAD_PARAMETER_IDX */
          PartitionIdx), PartitionIdx);

        /* Get index of last element in list (lvl is already decremented so lvl is last index). */
        lastTimeListUdpAliveIdx = (SoAd_SizeOfTimeoutListUdpAliveType)SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx,
          PartitionIdx);

        /* #3001 Check if list has more elements. */
        if ( (lastTimeListUdpAliveIdx > 0u) && (lastTimeListUdpAliveIdx != timeListUdpAliveIdx) )
        {
          /* #30010 Replace removed element by last element. */
          /* Replace at index. */
          SoAd_SetSoConIdxOfTimeoutListUdpAlive(timeListUdpAliveIdx,                                                   /* SBSW_SOAD_CALCULATED_TimeoutListUdpAlive */
            SoAd_GetSoConIdxOfTimeoutListUdpAlive(lastTimeListUdpAliveIdx, PartitionIdx), PartitionIdx);
          SoAd_SetTimeoutOfTimeoutListUdpAlive(timeListUdpAliveIdx,                                                    /* SBSW_SOAD_CALCULATED_TimeoutListUdpAlive */
            SoAd_GetTimeoutOfTimeoutListUdpAlive(lastTimeListUdpAliveIdx, PartitionIdx), PartitionIdx);

          /* Update map of replacing element. */
          SoAd_SetTimeoutListUdpAliveIdxOfTimeoutListUdpAliveMap(                                                      /* SBSW_SOAD_CALCULATED_TimeoutListUdpAliveMap */
            SoAd_GetSoConIdxOfTimeoutListUdpAlive(lastTimeListUdpAliveIdx, PartitionIdx),
            timeListUdpAliveIdx, PartitionIdx);
        }
      }
      break;
    }
    /* #31 Remove element from nPdu timeout. */
#if ( SOAD_TCP == STD_ON )
    case SOAD_TIMEOUT_LIST_N_PDU:
#else
    default: /* SOAD_TIMEOUT_LIST_N_PDU */
#endif /* SOAD_TCP == STD_ON */
    {
      SoAd_SizeOfNPduUdpTxType            nPduUdpTxIdx;
      SoAd_SizeOfTimeoutListNPduUdpTxType timeListNPduUdpTxIdx;
      SoAd_SizeOfTimeoutListNPduUdpTxType lastTimeListNPduUdpTxIdx;

      nPduUdpTxIdx = (SoAd_SizeOfNPduUdpTxType)HandleIdx;
      timeListNPduUdpTxIdx = SoAd_GetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(nPduUdpTxIdx, PartitionIdx);

      /* #310 Check if handle index has an element. */
      if ( timeListNPduUdpTxIdx != SoAd_GetSizeOfTimeoutListNPduUdpTx(PartitionIdx) )
      {
        /* Optional indirection with constant index: Refer to precondition in function description. */
        SoAd_SizeOfTimeoutListMgmtType timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListNPduUdpTxConfig(
          SoAd_GetTimeoutListNPduUdpTxConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, PartitionIdx), PartitionIdx);

        /* #3100 Remove element of handle index. */
        SoAd_DecLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx);                                                   /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(nPduUdpTxIdx, SoAd_GetSizeOfTimeoutListNPduUdpTx(     /* SBSW_SOAD_PARAMETER_IDX */
          PartitionIdx), PartitionIdx);

        /* Get index of last element in list (lvl is already decremented so lvl is last index). */
        lastTimeListNPduUdpTxIdx = (SoAd_SizeOfTimeoutListNPduUdpTxType)SoAd_GetLvlOfTimeoutListMgmt(timeListMgmtIdx,
          PartitionIdx);

        /* #3101 Check if list has more elements. */
        if ( (lastTimeListNPduUdpTxIdx > 0u) && (lastTimeListNPduUdpTxIdx != timeListNPduUdpTxIdx) )
        {
          /* #31010 Replace removed element by last element. */
          /* Replace at index. */
          SoAd_SetNPduUdpTxIdxOfTimeoutListNPduUdpTx(timeListNPduUdpTxIdx,                                             /* SBSW_SOAD_CALCULATED_TimeoutListNPduUdpTx */
            SoAd_GetNPduUdpTxIdxOfTimeoutListNPduUdpTx(lastTimeListNPduUdpTxIdx, PartitionIdx), PartitionIdx);
          SoAd_SetTimeoutOfTimeoutListNPduUdpTx(timeListNPduUdpTxIdx,                                                  /* SBSW_SOAD_CALCULATED_TimeoutListNPduUdpTx */
            SoAd_GetTimeoutOfTimeoutListNPduUdpTx(lastTimeListNPduUdpTxIdx, PartitionIdx), PartitionIdx);

          /* Update map of replacing element. */
          SoAd_SetTimeoutListNPduUdpTxIdxOfTimeoutListNPduUdpTxMap(                                                    /* SBSW_SOAD_CALCULATED_TimeoutListNPduUdpTxMap */
            SoAd_GetNPduUdpTxIdxOfTimeoutListNPduUdpTx(lastTimeListNPduUdpTxIdx, PartitionIdx),
            timeListNPduUdpTxIdx, PartitionIdx);
        }
      }
      break;
    }
    /* #32 Remove element from TCP auto connect timeout. */
#if ( SOAD_TCP == STD_ON )
    default: /* SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT */
    {
      SoAd_SizeOfSoConType                     soConIdx;
      SoAd_SizeOfTimeoutListTcpAutoConnectType timeListTcpAutoConnectIdx;
      SoAd_SizeOfTimeoutListTcpAutoConnectType lastTimeListTcpAutoConnectIdx;

      soConIdx = (SoAd_SizeOfSoConType)HandleIdx;
      timeListTcpAutoConnectIdx = SoAd_GetTimeoutListTcpAutoConnectIdxOfTimeoutListTcpAutoConnectMap(soConIdx,
        PartitionIdx);

      /* #320 Check if handle index has an element. */
      if ( timeListTcpAutoConnectIdx != SoAd_GetSizeOfTimeoutListTcpAutoConnect(PartitionIdx) )
      {
        /* Optional indirection with constant index: Refer to precondition in function description. */
        SoAd_SizeOfTimeoutListMgmtType timeListMgmtIdx = SoAd_GetTimeoutListMgmtIdxOfTimeoutListTcpAutoConnectConfig(
          SoAd_GetTimeoutListTcpAutoConnectConfigIdxOfTimeoutListIdent(timeoutListIdentIdx, PartitionIdx),
          PartitionIdx);

        /* #3200 Remove element of handle index. */
        SoAd_DecLvlOfTimeoutListMgmt(timeListMgmtIdx, PartitionIdx);                                                   /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTimeoutListTcpAutoConnectIdxOfTimeoutListTcpAutoConnectMap(soConIdx,                                   /* SBSW_SOAD_PARAMETER_IDX */
          SoAd_GetSizeOfTimeoutListTcpAutoConnect(PartitionIdx), PartitionIdx);

        /* Get index of last element in list (lvl is already decremented so lvl is last index). */
        lastTimeListTcpAutoConnectIdx = (SoAd_SizeOfTimeoutListTcpAutoConnectType)SoAd_GetLvlOfTimeoutListMgmt(
          timeListMgmtIdx, PartitionIdx);

        /* #3201 Check if list has more elements. */
        if ( (lastTimeListTcpAutoConnectIdx > 0u) && (lastTimeListTcpAutoConnectIdx != timeListTcpAutoConnectIdx) )
        {
          /* #32010 Replace removed element by last element. */
          /* Replace at index. */
          SoAd_SetSoConIdxOfTimeoutListTcpAutoConnect(timeListTcpAutoConnectIdx,                                       /* SBSW_SOAD_CALCULATED_TimeoutListTcpAutoConnect */
            SoAd_GetSoConIdxOfTimeoutListTcpAutoConnect(lastTimeListTcpAutoConnectIdx, PartitionIdx), PartitionIdx);
          SoAd_SetTimeoutOfTimeoutListTcpAutoConnect(timeListTcpAutoConnectIdx,                                        /* SBSW_SOAD_CALCULATED_TimeoutListTcpAutoConnect */
            SoAd_GetTimeoutOfTimeoutListTcpAutoConnect(lastTimeListTcpAutoConnectIdx, PartitionIdx), PartitionIdx);

          /* Update map of replacing element. */
          SoAd_SetTimeoutListTcpAutoConnectIdxOfTimeoutListTcpAutoConnectMap(                                          /* SBSW_SOAD_CALCULATED_TimeoutListTcpAutoConnectMap */
            SoAd_GetSoConIdxOfTimeoutListTcpAutoConnect(lastTimeListTcpAutoConnectIdx, PartitionIdx),
            timeListTcpAutoConnectIdx, PartitionIdx);
        }
      }
      break;
    }
#endif /* SOAD_TCP == STD_ON */
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_TimeoutList_RemoveElement() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/* Concept for TimeoutList:

  All timeouts handled in a list use the same mechanism. Therefore, there is a generic handling for all timeout lists.
  Each timeout list is identified by an enum value used in the API functions. A generic parameter name "HandleIdx" is
  used in the API functions to identify the object with timeout (e.g. timeout list: UdpAlive -> handle: SoConIdx). The
  timeout value depends on a global counter incremented in each main function. The timeout list stores the value of the
  global counter when the timeout will exceed. The timeout calculation is designed to handle one counter overflow.

  The timeout list entries are stored in "SoAd_TimeoutList<TimeoutList>". When an object adds a timeout to this list
  "SoAd_TimeoutList<TimeoutList>Map" is used to map the object index to the index/element of the timeout list.
  The timeout list is not sorted according to the timeouts. So in main function each element has to be checked. The
  timeout list is not designed to have unused elements. Therefore, when a timeout exceeds and is removed from the
  timeout list the last timeout list element is moved to the index of the removed element.

*/

/**********************************************************************************************************************
 *  END OF FILE: SoAd_TimeoutList.c
 *********************************************************************************************************************/
