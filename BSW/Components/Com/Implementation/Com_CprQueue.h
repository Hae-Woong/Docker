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
/*!        \file  Com_CprQueue.h
 *         \unit  CprQueue
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_CPRQUEUE_H)
# define COM_CPRQUEUE_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
# define COM_CPRQUEUE_HEADERVALUE_UNUSED        0xFFu
# define COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE    1u
# endif

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_CprQueue_Init
**********************************************************************************************************************/
/*! \brief       This function initializes the CrossPartitionRoutingQueue values.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Init(void);

/**********************************************************************************************************************
 * Com_CprQueue_UpdateReadIdx
 *********************************************************************************************************************/
/*!
 *  \brief       This function updates the readIdx.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex);

/**********************************************************************************************************************
 * Com_CprQueue_UpdateWriteIdx
 *********************************************************************************************************************/
/*!
 *  \brief       This function updates the writeIdx.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_UpdateWriteIdx(Com_CCprQueueIterType routingQueueIndex);

/**********************************************************************************************************************
 * Com_CprQueue_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*! \brief       This function resets the NumberOfBytesRead.
 *  \details     Called before a batch of read operations that should in sum not exceed a threshold of bytes read.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex);

/**********************************************************************************************************************
 * Com_CprQueue_GetWriteIdx
 *********************************************************************************************************************/
/*! \brief       Returns a valid write Idx if the data fits into the buffer
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[out]  writeIdx                 The returned writeIdx. Only valid for successful return value.
 *  \param[in]   payloadLengthWithHeader  length of the data to be put into the queue
 *  \return      E_OK                     Data fits into the buffer.
 *               E_NOT_OK                 Data does not fit into the buffer.
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *  \pre        -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetWriteIdx(Com_CCprQueueIterType routingQueueIndex,
                                                        CONSTP2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) writeIdx, PduLengthType payloadLengthWithHeader);

/**********************************************************************************************************************
 * Com_CprQueue_GetReadIdx
 *********************************************************************************************************************/
/*! \brief       Get a valid readIdx.
 *  \details -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[out]  readIdx                  the valid readIdx.
 *  \return      E_OK                     Some data is available to be read.
 *               E_NOT_OK                 No data is available to be read.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetReadIdx(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) readIdx);

/**********************************************************************************************************************
 * Com_CprQueue_ReadFromCprQueueDirectly
 *********************************************************************************************************************/
/*! \brief       Get the value in the queue at the position readIdx.
 *  \details     -
 *  \param[in]   readIdx                  the valid readIdx.
 *  \return      queueValue               the value in the queue at the position readIdx.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_CprQueue_ReadFromCprQueueDirectly(Com_CprQueueBufferIterType readIdx);

/**********************************************************************************************************************
 * Com_CprQueue_Write
 *********************************************************************************************************************/
/*! \brief       This function writes the parameter into the queue starting at the writeIdx position.
 *  \details     The intention of using a uint32 as index is that this function will be used by two different functionalities,
 *               which might not have the same type. Here, biggest type is used, to allow for an implicit upcast.
 *  \param[in]   writeIdx         Index to the position in the queue where the data shall be written
 *  \param[in]   index            Handle Id to be written into the queue
 *  \param[in]   data             pointer to the data to be written into the queue
 *  \param[in]   header           header to be written into the queue
 *  \param[in]   sizeOfIndexType  size of the index that is written into the queue
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Write(Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx, uint32 index, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data, uint8 header, uint8 sizeOfIndexType);

/**********************************************************************************************************************
 * Com_CprQueue_WriteHeader
 *********************************************************************************************************************/
/*! \brief            This function writes the header (HeaderIdentifier + index) into the queue starting at the writeIdx position.
 *  \details          The intention of using a uint32 as index is that this function will be used by two different functionalities,
 *                    which might not have the same type. Here, biggest type is used, to allow for an implicit upcast.
 *  \param[in,out]    writeIdx         Index to the position in the queue where the data shall be written
 *  \param[in]        index            Handle Id to be written into the queue
 *  \param[in]        headerIdentifier headerIdentifier to be written into the queue
 *  \param[in]        sizeOfIndexType  size of the index that is written into the queue
 *  \pre              -
 *  \context          TASK
 *  \reentrant        FALSE
 *  \synchronous      TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_WriteHeader(CONSTP2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) writeIdx, uint32 index, uint8 headerIdentifier, uint8 sizeOfIndexType);

/**********************************************************************************************************************
 * Com_CprQueue_WriteData
 *********************************************************************************************************************/
/*! \brief            This function writes the data into the queue starting at the writeIdx position.
 *  \details          -
 *  \param[in,out]    writeIdx      Index to the position in the queue where the data shall be written
 *  \param[in]        data          pointer to the data to be written into the queue
 *  \pre              -
 *  \context          TASK
 *  \reentrant        FALSE
 *  \synchronous      TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_WriteData(CONSTP2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) writeIdx, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data);
# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CPRQUEUE_H */
/**********************************************************************************************************************
  END OF FILE: Com_CprQueue.h
**********************************************************************************************************************/
