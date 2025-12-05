Safety Manual Ea
################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-93

    The user of MICROSAR Safe shall verify that, if NvM disabled polling mode (/NvM/NvMCommon/NvMPollingMode), valid notification routines are provided to Ea via configuration:
    
    - Therefore configuration parameters'/Ea/EaGeneral/EaNvmJobEndNotification' and '/Ea/EaGeneral/EaNvmJobErrorNotification' shall be filled out with respective NvM notification routines.
    - It shall be additionally verified that within file Ea_Cfg.c the configured notifications are assigned to Ea_CbkJobEndNotification and Ea_CbkJobErrorNotification function pointer.  


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

