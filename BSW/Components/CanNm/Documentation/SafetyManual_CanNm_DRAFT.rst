Safety Manual CanNm
###################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-326

    The user of MICROSAR Safe shall ensure that the pointer (*nmUserDataPtr*) passed to the function *CanNm_GetUserData* references a valid memory location and that the size of the array referenced by parameter *nmUserDataPtr* is greater or equal to *CanNm_GetRxMessageData_UserDataLengthOfPbChannelConfig(channel)*.
    
    This function is called by the application via the *Nm_GetUserData* function of the Nm. This interface only passes a pointer without a length. The length is statically configured in the CanNm for each channel.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-327

    The user of MICROSAR Safe shall ensure that the pointer (*nmPduDataPtr*) passed to the function *CanNm_GetPduData* references a valid memory location and that the size of the array referenced by parameter *nmPduDataPtr* is greater or equal to *CanNm_GetRxMessageDataLengthOfPbChannelConfig(channel)*.
    
    This function is called by the application via the *Nm_GetPduData* function of the Nm. This interface only passes a pointer without a length. The length is statically configured in the CanNm for each channel.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

