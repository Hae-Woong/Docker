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
 *         \file  PduR_EvC.h
 *         \unit  EvC
 *        \brief  Pdu Router Event Cache header file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (PDUR_EVC_H)
# define PDUR_EVC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "PduR_Types.h"
# include "PduR_Cfg.h"
# include "PduR_Lcfg.h"

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
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define PDUR_START_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#if(PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_Init
**********************************************************************************************************************/
/*! \brief       This function initializes the deferred event cache.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_EvC_Init(PduR_MemIdxType memIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_WriteDeferredEventCache
 *********************************************************************************************************************/
/*! \brief       Write the handle ID to the corresponding event cache.
 *  \details     -
 *  \param[in]   rmGDestRomIdx            Valid RmGDestRom table index. Internal handle of a global PduRDestPdu.
 *  \param[in]   applicationContext       Application call context
 *  \return      E_OK                     Write to the event cache was successful.
 *               E_NOT_OK                 Write to the event cache was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_EvC_WriteDeferredEventCache(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext);
#endif

# if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_MainFunction
 *********************************************************************************************************************/
/*!
 * \brief        Process the deferred event caches for this application.
 * \details      This function is supposed to be called cyclically by a MainFunction.
 * \param[in]    destApplicationManagerRomIdx   internal DestApplicationManagerRom index
 * \pre          -
 * \context      TASK
 * \reentrant    TRUE
 * \synchronous  TRUE
**********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_EvC_MainFunction(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx);
# endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_EVC_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_EvC.h
 *********************************************************************************************************************/
