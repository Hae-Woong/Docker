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
/*!        \file  CddDrm_Cbk.h
 *        \brief  CddDrm Callback functions header file
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CDDDRM_CBK_H
# define CDDDRM_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "CddDrm_Cfg.h"
# if (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON)
#  include "NvM.h"
# endif
/* ********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* --- Component Version Information ------------------------------------------------------------------------------- */
# define CDDDRM_CBK_MAJOR_VERSION                                     (7U)
# define CDDDRM_CBK_MINOR_VERSION                                     (0U)
# define CDDDRM_CBK_PATCH_VERSION                                     (1U)

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define CDDDRM_START_SEC_CODE
# include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  CddDrm_StartOfReception
**********************************************************************************************************************/
/*! \brief        This function is called at the start of receiving an I-PDU response.
*  \details       Called once to initialize the reception of a diagnostic response.
*  \param[in]     id                 Identification of the I-PDU.
*  \param[in]     info               Pointer to a PduInfoType structure containing the metadata if available,
*                                    otherwise NULL_PTR.
*  \param[in]     TpSduLength        Total length of the N-SDU to be received.
*  \param[out]    bufferSizePtr      Available receive buffer in the receiving module.
*  \return        BUFREQ_OK          A free buffer is available - start copy data
*  \return        BUFREQ_E_NOT_OK    No free buffer is available - ignore request
*  \return        BUFREQ_E_OVFL      The request message is too long for the configured buffer
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_StartOfReception(PduIdType id,
                                                  P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                  PduLengthType TpSduLength,
                                                  P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
  CddDrm_CopyRxData
**********************************************************************************************************************/
/*! \brief        This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer.
*                 Each call to this function provides the next part of the I-PDU data. The size of the remaining data
*                 is written to the position indicated by bufferSizePtr.
*  \details       Called once upon reception of each segment. Within this call, the received data is copied from the
*                 receive TP buffer to the DCM receive buffer.
*  \param[in]     id                 Identification of the received I-PDU.
*  \param[in]     info               Message context (data and length of the portion)
*  \param[out]    bufferSizePtr      Available receive buffer after data has been copied.
*  \return        BUFREQ_OK          A free buffer is available - data copied according to request
*  \return        BUFREQ_E_NOT_OK    Request could not be handled
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, CDDDRM_CODE) CddDrm_CopyRxData(PduIdType id,
                                                    P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                    P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
*  CddDrm_TpRxIndication()
*********************************************************************************************************************/
/*! \brief        Called after an I-PDU has been received via the TP API, the result indicates whether the
*                 transmission was successful or not.
*  \details       This is called by the PduR to indicate the completion of a reception.
*  \param[in]     id                 Identification of the received I-PDU.
*  \param[in]     result             Result of the reception.
*  \context       ISR1|ISR2|TASK
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \pre           -
*********************************************************************************************************************/
FUNC(void, CDDDRM_APPL_CODE) CddDrm_TpRxIndication(PduIdType id,
                                                   Std_ReturnType result);


/**********************************************************************************************************************
  CddDrm_CopyTxData
**********************************************************************************************************************/
/*! \brief        This function is called to acquire the transmit data of an I-PDU segment (N-PDU). Each call to this
*                 function provides the next part of the I-PDU data.
*  \details       Copy Tx data for the current frame.
*  \param[in]     id                 Identification of the transmitted I-PDU.
*  \param[in]     info               Provides the destination buffer (SduDataPtr) and \n
*                                    the number of bytes to be copied (SduLength).
*  \param[in]     retry              This parameter is used to acknowledge transmitted \n
*                                    data or to retransmit data after transmission problems.
*  \param[out]    availableDataPtr   Indicates the remaining number of bytes that are available in the upper \n
*                                    layer module's Tx buffer.
*  \return        BUFREQ_OK          Data has been copied to the transmit buffer completely as requested.
*  \return        BUFREQ_E_NOT_OK    Data has not been copied. Request failed.
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, CDDDRM_APPL_CODE) CddDrm_CopyTxData(PduIdType id,
                                                P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) info,
                                                P2VAR(RetryInfoType, AUTOMATIC, CDDDRM_APPL_DATA) retry,
                                                P2VAR(PduLengthType, AUTOMATIC, CDDDRM_APPL_DATA) availableDataPtr);

/**********************************************************************************************************************
*  CddDrm_TpTxConfirmation()
*********************************************************************************************************************/
/*! \brief        This function is called after the I-PDU has been transmitted via the TP API, the result indicates
*                 whether the transmission was successful or not.
*  \details       Confirmation of finished response transmission.
*  \param[in]     id                 Identification of the transmitted I-PDU.
*  \param[in]     result             Result of the transmission of the I-PDU.
*  \context       ISR1|ISR2|TASK
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \pre           -
*********************************************************************************************************************/
FUNC(void, CDDDRM_APPL_CODE) CddDrm_TpTxConfirmation(PduIdType id,
                                                     Std_ReturnType result);

