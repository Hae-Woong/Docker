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
 *  \defgroup   Dem_ClientAccessIF ClientAccessIF
 *  \{
 *  \file       Dem_ClientAccessIF_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) Interface declaration file
 *  \details    Public Interface of ClientAccess Unit
 *  \entity     ClientAccessIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CLIENTACCESSIF_INTERFACE_H)
#define DEM_CLIENTACCESSIF_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_FilterData_Types.h"
#include "Dem_DTCSelector_Types.h"


/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * \defgroup Dem_ClientAccessIF_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestDtcMatchesJ1939ClearFilter
 *****************************************************************************/
/*!
 * \brief         Tests if the DTC matches the given clear filter
 *
 * \details       Tests if the DTC corresponding to the given EventId matches 
 *                the given clear filter
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    TRUE: DTC matches the clear filter
 *                FALSE: DTC does not match the clear filter
 *
 * \pre           -
 * 
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDtcMatchesJ1939ClearFilter(
  Dem_EventIdType EventId,
  uint8 DTCTypeFilter
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GlobalClientCount
 *****************************************************************************/
/*!
 * \brief         Get the number of configured clients
 *
 * \details       Get the number of configured clients
 *
 * \return        The number of configured clients
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_Client_ClientAccess_GlobalClientCount(
  void
  );
 
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestSelectorValid
 *****************************************************************************/
/*!
 * \brief         Test if the client has a valid selector.
 *
 * \details       Test if the client has a valid selector.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                Client has a valid selector
 * \return        FALSE
 *                Client doesn't have a valid selector
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestSelectorValid(
  CONST(uint8, AUTOMATIC)  ClientId
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestClientIdValid
 *****************************************************************************/
/*!
 * \brief         Test that the client id is smaller than total number clients.
 *
 * \details       Test that the client id is smaller than total number clients.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                Client is valid
 * \return        FALSE
 *                Client is not valid
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestClientIdValid(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_SelectDTC
 *****************************************************************************/
/*!
 * \brief         Selects a DTC or DTC group as target for further operations.
 *
 * \details       For the given ClientId store the DTC, DTCFormat and
 *                DTCOrigin for later use by the following API calls:
 *                - Dem_GetStatusOfDTC
 *                - Dem_GetSeverityOfDTC
 *                - Dem_GetFunctionalUnitOfDTC
 *                - Dem_ClearDTC
 *                - Dem_DisableDTCRecordUpdate
 *                - Dem_EnableDTCRecordUpdate
 *                - Dem_GetDTCSelectionResult.
 *                The stored values may be invalid for the current
 *                configuration - they need a check before use!
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     DTC
 *                Defines the DTC in respective format that is selected. If the
 *                DTC fits to a DTC group number, the DTC group is selected.
 * \param[in]     DTCFormat
 *                Defines the input-format of the provided DTC value.
 * \param[in]     DTCOrigin
 *                The event memory of the requested DTC or group of DTC.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_Client_ClientAccess_SelectDTC(
  uint8 ClientId,
  uint32 DTC,
  Dem_DTCFormatType DTCFormat,
  Dem_DTCOriginType DTCOrigin
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetSelectionResult
 *****************************************************************************/
/*!
 * \brief         Returns the result of operation DTC selection.
 *
 * \details       For the given ClientId return the result of the asynchronous operation
 *                on API Dem_SelectDTC.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        DTC selection result.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCSelector_ResultType, DEM_CODE) 
Dem_Client_ClientAccess_GetSelectionResult(
  CONST(uint8, AUTOMATIC)  ClientId
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_GetSelectedMemoryId
 *****************************************************************************/
/*!
 * \brief         Returns the selected Memory Id
 *
 * \details       For the given ClientId return the memory Id selected by
 *                Dem_SelectDTC.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        Selected Memory Id
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE) 
Dem_Client_ClientAccess_GetSelectedMemoryId(
  CONST(uint8, AUTOMATIC) ClientId
  );
#endif

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetSelectedDTCFormat
 *****************************************************************************/
/*!
 * \brief         Returns the selected DTC Format
 *
 * \details       For the given ClientId return the DTC Format selected by
 *                Dem_SelectDTC.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        Selected DTC Format
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCFormatType, DEM_CODE)
Dem_Client_ClientAccess_GetSelectedDTCFormat(
  uint8 ClientId
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetSelectedEventId
 *****************************************************************************/
/*!
 * \brief         Returns the selected Event Id
 *
 * \details       For the given ClientId return the Event Id selected by
 *                Dem_SelectDTC.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        Selected Event Id
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE) 
Dem_Client_ClientAccess_GetSelectedEventId(
  CONST(uint8, AUTOMATIC) ClientId
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestSelectDTCLocked
 *****************************************************************************/
/*!
 * \brief         Tests if access to API Dem_SelectDTC is allowed
 *
 * \details       Verifies if for the given ClientId Dem_SelectDTC is granted
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                API access is locked
 *
 * \return        FALSE
 *                API access is granted
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) 
Dem_Client_ClientAccess_TestSelectDTCLocked(
  CONST(uint8, AUTOMATIC) ClientId
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestFilterValid
 *****************************************************************************/
