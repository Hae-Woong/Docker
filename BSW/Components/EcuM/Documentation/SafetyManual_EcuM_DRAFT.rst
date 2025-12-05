Safety Manual EcuM
##################

Assumed safety requirements
***************************

.. smi:: SMI-1304455

    This component assumes the following safety requirements:


    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10

       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-470 
         - EcuM shall provide a service to initialize the ECU management. 
         - TSR-1 
         - ASIL D

       * - CREQ-389 
         - EcuM shall provide a service to complete the startup phase. 
         - TSR-1 
         - ASIL D

       * - CREQ-543 
         - EcuM shall perform validation of all postbuild configurable BSW module configuration parameters. 
         - TSR-1 
         - ASIL D

       * - CREQ-525 
         - EcuM shall provide a callout to initialize driver prior the start of the OS. 
         - TSR-1 
         - ASIL D

       * - CREQ-488 
         - EcuM shall provide a callout to determine the Postbuild configuration data. 
         - TSR-1 
         - ASIL D

       * - CREQ-505 
         - EcuM shall provide a callout to initialize drivers prior Postbuild data is available. 
         - TSR-1 
         - ASIL D

       * - CREQ-509 
         - EcuM shall provide a callout to re-initialize drivers during a restart. 
         - TSR-1 
         - ASIL D

       * - CREQ-454 
         - EcuM shall provide ECU initialization on partitions. 
         - TSR-1 
         - ASIL D

       * - CREQ-494 
         - EcuM shall handle ECU phases on MultiCore ECUs. 
         - TSR-100551 
         - ASIL D

       * - CREQ-473 
         - EcuM shall provide a callout to synchronize the master and slave cores during shutdown. 
         - TSR-100551 
         - ASIL D

This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-823269

    The user of MICROSAR Safe shall configure the attribute /MICROSAR/EcuM/EcuMFlexGeneral/EcuMEnableDefBehaviour to FALSE.

    This setting is enforced by a MSSV plugin.


.. smi:: SMI-823270

    The user of MICROSAR Safe shall not configure 
    any reference in /MICROSAR/EcuM/EcuMConfiguration/EcuMCommonConfiguration/EcuMWakeupSource/EcuMComMPNCRef to a PNC for a wakeup source.

    This setting is enforced by a MSSV plugin.


Additional verification measures
********************************

.. smi:: SMI-39

    **The user of MICROSAR Safe shall ensure the intended initialization procedure during integration testing.**

    The user of MICROSAR Safe shall ensure the intended initialization procedure by performing the following tests:

    - Start the ECU and verify that the intended initalization routines are called. This needs to be verified for each Postbuild-selectable configuration.
    - Corrupt the Postbuild data and the corresponding CRC in non-volatile memory and start the ECU. Then verify that the corruption is detected by EcuM.
    - Start the ECU and verify that the intended Postbuild-selectable configuration is used by the EcuM. This needs to be verified for each Postbuild-selectable configuration.

    A start of the ECU includes a "cold-start", reset as well as wake-up from sleep if applicable.

    This requirement is only applicable if TSR-1 is assumed as a safety requirement.


Safety requirements required from other components
**************************************************

.. smi:: SMI-42

    This component requires the OS service to get the core ID functionality as an assumed safety requirement (TSR-1) from other interfacing components.

    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.

    This requirement is only applicable if TSR-1 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

