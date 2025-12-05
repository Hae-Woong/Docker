Safety Manual Dcm
#################

Assumed safety requirements
***************************

This component does not assume safety requirements. This version is not fully developed according to ISO 26262 ASIL D. Vector provides an alternative argument for freedom from interference with respect to memory though. The Silent methodology developed by Vector has been completely implemented for this component. 

Configuration constraints
*************************

.. smi:: SMI-167098

    The user of MICROSAR Safe shall configure */MICROSAR/Dcm/DcmConfigSet/DcmDsp/DcmDspRteImplicitComEnabled* to *true*. This setting is enforced by an MSSV plugin.


Additional verification measures
********************************

All hereafter listed requirements are necessary to ensure freedom from interference with respect to memory by this component.

General
=======

.. smi:: SMI-37386

    The user of MICROSAR Safe shall verify that none of the generated functions in *Dcm_Lcfg.c* modifies the pointer passed as an argument, but only writes a new value to it.


.. smi:: SMI-37387

    The user of MICROSAR Safe shall verify that none of the generated functions in *Dcm_Lcfg.c* modifies the pointer passed as an argument, but only forwards it to another function.


.. smi:: SMI-37388

    The user of MICROSAR Safe shall verify that none of the generated functions in *Dcm_Lcfg.c* modifies the pointer taken from a local variable, and that the function receiving those pointer parameter(s) neither stores the addresses after return nor writes more data to the addesses than specified by the byte size of the local variable (especially in case of local arrays).


.. smi:: SMI-585463

    The user of MICROSAR Safe shall verify that for all functions, that are entirely or partially (e.g. through a wrapper function) implemented by the application, the passed arguments are valid before using it for write operations. This applies primarily, but not only, to these functions:

    - *Dcm_WriteMemory*
    - *Dcm_ProcessTransferDataWrite*


DID Manager
===========

.. smi:: SMI-105069

    The user of MICROSAR Safe shall verify that for all generated *OpFunc* functions in table *Dcm_CfgDidMgrSignalOpClassInfo* located in *Dcm_Lcfg.c* with a *FuncClass* setting of 0x00XX or 0x30XX, that are entirely or partially (e.g. through a wrapper function) implemented by the application, no more data will be written than specified by the corresponding *MaxLength* setting.


Mode 0x01
=========

.. smi:: SMI-147940

    The user of MICROSAR Safe shall verify that for all generated *OpFunc* functions in table *Dcm_CfgSvc01ServiceInfoTable* located in *Dcm_Lcfg.c* no more data will be written than specified by the corresponding *ResLength* setting.


Mode 0x08
=========

.. smi:: SMI-143395

    The user of MICROSAR Safe shall verify that for all generated *OpFunc* functions in table *Dcm_CfgSvc08ServiceInfoTable* located in *Dcm_Lcfg.c* no more data will be written than specified by the corresponding *ResLength* setting.


Mode 0x09
=========

.. smi:: SMI-147938

    The user of MICROSAR Safe shall verify that for all generated *OpFunc* functions in table *Dcm_CfgSvc09ServiceInfoTable* located in *Dcm_Lcfg.c* no more data will be written than specified by the corresponding *ResLength* setting.


Service 0x22
============

.. smi:: SMI-105312

    The user of MICROSAR Safe shall verify that for all generated *OpFunc* functions in table *Dcm_CfgDidMgrSignalOpClassInfo* located in *Dcm_Lcfg.c* with a *FuncClass* setting of 0x0003 no more data will be written than specified by *DCM_DIDMGR_READ_PAGE_SIZE* setting located in *Dcm_Cfg.h*.


