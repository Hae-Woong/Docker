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
 *         File:  Dem_J1939Dcm.h
 *      Project:  MICROSAR Diagnostic Event Manager (Dem)
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Static public API declarations of Dem for the J1939Dcm.
 *                Dynamically available APIs are declared in the generated file Rte_Dem.h
 *
 *  \entity       Dem
 *********************************************************************************************************************/
/*!
 *  \addtogroup   Dem_Ungrouped
 *  \{
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_J1939DCM_H)
#define DEM_J1939DCM_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                               /* Dem itself */
/* ------------------------------------------------------------------------- */
#include "Dem.h"
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 #define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_J1939DcmAPI_Public Public Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmClearSingleDTC
 *****************************************************************************/
/*!
 * \brief         Clears a single J1939 DTC.
 *
 * \details       Clears a single J1939 DTC.
 *
 * \param[in]     DTCTypeFilter
 *                DEM_J1939DTC_CLEAR_ACTIVE
 *                Clear the requested DTC if it is active.
 *                DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE
 *                Clear the requested DTC if it was previously active.
 * \param[in]     J1939DTC
 *                The J1939 DTC to be cleared.
 * \param[in]     DTCOrigin
 *                The external identifier of the event memory.
 *                Must be primary memory.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                DTC successfully cleared
 * \return        DEM_WRONG_DTC
 *                DTC value not existing (in this format) or not a single DTC 
 *                was selected
 * \return        DEM_WRONG_DTCORIGIN
 *                Wrong DTC origin
 * \return        DEM_CLEAR_FAILED
 *                DTC clearing failed
 * \return        DEM_WRONG_DTCTYPEFILTER
 *                The J1939 DTC does not match the given DTCTypeFilter.
 * \return        DEM_PENDING
 *                The clear operation was started and is currently processed
 * \return        DEM_CLEAR_BUSY
 *                Another clear operation is currently processing
 * \return        DEM_CLEAR_MEMORY_ERROR
 *                The clear operation has completed in RAM, but synchronization to NVRAM has failed.
 *
 * \config        DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientId
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmClearSingleDTC(
  Dem_J1939DcmSetClearFilterType  DTCTypeFilter,
  uint32 J1939DTC,
  Dem_DTCOriginType  DTCOrigin,
  uint8  ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmClearDTC
 *****************************************************************************/
/*!
 * \brief         Clear the J1939 DTCs.
 *
 * \details       Clears the J1939 DTCs.
 *
 * \param[in]     DTCTypeFilter
 *                DEM_J1939DTC_CLEAR_ACTIVE
 *                All active DTCs shall be cleared.
 *                DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE
 *                All previously active DTCs shall be cleared.
 *                DEM_J1939DTC_CLEAR_ACTIVE_AND_PREVIOUSLY_ACTIVE
 *                All active and previously active DTCs shall be cleared.
 * \param[in]     DTCOrigin
 *                The external identifier of the event memory.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                DTC successfully cleared
 * \return        DEM_WRONG_DTC
 *                DTC value not existing (in this format)
 * \return        DEM_WRONG_DTCORIGIN
 *                Wrong DTC origin
 * \return        DEM_CLEAR_FAILED
 *                DTC clearing failed
 * \return        DEM_PENDING
 *                The clear operation was started and is currently processed
 * \return        DEM_CLEAR_BUSY
 *                Another clear operation is currently processing
 * \return        DEM_CLEAR_MEMORY_ERROR
 *                The clear operation has completed in RAM, but synchronization to NVRAM has failed.
 *
 * \config        DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientId
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmClearDTC(
  Dem_J1939DcmSetClearFilterType  DTCTypeFilter,
  Dem_DTCOriginType  DTCOrigin,
  uint8  ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmFirstDTCwithLampStatus
 *****************************************************************************/
/*!
 * \brief         Initialize the filter mechanism to the first event in the
 *                primary memory.
 *
 * \details       Initializes the filter mechanism to the first event in the
 *                primary memory.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \config           DEM_CFG_SUPPORT_J1939_DM31 == STD_ON
 *                && DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientId
 *****************************************************************************/
