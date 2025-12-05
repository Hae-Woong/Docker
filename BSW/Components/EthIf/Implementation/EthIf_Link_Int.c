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
/*!        \file  EthIf_Link_Int.c
 *        \brief  EthIf Link internal source file
 *
 *      \details  Contains the definition of data and implementation of functions of the sub-module Link of EthIf
 *                only to be used EthIf internal.
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

#define ETHIF_LINK_INT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1975
 *********************************************************************************************************/
#include "EthIf_Link_Int.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_ProcessSwtPortGroupLinkStateChange
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, ETHIF_LINK_INLINE_CODE) EthIf_ProcessSwtPortGroupLinkStateChange( /* PRQA S 3219 */ /* MD_EthIf_3219 */
  EthIf_PortGroupIterType  portGroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter exclusive area (Reason: Ensure consistency of controller mode and link state information) */
  EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Check if a link state change to link active occurred on the EthIf switch port group */
  if( EthIf_GetPortGroupLinkState(portGroupIdx) == ETHIF_PORTGROUP_LINK_STATE_LINK_CHANGE_UP )
  {
    /* #200 Issue a link up for the EthIf switch port group */
    EthIf_SetPortGroupLinkState(portGroupIdx, ETHIF_PORTGROUP_LINK_STATE_LINK);                                         /* SBSW_ETHIF_INT_FCT_IDX_PASS */
    BswM_EthIf_PortGroupLinkStateChg((uint8)portGroupIdx, ETHTRCV_LINK_STATE_ACTIVE);
  }
  /* #30 Check if a link state change to link down occurred on the EthIf switch port group */
  else if( EthIf_GetPortGroupLinkState(portGroupIdx) == ETHIF_PORTGROUP_LINK_STATE_LINK_CHANGE_DOWN )
  {
    /* #300 Issue a link down for the EthIf switch port group */
    EthIf_SetPortGroupLinkState(portGroupIdx, ETHIF_PORTGROUP_LINK_STATE_NO_LINK);                                      /* SBSW_ETHIF_INT_FCT_IDX_PASS */
    BswM_EthIf_PortGroupLinkStateChg((uint8)portGroupIdx, ETHTRCV_LINK_STATE_DOWN);
  }
  else
  {
    /* no link change occurred */
  }

  /* #40 Leave exclusive area -> processing on elements finished */
  EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_ProcessSwtPortGroupLinkStateChange() */
#endif /* ETHIF_ETHSWT_USED */
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthIf_Link_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHIF_CODE) EthIf_Link_Init( void )
{
  EthIf_EthIfCtrlIterType  ethIfCtrlIdx;
#if ( ETHIF_ETHSWT_USED == STD_ON )
  EthIf_PortGroupIterType  portGroupIdx;
#endif /* ETHIF_ETHSWT_USED */

#if ( ETHIF_ENABLE_MAIN_FUNCTION_STATE == STD_OFF )
  /* #10 Set the tx/state main-function decouple frequency to its initial value */
  EthIf_SetMainFuncStateDecoupleFreqCnt(EthIf_GetLinkStateChgMainReload());                                             /* SBSW_ETHIF_COMMON_CFG_PTR_INT_FCT_CALL_CONV */
#endif /* ETHIF_ENABLE_MAIN_FUNCTION_STATE */

  /* #20 Initialize the link state of each EthIf controller */
  for( ethIfCtrlIdx = 0;
       ethIfCtrlIdx < EthIf_GetSizeOfEthIfCtrl();
       ethIfCtrlIdx++ )
  {
    EthIf_SetEthIfCtrlLinkState(ethIfCtrlIdx, ETHIF_ETHIFCTRL_LINK_STATE_NO_LINK);                                      /* SBSW_ETHIF_COMMON_CSL01 */
  }

#if ( ETHIF_ETHSWT_USED == STD_ON )
  /* #30 Initialize the link state of each EthIf switch port group */
  for( portGroupIdx = 0;
       portGroupIdx < EthIf_GetSizeOfPortGroup();
       portGroupIdx++ )
  {
    EthIf_SetPortGroupLinkState(portGroupIdx, ETHIF_PORTGROUP_LINK_STATE_NO_LINK);                                      /* SBSW_ETHIF_COMMON_CSL01 */
  }
#endif /* ETHIF_ETHSWT_USED */
} /* EthIf_Link_Init() */

#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Link_Int.c
 *********************************************************************************************************************/

