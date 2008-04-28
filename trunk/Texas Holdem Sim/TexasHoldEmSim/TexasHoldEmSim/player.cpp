// Player.cpp

#include "player.h"

using namespace std;

Player::Player()
{

}


Player::Player(double m, map<string, double>& preFlop, string Name): job(-1), fold(false), bust(false), allIn(false), raised(false), myBet(0)
{ 
    money = m;
	preFlopOdds = preFlop;
	name = Name;
    SetSkillLvl();	
	
} // Player()

        
void Player::AddMoney(double amt)
{
    money += amt;
} // AddMoney()


double Player::GetMoney()
{
    return money;
} // GetMoney()


hand Player::GetHand()
{
    return myHand;
} // GetHand()

void Player:: MakeHand()
{
	myHand.initHand(tableCards, holeCards);
}

void Player::AddCard(card c, int loc)
{
    if(loc == HOLECARDS)
    {
        holeCards.push_back(c);   
    }
    else if( loc == FLOP || loc == TURN || loc == RIVER)
    {
        tableCards.push_back(c);
    }
} // AddCard()


void Player::ClearCards()
{
    myHand.clear();
    tableCards.clear();
    holeCards.clear();
	allCards.clear();
} // ClearCard()

void Player::SetPreFlopOdds(map<string, double>& oddTbl)
{
    preFlopOdds = oddTbl;
}


double Player::Action(bool limitRaise, double currentHighBet, bool amHole, bool amFirstIter, int numPlay)
{
	possibleTurnCards.ShuffleCard();
	possibleRiverCards.ShuffleCard();
	
	if(job==SMALLBLIND && amFirstIter && amHole)
	{
	    if(smallBlind > money)
	    {
	        allIn = true;
	        double temp = money;
	        myBet += money;
	        money = 0.0;
	        cout << name << " is the small blind but not enough $$$, so allIn with" << temp << "\n";
	        return temp;
	    }else
	    {
	        money -= smallBlind;
	        myBet += smallBlind;
	        cout << name << " is the small blind and is forced to bet " << smallBlind << "\n";
	        return smallBlind;
	    }
	}
	else if(job==BIGBLIND && amFirstIter && amHole)
	{
	    if(bigBlind > money)
	    {
	        allIn = true;
	        double temp = money;
	        myBet += money;
	        money = 0.0;
	        cout << name << " is the big blind but not enough $$$, so allIn with" << temp << "\n";
	        return temp;
	    }else
	    {
	        money -= bigBlind;
	        myBet += bigBlind;
	        cout << name << " is the big blind and is forced to bet " << bigBlind << "\n";
	        return bigBlind;
	    }
	}
	
	if(amHole)
	    PreFlopDec(numPlay, limitRaise, currentHighBet);
	
	else
	    PostFlopDec(limitRaise, currentHighBet);

	
	/*---------DON'T REPLACE BELOW---------*/
	switch(decision)
	{//now do decision
		case CHECK : 
			return Check();
			break;
		case CALL : 
			return Call(currentHighBet);
			break;
		case RAISE : 
			return Raise(currentHighBet, raiseAmt);
			break;
		case FOLD : 
			return Fold();
			break;
	    default:
	        cout << "did not make a decision";
	        return -1;
	        
	}
} // Action()

bool Player::DidFold()
{
    return fold;
} // DidFold()


bool Player::DidBust()
{
    return bust;
} // DidBust()


bool Player::DidAllIn()
{
    return allIn;
} // DidAllIn()

bool Player::DidRaised()
{
    return raised;
} // DidRaised()


void Player::Reset()
{
    bust  = false;
    fold  = false;
    allIn = false;
	myBet = 0;
	ClearCards();
} // Reset()

void Player::ResetRaised()
{
    raised = false;
} // ResetRaised()

void Player::ResetMyBet()
{
    myBet = 0.0;
} // ResetMyBet()

void Player::SetBusted()
{
    bust = true;
} // SetBusted()

string Player::GetName()
{
    return name;
} // GetName()

double Player::GetBet()
{
    return myBet;
} // GetBet()

/* Private Stuff */
void Player::CombineCards()
{   
    allCards.clear();
    allCards = tableCards;
    allCards.push_back(holeCards[0]);
    allCards.push_back(holeCards[1]);
}

void Player::SetSB(double amnt)
{
	smallBlind = amnt;
}

void Player::SetBB(double amnt)
{
	bigBlind = amnt;
}

