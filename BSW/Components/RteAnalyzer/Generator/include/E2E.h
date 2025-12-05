/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E.h
 *        \brief  E2E header file
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/


#if !defined (E2E_H)
# define E2E_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "E2E_SM.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define E2E_VENDOR_ID                     (30u)
# define E2E_MODULE_ID                     (207u)

# define E2E_AR_RELEASE_MAJOR_VERSION      (4u)
# define E2E_AR_RELEASE_MINOR_VERSION      (2u)
# define E2E_AR_RELEASE_REVISION_VERSION   (2u)

# define E2E_SW_MAJOR_VERSION              (2u)
# define E2E_SW_MINOR_VERSION              (3u)
# define E2E_SW_PATCH_VERSION              (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

typedef P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) E2E_BytePtrType;

# define E2E_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * \brief       Retrieves the version information of the E2E module.
 * \details     Stores version information, i.e. Module Id, Vendor Id, Vendor specific version numbers to structure pointed by VersionInfo
 * \param[in]   VersionInfo
 * \pre         -
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 * \trace CREQ-1090
 */
FUNC(void, E2E_CODE) E2E_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, E2E_APPL_DATA) VersionInfo);


# define E2E_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* E2E_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E.h
 *********************************************************************************************************************/
