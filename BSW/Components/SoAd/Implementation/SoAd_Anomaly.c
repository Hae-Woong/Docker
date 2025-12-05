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
/*!        \file  SoAd_Anomaly.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component Anomaly.
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

#define SOAD_ANOMALY_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_Anomaly.h"
#include "SoAd_Util.h"
#include "SoAd_Priv.h"

#if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
# include "IdsM.h"
#endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */

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

#if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_IncCtrValue()
 *********************************************************************************************************************/
/*! \brief          Increments the provided counter by one up to the maximum value.
 *  \details        - 
 *  \param[in]      MeasurementCounterIdx The index of the counter to be incremented.
 *                                        [range: MeasurementCounterIdx < SoAd_GetSizeOfMeasurementCounter()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         SOAD_GET_RESET_MEASUREMENT_DATA_API
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Anomaly_IncCtrValue(
  SoAd_SizeOfMeasurementCounterType MeasurementCounterIdx);

/**********************************************************************************************************************
 *  SoAd_Anomaly_GetAndResetCtrValues()
 *********************************************************************************************************************/
/*! \brief          Gets the sum of the relevant counter values and sets their reset flag if requested.
 *  \details        - 
 *  \param[in]      MeasurementCounterStartIdx The index of the first measurement counter to be get and reset.
 *                                             [range: MeasurementCounterStartIdx < SoAd_GetSizeOfMeasurementCounter()]
 *  \param[in]      MeasurementCounterEndIdx   The index of the first measurement counter not to be get and reset.
 *                                             [range: MeasurementCounterEndIdx < SoAd_GetSizeOfMeasurementCounter()]
 *  \param[in]      MeasurementResetNeeded     Flag to indicate if the counter needs to be reset.
 *                                             [range: TRUE, FALSE]
 *  \param[out]     MeasurementDataPtr         Buffer where the value of the counter is to be copied into.
 *                                             [range: POINTER may be NULL_PTR in a reset-only use-case]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         SOAD_GET_RESET_MEASUREMENT_DATA_API
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Anomaly_GetAndResetCtrValues(
  SoAd_SizeOfMeasurementCounterType MeasurementCounterStartIdx,
  SoAd_SizeOfMeasurementCounterType MeasurementCounterEndIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) MeasurementDataPtr);
#endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

#if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_ReportSecurityEvent()
 *********************************************************************************************************************/
/*! \brief          Reports the security event to the IdsM in case of a configured security event ID.
 *  \details        - 
 *  \param[in]      SecurityEventId   The ID of the security event to be reported.
 *                                    [range: SecurityEventId <= IDSM_EVENT_INVALID]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         SOAD_SECURITY_EVENT_REPORTING
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Anomaly_ReportSecurityEvent(
  IdsM_SecurityEventIdType SecurityEventId);
#endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_IncCtrValue()
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
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Anomaly_IncCtrValue(
  SoAd_SizeOfMeasurementCounterType MeasurementCounterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_AtomicUint32PtrType counterValPtr = SoAd_GetAddrMeasCounterValueOfMeasurementCounter(MeasurementCounterIdx);
  SoAd_AtomicUint8PtrType  counterFlagPtr = SoAd_GetAddrMeasCounterFlagOfMeasurementCounter(MeasurementCounterIdx);
  uint32                   currentCounterVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistent reads and writes on the counter from another instance. */
  SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();

  /* #20 Check if a reset of the counter value is required. */
  if ( SoAd_Util_LoadU8(counterFlagPtr) == SOAD_MEAS_FLAG_RESET_PENDING )                                              /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* #200 Set the flag to signalize that a reset is no longer pending. */
    SoAd_Util_StoreU8(counterFlagPtr, SOAD_MEAS_FLAG_RESET_NOT_PENDING, FALSE);                                        /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

    /* #201 Store the new count. */
    SoAd_Util_StoreU32(counterValPtr, SOAD_INC_DEC_BY_ONE, FALSE);                                                     /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }
  /* #21 Store the incremented value otherwise. */
  else
  {
    /* #210 Check if the counter has not yet reached the maximum value. */
    currentCounterVal = SoAd_Util_LoadU32(counterValPtr, FALSE);                                                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    if ( currentCounterVal < SOAD_MEASURE_COUNTER_MAX_VALUE )
    {
      /* #2100 Increment the counter. */
      SoAd_Util_FetchAddU32(counterValPtr, SOAD_INC_DEC_BY_ONE, FALSE);                                                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    }
  }

  /* #30 Leave critical section. */
  SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
} /* SoAd_Anomaly_IncCtrValue() */

