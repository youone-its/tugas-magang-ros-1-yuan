#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <custom_msg/dataDiri.h>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
using ll = uint64_t;

ros::Publisher main_irt;
ros::Publisher main_ekspor;
vector<custom_msg::dataDiri> data;

void imporCallback(const custom_msg::dataDiri::ConstPtr& msg) {
    data.push_back(*msg);
    ROS_INFO("Data dari impor_node diterima: %s", msg->Nama.c_str());
}

void irtCallback(const custom_msg::dataDiri::ConstPtr& msg) {
    for (auto& student : data) {
        if (student.NRP == msg->NRP) {
            student.Lulus = msg->Lulus;
            ROS_INFO("\nData lulus untuk %s diperbarui: %.2f%%", student.Nama.c_str(), msg->Lulus);
            return;
        }
    }
    ROS_WARN("\nData NRP %lu tidak ditemukan untuk diperbarui.\n", msg->NRP);
}

bool cekNRP(ll np) {
    return none_of(data.begin(), data.end(), [&](const custom_msg::dataDiri& d) { return d.NRP == np; });
}

void displayStudent(const custom_msg::dataDiri& student) {
    cout << "Nama: " << student.Nama << "\nNRP: " << student.NRP
         << "\nTO1: " << student.TryOut1 << "\nTO2: " << student.TryOut2
         << "\nTO3: " << student.TryOut3 << "\nKampus: " << student.Kampus
         << "\nJurusan: " << student.Jurusan << "\nLulus: " << student.Lulus
         << "%\n-------------------------\n";
}

void masuk() {
    while (true) {
        cout << "1 = Input data\n0 = Selesai\nInput: ";
        int n;
        cin >> n;
        if (n == 0) break;

        custom_msg::dataDiri student;
        cout << "\nMasukkan Nama: ";
        cin.ignore();
        getline(cin, student.Nama);

        do {
            cout << "\nMasukkan NRP: ";
            cin >> student.NRP;
            if (!cekNRP(student.NRP)) {
                cout << "\nNRP sudah ada. Coba NRP berbeda.\n";
            }
        } while (!cekNRP(student.NRP));

        cout << "\nMasukkan TO1: "; cin >> student.TryOut1;
        cout << "\nMasukkan TO2: "; cin >> student.TryOut2;
        cout << "\nMasukkan TO3: "; cin >> student.TryOut3;
        cout << "\nMasukkan kampus tujuan: ";
        cin.ignore();
        getline(cin, student.Kampus);
        cout << "\nMasukkan jurusan: ";
        getline(cin, student.Jurusan);

        main_irt.publish(student);
        data.push_back(student);
        ROS_INFO("\nData siswa baru dikirim ke irt_node untuk kalkulasi: %s\n", student.Nama.c_str());
    }
}

void modifikasi() {
    if(data.empty()){ 
        cout<<"\nData kosong. Tambah data terlebih dahulu.\n"; 
        return; 
    }

    ll np;
    cout<<"\nMasukkan NRP siswa untuk diubah: ";
    cin>>np;

    auto it=find_if(data.begin(), data.end(), [&](const custom_msg::dataDiri& student) { return student.NRP == np; });
    if(it != data.end()){
        custom_msg::dataDiri& student = *it;
        int choice;
        while(true) {
            cout<<"\nMasukkan opsi:\n1=Nama\n2=NRP\n3=TO1\n4=TO2\n5=TO3\n6=Kampus\n7=Jurusan\n8=Selesai\nInput: ";
            cin>>choice;

            switch (choice) {
                case 1: cout<<"\nNama: "; cin.ignore(); getline(cin, student.Nama); break;
                case 2: 
                    cout<<"\nNRP: ";
                    cin>>student.NRP;
                    while(!cekNRP(student.NRP)){
                        cout<<"\nNRP sudah ada. Coba NRP berbeda: ";
                        cin>>student.NRP;
                    }
                    break;
                case 3: cout<<"\nNilai TO1: ";cin>>student.TryOut1; break;
                case 4: cout<<"\nNilai TO2: ";cin>>student.TryOut2; break;
                case 5: cout<<"\nNilai TO3: ";cin>>student.TryOut3; break;
                case 6: cout<<"\nKampus: ";cin.ignore(); getline(cin, student.Kampus); break;
                case 7: cout<<"\nJurusan: ";cin.ignore(); getline(cin, student.Jurusan); break;
                case 8: 
                    main_irt.publish(student); // Publish to irt_node
                    main_ekspor.publish(student); // Also publish to ekspor_node to reflect changes
                    ROS_INFO("Data siswa dengan NRP %lu telah diperbarui.", student.NRP);
                    return;
                default: cout<<"\nInput tidak valid.\n"; break;
            }
            main_irt.publish(student); // Always publish after any change
        }
    } else {
        cout<<"\nData NRP tidak ditemukan.\n";
    }
}


void apus() {
    if (data.empty()) {
        cout << "\nData kosong. Tambah data terlebih dahulu.\n";
        return;
    }

    ll np;
    cout << "\nMasukkan NRP siswa untuk dihapus: ";
    cin >> np;

    // Use find_if to search for the student with the matching NRP
    auto it = find_if(data.begin(), data.end(), [&](const custom_msg::dataDiri& student) {
        return student.NRP == np;
    });

    if (it != data.end()) {
        // Erase the student from the vector
        data.erase(it);
        cout << "\nData dihapus.\n";

        // Optionally, publish an empty message with the deleted NRP to inform other nodes
        custom_msg::dataDiri deletedStudent;
        deletedStudent.NRP = np;
        main_ekspor.publish(deletedStudent); // Notify ekspor node about deletion
    } else {
        cout << "\nData tidak ditemukan.\n";
    }
}


void eksport(){
    // Clear or reset the previous export queue if needed
    for(const auto& student : data){
        main_ekspor.publish(student); // Publish the updated list of students
    }
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "main");
    ros::NodeHandle nh;

    main_irt = nh.advertise<custom_msg::dataDiri>("main_irt", 100);
    main_ekspor = nh.advertise<custom_msg::dataDiri>("main_ekspor", 100);
    ros::Subscriber impor_main = nh.subscribe("impor_main", 1000, imporCallback);

    int option;
    while (ros::ok()) {
        cout << "\n1: Masuk data baru, 2: Modifikasi data, 3: Hapus data, 0: Keluar\n";
        cin >> option;

        switch (option) {
            case 1: masuk(); break;
            case 2: modifikasi(); break;
            case 3: apus(); break;
            case 0: return 0;
            default: cout << "\nPilihan tidak valid.\n";
        }

        ros::spinOnce();
    }

    return 0;
}
