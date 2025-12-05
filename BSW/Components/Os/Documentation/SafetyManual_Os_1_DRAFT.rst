Safety Manual Os
****************

Assumed Safety Requirements
===========================

.. smi:: SMI-Os_30_TriCoreAurixGen7-1259

    This component assumes the following safety requirements:


    .. list-table::
       :header-rows: 1
       :widths: 25 45 20 10

       * - ID
         - Safety Requirement
         - Associated TSR
         - ASIL

       * - CREQ-118
         - OS shall provide different application modes.
         - TSR-1
         - ASIL D

       * - CREQ-63
         - OS shall provide a service to start the OS.
         - TSR-6,TSR-1
         - ASIL D

       * - CREQ-162
         - OS shall provide a service to initialize itself.
         - TSR-1,TSR-6
         - ASIL D

       * - CREQ-51
         - OS shall automatically start a subset of alarms for an application mode.
         - TSR-6
         - ASIL D

       * - CREQ-146
         - OS shall automatically start a subset of tasks for an application mode.
         - TSR-6
         - ASIL D

       * - CREQ-72
         - OS shall automatically start a subset of schedule tables for an application mode.
         - TSR-6
         - ASIL D

       * - CREQ-117
         - OS shall provide a service to get the current application mode.
         - TSR-6
         - ASIL D

       * - CREQ-45
         - OS shall provide a global callback during OS startup.
         - TSR-6
         - ASIL D

       * - CREQ-299
         - Os shall synchronize the startup in multi-core systems.
         - TSR-6
         - ASIL D

       * - CREQ-153
         - OS shall provide a service to shutdown the OS.
         - TSR-6
         - ASIL D

       * - CREQ-95
         - OS shall provide a service to shutdown all cores synchronously.
         - TSR-6
         - ASIL D

       * - CREQ-161
         - OS shall provide a global callback upon shutdown.
         - TSR-6
         - ASIL D

       * - CREQ-89
         - OS shall provide tasks.
         - TSR-16,TSR-6,TSR-7,TSR-12,TSR-8,TSR-9
         - ASIL D

       * - CREQ-58
         - OS shall provide basic tasks.
         - TSR-6,TSR-7,TSR-12,TSR-16,TSR-9,TSR-8
         - ASIL D

       * - CREQ-20
         - OS shall provide extended tasks.
         - TSR-7,TSR-6,TSR-16,TSR-12,TSR-9,TSR-8
         - ASIL D

       * - CREQ-71
         - OS shall provide a global callback directly before a task starts its execution.
         - TSR-6,TSR-8,TSR-7
         - ASIL D

       * - CREQ-165
         - OS shall provide a global callback directly before a task finishes its execution.
         - TSR-6,TSR-8,TSR-7
         - ASIL D

       * - CREQ-42
         - OS shall provide a service to activate a task.
         - TSR-6,TSR-8,TSR-7,TSR-9
         - ASIL D

       * - CREQ-Os-AsynCrossCoreTaskActivation
         - OS shall provide a service to activate a task asynchronously.
         - TSR-6,TSR-7,TSR-8
         - ASIL D

       * - CREQ-28
         - OS shall handle multiple activations of basic tasks.
         - TSR-6
         - ASIL D

       * - CREQ-101
         - OS shall provide a service to terminate the calling task.
         - TSR-9,TSR-6
         - ASIL D

       * - CREQ-121
         - OS shall provide a service to define the next activated task.
         - TSR-6
         - ASIL D

       * - CREQ-126
         - OS shall provide a service to explicitly invoke the scheduler.
         - TSR-6
         - ASIL D

       * - CREQ-80
         - OS shall provide a service to get the ID of the current task.
         - TSR-6
         - ASIL D

       * - CREQ-74
         - OS shall provide a service to get the state of a given task.
         - TSR-6
         - ASIL D

       * - CREQ-135
         - OS shall provide a service to declare a task.
         - TSR-6
         - ASIL D

       * - CREQ-131845
         - OS shall provide category 0 ISRs.
         - TSR-6
         - ASIL D

       * - CREQ-103
         - OS shall provide category 1 ISRs.
         - TSR-6
         - ASIL D

       * - CREQ-355
         - OS shall provide category 2 ISRs.
         - TSR-9,TSR-8,TSR-7,TSR-6
         - ASIL D

       * - CREQ-115
         - OS shall provide a service to execute a callback in category 2 ISRs.
         - TSR-6,TSR-9,TSR-7
         - ASIL D

       * - CREQ-16
         - OS shall provide a service to get the ID of the currently executing category 2 ISR.
         - TSR-7,TSR-6
         - ASIL D

       * - CREQ-24
         - OS shall handle un-configured interrupt sources.
         - TSR-6
         - ASIL D

       * - CREQ-78
         - OS shall provide a service to determine the interrupt source of a non-configured interrupt upon handling of such interrupt.
         - TSR-6
         - ASIL D

       * - CREQ-1200
         - OS shall allow usage of interrupt services before start and after shutdown of the OS.
         - TSR-1
         - ASIL D

       * - CREQ-154
         - OS shall provide a nestable service to disable all interrupts.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-98
         - OS shall provide a nestable service to enable all interrupts.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-151
         - OS shall provide a nestable service to disable all category 2 interrupts.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-82
         - OS shall provide a nestable service to enable all category 2 interrupts.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-111
         - OS shall provide a non nestable service to disable all interrupts.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-43
         - OS shall provide a non nestable service to enable all interrupts.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-1257
         - OS shall provide a non nestable service to disable all interrupts callable from kernel mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-1259
         - OS shall provide a non nestable service to enable all interrupts callable from kernel mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-1256
         - OS shall provide a non nestable service to disable all interrupts callable from user mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-1258
         - OS shall provide a non nestable service to enable all interrupts callable from user mode.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-108741
         - OS shall provide a non nestable service to disable all interrupts callable from any mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-108742
         - OS shall provide a non nestable service to enable all interrupts callable from any mode.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-108744
         - OS shall provide a non nestable service to disable all category 2 interrupts callable from kernel mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-108747
         - OS shall provide a non nestable service to enable all category 2 interrupts callable from kernel mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-108743
         - OS shall provide a non nestable service to disable all category 2 interrupts callable from user mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-108748
         - OS shall provide a non nestable service to enable all category 2 interrupts callable from user mode.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-108745
         - OS shall provide a non nestable service to disable all category 2 interrupts callable from any mode.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-108746
         - OS shall provide a non nestable service to enable all category 2 interrupts callable from any mode.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-Os-EnablePreStart
         - OS shall provide a service to enable interrupt handling before start of the OS.
         - TSR-6,TSR-101876
         - ASIL D

       * - CREQ-106181
         - OS shall provide a service to disable a specific interrupt source.
         - TSR-1,TSR-6
         - ASIL D

       * - CREQ-106182
         - OS shall provide a service to enable a specific interrupt source.
         - TSR-6,TSR-1
         - ASIL D

       * - CREQ-106183
         - OS shall provide a service to clear pending interrupts.
         - TSR-1,TSR-6
         - ASIL D

       * - CREQ-114872
         - OS shall provide a service to check whether or not the source of the given ISR is enabled.
         - TSR-6,TSR-1
         - ASIL D

       * - CREQ-114873
         - OS shall provide a service to check whether or not the given ISR has been requested.
         - TSR-1,TSR-6
         - ASIL D

       * - CREQ-183488
         - OS shall provide a service to enable multiple interrupt sources at once.
         - TSR-6
         - ASIL D

       * - CREQ-106
         - OS shall provide events.
         - TSR-6
         - ASIL D

       * - CREQ-68
         - OS shall provide a service to wait for the occurrence of events.
         - TSR-6
         - ASIL D

       * - CREQ-155
         - OS shall provide a service to signal the occurrence of events to a task.
         - TSR-6
         - ASIL D

       * - CREQ-53
         - OS shall provide a service to acknowledge the occurrence of events.
         - TSR-6
         - ASIL D

       * - CREQ-129
         - OS shall provide a service to get the event states of a given task.
         - TSR-6
         - ASIL D

       * - CREQ-133
         - OS shall provide a service to declare an event.
         - TSR-6
         - ASIL D

       * - CREQ-Os-AsynCrossCoreSetEvent
         - OS shall provide a service to set an event asynchronously.
         - TSR-6
         - ASIL D

       * - CREQ-140
         - OS shall provide counters.
         - TSR-111109,TSR-6
         - ASIL D

       * - CREQ-Os-CounterAlgorithm
         - OS shall provide different algorithms for working off alarms and schedule tables of a counter.
         - TSR-6
         - ASIL D

       * - CREQ-22
         - OS shall provide a service to increment a counter.
         - TSR-6
         - ASIL D

       * - CREQ-156
         - OS shall provide a service to get the current value of a counter.
         - TSR-6,TSR-111109
         - ASIL D

       * - CREQ-39
         - OS shall provide a service to get the difference between a given and the current counter value.
         - TSR-111109,TSR-6
         - ASIL D

       * - CREQ-44
         - OS shall provide a service for each hardware counter to translate a given period of time into number of ticks.
         - TSR-6
         - ASIL D

       * - CREQ-297
         - OS shall provide a service for each hardware counter to translate number of counter ticks into a period of time.
         - TSR-6
         - ASIL D

       * - CREQ-1260
         - OS shall provide a service to get the maximum possible value of a counter.
         - TSR-6
         - ASIL D

       * - CREQ-1261
         - OS shall provide a service to get the number of underlying driver ticks required to reach a specific unit.
         - TSR-6
         - ASIL D

       * - CREQ-1262
         - OS shall provide a service to get the minimum allowed number of ticks for a cyclic alarm of a counter.
         - TSR-6
         - ASIL D

       * - CREQ-138
         - OS shall provide alarms.
         - TSR-6
         - ASIL D

       * - CREQ-116
         - OS shall provide a service to set a relative alarm.
         - TSR-6
         - ASIL D

       * - CREQ-29
         - OS shall provide a service to set an absolute alarm.
         - TSR-6
         - ASIL D

       * - CREQ-93
         - OS shall provide a service to get an alarm.
         - TSR-6
         - ASIL D

       * - CREQ-164
         - OS shall provide a service to cancel an alarm.
         - TSR-6
         - ASIL D

       * - CREQ-19
         - OS shall provide a service to get the alarm base.
         - TSR-6
         - ASIL D

       * - CREQ-142
         - OS shall provide alarm callbacks.
         - TSR-6
         - ASIL D

       * - CREQ-32
         - OS shall provide a service to declare an alarm.
         - TSR-6
         - ASIL D

       * - CREQ-99
         - OS shall provide schedule tables.
         - TSR-6
         - ASIL D

       * - CREQ-298
         - OS shall provide expiry points.
         - TSR-6
         - ASIL D

       * - CREQ-14
         - OS shall provide single-shot schedule tables.
         - TSR-6
         - ASIL D

       * - CREQ-67
         - OS shall provide repeating schedule tables.
         - TSR-6
         - ASIL D

       * - CREQ-69
         - OS shall provide explicitly synchronized schedule tables.
         - TSR-6,TSR-111109
         - ASIL D

       * - CREQ-81
         - OS shall provide implicitly synchronized schedule tables.
         - TSR-6
         - ASIL D

       * - CREQ-61
         - OS shall provide a service to start a schedule table at a relative value.
         - TSR-6
         - ASIL D

       * - CREQ-136
         - OS shall provide a service to start a schedule table at an absolute value.
         - TSR-6
         - ASIL D

       * - CREQ-96
         - OS shall provide a service to stop the processing of a schedule table.
         - TSR-6
         - ASIL D

       * - CREQ-112
         - OS shall provide a service to switch the processing between different schedule tables.
         - TSR-6
         - ASIL D

       * - CREQ-100
         - OS shall provide a service to start an explicitly synchronized schedule table synchronously.
         - TSR-6
         - ASIL D

       * - CREQ-152
         - OS shall provide a service to synchronize a schedule table with a synchronization counter.
         - TSR-111109,TSR-6
         - ASIL D

       * - CREQ-25
         - OS shall provide a service to stop synchronization of a schedule table.
         - TSR-6
         - ASIL D

       * - CREQ-108
         - OS shall provide a service to query the state of a schedule table.
         - TSR-111109,TSR-6
         - ASIL D

       * - CREQ-36
         - OS shall provide a mechanism to coordinate concurrent access to shared resources.
         - TSR-6,TSR-8
         - ASIL D

       * - CREQ-157
         - OS shall provide linked resources.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-66
         - OS shall provide internal resources.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-56
         - OS shall provide a service to acquire a resource.
         - TSR-101876,TSR-8,TSR-6
         - ASIL D

       * - CREQ-107
         - OS shall provide a service to release a resource.
         - TSR-8,TSR-101876,TSR-6
         - ASIL D

       * - CREQ-163
         - OS shall provide a service to declare a resource.
         - TSR-6
         - ASIL D

       * - CREQ-15
         - OS shall provide spinlocks.
         - TSR-8
         - ASIL D

       * - CREQ-17
         - OS shall provide a service to acquire a spinlock.
         - TSR-8,TSR-101876,TSR-6
         - ASIL D

       * - CREQ-139
         - OS shall provide a service to asynchronously acquire a spinlock.
         - TSR-101876,TSR-6,TSR-8
         - ASIL D

       * - CREQ-167
         - OS shall provide a service to release a spinlock.
         - TSR-8,TSR-101876,TSR-6
         - ASIL D

       * - CREQ-208524
         - OS shall provide a service to synchronize participating tasks at a referenced barrier.
         - TSR-6
         - ASIL D

       * - CREQ-354
         - OS shall provide applications.
         - TSR-7,TSR-9,TSR-8
         - ASIL D

       * - CREQ-172
         - OS shall provide a service to determine the application ID to which the current execution context was configured.
         - TSR-9,TSR-7,TSR-6,TSR-8
         - ASIL D

       * - CREQ-60
         - OS shall provide a service to determine the application ID in which the current execution context is executed.
         - TSR-6,TSR-9,TSR-7,TSR-8
         - ASIL D

       * - CREQ-114
         - OS shall provide a service to make an application accessible.
         - TSR-9,TSR-8,TSR-6,TSR-7
         - ASIL D

       * - CREQ-109
         - OS shall provide a service to identify accessibility of OS objects .
         - TSR-9,TSR-8,TSR-7,TSR-6
         - ASIL D

       * - CREQ-18
         - OS shall provide a service to identify object ownership.
         - TSR-8,TSR-7,TSR-6,TSR-9
         - ASIL D

       * - CREQ-110
         - OS shall provide a service to terminate an application.
         - TSR-6,TSR-9,TSR-7,TSR-8
         - ASIL D

       * - CREQ-170
         - OS shall provide restart tasks.
         - TSR-7,TSR-6,TSR-9,TSR-8
         - ASIL D

       * - CREQ-104
         - OS shall provide a service to get the state of a given application.
         - TSR-7,TSR-6,TSR-8,TSR-9
         - ASIL D

       * - CREQ-34
         - OS shall provide a service to call exported services from trusted applications.
         - TSR-7,TSR-8,TSR-6,TSR-9
         - ASIL D

       * - CREQ-115372
         - OS shall allow usage of exported services from trusted applications before start of the OS.
         - TSR-7
         - ASIL D

       * - CREQ-105586
         - OS shall provide a service to call exported services from non-trusted applications.
         - TSR-7,TSR-6,TSR-8,TSR-9
         - ASIL D

       * - CREQ-105587
         - OS shall allow usage of exported services from non-trusted applications before start of the OS.
         - TSR-7
         - ASIL D

       * - CREQ-253949
         - OS shall provide a service to call lightweight exported services from trusted applications.
         - TSR-7,TSR-9,TSR-6,TSR-8
         - ASIL D

       * - CREQ-253950
         - OS shall allow usage of lightweight exported services from trusted applications before start of the OS.
         - TSR-7
         - ASIL D

       * - CREQ-48
         - OS shall provide an application specific callback during OS startup.
         - TSR-6,TSR-1
         - ASIL D

       * - CREQ-76
         - OS shall provide an application specific callback during OS shutdown.
         - TSR-6
         - ASIL D

       * - CREQ-54
         - OS shall provide an application specific callback if an error occurs.
         - TSR-6
         - ASIL D

       * - CREQ-844
         - OS shall provide global memory protection configuration.
         - TSR-7
         - ASIL D

       * - CREQ-41
         - OS shall provide memory protection configuration for applications.
         - TSR-7
         - ASIL D

       * - CREQ-62
         - OS shall provide memory protection configuration for tasks and category 2 ISRs.
         - TSR-7
         - ASIL D

       * - CREQ-73
         - OS shall provide a service to return the access rights of a memory access of a task.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-13
         - OS shall provide a service to return the access rights of a memory access of a category 2 ISR.
         - TSR-7,TSR-6
         - ASIL D

       * - CREQ-49
         - OS shall provide execution time protection.
         - TSR-8
         - ASIL D

       * - CREQ-85
         - OS shall provide inter-arrival time protection.
         - TSR-8
         - ASIL D

       * - CREQ-31
         - OS shall provide locking time protection.
         - TSR-8
         - ASIL D

       * - CREQ-845
         - OS shall monitor execution times.
         - TSR-8
         - ASIL D

       * - CREQ-846
         - OS shall monitor inter arrival time frames.
         - TSR-8
         - ASIL D

       * - CREQ-847
         - OS shall monitor locking times.
         - TSR-8
         - ASIL D

       * - CREQ-40
         - OS shall prevent OS-Applications from corrupting the OS through service calls.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-131
         - OS shall provide peripheral regions.
         - TSR-7
         - ASIL D

       * - CREQ-35
         - OS shall provide a service to modify a value in a peripheral region.
         - TSR-7,TSR-6
         - ASIL D

       * - CREQ-79
         - OS shall provide a service to read a value from a peripheral region.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-145
         - OS shall provide a service to write a value in a peripheral region.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-115373
         - OS shall allow usage of services for peripheral regions before start of the OS.
         - TSR-7
         - ASIL D

       * - CREQ-88
         - OS shall provide a stack monitoring mechanism.
         - TSR-7,TSR-6
         - ASIL D

       * - CREQ-26
         - OS shall be able to call a global callback function if an error occurs.
         - TSR-6
         - ASIL D

       * - CREQ-38
         - OS shall be able to call a global callback function if a fatal error occurs.
         - TSR-9,TSR-8,TSR-6
         - ASIL D

       * - CREQ-133061
         - OS shall be able to call a global callback function if an internal error occurs.
         - TSR-6
         - ASIL D

       * - CREQ-97
         - OS shall provide a service to all configured error callbacks, which return the parameters of the system service which triggered error handling.
         - TSR-6
         - ASIL D

       * - CREQ-23
         - OS shall provide a service to all configured error callbacks, which returns the service identifier where the error has been risen.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-102
         - OS shall provide information to determine the service and the cause of a reported error.
         - TSR-101876,TSR-6
         - ASIL D

       * - CREQ-129663
         - OS shall provide a service, which writes the context of the thread to which the system returns after error handling.
         - TSR-6
         - ASIL D

       * - CREQ-129664
         - OS shall provide a service, which returns the context of the thread which triggered a fatal error.
         - TSR-6
         - ASIL D

       * - CREQ-247263
         - OS shall provide a service, which returns the address of the latest instruction that caused a fatal error.
         - TSR-6
         - ASIL D

       * - CREQ-70
         - OS shall provide a service to forcibly terminate a task.
         - TSR-9,TSR-6
         - ASIL D

       * - CREQ-21
         - OS shall provide a service to forcibly terminate a category 2 ISR.
         - TSR-9,TSR-6
         - ASIL D

       * - CREQ-168
         - OS shall provide a service to select the idle mode action.
         - TSR-6
         - ASIL D

       * - CREQ-75
         - OS shall provide inter-application communication.
         - TSR-12,TSR-7
         - ASIL D

       * - CREQ-848
         - OS IOC shall provide queued communication.
         - TSR-12,TSR-6,TSR-7
         - ASIL D

       * - CREQ-849
         - OS IOC shall provide unqueued communication.
         - TSR-7,TSR-12
         - ASIL D

       * - CREQ-850
         - OS IOC shall provide 1:1 communication.
         - TSR-12,TSR-7
         - ASIL D

       * - CREQ-852
         - OS IOC shall provide N:1 communication.
         - TSR-7,TSR-12
         - ASIL D

       * - CREQ-851
         - OS IOC shall provide N:M communication.
         - TSR-7,TSR-12
         - ASIL D

       * - CREQ-150
         - OS shall provide a service to write data to an unqueued IOC channel.
         - TSR-6,TSR-7,TSR-12
         - ASIL D

       * - CREQ-55
         - OS shall provide a service to read data from a unqueued IOC channel.
         - TSR-7,TSR-6,TSR-12
         - ASIL D

       * - CREQ-91
         - OS shall provide a service to send data to a queued IOC channel.
         - TSR-7,TSR-6,TSR-12
         - ASIL D

       * - CREQ-160
         - OS shall provide a service to receive data from a queued IOC channel.
         - TSR-12,TSR-6,TSR-7
         - ASIL D

       * - CREQ-90
         - OS shall provide a service to write multiple data to an unqueued IOC channel.
         - TSR-7,TSR-6,TSR-12
         - ASIL D

       * - CREQ-147
         - OS shall provide a service to read multiple data from an unqueued IOC channel.
         - TSR-7,TSR-6,TSR-12
         - ASIL D

       * - CREQ-119
         - OS shall provide a service to send multiple data to a queued IOC channel.
         - TSR-6,TSR-7,TSR-12
         - ASIL D

       * - CREQ-113
         - OS shall service a method to receive multiple data from a queued IOC channel.
         - TSR-12,TSR-7,TSR-6
         - ASIL D

       * - CREQ-128
         - OS shall provide a service to clear all data from a queued IOC channel.
         - TSR-7,TSR-12,TSR-6
         - ASIL D

       * - CREQ-141
         - OS shall be able to call a callback function upon IOC data reception.
         - TSR-12,TSR-7,TSR-6
         - ASIL D

       * - CREQ-124
         - OS shall schedule tasks based on priorities.
         - TSR-6,TSR-8
         - ASIL D

       * - CREQ-86
         - OS shall provide full preemptive task scheduling.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-30
         - OS shall provide non preemptive task scheduling.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-229342
         - OS shall provide round robin scheduling.
         - TSR-6
         - ASIL D

       * - CREQ-149
         - OS shall provide a service to identify the local core.
         - TSR-6
         - ASIL D

       * - CREQ-148
         - OS shall provide a service to get the number of cores controlled by OS.
         - TSR-6
         - ASIL D

       * - CREQ-37
         - OS shall provide a service to start a core for usage of AUTOSAR OS software.
         - TSR-1,TSR-7,TSR-8,TSR-9,TSR-12,TSR-6
         - ASIL D

       * - CREQ-120
         - OS shall provide a service to start a core for usage of non AUTOSAR OS software.
         - TSR-1,TSR-6
         - ASIL D

       * - CREQ-115996
         - OS shall be able to initialize itself and the hardware on any of the available cores.
         - TSR-1
         - ASIL D

       * - CREQ-213720
         - OS shall provide a service to get the start state of a core.
         - TSR-6
         - ASIL D

       * - CREQ-130
         - OS shall allow to share stacks between tasks which do not preempt each other.
         - TSR-7
         - ASIL D

       * - CREQ-115010
         - OS shall provide a callback for signalling a task activation.
         - TSR-6
         - ASIL D

       * - CREQ-115028
         - OS shall provide a callback for signalling the setting of an event.
         - TSR-6
         - ASIL D

       * - CREQ-115029
         - OS shall provide a callback for signalling a thread switch.
         - TSR-6
         - ASIL D

       * - CREQ-115030
         - OS shall provide a callback for signalling forcible termination of a thread.
         - TSR-6
         - ASIL D

       * - CREQ-115031
         - OS shall provide a callback for signalling the acquirement of a resource.
         - TSR-6
         - ASIL D

       * - CREQ-115032
         - OS shall provide a callback for signalling the release of a resource.
         - TSR-6
         - ASIL D

       * - CREQ-115033
         - OS shall provide a callback for signalling the attempt to acquire a spinlock.
         - TSR-6
         - ASIL D

       * - CREQ-115034
         - OS shall provide a callback for signalling the acquirement of a spinlock.
         - TSR-6
         - ASIL D

       * - CREQ-115035
         - OS shall provide a callback for signalling the release of a spinlock.
         - TSR-6
         - ASIL D

       * - CREQ-115036
         - OS shall provide a callback for signalling the attempt to internally acquire a spinlock.
         - TSR-6
         - ASIL D

       * - CREQ-115037
         - OS shall provide a callback for signalling the internal acquirement of a spinlock.
         - TSR-6
         - ASIL D

       * - CREQ-115038
         - OS shall provide a callback for signalling the internal release of a spinlock.
         - TSR-6
         - ASIL D

       * - CREQ-115039
         - OS shall provide a callback for signalling the locking of interrupts.
         - TSR-6
         - ASIL D

       * - CREQ-115040
         - OS shall provide a callback for signalling the release of an interrupt lock.
         - TSR-6
         - ASIL D

       * - CREQ-140268
         - OS shall provide a callback for signalling failed task activation because the number of activations exceeds the limit.
         - TSR-6
         - ASIL D

       * - CREQ-140269
         - OS shall provide a callback for signalling that event is already set when WaitEvent is called.
         - TSR-6
         - ASIL D

       * - CREQ-Os-ArtiServiceCall
         - OS shall provide ARTI callbacks for the class OS_SERVICECALLS.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-Os-ArtiTask
         - OS shall provide ARTI callbacks for the class OSARTI_TASK.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-Os-ArtiCat2Isr
         - OS shall provide ARTI callbacks for the class OS_CAT2ISR.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-Os-ArtiSpinlock
         - OS shall provide ARTI callbacks for the class OS_SPINLOCK.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-Os-ArtiApplication
         - OS shall provide ARTI callbacks for the class OS_APPLICATION.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-Os-ArtiProtectionHook
         - OS shall provide ARTI callbacks for the class OS_PROTECTIONHOOK.
         - TSR-6,TSR-7
         - ASIL D

       * - CREQ-300
         - OS shall provide the state READY for tasks.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-302
         - OS shall provide the state RUNNING for tasks.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-303
         - OS shall provide the state SUSPENDED for tasks.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-301
         - OS shall provide the state WAITING for Tasks.
         - TSR-8,TSR-6
         - ASIL D

       * - CREQ-305
         - OS shall provide the state SCHEDULETABLE_NEXT for schedule tables.
         - TSR-8
         - ASIL D

       * - CREQ-308
         - OS shall provide the state SCHEDULETABLE_STOPPED for schedule tables.
         - TSR-8
         - ASIL D

       * - CREQ-309
         - OS shall provide the state SCHEDULETABLE_WAITING for schedule tables.
         - TSR-8
         - ASIL D

       * - CREQ-310
         - OS shall provide the state SCHEDULETABLE_RUNNING for schedule tables.
         - TSR-8
         - ASIL D

       * - CREQ-311
         - OS shall provide the state SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS for schedule tables.
         - TSR-8
         - ASIL D

       * - CREQ-1263
         - OS shall provide the state APPLICATION_ACCESSIBLE for applications.
         - TSR-8,TSR-7,TSR-6
         - ASIL D

       * - CREQ-1264
         - OS shall provide the state APPLICATION_RESTARTING for applications.
         - TSR-9,TSR-7,TSR-8,TSR-6
         - ASIL D

       * - CREQ-1265
         - OS shall provide the state APPLICATION_TERMINATED for applications.
         - TSR-9,TSR-7,TSR-6,TSR-8
         - ASIL D

       * - CREQ-1201
         - OS shall be executable on any combination of cores provided by the hardware.
         - TSR-7,TSR-12,TSR-1,TSR-4,TSR-8,TSR-9
         - ASIL D


    This component in addition to the assumed safety requirements ensures freedom from interference with respect to memory and timing with up to ASIL D.


