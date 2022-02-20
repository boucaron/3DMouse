# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'dialog.ui'
##
## Created by: Qt User Interface Compiler version 6.2.3
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QAbstractButton, QApplication, QComboBox, QDialog,
    QDialogButtonBox, QFormLayout, QGridLayout, QLabel,
    QLineEdit, QPlainTextEdit, QPushButton, QSizePolicy,
    QTabWidget, QVBoxLayout, QWidget)

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        if not Dialog.objectName():
            Dialog.setObjectName(u"Dialog")
        Dialog.resize(394, 414)
        self.verticalLayout = QVBoxLayout(Dialog)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.tab_3 = QTabWidget(Dialog)
        self.tab_3.setObjectName(u"tab_3")
        self.tab = QWidget()
        self.tab.setObjectName(u"tab")
        self.verticalLayout_2 = QVBoxLayout(self.tab)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.gridLayout = QGridLayout()
        self.gridLayout.setObjectName(u"gridLayout")
        self.BlenderTranslateMode = QPushButton(self.tab)
        self.BlenderTranslateMode.setObjectName(u"BlenderTranslateMode")
        sizePolicy = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.BlenderTranslateMode.sizePolicy().hasHeightForWidth())
        self.BlenderTranslateMode.setSizePolicy(sizePolicy)
        icon = QIcon()
        icon.addFile(u"blender_icon_64x64.png", QSize(), QIcon.Normal, QIcon.Off)
        self.BlenderTranslateMode.setIcon(icon)
        self.BlenderTranslateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.BlenderTranslateMode, 2, 0, 1, 1)

        self.Fusion360TranslateMode = QPushButton(self.tab)
        self.Fusion360TranslateMode.setObjectName(u"Fusion360TranslateMode")
        sizePolicy.setHeightForWidth(self.Fusion360TranslateMode.sizePolicy().hasHeightForWidth())
        self.Fusion360TranslateMode.setSizePolicy(sizePolicy)
        icon1 = QIcon()
        icon1.addFile(u"icons8-autodesk-fusion-360.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.Fusion360TranslateMode.setIcon(icon1)
        self.Fusion360TranslateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.Fusion360TranslateMode, 4, 1, 1, 1)

        self.mouseMode = QPushButton(self.tab)
        self.mouseMode.setObjectName(u"mouseMode")
        sizePolicy.setHeightForWidth(self.mouseMode.sizePolicy().hasHeightForWidth())
        self.mouseMode.setSizePolicy(sizePolicy)

        self.gridLayout.addWidget(self.mouseMode, 0, 0, 1, 1)

        self.zBrushRotateMode = QPushButton(self.tab)
        self.zBrushRotateMode.setObjectName(u"zBrushRotateMode")
        sizePolicy.setHeightForWidth(self.zBrushRotateMode.sizePolicy().hasHeightForWidth())
        self.zBrushRotateMode.setSizePolicy(sizePolicy)
        icon2 = QIcon()
        icon2.addFile(u"zBrush.png", QSize(), QIcon.Normal, QIcon.Off)
        self.zBrushRotateMode.setIcon(icon2)
        self.zBrushRotateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.zBrushRotateMode, 5, 0, 1, 1)

        self.zBrushTranslateMode = QPushButton(self.tab)
        self.zBrushTranslateMode.setObjectName(u"zBrushTranslateMode")
        sizePolicy.setHeightForWidth(self.zBrushTranslateMode.sizePolicy().hasHeightForWidth())
        self.zBrushTranslateMode.setSizePolicy(sizePolicy)
        self.zBrushTranslateMode.setIcon(icon2)
        self.zBrushTranslateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.zBrushTranslateMode, 4, 0, 1, 1)

        self.ChopChop3DSlicerTranslateMode = QPushButton(self.tab)
        self.ChopChop3DSlicerTranslateMode.setObjectName(u"ChopChop3DSlicerTranslateMode")
        sizePolicy.setHeightForWidth(self.ChopChop3DSlicerTranslateMode.sizePolicy().hasHeightForWidth())
        self.ChopChop3DSlicerTranslateMode.setSizePolicy(sizePolicy)
        icon3 = QIcon()
        icon3.addFile(u"CC3D.png", QSize(), QIcon.Normal, QIcon.Off)
        self.ChopChop3DSlicerTranslateMode.setIcon(icon3)
        self.ChopChop3DSlicerTranslateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.ChopChop3DSlicerTranslateMode, 2, 1, 1, 1)

        self.FreeCADTranslateMode = QPushButton(self.tab)
        self.FreeCADTranslateMode.setObjectName(u"FreeCADTranslateMode")
        sizePolicy.setHeightForWidth(self.FreeCADTranslateMode.sizePolicy().hasHeightForWidth())
        self.FreeCADTranslateMode.setSizePolicy(sizePolicy)
        icon4 = QIcon()
        icon4.addFile(u"Freecad.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.FreeCADTranslateMode.setIcon(icon4)
        self.FreeCADTranslateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.FreeCADTranslateMode, 0, 1, 1, 1)

        self.Fusion360RotateMode = QPushButton(self.tab)
        self.Fusion360RotateMode.setObjectName(u"Fusion360RotateMode")
        sizePolicy.setHeightForWidth(self.Fusion360RotateMode.sizePolicy().hasHeightForWidth())
        self.Fusion360RotateMode.setSizePolicy(sizePolicy)
        self.Fusion360RotateMode.setIcon(icon1)
        self.Fusion360RotateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.Fusion360RotateMode, 5, 1, 1, 1)

        self.FreeCADRotateMode = QPushButton(self.tab)
        self.FreeCADRotateMode.setObjectName(u"FreeCADRotateMode")
        sizePolicy.setHeightForWidth(self.FreeCADRotateMode.sizePolicy().hasHeightForWidth())
        self.FreeCADRotateMode.setSizePolicy(sizePolicy)
        self.FreeCADRotateMode.setIcon(icon4)
        self.FreeCADRotateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.FreeCADRotateMode, 1, 1, 1, 1)

        self.BlenderRotateMode = QPushButton(self.tab)
        self.BlenderRotateMode.setObjectName(u"BlenderRotateMode")
        sizePolicy.setHeightForWidth(self.BlenderRotateMode.sizePolicy().hasHeightForWidth())
        self.BlenderRotateMode.setSizePolicy(sizePolicy)
        self.BlenderRotateMode.setIcon(icon)
        self.BlenderRotateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.BlenderRotateMode, 3, 0, 1, 1)

        self.ChopChop3DSlicerRotateMode = QPushButton(self.tab)
        self.ChopChop3DSlicerRotateMode.setObjectName(u"ChopChop3DSlicerRotateMode")
        sizePolicy.setHeightForWidth(self.ChopChop3DSlicerRotateMode.sizePolicy().hasHeightForWidth())
        self.ChopChop3DSlicerRotateMode.setSizePolicy(sizePolicy)
        self.ChopChop3DSlicerRotateMode.setIcon(icon3)
        self.ChopChop3DSlicerRotateMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.ChopChop3DSlicerRotateMode, 3, 1, 1, 1)

        self.zBrushScaleMode = QPushButton(self.tab)
        self.zBrushScaleMode.setObjectName(u"zBrushScaleMode")
        sizePolicy.setHeightForWidth(self.zBrushScaleMode.sizePolicy().hasHeightForWidth())
        self.zBrushScaleMode.setSizePolicy(sizePolicy)
        self.zBrushScaleMode.setIcon(icon2)
        self.zBrushScaleMode.setIconSize(QSize(32, 32))

        self.gridLayout.addWidget(self.zBrushScaleMode, 6, 0, 1, 1)


        self.verticalLayout_2.addLayout(self.gridLayout)

        self.tab_3.addTab(self.tab, "")
        self.tab_2 = QWidget()
        self.tab_2.setObjectName(u"tab_2")
        self.tab_3.addTab(self.tab_2, "")
        self.tab_5 = QWidget()
        self.tab_5.setObjectName(u"tab_5")
        self.formLayout_2 = QFormLayout(self.tab_5)
        self.formLayout_2.setObjectName(u"formLayout_2")
        self.serialPorts = QComboBox(self.tab_5)
        self.serialPorts.setObjectName(u"serialPorts")

        self.formLayout_2.setWidget(0, QFormLayout.FieldRole, self.serialPorts)

        self.label = QLabel(self.tab_5)
        self.label.setObjectName(u"label")

        self.formLayout_2.setWidget(0, QFormLayout.LabelRole, self.label)

        self.serialOutput = QPlainTextEdit(self.tab_5)
        self.serialOutput.setObjectName(u"serialOutput")

        self.formLayout_2.setWidget(1, QFormLayout.FieldRole, self.serialOutput)

        self.serialInput = QLineEdit(self.tab_5)
        self.serialInput.setObjectName(u"serialInput")

        self.formLayout_2.setWidget(2, QFormLayout.FieldRole, self.serialInput)

        self.serialSend = QPushButton(self.tab_5)
        self.serialSend.setObjectName(u"serialSend")

        self.formLayout_2.setWidget(2, QFormLayout.LabelRole, self.serialSend)

        self.tab_3.addTab(self.tab_5, "")
        self.tab_4 = QWidget()
        self.tab_4.setObjectName(u"tab_4")
        self.tab_3.addTab(self.tab_4, "")

        self.verticalLayout.addWidget(self.tab_3)

        self.buttonBox = QDialogButtonBox(Dialog)
        self.buttonBox.setObjectName(u"buttonBox")
        self.buttonBox.setOrientation(Qt.Horizontal)
        self.buttonBox.setStandardButtons(QDialogButtonBox.Apply|QDialogButtonBox.Close|QDialogButtonBox.Reset)

        self.verticalLayout.addWidget(self.buttonBox)


        self.retranslateUi(Dialog)
        self.buttonBox.accepted.connect(Dialog.accept)
        self.buttonBox.rejected.connect(Dialog.reject)

        self.tab_3.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(Dialog)
    # setupUi

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QCoreApplication.translate("Dialog", u"3DMouse Controller", None))
        self.BlenderTranslateMode.setText(QCoreApplication.translate("Dialog", u"Translate", None))
