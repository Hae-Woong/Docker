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
/*!         \file    ComM_Dcm.h
 *          \unit    DCM
 *          \brief   Communication Manager ASR4
 *
 *         \details  Callback API services of the Autosar Communication Manager for the DCM unit.
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

#ifndef COMM_DCM_H
# define COMM_DCM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define COMM_DCM_MAJOR_VERSION    (0x17u)
# define COMM_DCM_MINOR_VERSION    (0x00u)
# define COMM_DCM_PATCH_VERSION    (0x01u)

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
 *  CALLBACK FUNCTIONS PROTOTYPES (called by Dcm)
 *********************************************************************************************************************/

# define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 * ComM_DCM_ActiveDiagnostic()
 *********************************************************************************************************************/
/*! \brief       Indication of active diagnostic by the DCM.
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \pre         Nm Variant of the channel must not be PASSIVE
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       CREQ-102677
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_DCM_ActiveDiagnostic( NetworkHandleType Channel );

/**********************************************************************************************************************
 * ComM_DCM_InactiveDiagnostic()
 *********************************************************************************************************************/
/*! \brief       Indication of inactive diagnostic by the DCM.
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \pre         Nm Variant of the channel must not be PASSIVE
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       CREQ-102677
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_DCM_InactiveDiagnostic( NetworkHandleType Channel );

# if (COMM_DCM_INDICATION == STD_ON)
/**********************************************************************************************************************
 * ComM_GetDcmRequestStatus()
 *********************************************************************************************************************/
/*! \brief       Queries the status of DCM active diagnostic request of the corresponding channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  Status                  Valid pointer where the request status shall be stored
 *                                       TRUE: DCM indicated active diagnostic
 *                                       FALSE: otherwise
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *               COMM_E_UNINIT           ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-102677
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_GetDcmRequestStatus( NetworkHandleType Channel, P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) Status );
# endif

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_DCM_H */

/**********************************************************************************************************************
  END OF FILE: ComM_Dcm.h
**********************************************************************************************************************/
