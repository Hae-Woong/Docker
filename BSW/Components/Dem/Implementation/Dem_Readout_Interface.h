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
/*! \ingroup    Dem_Master | Dem_Satellite
 *  \defgroup   Dem_Readout Readout
 *  \addtogroup Dem_Readout
 *  \{
 *  \file       Dem_Readout_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Readout Unit used for calculating DataContext specific (PID, DM, DID) readiness
 *              information and writing it to the destination buffer in the correct respective format.
 * \entity      Readout
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_READOUT_INTERFACE_H_)
#define DEM_READOUT_INTERFACE_H_

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Readout_Types.h"
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Readout_Properties Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Readout_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Readout_ReadDataOfPID01
 *****************************************************************************/
/*!
 * \brief         Provides the monitoring status since DTCs cleared
 *
 * \details       Provides the monitoring status since DTCs cleared, formatted
 *                according to OBD-II
 *
 * \param[out]    PID01value
 *                Pointer to destination buffer, which is filled with PID 01 data.
 * \param[in]     BufferSize
 *                Size of destination buffer
 *
 * \return        E_OK
 *                Read PID01 successful
 * \return        E_NOT_OK
 *                OBD not supported
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PID01 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfPID01(
  CONSTP2VAR(uint8, AUTOMATIC, DEM_SHARED_DATA)  PID01value,
  uint8 BufferSize
);


/* ****************************************************************************
* Dem_Readiness_Readout_ReadDataOfDM05
*****************************************************************************/
/*!
* \brief         Provides the diagnostic readiness
*
* \details       Provides the diagnostic readiness
*
* \param[out]    DestinationBuffer
*                Destination buffer containing the diagnostic readiness values

* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfDM05(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);


/* ****************************************************************************
 * Dem_Readiness_Readout_ReadDataOfDIDF501
 *****************************************************************************/
/*!
 * \brief         Provides the monitoring status since DTCs cleared
 *
 * \details       Provides the monitoring status since DTCs cleared, formatted
 *                according to OBD-II
 *
 * \param[out]    DIDF501value
 *                Pointer to destination buffer, which is filled with PID F501 data.
 *
 * \param[in]     BufferSize
 *                Size of destination buffer
 * \return        E_OK
 *                Read PID F501 successful
 * \return        E_NOT_OK
 *                OBD not supported
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfDIDF501(
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  DIDF501value,
  uint8 BufferSize
);


/* ****************************************************************************
 * Dem_Readiness_Readout_ReadDataOfDM26
 *****************************************************************************/
/*!
 * \brief         Provides the readiness for diagnostic readiness 3
 *
 * \details       Provides the readiness for diagnostic readiness 3
 *
 * \param[out]    DestinationBuffer
 *                Destination buffer containing the readiness values for
 *                diagnostic readiness 3 bytes 4 to 8
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfDM26(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC) DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Readout_ReadDataOfPID41
 *****************************************************************************/
/*!
 * \brief         Provides the monitoring status this driving cycle
 *
 * \details       Provides the monitoring status this driving cycle, formatted
 *                according to OBD-II
 *
 * \param[out]    PID41value
 *                Pointer to destination buffer, which is filled with PID 41 data.
 *
 * \param[in]     BufferSize
 *                Size of destination buffer
 * \return        E_OK
 *                Read PID41 successful
 * \return        E_NOT_OK
 *                OBD not supported
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Readiness_Readout_ReadDataOfPID41(
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID41value,
  uint8 BufferSize
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_READOUT_INTERFACE_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Readout_Interface.h
 *********************************************************************************************************************/
