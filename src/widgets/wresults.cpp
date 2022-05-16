#include "wresults.h"

#include "qcustomplot.h"

wResults::wResults(const QVector<QVector<double> > &data,
                   QStringList&& header, QWidget *parent)  :
    QWidget(parent), values (data), headerData(header)
{
    grid = new QGridLayout(this);
    btnSettingsDialog = new QPushButton(this);
    btnSettingsDialog->setText("Настройка отображения");
    vPlots.resize(data.size()-1);
    size_t plotsCount = data.size()-1;
    tableView = new QTableView(this);
    grid->addWidget(tableView,0,0,4,1);
    grid->addWidget(btnSettingsDialog,0,1);


    connect(btnSettingsDialog,&QPushButton::clicked,this,&wResults::slotOpenDialog);

//connect(ui->FSettings,SIGNAL(startSim(std::vector<RocketStage>&,Environment&)),
//        this, SLOT(uptageWidgets(std::vector<RocketStage>&,Environment&)));

    //Рисование графиков
    for(int i = 0; i < data.size()-1; i++)
    {
        //Создание объекта класса
        vPlots[i] = new QCustomPlot(this);
        grid->addWidget(vPlots[i],i+1,1);
        //Добавление графика на полотно
        vPlots[i]->addGraph(vPlots[i]->xAxis,vPlots[i]->yAxis);
        vPlots[i]->graph(0)->setName(headerData[i+1]);
        //Цвет графика
        vPlots[i]->graph(0)->setPen(QPen(colors[i]));
        //Заполнение данными
        vPlots[i]->graph(0)->setData(data[0],data[i+1]);

        //Подписи осей
        vPlots[i]->xAxis->setLabel(headerData[0]);
        vPlots[i]->yAxis->setLabel(headerData[i+1]);

        //Установка диапазона отображения
        vPlots[i]->xAxis->setRange(
                    0,*std::max_element(std::begin(data[0]),std::end(data[0])));

        auto [min,max] = std::minmax_element(std::begin(data[i+1]),std::end(data[i+1]));
        vPlots[i]->yAxis->setRange(*min, *max);


        //Перерисовка
        vPlots[i]->replot();
        //Возможноть масштабирования
        vPlots[i]->setInteraction(QCP::iRangeZoom,true);
        //Возможность перемещения
        vPlots[i]->setInteraction(QCP::iRangeDrag,true);

        vPlots[i]->legend->setVisible(true);
        vPlots[i]->legend->setFont(QFont("Helvetica", 9));
    }

   tableView->setMaximumWidth(plotsCount*150);


   tableModel = new MyTableModel(values,headerData);
   tableView->setModel(tableModel);


}

void wResults::slotOpenDialog()
{
   PlotSettingsDialog plot(vPlots,this);
   plot.setWindowTitle("Настройка отображения");

   plot.exec();



}


wResults::~wResults()
{
    delete tableModel;
}
