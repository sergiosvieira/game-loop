TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += "/usr/local/opt/sfml/include"
LIBS += -L"/usr/local/opt/sfml/lib"
LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
SOURCES += \
        main.cpp
