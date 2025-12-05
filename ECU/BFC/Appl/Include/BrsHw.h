/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  File:  BrsHw.h
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsHw for platform Infineon Aurix/AurixPlus
 *
 *  \brief Description:  This is the hardware specific header file for Vector Basic Runtime System (BRS).
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Description
 *  --------  ----------  ------  -------------------------------------------------------------------------------------
 *  01.00.00  2020-05-19  virrlu  New branch for vBaseEnv 2.0, based on zBrs_Aurix BrsHw.c 3.00.12
 *            2020-05-28  vismaa  Changed pin naming for BRS_ENABLE_FLEXRAY_CHANNEL_x_B from FRxB_ERRNA TO FRxB_ERRNB
 *  01.00.01  2020-06-16  vismaa  Changed brsStartupEntry into StartupEntry (Symbol is used with vLinGen 2.0),
 *                                added support for 40MHz OSC (AurixPlus)
 *  01.00.02  2020-06-26  visbpz  Implemented BrsHw_UnlockCores() as an empty function
 *  01.00.03  2020-07-15  visrgm  Fixed ETH port initialization due to support quest
 *  01.00.04  2020-07-22  visto   HALBE-2249: Remove unecessary inline function prototypes -> all prototypes are
 *                                            declared within tcasm.h
 *  01.01.00  2020-07-28  virrlu  Fixed Tasking BRS_STARTUP_LABEL in BrsHw.h to support default startup label,
 *                                changed Tasking startup stack labels in BrsHwStartup.c
 *  01.02.00  2020-08-04  visbwa  Experimental support for FBL UseCase Legacy w/o OS
 *            2020-09-17  visbwa  Fixed initialization of BRSHW_PORT_ETH0_MDIO
 *            2020-09-18  visrgm  Added linking of StartupCode to section .brsStartup in BrsHwStartup.c
 *            2020-09-20  vismaa  Added BRS_PORT_GROUP_C (TC49xx)
 *            2020-09-22  visto   HALBE-2176 create inline functions for ENDINIT handling
 *  01.03.00  2020-10-26  visto   HALBE-2633 - vBaseEnv_TricoreAurix: Adaptions for FBL UseCase,
 *                                             introduction of BrsHwIntTb_TCxxx.c
 *  01.03.01  2020-10-28  visbpz  Added inline to HighTec BRS_FUNC_DECLARATION in BrsHw.h, to get rid of "always_inline
 *                                function might not be inlinable [-Wattributes]" compiler warning
 *            2020-10-29  vishci  removed Author Identity and added wait status between PLL up rampings
 *  01.03.02  2021-01-15  visto   ESCAN00108312 - Incorrect condition on Wait for System PLL to be stable
 *  01.03.03  2021-01-20  visto   HALBE-2832 - Fix wrong implemented Errata PLL_TC.005 fix and add safety endinit
 *                                             handling
 *  01.03.04  2021-01-25  visto   HALBE-3613 - Setting up STM Frequency to 100 MHz
 *  01.03.05  2021-03-12  vismaa  Changed fOSCREF for BRS_PLL_GROUP_B, OSCVAL calcualation has changed
 *  01.04.00  2021-04-01  visbwa  Update to Brs_Template 1.02.01, moved revision histories of all files into BrsHw.h,
 *                                removed BMHD1..3, BMHD0 should always be sufficient,
 *                                changed BMHD0 of TC3xx to boot from 0xA0000000 instead of 0xA00A0020,
 *                                removed BRS_STARTUP_LABEL (not needed any more as linking of StartUpCode was changed)
 *  01.05.00  2021-06-02  vismaa  Little rework for TC4xx, splitted Startupcode for TC4xx group, HSM support for TC4xx
 *  01.05.01  2021-07-05  visbwa  PLL_ERAY_TC.001 fix in BrsHwErayPllInitPowerOn() for TC2xx FlexRay clock
 *  01.06.00  2021-07-07  visto   rename BRS_COMP_HIGHTEC to BRS_COMP_GCCHIGHTEC
 *  01.06.01  2021-07-09  vismaa  Added support for BRS_OSC_CLK=24 MHz
 *  01.06.02  2021-07-28  visbwa  Fixed CSA_STARTADDRESS of Core5 in BrsHwStartup.c
 *  01.06.03  2021-08-02  visbwa  Enhanced encapsulation of inline lock/unlock functions to prevent compiler warnings
 *  02.00.00  2021-10-20  visbwa  Initial updates to Brs_Template 1.03.00
 *            2021-11-09  visjhr  Extended BrsHwWatchdogInitPowerOn for MultiCore usage (HALBE-3678)
 *            2021-11-10  visbwa  Fixed BMHD implementation of TC2xx and TC3xx (changed StartupEntry to cached Pflash)
 *            2021-11-10  visjhr  BrsHwSoftwareResetECU: set SW reset type to System Reset (HALBE-3326)
 *  02.00.01  2021-11-26  visbwa  Fixed macro of BRS_SET_SP for DiabData compiler to prevent compiler warning
 *  02.00.02  2021-12-02  visbwa  Added a runtime check for CSA list entries into BrsHwStartup.c
 *  02.00.03  2021-12-02  visbwa  Added missing encapsulation with BRS_FIRST_EXECUTION_INSTANCE in BrsHwStartup.c
 *  02.00.04  2021-12-07  visbwa  Fix of ESCAN00110936, update to Brs_Template 1.03.01
 *  02.00.05  2021-12-15  visto   Fix Compiler warnings about unknown defines "BRSHW_SOURCECODE_TEMPLATE_VERSION"
 *                                BrsMain.h must be included after defining these
 *  02.00.06  2021-12-27  visto   HALBE-5680 / Clean up Watchdog handling functions and defines
 *                                upgrade to template 01.03.02
 *  02.00.07  2022-01-17  visbwa  Fixed setup of section .brsExcVectRam for GccHightec in BrsHwIntTb.c, to be handled
 *                                as executable code (otherwise, memory was not reserved correctly by linker)
 *  02.00.08  2022-01-18  visjhr  HALBE-5722 Implement Port Init for TC4xx, added peripheral port config for
 *                                TRIBOARD_TC499A_COM
 *                                HALBE-5723 Implement Other functions
 *            2022-02-18  visbwa  Added encapsulation with BRS_FIRST_EXECUTION_INSTANCE for BootModeHeader,
 *                                update to Brs_Template 1.03.03 (mainly BrsHwIntTb.c)
 *  02.01.00  2022-03-03  visto   HALBE-6208, HALBE-6315 Issue Fixes for TC4xx HSM usecase
 *  03.00.00  2022-03-10  vismaa  Moved TC4xx implementation in new package
 *  03.00.01  2022-04-11  virjas  Improved inline keyword compatibilty for HighTec compiler
 *  03.01.00  2022-05-12  visto   HALBE-5806: Perform an early stackpointer init in startup code
 *  03.01.01  2022-06-02  visto   Version numbers corrected
 *  03.01.02  2022-07-20  visbwa  Fixed TC3xx TriBoard CAN channel pin mappings in BrsHw_Ports.h
 *  03.01.03  2022-07-25  visjhr  HALBE-7176 ENDINIT goes RAM for FBL UseCase
 *  03.01.04  2022-08-12  visbwa  Update to Brs_Template 1.03.05; UsageType=external in ALM (DemoComponents concept)
 *  03.01.05  2022-08-17  visto   usage of BRS_READ_COREID() in BrsHw_ExceptionTable_Init instead of BrsHw_GetCore()
 *  03.01.06  2022-12-23  vismaa  Created kBrsWdtCon0Addresses for ROM and RAM (HALBE-7649)
 *  03.01.07  2023-02-10  visto   compiler error corrections
 *  03.01.08  2023-10-25  visjhr  Upgrade to template 01.03.06
 *  03.01.09  2023-10-31  vissgj  Updated BRSHW_SOURCECODE_TEMPLATE_BUGFIX_VERSION
 *  03.01.10  2023-11-27  vissgj  Added BrsHwDisableInterruptAtPowerOn() for all compilers (BASEENV-10961)
 *  03.01.11  2023-03-15  virrlu  Add missing parenthesis in BrsHw_ExceptionTable_Init()
 *  03.01.12  2023-03-15          Fixed Bugfix version
 *  03.01.13  2024-05-03  vissgj  BASEENV-11837 Introduced CDC disabling for FBL with Ethernet
 *  03.02.00  2024-05-28  kal     BASEENV-11656 Added support for Ethernet RGMII on TC3x7 and TC3x9
 *  03.02.01  2024-06-27  kal     BASEENV-11532 Added BRSHW_EARLYINIT_AVAILABLE for early activation of OLDA memory
 *  03.02.02  2024-07-08  vissgj  TAR-54891 Fixed missing PCSR setting for RGMII for TC3xx
 *  03.02.03  2024-08-01  vissgj  BASEENV-12366 speed up early init: enabled iCache, optimized early init loop
 *  03.02.04  2024-08-05  kal     BASEENV-12236 Adjusted GETH divider to 2 in accordance with DrvEth team
 *  03.03.00  2024-09-03  kal     BASEENV-12457 Added multiple frequencies for GETH and BRS_ETH_CLOCK check
 *                                              Removed outdated PLL_GROUP_C (TC397X_StepA, TC399X_StepA)
 *            2024-09-04  mna     BASEENV-12512 Optimize core-ID handling in Startup Code
 *  03.03.01  2024-09-17  kal     BASEENV-12521 Harmonized clock settings and implemented clock checks
 *                                              Added CPU divider option for TC3xx (300 MHz, 150 MHz)
 *  03.03.02  2024-10-18  visdri                Fix BRS_GET_CURRENT_CONFIG() for GHS.
 *  03.03.03  2024-10-21  mna                   Fix Brs_Gfd6 for Diab Compiler
 *  03.03.04  2024-12-05  sgj                   Fix Brs_Gfd6 for Diab Compiler
 *********************************************************************************************************************/

