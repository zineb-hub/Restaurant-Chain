#include "PrizeAVL.h"

string Prize::checkAlgerian() const
{
    ResData algeriawinner = restautree->FindWinner(1);
    return algeriawinner.NameR;
}
string Prize::checkSyrian() const
{
    ResData Syrianwinner = restautree->FindWinner(2);
    return Syrianwinner.NameR;
}
string Prize::checkChinese() const
{
    ResData Chinesewinner = restautree->FindWinner(3);
    return Chinesewinner.NameR;
}
string Prize::checkIndian() const
{
    ResData Indianwinner = restautree->FindWinner(4);
    return Indianwinner.NameR;
}
string Prize::checkEuropean() const
{
    ResData Europeanwinner = restautree->FindWinner(5);
    return Europeanwinner.NameR;
}
// returns the name of the winner restaurant, reads from the user the month(using check to get the name object2.name)
vector<string> Prize::getWinner() const
{
    Date date;//create Date object
    date.month = date.set_month();// get the month from the user
    date.year = date.set_year();// get the year from the user
    TimeRestaurant TR;//create Time restaurant object to get the tree of the restaurants in that month
    int pos = TR.getPosition(date);// to locate the tree
    vector<string>winners(5);//vector of strings to store the winner of each cuisine
    node* aptr = TR.getArray();
    restautree->makeACopy(aptr[pos].AVLtree);//assign the tree to the appropriate according to the month  
    winners[0] = checkAlgerian();//get the Algerian prize winner
    // cout <<"here algeria "<< winners[0] << endl;
    winners[1] = checkSyrian();//get the Algerian prize winner
    // cout <<"here syria "<< winners[1] << endl;
    winners[2] = checkChinese();//get the Algerian prize winner
    //cout <<"here china"<< winners[2] << endl;
    winners[3] = checkIndian();//get the Algerian prize winner
    // cout <<"here india"<< winners[3] << endl;
    winners[4] = checkEuropean();//get the Algerian prize winner
    //cout <<"here europe "<< winners[4] << endl;

    return winners;

}
