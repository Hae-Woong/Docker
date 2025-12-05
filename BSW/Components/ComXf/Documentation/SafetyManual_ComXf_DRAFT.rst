Safety Manual ComXf
###################

Assumed safety requirements
***************************

.. smi:: SMI-62930

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-108114
         - ComXf shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-108117
         - ComXf shall provide a service to transform a Sender/Receiver communication.
         - TSR-10,TSR-111108
         - ASIL D

       * - CREQ-108118
         - ComXf shall provide a service to retransform a Sender/Receiver communication.
         - TSR-10,TSR-111108
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

Please note that the ComXf Generator and RTE Analyzer only implement measures to detect systematic faults by the software. No measures are implemented to detect or mitigate random faults on the computer used for generation.

.. smi:: SMI-62929

    **The user of MICROSAR Safe shall execute the RTE Analyzer.**
    
    The RTE Analyzer performs checks to identify faults in ComXf. Especially out-of-bounds accesses within ComXf are detected. If RTE Analyzer reports a fault, the generated ComXf cannot be used. Moreover it provides the user of MICROSAR Safe with feedback what was generated. This feedback shall be reviewed during integration testing against the intended software design and its configuration.
    
    Please see the Technical Reference of the RTE Analyzer how to execute it.


.. smi:: SMI-62931

    **The user of MICROSAR Safe shall ensure that the RTE Analyzer does not report unsupported templates.**
    
    The generated ComXf code is based on templates. The templates are instantiated by the ComXf Generator in different variants. The RTE Analyzer verifies that the analyzed template variants have been tested during the development of the ComXf Generator according to ISO 26262.
    
    The last section of the RTE Analyzer configuration feedback report provides the information about the template variants.
    The report must show that no unsupported templates have been found.


Guided integration testing
==========================

Residual faults in the ComXf Generator can only be found during integration testing. Vector assumes that the user of MICROSAR Safe performs an integration testing and verification of software safety requirements according to ISO 26262 Part 6 Clauses 10 and 11 (see also SMI-4). To support this integration testing the RTE Analyzer produces a configuration feedback report. 

Please refer to the Technical Reference of the RTE Analyzer for a description of the configuration feedback report.

The following subsections describe the requirements that must be fulfilled during integration testing and verification of software safety requirements.

Each Safety Manual Item (SMI) is structured in the following way:

- The requirement that must be fulfilled
- Explanation of the requirement and a rationale
- Recommended configuration constraints (optional)
- Recommended means of complying with the requirement (optional)
- Details on the information provided by the RTE Analyzer supporting this requirement

.. smi:: SMI-62933

    **The user of MICROSAR Safe shall ensure that ComXf serializes and deserializes the data in the format that is expected by the network.**
    
    Inconsistencies in the communication extract, the configuration of the communication stack, the ComXf configuration and the RTE configuration can lead to missinterpretation of signal group data.
    
    This requires verification of:

    - data type
    - bit position
    - endianess
    - length
    
    for every group signal.
    
    Vector requires using E2E protection for safety-related signals.
    
    Verification of the serialization can e.g. be performed by integration testing on vehicle network level with minimum and maximum values for every group signal.
    Example:
    For a unsigned 3-bit group signal the values 0 and 7 specify the lower and upper limit.
    For a signed 3-bit group signal the values -4 and 3 specify the lower and upper limit.
    
    The RTE Analyzer lists the APIs of SWCs that use ComXf to assist in this integration step.
    The ComXf API names contain the signal group names.


Safety requirements required from other components
**************************************************

.. smi:: SMI-857104

    COMXF requires the following functionality as safety requirements from the RTE:

    - Transformer handling (TSR-10)
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

