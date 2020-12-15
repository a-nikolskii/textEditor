#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_filename("")
{

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

    QMenu* pmenuFile = new QMenu(tr("&File"));
    QMenu* pmenuTools = new QMenu(tr("&Tools"));
    QMenu* pmenuHelp = new QMenu(tr("&Help"));


    pmenuFile->addAction(tr("New File"),
                           this,
                           SLOT(slotNew_file()),
                           QKeySequence("CTRL+N")
                           );
    pmenuFile->addAction(tr("Open File..."),
                           this,
                           SLOT(slotOpen_file()),
                           QKeySequence("CTRL+O")
                           );
    pmenuFile->addAction(tr("Open File in RO mode..."),
                           this,
                           SLOT(slotOpen_file_readOnly())
                           );
    pmenuFile->addAction(tr("Save"),
                           this,
                           SLOT(slotSave()),
                           QKeySequence("CTRL+S")
                           );
    pmenuFile->addAction(tr("Save As..."),
                           this,
                           SLOT(slotSave_as()));
    pmenuFile->addSeparator();
    pmenuFile->addAction(tr("&Quit"),
                           QApplication::instance(),
                           SLOT(quit()),
                           QKeySequence("CTRL+Q"));

    pmenuTools->addMenu(createLanguagesMenu());

    m_pmenuBar->addMenu(pmenuFile);
    m_pmenuBar->addMenu(pmenuTools);
    m_pmenuBar->addMenu(pmenuHelp);
}



QMenu* MainWindow::createLanguagesMenu(){

    QMenu* psubmLang = new QMenu(tr("&Language"));

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

        QAction* action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(locale);

        plangGroup->addAction(action);
        psubmLang->addAction(action);

        if (defaultLocale == locale){
            action->setChecked(true);
        }
    }


    return psubmLang;
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
    emit changeWindowTitle("New File");
    m_textEdit->setPlainText(*m_data);
}



void MainWindow::slotChangeWindowTitle(const QString& str){
    setWindowTitle(str);
    m_textEdit->setReadOnly(true);
}



void MainWindow::slotOpen_file_readOnly(){
    slotOpen_file();
}



void MainWindow::slotChangeLanguage(QAction* pact){
    if (pact != nullptr){
        QString rLanguage = pact->data().toString();
        if (m_currLang != rLanguage) {
            QLocale locale = QLocale(m_currLang);
            QLocale::setDefault(locale);
            QString languageName = QLocale::languageToString(locale.language());
            switchTranslator(m_translator, QString("QtLanguage_%1.qm").arg(rLanguage));
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
        retranslate();
    }
    else QMainWindow::changeEvent(event);
}


void MainWindow::retranslate(){
    qDebug() << "retranslate function doesn`t implement";
}



