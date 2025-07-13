TARGET = bmpTOtxt
CC = g++
CXXFLAGS = -std=c++17

LOC_SRC = ./src/
LOC_OBJ = ./obj/

SRC = $(wildcard $(LOC_SRC)*.cpp)
OBJ = $(patsubst $(LOC_SRC)%.cpp, $(LOC_OBJ)%.o, $(SRC))

$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(LOC_OBJ)%.o: $(LOC_SRC)%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(LOC_OBJ)*.o