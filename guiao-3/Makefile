CC = gcc
CFLAGS = -O2 -Wall
TXT_DIR = saida
CSV_DIR = saida
OBJ_DIR = obj
LIBS_DIR = libs
SRC_DIR = src

PROGRAM = guiao-3
TESTS = testsQ
EXPECTED = expectedF

all: guiao-3 testsQ expectedF


$(PROGRAM): $(OBJ_DIR)/users.o $(OBJ_DIR)/repos.o $(OBJ_DIR)/commits.o $(OBJ_DIR)/menu.o $(OBJ_DIR)/valid.o $(OBJ_DIR)/queries.o $(OBJ_DIR)/guiao.o 
	$(CC) -I $(LIBS_DIR) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/users.o: $(SRC_DIR)/users.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/repos.o: $(SRC_DIR)/repos.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^

$(OBJ_DIR)/commits.o: $(SRC_DIR)/commits.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^

$(OBJ_DIR)/menu.o: $(SRC_DIR)/menu.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^

$(OBJ_DIR)/valid.o: $(SRC_DIR)/valid.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^

$(OBJ_DIR)/queries.o: $(SRC_DIR)/queries.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^

$(OBJ_DIR)/guiao.o: $(SRC_DIR)/guiao.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^



$(EXPECTED): $(OBJ_DIR)/users.o $(OBJ_DIR)/repos.o $(OBJ_DIR)/commits.o $(OBJ_DIR)/menu.o $(OBJ_DIR)/valid.o $(OBJ_DIR)/queries.o $(OBJ_DIR)/testQuery.o $(OBJ_DIR)/expected_files.o 
	$(CC) -I $(LIBS_DIR) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/expected_files.o: $(SRC_DIR)/expected_files.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^


$(TESTS):  $(OBJ_DIR)/users.o $(OBJ_DIR)/repos.o $(OBJ_DIR)/commits.o $(OBJ_DIR)/menu.o $(OBJ_DIR)/valid.o $(OBJ_DIR)/queries.o $(OBJ_DIR)/testQuery.o $(OBJ_DIR)/tests.o
	$(CC) -I $(LIBS_DIR) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/testQuery.o: $(SRC_DIR)/testQuery.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^

$(OBJ_DIR)/tests.o: $(SRC_DIR)/tests.c
	$(CC) -c -I $(LIBS_DIR) $(CFLAGS) -o  $@ $^


clean:
	@rm -rf $(OBJ_DIR)/*.o 
	@rm $(PROGRAM)
	@rm $(TESTS)
	@rm $(EXPECTED)
	@rm -rf $(CSV_DIR)/*.csv
	@rm -rf $(TXT_DIR)/*.txt