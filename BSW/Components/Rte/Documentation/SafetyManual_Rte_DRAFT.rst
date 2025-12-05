Safety Manual Rte
#################

Assumed safety requirements
***************************

.. smi:: SMI-323

    This component assumes the following safety requirements:
    
    
    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10
    
       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL 

       * - CREQ-1024
         - Rte shall provide a service to initiate the transmission of data elements with last-is-best semantic for explicit S/R communication.
         - TSR-16,TSR-12,TSR-111108,TSR-10
         - ASIL D

       * - CREQ-1021
         - Rte shall provide a service to copy the received data element to a buffer with last-is-best semantic for explicit S/R communication.
         - TSR-16,TSR-111108,TSR-10,TSR-12
         - ASIL D

       * - CREQ-1022
         - Rte shall provide a service to get the value of the received data element with last-is-best semantic for explicit S/R communication.
         - TSR-16,TSR-12
         - ASIL D

       * - CREQ-1031
         - Rte shall provide a service to read a data element for implicit S/R communication.
         - TSR-12,TSR-16
         - ASIL D

       * - CREQ-1029
         - Rte shall provide a service to write a data element for implicit S/R communication.
         - TSR-16,TSR-12
         - ASIL D

       * - CREQ-1041
         - Rte shall provide a service to get the reference of a data element to be written for implicit S/R communication.
         - TSR-16,TSR-12
         - ASIL D

       * - CREQ-1037
         - Rte shall provide a service to get the status of a data element for implicit S/R communication.
         - TSR-16,TSR-12
         - ASIL D

       * - CREQ-1033
         - Rte shall provide a service to access the update flag for a data element for explicit S/R communication.
         - TSR-12,TSR-16
         - ASIL D

       * - CREQ-1036
         - Rte shall provide a "Never-received" status of a data element for S/R communication.
         - TSR-16,TSR-12
         - ASIL D

       * - CREQ-1023
         - Rte shall provide a service to initiate the transmission of a data element with queued semantic for explicit S/R communication.
         - TSR-111108,TSR-12,TSR-10,TSR-16
         - ASIL D

       * - CREQ-1025
         - Rte shall provide a service to initiate the reception of a data element with queued semantic for explicit S/R communication.
         - TSR-111108,TSR-12,TSR-16,TSR-10
         - ASIL D

       * - CREQ-1042
         - Rte shall provide a service to initiate a client-server communication.
         - TSR-16,TSR-12
         - ASIL D

       * - CREQ-1043
         - Rte shall provide a service to get the result of an asynchronous client-server call.
         - TSR-12,TSR-16
         - ASIL D

       * - CREQ-1151
         - Rte shall provide the optional usage of one or several data transformers.
         - TSR-10
         - ASIL D

       * - CREQ-1109
         - Rte shall provide mode management.
         - TSR-100551
         - ASIL D

       * - CREQ-1055
         - Rte shall provide a service to get the currently active mode.
         - TSR-100551
         - ASIL D

       * - CREQ-1052
         - Rte shall provide a service to get the currently active, previous and next mode.
         - TSR-100551
         - ASIL D

       * - CREQ-1053
         - Rte shall provide a service to initiate a mode switch.
         - TSR-100551
         - ASIL D

       * - CREQ-1299
         - Rte shall provide Nv data communication.
         - TSR-5,TSR-4
         - ASIL D

       * - CREQ-1150
         - Rte shall provide a callback to copy data from a NVM buffer to RTE.
         - TSR-5
         - ASIL D

       * - CREQ-1148
         - Rte shall provide a callback to copy data from RTE to a NVM buffer.
         - TSR-4
         - ASIL D

       * - CREQ-1144
         - Rte shall provide a callback to get notified about a finished NVM job.
         - TSR-4,TSR-5
         - ASIL D

       * - CREQ-1147
         - Rte shall provide a callback to get notified about a requested mirror initialization.
         - TSR-5,TSR-4
         - ASIL D

       * - CREQ-1046
         - Rte shall provide a service to read Inter-Runnable Variables with explicit behavior.
         - TSR-16
         - ASIL D

       * - CREQ-1048
         - Rte shall provide a service to write Inter-Runnable Variables with explicit behavior.
         - TSR-16
         - ASIL D

       * - CREQ-1047
         - Rte shall provide a service to read Inter-Runnable Variables with implicit behavior.
         - TSR-16
         - ASIL D

       * - CREQ-1044
         - Rte shall provide a service to write Inter-Runnable Variables with implicit behavior.
         - TSR-16
         - ASIL D

       * - CREQ-149683
         - Rte shall provide a service to get the reference of an Inter-Runnable Variable with implicit behavior for write purposes.
         - TSR-16
         - ASIL D

       * - CREQ-174803
         - Rte shall provide a service to initiate the triggering of runnable entities over internal trigger communication.
         - TSR-6
         - ASIL D

       * - CREQ-1045
         - Rte shall provide a service to access per-instance memory.
         - TSR-16
         - ASIL D

       * - CREQ-1051
         - Rte shall provide a service to enter an exclusive area.
         - TSR-101876
         - ASIL D

       * - CREQ-1050
         - Rte shall provide a service to leave an exclusive area.
         - TSR-101876
         - ASIL D

       * - CREQ-1056
         - Rte's Basic Software Scheduler shall provide a service to enter an exclusive area of a BSW Module.
         - TSR-101876
         - ASIL D

       * - CREQ-1049
         - Rte's Basic Software Scheduler shall provide a service to leave an exclusive area of a BSW Module.
         - TSR-101876
         - ASIL D

       * - CREQ-1068
         - Rte shall provide a service to access internal calibration parameters.
         - TSR-5
         - ASIL D

       * - CREQ-SCHMCDATA
         - Rte shall provide a service to access internal calibration parameters of a BSW Module.
         - TSR-5
         - ASIL D

       * - CREQ-1075
         - Rte shall provide a service to access calibration parameters accessible via ports.
         - TSR-5
         - ASIL D

       * - CREQ-1059
         - Rte shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-184330
         - Rte shall provide a service to activate the triggers for cyclic and background runnables.
         - TSR-1
         - ASIL D

       * - CREQ-1073
         - Rte's Basic Software Scheduler shall provide a service to initialize itself.
         - TSR-1
         - ASIL D

       * - CREQ-1161
         - Rte shall provide a service to trigger executable entities.
         - TSR-6
         - ASIL D

       * - CREQ-1165
         - Rte shall use schedule points to invoke the scheduler of the OS.
         - TSR-6
         - ASIL D

       * - CREQ-1129
         - Rte shall provide the event handling of TimingEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-1112
         - Rte shall provide the event handling of SwcModeSwitchEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-1124
         - Rte shall provide the event handling of AsynchronousServerCallReturnsEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-1126
         - Rte shall provide the event handling of OperationInvokedEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-1118
         - Rte shall provide the event handling of DataReceivedEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-1132
         - Rte shall provide the event handling of ModeSwitchedAckEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-1114
         - Rte shall provide the event handling of the InitEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-174850
         - Rte shall provide the event handling of InternalTriggerOccuredEvents to trigger a runnable.
         - TSR-6
         - ASIL D

       * - CREQ-1295
         - Rte shall provide the event handling of TimingEvents to trigger a schedulable entity.
         - TSR-6
         - ASIL D

       * - CREQ-1152
         - Rte shall provide a "RTE_AND_SCHM_UNINIT" state.
         - TSR-6
         - ASIL D

       * - CREQ-1164
         - Rte shall provide a "RTE_UNINT_SCHM_INIT" state.
         - TSR-6
         - ASIL D

       * - CREQ-1166
         - Rte shall provide a "INIT" state.
         - TSR-6
         - ASIL D
    
    
    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration constraints
