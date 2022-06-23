#include "rocket.h"

#include "jsonwrapper.h"

Rocket::Rocket() {
    length_thrust = 0.0;
    diameter = 0.0;
    area = 0.0;
    length = 0.0;
    inertia_tensor << 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0;

    cant_angle_fin = 0.0;

    dynamic_pressure = 0.0;

    quaternion_dot << 0.0, 0.0, 0.0, 0.0;
    angular_velocity << 0.0, 0.0, 0.0;
    angular_acceleration << 0.0, 0.0, 0.0;
    angle_of_attack = 0.0;
    sideslip_angle = 0.0;

}

Rocket::Rocket(JsonWrapper& json)
{

    auto jsEngine = json.getSubItem("Engine");
    engine = Engine::create(jsEngine);

    auto js = json.getSubItem("Stage");

    diameter = js.getDouble("Diameter [m]") ;
    area = 0.25 * std::pow(diameter, 2) * Constants::pi;
    length = js.getDouble("Length [m]") ;
    mass.inert = js.getSubItem("Mass").getDouble("Inert [kg]");
    mass.propellant = js.getSubItem("Mass").getDouble("Propellant [kg]");

    if (js.getBool("Enable Program Attitude")) {
        auto attitude_program = LoadCsvLog(js.getSubItem("Program Attitude File").getString("Program Attitude File Path").toStdString());
        auto azi = InterpolateParameter(attitude_program[0], attitude_program[1], "same");
        auto elv = InterpolateParameter(attitude_program[0], attitude_program[2], "same");
        auto roll = InterpolateParameter(attitude_program[0], attitude_program[3], "same");
        setAttitudeProgram(azi, elv, roll);
    }

    setLengthCG(InterpolateParameter(js.getSubItem("Constant X-C.G.").getDouble("Constant X-C.G. from BodyTail [m]")));

    auto jc_const = js.getSubItem("Constant M.I.");
    auto yaw = InterpolateParameter(jc_const.getDouble("Yaw Axis [kg-m2]"));
    auto pitch = InterpolateParameter(jc_const.getDouble("Pitch Axis [kg-m2]"));
    auto roll = InterpolateParameter(jc_const.getDouble("Roll Axis [kg-m2]"));
    setInertiaTensor(roll, pitch, yaw);

    setLengthCP(InterpolateParameter( js.getSubItem("Constant X-C.P.").getDouble("Constant X-C.P. from BodyTail [m]")));
    length_thrust = js.getDouble("X-ThrustLoadingPoint from BodyTail [m]");
    setCA(InterpolateParameter(js.getSubItem("Constant CA").getDouble("Constant CA [-]")),
                  InterpolateParameter(js.getSubItem("Constant CA").getDouble("Constant BurnOut CA [-]")));
    setCNa(InterpolateParameter(js.getSubItem("Constant CNa").getDouble("Constant CNa [1/rad]")));
    cant_angle_fin =  js.getDouble("Fin Cant Angle [deg]") / 180.0 * Constants::pi;
    setCld(InterpolateParameter(js.getSubItem("Constant Cld").getDouble("Constant Cld [1/rad]")));
    setClp(InterpolateParameter(js.getSubItem("Constant Clp").getDouble("Constant Clp [-]")));
    setCmq(InterpolateParameter(js.getSubItem("Constant Cmq").getDouble("Constant Cmq [-]")));
    setCnr(InterpolateParameter(js.getSubItem("Constant Cnr").getDouble("Constant Cnr [-]")));

    setCdSParachute(1.5);

}

// Parameter Setter //////////////////
////////////////////////////////////////////////////////

// void Rocket::setEngine(Engine engine) {
//     this->engine = engine;
// };

void Rocket::setLengthCG(const InterpolateParameter length_CG) {
    this->length_CG_src = length_CG;
};

void Rocket::setLengthCP(const InterpolateParameter length_CP) {
    this->length_CP_src = length_CP;
};

void Rocket::setCA(const InterpolateParameter CA) {
    this->CA_src = CA;
};

void Rocket::setCA(const InterpolateParameter CA, const InterpolateParameter CA_burnout) {
    this->CA_src = CA;
    this->CA_burnout_src = CA_burnout;
};

void Rocket::setCNa(const InterpolateParameter CNa) {
    this->CNa_src = CNa;
};

void Rocket::setCld(const InterpolateParameter Cld) {
    this->Cld_src = Cld;
};

void Rocket::setClp(const InterpolateParameter Clp) {
    this->Clp_src = Clp;
};

void Rocket::setCmq(const InterpolateParameter Cmq) {
    this->Cmq_src = Cmq;
};

void Rocket::setCnr(const InterpolateParameter Cnr) {
    this->Cnr_src = Cnr;
};

void Rocket::setInertiaTensor(const InterpolateParameter MOI_xx, const InterpolateParameter MOI_yy, const InterpolateParameter MOI_zz) {
    this->inertia_moment_xx_src = MOI_xx;
    this->inertia_moment_yy_src = MOI_yy;
    this->inertia_moment_zz_src = MOI_zz;
};

