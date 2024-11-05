#include <ros/ros.h>
#include <std_msgs/String.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void hapusSiswaKecualiTerbaru(const string& nrp) {
    ifstream fileIn("db.txt");
    if (!fileIn.is_open()) {
        ROS_ERROR("Gagal membuka file db.txt untuk dibaca.");
        return;
    }

    vector<string> lines;       // Menyimpan semua baris yang tidak dihapus
    vector<string> dataSiswa;   // Menyimpan data siswa yang ditemukan
    string line;

    // Membaca baris dari file
    while (getline(fileIn, line)) {
        // Mengecek apakah baris berisi NRP yang dicari
        if (line.find("NRP     : " + nrp) != string::npos) {
            // Menyimpan baris NRP
            dataSiswa.push_back(line);
            do {
                // Menyimpan semua baris terkait siswa sampai separator
                dataSiswa.push_back(line);
            } while (getline(fileIn, line) && line != "-------------------------");
            dataSiswa.push_back("__________________________"); // Menambahkan penanda akhir
        } else {
            // Menyimpan baris yang tidak berhubungan dengan NRP
            lines.push_back(line);
        }
    }
    fileIn.close();

    if (dataSiswa.empty()) {
        ROS_WARN("Data siswa dengan NRP %s tidak ditemukan.", nrp.c_str());
        return;
    }

    // Menyimpan siswa terbaru
    string siswaTerbaru = dataSiswa.back();
    dataSiswa.clear();
    dataSiswa.push_back(siswaTerbaru);

    // Menulis kembali ke file
    ofstream fileOut("db.txt");
    if (!fileOut.is_open()) {
        ROS_ERROR("Gagal membuka file db.txt untuk ditulis.");
        return;
    }

    // Menulis kembali semua baris yang tidak dihapus
    for (const auto& l : lines) {
        fileOut << l << endl;
    }

    // Menulis kembali siswa terbaru
    for (const auto& l : dataSiswa) {
        fileOut << l << endl;
    }

    fileOut.close();
    ROS_INFO("Data siswa dengan NRP %s telah dihapus, hanya menyisakan data terbaru.", nrp.c_str());
}

void hapusCallback(const std_msgs::String::ConstPtr& msg) {
    hapusSiswaKecualiTerbaru(msg->data);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "hapus_terbaru_node");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("hapus_nrp", 1000, hapusCallback);

    ros::spin();
    return 0;
}
