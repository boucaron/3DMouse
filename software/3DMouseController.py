import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui
from PySide6.QtWidgets import QDialog
from ui_dialog import Ui_Dialog


class ControllerDialog(QDialog):

    def __init__(self, parent=None):
        super(ControllerDialog, self).__init__(parent)
        ui = Ui_Dialog();
        ui.setupUi(self);


if __name__ == "__main__":
    app = QtWidgets.QApplication([])


    dialog = ControllerDialog();   
    dialog.show();

    sys.exit(app.exec())
