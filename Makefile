#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# MAXMOD_SOUNDBANK contains a directory of music and sound effect files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
SOURCES		:=	src
SOURCEARM7  :=  srcarm7
DATA		:=	data  
INCLUDES	:=	include
SPRITES		:=  sprites

GAME_TITLE := Cave Story DS ~ CSE2
GAME_SUBTITLE1 := v0.4
GAME_SUBTITLE2 := github.com/tilderain/CaveStoryNDS
GAME_ICON := $(CURDIR)/../icon.bmp


ifeq ($(NITRO), 1)
NITRODATA	:=	game_english_bmp
endif
ifneq ($(strip $(NITRODATA)),)
	export NITRO_FILES	:=	$(CURDIR)/$(NITRODATA)
endif
export TOPDIR	:=	$(CURDIR)

#---------------------------------------------------------------------------------
# options for code generation
export SDL_LIB	:=	$(DEVKITPRO)/msys2/usr/lib/SDL
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:= -g -Wall -Os\
 		 -mcpu=arm946e-s -march=armv5te -mtune=arm946e-s -fomit-frame-pointer\
		-ffast-math -DFIX_BUGS \
		$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9 -DLODEPNG_NO_COMPILE_ANCILLARY_CHUNKS -DLODEPNG_NO_COMPILE_ENCODER -DLODEPNG_NO_COMPILE_DISK -DLODEPNG_NO_COMPILE_ERROR_TEXT -DLODEPNG_NO_COMPILE_CPP

ifeq ($(NITRO), 1)
	CFLAGS	+= -DREAD_FROM_SD -DNITROFS
endif

ifeq ($(SD), 1)
	CFLAGS	+= -DREAD_FROM_SD
endif

ifeq ($(JPN), 1)
	CFLAGS	+= -DJAPANESE
endif

ifeq ($(PROFILER), 1)
	CFLAGS	+= -DCYG_PROFILER -finstrument-functions
endif

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map) -L$(SDL_LIB)


#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project (order is important)
#---------------------------------------------------------------------------------
LIBS	:= 	-lfilesystem -lfat -ldswifi9 -lnds9 
 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBNDS)
 
#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIRREAL	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(SOURCES)/pxtone/,$(CURDIR)/$(dir)) \
					$(foreach dir,$(SOURCES)/pxtone/libogg,$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(SPRITES),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CFILES	+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/pxtone/libogg/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
CPPFILES	+=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/pxtone/*.cpp)))

SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
SPRITE_FILES   :=  $(foreach dir, $(SPRITES),$(notdir $(wildcard $(dir)/*.png)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(SPRITE_FILES:.png=.o) \
				$(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)  -I/opt/devkitpro/devkitARM/arm-none-eabi
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
 
.PHONY: $(BUILD) clean
 
#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
 
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -f $(BUILD)/* $(TARGET).elf $(TARGET).nds
	@make -C srcarm7 clean

#---------------------------------------------------------------------------------
else
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

.PHONY: $(SOURCEARM7)/CSE2-arm7.elf

$(OUTPUT).nds	: 	$(OUTPUT).elf $(SOURCEARM7)/CSE2-arm7.elf
$(OUTPUT).nds	: 	$(shell find $(TOPDIR)/$(NITRODATA))
$(OUTPUT).elf	:	$(OFILES)

$(SOURCEARM7)/CSE2-arm7.elf:
	$(MAKE) -C $(TOPDIRREAL)/$(SOURCEARM7)

#---------------------------------------------------------------------------------
%.s %.h : %.png
	grit $< -ff../sprites/sprite.grit -o$*
#---------------------------------------------------------------------------------

	
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)
 
-include $(DEPSDIR)/*.d
 
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------

ifeq ($(DSI), 1)
%.nds: %.elf
	$(SILENTCMD)ndstool -c $@ -9 $< -7 $(TOPDIRREAL)/$(SOURCEARM7)/CSE2-arm7.elf -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)" $(_ADDFILES)
	echo built ... $(notdir $@)
else
%.nds: %.elf
	$(SILENTCMD)ndstool -h 0x200 -c $@ -9 $< -7 $(TOPDIRREAL)/$(SOURCEARM7)/CSE2-arm7.elf -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)" $(_ADDFILES)
	echo built ... $(notdir $@)
endif
