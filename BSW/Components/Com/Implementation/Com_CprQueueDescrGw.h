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
/*!        \file  Com_CprQueueDescrGw.h
 *         \unit  CprQueueDescrGw
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_CPRQUEUEDESCRGW_H)
# define COM_CPRQUEUEDESCRGW_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
# define COM_CPRQUEUE_HEADERVALUE_DESCRIPTION   0x02u
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
 * Com_CprQueueDescrGw_ReadData
 *********************************************************************************************************************/
/*! \brief       Get the content of the next description routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[out]  gwTxPduIdx               Index of TxPduDescriptionInfo
 *  \param[out]  data                     Pointer to the pointer to the data and the length
 *  \param[in]   readIdx                  Index to the position in the queue where the data shall be read
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueDescrGw_ReadData(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_SizeOfGwTxPduDescriptionInfoType, AUTOMATIC, AUTOMATIC) gwTxPduIdx,
                                                  CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_WriteData
 *********************************************************************************************************************/
/*! \brief       Write the content of a description routing into the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   gwTxPduIdx               Index of TxPduDescriptionInfo
 *  \param[in]   data                     Pointer to the pointer to the data to be written and the length
 *  \return      E_OK                     Write operation was successful.
 *               E_NOT_OK                 Write operation was not successful.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueDescrGw_WriteData(Com_CCprQueueIterType routingQueueIndex, Com_SizeOfGwTxPduDescriptionInfoType gwTxPduIdx,
                                                             CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data);

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_UpdateReadIdx
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
FUNC(void, COM_CODE) Com_CprQueueDescrGw_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex);

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_GetReadIdx
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
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueDescrGw_GetReadIdx(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) readIdx);

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_ReadFromCprQueueDirectly
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
FUNC(uint8, COM_CODE) Com_CprQueueDescrGw_ReadFromCprQueueDirectly(Com_CprQueueBufferIterType readIdx);

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*! \brief       This function resets the NumberOfBytesRead.
 *  \details     Called before a batch of read operations that should in sum not exceed a threshold of bytes read.
 *  \pre         -
*  \param[in]    routingQueueIndex        Id of the queue to be used
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueDescrGw_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex);
# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CprQueueDescrGw_H */
/**********************************************************************************************************************
  END OF FILE: Com_CprQueueDescrGw.h
**********************************************************************************************************************/
