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
 *         \file  PduR_IFQ.h
 *         \unit  IFQ
 *        \brief  Pdu Router Interface FIFO Queue header file
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
#if !defined (PDUR_IFQ_H)
# define PDUR_IFQ_H

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

# if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_IFQ_Init
**********************************************************************************************************************/
/*! \brief       This function initializes the interface fifo queue administration values.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 ********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_IFQ_Init(PduR_MemIdxType memIdx);
# endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_PutFifo
 *********************************************************************************************************************/
/*! \brief       This function writes the Pdu to the FIFO queue.
 *  \details     The Pdus will be written in first-in-first-out order.
 *  \param[in]   interfaceFifoQueueRomIdx  Valid InterfaceFifoQueueRom table index. Internal handle of a PduRInterfaceFifoQueue.
 *  \param[in]   memIdx                    memory section index
 *  \param[in]   pduLength                 Length of the Pdu.
 *  \param[in]   sduDataPtr                Pointer to the data of the Pdu.
 *  \return      E_OK                      Write to the queue was successful.
 *               E_NOT_OK                  Write to the queue was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires interfaceFifoQueueRomIdx < PduR_GetSizeOfInterfaceFifoQueueRom(memIdx);
 *    requires sduDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_IFQ_PutFifo(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetNextElement
 *********************************************************************************************************************/
/*! \brief       This function gets the next Pdu from the FIFO queue.
 *  \details     The Pdus will be read in first-in-first-out order. This will not remove the Pdu from the queue.
 *  \param[in]   interfaceFifoQueueRomIdx  Valid InterfaceFifoQueueRom table index. Internal handle of a PduRInterfaceFifoQueue.
 *  \param[in]   memIdx                    memory section index
 *  \param[out]  rmDestRomIdx              read RmDestRom table index. Internal handle of a PduRDestPdu. This PduRDestPdu was stored in the queue.
 *  \param[out]  info                      read data from the queue. A pointer to the data and its length will be written to this value.
 *  \return      E_OK                      next Pdu was read successfully.
 *               E_NOT_OK                  no Pdu could be found. The queue is empty.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires interfaceFifoQueueRomIdx < PduR_GetSizeOfInterfaceFifoQueueRom(memIdx);
 *    requires rmDestRomIdx != NULL_PTR;
 *    requires info != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_IFQ_GetNextElement(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_RemoveElement
 *********************************************************************************************************************/
/*! \brief       Removes the oldest Pdu from the queue.
 *  \details     -
 *  \param[in]   interfaceFifoQueueRomIdx  Valid InterfaceFifoQueueRom table index. Internal handle of a PduRInterfaceFifoQueue.
 *  \param[in]   memIdx                    memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires interfaceFifoQueueRomIdx < PduR_GetSizeOfInterfaceFifoQueueRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_IFQ_RemoveElement(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_FlushQueue
 *********************************************************************************************************************/
/*! \brief       Flushes and resets the queue.
 *  \details     All Pdus will be removed from the queue. It will be empty afterwards.
 *  \param[in]   interfaceFifoQueueRomIdx  Valid InterfaceFifoQueueRom table index. Internal handle of a PduRInterfaceFifoQueue.
 *  \param[in]   memIdx                    memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires interfaceFifoQueueRomIdx < PduR_GetSizeOfInterfaceFifoQueueRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_IFQ_FlushQueue(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetFillLevel
 *********************************************************************************************************************/
/*! \brief       Gets the fill level of the queue.
 *  \details     -
 *  \param[in]   interfaceFifoQueueRomIdx  Valid InterfaceFifoQueueRom table index. Internal handle of a PduRInterfaceFifoQueue.
 *  \param[in]   memIdx                    memory section index
 *  \return      the fill level
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires interfaceFifoQueueRomIdx < PduR_GetSizeOfInterfaceFifoQueueRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint16, PDUR_CODE) PduR_IFQ_GetFillLevel(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_IFQ_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_IFQ.h
 *********************************************************************************************************************/