.. smi:: SMI-106112

    The user of MICROSAR Safe shall verify that all generated *OpFunc* functions in table *Dcm_CfgDidMgrSignalOpClassInfo* located in *Dcm_Lcfg.c* match the signature indicated by the *FuncClass* setting:
    
    
    .. list-table::
       :header-rows: 1
    
       * - FuncClass
         - Expected Signature

       * - 0x0000
         - ``Std_ReturnType <name>(uint16, uint8*, Dcm_OpStatusType, uint16*, Dcm_NegativeResponseCodeType*)``

       * - 0x0001
         - ``Std_ReturnType <name>(uint8*)``

       * - 0x0002
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*)``

       * - 0x0003
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*, uint16*)``

       * - 0x0004
         - | if (DCM_DCM_AR_VERSION < DCM_DCM_AR_VERSION_422) 

           | ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*)``
           | else 
           | ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*, uint8*)``

       * - 0x0007
         - ``Std_ReturnType <name>(uint16, Dcm_OpStatusType, Dcm_DiagDataContextPtrType)``

       * - 0x0008
         - ``Std_ReturnType <name>(Dcm_DiagDataContextPtrType)``

       * - 0x0100
         - ``Std_ReturnType <name>(uint16, Dcm_OpStatusType, uint16*)``

       * - 0x0101
         - ``Std_ReturnType <name>(uint16*)``

       * - 0x0102
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint16*)``

       * - 0x0200
         - ``Std_ReturnType <name>(Dcm_NegativeResponseCodeType*)``

       * - 0x0201
         - ``Std_ReturnType <name>(Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``


.. smi:: SMI-106815

    The user of MICROSAR Safe shall verify that in the table *Dcm_CfgDidMgrSignalOpClassInfo*, if *OpFunc* requires Nv Block ID, the Nv Block ID passed to the function *Dcm_DidMgrNvMReadSignal()* is intended to be used exclusively by Dcm.
    
    This SMI is required to fulfill SMI-22 by NvM.
    
    *OpFunc* functions that require Nv Block ID are implemented in *Dcm_Lcfg.c* both of:

    - with a generic name Dcm_DidMgr_<SignalName>_Read,
    - and the corresponding *FuncClass* equals to 0x0002.
    
    The table *Dcm_CfgDidMgrSignalOpClassInfo* is located in *Dcm_Lcfg.c*.


.. smi:: SMI-106816

    The user of MICROSAR Safe shall verify that in the table *Dcm_CfgDidMgrSignalOpClassInfo*, if *OpFunc* requires Nv Block ID, the Nv Block ID passed to the function *Dcm_DidMgrNvMReadSignal()* has a size equals to *MaxLength*.
    
    This SMI is required to fulfill SMI-23 by NvM.
    
    *OpFunc* functions that require Nv Block ID are implemented in *Dcm_Lcfg.c* both of:

    - with a generic name Dcm_DidMgr_<SignalName>_Read,
    - and the corresponding *FuncClass* equals to 0x0002.
    
    The table *Dcm_CfgDidMgrSignalOpClassInfo* is located in *Dcm_Lcfg.c*.


Service 0x24
============

.. smi:: SMI-126353

    The user of MICROSAR Safe shall verify that all generated *OpFunc* functions in table *Dcm_CfgDidMgrSignalOpClassInfo* located in *Dcm_Lcfg.c* match the signature indicated by the *FuncClass* setting:
    
    
    .. list-table::
       :header-rows: 1
    
       * - FuncClass
         - Expected Signature

       * - 0x3000
         - ``Std_ReturnType <name>(uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x3001
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*, Dcm_NegativeResponseCodeType*)``


Service 0x27
============

.. smi:: SMI-40607

    The user of MICROSAR Safe shall verify that all generated *GetSeedFunc* functions in table *Dcm_CfgSvc27SecLevelInfo* located in *Dcm_Lcfg.c* match the signature indicated by the *GetSeedFuncClass* setting.
    
    
    .. list-table::
       :header-rows: 1
    
       * - GetSeedFuncClass
         - Expected Signature

       * - 0
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*, Dcm_NegativeResponseCodeType*)``

       * - 1
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint8*, Dcm_NegativeResponseCodeType*)``


.. smi:: SMI-49478

    The user of MICROSAR Safe shall verify that for all generated *GetSeedFunc* functions in table *Dcm_CfgSvc27SecLevelInfo* located in *Dcm_Lcfg.c* no more data will be written than specified by *DCM_SAFEBSW_SVC27_MAX_SEED_LENGTH* setting located in *Dcm_Cfg.h*.


Service 0x29
============

.. smi:: SMI-440519

    The user of MICROSAR Safe shall verify that each Nv Block, with the ID defined by NvMBlockId setting of table *Dcm_CfgAuthMgrNetConnInfo* located in *Dcm_Lcfg.c*, is intended to be used exclusively by Dcm.
    
    This SMI is required to fulfill SMI-22 by NvM.


.. smi:: SMI-440520

    The user of MICROSAR Safe shall verify that each Nv Block, with the ID defined by NvMBlockId setting of table *Dcm_CfgAuthMgrNetConnInfo* located in *Dcm_Lcfg.c*, has a size of the structure data type *Dcm_AuthMgrNvMDataType*.
    
    This SMI is required to fulfill SMI-23 by NvM.


Service 0x2C
============

