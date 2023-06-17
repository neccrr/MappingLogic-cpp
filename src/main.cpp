#include <iostream>
#include <chrono>

#include "logic/Mapping.h"
#include "logic/Merge.h"

using namespace std;

int main() {
    // Mapping::processMapping();

    // Make user select logic to run
    cout << "Please select an option" << endl;
    cout << "1. Mapping" << endl;
    cout << "2. Merge" << endl;
    cout << "3. Exit" << endl;
    int selectedOption;
    cin >> selectedOption;

    // Check if the selected option is valid
    if (selectedOption < 1 || selectedOption > 3) {
        cout << "Invalid option" << endl;
        system("pause");
        return 0;
    }

    // Run the selected logic
    switch (selectedOption) {
        case 1:
            Mapping::processMapping();
            break;
        case 2:
            Merge::processMerge();
            break;
        case 3:
            system("pause");
            return 0;

        default:
            cout << "Invalid option" << endl;
            system("pause");
            return 0;
    }


    system("pause");
    return 0;
}