Configuration Constraints
=========================

.. smi:: SMI-Os_30_TriCoreAurixGen7-378

    The user of MICROSAR Safe shall configure and verify the extended OS status of APIs.

    The attribute */MICROSAR/Os_Core/Os/OsOs/OsStatus* shall be equal to *EXTENDED*.

    The OS safety measures rely on the validity checks defined for EXTENDED status of OS API calls. Without these checks invalid calls might destroy the system integrity and violate safety requirements. Ensuring the validity of API calls and arguments in STANDARD status for any caller (which e.g. might be QM software) is considered to be infeasible.

    The user does not need to check this setting explicitly, as the MICROSAR Safe Silence Verifier (MSSV) assures that this setting is correct.


.. smi:: SMI-Os_30_TriCoreAurixGen7-377

    The user of MICROSAR Safe shall configure and verify the service protection.

    The attribute */MICROSAR/Os_Core/Os/OsOs/OsServiceProtection* shall be equal to *TRUE*.

    The OS safety measures rely on the validity checks defined for OsServiceProtection. Without these checks, invalid API calls might destroy the system integrity and violate safety requirements. Ensuring the validity of API calls for each caller (which might be QM software) is not considered feasible if OsServiceProtection is disabled.

    The user does not need to check this setting explicitly, as the MICROSAR Safe Silence Verifier (MSSV) assures that this setting is correct.