/*!
 * \brief         Test if the client has a valid filter.
 *
 * \details       Test if the client has a valid filter.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                Client has a valid filter
 * \return        FALSE
 *                Client doesn't have a valid filter
 *
 * \config        -
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestFilterValid(
  CONST(uint8, AUTOMATIC)  ClientId
  );



#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_SetDTCFilter
 *****************************************************************************/
/*!
 * \brief         Initializes a DTC filter with the given criteria.
 *
 * \details       For the given ClientId set up a DTC filter with the given criteria.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     DTCStatusMask
 *                The status byte mask for DTC status byte filtering.
 * \param[in]     DTCFormat
 *                The DTC number format (OBD/OBD3Byte/UDS/J1939).
 * \param[in]     DTCOrigin
 *                The event memory of the requested DTC or group of DTC.
 * \param[in]     FilterWithSeverity
 *                Defines if severity information is used for filtering.
 * \param[in]     DTCSeverityMask
 *                The severity mask for severity filtering (only evaluated
 *                if severity information is used for filtering)
 * \param[in]     FilterForFDC
 *                Defines if FDC information is used for filtering.
 *
 * \return        E_OK
 *                Filter was accepted
 *                E_NOT_OK
 *                Filter was not accepted or no filter available for client
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) 
Dem_Client_ClientAccess_SetDTCFilter(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(uint8, AUTOMATIC)  DTCStatusMask,
  CONST(Dem_DTCFormatType, AUTOMATIC)  DTCFormat,
  CONST(Dem_DTCOriginType, AUTOMATIC)  DTCOrigin,
  CONST(boolean, AUTOMATIC)  FilterWithSeverity,
  CONST(Dem_DTCSeverityType, AUTOMATIC)  DTCSeverityMask,
  CONST(boolean, AUTOMATIC)  FilterForFDC
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextFilteredDTC
 *****************************************************************************/
/*!
 * \brief         Get the next matching event
 *
 * \details       Get the next event matching the filter criteria set with 
 *                Dem_Client_ClientAccess_SetDTCFilter
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    FilterResult
 *                Pointer to receive the next matching event
 *                FilterResult->EventId : The matching event
 *                FilterResult->FDC : The matching FDC, updated only when
 *                                    FilterForFDC is TRUE
 *                FilterResult->Severity : The matching severity, updated only
 *                                    when FilterWithSeverity is true
 *
 * \return        E_OK
 *                Matching element returned in FilterResult
 * \return        E_NOT_OK
 *                Filter was not set or no filter available for client
 * \return        E_DEM_NO_SUCH_ELEMENT
 *                No more matching elements, FilterResult not modified
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) 
Dem_Client_ClientAccess_GetNextFilteredDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(Dem_FilterData_ResultType, AUTOMATIC, AUTOMATIC)  FilterResult
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
 /* ****************************************************************************
 * Dem_Client_ClientAccess_TestDTCFilterSet
 *****************************************************************************/
/*!
 * \brief         Test if a DTC filter has been set.
 *
 * \details       Test if a DTC filter has been set for the given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A DTC filter has been set
 * \return        FALSE
 *                No DTC filter has been set
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) 
Dem_Client_ClientAccess_TestDTCFilterSet(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestDTCFilterWithFDCSet
 *****************************************************************************/
/*!
 * \brief         Test if a DTC filter with FDC filtering has been set.
 *
 * \details       Test if a DTC filter with FDC filtering has been set for 
 *                the given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A DTC filter with FDC filtering has been set
 * \return        FALSE
 *                No DTC filter with FDC filtering has been set
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) 
Dem_Client_ClientAccess_TestDTCFilterWithFDCSet(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestDTCFilterWithSeveritySet
 *****************************************************************************/
/*!
 * \brief         Test if a DTC filter with severity filtering has been set.
 *
 * \details       Test if a DTC filter with severity filtering has been set
 *                for the given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A DTC filter with severity filtering has been set
 * \return        FALSE
 *                No DTC filter with severity filtering has been set
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) 
Dem_Client_ClientAccess_TestDTCFilterWithSeveritySet(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
 /* ****************************************************************************
 * Dem_Client_ClientAccess_TestJ1939DTCFilterSet
 *****************************************************************************/
/*!
 * \brief         Test if a J1939 DTC filter has been set.
 *
 * \details       Test if a J1939 DTC filter has been set for the given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A J1939 DTC filter has been set
 * \return        FALSE
 *                No J1939 DTC filter has been set
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939DTCFilterSet(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestJ1939RatioFilterSet
 *****************************************************************************/
/*!
 * \brief         Test if a J1939 Ratio filter has been set.
 *
 * \details       Test if a J1939 Ratio filter has been set for the given
 *                ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A J1939 Ratio filter has been set.
 * \return        FALSE
 *                No J1939 Ratio filter has been set.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939RatioFilterSet(
  uint8 ClientId
);

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
 /* ****************************************************************************
 * Dem_Client_ClientAccess_TestJ1939DTCFilterWithLampStatusSet
 *****************************************************************************/
/*!
 * \brief         Test if a 'J1939 DTC with lamp status' filter has been set.
 *
 * \details       Test if a 'J1939 DTC with lamp status' filter has been set 
 *                for the given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A 'J1939 DTC with lamp status' filter has been set
 * \return        FALSE
 *                No 'J1939 DTC with lamp status' filter has been set
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939DTCFilterWithLampStatusSet(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
 /* ****************************************************************************
 * Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterSet
 *****************************************************************************/
