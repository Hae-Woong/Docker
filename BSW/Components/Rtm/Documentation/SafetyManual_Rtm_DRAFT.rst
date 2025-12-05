Safety Manual Rtm
#################

Assumed Safety Requirements
***************************

This component does not assume safety requirements.
This component is developed according to ISO 26262 ASIL D only to ensure freedom from interference with respect to memory.

.. smi:: SMI-2811

    This component can be used in the following two variants in an ASIL D context:

    - This component is disabled during normal operation.

      This component shall then only be enabled in an operating mode that does not impose risk for the health of persons.

    - This component supports a subset of its functionality used during normal operation.


Configuration Constraints
*************************

.. smi:: SMI-1242901

    The user of MICROSAR Safe shall configure the following:

    - Set ``/MICROSAR/Rtm/RtmGeneral/RtmControl`` to TRUE.
    
    The user of MICROSAR Safe shall verify that the corresponding define ``RTM_CONTROL`` is generated as ``STD_ON``. The define can be found in ``Rtm_Cfg.h``.
    The user of MICROSAR Safe shall use the variable ``Rtm_ControlState`` to activate and deactivate the Rtm component. This lock must be done after the call to Rtm_InitMemory but before the call to Rtm_Init.
    Activation and deactivation shall only be performed by a software component that is developed according to the highest ASIL that is allocated to the ECU.
    Rtm shall only be activated in an operating mode that does not impose risk for the health of persons. 
    
    Note: Rtm is active after ECU start up and must be disabled manually before the API ``Rtm_Init`` is called.
    
    This SMI is only applicable if Rtm is supposed to be disabled.


.. smi:: SMI-1242902

    The user of MICROSAR Safe shall ensure:

    - That **write** access to the memory section ``RTM_START_SEC_VAR_INIT_UNSPECIFIED_SAFE`` / ``RTM_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE`` is only granted to software components that have the highest ASIL level allocated to the ECU, and
    - That the component Rtm has **read-only** access to this memory section.
    
    This SMI is only applicable if Rtm is supposed to be disabled.


.. smi:: SMI-1242903

    The user of MICROSAR Safe shall configure the following:

    - Set all ``/MICROSAR/Rtm/RtmMeasurementPoint/RtmMeasurementType`` to GrossExecutionTime.
    
    This SMI is only applicable if Rtm is supposed to be used during normal operation.


Additional Verification Measures
********************************

Safe Disable
============

.. smi:: SMI-2808

    The user of MICROSAR Safe shall verify during integration testing that Rtm is disabled during normal operation.
    
    This SMI is only applicable if Rtm is supposed to be disabled.


.. smi:: SMI-2809

    The user of MICROSAR Safe shall verify that the following APIs of Rtm are not called from outside Rtm:

    - *Rtm_StartMeasurementFct*
    - *Rtm_StopMeasurementFct*
    - *Rtm_StartNetMeasurementFct*
    - *Rtm_StopNetMeasurementFct*
    - *Rtm_InitCpuLoadMeasurement*
    
    This SMI is only applicable if Rtm is supposed to be disabled.


.. smi:: SMI-3197

    The user of MICROSAR Safe shall verify that the following  generated macros and functions perform the check for ``Rtm_ControlState`` if Rtm is enabled. 
    
    .. list-table::
       :header-rows: 1
    
       * - Symbol (``<x>`` = placeholder)
         - Found in

       * - ``Rtm_Start_<x>``
         - Rtm_Cfg.h

       * - ``Rtm_Stop_<x>``
         - Rtm_Cfg.h

       * - ``Rtm_MainFunction_<x>``
         - Rtm_Cfg.c
    
    
    The user of MICROSAR Safe shall verify that further code of Rtm is only executed if ``Rtm_ControlState`` == ``RTM_CONTROL_STATE_ENABLED``.
    
    This SMI is only applicable if Rtm is supposed to be disabled.


.. smi:: SMI-3402

    The user of MICROSAR Safe shall verify for all generated functions within ``Rte_Hook_Rtm.c`` that only the functions ``Rtm_Start`` respectively ``Rtm_Stop`` are called.
    
    This SMI is only applicable if Rtm is supposed to be disabled.


Normal Operation
================

.. smi:: SMI-1242904

    The user of MICROSAR Safe shall verify that the following generated macros are correctly generated for each core id if there are more ``/MICROSAR/Rtm/RtmCoreDefinition`` than 1 and the ``/MICROSAR/Rtm/RtmCoreDefinition/RtmNumberOfTaskResponseTimePercentiles`` is configured:
    
    - The value of ``Rtm_CommonConst[<CoreId>].OsTaskInfoLength`` (Rtm_Cfg.c) is equal to ``RTM_NUMBER_OF_TASKS_ON_CORE_<CoreId>`` (Rtm_Cfg.h) of same ``<CoreId>``
    - The arrays referenced by ``Rtm_CommonConst[<CoreId>].TaskResponseTimeDataPtr`` (Rtm_Cfg.c) have the same size as ``RTM_NUMBER_OF_TASKS_ON_CORE_<CoreId>`` (Rtm_Cfg.h) of same ``<CoreId>``
    - ``RTM_NUMBER_OF_TASKS_ON_CORE_<CoreId>`` (Rtm_Cfg.h) is greater than 0
    
    The ``<CoreId>`` is a placeholder for the core id specified in ``/MICROSAR/Rtm/RtmCoreDefinition/RtmCore``.
    
    This SMI is only applicable if Rtm is supposed to be used during normal operation.


Safety Requirements Required From Other Components
**************************************************

This component does not require safety requirements from other components.

Dependencies To Hardware
************************

This component does not use a direct hardware interface.

