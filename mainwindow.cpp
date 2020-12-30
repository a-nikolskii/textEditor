#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_filename("")
{

    createAction();
    createMenu();

    m_textEdit = (new QPlainTextEdit());
    m_textEdit->setMinimumSize(720, 460);

    setMenuBar(m_pmenuBar);
    setCentralWidget(m_textEdit);

    connect(this, SIGNAL(changeWindowTitle(const QString&)),
            this, SLOT(slotChangeWindowTitle(const QString&)));

    m_data = new QString;
}


MainWindow::~MainWindow()
{
    delete m_data;
}



void MainWindow::createMenu(){
    m_pmenuBar = new QMenuBar(this);

    pmenuFile = new QMenu(tr("&File"));
    pmenuFile->addAction(pactNewFile);
    pmenuFile->addAction(pactOpenFile);
    pmenuFile->addAction(pactOpenRO);
    pmenuFile->addAction(pactSave);
    pmenuFile->addAction(pactSaveAs);
    pmenuFile->addSeparator();
    pmenuFile->addAction(pactPrint);
    pmenuFile->addSeparator();
    pmenuFile->addAction(pactQuit);

    pmenuTools = new QMenu(tr("&Tools"));
    createLanguagesMenu();
    pmenuTools->addMenu(pmenuLanguage);

    pmenuHelp = new QMenu(tr("&Help"));
    pmenuHelp->addAction(pactAboutApp);

    m_pmenuBar->addMenu(pmenuFile);
    m_pmenuBar->addMenu(pmenuTools);
    m_pmenuBar->addMenu(pmenuHelp);
}



void MainWindow::createAction(){

    pactNewFile = new QAction(tr("New File"), this);
    pactNewFile->setShortcut(QKeySequence("CTRL+N"));
    connect(pactNewFile, SIGNAL(triggered()), this, SLOT(slotNew_file()));

    pactOpenFile = new QAction(tr("Open File..."), this);
    pactOpenFile->setShortcut(QKeySequence("CTRL+O"));
    connect(pactOpenFile, SIGNAL(triggered()), this, SLOT(slotOpen_file()));

    pactOpenRO = new QAction(tr("Open for read..."), this);
    connect(pactOpenRO, SIGNAL(triggered()), this, SLOT(slotOpen_file_readOnly()));


    pactSave = new QAction(tr("Save"), this);
    pactSave->setShortcut(QKeySequence("CTRL+S"));
    connect(pactSave, SIGNAL(triggered()), this, SLOT(slotSave()));

    pactSaveAs = new QAction(tr("Save as ..."), this);
    connect(pactSaveAs, SIGNAL(triggered()), this, SLOT(slotSave_as()));

    pactPrint = new QAction(tr("Print"), this);
    pactPrint->setShortcut(QKeySequence("CTRL+P"));
    connect(pactPrint, SIGNAL(triggered()), this, SLOT(slotPrint()));

    pactQuit = new QAction(tr("Quit"), this);
    pactQuit->setShortcut(QKeySequence("CTRL+Q"));
    connect(pactQuit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    pactAboutApp = new QAction(tr("About App"), this);
    connect(pactAboutApp, SIGNAL(triggered()), this, SLOT(slotAbout_app()));
}



void MainWindow::createLanguagesMenu(){

    pmenuLanguage = new QMenu(tr("&Language"), this);

    QActionGroup* plangGroup = new QActionGroup(this);
    connect(plangGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(slotChangeLanguage(QAction*)));

    QString defaultLocale = QLocale::system().name(); // "en_EN"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // "en"

    m_langPath = ":/languages/";
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("QTLanguage_*.qm"));

    QString locale;
    QString lang;
    for (int i = 0; i < fileNames.size(); ++i) {
        locale = fileNames[i]; // "QtLanguages_ru.qm"
        locale.truncate(locale.lastIndexOf('.')); // "QtLanguages_ru"
        locale.remove(0, locale.lastIndexOf('_') + 1); // "ru"

        lang = QLocale::languageToString(QLocale(locale).language()); // "Russian"

        if (lang == "Russian"){
            lang = "Русский";
        }

        QAction* action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(locale);

        plangGroup->addAction(action);
        pmenuLanguage->addAction(action);

        if (defaultLocale == locale){
            action->setChecked(true);
        }
    }
}



void MainWindow::slotSave(){
    if (m_filename.isEmpty()){
        slotSave_as();
        return;
    }
    else {
        QFile file(m_filename);
        if (!file.open(QIODevice::WriteOnly)){
            qDebug() << "File " << m_filename << "isn`t open";
            return;
        }
        else{
            QTextStream stream(&file);
            *m_data = m_textEdit->toPlainText();
            stream << *m_data;
            file.close();
            emit changeWindowTitle(m_filename);
        }
    }
}



