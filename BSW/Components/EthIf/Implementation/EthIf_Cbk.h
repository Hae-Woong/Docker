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
/*!        \file  EthIf_Cbk.h
 *        \brief  Ethernet Interface callback header file
 *
 *      \details  Provides access to the public callback API of EthIf by including the respective public callback
 *                header EthIf_<Sub>_Cbk.h of each EthIf sub-module.
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

#if !defined (ETHIF_CBK_H)
# define ETHIF_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1977
 *********************************************************************************************************/
# include "EthIf_Types.h"
/* Includes of EthIf_<Sub>_Cbk.h according to \trace DSGN-EthIfDiag1977 */
# include "EthIf_EthCtrl_Cbk.h"
# include "EthIf_EthSwt_Cbk.h"
# include "EthIf_EthTrcv_Cbk.h"
# include "EthIf_Gw_Cbk.h"
# include "EthIf_Link_Cbk.h"
# include "EthIf_MacSec_Cbk.h"
# include "EthIf_Mirror_Cbk.h"
# include "EthIf_Mode_Cbk.h"
# include "EthIf_Rx_Cbk.h"
# include "EthIf_Stats_Cbk.h"
# include "EthIf_Tx_Cbk.h"
# include "EthIf_Utils_Cbk.h"
# include "EthIf_ZeroCopy_Cbk.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHIF_START_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define ETHIF_STOP_SEC_CODE
# include "EthIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHIF_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Cbk.h
 *********************************************************************************************************************/

