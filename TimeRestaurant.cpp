#include "TimeRestaurant.h"
#include "wilaya.h"
#include <iostream>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

TimeRestaurant::TimeRestaurant()
// consider the default as 0, this will be changed according to the date of the first insertion
{
    maxSize = 12;
    aptr = new node[maxSize];
    for (int i = 0; i < maxSize; i++)
    {
        aptr[i].d.month = 0;
        aptr[i].d.year = 0;
    }
    lastDate.month = 0; // initialise the last date to 0 since there is no last date initially
    lastDate.year = 0;

    // fill the data from the files
    fillFromCuisineFiles();
    fillFromCostFile();
    fillFromPublicityFile();
}

node *TimeRestaurant::getArray()
{
    return aptr;
}

Date TimeRestaurant::getLastDate() // return the last date of insertion
{
    return lastDate;
}

void TimeRestaurant::change(const Date &date)
{
    // if it's the first insertion, change the array's date objects accordingly
    if (aptr[0].d.month == 0 && aptr[0].d.year == 0)
    {
        aptr[0].d.year = date.year;
        aptr[0].d.month = date.month; // Assume we start from 1, so 1 is January

        // Initialize the rest of the months and years
        for (int i = 1; i < maxSize; i++)
        {
            aptr[i].d.month = (aptr[i - 1].d.month % 12) + 1;
            aptr[i].d.year = aptr[i - 1].d.year + ((aptr[i].d.month == 0 || aptr[i].d.month == 1) ? 1 : 0);
        }
        setLastDate(date); // change last
    }
}

void TimeRestaurant::insertResTree(ResDataBST &tree)
{
    Date d = getCurrentDate();
    if (aptr[0].d.month == 0 && aptr[0].d.year == 0) // if it's the first insertion, copy the tree change the dates accordingly
    {
        aptr[0].BStree = tree;
        change(d);
        setLastDate(d);
    }
    else
    { // it's not the first tree, insert it at that position then (mainly used when adding a new resto in middle of the years)
        int pos = getPosition(d);
        aptr[pos].BStree = tree;
        change(d);
        setLastDate(d);
    }
}

void TimeRestaurant::setLastDate(const Date &m)
{ // set last date of insertion
    lastDate.day = m.day;
    lastDate.month = m.month;
    lastDate.year = m.year;
}
bool TimeRestaurant::checkIfNextDate()
{ // when setting, check if the date is not in the current cell then insert a new tree in the position
    // for context all the pointers to the trees are null until we insert in that date
    if (getCurrentDate().year > getLastDate().year && getCurrentDate().month > getLastDate().month)
        return true;
    return false;
}

bool TimeRestaurant::reSize(Date &userDate) // if exceded the size, resize
{
    int diffrence = userDate - aptr[maxSize - 1].d;
    if (diffrence > 0) // so the users date is out of bound, should resize
    {
        int new_maxSize = maxSize + 12;
        node *newArray = new node[new_maxSize];

        // Copy data
        for (int i = 0; i < maxSize; i++)
        {
            newArray[i].d = aptr[i].d;
            newArray[i].BStree = aptr[i].BStree;
        }
        Date nextDate = aptr[maxSize - 1].d;
        nextDate.month++; // get the next month

        for (int i = maxSize; i < new_maxSize; i++) // repeate until the end of the array
        {
            newArray[i].d = nextDate; // copy the next month to the next cell (maxSize)
            nextDate.month++;         // add one to that month and check if it should change
            if (nextDate.month > 12)
            {
                nextDate.month = 1;
                nextDate.year++;
            }
        }
        delete[] aptr; // delete the old array
        aptr = newArray;
        maxSize = new_maxSize;

        return true;
    }
    return false;
}

bool TimeRestaurant::DateCheck(Date d1, Date d2)
{
    /* if the left date is bigger then the right date
     will use this function to compare the given date from the user and the current date to unsure
    the date is not in the future*/

    if (d1.operator>(d2))
        return true;
    return false;
}

// getters
Date TimeRestaurant::getCurrentDate()
{
    time_t now = time(0);
    tm *t = localtime(&now);
    int day, month, year;
    day = t->tm_mday;
    month = t->tm_mon + 1;
    year = 1900 + t->tm_year;

    return Date(day, month, year);
}

Date TimeRestaurant::getSelectedDate()
{
    Date d;
    do // check if the selected value is valid, because it can't be in the future!
    {
        d.year = d.set_year();
        d.month = d.set_month();
        d.day = d.set_day();
        if (DateCheck(d, getCurrentDate()))
        {
            cout << "invalid date try again: ";
        }
    } while (DateCheck(d, getCurrentDate()));
    return d;
}

int TimeRestaurant::getPosition(Date date)
{ // locate the place in the aptr array based on the given date and the first inserted
    int index = (date.year - aptr[0].d.year) * 12 + (date.month - aptr[0].d.month);
    return index;
}

void TimeRestaurant::getSales()
{
    string name;
    int choice;
    double alg = 0.0, chi = 0.0, syr = 0.0, euro = 0.0, ind = 0.0;
    ResData *theRestaurant;
    Date current = getCurrentDate();

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);
    do
    {
        cout << "Choose option: \n 1)-Get sales of a month \n 2)-Get sales of a period of time";
        cin >> choice;
        if (choice != 1 && choice != 2)
            cout << "Invalid choice, try again: ";
    } while (choice != 1 && choice != 2);

    if (choice == 1) // sales of a month
    {
        Date d;
        do
        {
            d.year = d.set_year();
            d.month = d.set_month();
            if (d.year > current.year || d.month > current.month)
                cout << "Invalid date, try again";
        } while (d.year > current.year || d.month > current.month);
        // locate the resto node first from the array using the date then from the BST using the name
        theRestaurant = &aptr[getPosition(d)].BStree.search(name);

        cout << "The sales of " << name << " in " << d.month << "/" << d.year << ": " << endl;
        // calculate the sum of the cuisines sales
        alg = theRestaurant->calc_sum_Sales(theRestaurant->Algerian);
        syr = theRestaurant->calc_sum_Sales(theRestaurant->Syrian);
        chi = theRestaurant->calc_sum_Sales(theRestaurant->Chinese);
        ind = theRestaurant->calc_sum_Sales(theRestaurant->Indian);
        euro = theRestaurant->calc_sum_Sales(theRestaurant->European);
    }
    if (choice == 2) // sales of a period of time
    {
        cout << "Enter the start date: ";
        Date start_date = getSelectedDate();
        cout << "Enter the end date: ";
        Date end_date = getSelectedDate();

        cout << "The sales of " << name << " from " << start_date.day << "/" << start_date.month << "/" << start_date.year
             << " to " << end_date.day << "/" << end_date.month << "/" << end_date.year << " :" << endl;

        int i = getPosition(start_date), j = getPosition(end_date), k = start_date.day - 1, l = end_date.day - 1;

        if (i < j) // the period of time is in multiple months
        {
            while (i != j)
            {
                theRestaurant = &aptr[i].BStree.search(name);
                while (k < 31) // as long as we are not in the end of the month
                {
                    alg += theRestaurant->Algerian[k].sales;
                    syr += theRestaurant->Syrian[k].sales;
                    chi += theRestaurant->Chinese[k].sales;
                    ind += theRestaurant->Indian[k].sales;
                    euro += theRestaurant->European[k].sales;
                    k++;
                }
                i++;
                k = 0;
            }
        }
        if (i == j) // the period of time is in the same month
        {
            theRestaurant = &aptr[i].BStree.search(name);
            while (k <= l)
            {
                alg += theRestaurant->Algerian[k].sales;
                syr += theRestaurant->Syrian[k].sales;
                chi += theRestaurant->Chinese[k].sales;
                ind += theRestaurant->Indian[k].sales;
                euro += theRestaurant->European[k].sales;
                k++;
            }
        }
    }
    cout << "Algerian cuisine: " << alg << endl
         << "Syrain cuisine: " << syr << endl
         << "Chinese cuisine: " << chi << endl
         << "Indian cuisine: " << ind << endl
         << "European cuisine: " << euro << endl
         << "Total sales: " << alg + syr + chi + euro + ind << endl;
}

