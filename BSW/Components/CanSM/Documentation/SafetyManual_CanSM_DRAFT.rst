Safety Manual CanSM
###################

Assumed Safety requirements
***************************

.. smi:: SMI-549175

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-363
         - CanSm shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-358
         - CanSm shall provide a service to change the communication mode
         - TSR-100551
         - ASIL D

       * - CREQ-1011
         - CanSm shall provide a service to access its communication mode.
         - TSR-100551
         - ASIL D

       * - CREQ-362
         - CanSm shall provide a service to toggle the passive mode.
         - TSR-100551
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-389

    The user of MICROSAR Safe shall ensure that only existing functions with the correct prototype are referred by the following function pointers.
    
    - The following function pointer is generated only if  the attribute */MICROSAR/CanSM/CanSMGeneral/CanSMBusOffBegin* is configured to an non-empty value:

        - CanSM_BusOffBeginIndicationFctPtr
    
    - The following function pointer is generated only if */MICROSAR/CanSM/CanSMGeneral/CanSMBusOffEnd* is configured to an non-empty value:

        - CanSM_BusOffEndIndicationFctPtr
    
    - The following function pointer is generated only if */MICROSAR/CanSM/CanSMGeneral/CanSMTxTimeoutEnd* is configured to an non-empty value:

        - CanSM_TxTimeoutExceptionEndIndicationFctPtr
    
    The function pointers shall especially not contain NULL pointers nor numeric values of memory addresses.
    
    All function pointers can be found in *CanSM_Lcfg.c*. The function prototypes can be found in *CanSM_Cfg.h*.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

.. smi:: SMI-550719

    This component requires mode switching functionality as an assumed safety requirement (TSR-100551) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-100551 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

