#include "results.h"

Results::Results(QWidget *parent) :
    QWidget(parent)
{
    auto grid = new QGridLayout(this);
    tabWidget = new QTabWidget(this);
    grid->addWidget(tabWidget, 0, 0, 1, 1);
}

Results::~Results()
{
}

void Results::SetObservers(QVector<FlightObserver*>obs)
{
    resetData();
    observers = obs; 
    stagesCount = observers.size();
}

void Results::paintResults()
{
    plot();
    plotPosition();
    plotVelocity();
    plotForce();
    plotAcceleration();
    plotMass();
    plotAmtosphere();
}

void Results::plot()
{
    QWidget *w = new QWidget(tabWidget);
    tabWidget->addTab(w,"Общий");

    QVector<QVector<double>> posLatLon,distance,velocity,mach;
    for(auto &fdr: observers)
    {

        posLatLon.push_back(fdr->vposition[6]);
        posLatLon.push_back(fdr->vposition[7]);

        distance.push_back(std::move(fdr->downrage));
        distance.push_back(fdr->vposition[8]);

        velocity.push_back(fdr->countup_time);
        velocity.push_back(std::move(fdr->sum_velocity));

        mach.push_back(fdr->countup_time);
        mach.push_back(std::move(fdr->mach_number));
    }

    wResults *dist = new wResults(
                PlotInfo{distance, stagesCount,QStringList{"Дистанция","Высота"},
                         {"Расстояние от точки старта"},false},w);
    wResults *latLon = new wResults(
                PlotInfo{posLatLon,stagesCount,QStringList{"Долгота","Широта"},
                         {"Долгота от широты"},false},w);

    wResults *vel = new wResults(
                PlotInfo{velocity,stagesCount,
                         QStringList{"Время","Скорость"},{"Вектор скорости"},false},w);

    wResults *mach_number = new wResults(
                PlotInfo{mach, stagesCount,QStringList{"Время","Число Маха"},
                         {"Число Маха от времени"},false},w);

  //  QWidget *pos = new QWidget(tab);
    auto grid = new QGridLayout(w);
    grid->addWidget(dist,0,0);
    grid->addWidget(latLon,0,1);
    grid->addWidget(vel);
    grid->addWidget(mach_number);


}

void Results::plotVelocity()
{
    QTabWidget *tabVelocity = new QTabWidget(tabWidget);
    tabWidget->addTab(tabVelocity,"Скорость");

    QVector<QVector<double>> velECEF,velECI,velNED;
    for(auto &fdr: observers)
    {
        velECEF.push_back(fdr->countup_time);
        velECEF.push_back(std::move(fdr->vvelocity[0]));
        velECEF.push_back(std::move(fdr->vvelocity[1]));
        velECEF.push_back(std::move(fdr->vvelocity[2]));

        velECI.push_back(fdr->countup_time);
        velECI.push_back(std::move(fdr->vvelocity[3]));
        velECI.push_back(std::move(fdr->vvelocity[4]));
        velECI.push_back(std::move(fdr->vvelocity[5]));

        velNED.push_back(fdr->countup_time);
        velNED.push_back(std::move(fdr->vvelocity[6]));
        velNED.push_back(std::move(fdr->vvelocity[7]));
        velNED.push_back(std::move(fdr->vvelocity[8]));
    }

    QStringList title{"Скорость по координате X","Скорость по координате Y",
                      "Скорость по координате Z" };
    wResults *ECEF = new wResults(
                PlotInfo{velECEF,stagesCount,
                         QStringList{"Время","X","Y","Z"},title},tabVelocity);

    wResults *ECI = new wResults(
                PlotInfo{velECI,stagesCount,
                         QStringList{"Время","X","Y","Z"},title},tabVelocity);

    wResults *NED = new wResults(
                PlotInfo{velNED,stagesCount,
                         QStringList{"Время",
                                     "North Velocity [m/s]",
                                     "East Velocity [m/s]",
                                     "Down Velocity [m/s]"},title},tabVelocity);

    tabVelocity->addTab(ECEF,"Скорость ECEF");
    tabVelocity->addTab(ECI,"Скорость ECI");
    tabVelocity->addTab(NED,"NED");
}

