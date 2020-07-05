# Name of target controller 
MCU=atmega644p
MCU_FREQ=12000000 

# Name of our project
PROJECTNAME=liblcd

# Source files
PRJSRC=lcd.c ili934x.c font.c

# Optimization level, 
OPTLEVEL=s

# compiler
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -DF_CPU=$(MCU_FREQ) -O$(OPTLEVEL) \
	-fpack-struct -funsigned-bitfields -funsigned-char    \
	-Wall -Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

# Archiver
ARFLAGS=rcs

##### executables ####
CC=avr-gcc
AR=avr-ar
REMOVE=rm -f

##### automatic target names ####
LIBTRG=$(PROJECTNAME).a

# List all object files we need to create
CFILES=$(filter %.c, $(PRJSRC))
OBJDEPS=$(CFILES:.c=.o) 

# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

.SUFFIXES : .c .o .h

.PHONY: clean

# Make targets:
all: $(LIBTRG)

$(LIBTRG): $(OBJDEPS) 
	$(AR) $(ARFLAGS) $(LIBTRG) $(OBJDEPS)

#### Generating object files ####
.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

#### Cleanup ####
clean:
	$(REMOVE) $(LIBTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST)
	
