CXX := g++
MODE := DEBUG
DEBUGFLAGS := -O0 -ggdb
RELEASEFLAGS := -O3

CXXFLAGS := $($(MODE)FLAGS) -pedantic -pedantic-errors -ansi -std=c++98 -Wall -Wextra -Werror -I.
CXXSTDINCLUDES := -Y

SOURCES := fm.cpp

OBJECTS := $(SOURCES:.cpp=.o)
DIST_ARCHIVE_FILE := fm-$(shell date --rfc-3339=date)-git-$(shell git describe --always).zip

fm: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(@) $(^)

clean:
	rm -f $(OBJECTS) fm *~

strip: fm
	strip -s $(<)

doc-clean:
	rm -rf doc/html

doc: Doxyfile $(SOURCES)
	doxygen $(<)

dist: clean doc-clean fm strip doc
	git archive --format=zip --output=$(DIST_ARCHIVE_FILE) HEAD
	zip -r $(DIST_ARCHIVE_FILE) .git doc/

depend:
	makedepend $(CXXSTDINCLUDES) -- $(CXXFLAGS) -- $(SOURCES) 2&> /dev/null

all: strip

.PHONY: clean doc all

# DO NOT DELETE THIS LINE -- make depend depends on it.

fm.o: graph.hpp tr1_memory.hpp prim.hpp kruskal.hpp ford_moore.hpp