#ifndef _BRSHW_H_
#define _BRSHW_H_
//test
/**********************************************************************************************************************
 *  MODULE VERSION
 *********************************************************************************************************************/
/*
 * Description: This is the BrsHw main and bug fix version. The version numbers are BCD coded.
 *              E.g. a main version of 1.23 is coded with 0x0123, a bug fix version of 9 is coded 0x09.
 */

/* This is the BrsHw template main and bug fix version, the platform BrsHw is reviewed to */
#define BRSHW_SOURCECODE_TEMPLATE_VERSION        0x0103u
#define BRSHW_SOURCECODE_TEMPLATE_BUGFIX_VERSION 0x06u

/* This is the BrsHw main and bug fix version */
#define BRSHW_VERSION        0x0303u
#define BRSHW_BUGFIX_VERSION 0x04u

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/*
 * Description: The BrsCfg header is used to configure different types of
 *              tests and system setups. Therefore it must be included first
 *              in each BRS and test module.
 *              This file is part of the BRS.
 */
#include "vBrsCfg.h"

#if defined (BRS_ENABLE_PORT)
  #include "BrsHw_Ports.h"
#endif

# include "BrsHwStartup_TC2xx_TC3xx.h"

#include "BrsMain.h"

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
/* The following parameters are necessary for this platform, to be generated by BRS-Cfg5-Generator into BrsCfg.h */
#if !defined (BRS_OSC_CLK)
  #error "BRS CHECK: The parameter BRS_OSC_CLK is missing! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_TIMEBASE_CLOCK)
  #error "BRS CHECK: The parameter BRS_TIMEBASE_CLOCK is missing! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_PERIPH_CLOCK)
  #error "BRS CHECK: The parameter BRS_PERIPH_CLOCK is missing! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_CPU_MAX_FREQUENCY)
  #error "BRS CHECK: The parameter BRS_CPU_MAX_FREQUENCY is missing! Please check your BRS Cfg5 configuration."
