Safety Manual FiM
#################

Assumed safety requirements
***************************

This component does not assume safety requirements. This component is developed according to ISO 26262 only to ensure freedom from interference with respect to memory with up to ASIL D.

Configuration constraints
*************************

This component does not have configuration constraints.

Additional verification measures
********************************

.. smi:: SMI-232888

    In configurations where the table **FiM_SatelliteInfoTable** (generated into FiM_Lcfg.c) exists, the user of MICROSAR Safe shall ensure that all entries in the same line of the table **FiM_SatelliteInfoTable** refer to the same FiM satellite (and thus to the same OsApplication).

    - **Sat_<n>** in **FiM_InitializationStatus_Sat_<n>** and **FiM_InhStatusTable_Sat_<n>** of the same line must be identical and
    - no other line may contain the same **Sat_<n>** and
    - no two lines of the table may contain the same **<OsApplicationNameOfSat_n>**.
    
    E.g.:
    For OsApplication with short name OsApplication_A hosting satellite 1 the corresponding line in table FiM_SatelliteInfoTable has to be composed in the following way:

    { ..., OsApplication_A, &FiM_InitializationStatus_Sat_1, FiM_InhStatusTable_Sat_1 }.
    
    In configurations where the table **FiM_SatelliteInfoTable** does not exist, the user of MICROSAR Safe shall ensure:

    - that the macro **FiM_GetInitStatusPtrOfSatelliteInfoTable** (generated into FiM_Cfg_General.h) is defined to **&FiM_InitializationStatus_Sat_0** and
    - that the macro **FiM_GetInhStatusBasePtrOfSatelliteInfoTable** (generated into FiM_Cfg_InhStatHdl.h) is defined to **FiM_InhStatusTable_Sat_0**.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-159210

    In configurations where the table **FiM_SatelliteInfoTable**  (generated into FiM_Lcfg.c) exists, the user of MICROSAR Safe shall ensure:

    - that the pointer **InitStatusPtrOfSatelliteInfoTable** in each line <n> of the table FiM_SatelliteInfoTable points to a variable FiM_InitializationStatus_Sat_<n> (generated into FiM_Lcfg.c) and
    - that **FiM_InitializationStatus_Sat_<n>** is of type FiM_InitializationStatusType.
    
    In configurations where the table **FiM_SatelliteInfoTable** does not exist, the user of MICROSAR Safe shall ensure:

    - that the macro **FiM_GetInitStatusPtrOfSatelliteInfoTable** (generated into FiM_Cfg_General.h) returns a pointer to a variable FiM_InitializationStatus_Sat_0 (generated into FiM_Lcfg.c) and
    - that **FiM_InitializationStatus_Sat_0** is of type FiM_InitializationStatusType.
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-159209

    In configurations where the table **FiM_SatelliteInfoTable** (generated into FiM_Lcfg.c) exists, the user of MICROSAR Safe shall ensure:

    - that the pointer **InhStatusBasePtrOfSatelliteInfoTable** in each line <n> of the table FiM_SatelliteInfoTable points to the first element of an array FiM_InhStatusTable_Sat_<n> (generated into FiM_Lcfg.c) and
    - if the table column **InhStatusSizeOfSatelliteInfoTable** of the table FiM_SatelliteInfoTable exists, that the value **InhStatusSizeOfSatelliteInfoTable** in each line <n> of the table FiM_SatelliteInfoTable is not larger than the number of elements of array FiM_InhStatusTable_Sat_<n> and
    - if the table column **InhStatusSizeOfSatelliteInfoTable** of the table FiM_SatelliteInfoTable does not exist, that the macro **FiM_GetInhStatusSizeOfSatelliteInfoTable** (generated into FiM_Cfg_InhStatHdl.h) returns a number that is not larger than the number of elements of any array FiM_InhStatusTable_Sat_<n> (where <n> is the line in table FiM_SatelliteInfoTable) and
    - that **FiM_InhStatusTable_Sat_<n>** is of type FiM_ProcessorWordType[].
     
    In configurations where the table **FiM_SatelliteInfoTable** does not exist, the user of MICROSAR Safe shall ensure:

    - that the macro **FiM_GetInhStatusBasePtrOfSatelliteInfoTable** (generated into FiM_Cfg_InhStatHdl.h) returns a pointer to the first element of an array FiM_InhStatusTable_Sat_0 (generated into FiM_Lcfg.c) and
    - that the macro **FiM_GetInhStatusSizeOfSatelliteInfoTable** (generated into FiM_Cfg_InhStatHdl.h) returns a number that is not larger than the number of elements of array FiM_InhStatusTable_Sat_0 and 
    - that **FiM_InhStatusTable_Sat_0** is of type FiM_ProcessorWordType[].
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


.. smi:: SMI-438945

    The user of MICROSAR Safe shall ensure:

    - that the variables **FiM_InitializationStatus_Sat_<n>** (which are generated into FiM_Lcfg.c) are encapsulated by the related memory mapping section directives **FIM_START_SEC_<OSAPPLICATION_NAME_OF_SAT_n>_VAR_CLEARED_8** and **FIM_STOP_SEC_<OSAPPLICATION_NAME_OF_SAT_n>_VAR_CLEARED_8** and
    - that **<OSAPPLICATION_NAME_OF_SAT_n>** is identical (case-insensitive comparison) to **<OsApplicationNameOfSat_n>** in **FiM_SatelliteInfoTable** for Sat_<n> and
    - that the variables **FiM_InitializationStatus_Sat_<n>** are initialized to "0".
    
    E.g.:
    For OsApplication with short name OsApplication_A hosting satellite 1 the corresponding lines have to be composed in the following way:

    .. code-block::

        #define FIM_START_SEC_OSAPPLICATION_A_CLEARED_8
        ...
        VAR(FiM_InitializationStatusType, FIM_VAR_ZERO_INIT) FiM_InitializationStatus_Sat_1 = 0u;
        #define FIM_STOP_SEC_OSAPPLICATION_A_VAR_CLEARED_8
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

