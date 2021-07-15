TARGET = $(BLD_DIR)/nucleof446re.elf
SRC_DIR = ./src
BSP_DIR = ./bsp
HAL_DIR = ./hal
LNK_DIR = ./lnk
OBJ_DIR = ./obj
BLD_DIR = ./build
OBJS = $(OBJ_DIR)/startup.o \
	   $(OBJ_DIR)/syscalls.o \
	   $(OBJ_DIR)/main.o \
	   $(OBJ_DIR)/ds1307.o
LIBS = -lstm32f446xx

CC = arm-none-eabi-gcc
MACH = cortex-m4
CFLAGS = -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -I$(HAL_DIR) -O0
LDFLAGS = -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -L$(HAL_DIR) -T $(LNK_DIR)/lk_f446re.ld -Wl,-Map=$(BLD_DIR)/nucleof446re.map

$(TARGET) : $(OBJS)
	@mkdir -p $(BLD_DIR)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(BSP_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

-include $(OBJ_DIR)*.d

.PHONY : all
all: $(TARGET)

.PHONY : clean
clean:
	rm -r $(OBJ_DIR) $(BLD_DIR)

.PHONY : load
load:
	openocd -f board/st_nucleo_f4.cfg