#endif

#if !(defined (BRS_CPU_CORE_TC161) || defined (BRS_CPU_CORE_TC162))
  #error "BRS CHECK: The parameter BRS_CPU_CORE_x is missing or not supported! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_CPU_CORE_AMOUNT)
  #error "BRS CHECK: The parameter BRS_CPU_CORE_AMOUNT is missing! Please check your BRS Cfg5 configuration."
#endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*
 * Description: Macro for access to IO addresses
 */
#define BRSHW_IOS(type, address) (*((volatile type *)(address)))

/* DERIVATIVE DEPENDENT STATIC */
/* See BrsHwStartup_TC2xx_TC3xx.h for the derivative group specific settings */
#define BRSHW_INIT_CORE_ID  BRSHW_DERIVATIVE_CORE0_ID

/*
 * Description: Configure here the platform/compiler specific macros, that are used in vBRS generated vBrs_Lcfg.c
 *              to define the core specific values of BrsMain_CoreConfig (labels for StartupStack, CoreExceptionTable,
 *              InterruptVectorTable)
 */
#define BRSHW_DEFINE_STARTUP_STACK(x)  _STACK_C##x##_LIMIT

#define BRSHW_DEFINE_EXCVEC(x)          _OS_EXCVEC_CORE##x##_CODE_START
#define BRSHW_DEFINE_INTVEC(x)          _OS_INTVEC_CORE##x##_CODE_START

/* Special value, needed for SingleCore UseCase w/o OS (e.g. FBL). vBRS will generate "BRSHW_DEFINE_STARTUP_STACK(BRSHW_INIT_CORE_ID)"
    for this. Configure here the valid value for the StartupStack pointer label of the boot core */
#define _STACK_CBRSHW_INIT_CORE_ID_LIMIT _STACK_C0_LIMIT

# define BRS_EXPAND(x)  BRS_EXPAND1(x)
# define BRS_EXPAND1(x) #x

# define kBrsWdtCon0Addresses(x) kBrsWdtCon0Addresses##x
/**********************************************************************************************************************
 * Compiler abstraction
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *                                           TASKING
 *********************************************************************************************************************/
#if defined (BRS_COMP_TASKING)
  #define nop()                    __nop()

  #define BRS_FORCE_ACTIVE         __attribute__((protect))

  #define BRS_ISYNC()              __isync()
  #define BRS_DSYNC()              __dsync()

  #define BRS_HAL_EXPAND(x)        #x

  #define BRS_MOVE_FROM_CSFR(x)    (uint32)__mfcr((sint32)(x))
  #define BRS_MOVE_TO_CSFR(x, y)   BRS_DSYNC(); __asm("mtcr  #"BRS_HAL_EXPAND(x)", %0" ::"d"(y):); BRS_ISYNC()

  #define BRS_SET_SP(x)            __asm("mov.a  a10, %0" ::"d"(x):"a10")

  #define BRS_FUNC_DECLARATION(rettype, attribute, functionName, arguments0,arguments1) \
    rettype functionName (arguments0, arguments1) __attribute__((attribute))

  #define BRS_FUNC_DECLARATION_PROTOTYPE(rettype, attribute, functionName, arguments0, arguments1) \
    rettype functionName (arguments0, arguments1) __attribute__((attribute))

