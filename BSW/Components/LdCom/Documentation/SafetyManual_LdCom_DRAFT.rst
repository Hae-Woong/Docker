Safety Manual LdCom
###################

Assumed safety requirements
***************************

This component assumes the following safety requirements:


.. list-table::
   :header-rows: 1
   :widths: 25 45 20 10

   * - ID
     - Safety Requirement
     - Associated TSR
     - ASIL

   * - CREQ-106197
     - LdCom shall provide a service to initialize itself.
     - TSR-1
     - ASIL D

   * - CREQ-106198
     - LdCom shall provide a service to deinitialize itself.
     - TSR-1
     - ASIL D

   * - CREQ-106200
     - LdCom shall provide a service to forward communication interface PDUs from the Rte to the PduR.
     - TSR-111108
     - ASIL D

   * - CREQ-106201
     - LdCom shall provide a service to forward communication interface PDUs from the PduR to the Rte.
     - TSR-111108
     - ASIL D

   * - CREQ-108860
     - LdCom shall provide a service to forward transport protocol PDUs from the Rte to the PduR.
     - TSR-111108
     - ASIL D

   * - CREQ-108861
     - LdCom shall provide a service to forward transport protocol PDUs from the PduR to the Rte.
     - TSR-111108
     - ASIL D


This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory (and timing) with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

This component does not require additional verification measures.

Safety requirements required from other components
**************************************************

.. smi:: SMI-548477
    
    This component requires sending and receiving functionality as an assumed safety requirement (TSR-111108) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

