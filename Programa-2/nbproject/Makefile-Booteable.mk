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
ifeq "$(wildcard nbproject/Makefile-local-Booteable.mk)" "nbproject/Makefile-local-Booteable.mk"
include nbproject/Makefile-local-Booteable.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Booteable
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
SOURCEFILES_QUOTED_IF_SPACED=src/misc/delay.c src/misc/strfunc.c src/sd/fsio.c src/sd/sd_spi.c src/wifi/wifi.c src/wifi/uart1.c src/config.c src/control.c src/http.c src/log.c src/main.c src/rtcc.c src/system.c src/io.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/misc/delay.o ${OBJECTDIR}/src/misc/strfunc.o ${OBJECTDIR}/src/sd/fsio.o ${OBJECTDIR}/src/sd/sd_spi.o ${OBJECTDIR}/src/wifi/wifi.o ${OBJECTDIR}/src/wifi/uart1.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/control.o ${OBJECTDIR}/src/http.o ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/io.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/misc/delay.o.d ${OBJECTDIR}/src/misc/strfunc.o.d ${OBJECTDIR}/src/sd/fsio.o.d ${OBJECTDIR}/src/sd/sd_spi.o.d ${OBJECTDIR}/src/wifi/wifi.o.d ${OBJECTDIR}/src/wifi/uart1.o.d ${OBJECTDIR}/src/config.o.d ${OBJECTDIR}/src/control.o.d ${OBJECTDIR}/src/http.o.d ${OBJECTDIR}/src/log.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/rtcc.o.d ${OBJECTDIR}/src/system.o.d ${OBJECTDIR}/src/io.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/misc/delay.o ${OBJECTDIR}/src/misc/strfunc.o ${OBJECTDIR}/src/sd/fsio.o ${OBJECTDIR}/src/sd/sd_spi.o ${OBJECTDIR}/src/wifi/wifi.o ${OBJECTDIR}/src/wifi/uart1.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/control.o ${OBJECTDIR}/src/http.o ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/io.o

# Source Files
SOURCEFILES=src/misc/delay.c src/misc/strfunc.c src/sd/fsio.c src/sd/sd_spi.c src/wifi/wifi.c src/wifi/uart1.c src/config.c src/control.c src/http.c src/log.c src/main.c src/rtcc.c src/system.c src/io.c



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

