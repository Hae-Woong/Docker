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
/*!        \file  Com_IPduGroupHdlr.h
 *         \unit  IPduGroupHdlr
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_IPDUGROUPHDLR_H)
# define COM_IPDUGROUPHDLR_H

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
  Com_IPduGroupHdlr_DisableReceptionDM
**********************************************************************************************************************/
/*! \brief        Disables the reception deadline monitoring for the I-PDUs within the given I-PDU group.
    \details      -
    \pre          -
    \param[in]    ipduGroupId ID of I-PDU group where reception DM shall be disabled.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ipduGroupId < Com_GetSizeOfIPduGroupInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DisableReceptionDM(Com_IPduGroupInfoIterType ipduGroupId);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_EnableReceptionDM
**********************************************************************************************************************/
/*! \brief        Enables the reception deadline monitoring for the I-PDUs within the given I-PDU group.
    \details      -
    \pre          -
    \param[in]    ipduGroupId ID of I-PDU Group where reception DM shall be enabled.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ipduGroupId < Com_GetSizeOfIPduGroupInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_EnableReceptionDM(Com_IPduGroupInfoIterType ipduGroupId);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStop
**********************************************************************************************************************/
/*! \brief        Stops a preconfigured IpduGroup.
    \details      -
    \pre          -
    \param[in]    ipduGroupId ID of I-PDU Group to be stopped
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ipduGroupId < Com_GetSizeOfIPduGroupInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStop(Com_IPduGroupInfoIterType ipduGroupId);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStart
**********************************************************************************************************************/
/*! \brief        Starts a preconfigured IpduGroup.
    \details      -
    \pre          -
    \param[in]    ipduGroupId ID of I-PDU Group to be stopped
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ipduGroupId < Com_GetSizeOfIPduGroupInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStart(Com_IPduGroupInfoIterType ipduGroupId, boolean initialize);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IPduGroupsInit
**********************************************************************************************************************/
/*! \brief        This function iterates over all ComIPduGroups and sets them to INACTIVE
    \details      -
    \pre          -
    \trace        SPEC-2736788
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IPduGroupsInit(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupsDeInit
**********************************************************************************************************************/
/*! \brief        This function deinitializes all IpduGroups. It deactivates all IPdus and IPduGroups.
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupsDeInit(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! \brief        This function activates Rx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateAlwaysOnRxComIPdus(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! \brief        This function activates Tx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateAlwaysOnTxComIPdus(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IsRxPduActive
**********************************************************************************************************************/
/*! \brief        This function returns the active state of the Rx I-PDU.
    \details      -
    \pre          -
    \param[in]    idxRxPduInfo    Rx I-PDU Id
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_IPduGroupHdlr_IsRxPduActive(Com_RxPduInfoIterType idxRxPduInfo);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IsTxPduActive
**********************************************************************************************************************/
/*! \brief        This function returns the active state of the Tx I-PDU.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo    Tx I-PDU Id
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_IPduGroupHdlr_IsTxPduActive(Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateRxPduActive
**********************************************************************************************************************/
/*! \brief        This function deactivates the active state of the Rx I-PDU.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo    Tx I-PDU Id
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxPduInfo < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateRxPduActive(Com_RxPduInfoIterType idxRxPduInfo);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateTxPduActive
**********************************************************************************************************************/
/*! \brief        This function deactivates the active state of the Tx I-PDU.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo    Tx I-PDU Id
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxPduInfo < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateTxPduActive(Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_InitRxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a rx I-PDU.
    \details      -
    \pre          -
    \param[in]    rxPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId <  Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_InitRxIpdu(Com_RxPduInfoIterType rxPduId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_IPDUGROUPHDLR_H */
/**********************************************************************************************************************
  END OF FILE: Com_IPduGroupHdlr.h
**********************************************************************************************************************/
