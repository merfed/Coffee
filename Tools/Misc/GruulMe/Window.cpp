#include "Window.h"

Window::Window() : QWidget(), isPainting(false), cursorPosX (0), cursorPosY (0), wdtPathSaveTo( QString() )
{
    mainLayout = new QVBoxLayout;

    mainTabWidget = new QTabWidget;

    tabGeneral = new QWidget;
    tabArray = new QWidget;
    tabWMO = new QWidget;

    // Tab General
    generalLayout = new QVBoxLayout;

    genInfosLayout = new QVBoxLayout;
    genFileButtonsLayout = new QHBoxLayout;
    new_b = new QPushButton("New...", this);
    QObject::connect(new_b, SIGNAL(clicked()), this, SLOT(newWDT()));
    load_b = new QPushButton("Load...", this);
    QObject::connect(load_b, SIGNAL(clicked()), this, SLOT(loadWDT()));
    save_b = new QPushButton("Save", this);
    QObject::connect(save_b, SIGNAL(clicked()), this, SLOT(saveWDT()));
    saveAs_b = new QPushButton("Save as...", this);
    QObject::connect(saveAs_b, SIGNAL(clicked()), this, SLOT(saveWDTas()));
    genFileButtonsLayout->addWidget(new_b);
    genFileButtonsLayout->addWidget(load_b);
    genFileButtonsLayout->addWidget(save_b);
    genFileButtonsLayout->addWidget(saveAs_b);
    wdtPath_label = new QLabel("No WDT loaded.");
    wdtPath_label->setAlignment(Qt::AlignCenter);
    flags_gb = new QGroupBox("WDT Flags");
    flags_gb->setAlignment(Qt::AlignCenter);
    flags_layout = new QVBoxLayout;
    flag1_cb = new QCheckBox("0x1 : use WMO based definition (gruuuuul)");
    QObject::connect(flag1_cb, SIGNAL(stateChanged(int)), this, SLOT(toggleWMOtab(int)));
    flag2_cb = new QCheckBox("0x2 : use vertex shading (MCCV)");
    flag4_cb = new QCheckBox("0x4 : use _env terrain shaders");
    flag8_cb = new QCheckBox("0x8 : use rendering thing");
    flag10_cb = new QCheckBox("0x10 : use vertex lighting (MCLV) [Cata]");
    flag20_cb = new QCheckBox("0x20 : use ceiling mode, flips ground upside down [Cata]");
    flag40_cb = new QCheckBox("0x40 : ? [Cata]");
    flag80_cb = new QCheckBox("0x80 : ? [MoP]");
    flags_layout->addWidget(flag1_cb);
    flags_layout->addWidget(flag2_cb);
    flags_layout->addWidget(flag4_cb);
    flags_layout->addWidget(flag8_cb);
    flags_layout->addWidget(flag10_cb);
    flags_layout->addWidget(flag20_cb);
    flags_layout->addWidget(flag40_cb);
    flags_layout->addWidget(flag80_cb);
    flags_gb->setLayout(flags_layout);
    genInfosLayout->addLayout(genFileButtonsLayout);
    genInfosLayout->addWidget(wdtPath_label);

    generalLayout->addLayout(genInfosLayout);
    generalLayout->addWidget(flags_gb);

    tabGeneral->setLayout(generalLayout);

    // Tab array
    arrayLayout = new QHBoxLayout;

    gps = new QLabel ( QString("X : 0, Y : 0") );
    gps->setAlignment(Qt::AlignCenter);

    arrayGScene = new AdtArrayScene;
    arrayGView = new AdtArray;
    arrayGView->setScene(arrayGScene);
    arrayGView->setFixedSize(515, 515);

    QObject::connect(arrayGView, SIGNAL(mousePos(int,int)), this, SLOT(refreshGPS(int,int)));
    QObject::connect(arrayGScene, SIGNAL(mouseIsPressed()), this, SLOT(applyBrush()));

    brush0 = new QRadioButton ("Delete tile");
    brush1 = new QRadioButton ("Add ADT");
    brush2 = new QRadioButton ("Add fake water");
    brush1->setChecked(true);
    arrayBrushHBL = new QVBoxLayout;
    arrayBrushHBL->addWidget(brush0);
    arrayBrushHBL->addWidget(brush1);
    arrayBrushHBL->addWidget(brush2);
    arrayBrushGB = new QGroupBox;
    arrayBrushGB->setLayout(arrayBrushHBL);

    arrayTools = new QVBoxLayout;
    autoscan_b = new QPushButton ("Scan dir");
    QObject::connect(autoscan_b, SIGNAL(clicked()), this, SLOT(autoscan()));
    deleteAll_b = new QPushButton ("Delete all");
    QObject::connect(deleteAll_b, SIGNAL(clicked()), this, SLOT(fillAll0()));
    fillAllGround_b = new QPushButton ("Fill with ground");
    QObject::connect(fillAllGround_b, SIGNAL(clicked()), this, SLOT(fillAll1()));
    fillAllWater_b = new QPushButton ("Fill with water");
    QObject::connect(fillAllWater_b, SIGNAL(clicked()), this, SLOT(fillAll2()));
    arrayTools->addWidget(gps);
    arrayTools->addWidget(arrayBrushGB);
    arrayTools->addWidget(autoscan_b);
    arrayTools->addWidget(deleteAll_b);
    arrayTools->addWidget(fillAllGround_b);
    arrayTools->addWidget(fillAllWater_b);

    arrayLayout->addLayout(arrayTools);
    arrayLayout->addWidget(arrayGView);
    tabArray->setLayout(arrayLayout);

    // Tab WMO
    wmoLayout = new QFormLayout;
    wmoPath_le = new QLineEdit;
    wmoID_sb = new QSpinBox;
    wmoUID_sb = new QSpinBox;
    posX_dsb = new QDoubleSpinBox; posX_dsb->setRange(-10000000,10000000); posX_dsb->setDecimals(5);
    posY_dsb = new QDoubleSpinBox; posY_dsb->setRange(-10000000,10000000); posY_dsb->setDecimals(5);
    posZ_dsb = new QDoubleSpinBox; posZ_dsb->setRange(-10000000,10000000); posZ_dsb->setDecimals(5);
    oriA_dsb = new QDoubleSpinBox; oriA_dsb->setRange(-10000000,10000000); oriA_dsb->setDecimals(5);
    oriB_dsb = new QDoubleSpinBox; oriB_dsb->setRange(-10000000,10000000); oriB_dsb->setDecimals(5);
    oriC_dsb = new QDoubleSpinBox; oriC_dsb->setRange(-10000000,10000000); oriC_dsb->setDecimals(5);
    upe1_dsb = new QDoubleSpinBox; upe1_dsb->setRange(-10000000,10000000); upe1_dsb->setDecimals(5);
    upe2_dsb = new QDoubleSpinBox; upe2_dsb->setRange(-10000000,10000000); upe2_dsb->setDecimals(5);
    upe3_dsb = new QDoubleSpinBox; upe3_dsb->setRange(-10000000,10000000); upe3_dsb->setDecimals(5);
    loe1_dsb = new QDoubleSpinBox; loe1_dsb->setRange(-10000000,10000000); loe1_dsb->setDecimals(5);
    loe2_dsb = new QDoubleSpinBox; loe2_dsb->setRange(-10000000,10000000); loe2_dsb->setDecimals(5);
    loe3_dsb = new QDoubleSpinBox; loe3_dsb->setRange(-10000000,10000000); loe3_dsb->setDecimals(5);
    wmoFlags_sb = new QSpinBox;
    wmoDDset_sb = new QSpinBox;
    wmoNameSet_sb = new QSpinBox;
    wmoLayout->addRow("WMO Path", wmoPath_le);
    wmoLayout->addRow("Index in MWID", wmoID_sb);
    wmoLayout->addRow("Unique ID", wmoUID_sb);
    wmoLayout->addRow("Position X", posX_dsb);
    wmoLayout->addRow("Position Y", posY_dsb);
    wmoLayout->addRow("Position Z", posZ_dsb);
    wmoLayout->addRow("Orientation A", oriA_dsb);
    wmoLayout->addRow("Orientation B", oriB_dsb);
    wmoLayout->addRow("Orientation C", oriC_dsb);
    wmoLayout->addRow("Upper extents 1", upe1_dsb);
    wmoLayout->addRow("Upper extents 2", upe2_dsb);
    wmoLayout->addRow("Upper extents 3", upe3_dsb);
    wmoLayout->addRow("Lower extents 1", loe1_dsb);
    wmoLayout->addRow("Lower extents 2", loe2_dsb);
    wmoLayout->addRow("Lower extents 3", loe3_dsb);
    wmoLayout->addRow("Flags (WMO)", wmoFlags_sb);
    wmoLayout->addRow("Doodad set index", wmoDDset_sb);
    wmoLayout->addRow("Name set", wmoNameSet_sb);

    tabWMO->setLayout(wmoLayout);

    // Bottom buttons
    genButtonsLayout = new QHBoxLayout;
    exit_b = new QPushButton("Close", this);
    QObject::connect(exit_b, SIGNAL(clicked()), qApp, SLOT(quit()));
    about_b = new QPushButton("About", this);
    QObject::connect(about_b, SIGNAL(clicked()), this, SLOT(aboutGruulMe()));
    genButtonsLayout->addWidget(about_b);
    genButtonsLayout->addWidget(exit_b);

    // and finally
    mainTabWidget->addTab(tabGeneral, "General");
    mainTabWidget->addTab(tabArray, "ADT Array");
    mainTabWidget->addTab(tabWMO, "WMO infos");

    mainLayout->addWidget(mainTabWidget);
    mainLayout->addLayout(genButtonsLayout);

    arrayGView->setStyleSheet("background-image: url(./adtarraybg.png);");

    setWindowTitle("GruulMe");
    setLayout(mainLayout);

    arrayGView->setMouseTracking(true);
}