void TimeRestaurant::getCost()
{
    string name;
    int choice;
    ResData *theRestaurant;

    Date current = getCurrentDate();
    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);

    do
    {
        cout << "Choose option: \n 1)-Get costs of a month \n 2)-Get costs of a period of time";
        cin >> choice;
        if (choice != 1 && choice != 2)
            cout << "Invalid choice, try again: ";
    } while (choice != 1 && choice != 2);

    if (choice == 1) // cost in a month
    {
        Date d;
        do
        {
            d.year = d.set_year();
            d.month = d.set_month();
            if (d.year > current.year || d.month > current.month)
                cout << "Invalid date, try again";
        } while (d.year > current.year || d.month > current.month);

        // locate the resto node first from the array using the date then from the BST using the name
        theRestaurant = &aptr[getPosition(d)].BStree.search(name);

        cout << "The costs of " << name << " in " << d.month << "/" << d.year << ":  "
             << theRestaurant->calc_sum(theRestaurant->cost) << endl;
    }
    if (choice == 2) // cost in a period of time
    {
        Date start_date = getSelectedDate();
        Date end_date = getSelectedDate();
        cout << "The costs of " << name << " from " << start_date.day << "/" << start_date.month << "/" << start_date.year
             << " to " << end_date.day << "/" << end_date.month << "/" << end_date.year << " : ";

        double costS = 0;
        int i = getPosition(start_date), j = getPosition(end_date), k = start_date.day - 1, l = end_date.day - 1;

        if (i < j) // in diffrent months
        {
            while (i != j)
            {
                theRestaurant = &aptr[i].BStree.search(name);
                while (k < 31) // as long as we are not in the end of the month
                {
                    costS += theRestaurant->cost[k];
                    k++;
                }
                i++;
                k = 0;
            }
        }
        if (i == j) // same month
        {
            theRestaurant = &aptr[i].BStree.search(name);
            while (k <= l)
            {
                costS += theRestaurant->cost[k];
                k++;
            }
        }
        cout << costS << endl;
    }
}

void TimeRestaurant::getPublicity()
{
    string name;
    int choice;
    ResData *theRestaurant;

    Date current = getCurrentDate();
    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);
    do
    {
        cout << "Choose option: " << endl
             << "1)-Get the cost on publicity of a month" << endl
             << "2)-Get the cost on publicity of a period of time" << endl;
        cin >> choice;

        if (choice != 1 && choice != 2)
            cout << "Invalid choice, try again: ";
    } while (choice != 1 && choice != 2);

    if (choice == 1) // cost on publicity in a month
    {
        Date d;
        do
        {
            d.year = d.set_year();
            d.month = d.set_month();
            if (d.year > current.year || d.month > current.month)
                cout << "Invalid date, try again";
        } while (d.year > current.year || d.month > current.month);

        // locate the resto's node then retrieve the data
        theRestaurant = &aptr[getPosition(d)].BStree.search(name);

        cout << "The costs on publicity of " << name << " in " << d.month << "/" << d.year << ": "
             << theRestaurant->calc_sum(theRestaurant->publicity) << endl;
    }
    if (choice == 2) // cost on publicity in a period of time
    {
        cout << "Enter the start date: ";
        Date start_date = getSelectedDate();
        cout << "Enter the end date: ";
        Date end_date = getSelectedDate();

        cout << "The costs on publicity of: " << name << " from " << start_date.day << "/" << start_date.month << "/" << start_date.year
             << " to " << end_date.day << "/" << end_date.month << "/" << end_date.year << " : ";

        double pubS = 0.0;
        int i = getPosition(start_date), j = getPosition(end_date), k = start_date.day - 1, l = end_date.day - 1;

        if (i < j) // in diffrent months
        {
            while (i != j)
            {
                theRestaurant = &aptr[i].BStree.search(name);
                while (k < 31) // as long as we are not in the end of the month
                {
                    pubS += theRestaurant->publicity[k];
                    k++;
                }
                i++;
                k = 0;
            }
        }
        if (i == j) // same month
        {
            theRestaurant = &aptr[i].BStree.search(name);
            while (k <= l)
            {
                pubS += theRestaurant->publicity[k];
                k++;
            }
        }
        cout << pubS << endl;
    }
}

void TimeRestaurant::getDailyCost()
{
    Date date;
    string name;
    int option;

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);
    do
    {
        cout << "Choose an option:\n"
             << "1) Today's cost\n"
             << "2) Not today's cost\n";
        cin >> option;

        if (option != 1 && option != 2)
        {
            cout << "Enter a valid option: ";
        }

    } while (option != 1 && option != 2);

    if (option == 2) // get the selected date from the user
    {
        date = getSelectedDate();
    }

    else
        date = getCurrentDate(); // get the current date if the user wants to set for the current day
    getDailyCost(name, date);    // get the cost
}

void TimeRestaurant::getDailyPublicity()
{

    Date date;
    string name;
    int option;

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);
    cout << name;

    do
    {
        cout << "Choose an option:" << endl
             << "1) Today's publicity" << endl
             << "2) Not today's publicity" << endl;
        cin >> option;

        if (option != 1 && option != 2)
        {
            cout << "Enter a valid option: ";
        }
    } while (option != 1 && option != 2);

    if (option == 2) // check if the selected value is valid, because it can't be in the future!
        date = getSelectedDate();
    else
        date = getCurrentDate();
    getDailyPublicity(name, date);
}

void TimeRestaurant::getDailySales()
{
    Date date;
    string name;
    int option;

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);

    do
    {
        cout << "Choose an option:\n"
             << "1) Today's sales\n"
             << "2) Not today's sales\n";
        cin >> option;

        if (option != 1 && option != 2)
        {
            cout << "Enter a valid option: ";
        }

    } while (option != 1 && option != 2);

    if (option == 2)
        date = getSelectedDate();
    else
        date = getCurrentDate();
    getDailySales(name, date);
}

void TimeRestaurant::getCuisineRating()
{
    int month, year, option;
    Date selected;
    string name;

    cout << "Enter the name of the restaurant ";
    getline(cin >> ws, name);

    cout << "Do you want to know the cuisine ratings of:\n 1) This month: \n 2) Another month: \n 3)Period of time: ";
    cin >> option;

    while (option != 1 && option != 2 && option != 3) // if an option is not in the menu
    {
        cout << "not valid try again: ";
        cin >> option;
    }

    if (option == 2) // in another month, get the month and the year from the user
    {
        do
        {
            cout << "Enter the year: ";
            selected.year = selected.set_year();
            cout << "Enter the month: ";
            selected.month = selected.set_month();
            if (selected.year > getCurrentDate().year || selected.month > getCurrentDate().month)
                cout << "not valid date try again: ";
        } while (selected.year > getCurrentDate().year || selected.month > getCurrentDate().month);
        getCuisineRatingMonth(name, selected); // get the rating based on the selected option
    }

    // period of time
    else if (option == 3)
    {
        Date start_date, end_date;
        do
        {
            do
            {
                cout << "Enter the start date: \n "; // get the start date
                start_date.year = start_date.set_year();
                start_date.month = start_date.set_month();
                start_date.day = start_date.set_day();

                if (DateCheck(start_date, getCurrentDate()))
                    cout << "invalid start date, try again";

            } while (DateCheck(start_date, getCurrentDate())); // while the start date is not in the future

            do
            {
                cout << "Enter end date: \n "; // get the end date
                end_date.year = end_date.set_year();
                end_date.month = end_date.set_month();
                end_date.day = end_date.set_day();
                if (DateCheck(end_date, getCurrentDate()))
                    cout << "invalid end date, try again";

            } while (DateCheck(end_date, getCurrentDate())); // while the start date is not in the future

            if (DateCheck(start_date, end_date)) // if the start date is after the end date (invalid)
                cout << "invalid period try again: ";
        } while (DateCheck(start_date, end_date));

        getCuisineRatingPeriod(name, start_date, end_date); // call private function to retrieve data
    }
    // current date
    else
    {
        selected = getCurrentDate();
        getCuisineRatingMonth(name, selected); // call private function to retrieve data
    }
}

