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
/*!        \file  Com_Unity.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_UNITY_SOURCE

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_Caching.c"
#include "Com_CheckUpdateBit.c"
#include "Com_CprQueue.c"
#include "Com_CprQueueDescrGw.c"
#include "Com_CprQueueSigGw.c"
#include "Com_Deserializer.c"
#include "Com_DesGw.c"
#include "Com_DesGwBc.c"
#include "Com_DesGwCp.c"
#include "Com_DesGwSp.c"
#include "Com_EventCache.c"
#include "Com_GwTout.c"
#include "Com_Initialization.c"
#include "Com_IPduGroupHdlr.c"
#include "Com_ISRThreshold.c"
#include "Com_LLRxIf.c"
#include "Com_LLRxTp.c"
#include "Com_LLTxIf.c"
#include "Com_LLTxTp.c"
#include "Com_MainFunctions.c"
#include "Com_Notifications.c"
#include "Com_Repetition.c"
#include "Com_Reporting.c"
#include "Com_RxDlMon.c"
#include "Com_RxInv.c"
#include "Com_RxPduBuffer.c"
#include "Com_RxPduProcessing.c"
#include "Com_RxSigBuffer.c"
#include "Com_RxSigBufferHelper.c"
#include "Com_RxSignalFiltering.c"
#include "Com_RxSignalIf.c"
#include "Com_RxSignalProcessing.c"
#include "Com_Serializer.c"
#include "Com_SigGw.c"
#include "Com_SigGwCP.c"
#include "Com_SigGwSP.c"
#include "Com_SignalFilterHdlr.c"
#include "Com_SignalFilterHdlrHelper.c"
#include "Com_Timer.c"
#include "Com_Transmission.c"
#include "Com_TxBuffer.c"
#include "Com_TxCyclic.c"
#include "Com_TxDlMon.c"
#include "Com_TxGroupSignalProcessing.c"
#include "Com_TxInv.c"
#include "Com_TxMinDelay.c"
#include "Com_TxModeHdlr.c"
#include "Com_TxSignalFiltering.c"
#include "Com_TxSignalIf.c"
#include "Com_TxSignalProcessing.c"
#include "Com_TxTransmit.c"
#include "Com_Util.c"
#include "Com.c"

/**********************************************************************************************************************
 *  END OF FILE: Com_Unity.c
 *********************************************************************************************************************/
