Safety Manual
#############

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Version
     - Date
     - Author
     - Remarks

   * - 1.00.00
     - 2015-11-13
     - Jonas Wolf
     - Initial creation

   * - 1.01.00
     - 2015-12-18
     - Jonas Wolf
     - Information about ASIL added.

   * - 1.02.00
     - 2016-01-29
     - Jonas Wolf
     - Improvements in formulation.

   * - 1.03.00
     - 2016-02-25
     - Jonas Wolf
     - Improvements in formulation.

   * - 1.03.01
     - 2016-03-30
     - Jonas Wolf
     - Review findings incorporated.

   * - 1.03.02
     - 2016-05-13
     - Jonas Wolf
     - Added hint on hardware-software integration (SMI-4).

   * - 1.03.03
     - 2016-07-20
     - Hartmut Hoerner
     - Added SMI related to interrupt handling.

   * - 1.04.00
     - 2016-09-02
     - Jonas Wolf
     - Added TSR-101876 for data consistency.

   * - 1.04.01
     - 2016-09-19
     - Jonas Wolf
     - Clarifications on SMI-100 and SMI-19.

   * - 1.04.02
     - 2016-12-02
     - Jonas Wolf
     - Version of referenced document fixed.

   * - 1.05.00
     - 2017-02-24
     - Jonas Wolf
     - Modified SMI-18: checks need to be enabled per component as well.

   * - 1.06.00
     - 2017-05-05
     - Jonas Wolf
     - Added SMI-36039: Pointer validity passed to call-outs.

       | Added SMI-36041: Information on Beta-ESCANs.
       | Update of versions of referenced documents.
       | Section 1.1.5 clarified.

   * - 1.07.00
     - 2017-06-20
     - Jonas Wolf
     - Update of TSRs from TSC (update is backward compatible)

       | Added info on this version history.
       | Fixes of typos.

   * - 1.07.01
     - 2017-08-18
     - Jonas Wolf
     - Added information on TCL for MSSV and RTE Analyzer.

   * - 1.08.01
     - 2017-11-10
     - Jonas Wolf
     - Added information on TCL for post-built tool chain.

       | Added information that OS timing protection is only a partial replacement for a watchdog.

   * - 1.09.00
     - 2018-01-11
     - Jonas Wolf
     - Added details to SMI-16 for interfaces between MICROSAR components.

   * - 1.09.01
     - 2018-01-30
     - Jonas Wolf
     - Fixed typo in SMI-16.

   * - 1.09.02
     - 2018-05-03
     - Jonas Wolf
     - Added information on SMI-12.

   * - 1.10.00
     - 2018-11-16
     - Jonas Wolf
     - Update of technical safety concept.

       | More info on SMI-99.
       | Added SMI-246328.

   * - 1.11.00
     - 2019-06-25
     - Jonas Wolf
     - Update of technical safety concept.

   * - 1.12.00
     - 2020-03-16
     - Jonas Wolf
     - Adding SMI-428133 for VCA.

       | Adding clarification for SMI-8.

   * - 1.13.00
     - 2020-06-03
     - Jonas Wolf
     - Update of technical safety concept.

       | Adding SMI-494148 and SMI-246328 and clarifications for systems with safety-related availability requirements.

   * - 1.14.00
     - 2020-09-08
     - Jonas Wolf
     - Make history more precise and improve wording of SMI-494148.

   * - 1.15.00
     - 2021-02-18
     - Jonas Wolf
     - Make document structure more precise.

       | Improve details on SMI-14 and SMI-4.

   * - 1.16.00
     - 2021-03-08
     - Jonas Wolf
     - Update of information on timing guarantees and TSR-6.

   * - 1.16.01
     - 2021-03-12
     - Jonas Wolf
     - Update with review comments.

   * - 1.16.02
     - 2022-04-20
     - Vibhu Layal
     - Added information to SMI-9.

   * - 1.16.03
     - 2022-10-28
     - Jan Toennemann
     - Fixed ESCAN00113079 in SMI-18 and adapted SMI-10.

   * - 1.16.04
     - 2023-02-16
     - Jan Toennemann
     - Adapted SMI-494148 to include timing behavior.

General Part
############

Introduction
************

Purpose
=======

This document describes the assumptions made by Vector during the development of MICROSAR Safe as Software Safety Element out of Context (SEooC). This document provides information on how to integrate MICROSAR Safe into a safety-related project.

This document is intended for the user of MICROSAR Safe. It shall be read by project managers, safety managers, and engineers to allow proper integration of MICROSAR Safe.

Scope
=====

This document adds additional information to the components that are marked with an ASIL in the delivery description provided by Vector. Neither QM Vector components, nor components by other vendors are in the scope of this document.

Vector assumes that hardware and compiler manuals are correct and complete.
Vector uses the hardware reference manuals and compiler manuals for the development of MICROSAR Safe. Vector has no means to verify correctness or completeness of the hardware and compiler manuals.
Example information that may be critical from these manuals is the register assignment by compiler. This information is used to built up the context that is saved and restored by the operating system.
The compiler manual from the compiler version specified for the project is considered. The considered hardware manuals are documented in the Technical Reference of the hardware-specific component.

A general description of Vector's approach to ISO 26262 is described in [2]. This document is available on request.

Definitions
===========

The words *shall*, *shall not*, *should*, *can* in this document are to be interpreted as described here:

- *Shall* means that the definition is an absolute requirement of the specification.
- *Shall not* means that the definition is an absolute prohibition of the specification.
- *Should* means that there may exist valid reasons in particular circumstances to ignore a particular definition, but the full implications must be understood and carefully weighed before choosing a different course.
- *Can* means that a definition is truly optional. 

Each requirement in this specification has a unique identifier beginning with SMI. This identifier is semantically the same even for different Safety Manuals provided by Vector.

