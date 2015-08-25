#ifndef WINDOW_H
#define WINDOW_H

#include <QApplication>
#include <QWidget>
#include <QtGui>
#include <QMouseEvent>
#include <sstream>
#include "Utilities.h"
#include "Wdt.h"
#include "adtarray.h"
#include "adtarrayscene.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

public slots:
    void aboutGruulMe();
    void newWDT();
    void loadWDT();
    void saveWDT();
    void saveWDTas();
    void refreshGPS(int x, int y);
    void applyBrush();
    void autoscan();
    void fillAll0();
    void fillAll1();
    void fillAll2();
    void toggleWMOtab(int newState);

private:
    // Window
    QVBoxLayout *mainLayout;

    QTabWidget *mainTabWidget;
    QWidget *tabGeneral;
    QWidget *tabArray;
    QWidget *tabWMO;

    QVBoxLayout *generalLayout;
    QVBoxLayout *genInfosLayout;
    QHBoxLayout *genFileButtonsLayout;
    QPushButton *new_b;
    QPushButton *load_b;
    QPushButton *save_b;
    QPushButton *saveAs_b;
    QLabel *wdtPath_label;
    QGroupBox *flags_gb;
    QVBoxLayout *flags_layout;
    QCheckBox *flag1_cb;
    QCheckBox *flag2_cb;
    QCheckBox *flag4_cb;
    QCheckBox *flag8_cb;
    QCheckBox *flag10_cb;
    QCheckBox *flag20_cb;
    QCheckBox *flag40_cb;
    QCheckBox *flag80_cb;
    QHBoxLayout *genButtonsLayout;
    QPushButton *exit_b;
    QPushButton *about_b;

    QHBoxLayout *arrayLayout;
    AdtArray *arrayGView;
    AdtArrayScene *arrayGScene;
    QVBoxLayout *arrayTools;
    QVBoxLayout *arrayBrushHBL;
    QGroupBox *arrayBrushGB;
    QRadioButton *brush0;
    QRadioButton *brush1;
    QRadioButton *brush2;
    QLabel *gps;
    QPushButton *autoscan_b;
    QPushButton *deleteAll_b;
    QPushButton *fillAllGround_b;
    QPushButton *fillAllWater_b;

    QFormLayout *wmoLayout;
    QLineEdit *wmoPath_le;
    QSpinBox *wmoID_sb;
    QSpinBox *wmoUID_sb;
    QDoubleSpinBox *posX_dsb;
    QDoubleSpinBox *posY_dsb;
    QDoubleSpinBox *posZ_dsb;
    QDoubleSpinBox *oriA_dsb;
    QDoubleSpinBox *oriB_dsb;
    QDoubleSpinBox *oriC_dsb;
    QDoubleSpinBox *upe1_dsb;
    QDoubleSpinBox *upe2_dsb;
    QDoubleSpinBox *upe3_dsb;
    QDoubleSpinBox *loe1_dsb;
    QDoubleSpinBox *loe2_dsb;
    QDoubleSpinBox *loe3_dsb;
    QSpinBox *wmoFlags_sb;
    QSpinBox *wmoDDset_sb;
    QSpinBox *wmoNameSet_sb;

    void refreshADTarray();
    void refreshWindow();

    char brushMode();
    bool isPainting;

    int cursorPosX;
    int cursorPosY;

    // WDT
    QString wdtPath;
    QString wdtPathSaveTo;

    Wdt loadedWDT;
};

#endif // WINDOW_H