*************************

.. smi:: SMI-2066

    **The user of MICROSAR Safe shall disable online calibration and measurement during series production.**
    
    The RTE can be generated with online calibration and measurement enabled for series production, but they shall be made inoperable for normal operation. Vector's XCP can e.g. be disabled safely during runtime by ASIL software.


.. smi:: SMI-530545

    **The user of MICROSAR Safe shall not use RTE Implementation Plugins for mode communication for ASIL software.**
    
    RTE implementation plugins can be used to partially replace RTE APIs with custom implementations.
    
    The user of MICROSAR Safe shall ensure that the safety related parts of the RTE do not use RTE implementation plugins for mode communication.
    
    The RTE Analyzer lists used RTE implementation plugins to assist in this integration step.


.. smi:: SMI-764244

    **The user of MICROSAR Safe shall disable Logical Execution Time Support for ASIL software**
    
    No Rte_LetFrame_<frame>_Release and Rte_LetFrame_<frame>_Terminate functions shall exist in the code.
    
    The RTE Analyzer reports not supported template variants in case it detects usage of LET.


.. smi:: SMI-1102567

    **The user of MICROSAR Safe shall disable ArrayImplPolicy PayloadAsPointerToArray for safety related SWCs**
    
    The RTE Analyzer reports not supported template variants in case it detects usage of arrays with ArrayImplPolicy set to PayloadAsPointerToArray.


Additional verification measures
********************************

Please note that the RTE Generator and RTE Analyzer only implement measures to detect systematic faults by the software. No measures are implemented to detect or mitigate random faults on the computer used for generation.

.. smi:: SMI-322

    **The user of MICROSAR Safe shall execute the RTE Analyzer.**
    
    The RTE Analyzer performs checks to identify faults in the generated RTE. Especially out-of-bounds accesses within the RTE are detected. If the RTE Analyzer reports a fault, the generated RTE cannot be used. Moreover it provides the user of MICROSAR Safe with feedback what was generated. This feedback shall be reviewed during integration testing against the intended software design and its configuration.
    
    For details on how to execute the RTE Analyzer, please refer to the Technical Reference of the RTE Analyzer.


