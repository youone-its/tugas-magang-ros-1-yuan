#include <ros/ros.h>
#include <custom_msg/dataDiri.h>

ros::Publisher irt_ekspor;

double IRT(custom_msg::dataDiri& student) {
    double average = (student.TryOut1 + student.TryOut2 + student.TryOut3) / 3.0;
    double lulus = 0.0;

    if (student.Kampus == "ITS") {
        lulus = 
        (student.Jurusan == "TEKNOLOGI INFORMASI") ? (average / 500 * 60) : 
        (student.Jurusan == "TEKNIK MESIN") ? (average / 450 * 60) : 0;
    } else if (student.Kampus == "UNAIR") {
        lulus = 
        (student.Jurusan == "KEDOKTERAN") ? (average / 600 * 60) : 
        (student.Jurusan == "MANAJEMEN BISNIS") ? (average / 400 * 60) : 0;
    } else if (student.Kampus == "ITB") {
        lulus = 
        (student.Jurusan == "STEI") ? (average / 600 * 55) : 
        (student.Jurusan == "FTTM") ? (average / 600 * 60) : 0;
    } else if (student.Kampus == "UI") {
        lulus = 
        (student.Jurusan == "KEDOKTERAN") ? (average / 600 * 60) : 
        (student.Jurusan == "FILSAFAT") ? (average / 500 * 60) : 0;
    } else {
        ROS_WARN("Kampus atau jurusan tidak dikenali untuk %s.", student.Nama.c_str());
        return 0.0; // Invalid data
    }
    return lulus;
}

void listenMainIrt(const custom_msg::dataDiri::ConstPtr& msg) {
    custom_msg::dataDiri student = *msg;
    student.Lulus = IRT(student);
    irt_ekspor.publish(student);  // Publish the processed data to be saved by ekspor_node
    ROS_INFO("Data siswa %s diproses: Lulus = %.2f%%", student.Nama.c_str(), student.Lulus);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "irt_node");
    ros::NodeHandle nh;

    irt_ekspor= nh.advertise<custom_msg::dataDiri>("irt_ekspor", 100);  // Publish to the topic "main_ekspor"
    ros::Subscriber main_irt = nh.subscribe("main_irt", 1000, listenMainIrt);

    ros::spin();
    return 0;
}