# if ((CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON) && (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON))
/**********************************************************************************************************************
  CddDrm_NvM_JobFinished
**********************************************************************************************************************/
/*! \brief         Callback routine to notify the upper layer that an asynchronous single block request has been finished.
*  \details       This function has to be called by the NvM after a write operation has finished.
*                 (See NvMBlockDescriptor/NvMSingleBlockCallback)
*  \param[in]     serviceId    The service identifier of the completed request.
*  \param[in]     jobResult    Result of the single block job.
*  \return        E_OK:        Always returned.
*  \return        E_NOT_OK:    Never returned.
*  \pre           -
*  \context       TASK
*  \reentrant     FALSE
*  \synchronous   TRUE
*  \note          The function is called by the NvM.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_JobFinished(uint8 serviceId, NvM_RequestResultType jobResult);
# endif

# if (CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_CanTpRxMetaData
**********************************************************************************************************************/
/*! \brief        This function is called by CanTp to acquire information how they should handle the metadata during reception.
*  \details       Sets the correct metadata for the N-Sdu and the FC-frame.
*  \param[in]     NPduPayLoad                 Payload which CanTp has retrieved from CanIf.
*  \param[in]     NPduMetaData                MetaData which CanTp has retrieved from CanIf. 
*  \param[out]    ResMetaDataOfNSduOutPtr     MetaData which shall be forwarded to the N-Sdu. The SduLength of this struct \n 
*                                             has the configured MetaDataLength value from the N-SDU.
*  \param[out]    ResMetaDataOfFcNPduOutPtr   MetaData used for sending the FC-frame. The same metadata length for incoming NPdu and outgoing FC-NPdu
*  \param[in,out] ResPayloadOfFcNPduInOutPtr  Contains the max length of Payload in CanTp memory and buffer, used for sending the FC-frame.
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
**********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CanTpRxMetaData(P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) NPduPayLoad,
                                                         P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) NPduMetaData,
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfNSduOutPtr,
                                                         P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfFcNPduOutPtr,
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) PayloadOfFcNPduInOutPtr);

/**********************************************************************************************************************
  CddDrm_CanTpTxMetaData
**********************************************************************************************************************/
/*! \brief        This function is called by CanTp to acquire information how they should handle the metadata during transmission.
*  \details       Sets the correct metadata for the N-Pdu and for matching received FC-frame.
*  \param[in]     MetaDataOfNSduPtr            MetaData which CanTp has retrieved from UL.
*  \param[out]    ResMetaDataOfDataNPduOutPtr  MetaData which shall be forwarded to the N-Pdu and used for all data frames (SF, FF, CF). \n
*                                              The SduLength of this struct has the configured MetaDataLength value from the N-PDU.
*  \param[out]    ResMetaDataOfFcNPduOutPtr    MetaData used for matching received FC-frame. The metadata length of FC-NPdu shall be identical to the length of NPdu.
*  \param[in,out] ResPayloadOfDataNPduInOutPtr Contains the max length of Payload in CanTp memory and buffer, used for all data frames (SF, FF, CF).
*  \param[in,out] ResPayloadOfFcNPduInOutPtr   Contains the max length of Payload in CanTp memory and buffer, used for matching received FC-frame.
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
**********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CanTpTxMetaData(P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfNSduPtr,
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfDataNPduOutPtr,
                                                         P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) MetaDataOfFcNPduOutPtr,
                                                         P2VAR(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) PayloadOfDataNPduInOutPtr,
                                                         P2VAR(uint8, AUTOMATIC, CDDDRM_APPL_DATA) PayloadOfFcNPduInOutPtr);

# endif /* CDDDRM_CFG_SUPPORT_DYNAMIC_CONNECTION == STD_ON */

# define CDDDRM_STOP_SEC_CODE
# include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CDDDRM_CBK_H */
/* ********************************************************************************************************************
 *  END OF FILE: CddDrm_Cbk.h
 *********************************************************************************************************************/
