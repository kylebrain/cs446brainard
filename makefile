EXE = sim2

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS += -Iinc -g -pthread

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	g++ $(CPPFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(OBJ_DIR) $(EXE)