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
/*!        \file  EthIf_Types.h
 *        \brief  Ethernet Interface data type header file
 *
 *      \details  Provides access to the public types of EthIf by including the respective public types header
 *                EthIf_<Sub>_Types.h of each EthIf sub-module.
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

#if !defined (ETHIF_TYPES_H)
# define ETHIF_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*********************************************************************************************************
 * Predefined file includes based on \trace DSGN-EthIfDiag1977
 *********************************************************************************************************/
/* Includes of EthIf_<Sub>_Types.h */
# include "EthIf_EthCtrl_Types.h"
# include "EthIf_EthSwt_Types.h"
# include "EthIf_EthTrcv_Types.h"
# include "EthIf_Gw_Types.h"
# include "EthIf_Link_Types.h"
# include "EthIf_MacSec_Types.h"
# include "EthIf_Mirror_Types.h"
# include "EthIf_Mode_Types.h"
# include "EthIf_Rx_Types.h"
# include "EthIf_Stats_Types.h"
# include "EthIf_Tx_Types.h"
# include "EthIf_Utils_Types.h"
# include "EthIf_ZeroCopy_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#endif /* ETHIF_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Types.h
 *********************************************************************************************************************/

