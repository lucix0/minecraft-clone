BGFX_SRC = third_party/bgfx

SHADERC = $(BGFX_SRC)/shaderc

SHADER_DIR = shaders

VERTEX_SHADER = $(SHADER_DIR)/src/vs_triangle.sc
FRAGMENT_SHADER = $(SHADER_DIR)/src/fs_triangle.sc

VERTEX_BIN = $(SHADER_DIR)/bin/vs_triangle.bin
FRAGMENT_BIN = $(SHADER_DIR)/bin/fs_triangle.bin

all: $(VERTEX_BIN) $(FRAGMENT_BIN)
	mkdir -p shaders/bin

$(VERTEX_BIN): $(VERTEX_SHADER)
	mkdir -p shaders/bin
	$(SHADERC) -f $< -o $@ \
		--type vertex \
		--platform osx \
		--profile metal \
		--varyingdef $(SHADER_DIR)/src/varying.def.sc \
		-i $(BGFX_SRC)

$(FRAGMENT_BIN): $(FRAGMENT_SHADER)
	mkdir -p shaders/bin
	$(SHADERC) -f $< -o $@ \
		--type fragment \
		--platform osx \
		--profile metal \
		--varyingdef $(SHADER_DIR)/src/varying.def.sc \
		-i $(BGFX_SRC)

clean:
	rm -f $(VERTEX_BIN) $(FRAGMENT_BIN)