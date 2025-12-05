/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E_PJ193976.c
 *        \brief  E2E source file for E2E Library Profile SAE PJ1939-76
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults 
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for 
 *                safety-related communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_PJ193976.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
#define E2E_PJ193976_MAX_COUNTER_VALUE 31u
#define E2E_PJ193976_CRC_OFFSET 1u
#define E2E_PJ193976_DATA_OFFSET 5u 

#if !defined(E2E_PJ193976_LOCAL) /* COV_E2E_COMPATIBILITY */
#  define E2E_PJ193976_LOCAL static
#endif

#if !defined(E2E_PJ193976_LOCAL_INLINE) /* COV_E2E_COMPATIBILITY */
#  define E2E_PJ193976_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  PROTECT HELPER FUNCTIONS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  E2E_PJ193976ProtectWriteCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Write the computed CRC value to the E2E header
 * \details    -
 * \param[out] DataPtr      Pointer to the data to be protected
 * \param[in]  ComputedCRC  The computed CRC value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
void E2E_PJ193976ProtectWriteCRC(
    uint8* DataPtr,
    const uint32 ComputedCRC);

/**********************************************************************************************************************
 *  E2E_PJ193976ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief      Checks the input data of E2E_PJ193976Protect
 * \details    All relevant input parameters are checked in order to proceed with E2E_PJ193976Protect service
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[in]  StatePtr   Pointer to the state data structure
 * \param[in]  DataPtr    Pointer to the data to be protected
 * \return     Standard return value, indicating valid/invalid input
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
Std_ReturnType E2E_PJ193976ProtectVerifyInputs(
    const E2E_PJ193976ConfigType* ConfigPtr,
    const E2E_PJ193976ProtectStateType* StatePtr,
    const uint8* DataPtr);

/**********************************************************************************************************************
 *  CHECK HELPER FUNCTIONS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  E2E_PJ193976CheckReadCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Return the CRC value of the E2E header
 * \details    Return the CRC value of the E2E header which was stored
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \return     ReadCrc  The CRC value of the E2E header
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
uint32 E2E_PJ193976CheckReadCRC(const uint8* DataPtr);

/**********************************************************************************************************************
 *  E2E_PJ193976CheckVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief         Checks the input data of E2E_PJ193976Check
 * \details       All relevant input parameters are checked in order to proceed with E2E_PJ193976Check service
 * \param[in]     ConfigPtr            Pointer to the config data structure
 * \param[in]     StatePtr             Pointer to the state data structure
 * \param[in]     DataPtr              Pointer to the data to be protected
 * \return        Standard return value, indicating valid/invalid input
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
Std_ReturnType E2E_PJ193976CheckVerifyInputs(
    const E2E_PJ193976ConfigType* ConfigPtr,
    const E2E_PJ193976CheckStateType* StatePtr,
    const uint8* DataPtr);

/**********************************************************************************************************************
 *  E2E_CheckDeltaCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Applies further checks regarding delta counter
 * \details    -
 * \param[in]  ConfigPtr       Pointer to the config data structure
 * \param[in]  StatePtr        Pointer to the state data structure
 * \param[in]  ReceivedCounter Received counter value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
void E2E_CheckDeltaCounter(const E2E_PJ193976ConfigType* ConfigPtr,
                           E2E_PJ193976CheckStateType* StatePtr, uint8 ReceivedCounter);
    

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  E2E_CheckDeltaCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 * 
 */
void E2E_CheckDeltaCounter(const E2E_PJ193976ConfigType* ConfigPtr, 
                        E2E_PJ193976CheckStateType* StatePtr, 
                        uint8 ReceivedCounter)
{
    /* #10 If this is NOT the fist message */
    if (StatePtr->Status != E2E_PJ193976STATUS_INITIAL)
    {
        /* #20 Calculate delta between received and last valid counter. Overflow is also considered */
        uint8 DeltaCounter = (uint8)(ReceivedCounter - StatePtr->LastValidCounter) & (uint8)0x1Fu; /* PRQA S 2897 */ /* MD_MSR_Dir1.1 */

        /* #30 Check if counter didn't change, thus no new data was received */
        if (DeltaCounter == 0u)
        {
            StatePtr->Status = E2E_PJ193976STATUS_REPEATED; /* SBSW_E2E_03 */
        }
        /* #40 If DeltaCounter equals 1, the correct data was received. No message was lost in between */
        else if (DeltaCounter == 1u)
        {

            StatePtr->LastValidCounter = ReceivedCounter; /* SBSW_E2E_03 */
            StatePtr->Status = E2E_PJ193976STATUS_OK; /* SBSW_E2E_03 */
        }
        /* #50 Received data is not subsequent data, but counter is still tolerable */
        else if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
        {

            StatePtr->LastValidCounter = ReceivedCounter; /* SBSW_E2E_03 */
            StatePtr->Status = E2E_PJ193976STATUS_OKSOMELOST; /* SBSW_E2E_03 */
        }
        /* #60 Counter of received data is out of range */
        else
        {
            StatePtr->LastValidCounter = ReceivedCounter; /* SBSW_E2E_03 */
            StatePtr->Status = E2E_PJ193976STATUS_WRONGSEQUENCE; /* SBSW_E2E_03 */
        }
            
    }
    /* #70 ELSE this is the error case */
    else
    {
        StatePtr->Status = E2E_PJ193976STATUS_ERROR;    /* SBSW_E2E_03 */
        StatePtr->LastValidCounter = ReceivedCounter;   /* SBSW_E2E_03 */
    }
}

