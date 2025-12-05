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
 *         \file  PduR_McQ.h
 *         \unit  McQ
 *        \brief  Pdu Router Multicore Queue header file
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
#if !defined (PDUR_MCQ_H)
# define PDUR_MCQ_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* \trace SPEC-2020230, SPEC-38402 */

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

# if(PDUR_MCQBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_McQ_Init
**********************************************************************************************************************/
/*! \brief       This function initializes the multicore queue administration values.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_McQ_Init(PduR_MemIdxType memIdx);
# endif

# if (PDUR_MCQBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_McQ_WriteData
 *********************************************************************************************************************/
/*! \brief       Write Id and data into the multicore queue.
 *  \details     -
 *  \param[in]   mcQBufferRomIdx          Id of the queue to be used
 *  \param[in]   rmDestRomIdx             Index to the destination Pdu to be queued. Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   data                     pointer to the data which will be written to the queue.
 *  \param[in]   memIdx                   memory section index
 *  \return      E_OK                     Write was successful.
 *               E_NOT_OK                 Write was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_McQ_WriteData(PduR_McQBufferRomIdxOfSrcApplicationRomType mcQBufferRomIdx, PduR_RmDestRomEndIdxOfRmSrcRomType rmDestRomIdx,
                                                   CONSTP2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) data, PduR_MemIdxType memIdx);
# endif

# if (PDUR_MCQBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_McQ_WriteTxConfirmation
 *********************************************************************************************************************/
/*! \brief       Write TxConfirmation Id into the multicore queue.
 *  \details     -
 *  \param[in]   mcQBufferRomIdx   Id of the queue to be used
 *  \param[in]   rmDestRomIdx      Index to the destination Pdu to be queued. Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   memIdx            memory section index
 *  \return      E_OK              Write was successful.
 *               E_NOT_OK          Write was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_McQ_WriteTxConfirmation(PduR_McQBufferRomIdxOfSrcApplicationRomType mcQBufferRomIdx, PduR_RmDestRomEndIdxOfRmSrcRomType rmDestRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_MCQBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_McQ_MainFunction
 *********************************************************************************************************************/
/*!\brief        Read and process the DestPdus which are stored in the multicore queues belonging to this target application (destApplicationManagerRomIdx).
 * \details      -
 * \param[in]    destApplicationManagerRomIdx   internal DestApplicationManagerRom index.
 * \pre          -
 * \context      TASK
 * \reentrant    TRUE
 * \synchronous  TRUE
**********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_McQ_MainFunction(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx);
# endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_MCQ_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_McQ.h
 *********************************************************************************************************************/
