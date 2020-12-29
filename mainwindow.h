#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QPlainTextEdit>
#include <QPalette>
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QTranslator>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void changeWindowTitle(const QString&);


private slots:
    void slotSave();
    void slotSave_as();
    void slotOpen_file();
    void slotNew_file();
    void slotOpen_file_readOnly();
    void slotPrint();
    void slotChangeWindowTitle(const QString& str);
    void slotChangeLanguage(QAction* pact);


private:
    QMenuBar* m_pmenuBar;

    QPlainTextEdit* m_textEdit;
    QString* m_data;
    QString m_filename;

    QString m_langPath;
    QString m_currLang;
    QTranslator m_translator;
    QTranslator m_translatorQt;


private:
    void createMenu();
    void createAction();
    void createLanguagesMenu();
    void switchTranslator(QTranslator& translator, const QString& filename);
    std::shared_ptr<QStringList> strToStringList(QString* pstr);

private:
    QMenu* pmenuFile;
    QMenu* pmenuTools;
    QMenu* pmenuHelp;

    QMenu* pmenuLanguage;

    QAction* pactNewFile;
    QAction* pactOpenFile;
    QAction* pactOpenRO;
    QAction* pactSave;
    QAction* pactSaveAs;
    QAction* pactPrint;
    QAction* pactQuit;





protected:
    void changeEvent(QEvent*) override;


};
#endif // MAINWINDOW_H
