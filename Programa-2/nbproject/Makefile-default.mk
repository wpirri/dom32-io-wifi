#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/misc/beep.c src/misc/delay.c src/misc/strfunc.c src/sd/fsio.c src/sd/sd_spi.c src/wifi/wifi.c src/config.c src/control.c src/http.c src/log.c src/main.c src/rtcc.c src/system.c src/wifi/uart1.c src/io.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/misc/beep.o ${OBJECTDIR}/src/misc/delay.o ${OBJECTDIR}/src/misc/strfunc.o ${OBJECTDIR}/src/sd/fsio.o ${OBJECTDIR}/src/sd/sd_spi.o ${OBJECTDIR}/src/wifi/wifi.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/control.o ${OBJECTDIR}/src/http.o ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/wifi/uart1.o ${OBJECTDIR}/src/io.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/misc/beep.o.d ${OBJECTDIR}/src/misc/delay.o.d ${OBJECTDIR}/src/misc/strfunc.o.d ${OBJECTDIR}/src/sd/fsio.o.d ${OBJECTDIR}/src/sd/sd_spi.o.d ${OBJECTDIR}/src/wifi/wifi.o.d ${OBJECTDIR}/src/config.o.d ${OBJECTDIR}/src/control.o.d ${OBJECTDIR}/src/http.o.d ${OBJECTDIR}/src/log.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/rtcc.o.d ${OBJECTDIR}/src/system.o.d ${OBJECTDIR}/src/wifi/uart1.o.d ${OBJECTDIR}/src/io.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/misc/beep.o ${OBJECTDIR}/src/misc/delay.o ${OBJECTDIR}/src/misc/strfunc.o ${OBJECTDIR}/src/sd/fsio.o ${OBJECTDIR}/src/sd/sd_spi.o ${OBJECTDIR}/src/wifi/wifi.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/control.o ${OBJECTDIR}/src/http.o ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/wifi/uart1.o ${OBJECTDIR}/src/io.o

