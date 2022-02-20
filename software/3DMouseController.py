import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui
from PySide6.QtWidgets import QDialog
from ui_dialog import Ui_Dialog

if __name__ == "__main__":
    app = QtWidgets.QApplication([])


    dialog = QDialog();
    ui = Ui_Dialog();
    ui.setupUi(dialog);
    dialog.show();

    sys.exit(app.exec())
