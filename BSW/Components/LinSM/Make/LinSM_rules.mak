############################################################################### 
# File Name  : LinSM_rules.mak 
# Description: Rules makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author  Description
#------------------------------------------------------------------------------
# 1.00.00   2007-06-13  Aba     Initial Version of Template (1.0)
# 1.00.00   2007-06-13  Aba     Initial Version of Template (1.0)
# 1.01.00   2008-04-30  fbg     Changes for ASR library support
# 1.00.01   2012-11-28  vismpf  Adapted to LinSM module
# 3.00.00   2014-11-20  vissrk  Fixed ESCAN00079705
# 3.00.01   2017-12-15  vismpf  Fixed ESCAN00097761
# 3.01.00   2017-12-15  vismpf  Clean Up
# 3.02.00   2019-02-06  vircbl  Added support of component-based SIP structure
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += LinSM$(BSW_SRC_DIR)\LinSM.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\LinSM_Cfg.c $(GENDATA_DIR)\LinSM_PBcfg.c

# Library Settings
LIBRARIES_TO_BUILD      += LinSM
LinSM_FILES              = LinSM$(BSW_SRC_DIR)\LinSM.c

