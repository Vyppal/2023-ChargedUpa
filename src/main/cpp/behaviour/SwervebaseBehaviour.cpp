#include "behaviour/SwerveBaseBehaviour.h"

#include <units/angular_velocity.h>
#include <units/charge.h>
#include <units/moment_of_inertia.h>

#include "ControlUtil.h"
// #include <units/units.h>

using namespace wom;

ManualDrivebase::ManualDrivebase(wom::SwerveDrive *swerveDrivebase, frc::XboxController *driverController) : _swerveDrivebase(swerveDrivebase), _driverController(driverController) {
  Controls(swerveDrivebase);
}

void ManualDrivebase::OnStart() {
  _swerveDrivebase->SetAccelerationLimit(6_mps_sq);
  std::cout << "Manual Drivebase Start" << std::endl;
}

void ManualDrivebase::OnTick(units::second_t deltaTime) {
  double l_x = wom::spow2(-wom::deadzone(_driverController->GetLeftY(), driverDeadzone));  // GetLeftY due to x being where y should be on field
  double l_y = wom::spow2(-wom::deadzone(_driverController->GetLeftX(), driverDeadzone));
  double r_x = wom::spow2(-wom::deadzone(_driverController->GetRightX(), turningDeadzone));

  if (_driverController->GetYButtonPressed()) {  isFieldOrientated = !isFieldOrientated;  }

  if (isFieldOrientated) {  // Field Relative Controls
    _swerveDrivebase->SetFieldRelativeVelocity(wom::FieldRelativeSpeeds{
        l_x * maxMovementMagnitude,
        l_y * maxMovementMagnitude,
        r_x * 360_deg / 1_s
    });
  } else {  // Robot Relative Controls
    _swerveDrivebase->SetVelocity(frc::ChassisSpeeds{
        l_x * maxMovementMagnitude,
        l_y * maxMovementMagnitude,
        r_x * 360_deg / 1_s
    });
  }
  _swerveDriveTable->GetEntry("isFieldOrientated").SetBoolean(isFieldOrientated);
  }

DrivebasePoseBehaviour::DrivebasePoseBehaviour(
    wom::SwerveDrive *swerveDrivebase, frc::Pose2d pose, bool hold)
    : _swerveDrivebase(swerveDrivebase), _pose(pose), _hold(hold) {
  Controls(swerveDrivebase);
}
void DrivebasePoseBehaviour::OnTick(units::second_t deltaTime) {
  double setPoseAngle = _pose.Rotation().Degrees().value();
  double difference = fmod(_swerveDrivebase->GetPose().Rotation().Degrees().value(), 360.0);

  double currentAngle = _swerveDrivebase->GetPose().Rotation().Degrees().value();
  units::degree_t adjustedAngle = 1_deg * (currentAngle - fmod(currentAngle, 360) + _pose.Rotation().Degrees().value());

  _swerveDrivebase->SetPose(frc::Pose2d{_pose.X(), _pose.Y(), adjustedAngle});

  if (_swerveDrivebase->IsAtSetPose() && !_hold){
    SetDone();
  }
}

<<<<<<< HEAD
// DrivebaseBalance::DrivebaseBalance(wom::SwerveDrive *swerveDrivebase, frc::XboxController *driverController) : _swerveDrivebase(swerveDrivebase), _driverController(driverController) {
//   Controls(swerveDrivebase);
// }
// void DrivebaseBalance::OnTick(units::second_t deltaTime){
//   // assumes that the robot is already rotated correctly

//   _currentSwervePose = _swerveDrivebase->GetPose();
//   double xDistance = (_convergencePoint.X() - _currentSwervePose.X()).value();
//   double yDistance = (_convergencePoint.Y() - _currentSwervePose.Y()).value();
//   double distanceToTarget = sqrt(pow(xDistance, 2) + pow(yDistance, 2));
//   if (abs(distanceToTarget) < _maxDistance) {
//      _swerveDrivebase->SetFieldRelativeVelocity(wom::FieldRelativeSpeeds {
//         (0.6_m / (1 + exp(-0.15 * xDistance)) - 0.3_m) / 1_s,
//         (0.6_m / (1 + exp(-0.15 * yDistance)) - 0.3_m) / 1_s,
//         0_deg / 1_s
//       });
//   }
//   else {
//     double angle = atan(yDistance / xDistance);
//     _maxConvergenceSpeed / sin(angle);
//     _swerveDrivebase->SetFieldRelativeVelocity(wom::FieldRelativeSpeeds {
//         _maxConvergenceSpeed / sin(angle) / 1_s,
//         _maxConvergenceSpeed / cos(angle) / 1_s,
//         0_deg / 1_s
//     });
//   }




// }

// void DrivebaseBalance::SetConvergencePoint(frc::Pose2d pose){
//   _convergencePoint = pose;
// }
=======
DrivebaseBalance::DrivebaseBalance(wom::SwerveDrive *swerveDrivebase, wom::NavX *gyro) : _swerveDrivebase(swerveDrivebase), _gyro(gyro) {
  Controls(swerveDrivebase);
}
void DrivebaseBalance::OnTick(units::second_t deltaTime) {
  units::meters_per_second_t lateralMotorSpeed = lateralBalancePID.Calculate(_gyro->GetPitch(), deltaTime);
  units::meters_per_second_t sidewaysMotorSpeed = sidwaysBalancePID.Calculate(-_gyro->GetRoll(), deltaTime);
  _swerveDrivebase->SetVelocity(frc::ChassisSpeeds{
    // units::math::min(units::math::max(lateralMotorSpeed, -0.8), 0.8),
    lateralMotorSpeed,
    sidewaysMotorSpeed,
    0_deg / 1_s
  });

  _swerveDriveTable->GetEntry("Pitch").SetDouble(_gyro->GetPitch().convert<units::degree>().value());
  _swerveDriveTable->GetEntry("BalanceLateralSpeed").SetDouble(lateralMotorSpeed.value());
  _swerveDriveTable->GetEntry("BalanceSidewaysSpeed").SetDouble(sidewaysMotorSpeed.value());
}


XDrivebase::XDrivebase(wom::SwerveDrive *swerveDrivebase) : _swerveDrivebase(swerveDrivebase) {
  Controls(swerveDrivebase);
}
void XDrivebase::OnTick(units::second_t deltaTime) {
  _swerveDrivebase->SetXWheelState();
}
>>>>>>> 196464c2623a52754945b90214277c13fd1c436b
