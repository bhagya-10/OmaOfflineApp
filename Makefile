##############################################################################################
#
#       !!!! Do NOT edit this makefile with an editor which replace tabs by spaces !!!!    
#edit by liyo from newpos 20170602
##############################################################################################
# 
# On command line:
#
# make all = Create project
# make clean = Clean project files.
# To rebuild project do "make clean" and "make all".
#

export POSDIR=$(shell pwd)

# Define project name here
PROJECT= $(shell basename `pwd`)

#ISSUE?= Debug

ifeq ($(ISSUE),Debug)
CUR_VERSION= "Debug"
obj= ./Debug/
OPTFLAGS = -O0
DEBUGFLAG = -g -ggdb
#DEBUGFLAG += -Wno-strict-aliasing
#DEBUGFLAG += -Wa,-adhlns="$(@:.o=.lst)"
else
CUR_VERSION	= "Release"
obj	= ./Release/
OPTFLAGS = -O2
endif

ifndef TOOLCHAIN_PATH
TOOLCHAIN_PATH = ./toolchain/glibc-oabi-toolchain-arm-generic/bin/
endif

ifndef CROSS_COMPILE
CROSS_COMPILE = $(TOOLCHAIN_PATH)arm-unknown-linux-gnu-
endif

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AS = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
LINK = $(CROSS_COMPILE)gcc
RANLIB = $(CROSS_COMPILE)ranlib
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip


MKDIR = $(TOOLCHAIN_PATH)mkdir
ECHO = $(TOOLCHAIN_PATH)echo
DEL = $(TOOLCHAIN_PATH)rm
MV = $(TOOLCHAIN_PATH)/mv
#********src_path************
COMM_SRC_PATH = Src
#application name
APP_NAME = OFFLINE_APP

#********soft-sdk path*******
sdkExtend=./LIB/sdk
sdkSeosPath=./LIB/SEOS/libs
sdkExtendDir = $(sdkExtend)/sdk-new8210-1.2.4/sdk/include
sdkExtendLib = $(sdkExtend)/sdk-new8210-1.2.4/sdk/lib
sdkExtendDir += $(sdkExtend)/sdk-new8210-1.2.4/sdk/include/directfb
#*********support-sdk path*******
pubExtendDir =  $(sdkSeosPath)/libseos/include
pubExtendDir += $(sdkSeosPath)/libposts/include
pubExtendDir += $(sdkSeosPath)/libtms/include
pubExtendDir += $(sdkSeosPath)/libwnet/include
pubExtendDir += $(sdkSeosPath)/fribidi/include

pubSeosLib = $(sdkSeosPath)/libseos/lib
pubPostLib = $(sdkSeosPath)/libposts/lib
pubTmsLib = $(sdkSeosPath)/libtms/lib
pubWnetLib = $(sdkSeosPath)/libwnet/lib
pubFribidiLib = $(sdkSeosPath)/fribidi/lib
##############################################################################################
# Start of user section
##############################################################################################

# List all user C define here, like -D_DEBUG=1
UDEFS = 

# Define ASM defines here
UADEFS = 

# List the user directory to look for the libraries here
ULIBDIR =                                              


MYDLLS +=-L$(sdkExtendLib)
MYDLLS +=-L$(pubTmsLib)
MYDLLS +=-L$(pubSeosLib)
MYDLLS +=-L$(pubWnetLib)
MYDLLS +=-L$(pubPostLib)
MYDLLS +=-L$(pubFribidiLib)
# List all user libraries here
ULIBS = -lped -lepp -lts -lpos -lcrypto -lemv -lpos -lnfc -liccard -lz -lfusion -ldirect -ldirectfb -lseos -ltms -lssl -lpng -lzint -lwnet -lposts
#

SDK_LIB = $(MYDLLS) $(ULIBS)

# End of user defines
##############################################################################################

##############################################################################################
# List C source files here
##############################################################################################
SRC-y := $(wildcard $(COMM_SRC_PATH)/*.c)\
$(wildcard $(COMM_SRC_PATH)/*/*.c)


##############################################################################################
# List all user directories here
##############################################################################################
UINCDIR-y := $(pubExtendDir)\
$(sdkExtendDir)
##############################################################################################
INCDIR = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR-y))
LIBDIR = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS = $(DDEFS) $(UDEFS)
ADEFS = $(DADEFS) $(UADEFS)
COBJS = $(patsubst %.c, $(obj)%.o, $(SRC-y))
OBJS = $(patsubst %.s, $(obj)%.o, $(COBJS))
LIBS = $(DLIBS) $(ULIBS)
DEPS = $(OBJS:%.o=%.d)

MCFLAGS = -marm -mapcs -mno-sched-prolog -mabi=apcs-gnu -mlittle-endian -mno-thumb-interwork -msoft-float 

