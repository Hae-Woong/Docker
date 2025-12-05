Safety Manual Crc
#################

Assumed safety requirements
***************************

.. smi:: SMI-344

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-858 
         - Crc shall provide a service to calculate 8-bit SAE-J1850 CRC. 
         - TSR-10 
         - ASIL D

       * - CREQ-859 
         - Crc shall provide a service to calculate 8-bit 0x2F CRC. 
         - TSR-10 
         - ASIL D

       * - CREQ-860 
         - Crc shall provide a service to calculate 16-bit CCITT CRC. 
         - TSR-10 
         - ASIL D

       * - CREQ-861 
         - Crc shall provide a service to calculate 32-bit IEEE-802.3 CRC. 
         - TSR-10 
         - ASIL D

       * - CREQ-862 
         - Crc shall provide a service to calculate 32-bit E2E Profile 4 CRC. 
         - TSR-10 
         - ASIL D

       * - CREQ-117997 
         - Crc shall provide a service to calculate 64-bit ECMA CRC. 
         - TSR-10 
         - ASIL D
    
    
This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-49

    The user of MICROSAR Safe shall verify that the CRC is calculated for the intended data.
    
    This includes the intended buffer and its size (see also SMI-16), start value and if it is the first call to the service.
    Verification can be performed by the "magic check" (see AUTOSAR SWS Crc).
    
    If Crc is used by a MICROSAR Safe component (e.g. E2E, NvM), this requirement is fulfilled for the MICROSAR Safe component.


Safety requirements required from other components
**************************************************

This component has no safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

