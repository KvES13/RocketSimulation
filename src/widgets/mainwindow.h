#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "flightsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void uptageWidgets(std::vector<RocketStage>& stages, Environment *env);

private:
    void addSideBar();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