/*!
 * \brief         Test if a J1939 freeze frame filter has been set.
 *
 * \details       Test if a J1939 freeze frame filter has been set for the 
 *                given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A J1939 freeze frame filter has been set
 * \return        FALSE
 *                No J1939 freeze frame filter has been set
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterSet(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

 /* ****************************************************************************
 * Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterFreezeFrameKind
 *****************************************************************************/
/*!
 * \brief         Test if a J1939 freeze frame filter has been set 
 *                to the expected FreezeFrameKind
 *
 * \details       Test if a J1939 freeze frame filter has been set 
 *                to the expected FreezeFrameKind
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     FreezeFrameKind
 *                Expected FreezeFrameKind
 *
 * \return        TRUE
 *                A J1939 freeze frame filter has been set to expected kind
 * \return        FALSE
 *                No J1939 freeze frame filter has been set to expected kind
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           J1939 should be supported
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterFreezeFrameKind(
  uint8  ClientId,
  Dem_J1939DcmSetFreezeFrameFilterType  FreezeFrameKind
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the buffers for all clients.
 *
 * \details       Initialize the DTC selector and filter for all clients.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_Client_ClientAccess_Init(
  void
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestReadoutBufferValid
 *****************************************************************************/
/*!
 * \brief         Test if the client has a valid readout buffer.
 *
 * \details       Test if the client has a valid readout buffer.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                Client has a valid readout buffer
 * \return        FALSE
 *                Client doesn't have a valid readout buffer
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestReadoutBufferValid(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_GetReadoutBuffer
 *****************************************************************************/
/*!
 * \brief         Return the readout buffer handle for the client
 *
 * \details       Return the readout buffer handle for the client
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        The readout buffer handle for the client
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCReadoutBuffer_HandleType, DEM_CODE)
Dem_Client_ClientAccess_GetReadoutBuffer(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestDTCRecordUpdateRequested
 *****************************************************************************/
/*!
 * \brief         Test if the DTC record update was requested
 *
 * \details       Test if the DTC record update was requested for the
 *                given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                The DTC record update was requested
 * \return        FALSE
 *                The DTC record update  was requested
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDTCRecordUpdateRequested(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestDTCRecordUpdateDisabled
 *****************************************************************************/
/*!
 * \brief         Test if the DTC record update is disabled
 *
 * \details       Test if the DTC record update is disabled for the
 *                given ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                The DTC record update is disabled
 * \return        FALSE
 *                The DTC record update is enabled
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDTCRecordUpdateDisabled(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestSnapshotRecordSelected
 *****************************************************************************/
/*!
 * \brief         Test if a snapshot record is selected
 *
 * \details       Test if a snapshot record is selected for the given 
 *                ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A snapshot record is selected
 * \return        FALSE
 *                No snapshot record is selected
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestSnapshotRecordSelected(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestExtendedRecordSelected
 *****************************************************************************/
/*!
 * \brief         Test if an extended data record is selected
 *
 * \details       Test if an extended data record is selected for the given
 *                ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                An extended data record is selected
 * \return        FALSE
 *                No extended data record is selected
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestExtendedRecordSelected(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_DisableDTCRecordUpdate
 *****************************************************************************/
/*!
 * \brief         Prepare an event entry for readout by the Dcm.
 *
 * \details       Disables the event memory update of a specific DTC (only one
 *                at one time).
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                Event memory update successfully disabled
 * \return        DEM_WRONG_DTC
 *                Selected DTC value in selected format does not exist.
 * \return        DEM_WRONG_DTCORIGIN
 *                Selected DTCOrigin does not exist.
 * \return        DEM_PENDING
 *                Disabling the DTC record update is currently in progress.
 *                The caller shall call this function again at a later moment.
 * \return        E_NOT_OK
 *                Required conditions for the respective API call are not
 *                fulfilled.
 *                The request is rejected due to invalid argument or state if
 *                Development Error Checks are active.
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_DisableDTCRecordUpdate(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_EnableDTCRecordUpdate
 *****************************************************************************/
/*!
 * \brief         Release a previously prepared event entry.
 *
 * \details       Releases the event entry previously locked by
 *                Dem_DisableDTCRecordUpdate.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                The event entry has been released.
 * \return        DEM_PENDING
 *                The asynchronous call Dem_DisableDTCRecordUpdate has not
 *                finished yet.
 * \return        E_NOT_OK
 *                There was no event entry to release.
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_EnableDTCRecordUpdate(
  CONST(uint8, AUTOMATIC) ClientId
  );
#endif

/* ****************************************************************************
 * Dem_Client_ClientAccess_FillDTCReadoutBufferData
 *****************************************************************************/
/*!
 * \brief         Copy the event entry contents into the ReadoutBuffer.
 *
 * \details       For all clients with ReadoutBuffer state QUEUED:
 *                copy the event entry contents into the ReadoutBuffer.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_Client_ClientAccess_FillDTCReadoutBufferData(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_ClearDTC
 *****************************************************************************/
