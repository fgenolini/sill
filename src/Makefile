# Application = sill, as in window sill
# Makefile for Linux
#   Tested on Ubuntu Linux 22.10
#   Using standard C18

APPLICATION     = ../bin/sill
INC_FOLDER      = /usr/local/include
INCLUDES        = -I$(INC_FOLDER) -I../include
INC_DEP         = $(INC_FOLDER)/raylib.h ../include/say.h ../include/init.h ../include/main_loop.h
RAYLIB_LIB_PATH = /usr/local/lib
LDFLAGS        += -L$(RAYLIB_LIB_PATH)
LDLIBS          = -lraylib -lGL -lm -lpthread -ldl -lrt
LDLIBS         += -lX11
PLATFORM        = PLATFORM_DESKTOP
OPTIMISE        = -flto -g0 -O3
COPT            = $(OPTIMISE) $(INCLUDES)
CFLAGS          = -Wall -std=c18 -Wno-missing-braces -Wunused-result $(COPT)
DEFINES         = -D_DEFAULT_SOURCE -D$(PLATFORM)
CPPFLAGS       += $(DEFINES)
SOURCES         = sill.c init.c say.c main_loop.c
OBJDIR          = ../obj
OBJECTS         = $(OBJDIR)/sill.o $(OBJDIR)/init.o $(OBJDIR)/say.o $(OBJDIR)/main_loop.o

.PHONY: all clean

all: $(APPLICATION)

clean:
	-rm -f $(APPLICATION) $(OBJECTS)

$(APPLICATION): $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(OBJDIR)/%.o: %.c $(INC_DEP)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