The user of MICROSAR Safe can deviate from all constraints and requirements in this Safety Manual in the responsibility of the user of MICROSAR Safe, if equivalent measures are used.
If a measure is equivalent can be decided in the responsibility of the user of MICROSAR Safe.

References
==========


.. list-table::
   :header-rows: 1

   * - No.
     - Source
     - Title
     - Version

   * - [1]
     - ISO
     - ISO 26262 Road vehicles — Functional safety (all parts)
     - 2011/2012

   * - [2]
     - Vector
     - ISO 26262 Compliance Document
     - 1.3.2

   * - [3]
     - Vector
     - MICROSAR Safe Product Information
     - As provided with the quote

   * - [4]
     - Vector
     - MICROSAR Safe Silence Verifier Technical Reference
     - As provided with the delivery

   * - [5]
     - Vector
     - Vector Code Analyzer Technical Reference
     - As provided with the delivery

Overview
========

This document is automatically generated for a delivery. The content of this document depends on the components (incl. their version) and used hardware (e.g. microcontroller or external hardware) in the delivery. This document is thus valid only for the delivery from Vector that it is included in. The version history above only reflects changes in the component independent part of the Safety Manual.

The structure of this document comprises:

- a general section that covers all assumptions and constraints that are always applicable, and
- at least one section for each ASIL component that covers its constraints and necessary verification steps. The section for a component is identified by its module abbreviation.
  

Vector's assumptions on the environment of the MICROSAR Safe components as well as the integration process are described.

Vector developed MICROSAR Safe as Safety Element out of Context for projects demanding ASIL D software. All requirements in this document apply independently from the actual highest ASIL of the project (unless otherwise stated). 

MICROSAR Safe comprises a set of components developed according to ISO 26262. These components can be combined - together with other measures - to build a safe system according to ISO 26262.

Please read the Product Information MICROSAR Safe [3] first.

Safety Concept
**************

It is assumed that the user of MICROSAR Safe is responsible for overall system safety. MICROSAR Safe can provide parts of safety mechanisms to its user. It is also assumed that the user of MICROSAR Safe verifies the robustness and effectiveness of the safety mechanisms within its system based on the configuration of MICROSAR Safe.

Technical Solution
==================

Possible hardware faults (transient or permanent) are detected and handled by the application (user of MICROSAR Safe) according to the required ASIL necessary for the project. 
This also implies protection against Single Event Upsets (SEUs) in volatile and non-volatile memory, as well as faults in registers and processing logic.

The hardware manual is assumed to be complete and correct.

MICROSAR Safe does not implement mechanisms to mitigate random hardware faults. Instead, MICROSAR Safe relies on hardware mechanisms, such as ECC memory and lock-step cores. For systems with safety-related availability requirements sufficiently reliable hardware or redundancy in hardware is assumed to cover hardware faults. Such hardware faults might e.g. occur during communication between the different parts of the redundant system.

Note that multi-core controllers with cores that provide different levels of diagnostic coverage (lock-step/non-lock-step cores) are considered during development of the MICROSAR Safe operating system. 
However, they also require a detailed analysis of core interdependency by the user of MICROSAR Safe.

It is assumed that systematic hardware faults are handled on system or application level by the user of MICROSAR Safe.

The Safety Manual of the hardware MICROSAR Safe controls is used during development. The respective part of MICROSAR Safe Safety Manual describes if MICROSAR Safe implements safety mechanisms required by the Safety Manual of the hardware it controls.

Systematic faults in functionality that is allocated an assumed TSR in MICROSAR Safe are prevented by implementation of an ISO 26262-compliant development process.
It is assumed that development according to an ISO 26262-compliant development process implicitly ensures freedom from interference with respect to memory. I.e. MICROSAR Safe components only write within bounds of memory that they have ownership of.

For systems with safety-related availability requirements, it is assumed that systematic faults in software in the chain of events must be avoided. Thus, the complete software in the chain of events must implement safety requirements (see e.g. TSR-111108). It is already ensured that no out-of-bounds memory write accesses are performed (see above). Invalid read accesses cannot interfere in memory, since they do not modify memory even if they trigger a trap. 

However, the software in the chain of events must also perform its function within a bounded time. Therefore, MICROSAR Safe provides a real-time scheduler in the operating system and the corresponding Runtime Environment (Rte) ensures scheduling as specified and configured by the user of MICROSAR Safe (see TSR-6). 

Moreover, components of MICROSAR Safe can provide a bounded execution time of their functionality. A finite execution time analysis (FETA) is performed for those components. This analysis ensures that components of MICROSAR Safe do not disturb execution flow of other modules by occupying CPU resources indefinitely, i.e. it ensures freedom from interference with respect to timing. This analysis is always a worst-case estimation. The information if a component provides this guarantee, can be found in the respective part of the Safety Manual of MICROSAR Safe. A Worst Case Execution Time (WCET) analysis still has to be performed by the user of MICROSAR Safe to evaluate if timing requirements in their system can be achieved.
Invalid read accesses to memory are not an issue in the first place, since the trap handler also is assumed to have a bounded runtime. If the upper bound of execution time of an algorithm in MICROSAR Safe components depends on the validity of read accesses this is analyzed during development of those components. 
Invalid read accesses could also impact the functionality of MICROSAR Safe components. However, the correctness of the used data is ensured by the testing activities during the development of MICROSAR Safe and the used defensive programming.

MICROSAR Safe does not implement redundant (inverted) data storage to mitigate software faults.

It is assumed that software with a different integrity level is separated using a memory protection unit (MPU) (see also TSR-7). 
The user of MICROSAR Safe has to ensure that write access to the same memory parts is only possible for software of the same or higher integrity level.

Depending on the requirements of the item, timing and program flow must be monitored. MICROSAR Safe provide mechanisms to implement this monitoring (see also TSR-13, TSR-14 and TSR-15). 
MICROSAR Safe does not automatically monitor itself for intended timing and program flow.
For systems with safety-related availability requirements, it may be required to avoid interference in time and not only to detect it using e.g. a watchdog. It is assumed that the user of MICROSAR Safe adequately ensures freedom from interference with respect to time for this use-case by configuration of MICROSAR Safe.

