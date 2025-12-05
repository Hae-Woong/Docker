/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: MemMap
 *           Program: 
 *          Customer: 
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to -
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: FrIf_MemMap.h
 *   Generation Time: 2021-10-29 09:33:54
 *           Project: TestApp001_ECU1.AR4 - Version 1.0
 *          Delivery: -
 *      Tool Version: DaVinci Configurator  5.12.21 SP3
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  PRE USER CONFIG FILE
 *********************************************************************************************************************/

/* User Config File Start */

/* User Config File End */

/**********************************************************************************************************************
 *  MEMMAP SECTIONS
 *********************************************************************************************************************/

#define MEMMAP_ERROR

#if defined FRIF_START_SEC_CODE
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_CODE_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_CODE
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_CODE_OPEN
#  error Closing via FRIF_STOP_SEC_CODE is not possible as that MemMap section is not opened.
# endif
# undef FRIF_CODE_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_CODE /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_CALLOUT_CODE
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_CALLOUT_CODE_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_CALLOUT_CODE /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_CALLOUT_CODE
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_CALLOUT_CODE_OPEN
#  error Closing via FRIF_STOP_SEC_CALLOUT_CODE is not possible as that MemMap section is not opened.
# endif
# undef FRIF_CALLOUT_CODE_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_CALLOUT_CODE /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_PBCFG
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_PBCFG_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_PBCFG
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_PBCFG_OPEN
#  error Closing via FRIF_STOP_SEC_PBCFG is not possible as that MemMap section is not opened.
# endif
# undef FRIF_PBCFG_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_PBCFG /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_PBCFG_GLOBALROOT
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_PBCFG_GLOBALROOT_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_PBCFG_GLOBALROOT
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_PBCFG_GLOBALROOT_OPEN
#  error Closing via FRIF_STOP_SEC_PBCFG_GLOBALROOT is not possible as that MemMap section is not opened.
# endif
# undef FRIF_PBCFG_GLOBALROOT_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_PBCFG_GLOBALROOT /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_CONST_32
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_CONST_32_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_CONST_32 /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_CONST_32
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_CONST_32_OPEN
#  error Closing via FRIF_STOP_SEC_CONST_32 is not possible as that MemMap section is not opened.
# endif
# undef FRIF_CONST_32_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_CONST_32 /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_CONST_UNSPECIFIED
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_CONST_UNSPECIFIED_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_CONST_UNSPECIFIED
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_CONST_UNSPECIFIED_OPEN
#  error Closing via FRIF_STOP_SEC_CONST_UNSPECIFIED is not possible as that MemMap section is not opened.
# endif
# undef FRIF_CONST_UNSPECIFIED_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_CONST_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_VAR_NOINIT_UNSPECIFIED
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_VAR_NOINIT_UNSPECIFIED_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_VAR_NOINIT_UNSPECIFIED_OPEN
#  error Closing via FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED is not possible as that MemMap section is not opened.
# endif
# undef FRIF_VAR_NOINIT_UNSPECIFIED_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_VAR_INIT_UNSPECIFIED
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_VAR_INIT_UNSPECIFIED_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_VAR_INIT_UNSPECIFIED
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_VAR_INIT_UNSPECIFIED_OPEN
#  error Closing via FRIF_STOP_SEC_VAR_INIT_UNSPECIFIED is not possible as that MemMap section is not opened.
# endif
# undef FRIF_VAR_INIT_UNSPECIFIED_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_VAR_INIT_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_VAR_CLEARED_UNSPECIFIED
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_VAR_CLEARED_UNSPECIFIED_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_VAR_CLEARED_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_VAR_CLEARED_UNSPECIFIED_OPEN
#  error Closing via FRIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED is not possible as that MemMap section is not opened.
# endif
# undef FRIF_VAR_CLEARED_UNSPECIFIED_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

/* -------------------------------------------------------------------------------- */

#elif defined FRIF_START_SEC_VAR_PBCFG
# ifdef MEMMAP_SECTION_OPEN
#  error A MemMap section is already open. Nesting is not supported.
# endif
# define MEMMAP_SECTION_OPEN
# define FRIF_VAR_PBCFG_OPEN

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_START_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */

#elif defined FRIF_STOP_SEC_VAR_PBCFG
# ifndef MEMMAP_SECTION_OPEN
#  error No MemMap section is currently opened.
# endif
# undef MEMMAP_SECTION_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */
# ifndef FRIF_VAR_PBCFG_OPEN
#  error Closing via FRIF_STOP_SEC_VAR_PBCFG is not possible as that MemMap section is not opened.
# endif
# undef FRIF_VAR_PBCFG_OPEN /* PRQA S 0841 */ /* MD_MSR_Undef */

/* Empty pragmas were generated. This was caused by using an unsupported compiler. */

# undef FRIF_STOP_SEC_VAR_PBCFG /* PRQA S 0841 */ /* MD_MSR_Undef */
# undef MEMMAP_ERROR /* PRQA S 0841 */ /* MD_MSR_Undef */
#endif

/* -------------------------------------------------------------------------------- */


/**********************************************************************************************************************
 *  MEMMAP ERROR CHECKS
 *********************************************************************************************************************/

#ifdef FRIF_START_SEC_CODE
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_CODE
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_CALLOUT_CODE
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_CALLOUT_CODE
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_PBCFG
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_PBCFG
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_PBCFG_GLOBALROOT
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_PBCFG_GLOBALROOT
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_CONST_32
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_CONST_32
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_CONST_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_CONST_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_VAR_NOINIT_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_VAR_INIT_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_VAR_INIT_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_VAR_CLEARED_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_START_SEC_VAR_PBCFG
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif
#ifdef FRIF_STOP_SEC_VAR_PBCFG
# error Each MemMap include may only perform one action. Include MemMap separately for each action.
#endif

#ifdef MEMMAP_ERROR
# error No MemMap section found in FrIf_MemMap.h. Check your section define for validity.
#endif

/**********************************************************************************************************************
 *  POST USER CONFIG FILE
 *********************************************************************************************************************/

/* User Config File Start */

/* User Config File End */

/**********************************************************************************************************************
 *  END OF FILE: FrIf_MemMap.h
 *********************************************************************************************************************/

