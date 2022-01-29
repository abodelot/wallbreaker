TARGET  := wallbreaker
SRCDIR  := src
SRC     := $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJDIR  := obj
OBJ     := $(SRC:%.cpp=$(OBJDIR)/%.o)
DEP     := $(SRC:%.cpp=$(OBJDIR)/%.d)

CC      := g++
CFLAGS  := -MMD -MP -I$(SRCDIR) -std=c++11 -pedantic -O2
WFLAGS  := -Wall -Wextra -Wold-style-cast -Wshadow -Wwrite-strings
LDFLAGS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

C_GREEN  := \033[1;32m
C_YELLOW := \033[1;33m
C_NONE   := \033[0m

$(TARGET): $(OBJ)
	@echo "$(C_GREEN)linking$(C_NONE) $@"
	@$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	@echo "$(C_GREEN)compiling\033[0m $<"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

-include $(DEP)

clean:
	@echo "$(C_YELLOW)removing$(C_NONE) $(OBJDIR)/"
	-@rm -r $(OBJDIR)

mrproper: clean
	@echo "$(C_YELLOW)removing$(C_NONE) $(TARGET)"
	-@rm $(TARGET)

all: mrproper $(TARGET)

# Linter commands
lint:
	clang-format $(SRC) --dry-run --Werror

format:
	clang-format $(SRC) -i --Werror

# Extract version number from latest tag (v0.1 => 0.1)
APP_VERSION=`git describe --tags --abbrev=0 | cut -c2-`
# Example: Linux_x86_64
APP_ARCH=`uname -s`_`uname -m`

TARBALL_NAME=$(TARGET)_$(APP_VERSION)_$(APP_ARCH).tar.gz
tarball: $(TARGET)
	@tar -cvzf $(TARBALL_NAME) --directory .. \
		wallbreaker/LICENSE \
		wallbreaker/README.md \
		wallbreaker/resources \
		wallbreaker/$(TARGET)
	@echo "$(C_GREEN)tarball$(C_NONE) $(TARBALL_NAME)"
