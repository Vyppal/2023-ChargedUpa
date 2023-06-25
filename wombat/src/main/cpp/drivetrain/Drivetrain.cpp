#include "drivetrain/Drivetrain.h"

using namespace wom;

Drivetrain::Drivetrain(std::string path, DrivetrainConfig config)
  : _config(config), _kinematics(config.trackWidth), 
    _leftVelocityController(path + "/pid/left", config.velocityPID),
    _rightVelocityController(path + "/pid/right", config.velocityPID) {}

void Drivetrain::OnUpdate(units::second_t dt) {
  units::volt_t leftVoltage{0};
  units::volt_t rightVoltage{0};

  auto wheelSpeeds = _kinematics.ToWheelSpeeds(_speed);
  wheelSpeeds.Desaturate((_config.leftDrive.motor.freeSpeed.value() * _config.wheelRadius.value()) * 1_mps);

  switch (_state) {
    case DrivetrainState::kManual:
      leftVoltage = _leftManualSetpoint;
      rightVoltage = _rightManualSetpoint;
      break;
    case DrivetrainState::kIdle:
      leftVoltage = 0_V;
      rightVoltage = 0_V;
      break;
    case DrivetrainState::kRaw:
      leftVoltage = _leftRawSetpoint;
      rightVoltage = _rightRawSetpoint;
      break;
    case DrivetrainState::kVelocity:
      {
        _leftVelocityController.SetSetpoint(wheelSpeeds.left);
        _rightVelocityController.SetSetpoint(wheelSpeeds.right);
        auto feedforwardLeft = _config.leftDrive.motor.Voltage(0_Nm, units::radians_per_second_t{(_leftVelocityController.GetSetpoint() / _config.wheelRadius).value()});
        auto feedforwardRight = _config.rightDrive.motor.Voltage(0_Nm, units::radians_per_second_t{(_rightVelocityController.GetSetpoint() / _config.wheelRadius).value()});
        leftVoltage = _leftVelocityController.Calculate(GetLeftSpeed(), dt, feedforwardLeft);
        rightVoltage = _rightVelocityController.Calculate(GetRightSpeed(), dt, feedforwardRight);
      }
      break;
    case DrivetrainState::kPose:

      break;
  }

  // [{REMOVED}] torque limits and max voltages for both left and right side, and associated commented lines

  units::newton_meter_t torque_limit = _config.leftDrive.motor.Torque(_config.currentLimit);
  units::volt_t max_volt = _config.leftDrive.motor.Voltage(torque_limit, units::math::max(
    units::math::abs(_config.leftDrive.encoder->GetEncoderAngularVelocity()),
    units::math::abs(_config.rightDrive.encoder->GetEncoderAngularVelocity())
  ));

  units::volt_t real_max_volt = units::math::max(units::math::abs(leftVoltage), units::math::abs(rightVoltage));

  if (real_max_volt > max_volt) {
    rightVoltage = rightVoltage / real_max_volt * max_volt;
    leftVoltage = leftVoltage / real_max_volt * max_volt;
  }

  _config.leftDrive.transmission->SetVoltage(leftVoltage);
  _config.rightDrive.transmission->SetVoltage(rightVoltage);

}

void Drivetrain::SetRawVoltage(units::volt_t left, units::volt_t right) {
  _state = DrivetrainState::kRaw;
  _leftRawSetpoint = left;
  _rightRawSetpoint = right;
}

void Drivetrain::SetManual(double leftPower, double rightPower) {
  _state = DrivetrainState::kManual;

  _leftRawSetpoint = leftPower * 12_V;
  _rightRawSetpoint = rightPower * 12_V;
}

void Drivetrain::SetIdle() {
  _state = DrivetrainState::kIdle;
}

void Drivetrain::SetVelocity(frc::ChassisSpeeds speeds) {
  _state = DrivetrainState::kVelocity;
  _speed = speeds;
}

void Drivetrain::SetTargetPose(frc::Pose2d pose) {
  _state = DrivetrainState::kPose;
  _targetPose = pose;
}

units::meter_t Drivetrain::GetLeftDistance() const {
  return _config.leftDrive.encoder->GetEncoderPosition().value() * _config.wheelRadius;
}

units::meter_t Drivetrain::GetRightDistance() const {
  return _config.rightDrive.encoder->GetEncoderPosition().value() * _config.wheelRadius;
}

units::meters_per_second_t Drivetrain::GetLeftSpeed() const {
  return units::meters_per_second_t{_config.leftDrive.encoder->GetEncoderAngularVelocity().value() * _config.wheelRadius.value()};
}

units::meters_per_second_t Drivetrain::GetRightSpeed() const {
  return units::meters_per_second_t{_config.rightDrive.encoder->GetEncoderAngularVelocity().value() * _config.wheelRadius.value()};
}

// Drivetrain Behaviours

DrivetrainDriveDistance::DrivetrainDriveDistance(Drivetrain *d, units::meter_t length)
  : _drivetrain(d),
    _distancePID("drivetrain/behaviours/DrivetrainDriveDistance/pid/distance", d->GetConfig().distancePID),
    _anglePID("drivetrain/behaviours/DrivetrainDriveDistance/pid/angle", d->GetConfig().anglePID) {
  Controls(d);
  _distancePID.SetSetpoint(length);
  _anglePID.SetSetpoint(0_deg);
  _anglePID.SetWrap(360_deg);
}

units::meter_t DrivetrainDriveDistance::GetDistance() const {
  return (_drivetrain->GetLeftDistance() + _drivetrain->GetRightDistance()) / 2;
}

units::degree_t DrivetrainDriveDistance::GetAngle() const {
  return _drivetrain->GetConfig().gyro->GetRotation2d().Degrees();
}

void DrivetrainDriveDistance::OnStart() {
  _start_distance = GetDistance();
  _start_angle = GetAngle();
}

void DrivetrainDriveDistance::OnTick(units::second_t dt) {
  auto fwd_speed = _distancePID.Calculate(GetDistance() - _start_distance, dt);
  units::radian_t target_angle = 0_deg;
  
  _anglePID.SetSetpoint(target_angle);
  auto ang_speed = _anglePID.Calculate(GetAngle() - _start_angle, dt);

  _drivetrain->SetVelocity(frc::ChassisSpeeds {
    fwd_speed, 0_mps, ang_speed
  });

  if (_distancePID.IsStable() && _anglePID.IsStable()) {
    _drivetrain->SetIdle();
    SetDone();
  }
}

// Turn to angle behaviours 

DrivetrainTurnToAngle::DrivetrainTurnToAngle(Drivetrain *d, units::degree_t setpoint) : _drivetrain(d), _pid("drivetrain/behaviours/DrivetrainTurnAngle/pid", d->GetConfig().anglePID) {
  Controls(d);
  _pid.SetWrap(360_deg);
  _pid.SetSetpoint(setpoint);
} 

units::degree_t DrivetrainTurnToAngle::GetAngle() const {
  return _drivetrain->GetConfig().gyro->GetRotation2d().Degrees();
}

void DrivetrainTurnToAngle::OnStart() {
  _start_angle = GetAngle();
}

void DrivetrainTurnToAngle::OnTick(units::second_t dt) {
  auto speed = _pid.Calculate(GetAngle() - _start_angle, dt);
  _drivetrain->SetVelocity(frc::ChassisSpeeds {
    0_mps, 0_mps, speed
  });

  if (_pid.IsStable()) {
    _drivetrain->SetIdle();
    SetDone();
  }
}