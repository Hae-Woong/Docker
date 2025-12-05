Safety Manual SecOC
###################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-41233

    The user of MICROSAR Safe shall ensure for each entry of table ``SecOC_VerificationStatusCallout`` that the function referred by member ``SecOCVerificationStatusCalloutType`` has the following signature (the placeholder ``<name>`` represents the function's name):
    
    ``void <name>(SecOC_VerificationStatusType verificationStatus)``
    
    The table ``SecOC_VerificationStatusCallout`` can be found in SecOC_Lcfg.c.
    
    This SMI is required to ensure freedom from interference of SecOC.
    Verification can be performed e.g. by review.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

