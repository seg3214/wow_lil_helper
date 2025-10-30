NAME_EXE=wlh
NAME_DLL=wlh.dll
NAME_SRCDIR=src
NAME_BUILDDIR=build
NAME_OBJ=obj
NAME_BIN=bin
NAME_HEADER_TO_PRECOMPILE=$(NAME_SRCDIR)/shared/main.h
NAME_STATEFILE=data/wowdump/state
#relative to bin folder
DX9_DIR=../../libs/DXSDK_June2010

wnos=	\
			-Wno-incompatible-pointer-types\
			-Wno-int-conversion\
			-Wno-dangling-else\
			-Wno-switch\
			-Wno-absolute-value
CFLAGS_SHARED=  -m32 -march=native -mtune=native -Wpadded -fasm-blocks $(wnos)
LDFLAGS_SHARED= -m32 -march=native -mtune=native -Wpadded

WIN32_CFLAGS= -std=c99 -g0 --target=i686-w64-windows-gnu $(CFLAGS_SHARED) -I"$(DX9_DIR)/Include"
WIN32_LDFLAGS= -shared -g0 --target=i686-w64-windows-gnu $(LDFLAGS_SHARED) -L"$(DX9_DIR)/Lib" -ld3d9 -ld3dx9 -lwinmm

LINUX32_CFLAGS= -std=gnu99 -g --target=i686-suse-linux $(CFLAGS_SHARED)
LINUX32_LDFLAGS= -g --target=i686-suse-linux -lm $(LDFLAGS_SHARED)

COM=clang
COM_BEAR=bear --append -- clang
COM=$(COM_BEAR)
#///////////////

OBJDIR=$(NAME_BUILDDIR)/$(NAME_OBJ)
BINDIR=$(NAME_BUILDDIR)/$(NAME_BIN)
DIRS=$(NAME_BUILDDIR) $(OBJDIR) $(BINDIR)
$(shell mkdir -p $(DIRS))

WIN32_SOURCES = $(shell find -L $(NAME_SRCDIR)/win32 -name "*.c" -not -name "*private*.c")
LINUX32_SOURCES = $(shell find -L $(NAME_SRCDIR)/linux32 -name "*.c" -not -name "*private*.c")
# headers that will trigger PCH to recompile. 
#WIN32_HEADERS = $(shell find -L $(NAME_SRCDIR) -name "*.h" -not -name "*private*.h" -not -path "*/linux32/*")
WIN32_HEADERS = $(shell find -L $(NAME_SRCDIR)/win32 -name "*.h" -not -name "*private*.h")
LINUX32_HEADERS = $(shell find -L $(NAME_SRCDIR)/linux32 -name "*.h" -not -name "*private*.h")

WIN32_OBJECTS:=$(patsubst %.c,%.o,$(WIN32_SOURCES))
LINUX32_OBJECTS:=$(patsubst %.c,%.o,$(LINUX32_SOURCES))

WIN32_OBJECTS:=$(patsubst $(NAME_SRCDIR)%,$(NAME_OBJ)%,$(WIN32_OBJECTS))
LINUX32_OBJECTS:=$(patsubst $(NAME_SRCDIR)%,$(NAME_OBJ)%,$(LINUX32_OBJECTS))

WIN32_OBJECTS:=$(addprefix $(NAME_BUILDDIR)/,$(WIN32_OBJECTS))
LINUX32_OBJECTS:=$(addprefix $(NAME_BUILDDIR)/,$(LINUX32_OBJECTS))

FOLDERS:=$(dir $(WIN32_OBJECTS) $(LINUX32_OBJECTS))
$(shell mkdir -p $(FOLDERS))

WIN32_DEPS:= $(WIN32_OBJECTS:.o=.d)
LINUX32_DEPS:= $(LINUX32_OBJECTS:.o=.d)

# $(info DIRS $(DIRS))
# $(info FOLDERS $(FOLDERS))
# $(info WIN32_HEADERS $(WIN32_HEADERS))
 # $(info WIN32_SOURCES $(WIN32_SOURCES))
