Safety Manual DiagXf
####################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory and timing with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-105504

    **The user of MICROSAR Safe shall execute the RTE Analyzer.**
    
    The RTE Analyzer performs checks to identify faults in DiagXf. Especially out-of-bounds accesses within DiagXf are detected. If RTE Analyzer reports a fault, the generated DiagXf cannot be used. Moreover it provides the user of MICROSAR Safe with feedback what was generated. This feedback shall be reviewed during integration testing against the intended software design and its configuration.
    
    Please see the Technical Reference of the RTE Analyzer how to execute it.


.. smi:: SMI-105505

    **The user of MICROSAR Safe shall ensure that the RTE Analyzer does not report unsupported templates.**
    
    The generated DiagXf code is based on templates. The templates are instantiated by the DiagXf Generator in different variants. The RTE Analyzer verifies that the analyzed template variants have been tested during the development of the DiagXf Generator according to ISO 26262.
    
    The last section of the RTE Analyzer configuration feedback report provides the information about the template variants.
    The report must show that no unsupported templates have been found.


Guided integration testing
==========================

Residual faults in the DiagXf Generator can only be found during integration testing. Vector assumes that the user of MICROSAR Safe performs an integration testing and verification of software safety requirements according to ISO 26262 Part 6 Clauses 10 and 11 (see also SMI-4). To support this integration testing the RTE Analyzer produces a configuration feedback report. 

Please refer to the Technical Reference of the RTE Analyzer for a description of the configuration feedback report.

The following subsections describe the requirements that must be fulfilled during integration testing and verification of software safety requirements.

Each Safety Manual Item (SMI) is structured in the following way:

- The requirement that must be fulfilled
- Explanation of the requirement and a rationale
- Recommended configuration constraints (optional)
- Recommended means of complying with the requirement (optional)
- Details on the information provided by the RTE Analyzer supporting this requirement

.. smi:: SMI-105506

    **The user of MICROSAR Safe shall ensure that DiagXf serializes and deserializes the data in the format that is expected by the DCM.**
    
    Inconsistencies in the diagnostic extract, the configuration of the communication stack, the DiagXf configuration and the RTE configuration can lead to missinterpretation of record data element data.
    
    This requires verification of:

    - data type
    - bit position
    - endianess
    - length
    
    for every record element.
    
    Verification of the serialization can e.g. be performed by integration testing on ECU level with minimum and maximum values for every record element.
    Example:
    For a unsigned 3-bit record element the values 0 and 7 specify the lower and upper limit.
    For a signed 3-bit record element the values -4 and 3 specify the lower and upper limit.
    
    The RTE Analyzer lists the APIs of SWCs that use DiagXf to assist in this integration step.
    The DiagXf API names contain the SWC, port and data element names.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

