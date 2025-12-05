/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SoAd_TxSocketManager_Cbk.h
 *         \unit  TxSocketManager
 *        \brief  Callback header file of the TxSocketManager unit for AUTOSAR Socket Adaptor module.
 *      \details  Callback definition of the TxSocketManager unit.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#if !defined (SOAD_TXSOCKETMANAGER_CBK_H)
# define SOAD_TXSOCKETMANAGER_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_CopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      SocketId                Socket identifier.
 *  \param[in]      BufPtr                  Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLength|BufLengthPtr  Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK               Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK         Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData(
  SoAd_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
# if ( SOAD_TX_DYN_LEN == STD_ON )
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr
# else
  uint16 BufLength
# endif /* SOAD_TX_DYN_LEN == STD_ON */
  );

/**********************************************************************************************************************
 *  SoAd_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Confirms transmission of data.
 *  \details        -
 *  \param[in]      SocketId                Socket identifier.
 *  \param[in]      Length                  Length of confirmed data.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxConfirmation(
  SoAd_SocketIdType SocketId,
  uint16 Length);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* !defined(SOAD_TXSOCKETMANAGER_CBK_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_TxSocketManager_Cbk.h
 *********************************************************************************************************************/
