Safety Manual Dem
#################

Assumed Safety requirements
***************************

This component does not assume safety requirements. This version is not fully developed according to ISO 26262 ASIL D. Vector provides an alternative argument for freedom from interference with respect to memory though. The Silent methodology developed by Vector has been completely implemented for this component. 

Configuration constraints
*************************

.. smi:: SMI-106113

    The user of MICROSAR Safe shall ensure the following restrictions, when using APIs Dem_PrestoreFreezeFrame(), Dem_GetEventFreezeFrameDataEx() or Dem_GetEventExtendedDataRecordEx() :

    - Don’t map application data callback runnables to Os tasks.
    - Provide application data callbacks on the same Os application as the Dem Master is located. 
    - If you use Sender/Receiver data callbacks, map the runnables Dem_PrestoreFreezeFrame(), Dem_GetEventFreezeFrameDataEx() and Dem_GetEventExtendedDataRecordEx() to the same Os task as Dem_MasterMainFunction. 
    
    No automatic checks of those restrictions takes place. 
    
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.

.. smi:: SMI-DEM-CalibrationDisabled

    The user of MICROSAR Safe shall configure calibration support as disabled:

    - If the parameter /MICROSAR/Dem/DemGeneral/DemCalibrationSupport exists, set it to FALSE. The parameter is optional. If it does not exist, it has the default value FALSE.
  
    This setting is enforced by an MSSV plugin.
  
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.

.. smi:: SMI-DEM-MirrorMemoryDisabled

    The user of MICROSAR Safe shall configure mirror memory as not supported:

    - Set /MICROSAR/Dem/DemGeneral/DemMaxNumberEventEntryMirror to 0.
  
    This setting is enforced by an MSSV plugin.
  
    This requirement is necessary to ensure freedom from interference with respect to memory by this component.

Additional verification measures
********************************

The following requirements are necessary to ensure freedom from interference with respect to memory by this component.
If DemMaster **does not** fulfill the requirement for freedom of interference, the following rules can be ignored: 

- SMI-123681
- SMI-105855
- SMI-105856
- SMI-106376
- SMI-376150 

.. smi:: SMI-105853

    The user of MICROSAR Safe shall ensure that tables *Dem_Cfg_<CallbackType>* only contain function pointers matching the signature listed for *<CallbackType>* in the table below.
    If all events use the same callback, some of the tables might be optimized into a constant access macro *Dem_Cfg_Get<CallbackType>*. In that case the respective table does not exist. Instead, the user of MICROSAR Safe shall ensure the correct signature of the function pointer returned by the macro *Dem_Cfg_Get<CallbackType>*.
    The user of MICROSAR Safe shall ensure the functional safety associated with execution of the functions referenced in the tables or by the constant access macros mentioned above.
    The value NULL_PTR is a valid signature for all callback types listed in table below.
    Based on to the structure of generated tables, the Linker cannot always verify the function signatures automatically, hence this verification is required.
    
    The tables are generated into *Dem_Lcfg.c*.

    The macros are generated into *Dem_Lcfg.h*.

    ``<CallbackType>`` represents the entry in column *CallbackType* from table below.
    
    
    .. list-table::
       :header-rows: 1
    
       * - CallbackType
         - Expected signature of the functions

       * - CallbackClearEventAllowed
         - ``Std_ReturnType <name>(boolean *IsAllowed)``

       * - CallbackDtcStatusChanged
         - ``Std_ReturnType <name>(uint32 DTC, Dem_UdsStatusByteType oldStatus, Dem_UdsStatusByteType newStatus)``

       * - CallbackGetFdc
         - ``Std_ReturnType <name>(sint8 *FDC)``

       * - EventDataChanged
         - ``Std_ReturnType <name>(void)``

       * - EventStatusChanged
         - ``Std_ReturnType <name>(Dem_UdsStatusByteType oldStatus, Dem_UdsStatusByteType newStatus)``

       * - InitMonitorForEvent
         - ``Std_ReturnType <name>(Dem_InitMonitorReasonType initReason)``

       * - InitMonitorsForFunc
         - ``Std_ReturnType <name>(void)``