void Results::plotPosition()
{
    QTabWidget *tabPosition = new QTabWidget(tabWidget);
    tabWidget->addTab(tabPosition,"Координаты");

    QVector<QVector<double>> posECEF,posECI,posLLH;
    for(auto &fdr: observers)
    {
        posECEF.push_back(fdr->countup_time);
        posECEF.push_back(std::move(fdr->vposition[0]));
        posECEF.push_back(std::move(fdr->vposition[1]));
        posECEF.push_back(std::move(fdr->vposition[2]));

        posECI.push_back(fdr->countup_time);
        posECI.push_back(std::move(fdr->vposition[3]));
        posECI.push_back(std::move(fdr->vposition[4]));
        posECI.push_back(std::move(fdr->vposition[5]));

        posLLH.push_back(fdr->countup_time);
        posLLH.push_back(fdr->vposition[6]);
        posLLH.push_back(fdr->vposition[7]);
        posLLH.push_back(fdr->vposition[8]);
    }


    QStringList title{"Позиция по координате X","Позиция по координате Y",
                      "Позиция по координате Z" };
    wResults  *ECEF = new wResults(
                PlotInfo{posECEF,stagesCount,
                         QStringList{"Время","X","Y","Z"},title},tabPosition);

    wResults  *ECI = new wResults(
                PlotInfo{posECI,stagesCount,
                         QStringList{"Время","X","Y","Z"},title},tabPosition);

    wResults  *LLH = new wResults(
                PlotInfo{posLLH, stagesCount,
                         QStringList{"Время",
                                   "Latitude [deg]",
                                   "Longitude [deg]",
                                   "Height for WGS84 [deg]"},
                         { "Изменение широты","Изменение долготы",
                           "Изменение высоты"  }},tabPosition);

    tabPosition->addTab(LLH,"Координаты LLH");
    tabPosition->addTab(ECEF,"Координаты ECEF");
    tabPosition->addTab(ECI,"Координаты ECI");

}

void Results::plotForce()
{
    QTabWidget *tabForce = new QTabWidget(tabWidget);
    tabWidget->addTab(tabForce,"Силы");

    QVector<QVector<double>> force,forceThrust,forceAero,forceGravity,sum;
    for(auto &fdr: observers)
    {
        force.push_back(fdr->countup_time);
        force.push_back(std::move(fdr->thrust));

        forceThrust.push_back(fdr->countup_time);
        forceThrust.push_back(std::move(fdr->vforce[0]));
        forceThrust.push_back(std::move(fdr->vforce[1]));
        forceThrust.push_back(std::move(fdr->vforce[2]));

        forceAero.push_back(fdr->countup_time);
        forceAero.push_back(std::move(fdr->vforce[3]));
        forceAero.push_back(std::move(fdr->vforce[4]));
        forceAero.push_back(std::move(fdr->vforce[5]));

        forceGravity.push_back(fdr->countup_time);
        forceGravity.push_back(std::move(fdr->vforce[6]));
        forceGravity.push_back(std::move(fdr->vforce[7]));
        forceGravity.push_back(std::move(fdr->vforce[8]));

        sum.push_back(fdr->countup_time);
        sum.push_back(std::move(fdr->vforce[9]));
        sum.push_back(std::move(fdr->vforce[10]));
        sum.push_back(std::move(fdr->vforce[11]));
    }
    wResults  *resForce = new wResults(
                PlotInfo{force,stagesCount,
                         QStringList{"Время","Thrust [N]]"},{"Тяга двигателя"}},tabForce);

    wResults *sumForce = new wResults(
                PlotInfo{sum,stagesCount,
                         QStringList{"Время","Fx [N]", "Fx [N]", "Fz [N]"},
                         {"Сила по координате Х",
                          "Сила по координате Y",
                          "Сила по координате Z"}},tabForce);

    wResults *resThrust = new wResults(
                PlotInfo{forceThrust,stagesCount,
                         QStringList{"Время","Fx-thrust [N]", "Fy-thrust [N]",
                                     "Fz-thrust [N]"},
                         {"Тяга двигателя по координате Х",
                          "Тяга двигателя по координате Y",
                          "Тяга двигателя по координате Z"}},tabForce);

    wResults *resAero = new wResults(
                PlotInfo{forceAero,stagesCount,
                         QStringList{"Время","Fx-aero [N]", "Fy-aero [N]",
                                     "Fz-aero [N]"},
                         {"Аэродинамические силы по координате Х",
                          "Аэродинамические силы по координате Y",
                          "Аэродинамические силы по координате Z"}},tabForce);

    wResults *resGravity = new wResults(
                PlotInfo{forceGravity,stagesCount,
                         QStringList{"Время","Fx-gravity [N]",
                                     "Fy-gravity [N]","Fz-gravity [N]"},
                         {"Гравитационные силы по координате Х",
                          "Гравитационные силы по координате Y",
                          "Гравитационные силы по координате Z"}},tabForce);

    tabForce->addTab(resForce,"Тяга двигателя");
    tabForce->addTab(sumForce,"Сумма сил");
    tabForce->addTab(resThrust,"Сила тяги");
    tabForce->addTab(resAero,"Аэродинамические силы");
    tabForce->addTab(resGravity,"Вес");
}

