Safety Manual Det
#################

Assumed safety requirements
***************************

This component does not assume safety requirements. 

This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

.. smi:: SMI-60

    If the DET is used in series production the extended debug features shall be switched off, because they are only relevant if a debugger is attached. 
    
    The user of MICROSAR Safe shall configure and verify the following attribute:

    - /MICROSAR/Det/DetGeneral/DetExtDebugSupport = False


Additional verification measures
********************************

.. smi:: SMI-4671

    **The user of MICROSAR Safe shall verify that the  enter  and  exit  functions  of  the  DET’s  exclusive  area  do  not  produce  DET errors.**
    
    Verification can e.g. be performed by review. If these functions are mapped to OS services it has to be checked that from the ErrorHook of the OS no DET error reporting functions are called if the ErrorHook has been called due to an error in the OS service used for the DET's exclusive area.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

For components provided as part of MICROSAR Safe, the respective section of the Safety Manual for that component allows you identifying if this requirement is fulfilled.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