/**
  new WDT
  */
void Window::newWDT()
{
    wdtPath = QFileDialog::getSaveFileName(this, "Select a path for new WDT", QString());
    wdtPathSaveTo = wdtPath;
    loadedWDT = Wdt();
    refreshWindow();
}

/**
  load a WDT and ask for refreshing the window
  */
void Window::loadWDT()
{
    QString wdtNewPath = QFileDialog::getOpenFileName(this, "Open WDT", QString(), "WDT (*.wdt)");
    if (wdtNewPath != wdtPath)
    {
        wdtPath = wdtNewPath;
        wdtPathSaveTo = wdtPath;
        loadedWDT = Wdt(wdtPath.toStdString());
        refreshWindow();
    }
}

/**
  save the WDT on the previous loaded
  */
void Window::saveWDT()
{
    if (!wdtPath.isEmpty())
    {
        std::vector<Chunk> fullWDT = loadedWDT.getFullWDT();
        int wdtFlags (0);
        if (flag1_cb->isChecked()) wdtFlags += 0x1;
        if (flag2_cb->isChecked()) wdtFlags += 0x2;
        if (flag4_cb->isChecked()) wdtFlags += 0x4;
        if (flag8_cb->isChecked()) wdtFlags += 0x8;
        if (flag10_cb->isChecked()) wdtFlags += 0x10;
        if (flag20_cb->isChecked()) wdtFlags += 0x20;
        if (flag40_cb->isChecked()) wdtFlags += 0x40;
        if (flag80_cb->isChecked()) wdtFlags += 0x80;

        Chunk mwmo = Wdt::buildMWMO(wmoPath_le->text().toStdString());
        Chunk modf = Wdt::buildMODF(wmoID_sb->value(), wmoUID_sb->value(), posX_dsb->value(), posY_dsb->value(), posZ_dsb->value(), oriA_dsb->value(), oriB_dsb->value(), oriC_dsb->value(),
                                    upe1_dsb->value(), upe2_dsb->value(), upe3_dsb->value(), loe1_dsb->value(), loe2_dsb->value(), loe3_dsb->value(),
                                    wmoFlags_sb->value(), wmoDDset_sb->value(), wmoNameSet_sb->value());
        Chunk emptyChunk;

        Wdt WDTtoWrite;                    
        if (flag1_cb->isChecked())
        {
            WDTtoWrite = Wdt(wdtPathSaveTo.toStdString(), fullWDT[0], fullWDT[1], fullWDT[2], mwmo, modf);
        }
        else
        {
            WDTtoWrite = Wdt(wdtPathSaveTo.toStdString(), fullWDT[0], fullWDT[1], fullWDT[2], emptyChunk, emptyChunk);
        }

        WDTtoWrite.setFlags(wdtFlags);

        WDTtoWrite.toFile();
        QMessageBox::information(this, "GruulMe", "Following file has been <font color=\"#226622\">saved</font> :\n" + wdtPathSaveTo);
        wdtPathSaveTo = wdtPath;
    }
    else
    {
        QMessageBox::information(this, "GruulMe", "No WDT loaded.");
    }
}