.. smi:: SMI-860026

    **The user of MICROSAR Safe shall execute the RTE Analyzer on Windows.**
    
    MICROSAR Classic RTE Analyzer for Linux is not yet qualified for series production.


.. smi:: SMI-466082

    **The user of MICROSAR Safe shall execute the RTE Analyzer with extended configuration feedback commandline switch for fail-operational systems.**
    
    The RTE Analyzer performs checks to identify faults in the generated RTE. By default, the RTE Analyzer configuration feedback is only printed for ASIL partitions. An MPU can protect the ASIL partition from memory corruption that is caused by the QM partition. However, the QM partition can also have an impact on the scheduling of the ASIL partition that needs to be considered for fail-operational systems.
    This is especially relevant when there are connections between ASIL and QM SWCs.
    When RTE Analyzer is started with the commandline switch for the extended configuration feedback, the configuration feedback is also printed for QM partitions.
    
    For details on how to execute the RTE Analyzer, please refer to the Technical Reference of the RTE Analyzer.


.. smi:: SMI-36067

    **The user of MICROSAR Safe shall ensure that the RTE Analyzer does not report unsupported templates.**
    
    The generated RTE code is based on templates. The templates are instantiated by the RTE Generator in different variants. The RTE Analyzer verifies that the analyzed template variants have been tested during the development of the RTE Generator according to ISO 26262.
    
    The last section of the RTE Analyzer configuration feedback report provides information about the template variants.
    The report must show that no unsupported templates have been found.


.. smi:: SMI-763706

    **The user of MICROSAR Safe shall ensure that the variables Rte_InitState<Core> are mapped to memory sections with the highest ASIL level.**
    
    The RTE uses RTE_START_SEC_VAR_<Partition>_ZERO_INIT_8BIT to map the Rte_InitState variables.
    In fail-safe systems with the Partitioning approach where BSW (Com, LdCom, NvM) is QM, the initialization of the Rte can be done from the partition that contains EcuM (QM Partition after StartOS).
    Also Rte_InitState can be mapped to this QM partition as long as the execution of the safety-related entities is supervised by additional safety mechanisms (e.g. a watchdog).
    When Rte_InitState is mapped to an ASIL partition, QM ECUM needs to call the lifecycle APIs within trusted functions.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


The RTE generator provides user code sections that can be used to adapt the memory mapping. Changes within the sections are kept for subsequent generator runs. As the user code may need to be adapted together with configuration changes, e.g. when a SWC is mapped to a different partition, the default code is printed as comment below the user code section.

Guided integration testing
==========================

Residual faults in the RTE Generator can only be found during integration testing. Vector assumes that the user of MICROSAR Safe performs an integration testing and verification of software safety requirements according to ISO 26262 Part 6 Clauses 10 and 11 (see also SMI-4). To support this integration testing, the RTE Analyzer produces a configuration feedback report. 

Please refer to the Technical Reference of the RTE Analyzer for a description of the configuration feedback report.

The following subsections describe the requirements that must be fulfilled during integration testing and verification of software safety requirements.

Each Safety Manual Item (SMI) is structured in the following way:

- The requirement that must be fulfilled
- Explanation of the requirement and a rationale
- Recommended configuration constraints (optional)
- Recommended means of complying with the requirement (optional)
- Details on the information provided by the RTE Analyzer supporting this requirement

BSW configuration
-----------------

.. smi:: SMI-2124

    **The user of MICROSAR Safe shall ensure that the RTE and the operating system assume the same scheduling properties.**
    
    The scheduling properties of the RTE tasks depend on the configuration of the operating system. The scheduling properties are e.g. preemptability, core assignment or task priority.
    
    The RTE Analyzer lists the scheduling properties in the configuration feedback report to assist in this integration step. The scheduling properties listed in the feedback report shall be verified.


.. smi:: SMI-2129

    **The user of MICROSAR Safe shall ensure that the assumptions of the operating system and the RTE are the same with regards to the locking behavior of the spinlocks.** 
    
    The RTE generator uses spinlocks from the operating system to protect inter-core communication. Spinlocks must not be called concurrently on the same core. The operating system optionally provides spinlocks that can prevent these concurrent accesses on the same core. If this protection by the operating system is not used, the RTE generator has to prevent concurrent calls to the spinlock APIs on the same core. 
    
    Verification can e.g. be performed by review of the configuration feedback report.
    
    The RTE Analyzer lists spinlocks that are not protected by the RTE to assist in this integration step.