/* =========================================================================== */
/*                                                                             */
/* inline assembler compiler abstraction                                       */
/*                                                                             */
/* =========================================================================== */

#define BRS_ASSEMBLER_CODESECTION(sectionName)    __asm("  .SDECL   \'.text."BRS_EXPAND(sectionName)"\',code,protect"); \
                                                  __asm("  .SECT    \'.text."BRS_EXPAND(sectionName)"\'");

#define BRS_ASSEMBLER_ALIGN(x)                    __asm("  .ALIGN "BRS_EXPAND(x)"");

#define BRS_ASSEMBLER_EXTERNDECLARATION(symbol)   __asm("  .EXTERN "BRS_EXPAND(symbol)"");

#define BRS_ASSEMBLER_GLOBALDECLARATION(symbol)   __asm("  .GLOBAL "BRS_EXPAND(symbol)"");

#define BRS_ASSEMBLER_LABEL(label)                __asm(""BRS_EXPAND(label)":");

#define BRS_ASSEMBLER_STARTUP_CODE() \
   __asm("  mfcr     d8, #65052");                                /* get current Core ID *//* D8 = current core id */\
   __asm("  movh.a   a11,#@his(BrsMain_CoreConfig_Size)");        /* get the size of BrsMain_CoreConfig */\
   __asm("  ld.w     d7,[a11]@los(BrsMain_CoreConfig_Size)");     /* D7 = sizeof(BrsMain_CoreConfig) */\
   __asm("  mov      d6, #0");                                    /* Storing 0 to d6, increment while running through loop to check against size of BrsMain_CoreConfig */\
   __asm("  movh.a   a14, #@his(BrsMain_CoreConfig)"); \
   __asm("  lea      a14,[a14]@los(BrsMain_CoreConfig)");         /* get start address of BrsMain_CoreConfig[x] *//* A14 = &BrsMain_CoreConfig */\
   __asm("brsStartupEntryloop:"); \
   __asm("  ld.w     d9,[a14]8");                                 /* get stack end address from offset [8] *//* D9 = BrsMain_CoreConfig[current].StartupStackEndLabel */\
   __asm("  ld.w     d10,[a14]4");                                /* get BrsMain_CoreConfig.PhysicalCoreId from offset [4] *//* D10 = BrsMain_CoreConfig[current].PhysicalCoreId */\
   __asm("  jne      d8, d10, brsGetNextCoreConfig");             /* if PhysicalCoreId == current Core ID */\
   __asm("  mov.a    a10, d9");                                   /*   setup SP *//* A10 = SP = D9 */\
   __asm("  j        brsStartupEntry_C");                         /*   jump to startup code */\
   __asm("brsGetNextCoreConfig:");                                /* else */\
   __asm("  add      d6, #1");                                    /* increment d7 to count cores that have been checked in Brs_MainCoreConfig */\
   __asm("  jeq      d6, d7, brsGoToStartupException");           /* Last element of BrsMain_CoreConfig and CurrentCore still not found. Jump to Startup Exception */\
   __asm("  movh.a   a12,#@his(Brs_SizeOfbrsMain_CoreType)");     /* get the size of brsMain_CoreType */\
   __asm("  ld.a     a12,[a12]@los(Brs_SizeOfbrsMain_CoreType)"); /* A12 = sizeof(brsMain_CoreType) */\
   __asm("  add.a    a14, a12");                                  /* get next element of BrsMain_CoreConfig[x+1] *//* A14 = A14+A12 = &BrsMain_CoreConfig[x+1] */\
   __asm("  j        brsStartupEntryloop");                       /*   jump to loop begin */\
   __asm("brsGoToStartupException:"); \
   __asm("  j        BrsMainExceptionStartup");                   /* Exception */

# define BRS_GET_CURRENT_CONFIG(x) \
   __asm("  st.w    [%0], d6" : : "a" (x));

/**********************************************************************************************************************
 *                                           HIGHTEC GCC
 *********************************************************************************************************************/
#else
//  #error "Compiler not yet supported"
#endif /*BRS_COMP_x*/

/**********************************************************************************************************************
 *  BrsHW configuration
 *********************************************************************************************************************/

/*******************************************************************************
  WATCHDOG  GROUP  CONFIG
*******************************************************************************/
#define BRS_SAFETY_WATCHDOG_INDEX  -1