.. smi:: SMI-327282

    The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by *DCM_SVC_2C_NVRAM_BLOCKID* located in *Dcm_Cfg.h* is intended to be used exclusively by Dcm.
    
    This SMI is required to fulfill SMI-22 by NvM.


.. smi:: SMI-327283

    The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by *DCM_SVC_2C_NVRAM_BLOCKID* located in *Dcm_Cfg.h* has a size equals to the size of the structure data type *Dcm_Svc2CDynDidNvMDataType*.
    
    This SMI is required to fulfill SMI-23 by NvM.


Service 0x2E
============

.. smi:: SMI-117436

    The user of MICROSAR Safe shall verify that all generated *OpFunc* functions in table *Dcm_CfgDidMgrSignalOpClassInfo* located in *Dcm_Lcfg.c* match the signature indicated by the *FuncClass* setting:
    
    
    .. list-table::
       :header-rows: 1
    
       * - FuncClass
         - Expected Signature

       * - 0x1000
         - ``Std_ReturnType <name>(uint16, const uint8*, Dcm_OpStatusType, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x1001
         - ``Std_ReturnType <name>(const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x1002
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``

       * - 0x1003
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x1004
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``

       * - 0x1005
         - ``Std_ReturnType <name>(const uint8*)``

       * - 0x1006
         - ``Std_ReturnType <name>(void)``


.. smi:: SMI-117437

    The user of MICROSAR Safe shall verify that in the table *Dcm_CfgDidMgrSignalOpClassInfo*, if *OpFunc* requires Nv Block ID, the Nv Block ID passed to the function *Dcm_DidMgrNvMWriteSignal()* is intended to be used exclusively by Dcm.
    
    This SMI is required to fulfill SMI-22 by NvM.
    
    *OpFunc* functions that require Nv Block ID are implemented in *Dcm_Lcfg.c* both of:

    - with a generic name Dcm_DidMgr_<SignalName>_Write,
    - and the corresponding *FuncClass* equals to 0x1002.
    
    The table *Dcm_CfgDidMgrSignalOpClassInfo* is located in *Dcm_Lcfg.c*.


.. smi:: SMI-117438

    The user of MICROSAR Safe shall verify that in the table *Dcm_CfgDidMgrSignalOpClassInfo*, if *OpFunc* requires Nv Block ID, the Nv Block ID passed to the function *Dcm_DidMgrNvMWriteSignal()* has a size equals to *MaxLength*.
    
    This SMI is required to fulfill SMI-23 by NvM.
    
    *OpFunc* functions that require Nv Block ID are implemented in *Dcm_Lcfg.c* both of:

    - with a generic name Dcm_DidMgr_<SignalName>_Write,
    - and the corresponding *FuncClass* equals to 0x1002.
    
    The table *Dcm_CfgDidMgrSignalOpClassInfo* is located in *Dcm_Lcfg.c*.


Service 0x2F
============

.. smi:: SMI-163630

    The user of MICROSAR Safe shall verify that all generated *OpFunc* functions in table *Dcm_CfgDidMgrSignalOpClassInfo* located in *Dcm_Lcfg.c* match the signature indicated by the *FuncClass* setting:
    
    
    .. list-table::
       :header-rows: 1
    
       * - FuncClass
         - Expected Signature

       * - 0x2000
         - ``Std_ReturnType <name>(Dcm_NegativeResponseCodeType*)``

       * - 0x2001
         - ``Std_ReturnType <name>(const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x2002
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x2003
         - ``Std_ReturnType <name>(Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``

       * - 0x2004
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``

       * - 0x2005
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``

       * - 0x2006
         - ``Std_ReturnType <name>(uint8, Dcm_NegativeResponseCodeType*)``

       * - 0x2007
         - ``Std_ReturnType <name>(const uint8*, uint8, Dcm_NegativeResponseCodeType*)``

       * - 0x2008
         - ``Std_ReturnType <name>(const uint8*, uint16, uint8, Dcm_NegativeResponseCodeType*)``

       * - 0x2009
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint8, Dcm_NegativeResponseCodeType*)``

       * - 0x200A
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint8, Dcm_NegativeResponseCodeType*)``

       * - 0x200B
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_OpStatusType, uint8, Dcm_NegativeResponseCodeType*)``

       * - 0x200C
         - ``Std_ReturnType <name>(uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x200D
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x200E
         - ``Std_ReturnType <name>(const uint8*, uint16, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x200F
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x2010
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x2011
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_OpStatusType, uint16, Dcm_NegativeResponseCodeType*)``

       * - 0x2012
         - ``Std_ReturnType <name>(uint32, Dcm_NegativeResponseCodeType*)``

       * - 0x2013
         - ``Std_ReturnType <name>(const uint8*, uint32, Dcm_NegativeResponseCodeType*)``

       * - 0x2014
         - ``Std_ReturnType <name>(const uint8*, uint16, uint32, Dcm_NegativeResponseCodeType*)``

       * - 0x2015
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint32, Dcm_NegativeResponseCodeType*)``

       * - 0x2016
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint32, Dcm_NegativeResponseCodeType*)``

       * - 0x2017
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_OpStatusType, uint32, Dcm_NegativeResponseCodeType*)``

       * - 0x2018
         - ``Std_ReturnType <name>(const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x2019
         - ``Std_ReturnType <name>(const uint8*, const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x201A
         - ``Std_ReturnType <name>(const uint8*, uint16, const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x201B
         - ``Std_ReturnType <name>(Dcm_OpStatusType, const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x201C
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x201D
         - ``Std_ReturnType <name>(const uint8*, uint16, Dcm_OpStatusType, const uint8*, Dcm_NegativeResponseCodeType*)``

       * - 0x201E
         - ``Std_ReturnType <name>(void)``


Service 0x31
============

.. smi:: SMI-64952

    The user of MICROSAR Safe shall verify that all generated *OpFunc* functions in table *Dcm_CfgRidMgrOpInfo* located in *Dcm_Lcfg.c* match the signature indicated by the *OpType* setting.
    
    
    .. list-table::
       :header-rows: 1
    
       * - OpType
         - Expected Signature

       * - 0
         - ``Std_ReturnType <name>(Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``

       * - 1
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, Dcm_NegativeResponseCodeType*)``

       * - 2
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint8*, Dcm_NegativeResponseCodeType*)``

       * - 3
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*, Dcm_NegativeResponseCodeType*)``

       * - 4
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint16, Dcm_NegativeResponseCodeType*)``

       * - 5
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint8*, uint16, Dcm_NegativeResponseCodeType*)``

       * - 6
         - ``Std_ReturnType <name>(Dcm_OpStatusType, uint8*, uint16*, Dcm_NegativeResponseCodeType*)``

       * - 7
         - ``Std_ReturnType <name>(const uint8*, Dcm_OpStatusType, uint8*, uint16*, Dcm_NegativeResponseCodeType*)``

       * - 8
         - ``Same as OpType 7``

       * - 9
         - ``Std_ReturnType <name>(Dcm_OpStatusType, Dcm_MsgContextPtrType, Dcm_RidMgrRidLengthPtrType, Dcm_NegativeResponseCodePtrType)``


