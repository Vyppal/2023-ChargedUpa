#include "behaviour/SwerveBaseBehaviour.h"
#include "ControlUtil.h"

#include <units/angular_velocity.h>
#include <units/charge.h>
#include <units/moment_of_inertia.h>
// #include <units/units.h>

using namespace wom;

ManualDrivebase::ManualDrivebase(wom::SwerveDrive *swerveDrivebase, frc::XboxController *driverController):  _swerveDrivebase(swerveDrivebase), _driverController(driverController){
    Controls(swerveDrivebase);
}

void ManualDrivebase::OnTick(units::second_t deltaTime){
  double l_x = wom::spow2(-wom::deadzone(_driverController->GetLeftY(), driverDeadzone)); // GetLeftY due to x being where y should be
  double l_y = wom::spow2(wom::deadzone(_driverController->GetLeftX(), driverDeadzone));
  double r_x = wom::spow2(wom::deadzone(_driverController->GetRightX(), turningDeadzone));

  // _swerveDrivebase->GetConfig();
  
  // Robot Relative Controls
  // _swerveDrivebase->SetVelocity(frc::ChassisSpeeds {
  //   l_x * maxMovementMagnitude,
  //   l_y * maxMovementMagnitude,
  //   r_x * 360_deg / 0.01_s // were once 90_deg / 1_s
  // });

  // Field Relative Controls
  _swerveDrivebase->SetFieldRelativeVelocity(wom::FieldRelativeSpeeds {
   l_x * l_x * maxMovementMagnitude,
   l_y * l_y * maxMovementMagnitude,
   r_x * 360_deg / 0.01_s // were once 360_deg / 1_s
  });
  
  // Tests if the Robot Moves
  //  _swerveDrivebase->SetVelocity(frc::ChassisSpeeds {
  //    0.5_mps,
  //    0_mps,
  //    0_rad_per_s
  //  });
}

DrivebasePoseBehaviour::DrivebasePoseBehaviour(wom::SwerveDrive *swerveDrivebase, frc::Pose2d pose) : _swerveDrivebase(swerveDrivebase), _pose(pose){
  Controls(swerveDrivebase);
}
void DrivebasePoseBehaviour::OnTick(units::second_t deltaTime){
  _swerveDrivebase->SetPose(_pose);

  if (_swerveDrivebase->IsAtSetPose()){
    SetDone();
  }
}

DrivebaseBalance::DrivebaseBalance(wom::SwerveDrive *swerveDrivebase) : _swerveDrivebase(swerveDrivebase) {
  Controls(swerveDrivebase);
}
void DrivebaseBalance::OnTick(units::second_t deltaTime){
  // assumes that the robot is already rotated correctly

  _currentSwervePose = _swerveDrivebase->GetPose();
  double xDistance = (_convergencePoint.X() - _currentSwervePose.X()).value();
  double yDistance = (_convergencePoint.Y() - _currentSwervePose.Y()).value();
  double distanceToTarget = sqrt(pow(xDistance, 2) + pow(yDistance, 2));
  if (abs(distanceToTarget) < _maxDistance) {
     _swerveDrivebase->SetFieldRelativeVelocity(wom::FieldRelativeSpeeds {
        (0.6_m / (1 + exp(-0.15 * xDistance)) - 0.3_m) / 1_s,
        (0.6_m / (1 + exp(-0.15 * yDistance)) - 0.3_m) / 1_s,
        0_deg / 1_s
      });
  }
  else {
    double angle = atan(yDistance / xDistance);
    _maxConvergenceSpeed / sin(angle);
    _swerveDrivebase->SetFieldRelativeVelocity(wom::FieldRelativeSpeeds {
        _maxConvergenceSpeed / sin(angle) / 1_s,
        _maxConvergenceSpeed / cos(angle) / 1_s,
        0_deg / 1_s
    });
  }




}

void DrivebaseBalance::SetConvergencePoint(frc::Pose2d pose){
  _convergencePoint = pose;
}