.. smi:: SMI-684

    **The user of MICROSAR Safe shall ensure that the configuration of the COM and the RTE are consistent.**
    
    The interfaces to the COM that are used for signal reception use *void* pointers as parameter. Inconsistencies between the configuration of the COM and the RTE might lead to memory corruption by the COM. During integration, the size assumptions between the COM and the RTE shall be compared. 
    
    Verification can be performed by review of the generated configuration and/or static code analysis.
    
    The RTE Analyzer lists relevant calls to assist in this integration step.
    
    The RTE Analyzer listing includes the number of written bytes for the MICROSAR COM.
    
    The RTE Analyzer lists the signals that are assigned to the individual partitions with callbacks.
    
    The RTE Analyzer lists the signals that are assigned to the individual partitions without callbacks.


.. smi:: SMI-685

    **The user of MICROSAR Safe shall ensure that the configuration of the NVM and the RTE are consistent.**
    
    The interfaces to the NVM that are used to handle NV Block SWCs use *void* pointers as parameters.
    Inconsistencies between the configuration of the NVM and the RTE might lead to memory corruption by the RTE.
    During integration, the size assumptions between the NVM and the RTE shall be compared and it shall be checked that the intended block is accessed and has the expected layout.
    Moreover it has to be assured that the NVM calls the Rte_GetMirror and Rte_SetMirror callbacks to store and restore the NVM data at the point in time that is expected by the application.
    
    The Rte_GetMirror and Rte_SetMirror functions, the block size and whether the block is read or written during read and write all can typically be found in the generated data structures of NVM in NvM_Cfg.c.
    The value that is used in the checks in the Rte_GetMirror and Rte_SetMirror callbacks needs to be the same as the block size.
    if (size <= <BlockSize>U)
    
    Verification can e.g. be performed by review of the generated configuration, static code analysis and/or integration testing.
    
    The RTE Analyzer lists relevant calls to assist in this integration step.
    
    For MICROSAR Classic NVM, the RTE Analyzer also checks that the written data does not exceed the maximum internal NVM buffer length.
    
    The RTE Analyzer lists the detected partition that contains NVM.


Executable Entity Scheduling
----------------------------

.. smi:: SMI-2063

    **The user of MICROSAR Safe shall ensure that all safety-related executable entities are triggered with their correct conditions.**
    
    These conditions are:

    - cylic triggers with cycle time and offset
    - init triggers
    - background triggers
    - triggers fired by RTE APIs
    
    Safety related executable entities include server runnables that are called with direct function calls from the ASIL partition.
    If triggers have a dependency on modes, the scheduling has to be verified for all modes. Modes can be switched with the *Rte_Switch* API.
    Moreover, mode switching can also happen in the task that contains the mode switch triggers or mode disablings. When mode receivers are distributed on several cores, the RTE synchronizes the mode machines in these tasks. Thus mode triggered runnables on other cores can block the scheduling on the core that initiated the mode switch. Please note that, if these tasks are tasks with lower safety level and freedom from interference with regards to memory is not ensured, also other code in the same partition can corrupt the mode disablings and lead to unintended blocking of the tasks.
    Moreover queues and APIs with waitpoints can lead to blocking of ASIL tasks. See SMI-2125 and SMI-2064.
    This needs to be considered when designing fail-operational systems.
    All partitions need to be analyzed for fail-operational systems, not only the ASIL partitions.
    Triggers can be decoupled by the minimum start interval functionality and the data reception filter functionality.
    The scheduling of the executable entities also depends on the configuration of the operating system, the used controller, other running tasks and interrupt service routines and the resource usage of the entities that are implemented by the user.
    
    Vector requires not using the minimum start interval for safety-related runnables.
    
    Vector requires not using the data reception filter functionality for safety-related runnables.
    
    Vector requires not using activation reasons for safety-related runnables.
    
    Vector recommends not using background triggers for safety-related functionality.
    
    Vector recommends not distributing mode receivers on different cores.
    
    Vector requires not using QM mode receivers on different cores for fail-operational systems.
    
    Vector recommends using cyclic scheduling without mode dependencies and using of a watchdog as safety mechanism for safety-related entities where possible.
    
    Cyclic triggers are e.g. scheduled deterministically. Thus, an integration test verifying that safety-related functionality is scheduled at the expected times may be sufficient.
    
    The RTE Analyzer lists the executable entities of SWCs and the tasks in which they are executed to assist in this integration step.
    
    The RTE Analyzer lists recursive calls to assist in this integration step.


.. smi:: SMI-2128

    **The user of MICROSAR Safe shall ensure that reentrant runnables are reentrant.**
    
    Runnables can be called reentrantly from multiple tasks. Their implementation needs to support this use case. 
    
    Verification can e.g. be performed by review, static code analysis and/or integration testing.
    
    The RTE Analyzer lists all runnables of SWCs that are called from concurrent tasks to assist in this integration step.
    
    Implicit exclusive areas and nonpreemptive tasks can be configured to prevent concurrent execution.