ASFLAGS = $(MCFLAGS) $(DEBUGFLAG) $(ADEFS)
CFLAGS = $(MCFLAGS) $(OPTFLAGS) $(DEBUGFLAG) -Wall -Wundef -Wstrict-prototypes -Werror-implicit-function-declaration -Wdeclaration-after-statement -fsigned-char -fPIC $(DEFS) -c
EXEC_LIB_PATH = -WL,-rpath -WL,"../lib"
LDFLAGS = $(MCFLAGS)$(EXEC_LIB_PATH) 

# Generate dependency information
CFLAGS += -I./ $(INCDIR)

ifdef V
	ifeq ("$(origin V)", "command line")
		KBUILD_VERBOSE = $(V)
	endif
endif

ifndef KBUILD_VERBOSE
	KBUILD_VERBOSE = 0
endif

ifeq ($(KBUILD_VERBOSE),0)
	quiet = quiet_
	Q = @
else
	quiet =
	Q =
endif


##############################################################################################
# The rules for outputing debug information, Do not modify them.
##############################################################################################
quiet_do_cc        = $(Q)$(ECHO)	"  CC       $<" && $(CC)
quiet_do_cxx       = $(Q)$(ECHO)	"  CXX      $<" && $(CXX)
quiet_do_as        = $(Q)$(ECHO)	"  AS       $<" && $(AS)
quiet_do_ar        = $(Q)$(ECHO)	"  AR       $@" && $(AR)
quiet_do_ranlib    = $(Q)$(ECHO)	"  RANLIB   $@" && $(RANLIB)
quiet_do_copy      = $(Q)$(ECHO)	"  OBJCOPY  $@" && $(OBJCOPY)
quiet_do_link      = $(Q)$(ECHO)	"  LINK     $@" && $(LINK)
quiet_do_strip     = $(Q)$(ECHO)	"  STRIP    $@" && $(STRIP)
quiet_do_mkver     = $(Q)$(ECHO)	"  MAKEVER  $@" && 
quiet_do_transfer  = $(Q)$(ECHO)	"  TRANSF   $@" && $(TRANSFER)
quiet_do_compress  = $(Q)$(ECHO)	"  COMPRES  $@" && $(COMPRESS)

do_cc              = $(CC)
do_cxx             = $(CXX)
do_as              = $(AS)
do_ar              = $(AR)
do_ranlib          = $(RANLIB)
do_copy            = $(OBJCOPY)
do_link            = $(LINK)
do_strip           = $(STRIP)
do_mkver           = 
do_transfer        = $(TRANSFER)
do_compress        = $(COMPRESS)

##############################################################################################
# makefile rules
##############################################################################################

##############################################################################################
# makefile rules
##############################################################################################
MAIN_BIN = $(APP_NAME)

all: info $(MAIN_BIN)
	@$(ECHO) ""
	@$(ECHO) "+---------------------------------------------------"
	@$(ECHO) "|"
	@$(ECHO) "|   Finished building target: $(APP_NAME)"
	@$(ECHO) "|   edited by liyo from newpos 20190919"
	@$(ECHO) "+---------------------------------------------------"

info:
	@$(ECHO) "****************************************************"
	@$(ECHO) "*             Compile $(CUR_VERSION)" version ...
	@$(ECHO) "****************************************************"
	@$(ECHO) ""


$(obj)%.o : %.c
	@[ -d $(dir $@) ] || $(MKDIR) -p $(dir $@)
	$($(quiet)do_cc) -c $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"

$(obj)%.o : %.s
	@[ -d $(dir $@) ] || $(MKDIR) -p $(dir $@)
	$($(quiet)do_as) -c $(ASFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"

$(obj)$(MAIN_BIN): $(OBJS)
	$($(quiet)do_link) $(LDFLAGS) -fPIC -Wl,-soname=$(MAIN_BIN) $(OBJS) $(LIBS) -o"$@"
	$($(quiet)do_strip) --strip-all $(obj)$(MAIN_BIN)

$(obj)$(APP_NAME).a: $(OBJS)
	$($(quiet)do_ar) cru "$@" $^
	$($(quiet)do_ranlib) "$@"

$(MAIN_BIN): $(OBJS)
	$(CC) $(MAIN_INCLUDES) -o $(MAIN_BIN) $^ $(SDK_LIB) $(LDFLAGS)  -lm -lpthread -ldl -lrt
	#@chmod +x $(MAIN_BIN)
	$($(quiet)do_strip) --strip-all $(MAIN_BIN)
	MV  $(MAIN_BIN) $(obj)$(APP_NAME)	
	


clean:
	-$(DEL) -f $(MAIN_BIN)
	-$(DEL) -f $(obj)$(APP_NAME)
	-$(DEL) -f $(OBJS)
	-$(DEL) -f $(DEPS)
	-$(DEL) -f $(patsubst %.o,%.lst,$(OBJS))

distclean:
	-$(DEL) -rf $(obj)

############################################################################################## 
# Include the dependency files, should be the last of the makefile
##############################################################################################
-include $(DEPS)

# *** EOF ***