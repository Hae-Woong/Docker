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
 *         \file  PduR_SingleBuffer.h
 *         \unit  SingleBuffer
 *        \brief  Pdu Router Single Buffer header file
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
#if !defined (PDUR_SINGLEBUFFER_H)
# define PDUR_SINGLEBUFFER_H

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

# if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_SingleBuffer_Init
 *********************************************************************************************************************/
/*! \brief       This function initializes the single buffer with default values
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
FUNC(void, PDUR_CODE) PduR_SingleBuffer_Init(PduR_MemIdxType memIdx);
# endif

#if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_SingleBuffer_Put
 *********************************************************************************************************************/
/*! \brief       This function writes the Pdu to the single buffer.
 *  \details     This will always overwrite the previous Pdu data.
 *  \param[in]   singleBufferRomIdx   Valid SingleBufferRom table index. Internal handle of a PduRSingleBuffer.
 *  \param[in]   memIdx               memory section index
 *  \param[in]   pduLength            Length of the Pdu.
 *  \param[in]   sduDataPtr           Pointer to the data of the Pdu.
 *  \return      E_OK                 This function will always return E_OK. This function is always successful as the Pdu will always be overwritten.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires singleBufferRomIdx < PduR_GetSizeOfSingleBufferRom(memIdx);
 *    requires sduDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_SingleBuffer_Put(PduR_SingleBufferRomIterType singleBufferRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

#if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_SingleBuffer_Get
 *********************************************************************************************************************/
/*! \brief       This function gets the Pdu from the single buffer.
 *  \details     This function does not remove the Pdu from the buffer. The latest data can be read any time.
 *  \param[in]   singleBufferRomIdx  Valid SingleBufferRom table index. Internal handle of a PduRSingleBuffer.
 *  \param[in]   memIdx              memory section index
 *  \param[out]  rmDestRomIdx        read RmDestRom table index. Internal handle of a PduRDestPdu. This PduRDestPdu was stored in the single buffer.
 *  \param[out]  info                read data from the queue. A pointer to the data and its length will be written to this value.
 *  \return      E_OK                This function will always return E_OK. This function is always successful as there is always (default) data in the single buffer.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires singleBufferRomIdx < PduR_GetSizeOfSingleBufferRom(memIdx);
 *    requires rmDestRomIdx != NULL_PTR;
 *    requires info != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_SingleBuffer_Get(PduR_SingleBufferRomIterType singleBufferRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
#endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_SINGLEBUFFER_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_SingleBuffer.h
 *********************************************************************************************************************/
