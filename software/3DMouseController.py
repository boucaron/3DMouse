# (c) 2022-3 Julien BOUCARON under MIT License

import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui
from PySide6.QtWidgets import QDialog, QMessageBox
from PySide6.QtCore import Qt, QObject, SIGNAL, SLOT
from PySide6.QtSerialPort import QSerialPortInfo, QSerialPort
from PySide6.QtCore import QIODevice, QByteArray, QTimer
from ui_dialog import Ui_Dialog

# Arduino Pro Micro SerialPort IDs
PRODUCT_ID = 32822
VENDOR_ID = 9025

# Speech Recognition
import pyaudio
import whisper
import numpy as np
import torch
# Speech Synthesis
import pyttsx3


class ControllerDialog(QDialog):


    ui = None
    ports = None
    port = None
    portSerial = None
    inputsMic = {}
    outputs = {}
    voices = {}
    
    def __init__(self, parent=None):
        super(ControllerDialog, self).__init__(parent)
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.serialPortSetup()
        self.connectUi(self.ui)

#List ports
#Try to detect the 3DMouse (if not show an error message)
# If so connect to the first one taken (send a warning if more than one)
    def serialPortSetup(self):
        self.ui.serialPorts.clear()
        self.ports = QSerialPortInfo.availablePorts()
        found = False
        multiple = False;
        for port in self.ports:
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
            QMessageBox().warning(self, "Multiple 3D Mices", "Already connected to a 3D Mouse, Please go in Settings, Select another Port and click on Connect")
        if ( found == False ):
            QMessageBox().warning(self, "No 3D Mouse", "No 3D Mouse Detected, Please go in Settings and click on Refresh")

#Connect to a specific port: index of the combobox
# See previous method + comment
    def serialPortConnectAtIndex(self, index):
        if ( self.port == None ):
            print("No 3DMouse Detected")
            return
        self.port = self.ports[ index ]
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
            self.serialPortSend('H', None)
            QMessageBox().information(self, "Connected to 3D Mouse", "3D Mouse Detected, Connected")
        
# Variant of the previous one
# Just called from @see serialPortSetup
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
            self.serialPortSend('H', None)
            QMessageBox().information(self, "Connected to 3D Mouse", "3D Mouse Detected, Connected")

# Read all from the serial port and put in the Settings/PlainTextEdit            
    def serialPortRead(self):
        print("serialPortRead")
        ret =  self.portSerial.readAll()
        print(ret)
        while ( self.portSerial.waitForReadyRead(100)):
            ret += self.portSerial.readAll()
        self.ui.serialOutput.appendPlainText(ret.data().decode())

# Send the value (char) to the 3D Mouse, optionally put a message to the user that autocloses        
    def serialPortSend(self, value, message):
        print("serialPortSend")
        print(value)
        ret = self.portSerial.putChar(value)
        print("ret = ")
        print(ret)
        self.ui.serialOutput.appendPlainText("Send = ")
        self.ui.serialOutput.appendPlainText(value)
        if ( message == None ):
            print("No message")
        else:
            qm = QMessageBox(self)
            qm.setWindowTitle("3D Mouse Controller")
            qm.setText(message)
            qm.setStandardButtons(QMessageBox.Ok)
            qm.setDefaultButton(QMessageBox.Ok)
            QTimer.singleShot(2000, lambda: qm.done(0)) # Autoclose trick
            qm.exec()
        
