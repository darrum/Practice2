#include <iostream>
#include <fstream> // Para usar ficheros
#include <vector> // Para usar vector

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

// int searchPatient(string nif) { //not finished
//     return -1;
// }

void addPatient(Database data) {
    Patient newPatient;
    string nif, name, telephone;
    bool legitName = false, legitPhone = false, legitNIF = false;

    int patientIndex = -1;
    do {
        cout << "Enter NIF: " << endl;
        getline(cin, nif); // nine characters are entered for the NIF, consisting of eight digits and one letter.
        if (nif.empty()) {
            return;
        }

        patientIndex = searchPatient(nif); // -1

        if (patientIndex != -1) {
            error(ERR_PATIENT_EXISTS);
        }
        else if (patientIndex == 0) { //else if // eight digits and one letter
            error(ERR_WRONG_NIF);
        }
        else {
            legitNIF = true;
            newPatient.nif = nif;
        }
    } while (!legitNIF);

    do {
        cout << "Enter name:" << endl;
        getline(cin, name);
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
        if (telephone.size() <= 12 && telephone.size() >= 10) {
            //number starts with the symbol +
            //isdigit()
        } else {
            error(ERR_WRONG_NUMBER);
        }
    } while (!legitPhone);
    newPatient.telephone = telephone;

    data.patients.push_back(newPatient);
}

void viewPatient(Database data) {
    string nif;
    int patientIndex;
    do {
        cout << "Enter NIF: " << endl;
        getline(cin, nif); // nine characters are entered for the NIF, consisting of eight digits and one letter.
        if (nif.empty()) {
            return;
        }

        patientIndex = searchPatient(nif); // -1
        if (patientIndex == -1) {
            error(ERR_PATIENT_NOT_EXISTS);
        }
    } while (patientIndex == -1);

    cout << "NIF: " << data.patients[patientIndex].nif;
    cout << "Name: " << data.patients[patientIndex].name;
    cout << "Telephone: " << data.patients[patientIndex].telephone;
    cout << "Id \t" << "Date \t" << "Height \t" << "Weight \t"; //not finished
}

void deletePatient(Database data) {}

void savePatients(){}

void addAnalysis(){}

void exportAnalysis(){}

void importAnalysis(){}

void statistics(){}
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
            case '4': // Llamar a la función "savePatients" para guardar las fichas de pacientes en fichero binario
                break;
            case '5': // Llamar a la función "addAnalysis" para anadir una analítica
                break;
            case '6': // Llamar a la función "exportAnalysis" para exportar las analiticas realizadas a fichero binario                
                break;
            case '7': // Llamar a la función "importAnalysis" para importar las analiticas en fichero binario
                break;
            case '8': // Llamar a la función "statistics" para guardar las preguntas en fichero
                break;
            case 'q': // Salir del programa 
                break;
            default: error(ERR_OPTION);
        }
    }while(option!='q');
   
    return 0;
}