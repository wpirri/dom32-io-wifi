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
FINAL_IMAGE=${DISTDIR}/Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=flash/plib_nvm.c mcc_generated_files/system.c mcc_generated_files/pin_manager.c mcc_generated_files/interrupt_manager.c mcc_generated_files/coretimer.c mcc_generated_files/clock.c mcc_generated_files/exceptions.c mcc_generated_files/uart1.c mcc_generated_files/mcc.c mcc_generated_files/uart2.c mcc_generated_files/rtcc.c sd/fsio.c sd/sd_spi.c main.c config.c control.c http.c strfunc.c wifi.c wiegand.c delay.c adc.c clog.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/flash/plib_nvm.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/coretimer.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/exceptions.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/uart2.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/sd/fsio.o ${OBJECTDIR}/sd/sd_spi.o ${OBJECTDIR}/main.o ${OBJECTDIR}/config.o ${OBJECTDIR}/control.o ${OBJECTDIR}/http.o ${OBJECTDIR}/strfunc.o ${OBJECTDIR}/wifi.o ${OBJECTDIR}/wiegand.o ${OBJECTDIR}/delay.o ${OBJECTDIR}/adc.o ${OBJECTDIR}/clog.o
POSSIBLE_DEPFILES=${OBJECTDIR}/flash/plib_nvm.o.d ${OBJECTDIR}/mcc_generated_files/system.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/coretimer.o.d ${OBJECTDIR}/mcc_generated_files/clock.o.d ${OBJECTDIR}/mcc_generated_files/exceptions.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/uart2.o.d ${OBJECTDIR}/mcc_generated_files/rtcc.o.d ${OBJECTDIR}/sd/fsio.o.d ${OBJECTDIR}/sd/sd_spi.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/config.o.d ${OBJECTDIR}/control.o.d ${OBJECTDIR}/http.o.d ${OBJECTDIR}/strfunc.o.d ${OBJECTDIR}/wifi.o.d ${OBJECTDIR}/wiegand.o.d ${OBJECTDIR}/delay.o.d ${OBJECTDIR}/adc.o.d ${OBJECTDIR}/clog.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/flash/plib_nvm.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/coretimer.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/exceptions.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/uart2.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/sd/fsio.o ${OBJECTDIR}/sd/sd_spi.o ${OBJECTDIR}/main.o ${OBJECTDIR}/config.o ${OBJECTDIR}/control.o ${OBJECTDIR}/http.o ${OBJECTDIR}/strfunc.o ${OBJECTDIR}/wifi.o ${OBJECTDIR}/wiegand.o ${OBJECTDIR}/delay.o ${OBJECTDIR}/adc.o ${OBJECTDIR}/clog.o

