//A basic implementation of blackjack, for IsoPlexis code assessment.

#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>

using std::string;
using std::cout;
using std::cin;
using std::endl;

//Some defintions/enums to make life easier.
#define inDeck 0
#define inPlay 1
enum Result {stand, playing, bust, blackjack};

//Card is defined as a struct as to avoid having 52 unique objects to handle.
//Deck is just a 52-length array to hold all the cards.
struct card
{
    int value;
    string face;
    string suit;
    int location;
}
Deck[53];

//A basic implementation of a player object. Holds all their attributes, and has hit functionality.
class player
{
    public:
    string name;
    int credits = 100;
    int score = 0;
    int has_ace = 0;
    Result still_in = playing;
    void hit();
    void hidden_hit();
};

//The hit functions.
void player::hit()
{
    int drawn = rand() % 52;
    //Here to ensure that the same card is not in play multiple times.
    while(Deck[drawn].location == 1)
    {
        int drawn = rand() % 52;
    }
    Deck[drawn].location = 1;
    cout << this->name << " has drawn the " << Deck[drawn].face << " of " << Deck[drawn].suit << endl;
    this->score += Deck[drawn].value;
    if(Deck[drawn].value == 11)
        this->has_ace += 1;
    if(this->score > 21 && this->has_ace > 0)
    {
        this->score -= 10;
        this->has_ace -= 1;
    }
    cout << this->name << "'s current score is: " << this->score << endl;
    if(this->score > 21)
    {
        cout << this->name << " has gone bust!" << endl;
        this->still_in = bust;
    }
    if(this->score == 21)
    {
        cout << this->name << " has gotten a blackjack!" << endl;
        this->still_in = blackjack;
    }
}

void player::hidden_hit()
{
    int drawn = rand() % 52;
    cout << this->name << " has drawn a card." << endl;
    this->score += Deck[drawn].value;
    if(this->score == 21)
        this->still_in = blackjack;
}
    
int main()
{
    //Try to ensure randomness, set up bet variable.
    srand(time(0));
    int bet = 0;
    
    //Set up player and dealer objects.
    player user;
    player dealer;
    user.name = "Player";
    dealer.name = "Dealer";
    
    //Establish the standard 52-card deck.
    int values[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    string faces[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
    "Nine", "Ten", "Jack", "Queen", "King"};
    string suits[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    int suitCount = 0;
    for (int x = 0; x < 52; x++)
    {
        Deck[x].value = values[x % 13];
        Deck[x].face = faces[x % 13];
        Deck[x].suit = suits[suitCount];
        Deck[x].location = inDeck;
        if((x + 1) % 13 == 0){
            suitCount++;
        }
    }

    //Game start! Two main loops, first the player makes their decisions and then the dealer.
    cout << "--------- Game Start. ---------" << endl;
    cout << "You currently have " << user.credits << " credits. How many would you like to bet?" << endl;
    cin >> bet;
    user.hit();
    user.hit();
    dealer.hit();
    dealer.hidden_hit();
    int move = 0;
    while (user.still_in == playing)
    {
        cout << "It is currently your turn. Would you like to hit(1) or stand(0)?" << endl;
        cin >> move;
        if(move)
            user.hit();
        else
            user.still_in = stand;
    }
    while (dealer.still_in == playing)
    {
        if (dealer.score < 17)
            dealer.hit();
        else
        {
            cout << "The dealer has decided to stand." << endl;
            dealer.still_in = stand;
        }
    }
    
    //Results time!
    cout << "Your final score is: " << user.score << endl;
    switch(user.still_in){
    case 0:
        break;
    case 2:
        cout << "You bust!" << endl;
        break;
    case 3:
        cout << "You got a blackjack!" << endl;
        break;
    }
    
    cout << "The dealer's final score is: " << dealer.score << endl;
    switch(dealer.still_in){
    case 0:
        break;
    case 2:
        cout << "The dealer bust!" << endl;
        break;
    case 3:
        cout << "The dealer got a blackjack!" << endl;
        break;
    }
    
    //Figure out the messiness of who won, based on standard blackjack rules. Then close.
    if (user.still_in == bust && dealer.still_in == bust || user.score == dealer.score)
        cout << "This round was a push. No credits have been taken/removed from your account." << endl;
        
    else if((user.score > dealer.score && user.still_in != bust) ||
    (user.score < dealer.score && user.still_in != bust && dealer.still_in == bust))
    {
        cout << "You win! " << bet << " credits have been added to your account!" << endl;
        user.credits += bet;
        cout << "You now have " << user.credits << " credits!" << endl;
    }
    else
    {
        cout << "The house has won. " << bet << " credits have been taken from your account." << endl;
        user.credits -= bet;
        cout << "You now have " << user.credits << " credits!" << endl;
    }
    return 0;
}