TARGET = image_identifier

SRCS  = ./src/main.cpp ./src/dir/filetypes.cpp ./src/dir/dir_reader.cpp
HEADS = ./includes/main.h ./includes/dir/filetypes.h ./includes/dir/dir_reader.h
OBJS = $(SRCS:.cpp=.o)
DEPS = Makefile.depend

INCLUDES = -I./includes
CXXFLAGS = -O2 -Wall --std=c++17 $(INCLUDES)
LDFLAGS = -lm

all: $(TARGET)

$(TARGET): $(OBJS) $(HEADS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

run: all
	@./$(TARGET)

.PHONY: depend clean
depend:
	$(CXX) $(INCLUDES) -MM $(SRCS) > $(DEPS)
	@sed -i -E "s/^(.+?).o: ([^ ]+?)\1/\2\1.o: \2\1/g" $(DEPS)

clean:
	$(RM) $(OBJS) $(TARGET)

-include $(DEPS)