double Player::Call(double theHighBet)
{
    double rval = 0.0;
    
	if((theHighBet - myBet) == 0)  //you idiot, you meant check!
	{
		rval = Check();
	}	
	else if((theHighBet - myBet) > money)   //if you don't have enough money to call
	{
		allIn = true;
		rval = money;
		money = 0.0;
	    cout << name << " went all in with " << rval << "\n";
	}
	else
	{
        rval = (theHighBet - myBet);
	    money -= rval;
	    myBet = theHighBet; 
	    cout << name << " called(owed) the bet of " << rval << "\n";
    }
    
    return rval;
    
}//Call

double Player::Fold()
{
	fold = true;
	cout << name << " folded -- loser!\n";
	return 0.0;
}//Fold

double Player::Check()
{
	cout << name << " checked (knock-knock)\n";
	return 0.0;
}//Check

double Player::Raise(double theHighBet, double amnt)
{
    double rval = 0.0;

	if((theHighBet - myBet) > money) // Not Enough $$ to raise, much less call, all in buddy
	{
		rval = Call(theHighBet);
	}
	else if((theHighBet - myBet + amnt) > money) // Don't have enough to call AND raise how much you want
	{
		allIn = true;
		myBet += money;
		rval = money;
		money = 0.0;
		cout << name << " went all in with " << rval << "\n";
		raised = true;
	}
	else
	{
        rval = (theHighBet - myBet + amnt);
	    money -= rval;
	    cout << name << " called(owed) the bet of " << (theHighBet - myBet) << " and raised " << amnt << "\n";
	    myBet += rval;
	    raised = true;
	}
	
	return rval;
}//Raise

double Player::AllIn()
{
	allIn = true;
	double temp = money;
	money = 0.0;
	return temp;
} //AllIn

int Player::GetJob()
{
    return job;
} // GetJob()

void Player::SetJob(int theJob)
{
    job = theJob;
} // SetJob()

void Player::PreFlopDec(int howMany, bool limitRaise, double currBet)
{
    double odds = 0.0;
    string lookup = holeCards[0].whatcard() + holeCards[1].whatcard();

    map<string, double>::iterator it;
    odds = preFlopOdds.find(lookup)->second;

    // See excel file for magic numbers
    switch(howMany)
    {
    
        case 2:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 41)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 49)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 55)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
            }
        break;
        
        case 3:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 26.5)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 31)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 37)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
            }        
        break;
        
        case 4:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 19.4)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 23.3)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 19.4)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
            }        
        break;
        
        case 5:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 41)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 49)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 55)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
            }        
        break;
        
        case 6:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 12.7)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 16)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 19)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
            }        
        break;
        
        case 7:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 10.7)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 13.7)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 16.5)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
            }        
        break;
        
        case 8:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 9.7)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 12)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 14.5)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
            }        
        break;
        
        case 9:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 8.4)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 11)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 13)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;       
            }        
        break;
        
        case 10:
            switch(GetSkillLvl())
            {
                case BEGINNER:
                  if(odds >= 7.6)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);
                break;
                
                case INTERMEDIATE:
                  if(odds >= 9.9)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;
                
                case EXPERT:
                  if(odds >= 12)
                    BetHelper(limitRaise, currBet);
                  else
                    FoldHelper(limitRaise);                
                break;   
            }        
        break;
        
        default: 
        break;
    
    }
} // PreFlopDec()

void Player::PostFlopDec(bool limitRaise, double currHighBet)
{
	decision = (rand()%4); 
	
	//Can't raise if limited and can't check if they owe money
	
	while((limitRaise && decision == RAISE) || (currHighBet > myBet && decision == 1))
	{
		decision = (rand()%4);
		raiseAmt = 2*bigBlind;
	}
} // PostFlopDec()

void Player::SetSkillLvl()
{
    skillLvl = rand() % 3; // 0-2
} // SetSkillLvl()

int Player::GetSkillLvl()
{
    return skillLvl;
} // GetSkillLvl()

void Player::SortHoleCards()
{
    card c0 = holeCards[0];
    card c1 = holeCards[1];
    
    if(c0 < c1)
    {
        holeCards[0] = c1;
        holeCards[1] = c0;
    }         
}

void Player::FoldHelper(double currentHighBet)
{
    if(currentHighBet == myBet)
        decision = CHECK;
    else
        decision = FOLD;
}

void Player::BetHelper(bool limitAction, double currentHighBet)
{
    if(currentHighBet >= 2*bigBlind && limitAction)
    {
        decision = CALL;
    }
    else
    {
        int mod = rand() % 5;                         
        raiseAmt = (double)bigBlind * mod;
        decision = RAISE;
    }
}
