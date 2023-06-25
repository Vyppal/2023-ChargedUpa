#pragma once

#include "Gearbox.h"
#include "PID.h"
#include "behaviour/HasBehaviour.h"
#include "behaviour/Behaviour.h"

#include <networktables/NetworkTable.h>
#include <units/angular_velocity.h>
#include <units/charge.h>

#include <memory>

namespace wom {
  enum class ShooterState {
    kPID,
    kManual,
    kIdle
  };

  struct ShooterParams {
    Gearbox gearbox;
    PIDConfig<units::radians_per_second, units::volt> pid;
    units::ampere_t currentLimit;
  };
  /**
   * @brief {WARNING : Not tested in competition} Initialises a shooter logic controller
  */
  class Shooter : public behaviour::HasBehaviour {
   public:
    Shooter(std::string path, ShooterParams params);
    /**
     * @brief sets state to manual, and sets the target voltage
     * @param voltage the voltage for the motors to target
    */
    void SetManual(units::volt_t voltage);
    /**
     * @brief sets state to PID, and sets the PID's target speed
     * @param goal the rotation speed the PID should target
    */
    void SetPID(units::radians_per_second_t goal);
    /**
     * @brief sets state to idle
    */
    void SetIdle();

    void OnUpdate(units::second_t dt);
    /**
     * @brief checks if the PID loop is stable
     * @returns the boolean stability of the PID loop
    */
    bool IsStable() const;

   private:
    ShooterParams _params;
    ShooterState _state;

    units::volt_t _setpointManual{0};

    PIDController<units::radians_per_second, units::volt> _pid;

    std::shared_ptr<nt::NetworkTable> _table;
  };
  /**
   * @brief {WARNING : Not tested in competition} Initialises a shooter behaviour to stay at a constant speed
   * @param s a pointer reference to the shooter
   * @param setpoint the targetted voltage of the shooter
  */
  class ShooterConstant : public behaviour::Behaviour {
   public:
    ShooterConstant(Shooter *s, units::volt_t setpoint);

    void OnTick(units::second_t dt) override;
   private:
    Shooter *_shooter;
    units::volt_t _setpoint;
  };
  /**
   * @brief {WARNING : Not tested in competition} Initialises a shooter behaviour to accelerate/decelerate to a speed
   * @param s a pointer reference to the shooter
   * @param speed the targetted radian/second rotational speed
   * @param hold tells the behaviour to hold the speed, preventing the behaviour from ending (defaults to false)
  */
  class ShooterSpinup : public behaviour::Behaviour {
   public:
    ShooterSpinup(Shooter *s, units::radians_per_second_t speed, bool hold = false);

    void OnTick(units::second_t dt) override;
   private:
    Shooter *_shooter;
    units::radians_per_second_t _speed;
    bool _hold;
  };
}