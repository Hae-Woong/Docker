Safety Manual StbM
##################

Assumed safety requirements
***************************

.. smi:: SMI-330196

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Linked TSR
         - ASIL 

       * - CREQ-126295
         - StbM shall provide a service to initialize the StbM module.
         - TSR-1
         - ASIL D

       * - CREQ-126468
         - StbM shall provide the Local Time of each synchronized and pure local Time Base.
         - TSR-1
         - ASIL D

       * - CREQ-139149
         - StbM shall provide a service to retrieve the status of a Time Base.
         - TSR-111109
         - ASIL D

       * - CREQ-126474
         - StbM shall provide a service to update a Time Base.
         - TSR-111109
         - ASIL D

       * - CREQ-126478
         - StbM shall provide a service to set the global time.
         - TSR-111109
         - ASIL D

       * - CREQ-137111
         - StbM shall provide a service to retrieve the last time leap.
         - TSR-111109
         - ASIL D

       * - CREQ-TimeLeapMonitoring
         - StbM shall provide time leap monitoring for each Time Base.
         - TSR-111109
         - ASIL D

       * - CREQ-TimeoutMonitoring
         - StbM shall provide timeout monitoring for each Time Base.
         - TSR-111109
         - ASIL D

       * - CREQ-126470
         - StbM shall provide a service to retrieve the current time in standard format.
         - TSR-111109
         - ASIL D

       * - CREQ-126471
         - StbM shall provide a service to retrieve the current time in extended format.
         - TSR-111109
         - ASIL D

       * - CREQ-234902
         - StbM shall provide a service to retrieve the current time tuple.
         - TSR-111109
         - ASIL D

       * - CREQ-234903
         - StbM shall provide a service to retrieve the current virtual local time.
         - TSR-111109
         - ASIL D

       * - CREQ-126584
         - StbM shall provide the OS as Triggered Customers.
         - TSR-111109
         - ASIL D

       * - CREQ-GetMainTime
         - StbM shall provide a service to retrieve the main time tuple, status and rate deviation.
         - TSR-111109
         - ASIL D

       * - CREQ-127310
         - StbM shall provide simultaneous rate measurements.
         - TSR-111109
         - ASIL D

       * - CREQ-126502
         - StbM shall provide rate correction for Time Bases.
         - TSR-111109
         - ASIL D

       * - CREQ-126479
         - StbM shall provide a service to retrieve the current rate deviation.
         - TSR-111109
         - ASIL D

       * - CREQ-126480
         - StbM shall provide a service to set the rate deviation of time base.
         - TSR-111109
         - ASIL D

       * - CREQ-126503
         - StbM shall provide offset correction for synchronized Time Bases.
         - TSR-111109
         - ASIL D

       * - CREQ-127523
         - The StbM shall provide smooth offset correction for synchronized Time Bases.
         - TSR-111109
         - ASIL D

       * - CREQ-CloneTimeBase
         - StbM shall provide a service to clone Time Base upon user request.
         - TSR-111109
         - ASIL D

       * - CREQ-ProcessCloneRequest
         - StbM shall provide management of Time Base clone requests.
         - TSR-111109
         - ASIL D

       * - CREQ-NotifyForLocalClockModification
         - StbM shall notify the application about status updates for local clock modification.
         - TSR-111109
         - ASIL D

       * - CREQ-HandleLocalClockModification
         - StbM shall provide handling of local clock modification.
         - TSR-111109
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


.. smi:: SMI-299647

    The following functionality of StbM is **not** part of the safe scope: 

    - Usage of user data 
    - Usage of offset timebases (master and slave)
    - Usage of time and status notifications (notification customer).
     
    That means that StbM in context of TSR-111109 can be used with ASIL D when:

    - as master or slave for synchronized  and pure local timebases
    - polling the current time and status (active customer)
    - synchronizing schedule tables (triggered customer).


Configuration constraints
*************************

.. smi:: SMI-327284

    The user of MICROSAR Safe shall enable time leap detection for time slaves. The configured value defines the acceptable imprecision of the time. This is required as detection of wrong times received from the Bus depends on the time leap check.
    
    - Set /MICROSAR/StbM/StbMSynchronizedTimeBase/StbMTimeLeapFutureThreshold and /MICROSAR/StbM/StbMSynchronizedTimeBase/StbMTimeLeapPastThreshold
    - Verify that StbM_TimeLeapConfig table in StbM_Cfg.c or StbM_Cfg_<Partition>.c contains the configured value in nanoseconds in FutureThreshold and PastThreshold for the Timebase. The relations between the rows in StbM_TimeLeapConfig table and the Timebase can be identified with the comment 'Referable Keys', which is the short name of the Timebase.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-327285

    The user of MICROSAR Safe shall enable timeout detection for time slaves. The configured value accounts for the drift of the local clock. This is required as detection of lost synchronization to the global time depends on this value.
    
    - Set /MICROSAR/StbM/StbMSynchronizedTimeBase/StbMSyncLossTimeout
    - Verify that StbM_TimeoutConfig table in StbM_Cfg.c or StbM_Cfg_<Partition>.c contains the configured value divided by /MICROSAR/StbM/StbMGeneral/StbMMainFunctionPeriod rounded down to an integer for the Timebase. The relations between the rows in StbM_TimeoutConfig table and the Timebase can be identified with the comment 'Referable Keys', which is the short name of the Timebase.

    Note: This implies that values which are not an integer multiple of or smaller than a mainfunction period might be detected earlier.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-327943

    The user of MICROSAR Safe shall configure a suitable period of schedule table synchronization to prevent drift if schedule table synchronization is used. This is required as an inaccurate OS timer may lead to a too infrequent schedule table synchronization.
    
    - Set /MICROSAR/StbM/StbMTriggeredCustomer/StbMTriggeredCustomerPeriod
    - Verify that StbM_TriggeredCustomerConfig table in StbM_Cfg.c or StbM_Cfg_<Partition>.c contains the configured value divided by /MICROSAR/StbM/StbMGeneral/StbMMainFunctionPeriod for the Timebase. The relations between the rows in StbM_TriggeredCustomerConfig table and the Timebase can be identified with the comment 'Referable Keys', which is the short name of the Timebase.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-895615

    The user of MICROSAR Safe shall ensure that safety-related Timebases are mapped to a partition with an appropriate ASIL level and not used in QM partitions or partitions with a lower ASIL level.
    
    - Verify that the Timebase is mapped to the expected partition via IndirectionToTimeBaseConfigPartitionIdx in StbM_TimeBasePartitionMapping table in
      StbM_Cfg.c and has only port connections to application SWCs that are mapped to partitions with an appropriate ASIL level.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


