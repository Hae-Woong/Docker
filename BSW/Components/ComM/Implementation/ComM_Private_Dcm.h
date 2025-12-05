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
/*!         \file     ComM_Private_Dcm.h
 *          \unit     DCM
 *          \brief    Communication Manager ASR4
 *
 *          \details  Private internal header of the Communication Manager for the DCM.
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

#ifndef COMM_PRIVATE_DCM_H
# define COMM_PRIVATE_DCM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Dcm.h"
# include "ComM_Private_Cfg.h"

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if (COMM_DCM_INDICATION == STD_ON)
/**********************************************************************************************************************
 * ComM_DCM_ProcessActiveDiagnostic()
 *********************************************************************************************************************/
/*! \brief       Process the indication of an active diagnostic by the DCM.
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \pre         Nm Variant of the channel must not be PASSIVE or LINSLAVE
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *********************************************************************************************************************/
extern FUNC( uint8, COMM_CODE ) ComM_DCM_ProcessActiveDiagnostic( NetworkHandleType Channel );

/**********************************************************************************************************************
 * ComM_DCM_ProcessInactiveDiagnostic()
 *********************************************************************************************************************/
/*! \brief       Process the indication of inactive diagnostic by the DCM.
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \pre         Nm Variant of the channel must not be PASSIVE
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *********************************************************************************************************************/
extern FUNC( uint8, COMM_CODE ) ComM_DCM_ProcessInactiveDiagnostic( NetworkHandleType Channel );

/**********************************************************************************************************************
 * ComM_DCM_GetActiveRequestStatus()
 *********************************************************************************************************************/
/*! \brief       Query of the status of DCM active diagnostic request.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  Status                  Valid pointer where the request status shall be stored
 *                                       TRUE: DCM indicated active diagnostic
 *                                       FALSE: otherwise
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_DCM_INDICATION == STD_ON)
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_DCM_GetActiveRequestStatus( NetworkHandleType Channel, P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) Status );
#endif

/**********************************************************************************************************************
 * ComM_DCM_NotificationHandling()
 *********************************************************************************************************************/
/*! \brief       ComM mode notification to Dcm
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[in]   ComMode                 Valid pointer where the BusSM mode is stored
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_DCM_NotificationHandling( NetworkHandleType Channel, P2CONST( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode );


# define COMM_STOP_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_PRIVATE_DCM_H */

/**********************************************************************************************************************
  END OF FILE: ComM_Private_Dcm.h
 **********************************************************************************************************************/
