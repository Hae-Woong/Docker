Safety Manual CanTp
###################

Assumed safety requirements
***************************

This component does not assume safety requirements. 

This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-2119

    The user of MICROSAR Safe shall configure the following:

    - Set /MICROSAR/CanTp/CanTpGeneral/CanTpOptimizeSingleRxBuffer to FALSE.
    - At least one /MICROSAR/CanTp/CanTpConfig/CanTpChannel/CanTpRxNSdu container
    - At least one /MICROSAR/CanTp/CanTpConfig/CanTpChannel/CanTpTxNSdu container
    
    These settings are enforced by an MSSV plugin.


Additional verification measures
********************************

.. smi:: SMI-746515

    The user of MICROSAR Safe shall ensure that any function configured under /MICROSAR/CanTp/CanTpConfig/CanTpChannel/CanTpRxNSdu/CanTpCustomAddressingCallout satisfies the following points:

    - ``ResMetadataOfNSduOutPtr->SduLength`` shall not be modified. 
    - Exactly ``ResMetadataOfNSduOutPtr->SduLength`` bytes shall be copied to ``ResMetadataOfNSduOutPtr->SduDataPtr``. 
    - Exactly ``NPduMetadataPtr->SduLength`` bytes shall be copied to ``ResMetadataOfFcNPduOutPtr``. 
    - ``ResPayloadOfFcNPduInOutPtr`` shall not be used.


.. smi:: SMI-750186

    The user of MICROSAR Safe shall ensure that any function configured under /MICROSAR/CanTp/CanTpConfig/CanTpChannel/CanTpTxNSdu/CanTpCustomAddressingCallout satisfies the following points:

    - ``ResMetadataOfDataNPduOutPtr->SduLength`` shall not be modified. 
    - Exactly ``ResMetadataOfDataNPduOutPtr->SduLength`` bytes shall be copied to ``ResMetadataOfDataNPduOutPtr->SduDataPtr``. 
    - Exactly ``ResMetadataOfDataNPduOutPtr->SduLength`` bytes shall be copied to ``ResMetaDataOfFcNPduOutPtr``. 
    - ``ResPayloadOfDataNPduInOutPtr`` shall not be used.
    - ``ResPayloadOfFcNPduOutPtr`` shall not be used.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