FUNC(void, DEM_CODE)
Dem_J1939DcmFirstDTCwithLampStatus(
  uint8  ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmGetNextDTCwithLampStatus
 *****************************************************************************/
/*!
 * \brief         Get the next filtered J1939 DTC for DM31 including current
 *                LampStatus.
 *
 * \details       Return the next filtered J1939 DTC for DM31 including current
 *                LampStatus.
 *
 * \param[out]    LampStatus
 *                Pointer to the LampStatus buffer.
 * \param[out]    J1939DTC
 *                Pointer to the J1939DTC buffer.
 * \param[out]    OccurrenceCounter
 *                Pointer to the OccurrenceCounter buffer.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                A matching DTC was returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No further element was found, iteration is complete.
 * \return        DEM_PENDING
 *                Never returned by this implementation.
 *
 * \config           DEM_CFG_SUPPORT_J1939_DM31 == STD_ON
 *                && DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 * \pre           A filter has been initialized using
 *                Dem_J1939DcmFirstDTCwithLampStatus
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmGetNextDTCwithLampStatus(
  P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_J1939DCM_DATA)  LampStatus,
  P2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  P2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  uint8  ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmSetDTCFilter
 *****************************************************************************/
/*!
 * \brief         Initialize the filter criteria for the J1939 DTC filter
 *                mechanism.
 *
 * \details       This function sets up the filter parameters to iterate all
 *                matching DTCs. If the filter is valid, the node specific
 *                lamp status is returned immediately, according to J1939.
 *                format:
 *                HighByte
 *                 bits 7,6 - Malfunction Indicator Lamp Status
 *                 bits 5,4 - Red Stop Lamp Status
 *                 bits 3,2 - Amber Warning Lamp Status
 *                 bits 1,0 - Protect Lamp Status
 *                LowByte
 *                 bits 7,6 - Flash Malfunction Indicator Lamp
 *                 bits 5,4 - Flash Red Stop Lamp
 *                 bits 3,2 - Flash Amber Warning Lamp
 *                 bits 1,0 - Flash Protect Lamp
 *
 * \param[in]     DTCStatusFilter
 *                DTC status to filter.
 * \param[in]     DTCKind
 *                DTC kind to filter.
 * \param[in]     DTCOrigin
 *                The external identifier of the event memory.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    LampStatus
 *                Pointer to receive the Lamp status for the requested node.
 *
 * \return        E_OK
 *                The filter parameters were accepted.
 * \return        E_NOT_OK
 *                The filter could not be set.
 *
 * \config        DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientId
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmSetDTCFilter(
  Dem_J1939DcmDTCStatusFilterType  DTCStatusFilter,
  Dem_DTCKindType  DTCKind,
  Dem_DTCOriginType  DTCOrigin,
  uint8  ClientId,
  P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_J1939DCM_DATA)  LampStatus
);
#endif

#if (DEM_CFG_SUPPORT_J1939_RATIO == STD_ON)
/* ****************************************************************************
 *  Dem_J1939DcmSetRatioFilter
 *****************************************************************************/
/*!
 * \brief         Initialize the Ratio filter.
 *
 * \details       Set the Ratio filter for a specific node and return the
 *                Ignition Cycle Counter and General Denominator.
 *
 * \param[out]    IgnitionCycleCounter
 *                Pointer to receive the Ignition Cycle Counter.
 * \param[out]    OBDMonitoringConditionsEncountered
 *                Pointer to receive the OBD General Denominator.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                The filter parameters were accepted.
 * \return        E_NOT_OK
 *                The filter could not be set.
 *
 * \config        DEM_CFG_SUPPORT_J1939_RATIO == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmSetRatioFilter(
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) IgnitionCycleCounter,
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) OBDMonitoringConditionsEncountered,
  uint8 ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmGetNumberOfFilteredDTC
 *****************************************************************************/
/*!
 * \brief         Calculate the number of DTCs matching the filter criteria.
 *
 * \details       Calculates the number of DTCs matching the filter criteria.
 *
 * \param[out]    NumberOfFilteredDTC
 *                Pointer to receive the DTC count.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                The number of matching DTCs was returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                The number of matching DTCs could not be calculated.
 * \return        DEM_PENDING
 *                Never returned by this implementation.
 *
 * \config        DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 * \pre           The filter has to be initialized by Dem_J1939DcmSetDTCFilter
 *
 * \context       TASK|ISR2
 * \synchronous   FALSE
 * \reentrant     FALSE
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmGetNumberOfFilteredDTC(
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA)  NumberOfFilteredDTC,
  uint8  ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmGetNextFilteredDTC
 *****************************************************************************/
/*!
 * \brief         Get the next matching DTC.
 *
 * \details       Returns the next matching DTC.
 *
 * \param[out]    J1939DTC
 *                Pointer to receive the J1939 DTC number.
 * \param[out]    OccurrenceCounter
 *                Pointer to receive the occurrence counter.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                The next filtered element was returned
 * \return        DEM_NO_SUCH_ELEMENT
 *                No further element was found, iteration is complete
 * \return        DEM_PENDING
 *                The requested element is calculated asynchronously and 
 *                currently not available. The caller can retry later.
 *
 * \config        DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 * \pre           The filter has to be initialized by Dem_J1939DcmSetDTCFilter
 *
 * \context       TASK|ISR2
 * \synchronous   FALSE
 * \reentrant     FALSE
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmGetNextFilteredDTC(
  P2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  P2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  uint8 ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_RATIO == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmGetNextFilteredRatio
  *****************************************************************************/
/*!
 * \brief         Get the next filtered Ratio.
 *
 * \details       Get the SPN, Numerator and Denominator of the next
 *                filtered Ratio.
 *
 * \param[out]    SPN
 *                Pointer to receive the SPN.
 * \param[out]    Numerator
 *                Pointer to receive the Numerator.
 * \param[out]    Denominator
 *                Pointer to receive the Denominator.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                The next filtered element was returned
 * \return        DEM_NO_SUCH_ELEMENT
 *                No further element was found, iteration is complete
 * \return        DEM_PENDING
 *                Never returned by this implementation.
 * \return        DEM_BUFFER_TOO_SMALL
 *                Never returned by this implementation.
 *
 * \config        DEM_CFG_SUPPORT_J1939_RATIO == STD_ON
 * \pre           The filter has to be initialized by
 *                Dem_J1939DcmSetRatioFilter().
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmGetNextFilteredRatio(
  P2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA) SPN,
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) Numerator,
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) Denominator,
  uint8 ClientId
);
#endif

#if ((DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME_FOR_OBDII == STD_ON))
/* ****************************************************************************
 * Dem_J1939DcmSetFreezeFrameFilter
 *****************************************************************************/
/*!
 * \brief         Initialize the filter criteria for freeze frame access.
 *
 * \details       Initializes the filter criteria for freeze frame access.
 *
 * \param[in]     FreezeFrameKind
 *                DEM_J1939DCM_FREEZEFRAME
 *                Set the filter for J1939 Freeze Frame data.
 *                DEM_J1939DCM_EXPANDED_FREEZEFRAME
 *                Set the filter for J1939 Expanded Freeze Frame data.
 *                DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME
 *                Set the filter for J1939 SPNs in Expanded Freeze Frame.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                The filter parameters were accepted.
 * \return        E_NOT_OK
 *                Filter could not be set.
 *
 * \config           DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *                || DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME_FOR_OBDII == STD_ON
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientId
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmSetFreezeFrameFilter(
  Dem_J1939DcmSetFreezeFrameFilterType  FreezeFrameKind,
  uint8  ClientId
);
#endif

#if ((DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME_FOR_OBDII == STD_ON))
/* ****************************************************************************
 * Dem_J1939DcmGetNextFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Get the next J1939DTC and Freeze Frame matching the
 *                filter criteria.
 *
 * \details       Returns the next J1939DTC and Freeze Frame matching the
 *                filter criteria.
 *
 * \param[out]    J1939DTC
 *                Pointer to the J1939DTC buffer.
 * \param[out]    OccurrenceCounter
 *                Pointer to the OccurrenceCounter buffer.
 * \param[out]    DestBuffer
 *                Pointer to the output buffer.
 * \param[in,out] BufSize
 *                In: Available space in the receiving buffer.
 *                Out: Number of written bytes.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                The next filtered element was returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No further element was found, iteration is complete.
 * \return        DEM_PENDING
 *                The operation was started and is currently processed.
 * \return        DEM_BUFFER_TOO_SMALL
 *                The provided buffer is not large enough.
 *
 * \config           DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *                || DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME_FOR_OBDII == STD_ON
 * \pre           A filter has been initialized using Dem_J1939DcmSetFreezeFrameFilter
 *
 * \context       TASK|ISR2
 * \synchronous   FALSE
 * \reentrant     FALSE
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmGetNextFreezeFrame(
  P2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  P2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  P2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  DestBuffer,
  P2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  BufSize,
  uint8  ClientId
);
#endif

#if ((DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON) || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME_FOR_OBDII == STD_ON))
/* ****************************************************************************
 * Dem_J1939DcmGetNextSPNInFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Get the SPNs that are stored in the J1939 FreezeFrame(s).
 *
 * \details       Get the SPNs that are stored in the J1939 FreezeFrame(s) for DM24.
 *
 * \param[out]    SPNSupported
 *                Contains the next SPN in the ExpandedFreezeFrame.
 * \param[out]    SPNDataLength
 *                Contains the corresponding dataLength of the SPN.
 * \param[in]     ClientId
 *                Unique client id to address the J1939 event memory.
 *
 * \return        E_OK
 *                Operation was successful.
 * \return        DEM_NO_SUCH_ELEMENT
 *                The requested element is not available.
 * \return        DEM_PENDING
 *                The operation was started and is currently processed.
 *                Not used by this implementation.
 * \return        DEM_BUFFER_TOO_SMALL
 *                Not used by this implementation.
 *
 * \config           DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON
 *                || DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME_FOR_OBDII == STD_ON
 *
 * \pre           A filter has been initialized using
 *                Dem_J1939DcmSetFreezeFrameFilter
 *
 * \context       TASK|ISR2
 * \synchronous   FALSE
 * \reentrant     FALSE
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmGetNextSPNInFreezeFrame(
  P2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  SPNSupported,
  P2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  SPNDataLength, 
  uint8  ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_READINESS1 == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmReadDiagnosticReadiness1
 *****************************************************************************/
/*!
 * \brief         Report the value of Diagnostic Readiness 1 (DM5)
 *
 * \details       Returns the value of Diagnostic Readiness 1 (DM5)
 *
 * \param[out]    DataValue
 *                Pointer to receive the DM5 data.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                Operation was successful.
 * \return        E_NOT_OK
 *                Operation failed.
 *
 * \config        DEM_CFG_SUPPORT_J1939_READINESS1 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientId
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness1(
  P2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, DEM_J1939DCM_DATA)  DataValue,
  uint8  ClientId
);
#endif

#if (DEM_CFG_SUPPORT_J1939_READINESS2 == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmReadDiagnosticReadiness2
 *****************************************************************************/
/*!
 * \brief         Service to report the value of Diagnostic Readiness 2 (DM21)
 *                computed by the Dem.
 *
 * \details       Reports the diagnostic information relevant to a second PGN
 *                conveying diagnostic readiness according to DM21.
 *
 * \param[out]    DataValue
 *                Buffer of 8 bytes containing the contents of Diagnostic
 *                Readiness 2 (DM21) computed by the Dem.
 * \param[in]     ClientId
 *                ClientId to address the J1939 event memory.
 *
 * \return        E_OK
 *                Operation was successful.
 * \return        E_NOT_OK
 *                Operation failed.
 *
 * \config        DEM_CFG_SUPPORT_J1939_READINESS2 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds, FALSE for same ClientId.
 *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness2(
  P2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_J1939DCM_DATA)  DataValue,
  uint8  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_J1939_READINESS3 == STD_ON)
/* ****************************************************************************
 * Dem_J1939DcmReadDiagnosticReadiness3
 *****************************************************************************/
/*!
 * \brief         Service to report the value of Diagnostic Readiness 3 (DM26)
 *                computed by the Dem.
 *
 * \details       Conveys the pending status of OBD system monitors for the
 *                current drive cycle.
 *
 * \param[out]    DataValue
 *                Buffer of 8 bytes containing the contents of Diagnostic
 *                Readiness 3 (DM26) computed by the Dem.
 * \param[in]     ClientId
 *                ClientId to address the J1939 event memory.
 *
 * \return        E_OK
 *                Operation was successful.
 * \return        E_NOT_OK
 *                Operation failed.
 *
 * \config        DEM_CFG_SUPPORT_J1939_READINESS3 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds, FALSE for same ClientId.
  *****************************************************************************/
FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness3(
  P2VAR(Dem_J1939DcmDiagnosticReadiness3Type, AUTOMATIC, DEM_J1939DCM_DATA) DataValue,
  uint8 ClientId
);
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif  /* DEM_J1939DCM_H */
/*!
 *  \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_J1939Dcm.h
 *********************************************************************************************************************/