MICROSAR Safe components without allocated safety requirements are developed according to an ISO 26262-compliant development process to provide an argument for coexistence. 

If safety requirements are implemented in components of MICROSAR Safe, Vector ensures that they are locally non-complex. 
For locally non-complex components no diverse implementation of algorithms is designed.

Tool Confidence
===============

MICROSAR Safe must be configured using the DaVinci tools. MICROSAR Safe must be compiled by using a compiler for the target hardware. 
These tools must be evaluated with respect to their impact on functional safety by the user.

It is assumed that the user of MICROSAR Safe evaluates and qualifies the defined compiler (incl. options) and linker according to ISO 26262 Part 8 Clause 11.

The DaVinci Developer and DaVinci Configurator are assumed to have an impact on functional safety. In the first place, only a tool error detection level of two can be expected. 
The resulting tool confidence level (TCL) would require a qualification of those tools.

To ensure freedom from interference with respect to memory for the BSW, Vector provides the MICROSAR Safe Silence Verifier (MSSV). 

To ensure freedom from interference with respect to memory and to detect additional faults that may have been introduced by the DaVinci tools for the RTE, Vector provides the RTE Analyzer.

If MSSV and RTE Analyzer are used according to their Technical References and the Safety Manual, the tool confidence level for the DaVinci tools can be reduced to TCL1 by the user of MICROSAR Safe.

Tools by Vector do not implement mechanisms to handle hardware faults on the host development computer.

Assumed Safety Requirements of MICROSAR Safe
********************************************

The items listed in this section are the assumed technical safety requirements on the Safety Element out of Context MICROSAR Safe. 
These requirements are expected to match the requirements in the actual item development.

All technical safety requirements (TSRs) are assigned an ASIL D (unless otherwise stated) to service as many projects as possible.

No fault tolerant time intervals are given. Timing depends on the used hardware and its configuration. 
It is assumed that the user configures MICROSAR Safe adequately for the intended use.

No safe state is defined since MICROSAR Safe allows the user to define the desired behavior in case of a detected fault.

**The Safety Manual provided with a delivery shows the details on the support of a safety requirement on component level for a specific version.**

Initialization
==============

**TSR-1 MICROSAR Safe shall provide a mechanism to initialize itself and its controlled hardware.**

It is sensible for safety-related systems to start performing potentially hazardous actions only in a defined and intended state. 
For example, adequate setup of clocks may be required to reach required fault tolerant times.

Components of MICROSAR Safe initialize their variables. For post build selectable and post build loadable configurations MICROSAR Safe components use a consistent and defined set of configuration data.

Hardware-specific components of MICROSAR Safe describe in their Technical References what hardware units are controlled.

The user of MICROSAR Safe shall ensure that initialization functions of MICROSAR Safe are called in the right order and at the right point in time.
  
The user of MICROSAR Safe is also responsible for the startup code and main function.

Data Consistency
================

**TSR-101876 MICROSAR Safe shall provide mechanisms to ensure data consistency.**

Concurrent access to resources (e.g. variables) from e.g. task and interrupt level, may lead to data inconsistencies.

MICROSAR Safe provides functions to enable or disable interrupts, spin-locks, resources or abstractions (i.e. exclusive areas) to enable the user of MICROSAR Safe to ensure data consistency. 

MICROSAR Safe will not unintentionally enable or disable the mechanisms to ensure data consistency.

MICROSAR Safe also uses this functionality in its own components to ensure data consistency.

Non-volatile Memory
===================

**TSR-4 MICROSAR Safe shall provide a mechanism to store data in non-volatile memory.**

Calibration or other application data may be safety-related, i.e. if there is data available from non-volatile memory it must not be corrupted by either software or hardware.

MICROSAR Safe implements an **end-to-end protection** in the Non-volatile RAM Manager (NvM) to ensure that data is neither corrupted nor masqueraded in either software or hardware.

Note that hardware may not even start storing data in non-volatile memory or loose it at any time.

Availability of data is, thus, not guaranteed. Availability may be increased by redundantly storing data in non-volatile memory. 
The user of MICROSAR Safe must handle unavailability of data on application level.

**TSR-5 MICROSAR Safe shall provide a mechanism to retrieve data from non-volatile memory.**

Calibration or other application data may be safety-related, i.e. if there is data available from non-volatile memory it must not be corrupted by either software or hardware.

MICROSAR Safe implements an **end-to-end protection** in the Non-volatile RAM Manager (NvM) to ensure that data is neither corrupted nor masqueraded in either software or hardware.

Note that hardware may not even start storing data in non-volatile memory or loose it at any time.
  
Availability of data is, thus, not guaranteed. Availability may be increased by redundantly storing data in non-volatile memory. 
The user of MICROSAR Safe must handle unavailability of data on application level.

Note that if data is written more than once and the latest data in the non-volatile memory gets corrupted, older (uncorrupted) data may be returned to the user of MICROSAR Safe.

Hard Real-time Scheduling
=========================

**TSR-6 MICROSAR Safe shall provide hard real-time scheduling properties.**

Hard real-time scheduling is not required for safety in the first place, since deadline violations can usually be detected and a safe state entered. 
However, hard real-time scheduling may support argumentation in some cases.

For systems with safety-related availability requirements hard real-time scheduling properties are essential for the software that needs to stay operational.

The operating system of MICROSAR Safe implements fixed priority scheduling and the immediate priority ceiling protocol specified by AUTOSAR to guarantee hard real-time scheduling properties.

The runtime environment of MICROSAR Safe (SafeRTE) schedules runnables as configured by the user of MICRSOR Safe. 

Note that safety-related availability requirements on a system usually require a second independent channel of control.
  