# Source Files
SOURCEFILES=src/misc/beep.c src/misc/delay.c src/misc/strfunc.c src/sd/fsio.c src/sd/sd_spi.c src/wifi/wifi.c src/config.c src/control.c src/http.c src/log.c src/main.c src/rtcc.c src/system.c src/wifi/uart1.c src/io.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MM0256GPM028
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/misc/beep.o: src/misc/beep.c  .generated_files/flags/default/6aa2b32b77013fbecfd4fa685e990377b3efaed7 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/beep.o.d 
	@${RM} ${OBJECTDIR}/src/misc/beep.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/beep.o.d" -o ${OBJECTDIR}/src/misc/beep.o src/misc/beep.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/misc/delay.o: src/misc/delay.c  .generated_files/flags/default/e536fe17b0e5d116bd3ae4797165beb8059037df .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/delay.o.d 
	@${RM} ${OBJECTDIR}/src/misc/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/delay.o.d" -o ${OBJECTDIR}/src/misc/delay.o src/misc/delay.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/misc/strfunc.o: src/misc/strfunc.c  .generated_files/flags/default/460b9360aa2b62348892a768110478afdd6e9a49 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o.d 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/strfunc.o.d" -o ${OBJECTDIR}/src/misc/strfunc.o src/misc/strfunc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/fsio.o: src/sd/fsio.c  .generated_files/flags/default/c347fdb26005ac9d1c391a98cdda167a24419656 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/fsio.o.d" -o ${OBJECTDIR}/src/sd/fsio.o src/sd/fsio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/sd_spi.o: src/sd/sd_spi.c  .generated_files/flags/default/d10129904655a2e2c504266e45dc88b840da7de9 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/sd_spi.o.d" -o ${OBJECTDIR}/src/sd/sd_spi.o src/sd/sd_spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/wifi.o: src/wifi/wifi.c  .generated_files/flags/default/9ebc35704af0f66f891b9a7999f4fe740e296c49 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/wifi.o.d" -o ${OBJECTDIR}/src/wifi/wifi.o src/wifi/wifi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/config.o: src/config.c  .generated_files/flags/default/9bf6378bc281e5effa88f31a3b2865f8f09d3118 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/control.o: src/control.c  .generated_files/flags/default/b78bff0d17ccd46fb66be7c7e7cd950a033c3fc8 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/control.o.d 
	@${RM} ${OBJECTDIR}/src/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/control.o.d" -o ${OBJECTDIR}/src/control.o src/control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/http.o: src/http.c  .generated_files/flags/default/315ee6755f39b893711e4aafe912d8ccf4ffdd57 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/http.o.d 
	@${RM} ${OBJECTDIR}/src/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/http.o.d" -o ${OBJECTDIR}/src/http.o src/http.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/log.o: src/log.c  .generated_files/flags/default/ec82f6e3726c871eedac1bee3e4a823006d44fe1 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/log.o.d 
	@${RM} ${OBJECTDIR}/src/log.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/log.o.d" -o ${OBJECTDIR}/src/log.o src/log.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/default/eaa3c6e8030eca786ec6fb54c58c7bdb13befbf9 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  .generated_files/flags/default/45e36961c17a93fa6e7877973c8ec8070a90e831 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/rtcc.o.d" -o ${OBJECTDIR}/src/rtcc.o src/rtcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/system.o: src/system.c  .generated_files/flags/default/631926e7c8904ea4cdc04cfc16a64507cbeb3349 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/system.o.d" -o ${OBJECTDIR}/src/system.o src/system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/uart1.o: src/wifi/uart1.c  .generated_files/flags/default/be9603259999ad18e4650862adccf6e5314a6138 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/uart1.o.d" -o ${OBJECTDIR}/src/wifi/uart1.o src/wifi/uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/flags/default/af17ce6d38262e8d258493e0b9b17244a7d2be97 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/io.o.d" -o ${OBJECTDIR}/src/io.o src/io.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/src/misc/beep.o: src/misc/beep.c  .generated_files/flags/default/a4ca6f609abe70db36d5435f7641415448123a56 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/beep.o.d 
	@${RM} ${OBJECTDIR}/src/misc/beep.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/beep.o.d" -o ${OBJECTDIR}/src/misc/beep.o src/misc/beep.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/misc/delay.o: src/misc/delay.c  .generated_files/flags/default/6f70de7c9f516112706ee2bacc800639f45bbd42 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/delay.o.d 
	@${RM} ${OBJECTDIR}/src/misc/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/delay.o.d" -o ${OBJECTDIR}/src/misc/delay.o src/misc/delay.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/misc/strfunc.o: src/misc/strfunc.c  .generated_files/flags/default/34c49edbfa0c707f302030cedaa23aecd6b22def .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o.d 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/strfunc.o.d" -o ${OBJECTDIR}/src/misc/strfunc.o src/misc/strfunc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/fsio.o: src/sd/fsio.c  .generated_files/flags/default/acbad740ed37451280e69dc96475b0b0c6364afb .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/fsio.o.d" -o ${OBJECTDIR}/src/sd/fsio.o src/sd/fsio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/sd_spi.o: src/sd/sd_spi.c  .generated_files/flags/default/3b332fd4be58ea1680dfc3987974805317fd9b7f .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/sd_spi.o.d" -o ${OBJECTDIR}/src/sd/sd_spi.o src/sd/sd_spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/wifi.o: src/wifi/wifi.c  .generated_files/flags/default/b024235786e119aea544751cf4a6b5765147b22e .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/wifi.o.d" -o ${OBJECTDIR}/src/wifi/wifi.o src/wifi/wifi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/config.o: src/config.c  .generated_files/flags/default/8d3812b6911e75b5d6ce3edee446f22e04f51634 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/control.o: src/control.c  .generated_files/flags/default/cd3a3910eb6ba6ba5faa075c1035460ba08c5221 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/control.o.d 
	@${RM} ${OBJECTDIR}/src/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/control.o.d" -o ${OBJECTDIR}/src/control.o src/control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/http.o: src/http.c  .generated_files/flags/default/fd8085af023bd9b0487ece8cbf2536c0986956d8 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/http.o.d 
	@${RM} ${OBJECTDIR}/src/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/http.o.d" -o ${OBJECTDIR}/src/http.o src/http.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/log.o: src/log.c  .generated_files/flags/default/643287cb38273194d75b21788619e886fe540d18 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/log.o.d 
	@${RM} ${OBJECTDIR}/src/log.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/log.o.d" -o ${OBJECTDIR}/src/log.o src/log.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/default/f04380ef9c46092dcf2e0a06ebeda006d834ceb1 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  .generated_files/flags/default/b8a1acd3737a3fe21b5750213f141695c6ec143f .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/rtcc.o.d" -o ${OBJECTDIR}/src/rtcc.o src/rtcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/system.o: src/system.c  .generated_files/flags/default/d54845d6ea2ffc97731822cb2d1f416d0562acf6 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/system.o.d" -o ${OBJECTDIR}/src/system.o src/system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/uart1.o: src/wifi/uart1.c  .generated_files/flags/default/425b987afa3c8da9bf03abf55380193499fd721 .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/uart1.o.d" -o ${OBJECTDIR}/src/wifi/uart1.o src/wifi/uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/flags/default/8745174fd71c1df6206ecaeff15a7743ecf71ba .generated_files/flags/default/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/src/io.o.d" -o ${OBJECTDIR}/src/io.o src/io.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
	@echo Normalizing hex file
	@"C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" ${DISTDIR}/Programa-2.${IMAGE_TYPE}.hex -o${DISTDIR}/Programa-2.${IMAGE_TYPE}.hex

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