/*!
 * \brief         Clear the selected DTC/DTC Group/All DTCs
 *
 * \details       Clear the selected DTC/DTC Group/All DTCs
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        Status of the clear request
 *
 * \pre           A DTC selection is available
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) 
Dem_Client_ClientAccess_ClearDTC(
  CONST(uint8, AUTOMATIC) ClientId
  );
  
/* ****************************************************************************
 * Dem_Client_ClientAccess_J1939ClearSingleDTC
 *****************************************************************************/
/*!
 * \brief         Clear the selected J1939 DTC
 *
 * \details       -
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     J1939DTC
 *                J1939 DTC to be cleared.
 * \param[in]     DTCOrigin
 *                The external identifier of the event memory.
 *                Must be primary memory.
 * \param[in]     DTCTypeFilter
 *                The type of the DTC to be cleared.
 *                DEM_J1939DTC_CLEAR_ACTIVE
 *                Clear the DTC if it is currently active.
 *                DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE
 *                Clear the DTC if it was previously active.
 * 
 * \return        DEM_CLEAR_FAILED
 *                Clear DTC request failed.
 * \return        DEM_PENDING
 *                The Clear DTC operation was started.
 * \return        DEM_CLEAR_BUSY
 *                Another Clear DTC operation is active.
 * \return        DEM_WRONG_DTC
 *                The J1939 DTC is not assigned to the primary memory or
 *                is currently set unavailable.
 * \return        DEM_WRONG_DTCTYPEFILTER
 *                The J1939 DTC does not match the given DTCTypeFilter.
 * \return        DEM_CLEAR_MEMORY_ERROR
 *                The clear operation has completed in RAM, but synchronization to NVRAM has failed.
 * \return        E_OK
 *                Clear DTC request completed successfully.
 *
 * \pre           DTCTypeFilter must be DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE or
 *                DEM_J1939DTC_CLEAR_ACTIVE
 * \pre           Valid node is assigned to the client.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different ClientId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) 
Dem_Client_ClientAccess_J1939ClearSingleDTC(
  uint8 ClientId,
  uint32 J1939DTC,
  Dem_DTCOriginType DTCOrigin,
  uint8 DTCTypeFilter
  );

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON) 
/* ****************************************************************************
 * Dem_Client_ClientAccess_J1939ClearDTC
 *****************************************************************************/
/*!
 * \brief         Clear the selected J1939 DTCs
 *
 * \details       Clear the selected J1939 DTCs
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     DTCTypeFilter
 *                The type of DTCs to be cleared
 * \param[in]     DTCOrigin
 *                The event memory of the requested DTC or group of DTC.
 *
 * \return        DEM_CLEAR_FAILED
 *                Clear DTC request failed.
 * \return        DEM_WRONG_DTCORIGIN
 *                DTCOrigin is not 'Primary Memory' in configuration variants
 *                with emission OBD support.
 * \return        DEM_PENDING
 *                The Clear DTC operation was started.
 * \return        DEM_CLEAR_BUSY
 *                Another Clear DTC operation is active.
 * \return        E_OK
 *                Clear DTC request completed successfully.
 *
 * \pre           Origin is valid.
 * \pre           Valid node is assigned to the client.
 * 
 * \config        DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) 
Dem_Client_ClientAccess_J1939ClearDTC(
  uint8 ClientId,
  uint8 DTCTypeFilter,
  Dem_DTCOriginType DTCOrigin
  );
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_CFG_SUPPORT_J1939 == STD_ON) )
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestFreezeFrameIteratorValid
 *****************************************************************************/
/*!
 * \brief         Test if the client has a valid freeze frame iterator.
 *
 * \details       Test if the client has a valid freeze frame iterator.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                Client has a valid freeze frame iterator
 * \return        FALSE
 *                Client doesn't have a valid freeze frame iterator
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON ||
 *                DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestFreezeFrameIteratorValid(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_TestFreezeFrameFilterSet
 *****************************************************************************/
/*!
 * \brief         Test if a freeze frame filter has been set.
 *
 * \details       Test if a freeze frame filter has been set for the given 
 *                ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                A freeze frame filter has been set
 * \return        FALSE
 *                No freeze frame filter has been set
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestFreezeFrameFilterSet(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 * Dem_Client_ClientAccess_StartFreezeFrameIterator
 *****************************************************************************/
/*!
 * \brief         Sets a freeze frame record filter with the given criteria.
 *
 * \details       Sets a freeze frame record filter with the given criteria.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     DTCFormat
 *                Defines the output-format of the requested DTC values.
 * \param[out]    NumberOfFilteredRecords
 *                Pointer to receive the number of stored snapshot records
 *
 * \return        E_OK
 *                The filter is accepted
 * \return        E_NOT_OK
 *                Wrong filter selected
 *
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 * \pre           -
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE  FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_StartFreezeFrameIterator(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(Dem_DTCFormatType, AUTOMATIC)  DTCFormat,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_DCM_DATA)  NumberOfFilteredRecords
);
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextFreezeFrameRecord
 *****************************************************************************/
