#pragma once

#include <frc/geometry/Pose2d.h>

struct Poses {
<<<<<<< HEAD
    static constexpr frc::Pose2d innerGrid1{1_m, 1_m, 0_deg}; // Closest grid position to the Wall
    static constexpr frc::Pose2d innerGrid2{1_m, 2_m, 0_deg}; // Middle of Inner Grid
    static constexpr frc::Pose2d innerGrid3{1_m, 3_m, 0_deg}; // Centremost Inner Grid position
    static constexpr frc::Pose2d centreGrid1{1_m, 4_m, 0_deg}; // The non central grid on the Inner Grid side
    static constexpr frc::Pose2d centreGrid2{1_m, 5_m, 0_deg}; // The middle most grid 
    static constexpr frc::Pose2d centreGrid3{1_m, 6_m, 0_deg}; // The non central grid on the Outer Grid side
    static constexpr frc::Pose2d outerGrid1{1_m, 7_m, 0_deg}; // Centremost outer grid position
    static constexpr frc::Pose2d outerGrid2{1_m, 8_m, 0_deg}; // Middle of Outer Grid
    static constexpr frc::Pose2d outerGrid3{1_m, 9_m, 0_deg}; // Closest grid position to enemy Loading Zone
=======
    static constexpr frc::Pose2d innerGrid1{0.1_m, 0.1_m, 0_deg}; // Closest grid position to the Wall
    static constexpr frc::Pose2d innerGrid2{0.5_m, 0_m, 0_deg}; // Middle of Inner Grid
    static constexpr frc::Pose2d innerGrid3 ={0.5_m, -0.5_m, 0_deg}; // Centremost Inner Grid position
    static constexpr frc::Pose2d centreGrid1 ={0_m, 0.5_m, 0_deg}; // The non central grid on the Inner Grid side
    static constexpr frc::Pose2d centreGrid2 ={0_m, 0_m, 270_deg}; // The middle most grid 
    static constexpr frc::Pose2d centreGrid3 ={0_m, -0.5_m, 0_deg}; // The non central grid on the Outer Grid side
    static constexpr frc::Pose2d outerGrid1 ={-0.5_m, 0.5_m, 0_deg}; // Centremost outer grid position
    static constexpr frc::Pose2d outerGrid2 ={1_m, 0_m, 0_deg}; // Middle of Outer Grid
    static constexpr frc::Pose2d outerGrid3 ={-0.5_m, -0.5_m, 0_deg}; // Closest grid position to enemy Loading Zone
>>>>>>> 196464c2623a52754945b90214277c13fd1c436b
};