# Setup the ui and connect buttons
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
        ui.m3DBuilderTranslateMode.clicked.connect(self.m3DBuilderTranslateMode)
        ui.m3DBuilderRotateMode.clicked.connect(self.m3DBuilderRotateMode)
        ui.Paint3DTranslateMode.clicked.connect(self.Paint3DTranslateMode)
        ui.Paint3DRotateMode.clicked.connect(self.Paint3DRotateMode)
        ui.OpenSCADTranslateMode.clicked.connect(self.OpenSCADTranslateMode)
        ui.OpenSCADRotateMode.clicked.connect(self.OpenSCADRotateMode)
        ui.serialSend.clicked.connect(self.onSendButton)
        ui.refresh.clicked.connect(self.onRefresh)
        ui.connect.clicked.connect(self.onConnect)

        ui.scanMicros.clicked.connect(self.onScanMicros)
        ui.testMicro.clicked.connect(self.onTestMicro)
        ui.scanOutputs.clicked.connect(self.onScanOutputs)
        ui.speechRecognition.stateChanged.connect(self.onSpeechRecognition)
        ui.testRecognition.clicked.connect(self.onWhisper)

        ui.speechSynthesis.stateChanged.connect(self.onSpeechSynthesis)
        ui.scanVoices.clicked.connect(self.onScanVoices)
        ui.testSynthesis.clicked.connect(self.onTestVoice)

# Just callbacks from the buttons        
        
    def mouseMode(self):
        print("mouseMode")
        self.serialPortSend('m',"Basic Mouse Mode Enabled")

    def freeCADTranslateMode(self):
        print("freeCADTranslateMode")
        self.serialPortSend('f',"FreeCAD Translate Mode Enabled")

    def freeCADRotateMode(self):
        print("freeCADRotateMode")
        self.serialPortSend('F',"FreeCAD Rotate Mode Enabled")

    def chopchop3DSlicerTranslateMode(self):
        print("chopchop3DSlicerTranslateMode")
        self.serialPortSend('d',"ChopChop3D Slicer Translate Mode Enabled")

    def chopchop3DSlicerRotateMode(self):
        print("chopchop3DSlicerRotate")
        self.serialPortSend('D',"ChopChop3D Slicer Rotate Mode Enabled")

    def blenderTranslateMode(self):
        print("blenderTranslateMode")
        self.serialPortSend('b',"Blender Translate Mode Enabled")

    def blenderRotateMode(self):
        print("blenderRotateMode")
        self.serialPortSend('B',"Blender Rotate Mode Enabled")

    def fusion360TranslateMode(self):
        print("fusion360TranslateMode")
        self.serialPortSend('u',"Fusion360 Translate Mode Enabled")

    def fusion360RotateMode(self):
        print("fusion360RotateMode")
        self.serialPortSend('U',"Fusion360 Rotate Mode Enabled")

    def zBrushTranslateMode(self):
        print("zBrushTranslateMode")
        self.serialPortSend('z',"zBrush Translate Mode Enabled")

    def zBrushScaleMode(self):
        print("zBrushScaleMode")
        self.serialPortSend('x',"zBrush Scale Mode Enabled")

    def zBrushRotateMode(self):
        print("zBrushRotateMode")
        self.serialPortSend('Z',"zBrush Rotate Mode Enabled")

    def m3DBuilderTranslateMode(self):
        print("m3DBuilderTranslateMode")
        self.serialPortSend('i',"3DBuilder Translate Mode Enabled")

    def m3DBuilderRotateMode(self):
        print("m3DBuilderRotateMode")
        self.serialPortSend('I',"3DBuilder Rotate Mode Enabled")

    def Paint3DTranslateMode(self):
        print("Paint3DTranslateMode")
        self.serialPortSend('p',"Paint3D Translate Mode Enabled")

    def Paint3DRotateMode(self):
        print("Paint3DRotateMode")
        self.serialPortSend('P',"Paint3D Rotate Mode Enabled")

    def OpenSCADTranslateMode(self):
        print("OpenSCADTranslateMode")
        self.serialPortSend('o',"OpenSCAD Translate Mode Enabled")

    def OpenSCADRotateMode(self):
        print("OpenSCADRotateMode")
        self.serialPortSend('O',"OpenSCAD Rotate Mode Enabled")        

