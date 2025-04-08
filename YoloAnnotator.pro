QT += widgets
CONFIG += c++17 windows release

# 可选：禁用过时 API
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += main.cpp

# 若采用单文件模式，不需额外头文件；否则删除下面 HEADERS 行
# 这里采用单文件模式，将所有代码均放在 main.cpp 中