/*!
 * \brief         Iterate the next snapshot record number matching the filter.
 *
 * \details       Iterates the next snapshot record number matching the filter,
 *                that was set by calling Dem_SetFreezeFrameRecordFilter.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    DTC
 *                Pointer to receive the DTC value in respective format of the
 *                filter returned by this function. If the return value of the
 *                function is other than E_OK this parameter does not contain
 *                valid data.
 * \param[out]    RecordNumber
 *                Pointer to receive the Freeze frame record number of the
 *                reported DTC (relative addressing). If the return value of
 *                the function is other than E_OK this parameter does not
 *                contain valid data.
 *
 * \return        TRUE
 *                Returned next filtered element.
 * \return        FALSE
 *                No further element (matching the filter criteria) found,
 *                iteration is complete.
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           The record filter has been initialized by
 *                Dem_SetFreezeFrameRecordFilter.
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_GetNextFreezeFrameRecord(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                              /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber                                                                       /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
);
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_GetDTCByOccurrenceTime
 *****************************************************************************/
/*!
 * \brief         Get a DTC based on occurrence time.
 *
 * \details       Returns a DTC based on occurrence time.
 *
 * \param[in]     DTCRequest
 *                The requested occurrence time of the DTC:
 *                - DEM_FIRST_FAILED_DTC: first failed DTC
 *                - DEM_MOST_RECENT_FAILED_DTC: most recent failed DTC
 *                - DEM_FIRST_DET_CONFIRMED_DTC: first detected confirmed DTC
 *                - DEM_MOST_REC_DET_CONFIRMED_DTC: most recently detected
 *                  confirmed DTC 
 * \param[out]    DTC
 *                Receives the DTC value in UDS format returned by the
 *                function. If the return value of the function is other than
 *                OK this parameter does not contain valid data. Validity of 
 *                this pointer has to be guaranteed by the caller.
 *
 * \return        TRUE
 *                Get of DTC was successful.
 * \return        FALSE
 *                The call was not successful, no DTC is available for the
 *                given occurrence time.
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           Arguments are checked and valid.
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_GetDTCByOccurrenceTime(
  CONST(Dem_DTCRequestType, AUTOMATIC)  DTCRequest,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_FilterNumberMemory
 *****************************************************************************/
/*!
 * \brief         Fetch number of DTCs matching the client specific filter settings.
 *
 * \details       Fetch number of DTCs matching the client specific filter settings.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        Number of DTCs matching the filter criteria
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClientAccess_FilterNumberMemory(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_EventChangeDtcSuppression
 *****************************************************************************/
/*!
 * \brief         Suppress event or release event suppression as requested by the client 
 *
 * \details       Suppress event or release event suppression as requested by the client 
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.

 * \param[in]     SuppressionStatus
 *                TRUE: Suppress the DTC
 *                FALSE: Lift suppression of the DTC
 *
 * \return        E_OK
 *                The suppression was changed successfully
 * \return        E_NOT_OK
 *                The request was rejected because no valid event was found for
 *                the given ClientId
 * \return        DEM_WRONG_DTC
 *                The selected DTC format is not allowed.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_EventChangeDtcSuppression(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(boolean, AUTOMATIC)  SuppressionStatus
  );
#endif

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_EventRetrieveDtcSuppression
 *****************************************************************************/
/*!
 * \brief         Suppress event or release event suppression as requested by the client 
 *
 * \details       Suppress event or release event suppression as requested by the client 
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.

 * \param[out]    SuppressionStatus
 *                Pointer to receive the suppression state of the DTC.
 *
 * \return        E_OK
 *                The suppression state was retrieved successfully
 * \return        E_NOT_OK
 *                The request was rejected because no valid event was found for
 *                the given ClientId.
 * \return        DEM_WRONG_DTC
 *                The selected DTC format is not allowed.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_EventRetrieveDtcSuppression(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  SuppressionStatus
  );
#endif

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestNodeValid
 *****************************************************************************/
/*!
 * \brief         Test if the client has a valid node.
 *
 * \details       Test if the client has a valid node.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                Client has a valid node
 * \return        FALSE
 *                Client doesn't have a valid node
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestNodeValid(
  CONST(uint8, AUTOMATIC)  ClientId
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNodeIndex
 *****************************************************************************/
/*!
 * \brief         Return the node index in Dem_Cfg_J1939NodeId for the client
 *
 * \details       Return the node index in Dem_Cfg_J1939NodeId for the client
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        The node index in Dem_Cfg_J1939NodeId
 *
 * \pre           Function to be invoked only when J1939 is enabled.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Client_ClientAccess_GetNodeIndex(
  CONST(uint8, AUTOMATIC)  ClientId
  );

#if (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_SetFirstDTCWithLampStatus
 *****************************************************************************/
/*!
 * \brief         Initializes a 'First DTC with lamp status' filter for the 
 *                given client
 *
 * \details       Initializes a 'First DTC with lamp status' filter for the 
 *                given client
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Client_ClientAccess_SetFirstDTCWithLampStatus(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

#if (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextDTCwithLampStatus
 *****************************************************************************/
/*!
 * \brief         Filters the next J1939 DTC with lamp status.
 *
 * \details       Filters the next J1939 DTC with lamp status.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    LampStatus
 *                Pointer to the LampStatus buffer.
 * \param[out]    J1939DTC
 *                Pointer to the J1939DTC .
 * \param[out]    OccurrenceCounter
 *                Pointer to the OccurrenceCounter buffer.
 *
 * \return        E_OK
 *                Matching DTC with required attributes returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No matching DTC returned.
 *
 * \pre           J1939 iteration has been started by Dem_J1939DcmFirstDTCwithLampStatus
 * \config        DEM_CFG_SUPPORT_J1939_DM31 == STD_ON || DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextDTCwithLampStatus(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_J1939DCM_DATA)  LampStatus,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter
  );
#endif

/* ****************************************************************************
 * Dem_Client_ClientAccess_SetJ1939DTCFilter
 *****************************************************************************/