# Source Files
SOURCEFILES=flash/plib_nvm.c mcc_generated_files/system.c mcc_generated_files/pin_manager.c mcc_generated_files/interrupt_manager.c mcc_generated_files/coretimer.c mcc_generated_files/clock.c mcc_generated_files/exceptions.c mcc_generated_files/uart1.c mcc_generated_files/mcc.c mcc_generated_files/uart2.c mcc_generated_files/rtcc.c sd/fsio.c sd/sd_spi.c main.c config.c control.c http.c strfunc.c wifi.c wiegand.c delay.c adc.c clog.c



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
ProjectDir="C:\Users\walte\OneDrive\Documentos\Git\dom32-io-wifi\Programa"
ProjectName=Dom32-IO-WiFi
ConfName=default
ImagePath="${DISTDIR}\Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="${DISTDIR}"
ImageName="Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [cp dist\default\production\Programa.production.hex pgm.hex]"
	@cp dist\default\production\Programa.production.hex pgm.hex
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
${OBJECTDIR}/flash/plib_nvm.o: flash/plib_nvm.c  .generated_files/flags/default/b385f6f289da5f3772af6e05f6cb8a0f8a26fec8 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/flash" 
	@${RM} ${OBJECTDIR}/flash/plib_nvm.o.d 
	@${RM} ${OBJECTDIR}/flash/plib_nvm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/flash/plib_nvm.o.d" -o ${OBJECTDIR}/flash/plib_nvm.o flash/plib_nvm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/5fb80b3aaed05789a5ee421c6b442c6873297d34 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d" -o ${OBJECTDIR}/mcc_generated_files/system.o mcc_generated_files/system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/62f9d240f127c2e73f114971dd718c6719f19f1f .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o mcc_generated_files/pin_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/fa5b9a8eb5057cc3cc88b846444a99ca05fef759 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o mcc_generated_files/interrupt_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/coretimer.o: mcc_generated_files/coretimer.c  .generated_files/flags/default/7e956c8f526f66692860643f32a37cbc9bcb425f .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/coretimer.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/coretimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/coretimer.o.d" -o ${OBJECTDIR}/mcc_generated_files/coretimer.o mcc_generated_files/coretimer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/13dfb75814fb6c2fef05ee6a3824bc6afec9488b .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d" -o ${OBJECTDIR}/mcc_generated_files/clock.o mcc_generated_files/clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/exceptions.o: mcc_generated_files/exceptions.c  .generated_files/flags/default/5ece0264f12e68afd2b60cf2e9db775dd6b768b0 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/exceptions.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/exceptions.o.d" -o ${OBJECTDIR}/mcc_generated_files/exceptions.o mcc_generated_files/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/1c964388c1bf328f6cb8d14d1fbd4c3196680977 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d" -o ${OBJECTDIR}/mcc_generated_files/uart1.o mcc_generated_files/uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/5059f13fa080bb719c18b7c73cbf997126405f00 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d" -o ${OBJECTDIR}/mcc_generated_files/mcc.o mcc_generated_files/mcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/uart2.o: mcc_generated_files/uart2.c  .generated_files/flags/default/ede4185014c83970ac113db6f20803776ed6ede9 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart2.o.d" -o ${OBJECTDIR}/mcc_generated_files/uart2.o mcc_generated_files/uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  .generated_files/flags/default/5af4ece730b2d69bf9cc6c541e680a75bd5422f8 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d" -o ${OBJECTDIR}/mcc_generated_files/rtcc.o mcc_generated_files/rtcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/sd/fsio.o: sd/fsio.c  .generated_files/flags/default/4d5375868b9c3c63cb26e61e329124db01c04afc .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/sd" 
	@${RM} ${OBJECTDIR}/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/sd/fsio.o.d" -o ${OBJECTDIR}/sd/fsio.o sd/fsio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/sd/sd_spi.o: sd/sd_spi.c  .generated_files/flags/default/fcc5c288c04dde9abedeccc27a3a04e36622498c .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/sd" 
	@${RM} ${OBJECTDIR}/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/sd/sd_spi.o.d" -o ${OBJECTDIR}/sd/sd_spi.o sd/sd_spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/90c9e26db987de5e2da773ed0203bf22423cfc3 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/config.o: config.c  .generated_files/flags/default/7da0aabb60714ff81132d2c559b48de7f08dea04 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.o.d 
	@${RM} ${OBJECTDIR}/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/config.o.d" -o ${OBJECTDIR}/config.o config.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/control.o: control.c  .generated_files/flags/default/2a8ab0c61acd7f2f2b62be45041f3924fd924778 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/control.o.d 
	@${RM} ${OBJECTDIR}/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/control.o.d" -o ${OBJECTDIR}/control.o control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/http.o: http.c  .generated_files/flags/default/622735566b33edbd5cf1fe1339088bc25fce9990 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/http.o.d 
	@${RM} ${OBJECTDIR}/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/http.o.d" -o ${OBJECTDIR}/http.o http.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/strfunc.o: strfunc.c  .generated_files/flags/default/3592eabff0e6ce31351e56e2a7921213025fc3a5 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/strfunc.o.d 
	@${RM} ${OBJECTDIR}/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/strfunc.o.d" -o ${OBJECTDIR}/strfunc.o strfunc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/wifi.o: wifi.c  .generated_files/flags/default/afedfd8972d99d08b8d0ce83c812758563b1e1e4 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wifi.o.d 
	@${RM} ${OBJECTDIR}/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/wifi.o.d" -o ${OBJECTDIR}/wifi.o wifi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/wiegand.o: wiegand.c  .generated_files/flags/default/d6a1ea7ef17546b832c7f760bf49a4f73cdefe88 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wiegand.o.d 
	@${RM} ${OBJECTDIR}/wiegand.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/wiegand.o.d" -o ${OBJECTDIR}/wiegand.o wiegand.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/delay.o: delay.c  .generated_files/flags/default/922772bb36c86cff40e85a1737dd55218a341927 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/delay.o.d 
	@${RM} ${OBJECTDIR}/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/delay.o.d" -o ${OBJECTDIR}/delay.o delay.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/f2a3356a7dc654bcbf3f030010cf2e14d901bb75 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/adc.o.d" -o ${OBJECTDIR}/adc.o adc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/clog.o: clog.c  .generated_files/flags/default/1f0edfddd0aae9aec25fdb3110ca82c61fc109ba .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clog.o.d 
	@${RM} ${OBJECTDIR}/clog.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/clog.o.d" -o ${OBJECTDIR}/clog.o clog.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/flash/plib_nvm.o: flash/plib_nvm.c  .generated_files/flags/default/871ae5921243a0df531fd24a20db6e63395c65a0 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/flash" 
	@${RM} ${OBJECTDIR}/flash/plib_nvm.o.d 
	@${RM} ${OBJECTDIR}/flash/plib_nvm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/flash/plib_nvm.o.d" -o ${OBJECTDIR}/flash/plib_nvm.o flash/plib_nvm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/406d4c060c56a96722915bf95f6bc82fdc3af766 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d" -o ${OBJECTDIR}/mcc_generated_files/system.o mcc_generated_files/system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/8f5d01699dbb44c2a4763ecaac27a952d5bcbeff .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o mcc_generated_files/pin_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/c409b91eeaf8d3f2fb49c03e73c097c6f9a0e21b .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o mcc_generated_files/interrupt_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/coretimer.o: mcc_generated_files/coretimer.c  .generated_files/flags/default/db540eaf5301a394ab67cfcacbeced875b123086 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/coretimer.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/coretimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/coretimer.o.d" -o ${OBJECTDIR}/mcc_generated_files/coretimer.o mcc_generated_files/coretimer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/a9ceccbc4fc5870c14f47e19c40df89830e6ec0a .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d" -o ${OBJECTDIR}/mcc_generated_files/clock.o mcc_generated_files/clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/exceptions.o: mcc_generated_files/exceptions.c  .generated_files/flags/default/7e050cb9f89d06af0d5fd2124b9f6ca3398e4306 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/exceptions.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/exceptions.o.d" -o ${OBJECTDIR}/mcc_generated_files/exceptions.o mcc_generated_files/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/86f2aa111b6eb7065ceb75b04813e927b415ac0a .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d" -o ${OBJECTDIR}/mcc_generated_files/uart1.o mcc_generated_files/uart1.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/6e7d654e0105dbfc4fbf1e482bceb876d41737b6 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d" -o ${OBJECTDIR}/mcc_generated_files/mcc.o mcc_generated_files/mcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/uart2.o: mcc_generated_files/uart2.c  .generated_files/flags/default/93128e6635144627fd007d4709ac6e8f06e0af9 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart2.o.d" -o ${OBJECTDIR}/mcc_generated_files/uart2.o mcc_generated_files/uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  .generated_files/flags/default/c8694f5e69edce12251694a241549cf5bac51f6a .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d" -o ${OBJECTDIR}/mcc_generated_files/rtcc.o mcc_generated_files/rtcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/sd/fsio.o: sd/fsio.c  .generated_files/flags/default/56f29f0b5f49ff92dbb743ac1612007ee5699d5a .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/sd" 
	@${RM} ${OBJECTDIR}/sd/fsio.o.d 
	@${RM} ${OBJECTDIR}/sd/fsio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/sd/fsio.o.d" -o ${OBJECTDIR}/sd/fsio.o sd/fsio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/sd/sd_spi.o: sd/sd_spi.c  .generated_files/flags/default/d7b56910fc917adb63d117953c7934fdfd5fb732 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}/sd" 
	@${RM} ${OBJECTDIR}/sd/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/sd/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/sd/sd_spi.o.d" -o ${OBJECTDIR}/sd/sd_spi.o sd/sd_spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/61e4d7cc74761653497d6ae42b512d2d8492dc1f .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/config.o: config.c  .generated_files/flags/default/c122705922390072b51272690fa6abfa00c3f0bd .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.o.d 
	@${RM} ${OBJECTDIR}/config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/config.o.d" -o ${OBJECTDIR}/config.o config.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/control.o: control.c  .generated_files/flags/default/e668d6fec4fd3cd97252d43de9863704a33ad596 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/control.o.d 
	@${RM} ${OBJECTDIR}/control.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/control.o.d" -o ${OBJECTDIR}/control.o control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/http.o: http.c  .generated_files/flags/default/b229c33439072b86026c2737a2d53d26b4c6286a .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/http.o.d 
	@${RM} ${OBJECTDIR}/http.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/http.o.d" -o ${OBJECTDIR}/http.o http.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/strfunc.o: strfunc.c  .generated_files/flags/default/29b9bd635085f5279975c11bc74d3e9ce2070d1f .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/strfunc.o.d 
	@${RM} ${OBJECTDIR}/strfunc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/strfunc.o.d" -o ${OBJECTDIR}/strfunc.o strfunc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/wifi.o: wifi.c  .generated_files/flags/default/5e5a6f06bb3f76bef61f770580e4422cbbe41ce8 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wifi.o.d 
	@${RM} ${OBJECTDIR}/wifi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/wifi.o.d" -o ${OBJECTDIR}/wifi.o wifi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/wiegand.o: wiegand.c  .generated_files/flags/default/80aaae14ff3d1dcf217e492c72c7c90439940e44 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wiegand.o.d 
	@${RM} ${OBJECTDIR}/wiegand.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/wiegand.o.d" -o ${OBJECTDIR}/wiegand.o wiegand.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/delay.o: delay.c  .generated_files/flags/default/38ca1e1a0edf980bd91c775144099c617d280e3e .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/delay.o.d 
	@${RM} ${OBJECTDIR}/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/delay.o.d" -o ${OBJECTDIR}/delay.o delay.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/592abbde744d777f2acbc07bfe63a3d5211b7335 .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/adc.o.d" -o ${OBJECTDIR}/adc.o adc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/clog.o: clog.c  .generated_files/flags/default/ce1eb15509db880fb520485dad9a8adf1c89ea5a .generated_files/flags/default/8c9efd018bf36f61fa53e2f3fa7357a06b4304bf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clog.o.d 
	@${RM} ${OBJECTDIR}/clog.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -fno-common -MP -MMD -MF "${OBJECTDIR}/clog.o.d" -o ${OBJECTDIR}/clog.o clog.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../pic32mmBoot/Programa/app_p32MM0256GPM028.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=257,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/Programa.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../pic32mmBoot/Programa/app_p32MM0256GPM028.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/Programa.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=257,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-s,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/Programa.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
	@echo Normalizing hex file
	@"C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="C:/Program Files/Microchip/MPLABX/v6.00/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" ${DISTDIR}/Programa.${IMAGE_TYPE}.hex -o${DISTDIR}/Programa.${IMAGE_TYPE}.hex

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
