# Add more folders to ship with the application, here
folder_01.source = qml/qmlfbacapex4harmattan
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE1A1C75F

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

SOURCES += \
    src/kzfbacapex.cpp \
    src/kfilebrowser.cpp
HEADERS += \
    src/kzfbarominfo.h \
    src/kzfbacapex.h \
    src/kfileinfo.h \
    src/kfilebrowser.h

rsc_files.files = rsc/icon-m-toolbar-application.png \
rsc/icon-m-toolbar-directory.png \
rsc/icon-m-toolbar-home.png \
rsc/icon-m-toolbar-list.png \
rsc/icon-m-toolbar-settings.png \
qmlfbacapex_splash.jpg \
rsc/fbaicon.png \
rsc/whip.gif

doc_files.files = doc/zipname.fba

rsc_files.path = /opt/qmlfbacapex4harmattan/rsc
doc_files.path = /opt/qmlfbacapex4harmattan/doc
INSTALLS += rsc_files doc_files

unix:!symbian:!maemo5 {
    translation.files = i18n/qmlfbacapex.zh_CN.ts i18n/qmlfbacapex.zh_CN.qm
    translation.path = /opt/$$TARGET/i18n
    INSTALLS += translation
}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

