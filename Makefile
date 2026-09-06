CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra
LIBS     := -lpthread -ldl -lm

VERSION  := $(shell cat version.txt 2>/dev/null || echo "1.0.0")
CXXFLAGS += -DPROJECT_VERSION=\"$(VERSION)\"

SRC_DIR  := src
BUILD_DIR:= build
SRCS     := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS     := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET   := $(BUILD_DIR)/vmlite

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo "Build successful: $(TARGET) (v$(VERSION))"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean