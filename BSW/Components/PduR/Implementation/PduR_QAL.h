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
 *         \file  PduR_QAL.h
 *         \unit  QAL
 *        \brief  Pdu Router Queue Abstraction Layer header file
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
#if !defined (PDUR_QAL_H)
# define PDUR_QAL_H

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

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_Put
 *********************************************************************************************************************/
/*! \brief      Writes the Pdu to the queue. It calls the corresponding implementation specific function.
 *  \details    -
 *  \param[in]  rmDestRomIdx         Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]  rmGDestRomIdx        Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]  pduLength            Pdu length
 *  \param[in]  sduDataPtr           Pdu data
 *  \return     E_OK                 Pdu was written successfully.
 *              E_NOT_OK             Pdu was not written.
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *    requires sduDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_QAL_Put(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_RmDestRomIterType rmDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_Get
 *********************************************************************************************************************/
/*! \brief       Gets the next element from the queue if available. It calls the corresponding implementation specific function.
 *  \details     -
 *  \param[in]   rmGDestRomIdx       Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[out]  rmDestRomIdx        Read RmDestRom table index from the queue. Internal handle of a PduRDestPdu.
 *  \param[out]  info                Read data from the queue.
 *  \return      E_OK                A Pdu could be read and will be returned via the pointers.
 *               E_NOT_OK            No Pdu could be read.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *    requires info != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_QAL_Get(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_Remove
 *********************************************************************************************************************/
/*! \brief       Removes the oldest element from the queue. It calls the corresponding implementation specific function.
 *  \details     -
 *  \param[in]   rmGDestRomIdx       Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_QAL_Remove(PduR_RmGDestRomIterType rmGDestRomIdx);
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_GetFillLevel
 *********************************************************************************************************************/
/*! \brief       Gets the fill level of the queue. It calls the corresponding implementation specific function.
 *  \details     -
 *  \param[in]   rmGDestRomIdx       Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \return                          Fill level of the queue.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint16, PDUR_CODE) PduR_QAL_GetFillLevel(PduR_RmGDestRomIterType rmGDestRomIdx);
#endif

# if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_Flush
 *********************************************************************************************************************/
/*! \brief       Flushes the FIFO queue. It calls the corresponding implementation specific function.
 *  \details     -
 *  \param[in]   rmGDestRomIdx  Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_QAL_Flush(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_QAL_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_QAL.h
 *********************************************************************************************************************/