void TimeRestaurant::modify()
{
    string name;
    int choice, year, month, day;
    int option_cuisine;
    double newValue;
    Date d;

    cout << "Enter the name of the restaurant you want to modify in: ";
    getline(cin >> ws, name);

    cout << "Enter the date you want to modify in: \n"; // getting the desired date for modification
    d = getSelectedDate();

    cout << "Menu: \n 1)-Modify sales \n 2)-modify cost \n 3)-modify publicity ";
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        cout << "Choose the cuisine you want to modify its cuisine:\n 1)-Algerian \n 2)-Syrian \n 3)-Chinese \n 4)-Indian\n 5)-European";
        cin >> option_cuisine;

        // if the cuisine's option is invalid
        while (option_cuisine != 1 && option_cuisine != 2 && option_cuisine != 3 && option_cuisine != 4 && option_cuisine != 5)
        {
            cout << "Invalid choice, please try again: ";
            cin >> option_cuisine;
        }

        do
        {
            cout << "Enter the new sale's value";
            cin >> newValue;
            if (newValue < 0)
                cout << "Invalid sale, try again: ";
        } while (newValue < 0);

        modifySales(name, d, newValue, option_cuisine);
    }
    break;

    case 2:
        do
        {
            cout << "Enter the new cost's value";
            cin >> newValue;
            if (newValue < 0)
                cout << "Invalid cost, try again: ";
        } while (newValue < 0);

        modifyCost(name, d, newValue);
        break;

    case 3:
        do
        {
            cout << "Enter the new publicity's value";
            cin >> newValue;
            if (newValue < 0)
                cout << "Invalid publicity, try again: ";
        } while (newValue < 0);

        modifyPublicity(name, d, newValue); // call private function to modify
        break;

    default:
        cout << "Invalid option. \n";
        break;
    }
}

// setters

void TimeRestaurant::setTodaySale() // setting the sales of the restaurant in current date
{
    string name, choice;
    unordered_set<string> cuisine = {"algerian", "Syrian", "chinese", "indian", "european"};
    double sale;

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);

    cout << "Enter the cuisine: ";
    cin >> choice;

    // transform the choice to lower case
    for (char &l : choice)
    {
        l = tolower(l);
    }

    while (cuisine.find(choice) == cuisine.end()) // cuisine not found
    {
        cout << "Invalid option, try choosing a cuisine again. ";
        cin >> choice;
    }
    do // get the sale from user, perform validity check
    {
        cout << "Enter the sale: ";
        cin >> sale;
        if (sale < 0)
            cout << "invalid value for sale, try again: ";
    } while (sale < 0);

    Date d = getCurrentDate();
    setTodaySale(name, d, sale, choice); // call private function to set the sales
}

void TimeRestaurant::setTodayCost()
{
    string name;
    double value;

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);

    do
    {
        cout << "Enter today's cost: ";
        cin >> value;
        if (value < 0)
            cout << "invalid value for cost, try again: ";
    } while (value < 0);

    Date d = getCurrentDate();
    setTodayCost(name, d, value);
}

void TimeRestaurant::setTodayPublicity()
{
    string name;
    double value;

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);
    do
    {
        cout << "Enter today's cost on publicity: ";
        cin >> value;
        if (value < 0)
            cout << "invalid value for cost on publicity, try again: ";
    } while (value < 0);

    Date d = getCurrentDate();
    setTodayPublicity(name, d, value);
}

void TimeRestaurant::setTodayRating()
{
    string name, choice;
    Date d = getCurrentDate();
    double value;
    unordered_set<string> cuisine = {"algerian", "Syrian", "chinese", "indian", "european"};

    cout << "Enter the name of the restaurant: ";
    getline(cin >> ws, name);

    cout << "Enter the cuisine: ";
    cin >> choice;
    // transform the choice to lower case
    for (char &l : choice)
    {
        l = tolower(l);
    }

    while (cuisine.find(choice) == cuisine.end()) // cuisine not found
    {
        cout << "Invalid option, try choosing a cuisine again. ";
        cin >> choice;
    }
    setTodayRating(name, d, choice);
}

// private:
void TimeRestaurant::getDailySales(string name, Date date)
{
    ResData *theRestaurant;
    int total = 0;
    // search for the appropriate restaurant in the tree, based on the time and the name
    theRestaurant = &aptr[getPosition(date)].BStree.search(name);

    // get the sales of the resto in each cuisine for the given day
    double algerian = theRestaurant->Algerian[date.day - 1].sales,
           Syrian = theRestaurant->Syrian[date.day - 1].sales,
           chinese = theRestaurant->Chinese[date.day - 1].sales,
           indian = theRestaurant->Indian[date.day - 1].sales,
           european = theRestaurant->European[date.day - 1].sales;

    cout << "The daily sales of: " << name << endl
         << " in the Algerian cuisine: " << algerian << endl
         << " in the Syrian cuisine is: " << Syrian << endl
         << " in the Chinese cuisine is: " << chinese << endl
         << " in the Indian cuisine is: " << indian << endl
         << " in the European cuisine is: " << european << endl;
    if (algerian != -1)
        total += algerian;
    if (Syrian != -1)
        total += Syrian;
    if (chinese != -1)
        total += chinese;
    if (indian != -1)
        total += indian;
    if (european != -1)
        total += european;
    cout << "Total is: " << total << endl;
}

void TimeRestaurant::getDailyCost(string name, Date date)
{
    ResData *theRestaurant;
    // locate the restaurant based on the date (in aptr) and the name of the restaurant in the tree
    theRestaurant = &aptr[getPosition(date)].BStree.search(name);
    cout << "The daily cost of " << name << " is: " << theRestaurant->cost[date.day - 1] << endl;
}

void TimeRestaurant::getDailyPublicity(string name, Date date)
{
    ResData *theRestaurant;
    // locate the restaurant based on the date (in aptr) and the name of the restaurant in the tree
    theRestaurant = &aptr[getPosition(date)].BStree.search(name);
    cout << "The daily average of publicity of: " << name << " is:  "
         << theRestaurant->publicity[date.day - 1] << endl;
}

void TimeRestaurant::getCuisineRatingMonth(string name, Date date)
{
    ResData *theRestaurant;
    // locate the restaurant based on the date (in aptr) and the name of the restaurant in the tree
    theRestaurant = &aptr[getPosition(date)].BStree.search(name);
    cout << "The rating of " << name << " in: " << date.month << "/" << date.year << ":" << endl;

    // get the sum of all the ratings of the days in the given month for each cuisine
    cout << "Algerian cuisine: " << theRestaurant->calc_sum_Rating(theRestaurant->Algerian) << endl
         << "Syrain cuisine: " << theRestaurant->calc_sum_Rating(theRestaurant->Syrian) << endl
         << "Chinese cuisine: " << theRestaurant->calc_sum_Rating(theRestaurant->Chinese) << endl
         << "Indian cuisine: " << theRestaurant->calc_sum_Rating(theRestaurant->Indian) << endl
         << "European cuisine: " << theRestaurant->calc_sum_Rating(theRestaurant->European) << endl;
}

