# Application = sill, as in window sill
# Makefile for Linux
#   Tested on Ubuntu Linux 22.10, with GCC 12.2
#   Using standard C23 (GCC C2x = some of C23)

APPLICATION     = ../bin/sill
INC_FOLDER      = /usr/local/include
INCLUDES        = -I$(INC_FOLDER) -I../include
INC_DEP         = $(INC_FOLDER)/raylib.h ../include/say.h ../include/init.h \
                  ../include/main_loop.h ../include/load_resources.h \
				  ../include/object/collision.h ../include/object/dice.h \
				  ../include/object/egg.h ../include/object/object.h \
				  ../include/object/text.h
RAYLIB_LIB_PATH = /usr/local/lib
LDFLAGS        += -L$(RAYLIB_LIB_PATH)
LDLIBS          = -lraylib -lGL -lm -lpthread -ldl -lrt
LDLIBS         += -lX11
PLATFORM        = PLATFORM_DESKTOP
OPTIMISE        = -flto -g0 -O3
COPT            = $(OPTIMISE) $(INCLUDES)
CFLAGS          = -Wall -std=c2x -Wno-missing-braces -Wunused-result $(COPT)
DEFINES         = -D_DEFAULT_SOURCE -D$(PLATFORM)
CPPFLAGS       += $(DEFINES)
SOURCES         = sill.c init.c say.c main_loop.c load_resources.c \
                  object/collision.c object/dice.c object/egg.c \
				  object/object.c object/text.c
OBJDIR          = ../obj
OBJECTS         = $(OBJDIR)/sill.o $(OBJDIR)/init.o $(OBJDIR)/say.o \
                  $(OBJDIR)/main_loop.o $(OBJDIR)/load_resources.o \
				  $(OBJDIR)/object.o $(OBJDIR)/collision.o \
				  $(OBJDIR)/dice.o $(OBJDIR)/egg.o $(OBJDIR)/text.o

.PHONY: all clean

all: $(APPLICATION)

clean:
	-rm -f $(APPLICATION) $(OBJECTS)

$(APPLICATION): $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(OBJDIR)/%.o: %.c $(INC_DEP)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: object/%.c $(INC_DEP)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