void MainWindow::slotSave_as(){
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    QDir::currentPath(),
                                                    tr("all(*.*);;*.txt(*.txt)"));

    if (filename.isEmpty()){
        return;
    }

    m_filename = filename;
    slotSave();
}



void MainWindow::slotOpen_file(){
    *m_data = "";
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    QDir::currentPath(),
                                                    tr("*.txt(*.txt);;all(*.*)"));
    if (filename.isEmpty()){
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly)){
        qDebug() << "File " << filename << "isn`t open";
        return;
    }
    else {
        QTextStream stream(&file);
        while (!stream.atEnd()){
            *m_data += stream.readLine() + "\n";
        }
        file.close();

        m_filename = filename;
        emit changeWindowTitle(m_filename);
        m_textEdit->setPlainText(*m_data);
    }
    m_textEdit->setReadOnly(false);
}



void MainWindow::slotNew_file(){
    *m_data = "";
    m_filename = "";
    emit changeWindowTitle(tr("New File.txt"));
    m_textEdit->setPlainText(*m_data);
}



void MainWindow::slotAbout_app(){
    //std::shared_ptr<QMessageBox> pmes = std::make_shared<QMessageBox>(new QMessageBox);
    QMessageBox::information(this, tr("About application"),
                             tr("Text Editor\n"
                                "version 1.0\n"
                                "2020"
                                "\n"
                                "contacts: "
                                "alexandr.nikolskii@gmail.com"));
}



void MainWindow::slotChangeWindowTitle(const QString& str){
    setWindowTitle(str);
}



void MainWindow::slotOpen_file_readOnly(){
    slotOpen_file();
    m_textEdit->setReadOnly(true);
}



void MainWindow::slotPrint(){
    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle(tr("Print"));
    if (dlg.exec() != QDialog::Accepted){
        return;
    }
    else {
        std::shared_ptr<QStringList> list = strToStringList(m_data);
        int line = 0;
        QPainter painter;
        painter.begin(&printer);
        int w = painter.window().width();
        int h = painter.window().height();
        int amount = list->count();
        QFont font = painter.font();
        QFontMetrics fmetrics(font);
        for (int i = 0; i < amount; i++)
        {
               QPointF pf;
               pf.setX(10);
               pf.setY(line);
               painter.drawText(pf, list->at(i));
               line += fmetrics.height();
               if (h - line <= fmetrics.height())
               {
                   printer.newPage();
                   line = 0;
               }

           }
           painter.end();
    }
}



std::shared_ptr<QStringList> MainWindow::strToStringList(QString* pstr) {
    std::shared_ptr<QStringList> list = std::make_shared<QStringList>();
    QTextStream stream(pstr, QIODevice::ReadOnly);
    while (!stream.atEnd()){
        list->append(stream.readLine());
    }
    return list;
}



void MainWindow::slotChangeLanguage(QAction* pact){
    if (pact != nullptr){
        QString rLanguage = pact->data().toString();
        if (m_currLang != rLanguage) {
            QLocale locale = QLocale(m_currLang);
            QLocale::setDefault(locale);
            QString languageName = QLocale::languageToString(locale.language());
            switchTranslator(m_translator, QString("QtLanguage_%1.qm").arg(rLanguage));
            switchTranslator(m_translatorQt, QString("qtbase_%1.qm").arg(rLanguage));
        }
    }
}



void MainWindow::switchTranslator(QTranslator& translator, const QString& filename){

    QApplication::instance()->removeTranslator(&translator);
    if (translator.load(":/languages/" + filename)){
        QApplication::instance()->installTranslator(&translator);
    }
}



void MainWindow::changeEvent(QEvent* event){
    if (event->type() == QEvent::LanguageChange){

        pmenuFile->setTitle(tr("&File"));
        pactNewFile->setText(tr("New File"));
        pactOpenFile->setText(tr("Open File..."));
        pactOpenRO->setText(tr("Open for read..."));
        pactSave->setText(tr("Save"));
        pactSaveAs->setText(tr("Save as ..."));
        pactPrint->setText(tr("Print"));
        pactQuit->setText(tr("Quit"));

        pmenuTools->setTitle(tr("&Tools"));
        pmenuLanguage->setTitle(tr("&Language"));

        pmenuHelp->setTitle(tr("&Help"));
        pactAboutApp->setText(tr("About App"));


        if (m_filename.isEmpty()) {
            emit changeWindowTitle(tr("New File.txt"));
        }
    }
    else QMainWindow::changeEvent(event);
}