.. smi:: SMI-2064

    **The user of MICROSAR Safe shall ensure that the timeouts configured for blocking and non-blocking APIs that are used in safety-related executable entities are adequately addressed.**
    
    If a timeout for an API is used as a safety mechanism (E.g. no checkpoint with deadline monitoring in the task), the user of MICROSAR Safe shall also ensure that the timeout value is adequate.
    
    Relevant timeouts are:

    - timeouts of *Rte_Call* APIs
    - timeouts of *Rte_Result* APIs
    - timeouts of *Rte_Receive* APIs
    - timeouts of *Rte_Feedback* APIs
    - timeouts of *Rte_SwitchAck* APIs
    
    The timeouts also depend on the configuration of the operating system, the used controller, other running tasks and interrupt service routines and the resource usage of entities that are implemented by the user.
    
    For blocking APIs:
    Vector recommends not using blocking APIs in safety-related entities except for cross partition client-/server communication.
    
    Vector strongly recommends not using blocking APIs without timeout.
    
    Vector requires not using blocking APIs without timeout for fail-operational systems.
    
    This is necessary because otherwise the APIs will wait forever after a communication loss and cannot be used for additional requests until the RTE is restarted. 
    
    For non-blocking APIs:
    Vector requires not using non-blocking APIs with timeout in safety-related entities.
    
    A review may be sufficient to verify that timeout handling is implemented properly by the SWC.
    If no other safety mechanism is in place, a test that the timeout is notified at the expected time by the RTE can be used as means of verification.
    
    The RTE Analyzer lists the blocking and non-blocking APIs of SWCs with and without timeout to assist in this integration step.


.. smi:: SMI-2122

    **The user of MICROSAR Safe shall ensure that the correct implementation method has been chosen for every exclusive area.**
    
    Exclusive areas can be used to ensure data consistency (see SMI-11).
    The implementation depends on the requirements of the application and on other factors like the expected duration of the exclusive area. Interrupt locks are typically faster than resources but can only be used for short sequences. OS_INTERRUPT_BLOCKING only blocks the interrupts of the operating system whereas ALL_INTERRUPT_BLOCKING blocks all interrupts.
    
    Verification can e.g. be performed by review, static code analysis and/or integration testing.
    
    The RTE Analyzer lists the exclusive areas and their implementation method to assist in this integration step.


SWC Communication
-----------------

.. smi:: SMI-41492

    **The user of MICROSAR SafeBRE shall provide the RTE APIs for systems without RTE.**
    
    MICROSAR Classic Basic Runtime Environment (BRE) only provides the BSW scheduler functionality of the RTE and does not support application SWCs. The implementation of the interface from the application to BSW modules must be developed according to ISO 26262. The Technical References of the BSW modules as well as the AUTOSAR standard define the semantics and APIs that will have to be implemented while integrating the BRE.
    
    The RTE Analyzer does not assist in this integration step.


SMI-324, SMI-2065 and SMI-2123 do not apply to MICROSAR SafeBRE.

.. smi:: SMI-324

    **The user of MICROSAR Safe shall ensure that the connections between SWCs are as intended.**
    
    Many types of faults can lead to a mix of connections between SWCs. These are unlikely and usually already addressed by straight-forward integration testing.
    The list of senders needs to be correct for every receiver and the subset of the received data needs to be correct.
    This includes senders and receivers that are connected through implementation plugins.
    For implicit communication also the buffering in the RTE needs to be considered.
    
    Vector recommends the following RTE subset for safety-related SWCs:

    - use only 1:1 or 1:N connections.
    - use the same datatype on both sides of a connection
    - avoid data conversion
    
    Information that is used from non-safety-related SWCs has to be checked for plausibility. If such a data path is found during integration, this is an indicator that your safety analysis has to be reconsidered. Please note that also other code in the same partition as the non-safety-related SWCs can corrupt the communication if freedom from interference with regards to memory is not ensured.
    Moreover a mismatch between the SWC implementation and the configuration that is seen by the RTE generator when it comes to the argument direction of client-server arguments may lead to wrong input and output values.
    
    Verification can be performed by review and/or an integration test testing the normal operation.
    
    The RTE Analyzer list the connections between SWCs to assist in this integration step.


