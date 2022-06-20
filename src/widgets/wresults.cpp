#include "wresults.h"

#include "qcustomplot.h"


wResults::wResults(PlotInfo&& info, QWidget *parent)  :
    QWidget(parent), plotInfo(info)
{    



    setUI();
}

void wResults::slotOpenDialog()
{
   PlotSettingsDialog plot(vPlots);
   plot.setWindowTitle("Настройка отображения");

   plot.exec();

}

void wResults::setUI()
{
    auto grid = new QGridLayout(this);
    auto btnSettingsDialog = new QPushButton("Настройка отображения",this);
    auto btnShowHideTable = new QPushButton("Показать/Скрыть таблицу",this);
    size_t plotsCount = (values.size()-1)/plotInfo.graphsCount;
    vPlots.resize(plotsCount);

    grid->addWidget(btnSettingsDialog,0,0);
    grid->addWidget(btnShowHideTable,0,1);

    connect(btnSettingsDialog,&QPushButton::clicked,this,&wResults::slotOpenDialog);

    //Рисование графиков

    qDebug()<<plotInfo.graphsCount<<" "<< plotInfo.headerData;

    // Задаем одинаковый цвет у графиков ступени
    QVector<QColor> graphColor;
    for(int g = 0; g< plotInfo.graphsCount;g++)
    {
        auto color = colors[rand() % colors.size()];
        if (!graphColor.contains(color))
            graphColor.append(color);
        else
            g--;
    }

    for(size_t i = 0; i < plotsCount; i++)
    {
        //Создание объекта класса
        vPlots[i] = new QCustomPlot(this);
        grid->addWidget(vPlots[i],i+1,0,1,2);


        // set title of plot:
        if(!plotInfo.title.isEmpty())
        {
            vPlots[i]->plotLayout()->insertRow(0);
            vPlots[i]->plotLayout()->addElement(0, 0, new QCPTextElement(vPlots[i], plotInfo.title, QFont("sans", 12, QFont::Bold)));
        }

        double minX = std::numeric_limits<double>::max();
        double minY = minX;
        double maxX = std::numeric_limits<double>::min();
        double maxY = maxX;

        //Добавление графика на полотно
        for(int g = 0; g < plotInfo.graphsCount; g++)
        {
            vPlots[i]->addGraph(vPlots[i]->xAxis,vPlots[i]->yAxis);
            //Подпись графика
            vPlots[i]->graph(g)->setName(QString::number(g+1)+" ступень " +plotInfo.headerData[i+1]);
            //Размер и цвет линии
            QPen pen;
            pen.setWidth(3);
            pen.setColor(graphColor[g]);
            vPlots[i]->graph(g)->setPen(pen);
            //Заполнение данными
            //Индекс, с которого начинаются данные для g графика
            // plotsCount+1 - количество графиков + ось Ох (время)
            int first_index = g*(plotsCount+1);
            vPlots[i]->graph(g)->setData(values[first_index],values[first_index+i+1]);

            //Поиск минимальных и максимальных значений для осей X,Y
            auto [graphMinX,graphMaxX] = std::minmax_element(
                    std::begin(values[first_index]),std::end(values[first_index]));
            auto [graphMinY,graphMaxY] = std::minmax_element(
                    std::begin(values[first_index+i+1]),std::end(values[first_index+i+1]));

            minX = std::fmin(minX,*graphMinX);
            maxX = std::fmax(maxX,*graphMaxX);
            minY = std::fmin(minY,*graphMinY);
            maxY = std::fmax(maxY,*graphMaxY);
        }

        //Подписи осей
        vPlots[i]->xAxis->setLabel(plotInfo.headerData[0]);
        vPlots[i]->yAxis->setLabel(plotInfo.headerData[i+1]);

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

        vPlots[i]->axisRect()->setRangeDrag(Qt::Horizontal);
        vPlots[i]->axisRect()->setRangeZoom(Qt::Horizontal);

        vPlots[i]->legend->setVisible(true);
        vPlots[i]->legend->setFont(QFont("Helvetica", 8));
    }


    if(plotInfo.isDisplayedTable)
    {

        auto tabWidget = new QTabWidget(this);
        tabWidget->setMaximumWidth(450);
        grid->addWidget(tabWidget,0,2,plotsCount+1,1);
        //Добавление табличного отображения для каждой ступени
        for(int g = 0 ; g < plotInfo.graphsCount; g++)
        {
            auto tableView = new QTableView(tabWidget);
            tabWidget->addTab(tableView,QString::number(g+1)+" ступень");
            auto tableModel = new MyTableModel(
                        values.mid(g*(plotsCount+1),plotsCount+1),plotInfo.headerData,tableView);
            tableView->setModel(tableModel);

        }

//        connect(btnShowHideTable,&QPushButton::clicked,
//                tabWidget,[&]()
//        {
//        qDebug()<<"tab";
//            tabWidget->setVisible(false);
//        });


     //   tabWidget->setVisible(false);
    }

}


wResults::~wResults()
{
}
