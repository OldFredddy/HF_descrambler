#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_textBrowser_anchorClicked(const QUrl &arg1);

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void handleFileDrop(const QString &filePath);

    void on_textEdit_textChanged();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_textEdit_3_textChanged();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