.. smi:: SMI-Os_30_TriCoreAurixGen7-379

    The user of MICROSAR Safe shall configure and verify the scalability class 3 or 4.

    The attribute */MICROSAR/Os_Core/Os/OsOs/OsScalabilityClass* shall be equal to *SC3* or *SC4*.

    The OS safety measures rely on memory protection and service protection provided by the scalability classes SC3 and SC4. Without memory protection, all software parts (even QM parts) would have to ensure freedom from interference regarding memory (including absence of stack overflow). Without service protection, all software parts (even QM parts) would have to ensure only calls with valid access rights.

    The user does not need to check this setting explicitly, as the MICROSAR Safe Silence Verifier (MSSV) assures that this setting is correct.


.. smi:: SMI-Os_30_TriCoreAurixGen7-385

    The user of MICROSAR Safe shall not use ISRs of category 1 if timing protection is configured.

    If a thread is killed because of timing protection, ISRs of category 1 might be aborted.

    A possible workaround is to use ISRs of category 2 instead of category 1.


Additional Verification Measures
================================

.. smi:: SMI-Os_30_TriCoreAurixGen7-380

    The user of MICROSAR Safe shall ensure the correct usage of the OS regarding program flow. The correct program flow is only ensured if all OS API functions fulfill the expectations of the user application designer and are used correctly according to the AUTOSAR OS specification and the technical reference.

    This requirement is fulfilled by all MICROSAR Safe components. It only applies if APIs of the operating system are used directly by the user of MICROSAR Safe.

    This may be checked by monitoring the program flow at development time. Another measure may be, to check whether the API functions have the expected effects by double checking the technical reference.