#if defined (BRS_WATCHDOG_GROUP_A)
  #define BRS_SFR_WDTCPU0CON0_ADDRESS 0xF0036100
  #define BRS_SFR_WDTCPU0CON1_ADDRESS 0xF0036104
  #define BRS_SFR_WDTCPU1CON0_ADDRESS 0xF003610C
  #define BRS_SFR_WDTCPU1CON1_ADDRESS 0xF0036110
  #define BRS_SFR_WDTCPU2CON0_ADDRESS 0xF0036118
  #define BRS_SFR_WDTCPU2CON1_ADDRESS 0xF003611C
  #define BRS_SFR_WDTSCON0_ADDRESS    0xF00360F0
  #define BRS_SFR_WDTSCON1_ADDRESS    0xF00360F4

  #define BRS_SAFETY_WATCHDOG_ARRAY_INDEX 3

#elif defined (BRS_WATCHDOG_GROUP_B)
  #define BRS_SFR_WDTCPU0CON0_ADDRESS 0xF003624C
  #define BRS_SFR_WDTCPU0CON1_ADDRESS 0xF0036250
  #define BRS_SFR_WDTCPU1CON0_ADDRESS 0xF0036258
  #define BRS_SFR_WDTCPU1CON1_ADDRESS 0xF003625C
  #define BRS_SFR_WDTCPU2CON0_ADDRESS 0xF0036264
  #define BRS_SFR_WDTCPU2CON1_ADDRESS 0xF0036268
  #define BRS_SFR_WDTCPU3CON0_ADDRESS 0xF0036270
  #define BRS_SFR_WDTCPU3CON1_ADDRESS 0xF0036274
  #define BRS_SFR_WDTCPU4CON0_ADDRESS 0xF003627C
  #define BRS_SFR_WDTCPU4CON1_ADDRESS 0xF0036280
  #define BRS_SFR_WDTCPU5CON0_ADDRESS 0xF0036288
  #define BRS_SFR_WDTCPU5CON1_ADDRESS 0xF003628C
  #define BRS_SFR_WDTSCON0_ADDRESS    0xF00362A8
  #define BRS_SFR_WDTSCON1_ADDRESS    0xF00362AC

  #define BRS_SAFETY_WATCHDOG_ARRAY_INDEX 7

#endif /*BRS_WATCHDOG_GROUP_x*/

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/
/*
 * Description: These constants are used to propagate the Versions over module boundaries.
 *              The version numbers are BCD coded. E.g. a sub version of 23 is coded with 0x23,
 *              a bug fix version of 9 is coded 0x09.
 */
extern uint8 const kBrsHwMainVersion;
extern uint8 const kBrsHwSubVersion;
extern uint8 const kBrsHwBugfixVersion;

struct Brs_WdtRegTypes_Tag
{
  uint32 WdtCon0Address;
  uint32 WdtCon1Address;
};

typedef struct Brs_WdtRegTypes_Tag Brs_WdtRegTypes;

#if defined (BRS_WATCHDOG_GROUP_A)
extern const Brs_WdtRegTypes kBrsWdtCon0Addresses_RAM[4];
extern const Brs_WdtRegTypes kBrsWdtCon0Addresses_ROM[4];
#elif defined (BRS_WATCHDOG_GROUP_B)
extern const Brs_WdtRegTypes kBrsWdtCon0Addresses_RAM[8];
extern const Brs_WdtRegTypes kBrsWdtCon0Addresses_ROM[8];
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#if defined (BRS_DERIVATIVE_GROUP_TC2xx) || defined (BRS_DERIVATIVE_GROUP_TC3xx)
# if defined (_BRSHW_C_) || defined (_BRSHWSTARTUP_C_)
 /*******************************************************************************
 CPU Core Amount (number of cores) and memory Type (RAM or ROM)
*******************************************************************************/
BRS_FUNC_DECLARATION_PROTOTYPE(static uint32 *, always_inline, BrsHw_GetAddrWDTCON0, sint32 nr, uint32 memoryType);
BRS_FUNC_DECLARATION(static uint32 *, always_inline, BrsHw_GetAddrWDTCON0, sint32 nr, uint32 memoryType)
{
  if (nr == BRS_SAFETY_WATCHDOG_INDEX)
  {
    if (memoryType == MEMORY_TYPE_RAM)
    {
      return (uint32*)(kBrsWdtCon0Addresses_RAM[BRS_SAFETY_WATCHDOG_ARRAY_INDEX].WdtCon0Address);
    }
    else
    {
      return (uint32*)(kBrsWdtCon0Addresses_ROM[BRS_SAFETY_WATCHDOG_ARRAY_INDEX].WdtCon0Address);
    }
  }
  else
  {
    if (memoryType == MEMORY_TYPE_RAM)
    {
      return (uint32*)(kBrsWdtCon0Addresses_RAM[nr].WdtCon0Address);
    }
    else
    {
      return (uint32*)(kBrsWdtCon0Addresses_ROM[nr].WdtCon0Address);
    }
  }
}

