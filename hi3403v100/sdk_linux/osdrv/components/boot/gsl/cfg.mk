#
#  cfg.mk
#

RAM_BASE  = 0x04010000
TEXT_OFST = 0x2240
TEXT_BASE = $(shell printf "0x%x" $$(($(RAM_BASE) + $(TEXT_OFST))))

#  CFG_DEBUG - debug control, printf
#export CFG_DEBUG=1

#  CFG_EDA_VERIFY - used for EDA verification
#CFG_EDA_VERIFY=1
