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
    QTabWidget *tab = new QTabWidget(tabWidget);
    tabWidget->addTab(tab,"Общий");

    QVector<QVector<double>> posLatLon,distance,velocity;
    for(auto &fdr: observers)
    {

        posLatLon.push_back(fdr->vposition[6]);
        posLatLon.push_back(fdr->vposition[7]);

        distance.push_back(std::move(fdr->downrage));
        distance.push_back(fdr->vposition[8]);

        velocity.push_back(fdr->countup_time);
        velocity.push_back(std::move(fdr->sum_velocity));
    }

    wResults *dist = new wResults(distance,
                                     QStringList{"Дистанция","Высота"},
                                      stagesCount,false,tab);
    wResults *latLon = new wResults(posLatLon,
                                     QStringList{"Долгота","Широта"},
                                      stagesCount,false,tab);

    wResults *vel = new wResults(velocity,
                                    QStringList{"Время","Скорость"},
                                      stagesCount,false,tab);

  //  QWidget *pos = new QWidget(tab);
    auto grid = new QGridLayout(tab);
    grid->addWidget(dist);
    grid->addWidget(latLon);
    grid->addWidget(vel);
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
    wResults *ECEF = new wResults(velECEF, QStringList{"Время","X","Y","Z"},
                                   stagesCount,tabVelocity);

    wResults *ECI = new wResults(velECI,QStringList{"Время","X","Y","Z"},
                                     stagesCount,tabVelocity);

    wResults *NED = new wResults(velNED,QStringList{"Время",
                                                     "North Velocity [m/s]",
                                                     "East Velocity [m/s]",
                                                     "Down Velocity [m/s]"},
                                  stagesCount,tabVelocity);
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


    wResults  *ECEF = new wResults(posECEF,
                                     QStringList{"Время","X","Y","Z"},
                                      stagesCount,false,tabPosition);

    wResults  *ECI = new wResults(posECI,
                                     QStringList{"Время","X","Y","Z"},
                                     stagesCount,false,tabPosition);

    wResults  *LLH = new wResults(posLLH,
                                     QStringList{"Время",
                                                 "Latitude [deg]",
                                                 "Longitude [deg]",
                                                 "Height for WGS84 [deg]"},
                                     stagesCount,false,tabPosition);


    tabPosition->addTab(LLH,"Координаты LLH");
    tabPosition->addTab(ECEF,"Координаты ECEF");
    tabPosition->addTab(ECI,"Координаты ECI");

}

void Results::plotForce()
{
    QTabWidget *tabForce = new QTabWidget(tabWidget);
    tabWidget->addTab(tabForce,"Силы");

    QVector<QVector<double>> force,forceThrust,forceAero,forceGravity;
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
    }
    wResults  *resForce = new wResults(force,QStringList{"Время","Thrust [N]]"},
                                       stagesCount,tabForce);

    wResults *resThrust = new wResults(forceThrust, QStringList{
                                            "Время","Fx-thrust [N]",
                                            "Fy-thrust [N]","Fz-thrust [N]"},
                                        stagesCount,tabForce);

    wResults *resAero = new wResults(forceAero, QStringList{
                                          "Время","Fx-aero [N]",
                                          "Fy-aero [N]","Fz-aero [N]"},
                                      stagesCount,tabForce);

    wResults *resGravity = new wResults(forceGravity,QStringList{
                                             "Время","Fx-gravity [N]",
                                             "Fy-gravity [N]","Fz-gravity [N]"},
                                         stagesCount,tabForce);
    tabForce->addTab(resForce,"Общая");
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
    wResults *resAcECI = new wResults(acECI, QStringList{"Время","X","Y","Z"},
                                       stagesCount,tabAc);

    wResults *resAcBody = new wResults(acBody,QStringList{
                                            "Время","Accx-body [m/s2]",
                                            "Accy-body [m/s2]",
                                            "Accz-body [m/s2]"},
                                        stagesCount,tabAc);
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
    wResults *resMass = new wResults(mass,QStringList{"Время","Mass [kg]",
                                                       "Propellant Mass [kg]"},
                                     stagesCount,tabWidget);
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
        atmosphere.push_back(alt);
        atmosphere.push_back(temp);
        atmosphere.push_back(denst);
        atmosphere.push_back(press);
    }

    wResults *res = new wResults(atmosphere,QStringList{"Высота","Температура",
                                                 "Плотность","Давление"},
                                 stagesCount,tabWidget);
    tabWidget->addTab(res,"Атмосфера");
}

void Results::resetData()
{
    qDebug()<<"void Results::resetData() "<<tabWidget->children();
}
