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
/*!        \file  Com_TxModeHdlr.h
 *         \unit  TxModeHdlr
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXMODEHDLR_H)
# define COM_TXMODEHDLR_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

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
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/**********************************************************************************************************************
Com_TxModeHdlr_IsDirectTxMode
**********************************************************************************************************************/
/*! \brief        This function checks if the current tx mode of the passed Tx ComIPdu is DIRECT.
    \details      -
    \pre          -
    \param[in]    TxPduId               Id of Tx ComIPdu.
    \return       boolean
                    TRUE if current tx mode is DIRECT
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxModeHdlr_IsDirectTxMode(Com_TxPduInfoIterType TxPduId);
#ifdef __PRQA__
#pragma PRQA_NO_SIDE_EFFECTS Com_TxModeHdlr_IsDirectTxMode
#endif

/**********************************************************************************************************************
  Com_TxModeHdlr_IsPeriodicTxMode
**********************************************************************************************************************/
/*! \brief        This function checks if the current tx mode of the passed Tx ComIPdu is periodic.
    \details      -
    \pre          -
    \return       boolean
                    TRUE if current tx mode is periodic
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxModeHdlr_IsPeriodicTxMode(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_TxModeHdlr_GetCurrentTimePeriod
**********************************************************************************************************************/
/*! \brief        This function returns the configured time period of the current tx mode.
    \details      -
    \pre          -
    \return       Com_CycleTimeCntType configured time period of the current tx mode.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_CycleTimeCntType, COM_CODE) Com_TxModeHdlr_GetCurrentTimePeriod(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_TxModeHdlr_GetCurrentTimeOffset
**********************************************************************************************************************/
/*! \brief        This function returns the configured time offset (time between the start of the I-PDU by
 *                Com_IpduGroupControl and the first transmission request) of the current tx mode.
    \details      -
    \pre          -
    \return       Com_CycleTimeCntType configured time offset of the current tx mode.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_CycleTimeCntType, COM_CODE) Com_TxModeHdlr_GetCurrentTimeOffset(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_TxModeHdlr_InitTxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a Tx I-PDU in Com_TxModeHdlr.
    \details      -
    \pre          -
    \param[in]    ComPduId Handle of the Tx I-PDU.
    \trace        SPEC-2736775
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxModeHdlr_InitTxIpdu(Com_TxPduInfoIterType ComPduId);

/**********************************************************************************************************************
  Com_TxModeHdlr_UpdateTMS
**********************************************************************************************************************/
/*! \brief        This method manages the I-PDU transmission mode selection.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo    ID of Tx I-PDU.
    \context      TASK|ISR2
    \trace        SPEC-2736812, SPEC-2736813, SPEC-2736814
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxPduInfo < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxModeHdlr_UpdateTMS(Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_TxModeHdlr_StopTxIpdus
**********************************************************************************************************************/
/*! \brief        This method cancels outstanding transmission requests of an I-PDU and reloads the time offset counter.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \trace        SPEC-2736842
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxModeHdlr_StopTxIpdus(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxModeHdlr_SwitchIpduTxMode
**********************************************************************************************************************/
/*! \brief        This method switches the TxMode of the referenced IPdu to Mode.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \param[in]    Mode        Tx Mode to be set.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxModeHdlr_SwitchIpduTxMode(Com_TxPduInfoIterType ComTxPduId, boolean Mode);

/**********************************************************************************************************************
  Com_TxModeHdlr_IsCurrentTxModeTrue
**********************************************************************************************************************/
/*! \brief        This method checks if the TxMode of the referenced IPdu is the TxMode TRUE.
    \details      -
    \pre          -
    \param[in]    TxPduId  ID of Tx I-PDU.
    \return       TRUE    the current TxMode is TxMode TRUE
                  FALSE   the current TxMode is TxMode FALSE
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxModeHdlr_IsCurrentTxModeTrue(Com_TxPduInfoIterType TxPduId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXMODEHDLR_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxModeHdlr.h
**********************************************************************************************************************/