void TimeRestaurant::getCuisineRatingPeriod(string name, Date start_date, Date end_date)
{
    ResData *theRestaurant;

    // the indexes in the aptr array and cuisines arrays
    int i = getPosition(start_date), j = getPosition(end_date), k = start_date.day - 1, l = end_date.day - 1;

    // counters
    double alg = 0, syr = 0, chi = 0, ind = 0, euro = 0;
    int numPeopleAlgerian = 0, numPeopleSyrian = 0, numPeopleChinese = 0, numPeopleIndian = 0, numPeopleEuropean = 0;

    cout << "The rating of: " << name << " from: " << start_date.day << "/" << start_date.month << "/" << start_date.year << " to "
         << end_date.day << "/" << end_date.month << "/" << end_date.year << " :" << endl;

    if (i < j) // in diffrent months
    {
        theRestaurant = &aptr[i].BStree.search(name);
        while (k < 31 && i < j) // as long as we are not in the end of the month,
        {                       // get the rating of the days in the interval for each cuisine
            alg += theRestaurant->Algerian[k].average_rating;
            numPeopleAlgerian++;
            syr += theRestaurant->Syrian[k].average_rating;
            numPeopleSyrian++;
            chi += theRestaurant->Chinese[k].average_rating;
            numPeopleChinese++;
            ind += theRestaurant->Indian[k].average_rating;
            numPeopleIndian++;
            euro += theRestaurant->European[k].average_rating;
            numPeopleEuropean++;
            k++;
        }
        i++;   // move to the next month
        k = 0; // position of the first day in the next month
    }
    if (i == j) // same month
    {
        theRestaurant = &aptr[i].BStree.search(name);
        while (k <= l) // while we didn't exceed the end day of the choosen period of time get the sum of the ratings
        {
            alg += theRestaurant->Algerian[k].average_rating;
            numPeopleAlgerian++;
            syr += theRestaurant->Syrian[k].average_rating;
            numPeopleSyrian++;
            chi += theRestaurant->Chinese[k].average_rating;
            numPeopleChinese++;
            ind += theRestaurant->Indian[k].average_rating;
            numPeopleIndian++;
            euro += theRestaurant->European[k].average_rating;
            numPeopleEuropean++; // increment the #of people that rated
            k++;                 // move to the next day
        }
    }
    cout << "Algerian cuisine: " << alg / numPeopleAlgerian << endl // calculate the average
         << "Syrain cuisine: " << syr / numPeopleSyrian << endl
         << "Chinese cuisine: " << chi / numPeopleChinese << endl
         << "Indian cuisine: " << ind / numPeopleIndian << endl
         << "European cuisine: " << euro / numPeopleEuropean << endl;
}

void TimeRestaurant::modifySales(string name, Date d, double newValue, char option_cuisine)
{
    double oldSale;
    ResData *TheRestaurant;
    // locate the restaurant
    TheRestaurant = &aptr[getPosition(d)].BStree.search(name);

    switch (option_cuisine)
    {
    case 1:
        oldSale = TheRestaurant->Algerian[d.day - 1].sales;    // store the previous value
        TheRestaurant->Algerian[d.day - 1].SetSales(newValue); // change to the new value
        // update the sum which is the previous sum - the removed value plus the new value
        TheRestaurant->Algerian[31].SetSales(TheRestaurant->Algerian[31].sales - oldSale + newValue);
        break;

    case 2:
        oldSale = TheRestaurant->Syrian[d.day - 1].sales;
        TheRestaurant->Syrian[d.day - 1].SetSales(newValue);
        // update the sum
        TheRestaurant->Syrian[31].SetSales(TheRestaurant->Syrian[31].sales - oldSale + newValue);
        break;

    case 3:

        oldSale = TheRestaurant->Chinese[d.day - 1].sales;
        TheRestaurant->Chinese[d.day - 1].SetSales(newValue);
        // update the sum
        TheRestaurant->Chinese[31].SetSales(TheRestaurant->Chinese[31].sales - oldSale + newValue);
        break;

    case 4:

        oldSale = TheRestaurant->Indian[d.day - 1].sales;
        TheRestaurant->Indian[d.day - 1].SetSales(newValue);
        // update the sum
        TheRestaurant->Indian[31].SetSales(TheRestaurant->Indian[31].sales - oldSale + newValue);
        break;

    case 5:

        oldSale = TheRestaurant->European[d.day - 1].sales;
        TheRestaurant->European[d.day - 1].SetSales(newValue);
        // update the last cell in the array by removing the initial value we had and adding the new value
        TheRestaurant->European[31].SetSales(TheRestaurant->European[31].sales - oldSale + newValue);
        break;

    default:
        cout << "Invalid choice of cuisine. \n";
        break;
    }
}

void TimeRestaurant::modifyCost(string name, Date d, double newValue)
{
    double oldCos;
    ResData *TheRestaurant = &aptr[getPosition(d)].BStree.search(name);

    oldCos = TheRestaurant->cost[d.day - 1];     // store the previous value
    TheRestaurant->setCost(d.day - 1, newValue); // set the new value
    // update the sum (take the old sum - the old value in the position given by the user + the new cost, position 31 ie last one)
    TheRestaurant->setCost(31, TheRestaurant->cost[31] - oldCos + newValue);
}

void TimeRestaurant::modifyPublicity(string name, Date d, double newValue)
{
    double oldPub;
    ResData *TheRestaurant = &aptr[getPosition(d)].BStree.search(name);
    oldPub = TheRestaurant->publicity[d.day - 1];
    TheRestaurant->setPublicity(d.day - 1, newValue);
    // update the sum
    TheRestaurant->setPublicity(31, TheRestaurant->publicity[31] - oldPub + newValue);
}

void TimeRestaurant::setTodaySale(string name, Date d, double sale, string choice)
{
    ResData *theRestaurant;

    change(d); // if it's the first insertion change the dates accordingly
    reSize(d); // if the array is full resize
    int position = getPosition(d);

    if (checkIfNextDate() && position != -1) // if the current date month or year is diffrent from the last one, take a copy of the last tree and insert it in the appropriate position
    {                                        // should insert a bst in next current date->month cell
        aptr[position].BStree = aptr[getPosition(getLastDate())].BStree;
    }
    // if the restaurant is not present then insert it in the position
    if (!aptr[position].BStree.ContainsRes(name))
    {
        ResData Rest;
        Rest.NameR = name;
        aptr[position].BStree.InsertRes(Rest);
    }
    // search the node of the restaurant in the whole structure
    theRestaurant = &aptr[getPosition(d)].BStree.search(name);
    // set the sales in the appropraite place which is the day-1 because of the start index which is 0
    if (choice == "algerian")
    {
        theRestaurant->Algerian[d.day - 1].SetSales(sale);
        // update the sum of the sales
        theRestaurant->Algerian[31].SetSales(theRestaurant->Algerian[31].sales + sale);
    }
    else if (choice == "Syrian")
    {
        theRestaurant->Syrian[d.day - 1].SetSales(sale);
        theRestaurant->Syrian[31].SetSales(theRestaurant->Syrian[31].sales + sale);
    }
    else if (choice == "chinese")
    {
        theRestaurant->Chinese[d.day - 1].SetSales(sale);
        theRestaurant->Chinese[31].SetSales(theRestaurant->Chinese[31].sales + sale);
    }
    else if (choice == "indian")
    {
        theRestaurant->Indian[d.day - 1].SetSales(sale);
        theRestaurant->Indian[31].SetSales(theRestaurant->Indian[31].sales + sale);
    }
    else if (choice == "european")
    {
        theRestaurant->European[d.day - 1].SetSales(sale);
        theRestaurant->European[31].SetSales(theRestaurant->European[31].sales + sale);
    }
    setLastDate(d); // set the new last date of insertion
}
void TimeRestaurant::setTodayCost(string name, Date d, double value)
{
    change(d); // if it's the first insertion change the dates accordingly
    reSize(d); // resize if needed
    int position = getPosition(d);

    if (checkIfNextDate() && position != -1)
    { // should insert a bst in next current date->month cell
        aptr[position].BStree = aptr[getPosition(getLastDate())].BStree;
    }

    if (!aptr[position].BStree.ContainsRes(name))
    {
        ResData Rest;
        Rest.NameR = name;
        aptr[position].BStree.InsertRes(Rest);
    }

    ResData *theRestaurant;
    theRestaurant = &aptr[getPosition(d)].BStree.search(name);
    theRestaurant->setCost(d.day - 1, value);
    // update the sum
    theRestaurant->setCost(31, theRestaurant->cost[31] + value);
    setLastDate(d); // set the new last date of insertion
}