#if QT_CONFIG(shortcut)
        self.BlenderTranslateMode.setShortcut(QCoreApplication.translate("Dialog", u"B", None))
#endif // QT_CONFIG(shortcut)
        self.Fusion360TranslateMode.setText(QCoreApplication.translate("Dialog", u"Translate", None))
#if QT_CONFIG(shortcut)
        self.Fusion360TranslateMode.setShortcut(QCoreApplication.translate("Dialog", u"U", None))
#endif // QT_CONFIG(shortcut)
        self.mouseMode.setText(QCoreApplication.translate("Dialog", u"Mouse", None))
#if QT_CONFIG(shortcut)
        self.mouseMode.setShortcut(QCoreApplication.translate("Dialog", u"M", None))
#endif // QT_CONFIG(shortcut)
        self.zBrushRotateMode.setText(QCoreApplication.translate("Dialog", u"Rotate", None))
#if QT_CONFIG(shortcut)
        self.zBrushRotateMode.setShortcut(QCoreApplication.translate("Dialog", u"Shift+Z", None))
#endif // QT_CONFIG(shortcut)
        self.zBrushTranslateMode.setText(QCoreApplication.translate("Dialog", u"Translate", None))
#if QT_CONFIG(shortcut)
        self.zBrushTranslateMode.setShortcut(QCoreApplication.translate("Dialog", u"Z", None))
