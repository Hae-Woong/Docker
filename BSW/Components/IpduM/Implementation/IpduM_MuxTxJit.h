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
 *         \file  IpduM_MuxTxJit.h
 *         \unit  MuxTxJit
 *        \brief  Microsar IpduM Multiplexer Tx Just in Time update header file
 *      \details  This is the implementation of the MICROSAR IpduM module.
 *                The basic software module is based on the AUTOSAR IpduM specification.
 *
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
#ifndef IPDUM_MUXTXJIT_H
# define IPDUM_MUXTXJIT_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "IpduM_Cfg.h"
# include "IpduM_Lcfg.h"
# include "IpduM_PBcfg.h"
# include "ComStack_Types.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define IPDUM_START_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_UpdateCounterPart
*******************************************************************************************/
/*! \brief          Updates static or dynamic part. Depending on txPartId it updates the counterpart.
 *  \details        -
 *  \param[in]      txPathwayId       tx pathway handle
 *  \param[in]      txPartId          id of part
 *  \return         E_OK              the trigger transmit call has been successful and data has been updated.
 *  \return         E_NOT_OK          the trigger transmit call has been rejected by the upper layer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different tx Pathway IDs
 *  \synchronous    TRUE
 *  \spec
 *    requires txPathwayId < IpduM_GetSizeOfCTxPathway();
 *    requires    txPartId < IpduM_GetSizeOfTxPart();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_UpdateCounterPart(IpduM_CTxPathwayIterType txPathwayId, IpduM_TxUpIndIdxOfTxUpInfoType txPartId);

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_UpdateAll
*******************************************************************************************/
/*! \brief          Updates all parts of a multiplexer Pdu
 *  \details        -
 *  \param[in]      txPathwayId       tx pathway handle
 *  \return         E_OK              the trigger transmit call has been successful and data has been updated.
 *  \return         E_NOT_OK          the trigger transmit call has been rejected by the upper layer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different tx Pathway IDs
 *  \synchronous    TRUE
 *  \spec
 *    requires txPathwayId < IpduM_GetSizeOfCTxPathway();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_UpdateAll(IpduM_CTxPathwayIterType txPathwayId);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_MUXTXJIT_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_MuxTxJit.h
 *********************************************************************************************************************/
