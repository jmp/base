CC=gcc
TARGET=bin/base
EXTLIBS=-lSDL2main -lSDL2 -lSDL2_mixer -lphysfs
CFLAGS=-m64 -O2 -std=c99 -pedantic -Wall -Werror -Wextra -Wno-unused
LDFLAGS=-m64 -lm $(EXTLIBS)
OBJECTS=$(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))

ifdef ComSpec
	TARGET := $(TARGET).exe
	CFLAGS := $(CFLAGS) -Iext/include -Lext/lib
	LDFLAGS := -mconsole -mwindows -lmingw32 $(LDFLAGS) -lwinmm -limm32 -lole32 -loleaut32 -lversion -static
	mkdir = mkdir $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
	rm = $(wordlist 2,65535,$(foreach FILE,$(subst /,\,$(1)),& del $(FILE) > nul 2>&1)) || (exit 0)
	rmdir = rmdir /s /q $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
	echo = echo $(1)
else
	mkdir = mkdir -p $(1)
	rm = rm $(1) > /dev/null 2>&1 || true
	rmdir = rm -r $(1) > /dev/null 2>&1 || true
	echo = echo "$(1)"
endif

$(TARGET): $(OBJECTS)
	@$(call mkdir,bin)
	@$(call echo,LINK $(OBJECTS))
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@$(call echo,STRIP $(TARGET))
	@strip -s $(TARGET)

obj/%.o: src/%.c
	@$(call mkdir,obj)
	@$(call echo,CC $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@$(call rmdir,obj)
	@$(call rm,$(TARGET))

.PHONY: clean
