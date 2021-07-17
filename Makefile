TARGET1 = $(BLD_DIR)/nucleof446re.elf
TARGET2 = $(BLD_DIR)/nucleof446re_sh.elf
SRC_DIR = ./src
BSP_DIR = ./bsp
HAL_DIR = ./hal
LNK_DIR = ./lnk
OBJ_DIR = ./obj
BLD_DIR = ./build
OBJS1 = $(OBJ_DIR)/startup.o \
		$(OBJ_DIR)/syscalls.o \
		$(OBJ_DIR)/main.o \
		$(OBJ_DIR)/ds1307.o
OBJS2 = $(OBJ_DIR)/startup.o \
		$(OBJ_DIR)/main.o \
		$(OBJ_DIR)/ds1307.o
LIBS = -lstm32f446xx

CC = arm-none-eabi-gcc
MACH = cortex-m4
CFLAGS = -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -I$(HAL_DIR) -I$(BSP_DIR) -O0
LDFLAGS = -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -L$(HAL_DIR) -T$(LNK_DIR)/lk_f446re.ld -Wl,-Map=$(BLD_DIR)/nucleof446re.map
LDFLAGS_SH = -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=rdimon.specs -L$(HAL_DIR) -T$(LNK_DIR)/lk_f446re.ld -Wl,-Map=$(BLD_DIR)/nucleof446re.map

$(TARGET1) : $(OBJS1)
	@mkdir -p $(BLD_DIR)
	$(CC) $(LDFLAGS) $(OBJS1) -o $(TARGET1) $(LIBS)

$(TARGET2) : $(OBJS2)
	@mkdir -p $(BLD_DIR)
	$(CC) $(LDFLAGS_SH) $(OBJS2) -o $(TARGET2) $(LIBS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(BSP_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

-include $(OBJ_DIR)*.d

.PHONY : all
all: $(TARGET1)

.PHONY : semi
semi: $(TARGET2)

.PHONY : clean
clean:
	rm -r $(OBJ_DIR) $(BLD_DIR)

.PHONY : load
load:
	openocd -f board/st_nucleo_f4.cfg