Note that safety-related availability requirements require analyses of the worst-case execution time (WCET) of the software that needs to stay operational. These analyses are expected from the user of MICROSAR Safe.

Memory Protection
=================

**TSR-7 MICROSAR Safe shall provide mechanisms to protect software applications from unspecified memory access.**

Partitioning in software is often introduced because of different quality levels of software and different responsibilities of software development on one ECU.

Memory partitioning relies on the memory protection unit (MPU) in hardware for the effectiveness of the mechanism.

MICROSAR Safe configures exactly the configured memory access rights for each task and ISR at the hardware interface of the MPU. 

Note that MICROSAR Safe does not necessarily control all available protection units (e.g. system MPUs or peripheral protection units).

The user of MICROSAR Safe is responsible for adequate configuration of the memory partitions.

Communication
=============

Inter ECU Communication
-----------------------

**TSR-10 MICROSAR Safe shall provide mechanisms to protect communication between ECUs.**

Communication between ECUs may be corrupted, unintentionally replayed, lost or masked. To protect against these failure modes MICROSAR Safe provides the end-to-end (E2E) protection mechanism defined by AUTOSAR.

MICROSAR Safe detects repetition, loss, insertion, masking, reodering and corruption of messages between ECUs.
    
MICROSAR Safe allows the usage of cyclic redundancy checks (CRCs) and cryptographic algorithms to protect the integrity of messages between ECUs.
CRCs provide a certain hamming distance given a polynomial and maximum data block size.
Cryptographic hash functions or message authentication codes (MACs) provide a probabilistic statement on data corruption detection depending on the hash function or MAC, data block size and hash value size.

The user of MICROSAR Safe is responsible for the selection of the E2E profile or algorithm that is adequate to the requirements of the item development.

MICROSAR Safe does not reject messages with a valid CRC, hash or MAC. This is required for systems with safety-related availability requirements to claim reliability of a single channel. MICROSAR Safe relies on the integrity of the underlying hardware for e.g. verification of a MAC if configured.

**TSR-111108 MICROSAR Safe shall provide mechanisms to communicate between ECUs.**

End-to-end protection ensures that repetition, loss, insertion, masking, reordering and corruption of messages between ECUs can be detected. These kinds of faults can be introduced by hardware or software.
    
For systems with safety-related availability requirements it is required to eliminate software as a source of the above-mentioned faults on sender and receiver side. 

Thus, MICROSAR Safe does not unintentionally repeat, drop, insert, mask, reorder or corrupt messages. MICROSAR Safe might drop messages that have been corrupted by hardware faults, e.g. corruption by electromagnetic interference on the bus.

Usage of end-to-end protection is still required to ensure protection against hardware faults.

Intra ECU Communication
-----------------------

**TSR-16 MICROSAR Safe shall provide mechanisms to communicate within its applications.**

Software components need to communicate in order to fulfill their task.

MICROSAR Safe provides mechanisms to communicate within OS applications without end-to-end protection.
  
MICROSAR Safe does not unintentionally repeat, delay, insert, mask, corrupt or loose data communicated within OS applications.

MICROSAR Safe assumes protection of the memory against random hardware faults by the system, e.g. via ECC RAM and lock-step mode.

**TSR-12 MICROSAR Safe shall provide mechanisms to communicate between its applications.**

Mixed ASIL or multi-core systems need to exchange safety-related information between applications.

MICROSAR Safe provides mechanisms to communicate between OS applications without end-to-end protection.

MICROSAR Safe does not unintentionally repeat, delay, insert, mask, corrupt or loose data communicated between OS applications.

MICROSAR Safe assumes protection of the memory against random hardware faults by the system, e.g. via ECC RAM and lock-step mode.

Monitoring of Software
======================

MICROSAR Safe provides monitoring mechanisms to supervise correct execution of software. These mechanisms must be configured by the user of MICROSAR Safe according to the requirements within the context of the item development.

**TSR-13 MICROSAR Safe shall provide a mechanism to detect faults in program flow.**

Program flow can be corrupted by random hardware faults or software faults.

The user of MICROSAR Safe can configure a graph of the logical program flow that is then supervised by MICROSAR Safe. A transition from one checkpoint to another that is not allowed is detected.

**TSR-14 MICROSAR Safe shall provide a mechanism to detect stuck software.**

Alive monitoring is used to reset the software or controller in case it is unresponsive.

The user of MICROSAR Safe can configure entities that need to regularly report their alive status that is then supervised by MICROSAR Safe. Omission of a regular report is detected.

**TSR-15 MICROSAR Safe shall provide a mechanism to detect deadline violations.**

Deadlines are important to reach a safe state within the defined fault tolerant time interval. 

The user of MICROSAR Safe can configure a tolerable time interval for each transition in a logical program flow graph that is then supervised by MICROSAR Safe. Violation of a deadline is detected.

**TSR-8 MICROSAR Safe shall provide a mechanism to detect time budget violations.**

Budgets for task execution times, inter-arrival times and locking times allow to identify the originator of a deadline violation.

The user of MICROSAR Safe can configure budgets for execution times of tasks and category 2 ISRs, inter-arrival times and duration of locks (e.g. resource or interrupt locks) that are then supervised by MICROSAR Safe. 
Exhaustion of budgets is detected.

**TSR-9 MICROSAR Safe shall provide a mechanism to terminate software applications.**

Terminating a complete OS application may be used as a safety mechanisms to mitigate a software fault within this OS application. This terminated OS application typically comprises QM software.

MICROSAR Safe terminates exactly the requested OS application and only upon request.

Note that MICROSAR Safe will not terminate OS applications automatically without configuration or request.

Operating Modes
===============

**TSR-100551 MICROSAR Safe shall provide a mechanism to switch between operating modes.**

Fault detection and mitigation often result in a degraded or safe state. 

MICROSAR Safe does not unintentionally switch between operating modes. MICROSAR Safe switches to exactly the requested operating mode.

