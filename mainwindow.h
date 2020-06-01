#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <boost/multiprecision/gmp.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static void stopAxis(int axis);
    void keepDriving(double rA, double dec);
    void motorSetup();

    int motorPower1;
    int motorPower2;

    int PowerClamp(int input);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_trackButton_clicked();

    void on_calibrateButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_rightButton_clicked();

    void on_leftButton_clicked();

private:
    bool calibrating;

    Ui::MainWindow *ui;

    QLineEdit *NGCCode;
    QLineEdit *ICCode;
    QLineEdit *MCode;

    QPushButton *upButton;
    QPushButton *downButton;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *slewButton;
    QPushButton *trackButton;
    QPushButton *calibrate;
    QPushButton *clearButton;
    QPushButton *updateButton;

};


#endif // MAINWINDOW_H