/*!
 * \brief         Initializes a J1939 DTC filter with the given criteria.
 *
 * \details       For the given ClientId set up a J1939 DTC filter with the 
 *                given criteria.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     DTCStatusFilter
 *                The filter type defining the DTCs to be filtered.
 * \param[in]     DTCKind
 *                DTC kind to filter.
 * \param[in]     DTCOrigin
 *                The external identifier of the event memory.
 * \param[out]    LampStatus
 *                Pointer to receive the Lamp status for the node assigned to 
 *                the client.
 *
 * \return        E_OK
 *                Filter was accepted
 *                E_NOT_OK
 *                Filter was not accepted or no filter available for client
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetJ1939DTCFilter(
  uint8 ClientId,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  Dem_DTCKindType DTCKind,
  Dem_DTCOriginType DTCOrigin,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_J1939DCM_DATA)  LampStatus
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_SetJ1939RatioFilter
 *****************************************************************************/
/*!
 * \brief         Initializes a Ratio filter with the given criteria.
 *
 * \details       For the given ClientId set up a Ratio filter with the
 *                given criteria.
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
 *                Filter was accepted
 *                E_NOT_OK
 *                Filter was not accepted or no filter available for client
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetJ1939RatioFilter(
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) IgnitionCycleCounter,
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) OBDMonitoringConditionsEncountered,
  uint8  ClientId
);

#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNumberOfFilteredJ1939DTC
 *****************************************************************************/
/*!
 * \brief         Fetch number of DTCs matching the client specific J1939 DTC 
 *                filter settings.
 *
 * \details       Fetch number of DTCs matching the client specific J1939 DTC 
 *                filter settings.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        Number of DTCs matching the J1939 DTC filter criteria
 *
 * \pre           J1939 iteration has been started by Dem_J1939DcmSetDTCFilter
 * \config        DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClientAccess_GetNumberOfFilteredJ1939DTC(
  CONST(uint8, AUTOMATIC)  ClientId
  );
#endif

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextFilteredJ1939DTC
 *****************************************************************************/
/*!
 * \brief         Get the next matching DTC
 *
 * \details       Get the next DTC matching the J1939 DTC filter criteria
 *                set with Dem_Client_ClientAccess_SetJ1939DTCFilter
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    J1939DTC
 *                Pointer to the J1939DTC .
 * \param[out]    OccurrenceCounter
 *                Pointer to the OccurrenceCounter buffer.
 *
 * \return        E_OK
 *                Matching DTC with required attributes returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No matching DTC returned.
 * \return        DEM_PENDING
 *                Filtering is currently in progress.
 *
 * \config        DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 * \pre           J1939 iteration has been started by Dem_J1939DcmSetDTCFilter
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextFilteredJ1939DTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextFilteredJ1939Ratio
 *****************************************************************************/
/*!
 * \brief         Get the next filtered Ratio.
 *
 * \details       Get the SPN, numerator and denominator of the next
 *                filtered Ratio.
 *
 * \param[out]    SPN
 *                Pointer to receive the SPN.
 * \param[out]    Numerator
 *                Pointer to receive the numerator.
 * \param[out]    Denominator
 *                Pointer to receive the denominator.
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        E_OK
 *                Matching DTC with required attributes returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No matching DTC returned.
 *
 * \pre           The filter has to be initialized by
 *                Dem_J1939DcmSetRatioFilter().
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextFilteredJ1939Ratio(
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA) SPN,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) Numerator,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) Denominator,
  uint8 ClientId
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_ReadDiagnosticReadiness1
 *****************************************************************************/
/*!
 * \brief         Get the DiagnosticReadiness1 for the given client
 *
 * \details       Get the DiagnosticReadiness1 for the given client
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    DiagnosticReadiness
 *                Pointer to the DiagnosticReadiness1 value.
 *
 * \return        E_OK
 *                DiagnosticReadiness1 was written to output value.
 * \return        E_NOT_OK
 *                DiagnosticReadiness1 couldn't be read.
 *
 * \pre           J1939 DiagnosticReadiness1 calculation is supported
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_ReadDiagnosticReadiness1(
  uint8  ClientId,
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, DEM_J1939DCM_DATA)  DiagnosticReadiness
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_ReadDiagnosticReadiness2
 *****************************************************************************/
/*!
 * \brief         Gets the diagnostic readiness 2 for the given client
 *
 * \details       Gets the diagnostic readiness 2 for the given client
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    DiagnosticReadiness2
 *                Pointer to the diagnostic readiness value.
 *
 * \return        E_OK
 *                Diagnostic readiness was written to output value.
 * \return        E_NOT_OK
 *                Diagnostic readiness couldn't be read, or DM21 is disabled,
 *                or J1939 is disabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_ReadDiagnosticReadiness2(
  uint8  ClientId,
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_J1939DCM_DATA)  DiagnosticReadiness2
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_ReadDiagnosticReadiness3
 *****************************************************************************/
