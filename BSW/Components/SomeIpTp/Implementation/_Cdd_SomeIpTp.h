#ifndef _CDD_SOMEIPTP_H
# define _CDD_SOMEIPTP_H

# include "ComStack_Types.h"

# define CDD_SOMEIPTP_MAX_SDU_LENGTH 4000  /* Change if required. */
# define CDD_SOMEIPTP_XF_DATA_OFFSET 8    /* Not expected to be changed. */
# define CDD_SOMEIPTP_LDCOM_IPDU_TX LdComConf_LdComIPdu_Pdu_SomeIp_LargeData_Tx_Out_e2acea87      /* LdCom PDU used during Tx. MUST be changed. */
static const uint8 Cdd_SomeIpTp_xfHeader[CDD_SOMEIPTP_XF_DATA_OFFSET] = {0x00, 0x00, 0x00, 0x01,  /* Request Id used during Tx. Change if required. */
                                                                         0x01,                    /* Protocol Version used during Tx. Change if required.*/
                                                                         0x01,                    /* Interface Version used during Tx. Change if required.*/
                                                                         0x00,                    /* Message Type used during Tx. Change if required.*/
                                                                         0x00};                   /* Return Code used during Tx. Change if required.*/


/* Callback functions to be configured in the LdCom's Tx I-PDU. */
extern BufReq_ReturnType Cdd_SomeIpTp_LdComCbkCopyTxData(const PduInfoType *PduInfoPtr,
                                                         RetryInfoType *RetryInfoPtr,
                                                         PduLengthType *AvailableDataCntPtr);

extern void Cdd_SomeIpTp_LdComCbkTpTxConfirmation(Std_ReturnType result);


/* Callback functions to be configured in the LdCom's Rx I-PDU. */
extern BufReq_ReturnType Cdd_SomeIpTp_LdComCbkRxStartOfReception(const PduInfoType *PduInfoPtr,
                                                                 PduLengthType TpSduLength,
                                                                 PduLengthType *AvailableBuffSize);

extern BufReq_ReturnType Cdd_SomeIpTp_LdComCbkCopyRxData(const PduInfoType *PduInfoPtr,
                                                         PduLengthType *AvailableBuffSize);

extern void Cdd_SomeIpTp_LdComCbkTpRxIndication(Std_ReturnType result);


/* Functions intended to be used by the SWC. */
extern Std_ReturnType Cdd_SomeIpTp_Read(uint8 *dst, PduLengthType *length);

extern Std_ReturnType Cdd_SomeIpTp_Write(uint8 *src, uint16 length);

extern boolean Cdd_SomeIpTp_IsUpdated(void);

#endif /* _CDD_SOMEIPTP_H */
