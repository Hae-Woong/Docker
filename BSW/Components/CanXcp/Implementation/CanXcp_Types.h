/******************************************************************************
 | Project Name:   XCP on CAN Transport Layer for AUTOSAR CanIf Release 3.0
 |    File Name:   CanXcp_Types.h
 |
 |  Description:
 |    Header for definition of types.
 |
 |  Limitations:
 |    - Detection of all XCP slaves within a CAN network is not supported
 |        ( GET_SLAVE_ID )
 |    - Assignment of CAN identifiers to DAQ lists or STIM is not supported
 |        ( GET_DAQ_ID, SET_DAQ_ID )
 |
 |------------------------------------------------------------------------------
 |               C O P Y R I G H T
 |------------------------------------------------------------------------------
 | Copyright (c) 2023 by Vector Informatik GmbH.          All rights reserved.
 |
 |       This software is copyright protected and 
 |       proporietary to Vector Informatik GmbH.
 |       Vector Informatik GmbH grants to you only
 |       those rights as set out in the license conditions.
 |       All other rights remain with Vector Informatik GmbH.
 | 
 |       Diese Software ist urheberrechtlich geschuetzt. 
 |       Vector Informatik GmbH raeumt Ihnen an dieser Software nur 
 |       die in den Lizenzbedingungen ausdruecklich genannten Rechte ein.
 |       Alle anderen Rechte verbleiben bei Vector Informatik GmbH.
 |
 |------------------------------------------------------------------------------
 |  REVISION HISTORY
 |  -------------------------------------------------------------------------------------------------------------------
 |  Refer to the module's header file.
 | 
 |*****************************************************************************/
#if !defined ( CANXCP_TYPES_H )
#define CANXCP_TYPES_H

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "ComStack_Types.h"              /* Include Communication Stack Types */


/*******************************************************************************
* Version
*******************************************************************************/
#define CANXCP_TPS_MAJOR_VERSION         0x04u
#define CANXCP_TPS_MINOR_VERSION         0x00u
#define CANXCP_TPS_PATCH_VERSION         0x01u


/*******************************************************************************
* Default Defintions
*******************************************************************************/

/*******************************************************************************
* Type definitions
*******************************************************************************/

typedef uint16 CanXcp_TimeoutTimerType;

typedef enum
{
  CANXCP_SET_OFFLINE = 0,
  CANXCP_SET_ONLINE
} CanXcp_PduSetModeType;

typedef struct
{
  PduIdType CanXcpRxPduId;
  PduIdType CanXcpTxPduId;
  PduIdType CanIfTxPduId;
  uint8     MaxCto;
  uint16    MaxDto;
  PduLengthType PduSizeTx;
} CanXcp_XcpPduType;

typedef struct
{
  P2CONST(CanXcp_XcpPduType, TYPEDEF, CANXCP_PBCFG) CanXcp_XcpPduPtr;
  P2CONST(NetworkHandleType, TYPEDEF, CANXCP_PBCFG) CanXcp_NwHMappingPtr;
  uint8 CanXcp_NumberOfSystemChannels;
} CanXcp_ConfigType;

#endif /* CANXCP_TYPES_H */

