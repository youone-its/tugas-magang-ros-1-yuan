#include <ros/ros.h>
#include <custom_msg/dataDiri.h>
#include <fstream>
using namespace std;

void listenMainEkspor(const custom_msg::dataDiri::ConstPtr& msg) {
    ofstream file("db.txt", ios::app);  // Open file in append mode
    if (!file.is_open()) {
        ROS_ERROR("Gagal membuka db.txt untuk menulis");
        return;
    }

    // Write the data to the file
    file << "Nama    : " << msg->Nama << "\n"
         << "NRP     : " << msg->NRP << "\n"
         << "TO1     : " << msg->TryOut1 << "\n"
         << "TO2     : " << msg->TryOut2 << "\n"
         << "TO3     : " << msg->TryOut3 << "\n"
         << "Kampus  : " << msg->Kampus << "\n"
         << "Jurusan : " << msg->Jurusan << "\n"
         << "Lulus   : " << msg->Lulus << " %\n"
         << "-------------------------\n";

    ROS_INFO("Data siswa disimpan: %s", msg->Nama.c_str());
    file.close();
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "ekspor_node");
    ros::NodeHandle nh;

    // Subscribe to the "main_ekspor" topic
    ros::Subscriber irt_ekspor = nh.subscribe("irt_ekspor", 1000, listenMainEkspor);

    ros::spin();
    return 0;
}