Peripheral In- and output
=========================

**TSR-17 MICROSAR Safe shall provide a mechanism to read input data from peripheral units.**

Input of data from microcontroller peripheral units is required to implement almost any safety-related system.

MICROSAR Safe provides the data read from peripheral units without corruption or unintentional delay. 

MICROSAR Safe does not unintentionally reorder nor masquerade data read from peripheral units.

Note that the peripheral hardware may not provide sufficient diagnostic coverage without redundant input.

For example, DIO and SPI drivers provided by Vector support the reading input data from peripheral units as safety requirement.

**TSR-18 MICROSAR Safe shall provide a mechanism to write output data to peripheral units.**

Output of data from microcontroller peripheral units is required to implement almost any safety-related system.

MICROSAR Safe writes the data provided at the software interfaces without corruption or unintentional delay. 
  
MICROSAR Safe does not unintentionally trigger peripheral output units.
  
MICROSAR Safe does not unintentionally reorder nor masquerade data read from peripheral units.

Note that the peripheral hardware may require additional mechanisms on application or system level to ensure sufficient safety.

For example, DIO and SPI drivers provided by Vector support the output of data to peripheral units as safety requirement (e.g. to trigger external watchdogs or switch actuation paths).

Time Synchronization
====================

**TSR-111109 MICROSAR Safe shall provide a mechanism synchronize time between different ECUs.**

For advanced driver assistance systems (ADAS) data from different sensor ECUs must be synchronized to perform sensor fusion. It is necessary that all participating ECUs must have a synchronized time for the sensor fusion to yield correct results. Moreover, it is required that the synchronized time correlates to the real time in the real world. If e.g. synchronized time and real time drift apart, sensor fusion will not yield correct results anymore.

MICROSAR Safe provides time synchronization using different communication busses like CAN, FlexRay and Ethernet. The used mechanisms for time synchronization differ between the different bus systems. This impacts the achievable accuracy of the synchronized time.

MICROSAR Safe can be used as time master and time slave.

MICROSAR Safe does not claim a synchronized state if the reported time is not synchronized to the master.

MICROSAR Safe relies on the integrity of the local clock source used for synchronization. MICROSAR Safe does not monitor the local clock source. Especially, drift and jumps are not monitored by MICROSAR Safe.

MICROSAR Safe does not detect a loss of accuracy resulting from faults in the local clock source (Ethernet) or unexpected interrupt latency (CAN and FlexRay).

Assumptions of Use
******************

Safety Concept
==============

.. smi:: SMI-14

    **The user of MICROSAR Safe shall be responsible for the functional safety concept.**
    
    The overall functional safety concept is in the responsibility of the user of MICROSAR Safe. MICROSAR Safe can only provide parts that can be used to implement the functional safety concept of the item.
    It is also the responsibility of the user of MICROSAR Safe to configure MICROSAR Safe as intended by the user's safety concept. 
    The safety concept shall only rely on safety requirements explicitly described in this safety manual. If a component from MICROSAR Safe does not explicitly describe safety requirements in this safety manual, this component has been developed according to the methods for ASIL D to provide coexistence with other ASIL components. 

    - Example: NvM provides safety requirements for writing and reading of data, the lower layers, i.e. MemIf, Ea, Fee and drivers, only provide the ASIL for coexistence.
    
    The safety concept shall **not** rely on functionality that is **not** explicitly described as safety requirement in this safety manual. This functionality may fail silently in case of a detected fault. 

    - Example: If a potential out-of-bounds memory access, e.g. due to invalid input or misconfiguration, is detected the requested function will not be performed. An error via DET is only reported if error reporting is enabled.
    
    Vector recommends including the components from MICROSAR Safe in the software architecture of the project. 
    Vector assumes that safety requirements in the user's project are mapped to the corresponding components of MICROSAR Safe.
    Vector assumes that the user checked that the assumed safety requirements provided by the component of MICROSAR Safe matches the allocated safety requirement in the software architecture of the project.


.. smi:: SMI-1

    **The user of MICROSAR Safe shall adequately address hardware faults.**
    
    The components of MICROSAR Safe can support in the detection and handling of some hardware faults (e.g. using watchdog).
    MICROSAR Safe does not provide redundant data storage.
    The user of MICROSAR Safe especially shall address faults in volatile random access memory, non-volatile memory, e.g. flash or EEPROM, and the CPU.
    MICROSAR Safe relies on the adequate detection of faults in memory and the CPU by other means, e.g. hardware. Thus, Vector recommends using lock-step CPUs together with ECC memory.
    See also SMI-14.


.. smi:: SMI-10

    **The user of MICROSAR Safe shall ensure that the reset or powerless state is a safe state of the system.**
    
    Vector uses this assumption in its safety analyses and development process. This is true for individual instances of MICROSAR Safe; if safety-related availability requirements are allocated to the system, the user of MICROSAR Safe shall ensure availability on the level of the overall system, e.g. by introducing redundancy where appropriate.


.. smi:: SMI-20

    **The user of MICROSAR Safe shall implement a timing monitoring using e.g. a watchdog.**
    
    The components of MICROSAR Safe do not provide mechanisms to monitor their own timing behavior.
    The watchdog stack that is part of MICROSAR Safe can be used to fulfill this assumption.
    If the safety concept also requires logic monitoring, the watchdog stack that is part of MICROSAR Safe can be used to implement it.
    The watchdog is one way to perform timing monitoring. Today the watchdog is the most common approach. In future, there may be different approaches e.g. by monitoring using a different ECU.
    The timing protection provided by the operating system can only partially replace the usage of a timing monitoring by a watchdog, because e.g. deadline violations cannot be directly detected by the OS timing protection.
    See also SMI-14.