.. smi:: SMI-Os_30_TriCoreAurixGen7-3732

    The user of MICROSAR Safe shall ensure the correct usage of the hardware peripherals.
    It is assumed that user software uses the microcontroller exactly as specified in the vendors hardware documentation. Hardware peripheral access may be performed using the MICROSAR OS Peripheral Access APIs.


.. smi:: SMI-Os_30_TriCoreAurixGen7-383

    The user of MICROSAR Safe shall not call OS hook functions directly. The OS hook functions shall be called by the OS only. This applies to the following hook functions:

    - *StartupHook*
    - *ShutdownHook*
    - *ProtectionHook*
    - *PreTaskHook*
    - *PostTaskHook*
    - *ErrorHook*
    - *PanicHook*

    The OS makes assumptions which are valid if these hook functions are called by the OS. These assumptions might be violated if hook functions are called directly by the user. As a hook may expect that it is called from a specific context, hooks shall not be called from user code directly.


.. smi:: SMI-Os_30_TriCoreAurixGen7-1047

    The user of MICROSAR Safe shall verify that the context described in the Hardware Software Interface (HSI) of the used platform is sufficient for the requirements of his application. The context is derived from the EABI of the platform as well as the calling conventions of the compiler. Discrepancies may occur in case a coprocessor is used, assembler code uses additional registers or precompiled libraries use different registers.


.. smi:: SMI-Os_30_TriCoreAurixGen7-100816

    The user of MICROSAR Safe shall assure that the application initializes the user controlled processor state bits before Os_Init(). The user controlled processor state bits are transfered into every new context as a copy from the previous context. Changing the user controlled processor state bits during runtime may lead to unexpected behavior of the system.

    The technical reference of the platform describes all processor state bits controlled by the OS. All other bits are either under user or hardware control. Refer to the hardware manual for a detailed description.


.. smi:: SMI-Os_30_TriCoreAurixGen7-592

    The user of MICROSAR Safe shall verify that software, which implements safety functionality, shall not rely on functionality provided by the following APIs:

    - *Os_GetVersionInfo*
    - Stack Usage API (*Os_Get\*StackUsage*)
    - *Os_InitInterruptOnly*



Interrupt handling
------------------

