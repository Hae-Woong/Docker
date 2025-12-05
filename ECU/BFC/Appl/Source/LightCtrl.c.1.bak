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
 *             File:  LightCtrl.c
 *           Config:  LightControl.dpa
 *        SW-C Type:  LightCtrl
 *
 *        Generator:  MICROSAR RTE Generator Version 4.37.00
 *                    RTE Core Version 4.37.00
 *          License:  PAI11871
 *
 *      Description:  C-Code implementation template for SW-C <LightCtrl>
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of version logging area >>                DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/* PRQA S 0777, 0779 EOF */ /* MD_MSR_Rule5.1, MD_MSR_Rule5.2 */

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of version logging area >>                  DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "Rte_LightCtrl.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#ifdef LEVEL1_vECU
    #include "LightCtrl_SWC.h"
    #include "LightCtrl_SWC_types.h"
#else
    #include "LightCtrl_SWC.h"
    #include "LightCtrl_SWC_types.h"
    #include "Dio.h"
	  #include "EcuM.h"
  #define ECUM_PRIVATE_CFG_INCLUDE
    #include "EcuM_PrivateCfg.h"
  #undef ECUM_PRIVATE_CFG_INCLUDE    
#endif
/* Named constants for Chart: '<S1>/Chart' */
#define LightCtrl_SWC_IN_AutoMode      ((uint8_T)1U)
#define LightCtrl_SWC_IN_EnterAutoMode ((uint8_T)1U)
#define LightCtrl_SWC_IN_LightOff      ((uint8_T)2U)
#define LightCtrl_SWC_IN_LightOn       ((uint8_T)3U)
#define LightCtrl_SWC_IN_LightToOff    ((uint8_T)4U)
#define LightCtrl_SWC_IN_LightToOn     ((uint8_T)5U)
#define LightCtrl_SWC_IN_OffMode       ((uint8_T)2U)
#define LightCtrl_SWC_IN_OnMode        ((uint8_T)3U)
#define LightCtrl_SW_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define test1 false

