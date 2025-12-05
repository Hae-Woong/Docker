Safety Manual LinIf and LinTp
#############################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory and timing with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-148

    The user of MICROSAR Safe shall ensure that in the configured array *LinIf_FrameList* each *Length* attribute shall be between 1 (inclusive) and 8 (inclusive).
    The array can be found in *LinIf_Lcfg.c* or *LinIf_PBcfg.c*.
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-388

    The user of MICROSAR Safe shall ensure that only existing functions with the correct prototype are listed in the following arrays:
    
    - The following arrays are not generated if PduR is configured as the only upper layer for all PDUs.

      - LinIf_TxTriggerFctList
      - LinIf_TxConfFctList
      - LinIf_RxIndFctList

    - The following arrays are not generated if LinSm is configured as the only upper layer for all PDUs.

      - LinIf_GotoSleepConfFctList 
      - LinIf_WakeupConfFctList 
      - LinIf_ScheduleReqConfFctList 

    - The following arrays are only generated if transceiver handling is enabled.

      - LinIf_LinTrcv_CheckWakeupFct
      - LinIf_LinTrcv_GetBusWuReasonFct
      - LinIf_LinTrcv_GetOpModeFct
      - LinIf_LinTrcv_SetOpModeFct
      - LinIf_LinTrcv_SetWakeupModeFct

    - The following arrays are only generated if multiple Lin drivers are used.

      - LinIf_LinInstFctTable
    
    The lists shall especially not contain NULL pointers nor numeric values of memory addresses.
    All arrays can be found in *LinIf_Lcfg.c*.
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

