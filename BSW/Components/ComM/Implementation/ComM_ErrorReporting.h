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
/*!         \file     ComM_ErrorReporting.h
 *          \unit     ErrorReporting
 *          \brief    Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the ErrorReporting unit.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#ifndef COMM_ERRORREPORTING_H
# define COMM_ERRORREPORTING_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Private_Cfg.h"
# if (COMM_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif
# if (COMM_USE_ECUM_BSW_ERROR_HOOK == STD_ON || COMM_USE_INIT_POINTER == STD_ON)
#  include "EcuM_Error.h"
# endif

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 * ComM_ErrorReporting_ProcessDetErrorReport()
 *********************************************************************************************************************/
/*! \brief       Reports development errors to Det
 *  \details     -
 *  \param[in]   serviceId                  the service ID for which the error was detected
 *  \param[in]   errorId                    The error Id code
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_ErrorReporting_ProcessDetErrorReport( uint8 serviceId, uint8 errorId );

/**********************************************************************************************************************
 * ComM_ErrorReporting_ProcessDetErrorReport()
 *********************************************************************************************************************/
/*! \brief       Reports development errors to Det
 *  \details     -
 *  \param[in]   errorId                    The error Id code
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_ErrorReporting_ProcessBswHookError( uint8 errorId );

# define COMM_STOP_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_ERRORREPORTING_H */

/**********************************************************************************************************************
  END OF FILE: ComM_ErrorReporting.h
 **********************************************************************************************************************/