void Window::saveWDTas()
{
    wdtPathSaveTo = QFileDialog::getSaveFileName(this, "Save WDT as", QString());
    saveWDT();
}



void Window::applyBrush()
{
    if (!wdtPath.isEmpty())
    {
        loadedWDT.setMAINentry(cursorPosY, cursorPosX, brushMode());
        refreshADTarray();
    }
}

void Window::autoscan()
{
    if (!wdtPath.isEmpty())
    {
        std::string path = wdtPath.toStdString();
        size_t ofsFileName = path.find_last_of("/\\");
        std::string name = path.substr(ofsFileName + 1);
        path = path.substr(0, ofsFileName + 1);
        size_t ofsDot = name.find_last_of(".");
        name = name.substr(0, ofsDot);

        //QMessageBox::information(this, "lawl", QString().fromStdString(path));
        std::string adtFileToSearch;
        std::stringstream strstm;
        for (int i = 0; i < 4096; ++i)
        {
            strstm << path << name << "_" << (i / 64) << "_" << (i % 64) << ".adt";
            adtFileToSearch = strstm.str();
            if (Utilities::fexists(adtFileToSearch))
                loadedWDT.setMAINentry((i % 64), (i / 64), 1);
            strstm.str("");
        }
        refreshADTarray();
    }
}

