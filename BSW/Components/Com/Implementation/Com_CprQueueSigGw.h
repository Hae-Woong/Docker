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
/*!        \file  Com_CprQueueSigGw.h
 *         \unit  CprQueueSigGw
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_CPRQUEUESIGGW_H)
# define COM_CPRQUEUESIGGW_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
# define COM_CPRQUEUE_HEADERVALUE_SIGNAL        0x01u
# define COM_CPRQUEUE_HEADERVALUE_SIGNALGROUP   0x03u
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
 * Com_CprQueueSigGw_Read_Signal
 *********************************************************************************************************************/
/*! \brief       Get the content of the next signal gateway routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   readIdx                  Index to the position in the queue where the data shall be read
 *  \param[out]  idxGwSigMapping          Index of the gateway signal mapping
 *  \param[out]  data                     Pointer to the data
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_Signal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                   CONSTP2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigMapping, CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_SignalGroupId
 *********************************************************************************************************************/
/*! \brief       Read the index of the signalGroup gateway routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   readIdx                  Index to the position in the queue where the data shall be read
 *  \param[out]  idxGwSigGrpMapping       Read index of the gateway signalGroup mapping
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_SignalGroupId(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                        CONSTP2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigGrpMapping);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_GroupSignal
 *********************************************************************************************************************/
/*! \brief       Get the content of the next group signal gateway routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[out]  idxGwGrpSigMapping       Index of the gateway group signal mapping
 *  \param[out]  data                     Pointer to the data
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_GroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingIterType idxGwGrpSigMapping,
                                                        CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Write_Signal
 *********************************************************************************************************************/
/*! \brief       Write the content of a signal or groupSignal gateway routing into the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   idxGwSigMapping          Index of the gateway signal mapping
 *  \param[in]   data                     Pointer to the data to be written
 *  \return      E_OK                     Write operation was successful.
 *               E_NOT_OK                 Write operation was not successful.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_Signal(Com_CCprQueueIterType routingQueueIndex, Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping,
                                                              CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Write_SignalGroup
 *********************************************************************************************************************/
/*! \brief       Write the signalGroup gateway routing into the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   idxGwSigGrpMapping       Index of the gateway signalGroup mapping
 *  \param[in]   length                   Length of the Signal Group in Byte
 *  \return      E_OK                     Write operation was successful.
 *               E_NOT_OK                 Write operation was not successful.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping,
                                                                  Com_SignalGroupLengthInByteOfGwSigGrpMappingType length);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_UpdateReadIdx
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
FUNC(void, COM_CODE) Com_CprQueueSigGw_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetReadIdx
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
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_GetReadIdx(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) readIdx);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_ReadFromCprQueueDirectly
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
FUNC(uint8, COM_CODE) Com_CprQueueSigGw_ReadFromCprQueueDirectly(Com_CprQueueBufferIterType readIdx);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*! \brief       This function resets the NumberOfBytesRead.
 *  \details     Called before a batch of read operations that should in sum not exceed a threshold of bytes read.
 *  \pre         -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex);
#endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CprQueueSigGw_H */
/**********************************************************************************************************************
  END OF FILE: Com_CprQueueSigGw.h
**********************************************************************************************************************/
