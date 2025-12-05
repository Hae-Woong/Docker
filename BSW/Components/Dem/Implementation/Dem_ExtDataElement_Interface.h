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
 *  \defgroup   Dem_ExtDataElement ExtDataElement
 *  \{
 *  \file       Dem_ExtDataElement_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     ExtDataElement
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EXTDATAELEMENT_INTERFACE_H)
#define DEM_EXTDATAELEMENT_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_ExtDataElement_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ExtDataElementIF_PublicProperties Public Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ExtDataElementIF_Public Public Methods
 * \{
 */
#if ( (DEM_CFG_SUPPORT_USER_ERECS == STD_ON) \
   || (DEM_CFG_SUPPORT_SRECS == STD_ON) \
   || (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) \
   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_InitCollectDidInfo
 *****************************************************************************/
/*!
 * \brief         Initializes CollectDidInfo context
 *
 * \details       Initializes CollectDidInfo context
 *
 * \param[in,out] CollectDidInfoPtr
 *                CollectDid context
 * \param[in]     DestinationBuffer
 *                Pointer to Destination Buffer context
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON  ||
 *                DEM_CFG_SUPPORT_SRECS == STD_ON  ||
 *                DEM_FEATURE_NEED_EMISSIONOBD == STD_ON  ||
 *                DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON ||
 *                DEM_FEATURE_NEED_TIME_SERIES == STD_ON ||
 *                DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ExtDataElement_InitCollectDidInfo(
  CONST(Dem_ExtDataElement_CollectDidInfoPtrType, AUTOMATIC)  CollectDidInfoPtr,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectDataElement
 *****************************************************************************/
/*!
 * \brief         Copies data element collected from application into a buffer.
 *
 * \details       Collects a data element from an application and copies it
 *                into the destination buffer.
 *
 * \param[in,out] CollectDidInfoPtr
 *                CollectDid context.
 * \param[in]     DataIndex
 *                Unique handle of the data element.
 *
 * \return        TRUE
 *                Data was copied.
 * \return        FALSE
 *                No data was copied.
 *
 *
 * \pre           -
 * \config        ( (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_ExtDataElement_CollectDataElement(
  CONST(Dem_ExtDataElement_CollectDidInfoPtrType, AUTOMATIC)  CollectDidInfoPtr,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex
  );
#endif

#if ( (DEM_CFG_SUPPORT_USER_ERECS == STD_ON) \
   || (DEM_CFG_SUPPORT_SRECS == STD_ON) \
   || (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) \
   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
/* ****************************************************************************
 * Dem_ExtDataElement_CollectStoredDid
 *****************************************************************************/
/*!
 * \brief         Collects the environment data to store for a data identifier.
 *
 * \details       Stores all data elements attached to a data identifier object
 *                into the destination buffer.
 *                Note that a data identifier in this context is not only a
 *                UDS DID, but can also be an extended data record or PID.
 *
 * \param[in,out] CollectDidInfoPtr
 *                CollectDid context.
 * \param[in]     DidIndex
 *                Unique handle of the DID.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON  ||
 *                DEM_CFG_SUPPORT_SRECS == STD_ON  ||
 *                DEM_FEATURE_NEED_EMISSIONOBD == STD_ON  ||
 *                DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON ||
 *                DEM_FEATURE_NEED_TIME_SERIES == STD_ON ||
 *                DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectStoredDid(
  CONST(Dem_ExtDataElement_CollectDidInfoPtrType, AUTOMATIC)  CollectDidInfoPtr,
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  );
#endif

#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 * Dem_ExtDataElement_GetCustomizedCurrentOdometer
 *****************************************************************************/
/*!
 * \brief         Fetches current odometer value for storage in StdEnvData
 *
 * \details       Fetches current odometer value for storage in StdEnvData
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 * \config        DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON ||
 *                DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_GetCustomizedCurrentOdometer(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

#if (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 * Dem_ExtDataElement_GetCustomizedOperatingTime
 *****************************************************************************/
/*!
 * \brief         Fetches the current operating time and status and stores
 *                them in extended StdEnvData
 *
 * \details       Fetches the current operating time and status and stores
 *                them in extended StdEnvData
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 * \config        DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_GetCustomizedOperatingTime(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 * Dem_ExtDataElement_GetCustomizedExternalTester
 *****************************************************************************/
/*!
 * \brief         Fetches the current tester present information for storage in
 *                StdEnvData
 *
 * \details       Fetches the current tester present information for storage in
 *                StdEnvData
 *
 * \param[out]    Buffer
 *                Pointer to the buffer to store the data
 *
 * \pre           -
 * \config        DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON ||
 *                DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_GetCustomizedExternalTester(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  Buffer
  );
#endif

/* ****************************************************************************
 * Dem_FaultMemory_ExtDataElement_CollectOBDRelevantUserData
 *****************************************************************************/
/*!
 * \brief         Requests required OBD data from application.
 *
 * \details       Retrieves required OBD data as CS data with a size of 4 bytes
 *                from the application and stores it into the passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 *
 * \return        E_OK
 *                The requested data was stored in DestinationBuffer.
 *                E_NOT_OK
 *                Request was rejected.
 *
 * \pre           Function can only be called when
 *                - Data element type DEM_CFG_DATA_TYPE_UINT8_N is used.
 *                - Data callback type DEM_CFG_DATA_FROM_CS_PORT is used.
 *                - The requested data element has a size of 4 bytes.
 *                - The requested data element is one of special elements
 *                  required by OBD (Odometer, EngineRuntime)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FaultMemory_ExtDataElement_CollectOBDRelevantUserData(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC) DestinationBuffer,
  Dem_Cfg_DataIndexType DataIndex
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EXTDATAELEMENT_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ExtDataElement_Interface.h
 *********************************************************************************************************************/
