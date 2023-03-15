#ifndef __MY_ROBOT_AGENT__H
#define __MY_ROBOT_AGENT__H 

#include "enviro.h"
#include <string>
#include <chrono>

using namespace enviro;

class MyRobotController : public Process, public AgentInterface   {

    public:
    MyRobotController() : Process(), AgentInterface() {}

    // update sensor and angle values on each sensor
    void update_sensors_and_angles() {
        top_sensor = sensor_value(0);
        bottom_sensor = sensor_value(1);
        left_sensor = sensor_value(2);
        right_sensor = sensor_value(3);

        top_facing = sensor_reflection_type(0);
        bottom_facing = sensor_reflection_type(1);
        left_facing = sensor_reflection_type(2);
        right_facing = sensor_reflection_type(3);

        facing_angle = angle();
    }

    // Once the robot makes it to the exit, it transports back to the beginning
    void reset_robot() {
        if (top_facing == "None" && bottom_facing == "None"
            && left_facing == "None" && right_facing == "None") {
                teleport(-325, 0, 0);
                loops++;
            }
    }

    void init() {}
    void start() {
        std::chrono::seconds duration(10);
        std::this_thread::sleep_for(duration);
        update_sensors_and_angles();
    }
    void update() {

        update_sensors_and_angles();

        track_velocity(75, 0);
        if (top_sensor < 30 && bottom_sensor < 30) {


            if (left_sensor > right_sensor) {
                teleport(x(), y(), angle()+1.5708);
            } else if (left_sensor < right_sensor) {
                teleport(x(), y(), angle()-1.5708);
            } else if (left_sensor == right_sensor) {
                teleport(x(), y(), angle()-1.5708);
            }

        } 
    

        std::cout << "Loops: " << loops << std::endl;
        std::cout << "Current Angle: " << facing_angle << "rads" << std::endl;
        std::cout << "Top sensor value: " << top_sensor << " and is facing " << top_facing << std::endl;
        std::cout << "Bottom sensor value: " << bottom_sensor << " and is facing " << bottom_facing << std::endl;
        std::cout << "Left sensor value: " << left_sensor << " and is facing " << left_facing << std::endl;
        std::cout << "Right sensor value: " << right_sensor << " and is facing " << right_facing << "\n\n";
        reset_robot();

    }
    void stop() {}

    int loops;
    double top_sensor, bottom_sensor, left_sensor, right_sensor, facing_angle;
    std::string top_facing, bottom_facing, left_facing, right_facing;

};

class MyRobot : public Agent {
    public:
    MyRobot(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    MyRobotController c;
};

DECLARE_INTERFACE(MyRobot)

#endif