.. smi:: SMI-Os_30_TriCoreAurixGen7-381

    The user of MICROSAR Safe shall ensure the correct usage of the OS regarding interrupt disabling. Unintended disabling of interrupts may lead to timing inconsistencies because pending interrupts might be delayed.

    The following interrupt disabling API functions shall be used correctly according to the AUTOSAR OS specification:

    - *DisableAllInterrupts*
    - *SuspendAllInterrupts*
    - *SuspendOSInterrupts*

    The user shall select the function that is in accordance to the system design expectations.


.. smi:: SMI-Os_30_TriCoreAurixGen7-382

    The user of MICROSAR Safe shall ensure the correct usage of the OS regarding interrupt enabling. Unintended enabling of interrupts may lead to timing inconsistencies (because interrupts, which should be disabled, might occure) and data inconsistencies (see also SMI-11).

    The following interrupt enabling API functions shall be used correctly according to the AUTOSAR OS specification:

    - *EnableAllInterrupts*
    - *ResumeAllInterrupts*
    - *ResumeOSInterrupts*

    The user shall select the function that is in accordance to the system design expectations.


.. smi:: SMI-Os_30_TriCoreAurixGen7-482

    The user of MICROSAR Safe shall verify that the following API functions:

    - *Os_DisableLevel[KM|UM|AM]*
    - *Os_EnableLevel[KM|UM|AM]*
    - *Os_DisableGlobal[KM|UM|AM]*
    - *Os_EnableGlobal[KM|UM|AM]*

    are never called in the following cases:

    - within critical sections
    - nested within other interrupt APIs
    - within a section where an interrupt resource is locked
    - within interrupt locking spinlocks
    - within ISRs, Hook functions, non-trusted functions, trusted functions and alarm callbacks

    Furthermore, the disable functions shall only be called when the interrupts are enabled.


.. smi:: SMI-Os_30_TriCoreAurixGen7-488

    The user of MICROSAR Safe shall verify that the following API functions are only called from privileged mode:

    - *Os_DisableLevelKM*
    - *Os_EnableLevelKM*
    - *Os_DisableGlobalKM*
    - *Os_EnableGlobalKM*


.. smi:: SMI-Os_30_TriCoreAurixGen7-489

    The user of MICROSAR Safe shall verify that the API functions *Os_EnableGlobal[KM|UM|AM]* are only called if interrupts were disabled previously by calling the corresponding *Os_DisableGlobal[KM|UM|AM]* function.


.. smi:: SMI-Os_30_TriCoreAurixGen7-490

    The user of MICROSAR Safe shall verify that the API functions *Os_EnableLevel[KM|UM|AM]* are only called if interrupts were disabled previously by calling the corresponding *Os_DisableLevel[KM|UM|AM]* function.


.. smi:: SMI-Os_30_TriCoreAurixGen7-44677

    The user of MICROSAR Safe shall verify that all APIs called in ISRs of category 0 are allowed to be called in this context by the technical reference.

    The call context „inside category 0 ISR" cannot be checked by the API functions. Erroneous calls are not detected.


.. smi:: SMI-Os_30_TriCoreAurixGen7-590

    The user of MICROSAR Safe shall verify that all APIs called in ISRs of category 1 are allowed to be called in this context by the AUTOSAR specification and the technical reference.

    The call context „inside category 1 ISR" cannot be checked by the API functions. Erroneous calls are not detected.


.. smi:: SMI-Os_30_TriCoreAurixGen7-44676

    The user of MICROSAR Safe shall verify that all ISRs of category 0 are implemented transparent. Transparent means, that the execution of a category 0 interrupt can not be detected by the OS. Thus, the user has to ensure, that the context described in the HSI of the platform remains unchanged or is restored correctly after the execution of a category 0 ISR.


.. smi:: SMI-Os_30_TriCoreAurixGen7-541

    The user of MICROSAR Safe shall verify that all ISRs of category 1 are implemented transparent. Transparent means, that the execution of a category 1 interrupt can not be detected by the OS. Thus, the user has to ensure, that the context described in the HSI of the platform remains unchanged or is restored correctly after the execution of a category 1 ISR.


.. smi:: SMI-Os_30_TriCoreAurixGen7-491

    The user of MICROSAR Safe shall verify the functionality of each configured ISR.

    This includes the call of the correct ISR handler with the expected priority. In case  that the following APIs are used, the user has to check that the correct ISR is influenced.

    - Os_InitialEnableInterruptSources
    - Os_EnableInterruptSource
    - Os_DisableInterruptSource
    - Os_ClearPendingInterrupt

    The aim of the check is to make sure, that no interrupt sources or vectors are mixed up. Therefore, it is sufficient to perform the check with one of the mentioned functions for each ISR.


.. smi:: SMI-Os_30_TriCoreAurixGen7-44675

    The user of MICROSAR Safe shall avoid forcible termination by means of the protection reaction of the *ProtectionHook* in case category 0 ISRs are configured.

    If a category 0 ISR causes the *ProtectionHook* call, the interrupted Task or ISR would be terminated together with the category 0 ISR.
    This situation should be handled safely in the application design.


.. smi:: SMI-Os_30_TriCoreAurixGen7-44678

    The user of MICROSAR Safe shall avoid forcible termination by means of the protection reaction of the *ProtectionHook* in case category 1 ISRs are configured.

    If a category 1 ISR causes the *ProtectionHook* call, the interrupted Task or ISR would be terminated together with the category 1 ISR.
    This situation should be handled safely in the application design.


.. smi:: SMI-Os_30_TriCoreAurixGen7-44681

    The user of MICROSAR Safe shall be aware that category 0 ISRs cannot be disabled or suspended by the OS interrupt APIs.
    The following APIs have no effect on category 0 ISRs:

    - *DisableAllInterrupts*
    - *EnableAllInterrupts*
    - *SuspendAllInterrupts*
    - *ResumeAllInterrupts*
    - *SuspendOSInterrupts*
    - *ResumeOSInterrupts*
    - *Os_DisableLevelAM*
    - *Os_EnableLevelAM*
    - *Os_DisableLevelKM*
    - *Os_EnableLevelKM*
    - *Os_DisableLevelUM*
    - *Os_EnableLevelUM*
    - *Os_DisableGlobalAM*
    - *Os_EnableGlobalAM*
    - *Os_DisableGlobalKM*
    - *Os_EnableGlobalKM*
    - *Os_DisableGlobalUM*
    - *Os_EnableGlobalUM*

    The OS provides no measures to ensure data consistency for variables shared between category 0 ISRs and other Tasks or ISRs. Such measures need to be implemented by the user.


.. smi:: SMI-Os_30_TriCoreAurixGen7-334973

    The user of MICROSAR Safe shall verify that the time between two high resolution timer interrupts is sufficiently small.

    In case of a timer of at least 32 bits, the time must not be bigger than 2,147,483,647 hardware ticks.

    In case of a timer with less then 32 bits, the time must not be bigger than 2^(n-1) - 1 hardware ticks, where n is the register bit width of the hardware timer module.

    Timer interrupts can be delayed by disabling or suspending the interrupt handling or by other interrupts with higher priority.

    High resolution timer interrupts are used as Timing Protection interrupts. Additionally, they are used for OS Counters which have the high resolution timer flag enabled.


Memory mapping and linking
--------------------------

.. smi:: SMI-Os_30_TriCoreAurixGen7-340

    The user of MICROSAR Safe shall verify that the complete range specified by each *Os_PeripheralConfigType* object in *Os_Peripheral_Lcfg.c* is intended to be accessed by the (non-trusted) callers of the peripheral API. The first writable address is denoted as *AddressStart* and the last writable address is denoted as *AddressEnd*.


.. smi:: SMI-Os_30_TriCoreAurixGen7-494

    The user of MICROSAR Safe shall verify for IOC functions, that the configured access rights and linker configuration allow only valid callers to write the corresponding IOC data.

    Valid callers are the configured sender applications for the corresponding IOC send data structure and the configured receiver applications for the corresponding IOC data structure.

    In order to optimize IOC functionality, the OS links the IOC data structures to the sender/receiver application in many cases. In these cases, the user has to ensure, that no other application has write access to this memory section.
    In case the IOC data structures are linked to the OS data section, the OS guarantees that only valid senders/receivers can write to the IOC data structure.

    The user may find all IOC data structures in the generated file Os_Ioc_Lcfg.c as dynamic objects.


.. smi:: SMI-Os_30_TriCoreAurixGen7-495

    The user of MICROSAR Safe shall ensure by linkage for each optimized spinlock that only intended tasks and ISRs have write access to the corresponding spinlock data (or at least only tasks and ISRs of partitions with the same ASIL levels).

    The user of MICROSAR Safe shall verify that no unintended task or ISR has access to data of optimized spinlocks.


