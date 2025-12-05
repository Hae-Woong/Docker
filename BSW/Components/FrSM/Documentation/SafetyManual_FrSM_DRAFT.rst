Safety Manual FrSM
##################

Safety requirements
*******************

.. smi:: SMI-549176

    This component provides the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 30
    
       * - ID
         - Safety Requirement
         - Linked TSR 

       * - CREQ-105677
         - FrSM shall provide a service to initialize itself.
         - TSR-1

       * - CREQ-105678
         - FrSM shall provide a service to (de)activate the communication
         - TSR-100551

       * - CREQ-105680
         - FrSM shall provide a service to toggle the passive mode.
         - TSR-100551

       * - CREQ-105684
         - FrSM shall provide a service for FlexRay Slot Mode.
         - TSR-100551


Configuration constraints
*************************

The functionality "Allow Wake Up Attempts On Both Channels" is in BETA state, see ESCAN00083894.

Additional verification measures
********************************

.. smi:: SMI-332

    The user of MICROSAR Safe shall verify that only existing functions with the correct prototype are referred by the following function pointer  if  the attribute */MICROSAR/FrSM/FrSMGeneral/FrSMSyncLossErrorIndicationName* is configured to an non-empty value  

    - FrSMSyncLossErrorIndicationFctPtr
     
    The function pointers shall especially not contain numeric values of memory addresses.
    
    All function pointers can be found in *FrSM_Lcfg.c*. 
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

.. smi:: SMI-550720

    This component requires mode switching functionality as an assumed safety requirement (TSR-100551) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-100551 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

