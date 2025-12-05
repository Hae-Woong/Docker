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
/*!        \file  Com_Deserializer.h
 *         \unit  Deserializer
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_DESERIALIZER_H)
# define COM_DESERIALIZER_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

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

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_UInt8
*********************************************************************************************************************/
/*!
    \brief        Read a uint8 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read uint8 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Deserializer_ReadSignal_UInt8(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_SInt8
*********************************************************************************************************************/
/*!
    \brief        Read a sint8 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read sint8 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(sint8, COM_CODE) Com_Deserializer_ReadSignal_SInt8(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_UInt16
*********************************************************************************************************************/
/*!
    \brief        Read a uint16 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read uint16 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint16, COM_CODE) Com_Deserializer_ReadSignal_UInt16(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_SInt16
*********************************************************************************************************************/
/*!
    \brief        Read a sint16 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read sint16 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(sint16, COM_CODE) Com_Deserializer_ReadSignal_SInt16(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_UInt32
*********************************************************************************************************************/
/*!
    \brief        Read a uint32 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read uint32 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint32, COM_CODE) Com_Deserializer_ReadSignal_UInt32(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_SInt32
*********************************************************************************************************************/
/*!
    \brief        Read a sint32 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read sint32 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(sint32, COM_CODE) Com_Deserializer_ReadSignal_SInt32(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_UInt64
*********************************************************************************************************************/
/*!
    \brief        Read a uint64 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read uint64 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(uint64, COM_CODE) Com_Deserializer_ReadSignal_UInt64(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_SInt64
*********************************************************************************************************************/
/*!
    \brief        Read a sint64 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read sint64 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(sint64, COM_CODE) Com_Deserializer_ReadSignal_SInt64(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_Float32
*********************************************************************************************************************/
/*!
    \brief        Read a float32 Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read float32 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(float32, COM_CODE) Com_Deserializer_ReadSignal_Float32(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
*  Com_Deserializer_ReadSignal_Float64
*********************************************************************************************************************/
/*!
    \brief        Read a float64  Signal value from the passed PDU buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo      index of ComSignal.
    \param[in]    SignalBusSourcePtr   buffer the value is read from.
    \return       read float64 signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalBusSourcePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(float64, COM_CODE) Com_Deserializer_ReadSignal_Float64(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_DESERIALIZER_H */
/**********************************************************************************************************************
  END OF FILE: Com_Deserializer.h
**********************************************************************************************************************/
