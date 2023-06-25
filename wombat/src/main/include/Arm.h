#pragma once

#include "behaviour/HasBehaviour.h"
#include "Encoder.h"
#include "Gearbox.h"
#include "PID.h"

#include <frc/DigitalInput.h>
#include <frc/simulation/DIOSim.h>
#include <units/mass.h>
#include <units/voltage.h>
#include <units/current.h>

namespace wom {
  struct ArmConfig {
    std::string path;

    /**
     * The left gearbox controlling the motion of the arm
    */
    wom::Gearbox leftGearbox;
    /**
     * The right gearbox controlling the motion of the arm
    */
    wom::Gearbox rightGearbox;
    /**
     * The spark max relative encoder for the arm
    */
    rev::SparkMaxRelativeEncoder armEncoder;
    /**
     * The Positional-Control PID config for the arm
    */
    wom::PIDConfig<units::radian, units::volt> pidConfig;
    /**
     * The Speed-Control PID config for the arm
    */
    wom::PIDConfig<units::radians_per_second, units::volt> velocityConfig;

    /**
     * The mass of the arm
    */
    units::kilogram_t armMass;
    /**
     * The mass of the load on the arm
    */
    units::kilogram_t loadMass;
    /**
     * The length of the arm
    */
    units::meter_t armLength;
    /**
     * The minimum angle of the arm
    */
    units::radian_t minAngle = 0_deg;
    /**
     * The maximum angle of the arm
    */
    units::radian_t maxAngle = 180_deg;
    /**
     * The initial angle of the arm
    */
    units::radian_t initialAngle = 0_deg;
    /**
     * The angle offset of the arm
    */
    units::radian_t angleOffset = 0_deg;
    
    
    /**
     * @brief sends all ArmConfig details to NetworkTables
     * @param table the table to send the ArmConfig details to
    */
    void WriteNT(std::shared_ptr<nt::NetworkTable> table);
  };

  enum class ArmState {
    /**
     * the armavator idle state
    */
    kIdle,
    /**
     * the state for positional control
    */
    kAngle,
    /**
     * the state for raw control
    */
    kRaw,
    /**
     * the state for velocity control
    */
    kVelocity
  };
  /**
   * @brief {WARNING : assumes leftGearbox is physically the same as rightGearbox} {ISSUE : Only works with both a left and right gearbox}
   * 
  */
  class Arm : public behaviour::HasBehaviour {
  public:
    Arm(ArmConfig config);

    void OnUpdate(units::second_t dt);

    /**
     * @brief sets state to idle
    */
    void SetIdle();
    /**
     * @brief sets the targetted angle for the angle PID. Sets state to angle (positional control)
     * @param angle the angle for the PID loop to target
    */
    void SetAngle(units::radian_t angle);
    /**
     * @brief sets the raw voltage. Sets the state to raw
     * @param voltage the raw voltage
    */
    void SetRaw(units::volt_t voltage);
    /**
     * @brief sets the targetted velocity for the velocity PID. Sets state to velocity (velocity control)
     * @param velocity the velocity for the PID loop to target
    */
    void SetVelocity(units::radians_per_second_t velocity);
    /**
     * @brief sets the maximum speed for the arm
     * @param limit 
    */
    void SetArmSpeedLimit(double limit); //units, what are they?? 

    ArmConfig &GetConfig();

    units::radian_t GetAngle() const;
    units::radians_per_second_t MaxSpeed() const;
    units::radians_per_second_t GetArmVelocity() const;
    
    bool IsStable() const;
  private:
    ArmConfig _config;
    ArmState _state = ArmState::kIdle;
    wom::PIDController<units::radian, units::volt> _pid;
    wom::PIDController<units::radians_per_second, units::volt> _velocityPID;
    
    std::shared_ptr<nt::NetworkTable> _table;

    double armLimit = 0.4;
    units::radians_per_second_t lastVelocity;

    units::volt_t _voltage{0};
  };
};