BRS_FUNC_DECLARATION_PROTOTYPE(static uint32 *, always_inline, BrsHw_GetAddrWDTCON1, sint32 nr, uint32 memoryType);
BRS_FUNC_DECLARATION(static uint32 *, always_inline, BrsHw_GetAddrWDTCON1, sint32 nr, uint32 memoryType)
{
  if (nr == BRS_SAFETY_WATCHDOG_INDEX)
  {
    if (memoryType == MEMORY_TYPE_RAM)
    {
      return (uint32*)(kBrsWdtCon0Addresses_RAM[BRS_SAFETY_WATCHDOG_ARRAY_INDEX].WdtCon1Address);
    }
    else
    {
      return (uint32*)(kBrsWdtCon0Addresses_ROM[BRS_SAFETY_WATCHDOG_ARRAY_INDEX].WdtCon1Address);
    }
  }
  else
  {
    if (memoryType == MEMORY_TYPE_RAM)
    {
    return (uint32*)(kBrsWdtCon0Addresses_RAM[nr].WdtCon1Address);
    }
    else
    {
    return (uint32*)(kBrsWdtCon0Addresses_ROM[nr].WdtCon1Address);
    }
  }
}

/*****************************************************************************/
/* @brief Routine to unlock registers that are normally protected
 * @pre   Interrupts must be disabled

 *****************************************************************************/
BRS_FUNC_DECLARATION_PROTOTYPE(static void, always_inline, BrsHwUnlockInitInline, sint32 coreID, uint32 memoryType);
BRS_FUNC_DECLARATION(static void, always_inline, BrsHwUnlockInitInline, sint32 coreID, uint32 memoryType)
{
  volatile uint32 wdtcon0value;
  uint32 *WDTCON0;
  WDTCON0 = BrsHw_GetAddrWDTCON0(coreID, memoryType);

  /*
   * 1st step: Password access (create password and send to WDT_CON0)
   */
  wdtcon0value = *WDTCON0;
  wdtcon0value &= 0xFFFFFF01;  /* clear WDTLCK, WDTHPW0, WDTHPW1 */
  wdtcon0value |= 0xF0;        /* set WDTHPW1 to 0xf */
  wdtcon0value |= 0x01;        /* 1 must be written to ENDINIT for password access
                                (but this will not actually modify the bit) */
  *WDTCON0 = wdtcon0value; /* modify access to WDTCON0 */
  /*
   * 2nd step: Modify access, set the bit ENDINIT to 1 or 0 to allow access to
   *           registers: WDT_CON1, BTV, BIV, ISP and mod_CLC
   */
  wdtcon0value &= 0xFFFFFFF0;  /* clear WDTHPW0, WDTLCK, ENDINIT  */
  wdtcon0value |= 0x02;        /* WDTHPW0=0, WDTLCK=1, ENDINIT=0 */
  BRS_ISYNC();
  *WDTCON0 = wdtcon0value;
  wdtcon0value = *WDTCON0; /* read is required */
  BRS_ISYNC();
  BRS_DSYNC();
}

/*****************************************************************************/
/* @brief Routine to lock registers that are normally protected
 * @pre   Interrupts must be disabled
 * @param[in]  coreID
 *             memoryType IN_RAM or IN_ROM
 *****************************************************************************/
BRS_FUNC_DECLARATION_PROTOTYPE(static void, always_inline, BrsHwLockInitInline, sint32 coreID, uint32 memoryType);
BRS_FUNC_DECLARATION(static void, always_inline, BrsHwLockInitInline, sint32 coreID, uint32 memoryType)
{
  volatile uint32 wdtcon0value;
  uint32 *WDTCON0;
  WDTCON0 = BrsHw_GetAddrWDTCON0(coreID, memoryType);

  /*
   * 1st step: Password access (create password and send to WDT_CON0)
   */
  wdtcon0value = *WDTCON0;
  wdtcon0value &= 0xFFFFFF01;  /* clear WDTLCK, WDTHPW0, WDTHPW1 */
  wdtcon0value |= 0xF0;        /* set WDTHPW1 to 0xf */
  wdtcon0value |= 0x01;        /* 1 must be written to ENDINIT for password access
                                 (but this will not actually modify the bit) */
  *WDTCON0 = wdtcon0value; /* modify access to WDTCON0 */
  /*
   * 2nd step: Modify access, set the bit ENDINIT to 1 or 0 to allow access to
   *           registers: WDT_CON1, BTV, BIV, ISP and mod_CLC
   */
  wdtcon0value &= 0xFFFFFFF0;  /* clear WDTHPW0, WDTLCK, ENDINIT  */
  wdtcon0value |= 0x03;        /* WDTHPW0=0, WDTLCK=1, ENDINIT=1 */
  BRS_ISYNC();
  *WDTCON0 = wdtcon0value;
  wdtcon0value = *WDTCON0; /* read is required */
  BRS_ISYNC();
  BRS_DSYNC();
}
# endif /*_BRSHW_C_||_BRSHWSTARTUP_C_*/
#endif /*BRS_DERIVATIVE_GROUP_TC2xx||BRS_DERIVATIVE_GROUP_TC3xx*/

