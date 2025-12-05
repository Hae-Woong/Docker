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
/*!        \file  EthIf_Mode_Int.h
 *        \brief  EthIf Mode internal header file
 *
 *      \details  Provides access to types/functions/data of the sub-module Mode of EthIf only to be used internally,
 *                where functions are intended to be linked by linker.
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

/* PRQA S 0883 EOF */ /* MD_EthIf_0883 */

#if !defined (ETHIF_MODE_INT_H)
# define ETHIF_MODE_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1975
 *********************************************************************************************************/
# include "EthIf_Mode_Types_Int.h"

# include "EthIf.h"      /* access to DET error IDs */
# include "EthIf_Int.h"  /* access to components/utils shared with all EthIf modules */

/*********************************************************************************************************
 * Predefined file includes based on usage of other components/sub-modules within this sub-module
 * defined in \trace DSGN-EthIfDiag1937
 *********************************************************************************************************/
/* Dependency to Eth resolved through EthIf_Lcfg.h -> no include needed */
/* Dependency to EthTrcv resolved through EthIf_Lcfg.h -> no include needed */
/* Dependency to EthSwt resolved through EthIf_Lcfg.h -> no include needed */
# include "EthIf_Stats_Int.h"
# include "EthIf_Link_Int.h"
# include "EthIf_Gw_Int.h"

/*********************************************************************************************************
 * Predefined file includes based on usage of this sub-module by other components/sub-modules
 * defined in \trace DSGN-EthIfDiag1937
 *********************************************************************************************************/
# include "EthSM_Types.h"
# if( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
#  include "BswM_Cfg.h"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# if !defined (ETHIF_MODE_INT_LOCAL)                                                                                    /* COV_ETHIF_LOCAL_INLINE */
#  define ETHIF_MODE_INT_LOCAL                                        static
# endif

# if !defined (ETHIF_MODE_INT_LOCAL_INLINE)                                                                             /* COV_ETHIF_LOCAL_INLINE */
#  define ETHIF_MODE_INT_LOCAL_INLINE                                 LOCAL_INLINE
# endif

/*! Value defines the mode count that eventually leads to a mode DOWN request to the respective driver
 *  when a DOWN request is processed for an related EthIf controller. */
# define ETHIF_DRV_DOWN_REQUEST_MODE_CNT                              (1u)

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

# define ETHIF_START_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthIf_Mode_Init
 *********************************************************************************************************************/
/*! \brief          Initializes the EthIf Mode sub-module
 *  \details        -
 *  \pre            Configuration pointer must have been initialized
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Mode_Init( void );

# if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_AdjustEthIfSwtPortGroupModeCount
 *********************************************************************************************************************/
/*! \brief          Increments or decrements the Port Group Ports mode counter values
 *  \details        This functions adjusts the Port group mode counters after a request to ACTIVATE or DEACTIVATE the
 *                  switch port group is served.
 *  \param[in]      portGroupIdx  EthIf switch port group index
 *                                [range: portGroupIdx < EthIf_GetSizeOfPortGroup()]
 *  \param[in]      targetMode    Increment or decrement the Port Group Ports Mode counter values
 *                                ETHTRCV_MODE_DOWN - decrement the Port Group Ports Mode counter values
 *                                ETHTRCV_MODE_ACTIVE - increments the Port Group Ports Mode counter values
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHIF_MODE_INLINE_CODE) EthIf_AdjustEthIfSwtPortGroupModeCount(
  EthIf_PortGroupIterType  portGroupIdx,
  EthTrcv_ModeType         targetMode);

/**********************************************************************************************************************
 *  EthIf_SetEthIfSwtPortGroupMode
 *********************************************************************************************************************/
/*! \brief          Sets the Port Group Ports to ACTIVE or DOWN mode
 *  \details        This functions sets the hardware elements mapped to the EthIf controller to state ACTIVE by utilizing
 *                  the respective driver.
 *  \param[in]      portGroupIdx  EthIf switch port group index
 *                                [range: portGroupIdx < EthIf_GetSizeOfPortGroup()]
 *  \param[in]      targetMode    Mode to set for the EthIf switch port group
 *                                ETHTRCV_MODE_DOWN - disable the port group
 *                                ETHTRCV_MODE_ACTIVE - enable the port group
 *  \return         E_OK - success
 *  \return         E_NOT_OK - mode adaption wasn't possible
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_MODE_INLINE_CODE) EthIf_SetEthIfSwtPortGroupMode(
  EthIf_PortGroupIterType  portGroupIdx,
  EthTrcv_ModeType         targetMode);

/**********************************************************************************************************************
 *  EthIf_DeactivateAllPortGroups
 *********************************************************************************************************************/
/*! \brief          Set all switch port groups to DOWN
 *  \details        This function deactivates all switch port groups
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, ETHIF_MODE_INLINE_CODE) EthIf_DeactivateAllPortGroups( void );
# endif /* ETHIF_ETHSWT_USED */

# define ETHIF_STOP_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* ETHIF_MODE_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Mode_Int.h
 *********************************************************************************************************************/

