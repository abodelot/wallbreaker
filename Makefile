TARGET  := wallbreaker
SRCDIR  := src
SRC     := $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJDIR  := obj
OBJ     := $(SRC:%.cpp=$(OBJDIR)/%.o)

CC      := g++
CFLAGS  := -I$(SRCDIR) -std=c++11 -pedantic -Wall -Wextra -Wwrite-strings -O2
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

$(TARGET): $(OBJ)
	@echo "\033[1;33mlinking\033[0m $@"
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "\033[1;32mDone!\033[0m"

$(OBJDIR)/%.o: %.cpp
	@echo "\033[1;33mcompiling\033[0m $<"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "\033[1;33mremoving\033[0m $(OBJDIR)"
	-@rm -r $(OBJDIR)

mrproper: clean
	@echo "\033[1;33mremoving\033[0m $(TARGET)"
	-@rm $(TARGET)

all: mrproper $(TARGET)

APP_NAME=$(TARGET)
# Extract version number from latest tag (v0.1 => 0.1)
APP_VERSION=`git describe --tags --abbrev=0 | cut -c2-`
# Example: linux-64bits
APP_ARCH=`uname -s | tr '[:upper:]' '[:lower:]'`-`getconf LONG_BIT`bits

PACKAGE_NAME=$(APP_NAME)_$(APP_VERSION)_$(APP_ARCH).tar.gz
tarball: $(TARGET)
	@tar -cvzf $(PACKAGE_NAME) --directory .. \
		wallbreaker/COPYING \
		wallbreaker/README.md \
		wallbreaker/resources \
		wallbreaker/$(TARGET)
	@echo "\033[1;32mgenerated archive: $(PACKAGE_NAME)\033[0m"

