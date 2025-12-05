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
/*!         \file    ComM_NvM.h
 *          \unit    NvM
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the NvM unit
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

#ifndef COMM_NVM_H
# define COMM_NVM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Private_Cfg.h"

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 ComM_NvM_GetErrorStatus()
 *********************************************************************************************************************/
/*! \brief       Retrieves the current NvM error status
 *  \details     -
 *  \param[in]   isNvMDataRestored      Valid pointer where the request result shall be stored
 *  \return      E_OK                   Query for NvM error status was successful
 *               E_NOT_OK               Query for NvM error status was not successful
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( Std_ReturnType, COMM_CODE ) ComM_NvM_GetErrorStatus( P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) isNvMDataRestored );

/**********************************************************************************************************************
 ComM_NvM_StoreRequest()
 *********************************************************************************************************************/
/*! \brief       Trigger storage of non-volatile values.
 *  \details     -
 *  \return      E_OK                   NvM indicated RAM status set
 *               E_NOT_OK               NvM indicated RAM status not set
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC( Std_ReturnType, COMM_CODE ) ComM_NvM_StoreRequest( void );


# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

#endif  /* COMM_NVM_H */
