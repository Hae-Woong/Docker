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
 *         \file  PduR_RmIf_DeferredProcessing.h
 *         \unit  RmIf_DeferredProcessing
 *        \brief  Pdu Router Routing Manager Interface Deferred Processing header file
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
#if !defined (PDUR_RMIF_DEFERREDPROCESSING_H)
# define PDUR_RMIF_DEFERREDPROCESSING_H

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

# if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_Init
**********************************************************************************************************************/
/*! \brief       Initialize this unit.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_DeferredProcessing_Init(PduR_MemIdxType memIdx);
# endif

# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_MainFunction
 *********************************************************************************************************************/
/*!
 * \brief        Process deferred If Tx Pdus in context of cyclic MainFunction
 * \details      -
 * \param[in]    rmGDestRomIdx    Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 * \pre         -
 * \context      TASK
 * \reentrant    TRUE
 * \synchronous  TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
**********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_DeferredProcessing_MainFunction(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_Transmit
**********************************************************************************************************************/
/*! \brief       This function transmits a Pdu either immediately or in the context of the next MainFunction, depending on the configuration.
 *  \details     -
 *  \param[in]   rmDestRomIdx      Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   apiId             Error ID which shall be reported in case of an unsuccessful transmit.
 *  \return      Std_ReturnType    E_OK:     The Pdu has been transmitted successfully. Always E_OK for deferred Pdus.
 *                                 E_NOT_OK: The Pdu has not been transmitted successfully.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));
 *  \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_DeferredProcessing_Transmit(PduR_RmDestRomIterType rmDestRomIdx, uint8 apiId);
# endif

# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_IsDeferredTransmissionTriggered
**********************************************************************************************************************/
/*! \brief       Will a deferred transmission be triggered in the next MainFunction call.
 *  \details     -
 *  \param[in]   rmGDestRomIdx     Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \return      TRUE              A transmission is triggered in the next MainFunction call.
 *               FALSE             No transmission is triggered in the next MainFunction call.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
**********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmIf_DeferredProcessing_IsDeferredTransmissionTriggered(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMIF_DEFERREDPROCESSING_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmIf_DeferredProcessing.h
 *********************************************************************************************************************/