/**********************************************************************************************************************
 *  PROTECT HELPER FUNCTIONS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  E2E_PJ193976ProtectWriteCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
void E2E_PJ193976ProtectWriteCRC(
    uint8* DataPtr,
    const uint32 ComputedCRC)
{
    DataPtr[E2E_PJ193976_CRC_OFFSET] =  (uint8) ((ComputedCRC ) & (uint32)0xFF);                 /* SBSW_E2E_03 */
    DataPtr[E2E_PJ193976_CRC_OFFSET + 1u] = (uint8) ((ComputedCRC  >>  8u) & (uint32)0xFF);      /* SBSW_E2E_03 */
    DataPtr[E2E_PJ193976_CRC_OFFSET + 2u] = (uint8) ((ComputedCRC  >> 16u) & (uint32)0xFF);      /* SBSW_E2E_03 */
    DataPtr[E2E_PJ193976_CRC_OFFSET + 3u] = (uint8) ((ComputedCRC >> 24u));                      /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_PJ193976ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
Std_ReturnType E2E_PJ193976ProtectVerifyInputs(
    const E2E_PJ193976ConfigType* ConfigPtr,
    const E2E_PJ193976ProtectStateType* StatePtr,
    const uint8* DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check input pointer parameter for NULL_PTR */
    if ((ConfigPtr  != NULL_PTR)
       && (StatePtr != NULL_PTR)
       && (DataPtr  != NULL_PTR)
       )
    {
        uint8 dataLength = ConfigPtr->DataLength >> 3u;

        /* #20 Check minimal allowed DataLength */
        if (dataLength < E2E_PJ193976_DATA_OFFSET)
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
        else
        {
            ret = E2E_E_OK;
        }

    }

    return ret;
}