.. smi:: SMI-98

    **The user of MICROSAR Safe shall ensure an end-to-end protection for safety-related communication between ECUs.**
    
    The communication components of MICROSAR Safe do not assume sending or receiving as a safety requirement, because considered faults can only be detected using additional information like a cycle counter. Vector always assumes that an end-to-end protection or equivalent mechanism is implemented on application level.
    Considered faults in communication are:

    - Failure of communication peer 
    - Message masquerading 
    - Message corruption
    - Unintended message repetition 
    - Insertion of messages 
    - Re-sequencing 
    - Message loss 
    - Message delay
    
    This requirement can be fulfilled by e.g. using the end-to-end protection wrapper for safety related communication.


.. smi:: SMI-11

    **The user of MICROSAR Safe shall ensure data consistency for its application.**
    
    Data consistency is not automatically provided when using MICROSAR Safe. MICROSAR Safe only provides services to support enforcement of data consistency. Their application is in the responsibility of the user of MICROSAR Safe.
    To ensure data consistency in an application, critical sections need to be identified and protected.
    To identify critical sections in the code, e.g. review or static code analysis can be used.
    To protect critical sections, e.g. the services to disable and enable interrupts provided by the MICROSAR Safe operating system can be used.
    To verify correctly implemented protection, e.g. stress testing or review can be used.
    Note the AUTOSAR specification with respect to nesting and sequence of calls to interrupt enabling and disabling functions.
    See also TSR-101876.


Integration of MICROSAR Safe Components
=======================================

.. smi:: SMI-2

    **The user of MICROSAR Safe shall adequately select the type definitions to reflect the hardware platform and compiler environment.**
    
    The user of MICROSAR Safe is responsible for selecting the correct platform types (PlatformTypes.h) and compiler abstraction (Compiler.h). Especially the size of the predefined types shall match the target environment.
    Example: A uint32 shall be mapped to an unsigned integer type with a size of 32 bits.
    The user of MICROSAR Safe can use the platform types provided by Vector. Vector has created and verified the platform types mapping according to the information provided by the user of MICROSAR Safe.


.. smi:: SMI-12

    **The user of MICROSAR Safe shall initialize all components of MICROSAR Safe prior to using them.**
    
    This constraint is required by AUTOSAR anyway. Vector uses this assumption in its safety analyses and development process.
    Unintended re-initialization may lead to unintended behavior. Thus, the user shall ensure that initialization shall occur only at defined points in time.
    Correct initialization can be verified, e.g. during integration testing.


.. smi:: SMI-16

    **The user of MICROSAR Safe shall only pass valid pointers at all interfaces to MICROSAR Safe components.**
    
    Plausibility checks on pointers are performed by MICROSAR Safe (see also SMI-18), but they are limited. MICROSAR Safe components potentially use provided pointers to write to the location in memory.
    Also the length and pointer of a buffer provided to a MICROSAR Safe component need to be consistent.
    This requirement applies to QM as well as ASIL components. For example an NvM executed in an ASIL context will write to addresses provided by a QM software components. This requirement does not apply to interfaces between MICROSAR Safe components, because that has been verified by Vector already.
    This can e.g. be verified using static code analysis tools, reviews and integration testing.


.. smi:: SMI-36039

    **The user of MICROSAR Safe shall only retain pointers provided by MICROSAR Safe where explicitly stated.**
    
    Some MICROSAR Safe components provide call-outs to user or application code. The pointers passed to these call-outs may have limited validity.
    The user of MICROSAR Safe shall ensure that pointers are not used neither for reading nor for writing after returning from the call-out. Exceptions of this rule are explicitly documented for the respective call-out.


.. smi:: SMI-494148

    **The user of MICROSAR Safe shall ensure the correct usage of the provided API from MICROSAR Safe components.**
    
    The correct usage of APIs is documented in the respective Technical Reference of the component, provided safety requirements are listed in the respective Safety Manual part.
    
    If a component is used incorrectly, it may not fulfill its functionality as specified. Example: If the wrong data is handed over to the communication stack, the communication stack will send this wrong data on the bus.
    
    The timing behavior of any API might be affected by configuration as well as passed parameters. Computation time typically grows linearly e.g. with the amount of loop iterations but may also depend on other factors, e.g. depending on the actual key (not key size) in asymmetrical cryptographic operations. Components that provide freedom-from-interference with regard to timing allow WCET analysis tools to determine an upper bound for the computation time. Incorrect use of APIs can lead to unexpected timing behavior.
    
    This requirement is only applicable if using MICROSAR Safe in a system with safety-related availability requirements.


.. smi:: SMI-18

    **The user of MICROSAR Safe shall enable plausibility checks for the MICROSAR Safe components.**
    
    This setting is necessary to introduce defensive programming and increase robustness at the interfaces as required by ISO 26262.
    This setting shall be configured at ``<Component-specific path>/<Ma>SafeBswChecks`` for all components that are intended to be ASIL, where ``<Ma>`` is the Module Abbreviation. This is only required for components that actually provide this setting.
    
    This setting is enforced by an MSSV plug-in. The MSSV report shall be checked to ensure that all ASIL components are listed accordingly. 
    
    This setting does not enable error reporting to the DET component. 


.. smi:: SMI-1725

    **The user of MICROSAR Safe shall configure and use the interrupt system correctly.**
    
    The user of MICROSAR Safe is responsible for a correct and consistent configuration and usage of the interrupt system.
    Especially the following topics shall be verified:

    - Consistent configuration of interrupt category, level and priority in OS and MCAL modules
    - Correct assignment of logical channels/instances to interrupt vectors in case of MCAL modules with multiple channels/instances
    - The interrupt controller is configured in a mode which processes interrupts of the same level sequentially to avoid unbounded interrupt nesting


.. smi:: SMI-36041

    **The user of MICROSAR Safe shall not use functionality of MICROSAR Safe components marked with a BETA-ESCAN.**
    
    Functionality marked with a BETA-ESCAN is not thoroughly tested by Vector. 
    Vector ensures that functionality marked with a BETA-ESCAN can be completely disabled.
    
    The list of ESCANs (incl. BETA-ESCANs) is provided with a delivery. 


