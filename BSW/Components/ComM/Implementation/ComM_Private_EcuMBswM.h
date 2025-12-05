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
/*!         \file     ComM_Private_EcuMBswM.h
 *          \unit     EcuMBswM
 *          \brief    Communication Manager ASR4
 *
 *          \details  Private internal header of the Communication Manager for the EcuMBswM unit.
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

#ifndef COMM_PRIVATE_ECUMBSWM_H
# define COMM_PRIVATE_ECUMBSWM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_EcuMBswM.h"
# include "ComM_Private_Cfg.h"

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ComM_EcuM_ProcessWakeUpIndication()
 *********************************************************************************************************************/
/*! \brief       Process the indication of a passive wake-up by the EcuM.
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       CREQ-109578
 *********************************************************************************************************************/
FUNC( void, COMM_CODE ) ComM_EcuM_ProcessWakeUpIndication( NetworkHandleType Channel );

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * ComM_EcuM_ProcessPNCWakeUpIndication()
 *********************************************************************************************************************/
/*! \brief       Process the indication of a wake-up on the corresponding Partial Network Cluster.
 *  \details     -
 *  \param[in]   PNCid                  Valid PNC identifier
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_EcuM_ProcessPNCWakeUpIndication( PNCHandleType PNCid );
#endif

# define COMM_STOP_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_PRIVATE_ECUMBSWM_H */

/**********************************************************************************************************************
  END OF FILE: ComM_Private_EcuMBswM.h
 **********************************************************************************************************************/
