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
 *         File:  LinTp_Types.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4TpLin
 *    Generator:  If_Asr4TpLin
 *
 *  Description:  Declaration of types and defines of the AUTOSAR LIN Transport Protocol
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#if !defined (LINTP_TYPES_H)
# define LINTP_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* include ComStack_Types.h so all needed types are known */
# include "ComStack_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define LINTP_TPS_MAJOR_VERSION                    (12u)
# define LINTP_TPS_MINOR_VERSION                    (0u)
# define LINTP_TPS_PATCH_VERSION                    (0u)

/* generator compatibility */
# define LINTP_GENERATOR_COMPATIBILITY_VERSION      ((uint16)0x0107u)

/* type definitions of LinTp */
# define LINTP_FUNC_REQUEST_IDLE                    (0x00u)
# define LINTP_FUNC_REQUEST_PENDING                 (0x01u)
# define LINTP_FUNC_REQUEST_TRANSMITTED             (0x02u)

# define LINTP_LOCAL_BUFFER_SIZE                    (8u) /* currently must be a power of 2 because of mask definition below */
# define LINTP_LOCAL_BUFFER_SIZE_MASK               (LINTP_LOCAL_BUFFER_SIZE - 1u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8                                       LinTp_ChannelType;

typedef struct LinTp_ControlTypeTag /*! Control structure of LinTp for one channel */
{
  uint16                           LinTp_Timer;               /*!< Current timer value for timeout handling               */
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  uint16                           LinTp_P2Timer;             /*!< Storing current timer value used for P2 and P2max      */
  uint16                           LinTp_RespPendingCounter;  /*!< Number of received response pending frames             */
# endif
  PduLengthType                    LinTp_RemainingLength;     /*!< Bytes to be stored in PduR buffer / to be transmitted  */
  PduIdType                        LinTp_Curr_UL_NSdu_Id;     /*!< Network layer Pdu ID of current request / response     */

  PduInfoType                      LinTp_PduInfoPtr;          /*!< PduInfo structure passed to PduR                       */
  PduIdType                        LinTp_Curr_Rx_Id;          /*!< Internal ID of expected or current RxNSdu              */
  PduIdType                        LinTp_Curr_Tx_Id;          /*!< Internal ID of current TxNSdu                          */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  PduInfoType                      LinTp_FuncPduInfoPtr;      /*!< PduInfo structure passed to PduR for functional req    */
  PduIdType                        LinTp_Curr_Func_NSdu_Id;   /*!< Network layer Pdu ID of functional request             */
  uint8                            LinTp_ComModeRestore;      /*!< Recovery to correct schedule after functional request  */
  uint8                            LinTp_FuncRequest;         /*!< Status of the functional request                       */
#  endif
# endif

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON ) && ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  uint8                            LinTp_NodeType;            /*!< Node type of Tp channel, either master or slave        */
# endif

  uint8                            LinTp_Ch_State;            /*!< State engine                                           */
  uint8                            LinTp_NAD_Current;         /*!< NAD of current request or response, also used to compare
                                                                   request NAD to response NAD in LIN master case         */
  uint8                            LinTp_Tx_Rx_Buffer_Idx;    /*!< Null based index of Tp buffer                          */
  uint8                            LinTp_Tx_Rx_Buffer_Length; /*!< Number of payload bytes of current frame               */
  uint8                            LinTp_Tx_Rx_Buffer[LINTP_LOCAL_BUFFER_SIZE];     /*!< Local Tp buffer                  */
  uint8                            LinTp_SN;                  /*!< Sequence Number of CFs                                 */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  uint8                            LinTp_PortNotification;    /*!< Indication flag to change the schedule table           */

  uint8                            LinTp_PduRNotification;    /*!< Indication flag to notify PduR on Tp Tx/Rx frame       */
  uint8                            LinTp_PduRNotifyResultTx;  /*!< Result of Tx notification to PduR                      */
  PduIdType                        LinTp_PduRNSduIdTx;        /*!< Tx NsduId which shall be reported to PduR              */
  uint8                            LinTp_PduRNotifyResultRx;  /*!< Result of Rx notification to PduR                      */
  PduIdType                        LinTp_PduRNSduIdRx;        /*!< Rx NsduId which shall be reported to PduR              */
  PduIdType                        LinTp_PduRNSduIdTm;        /*!< NsduId which shall be reported to PduR                 */
# endif
} LinTp_ControlType;

#endif /* LINTP_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: LinTp_Types.h
 *********************************************************************************************************************/

