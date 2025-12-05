Safety Manual Arti
##################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory  with up to ASIL D.

Configuration constraints
*************************

.. smi:: SMI-978292

    The user of MICROSAR Safe shall ensure that no variable is accessed via an array index in the file *Arti_Lcfg.h*. Such accesses are generated when a */MICROSAR/Arti/ArtiHookTraceVariable/ArtiHookTraceVariableIndex* is configured. This check is required because the indices cannot be checked during runtime due to performance reasons. Furthermore, the ARTI component cannot guarantee by static measures that the index is always in range, since the generated macros are only called by other components.


Additional verification measures
********************************

.. smi:: SMI-978293

    The user of MICROSAR Safe shall ensure that write accesses in the file *Arti_Lcfg.h* do not lead to data consistency problems. Some possible data consistency problems are described in the section "Data Consistency" of the technical reference. This check is required because the generated code does not automatically prevent data consistency problems.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