// looks so wrong ? i know...
void Window::fillAll0()
{
    if (!wdtPath.isEmpty())
    {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                loadedWDT.setMAINentry(j, i, 0);
        refreshADTarray();
    }
}
void Window::fillAll1()
{
    if (!wdtPath.isEmpty())
    {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                loadedWDT.setMAINentry(j, i, 1);
        refreshADTarray();
    }
}
void Window::fillAll2()
{
    if (!wdtPath.isEmpty())
    {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                loadedWDT.setMAINentry(j, i, 2);
        refreshADTarray();
    }
}




void Window::refreshGPS(int x, int y)
{
    cursorPosX = x / 8;
    cursorPosY = y / 8;
    if (cursorPosX < 64 && cursorPosX >= 0 && cursorPosY < 64 && cursorPosY >= 0)
        gps->setText( QString("X : " + QString().number(cursorPosX) + ", Y : " + QString().number(cursorPosY)) );
}

/**
  set up to date all the window
  */
void Window::refreshWindow()
{
    // general
    wdtPath_label->setText(wdtPath);
    if (loadedWDT.getFlags() & 0x1)
        flag1_cb->setChecked(true);
    else
        flag1_cb->setChecked(false);
    if (loadedWDT.getFlags() & 0x2)
        flag2_cb->setChecked(true);
    else
        flag2_cb->setChecked(false);
    if (loadedWDT.getFlags() & 0x4)
        flag4_cb->setChecked(true);
    else
        flag4_cb->setChecked(false);
    if (loadedWDT.getFlags() & 0x8)
        flag8_cb->setChecked(true);
    else
        flag8_cb->setChecked(false);
    if (loadedWDT.getFlags() & 0x10)
        flag10_cb->setChecked(true);
    else
        flag10_cb->setChecked(false);
    if (loadedWDT.getFlags() & 0x20)
        flag20_cb->setChecked(true);
    else
        flag20_cb->setChecked(false);
    if (loadedWDT.getFlags() & 0x40)
        flag40_cb->setChecked(true);
    else
        flag40_cb->setChecked(false);
    if (loadedWDT.getFlags() & 0x80)
        flag80_cb->setChecked(true);
    else
        flag80_cb->setChecked(false);

    refreshADTarray();

    // wmo
    if (loadedWDT.getFlags() & 0x1)
    {
        setWindowTitle("GruulMe - " + wdtPath);
        mainTabWidget->setTabEnabled(2, true);
        wmoPath_le->setText(QString::fromStdString(loadedWDT.getWMOpath()));
        std::vector<int> curVector3;
        curVector3 = loadedWDT.getWMOIDinfos();
        wmoID_sb->setValue(curVector3[0]);
        wmoUID_sb->setValue(curVector3[1]);
        std::vector<float> curVector;
        curVector = loadedWDT.getWMOpos();
        posX_dsb->setValue(curVector[0]);
        posY_dsb->setValue(curVector[1]);
        posZ_dsb->setValue(curVector[2]);
        curVector = loadedWDT.getWMOori();
        oriA_dsb->setValue(curVector[0]);
        oriB_dsb->setValue(curVector[1]);
        oriC_dsb->setValue(curVector[2]);
        curVector = loadedWDT.getWMOext();
        upe1_dsb->setValue(curVector[0]);
        upe2_dsb->setValue(curVector[1]);
        upe3_dsb->setValue(curVector[2]);
        loe1_dsb->setValue(curVector[3]);
        loe2_dsb->setValue(curVector[4]);
        loe3_dsb->setValue(curVector[5]);
        std::vector<short> curVector2;
        curVector2 = loadedWDT.getWMOaddInfos();
        wmoFlags_sb->setValue(int(curVector2[0]));
        wmoDDset_sb->setValue(int(curVector2[1]));
        wmoNameSet_sb->setValue(int(curVector2[2]));
    }
    else
    {
        mainTabWidget->setTabEnabled(2, false);
    }
}

