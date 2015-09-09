CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/mppt

SRCEXT := cpp
SOURCES := $(wildcard src/*.$(SRCEXT))
OBJECTS := $(SOURCES:.$(SRCEXT)=.o)
CFLAGS := -g -Wall

LIB := 
INC := -I include

$(TARGET): $(OBJECTS)
	@echo "Linking $^ -> $(TARGET)"
	$(CC) $^ -o $(TARGET) $(LIB)

# Build all src files
$(BUILDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Building $< -> $@" 
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."
	@echo "$(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