.. smi:: SMI-123681

    The user of MICROSAR Safe shall ensure that the tables Dem_Cfg_<CallbackType>, if they exist, only contain function pointers matching the signature listed for <CallbackType> in the table below.
    As some of the tables might be optimized into a constant access macro Dem_Cfg_Get<CallbackType>, the user of MICROSAR Safe shall also ensure the correct signature of the function pointers returned by the macros Dem_Cfg_Get<CallbackType>, if they exist.
    The user of MICROSAR Safe shall ensure the functional safety associated with execution of the functions referenced in the tables or by the constant access macros mentioned above.
    The value NULL_PTR is a valid signature for all callback types listed in table below.
    Based on to the structure of generated tables, the Linker cannot always verify the function signatures automatically, hence this verification is required.
    
    The tables are generated into *Dem_Lcfg.c*.
    The macros are generated into *Dem_Lcfg.h*.
    ``<CallbackType>`` represents the entry in column *CallbackType* from table below.
    
    
    .. list-table::
       :header-rows: 1
    
       * - CallbackType
         - Expected signature of the functions

       * - ClearDtcNotificationStartTable
         - ``Std_ReturnType <name>(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)``

       * - ClearDtcNotificationFinishTable
         - ``Std_ReturnType <name>(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)``

       * - CallbackJ1939DtcStatusChanged
         - ``Std_ReturnType <name>(uint32 DTC, Dem_UdsStatusByteType oldStatus, Dem_UdsStatusByteType newStatus)``


.. smi:: SMI-105854

    The user of MICROSAR Safe shall ensure that the macro definitions in table below are defined to function pointers matching the listed signature.
    The user of MICROSAR Safe shall ensure the functional safety associated with execution of the functions referenced by the macro definitions.
    The value *NULL_PTR* is a valid signature for all callback types listed in table below.
    Based on to the structure of generated tables, the Linker cannot always verify the function signatures automatically, hence this verification is required.
    The macros are generated into *Dem_Lcfg.h*.
    
    
    .. list-table::
       :header-rows: 1
    
       * - Configuration Macro
         - Expected signature of the function

       * - *DEM_CFG_GLOBALCBKDATA_FUNC*
         - ``Std_ReturnType <name>(Dem_EventIdType EventId)``

       * - *DEM_CFG_GLOBALCBKSTATUS_FUNC*
         - ``Std_ReturnType <name>(Dem_EventIdType EventId, Dem_UdsStatusByteType oldStatus, Dem_UdsStatusByteType newStatus)``

       * - *DEM_CFG_GLOBALCBKCONTROLDTCSETTING_FUNC*
         - ``Std_ReturnType <name>(boolean Status)``

       * - *DEM_CFG_GLOBALTRIGGERONMONITORSTATUS_FUNC*
         - ``void <name>(Dem_EventIdType EventId)``

       * - *DEM_CFG_OEMCBKGETEXTERNALTESTERSTATUS_FUNC*
         - ``Std_ReturnType <name>(boolean *TesterPresent)``

       * - *DEM_CFG_OEMCBKGETCURRENTODOMETER_FUNC*
         - ``Std_ReturnType <name>(uint32 *Odometer)``


