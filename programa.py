from PyQt5.QtCore import *
from PyQt5.QtGui import QPixmap
from PyQt5 import QtWidgets, QtCore
from PyQt5 import uic

# funções

app = QtWidgets.QApplication([])
janela = uic.loadUi("interface.ui")

# declaração dos componentes

janela.show()
app.exec_()
