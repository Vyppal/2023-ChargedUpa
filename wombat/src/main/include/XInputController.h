#pragma once

#include <frc/XboxController.h>
#include <frc/PS4Controller.h>

namespace wom {
  /**
   * @brief enum to specify the type of controller (XBOX1, PS4)
  */
  enum ControllerType {
    XBOX1,
    PS4
  };

  /**
  *@brief {WARNING : Untested in a competition} Controller generalisation for XBOX1 and PS4 controllers.
  * The purpose of this generalisation, is to allow teams to easily switch between PS4 controllers and XBOX 1 controllers with only 1 modification to the code.
  *@param controllerType the type of controller (XBOX1, PS4)
  *@param port The controller's designated port number
  */
  class Controller {
   public:
    Controller(ControllerType controllerType, int port) : _controllerType(controllerType), _port(port) {
      if (_controllerType == ControllerType::XBOX1){
      xbox = new frc::XboxController(port);
      isXbox = true;
    }
    else {
      ps4 = new frc::PS4Controller(port);
    }};
     /**
     *@brief Gets the X (horizontal) component of the Right-Side Joystick
     */
    virtual double GetRightX() {   return (isXbox ? xbox->GetRightX() : ps4->GetRightX());   };
    /**
     *@brief Gets the Y (vertical) component of the Right-Side Joystick
     */
    virtual double GetRightY() {   return (isXbox ? xbox->GetRightY() : ps4->GetRightY());   };
     /**
     *@brief Gets the X (horizontal) component of the Left-Side Joystick
     */
    virtual double GetLeftX() {   return (isXbox ? xbox->GetLeftX() : ps4->GetLeftX());   };
    /**
     *@brief Gets the Y (vertical) component of the Left-Side Joystick
     */
    virtual double GetLeftY() {   return (isXbox ? xbox->GetLeftY() : ps4->GetLeftY());   };
    /**
     *@brief Gets the value of the Left-Side trigger
     */
    virtual double GetLeftTrigger() {   return (isXbox ? xbox->GetLeftTriggerAxis() : ps4->GetL2Button());   };
    /**
     *@brief Gets the value of the Right-Side trigger
     */
    virtual double GetRightTrigger() {   return (isXbox ? xbox->GetRightTriggerAxis() : ps4->GetR2Button());   };
    /**
     *@brief Gets the pressed state of the Left-Side bumper
     */
    virtual bool GetLeftBumperPressed() {   return (isXbox ? xbox->GetLeftBumper() : ps4->GetL1Button());   };
    /**
     *@brief Gets the pressed state of the Right-Side bumper
     */
    virtual bool GetRightBumperPressed() {   return (isXbox ? xbox->GetRightBumper() : ps4->GetR1Button());   };
    /**
     *@brief Gets the pressed state of the Left-Side Joystick Button (press down)
     */
    virtual bool GetLeftJoystickPressed() {   return (isXbox ? xbox->GetLeftStickButton() : ps4->GetL3Button());   };
    /**
     *@brief Gets the state of the Right-Side Joystick Button (press down)
     */
    virtual bool GetRightJoystickPressed() {   return (isXbox ? xbox->GetRightStickButton() : ps4->GetR3Button());   };
    /**
     *@brief Gets the pressed state of the A (xbox) or CROSS (PS4) button
     */
    virtual bool GetCPAD_BottomPressed() {   return (isXbox ? xbox->GetAButton() : ps4->GetCrossButton());   };
    /**
     *@brief Gets the pressed state of the B (xbox) or CIRCLE (PS4) button
     */
    virtual bool GetCPAD_RightPressed() {   return (isXbox ? xbox->GetBButton() : ps4->GetCircleButton());   };
    /**
     *@brief Gets the pressed state of the X (xbox) or SQUARE (PS4) button
     */
    virtual bool GetCPAD_LeftPressed() {   return (isXbox ? xbox->GetXButton() : ps4->GetSquareButton());   };
    /**
     *@brief Gets the pressed state of the Y (xbox) or TRIANGLE (PS4) button
     */
    virtual bool GetCPAD_TopPressed() {   return (isXbox ? xbox->GetYButton() : ps4->GetTriangleButton());   };
    /**
     *@brief Gets the pressed state of the BACK (xbox) SHARE (PS4)
     */
    virtual bool GetMiniLeftButtonPressed() {   return (isXbox ? xbox->GetBackButton() : ps4->GetShareButton());   };
    /**
     *@brief Gets the pressed state of the START (xbox) OPTIONS (PS4)
     */
    virtual bool GetMiniRightButtonPressed() {   return (isXbox ? xbox->GetStartButton() : ps4->GetOptionsButton());   };
    /**
     *@brief Gets the pressed state of the Logo Button (not recommended for use, can't be used for XBOX1)
     */
    virtual bool GetLogoButtonPressed() {   return (isXbox ? false : ps4->GetPSButton());   };

   private:
    ControllerType _controllerType;
    frc::XboxController *xbox;
    frc::PS4Controller *ps4;
   
    bool isXbox = false;
    int _port;
  };

} // wom