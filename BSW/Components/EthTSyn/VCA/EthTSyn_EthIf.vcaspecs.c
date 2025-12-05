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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_EthIf.vcaspecs.c
 *        \brief  EthIf stub for Vector Code Analyzer
 *      \details  Stub of EthIf functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2021-07-15  vissem                      Initial creation.
 *  02.00.00  2023-07-14  vissem        SAA-4016      Replace all usage of Eth_TimestampQualityType by
 *                                                    Eth_TimeStampQualType
 *********************************************************************************************************************/

#include "EthIf.h"

/*! Stubs the function such that it sets a buffer in external memory with at least the requested length.
 * May return E_OK or E_NOT_OK
 * \spec
 *    requires BufPtr != NULL_PTR;
 *    requires LenBytePtr != NULL_PTR;
 * \endspec
 */
BufReq_ReturnType EthIf_ProvideTxBuffer(
  uint8          CtrlIdx,
  Eth_FrameType  FrameType,
  uint8          Priority,
  uint8         *BufIdxPtr,
  Eth_DataType **BufPtr,
  uint16        *LenBytePtr)
{
  *BufPtr = $undef();
  $assume($external(*BufPtr));
  $assume($lengthOf(*BufPtr) >= (*LenBytePtr));

  return $range(0,255);
}

/*! Stubs the function to have no side effects (because PhysAddrPtr is not const).
 * May return any return code.
 */
Std_ReturnType EthIf_Transmit(
  uint8          CtrlIdx,
  uint8          BufIdx,
  Eth_FrameType  FrameType,
  boolean        TxConfirmation,
  uint16         LenByte,
  uint8         *PhysAddrPtr)
{
  return $range(0,255);
}

/*!
 * \spec
 *  requires TimestampPtr != NULL_PTR;
 *  requires TimestampQualityPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType EthIf_GetEgressTimestamp(
  uint8                  CtrlIdx,
  uint8                  BufIdx,
  Eth_TimeStampType     *TimestampPtr,
  Eth_TimeStampQualType *TimestampQualityPtr)
{
  return $range(0,255);
}

/*!
 * \spec
 *  requires DataPtr != NULL_PTR;
 *  requires TimestampPtr != NULL_PTR;
 *  requires TimestampQualityPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType EthIf_GetIngressTimestamp(
  uint8                  CtrlIdx,
  Eth_DataType          *DataPtr,
  Eth_TimeStampType     *TimestampPtr,
  Eth_TimeStampQualType *TimestampQualityPtr)
{
  return $range(0,255);
}

/*!
 * \spec
 *  requires PhysAddrPtr != NULL_PTR;
 *  requires $lengthOf(PhysAddrPtr) >= 6u;
 * \endspec
 */
void EthIf_GetPhysAddr(
  uint8  CtrlIdx,
  uint8 *PhysAddrPtr)
{
  uint8 i;
  for (i = 0; i < 6u; i++)
  {
    PhysAddrPtr[i] = $range(0, 255);
  }
}

/*!
 * \spec
 *  requires PhysAddrPtr != NULL_PTR;
 *  requires $lengthOf(PhysAddrPtr) >= 6u;
 *  requires Action == ETH_ADD_TO_FILTER || Action == ETH_REMOVE_FROM_FILTER;
 * \endspec
 */
Std_ReturnType EthIf_UpdatePhysAddrFilter(
        uint8                 CtrlIdx,
  const uint8                *PhysAddrPtr,
        Eth_FilterActionType  Action)
{
  return $range(0,255);
}

/*!
 * \spec
 *  requires timeQualPtr != NULL_PTR;
 *  requires timeStampPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType EthIf_GetCurrentTime(
  uint8                  CtrlIdx,
  Eth_TimeStampQualType *timeQualPtr,
  Eth_TimeStampType     *timeStampPtr)
{
  return $range(0,255);
}

/*! Stubs the function such that it sets a MgmtObject in external memory.
 * May return E_OK or E_NOT_OK
 * \spec
 *  requires DataPtr != NULL_PTR;
 *  requires MgmtObjectPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType EthIf_GetRxMgmtObject(
  uint8                   CtrlIdx,
  Eth_DataType           *DataPtr,
  EthSwt_MgmtObjectType **MgmtObjectPtr)
{
  if(MgmtObjectPtr != NULL_PTR)
  {
    *MgmtObjectPtr = $undef();
    $assume($external(*MgmtObjectPtr));
  }

  return $range(0,255);
}

/*! Stubs the function such that it sets a MgmtObject in external memory.
 * May return E_OK or E_NOT_OK
 * \spec
 *  requires MgmtObjectPtr != NULL_PTR;
 * \endspec
 */
Std_ReturnType EthIf_GetTxMgmtObject(
  uint8                   CtrlIdx,
  Eth_BufIdxType          BufIdx,
  EthSwt_MgmtObjectType **MgmtObjectPtr)
{
  if(MgmtObjectPtr != NULL_PTR)
  {
    *MgmtObjectPtr = $undef();
    $assume($external(*MgmtObjectPtr));
  }

  return $range(0,255);
}
