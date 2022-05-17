QT       += core gui opengl
LIBS     += -lopengl32 #-lglu32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport #openglwidgets

CONFIG += c++17



prj_dir = $${_PRO_FILE_PWD_}
bld_dir = $${prj_dir}/../bld
src_dir = $${prj_dir}/src
gui_dir = $${prj_dir}
cfg_dir = $${prj_dir}/../exec/data
res_dir = $${prj_dir}/res

INCLUDEPATH += $${prj_dir}/lib
INCLUDEPATH += $${prj_dir}/src
INCLUDEPATH += $${src_dir}/rocket
INCLUDEPATH += $${src_dir}/environment
INCLUDEPATH += $${src_dir}/utils
INCLUDEPATH += $${src_dir}/widgets

# boost library
INCLUDEPATH += C:/boost/boost_mingw/include/boost-1_78
LIBS += -LC:/boost/boost_mingw/lib

MOC_DIR     = $${bld_dir}/moc
OBJECTS_DIR = $${bld_dir}/obj
RCC_DIR     = $${bld_dir}/rcc
UI_DIR      = $${bld_dir}/gui
DESTDIR     = $${prj_dir}/../exec


win32 {
    SOURCES     += $$system("where /r .\ *.cpp* 2> nul")
    HEADERS     += $$system("where /r .\ *.h* 2> nul")
    FORMS       += $$system("where /r .\ *.ui 2> nul")
} else {
    SOURCES     += $$system("find '$${src_dir}/' -name '*.cpp*' 2>/dev/null")
    HEADERS     += $$system("find '$${src_dir}/' -name '*.h*' 2>/dev/null")
    FORMS       += $$system("find '$${gui_dir}/' -name '*.ui' 2>/dev/null")
}

RESOURCES += $${res_dir}/styles/dark/style.qrc $${res_dir}/resource.qrc
#RESOURCES += $${res_dir}/styles/light/style.qrc $${res_dir}/resource.qrc