# $(info LINUX32_SOURCES $(LINUX32_SOURCES))
 # $(info WIN32_OBJECTS $(WIN32_OBJECTS))
# $(info LINUX32_OBJECTS $(LINUX32_OBJECTS))

WIN32_PCH_FILE=$(OBJDIR)/winpch.pch
LINUX32_PCH_FILE=$(OBJDIR)/linpch.pch

win:$(BINDIR)/$(NAME_DLL) compile_commands copydll
	@echo WIN32-----------------------------------------------------
copydll:
	cp $(BINDIR)/$(NAME_DLL) bin/ && echo -n 1 > $(NAME_STATEFILE)
$(BINDIR)/$(NAME_DLL):$(WIN32_OBJECTS)
	@echo WIN32 LINKING--------------------------------------------------
	clang $(WIN32_OBJECTS) $(WIN32_LDFLAGS) -o $(BINDIR)/$(NAME_DLL)
$(WIN32_OBJECTS):$(OBJDIR)/%.o:$(NAME_SRCDIR)/%.c $(WIN32_PCH_FILE)
	@echo WIN32 OBJECTS--------------------------------------------------
#	$(COM) -include-pch $(WIN32_PCH_FILE) -c -MMD $< $(WIN32_CFLAGS) -o $@
	clang -include-pch $(WIN32_PCH_FILE) -c -MMD $< $(WIN32_CFLAGS) -o $@ -MJ $@.json
$(WIN32_PCH_FILE):$(WIN32_HEADERS)
	@echo WIN32 PCH FILE--------------------------------------------------
	clang -xc-header $(NAME_HEADER_TO_PRECOMPILE) -o $(WIN32_PCH_FILE) $(WIN32_CFLAGS)

lin:$(BINDIR)/$(NAME_EXE) compile_commands
	@echo LINUX32-----------------------------------------------------
$(BINDIR)/$(NAME_EXE):$(LINUX32_OBJECTS)
	@echo LINUX32 LINKING--------------------------------------------------
	clang $(LINUX32_OBJECTS) $(LINUX32_LDFLAGS) -o $(BINDIR)/$(NAME_EXE)
$(LINUX32_OBJECTS): $(OBJDIR)/%.o:$(NAME_SRCDIR)/%.c $(LINUX32_PCH_FILE)
	@echo LINUX32 OBJECTS-----------------------------------------------------
#	$(COM) -include-pch $(LINUX32_PCH_FILE) -c -MMD $< $(LINUX32_CFLAGS) -o $@
	clang -include-pch $(LINUX32_PCH_FILE) -c -MMD $< $(LINUX32_CFLAGS) -o $@ -MJ $@.json
$(LINUX32_PCH_FILE):$(LINUX32_HEADERS)
	@echo LINUX32 PCH FILE--------------------------------------------------
	clang -xc-header $(NAME_HEADER_TO_PRECOMPILE) -o $(LINUX32_PCH_FILE) $(LINUX32_CFLAGS)

compile_commands:
	@echo compile_commands.json-----------------------------------------------------
	@(echo "[";find $(OBJDIR) -type f -name "*.json" -exec cat {} \;; echo "]") > compile_commands.json
	@sed -i 's/$(NAME_SRCDIR)\/linux32\/shared/$(NAME_SRCDIR)\/shared/g' compile_commands.json
	@sed -i 's/$(NAME_SRCDIR)\/win32\/shared/$(NAME_SRCDIR)\/shared/g' compile_commands.json

ifneq ($(MAKECMDGOALS),clean)
ifeq ($(MAKECMDGOALS),lin)
$(info including lin32 deps......................)
-include $(LINUX32_DEPS)
endif
ifeq ($(MAKECMDGOALS),win)
$(info including win32 deps......................)
-include $(WIN32_DEPS)
endif
endif

clean:
	rm -r $(NAME_BUILDDIR)

.PHONY: clean prologue win lin copydll
