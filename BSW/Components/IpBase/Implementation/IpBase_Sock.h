/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  IpBase_Sock.h
 *         \unit  Sock
 *        \brief  IpBase socket handling header file
 *      \details  IpBase socket handling routines declaration.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (IPBASE_SOCK_H)
# define IPBASE_SOCK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define IPBASE_START_SEC_CODE
# include "IpBase_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  IpBase_DelSockAddr
 **********************************************************************************************************************/
/*! \brief         Delete socket address
 *  \details       Delete socket address i.e. reset values (incl. family, port, ip-addr).
 *  \param[in]     SockPtr              socket address
 *                                      \spec
 *                                        requires ((Family == IPBASE_AF_INET) &&
 *                                                  $lengthOf(SockPtr) >= sizeof(IpBase_SockAddrInType)) ||
 *                                                 ((Family == IPBASE_AF_INET6) &&
 *                                                  $lengthOf(SockPtr) >= sizeof(IpBase_SockAddrIn6Type));
 *                                      \endspec
 *  \param[in]     Family               supported family
 *  \return        E_OK                 SockAddr could be deleted
 *  \return        E_NOT_OK             SockAddr could not be deleted
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via socket address pointer has to be large enough to reset depending on the
 *                 address family either IPv4 (4 bytes) or IPv6 (16 bytes) address.
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_DelSockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) SockPtr,
                                                   uint16 Family);

/***********************************************************************************************************************
 *  IpBase_CopySockAddr
 **********************************************************************************************************************/
/*! \brief         Copy socket address
 *  \details       Copy socket address (incl. family, port, ip-addr) from Src to Tgt.
 *  \param[out]    TgtSockPtr           target socket address
 *                                      \spec
 *                                        requires ((SrcSockPtr->sa_family == IPBASE_AF_INET) &&
 *                                                  $lengthOf(TgtSockPtr) >= sizeof(IpBase_SockAddrInType)) ||
 *                                                 ((SrcSockPtr->sa_family == IPBASE_AF_INET6) &&
 *                                                  $lengthOf(TgtSockPtr) >= sizeof(IpBase_SockAddrIn6Type));
 *                                      \endspec
 *  \param[in]     SrcSockPtr           source socket address
 *  \return        E_OK                 SockAddr could be copied
 *  \return        E_NOT_OK             SockAddr could not be copied
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via source and target socket address pointer has to be large enough to store
 *                 depending on source address family either IPv4 (4 bytes) or IPv6 (16 bytes) address plus 2 bytes for
 *                 the address family.
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopySockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) TgtSockPtr,
                                                    IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SrcSockPtr);

/***********************************************************************************************************************
 *  IpBase_CopyIpV6Addr
 **********************************************************************************************************************/
/*! \brief         Copy IPv6 socket address
 *  \details       Copy IPv6 socket address (incl. family, port, ip-addr) from Src to Tgt.
 *  \param[out]    TgtIpAddrPtr         target ip address
 *                                      \spec requires $lengthOf(TgtIpAddrPtr) >= sizeof(IpBase_AddrIn6Type); \endspec
 *  \param[in]     SrcIpAddrPtr         source ip address
 *  \return        E_OK                 IP address could be copied
 *  \return        E_NOT_OK             IP address could not be copied
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via source and target socket address pointer has to be large enough to store
 *                 IPv6 (16 bytes).
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyIpV6Addr(IPBASE_P2VARAPPLDATA(IpBase_AddrIn6Type) TgtIpAddrPtr,
                                                    IPBASE_P2CONSTAPPLDATA(IpBase_AddrIn6Type) SrcIpAddrPtr);

/***********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqual
 **********************************************************************************************************************/
/*! \brief         Check if IP address is equal
 *  \details       Check if IP address of sockets is equal.
 *  \param[in]     SockAPtr             target socket address
 *  \param[in]     SockBPtr             source socket address
 *  \return        TRUE                 IP address is equal
 *  \return        FALSE                IP address is not equal
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via source and target socket address pointer has to be large enough to store
 *                 depending on source address family either IPv4 (4 bytes) or IPv6 (16 bytes) address plus 2 bytes for
 *                 the address family.
 **********************************************************************************************************************/
IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                  IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr);

/***********************************************************************************************************************
 *  IpBase_SockPortIsEqual
 **********************************************************************************************************************/
/*! \brief         Check if port is equal
 *  \details       Check if port of sockets is equal.
 *  \param[in]     SockAPtr             target socket address
 *  \param[in]     SockBPtr             source socket address
 *  \return        TRUE                 IP address is equal
 *  \return        FALSE                IP address is not equal
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via source and target socket address pointer has to be large enough to store
 *                 address family (2 bytes) plus 2 bytes for the address port.
 **********************************************************************************************************************/
