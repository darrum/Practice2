#include <iostream>
#include <fstream> // Para usar ficheros
#include <vector> // Para usar vector
#include <regex>
#include <cstring>
#include <iomanip>

using namespace std;

const int KMAXNIF=10;
const int KMAXNAME=50;
const int KMAXTELEPHONE=14;

// Registro para los pacientes
struct Patient{
    string nif;
    string name;
    string telephone;
};

// Registro para los pacientes en fichero binario
struct PatientBin{
    char nif[KMAXNIF];
    char name[KMAXNAME];
    char telephone[KMAXTELEPHONE];
};

// Registro para las fechas
struct Date{
    int day;
    int month;
    int year;
};

// Registro para las analíticas
struct Analysis{
    unsigned int id;
    char nif[KMAXNIF];
    Date dateAnalysis ;
    float weight;
    float height;
};

// Registro para la base de datos
struct Database{
    unsigned int nextId;
    vector<Patient> patients;
    vector<Analysis> analysis;
};

// Tipos de errores posibles
enum Error {
    ERR_ARGS,
    ERR_FILE,
    ERR_OPTION,
    ERR_PATIENT_EXISTS,
    ERR_PATIENT_NOT_EXISTS,
    ERR_WRONG_DATE,
    ERR_WRONG_NAME,
    ERR_WRONG_NIF,
    ERR_WRONG_NUMBER,
    ERR_WRONG_TELEPHONE
};

/*
Función que muestra los distintos tipos de errores posibles
e: tipo de error a mostrar
return: void
*/
void error(Error e){
    switch (e){
        case ERR_ARGS: cout << "ERROR: wrong arguments" << endl;
            break;
        case ERR_FILE: cout << "ERROR: cannot open file" << endl;
            break;
        case ERR_OPTION: cout << "ERROR: wrong option" << endl;
            break;
        case ERR_PATIENT_EXISTS: cout << "ERROR: patient already exists" << endl;
            break;
        case ERR_PATIENT_NOT_EXISTS: cout << "ERROR: patient does not exist" << endl;
            break;
        case ERR_WRONG_DATE: cout << "ERROR: wrong date" << endl;
            break;
        case ERR_WRONG_NAME: cout << "ERROR: wrong name " << endl;
            break;
        case ERR_WRONG_NIF: cout << "ERROR: wrong NIF " << endl;
            break;
        case ERR_WRONG_NUMBER: cout << "ERROR: wrong number " << endl;
            break;
        case ERR_WRONG_TELEPHONE: cout << "ERROR: wrong telephone number" << endl;
    }
}

/*
Función que muestra el menú de opciones
return: void
*/
void showMenu() {
    cout << "1- Add patient" << endl
         << "2- View Patient" << endl
         << "3- Delete patient" << endl
         << "4- Save patients" << endl
         << "5- Add analysis" << endl
         << "6- Export analysis" << endl
         << "7- Import analysis" << endl
         << "8- Statistics" << endl
         << "q- Quit" << endl
         << "Option: ";
}

int searchPatient(const Database& data, const string &nif) { //not finished
    int patientIndex = -1;
    bool found = false;
    for (size_t i = 0; i < data.patients.size() && !found; i++) {
        if (nif == data.patients[i].nif) {
            found = true;
            patientIndex = i;
        }
    }
    return patientIndex;
}

bool isValidNIF(const std::string& nif) {
    std::regex nifPattern("^[0-9]{8}[A-Z]$"); // 8 digits + 1 uppercase letter
    return std::regex_match(nif, nifPattern);
}

bool isValidDate(const Date& date) {
    if (date.month < 1 || date.month > 12 || date.day < 1 || date.day > 31 || date.year < 2025 || date.year > 2050) {
        return false;
    }
    return true;
}

void addPatient(Database& data) {
    Patient newPatient;
    string nif, name, telephone;
    bool legitName = false, legitPhone = false, legitNIF = false;

    do {
        int patientIndex;
        cout << "Enter NIF: ";
        getline(cin, nif);
        if (nif.empty()) {
            return;
        }

        patientIndex = searchPatient(data, nif);

        if (patientIndex != -1) {
            error(ERR_PATIENT_EXISTS);
        }
        else {
            legitNIF = isValidNIF(nif);
            if (legitNIF) {
                newPatient.nif = nif;
            } else {
                error(ERR_WRONG_NIF);
            }
        }
    } while (!legitNIF);

    do {
        cout << "Enter name:";
        getline(cin, name);
        if (nif.empty()) {
            return;
        }
        if (name.size() < 3) {
            error(ERR_WRONG_NAME);
        }
        else {
            legitName = true;
            newPatient.name = name;
        }
    } while (!legitName);

    do {
        cout << "Enter telephone:";
        getline(cin, telephone);
        if (telephone.size() <= (KMAXTELEPHONE - 1) && telephone.size() >= 11) {
            if (telephone[0] != '+') { // not finished about numbers
                error(ERR_WRONG_TELEPHONE);
            } else {
                legitPhone = true;
                newPatient.telephone = telephone;
            }
        } else {
            error(ERR_WRONG_TELEPHONE);
        }
    } while (!legitPhone);

    data.patients.push_back(newPatient);
}