void TimeRestaurant::setTodayPublicity(string name, Date d, double value)
{
    change(d);
    reSize(d);
    int position = getPosition(d);

    if (checkIfNextDate() && position != -1)
    { // should insert a bst in next current date->month cell
        aptr[position].BStree = aptr[getPosition(getLastDate())].BStree;
    }

    if (!aptr[position].BStree.ContainsRes(name))
    {
        ResData Rest;
        Rest.NameR = name;
        aptr[position].BStree.InsertRes(Rest);
    }

    ResData *theRestaurant = &aptr[getPosition(d)].BStree.search(name);

    theRestaurant->setPublicity(d.day - 1, value); // assume days are starting from 0
    // update the sum
    theRestaurant->setPublicity(31, theRestaurant->publicity[31] + value);
    setLastDate(d); // set the new last date of insertion
}

void TimeRestaurant::setTodayRating(string name, Date d, string option)
{
    change(d);
    reSize(d);
    int position = getPosition(d);

    if (checkIfNextDate() && position != -1)
    { // should insert a bst in next current date->month cell
        aptr[position].BStree = aptr[getPosition(getLastDate())].BStree;
    }

    if (!aptr[position].BStree.ContainsRes(name))
    {
        ResData Rest;
        Rest.NameR = name;
        aptr[position].BStree.InsertRes(Rest);
    }

    int sumVal = 0;
    double value;
    ResData *theRestaurant = &aptr[getPosition(d)].BStree.search(name);
    if (option == "algerian")
    {

        cout << "Enter today's ratings in the algerian cuisine, press -1 to stop: \n";
        do
        {
            cout << "Enter a rating: ";
            cin >> value;
            while (value >= 6.0 || value < 0.0 && value != -1)
            {
                cout << "Invalid value it should be between 0 & 5, please try again ";
                cin >> value;
            }
            if (value != -1)
            {
                theRestaurant->Algerian[d.day - 1].SetRating(value);
                sumVal += value;
            }
        } while (value != -1);
        // update the sum
        theRestaurant->Algerian[31].SetRating(theRestaurant->Algerian[31].average_rating + sumVal);
    }

    else if (option == "Syrian")
    {

        cout << "Enter today's ratings in the Syrian cuisine, press -1 to stop: \n";
        do
        {
            cout << "Enter a rating: ";
            cin >> value;
            while (value >= 6 || value < 0 && value != -1)
            {
                cout << "Invalid, try again ";
                cin >> value;
            }
            if (value != -1)
            {
                theRestaurant->Syrian[d.day - 1].SetRating(value);
                sumVal += value;
            }
        } while (value != -1);
        // update the sum
        theRestaurant->Syrian[31].SetRating(theRestaurant->Syrian[31].average_rating + sumVal);
    }

    else if (option == "chinese")
    {

        cout << "Enter today's ratings in the chinese cuisine, press -1 to stop: \n";
        do
        {
            cout << "Enter a rating: ";
            cin >> value;
            while (value >= 6 || value < 0 && value != -1)
            {
                cout << "Invalid, try again ";
                cin >> value;
            }
            if (value != -1)
            {
                theRestaurant->Chinese[d.day - 1].SetRating(value);
                sumVal += value;
            }
        } while (value != -1);
        // update the sum
        theRestaurant->Chinese[31].SetRating(theRestaurant->Chinese[31].average_rating + sumVal);
    }

    else if (option == "indian")
    {
        cout << "Enter today's ratings in the indian cuisine, press -1 to stop: \n";
        do
        {
            cout << "Enter a rating: ";
            cin >> value;
            while (value >= 6 || value < 0 && value != -1)
            {
                cout << "Invalid, try again ";
                cin >> value;
            }
            if (value != -1)
            {
                theRestaurant->Indian[d.day - 1].SetRating(value);
                sumVal += value;
            }
        } while (value != -1);
        // update the sum
        theRestaurant->Indian[31].SetRating(theRestaurant->Indian[31].average_rating + sumVal);
    }

    else if (option == "european")
    {
        cout << "Enter today's ratings in the european cuisine, press -1 to stop: \n";
        do
        {
            cout << "Enter a rating: ";
            cin >> value;
            while (value >= 6 || value < 0 && value != -1)
            {
                cout << "Invalid, try again ";
                cin >> value;
            }
            if (value != -1)
            {
                theRestaurant->European[d.day - 1].SetRating(value);
                sumVal += value;
            }
            if (value == -1)
                break;
        } while (true);
        // update the sum
        theRestaurant->European[31].SetRating(theRestaurant->European[31].average_rating + sumVal);
    }
}
// this is the function that will be called in the fillFromFile
void TimeRestaurant::setTodayRating(string name, Date d, string option, double value)
{
    change(d);
    reSize(d);
    int position = getPosition(d);

    if (checkIfNextDate() && position != -1)
    { // should insert a bst in next current date->month cell
        aptr[position].BStree = aptr[getPosition(getLastDate())].BStree;
    }

    if (!aptr[position].BStree.ContainsRes(name))
    {
        ResData Rest;
        Rest.NameR = name;
        aptr[position].BStree.InsertRes(Rest);
    }

    double sumVal = 0;
    ResData *theRestaurant = &aptr[getPosition(d)].BStree.search(name);
    if (option == "algerian")
    {
        theRestaurant->Algerian[d.day - 1].SetRating(value);
        sumVal += value;
        // update the sum
        theRestaurant->Algerian[31].SetRating(theRestaurant->Algerian[31].average_rating + sumVal);
    }

    else if (option == "Syrian")
    {

        theRestaurant->Syrian[d.day - 1].SetRating(value);
        sumVal += value;
        // update the sum
        theRestaurant->Syrian[31].SetRating(theRestaurant->Syrian[31].average_rating + sumVal);
    }

    else if (option == "chinese")
    {
        theRestaurant->Chinese[d.day - 1].SetRating(value);
        sumVal += value;
        // update the sum
        theRestaurant->Chinese[31].SetRating(theRestaurant->Chinese[31].average_rating + sumVal);
    }

    else if (option == "indian")
    {

        theRestaurant->Indian[d.day - 1].SetRating(value);
        sumVal += value;
        // update the sum
        theRestaurant->Indian[31].SetRating(theRestaurant->Indian[31].average_rating + sumVal);
    }

    else if (option == "european")
    {
        theRestaurant->European[d.day - 1].SetRating(value);
        sumVal += value;
        // update the sum
        theRestaurant->European[31].SetRating(theRestaurant->European[31].average_rating + sumVal);
    }
}

