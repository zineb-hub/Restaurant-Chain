#include <iostream>
#include "wilayaAVL.h"
#include <fstream>
#include <sstream>
#include "restauAVL.h"
#include"TimeRestaurantAVL.h"
#include"PrizeAVL.h"
#include <chrono>
using namespace std;
using namespace chrono;
int main() {
    wilayaAVL country;
    restauAVL restaurant;
    TimeRestaurant t;
    ResDataAVL tree;
    ResData resto;
    int choice;
    string dummy; // to consume the newline character
    int i = 0;
    auto timeConsumed = microseconds(0);
    int c = 1;
    while (c!=0) {
        auto start = high_resolution_clock::now();
        cout << "Enter the number of the operation you want to perform" << endl;
        cout << "1: Add a restaurant." << endl;
        cout << "2: Delete a restaurant." << endl;
        cout << "3: Display All restaurants." << endl;
        cout << "4: Modify in restaurants' data." << endl;
        cout << "5: Number of Restaurants." << endl;
        cout << "6: Does exist any Restaurant." << endl;
        cout << "7: Display All restaurants of a specific district." << endl;
        cout << "8: Display All restaurants of a specific city." << endl;
        cout << "9: Display All restaurants of a specific Wilaya." << endl;
        cout << "10: Display All restaurants of all the country: " << country.getCountryName() << " with region names. " << endl;
        cout << "11: Does a Wilaya exist: " << endl;
        cout << "12: Does a City exist: " << endl;
        cout << "13: Does a District exist: " << endl;
        cout << "14: Does a Restaurant exist: " << endl;
        cout << "15: Set Sales" << endl
            << "16: Set cost" << endl
            << "17: Set publicity" << endl
            << "18: Set cuisine rating" << endl
            << "19: Get sales" << endl
            << "20: Get cost" << endl
            << "21: Get publicity" << endl
            << "22: Get cuisine rating" << endl
            << "23: Get ratio of the monthly sales to the amount of money spent on publicity" << endl
            << "24 : Get the winner prize of each cuisine " << endl;
        cout << "0- Exit." << endl;

        cin >> choice;
        getline(cin, dummy); // consume the newline character

        if (choice == 1) {
            cout << "Give the name of the restaurant you want to add: ";
            string nameRestaurant;
            getline(cin, nameRestaurant);
            cout << "Give the name of the district you want the restaurant be added to: ";
            string nameDistrict;
            getline(cin, nameDistrict);
            cout << "Give the name of the city you want the restaurant be added to: ";
            string nameCity;
            getline(cin, nameCity);
            cout << "Give the name of the Wilaya you want the restaurant be added to: ";
            string nameWilaya;
            getline(cin, nameWilaya);
            country.addRestaurant(nameWilaya, nameCity, nameDistrict, nameRestaurant);
            int choice2;
            string state;
            do
            {
                cout << "choose 1 if the restaurant is owned and 2 if the Restaurant is franchised: ";
                cin >> choice2;
            } while (choice2 != 1 && choice2 != 2);
            if (choice2 == 1)
            {
                state = "owned";
            }
            else {
                state = "franchised";
            }
            int numEmp;
            do
            {
                cout << "Enter the number of employees: ";
                cin >> numEmp;
            } while (isalpha(numEmp));
            restaurant.insert(state, nameRestaurant, numEmp);
        }
        else if (choice == 2)
        {
            cout << "Give the name of the restaurant you want to add: ";
            string nameRestaurant;
            getline(cin, nameRestaurant);
            cout << "Give the name of the district you want the restaurant be added to: ";
            string nameDistrict;
            getline(cin, nameDistrict);
            cout << "Give the name of the city you want the restaurant be added to: ";
            string nameCity;
            getline(cin, nameCity);
            cout << "Give the name of the Wilaya you want the restaurant be added to: ";
            string nameWilaya;
            getline(cin, nameWilaya);
            country.deleteRestaurant(nameWilaya, nameCity, nameDistrict, nameRestaurant);
            restaurant.removeRestau(nameRestaurant);
        }
        else if (choice == 3)
        {
            restaurant.printTree();
        }
        else if (choice == 4)
        {
            cout << "Give the name of the restaurant you want to add: ";
            string name;
            getline(cin, name);
            restaurant.modifyrestaurantAVL(name);
        }
        else if (choice == 5)
        {
            restaurant.getResturantnum();
        }
        else if (choice == 6)
        {
            restaurant.isEmpty();
        }
        else if (choice == 7)
        {
            cout << "Give the name of the district you want the restaurant be printed from: ";
            string nameDistrict;
            getline(cin, nameDistrict);
            cout << "Give the name of the city you want the restaurant be printed from: ";
            string nameCity;
            getline(cin, nameCity);
            cout << "Give the name of the Wilaya you want the restaurant be printed from: ";
            string nameWilaya;
            getline(cin, nameWilaya);

            if (country.doesExistDistrict(nameWilaya, nameCity, nameDistrict))
            {
                cout << "District: " << nameDistrict << endl;
                if (country.getDistrictRestaurants(nameWilaya, nameCity, nameDistrict))
                {
                    const wilayaAVL::RestauNode* restoList = country.getDistrictRestaurants(nameWilaya, nameCity, nameDistrict);
                    while (restoList != nullptr) {
                        cout << "  Restaurant: " << restoList->nameRest << endl;
                        restoList = restoList->next;
                    }
                }
                else {
                    cout << "There are no restaurants in this district." << endl;
                }
            }
            else {
                cout << "District does not exist" << endl;
            }
        }
        else if (choice == 8)
        {
            cout << "Give the name of the city you want the restaurant be printed from: ";
            string nameCity;
            getline(cin, nameCity);
            cout << "Give the name of the Wilaya you want the restaurant be printed from: ";
            string nameWilaya;
            getline(cin, nameWilaya);

            if (country.doesExistCity(nameWilaya, nameCity))
            {
                vector<wilayaAVL::arrayCaseRList> districts = country.getCityRestaurants(nameWilaya, nameCity);

                cout << "Wilaya: " << nameWilaya << endl;
                cout << "City: " << nameCity << endl;

                for (auto& element : districts)
                {
                    cout << "District: " << element.regionName << endl;

                    const wilayaAVL::RestauNode* restoList = element.pointerToLinkedList;
                    if (restoList)
                    {
                        while (restoList != nullptr) {
                            cout << "  Restaurant: " << restoList->nameRest << endl;
                            restoList = restoList->next;
                        }
                    }
                    else {
                        cout << "There are no restaurants in this district." << endl;
                    }
                }
            }
            else
            {
                cout << "City does not exist" << endl;
            }

        }
        else if (choice == 9)
        {
            cout << "Give the name of the Wilaya you want the restaurant to be printed from: ";
            string nameWilaya;
            getline(cin, nameWilaya);
            if (country.doesExistWilaya(nameWilaya))
            {
                vector<wilayaAVL::arrayCase1To2> cities = country.getWilayaRestaurants(nameWilaya);
                cout << "Wilaya: " << nameWilaya << endl;
                for (const auto& city : cities) {
                    cout << "City: " << city.regionName << endl;

                    for (const auto& district : *(city.pointerToVector)) {
                        cout << "District: " << district.regionName << endl;

                        const wilayaAVL::RestauNode* restoList = district.pointerToLinkedList;
                        if (restoList)
                        {
                            while (restoList != nullptr) {
                                cout << "  Restaurant: " << restoList->nameRest << endl;
                                restoList = restoList->next;
                            }
                        }
                        else {
                            cout << "There are no restaurants in this district." << endl;
                        }
                    }
                }
            }
        }
        else if (choice == 10)
        {
            cout << country.getCountryName() << ": " << endl;
            vector<wilayaAVL::arrayCase> wilayas = country.getCountryRestaurants();
            for (auto& element1 : wilayas)
            {
                cout << "wilaya: " << element1.regionName << endl;
                vector<wilayaAVL::arrayCase1To2> cities = *(element1.pointerToVector);;

                for (auto& element : cities) {
                    cout << "City: " << element.regionName << endl;
                    vector<wilayaAVL::arrayCaseRList> districts = *(element.pointerToVector);

                    for (auto& element2 : districts)
                    {
                        cout << "District: " << element2.regionName << endl;

                        const wilayaAVL::RestauNode* restoList = element2.pointerToLinkedList;
                        if (restoList)
                        {
                            while (restoList != nullptr) {
                                cout << "  Restaurant: " << restoList->nameRest << endl;
                                restoList = restoList->next;
                            }
                        }
                        else {
                            cout << "There are no restaurants in this district." << endl;
                        }
                    }
                }
            }
        }
        else if (choice == 11)
        {
            cout << "Give the name of the Wilaya you want to check if it exists or not: ";
            string nameWilaya;
            getline(cin, nameWilaya);
            if (country.doesExistWilaya(nameWilaya))
            {
                cout << "Yes, the Wilaya " << nameWilaya << " exist." << endl;
            }
            else {
                cout << "No, the Wilaya " << nameWilaya << "does not exist." << endl;
            }
        }
        else if (choice == 12)
        {
            cout << "Give the name of the City you want to check if it exists or not: ";
            string nameCity;
            getline(cin, nameCity);
            cout << "Give the name of the Wilaya you want to check if it exists or not: ";
            string nameWilaya;
            getline(cin, nameWilaya);
            if (country.doesExistCity(nameWilaya, nameCity))
            {
                cout << "Yes, the City " << nameCity << "in the wilaya" << nameWilaya << " exist." << endl;
            }
            else {
                cout << "No, the City " << nameCity << "in the wilaya" << nameWilaya << "does not exist." << endl;
            }
        }
        else if (choice == 13)
        {
            cout << "Give the name of the District you want to check if it exists or not: ";
            string nameDistrict;
            getline(cin, nameDistrict);
            cout << "Give the name of the City you want to check if it exists or not: ";
            string nameCity;
            getline(cin, nameCity);
            cout << "Give the name of the Wilaya you want to check if it exists or not: ";
            string nameWilaya;
            getline(cin, nameWilaya);
            if (country.doesExistDistrict(nameWilaya, nameCity, nameDistrict))
            {
                cout << "Yes, the District " << nameDistrict << "in the city" << nameCity << "in the wilaya" << nameWilaya << " exist." << endl;
            }
            else {
                cout << "No, the District " << nameDistrict << "in the city" << nameCity << "in the wilaya" << nameWilaya << "does not exist." << endl;
            }
        }
        else if (choice == 14)
        {
            cout << "Give the name of the Restaurant you want to check if it exists or not: ";
            string nameRestaurant;
            getline(cin, nameRestaurant);
            cout << "Give the name of the District you want to check if it exists or not: ";
            string nameDistrict;
            getline(cin, nameDistrict);
            cout << "Give the name of the City you want to check if it exists or not: ";
            string nameCity;
            getline(cin, nameCity);
            cout << "Give the name of the Wilaya you want to check if it exists or not: ";
            string nameWilaya;
            getline(cin, nameWilaya);
            if (country.doesExistRestaurant(nameWilaya, nameCity, nameDistrict, nameRestaurant))
            {
                cout << "Yes, the Restaurant " << nameRestaurant << "in the District " << nameDistrict << "in the city" << nameCity << "in the wilaya" << nameWilaya << " exist." << endl;
            }
            else {
                cout << "No, the Restaurant " << nameRestaurant << "in the District " << nameDistrict << "in the city" << nameCity << "in the wilaya" << nameWilaya << "does not exist." << endl;
            }
        }
        else if (choice == 15)
            t.setTodaySale();

        else if (choice == 16)
            t.setTodayCost();

        else if (choice == 17)
            t.setTodayPublicity();

        else if (choice == 18)
            t.setTodayRating();

        else if (choice == 19)
        {
            int choiceS;
            cout << "Choose an option: " << endl
                << "1: Sales related to place" << endl
                << "2: Daily sales" << endl
                << "3: Monthly sales or in a period of time" << endl;
            cin >> choiceS;
            while (choiceS < 1 || choiceS > 3)
            {
                cout << "Invalid choice, try again: ";
                cin >> choiceS;
            }

            if (choice == 1)
            {
                int choiceP;
                cout << "1: Sales of a district" << endl
                    << "2: Sales of a city" << endl
                    << "3: Sales of a wilaya" << endl
                    << "4: Sales of the country" << endl;
                cin >> choiceP;
                while (choiceP < 1 || choiceP > 4)
                {
                    cout << "Invalid choice, try again: ";
                    cin >> choiceP;
                }

                if (choiceP == 1)
                    t.districtSales(country);
                else if (choiceP == 2)
                    t.citySales(country);
                else if (choiceP == 3)
                    t.wilayaSales(country);
                else if (choiceP == 4)
                    t.countrySales(country);
            }

            else if (choiceS == 2)
                t.getDailySales();
            else if (choiceS == 3)
                t.getSales();
            break;
        }
        else if (choice == 20)
        {
            int choiceC;
            cout << "Choose an option: " << endl
                << "1: Daily cost" << endl
                << "2: Monthly cost or in a period of time" << endl;
            cin >> choiceC;

            while (choiceC < 1 || choiceC > 2)
            {
                cout << "Invalid choice, try again: ";
                cin >> choiceC;
            }

            if (choiceC == 1)
                t.getDailyCost();
            else if (choiceC == 2)
                t.getCost();
        }

        else if (choice == 21)
        {
            int choicePub;
            cout << "Choose an option: " << endl
                << "1: Daily publicity" << endl
                << "2: Monthly publicity  or in a period of time" << endl;
            cin >> choicePub;
            while (choicePub < 1 || choicePub > 2)
            {
                cout << "Invalid choice, try again: ";
                cin >> choicePub;
            }
            if (choicePub == 1)
                t.getDailyPublicity();
            else if (choicePub == 2)
                t.getPublicity();
        }

        else if (choice == 22)
            t.getCuisineRating();

        else if (choice == 23)
            t.getRatio(country);
        else if (choice == 24)
        {
            cout << "choose an option " << endl;
            int c;
            cout << "1: to Get the algerian winner " << endl
                << "2: to Get the Syrian winner " << endl
                << "3: to Get the Chinese winner " << endl
                << "4: to Get the Indian winner " << endl
                << "5: to Get the European winner " << endl
                << "6: to get all winners " << endl;
            cin >> c;
            while (c < 1 || c > 5)
            {
                cout << " option out_of_range enter integer from 1 TO 5 only please " << endl;
                cout << "1: to get the algerian winner " << endl
                    << "2: to get the Syrian winner " << endl
                    << "3: to get the Chinese winner " << endl
                    << "4: to get the Indian winner " << endl
                    << "5: to get the European winner " << endl;
                cin >> c;
            }

            Prize prizewinners(&tree);
            vector<string> winner = prizewinners.getWinner();
            if (c == 1)
            {
                cout << " The Algerian Prize Winner is : " << winner[0] << endl;
            }
            else if (c == 2)
            {
                cout << " The Syrian Prize Winner is : " << winner[1] << endl;
            }
            else if (c == 3)
            {
                cout << " The Chinese Prize Winner is : " << winner[2] << endl;
            }
            else if (c == 4)
            {
                cout << " The Indian Prize Winner is : " << winner[3] << endl;
            }
            else if (c == 5)
            {
                cout << " The European Prize Winner is : " << winner[4] << endl;
            }
            else
            {
                cout << " The Algerian Prize Winner is : " << winner[0] << endl;
                cout << " The Syrian Prize Winner is : " << winner[1] << endl;
                cout << " The Chinese Prize Winner is : " << winner[2] << endl;
                cout << " The Indian Prize Winner is : " << winner[3] << endl;
                cout << " The European Prize Winner is : " << winner[4] << endl;
            }
        }
        else if (choice == 0) {
            c = 0;
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        timeConsumed += duration;
        cout << " the  time for displaying the operation : " << choice << " is :" << duration.count() << " microseconds " << endl;
        i++;
        cout << "***************" << endl;
        cout << "A new Operation" << endl;
    }
    auto average = timeConsumed / i;
    cout << "the average time for displaying the various results is : " << average.count() << "microseconds" << endl;

    return 0;
}