# Send to the firmware the command (single char otherstuff ignored)      
    def onSendButton(self):
        print("onSendButton")
        ret = self.ui.serialInput.text();
        print(ret);
        if ( len(ret) > 0 ):
            self.serialPortSend(ret[0], None);
        else:
            QMessageBox().warning(self, "Nothing to Send", "Please type something to send")
            
# Refresh all serialports and try to detect the 3DMouse
    def onRefresh(self):
        print("onRefresh")
        self.serialPortSetup()

# Connect with the selected port in the combobox        
    def onConnect(self):
        print("onConnect")
        if ( self.port == None or self.ports == None ):
            print("No 3D Mouse Detected")
            return
        if ( len(self.ports) == 0 ):
            print("No 3D Mouse Detected")
            return
        if ( self.ui.serialPorts.currentIndex() == -1 ):
            print("No Port Selected")
            QMessageBox().warning(self, "Select a Port to Connect", "Please Select first a port to connect")
            return
        self.serialPortConnectAtIndex(self.ui.serialPorts.currentIndex())
        

# Voice Recognition

    def onSpeechRecognition(self):
        print("onSpeechRecognition")
        if self.ui.speechRecognition.checkState() == Qt.CheckState.Checked:
            self.ui.label_2.setEnabled(True)
            self.ui.recognitionLanguage.setEnabled(True)
            self.ui.label_6.setEnabled(True)
            self.ui.outputs.setEnabled(True)
            self.ui.label_5.setEnabled(True)
            self.ui.micros.setEnabled(True)
            self.ui.scanMicros.setEnabled(True)
            self.ui.scanOutputs.setEnabled(True)
            self.ui.testMicro.setEnabled(True)
            self.ui.testRecognition.setEnabled(True)
            self.ui.recognitionOutput.setEnabled(True)

            self.onScanMicros()
            self.onScanOutputs()
        else:
            self.ui.label_2.setEnabled(False)
            self.ui.recognitionLanguage.setEnabled(False)
            self.ui.label_6.setEnabled(False)
            self.ui.outputs.setEnabled(False)
            self.ui.label_5.setEnabled(False)
            self.ui.micros.setEnabled(False)
            self.ui.scanMicros.setEnabled(False)
            self.ui.scanOutputs.setEnabled(False)
            self.ui.testMicro.setEnabled(False)
            self.ui.testRecognition.setEnabled(False)
            self.ui.recognitionOutput.setEnabled(False)
            
        
# Scan all micros
    def onScanMicros(self):
        print("onScanMicros")
        self.ui.micros.clear()
        self.inputsMic.clear()
        p = pyaudio.PyAudio()
        info = p.get_host_api_info_by_index(0)
        numdevices = info.get('deviceCount')

        count = 0
        for i in range(0, numdevices):            
            if (p.get_device_info_by_host_api_device_index(0, i).get('maxInputChannels')) > 0:
                print("Input Device id ", i, " - ", p.get_device_info_by_host_api_device_index(0, i).get('name'))
                self.ui.micros.addItem(p.get_device_info_by_host_api_device_index(0, i).get('name'))
                self.inputsMic[count] = i
                count += 1
        p.terminate()

# Scan all outputs
    def onScanOutputs(self):
        print("onScanOutputs")
        self.ui.outputs.clear()
        self.outputs.clear()
        p = pyaudio.PyAudio()
        info = p.get_host_api_info_by_index(0)
        numdevices = info.get('deviceCount')

        count = 0
        for i in range(0, numdevices):
            if (p.get_device_info_by_host_api_device_index(0, i).get('maxOutputChannels')) > 0:
                print("Output Device id ", i, " - ", p.get_device_info_by_host_api_device_index(0, i).get('name'))
                self.ui.outputs.addItem(p.get_device_info_by_host_api_device_index(0, i).get('name'))
                self.outputs[count] = i
                count += 1
        p.terminate()
        
                
