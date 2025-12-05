/**********************************************************************************************************************
 *  FILE REQUIRES USER MODIFICATIONS
 *  Template Scope: sections marked with Start and End comments
 *  -------------------------------------------------------------------------------------------------------------------
 *  This file includes template code that must be completed and/or adapted during BSW integration.
 *  The template code is incomplete and only intended for providing a signature and an empty implementation.
 *  It is neither intended nor qualified for use in series production without applying suitable quality measures.
 *  The template code must be completed as described in the instructions given within this file and/or in the.
 *  Technical Reference.
 *  The completed implementation must be tested with diligent care and must comply with all quality requirements which.
 *  are necessary according to the state of the art before its use.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *             File:  IoHwAb.c
 *           Config:  LightControl.dpa
 *        SW-C Type:  IoHwAb
 *
 *        Generator:  MICROSAR RTE Generator Version 4.37.00
 *                    RTE Core Version 4.37.00
 *          License:  PAI11871
 *
 *      Description:  C-Code implementation template for SW-C <IoHwAb>
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of version logging area >>                DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/* PRQA S 0777, 0779 EOF */ /* MD_MSR_Rule5.1, MD_MSR_Rule5.2 */

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of version logging area >>                  DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * AUTOSAR Modelling Object Descriptions
 *
 **********************************************************************************************************************
 *
 * Runnable Entities:
 * ==================
 * IoHwAb_LightIntensityPort_LightIntensity
 *   Runnable Entity for calling the Operation 'LightIntensity' of the Client/Server Port Prototype 'LightIntensityPort'.
 *
 *********************************************************************************************************************/
#include "Rte_IoHwAb.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#if defined LEVEL3_vECU 
#include "Adc.h"
static Adc_ValueGroupType LightTempBuffer;
static Adc_ValueGroupType LightResultBuffer;
static uint16 LightNormResultBuffer;

#elif defined LEVEL1_vECU
#include "Vtt_CustomVars.h"

static uint64 LightTempBuffer;
static uint64 LightResultBuffer;
static uint16 LightNormResultBuffer;

#else
#include "Adc.h"
static Adc_ValueGroupType LightTempBuffer;
static Adc_ValueGroupType LightResultBuffer;
static uint16 LightNormResultBuffer;
#endif
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of include and declaration area >>          DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * Used AUTOSAR Data Types
 *
 **********************************************************************************************************************
 *
 * Primitive Types:
 * ================
 * uint16: Integer in interval [0...65535] (standard type)
 *
 *********************************************************************************************************************/


#define IoHwAb_START_SEC_CODE
#include "IoHwAb_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: IoHwAb_Init
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed once after the RTE is started
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: IoHwAb_Init_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, IoHwAb_CODE) IoHwAb_Init(void) /* PRQA S 0624, 3206 */ /* MD_Rte_0624, MD_Rte_3206 */
{
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
   * Symbol: IoHwAb_Init
   *********************************************************************************************************************/

#ifdef LEVEL3_vECU
  Adc_SetupResultBuffer(AdcConf_AdcGroup_AdcGroupLight, &LightTempBuffer);
  Adc_StartGroupConversion(AdcConf_AdcGroup_AdcGroupLight);
#elif defined LEVEL1_vECU
/*Nothing for vECU lvl1*/  
#else
  Adc_SetupResultBuffer(AdcConf_AdcGroup_AdcGroupLight, &LightTempBuffer);	
#endif
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
   *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: IoHwAb_IoHwAb_IoHwAbHandler
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on TimingEvent every 10ms
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: IoHwAb_IoHwAb_IoHwAbHandler_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, IoHwAb_CODE) IoHwAb_IoHwAb_IoHwAbHandler(void) /* PRQA S 0624, 3206 */ /* MD_Rte_0624, MD_Rte_3206 */
{
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
   * Symbol: IoHwAb_IoHwAb_IoHwAbHandler
   *********************************************************************************************************************/

#if defined (LEVEL3_vECU)  ||  LEVEL1_vECU
    return;
#else	
    Adc_StartGroupConversion(AdcConf_AdcGroup_AdcGroupLight);
#endif
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
   *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: IoHwAb_LightIntensityPort_LightIntensity
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <LightIntensity> of PortPrototype <LightIntensityPort>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void IoHwAb_LightIntensityPort_LightIntensity(uint16 *io_signal)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: IoHwAb_LightIntensityPort_LightIntensity_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, IoHwAb_CODE) IoHwAb_LightIntensityPort_LightIntensity(P2VAR(uint16, AUTOMATIC, RTE_IOHWAB_APPL_VAR) io_signal) /* PRQA S 0624, 3206 */ /* MD_Rte_0624, MD_Rte_3206 */
{
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
   * Symbol: IoHwAb_LightIntensityPort_LightIntensity
   *********************************************************************************************************************/

#ifdef LEVEL3_vECU
  Adc_ReadGroup(AdcConf_AdcGroup_AdcGroupLight, &LightResultBuffer);
  LightNormResultBuffer = (uint16) LightResultBuffer * 100 / ((1 << 10) - 1);
  *io_signal =  LightNormResultBuffer;
#elif defined LEVEL1_vECU
  LightResultBuffer = ReadLightIntensity();
  LightNormResultBuffer = (uint16) LightResultBuffer * 100 / ((1 << 10) - 1);
  *io_signal =  LightNormResultBuffer;
#else
   Adc_ReadGroup(AdcConf_AdcGroup_AdcGroupLight, &LightResultBuffer);
   LightNormResultBuffer = (uint16)LightResultBuffer * 25 / ((1 << 10) - 1);
   *io_signal = LightNormResultBuffer;
#endif
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
   *********************************************************************************************************************/
}


#define IoHwAb_STOP_SEC_CODE
#include "IoHwAb_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of function definition area >>              DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of removed code area >>                   DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of removed code area >>                     DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 MISRA 2012 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_0624:  MISRA rule: Rule8.3
     Reason:     This MISRA violation is a consequence from the RTE requirements [SWS_Rte_01007] [SWS_Rte_01150].
                 The typedefs are never used in the same context.
     Risk:       No functional risk. Only a cast to uint8* is performed.
     Prevention: Not required.

   MD_Rte_3206:  MISRA rule: Rule2.7
     Reason:     The parameters are not used by the code in all possible code variants.
     Risk:       No functional risk.
     Prevention: Not required.

*/
