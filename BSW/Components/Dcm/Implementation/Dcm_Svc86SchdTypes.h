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
/**        \file  Dcm_Svc86SchdTypes.h
 *         \unit  Svc01
 *        \brief  Contains native types and forward declarations of Service 0x86 Scheduler unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
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
#if !defined(DCM_SVC86SCHDTYPES_H)
# define DCM_SVC86SCHDTYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Types.h"
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
 /*! Enable the RoE task */
#  define DCM_TSK_NUM_ROE                                            1u
/*! Configures the RoE task as a one with a single activation within a single Dcm_MainFunction(Worker) activation since no chaining of complex activities is used */
#  define DCM_TSK_ATR_ROE_EXEC                                       DCM_TSK_ATR_EXEC_ONCE

/*! Events of Dcm_Svc86Task() */
#  define DCM_TSK_EV_ROE_SCHEDULER_RATE_ELAPSED                      ((Dcm_TskTaskEvMemType)0x001u) /*!< RoE scheduler rate elapsed */
#  define DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_DETECTED            ((Dcm_TskTaskEvMemType)0x002u) /*!< Relevant change in status of DTC for subfunction 0x01 detected */
#  define DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_SEND_STRT           ((Dcm_TskTaskEvMemType)0x004u) /*!< Trigger STRT for subfunction 0x01 */
#  define DCM_TSK_EV_ROE_86_03_DID_SAMPLE_AND_COMPARE                ((Dcm_TskTaskEvMemType)0x008u) /*!< Read the setup DID and compare its data with the stored reference for subfunction 0x03 */
#  define DCM_TSK_EV_ROE_86_03_DID_SEND_STRT                         ((Dcm_TskTaskEvMemType)0x010u) /*!< Trigger STRT for subfunction 0x03 */
#  define DCM_TSK_EV_ROE_86_07_DID_SAMPLE_AND_COMPARE                ((Dcm_TskTaskEvMemType)0x020u) /*!< Read the setup DID and compare its data with the stored reference for subfunction 0x07 */
#  define DCM_TSK_EV_ROE_86_07_DID_SEND_STRT                         ((Dcm_TskTaskEvMemType)0x040u) /*!< Trigger STRT for subfunction 0x07 */
#  define DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_DETECTED            ((Dcm_TskTaskEvMemType)0x080u) /*!< Relevant change in status of DTC for subfunction 0x09 detected */
#  define DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_BUFFER_FULL         ((Dcm_TskTaskEvMemType)0x100u) /*!< DTC ring buffer for subfunction 0x09 overflowed */
#  define DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_STRT           ((Dcm_TskTaskEvMemType)0x200u) /*!< Trigger STRT for subfunction 0x09 */
#  define DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_INTM_RESP      ((Dcm_TskTaskEvMemType)0x400u) /*!< Trigger the transmission of intermediate response for subfunction 0x09 */
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC86SCHDTYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc86SchdTypes.h
 *********************************************************************************************************************/
