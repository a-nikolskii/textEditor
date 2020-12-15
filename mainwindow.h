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


private:
    void createMenu();
    QMenu* createLanguagesMenu();
    void switchTranslator(QTranslator& translator, const QString& filename);
    void retranslate();

protected:
    void changeEvent(QEvent*) override;


};
#endif // MAINWINDOW_H
