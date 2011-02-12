

CONFIG(release, debug|release)
{
    OBJECTS_DIR = $${PWD}/bin/release
}

CONFIG(debug, debug|release)
{
    OBJECTS_DIR = $${PWD}/bin/debug
}

MOC_DIR = $${OBJECTS_DIR}
DESTDIR = $${OBJECTS_DIR}
UI_DIR = $${OBJECTS_DIR}
RCC_DIR = $${OBJECTS_DIR}
LIB_PATH = $${OBJECTS_DIR}