// place related
void TimeRestaurant::getRatio(wilaya mywilaya)
{
    int choice, dateOption;
    Date d, d2;
    string name, wil;
    double sal, pub;

    cout << "Get the ratio of the monthy sales to the amount spent on publicity for: " << endl;

    do
    {
        cout << "1)- Specific restaurant" << endl
             << "2)- In a city" << endl
             << "3)- In a wilaya " << endl
             << "4)- In the country" << endl;
        cin >> choice;
        if (choice != 1 && choice != 2 && choice != 3 && choice != 4)
        {
            cout << "Wrong option please try again" << endl;
        }

    } while (choice != 1 && choice != 2 && choice != 3 && choice != 4);

    do
    {
        cout << "Do you want to get the ratio of: \n 1)-This month\n 2)-Another month\n 3)-Period of time";
        cin >> dateOption;
        if (dateOption != 1 && dateOption != 2 && dateOption != 3)
            cout << "Please enter a valid option.";

    } while (dateOption != 1 && dateOption != 2 && dateOption != 3);
    if (dateOption == 1) // get for the current month
        d = getCurrentDate();
    else if (dateOption == 2) // get in another month
    {
        d.year = d.set_year();
        d.month = d.set_month();
    }
    else if (dateOption == 3) // get in a period of time
    {
        do
        {
            cout << "Enter your start date: ";
            d = getSelectedDate();
            cout << "Enter your end date: ";
            d2 = getSelectedDate();
            // if the start date is bigger than the end date or one of the entered dates is in the future
            if (DateCheck(d, d2) || DateCheck(d, getCurrentDate()) || DateCheck(d2, getCurrentDate()))
                cout << "try again";
        } while (DateCheck(d, d2) || DateCheck(d, getCurrentDate()) || DateCheck(d2, getCurrentDate()));
    }

    switch (choice)
    {
    case 1: // case we want for a specific restaurant
        cout << "Enter the name of the restaurant: ";
        getline(cin >> ws, name);

        if (dateOption == 1 || dateOption == 2)

            cout << "The ratio of " << name << " in " << d.year << "/" << d.month << " is: " << getRatioRestaurant(d, d, name) << endl;

        else if (dateOption == 3)
        {
            cout << "The ratio of " << name << " from " << d.day << "/" << d.month << "/" << d.year
                 << " to " << d2.day << "/" << d2.month << "/" << d2.year << " is: " << getRatioRestaurant(d, d2, name) << endl;
        }
        break;

    case 2: // case we want for a given city
        cout << "Enter the name of the city: ";
        getline(cin >> ws, name);

        cout << "Enter the wilaya this city is in: ";
        getline(cin >> ws, wil);

        if (dateOption == 1 || dateOption == 2)
            RatioCity(d, d, name, wil, mywilaya); // call private function by setting the two date as the same since we are not in a period of time
        else
            RatioCity(d, d2, name, wil, mywilaya); // call the function by setting the dates accordingly since we are in a period of time
        break;

    case 3: // in a wilaya
        cout << "Enter the wilaya: ";
        getline(cin >> ws, wil);

        if (dateOption == 1 || dateOption == 2)
            RatioWilaya(d, d, wil, mywilaya);
        else
            RatioWilaya(d, d2, wil, mywilaya);
        break;
    case 4: // country
        if (dateOption == 1 || dateOption == 2)
            RatioCountry(d, d, mywilaya);
        else
            RatioCountry(d, d2, mywilaya);
        break;

    default:
        cout << "This program should never reach this part" << endl;
        break;
    }
}

void TimeRestaurant::RatioCity(Date start, Date end, string cit, string wil, wilaya country)
{
    double val = 0;
    int counter = 0;
    if (country.doesExistCity(wil, cit)) // if the city exists
    {
        // get the restaurants of the city
        vector<wilaya::arrayCaseRList> districts = country.getCityRestaurants(wil, cit);

        cout << "Wilaya: " << wil << endl;
        cout << "City: " << cit << endl;

        for (auto &element : districts) // for each district
        {
            cout << "District: " << element.regionName << endl;

            const wilaya::RestauNode *restoList = element.pointerToLinkedList;
            if (restoList)
            {
                while (restoList != nullptr)
                {
                    if (start.year == end.year && start.month == end.month) // we are in a given month
                        cout << "The ratio of " << restoList->nameRest << " in " << start.month << "/"
                             << start.year << " is: " << getRatioRestaurant(start, end, restoList->nameRest) << endl;

                    else if (start.year < end.year || (start.month < end.month && start.year == end.year)) // period of time
                        cout << "The ratio of " << restoList->nameRest << " from " << start.day << "/"
                             << start.month << "/" << start.year << " to " << end.day << " / " << end.month << " / " << end.year
                             << " is: " << getRatioRestaurant(start, end, restoList->nameRest) << endl;
                    // calculate the total for the wilaya
                    val += getRatioRestaurant(start, end, restoList->nameRest); // total of all the resto
                    counter++;                                                  // number of resto
                    restoList = restoList->next;
                }
                if (counter != 0)
                    cout << "The average ratio of the city " << cit << " is " << val / counter << endl;
            }
            else
            {
                cout << "There are no restaurants in this City." << endl;
            }
        }
    }
    else
    {
        cout << "City does not exist" << endl;
    }
}

void TimeRestaurant::RatioWilaya(Date start, Date end, string wilayaName, wilaya country)
{
    double val = 0;
    int counter = 0;
    if (country.doesExistWilaya(wilayaName))
    {
        vector<wilaya::arrayCase1To2> cities = country.getWilayaRestaurants(wilayaName);
        cout << "Wilaya: " << wilayaName << endl;
        for (const auto &city : cities)
        {
            cout << "City: " << city.regionName << endl;

            for (const auto &district : *(city.pointerToVector))
            {
                cout << "District: " << district.regionName << endl;

                const wilaya::RestauNode *restoList = district.pointerToLinkedList;
                if (restoList)
                {
                    while (restoList != nullptr)
                    {
                        if (start == end) // if we are in the same month
                            cout << "The ratio of " << restoList->nameRest << " in " << start.month << "/"
                                 << start.year << " is: " << getRatioRestaurant(start, end, restoList->nameRest) << endl;
                        else // we are in a period of time
                            cout << "The ratio of " << restoList->nameRest << " from " << start.day << "/"
                                 << start.month << "/" << start.year << " to " << end.day << " / " << end.month << " / " << end.year
                                 << " is: " << getRatioRestaurant(start, end, restoList->nameRest) << endl;

                        val += getRatioRestaurant(start, end, restoList->nameRest);
                        counter++;
                        restoList = restoList->next;
                    }
                    if (counter != 0)
                        cout << "The average ratio of the wilaya " << wilayaName << val / counter << endl;
                }
                else
                {
                    cout << "There are no restaurants in this Wilaya." << endl;
                }
            }
        }
    }
    else
    {
        cout << "Wilaya does not exist" << endl;
    }
}

void TimeRestaurant::RatioCountry(Date start, Date end, wilaya country)
{
    double val = 0;
    int counter = 0;

    cout << country.getCountryName() << ": " << endl;
    vector<wilaya::arrayCase> wilayas = country.getCountryRestaurants();
    for (auto &element1 : wilayas)
    {
        cout << "wilaya: " << element1.regionName << endl;
        vector<wilaya::arrayCase1To2> cities = *(element1.pointerToVector);

        for (auto &element : cities)
        {
            cout << "City: " << element.regionName << endl;
            vector<wilaya::arrayCaseRList> districts = *(element.pointerToVector);

            for (auto &element2 : districts)
            {
                cout << "District: " << element2.regionName << endl;

                const wilaya::RestauNode *restoList = element2.pointerToLinkedList;
                if (restoList)
                {
                    while (restoList != nullptr)
                    {
                        if (start == end)
                            cout << "The ratio of " << restoList->nameRest << " in " << start.month << "/"
                                 << start.year << " is: " << getRatioRestaurant(start, end, restoList->nameRest) << endl;
                        else
                            cout << "The ratio of " << restoList->nameRest << " from " << start.day << "/"
                                 << start.month << "/" << start.year << " to " << end.day << " / " << end.month << " / " << end.year
                                 << " is: " << getRatioRestaurant(start, end, restoList->nameRest) << endl;
                        val += getRatioRestaurant(start, end, restoList->nameRest);
                        counter++;
                        restoList = restoList->next;
                    }
                }
                if (counter != 0)
                    cout << "The average ratio of the Country " << country.getCountryName() << (val / counter * 100) / 100 << endl;
                else
                {
                    cout << "There are no restaurants in this Country." << endl;
                }
            }
        }
    }
}