.. smi:: SMI-Os_30_TriCoreAurixGen7-539

    The user of MICROSAR Safe shall verify that no configured MPU/MMU region grants write access to OS variables from non-trusted software.


.. smi:: SMI-Os_30_TriCoreAurixGen7-549

    The user of MICROSAR Safe shall verify that none of the configured MPU/MMU regions grants write access to a memory section where OS stacks are linked to. To fulfill this requirement, the generated map file may be helpful.


.. smi:: SMI-Os_30_TriCoreAurixGen7-1045

    The user of MICROSAR Safe shall verify that the linkage scheme includes a stack safety gap linked adjacent to the stack section (in dependency of the stack growth direction, see technical reference). No software parts shall have write access to the stack safety gap.

    This measure allows the MPU/MMU to detect stack overflows even if the owner of the stack has write access to data linked adjacent to the stack section.


.. smi:: SMI-Os_30_TriCoreAurixGen7-564

    The user of MICROSAR Safe shall verify the configuration of access rights to sections.
    A precondition to this is, that a concept exists, which describes the partitioning of the address range and the access rights to the partitions for different parts of the software.
	
    This can be done by stopping the execution and checking the MPU/MMU register values. Dependent on the granularity of the access right configuration, this should be done globally (e.g. static MPU regions), for each OS Application or for each Task and each ISR. 


.. smi:: SMI-Os_30_TriCoreAurixGen7-109809

    The user of MICROSAR Safe shall ensure that the whole OS code is linked within OS start and end code labels as described in the technical reference. The start and end labels are generated into the array *OsCfg_OsCode_Section<x>*. This array may be found in Os_Error_Lcfg.c.

    Exceptions which occur in this address range will lead to the safe state of the OS, while other exceptions cause a call of the *ProtectionHook*.


Stack
-----

.. smi:: SMI-Os_30_TriCoreAurixGen7-565

    The user of MICROSAR Safe shall ensure that sufficient stack is available for call/execution of StartOS.

    StartOS performs some initializations before switching to an internal stack and enabling the memory protection. The active stack at call of StartOS shall provide sufficient space to execute this code. Because the stack consumption depends on the compiler and the compiler options, it is recommended to switch to a stack provided by the OS before calling StartOS and to use the stack usage measurement API of the OS to determine the necessary stack size.


.. smi:: SMI-Os_30_TriCoreAurixGen7-4452

    If the attribute */MICROSAR/Os/OsOS/OsGenerateMemMap* is equal to *USERCODE_AND_STACKS_GROUPED_PER_CORE*, the user of MICROSAR Safe shall ensure that all configured stack sizes match the MPU/MMU granularity and alignment. Otherwise stack protection cannot be ensured.


Multicore systems with mixed diagnostic coverage capability
-----------------------------------------------------------

.. smi:: SMI-Os_30_TriCoreAurixGen7-483

    The user of MICROSAR Safe shall ensure that frequent cross core API calls from cores with lower diagnostic coverage to cores with higher diagnostic coverage do not interfere with the application design.
    Excessive runtime consumption of cores with lower diagnostic coverage shall not prevent cores with higher diagnostic coverage from keeping the timing constraints.

    One possible measure is to use timing protection for the X-Signal ISRs. Furthermore, X-Signal Receiver ISRs may be split among different sender cores and provided API functions may be restricted.


.. smi:: SMI-Os_30_TriCoreAurixGen7-484

    The user of MICROSAR Safe shall ensure that synchronous cross core API calls from  a core with higher diagnostic coverage to a core with lower diagnostic coverage do not violate the safety goals in case the API calls never return.

    Synchronous calls block the caller until the return result is received. If, for any reason, a core with lower diagnostic coverage does not return the result or does not return the result in time, the caller has to deal with this situation.

    One possible measure is to use asynchronous X-Signal calls.


.. smi:: SMI-Os_30_TriCoreAurixGen7-485

    The user of MICROSAR Safe shall call *ShutdownAllCores* only on cores with the highest diagnostic coverage.


.. smi:: SMI-Os_30_TriCoreAurixGen7-486

    The user of MICROSAR Safe shall note that the *ShutdownHook* might not be called during Shutdown on multicore systems with mixed diagnostic coverage capability.

    Errors caused by cores with lower diagnostic coverage capabilities (data overwrite) might prevent the call of the *ShutdownHook* by cores with higher diagnostic coverage.

    Note that the AUTOSAR OS specification requires a synchronization barrier before the *ShutdownHook*. If one core does not reach the barrier, no core will reach the *ShutdownHook*.


.. smi:: SMI-Os_30_TriCoreAurixGen7-487

    The user of MICROSAR Safe shall configure and verify that the core with the highest diagnostic coverage initializes the peripheral moduls used by the OS (e.g. MPU, Interrupt Controller).
    The attribute */MICROSAR/Os/OsOS/OsHardwareInitCore* shall be set to the core reference with the highest diagnostic coverage.
    The user of MICROSAR Safe shall ensure, that, if a core with lower diagnostic coverage initializes peripherals or hardware components (like e.g. a system MPU), the core with higher diagnostic coverage does not rely on this initialization.


.. smi:: SMI-Os_30_TriCoreAurixGen7-493

    The user of MICROSAR Safe shall verify that the configuration of cross core API calls prevents cores with lower diagnostic coverage from shutting down cores with higher diagnostic coverage.

    Mind that X-Signal Receiver ISRs may be split among different sender cores and provided API functions may be restricted.


.. smi:: SMI-Os_30_TriCoreAurixGen7-25766

    The user of MICROSAR Safe shall ensure that the receiver core of cross core API calls is able to handle unintended calls of APIs. This applies only to APIs which are allowed to be called between two cores by configuration.


(Non-)Trusted Functions
-----------------------

.. smi:: SMI-Os_30_TriCoreAurixGen7-497

    The user of MICROSAR Safe shall verify that a trusted, non-trusted or fast trusted function checks the validity of passed arguments before usage.
    The caller of CallTrustedFunction, Os_CallNonTrustedFunction or Os_CallFastTrustedFunction might be non-trusted. Therefore, the passed data might also not be trustworthy.


.. smi:: SMI-Os_30_TriCoreAurixGen7-542

    The user of MICROSAR Safe shall verify that each caller of a trusted, non-trusted or fast trusted function is allowed to call the function, or the function validates the caller before performing its functionality to prevent from any violation of safety goals.


.. smi:: SMI-Os_30_TriCoreAurixGen7-95699

    The user of MICROSAR Safe shall verify that the caller Task/ISR of each trusted, non-trusted or fast trusted function which uses the FPU, is configured to use the FPU context.


Miscellaneous
-------------

.. smi:: SMI-Os_30_TriCoreAurixGen7-480

    The user of MICROSAR Safe shall not rely on the error parameter macros.
    These are named: OSError_<API-name>_<parameter-name>().
    They are not assumed as safety requirements by Vector.


.. smi:: SMI-Os_30_TriCoreAurixGen7-481

    The user of MICROSAR Safe shall notify that the *PanicHook* might not be called if the active thread is not allowed to modify the interrupt enable/disable state.

    Before calling the *PanicHook*, the OS disables all interrupts. If this fails, the *ProtectionHook* might be called, caused by the illegal access (depending on hardware platform).


.. smi:: SMI-Os_30_TriCoreAurixGen7-492

    The user of MICROSAR Safe shall verify for cross core API calls that for each pair of sender/receiver cores at least one API call is tested and verified across these cores.
    The test shall be performed for both directions.


.. smi:: SMI-Os_30_TriCoreAurixGen7-496

    The user of MICROSAR Safe shall verify that calls of the optimized spinlock API don’t violate any of the spinlock API constraints (e.g. the order of locking, no simultaneous locks from the same core, allowed caller). The optimized spinlock API skips any checks and therefore does not detect wrong calls.
    Mind that the order of GetSpinlock calls also depends on preemption of tasks and ISRs.


.. smi:: SMI-Os_30_TriCoreAurixGen7-591

    The user of MICROSAR Safe shall verify the correct usage of IOC API functions. These functions don’t call the ErrorHook. It is recommended to check the return code of these functions:

    - IocSend/IocWrite
    - IocSendGroup/IocWriteGroup
    - IocReceive/IocRead
    - IocReceiveGroup/IocReadGroup


