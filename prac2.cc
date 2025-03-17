#include <iostream>
#include <fstream> // Para usar ficheros
#include <vector> // Para usar vector
#include <regex>

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

int searchPatient(const Database data, const string nif) { //not finished
    int patientIndex = -1;
    bool found = false;
    for (int i = 0; i < data.patients.size() || !found; i++) {
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

bool isValidDate(const std::string& date) {
    std::regex datePattern("^[1-31]/[1-12]/[2025-2050]$"); // 8 digits + 1 uppercase letter
    return std::regex_match(date, datePattern);
}

void addPatient(Database data) {
    Patient newPatient;
    string nif, name, telephone;
    bool legitName = false, legitPhone = false, legitNIF = false;

    do {
        int patientIndex;
        cout << "Enter NIF: " << endl;
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
        cout << "Enter name:" << endl;
        getline(cin, name);
        if (nif.empty()) {
            return;
        }
        if (name.size() < 3 && name.size() > KMAXNAME) {
            error(ERR_WRONG_NAME);
        }
        else {
            legitName = true;
            newPatient.name = name;
        }
    } while (!legitName);

    do {
        cout << "Enter telephone:" << endl;
        getline(cin, telephone);
        if (telephone.size() <= (KMAXTELEPHONE - 1) && telephone.size() >= 11) {
            if (telephone[0] != '+') { // not finished about numbers
                error(ERR_WRONG_NUMBER);
            } else {
                legitPhone = true;
                newPatient.telephone = telephone;
            }
        } else {
            error(ERR_WRONG_NUMBER);
        }
    } while (!legitPhone);

    data.patients.push_back(newPatient);
}

void viewPatient(Database data) {
    string nif;
    int patientIndex = -1;
    while (patientIndex == -1){
        cout << "Enter NIF: " << endl;
        getline(cin, nif);
        if (nif.empty()) {
            return;
        }
        patientIndex = searchPatient(data, nif); // -1
        if (patientIndex == -1) {
            error(ERR_PATIENT_NOT_EXISTS);
        }
    }

    cout << "NIF: " << data.patients[patientIndex].nif;
    cout << "Name: " << data.patients[patientIndex].name;
    cout << "Telephone: " << data.patients[patientIndex].telephone;
    cout << "Id \t" << "Date \t" << "Height \t" << "Weight \t"; //not finished
}

void deletePatient(Database data) {
    string nif;
    bool validNIF = false;
    int patientIndex;

    do {
        cout << "Enter NIF: " << endl;
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
    for (int i = 0; i < data.analysis.size(); i++) {
        if (data.analysis[i].nif == nif) {
            data.analysis.erase(data.analysis.begin()+i);
            i--;
        }
    }
}

void savePatients(Database data) {
    PatientBin binPatient{};
    ofstream file("patients.bin", ios::binary);

    if (file.is_open()) {
        for (int i = 0; i < data.patients.size(); i++) {
            strcpy(binPatient.nif, data.patients[i].nif.c_str());
            strcpy(binPatient.name, data.patients[i].name.c_str());
            strcpy(binPatient.telephone, data.patients[i].telephone.c_str());

            file.write((const char *)&binPatient, sizeof(binPatient));
        }
    }
    file.close();
}

void addAnalysis(Database data) {
    Analysis newAnalysis{data.nextId++};
    string nif, date;
    float weight, height;
    bool patientExists = false, validDate, validWeight = false, validHeight = false;

    do {
        int patientIndex;
        cout << "Enter NIF: " << endl;
        getline(cin, nif);
        if (nif.empty()) {
            return;
        }

        patientIndex = searchPatient(data, nif);

        if (patientIndex != -1) {
            patientExists = true;
            strcpy(newAnalysis.nif, nif.c_str());
        } else {
            error(ERR_PATIENT_NOT_EXISTS);
        }
    } while (!patientExists);

    do {
        cout << "Enter date (day/month/year):" << endl;
        getline(cin, date);
        validDate = isValidDate(date);
        if (!validDate) {
            error(ERR_WRONG_DATE);
        } else {
            // strcpy(newAnalysis.dateAnalysis, date) //not finished, Date struct
        }
    } while (!validDate);

    do {
        cout << "Enter weight:" << endl;
        cin >> weight;
        if (weight > 0) {
            validWeight = true;
            newAnalysis.weight = weight;
        }
    } while(!validWeight);

    do {
        cout << "Enter height:" << endl;
        cin >> height;
        if (height > 0) {
            validHeight = true;
            newAnalysis.height = height;
        }
    } while(!validHeight);

    data.analysis.push_back(newAnalysis);
}

void exportAnalysis(Database data) {
    ofstream file("analysis.bin", ios::binary);

    if (file.is_open()) {
        for (int i = 0; i < data.analysis.size(); i++) {
            // strcpy(binPatient.nif, data.patients[i].nif.c_str());
            // strcpy(binPatient.name, data.patients[i].name.c_str());
            // strcpy(binPatient.telephone, data.patients[i].telephone.c_str());
            //
            // file.write((const char *)&binPatient, sizeof(binPatient));
        }
    }
    file.close();
}

void importAnalysis(Database data) {
    ofstream fw("wrong_patients.txt", ios::app);
    ifstream fr("analysis.bin", ios::binary);
    if (fr.is_open() && fw.is_open()) {
        string line;

        while (getline(fr,line)) {
            cout << line << endl; //not finished
        }
    } else {
       error(ERR_FILE);
    }
}

string bmiCalculator(float weight, float height) {
    float bmi = weight / (height * height);
    string strBMI;
        if (bmi < 18.5) strBMI = "Underweight";
        else if (bmi < 25) strBMI = "Healthy";
        else if (bmi < 30) strBMI = "Overweight";
        else strBMI = "Obesity";
    return strBMI;
}

void statistics(Database data) {
    ofstream file("statistics.txt");
    for (int i = 0; i < data.analysis.size();i++) {
        string bmi = bmiCalculator(data.analysis[i].weight, data.analysis[i].height);

        cout << data.analysis[i].nif << ";" << data.analysis[i].dateAnalysis.day << "/" << data.analysis[i].dateAnalysis.month << "/" << data.analysis[i].dateAnalysis.year << "/"
        << ";" << data.analysis[i].weight << ";" << data.analysis[i].height << ";" << bmi << endl;
    }
}
/*
Función principal: Tendrás que añadir más código tuyo
return: 0
*/
int main(int argc, char *argv[]){
    Database data;
    data.nextId=1;
    char option;

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