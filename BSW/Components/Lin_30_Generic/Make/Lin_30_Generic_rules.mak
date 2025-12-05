############################################################################### 
# File Name  : Lin_30_Generic_rules.mak 
# Description: Rules makefile 
#-----------------------------------------------------------------------------
# COPYRIGHT
#-----------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.  All rights reserved.
#-----------------------------------------------------------------------------
# REVISION HISTORY
#-----------------------------------------------------------------------------
# Date         Version  Sign Description
# ----------   -------  ---- -------------------------------------------------
# 2018-06-15   4.00.00  Jgl  Initial Version of file
# 2019-03-28   4.00.01  Jgl  Change file variable to correct name
#                            Added support of component-based SIP structure
#-----------------------------------------------------------------------------
# TemplateVersion = 1.01
############################################################################### 

# Component Files
CC_FILES_TO_BUILD         += Lin_30_Generic$(BSW_SRC_DIR)\Lin_30_Generic.c
GENERATED_SOURCE_FILES    += $(GENDATA_DIR)\Lin_30_Generic_*.c

# Library Settings
LIBRARIES_TO_BUILD        += Lin_30_Generic
Lin_30_Generic_FILES      += Lin_30_Generic$(BSW_SRC_DIR)\Lin_30_Generic.c