.. smi:: SMI-Os_30_TriCoreAurixGen7-109842

    The user of MICROSAR Safe shall be aware that spinlocks occupied by an IOC channel are not released when the communicating task or ISR is terminated via protection hook.
    The operating system does not guarantee that calls to the IOC API for channels, which have blocked spinlocks, will return.
    Spinlocks in IOC APIs are, for example, used when

    -	An IOC channel is non-queued or
    -	Multiple senders are configured for the same queued channel


.. smi:: SMI-Os_30_TriCoreAurixGen7-540

    The user of MICROSAR Safe shall verify that the user software does not contain system call instructions.

    Any system call instruction will result in an OS API or in an OS Error. If the user code uses a system call instruction directly, the triggered OS API will most likely not work as expected. Instead, system calls shall only be used by OS APIs.

    A possible verification method might be reviewing the code for (inline) assembler statements, pragmas or intrinsic functions containing system call instructions.

    System calls used by the OS are described in the technical reference.


.. smi:: SMI-Os_30_TriCoreAurixGen7-2900

    The user of MICROSAR Safe shall verify that the array OsCfg_CorePhysicalRefs contains all physical cores. The array may be found in Os_Hal_Kernel_Lcfg.c.
    For each existing physical hardware core identifier there shall be one corresponding entry inside the array. This entry is indexed by the physical hardware core identifier which is provided directly by the hardware registers.


.. smi:: SMI-Os_30_TriCoreAurixGen7-39288

    The user of MICROSAR Safe shall verify that the array OsCfg_Hal_Context_ExceptionContextRef contains all physical cores. The array may be found in Os_Hal_Context_Lcfg.c.
    For each existing physical hardware core identifier, which is also an Autosar core, there shall be one corresponding entry (not NULL_PTR) inside the array. This entry is indexed by the physical hardware core identifier which is provided directly by the hardware registers.


.. smi:: SMI-Os_30_TriCoreAurixGen7-487587

    The user of MICROSAR Safe shall verify the expected behavior of the system if the following OS APIs are used:

    - Os_GetExceptionContext
    - Os_SetExceptionContext

    The OS is not able to guarantee the correctness of the context structure for each possible situation. Thus, the user of MICROSAR Safe needs to assure the functionality for his use case on his own. For this, the values of the members of the structure Os_ExceptionContextType need to be checked for correctness. In case of Os_SetExceptionContext usage, the register values need to be checked against the values in the context structure after returning. Os_GetExceptionContext usage can be checked by preparing a context, triggering an exception and checking the returned values by a runtime test or with the help of a debugger. For Os_SetExceptionContext a breakpoint may be set to the return address in order to check the restored register values against the values in the context structure.


.. smi:: SMI-Os_30_TriCoreAurixGen7-44342

    The user of MICROSAR Safe shall verify that peripheral APIs

    - Os_ReadPeripheral8Legacy
    - Os_ReadPeripheral16Legacy
    - Os_ReadPeripheral32Legacy
    - Os_WritePeripheral8Legacy
    - Os_WritePeripheral16Legacy
    - Os_WritePeripheral32Legacy
    - Os_ModifyPeripheral8Legacy
    - Os_ModifyPeripheral16Legacy
    - Os_ModifyPeripheral32Legacy

    are not used on platforms which address width is greater than 32 bits.


.. smi:: SMI-Os_30_TriCoreAurixGen7-44679

    The user shall ensure real time behavior of the system, even in case of delayed calls of the *ProtectionHook*.
    The *ProtectionHook* may be delayed by the execution of Cat 0 ISRs. So Cat 0 ISRs should either not be used or assure a sufficiently short runtime.


.. smi:: SMI-Os_30_TriCoreAurixGen7-109810

    The user of MICROSAR Safe shall be aware that in case that MICROSAR OS detects a potentially internal inconsistency, MICROSAR OS enters the PanicHook. The PanicHook is executed with all interrupts disabled. If the PanicHook returns, the OS enters an endless loop with all interrupts disabled. This is assumed to be the safe state of the ECU.


.. smi:: SMI-Os_30_TriCoreAurixGen7-429122

    The user of MICROSAR Safe shall verify that the type Os_AppAccessMaskType is generated in accordance to the number of configured OS Applications and that OS_APPMASK_BITWIDTH is defined in accordance to the processors bit width.

    The definition for OS_APPMASK_BITWIDTH may be found in Os_Application_Lcfg.h:
    #define OS_APPMASK_BITWIDTH        (<BitWidth>u)

    The type definition for AppAccessMaskType may be found in Os_Types_Lcfg.h.

    If OS_APPID_COUNT is larger than <BitWidth>:

    typedef P2CONST(uint<BitWidth>, TYPEDEF, OS_CONST)  Os_AppAccessMaskType;

    Otherwise:

    typedef uint<BitWidth>  Os_AppAccessMaskType;

    Where <BitWidth> is the bit width of the processor (32 or 64).


.. smi:: SMI-Os_30_TriCoreAurixGen7-1226694

    The user of MICROSAR Safe shall be aware that when using an asynchronous cross core API, the error messages will not be reported directly and might be delayed. For asynchronous cross core APIs, the caller core does only trigger an action on the destination core without waiting for the result, as described in the TechRef in the chapter about X-Signal. Some of the error checks need to be performed on the destination core. As a result, the reaction on the caller core for errors, detected on the destination core, is delayed. The ErrorHook call will occur at a subsequent call of a cross core API, which uses the same X-Signal channel.


Tracing
-------

.. smi:: SMI-Os_30_TriCoreAurixGen7-69754

    The user of MICROSAR Safe shall be aware that user timing or ARTI hook implementation influences the runtime behaviour of the system.


.. smi:: SMI-Os_30_TriCoreAurixGen7-69755

    The user of MICROSAR Safe shall not use any OS API within timing or ARTI hooks.


Barriers
--------

.. smi:: SMI-Os_30_TriCoreAurixGen7-309405

    The user of MICROSAR Safe shall ensure, that the number of calls to the API *Os_BarrierSynchronize* is identical for all participating tasks of the referenced barrier.
    Otherwise, a participating task may block an entire core, which results in unpredictable behaviour.


Inter OS Application Communication (IOC)
----------------------------------------

This chapter describes the review of the correctness of the locking mode generation in file Os_Ioc_Lcfg.c for all configured senders, writers, receivers and readers of IOC communications. In order to keep each safety manual item sufficiently simple, the review has been split into several items. However, it may make sense, to scan through the file for any sender, writer, receiver or reader configuration structures and to perform the respective review on them. Typically two safety manual items will describe the complete review for a specific sender, writer, receiver or reader. For the receivers of queued IOC communications, only one safety manual item needs to be considered.

.. smi:: SMI-Os_30_TriCoreAurixGen7-889322

    The user of MICROSAR Safe shall verify the consistency of the LockMode and the Spinlock pointer for all generated configuration structures of readers on IOC communications. The configuration data structure of readers in  Os_Ioc_Lcfg.c looks like:

    .. code-block::

        CONST(Os_IocReadConfigType, OS_CONST) OsCfg_IocRecv_<NameOfIocCommunication> =
        {

          /* .Ioc      = */
          {

            /* .Spinlock              = */ <SpinlockPointer>,
            /* .LockMode              = */ <LockModeValue>,
            ...
            /* .AccessingApplications = */ OS_APPID2MASK(<NameOfOsApplication>)
                                           <|OS_APPID2MASK(<NameOfOsApplication>)>*

          },
          ...

       };

    Where <NameOfIocCommunication> is the configured short name of a container with definition ref: /MICROSAR/Os/OsIoc/OsIocCommunication and <NameOfOsApplication> is the short name of a configuration container, reffered to by the parameter: /MICROSAR/Os/OsIoc/OsIocCommunication/OsIocReceiverProperties/OsIocReceivingOsApplicationRef of the Ioc communication. The values at <SpinlockPointer> and <LockModeValue> shall be reviewed against the table for consistency:

    .. list-table::
       :header-rows: 1

       * - <LockModeValue>
         - <SpinlockPointer>

       * - OS_IOCLOCKMODE_NONE
         - NULL_PTR

       * - OS_IOCLOCKMODE_INTERRUPTS
         - NULL_PTR

       * - OS_IOCLOCKMODE_SPINLOCKS
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>

       * - OS_IOCLOCKMODE_ALL
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>


