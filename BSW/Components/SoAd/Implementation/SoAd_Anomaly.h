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
/*!        \file  SoAd_Anomaly.h
 *        \brief  Socket Adaptor header file
 *
 *      \details  Vector static code header file for AUTOSAR Socket Adaptor sub-component Anomaly.
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

#if !defined (SOAD_ANOMALY_H)
# define SOAD_ANOMALY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "SoAd.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_Init()
 *********************************************************************************************************************/
/*! \brief          Initializes the count of dropped TCP/UDP PDUs, dropped TCP connections, dropped UDP frames and
 *                  invalid SOME/IP (SD) service/method IDs to zero.
 *  \details        - 
 *  \pre            - 
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         SOAD_GET_RESET_MEASUREMENT_DATA_API
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Anomaly_Init(void);
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

# if ( (SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON) || (SOAD_SECURITY_EVENT_REPORTING == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_Anomaly_Report()
 *********************************************************************************************************************/
/*! \brief          Reports an anomaly by incrementing the corresponding measurement counters and reporting the
 *                  security event if configured.
 *  \details        -
 *  \param[in]      MeasurementIdx  The measurement index of the counter type to be incremented and corresponding
 *                                  security event to be reported.
 *                                  [range: SOAD_MEAS_DROP_TCP, SOAD_MEAS_DROP_UDP, SOAD_MEAS_DROP_TCP_CONNECTION,
 *                                          SOAD_MEAS_DROP_UDP_SOCKET, SOAD_MEAS_DROP_UDP_LENGTH,
 *                                          SOAD_MEAS_INVALID_SOME_IP_SERVICE_ID, SOAD_MEAS_INVALID_SOME_IP_METHOD_ID,
 *                                          SOAD_MEAS_INVALID_SOME_IP_SD_SERVICE_ID,
 *                                          SOAD_MEAS_INVALID_SOME_IP_SD_METHOD_ID, SOAD_MEAS_ALL]
 *  \param[in]      PartitionIdx    Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         SOAD_GET_RESET_MEASUREMENT_DATA | SOAD_SECURITY_EVENT_REPORTING
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Anomaly_Report(
  SoAd_MeasurementIdxType MeasurementIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
# endif /* (SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON) || (SOAD_SECURITY_EVENT_REPORTING == STD_ON) */

# if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Anomaly_GetAndResetMeasurementData()
 *********************************************************************************************************************/
/*! \brief          Gets and Resets (if requested) the counter for dropped TCP/UDP PDUs, dropped TCP connections,
 *                  dropped UDP frames (wrong socket/length), invalid SOME/IP service identifiers, invalid SOME/IP
 *                  method identifiers, invalid SOME/IP SD service identifiers and invalid SOME/IP SD method
 *                  identifiers.
 *  \details        -
 *  \param[in]      MeasurementIdx          The index to select specific measurement data.
 *                                          [range: SOAD_MEAS_DROP_TCP, SOAD_MEAS_DROP_UDP,
 *                                                  SOAD_MEAS_DROP_TCP_CONNECTION, SOAD_MEAS_DROP_UDP_SOCKET,
 *                                                  SOAD_MEAS_DROP_UDP_LENGTH, SOAD_MEAS_INVALID_SOME_IP_SERVICE_ID,
 *                                                  SOAD_MEAS_INVALID_SOME_IP_METHOD_ID,
 *                                                  SOAD_MEAS_INVALID_SOME_IP_SD_SERVICE_ID,
 *                                                  SOAD_MEAS_INVALID_SOME_IP_SD_METHOD_ID, SOAD_MEAS_ALL]
 *  \param[in]      MeasurementResetNeeded  Flag to indicate if the counter needs to be reset.
 *                                          [range: TRUE, FALSE]
 *  \param[out]     MeasurementDataPtr      Buffer where the value of the counter is to be copied into.
 *                                          [range: POINTER may be NULL_PTR]
 *  \pre            -
 *  \return         E_OK                    The operations were successful.
 *  \return         E_NOT_OK                The operations failed.
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different measurement indexes, FALSE for the same measurement index.
 *  \synchronous    TRUE
 *  \config         SOAD_GET_RESET_MEASUREMENT_DATA_API
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Anomaly_GetAndResetMeasurementData(
  SoAd_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) MeasurementDataPtr);
# endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* !defined(SOAD_ANOMALY_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Anomaly.h
 *********************************************************************************************************************/