void viewPatient(const Database& data) {
    string nif;
    int patientIndex = -1;
    while (patientIndex == -1){
        cout << "Enter NIF: ";
        getline(cin, nif);
        if (nif.empty()) {
            return;
        }
        patientIndex = searchPatient(data, nif);
        if (patientIndex == -1) {
            error(ERR_PATIENT_NOT_EXISTS);
        }
    }

    cout << "NIF: " << data.patients[patientIndex].nif << endl;
    cout << "Name: " << data.patients[patientIndex].name << endl;
    cout << "Telephone: " << data.patients[patientIndex].telephone << endl;
    cout << "Id \t" << "Date \t" << "Height \t" << "Weight \t" << endl;

    for (size_t i = 0; i < data.analysis.size(); i++) {
        if (data.analysis[i].nif == nif) {
            cout << data.analysis[i].id << "\t"
            << setw(2) << setfill('0') << data.analysis[i].dateAnalysis.day << "/"
            << setw(2) << setfill('0') << data.analysis[i].dateAnalysis.month << "/"
            << setw(2) << setfill('0') << data.analysis[i].dateAnalysis.year << "\t"
            << data.analysis[i].height << "\t"
            << data.analysis[i].weight << endl;
        }
    }
}

void deletePatient(Database& data) {
    string nif;
    bool validNIF = false;
    int patientIndex;

    do {
        cout << "Enter NIF: ";
        getline(cin, nif); // nine characters are entered for the NIF, consisting of eight digits and one letter.
        if (nif.empty()) {
            return;
        }
        patientIndex = searchPatient(data, nif);
        if (patientIndex == -1) {
            error(ERR_PATIENT_NOT_EXISTS);
        } else {
            validNIF = true;
        }
    } while (!validNIF);

    data.patients.erase(data.patients.begin() + patientIndex);
    for (size_t i = 0; i < data.analysis.size(); i++) {
        if (data.analysis[i].nif == nif) {
            data.analysis.erase(data.analysis.begin()+i);
            i--;
        }
    }
}

void savePatients(const Database& data) {
    PatientBin binPatient{};
    ofstream file("patients.bin", ios::binary);

    if (file.is_open()) {
        for (size_t i = 0; i < data.patients.size(); i++) {
            strcpy(binPatient.nif, data.patients[i].nif.c_str());
            strcpy(binPatient.name, data.patients[i].name.c_str());
            strcpy(binPatient.telephone, data.patients[i].telephone.c_str());

            file.write((const char *)&binPatient, sizeof(binPatient));
        }
    }
    file.close();
}

void addAnalysis(Database& data) {
    Analysis newAnalysis{data.nextId++};
    Date date;
    string nif;
    float weight, height;
    bool patientExists = false, validDate, validWeight = false, validHeight = false;

    do {
        cout << "Enter NIF: ";
        getline(cin, nif);
        if (nif.empty()) {
            return;
        }

        bool legitNIF = isValidNIF(nif);

        if (legitNIF) {
            int patientIndex = searchPatient(data, nif);

            if (patientIndex != -1) {
                patientExists = true;
                strcpy(newAnalysis.nif, nif.c_str());
            } else {
                error(ERR_PATIENT_NOT_EXISTS);
            }
        } else {
            error(ERR_WRONG_NIF);
        }
    } while (!patientExists);

    do {
        cout << "Enter date (day/month/year):";
        char slash;  // To discard the slashes
        cin >> date.day >> slash >> date.month >> slash >> date.year;
        validDate = isValidDate(date);
        if (!validDate) {
            error(ERR_WRONG_DATE);
        } else {
            newAnalysis.dateAnalysis.day = date.day;
            newAnalysis.dateAnalysis.month = date.month;
            newAnalysis.dateAnalysis.year = date.year;
        }
    } while (!validDate);

    do {
        cout << "Enter weight:";
        cin >> weight;
        if (weight > 0) {
            validWeight = true;
            newAnalysis.weight = weight;
        } else {
            error(ERR_WRONG_NUMBER);
        }
    } while(!validWeight);

    do {
        cout << "Enter height:";
        cin >> height;
        if (height > 0) {
            validHeight = true;
            newAnalysis.height = height;
        } else {
            error(ERR_WRONG_NUMBER);
        }
    } while(!validHeight);

    data.analysis.push_back(newAnalysis);
}

