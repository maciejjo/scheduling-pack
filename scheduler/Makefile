TARGET   = scheduler

CC       = gcc
CXXFLAGS = -Wall -Wextra -pedantic -O2 -std=gnu99 -g

LINKER   = gcc -o
LFLAGS   = -Wall -Wextra -pedantic -O2 -std=gnu99 -g
SRCDIR   = src
OBJDIR   = obj

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Zakończono linkowanie!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CXXFLAGS) -c $< -o $@
	@echo "Skompilowano "$<" pomyślnie!"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Wyczyszczono!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Usunięto plik wykonywalny!"