/*!
 * \brief         Gets the diagnostic readiness 3 for the given client
 *
 * \details       Gets the diagnostic readiness 3 for the given client
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    DiagnosticReadiness3
 *                Pointer to the diagnostic readiness value.
 *
 * \return        E_OK
 *                Diagnostic readiness was written to output value.
 * \return        E_NOT_OK
 *                Diagnostic readiness couldn't be read, or DM26 is disabled,
 *                or J1939 is disabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_ReadDiagnosticReadiness3(
  uint8  ClientId,
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness3Type, AUTOMATIC, DEM_J1939DCM_DATA) DiagnosticReadiness3
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_SetJ1939FreezeFrameIterator
 *****************************************************************************/
/*!
 * \brief         Sets a J1939 freeze frame record filter with the given 
 *                criteria.
 *
 * \details       Sets a J1939 freeze frame record filter with the given 
 *                criteria.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     FreezeFrameKind
 *                Freeze frame kind
 *
 * \return        E_OK
 *                The filter is accepted
 * \return        E_NOT_OK
 *                Wrong filter selected
 *
 * \config        -
 *
 * \pre           -
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetJ1939FreezeFrameIterator(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(Dem_J1939DcmSetFreezeFrameFilterType, AUTOMATIC) FreezeFrameKind
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextJ1939FreezeFrame
 *****************************************************************************/
/*!
 * \brief         Get the next matching J1939 freeze frame record.
 *
 * \details       Get the next J1939 freeze frame record matching the filter
 *                set with Dem_Client_ClientAccess_SetJ1939FreezeFrameIterator
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
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
 *                Matching J1939 freeze frame with required attributes returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No matching J1939 freeze frame returned.
 * \return        DEM_PENDING
 *                The operation was started and is currently processed.
 * \return        DEM_BUFFER_TOO_SMALL
 *                The provided buffer is not large enough.
 *
 * \config        -
 * \pre           J1939 iteration has been started by Dem_J1939DcmSetFreezeFrameFilter
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextJ1939FreezeFrame(
  uint8  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  DestBuffer,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  BufSize 
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextSPNInFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Gets the next SPN in the ExpandedFreezeFrame.
 *
 * \details       Gets the next SPN in the ExpandedFreezeFrame for DM24.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    SPN
 *                Pointer to the next SPN in the ExpandedFreezeFrame.
 * \param[out]    SPNDataLength
 *                Pointer to the data length of the SPN.
 *
 * \return        E_OK
 *                SPN of matching J1939 freeze frame with required attributes returned.
 * \return        DEM_NO_SUCH_ELEMENT
 *                No J1939 freeze frame SPN returned.
 *
 * \pre           J1939 iteration has been started by Dem_J1939DcmSetFreezeFrameFilter
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   FALSE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextSPNInFreezeFrame(
  uint8  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA) SPN,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  SPNDataLength
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestExtendedDataFilterValid
 *****************************************************************************/
/*!
 * \brief         Test if the client has a valid extended data iterator.
 *
 * \details       Test if the client has a valid extended data iterator.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                Client has a valid extended data iterator
 * \return        FALSE
 *                Client doesn't have a valid extended data iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestExtendedDataFilterValid(
  uint8 ClientId
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_TestExtendedDataFilterSet
 *****************************************************************************/
/*!
 * \brief         Test if an extended data filter has been set.
 *
 * \details       Test if an extended data filter has been set for the given
 *                ClientId.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 *
 * \return        TRUE
 *                An extended data filter has been set
 * \return        FALSE
 *                No extended data filter has been set
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestExtendedDataFilterSet(
  uint8 ClientId
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_SetExtendedDataRecordFilter
 *****************************************************************************/
/*!
 * \brief         Sets an extended data record filter with the given criteria.
 *
 * \details       Sets an extended data record filter with the given criteria.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     RecordNumber
 *                Requested record number.
 *
 * \return        E_OK
 *                The filter is accepted
 * \return        E_NOT_OK
 *                Wrong filter parameter or service 19 subfunction 16 feature is disabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetExtendedDataRecordFilter(
  uint8 ClientId,
  uint8 RecordNumber
);


/* ****************************************************************************
 * Dem_Client_ClientAccess_GetSizeOfFilteredExtendedDataRecords
 *****************************************************************************/
/*!
 * \brief         Provides number and size of the stored ERecs.
 *
 * \details       Provides number and size of the stored Extended Data Records.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    NumberOfFilteredRecords
 *                Pointer to receive the number of filtered ERecs
 * \param[out]    SizeOfRecords
 *                Pointer to receive the size of the filtered records
 *
 * \return        E_OK
 *                The filter is accepted
 * \return        E_NOT_OK
 *                Service 0x19 0x16 is not supported
 *                Invalid ClientId.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetSizeOfFilteredExtendedDataRecords(
  uint8 ClientId,
  P2VAR(uint16, AUTOMATIC, DEM_DCM_DATA)  NumberOfFilteredRecords,
  P2VAR(uint16, AUTOMATIC, DEM_DCM_DATA)  SizeOfRecords
);


/* ****************************************************************************
 * Dem_Client_ClientAccess_GetNextFilteredERecData
 *****************************************************************************/
