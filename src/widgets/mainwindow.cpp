#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDate date(2022, 7, 8);
    QDate leftDay = QDate::currentDate();
    int days = leftDay.daysTo(date);
    setWindowTitle("RocketSimulation "+QString::number(days));

    connect(ui->FSettings,SIGNAL(startSim(std::vector<RocketStage>&,Environment*)),
            this, SLOT(uptageWidgets(std::vector<RocketStage>&,Environment*)));

//    connect(ui->Sim,&wSimulation::BackToSettings,ui->stackedWidget,
//            ([this](){ui->stackedWidget->setCurrentIndex(0);}));

    connect(ui->Sim,&wSimulation::finish,ui->Res,&Results::paintResults);


    QStringList names = {"Настройки","Симуляция","Результаты","Геометрическое ядро"};
    for(int i = 0; i < names.size();++i)
    {
        QAction *action = new QAction(QIcon(QString(":/icons/icon%1").arg(i)),
                                      names[i],ui->widget);
        ui->widget->addAction(action);
        connect(action,&QAction::toggled,[this,i](){ui->stackedWidget->setCurrentIndex(i);});
    }
    ui->widget->activateFirstAction();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uptageWidgets(std::vector<RocketStage> &stages,Environment* env)
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->Sim->init(stages,env);
    ui->Res->SetFilePath(QString::fromStdString(stages[0].fdr.filePath),&ui->Sim->stageVector[0].fdr);
}




