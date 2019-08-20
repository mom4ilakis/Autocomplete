#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Trie.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_InputBox_textChanged(const QString &arg1);

    void on_LoadFromFileButton_clicked();

    void on_NumberOfSuggestions_valueChanged(int arg1);

    void on_InputBox_returnPressed();

    void on_ExitButton_clicked();

private:
    Ui::MainWindow *ui;
    PrefixTree tree;
    PrefixTree user_input_tree;
    QString iWord;
    size_t lastSize;
};

#endif // MAINWINDOW_H