Partitioning
============

.. smi:: SMI-9

    **The user of MICROSAR Safe shall ensure that for one AUTOSAR functional cluster (e.g. System Services, Operating System, CAN, COM, etc.) only components from Vector are used.**
    
    This assumption is required because of dependencies within the development process of Vector.
    This assumption does not apply to the MCAL or the EXT cluster.
    Vector may have requirements on MCAL or EXT components depending on the upper layers that are used and provided by Vector. For example, the watchdog driver is considered to have safety requirements allocated to its initialization and triggering services. Details are described in the component specific parts of this safety manual.
    This assumption does not apply to components that are not provided by Vector at all.
    In case the partitioning solution is used, this assumption only partially applies to the System Services cluster. Only the Watchdog Manager and Watchdog Interface need to be used from Vector then, because the Watchdog Manager and Watchdog Interface will be placed in separate memory partitions apart from the other System Services components.


.. smi:: SMI-32

    **The user of MICROSAR Safe shall provide an argument for coexistence for software that resides in the same partition as components from MICROSAR Safe.**
    
    Vector considers an ISO 26262-compliant development process for the software as an argument for coexistence (see [1] Part 9 Clause 6). Vector assumes that especially freedom from interference with respect to memory is provided by an ISO 26262-compliant development process.
    Redundant data storage as the only measure by the other software is not considered a sufficient measure.
    If ASIL components provided by Vector are used, this requirement is fulfilled.
    In general Vector components do not implement methods to interface with other software (e.g. components, hooks, callouts) in other partitions. They assume that all interfacing components reside in the same partition. Interfacing components are described in the respective technical reference.
    If an argument for coexistence cannot be provided, other means of separation shall be implemented (e.g. trusted or non-trusted function calls).


.. smi:: SMI-99

    **The user of MICROSAR Safe shall verify that the memory mapping is consistent with the partitioning concept.**
    
    The volatile data of every component shall be placed in the associated memory partition. This can be verified e.g. by review of the linker map file.
    The memory sections for each component placed in RAM can be identified <MIP>_START_SEC_VAR[_<xxx>], where <MIP> is the Module Implementation Prefix of the component.
    It is assumed that the MPU is configured to not allow write access by a partition with lower integrity to data from a partition with higher integrity.


Resources
=========

.. smi:: SMI-33

    **The user of MICROSAR Safe shall provide sufficient resources in RAM, ROM, stack and CPU runtime for MICROSAR Safe.**
    
    Selection of the microcontroller and memory capacities as well as dimensioning of the stacks is in the responsibility of the user of MICROSAR Safe.
    If MICROSAR Safe components have specific requirements, these are documented in the respective Technical Reference document.


.. smi:: SMI-246328

    **The user of MICROSAR Safe shall perform a worst-case execution time analysis when using MICROSAR Safe in systems with safety-related availability requirements.**
    
    In systems with safety-related availability requirements, it is assumed that an upper bound for the execution time of MICROSAR functionality is required. 
    
    This requirement is only applicable if using MICROSAR Safe in a system with safety-related availability requirements.


Process
=======

.. smi:: SMI-15

    **The user of MICROSAR Safe shall follow the instructions of the corresponding Technical Reference of the components.**
    
    Especially deviations from AUTOSAR specifications are described in the Technical References.
    If there are constraints for the implementation of an exclusive area, these are described in the Technical References.


.. smi:: SMI-5

    **The user of MICROSAR Safe shall verify all code that is modified during integration of MICROSAR Safe.**
    
    Code that is typically modified by the user of MICROSAR Safe during integration comprises generated templates, hooks, callouts, or similar.
    This assumption also applies if interfaces between components are looped through user-defined functions.
    
    Vector assumes that this verification also covers ISO 26262:6-9. Vector assumes that modified code that belongs to a Vector component, e.g. EcuM callouts or OS trace hooks can at least coexist with this component, because no separation in memory or time is implemented.
    
    Example: Callouts of the EcuM are executed in the context of the EcuM.
    Non-trusted functions provided by the Vector operating system can be used to implement a separation in memory in code modified by the user of MICROSAR Safe.


.. smi:: SMI-30

    **The user of MICROSAR Safe shall only modify source code of MICROSAR Safe that is explicitly allowed to be changed.**
    
    Usually no source code of MICROSAR Safe is allowed to be changed by the user of MICROSAR Safe.
    The user of MICROSAR Safe can check if the source code was modified by e.g, comparing it to the original delivery. 


.. smi:: SMI-8

    **The user of MICROSAR Safe shall verify generated functions according to ISO 26262:6-9.**
    
    Generated functions can be identified when searching through the generated code.
    
    An example of generated functions is the configured rules of the Basic Software Manager (BswM). Their correctness can only be verified by the user of MICROSAR Safe. Please note, however, that BswM not necessarily assumes a safety requirement.
    This requirement does not apply to MICROSAR SafeRTE and the transformers (ComXf, DiagXf, E2eXf, SomeIpXf).


.. smi:: SMI-19

    **The user of MICROSAR Safe shall execute the MICROSAR Safe Silence Verifier (MSSV).**
    
    MSSV is used to detect potential out-of-bounds accesses by Vector's basic software based on inconsistent configuration.
    Details on the required command line arguments and integration into the tool chain can be found in [4].
    If the report shows "Overall Check Result: Fail", please contact Vector. 


.. smi:: SMI-428133

    **The user of MICROSAR Safe shall execute the Vector Code Analyzer (VCA).**
    
    The Vector Code Analyzer is used to detect potential out-of-bounds accesses by Vector's basic software.
    This requirement only applies if the Vector Code Analyzer is delivered.
    Details on the required command line arguments and integration into the tool chain can be found in [5].
    If the report shows "Overall Check Result: Fail", please contact Vector. 


