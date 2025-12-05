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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  TcpIp_Priv_Types.h
 *        \brief  Implementation of TcpIp Module - Internal Type Definitions
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_PRIV_TYPES_H)
# define TCPIP_PRIV_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "TcpIp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef TCPIP_P2V(uint8)           TcpIp_Uint8P2V;
typedef TCPIP_P2C(uint8)           TcpIp_Uint8P2C;

/* Generic data container describing a buffer or a segment within a buffer */
typedef struct
{
  TCPIP_P2V(uint8) BasePtr;   /* Points to the beginning of a data buffer */
  TCPIP_P2V(uint8) AvailPtr;  /* Points to the next available byte position of the data buffer */
  uint32           Size;      /* Specifies the total size of the referenced buffer */
  uint32           Usage;     /* Keeps track of the used space in the referenced buffer */
  uint32           AvailLen;  /* Remaining number of bytes that can be written */
} TcpIp_SegmentType;

typedef struct
{
  TCPIP_P2C(uint8) BasePtr;   /* Points to the beginning of a buffer where data to read is located */
  TCPIP_P2C(uint8) AvailPtr;  /* Points to the first available byte position of the data buffer which was not read yet */
  uint32           Size;      /* Specifies the total size of the referenced buffer */
  uint32           Usage;     /* Keeps track of the read space in the referenced buffer */
  uint32           AvailLen;  /* Remaining number of bytes that can be read */
} TcpIp_ConstSegmentType;

typedef TCPIP_P2V(TcpIp_SegmentType)      TcpIp_SegmentPtrType;
typedef TCPIP_P2V(TcpIp_ConstSegmentType) TcpIp_ConstSegmentPtrType;
#endif
  /* TCPIP_PRIV_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Priv_Types.h
 *********************************************************************************************************************/