/**********************************************************************************************************************
 *  CHECK HELPER FUNCTIONS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  E2E_PJ193976CheckReadCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
uint32 E2E_PJ193976CheckReadCRC(const uint8* DataPtr)
{
    return (((uint32) DataPtr[E2E_PJ193976_CRC_OFFSET]))
                | (((uint32) DataPtr[E2E_PJ193976_CRC_OFFSET + 1u]) << 8u)
                | (((uint32) DataPtr[E2E_PJ193976_CRC_OFFSET + 2u]) << 16u)
                | (((uint32) DataPtr[E2E_PJ193976_CRC_OFFSET + 3u]) << 24u);
}
/**********************************************************************************************************************
 *  E2E_PJ193976CheckVerifyInputs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
Std_ReturnType E2E_PJ193976CheckVerifyInputs(
    const E2E_PJ193976ConfigType* ConfigPtr,
    const E2E_PJ193976CheckStateType* StatePtr,
    const uint8* DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;
    
    /* #10 Check input pointer parameter for NULL_PTR */
    if ( (ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR) )
    {
        uint8 dataLength = ConfigPtr->DataLength >> 3u;

        /* #20 #20 Check if value of LastValidCounter and DataLength is within valid range */
        if ((StatePtr->LastValidCounter <= E2E_PJ193976_MAX_COUNTER_VALUE)
            && (ConfigPtr->MaxDeltaCounter <= E2E_PJ193976_MAX_COUNTER_VALUE)
            && (dataLength >= E2E_PJ193976_DATA_OFFSET))
        {
            ret = E2E_E_OK;
        }
        else
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
    }

    return ret;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_PJ193976Protect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_PJ193976Protect(
    const E2E_PJ193976ConfigType* ConfigPtr,
    E2E_PJ193976ProtectStateType* StatePtr,
    uint8* DataPtr)
{
    /* #10 Check validity of input parameters */
    Std_ReturnType ret = E2E_PJ193976ProtectVerifyInputs(ConfigPtr, StatePtr, DataPtr); /* SBSW_E2E_03 */

    if (ret == E2E_E_OK)
    {
        uint8 dataLength = ConfigPtr->DataLength >> 3u;

        uint8 lengthCRCCalculation = dataLength - E2E_PJ193976_DATA_OFFSET;

        /* #20 Compute CRC32 over the whole buffer */
        uint32 ComputedCRC = Crc_CalculateCRC32PJ193976(&DataPtr[E2E_PJ193976_DATA_OFFSET], lengthCRCCalculation, 0xFF, TRUE);      /* SBSW_E2E_03 */

        /* #30 Write SequenceNumber to DataPtr (just use the right 5 bits) */
        DataPtr[0] = (StatePtr->SequenceNumber & (uint8)0x1F);                                 /* SBSW_E2E_03 */

        /* #40 Write computed CRC to DataPtr */
        E2E_PJ193976ProtectWriteCRC(DataPtr, ComputedCRC);                              /* SBSW_E2E_03 */

        /* #50 Increment value of SequenceNumber. Valid values are 0..31  */
        StatePtr->SequenceNumber = (uint8)((StatePtr->SequenceNumber + 1u) % (E2E_PJ193976_MAX_COUNTER_VALUE + 1u)); /* SBSW_E2E_03 */
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_PJ193976ProtectInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_PJ193976ProtectInit(E2E_PJ193976ProtectStateType* StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check StatePtr for NULL_PTR */
    if (StatePtr != NULL_PTR)
    {
        /* #20 If StatePtr is valid, initialize it */
        StatePtr->SequenceNumber = 0u; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_PJ193976Check()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_PJ193976Check(
    const E2E_PJ193976ConfigType* ConfigPtr,
    E2E_PJ193976CheckStateType* StatePtr,
    const uint8* DataPtr)
{
    /* #10 Check validity of input parameters */
    Std_ReturnType ret = E2E_PJ193976CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr); /* SBSW_E2E_03 */

    if (ret == E2E_E_OK)
    {
        /* #20 Read counter from Data */
        uint8 ReceivedCounter = DataPtr[0] & 0x1Fu;

        /* #30 Read CRC from Data */
        uint32 ReceivedCRC = E2E_PJ193976CheckReadCRC(DataPtr);     /* SBSW_E2E_03 */

        /* #40 Calculate CRC in the same way as the Sender did */
        uint8 dataLength = ConfigPtr->DataLength >> 3u;
        uint8 lengthCRCCalculation = dataLength - E2E_PJ193976_DATA_OFFSET;
        uint32 ComputedCRC = Crc_CalculateCRC32PJ193976(&DataPtr[E2E_PJ193976_DATA_OFFSET], lengthCRCCalculation, 0xFF, TRUE);      /* SBSW_E2E_03 */

        /* #50 Check if received CRC matches the computed one */
        if (ReceivedCRC == ComputedCRC)
        {
            /* #60 Evaluate received counter value */
            E2E_CheckDeltaCounter(ConfigPtr, StatePtr, ReceivedCounter);        /* SBSW_E2E_03 */
        }
        else
        {
            StatePtr->Status = E2E_PJ193976STATUS_WRONGCRC;         /* SBSW_E2E_03 */
        }
        
    }
        
    return ret;
}

/**********************************************************************************************************************
 *  E2E_PJ193976CheckInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_PJ193976CheckInit(E2E_PJ193976CheckStateType* StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check StatePtr for NULL_PTR */
    if (StatePtr != NULL_PTR)
    {
        /* #20 If StatePtr is valid, initialize it */
        StatePtr->Status = E2E_PJ193976STATUS_INITIAL;  /* SBSW_E2E_03 */
        StatePtr->LastValidCounter = 0u;                /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_PJ193976MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_PJ193976MapStatusToSM(
    const Std_ReturnType CheckReturn,
    const E2E_PJ193976CheckStatusType Status)
{
    E2E_PCheckStatusType ret = E2E_P_ERROR;

    /* #10 Check CheckReturn value */
    if (CheckReturn == E2E_E_OK)
    {
        /* #20 Map profile J1939-76 specific status to generic SM status */
        switch (Status)
        {
            case E2E_PJ193976STATUS_OK            : ret = E2E_P_OK; break;
            case E2E_PJ193976STATUS_OKSOMELOST    : ret = E2E_P_OK; break;
            case E2E_PJ193976STATUS_REPEATED      : ret = E2E_P_REPEATED; break;
            case E2E_PJ193976STATUS_WRONGSEQUENCE : ret = E2E_P_WRONGSEQUENCE; break;
            default: /* E2E_PJ193976STATUS_ERROR; E2E_PJ193976STATUS_WRONGCRC; E2E_PJ193976STATUS_INITIAL*/
              break;
        }
    }

    return ret;
}

#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: E2E_PJ193976.c
 *********************************************************************************************************************/
