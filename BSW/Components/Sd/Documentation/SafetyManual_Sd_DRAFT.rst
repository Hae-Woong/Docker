Safety Manual Sd
################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory  up to ASIL B.

Configuration constraints
*************************

.. smi:: SMI-1132494

    The user of MICROSAR Safe shall configure the following parameters:
    
    - Do not reference more than one unique /MICROSAR/EcuC/EcucPartitionCollection/EcucPartition by all /MICROSAR/Sd/SdConfig/SdInstance/SdPartitionRef.
    
    This setting is enforced by an MSSV plugin.


Additional verification measures
********************************

This component does not require additional verification measures.

Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