.. smi:: SMI-2065

    **The user of MICROSAR Safe shall ensure that inter-ECU sender-/receiver and inter-ECU client-/server communication work as expected.**
    
    This requires verification of:

    - data needs to be routed to the correct ECU by the underlying communication stack. This includes 1:1, 1:N, N:1 and reception of partial signal data.
    - both ECUs need to use the same data representation (datatypes, endianness, serialization)
    
    Vector requires using E2E protection for safety-related signals.
    Only evaluating the status information from the COM (e.g. update, alive timeout, invalidation status) is no sufficient safety menchanism.
    
    Vector requires not using Inter-ECU client-server communication for safety-related signals.
    
    Vector requires not using Inter-ECU trigger communication for safety-related signals.
    
    Vector requires not using implicit communication for safety-related signals.
    
    Vector requires not using blocking queued communication for safety-related signals.
    
    Vector requires not using LDCOM trigger transmit for safety-related signals.
    
    Vector requires not using queued communication with TP signals for safety-related signals.
    
    Vector requires not using invalidation for safety-related signals.
    
    Vector requires not using scaling for safety-related signals.
    
    Vector requires not using read by value for safety-related signals.
    
    Vector requires not using fan-out for safety-related signals if the COM/LDCOM instance is in a different partition than the SWC.
    
    Vector requires that other software does not corrupt RTE variables in the BSW partition when client or server use E2E protection and are not located in the BSW partition. This is necessary because the transformers including the E2E transformer need to be called in the BSW partition in order to route signals to the requesting component.
    
    Vector recommends using 1:1 connections.
    
    Vector recommends always sending and receiving complete data elements.
    
    Integration testing on vehicle network level using fault-injection can be used. Vector assumes that this is normally done to verify the effectiveness of the E2E protection.
    Testing needs to be performed for all postbuild variants.
    
    The RTE Analyzer lists the APIs of SWCs with inter-ECU communication to assist in this integration step.


.. smi:: SMI-2123

    **The user of MICROSAR Safe shall ensure that all connected SWCs expect the same converted data.**
    
    The RTE offers conversions that can be applied to specific connections.
    
    Vector recommends not using data conversion for safety-related connections.
    
    Verification can e.g. be performed by review or integration testing of all data conversions.
    
    The RTE Analyzer lists all APIs of SWCs that perform data conversion to assist in this integration step.


.. smi:: SMI-1218483

    **The user of MICROSAR Safe shall ensure that all connected SWCs expect the same out of range handling.**
    
    The RTE offers optional range checks that can map out of range values to the configured limits of the data type.
    
    Verification can e.g. be performed by review or integration testing of all range checks.
    
    Vector requires not using range checks for safety-related runnables.
    
    The RTE Analyzer lists all APIs of SWCs that perform range checks to assist in this integration step.


Usage of RTE Headers
--------------------

.. smi:: SMI-2067

    **The user of MICROSAR Safe shall ensure that the *defines* and *typedefs* that are generated by the RTE match the expectations of the SWCs that use them.**
    
    Inconsistencies may lead to e.g, memory corruption when a runnable uses an RTE array datatype within its implementation and writes beyond the bounds of this array.
    Moreover, different SWCs may have different assumptions with regards to the meaning of communicated values, e.g. if one SWC uses the symbolic name, another SWC the integral value of an enumerated type.
    
    The following code is provided:

    - Configured Application Error Defines for Client-/Server Communication
    - Configured AUTOSAR Datatypes
    - Configured Upper and Lower Limit Defines for Primitive Application Data Types
    - Configured Init Value Defines for Sender-/Receiver Communication
    - Configured InvalidValue Defines for Sender-/Receiver Communication
    - Configured Enumeration Defines for CompuMethods
    - Configured Mask Defines for CompuMethods
    - Configured Mode Defines for Mode Communication
    - Configured ActivationReasons
    - Configured Rte_IsAvailable and Rte_SetAvailable defines for records with optional elements
    
    The defines and typedefs are part of the *Rte_Type*._h_, *Rte__<SWC>._h* and *Rte__<SWC>_Type._h* headers.
    
    Vector recommends using the headers generated by the RTE when a static code analysis is performed on the application code.
    
    Vector recommends only using the *defines* instead of the defined values in the code.
    
    Vector recommends using the *defines* only when needed (Mode, Application Error Defines).
    
    Vector recommends reviewing the used *defines* for safety-related SWCs.
    
    Vector recommends not using union types in the SWCs.
    
    Verification for correct usage of datatypes may be performed by review and/or static code analysis. Consistent usage of *defines* can be verified by review and/or integration testing with all used values.
    
    The RTE Analyzer verifies that all accesses within the RTE do not lead to memory corruption.


.. smi:: SMI-2071

    **The user of MICROSAR Safe shall ensure that the indirect API is used consistently.**
    
    Indirect API functionality consists of the APIs:

    - *Rte_Port*
    - *Rte_Ports*
    - *Rte_NPorts*
     
    The indirect API makes it possible to call different APIs through an array access. The indirect API functionality can be enabled individually per port.
    A wrong configuration switch can easily lead to a call outside of the array returned by the Rte_Ports API.
    
    Vector recommends not using the indirect API.
    
    Verification can e.g. be performed by review that the intended APIs are returned.
    
    The RTE Analyzer does not assist in this integration step.


Usage of RTE APIs
-----------------

