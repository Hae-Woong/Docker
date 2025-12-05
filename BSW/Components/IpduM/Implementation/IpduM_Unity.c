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
/*!        \file  IpduM_Unity.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define IPDUM_UNITY_SOURCE

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "IpduM_ContainerTxRequestQueueUtil.c"
#include "IpduM_ContainerTxSendTimeout.c"
#include "IpduM_ContainerTxTrigger.c"
#include "IpduM_ContainerUtil.c"
#include "IpduM_MuxRx.c"
#include "IpduM_MuxTx.c"
#include "IpduM_MuxTxJit.c"
#include "IpduM_MuxUtil.c"
#include "IpduM_Reporting.c"
#include "IpduM_TxConfBuffer.c"
#include "IpduM_TxConfirmation.c"
#include "IpduM_ContainerRx.c"
#include "IpduM_ContainerRxProcessing.c"
#include "IpduM_ContainerRxQueue.c"
#include "IpduM_ContainerTx.c"
#include "IpduM_ContainerTxDataQueue.c"
#include "IpduM_ContainerTxDataQueueHandling.c"
#include "IpduM_ContainerTxDataQueueInstance.c"
#include "IpduM_ContainerTxDataQueueUtil.c"
#include "IpduM_ContainerTxRequestQueue.c"
#include "IpduM_ContainerTxRequestQueueHandling.c"
#include "IpduM_ContainerTxRequestQueuePriorityHandling.c"
#include "IpduM.c"

/**********************************************************************************************************************
 *  END OF FILE: IpduM_Unity.c
 *********************************************************************************************************************/
