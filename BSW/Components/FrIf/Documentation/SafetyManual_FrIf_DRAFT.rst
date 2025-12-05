Safety Manual FrIf
##################

Assumed safety requirements
***************************

.. smi:: SMI-940124

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Linked TSR
         - ASIL 

       * - CREQ-1222
         - Frif shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-105565
         - FrIf shall provide a service to retrieve the value of time-related configuration parameters.
         - TSR-111109
         - ASIL D

       * - CREQ-105570
         - FrIf shall abstract the services related to the controller status.
         - TSR-111109
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory up to ASIL D.


Configuration constraints
*************************

.. smi:: SMI-1103

    The user of MICROSAR Safe shall configure the following attributes:

    - Set /MICROSAR/FrIf/FrIfGeneral/FrIf3rdPartyDriverSupport to FALSE.
    - Set /MICROSAR/FrIf/FrIfGeneral/FrIfWrapperAPIsAsMacro to FALSE.
    - Just a single /MICROSAR/FrIf/FrIfConfig/FrIfCluster container exists.
    
    These settings are enforced by a MSSV plugin.


Additional verification measures
********************************

.. smi:: SMI-1102

    The user of MICROSAR Safe shall verify that the function referenced by define ``FrIf_RxVotingFunction`` returns ``E_NOT_OK`` if all the reduntant Rx PDUs are invalid. A PDU is invalid if ``PduInfo->SduDataPtr`` is ``NULL_PTR`` or ``PduInfo->SduLength`` is ``0``.
    
    This measure applies only if define ``FRIF_DUALCHANNELREDUNDANCYSUPPORT`` is ``STD_ON``. 
    
    Both define ``FrIf_RxVotingFunction`` and ``FRIF_DUALCHANNELREDUNDANCYSUPPORT`` can be found in ``FrIf_Cfg.h``.
    
    The values of the defines are derived from ``/MICROSAR/FrIf/FrIfGeneral/FrIfRxVotingFunction`` and ``/MICROSAR/FrIf/FrIfGeneral/FrIfDualChannelRedundancySupport``.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.
For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

