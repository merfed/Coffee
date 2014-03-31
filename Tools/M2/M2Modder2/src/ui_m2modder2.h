/********************************************************************************
** Form generated from reading ui file 'm2modder2.ui'
**
** Created: Mon 25. May 21:56:42 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_M2MODDER2_H
#define UI_M2MODDER2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_M2Modder2Class
{
public:
    QAction *actionDatei_ffnen;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *M2Modder2Class)
    {
        if (M2Modder2Class->objectName().isEmpty())
            M2Modder2Class->setObjectName(QString::fromUtf8("M2Modder2Class"));
        M2Modder2Class->setWindowModality(Qt::NonModal);
        M2Modder2Class->resize(800, 600);
        actionDatei_ffnen = new QAction(M2Modder2Class);
        actionDatei_ffnen->setObjectName(QString::fromUtf8("actionDatei_ffnen"));
        centralwidget = new QWidget(M2Modder2Class);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        M2Modder2Class->setCentralWidget(centralwidget);
        menubar = new QMenuBar(M2Modder2Class);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        M2Modder2Class->setMenuBar(menubar);
        statusbar = new QStatusBar(M2Modder2Class);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        M2Modder2Class->setStatusBar(statusbar);

        retranslateUi(M2Modder2Class);

        QMetaObject::connectSlotsByName(M2Modder2Class);
    } // setupUi

    void retranslateUi(QMainWindow *M2Modder2Class)
    {
        M2Modder2Class->setWindowTitle(QApplication::translate("M2Modder2Class", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionDatei_ffnen->setText(QApplication::translate("M2Modder2Class", "Datei \303\266ffnen", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(M2Modder2Class);
    } // retranslateUi

};

namespace Ui {
    class M2Modder2Class: public Ui_M2Modder2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_M2MODDER2_H