.. smi:: SMI-2072

    **The user of MICROSAR Safe shall ensure that the RTE and all of its users have the same assumptions with regards to the sizes of the datatypes.**
    
    The RTE supports the configuration of custom datatypes for its APIs. The RTE specification mandates that arrays are passed as pointer to the array base type.
    The RTE does not enforce that both sides of a connection use arrays of the same size.
    
    No NULL pointers or invalid pointers shall be passed to RTE APIs.
    As an exception NULL pointers can be passed to IN arguments of client-server operations that use a data type of category data reference.
    The object to which the pointer points needs to have at least the size of the pointer base type.
    No variables from ASIL partitions shall be passed as out parameters to RTE APIs in QM partitions.
    
    RTE APIs returning a pointer/reference need special care:

    - Use correct data-type/data-size for reading or writing.
    - Use the pointer/reference only during execution of the runnable and within its scope.
    - Do not globally store retrieved pointers, they might not be valid after runnable execution.
    
    Vector recommends using the headers generated by the RTE when static code analysis is performed on the application code.
    
    Vector recommends using the same datatypes on both sides of a connection.
    Arrays and void pointers on interfaces where the called function writes to them are considered especially relevant.
    
    Verification can e.g. be performed by review and/or static code analysis.
    
    The RTE Analyzer lists APIs and runnables that use such parameters to assist in this integration step.


.. smi:: SMI-2073

    **The user of MICROSAR Safe shall ensure that RTE APIs are only called from their configured contexts.**
    
    Fast response times are crucial in embedded systems. Therefore, the RTE generator analyzes the call contexts of all APIs in order to optimize away unneeded interrupt locks. When the application calls the APIs from a different context than the RTE assumes, data consistency problems may arise.
    
    In systems with ASIL partitions, the RTE generator uses a conservative locking strategy unless this is explicitly disabled with the parameter OptimizeLocksInASILPartitions. Locks are only optimized away if all accesses are done within the same task.
    
    Verification can e.g. be performed by review and/or static code analysis.
    
    The RTE Analyzer lists the optimized APIs of SWCs to assist in this integration step.
    
    The RTE Analyzer lists APIs that must not be called by the application because they are considered unreachable due to the RTE configuration.


.. smi:: SMI-859695

    **The user of MICROSAR Safe shall ensure that implicit accesses with RteExclusiveAccessOptimization are really exclusive.**
    
    RteExclusiveAccessOptimization containers can be configured to mark implicit accesses to NV block SWCs as exclusive. As the RTE generator does not implement any data consistency mechanisms for such accesses, RteExclusvieAccessOptimization must only be configured in case the RAM mirror is not accessed concurrently in the target system.
    
    Verification can e.g. be performed by review.
    
    The RTE Analyzer lists concurrent accesses to the RAM mirror of NvBlockSWCs as if RteExclusiveAccessOptimization was not configured.


Configuration of RTE APIs
-------------------------

.. smi:: SMI-2074

    **The user of MICROSAR Safe shall ensure that the receivers can handle the initial value provided by the RTE if no write or calibration occurred.**
    
    For implicit accesses the user of MICROSAR Safe shall assure that the correct initial value is sent when *Rte_IWrite* or *Rte_IWriteRef* were not called in the runnable.
    
    Initial values can be configured for:

    - non-queued sender-/receiver communication
    - inter-runnable variables
    - mode ports 
    - calibration parameters 
    - per-instance memories
    
    The initial value is returned when no sending API was called before the first read or no calibration was performed before the first read. The initial values depend on the connected components.
    
    Vector recommends using the same initial value on all port data elements that are connected with each other.
    
    The RTE Analyzer lists all APIs of SWCs that may provide an initial value to assist in this integration step. If possible, the RTE Analyzer reports the initial value generated by the RTE into the configuration feedback report.


.. smi:: SMI-2075

    **The user of MICROSAR Safe shall ensure that the alive timeout by the COM is not used for safety-related inter-ECU sender/receiver communication.**
    
    Safety-related communication must be protected by E2E protection. The decision if new data is available (alive) can only be made by the E2E mechanism. Data is not interpreted by the COM. For example the sending ECU might repeat old data. This is only detected by the cycle counter that is part of the E2E protection.
    
    The RTE Analyzer lists the reading APIs that provide the alive timeout status to assist in this integration step.


.. smi:: SMI-2126

    **The user of MICROSAR Safe shall ensure that SWCs handle the *RTE_E_INVALID* return code properly.**
    
    The RTE offers a functionality to invalidate signals.
    
    Vector requires using end-to-end protection for safety-related inter-ECU communication. Relying on the invalidation mechanism for safety-related signals is not an option. The user of MICROSAR Safe shall not use invalidation for inter-ECU communication.
    
    Verification can e.g. be performed by review and/or integration testing.
    
    The RTE Analyzer lists RTE APIs that return the *RTE_E_INVALID* return code to assist in this integration step.