void Results::plotAcceleration()
{
    QTabWidget *tabAc = new QTabWidget(tabWidget);
    tabWidget->addTab(tabAc,"Ускорение");

    QVector<QVector<double>> acECI,acBody;
    for(auto &fdr: observers)
    {
        acECI.push_back(fdr->countup_time);
        acECI.push_back(std::move(fdr->vacceleration[0]));
        acECI.push_back(std::move(fdr->vacceleration[1]));
        acECI.push_back(std::move(fdr->vacceleration[2]));

        acBody.push_back(fdr->countup_time);
        acBody.push_back(std::move(fdr->vacceleration[3]));
        acBody.push_back(std::move(fdr->vacceleration[4]));
        acBody.push_back(std::move(fdr->vacceleration[5]));
    }
    wResults *resAcECI = new wResults(
                PlotInfo{acECI,stagesCount,
                         QStringList{"Время","X","Y","Z"},
                         {"Ускорение по координате Х",
                          "Ускорение по  координате Y",
                          "Ускорение по  координате Z"}},tabAc);

    wResults *resAcBody = new wResults(
                PlotInfo{acBody,stagesCount,
                         QStringList{"Время","Accx-body [m/s2]",
                                     "Accy-body [m/s2]",
                                     "Accz-body [m/s2]"},
                         {"Ускорение по координате Х",
                          "Ускорение по  координате Y",
                          "Ускорение по  координате Z"}},tabAc);
    tabAc->addTab(resAcECI,"ECI");
    tabAc->addTab(resAcBody,"body");

}

void Results::plotMass()
{

    QVector<QVector<double>> mass;
    for(auto &fdr: observers)
    {
        mass.push_back(fdr->countup_time);
        mass.push_back(std::move(fdr->mass));
        mass.push_back(std::move(fdr->mass_prop));
    }
    wResults *resMass = new wResults(
                PlotInfo{mass,stagesCount,QStringList{"Время","Mass [kg]",
                                                       "Propellant Mass [kg]"},
                         {"Изменение общей массы ступени",
                          "Изменение массы топлива"}},tabWidget);
    tabWidget->addTab(resMass,"Масса");

}

void Results::plotAmtosphere()
{
    Environment env;
    QVector<QVector<double>> atmosphere;
    QVector<double> temp,alt,denst,press;
    for(auto &fdr: observers)
    {
        for(int i = 0; i < fdr->countup_time.size();i++)
        {
             env.updateAltitude(fdr->position[i].LLH[2]);
             alt.push_back(fdr->position[i].LLH[2]);
             temp.push_back(env.atmosphere.getTemperature());
             denst.push_back(env.atmosphere.getDensity());
             press.push_back(env.atmosphere.getPressure());
        }
        atmosphere.push_back(std::move(alt));
        atmosphere.push_back(std::move(temp));
        atmosphere.push_back(std::move(denst));
        atmosphere.push_back(std::move(press));
    }

    wResults *res = new wResults(
                PlotInfo{atmosphere,stagesCount,
                         QStringList{"Высота","Температура", "Плотность","Давление"},
                         {"Зависимость температуры от высоты",
                          "Зависимость плотности от высоты",
                          "Зависимость давления от высоты"}},tabWidget);
    tabWidget->addTab(res,"Атмосфера");
}

void Results::resetData()
{

}
