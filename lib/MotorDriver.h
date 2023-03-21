/*
 * Motor Driver logic
 */
#ifndef MotorDriver_h
#define MotorDriver_h

#include "Directions.h"
#include "Analog.h"

#include <list>

using namespace std;

enum driver_mode
{
    tank,
    omni
};

/*
 * This class can be used for driving the motors of a symmetrical verhicle using the L293D MOTOR DRIVER chip.
 *
 * Workflow:
 * 1. Define motors! Preferably an even amount.
 * 2. Choose driver modes. (default set to tank steering)
 * 3. Start a movement in any direction.
 * 4. Start a new movement or stop all movement.
 */
class MotorDriver
{
public:
    struct Motor
    {
        int in1;
        int in2;
        int en;
        direction pos;
    };

private:
    enum motor_state
    {
        forward,
        backward,
        idle
    };
    static list<Motor> motors;
    static driver_mode driverMode;
    static int speed;
    static unsigned long movementStart;

    static void changeMotorState(Motor const &motor, motor_state dir, int speed = 0)
    {
        Serial.println(motor.en);

        switch (dir)
        {
        case motor_state::idle:
            digitalWrite(motor.in1, 0);
            digitalWrite(motor.in2, 0);
            Analog::write(motor.en, 0);
            break;

        case motor_state::forward:
            digitalWrite(motor.in1, 1);
            digitalWrite(motor.in2, 0);
            Analog::write(motor.en, speed);
            break;

        case motor_state::backward:
            digitalWrite(motor.in1, 0);
            digitalWrite(motor.in2, 1);
            Analog::write(motor.en, speed);
            break;

        default:
            break;
        }
    }

    static void tankMovementHandle(direction dir, int speed)
    {
        switch (dir)
        {
        case direction::forward:
            for (Motor const &motor : motors)
            {
                if (motor.pos == direction::left || motor.pos == direction::right)
                {
                    changeMotorState(motor, motor_state::forward, speed);
                }
            }
            break;

        case direction::backward:
            for (Motor const &motor : motors)
            {
                if (motor.pos == direction::left || motor.pos == direction::right)
                {
                    changeMotorState(motor, motor_state::backward, speed);
                }
            }
            break;

        case direction::left:
            for (Motor const &motor : motors)
            {
                if (motor.pos == direction::left)
                {
                    changeMotorState(motor, motor_state::backward, speed);
                }
                else if (motor.pos == direction::right)
                {
                    changeMotorState(motor, motor_state::forward, speed);
                }
            }
            break;

        case direction::right:
            for (Motor const &motor : motors)
            {
                if (motor.pos == direction::right)
                {
                    changeMotorState(motor, motor_state::backward, speed);
                }
                else if (motor.pos == direction::left)
                {
                    changeMotorState(motor, motor_state::forward, speed);
                }
            }
            break;
        }
    }

    static void omniMovementHandle(direction dir, int speed)
    {
        // implement n stuff
    }

public:
    /*
     * Defines and register a new motor.
     *
     * params:
     * - in1 & in2: directional control pins (digital pins)
     * - en:        speed control pin (pwm pin)
     * - dir:       motor position relative to verhicle
     * - inverse:   true if motor is mounted backwards (optional)
     */
    static void defineMotor(int in1, int in2, int en, direction pos)
    {
        pinMode(in1, OUTPUT);
        pinMode(in2, OUTPUT);
        Analog::write(en, 0);

        motors.push_back({in1 = in1,
                          in2 = in2,
                          en = en,
                          pos = pos});
    }

    /*
     * Change the driver mode.
     *
     * suported driver modes:
     * - tank: change facing directing using left/right counter rotation (default)
     * - omni: movement similar to tank, but suport linear axis movement (requires omnidirectional wheels)
     */
    static void setDriverMode(driver_mode mode)
    {
        driverMode = mode;
    }

    /*
     * Start a new movement.
     *
     * (optionally):
     * duration: add a duration in ms for the movement
     * callback: add callback function that gets called after the movement is ended
     */
    static void startMovement(direction dir, int speed, int duration = 0)
    { // add callback func
        // MotorDriver::speed = speed;
        // movementStart = millis();

        switch (driverMode)
        {
        case driver_mode::tank:
            tankMovementHandle(dir, speed);
            break;

        case driver_mode::omni:
            omniMovementHandle(dir, speed);
            break;
        }
    }

    /*
     * Stop all movements.
     */
    static int stopMovement()
    {
        for (Motor const &motor : motors)
        {
            changeMotorState(motor, motor_state::idle);
        }
        return millis() - movementStart; // maybe create special units to keep track of distance traveld
    }

    static void setMotorDirect(int motorIndex, float value)
    {

        auto lst = motors.begin();
        advance(lst, motorIndex);

        motor_state dir = (value > 0) ? motor_state::forward : motor_state::backward;

        Motor motor = *lst;
        int power = (int)(255 * abs(value));

        if (power < 170)
            power = 0;
        changeMotorState(motor, dir, power);
        // Analog::write(lst->en, value*255);
    }
};

list<MotorDriver::Motor> MotorDriver::motors = {};
driver_mode MotorDriver::driverMode = driver_mode::tank;

#endif