#if defined (BRS_FIRST_EXECUTION_INSTANCE)
#define BRS_START_SEC_STARTUP_CODE
#include "Brs_MemMap.h"
/*****************************************************************************/
/* @brief      This function can be used to initialize controller specific
 *             stuff that should be handled by the first execution instance,
 *             even before the memory initialization took place (e.g.
 *             everything that is necessary to access the memory or to handle
 *             exceptions). This function is called at the beginning of
 *             BrsMainStartup(), immediately after the stackpointer was
 *             initialized in StartupCode
 *             (if BRSHW_EARLYINIT_AVAILABLE is set in BrsHw.h).
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() at power on
 *             initialization
 *****************************************************************************/
#define BRSHW_EARLYINIT_AVAILABLE
void BrsHwEarlyInitPowerOn(void);
#define BRS_STOP_SEC_STARTUP_CODE
#include "Brs_MemMap.h"
#endif /*BRS_FIRST_EXECUTION_INSTANCE*/

#if defined (BRS_ENABLE_WATCHDOG)
/*****************************************************************************/
/* @brief      This function must be used to initialize the Watchdog.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwWatchdogInitPowerOn(void);
#endif /*BRS_ENABLE_WATCHDOG*/

#if defined (BRS_ENABLE_PLLCLOCKS)
/*****************************************************************************/
/* @brief      This function must be used to initialize the PLL.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwPllInitPowerOn(void);
#endif /*BRS_ENABLE_PLLCLOCKS*/

#if defined (BRS_ENABLE_PORT)
/*****************************************************************************/
/* @brief      This function configures a port pin as input pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortInput(brsHw_Port_PortType);

/*****************************************************************************/
/* @brief      This function configures a port pin as output pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortOutput(brsHw_Port_PortType);

/*****************************************************************************/
/* @brief      This function configures a port pin as input and output pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortInputOutput(brsHw_Port_PortType);

# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_START_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif
/*****************************************************************************/
/* @brief      This function sets the output level of a port pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h,
 *             no DrvPort used for port pin initialization and
 *             transferred port pin has to be initialized as output pin with
 *             GPIO functionality.
 * @param[in]  p     - brsHw_Port_PortType, to be set,
 *             Level - level, port pin has to be set to
 *                     (BRSHW_PORT_LOGIC_LOW or BRSHW_PORT_LOGIC_HIGH).
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. BrsMainTogglePin()).
 *****************************************************************************/
void BrsHwPort_SetLevel(brsHw_Port_PortType, uint8);

/*****************************************************************************/
/* @brief      This function reads the input level of a port pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h,
 *             no DrvPort used for port pin initialization and
 *             transferred port pin has to be initialized as input pin with
 *             GPIO functionality.
 * @param[in]  p - brsHw_Port_PortType, to be read.
 * @param[out] -
 * @return     Level, read from port pin
 *             (BRSHW_PORT_LOGIC_LOW or BRSHW_PORT_LOGIC_HIGH).
 * @context    Function is provided to external modules.
 *****************************************************************************/
uint8 BrsHwPort_GetLevel(brsHw_Port_PortType);
# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_STOP_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif

/*****************************************************************************/
/* @brief      This function must be used to initialize the used ports.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwPortInitPowerOn(void);
#endif /*BRS_ENABLE_PORT*/

/*****************************************************************************/
/* @brief      Disable the global system interrupt.
 * @pre        Must be the first function call in main@BrsMain
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from main@BrsMain at power on initialization
 *****************************************************************************/
void BrsHwDisableInterruptAtPowerOn(void);

/*****************************************************************************/
/* @brief      Program the vector table base addresses
 * @pre        -
 * @param[in]  ExcVecLabel - address of the core exception table
 *             IntVecLabel - address of the interrupt vector table
 *             Use BRS_DEFINE_ADDRESS_UNUSED for unused values
 * @param[out] -
 * @return     -
 * @context    Function is called from main@BrsMain at power on initialization
 *****************************************************************************/
void BrsHw_ExceptionTable_Init(Brs_AddressOfConstType, Brs_AddressOfConstType);

/*****************************************************************************/
/* @brief      Restart ECU (issue a software reset or jump to startup code)
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from e.g. ECU state handling
 *****************************************************************************/
void BrsHwSoftwareResetECU(void);