double TimeRestaurant::getRatioRestaurant(Date d, Date d2, string name) // start date and end date
{
    double sal, pub;
    if (d.month == d2.month && d.year == d2.year) // here we're in the same month
    {
        ResData *theRestaurant = &aptr[getPosition(d)].BStree.search(name); // locate the restaurant
        sal = theRestaurant->calc_sum_Sales(theRestaurant->Algerian) +      // calculate the sales sum for each cuisine
              theRestaurant->calc_sum_Sales(theRestaurant->Syrian) +
              theRestaurant->calc_sum_Sales(theRestaurant->Chinese) +
              theRestaurant->calc_sum_Sales(theRestaurant->Indian) +
              theRestaurant->calc_sum_Sales(theRestaurant->European);
        pub = theRestaurant->calc_sum(theRestaurant->publicity); // calculate the sum of the publicity
        if (pub != 0)
            return (sal / pub * 100) / 100; // do the *100 /100 to get two decimal degits
        return 0;
    }
    else if (d.year < d2.year || (d.month < d2.month && d.year == d2.year)) // period of time
    {
        int i = getPosition(d), j = getPosition(d2), k = d.day - 1, l = d2.day - 1;
        ResData *theRestaurant;
        if (i < j) // in diffrent months
        {
            while (i != j)
            {
                theRestaurant = &aptr[i].BStree.search(name);
                while (k < 31) // as long as we are not in the end of the month
                {
                    sal += theRestaurant->Algerian[k].sales +
                           theRestaurant->Syrian[k].sales +
                           theRestaurant->Chinese[k].sales +
                           theRestaurant->Indian[k].sales +
                           theRestaurant->European[k].sales;

                    pub += theRestaurant->publicity[k];
                    k++;
                }
                i++;
                k = 0;
            }
        }
        if (i == j) // same month
        {
            theRestaurant = &aptr[i].BStree.search(name);
            while (k <= l)
            {
                sal += theRestaurant->Algerian[k].sales +
                       theRestaurant->Syrian[k].sales +
                       theRestaurant->Chinese[k].sales +
                       theRestaurant->Indian[k].sales +
                       theRestaurant->European[k].sales;

                pub += theRestaurant->publicity[k];
                k++;
            }
        }
        if (pub != 0)
            return (sal / pub * 100) / 100;
        return 0;
    }
}

double TimeRestaurant::districtSales(string wil, string cit, string dist, const wilaya::RestauNode *restoList)
{
    double sal = 0.0;
    ResData *theRestaurant;
    int position = getPosition(getLastDate());

    while (restoList != nullptr) // while not at the end of the list
    {
        for (int i = 0; i < position; i++) // from the start date to the end date in the aptr array
        {
            string name;
            name = restoList->nameRest;
            if (aptr[i].BStree.ContainsRes(name)) // if the restaurant is present calculate the sales for each cuisine
            {
                theRestaurant = &aptr[i].BStree.search(name);
                sal += theRestaurant->calc_sum_Sales(theRestaurant->Algerian) +
                       theRestaurant->calc_sum_Sales(theRestaurant->Syrian) +
                       theRestaurant->calc_sum_Sales(theRestaurant->Chinese) +
                       theRestaurant->calc_sum_Sales(theRestaurant->Indian) +
                       theRestaurant->calc_sum_Sales(theRestaurant->European);

                break; // if resto found break move to the next one
            }
        }
        if (restoList->next != nullptr)
        {
            restoList = restoList->next;
        }
        else
            break;
    }
    return sal;
}
void TimeRestaurant::districtSales(wilaya mywilaya)
{
    string dist, cit, wil;

    cout << "Enter the name of the wilaya this disctrict is in: ";
    cin >> wil;
    cout << "Enter the name of the city this disctrict is in: ";
    cin >> cit;
    cout << "Enter the name of the district: ";
    cin >> dist;

    const wilaya::RestauNode *restoList = mywilaya.getDistrictRestaurants(wil, cit, dist); // the list of the resto in that district
    cout << "Sales of district: " << dist << districtSales(wil, cit, dist, restoList) << endl;
}

void TimeRestaurant::citySales(wilaya mywilaya)
{
    string cit, wil;
    double sal = 0;

    cout << "Enter the name of the wilaya this city is in: ";
    getline(cin, wil);

    cout << "Enter the name of the city: ";
    getline(cin, cit);

    vector<wilaya::arrayCaseRList> districts = mywilaya.getCityRestaurants(wil, cit);

    for (auto &element2 : districts)
    {
        cout << "District: " << element2.regionName << endl;
        const wilaya::RestauNode *restoList = element2.pointerToLinkedList;
        cout << restoList->nameRest;
        sal += districtSales(wil, cit, element2.regionName, restoList);
    }
    cout << "The total sales of " << cit << " from the start of the company to this day is: " << sal << endl;
}

void TimeRestaurant::wilayaSales(wilaya mywilaya)
{
    string wil;
    double sal = 0;

    cout << "Enter the name of the wilaya: ";
    getline(cin, wil);

    vector<wilaya::arrayCase1To2> cities = mywilaya.getWilayaRestaurants(wil);

    for (auto &element : cities)
    {
        cout << "City: " << element.regionName << endl;
        vector<wilaya::arrayCaseRList> districts = *(element.pointerToVector);
        for (auto &element2 : districts)
        {
            cout << "District: " << element2.regionName << endl;

            const wilaya::RestauNode *restoList = element2.pointerToLinkedList;
            sal += districtSales(element2.regionName, element.regionName, wil, restoList);
        }
    }
    cout << "The total sales of " << wil << " from the start to this day is: " << sal << endl;
}

void TimeRestaurant::countrySales(wilaya mywilaya)
{
    double sal = 0;

    vector<wilaya::arrayCase> wilayas = mywilaya.getCountryRestaurants();
    for (auto &element1 : wilayas)
    {
        cout << "wilaya: " << element1.regionName << endl;
        vector<wilaya::arrayCase1To2> cities = *(element1.pointerToVector);

        for (auto &element : cities)
        {
            cout << "City: " << element.regionName << endl;

            vector<wilaya::arrayCaseRList> districts = *(element.pointerToVector);
            for (auto &element2 : districts)
            {
                cout << "District: " << element2.regionName << endl;
                const wilaya::RestauNode *restoList = element2.pointerToLinkedList;

                sal += districtSales(element1.regionName, element.regionName, element2.regionName, restoList);
                cout << "Sales of district " << element2.regionName << " :" << districtSales(element1.regionName, element.regionName, element2.regionName, restoList) << endl;
            }
            cout << endl;
        }
    }
    cout << "The total sales of Algeria from the start of the company to this day is: " << sal << endl;
}

// file handling
TimeRestaurant::~TimeRestaurant()
{
    makeAllEmpty();
    writeAllDataToFile();
    delete[] aptr;
}

void TimeRestaurant::fillFromCuisineFiles()
{
    ifstream file;
    file.open("sales.txt", ios::in);

    if (!file.is_open())
    {
        cerr << "Error couldn't open sales file" << endl;
        return;
    }
    file.seekg(0);

    string line;

    string year, month, name, day, sale, avrg, cuisine;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue; // Skip empty lines if present
        }
        istringstream iss(line);
        getline(iss, year, ',');
        getline(iss, month, ',');
        getline(iss, day, ',');
        getline(iss, cuisine, ',');
        getline(iss, name, ',');
        getline(iss, sale, ',');
        getline(iss, avrg, '/');

        int y = stoi(year);
        int m = stoi(month);
        int d = stoi(day);
        double sal = stod(sale);
        double average = stod(avrg);

        Date date(d, m, y);
        change(date); // change the dates of the aptr if first insertion
        reSize(date); // resize if needed

        if (!aptr[getPosition(date)].BStree.ContainsRes(name)) // if the resto is not present insert it
        {
            ResData Rest;
            Rest.NameR = name;
            aptr[getPosition(date)].BStree.InsertRes(Rest);
        }

        ResData *therest = &aptr[getPosition(date)].BStree.search(name); // locate the resto
        // check which cuisine set the rating and the sales for that day read from the file (position date.day-1) since we are starting from 0
        if (cuisine == "algerian")
        {
            therest->Algerian[date.day - 1].SetRating(average);
            therest->Algerian[date.day - 1].SetSales(sal);
        }
        else if (cuisine == "chinese")
        {
            therest->Chinese[date.day - 1].SetSales(sal);
            therest->Chinese[date.day - 1].SetRating(average);
        }
        else if (cuisine == "indian")
        {
            therest->Indian[date.day - 1].SetRating(average);
            therest->Indian[date.day - 1].SetSales(sal);
        }
        else if (cuisine == "european")
        {
            therest->European[date.day - 1].SetRating(average);
            therest->European[date.day - 1].SetSales(sal);
        }
        else if (cuisine == "syrian")
        {
            therest->Syrian[date.day - 1].SetRating(average);
            therest->Syrian[date.day - 1].SetSales(sal);
        }
        setLastDate(date); // change the last insertion date
    }
    file.close();
}

