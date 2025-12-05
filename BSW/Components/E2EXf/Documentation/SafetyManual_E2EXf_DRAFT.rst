Safety Manual E2EXf
###################

Assumed safety requirements
***************************

.. smi:: SMI-62928

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-131007
         - E2EXf shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-131011
         - E2EXf shall provide a service to protect data.
         - TSR-10,TSR-111108
         - ASIL D

       * - CREQ-133278
         - E2EXf shall provide a service to verify protected data.
         - TSR-10,TSR-111108
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

Please note that the E2EXf Generator and RTE Analyzer only implement measures to detect systematic faults by the software. No measures are implemented to detect or mitigate random faults on the computer used for generation.

.. smi:: SMI-62927

    **The user of MICROSAR Safe shall execute the RTE Analyzer.**
    
    The RTE Analyzer performs checks to identify faults in E2EXf. Especially out-of-bounds accesses within E2EXf are detected. If RTE Analyzer reports a fault, the generated E2EXf cannot be used. Moreover it provides the user of MICROSAR Safe with feedback what was generated. This feedback shall be reviewed during integration testing against the intended software design and its configuration.
    
    Please see the Technical Reference of the RTE Analyzer how to execute it.


Guided integration testing
==========================

Residual faults in the E2EXf Generator can only be found during integration testing. Vector assumes that the user of MICROSAR Safe performs an integration testing and verification of software safety requirements according to ISO 26262 Part 6 Clauses 10 and 11 (see also SMI-4). To support this integration testing the RTE Analyzer produces a configuration feedback report. 

Please refer to the Technical Reference of the RTE Analyzer for a description of the configuration feedback report.

The following subsections describe the requirements that must be fulfilled during integration testing and verification of software safety requirements.

Each Safety Manual Item (SMI) is structured in the following way:

- The requirement that must be fulfilled
- Explanation of the requirement and a rationale
- Recommended configuration constraints (optional)
- Recommended means of complying with the requirement (optional)
- Details on the information provided by the RTE Analyzer supporting this requirement

.. smi:: SMI-62932

    **The user of MICROSAR Safe shall ensure that the E2E configuration is as expected.**
    
    This requires verification of:

    - Profile
    - E2E header length
    - Input header length
    - Execute with no data
    - Check incoming data
    - Window size (4.4 State Machine)
    - Window size init (19-11 State Machine)
    - Window size invalid (19-11 State Machine)
    - Window size valid (19-11 State Machine)
    - Clear from valid to invalid  (19-11 State Machine)
    - Transit to invalid extended (19-11 State Machine)
    - State changes
    - Offset of counter
    - Offset of CRC
    - Offset of data ID high byte
    - Offset of E2E header
    - Data ID
    - Data ID nibble mode
    - Data length
    - Initial max. allowed counter gap
    - Max. missing or repeated data
    - Initial counter sync data
    - Max. allowed counter gap
    
    for every E2E protected data element.
    
    Verification can e.g. be performed by review of the configuration feedback report against the communication extract and the E2ELIB specification (e.g. for the E2E header length).
    *Input header length* refers to the length of the payload header e.g. SOMEIPXF header length.
    
    Vector assumes integration testing on vehicle network level using fault-injection is used to additionally verify the effectiveness of the E2E protection.
    
    The RTE Analyzer lists the RTE APIs that use E2E transformation to assist in this integration step.
    The RTE Analyzer lists the E2E configuration to assist in this integration step.
    E2EXF configurations for which the check of incoming data is disabled are not listed.


Safety requirements required from other components
**************************************************

.. smi:: SMI-66294

    E2EXF requires the following functionality as safety requirement from the E2ELIB:

    - data protection
    - initialization of protection state
    - checking of protection state
    - mapping of result state to state machine status
    - checking of data
    - initialization of state machine
    
    This requirement is fulfilled if an ASIL E2ELIB by Vector is used.


.. smi:: SMI-857105

    E2EXF requires the following functionality as safety requirements from the RTE:

    - Transformer handling (TSR-10)
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

