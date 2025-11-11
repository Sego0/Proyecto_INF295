CXX = g++
CXXFLAGS = -Wall -Wextra -g3 -std=c++17

# Carpeta de salida
OUT_DIR = output

# ==============================
# Ejecutable 1: testCall
# ==============================
testCall: stroke.cpp testCall.cpp
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) stroke.cpp testCall.cpp -o $(OUT_DIR)/testCall
	@echo "✅ Ejecutable creado: $(OUT_DIR)/testCall"

# ==============================
# Ejecutable 2: pruebas
# ==============================
pruebas: stroke.cpp pruebas.cpp
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) stroke.cpp pruebas.cpp -o $(OUT_DIR)/pruebas
	@echo "✅ Ejecutable creado: $(OUT_DIR)/pruebas"

# ==============================
# Limpiar archivos generados
# ==============================
clean:
	rm -f *.o $(OUT_DIR)/*
