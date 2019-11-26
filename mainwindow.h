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

    double targetAz;
    double targetAlt;
    double targetZen;

    void convert(double rightAscension, double declination, double &azimuth, double &altitude, double &Zen);

    void stopAll();
    void keepDriving(double rA, double dec);
    void motorSetup();

    const int InputInclineA = 12;
    const int InputInclineB = 18;

    const int InputRotateA = 13;
    const int InputRotateB = 19;

    int motorPower1;
    int motorPower2;

    void MotorControl(int powerPercent, int motorLead1, int motorLead2, bool forward);
    int PowerClamp(int input);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_trackButton_clicked();

    void on_calibrateButton_clicked();

private:
    bool tracking;
    bool calibrating;

    double hourAngle;
    double CIOdec;
    double CIORA;
    double EqOrigins;


    double jTime1;
    double jTime2;

    struct datetime
    {
            int year;
            int month;
            int day;
            int hour;
            int minute;
            double second;
            double fraction;
    };
    datetime getTime(std::string str)
    {
        datetime dt;
        dt.year = stoi(str.substr(0,4));
        dt.month = stoi(str.substr(4,2));
        dt.day = stoi(str.substr(6,2));
        dt.hour = stoi(str.substr(9,2));
        dt.minute = stoi(str.substr(11,2));
        dt.second = stod(str.substr(13,2));
        dt.fraction = stod(str.substr(16,8));
        return dt;
    }

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