void Rocket::setAttitudeProgram(const InterpolateParameter azimuth, const InterpolateParameter elevation, const InterpolateParameter roll) {
    this->azimuth_program_src = azimuth;
    this->elevation_program_src = elevation;
    this->roll_program_src = roll;
};

void Rocket::setCdSParachute(const double CdS_first) {
    CdS_parachute_src.push_back(CdS_first);
};

void Rocket::setCdSParachute(const double CdS_first, const double CdS_second) {
    CdS_parachute_src.push_back(CdS_first);
    CdS_parachute_src.push_back(CdS_second);
};




// Parameter Getter //////////////////

double Rocket::calcOverload(double lambda,double separation_height,double angle )
{
    angle = 0; // tg(Vx/Vy)
    double n_max = -((15*angle)/std::exp(1.0))*std::sqrt(separation_height*lambda);
    return n_max;;
}

double Rocket::calcHeatFlow(double mass,double lambda,double separation_height,
                            double density,double velocity,double velocity_kr,
                            double radius)
{
    lambda = 0;
    velocity_kr = 0;
    radius = 0;
    double y = ((CA*area)/mass)*sqrt((separation_height*density)/lambda);
    double x = std::log2(velocity/velocity_kr);
    double e = std::exp(1.0);
    double q_max = (105*std::pow(y,0.8)*std::pow(e,-3.19*x))/
            (std::pow(radius,0.2)*std::pow((CA*area/mass),0.8));
    return q_max;
}


double Rocket::getLengthCG() {
    if (engine.burning) {
        this->length_CG = length_CG_src(burn_clock.countup_time);
    }
        
    return this->length_CG;
};

double Rocket::getLengthCP(const double mach_number) {
    this->length_CP = length_CP_src(mach_number);
    return this->length_CP;
};

double Rocket::getCA(const double mach_number) {
    if (engine.burning) {
        this->CA = CA_src(mach_number);
    } else {
        this->CA = CA_burnout_src(mach_number);
    }
    return this->CA;
};

double Rocket::getCNa(const double mach_number) {
    this->CNa = CNa_src(mach_number);
    return this->CNa;
};

double Rocket::getCld(const double mach_number) {
    this->Cld = Cld_src(mach_number);
    return this->Cld;
};

double Rocket::getClp(const double mach_number) {
    this->Clp = Clp_src(mach_number);
    if (this->Clp > 0.0) {
        this->Clp *= -1.0;
    }
    return this->Clp;
};

double Rocket::getCmq(const double mach_number) {
    this->Cmq = Cmq_src(mach_number);
    if (this->Cmq > 0.0) {
        this->Cmq *= -1.0;
    }
    return this->Cmq;
};

double Rocket::getCnr(const double mach_number) {
    this->Cnr = Cnr_src(mach_number);
    if (this->Cnr > 0.0) {
        this->Cnr *= -1.0;
    }
    return this->Cnr;
};


Eigen::Vector3d Rocket::getThrust(const double air_pressure) {
    engine.Update(burn_clock.countup_time, air_pressure);

    Eigen::Vector3d thrust;
    if (engine.burning) {
        Eigen::Vector3d gimbal_angle(std::cos(engine.gimbal_angle_y_axis) * std::cos(engine.gimbal_angle_y_axis),
                                    std::sin(engine.gimbal_angle_z_axis),
                                    -std::sin(engine.gimbal_angle_y_axis));
        thrust << engine.thrust * gimbal_angle.array();
    } else {
        thrust << 0.0, 0.0, 0.0;
    }

    return thrust;
};

Eigen::Matrix3d Rocket::getInertiaTensor() {
    if (engine.burning) {
        Eigen::Matrix3d tensor;
        double t = burn_clock.countup_time;
        tensor << inertia_moment_xx_src(t), 0.0, 0.0,
                0.0, inertia_moment_yy_src(t), 0.0,
                0.0, 0.0, inertia_moment_zz_src(t);
        this->inertia_tensor = tensor;
    }
    return this->inertia_tensor;
};

/// @todo УБРАТЬ
Eigen::Vector3d Rocket::getAttitude() {

    Eigen::Vector3d attitude;
    double t = burn_clock.countup_time;
    attitude << azimuth_program_src(t), elevation_program_src(t), roll_program_src(t);
    return attitude;
};


// SOE Handler
void Rocket::IgnitionEngine(QDateTime UTC_init, double countup_time_init) {
    burn_clock = SequenceClock(UTC_init, countup_time_init);
    engine.Ignittion();
};

void Rocket::CutoffEngine() {
    engine.Cutoff();
};


void Rocket::JettsonFairing(const double mass_fairing) {
    mass.inert = mass.inert - mass_fairing;
    if (mass.inert <= 0.0) mass.inert = 1.0;
};

void Rocket::SeparateUpperStage(const double mass_upper_stage) {
    mass.inert = mass.inert - mass_upper_stage;
    if (mass.inert <= 0.0) mass.inert = 1.0;
};

void Rocket::OpenParachute() {

    for (size_t i=0; i < CdS_parachute_src.size(); ++i) {
        CdS_parachute += CdS_parachute_src[i];
        CdS_parachute_src[i] = 0; /// @badcode
    }
};