#if defined (BRS_FIRST_EXECUTION_INSTANCE)
/* This code is only needed for the first instance/executable in the system */
#define BRS_START_SEC_STARTUP_CODE
#include "Brs_MemMap.h"
/*****************************************************************************/
/* @brief      Get reset reason
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     Reset reason
 * @context    Function is called from BrsMainStartup to determine if reset
 *             was triggered through software call (BrsHwSoftwareResetECU()).
 *             The result is stored by BrsMainStartup in the global variable
 *             brsMain_ResetReason. It should only be called once, during
 *             startup. The old API name BrsHwGetResetReason() is remapped
 *             to BrsMainGetResetReason.
 *****************************************************************************/
//brsMain_ResetReasonType BrsHwGetResetReasonStartup(void);
#define BRS_STOP_SEC_STARTUP_CODE
#include "Brs_MemMap.h"

/* Wrapper from old BrsHwGetResetReason() function to new
 * BrsMainGetResetReason(). This is to prevent multiple selection of reset
 * reason registers, as this is not supported on all platforms.
 */
#define BrsHwGetResetReason() BrsMainGetResetReason()
#endif /*BRS_FIRST_EXECUTION_INSTANCE*/

/*****************************************************************************/
/* @brief      This API is used for the BRS time measurement support to get a
 *             default time value for all measurements with this platform to
 *             be able to compare time measurements on different dates based
 *             on this time result.
 * @pre        Should be called with interrupts global disabled
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from e.g. component testsuits for calibration
 *****************************************************************************/
void BrsHwTime100NOP(void);

#if defined (BRS_ENABLE_SAFECTXSUPPORT)
/*****************************************************************************/
/* @brief      This API is used to enable hardware access in untrusted mode
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function must be called after all depending peripheral modules
 *             are supplied by proper clocks AND before the OS is started.
 *****************************************************************************/
void BrsHw_EnableHwAccess(void);
#endif /*BRS_ENABLE_SAFECTXSUPPORT*/

/* check if necessary vismaa*/
//#if defined (BRS_ENABLE_FBL_SUPPORT)
// #define BRS_START_SEC_ROM_CODE
// #include "Brs_MemMap.h"
//#endif
/*****************************************************************************/
/* @brief      This API is used to read the core ID of the actual running core
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     Core ID of the actual running core
 * @context    Function is e.g. called from main@BrsMain, to only call HW-init
 *             code once, on the boot core.
 *             In MultiCore setups, additional BRSHW_INIT_CORE_ID must be
 *             declared inside BrsHw.h, to configure the proper core ID value
 *             of that boot core.
 *****************************************************************************/
uint32 BrsHw_GetCore(void);
/* check if necessary vismaa*/
//#if defined (BRS_ENABLE_FBL_SUPPORT)
// #define BRS_STOP_SEC_ROM_CODE
// #include "Brs_MemMap.h"
//#endif

/*****************************************************************************/
/* @brief      This API is used to enable an interrupt source in the core
 *             interrupt controller.
 * @pre        -
 * @param[in]  Source to be enabled.
 * @param[in]  Priority level to be set.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_EnableInterrupt(uint32, uint8);

/*****************************************************************************/
/* @brief      This API is used to disable an interrupt source in the core
 *             interrupt controller.
 * @pre        -
 * @param[in]  Source to be disabled.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_DisableInterrupt(uint32);

/*****************************************************************************/
/* @brief      This API is used to trigger the given software interrupt source.
 * @pre        -
 * @param[in]  Source to be triggered.
 *             Some derivatives only support few software triggerable sources,
 *             check for their validity.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_TriggerSoftwareInterrupt(uint32);

#if defined (BRS_ENABLE_FBL_SUPPORT)
#define BRSHW_DISABLE_ECC_AVAILABLE
/*****************************************************************************/
/* @brief      This API is used to disable the flash ECC error reporting,
 *             if this is necessary on the platform to grant access to it.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from FBL boot code, before flash is accessed.
 *****************************************************************************/
void BrsHwDisableEccErrorReporting(void);
#endif /*BRS_ENABLE_FBL_SUPPORT*/

# if defined (BRS_ENABLE_FLEXRAY_SUPPORT)
/*****************************************************************************/
/**
 * @brief      Callback from communication stack. Init E-RAY RAM to avoid ECC errors
 * @pre
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called Fr_InitMemory(). It is just an empty function
 *             because the RAM init is already done during startup.
 */
/*****************************************************************************/
void Appl_TricoreAurixInit(void);
# endif /* BRS_ENABLE_FLEXRAY_SUPPORT */

#if defined (BRS_ENABLE_OS_MULTICORESUPPORT)
/*****************************************************************************/
/* @brief      This API is used as sync barrier. It is used to unlock cores
 *             that are locked in the Startup code. The implementation is only
 *             needed on platforms, where all cores are started automatically
 *             after a power-on reset. All additional cores are looped, until
 *             the boot core reaches this function.
 * @pre        -
 * @param[in]  uint32 coreId
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainStartup on the boot core.
 *****************************************************************************/
void BrsHw_UnlockCores(uint32);
#endif /*BRS_ENABLE_OS_MULTICORESUPPORT*/

#endif /*_BRSHW_H_*/