Additional verification measures
********************************

.. smi:: SMI-327486

    The user of MICROSAR Safe shall ensure that the services are called with the correct Timebase identifier. 
    StbM cannot detect if an incorrect identifier that is configured is used and thus the wrong time might be used.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-327487

    The user of MICROSAR Safe shall ensure that the Global Time is updated as frequently as needed to gain the required accuracy for global time masters.
    This is needed as the accuracy of the system depends on the accuracy of the Global Time Master.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-327488

    The user of MICROSAR Safe shall check the Timebase status to ensure the validity of the timestamps. 
    The status indicates if the timestamps delivered by StbM are based on the global time or not. Erroneous timestamps provided by the TSyn modules can be detected by timeout and time leap.
    The check shall be done at least two times the frequency of time updates from the Bus and on every time retrieval. If leap check fails, it is assumed that this is a fault situation.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-329662

    The user of MICROSAR Safe shall verify that the *StbM_BusSetGlobalTime* service is only called by TSyn modules.
    This requirement is fulfilled for all components of MICROSAR.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-329664

    The user of MICROSAR Safe shall ensure that local clock jitter is in a range small enough to ensure the required accuracy, considering the frequency of synchronization messages.
    This is required as the TSyn protocols and the leap check depend on the integrity of the local time.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-330197

    The user of MICROSAR Safe shall monitor the integrity of the local clock source at hardware interface level. The local clock must neither get stuck nor jump.
    This is required as the TSyn protocols and the leap check depend on the integrity of the local time.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-329893

    The user of MICROSAR Safe shall ensure that local applications on a Global Time Master ECU are informed about time leaps of the global time.
    This is needed as the Global Time Master services do not change the time leap status and thus applications will not be informed about time leaps.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-329905

    The user of MICROSAR Safe shall monitor that the *StbM_MainFunction* is scheduled with the intended cycle time. 
    This is required for the timeout to work as expected.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-947249

    The user of MICROSAR Safe shall ensure that if either Os counter or GPT is used as local clock source, the table StbM_LocalClockConfig in StbM_Cfg.c or StbM_Cfg_<Partition>.c contains the following configured and simplified values for frequency and prescaler:
    
    NumeratorOfFactorClockTicksToLocalTimeOfLocalClockConfig = (StbMClockPrescaler \* 10^9) / gcd(StbMClockFrequency, StbMClockPrescaler \* 10^9)
    
    DenominatorOfFactorClockTicksToLocalTimeOfLocalClockConfig = StbMClockFrequency / gcd(StbMClockFrequency, StbMClockPrescaler \* 10^9)
    
    Where gcd is the greatest common divisor.
    
    The relations between the rows in StbM_LocalClockConfig table and the Timebase can be identified with the comment 'Referable Keys', which is the short name of the Timebase.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-1030511

    The user of MICROSAR Safe shall ensure that applications allowed to modify the local clock provide correct functionality.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


Safety requirements required from other components
**************************************************

.. smi:: SMI-327286

    StbM requires the following safety requirements from the TSyn modules:
    
    - CRC validation if time gateway is configured
    - Usage of services as specified, i.e. with correct Timebase identifier and correctly processed global time values and status
    
    This requirement is fulfilled if:

    - an ASIL TSyn component by Vector is used, and 
    - it provides safety requirements, and 
    - the CRC validation functionality is activated.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-327287

    StbM requires the following functionality as safety requirements from the operating system:
    
    - Schedule table handling
    
    This requirement is fulfilled if an ASIL operating system by Vector is used.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


.. smi:: SMI-329663

    StbM requires the following functionality as safety requirements from the local clock sources (Os, Gpt, Eth):
    
    - The local clock must neither get stuck nor jump undetected. See also SMI-330197.
    
    This requirement is fulfilled if an ASIL operating system by Vector is used. For Eth and Gpt the user shall ensure the fulfilment of this requirement.
    
    This requirement is only applicable if TSR-111109 is assumed as a safety requirement.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