.. smi:: SMI-64953

    The user of MICROSAR Safe shall verify that for all generated *OpFunc* functions in table *Dcm_CfgRidMgrOpInfo* located in *Dcm_Lcfg.c* no more data will be written than specified by the corresponding *ResMaxLength* setting.


.. smi:: SMI-65597

    The user of MICROSAR Safe shall verify for all generated Dcm_RidMgr_<RID>_<Start|Stop|RequestResults> functions implemented in *Dcm_Lcfg.c* that for each Dcm_UtiGetResDataRel(pMsgContext, **<offset>**) usage on out signals the application does not write more data than (*ResMaxLength* - **<offset>**), specified by the corresponding *ResMaxLength* setting in table *Dcm_CfgRidMgrOpInfo* located in _Dcm_Lcfg.c.


Service 0x37
============

.. smi:: SMI-579281

    The user of MICROSAR Safe shall verify that the implementation of the call-out function *Dcm_ProcessRequestTransferExit()* will not write more data than specified by the *transferResponseParameterRecordSize* parameter.


Service 0x86
============

.. smi:: SMI-163518

    The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by *DCM_EXT_SVC_86_NVRAM_BLOCKID* located in *Dcm_Cfg.h* is intended to be used exclusively by Dcm.
    
    This SMI is required to fulfill SMI-22 by NvM.


.. smi:: SMI-163519

    The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by *DCM_EXT_SVC_86_NVRAM_BLOCKID* located in *Dcm_Cfg.h* has a size equals to the size of the structure data type *Dcm_ExtSvc86NvMDataType*.
    
    This SMI is required to fulfill SMI-23 by NvM.


Safety requirements required from other components
**************************************************

This component does not require safety requirements from other components.

Dependencies to hardware
************************

This component does not use a direct hardware interface.

