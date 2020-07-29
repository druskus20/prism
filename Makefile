CC ?= gcc
LD  = $(CC)

CFLAGS		=	-Wall -Wextra -pedantic -O2
CPPFLAGS	=	-MD -MP -D_POSIX_C_SOURCE=200809L
DEPENDS		=	xcb xcb-ewmh xcb-icccm xcb-randr xcb-util cairo
LDFLAGS		=	`pkg-config --cflags --libs $(DEPENDS)`

PREFIX		= 	/usr/local
BINPREFIX	= 	$(PREFIX)/bin
MANPREFIX	= 	$(PREFIX)/share/man

SRCPREFIX	=	src
BUILDPREFIX = 	build

TARGET	=	prism
SRC		:=	$(wildcard $(SRCPREFIX)/*.c)
SUBDIRS :=  $(wildcard $(SRCPREFIX)/*/*.c)
OBJ		:=	$(subst $(SRCPREFIX),$(BUILDPREFIX),$(SRC:.c=.o))
OBJ		+=	$(subst $(SRCPREFIX),$(BUILDPREFIX),$(SUBDIRS:.c=.o))
DEPS	=	$(OBJ:.o=.d)
VPATH	=	$(SRCPREFIX)
MKDIR   =   mkdir

.PHONY: all clean install uninstall

all: prepare $(TARGET)

$(TARGET): $(OBJ)
	$(LD) -o $(BUILDPREFIX)/$@ $(OBJ) $(LDFLAGS)

$(BUILDPREFIX)/%.o: $(SRCPREFIX)/%.c
	$(CC) -o $@ $(CFLAGS) -c $<

prepare:
	@[ -d $(BUILDPREFIX) ] || mkdir -p $(BUILDPREFIX)
	@[ -d $(BUILDPREFIX)/wm ] || mkdir -p $(BUILDPREFIX)/wm
	@[ -d $(BUILDPREFIX)/xcb ] || mkdir -p $(BUILDPREFIX)/xcb
	@[ -d $(BUILDPREFIX)/ipc ] || mkdir -p $(BUILDPREFIX)/ipc
	@[ -d $(BUILDPREFIX)/util ] || mkdir -p $(BUILDPREFIX)/util

clean:
	$(RM) -r $(BUILDPREFIX)

install: all
	install -Dm755 "$(BUILDPREFIX)/$(TARGET)" "$(DESTDIR)$(BINPREFIX)/$(TARGET)"
#	install -Dm644 "$(SRCPREFIX)/man/custard.man" "$(DESTDIR)$(MANPREFIX)/man1/custard.1"

uninstall:
	$(RM) "$(DESTDIR)$(BINPREFIX)/$(TARGET)"