IPBASE_FUNCCODE(boolean) IpBase_SockPortIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr);

/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum
 **********************************************************************************************************************/
/*! \brief         Calculate TCP/IP checksum
 *  \details       This API calculates the checksum over a given data range. The checksum is TcpIp specific. I.e. it
 *                 expects 16bit data chunks and uses one's complement checksum algorithm.
 *  \param[in]     DataPtr           pointer to the data
 *                                   \spec requires $lengthOf(DataPtr) >= LenByte; \endspec
 *  \param[in]     LenByte           data length in bytes
 *  \return        Calculated checksum
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via data pointer has to be large enough to read data length parameter bytes.
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte);

/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum2
 **********************************************************************************************************************/
/*! \brief         Calculate TCP/IP checksum
 *  \details       This API calculates the checksum over a given data range. The checksum is TcpIp specific. I.e. it
 *                 expects 16bit data chunks and uses one's complement checksum algorithm.
 *  \param[in]     DataPtr           pointer to the data
 *  \param[in]     LenByte           data length in bytes
 *  \param[in]     PseudoHdrPtr      pointer to optional pseudo header (NULL_PTR if omitted)
 *                                   \spec requires if (PseudoHdrPtr != NULL_PTR)
 *                                                    $lengthOf(PseudoHdrPtr) >= PseudoHdrLenByte; \endspec
 *  \param[in]     PseudoHdrLenByte  pseudo header length in bytes
 *  \return        Calculated checksum
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via data pointer has to be large enough to read data length parameter bytes.
 *                 The memory addressed via pseudo header pointer has to be large enough to read pseudo header length
 *                 parameter bytes.
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum2(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte,
                                                  IPBASE_P2CONSTAPPLDATA(uint8) PseudoHdrPtr,
                                                  uint32 PseudoHdrLenByte);
/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAdd
 **********************************************************************************************************************/
/*! \brief         Adds to TCP/IP checksum.
 *  \details       This API adds a range to TcpIp checksum calculation. The checksum is TcpIp specific. I.e. it
 *                 expects 16bit data chunks and uses one's complement checksum algorithm.
 *  \param[in]     DataPtr           pointer to the data
 *                                   \spec requires $lengthOf(DataPtr) >= LenByte; \endspec
 *  \param[in]     LenByte           data length in bytes
 *  \param[in]     Checksum          current 32-bit checksum in host byte order. 0 to start a new checksum calculation
 *  \param[in]     Stop              builds the one's complement to finalize the checksum
 *  \return        Calculated 32-bit checksum in host byte order (Stop==FALSE) or 16-bit checksum in network byte order
 *                 (Stop==TRUE).
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 *  \note          The memory addressed via data pointer has to be large enough to read data length parameter bytes.
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAdd(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte,
                                                uint32 Checksum, boolean Stop);

/**********************************************************************************************************************
 *  IpBase_TcpIpChecksumCopyAdd
 *********************************************************************************************************************/
/*! \brief         Copies data and calculates its additive checksum
 *  \details       Copies a source array of any alignment to a target array of any alignment and calculates the
 *                 additive TcpIp checksum of the data during copy. Processing 32-bit aligned data is the fastest.
 *  \param[out]    TgtDataPtr        Pointer to target data.
 *                                   \spec requires $lengthOf(TgtDataPtr) >= LenByte; \endspec
 *  \param[in]     SrcDataPtr        Pointer to source data.
 *                                   \spec requires  $lengthOf(SrcDataPtr) >= LenByte; \endspec
 *  \param[in]     LenByte           Data length in bytes. Must be a multiple of 2 for all intermediate data blocks,
 *                                   can be odd for the last data block that is copied.
 *                                   The memory addressed via the pointers TgtDataPtr and SrcDataPtr has to be large 
 *                                   enough to copy 'LenByte' bytes. 
 *  \param[in]     Checksum          Current 16-bit checksum in host byte order. 0 to start a new checksum calculation.
 *  \param[in]     Stop              Build the one's complement to finalize the checksum.
 *  \return        Calculated 16-bit checksum in host byte order (Stop==FALSE) or 16-bit checksum in network byte order
 *                 (Stop==TRUE).
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 *  \note          The memory addressed via the data pointers has to be large enough to copy 'LenByte' bytes
 *                 (SMI-1134760).
 *********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumCopyAdd(IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                    IPBASE_CONSTP2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                    uint32 LenByte,
                                                    uint32 Checksum,
                                                    boolean Stop);

# define IPBASE_STOP_SEC_CODE
# include "IpBase_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif
  /* IPBASE_SOCK_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Sock.h
 *********************************************************************************************************************/
