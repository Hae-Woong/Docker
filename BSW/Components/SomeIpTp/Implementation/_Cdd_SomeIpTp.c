#include "Cdd_SomeIpTp.h"
#include "LdCom_Cfg.h"
#include "LdCom.h"
#include "vstdlib.h"

/*
 * This code allows the reception of a single SomeIpTp Rx SDU and the transmission
 * of a single SomeIpTp Tx SDU. If more SDUs must be supported, then extra buffers,
 * flags and functions would have to be added accordingly for each extra SDU.
 */

static uint8 txBuffer[CDD_SOMEIPTP_MAX_SDU_LENGTH];
static uint16 txDataLength = 0;
static uint16 txDataIndex = 0;

static uint8 rxBuffer1[CDD_SOMEIPTP_MAX_SDU_LENGTH];
static uint8 rxBuffer2[CDD_SOMEIPTP_MAX_SDU_LENGTH];

static uint8 *unlockedRxData = rxBuffer1;
static uint16 unlockedRxDataLength = 0;

static uint8 *lockedRxData = rxBuffer2;
static uint16 lockedRxDataLength = 0;

static boolean isTxBusy = FALSE;
static boolean isRxDataUpdated = FALSE;


static Std_ReturnType Cdd_SomeIpTp_Xf(uint8 *dst, uint16 *dstLength, uint8 *data, uint16 dataLength);
static Std_ReturnType Cdd_SomeIpTp_XfInv(uint8 *dst, uint16 *length);
static void Cdd_SomeIpTp_ToggleRxBuffer(void);


BufReq_ReturnType Cdd_SomeIpTp_LdComCbkCopyTxData(const PduInfoType *PduInfoPtr,
                                                  RetryInfoType *RetryInfoPtr,
                                                  PduLengthType *AvailableDataCntPtr)
{
  BufReq_ReturnType lRet;
  (void)RetryInfoPtr;

  if(PduInfoPtr->SduLength <= (txDataLength - txDataIndex))
  {
    VStdRamMemCpy(PduInfoPtr->SduDataPtr, &txBuffer[txDataIndex], PduInfoPtr->SduLength);
    txDataIndex = txDataIndex + PduInfoPtr->SduLength;
    lRet= BUFREQ_OK;
  }
  else
  {
    lRet = BUFREQ_E_NOT_OK;
  }

  *AvailableDataCntPtr = txDataLength - txDataIndex;
  return lRet;
}

void Cdd_SomeIpTp_LdComCbkTpTxConfirmation(Std_ReturnType result)
{
  (void)result;
  txDataLength = 0;
  txDataIndex = 0;
  isTxBusy = FALSE;
}

BufReq_ReturnType Cdd_SomeIpTp_LdComCbkRxStartOfReception(const PduInfoType *PduInfoPtr,
                                                          PduLengthType TpSduLength,
                                                          PduLengthType *AvailableBuffSize)
{
  (void)PduInfoPtr;
  (void)TpSduLength;
  unlockedRxDataLength = 0;

  *AvailableBuffSize = CDD_SOMEIPTP_MAX_SDU_LENGTH;
  return BUFREQ_OK;
}

BufReq_ReturnType Cdd_SomeIpTp_LdComCbkCopyRxData(const PduInfoType *PduInfoPtr,
                                                  PduLengthType *AvailableBuffSize)
{
  /* Check if enough Rx buffer is currently available. */
  if(PduInfoPtr->SduLength <= (CDD_SOMEIPTP_MAX_SDU_LENGTH - unlockedRxDataLength))
  {
    VStdRamMemCpy(&unlockedRxData[unlockedRxDataLength], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
    unlockedRxDataLength += PduInfoPtr->SduLength;
    *AvailableBuffSize = CDD_SOMEIPTP_MAX_SDU_LENGTH - unlockedRxDataLength;
    return BUFREQ_OK;
  }
  else
  {
    return BUFREQ_E_NOT_OK;
  }
}

void Cdd_SomeIpTp_LdComCbkTpRxIndication(Std_ReturnType result)
{
  if (result == E_OK)
  {
    Cdd_SomeIpTp_ToggleRxBuffer();
    isRxDataUpdated = TRUE;
  }
}

Std_ReturnType Cdd_SomeIpTp_Read(uint8 *dst, uint16 *length)
{
  Cdd_SomeIpTp_XfInv(dst, length);
  isRxDataUpdated = FALSE;
  return RTE_E_OK;
}

Std_ReturnType Cdd_SomeIpTp_Write(uint8 *src, uint16 length)
{
  Std_ReturnType ret;

  if (isTxBusy == FALSE)
  {
    ret = Cdd_SomeIpTp_Xf(txBuffer, &txDataLength, src, length);
    if (ret == E_OK)
    {
      PduInfoType pduInfo;
      pduInfo.SduDataPtr = txBuffer;
      pduInfo.SduLength = txDataLength;

      ret = LdCom_Transmit(CDD_SOMEIPTP_LDCOM_IPDU_TX, &pduInfo);
      if (ret == E_OK)
      {
        isTxBusy = TRUE;
        ret = RTE_E_OK;
      }
    }
  }
  else
  {
    ret = RTE_E_COM_BUSY;
  }
  return ret;
}

boolean Cdd_SomeIpTp_IsUpdated(void)
{
  return isRxDataUpdated;
}

Std_ReturnType Cdd_SomeIpTp_Xf(uint8 *dst, uint16 *dstLength, uint8 *data, uint16 dataLength)
{
  /* Append Request Id, Protocol Version, Interface Version, Message Type and Return Code. */
  if ((CDD_SOMEIPTP_XF_DATA_OFFSET + dataLength) <= CDD_SOMEIPTP_MAX_SDU_LENGTH)
  {
    uint16 i = 0;
    for (i = 0; i < CDD_SOMEIPTP_XF_DATA_OFFSET; i++)
    {
      dst[i] = Cdd_SomeIpTp_xfHeader[i];
    }

    VStdRamMemCpy(&dst[CDD_SOMEIPTP_XF_DATA_OFFSET], data, dataLength);
    *dstLength = CDD_SOMEIPTP_XF_DATA_OFFSET + dataLength;
    return E_OK;
  }
  else
  {
    return E_NOT_OK;
  }
}

Std_ReturnType Cdd_SomeIpTp_XfInv(uint8 *dst, uint16 *length)
{
  /* Remove Request Id, Protocol Version, Interface Version, Message Type and Return Code. */
  VStdRamMemCpy(dst, &lockedRxData[CDD_SOMEIPTP_XF_DATA_OFFSET], lockedRxDataLength);
  *length = lockedRxDataLength - CDD_SOMEIPTP_XF_DATA_OFFSET;
  return E_OK;
}

void Cdd_SomeIpTp_ToggleRxBuffer(void)
{
  uint8 *tmpBuffer = unlockedRxData;
  uint16 tmpBufferLength = unlockedRxDataLength;

  unlockedRxData = lockedRxData;
  unlockedRxDataLength = lockedRxDataLength;

  lockedRxData = tmpBuffer;
  lockedRxDataLength = tmpBufferLength;
}
