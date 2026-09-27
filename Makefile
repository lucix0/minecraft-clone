BGFX_SRC = third_party/bgfx
SHADERC = $(BGFX_SRC)/shaderc

SRC_DIR    = shaders/src
BIN_DIR    = shaders/bin
VARYING    = $(SRC_DIR)/varying.def.sc
INCLUDES   = $(wildcard $(SRC_DIR)/*.sh)

VS_SRC     = $(wildcard $(SRC_DIR)/vs_*.sc)
FS_SRC     = $(wildcard $(SRC_DIR)/fs_*.sc)
VS_BIN     = $(patsubst $(SRC_DIR)/%.sc,$(BIN_DIR)/%.bin,$(VS_SRC))
FS_BIN     = $(patsubst $(SRC_DIR)/%.sc,$(BIN_DIR)/%.bin,$(FS_SRC))

SHADERC_FLAGS = --platform osx --profile metal \
                --varyingdef $(VARYING) \
                -i $(BGFX_SRC) -i $(SRC_DIR)

.PHONY: all clean
all: $(VS_BIN) $(FS_BIN)

$(BIN_DIR)/vs_%.bin: $(SRC_DIR)/vs_%.sc $(VARYING) $(INCLUDES) | $(BIN_DIR)
	$(SHADERC) -f $< -o $@ --type vertex $(SHADERC_FLAGS)

$(BIN_DIR)/fs_%.bin: $(SRC_DIR)/fs_%.sc $(VARYING) $(INCLUDES) | $(BIN_DIR)
	$(SHADERC) -f $< -o $@ --type fragment $(SHADERC_FLAGS)

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -f $(BIN_DIR)/*.bin