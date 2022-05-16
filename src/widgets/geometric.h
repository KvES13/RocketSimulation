#ifndef GEOMETRIC_H
#define GEOMETRIC_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class Geometric;
}

class Geometric : public QWidget
{
    Q_OBJECT

public:
    explicit Geometric(QWidget *parent = nullptr);
    ~Geometric();

private slots:
  //  void slotTimerAlarm();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Geometric *ui;
    QList<QPixmap> img;
    int index = 0;;
};

#endif // GEOMETRIC_H