.. smi:: SMI-4

    **The user of MICROSAR Safe shall perform the integration (ISO 26262:6-10) and verification (ISO 26262:6-11) processes as required by ISO 26262.**
    
    Especially, the safety mechanisms and safety-related functionality shall be verified in the final target ECU.
    As a prerequisite, Vector assumes that the safety concept by the user of MICROSAR Safe allocates safety requirements to the individual components of MICROSAR Safe (see SMI-14).
    
    Vector assumes that by performing the integration and verification processes as required by ISO 26262 the generated configuration data, e.g. data tables, task priorities or PDU handles, are sufficiently checked. An additional review of the configuration data is then considered not necessary.
    
    Integration does not apply to a MICROSAR Safe component that consists of several units. This integration is already performed by Vector.
    
    However, integration of all MICROSAR Safe components in the specific use-case of the user of MICROSAR Safe is the responsibility of the user of MICROSAR Safe. This also includes the hardware-software integration in the context of the target ECU.
    
    Vector assumes that the integration test performed by the user fulfills the exit criteria defined by ISO 26262:6-10 (Structural coverage at the software architectural level) and ISO 26262:6-11. 
    
    Vector assumes that in corresponce with the required ASIL for the project of the user of MICROSAR Safe, typical tests methods, like equivalence classes and boundary value analysis are applied. Creating equivalence classes might reduce testing effort with respect to testing all individual instances of a test object.
    
    Vector especially assumes that in correspondence with the required ASIL for the project of the user of MICROSAR Safe, safety mechanisms are tested using fault injection. Example: Restbus simulation of relevant E2E faults for protected signal groups.


.. smi:: SMI-100

    **The user of MICROSAR Safe shall ensure that a consistent set of generated configuration is used for verification and production.**
    
    Make sure that the same generated files are used for testing and production code. E.g. be aware that configuration can be changed in the DaVinci tools without generating the code again.
    
    Make sure that all generated files have the same configuration basis, i.e. always generate the MICROSAR Safe configuration for all components for a relevant release of the ECU software.
    
    The use of post build loadable is supported but not recommended by Vector. Post build loadable increases complexity of verification and validation processes due to increased number of variants. There is no support by Vector to qualify the post build loadable tool chain provided by Vector to create binary data without a qualified compiler.


.. smi:: SMI-176

    **The user of MICROSAR Safe shall verify the integrity of the delivery by Vector.**
    
    Run the SIPModificationChecker.exe and verify that the source code, BSWMD and safety manual files are unchanged.


.. smi:: SMI-31

    **The user of MICROSAR Safe shall verify the consistency of the binary downloaded into the ECU's flash memory.**
    
    This also includes re-programming of flash memory via a diagnostics service. The consistency of the downloaded binary can be checked by the bootloader or the application. MICROSAR Safe assumes a correct program image. 


.. smi:: SMI-3

    **The user of MICROSAR Safe shall evaluate all tools (incl. compiler) that are used by the user of MICROSAR Safe according to ISO 26262:8-11.**
    
    Evaluation especially shall be performed for the compiler, linker, debugging and test tools.
    Vector provides a guideline for the evaluation of the Tool Confidence Level (TCL) for the tools provided by Vector (e.g. DaVinci Configurator PRO).
    MSSV and RTE Analyzer can be assumed TCL2 and qualified for this TCL, since a safety-related development process at Vector is applied for those tools. Confirmation that this safety-related development process was applied is part of the safety case.
    Vector has only evaluated the tools used by Vector during the development of MICROSAR Safe. Tool evaluation, for tools used by the user of MICROSAR Safe, is in the responsibility of the user of MICROSAR Safe.



Glossary and Abbreviations
##########################

Glossary
********


.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Term
     - Definition

   * - User of MICROSAR Safe
     - Integrator and user of components from MICROSAR Safe provided by Vector.

   * - MICROSAR Safe
     - MICROSAR Safe comprises MICROSAR SafeBSW and MICROSAR SafeRTE as Safety Element out of Context. MICROSAR SafeBSW is a set of components, that are developed according to ISO 26262 [1], and are provided by Vector in the context of this delivery. The list of MICROSAR Safe components in this delivery can be taken from the documentation of the delivery.

   * - Critical section
     - A section of code that needs to be protected from concurrent access. A critical section may be protected by using the AUTOSAR exclusive area concept.

   * - Configuration data
     - Data that is used to adapt the MICROSAR Safe component to the specific use-case of the user of MICROSAR Safe. Configuration data typically comprises among others: feature selection, routing tables, channel tables, task priorities, memory block descriptions.

   * - Generated code
     - Source code that is generated as a result of the configuration in DaVinci Configurator Pro

   * - Partition
     - A set of memory regions that is accessible by tasks and ISRs. Synonym to OSApplication. 

Abbreviations
*************


.. list-table::
   :header-rows: 1
   :widths: 20 80
   
   * - Abbreviation
     - Description

   * - ASIL
     - Automotive Safety Integrity Level

   * - BSWMD
     - Basic Software Module Description

   * - CPU
     - Central Processing Unit

   * - CREQ
     - Component Requirement

   * - EEPROM
     - Electronically Erasable and Programmable Read-only Memory

   * - ECC
     - Error Correcting Code

   * - ECU
     - Electronic Control Unit

   * - EXT
     - Driver for an external hardware unit

   * - ISO
     - International Standardization Organization

   * - MCAL
     - Microcontroller Abstraction

   * - MIP
     - Module Implementation Prefix

   * - MSSV
     - MICROSAR Safe Silence Verifier

   * - OS
     - Operating System

   * - PDU
     - Protocol Data Unit

   * - QM
     - Quality Management

   * - RAM
     - Random Access Memory

   * - SMI
     - Safety Manual Item

   * - TCL
     - Tool Confidence Level

Contact
#######

Visit our website for more information on

- News
- Products
- Demo software
- Support
- Training data
- Addresses

www.vector.com 