/*!
 * \brief         Retrieves the next stored ERec.
 *
 * \details       Retrieves the next stored ERec.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[out]    DTC
 *                DTC identifier in the requested format
 * \param[out]    DTCStatus
 *                Current UDS status of the DTC
 * \param[out]    DestBuffer
 *                Destination buffer, in which the ERec data is written
 * \param[in,out] BufSize
 *                In: Size of Buffer
 *                Out: Number of written bytes
 *
 * \return        E_OK
 *                The filter is accepted
 * \return        DEM_PENDING
 *                Concurrent write is in progress. Retry the operation
 * \return        DEM_NO_SUCH_ELEMENT
 *                No further extended data records available
 * \return        DEM_BUFFER_TOO_SMALL
 *                Provided DestBuffer size is to small to write the extended data record into
 * \return        E_NOT_OK
 *                Service 0x19 0x16 is not supported.
 *                Invalid ClientId.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextFilteredERecData(
  uint8 ClientId,
  P2VAR(uint32, AUTOMATIC, DEM_DCM_DATA) DTC,
  P2VAR(uint8,  AUTOMATIC, DEM_DCM_DATA) DTCStatus,
  P2VAR(uint8,  AUTOMATIC, DEM_DCM_DATA) DestBuffer,
  P2VAR(uint16, AUTOMATIC, DEM_DCM_DATA) BufSize
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_SetDtcFilterByReadinessGroup
 *****************************************************************************/
/*!
 * \brief         Sets an DTC filter to filter by readiness group.
 *
 * \details       Sets an DTC filter to filter by readiness group.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     DTCFormat
 *                The requested DTC format
 * \param[in]     ReadinessGroup
 *                OBD readiness group.
 *
 * \return        E_OK
 *                The filter is accepted
 * \return        E_NOT_OK
 *                Wrong filter parameter or service 19 subfunction 56 feature is disabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetDtcFilterByReadinessGroup(
  uint8 ClientId,
  Dem_DTCFormatType DTCFormat,
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_SetDTCFilterByExtendedDataRecordNumber
 *****************************************************************************/
/*!
 * \brief         Sets an DTC filter to filter by extended data record number.
 *
 * \details       Sets an DTC filter to filter by extended data record number.
 *
 * \param[in]     ClientId
 *                Unique client id, assigned to the instance of the calling
 *                module.
 * \param[in]     DTCFormat
 *                The requested DTC format
 * \param[in]     ExtendedDataRecordNumber
 *                Number of the extended data record
 *
 * \return        E_OK
 *                The filter is accepted
 * \return        E_NOT_OK
 *                Wrong filter parameter or service 19 subfunction 1A feature is disabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ClientIDs
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetDTCFilterByExtendedDataRecordNumber(
  uint8 ClientId,
  Dem_DTCFormatType DTCFormat,
  uint8 ExtendedDataRecordNumber
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_IsEventAssignedToNode
 *****************************************************************************/
/*!
 * \brief         Tests whether J1939 node is configured for event
 *
 * \details       Tests whether J1939 node is configured for event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     NodeIndex
 *                The node Index.
 *
 * \return        TRUE: Node is configured for event
 *                FALSE: Event is not assigned to this node
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsEventAssignedToNode(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  NodeIndex
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_GetEventJ1939LampStatus
 *****************************************************************************/
/*!
 * \brief         Calculate the event specific J1939 lamp status.
 *
 * \details       Calculate the event specific J1939 lamp status if the event 
 *                is configured for the requested NodeId.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     NodeIndex
 *                The node Index.
 * \param[out]    FilterResult
 *                Pointer to receive the next matching event.
 *                FilterResult->LampStatus : 
 *                              The lamp status of the matching event.
 *                Ignore other member of FilterResult
 *
 * \return        E_OK
 *                Event is configured for the requested node identifier.
 *                DEM_NO_SUCH_ELEMENT
 *                Event is not configured for the requested node identifier.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetEventJ1939LampStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  NodeIndex,
  CONSTP2VAR(Dem_FilterData_ResultType, AUTOMATIC, AUTOMATIC)  FilterResult
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_IsJ1939FreezeFrameSupported
 *****************************************************************************/
/*!
 * \brief         Tests if J1939 freeze frame is supported.
 *
 * \details       Tests if J1939 freeze frame is supported.
 *
 * \return        TRUE
 *                J1939 Freeze frame is supported.
 * \return        FALSE
 *                J1939 Freeze frame is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939FreezeFrameSupported(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_IsJ1939ExpandedFreezeFrameSupported
 *****************************************************************************/
/*!
 * \brief         Tests if J1939 expanded freeze frame is supported.
 *
 * \details       Tests if J1939 expanded freeze frame is supported.
 *
 * \return        TRUE
 *                J1939 Expanded Freeze frame is supported.
 * \return        FALSE
 *                J1939 Expanded Freeze frame is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939ExpandedFreezeFrameSupported(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClientAccess_IsJ1939Readiness3Supported
 *****************************************************************************/
/*!
 * \brief         Tests if J1939 Readiness 3 calculation is supported.
 *
 * \details       Tests if J1939 Readiness 3 calculation is supported.
 *
 * \return        TRUE
 *                J1939 Readiness 3 calculation is supported.
 * \return        FALSE
 *                J1939 Readiness 3 calculation is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939Readiness3Supported(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_CLIENTACCESSIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ClientAccessIF_Interface.h
 *********************************************************************************************************************/