.. smi:: SMI-2127

    **The user of MICROSAR Safe shall ensure that SWCs handle the *RTE_E_NEVER_RECEIVED* return code properly.**
    
    The RTE offers a functionality to report if a signal was received after the ECU was started.
    
    Vector requires using end-to-end protection for safety-related signals. Relying on the never received mechanism for safety-related signals is not an option. Especially, when using the E2E transformer, its return value shall be evaluated.
    
    Verification can e.g. be performed by review or integration testing.
    
    The RTE Analyzer lists RTE APIs that return the *RTE_E_NEVER_RECEIVED* return code to assist in this integration step.


.. smi:: SMI-2125

    **The user of MICROSAR Safe shall ensure that the queue sizes that were chosen during the configuration are sufficient for the integrated system.**
    
    The RTE uses queues for mode communication, sender-/receiver communication and mapped client-/server communication. The queue sizes depend on the scheduling of entities and the call sequences of the APIs.
    The queue handling code for server runnables and mode switches is executed in a loop in the task.
    Adding new data while the queue is processed leads to additional loop iterations.
    The RTE uses the configured queue sizes to limit the number of queue entries that are processed at once when the server runnable is triggered so that other entities in the same task and tasks with higher priority can be scheduled in between.
    Still, entities in tasks with lower and equal priority may be blocked until the queue is empty. This needs to be considered when designing fail-operational systems. All partitions need to be analyzed for fail-operational systems, not only the ASIL partitions.
    
    Vector recommends not using APIs with queues for safety-related functionality.
    
    Vector requires not using queues with more than 1000000 elements for safety-related functionality.
    
    Verification can e.g. be performed by stress testing.
    
    The RTE Analyzer lists the queue sizes to assist in this integration step.


.. smi:: SMI-36068

    **The user of MICROSAR Safe shall ensure that all connections with end-to-end (E2E) protection are generated.**
    
    The RTE Analyzer lists RTE APIs that read or write end-to-end protected data (see SMI-98).


.. smi:: SMI-1093171

    **The user of MICROSAR Safe shall ensure that the generated code with activated illegal invocation detection is not used in ASIL Software.**
    
    The RTE offers a functionality to detect RTE API Calls called by a Runnable which should not call the RTE API according to the Runnables contract and report those to the DET.
    This functionality is intended to be only used as a checking mechanism to ensure that the RTE APIs are used as configured.
    
    Using this functionality in production software could have negative impact in the overall time behaviour of the system, as a check is performed for every call to one of the RTE APIs, that are configured to be checked.
    
    Vector recommends not using the illegal invocation detection for safety-related functionality.


.. smi:: SMI-RTE-INITMEMORY

    **The user of MICROSAR Safe shall ensure that the global Rte variables are initialized during startup routine.**
    
    The global Rte variables should be initialized in the Rte_InitMemory Api that is called during the startup routine.
    The generation of the Rte_InitMemory is optional. If the Rte does not generate the Rte_InitMemory, the user must ensure that the global Rte variables are initialized.

    The RTE Analyzer lists the variables that are not initialized to assist in this integration step.


Safety requirements required from other components
**************************************************

.. smi:: SMI-2121

    The RTE requires the following functionality as safety requirements from the operating system:

    - Interrupt enabling/disabling (TSR-101876)
    - Resource handling (TSR-101876)
    - Inter OS Application Communicator (IOC) sending and receiving functionality (TSR-12)
    - Spin-lock functionality (TSR-101876)
    - Alarm handling (TSR-6)
    - Core handling (TSR-6)
    - Counter handling (TSR-6)
    - Schedule table handling (TSR-6)
    - Activation of tasks (TSR-6)
    - Event handling (TSR-6)
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.


.. smi:: SMI-2978

    The RTE requires the following functionality as safety requirements from the NVM:

    - Reading blocks (TSR-5)
    - Writing blocks (TSR-4)
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    This requirement only applies if TSR-4 and TSR-5 are considered a safety requirement.


.. smi:: SMI-661323

    This component requires sending and receiving functionality as an assumed safety requirement (TSR-111108) from other interfacing components.
    
    For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.
    
    This requirement is only applicable if TSR-111108 is assumed as a safety requirement.


.. smi:: SMI-1185442

    The RTE requires the following functionality as safety requirement from RTE implementation plugins for explicit access protection:
    
    - Data Consistency (TSR-101876)


.. smi:: SMI-1185443

    The RTE requires the following functionality as safety requirements from RTE implementation plugins for explicit sender-receiver communication services:
    
    - Communication within applications (TSR-16)
    - Communication between applications (TSR-12)


.. smi:: SMI-1185444

    The RTE requires the following functionality as safety requirements from RTE implementation plugins for implicit sender-receiver communication services:
    
    - Communication within applications (TSR-16)
    - Communication between applications (TSR-12)


Dependencies to hardware
************************

This component does not use a direct hardware interface.

