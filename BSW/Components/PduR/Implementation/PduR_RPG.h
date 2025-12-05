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
 *         \file  PduR_RPG.h
 *         \unit  RPG
 *        \brief  Pdu Router Routing Path Group header file
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
#if !defined (PDUR_RPG_H)
# define PDUR_RPG_H

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

/**********************************************************************************************************************
 * PduR_RPG_Init
 *********************************************************************************************************************/
/*! \brief       This function initializes the RPG unit.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RPG_Init(PduR_MemIdxType memIdx);

/**********************************************************************************************************************
 * PduR_RPG_EnableRouting
**********************************************************************************************************************/
/*! \brief       This function enables a routing path group.
 *  \details     -
 *  \param[in]   rpgRomIdx Valid RpgRom table index. Internal handle of a routing path group.
 *  \param[in]   memIdx    memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires rpgRomIdx < PduR_GetSizeOfRpgRom(memIdx);
 *  \endspec
 **********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RPG_EnableRouting(PduR_RpgRomIterType rpgRomIdx, PduR_MemIdxType memIdx);

/**********************************************************************************************************************
 * PduR_RPG_DisableRouting
**********************************************************************************************************************/
/*! \brief       This function disables a routing path group.
 *  \details     -
 *  \param[in]   rpgRomIdx Valid RpgRom table index. Internal handle of a routing path group.
 *  \param[in]   memIdx    memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires rpgRomIdx < PduR_GetSizeOfRpgRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RPG_DisableRouting(PduR_RpgRomIterType rpgRomIdx, PduR_MemIdxType memIdx);

/**********************************************************************************************************************
 * PduR_RPG_IsDestPduEnabled
**********************************************************************************************************************/
/*! \brief       This API checks if a destination Pdu is enabled.
 *  \details     The destination is either enabled if it is not assigned to any routing path group
 *               or if it is assigned to at least one RPG and at least one of these RPGs is enabled.
 *               If no RPGs are configured at all, this function will always return TRUE.
 *  \param[in]   rmDestRomIdx  Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \return      TRUE          The destination Pdu is enabled.
 *               FALSE         The destination Pdu is disabled.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RPG_IsDestPduEnabled(PduR_RmDestRomIterType rmDestRomIdx);

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RPG_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RPG.h
 *********************************************************************************************************************/