.. smi:: SMI-Os_30_TriCoreAurixGen7-889323

    The user of MICROSAR Safe shall verify the consistency of the LockMode and the Spinlock pointer for all generated configuration structures of writers on IOC communications. The configuration structure of writers in Os_Ioc_Lcfg.c looks like:

    .. code-block::

        CONST(Os_IocWriteConfigType, OS_CONST)
        OsCfg_IocSend_<NameOfIocCommunication><_<OsIocSenderId>>? =
        {

          /* .Ioc       = */
          {

            /* .Spinlock              = */ <SpinlockPointer>,
            /* .LockMode              = */ <LockModeValue>,
            ...
            /* .AccessingApplications = */ OS_APPID2MASK(<NameOfOsApplication>)

          },
          ...

        };

    Where <NameOfIocCommunication> is the configured short name of a container with definition ref: /MICROSAR/Os/OsIoc/OsIocCommunication, <NameOfOsApplication> is the short name of a configuration container, reffered to by the parameter: /MICROSAR/Os/OsIoc/OsIocCommunication/OsIocSenderProperties/OsIocSendingOsApplicationRef and <_<OsIocSenderId>>? is the value of a parameter with definition ref: /MICROSAR/Os/OsIoc/OsIocCommunication/OsIocSenderProperties/OsIocSenderId of that IOC communication. This part of the name may not be available in 1:1 communication as the parameter needs not be configured then. The values at <SpinlockPointer> and <LockModeValue> shall be reviewed against the table for consistency:

    .. list-table::
       :header-rows: 1

       * - <LockModeValue>
         - <SpinlockPointer>

       * - OS_IOCLOCKMODE_NONE
         - NULL_PTR

       * - OS_IOCLOCKMODE_INTERRUPTS
         - NULL_PTR

       * - OS_IOCLOCKMODE_SPINLOCKS
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>

       * - OS_IOCLOCKMODE_ALL
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>


.. smi:: SMI-Os_30_TriCoreAurixGen7-889324

    The user of MICROSAR Safe shall verify the correct generation of the LockMode for all readers and writers of a specific IOC communication. See SMI-889322 and SMI-889323 for the relevant configuration structures. The value for <LockMode> shall be identical for all readers and writers and be in accordance to the table:

    .. list-table::
       :header-rows: 1

       * - Data can be read/written atomically
         - Core local and/or Cross-Core communication
         - <LockMode>

       * - yes
         - don't care
         - OS_IOCLOCKMODE_NONE

       * - no
         - All writer and reader applications are located on the same core.
         - OS_IOCLOCKMODE_INTERRUPTS

       * - no
         - All writer and reader applications are located on different cores.
         - OS_IOCLOCKMODE_SPINLOCKS

       * - no
         - Some writer and/or reader applications are located on the same core, while others are located on different cores.
         - OS_IOCLOCKMODE_ALL


.. smi:: SMI-Os_30_TriCoreAurixGen7-889325

    The user of MICROSAR Safe shall verify the consistency of the LockMode and the Spinlock pointer for all generated configuration structures of receivers on IOC communications. The configuration data structure of receivers in  Os_Ioc_Lcfg.c looks like:

    .. code-block::

        CONST(Os_Ioc<Bytes>ReceiveConfigType, OS_CONST)
        OsCfg_IocRecv_<NameOfIocCommunication> =
        {

          /* .Ioc      = */
          {

            /* .Base     = */
            {

              /* .Spinlock              = */ <SpinlockPointer>,
              /* .LockMode              = */ <LockModeValue>,
              ...
              /* .AccessingApplications = */ OS_APPID2MASK(<NameOfOsApplication>)

            },
            ...

          },
          ...

        };

    Where <NameOfIocCommunication> is the configured short name of a container with definition ref: /MICROSAR/Os/OsIoc/OsIocCommunication. The value of <Bytes> is one of: { 08, 16, 32, Ref }. The value <NameOfOsApplication> is the short name of a configuration container, reffered to by the parameter: /MICROSAR/Os/OsIoc/OsIocCommunication/OsIocReceiverProperties/OsIocReceivingOsApplicationRef of the Ioc communication. The values at <SpinlockPointer> and <LockModeValue> shall be reviewed against the table for consistency:

    .. list-table::
       :header-rows: 1

       * - <LockModeValue>
         - <SpinlockPointer>

       * - OS_IOCLOCKMODE_NONE
         - NULL_PTR

       * - OS_IOCLOCKMODE_INTERRUPTS
         - NULL_PTR

       * - OS_IOCLOCKMODE_SPINLOCKS
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>

       * - OS_IOCLOCKMODE_ALL
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>


.. smi:: SMI-Os_30_TriCoreAurixGen7-889326

    The user of MICROSAR Safe shall verify the consistency of the LockMode and the Spinlock pointer for all generated configuration structures of senders on IOC communications. The configuration structure of senders in Os_Ioc_Lcfg.c looks like:

    .. code-block::

        CONST(Os_Ioc<Bytes>SendConfigType, OS_CONST)
        OsCfg_IocSend_<NameOfIocCommunication><_<OsIocSenderId>>? =
        {

          /* .Ioc       = */
          {

            /* .Base     = */
            {

              /* .Spinlock              = */ <SpinlockPointer>,
              /* .LockMode              = */ <LockModeValue>,
              ...
              /* .AccessingApplications = */ OS_APPID2MASK(<NameOfOsApplication>)

            },
            ...

          },
          /* .FifoWrite = */
          {

            ...
            /* .Data     = */ <DataBufferPointer>,
            ...

          }

        };

    Where <NameOfIocCommunication> is the configured short name of a container with definition ref: /MICROSAR/Os/OsIoc/OsIocCommunication and <_<OsIocSenderId>>? is the value of a parameter with definition ref: /MICROSAR/Os/OsIoc/OsIocCommunication/OsIocSenderProperties/OsIocSenderId of that IOC communication. This part of the name may not be available in 1:1 communication as the parameter needs not be configured then. The value of <Bytes> is one of: { 08, 16, 32, Ref }. The value of <NameOfOsApplication> is the short name of a configuration container, reffered to by the parameter: /MICROSAR/Os/OsIoc/OsIocCommunication/OsIocReceiverProperties/OsIocReceivingOsApplicationRef of the Ioc communication. The value of <DataBufferPointer> will be OsCfg_Ioc_<NameOfIocCommunication><_<OsIocSenderId>>?_Buffer_Dyn, where the sender ID may not be identical to the one used for the name of the configuration structure. The values at <SpinlockPointer> and <LockModeValue> shall be reviewed against the table for consistency:

    .. list-table::
       :header-rows: 1

       * - <LockModeValue>
         - <SpinlockPointer>

       * - OS_IOCLOCKMODE_NONE
         - NULL_PTR

       * - OS_IOCLOCKMODE_INTERRUPTS
         - NULL_PTR

       * - OS_IOCLOCKMODE_SPINLOCKS
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>

       * - OS_IOCLOCKMODE_ALL
         - &OsCfg_Spinlock_IocSpinlock_<NameOfIocCommunication>


.. smi:: SMI-Os_30_TriCoreAurixGen7-889327

    The user of MICROSAR Safe shall verify the correct generation of the LockMode for all senders of a specific IOC communication. See SMI-889326 for the relevant configuration structures. The value for <LockMode> shall be identical for all senders and be in accordance to the table:

    .. list-table::
       :header-rows: 1

       * - More than one sender
         - <DataBufferPointer>
         - Core local and/or Cross-Core communication
         - <LockModeValue>

       * - no
         - not relevant
         - not relevant
         - OS_IOCLOCKMODE_NONE

       * - yes
         - different for all senders
         - not relevant
         - OS_IOCLOCKMODE_NONE

       * - yes
         - Identical for all senders (typically named with lowest sender ID)
         - All sender applications are located on the same core.
         - OS_IOCLOCKMODE_INTERRUPTS

       * - yes
         - Identical for all senders (typically named with lowest sender ID)
         - All sender applications are located on diferent cores.
         - OS_IOCLOCKMODE_SPINLOCKS

       * - yes
         - Identical for all senders (typically named with lowest sender ID)
         - Some sender applications are located on the same core while others are located on different core.
         - OS_IOCLOCKMODE_ALL


    Mind that MICROSAR-OS uses the same data pointer for all sender applications when a receiver callback is configured.


Safety Requirements Required From Other Components
==================================================

This component does not require safety requirements from other components.

Dependencies To Hardware
========================

The dependencies of this component to hardware is described in the platform specific part of the Safety Manual.

