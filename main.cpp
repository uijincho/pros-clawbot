#include "main.h"
#include "pros/motors.hpp"
#include <cmath>

// change numbers as needed to match with ports
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 10
#define ARM 8
#define CLAW 3

pros::MotorGroup LeftDriveSmart({LEFT_MOTOR});
pros::MotorGroup RightDriveSmart({RIGHT_MOTOR});
pros::Motor Arm(ARM);
pros::Motor Claw(CLAW);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller Controller1(pros::E_CONTROLLER_MASTER);

	while (true) {
		// Calculate drivetrain motor velocities
        // Left joystick (up/down) for forward/backward of left side motor (Axis3)
        // Right joystick (up/down) for forward/backward of right side motor  (Axis2)
        int left = Controller1.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); // left side speed
        int right = Controller1.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y); // right side speed

        // Set motor velocities
        LeftDriveSmart.move_velocity(-(left * 2)); // if going backwards, remove negative here and add negative for right
        RightDriveSmart.move_velocity((right * 2));

        // Control Arm using shoulder buttons (L1/L2)
        if (Controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            Arm.move_velocity(200); // Spin Arm forward
        }
        else if (Controller1.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        {
            Arm.move_velocity(-200); // Spin Arm backward
        }
        else
        {
            Arm.move_velocity(0); // Stop Arm
        }

        if (Controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            Claw.move_velocity(-100); // Spin high stakes forward
        }
        else if (Controller1.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        {
            Claw.move_velocity(100); // Spin high stakes backward
        }
        else
        {
            Claw.move_velocity(0); // Stop high stakes
        }

        // Delay to prevent CPU overload
        pros::delay(20);
	}
}