/**********************************************************************************************************************
 *  SoAd_Anomaly_GetAndResetCtrValues()
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
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Anomaly_GetAndResetCtrValues(
  SoAd_SizeOfMeasurementCounterType MeasurementCounterStartIdx,
  SoAd_SizeOfMeasurementCounterType MeasurementCounterEndIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_MeasurementCounterIterType measCntIter;
  uint32                          currentMeasVal;
  uint32                          measDataSum = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistent reads and writes on the counter from another instance. */
  SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();

  /* #20 Iterate over all relevant measurement counters to add up their counter values. */
  for ( measCntIter = MeasurementCounterStartIdx; measCntIter < MeasurementCounterEndIdx; measCntIter++ )
  {
    /* #30 Check if no reset is pending for the measurement counter value. */
    if ( SoAd_Util_LoadU8(SoAd_GetAddrMeasCounterFlagOfMeasurementCounter(measCntIter)) ==                             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      SOAD_MEAS_FLAG_RESET_NOT_PENDING )
    {
      /* #40 Add the current counter value considering the maximum value. */
      currentMeasVal = SoAd_Util_LoadU32(SoAd_GetAddrMeasCounterValueOfMeasurementCounter(measCntIter), FALSE);        /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      if ( currentMeasVal < (SOAD_MEASURE_COUNTER_MAX_VALUE - measDataSum) )
      {
        measDataSum += currentMeasVal;
      }
      else
      {
        measDataSum = SOAD_MEASURE_COUNTER_MAX_VALUE;
      }

      /* #50 Set the measurement counter flag to signalize a pending reset if a measurement reset is requested. */
      if ( MeasurementResetNeeded == TRUE )
      {
        SoAd_Util_StoreU8(SoAd_GetAddrMeasCounterFlagOfMeasurementCounter(measCntIter), SOAD_MEAS_FLAG_RESET_PENDING,  /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
          FALSE);
      }
    }
    /* #31 Assume the current counter value to be zero otherwise. */
    else
    {
      /* Nothing to do as the counter value is assumed to be zero. */
    }
  }

  /* #60 Store the sum of the measurement counters to a buffer if measurement data pointer is no NULL_PTR. */
  if ( MeasurementDataPtr != NULL_PTR )
  {
    *MeasurementDataPtr = measDataSum;                                                                                 /* SBSW_SOAD_POINTER_WRITE */
  }

  /* #70 Leave critical section. */
  SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
} /* SoAd_Anomaly_GetAndResetCtrValues() */
#endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

#if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_ReportSecurityEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Anomaly_ReportSecurityEvent(
  IdsM_SecurityEventIdType SecurityEventId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reports the Security Event ID to the IdsM if it is valid. */
  if ( SecurityEventId != IDSM_EVENT_INVALID )
  {
    IdsM_SetSecurityEvent(SecurityEventId);
  }
} /* SoAd_Anomaly_ReportSecurityEvent() */
#endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_Anomaly_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_MeasurementCounterIterType measCntIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set all the measurement data counters and flags to their default value. */
  for ( measCntIter = 0; measCntIter < SoAd_GetSizeOfMeasurementCounter(); measCntIter++ )
  {
    SoAd_Util_StoreU32(SoAd_GetAddrMeasCounterValueOfMeasurementCounter(measCntIter), 0u, FALSE);                      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    SoAd_Util_StoreU8(SoAd_GetAddrMeasCounterFlagOfMeasurementCounter(measCntIter), SOAD_MEAS_FLAG_RESET_NOT_PENDING,  /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      FALSE);
  }
} /* SoAd_Anomaly_Init() */
#endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

