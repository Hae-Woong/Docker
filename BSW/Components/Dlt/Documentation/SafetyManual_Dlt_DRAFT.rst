Safety Manual Dlt
#################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-1122018

    The user of MICROSAR Safe shall configure the following attribute:

    - Set /MICROSAR/Dlt/DltGeneral/DltComLayer to PduR.
    
    This setting is enforced by a MSSV plugin.


Additional verification measures
********************************

This component does not require additional verification measures.

Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.
For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