# The following macros may be used in the pre and post step lines
_/_=\\
ShExtension=.bat
Device=PIC32MM0256GPM028
ProjectDir="C:\Users\walte\OneDrive\Documentos\Git\dom32-io-wifi\Programa-2"
ProjectName=Dom32P-IO
ConfName=Booteable
ImagePath="${DISTDIR}\Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="${DISTDIR}"
ImageName="Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Booteable.mk ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [cp dist/Booteable/production/Programa-2.production.hex ./pgm.hex]"
	@cp dist/Booteable/production/Programa-2.production.hex ./pgm.hex
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=32MM0256GPM028
MP_LINKER_FILE_OPTION=,--script="..\..\pic32mmBoot\Programa\app_p32MM0256GPM028.ld"
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
${OBJECTDIR}/src/misc/delay.o: src/misc/delay.c  .generated_files/flags/Booteable/e5544d334d13a39d01b5a9dffd391037e1e2e6df .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/delay.o.d 
	@${RM} ${OBJECTDIR}/src/misc/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/delay.o.d" -o ${OBJECTDIR}/src/misc/delay.o src/misc/delay.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/misc/strfunc.o: src/misc/strfunc.c  .generated_files/flags/Booteable/9d3232002ba082fcbeaec7ed3c587bfdfabff67a .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o.d 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/strfunc.o.d" -o ${OBJECTDIR}/src/misc/strfunc.o src/misc/strfunc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/fsio.o: src/sd/fsio.c  .generated_files/flags/Booteable/b5eff75a59f7dc3cf831a5a40980ae1695dbf7ab .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/fsio.o.d" -o ${OBJECTDIR}/src/sd/fsio.o src/sd/fsio.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/sd_spi.o: src/sd/sd_spi.c  .generated_files/flags/Booteable/61ac25518fb4b4f01612f53e36b4315f55dea5bf .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/sd_spi.o.d" -o ${OBJECTDIR}/src/sd/sd_spi.o src/sd/sd_spi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/wifi.o: src/wifi/wifi.c  .generated_files/flags/Booteable/3333519edb8956e84c4aae4cb4c4ebf7243fcc6a .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/wifi.o.d" -o ${OBJECTDIR}/src/wifi/wifi.o src/wifi/wifi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/uart1.o: src/wifi/uart1.c  .generated_files/flags/Booteable/cad85b3600c20a56ff8ece3884635c868e7b687f .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/uart1.o.d" -o ${OBJECTDIR}/src/wifi/uart1.o src/wifi/uart1.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/config.o: src/config.c  .generated_files/flags/Booteable/7da9ba708d8c8b2c4cced032109a02b2a97bf50c .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/control.o: src/control.c  .generated_files/flags/Booteable/9cdc93ab32a2bd5c571f56481ba3e02bbc4d6d6f .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/control.o.d 
	@${RM} ${OBJECTDIR}/src/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/control.o.d" -o ${OBJECTDIR}/src/control.o src/control.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/http.o: src/http.c  .generated_files/flags/Booteable/3e1921921ee53bb60d61d0094696bcbd1ec6050d .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/http.o.d 
	@${RM} ${OBJECTDIR}/src/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/http.o.d" -o ${OBJECTDIR}/src/http.o src/http.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/log.o: src/log.c  .generated_files/flags/Booteable/980d646b51c1d1fabd2da97aa6336901b04f73dd .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/log.o.d 
	@${RM} ${OBJECTDIR}/src/log.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/log.o.d" -o ${OBJECTDIR}/src/log.o src/log.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/Booteable/9cacf72df9d1c508fc9879db88591ed9d21190d6 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  .generated_files/flags/Booteable/1f917d63788b66423bb444745c1ce8edc4c0458e .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/rtcc.o.d" -o ${OBJECTDIR}/src/rtcc.o src/rtcc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/system.o: src/system.c  .generated_files/flags/Booteable/958c697e2ab125de729333b7a981f118c4e5e423 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/system.o.d" -o ${OBJECTDIR}/src/system.o src/system.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/flags/Booteable/64cdfab81d7d411b96f59b6f1d792840ccf8613 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/io.o.d" -o ${OBJECTDIR}/src/io.o src/io.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/src/misc/delay.o: src/misc/delay.c  .generated_files/flags/Booteable/eb45aaefe52141357aec861bcef8f6433bd9e96e .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/delay.o.d 
	@${RM} ${OBJECTDIR}/src/misc/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/delay.o.d" -o ${OBJECTDIR}/src/misc/delay.o src/misc/delay.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/misc/strfunc.o: src/misc/strfunc.c  .generated_files/flags/Booteable/268af77eb309a1f7dee2e5da27ee5cf4c289bebb .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/misc" 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o.d 
	@${RM} ${OBJECTDIR}/src/misc/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/misc/strfunc.o.d" -o ${OBJECTDIR}/src/misc/strfunc.o src/misc/strfunc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/fsio.o: src/sd/fsio.c  .generated_files/flags/Booteable/27c1286cd9008666795704e4f7a895fa059f81e4 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/src/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/fsio.o.d" -o ${OBJECTDIR}/src/sd/fsio.o src/sd/fsio.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/sd/sd_spi.o: src/sd/sd_spi.c  .generated_files/flags/Booteable/cb800e61b4f7c94272219241f5354e380055b7b0 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/sd" 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/src/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/sd/sd_spi.o.d" -o ${OBJECTDIR}/src/sd/sd_spi.o src/sd/sd_spi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/wifi.o: src/wifi/wifi.c  .generated_files/flags/Booteable/b6695b2b0555cc9c8b8b4ea05d81c8b6433e855f .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/wifi.o.d" -o ${OBJECTDIR}/src/wifi/wifi.o src/wifi/wifi.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/wifi/uart1.o: src/wifi/uart1.c  .generated_files/flags/Booteable/b802f3204bead2cb68b1c140cfabb887438dd173 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src/wifi" 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o.d 
	@${RM} ${OBJECTDIR}/src/wifi/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/wifi/uart1.o.d" -o ${OBJECTDIR}/src/wifi/uart1.o src/wifi/uart1.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/config.o: src/config.c  .generated_files/flags/Booteable/ebfdc73632526571bf64b03030c45b8b2a94dc7a .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/control.o: src/control.c  .generated_files/flags/Booteable/9c505a0ce6ec8d0b089114806f218377c454e4c0 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/control.o.d 
	@${RM} ${OBJECTDIR}/src/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/control.o.d" -o ${OBJECTDIR}/src/control.o src/control.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/http.o: src/http.c  .generated_files/flags/Booteable/6f4b271075933f6d87804c31188f644a3b7a7930 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/http.o.d 
	@${RM} ${OBJECTDIR}/src/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/http.o.d" -o ${OBJECTDIR}/src/http.o src/http.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/log.o: src/log.c  .generated_files/flags/Booteable/1055ab83b7429438da6c774664dc533bf67ed8d7 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/log.o.d 
	@${RM} ${OBJECTDIR}/src/log.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/log.o.d" -o ${OBJECTDIR}/src/log.o src/log.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/Booteable/f4b985c6bc9d5688343b4233d948933061ea755 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  .generated_files/flags/Booteable/182d907bb027049076426d610380f211cba0e264 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/rtcc.o.d" -o ${OBJECTDIR}/src/rtcc.o src/rtcc.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/system.o: src/system.c  .generated_files/flags/Booteable/3f61f2f1fcd6c92942300b8cd345b95cb21d01f8 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/system.o.d" -o ${OBJECTDIR}/src/system.o src/system.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/flags/Booteable/5f15f62a7454e666459b92bab4d26152ef6d6b73 .generated_files/flags/Booteable/89084a3481147dd80e2344e29edcb351eeeb0ca
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -Os -fno-common -MP -MMD -MF "${OBJECTDIR}/src/io.o.d" -o ${OBJECTDIR}/src/io.o src/io.c    -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../pic32mmBoot/Programa/app_p32MM0256GPM028.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -Os -o ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/Programa-2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../pic32mmBoot/Programa/app_p32MM0256GPM028.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -Os -o ${DISTDIR}/Programa-2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_Booteable=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
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
