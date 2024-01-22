CC = gcc

TXT_DIR = saida
CSV_DIR = saida
OBJ_DIR = obj
LIBS_DIR = libs
SRC_DIR = src

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

PROGRAM = guiao-2

.DEFAULT_GOAL = $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -I $(LIBS_DIR) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -I $(LIBS_DIR) -o $@ $^

clean:
	@rm -rf $(OBJ_DIR)/*.o 
	@rm $(PROGRAM)
	@rm -rf $(CSV_DIR)/*.csv
	@rm -rf $(TXT_DIR)/*.txt