# Test current micro
    def onTestMicro(self):
        print("onTestMicro");

        if self.ui.micros.currentIndex() < 0:
            print("No Micro Set - Please Scan Micros first ")
            return
        if self.ui.outputs.currentIndex() < 0:
            print("No Output Set - Please Scan Outputs first ")
            return
        

        print("Say something in the microphone")
        fs = 8000
        duration = 3
        nsamples  = fs * duration
        p = pyaudio.PyAudio()        
        streamIn = p.open(format=pyaudio.paInt16, channels=1, rate=fs, input=True,
                          frames_per_buffer=nsamples, input_device_index=self.inputsMic[self.ui.micros.currentIndex()])
        buffer = streamIn.read(nsamples)
        streamIn.stop_stream()
        streamIn.close()

        # Play the 3 seconds record now
        print("Playing out the recorded something")
        streamOut = p.open(format=pyaudio.paInt16, channels=1, rate=fs, output=True,
                           frames_per_buffer=nsamples, output_device_index=self.outputs[self.ui.outputs.currentIndex()])
        streamOut.write(buffer)
        streamOut.stop_stream()
        streamOut.close()
        p.terminate()

# Speech Synthesis
    def onSpeechSynthesis(self):
         print("onSpeechSynthesis")

         if self.ui.speechSynthesis.checkState() == Qt.CheckState.Checked:
             self.ui.label_3.setEnabled(True)
             self.ui.voices.setEnabled(True)
             self.ui.scanVoices.setEnabled(True)
             self.ui.label_7.setEnabled(True)
             self.ui.testMessage.setEnabled(True)
             self.ui.testSynthesis.setEnabled(True)

             self.onScanVoices()
         else:
             self.ui.label_3.setEnabled(False)
             self.ui.voices.setEnabled(False)
             self.ui.scanVoices.setEnabled(False)
             self.ui.label_7.setEnabled(False)
             self.ui.testMessage.setEnabled(False)
             self.ui.testSynthesis.setEnabled(False)

    def onScanVoices(self):
        print("onScanVoices")

        engine = pyttsx3.init()
        rate = engine.getProperty("rate")
        svoices = engine.getProperty("voices")

        print(svoices)
        
        for i in svoices:
            print(i)

        self.ui.voices.clear()
        count = 0
        for i in svoices:
            self.ui.voices.addItem(i.name)
            self.voices[count] = i
            count += 1

    def onTestVoice(self):
        print("onTestVoice")

        engine = pyttsx3.init()
        rate = engine.getProperty("rate")
        
        currentIdx = self.ui.voices.currentIndex()
        print("currentIdx = ", currentIdx)
        print(self.voices);
        
        engine.setProperty('voice', self.voices[self.ui.voices.currentIndex()].id)
        engine.say(self.ui.testMessage.text())
        engine.runAndWait()

    def onWhisper(self):
        print("onWhisper");
        
        model = whisper.load_model("tiny")

        if self.ui.micros.currentIndex() < 0:
            print("No Micro Set - Please Scan Micros first ")
            return
        if self.ui.outputs.currentIndex() < 0:
            print("No Output Set - Please Scan Outputs first ")
            return
        

        print("Say something in the microphone")
        fs = 16000
        duration = 3
        nsamples  = fs * duration
        p = pyaudio.PyAudio()        
        streamIn = p.open(format=pyaudio.paInt16, channels=1, rate=fs, input=True,
                          frames_per_buffer=nsamples, input_device_index=self.inputsMic[self.ui.micros.currentIndex()])
        buffer = streamIn.read(nsamples)
        streamIn.stop_stream()
        streamIn.close()


        torch_audio = torch.from_numpy(np.frombuffer(buffer, np.int16).flatten().astype(np.float32) / 32768.0)
        result = model.transcribe( torch_audio )

        # Close
        p.terminate()
        print(result["text"])

        self.ui.recognitionOutput.setText(result["text"])
        

if __name__ == "__main__":
    app = QtWidgets.QApplication([])


    dialog = ControllerDialog()   
    dialog.show()

    sys.exit(app.exec())