void exportAnalysis(Database data) {
    ofstream file("analysis.bin", ios::binary);

    if (file.is_open()) {
        for (size_t i = 0; i < data.analysis.size(); i++) {
            file.write((const char *)&data.analysis[i], sizeof(Analysis));
        }
    }
    file.close();
}

void importAnalysis(Database& data) {
    ofstream fw("wrong_patients.txt", ios::app);
    ifstream fr("analysis.bin", ios::binary);

    if (!fr.is_open() || !fw.is_open()) {
        error(ERR_FILE);
        return;
    }

    Analysis binAnalysis{};
    while (fr.read((char *)&binAnalysis, sizeof(Analysis))) {
        int patientIndex = searchPatient(data, binAnalysis.nif);

        if (patientIndex == -1) { //not sure about format
            fw << binAnalysis.nif << endl;
        } else {
            binAnalysis.id = data.nextId++;
            data.analysis.push_back(binAnalysis);
        }
    }
}

void loadPatients(Database & data) {
    ifstream fr("patients.bin", ios::binary);
    PatientBin binP;
    Patient strP;

    if (fr.is_open()) {
        while (fr.read((char *)&binP, sizeof(PatientBin))) {
            strP.name = binP.name;
            strP.nif = binP.nif;
            strP.telephone = binP.telephone;
            data.patients.push_back(strP);
        }
    }

    fr.close();
}

string bmiCalculator(const float& weight, const float& height) {
    float bmi = weight / ((height/100) * (height/100));
    string strBMI;
        if (bmi < 18.5) strBMI = "Underweight";
        else if (bmi < 25) strBMI = "Healthy";
        else if (bmi < 30) strBMI = "Overweight";
        else strBMI = "Obesity";
    return strBMI;
}

void statistics(const Database& data) {
    ofstream file("statistics.txt");
    for (size_t i = 0; i < data.analysis.size();i++) {
        string bmi = bmiCalculator(data.analysis[i].weight, data.analysis[i].height);

        cout << data.analysis[i].nif << ";"
        << data.analysis[i].dateAnalysis.day << "/"
        << data.analysis[i].dateAnalysis.month << "/"
        << data.analysis[i].dateAnalysis.year << ";"
        << data.analysis[i].weight << ";"
        << data.analysis[i].height << ";"
        << bmi << endl;

        file << data.analysis[i].nif << ";"
        << data.analysis[i].dateAnalysis.day << "/"
        << data.analysis[i].dateAnalysis.month << "/"
        << data.analysis[i].dateAnalysis.year << ";"
        << data.analysis[i].weight << ";"
        << data.analysis[i].height << ";"
        << bmi << endl;
    }
    file.close();
}
/*
Función principal: Tendrás que añadir más código tuyo
return: 0
*/
int main(int argc, char *argv[]){
    Database data;
    data.nextId=1;
    char option;
    loadPatients(data);

    do{
        showMenu();
        cin >> option;
        cin.ignore();

        switch(option){
            case '1': addPatient(data); // Llamar a la función "addPatient" para añadir una nueva ficha de paciente
                break;
            case '2': viewPatient(data);// Llamar a la función "viewPatient" para ver la información de un paciente
                break;
            case '3': deletePatient(data);// Llamar a la función "deletePatient" para eliminar una ficha de paciente
                break;
            case '4': savePatients(data);// Llamar a la función "savePatients" para guardar las fichas de pacientes en fichero binario
                break;
            case '5': addAnalysis(data);// Llamar a la función "addAnalysis" para anadir una analítica
                break;
            case '6': exportAnalysis(data);// Llamar a la función "exportAnalysis" para exportar las analiticas realizadas a fichero binario
                break;
            case '7': importAnalysis(data);// Llamar a la función "importAnalysis" para importar las analiticas en fichero binario
                break;
            case '8': statistics(data);// Llamar a la función "statistics" para guardar las preguntas en fichero
                break;
            case 'q': // Salir del programa
                break;
            default: error(ERR_OPTION);
        }
    } while(option!='q');

    return 0;
}