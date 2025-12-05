Safety Manual CddDrm
####################

Assumed safety requirements
***************************

This component does not assume safety requirements.
This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

This component does not have any configuration constraints.

Additional verification measures
********************************

The following requirements are necessary to ensure freedom from interference with respect to memory by this component.

.. smi:: SMI-1019002

    The user of MICROSAR Safe shall verify that the function signature of the configured callback ``CddResponseNotification`` function ``CddDrm_Cfg_ResponseNotification`` matches the expected signature.
    
    
    .. list-table::
       :header-rows: 1
    
       * - Callback function
         - Expected signature of the function

       * - ``CddDrm_Cfg_ResponseNotification``
         - ``void <name> (const CddDrm_RespInfoStructType* Response)``


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