void Window::refreshADTarray()
{
    arrayGScene->clear();

    // array
    QPen pen (QColor(50, 50, 50), 1, Qt::SolidLine);
    QBrush brushWhite (QColor(255, 255, 255));
    QBrush brushGround (QColor(171, 131, 61));
    QBrush brushWater (QColor(168,153,222));

    for (int i = 0; i < 64; ++i)
    {
        for (int j = 0; j < 64; ++j)
        {
            if (loadedWDT.getMAINentry(i, j) == 0)
            {
                QRect groundTile(j*8, i*8, 8, 8);
                arrayGScene->addRect(groundTile, pen, brushWhite);
            }
            else if (loadedWDT.getMAINentry(i, j) == 1)
            {
                QRect groundTile(j*8, i*8, 8, 8);
                arrayGScene->addRect(groundTile, pen, brushGround);
            }
            else if (loadedWDT.getMAINentry(i, j) == 2)
            {
                QRect groundTile(j*8, i*8, 8, 8);
                arrayGScene->addRect(groundTile, pen, brushWater);
            }
        }
    }
}




char Window::brushMode()
{
    char c (0);
    if (brush0->isChecked())
        c = 0;
    else if (brush1->isChecked())
        c = 1;
    else if (brush2->isChecked())
        c = 2;

    return c;
}

void Window::toggleWMOtab(int newState)
{
    if (!wdtPath.isEmpty())
    {
        switch (newState)
        {
            case (0): mainTabWidget->setTabEnabled(2, false);
            break;
            case (2): mainTabWidget->setTabEnabled(2, true);
            break;
        }
    }
}





void Window::aboutGruulMe()
{
    QMessageBox aboutBox;
    aboutBox.setIconPixmap(QPixmap("./icon.bmp"));
    aboutBox.setWindowTitle("About GruulMe");
    aboutBox.setText("GruulMe is a WDT editor developed by Gamhea, for Architecture Department and friends.<br />Licence CC BY-NC-SA<br />Thanks goes to :<ul><li>Malu05, Schlumpf, Tigurius and Mjollnà for docs.</li><li>Mjollnà for GP design used here</li><li>#modcraft for help and friendly devs</li></ul>Spring 2012<br /><a href=\"http://gamhea.net\">gamhea.net</a>");
    aboutBox.exec();
}