.. smi:: SMI-105855

    The user of MICROSAR Safe shall ensure for all function pointers stored in table *Dem_Cfg_DataElementTable[]* that the function signature matches the *ElementKind* and *ElementDataType* value according to the table below.
    The user of MICROSAR Safe shall ensure the functional safety associated with execution of the functions referenced in the table mentioned above.
    Based on to the structure of generated tables, the Linker cannot always verify the function signatures automatically, hence this verification is required.
    *Dem_Cfg_DataElementTable[]* is generated into *Dem_Lcfg.c*.
    
    **If ElementKind is DEM_CFG_DATA_FROM_CS_PORT or DEM_CFG_DATA_FROM_SR_PORT**
    
    .. list-table::
       :header-rows: 1
    
       * - ElementDataType
         - Expected signature of the function

       * - *DEM_CFG_DATA_TYPE_BOOLEAN*
         - ``Std_ReturnType <name>(boolean *data)``

       * - | *DEM_CFG_DATA_TYPE_SINT8*
           | *DEM_CFG_DATA_TYPE_SINT8_N*

         - ``Std_ReturnType <name>(sint8 *data)``

       * - | *DEM_CFG_DATA_TYPE_SINT16*
           | *DEM_CFG_DATA_TYPE_SINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT16_N*
           | *DEM_CFG_DATA_TYPE_SINT16_N_INTEL*

         - ``Std_ReturnType <name>(sint16 *data)``

       * - | *DEM_CFG_DATA_TYPE_SINT32*
           | *DEM_CFG_DATA_TYPE_SINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT32_N*
           | *DEM_CFG_DATA_TYPE_SINT32_N_INTEL*

         - ``Std_ReturnType <name>(sint32 *data)``

       * - | *DEM_CFG_DATA_TYPE_UINT8*
           | *DEM_CFG_DATA_TYPE_UINT8_N*

         - ``Std_ReturnType <name>(uint8 *data)``

       * - | *DEM_CFG_DATA_TYPE_UINT16*
           | *DEM_CFG_DATA_TYPE_UINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT16_N*
           | *DEM_CFG_DATA_TYPE_UINT16_N_INTEL*

         - ``Std_ReturnType <name>(uint16 *data)``

       * - | *DEM_CFG_DATA_TYPE_UINT32*
           | *DEM_CFG_DATA_TYPE_UINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT32_N*
           | *DEM_CFG_DATA_TYPE_UINT32_N_INTEL*

         - ``Std_ReturnType <name>(uint32 *data)``

       * - | *DEM_CFG_DATA_TYPE_FLOAT*
           | *DEM_CFG_DATA_TYPE_FLOAT_INTEL*
           | *DEM_CFG_DATA_TYPE_FLOAT_N*
           | *DEM_CFG_DATA_TYPE_FLOAT_N_INTEL*

         - ``Std_ReturnType <name>(float32 *data)``
    
    
    **If ElementKind is DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID**
    
    .. list-table::
       :header-rows: 1
    
       * - ElementDataType
         - Expected signature of the function

       * - *DEM_CFG_DATA_TYPE_BOOLEAN*
         - ``Std_ReturnType <name>(Dem_EventIdType EventId, boolean *data)``

       * - | *DEM_CFG_DATA_TYPE_SINT8*
           | *DEM_CFG_DATA_TYPE_SINT8_N*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, sint8 *data)``

       * - | *DEM_CFG_DATA_TYPE_SINT16*
           | *DEM_CFG_DATA_TYPE_SINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT16_N*
           | *DEM_CFG_DATA_TYPE_SINT16_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, sint16 *data)``

       * - | *DEM_CFG_DATA_TYPE_SINT32*
           | *DEM_CFG_DATA_TYPE_SINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT32_N*
           | *DEM_CFG_DATA_TYPE_SINT32_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, sint32 *data)``

       * - | *DEM_CFG_DATA_TYPE_UINT8*
           | *DEM_CFG_DATA_TYPE_UINT8_N*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, uint8 *data)``

       * - | *DEM_CFG_DATA_TYPE_UINT16*
           | *DEM_CFG_DATA_TYPE_UINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT16_N*
           | *DEM_CFG_DATA_TYPE_UINT16_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, uint16 *data)``

       * - | *DEM_CFG_DATA_TYPE_UINT32*
           | *DEM_CFG_DATA_TYPE_UINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT32_N*
           | *DEM_CFG_DATA_TYPE_UINT32_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, uint32 *data)``

       * - | *DEM_CFG_DATA_TYPE_FLOAT*
           | *DEM_CFG_DATA_TYPE_FLOAT_INTEL*
           | *DEM_CFG_DATA_TYPE_FLOAT_N*
           | *DEM_CFG_DATA_TYPE_FLOAT_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, float32 *data)``
    
    
    **If ElementKind is DEM_CFG_DATA_FROM_FCT_WITH_EVENTID_BUFFERLENGTH**
    
    .. list-table::
       :header-rows: 1
    
       * - ElementDataType
         - Expected signature of the function

       * - *DEM_CFG_DATA_TYPE_BOOLEAN*
         - ``Std_ReturnType <name>(Dem_EventIdType EventId, boolean *data, uint16 DataLength)``

       * - | *DEM_CFG_DATA_TYPE_SINT8*
           | *DEM_CFG_DATA_TYPE_SINT8_N*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, sint8 *data, uint16 DataLength)``

       * - | *DEM_CFG_DATA_TYPE_SINT16*
           | *DEM_CFG_DATA_TYPE_SINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT16_N*
           | *DEM_CFG_DATA_TYPE_SINT16_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, sint16 *data, uint16 DataLength)``

       * - | *DEM_CFG_DATA_TYPE_SINT32*
           | *DEM_CFG_DATA_TYPE_SINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT32_N*
           | *DEM_CFG_DATA_TYPE_SINT32_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, sint32 *data, uint16 DataLength)``

       * - | *DEM_CFG_DATA_TYPE_UINT8*
           | *DEM_CFG_DATA_TYPE_UINT8_N*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, uint8 *data, uint16 DataLength)``

       * - | *DEM_CFG_DATA_TYPE_UINT16*
           | *DEM_CFG_DATA_TYPE_UINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT16_N*
           | *DEM_CFG_DATA_TYPE_UINT16_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, uint16 *data, uint16 DataLength)``

       * - | *DEM_CFG_DATA_TYPE_UINT32*
           | *DEM_CFG_DATA_TYPE_UINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT32_N*
           | *DEM_CFG_DATA_TYPE_UINT32_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, uint32 *data, uint16 DataLength)``

       * - | *DEM_CFG_DATA_TYPE_FLOAT*
           | *DEM_CFG_DATA_TYPE_FLOAT_INTEL*
           | *DEM_CFG_DATA_TYPE_FLOAT_N*
           | *DEM_CFG_DATA_TYPE_FLOAT_N_INTEL*

         - ``Std_ReturnType <name>(Dem_EventIdType EventId, float32 *data, uint16 DataLength)``
         
         
    **If ElementKind is DEM_CFG_DATA_FROM_FCT_WITH_PROXY**
    
    .. list-table::
       :header-rows: 1
    
       * - ElementDataType
         - Expected signature of the function

       * - *DEM_CFG_DATA_TYPE_BOOLEAN*
         - ``Std_ReturnType <name>(boolean *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``

       * - | *DEM_CFG_DATA_TYPE_SINT8*
           | *DEM_CFG_DATA_TYPE_SINT8_N*

         - ``Std_ReturnType <name>(sint8 *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``

       * - | *DEM_CFG_DATA_TYPE_SINT16*
           | *DEM_CFG_DATA_TYPE_SINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT16_N*
           | *DEM_CFG_DATA_TYPE_SINT16_N_INTEL*

         - ``Std_ReturnType <name>(sint16 *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``

       * - | *DEM_CFG_DATA_TYPE_SINT32*
           | *DEM_CFG_DATA_TYPE_SINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_SINT32_N*
           | *DEM_CFG_DATA_TYPE_SINT32_N_INTEL*

         - ``Std_ReturnType <name>(sint32 *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``

       * - | *DEM_CFG_DATA_TYPE_UINT8*
           | *DEM_CFG_DATA_TYPE_UINT8_N*

         - ``Std_ReturnType <name>(uint8 *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``

       * - | *DEM_CFG_DATA_TYPE_UINT16*
           | *DEM_CFG_DATA_TYPE_UINT16_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT16_N*
           | *DEM_CFG_DATA_TYPE_UINT16_N_INTEL*

         - ``Std_ReturnType <name>(uint16 *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``

       * - | *DEM_CFG_DATA_TYPE_UINT32*
           | *DEM_CFG_DATA_TYPE_UINT32_INTEL*
           | *DEM_CFG_DATA_TYPE_UINT32_N*
           | *DEM_CFG_DATA_TYPE_UINT32_N_INTEL*

         - ``Std_ReturnType <name>(uint32 *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``

       * - | *DEM_CFG_DATA_TYPE_FLOAT*
           | *DEM_CFG_DATA_TYPE_FLOAT_INTEL*
           | *DEM_CFG_DATA_TYPE_FLOAT_N*
           | *DEM_CFG_DATA_TYPE_FLOAT_N_INTEL*

         - ``Std_ReturnType <name>(float32 *data, uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode)``
         
         
    **For all other values of ElementKind, the function pointer must be NULL_PTR.**


.. smi:: SMI-105856

    The user of MICROSAR Safe shall ensure that all function callbacks configured in table *Dem_Cfg_DataElementTable[]* write at most *ElementSize* bytes.
    
    E.g. for a line {  DEM_CFG_DATA_FROM_CBK, 4U, (Dem_ReadDataFPtrType)Rte_Call_CBReadData_DemExternalCSDataElementClass_ReadData },
    ensure that the application runnable triggered by the Rte when the Dem invokes operation *ReadData* on PortPrototype *DemExternalCSDataElementClass* will at most write 4 bytes.
    
    The table is generated into *Dem_Lcfg.c*. 


.. smi:: SMI-106376

    The user of MICROSAR Safe shall ensure that if the following macros are not defined to a NULL_PTR, then the configured functions only writes the corresponding size to the data pointer.
    
    
    .. list-table::
       :header-rows: 1
    
       * - Macro
         - Data Size written by function

       * - *DEM_CFG_OEMCBKGETEXTERNALTESTERSTATUS_FUNC*
         - One byte (a boolean value)

       * - *DEM_CFG_OEMCBKGETCURRENTODOMETER_FUNC*
         - At most 4 bytes (uint32 odometer value)
    
    
    The macros are generated into *Dem_Lcfg.h*.


.. smi:: SMI-623276

    The user of MICROSAR Safe shall ensure that if the function callbacks configured in table Dem_Cfg_<CallbackType> or by the constant access macro Dem_Cfg_Get<CallbackType> are not defined as NULL_PTR, they only write the corresponding size to the data pointer.
    
    
    .. list-table::
       :header-rows: 1
    
       * - CallbackType
         - Data Size written by function

       * - *CallbackClearEventAllowed*
         - One byte (a boolean value)

       * - *CallbackGetFdc*
         - One byte (a signed integer value)
    
    
    The tables are generated into *Dem_Lcfg.c*.
    The macros are generated into *Dem_Lcfg.h*.


.. smi:: SMI-2196

    The user of MICROSAR Safe shall ensure that all NV Block IDs listed in table *Dem_Cfg_NvBlockId[]* belong to Dem. This is required to fulfill SMI-22 by NvM.
    
    The table is generated into *Dem_Lcfg.c*


.. smi:: SMI-2197

    The user of MICROSAR Safe shall ensure that the configured NvM block size of the NV blocks referenced in table *Dem_Cfg_NvBlockId* is equal to the data size in table *Dem_Cfg_NvBlockDataSize* for the same index.
    The user of MICROSAR Safe shall ensure that the NV block is also configured to use the RAM buffer referenced in *Dem_Cfg_NvBlockDataPtr* for the same index. This is required to fulfill SMI-23 by NvM.
    
    Example: Ensure that the NvM block descriptor referenced in *Dem_Cfg_NvBlockId[1]* is configured with data length *Dem_Cfg_NvBlockDataSize[1]* and RAM buffer *Dem_Cfg_NvBlockDataPtr[1]*
    
    The tables *Dem_Cfg_NvBlockId*, *Dem_Cfg_NvBlockDataSize* and *Dem_Cfg_NvBlockDataPtr* are generated into *Dem_Lcfg.c*.
    Refer to the NvM for the location of the tables generated by the NvM to ensure the correct configuration.


.. smi:: SMI-524392

    The user of MICROSAR Safe shall ensure that each entry in table *Dem_Cfg_EventEntryPtr* is a valid pointer to a global variable with data type *Dem_Cfg_PrimaryEntryType*, 
    defined by the DEM.
    
    The table *Dem_Cfg_EventEntryPtr* is generated in Dem_Lcfg.c.


.. smi:: SMI-166740

    **Relevant only for multi satellite configurations:**
    
    The user of MICROSAR Safe shall ensure that the table *Dem_Cfg_SatelliteInfo* is generated into *Dem_Lcfg.c*. 
    Furthermore, the user of MICROSAR Safe shall ensure that all entries in a row of this table belong to the same OsApplication, by ensuring that the entries belong to the structure named *Dem_Cfg_SatelliteInfo<X>*.
    E.g. for OsApplication with short name OsApplication_Sat1 the corresponding line in table *Dem_Cfg_SatelliteInfo* has to be composed in the following way:
    
    .. code-block::

        { ..., 
          OsApplication_Sat1, 
          &Dem_Cfg_SatelliteInfoOsApplication_Sat1.MaxDebounceValues[0] (if present), 
          &Dem_Cfg_SatelliteInfoOsApplication_Sat1.DebounceLoResTimer (if present), 
          &Dem_Cfg_SatelliteInfoOsApplication_Sat1.SatelliteStatus, 
          &Dem_Cfg_SatelliteInfoOsApplication_Sat1.SatelliteData[0], 
          &Dem_Cfg_SatelliteInfoOsApplication_Sat1.SatelliteExtendedData[0] (if present)
        }


.. smi:: SMI-105857

    **Relevant only for multi satellite configurations:**
    
    The user of MICROSAR Safe shall ensure, that the arrays *Dem_Cfg_SatelliteInfo<X>.SatelliteData[]*, *Dem_Cfg_SatelliteInfo<X>.SatelliteExtendedData[]* (if generated) and *Dem_Cfg_SatelliteInfo<X>.MaxDebounceValues[]* (if generated) are referenced in table *Dem_Cfg_SatelliteInfo* and the size of each array is atleast equal to *EventCount* in the same line.
    
    E.g. for a line in table *Dem_Cfg_SatelliteInfo*

    .. code-block::

        { 
          37U, 
          ..., 
          &Dem_Cfg_SatelliteInfoOsApplication_Master.MaxDebounceValues[0], 
          ..., 
          &Dem_Cfg_SatelliteInfoOsApplication_Master.SatelliteData[0], 
          &Dem_Cfg_SatelliteInfoOsApplication_Master.SatelliteExtendedData[0] 
        },

    where 37 is the *EventCount*, ensure that the structure Dem_Cfg_SatelliteInfoTypeOsApplication_Master is generated as follows into *Dem_Lcfg.h*:

    .. code-block::

        typedef struct Dem_Cfg_SatelliteInfoTypeOsApplication_Master_s {
          ...
          volatile Dem_SatelliteData_DataType         SatelliteData[ 37 ];
          volatile Dem_SatelliteData_ExtendedDataType SatelliteExtendedData[ 37 ];
          volatile Dem_Cfg_MaxDebounceValuesType      MaxDebounceValues[ 37 ];
          ...
        } Dem_Cfg_SatelliteInfoTypeOsApplication_Master;
    
    
    The user of MICROSAR Safe shall ensure that the above mentioned pointers listed in the table Dem_Cfg_SatelliteInfo always point to the first element in the corresponding arrays in *Dem_Cfg_SatelliteInfo<X>*.
    
    The user of MICROSAR Safe shall ensure that the function like macro *Dem_Cfg_GetEventCountOfSatelliteInfo()* always returns the value of *EventCount* of the corresponding line in table *Dem_Cfg_SatelliteInfo*.
    
    **Relevant only for single satellite configurations:**
    The user of MICROSAR Safe shall ensure that the size of the arrays whose addresses are returned by the function like macros *Dem_Cfg_GetMaxDebounceValuesOfSatelliteInfo()*, *Dem_Cfg_GetSatelliteExtendedDataOfSatelliteInfo()* and *Dem_Cfg_GetSatelliteDataOfSatelliteInfo()* is equal to the value returned by function like macro *Dem_Cfg_GetEventCountOfSatelliteInfo()*. The macros are generated into *Dem_Lcfg.h*.
    
    **Relevant for all configurations:**
    The user of MICROSAR Safe shall ensure that the pointer returned by the function like macro *Dem_Cfg_GetSatelliteDataOfSatelliteInfo()* always points to member *Dem_Cfg_SatelliteInfo<X>.SatelliteData[0]*.
    
    The user of MICROSAR Safe shall ensure that if the pointer returned by the function like macro *Dem_Cfg_GetMaxDebounceValuesOfSatelliteInfo()* does not point to optional *Dem_Cfg_SatelliteInfo<X>.MaxDebounceValues[0]*, the function like macro is defined to *NULL_PTR*.
    
    The user of MICROSAR Safe shall ensure that if the pointer returned by the function like macro *Dem_Cfg_GetSatelliteExtendedDataOfSatelliteInfo()* does not point to optional *Dem_Cfg_SatelliteInfo<X>.SatelliteExtendedData[0]*, the function like macro is defined to *NULL_PTR*


.. smi:: SMI-105858

    **Relevant only for multi satellite configurations:**
    
    The user of MICROSAR Safe shall ensure that the  pointers to SatelliteStatus and DebounceLoResTimer (if generated) in table *Dem_Cfg_SatelliteInfo* in *Dem_Lcfg.c*, reference the members of the corresponding Dem_Cfg_SatelliteInfo<X>:
    
    i.e. ensure that the pointers in table *Dem_Cfg_SatelliteInfo* are generated the following way for each row:

    .. code-block::

      {  ..., 

        &Dem_Cfg_SatelliteInfo<X>.DebounceLoResTimer (if generated), 
        &Dem_Cfg_SatelliteInfo<X>.SatelliteStatus, ... 

      }
    
    
    **Relevant for all configurations:**
    The user of MICROSAR Safe shall ensure that the structures Dem_Cfg_SatelliteInfo<X> are generated as follows into *Dem_Lcfg.h*:

    .. code-block::

        typedef struct Dem_Cfg_SatelliteInfo<X>_s 
        {

          ...
          volatile Dem_Com_ProcessorWordType          SatelliteStatus;
          Dem_Cfg_DebounceLoResTimerType              DebounceLoResTimer (if generated);
          ...
          } Dem_Cfg_SatelliteInfoType<X>;
    
    
    The user of MICROSAR Safe shall ensure that the pointer returned by the function like macro *Dem_Cfg_GetSatelliteStatusOfSatelliteInfo()* always points to member *Dem_Cfg_SatelliteInfo<X>.SatelliteStatus*.
    
    The user of MICROSAR Safe shall ensure that if the pointer returned by the function like macro *Dem_Cfg_GetDebounceLoResTimerOfSatelliteInfo()* does not point to Dem_Cfg_SatelliteInfo<X>.DebounceLoResTimer, the function like macro is defined to *NULL_PTR*.
    
    The macros are generated into *Dem_Lcfg.h*.


.. smi:: SMI-105859

    The user of MICROSAR Safe shall ensure that the struct variables *Dem_Cfg_SatelliteInfo<X>* in *Dem_Lcfg.c* are encapsulated by the related memory mapping sections DEM_START_SEC_<X>_VAR_CLEARED_UNSPECIFIED and DEM_STOP_SEC_<X>_VAR_CLEARED_UNSPECIFIED.
    
    In a multi satellite configuration, the user of MICROSAR Safe shall ensure that write and read accesses to specified memory sections are restricted to corresponding partitions as described in the Technical Reference.


.. smi:: SMI-106114

    The user of MICROSAR Safe shall ensure that the initialization functions are called from the correct partition:

    - Dem_MasterPreInit() and Dem_MasterInit() only from the master partition
    - Dem_SatellitePreInit() and Dem_SatelliteInit() only from the respective partition.
    
    When calling Dem_SatellitePreInit() or Dem_SatelliteInit() the following value has to be used as input parameter SatelliteId:

    - In a multi satellite configuration: The symbolic name DEM_SATELLITEINFO_<Short name of respective /Os/OsApplication> can be used. It must be ensured that value of this symbolic name is equal to index of the row belonging to the respective satellite in table *Dem_Cfg_SatelliteInfo*.

    (.i.e.  row containing short name of the respective OsApplication as *ApplicationId*).

    - In a single satellite configuration: the value 0
    
    Also the user of MICROSAR Safe shall ensure that the initialization functions are not called again during run-time of the Dem.


.. smi:: SMI-376150

    If the type definition Dem_Cfg_ProcessedCombinedDTCGroupType is generated, the user of MICROSAR Safe shall ensure that:

    - Dem_Cfg_ProcessedCombinedDTCGroupType is defined as an array type
    - The array size of Dem_Cfg_ProcessedCombinedDTCGroupType is equal to DEM_CFG_PROCESSEDCOMBINEDDTCGROUP_SIZE
    - If the structure type "Dem_Cfg_FreezeFrameIterator_FilterType_s", is generated and it has a member variable "ProcessedCombinedDTCGroups", then it must be ensured that the member variable is of type Dem_Cfg_ProcessedCombinedDTCGroupType.
    - If the structure type "Dem_Cfg_FilterData_InfoType_s", is generated and it has a member variable "ProcessedCombinedDTCGroups", then it must be ensured that the member variable is of type Dem_Cfg_ProcessedCombinedDTCGroupType.

    The aforementioned type definitions are present in Dem_Lcfg.h.
    
    E.g. if DEM_CFG_PROCESSEDCOMBINEDDTCGROUP_SIZE is defined as 5, ensure that the data type is generated as follows into *Dem_Lcfg.h*: 
    
    .. code-block::
    
        typedef Dem_Com_ProcessorWordType Dem_Cfg_ProcessedCombinedDTCGroupType[5];
    
    .. code-block::

        typedef struct Dem_Cfg_FreezeFrameIterator_FilterType_s {
        ...

          Dem_Cfg_ProcessedCombinedDTCGroupType ProcessedCombinedDTCGroups;

        ...
        }
    

    .. code-block::

        typedef struct Dem_Cfg_FilterData_InfoType_s{
        ...

          Dem_Cfg_ProcessedCombinedDTCGroupType   ProcessedCombinedDTCGroups;

        ...
        }


.. smi:: SMI-816233

    The user of MICROSAR Safe shall verify that the table *Dem_Cfg_MemoryInfoTable* is generated into *Dem_Lcfg.c*.
    
    Furthermore, the user of MICROSAR Safe shall verify that for each row in *Dem_Cfg_MemoryInfoTable*, the member "*ChronoPtrOfMemoryInfoTable*" is generated as either a NULL_PTR or contains address to the first element of a chronology array defined in Dem_Lcfg.c. The user of MICROSAR Safe shall also verify that the size of each of the aforementioned Chronology array, is equal to the value present in member "*MaxSizeOfMemoryInfoTable*" in the same row.
    
    In case *Dem_Cfg_MemoryInfoTable* is optimized out, the user must verify that for all input parameters, the function *Dem_Cfg_GetChronoPtrOfMemoryInfoTable()* points to the address of the first element of a chronology array, defined in Dem_Lcfg.c. It must also be verified that size of the chronology array is equal to the value returned by *Dem_Cfg_GetMaxSizeOfMemoryInfoTable()* for the same input parameter.


.. smi:: SMI-DEM-1000

    The user of MICROSAR Safe shall ensure that each entry in table *Dem_Cfg_CustomTriggerEntryPtr* is a valid pointer to a global variable with data type *Dem_Cfg_CustomTriggerEntryType*, 
    defined by the DEM.
    
    The table *Dem_Cfg_CustomTriggerEntryPtr* is generated in Dem_Lcfg.c.



Verification measures for usage scenario: Untrusted Master and Trusted Satellite
================================================================================

The verification measures described in this section are only relevant for a multi satellite configuration in which the DemMaster do not fulfill the requirement for freedom of interference and at least one other Dem satellite is running on a Trusted Partition and does fulfill the requirement for freedom of interference. 

.. smi:: SMI-439315

    The user of MICROSAR Safe shall ensure that the extended initialization sequence described in the Technical Reference, which uses the APIs Dem_SafePreInit() and Dem_SafeInit() is followed.


.. smi:: SMI-439347

    The user of MICROSAR Safe shall ensure that only the allowed operations for the satellite running in ASIL partition are used. The list of allowed operations for this partitioning scenario is documented in the Technical Reference.


.. smi:: SMI-439375

    The user of MICROSAR Safe shall ensure that the preprocessor macro *DEM_CFG_SUPPORT_MULTIPLE_SATELLITES* is defined as *STD_ON* in Dem_Cfg.h. 
    The user shall also ensure that the preprocessor macro *DEM_CFG_MASTER_APPLICATION_ID* is defined as the short name of the OS Application which is referred in /Dem/DemGeneral/DemMasterOsApplicationRef.


Safety requirements required from other components
**************************************************

.. smi:: SMI-442831

    In a multi satellite configuration in which the Dem Master runs in an untrusted partition and one or more satellites run in ASIL partitions, this component requires a service to determine the Application-ID in which the current execution runs, as safety critical requirement from the operating system. This requirement is fulfilled if an ASIL operating system by Vector is used.


Dependencies to hardware
************************

This component does not use a direct hardware interface.