#if ( (SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON) || (SOAD_SECURITY_EVENT_REPORTING == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_Anomaly_Report()
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
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, SOAD_CODE) SoAd_Anomaly_Report(
  SoAd_MeasurementIdxType MeasurementIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
  SoAd_SizeOfMeasurementCounterType measurementCounterIdx;
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Select the anomaly type to be incremented/reported. */
  switch ( MeasurementIdx )
  {
# if ( SOAD_TCP == STD_ON )
    case SOAD_MEAS_DROP_TCP:
    {
#  if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
      /* #20 Increment the dropped TCP PDU counter. */
      if ( SoAd_IsMeasurementCounterDroppedTcpPduUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterDroppedTcpPduIdxOfMeasurementDataMgmt(0u, PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
#  endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

#  if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
      /* #30 Report the security event for dropped TCP PDUs. */
      SoAd_Anomaly_ReportSecurityEvent(SoAd_GetSevDropPduRxTcpOfSecurityEvent(0u));
#  endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */
      break;
    }
# endif /* SOAD_TCP == STD_ON */
    case SOAD_MEAS_DROP_UDP:
    {
# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
      /* #40 Increment the dropped UDP PDU counter. */
      if ( SoAd_IsMeasurementCounterDroppedUdpPduUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterDroppedUdpPduIdxOfMeasurementDataMgmt(0u, PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

# if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
      /* #50 Report the security event for dropped UDP PDUs. */
      SoAd_Anomaly_ReportSecurityEvent(SoAd_GetSevDropPduRxUdpOfSecurityEvent(0u));
# endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */
      break;
    }
# if ( SOAD_TCP == STD_ON )
    case SOAD_MEAS_DROP_TCP_CONNECTION:
    {
#  if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
      /* #60 Increment the dropped TCP connection counter. */
      if ( SoAd_IsMeasurementCounterDroppedTcpConUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterDroppedTcpConIdxOfMeasurementDataMgmt(0u, PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
#  endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

#  if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
      /* #70 Report the security event for dropped TCP connections. */
      SoAd_Anomaly_ReportSecurityEvent(SoAd_GetSevRejectedTcpConnectionOfSecurityEvent(0u));
#  endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */
      break;
    }
# endif /* SOAD_TCP == STD_ON */
    case SOAD_MEAS_DROP_UDP_SOCKET:
    {
# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
      /* #80 Increment the dropped UDP socket counter. */
      if ( SoAd_IsMeasurementCounterDroppedUdpSocketUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterDroppedUdpSocketIdxOfMeasurementDataMgmt(0u, PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

# if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
      /* #90 Report the security event for dropped UDP sockets. */
      SoAd_Anomaly_ReportSecurityEvent(SoAd_GetSevDropMsgRxUdpSocketOfSecurityEvent(0u));
# endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */
      break;
    }
    case SOAD_MEAS_DROP_UDP_LENGTH:
    {
# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
      /* #100 Increment the dropped UDP length counter. */
      if ( SoAd_IsMeasurementCounterDroppedUdpLengthUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterDroppedUdpLengthIdxOfMeasurementDataMgmt(0u, PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

# if ( SOAD_SECURITY_EVENT_REPORTING == STD_ON )
      /* #110 Report the security event for dropped UDP length. */
      SoAd_Anomaly_ReportSecurityEvent(SoAd_GetSevDropMsgRxUdpLengthOfSecurityEvent(0u));
# endif /* SOAD_SECURITY_EVENT_REPORTING == STD_ON */
      break;
    }
# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
    case SOAD_MEAS_INVALID_SOME_IP_SERVICE_ID:
    {
      /* #120 Increment the invalid SOME/IP service ID counter. */
      if ( SoAd_IsMeasurementCounterInvalidSomeIpServiceIdUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterInvalidSomeIpServiceIdIdxOfMeasurementDataMgmt(0u,
          PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
      break;
    }
    case SOAD_MEAS_INVALID_SOME_IP_METHOD_ID:
    {
      /* #130 Increment the invalid SOME/IP method ID counter. */
      if ( SoAd_IsMeasurementCounterInvalidSomeIpMethodIdUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterInvalidSomeIpMethodIdIdxOfMeasurementDataMgmt(0u,
          PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
      break;
    }
    case SOAD_MEAS_INVALID_SOME_IP_SD_SERVICE_ID:
    {
      /* #140 Increment the invalid SOME/IP SD service ID counter. */
      if ( SoAd_IsMeasurementCounterInvalidSomeIpSdServiceIdUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterInvalidSomeIpSdServiceIdIdxOfMeasurementDataMgmt(
          0u, PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
      break;
    }
    case SOAD_MEAS_INVALID_SOME_IP_SD_METHOD_ID:
    {
      /* #150 Increment the invalid SOME/IP SD method ID counter. */
      if ( SoAd_IsMeasurementCounterInvalidSomeIpSdMethodIdUsedOfMeasurementDataMgmt(0u, PartitionIdx) )
      {
        measurementCounterIdx = SoAd_GetMeasurementCounterInvalidSomeIpSdMethodIdIdxOfMeasurementDataMgmt(0u,
          PartitionIdx);
        SoAd_Anomaly_IncCtrValue(measurementCounterIdx);
      }
      break;
    }
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */
    default:
    {
      /* Nothing to do. */
      break;
    }
  }

# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_OFF )
  SOAD_DUMMY_STATEMENT(PartitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_OFF */
} /* SoAd_Anomaly_Report() */
#endif /* (SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON) || (SOAD_SECURITY_EVENT_REPORTING == STD_ON) */

#if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_GetAndResetMeasurementData()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Anomaly_GetAndResetMeasurementData(
  SoAd_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfMeasurementCounterType measurementCounterStartIdx = 0u;
  SoAd_SizeOfMeasurementCounterType measurementCounterEndIdx = 0u;
  SoAd_MeasurementCounterIterType   measCntIter;
  Std_ReturnType                    retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch ( MeasurementIdx )
  {
# if ( SOAD_TCP == STD_ON )
    /* #10 Check if the measurement index is that of the dropped TCP PDU counter. */
    case SOAD_MEAS_DROP_TCP:
    {
      /* #20 Set the local measurement counter start and end index to those of the dropped TCP PDU counters. */
      if ( SoAd_IsMeasurementCounterUsedOfDroppedTcpPduCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfDroppedTcpPduCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfDroppedTcpPduCtr(0u);
        retVal = E_OK;
      }
      break;
    }
# endif /* SOAD_TCP == STD_ON */

    /* #30 Check if the measurement index is that of the dropped UDP PDU counter. */
    case SOAD_MEAS_DROP_UDP:
    {
      /* #40 Set the local measurement counter start and end index to those of the dropped UDP PDU counters. */
      if ( SoAd_IsMeasurementCounterUsedOfDroppedUdpPduCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfDroppedUdpPduCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfDroppedUdpPduCtr(0u);
        retVal = E_OK;
      }
      break;
    }

# if ( SOAD_TCP == STD_ON )
    /* #50 Check if the measurement index is that of the dropped TCP connections counter. */
    case SOAD_MEAS_DROP_TCP_CONNECTION:
    {
      /* #60 Set the local measurement counter start and end index to those of the dropped TCP connections counters. */
      if ( SoAd_IsMeasurementCounterUsedOfDroppedTcpConCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfDroppedTcpConCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfDroppedTcpConCtr(0u);
        retVal = E_OK;
      }
      break;
    }
# endif /* SOAD_TCP == STD_ON */

    /* #70 Check if the measurement index is that of the dropped UDP socket counter. */
    case SOAD_MEAS_DROP_UDP_SOCKET:
    {
      /* #80 Set the local measurement counter start and end index to those of the dropped UDP socket counters. */
      if ( SoAd_IsMeasurementCounterUsedOfDroppedUdpSocketCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfDroppedUdpSocketCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfDroppedUdpSocketCtr(0u);
        retVal = E_OK;
      }
      break;
    }

    /* #90 Check if the measurement index is that of the dropped UDP length counter. */
    case SOAD_MEAS_DROP_UDP_LENGTH:
    {
      /* #100 Set the local measurement counter start and end index to those of the dropped UDP length counters. */
      if ( SoAd_IsMeasurementCounterUsedOfDroppedUdpLengthCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfDroppedUdpLengthCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfDroppedUdpLengthCtr(0u);
        retVal = E_OK;
      }
      break;
    }

    /* #110 Check if the measurement index is that of the invalid SOME/IP service identifier counter. */
    case SOAD_MEAS_INVALID_SOME_IP_SERVICE_ID:
    {
      /* #120 Set the local measurement counter start and end index to those of the invalid SOME/IP service identifier
       *  counters. */
      if ( SoAd_IsMeasurementCounterUsedOfInvalidSomeIpServiceIdCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfInvalidSomeIpServiceIdCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfInvalidSomeIpServiceIdCtr(0u);
        retVal = E_OK;
      }
      break;
    }

    /* #130 Check if the measurement index is that of the invalid SOME/IP method identifier counter. */
    case SOAD_MEAS_INVALID_SOME_IP_METHOD_ID:
    {
      /* #140 Set the local measurement counter start and end index to those of the invalid SOME/IP method identifier
       *  counters. */
      if ( SoAd_IsMeasurementCounterUsedOfInvalidSomeIpMethodIdCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfInvalidSomeIpMethodIdCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfInvalidSomeIpMethodIdCtr(0u);
        retVal = E_OK;
      }
      break;
    }

    /* #150 Check if the measurement index is that of the invalid SOME/IP SD service identifier counter. */
    case SOAD_MEAS_INVALID_SOME_IP_SD_SERVICE_ID:
    {
      /* #160 Set the local measurement counter start and end index to those of the invalid SOME/IP SD service
       *      identifier counters. */
      if ( SoAd_IsMeasurementCounterUsedOfInvalidSomeIpSdServiceIdCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfInvalidSomeIpSdServiceIdCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfInvalidSomeIpSdServiceIdCtr(0u);
        retVal = E_OK;
      }
      break;
    }

    /* #170 Check if the measurement index is that of the invalid SOME/IP SD method identifier counter. */
    case SOAD_MEAS_INVALID_SOME_IP_SD_METHOD_ID:
    {
      /* #180 Set the local measurement counter start and end index to those of the invalid SOME/IP SD method
       *      identifier counters. */
      if ( SoAd_IsMeasurementCounterUsedOfInvalidSomeIpSdMethodIdCtr(0u) )
      {
        measurementCounterStartIdx = SoAd_GetMeasurementCounterStartIdxOfInvalidSomeIpSdMethodIdCtr(0u);
        measurementCounterEndIdx = SoAd_GetMeasurementCounterEndIdxOfInvalidSomeIpSdMethodIdCtr(0u);
        retVal = E_OK;
      }
      break;
    }

    /* #190 Check if the measurement index is that of ALL. */
    case SOAD_MEAS_ALL:
    {
      if ( MeasurementResetNeeded == TRUE )
      {
        /* #200 Enter critical section to prevent inconsistent reads and writes on the measurement counter flags. */
        SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();

        /* #210 Reset all the measurement data flags to signalize a pending reset if a reset is requested. */
        for ( measCntIter = 0; measCntIter < SoAd_GetSizeOfMeasurementCounter(); measCntIter++ )
        {
          SoAd_Util_StoreU8(SoAd_GetAddrMeasCounterFlagOfMeasurementCounter(measCntIter),                              /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
            SOAD_MEAS_FLAG_RESET_PENDING, FALSE);
        }

        /* #220 Leave critical section. */
        SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
      }

      retVal = E_OK;
      break;
    }

    default:
    {
      /* #230 Return E_NOT_OK as the requested measurement index is not supported. */
      break;
    }
  }

  /* #240 Check if measurement data has to be reported. */
  if ( measurementCounterStartIdx != measurementCounterEndIdx )
  {
    /* #250 Get the requested measurement counter values and set their reset flag if required. */
    SoAd_Anomaly_GetAndResetCtrValues(measurementCounterStartIdx, measurementCounterEndIdx, MeasurementResetNeeded,
      MeasurementDataPtr);                                                                                             /* SBSW_SOAD_POINTER_AND_NULL_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_Anomaly_GetAndResetMeasurementData() */
#endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Anomaly.c
 *********************************************************************************************************************/
