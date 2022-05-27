#include "wresults.h"

#include "qcustomplot.h"


wResults::wResults(const QVector<QVector<double> > &data, QStringList&& header,
                   int stagesCount, bool displayTable, QWidget *parent)  :
    QWidget(parent), values (data), headerData(header),graphsCount(stagesCount),
    isDisplayedTable(displayTable)
{
    auto grid = new QGridLayout(this);
    auto btnSettingsDialog = new QPushButton(this);
    btnSettingsDialog->setText("Настройка отображения");
    size_t plotsCount = (data.size()-1)/graphsCount;
    vPlots.resize(plotsCount);

    grid->addWidget(btnSettingsDialog,0,0);


    connect(btnSettingsDialog,&QPushButton::clicked,this,&wResults::slotOpenDialog);

    //Рисование графиков
    for(size_t i = 0; i < plotsCount; i++)
    {
        //Создание объекта класса
        vPlots[i] = new QCustomPlot(this);
        grid->addWidget(vPlots[i],i+1,0);


        double minX = std::numeric_limits<double>::max();
        double minY = minX;
        double maxX = std::numeric_limits<double>::min();
        double maxY = maxX;
        //Добавление графика на полотно
        for(int g = 0; g < graphsCount; g++)
        {
            vPlots[i]->addGraph(vPlots[i]->xAxis,vPlots[i]->yAxis);
            //Подпись графика
            vPlots[i]->graph(g)->setName(QString::number(g+1)+" ступень " +headerData[i+1]);
            //Размер и цвет линии
            QPen pen;
            pen.setWidth(3);
            pen.setColor(colors[rand() % colors.size()]);
            vPlots[i]->graph(g)->setPen(pen);
            //Заполнение данными
            //Индекс, с которого начинаются данные для g графика
            // plotsCount+1 - количество графиков + ось Ох (время)
            int first_index = g*(plotsCount+1);
            vPlots[i]->graph(g)->setData(data[first_index],data[first_index+i+1]);

            //Поиск минимальных и максимальных значений для осей X,Y
            auto [graphMinX,graphMaxX] = std::minmax_element(
                    std::begin(data[first_index]),std::end(data[first_index]));
            auto [graphMinY,graphMaxY] = std::minmax_element(
                    std::begin(data[first_index+i+1]),std::end(data[first_index+i+1]));

            if(minX > *graphMinX) minX = *graphMinX;
            if(maxX < *graphMaxX) maxX = *graphMaxX;
            if(minY > *graphMinY) minY = *graphMinY;
            if(maxY < *graphMaxY) maxY = *graphMaxY;

        }

        //Подписи осей
        vPlots[i]->xAxis->setLabel(headerData[0]);
        vPlots[i]->yAxis->setLabel(headerData[i+1]);

        //Установка диапазона отображения
        vPlots[i]->xAxis->setRange(0,maxX);
        vPlots[i]->yAxis->setRange(minY, maxY);

        //Фон полотна
        vPlots[i]->setBackground(QBrush(QColor(30, 30, 30)));  

        //Цвет подписей
        vPlots[i]->xAxis->setBasePen(QPen(Qt::white));
        vPlots[i]->yAxis->setBasePen(QPen(Qt::white));
        vPlots[i]->xAxis->setLabelColor(Qt::white);
        vPlots[i]->yAxis->setLabelColor(Qt::white);

        vPlots[i]->xAxis->setTickPen(QPen(Qt::white));
        vPlots[i]->xAxis->setSubTickPen(QPen(Qt::white));
        vPlots[i]->xAxis->setTickLabelColor(Qt::white);

        vPlots[i]->yAxis->setTickPen(QPen(Qt::white));
        vPlots[i]->yAxis->setSubTickPen(QPen(Qt::white));
        vPlots[i]->yAxis->setTickLabelColor(Qt::white);

        //Перерисовка
        vPlots[i]->replot();
        //Возможноть масштабирования
        vPlots[i]->setInteraction(QCP::iRangeZoom,true);
        //Возможность перемещения
        vPlots[i]->setInteraction(QCP::iRangeDrag,true);

        vPlots[i]->legend->setVisible(true);
        vPlots[i]->legend->setFont(QFont("Helvetica", 8));
    }


    if(isDisplayedTable)
    {
        auto tabWidget = new QTabWidget(this);
        tabWidget->setMaximumWidth(450);
        grid->addWidget(tabWidget,0,0,plotsCount+1,1);
        //Добавление табличного отображения для каждой ступени
        for(int g = 0 ; g < graphsCount; g++)
        {
            auto tableView = new QTableView(tabWidget);
            tabWidget->addTab(tableView,QString::number(g+1)+" ступень");
            auto tableModel = new MyTableModel(
                        values.mid(g*(plotsCount+1),plotsCount+1),headerData,tableView);
            tableView->setModel(tableModel);
        }

        tabWidget->setVisible(false);
    }


}

void wResults::slotOpenDialog()
{
   PlotSettingsDialog plot(vPlots/*,this*/);
   plot.setWindowTitle("Настройка отображения");

   plot.exec();

}


wResults::~wResults()
{
}
