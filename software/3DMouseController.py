import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui
from PySide6.QtWidgets import QDialog, QMessageBox
from PySide6.QtCore import QObject, SIGNAL, SLOT
from PySide6.QtSerialPort import QSerialPortInfo, QSerialPort
from PySide6.QtCore import QIODevice, QByteArray
from ui_dialog import Ui_Dialog


PRODUCT_ID = 32822
VENDOR_ID = 9025

class ControllerDialog(QDialog):


    ui = None
    port = None
    portSerial = None
    
    def __init__(self, parent=None):
        super(ControllerDialog, self).__init__(parent)
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.serialPortSetup()
        self.connectUi(self.ui)

    def serialPortSetup(self):
        self.ui.serialPorts.clear()
        ports = QSerialPortInfo.availablePorts()
        found = False
        multiple = False;
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
                if ( found == False ):
                    self.serialPortConnect()
                    found = True
                else:
                    print("Already a 3D Mouse found, skipping")
                    multiple = True
        if ( multiple == True ):
            QMessageBox().warning(self, "Multiple 3D Mices", "Already connected to a 3D Mouse")
        if ( found == False ):
            QMessageBox().warning(self, "No 3D Mouse", "No 3D Mouse Detected, Please go in Settings and click on Refresh")
                
    def serialPortConnect(self):
        print("Trying to connect to the serialPort")
        if ( self.port == None ):
            print("No 3DMouse Detected")
            return
        self.portSerial = QSerialPort(self.port)
        self.portSerial.setBaudRate(QSerialPort.Baud9600)
        self.portSerial.setDataBits(QSerialPort.Data8)
        self.portSerial.setParity(QSerialPort.NoParity)
        self.portSerial.setStopBits(QSerialPort.OneStop)
        self.portSerial.setFlowControl(QSerialPort.NoFlowControl)
        self.portSerial.readyRead.connect(self.serialPortRead)
        ret = self.portSerial.open(QIODevice.ReadWrite)
        if ( ret == False ):
            print("Error opening serial port")
            print(self.portSerial.error())
            return
        else:
            print("Connected to 3DMouse")
            self.portSerial.setDataTerminalReady(True)
            self.portSerial.setRequestToSend(True)
            self.serialPortSend('H')
            QMessageBox().information(self, "Connected to 3D Mouse", "3D Mouse Detected, Connected")
        
    def serialPortRead(self):
        print("serialPortRead")
        ret =  self.portSerial.readAll()
        print(ret)
        while ( self.portSerial.waitForReadyRead(100)):
            ret += self.portSerial.readAll()
        self.ui.serialOutput.appendPlainText(ret.data().decode())
        
    def serialPortSend(self, value):
        print("serialPortSend")
        print(value)
        ret = self.portSerial.putChar(value)
        print("ret = ")
        print(ret)
        self.ui.serialOutput.appendPlainText("Send = ")
        self.ui.serialOutput.appendPlainText(value)                                
        

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
        ui.serialSend.clicked.connect(self.onSendButton)
        ui.refresh.clicked.connect(self.onRefresh)

    def mouseMode(self):
        print("mouseMode")
        self.serialPortSend('m')

    def freeCADTranslateMode(self):
        print("freeCADTranslateMode")
        self.serialPortSend('f')

    def freeCADRotateMode(self):
        print("freeCADRotateMode")
        self.serialPortSend('F')

    def chopchop3DSlicerTranslateMode(self):
        print("chopchop3DSlicerTranslateMode")
        self.serialPortSend('d')

    def chopchop3DSlicerRotateMode(self):
        print("chopchop3DSlicerRotate")
        self.serialPortSend('D')

    def blenderTranslateMode(self):
        print("blenderTranslateMode")
        self.serialPortSend('b')

    def blenderRotateMode(self):
        print("blenderRotateMode")
        self.serialPortSend('B')

    def fusion360TranslateMode(self):
        print("fusion360TranslateMode")
        self.serialPortSend('u')

    def fusion360RotateMode(self):
        print("fusion360RotateMode")
        self.serialPortSend('U')

    def zBrushTranslateMode(self):
        print("zBrushTranslateMode")
        self.serialPortSend('z')

    def zBrushScaleMode(self):
        print("zBrushScaleMode")
        self.serialPortSend('x')

    def zBrushRotateMode(self):
        print("zBrushRotateMode")
        self.serialPortSend('Z')

    def onSendButton(self):
        print("onSendButton")
        ret = self.ui.serialInput.text();
        print(ret);
        self.serialPortSend(ret[0]);

    def onRefresh(self):
        print("onRefresh")
        self.serialPortSetup()
        

        

if __name__ == "__main__":
    app = QtWidgets.QApplication([])


    dialog = ControllerDialog()   
    dialog.show()

    sys.exit(app.exec())
