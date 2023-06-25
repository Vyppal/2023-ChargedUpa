#pragma once 

#include "Gearbox.h"
#include "PID.h"
#include "behaviour/HasBehaviour.h"
#include "behaviour/Behaviour.h"
#include <units/length.h>
#include <units/mass.h>

#include <frc/DigitalInput.h>
#include <frc/simulation/DIOSim.h>
#include <frc/simulation/ElevatorSim.h>
#include <networktables/NetworkTable.h>

#include <memory>

namespace wom {
  enum class ElevatorState {
    /**
     * The idle state for the elevator
    */
    kIdle,
    /**
     * The positional PID controlled state
    */
    kPID,
    /**
     * The manual controlled state
    */
    kManual,
    /**
     * The velocity PID controlled state
    */
    kVelocity
  };

  struct ElevatorConfig {
    // The NetworkTable path
    std::string path;
    // The left gearbox
    wom::Gearbox leftGearbox;
    // The right gearbox
    wom::Gearbox rightGearbox;
    // The elevator's encoder
    rev::SparkMaxRelativeEncoder elevatorEncoder;
    // The upper limit switch
    frc::DigitalInput *topSensor;
    // The lower limit switch
    frc::DigitalInput *bottomSensor;
    // The radius of the elevator's spool
    units::meter_t radius;
    // The elevator's moving mass
    units::kilogram_t mass;
    // The maximum height of the elevator in meters
    units::meter_t maxHeight;
    // The minimum height of the elevator in meters
    units::meter_t minHeight;
    // The starting height of the elevator in meters
    units::meter_t initialHeight;
    // The position-controlled PID controller for the elevator
    PIDConfig<units::meter, units::volt> pid;
    // The velocity-controlled PID controller for the elevator
    PIDConfig<units::meters_per_second, units::volt> velocityPID;

    void WriteNT(std::shared_ptr<nt::NetworkTable> table);
  };


  class Elevator : public behaviour::HasBehaviour {
   public: 
    /**
    * @brief A behaviour class for elevator control
    * @param params an instance of an elevator config
    */
    Elevator(ElevatorConfig params);

    void OnUpdate(units::second_t dt);

    /**
     * @brief Sets state to manual, and sets the voltage setpoint
     * @param voltage the value of the voltage setpoint
    */
    void SetManual(units::volt_t voltage);
    /**
     * @brief Sets state to positional PID, and sets targetted height
     * @param height the height (in meters) the PID loop targets 
    */
    void SetPID(units::meter_t height);
    /**
     * @brief Sets state to idle
    */
    void SetIdle();
    /**
     * @brief Unused, unimplemented raw state
    */
    void SetRaw();

    /**
     * @brief Sets state to velocity PID, and sets targetted velocity
     * @param velocity the targetted velocity value (in meters per second)
    */

    void SetVelocity(units::meters_per_second_t velocity);
    /**
     * @brief Unused, unimplemented get raw
    */
    units::volt_t GetRaw();

    /**
     * @brief Gets the position of the encoder. Internal values will need to be changed for different robots
    */
    double GetElevatorEncoderPos();
    /**
     * @brief Sets the speed limit of the elevator
     * @param limit the new speed limit of the elevator
    */
    void SetElevatorSpeedLimit(double limit);
    /**
     * @brief Gets a reference to the behaviour's config
    */
    ElevatorConfig &GetConfig();
    /**
     * @brief Checks if the PID is stable (is at the value it's meant to be at)
    */
    bool IsStable() const;
    /**
     * @brief Gets the elevator's state
    */
    ElevatorState GetState() const;
    /**
     * @brief Gets the elevator's height in meters
    */
    units::meter_t GetHeight() const;
    /**
     * @brief Gets the maximum speed of the elevator in meters per second
    */
    units::meters_per_second_t MaxSpeed() const;
    /**
     * @brief Get's the current velocity of the elevator in meters per second
    */
    units::meters_per_second_t GetElevatorVelocity() const;
  
   private:
    units::volt_t _setpointManual{0};

    ElevatorConfig _config;
    ElevatorState _state;
    double speedLimit = 0.5;

    units::meters_per_second_t _velocity;

    PIDController<units::meter, units::volt> _pid;
    PIDController<units::meters_per_second, units::volt> _velocityPID;

    std::shared_ptr<nt::NetworkTable> _table;
  };
};