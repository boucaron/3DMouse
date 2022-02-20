import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui
from PySide6.QtWidgets import QDialog
from PySide6.QtCore import QObject, SIGNAL, SLOT
from PySide6.QtSerialPort import QSerialPortInfo
from ui_dialog import Ui_Dialog


PRODUCT_ID = 32822
VENDOR_ID = 9025

class ControllerDialog(QDialog):

    port = None
    ui = None
    
    def __init__(self, parent=None):
        super(ControllerDialog, self).__init__(parent)
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.serialPortSetup()
        self.connectUi(self.ui)

    def serialPortSetup(self):
        self.ui.serialPorts.clear()
        ports = QSerialPortInfo.availablePorts()
        for port in ports:
            print(port.productIdentifier())
            print(port.vendorIdentifier())
            print(port.portName())
            print(port.serialNumber())
            print(port.manufacturer())
            if ( port.productIdentifier() == PRODUCT_ID and port.vendorIdentifier() == VENDOR_ID ):
                print("Found 3DMouse on Port <<")
                print(port.portName())
                print(">>")
                self.port = port
                self.ui.serialPorts.addItem(self.port.portName(), self.port)
                
        

    def connectUi(self, ui):
        ui.mouseMode.clicked.connect(self.mouseMode)
        ui.FreeCADTranslateMode.clicked.connect(self.freeCADTranslateMode)
        ui.FreeCADRotateMode.clicked.connect(self.freeCADRotateMode)
        ui.ChopChop3DSlicerTranslateMode.clicked.connect(self.chopchop3DSlicerTranslateMode)
        ui.ChopChop3DSlicerRotateMode.clicked.connect(self.chopchop3DSlicerRotateMode)
        ui.BlenderTranslateMode.clicked.connect(self.blenderTranslateMode)
        ui.BlenderRotateMode.clicked.connect(self.blenderRotateMode)
        ui.Fusion360TranslateMode.clicked.connect(self.fusion360TranslateMode)
        ui.Fusion360RotateMode.clicked.connect(self.fusion360RotateMode)
        ui.zBrushTranslateMode.clicked.connect(self.zBrushTranslateMode)
        ui.zBrushScaleMode.clicked.connect(self.zBrushScaleMode)
        ui.zBrushRotateMode.clicked.connect(self.zBrushRotateMode)

    def mouseMode(self):
        print("mouseMode")

    def freeCADTranslateMode(self):
        print("freeCADTranslateMode")

    def freeCADRotateMode(self):
        print("freeCADRotateMode")

    def chopchop3DSlicerTranslateMode(self):
        print("chopchop3DSlicerTranslateMode")

    def chopchop3DSlicerRotateMode(self):
        print("chopchop3DSlicerRotate")

    def blenderTranslateMode(self):
        print("blenderTranslateMode")

    def blenderRotateMode(self):
        print("blenderRotateMode")

    def fusion360TranslateMode(self):
        print("fusion360TranslateMode")

    def fusion360RotateMode(self):
        print("fusion360RotateMode")

    def zBrushTranslateMode(self):
        print("zBrushTranslateMode")

    def zBrushScaleMode(self):
        print("zBrushScaleMode")

    def zBrushRotateMode(self):
        print("zBrushRotateMode")


        

if __name__ == "__main__":
    app = QtWidgets.QApplication([])


    dialog = ControllerDialog()   
    dialog.show()

    sys.exit(app.exec())
