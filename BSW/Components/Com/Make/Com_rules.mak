###############################################################################
# File Name  : Com_rules.mak
# Description: Rules makefile
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2024 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author  Description
#------------------------------------------------------------------------------
# 1.00.00   2011-11-30  ms      initial CFG5 version derived from
#                               Il_AsrIpduM Version 1.00.00
# 1.01.00   2014-08-04  hho     removed Com_Cfg.c
# 1.02.00   2019-02-06  vircbl  Added support of component-based SIP structure
# 2.00.00   2020-12-23  visssg  COM-1848 Introduce Com Units
# 3.00.00   2021-01-26  visssg  COM-2215 Support a ComIPdu references to a MainFunction
# 4.00.00   2021-09-08  viscpe  COM-1851 Implement Multipartition Com - Signal Gateway
# 5.00.00   2022-08-26  visms   COM-3149 Improve Units for Test2020
# 5.00.01   2023-02-07  vadgsz  COM-4309 ESCAN00112955 - Linker Error:
#                               Unknown functions Com_MainFunctionRx_<name> Com_MainFunctionTx_<name>
# 6.00.00   2023-03-22  visssg  COM-4441 Refactor SignalGateway
# 6.00.00   2023-07-11  vadnra  COM-4634 Support Gateway Units
# 6.01.00   2023-10-11  visbbk  COM-4648 Structural Improvements based on SafetyAnalysis
# 6.02.00   2024-03-07  visms   COM-5239 Compile Code as Unity Build
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################


###############################################################
# REGISTRY
#

#e.g.: LIBRARIES_TO_BUILD      +=    $(LIB_OUPUT_PATH)\vendorx_canlib1.$(LIB_FILE_SUFFIX)
LIBRARIES_TO_BUILD      += Com
Com_FILES                = Com$(BSW_SRC_DIR)\Com_Unity.c
CC_FILES_TO_BUILD       += Com$(BSW_SRC_DIR)\Com_Unity.c
CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

#LIBRARIES_LINK_ONLY     += (not yet supported)
#OBJECTS_LINK_ONLY       += (not yet supported)

#-------------------------------------------------------------------------------------------------
#only define new dirs, OBJ, LIB, LOG were created automaticly
#-------------------------------------------------------------------------------------------------
DIRECTORIES_TO_CREATE   +=

#DEPEND_GCC_OPTS         += (not yet supported)

# e.g.:  GENERATED_SOURCE_FILES += $(GENDATA_DIR)\drv_par.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Com_PBcfg.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Com_Lcfg.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Com_GeneratedMainFunctions.c

#e.g.: COMMON_SOURCE_FILES     += $(GENDATA_DIR)\v_par.c
COMMON_SOURCE_FILES     +=

#-------------------------------------------------------------------------------------------------
# <project>.dep & <projekt>.lnk & <project>.bin and.....
# all in err\ & obj\ & lst\ & lib\ & log\ will be deleted by clean-rule automaticly
# so in this clean-rule it is only necessary to define additional files which
# were not delete automaticly.
# e.g.: $(<PATH>)\can_test.c
#-------------------------------------------------------------------------------------------------
MAKE_CLEAN_RULES        +=
#MAKE_GENERATE_RULES     +=
#MAKE_COMPILER_RULES     +=
#MAKE_DEBUG_RULES        +=
#MAKE_CONFIG_RULES       +=
#MAKE_ADD_RULES          +=


###############################################################
# REQUIRED   (defined in BaseMake (global.Makefile.target.make...))
#
# SSC_ROOT      (required)
# PROJECT_ROOT  (required)
#
# LIB_OUTPUT_PATH  (optional)
# OBJ_OUTPUT_PATH  (optional)
#
# OBJ_FILE_SUFFIX
# LIB_FILE_SUFFIX
#
###############################################################


###############################################################
# PROVIDE   this Section can be used to define own additional rules
#
# In vendorx_can_cfg.mak:
# Please configure the project file:
#CAN_CONFIG_FILE = $(PROJECT_ROOT)\source\network\can\my_can_config.cfg

#In vendorx_can_config :
#generate_can_config:
#$(SSC_ROOT)\core\com\can\tools\canconfiggen.exe -o $(CAN_CONFIG_FILE)


###############################################################
# SPECIFIC
#
# There are no rules defined for the Specific part of the
# Rules-Makefile. Each author is free to create temporary
# variables or to use other resources of GNU-MAKE
#
###############################################################


