//
// Created by nmarkert on 05.12.24.
//

#include <cstring>
#include <iostream>
#include "mavlink/common/mavlink.h"
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

#define BUFFER_SIZE 256
static void send_fuzzing_Message(MavlinkPassthrough &mavlink_passthrough, std::streamsize bytesRead);

int main()
{
  // Read 256 bytes from stdin
  char buffer[BUFFER_SIZE];
  std::cout << "Enter up to 256 bytes: ";
  std::cin.read(buffer, BUFFER_SIZE);
  std::streamsize bytesRead = std::cin.gcount();

  // Fork the process
  pid_t pid = fork();
  if (pid < 0)
  {
    std::cerr << "Fork failed!" << std::endl;
    return 1;
  }

  if (pid == 0)
  { // Child process
    // Wait for 10 seconds
    sleep(3);

    // Create a MAVLink message
    //        mavlink_message_t msg;
    //        uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    // Fill the MAVLink message with the input data
    //        mavlink_msg_data_stream_send(MAVLINK_COMM_0, 0, 0, 0, 0, 0); //
    //        Example message, replace with your own
    // You can use mavlink_msg_<your_message_type>_pack to create your message

    // Send the MAVLink message (this is a placeholder, replace with actual
    // sending code) For example, you might send it over a serial port or UDP
    // socket mavlink_send(buf, sizeof(buf)); // Implement your sending logic
    // here

    std::cout << "Child process sent MAVLink message." << std::endl;
    Mavsdk mavsdk{Mavsdk::Configuration{ComponentType::GroundStation}};
    ConnectionResult connection_result = mavsdk.add_any_connection("tcp://127.0.0.1:5760");
    if (connection_result != ConnectionResult::Success)
    {
      std::cerr << "Connection failed: " << connection_result << '\n';
      return 1;
    }
    //
    auto system = mavsdk.first_autopilot(3.0);
    if (!system)
    {
      std::cerr << "Timed out waiting for system\n";
      return 1;
    }

    // Instantiate plugins.
    // auto telemetry = Telemetry{system.value()};
    // auto action = Action{system.value()};
    auto mavlink_passthrough = MavlinkPassthrough{system.value()};

    // We want to listen to the altitude of the drone at 1 Hz.
    // const auto set_rate_result = telemetry.set_rate_position(1.0);
    // if (set_rate_result != Telemetry::Result::Success)
    // {
    //   std::cerr << "Setting rate failed: " << set_rate_result << '\n';
    //   return 1;
    // }

    // // Set up callback to monitor altitude while the vehicle is in flight
    // telemetry.subscribe_position([](Telemetry::Position position)
    //                              { std::cout << "Altitude: " << position.relative_altitude_m << " m\n"; });

    // Check until vehicle is ready to arm
    // while (telemetry.health_all_ok() != true)
    // {
    //   std::cout << "Vehicle is getting ready to arm\n";
    //   sleep_for(seconds(1));
    // }

    // Arm vehicle
    // std::cout << "Arming...\n";
    // const Action::Result arm_result = action.arm();

    // mavlink_message_t msg = mavlink_message_t;

    send_fuzzing_Message(mavlink_passthrough, bytesRead);

    return 0;
  }
  else
  { // Parent process
    std::cout << "I'm the parent process." << std::endl;
    // Execute the ArduCopter executable
    const char *args[] = {"/home/nmarkert/git/ardupilot/build/sitl/bin/arducopter", "-S", "--model", "+", "--defaults", "/home/nmarkert/git/ardupilot/Tools/autotest/default_params/copter.parm", nullptr};
    if (execvp(args[0], const_cast<char *const *>(args)) == -1)
    {
      perror("execvp failed");
    }
    return 1;
  }

  return 0;
}

void send_fuzzing_Message(MavlinkPassthrough &mavlink_passthrough, std::streamsize bytesRead)
{

  MavlinkPassthrough::CommandLong command{};
  command.command = MAV_CMD_DO_SET_ROI;
  command.param1 = 841;
  command.param2 = -355;
  command.param3 = -478;
  command.param4 = -605;
  command.param5 = 409;
  command.param6 = -237;
  command.param7 = -691;
  mavlink_passthrough.send_command_long(command);

  mavlink_passthrough.queue_message(
      [&](MavlinkAddress mavlink_address, uint8_t channel)
      {
        mavlink_message_t message;
        mavlink_msg_heartbeat_pack_chan(
            mavlink_address.system_id,
            mavlink_address.component_id,
            channel,
            &message,
            MAV_TYPE_GENERIC,
            MAV_AUTOPILOT_GENERIC,
            0,
            0,
            0);
        return message;
      });
}