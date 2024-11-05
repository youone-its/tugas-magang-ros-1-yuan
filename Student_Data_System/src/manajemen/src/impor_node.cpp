#include <ros/ros.h>
#include <std_msgs/String.h>
#include <custom_msg/dataDiri.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;
using ll = uint64_t;
ros::Publisher impor_main;

void readAndPublishData() {
    ifstream file("db.txt");
    if (!file.is_open()) {
        ROS_ERROR("Failed to open db.txt");
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        custom_msg::dataDiri student;
        try {
            // Read the fields one by one in the expected format
            if (line.find("Nama    : ") == 0) {
                student.Nama = line.substr(10);
                getline(file, line);
                student.NRP = stoll(line.substr(10));
                
                getline(file, line);
                student.TryOut1 = stoll(line.substr(10));
                
                getline(file, line);
                student.TryOut2 = stoll(line.substr(10));
                
                getline(file, line);
                student.TryOut3 = stoll(line.substr(10));
                
                getline(file, line);
                student.Kampus = line.substr(10);
                
                getline(file, line);
                student.Jurusan = line.substr(10);
                
                getline(file, line);
                student.Lulus = stod(line.substr(10));

                // Publish the student data to the ROS topic
                impor_main.publish(student);
                ROS_INFO("Data siswa published: %s", student.Nama.c_str());
            }
        } catch (const exception& e) {
            ROS_WARN("Error parsing data: %s", e.what());
            continue;  // Skip to the next entry in case of an error
        }
    }

    file.close();
    ROS_INFO("Data successfully loaded and published from db.txt.");
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "impor_node");
    ros::NodeHandle nh;

    impor_main = nh.advertise<custom_msg::dataDiri>("impor_main", 100);
    readAndPublishData();

    ros::spin();
    return 0;
}