void TimeRestaurant::fillFromCostFile()
{
    ifstream file("cost.txt");
    file.seekg(0); // get the cursor to the start of the file
    if (!file.is_open())
    {
        cerr << "Error opening cost's file" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);
        string year, month, day, name, co;

        getline(ss, year, ',');
        getline(ss, month, ',');
        getline(ss, day, ',');
        getline(ss, name, ',');
        getline(ss, co, '/');

        int y = stoi(year);
        int m = stoi(month);
        int d = stoi(day);
        double cost = stod(co);

        Date date(d, m, y);
        change(date);
        reSize(date);

        if (!aptr[getPosition(date)].BStree.ContainsRes(name)) // if not found insert it
        {
            ResData Rest;
            Rest.NameR = name;
            aptr[getPosition(date)].BStree.InsertRes(Rest);
        }

        ResData *therest = &aptr[getPosition(date)].BStree.search(name); // locate the resto
        therest->setCost(date.day - 1, cost);                            // set the cost
        setLastDate(date);                                               // change the last insertion date
    }

    file.close();
}

void TimeRestaurant::fillFromPublicityFile()
{
    ifstream file("publicity.txt");

    if (!file.is_open())
    {
        cerr << "Error opening publicity's file!" << endl;
        return;
    }
    file.seekg(0);
    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);
        string year, month, day, name, pub;

        getline(ss, year, ',');
        getline(ss, month, ',');
        getline(ss, day, ',');
        getline(ss, name, ',');
        getline(ss, pub, '/');

        int y = stoi(year);
        int m = stoi(month);
        int d = stoi(day);
        double p = stod(pub);

        Date date(d, m, y);
        change(date);
        reSize(date);

        if (!aptr[getPosition(date)].BStree.ContainsRes(name))
        {
            ResData Rest;
            Rest.NameR = name;
            aptr[getPosition(date)].BStree.InsertRes(Rest);
        }

        ResData *therest = &aptr[getPosition(date)].BStree.search(name);
        therest->setPublicity(date.day - 1, p);
        setLastDate(date);
    }

    file.close();
}

void TimeRestaurant::makeAllEmpty() // empty all the files
{
    makeCuisineEmpty();
    makeCostEmpty();
    makePublicityEmpty();
}
void TimeRestaurant::makeCuisineEmpty()
{
    ofstream outS;
    outS.open("sales.txt", ofstream::out | ofstream::trunc);
    outS.close();
}
void TimeRestaurant::makeCostEmpty()
{
    ofstream outS;
    outS.open("cost.txt", ofstream::out | ofstream::trunc);
    outS.close();
}
void TimeRestaurant::makePublicityEmpty()
{
    ofstream outS;
    outS.open("publicity.txt", ofstream::out | ofstream::trunc);
    outS.close();
}

void TimeRestaurant::writeAllDataToFile() // from the constructed data write in files
{
    ofstream cuisineFile("sales.txt", ios::app);
    ofstream costFile("cost.txt", ios::app);
    ofstream PublicityFile("publicity.txt", ios::app);

    if (!cuisineFile.is_open() || !costFile.is_open() || !PublicityFile.is_open())
    {
        cerr << "Error coudln't open the file." << endl;
        return;
    }
    Date d = getLastDate();
    int position = getPosition(d);

    for (int i = 0; i <= position; i++)
    {
        writeCuisineToFile(cuisineFile, aptr[i].BStree.getRoot(), "algerian", i);
        cuisineFile.flush();
        writeCuisineToFile(cuisineFile, aptr[i].BStree.getRoot(), "syrian", i);
        cuisineFile.flush();
        writeCuisineToFile(cuisineFile, aptr[i].BStree.getRoot(), "chinese", i);
        cuisineFile.flush();
        writeCuisineToFile(cuisineFile, aptr[i].BStree.getRoot(), "indian", i);
        cuisineFile.flush();
        writeCuisineToFile(cuisineFile, aptr[i].BStree.getRoot(), "european", i);
        cuisineFile.flush();
        writeCostToFile(costFile, aptr[i].BStree.getRoot(), i);
        costFile.flush();
        writePublicityToFile(PublicityFile, aptr[i].BStree.getRoot(), i);
        PublicityFile.flush();
    }
    costFile.close();
    PublicityFile.close();
    cuisineFile.close();
}

void TimeRestaurant::writeCuisineToFile(ofstream &file, BinaryNode *node, const string &cuisine, int indexAptr)
{ // use the index of aptr to print the year and month in each line
    if (node != nullptr)
    {

        for (int j = 0; j < 31; j++)
        {
            int sales = -1;
            double avgRating = -1;

            if (cuisine == "algerian")
            {
                sales = node->element.Algerian[j].sales;
                avgRating = node->element.Algerian[j].average_rating;
            }
            else if (cuisine == "syrian")
            {
                sales = node->element.Syrian[j].sales;
                avgRating = node->element.Syrian[j].average_rating;
            }
            else if (cuisine == "chinese")
            {
                sales = node->element.Chinese[j].sales;
                avgRating = node->element.Chinese[j].average_rating;
            }
            else if (cuisine == "indian")
            {
                sales = node->element.Indian[j].sales;
                avgRating = node->element.Indian[j].average_rating;
            }
            else if (cuisine == "european")
            {
                sales = node->element.European[j].sales;
                avgRating = node->element.European[j].average_rating;
            }

            if (sales != -1)
            {
                file << aptr[indexAptr].d.year << "," << aptr[indexAptr].d.month << ","
                     << j + 1 << "," << cuisine << "," << node->element.NameR << ",";

                file << sales;
                if (avgRating != -1)
                    file << "," << avgRating << "/" << endl;
                else
                    file << "/" << endl;
            }
        }

        writeCuisineToFile(file, node->left, cuisine, indexAptr);
        writeCuisineToFile(file, node->right, cuisine, indexAptr);
    }
}

void TimeRestaurant::writeCostToFile(ofstream &file, BinaryNode *node, int indexAptr)
{
    if (node != nullptr) // if the resto node is not null
    {
        for (int j = 0; j < 31; j++) // iterate over the days, stop before the last cell which contains the cumulative
        {
            if (node->element.cost[j] != 0) // if the cost is not 0 (ie: there is entered data in there)
            {
                file << aptr[indexAptr].d.year << "," << aptr[indexAptr].d.month << ",";
                file << j + 1 << "," << node->element.NameR << "," << node->element.cost[j] << "/" << endl;
            }
        }
        writeCostToFile(file, node->left, indexAptr); // do the same for the left part and right part of the tree
        writeCostToFile(file, node->right, indexAptr);
    }
}

void TimeRestaurant::writePublicityToFile(ofstream &file, BinaryNode *node, int indexAptr)
{
    if (node != nullptr)
    {
        for (int j = 0; j < 31; j++)
        {
            if (node->element.publicity[j] != 0)
            {
                file << aptr[indexAptr].d.year << "," << aptr[indexAptr].d.month << ",";
                file << j + 1 << "," << node->element.NameR << "," << node->element.publicity[j] << "/" << endl;
            }
        }
        writePublicityToFile(file, node->left, indexAptr);
        writePublicityToFile(file, node->right, indexAptr);
    }
}
