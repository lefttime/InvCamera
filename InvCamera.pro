######################################################################
# Automatically generated by qmake (2.01a) ?? ?? 6 14:07:39 2013
######################################################################

TEMPLATE = app

QT += Qt3D

# Input
HEADERS += AbstractTextureImage.hpp \
           GenericTextureImage.hpp \
           InvBoudingAxis.hpp \
           InvExtrusionSceneNode.hpp \
           InvShapeSceneNode.hpp \
           InvSphereSceneNode.hpp \
           Util.hpp
SOURCES += AbstractTextureImage.cpp \
           GenericTextureImage.cpp \
           InvBoudingAxis.cpp \
           InvExtrusionSceneNode.cpp \
           InvShapeSceneNode.cpp \
           InvSphereSceneNode.cpp \
           main.cpp \
           Util.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/D:/Qt/4.8.4/lib/ -lQt3D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/D:/Qt/4.8.4/lib/ -lQt3Dd
else:unix: LIBS += -L$$PWD/D:/Qt/4.8.4/lib/ -lQt3D

INCLUDEPATH += $$PWD/D:/Qt/4.8.4/include/Qt3D
DEPENDPATH += $$PWD/D:/Qt/4.8.4/include/Qt3D