/* Block states (auto storage) */
DW_LightCtrl_SWC_T LightCtrl_SWC_DW;

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
 * Boolean: Boolean
 * uint16: Integer in interval [0...65535] (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 *
 *********************************************************************************************************************/


#define LightCtrl_START_SEC_CODE
#include "LightCtrl_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: ECU_Reset
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on entering of Mode <HARD> of ModeDeclarationGroupPrototype <DcmEcuReset> of PortPrototype <DcmEcuReset>
 *   - triggered on entering of Mode <JUMPTOBOOTLOADER> of ModeDeclarationGroupPrototype <DcmEcuReset> of PortPrototype <DcmEcuReset>
 *   - triggered on entering of Mode <JUMPTOSYSSUPPLIERBOOTLOADER> of ModeDeclarationGroupPrototype <DcmEcuReset> of PortPrototype <DcmEcuReset>
 *
 **********************************************************************************************************************
 *
 * Mode Interfaces:
 * ================
 *   uint8 Rte_Mode_DcmEcuReset_DcmEcuReset(void)
 *   Modes of Rte_ModeType_DcmEcuReset:
 *   - RTE_MODE_DcmEcuReset_EXECUTE
 *   - RTE_MODE_DcmEcuReset_HARD
 *   - RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER
 *   - RTE_MODE_DcmEcuReset_JUMPTOSYSSUPPLIERBOOTLOADER
 *   - RTE_MODE_DcmEcuReset_KEYONOFF
 *   - RTE_MODE_DcmEcuReset_NONE
 *   - RTE_MODE_DcmEcuReset_SOFT
 *   - RTE_TRANSITION_DcmEcuReset
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: ECU_Reset_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, LightCtrl_CODE) ECU_Reset(void) /* PRQA S 0624, 3206 */ /* MD_Rte_0624, MD_Rte_3206 */
{
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
   * Symbol: ECU_Reset
   *********************************************************************************************************************/
#if !defined (LEVEL1_vECU) && !defined (LEVEL3_vECU)
    uint8 Mode = Rte_Mode_DcmEcuReset_DcmEcuReset();

    switch (Mode)
    {
    case RTE_TRANSITION_DcmEcuReset:
        break;
    case RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER:
        break;
    }
	
#else

	//Nothing for vECU lvl1 and lvl3
	
#endif	
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
   *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: ECU_Reset_Execution
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on entering of Mode <EXECUTE> of ModeDeclarationGroupPrototype <DcmEcuReset> of PortPrototype <DcmEcuReset>
 *
 **********************************************************************************************************************
 *
 * Mode Interfaces:
 * ================
 *   uint8 Rte_Mode_DcmEcuReset_DcmEcuReset(void)
 *   Modes of Rte_ModeType_DcmEcuReset:
 *   - RTE_MODE_DcmEcuReset_EXECUTE
 *   - RTE_MODE_DcmEcuReset_HARD
 *   - RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER
 *   - RTE_MODE_DcmEcuReset_JUMPTOSYSSUPPLIERBOOTLOADER
 *   - RTE_MODE_DcmEcuReset_KEYONOFF
 *   - RTE_MODE_DcmEcuReset_NONE
 *   - RTE_MODE_DcmEcuReset_SOFT
 *   - RTE_TRANSITION_DcmEcuReset
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: ECU_Reset_Execution_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, LightCtrl_CODE) ECU_Reset_Execution(void) /* PRQA S 0624, 3206 */ /* MD_Rte_0624, MD_Rte_3206 */
{
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
   * Symbol: ECU_Reset_Execution
   *********************************************************************************************************************/
#if !defined (LEVEL1_vECU) && !defined (LEVEL3_vECU)
	uint8 Mode = Rte_Mode_DcmEcuReset_DcmEcuReset();
	
	switch (Mode)
    {
    case RTE_TRANSITION_DcmEcuReset:
        EcuM_AL_Reset(EcuMConf_EcuMResetMode_ECUM_RESET_MCU);
        break;
    }
#else

	//Nothing for vECU lvl1 and lvl3
	
#endif
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
   *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: LightCtrl
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on TimingEvent every 10ms
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_Input_LightSwitch(uint8 *data)
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_Output_HeadLight(Boolean data)
 *
 * Client/Server Interfaces:
 * =========================
 *   Server Invocation:
 *   ------------------
 *   Std_ReturnType Rte_Call_LightIntensity_LightIntensity(uint16 *io_signal)
 *     Synchronous Server Invocation. Timeout: None
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: LightCtrl_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, LightCtrl_CODE) LightCtrl(void) /* PRQA S 0624, 3206 */ /* MD_Rte_0624, MD_Rte_3206 */
{
  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
   * Symbol: LightCtrl
   *********************************************************************************************************************/
  /*LÄGG TILL DENNA KOD RAD FÖR ATT ETT TEST SKA FAILA (APPLIKATIONS TEST) LOWERTHRESHOLD = 60, UPPERTHRESHOLD=70*/
  //LightCtrl_SWC_P.LightIntensityToOff_Value = 55; 

  LightCtrl_SWC_P.LightIntensityToOff_Value = 60;

  uint8_T Input_LightSwitch_local_storage;
  uint16_T rtb_LightIntensity_LightIntensi;

  /* Inport: '<Root>/Input_LightSwitch' incorporates:
   *  Inport: '<Root>/Input_LightSwitch'
   */
  Rte_Read_Input_LightSwitch(&Input_LightSwitch_local_storage);

  /* FunctionCaller: '<Root>/LightIntensity_LightIntensity' */
  Rte_Call_LightIntensity_LightIntensity(&rtb_LightIntensity_LightIntensi);


  /* Outputs for Atomic SubSystem: '<Root>/BC' */
  /* Chart: '<S1>/Chart' incorporates:
   *  Constant: '<S1>/CycleTime'
   *  Constant: '<S1>/HysteresisTimeToOff'
   *  Constant: '<S1>/HysteresisTimeToOn'
   *  Constant: '<S1>/LightIntensityToOff'
   *  Constant: '<S1>/LightIntensityToOn'
   */
  /* Gateway: BC/Chart */
  /* During: BC/Chart */

  if (LightCtrl_SWC_DW.is_active_c1_LightCtrl_SWC == 0U) {
    /* Entry: BC/Chart */
    LightCtrl_SWC_DW.is_active_c1_LightCtrl_SWC = 1U;

    /* Entry Internal: BC/Chart */
    /* Transition: '<S2>:37' */
    LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_OffMode;
  } else {
    switch (LightCtrl_SWC_DW.is_c1_LightCtrl_SWC) {
     case LightCtrl_SWC_IN_AutoMode:
      /* During 'AutoMode': '<S2>:34' */
      if (Input_LightSwitch_local_storage == 1) {
        /* Transition: '<S2>:39' */
        /* Exit Internal 'AutoMode': '<S2>:34' */
        LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SW_IN_NO_ACTIVE_CHILD;
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_OnMode;
      } else if (Input_LightSwitch_local_storage == 0) {
        /* Transition: '<S2>:41' */
        /* Exit Internal 'AutoMode': '<S2>:34' */
        LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SW_IN_NO_ACTIVE_CHILD;
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_OffMode;
      } else {
        switch (LightCtrl_SWC_DW.is_AutoMode) {
         case LightCtrl_SWC_IN_EnterAutoMode:
          /* During 'EnterAutoMode': '<S2>:30' */
          if (rtb_LightIntensity_LightIntensi >
              LightCtrl_SWC_P.LightIntensityToOff_Value) {
            /* Transition: '<S2>:32' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOff;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_LOW);
            #endif
          } else {
            /* Transition: '<S2>:33' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOn;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_HIGH);
            #endif
          }
          break;

         case LightCtrl_SWC_IN_LightOff:
          /* During 'LightOff': '<S2>:2' */
          if (rtb_LightIntensity_LightIntensi <
              LightCtrl_SWC_P.LightIntensityToOn_Value) {
            /* Transition: '<S2>:19' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightToOn;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_HIGH);
            #endif
          } else {
            /* Outport: '<Root>/Output_HeadLight' */
            /* Transition: '<S2>:29' */
            Rte_Write_Output_HeadLight(false);
            LightCtrl_SWC_DW.HysteresisTimer = 0.0;
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOff;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_LOW);
            #endif
          }
          break;

         case LightCtrl_SWC_IN_LightOn:
          /* During 'LightOn': '<S2>:1' */
          if (rtb_LightIntensity_LightIntensi >
              LightCtrl_SWC_P.LightIntensityToOff_Value) {
            /* Transition: '<S2>:20' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightToOff;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_LOW);
            #endif
          } else {
            /* Outport: '<Root>/Output_HeadLight' */
            /* Transition: '<S2>:28' */
            Rte_Write_Output_HeadLight(true);
            LightCtrl_SWC_DW.HysteresisTimer = 0.0;
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOn;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_HIGH);
            #endif
          }
          break;

         case LightCtrl_SWC_IN_LightToOff:
          /* During 'LightToOff': '<S2>:3' */
          if (rtb_LightIntensity_LightIntensi <=
              LightCtrl_SWC_P.LightIntensityToOff_Value) {
            /* Transition: '<S2>:23' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOn;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_HIGH);
            #endif
          } else if (LightCtrl_SWC_DW.HysteresisTimer >=
                     LightCtrl_SWC_P.HysteresisTimeToOff_Value) {
            /* Transition: '<S2>:6' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOff;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_LOW);
            #endif
          } else {
            /* Transition: '<S2>:25' */
            LightCtrl_SWC_DW.HysteresisTimer += LightCtrl_SWC_P.CycleTime_Value;
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightToOff;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_LOW);
            #endif
          }
          break;

         default:
          /* During 'LightToOn': '<S2>:4' */
          if (rtb_LightIntensity_LightIntensi >=
              LightCtrl_SWC_P.LightIntensityToOn_Value) {
            /* Transition: '<S2>:22' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOff;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_LOW);
            #endif      
          } else if (LightCtrl_SWC_DW.HysteresisTimer >=
                     LightCtrl_SWC_P.HysteresisTimeToOn_Value) {
            /* Transition: '<S2>:18' */
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightOn;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_HIGH);
            #endif
          } else {
            /* Transition: '<S2>:24' */
            LightCtrl_SWC_DW.HysteresisTimer += LightCtrl_SWC_P.CycleTime_Value;
            LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_LightToOn;
            #ifndef LEVEL1_vECU
			Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_HIGH);
            #endif
          }
          break;
        }
      }
      break;

     case LightCtrl_SWC_IN_OffMode:
      /* During 'OffMode': '<S2>:35' */
      if (Input_LightSwitch_local_storage == 2) {
        /* Transition: '<S2>:38' */
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_AutoMode;

        /* Entry Internal 'AutoMode': '<S2>:34' */
        /* Transition: '<S2>:31' */
        LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_EnterAutoMode;
      } else if (Input_LightSwitch_local_storage == 1) {
        /* Transition: '<S2>:45' */
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_OnMode;
      } else {
        /* Outport: '<Root>/Output_HeadLight' */
        /* Transition: '<S2>:43' */
        Rte_Write_Output_HeadLight(false);
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_OffMode;
        #ifndef LEVEL1_vECU
		Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_LOW);
        #endif
      }
      break;

     default:
      /* During 'OnMode': '<S2>:36' */
      if (Input_LightSwitch_local_storage == 0) {
        /* Transition: '<S2>:40' */
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_OffMode;
      } else if (Input_LightSwitch_local_storage == 2) {
        /* Transition: '<S2>:42' */
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_AutoMode;

        /* Entry Internal 'AutoMode': '<S2>:34' */
        /* Transition: '<S2>:31' */
        LightCtrl_SWC_DW.is_AutoMode = LightCtrl_SWC_IN_EnterAutoMode;
      } else {
        /* Outport: '<Root>/Output_HeadLight' */
        /* Transition: '<S2>:44' */
        Rte_Write_Output_HeadLight(true);
        LightCtrl_SWC_DW.is_c1_LightCtrl_SWC = LightCtrl_SWC_IN_OnMode;
        #ifndef LEVEL1_vECU
		Dio_WriteChannel(DioConf_DioChannel_DioChannel_Output_HeadLight, STD_HIGH);
        #endif
      }
      break;
    }
  }


  /**********************************************************************************************************************
   * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
   *********************************************************************************************************************/
}


#define LightCtrl_STOP_SEC_CODE
#include "LightCtrl_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


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