#endif // QT_CONFIG(shortcut)
        self.ChopChop3DSlicerTranslateMode.setText(QCoreApplication.translate("Dialog", u"Translate", None))
#if QT_CONFIG(shortcut)
        self.ChopChop3DSlicerTranslateMode.setShortcut(QCoreApplication.translate("Dialog", u"D", None))
#endif // QT_CONFIG(shortcut)
        self.FreeCADTranslateMode.setText(QCoreApplication.translate("Dialog", u"Translate", None))
#if QT_CONFIG(shortcut)
        self.FreeCADTranslateMode.setShortcut(QCoreApplication.translate("Dialog", u"F", None))
#endif // QT_CONFIG(shortcut)
        self.Fusion360RotateMode.setText(QCoreApplication.translate("Dialog", u"Rotate", None))
#if QT_CONFIG(shortcut)
        self.Fusion360RotateMode.setShortcut(QCoreApplication.translate("Dialog", u"Shift+U", None))
#endif // QT_CONFIG(shortcut)
        self.FreeCADRotateMode.setText(QCoreApplication.translate("Dialog", u"Rotate", None))
#if QT_CONFIG(shortcut)
        self.FreeCADRotateMode.setShortcut(QCoreApplication.translate("Dialog", u"Shift+F", None))
#endif // QT_CONFIG(shortcut)
        self.BlenderRotateMode.setText(QCoreApplication.translate("Dialog", u"Rotate", None))
#if QT_CONFIG(shortcut)
        self.BlenderRotateMode.setShortcut(QCoreApplication.translate("Dialog", u"Shift+B", None))
#endif // QT_CONFIG(shortcut)
        self.ChopChop3DSlicerRotateMode.setText(QCoreApplication.translate("Dialog", u"Rotate", None))
#if QT_CONFIG(shortcut)
        self.ChopChop3DSlicerRotateMode.setShortcut(QCoreApplication.translate("Dialog", u"Shift+D", None))
#endif // QT_CONFIG(shortcut)
        self.zBrushScaleMode.setText(QCoreApplication.translate("Dialog", u"Scale", None))
#if QT_CONFIG(shortcut)
        self.zBrushScaleMode.setShortcut(QCoreApplication.translate("Dialog", u"Alt+Z", None))
#endif // QT_CONFIG(shortcut)
        self.tab_3.setTabText(self.tab_3.indexOf(self.tab), QCoreApplication.translate("Dialog", u"Mode", None))
        self.tab_3.setTabText(self.tab_3.indexOf(self.tab_2), QCoreApplication.translate("Dialog", u"Custom", None))
        self.label.setText(QCoreApplication.translate("Dialog", u"Serial Port:", None))
        self.serialSend.setText(QCoreApplication.translate("Dialog", u"Send", None))
        self.tab_3.setTabText(self.tab_3.indexOf(self.tab_5), QCoreApplication.translate("Dialog", u"Settings", None))
        self.tab_3.setTabText(self.tab_3.indexOf(self.tab_4), QCoreApplication.translate("Dialog", u"About", None))
    # retranslateUi

