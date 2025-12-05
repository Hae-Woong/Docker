Safety Manual IpduM
###################

Assumed safety requirements
***************************


.. smi:: SMI-IpduM-Affected-TSRs
   
    This component assumes the following safety requirements:

    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10

       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL

       * - CREQ-IpduM-Initialization
         - IpduM shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-IpduM-MuxPduTransmission 
         - The IpduM shall provide a service to transmit a multiplexed Pdu. 
         - TSR-111108
         - ASIL D

       * - CREQ-IpduM-MuxPduReception 
         - The IpduM shall provide a service to receive a multiplexed Pdu. 
         - TSR-111108
         - ASIL D

       * - CREQ-IpduM-ContainerTransmission 
         - The IpduM shall provide a service to transmit Pdus inside a container Pdu. 
         - TSR-111108
         - ASIL D

       * - CREQ-IpduM-ContainerReception 
         - The IpduM shall provide a service to receive a container Pdu. 
         - TSR-111108
         - ASIL D


    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory (and timing) with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-92365

    The user of MICROSAR Safe shall ensure that the API IpduM_CalculateSizeOfContainer may only be called by Microsar SecOC.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

.. smi:: SMI-IpduM-Send-Receive

    This component requires sending and receiving functionality as an assumed safety requirement (TSR-111108) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

