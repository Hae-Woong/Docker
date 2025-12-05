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
/*!         \file    ComM_User.h
 *          \unit    User
 *          \brief   Communication Manager ASR4
 *
 *          \details  Header of the Autosar Communication Manager for the User unit
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

#ifndef COMM_USER_H
# define COMM_USER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Private_Cfg.h"

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
 *  INTERNAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 * ComM_User_RequestComMode()
 *********************************************************************************************************************/
 /*! \brief       Requesting of a Communication Mode by a user.
 *  \details     -
 *  \param[in]   User                    Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[in]   ComMode                 The requested communication mode (COMM_FULL_COMMUNICATION or COMM_NO_COMMUNICATION)
 *  \param[in]   partitionIdx            Identification of the user partition index
 *  \return      E_OK                    Request is accepted
 *               COMM_E_MODE_LIMITATION  FULL_COM request is inhibited
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       SPEC-5440, SPEC-5434, SPEC-5358, SPEC-5112, SPEC-5082, SPEC-5099, SPEC-5094, SPEC-5089
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_User_RequestComMode( ComM_UserIterType User, ComM_ModeType ComMode, CONST( ComM_PCPartitionConfigIdxOfPartitionIdentifiersType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
 * ComM_User_GetMaxComMode()
 *********************************************************************************************************************/
/*! \brief       Process the request to query the maximum allowed communication mode of the corresponding user.
 *  \details     -
 *  \param[in]   User                   Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[out]  ComMode                Valid pointer where to store the maximal communication mode of the given user
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_User_GetMaxComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode );

/**********************************************************************************************************************
 * ComM_User_GetRequestedComMode()
 *********************************************************************************************************************/
/*! \brief       Process the request to query the requested communication mode of the corresponding user.
 *  \details     -
 *  \param[in]   User                   Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[in]   partitionIdx            Identification of the user partition index
 *  \param[out]  ComMode                Valid pointer where to store the requested communication mode of the given user
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, COMM_CODE ) ComM_User_GetRequestedComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode, CONST( ComM_PCPartitionConfigIdxOfPartitionIdentifiersType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
 * ComM_User_GetCurrentComMode()
 *********************************************************************************************************************/
/*! \brief       Process the request to query the current communication mode of the corresponding user.
 *  \details     -
 *  \param[in]   User                   Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[out]  ComMode                Valid pointer where to store the current communication mode of the given user
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               Request is not accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_User_GetCurrentComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode );

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * ComM_User_GetCurrentPNCComMode()
 *********************************************************************************************************************/
/*! \brief       Process the request to query the current communication mode of the corresponding PNC the user is assigned to.
 *  \details     -
 *  \param[in]   User                          Valid handle of the User, can be found in ComM_Lcfg.h
 *  \param[out]  ComMode                       Valid pointer where to store the current communication mode of the given user
 *  \return      E_OK                          Request is accepted
 *               E_MULTIPLE_PNC_ASSIGNED       Multiple PNCs are assigned to the user
 *               E_NO_PNC_ASSIGNED             No PNC is assigned to the affected user
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( Std_ReturnType, COMM_CODE ) ComM_User_GetCurrentPNCComMode( ComM_UserIterType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode );
#endif

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"


#endif  /* COMM_USER_H */

/**********************************************************************************************************************
  END OF FILE: ComM_User.h
**********************************************************************************************************************/
