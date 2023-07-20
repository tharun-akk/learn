#include"header.h"

/* class of players & his/her information
   =========================================  */
class PlayerInfo
{                                         // data members of class
	public:
		double account_bal;
		string password;
		string playername;
		double bid_amount;
		int c;                    // count no.of players entering
		int count;	           // counter using while printing players details
					   // member functions of class
		void set_details();
		void get_details();
		void set_bidAmount();
};


/* function to set player details 
   =================================== */

void PlayerInfo::set_details()
{
	count++;
	cout<< "Enter player " <<count<<" name "<<endl;
	cin.ignore();
	getline(cin,playername);
	cout<<playername<<" Enter Account balance"<<endl;
	cin>>account_bal;
	cout<<playername<<" Set your password "<<endl;
	cin.ignore();
	getline(cin,password);
}

/*function to get player details
  =================================  */

void PlayerInfo::get_details()
{
	c++;
	cout<<"player "<<count<<" : "<<playername;
	cout<<" Bid amount : "<<bid_amount<<endl;
}



/* function to set bid amount
   ==============================  */

void PlayerInfo::set_bidAmount()
{
	cout<<playername<<" Enter your's bid amount"<<endl;
	cin>>bid_amount;
	cout<<"----------------------------------------------"<<endl;
}


/* interfaces for guessing number
   ==================================  */

class Guess
{
	public:
		virtual void guessnum() = 0;
		//                virtual void savetofile()=0;

};

/* class for saving data into files
   =================================== */

class SaveFile
{
	public:
		ofstream fwon;              //creating file object 
		ofstream flost;

		~SaveFile()                                // Destructor of class
		{
			fwon.close();
			flost.close();
		}

};    

/* class for casino game
   ===========================  */

class CasinoGame : public PlayerInfo,public Guess,public SaveFile
{
	protected:
		vector<PlayerInfo> players;
	public:
		int guess;
	public:
		CasinoGame( vector<PlayerInfo>& playerlist):players(playerlist)
	{}
		void select_option(vector<PlayerInfo>&,vector<int>&,int);

		void guessnum() override   // defining and accesing the pure virtual function in derived class
		{
			cout<<playername<<" Enter your guess(only numbers) "<<endl;
			cin>>guess;
			while(guess <= 0 || guess > 10)
			{
				try{
					if(guess <= 0 || guess > 10)
					{
						throw  " Please !! Select the choice within range(1-10)";
					}
				}
				catch(const char *s)
				{
					cout<<"Exception : "<<playername<<s<<endl;
					cin>>guess;
				}
			}
		}
		void playgame(vector<PlayerInfo> &,int);
};

/* function to play game
   =========================  */

void CasinoGame::playgame( vector<PlayerInfo>&players, int n)
{
	int counter=0;
	int opt;
	int extra_amount;
	for(auto &players :players)
	{	
		counter++;
		//player_guess();
		srand(getpid());
		if(players.account_bal > players.bid_amount)
		{
			cout<<players.playername<<" ";
			guessnum();
			int dice;
			auto Equal_or_not=[&](int guess)        // lambda function
			{
				srand(getpid());                      // to change the seed value 
				dice=rand()%10+1;                      // random number generation within range
				return (dice == guess);            // checking players guess is equal to dice or not

			};
			if( Equal_or_not(guess) )                 // call to lambda function
			{                                          // if player won
				cout<<"\x1B[32m"<<players.playername<<"  Congrats....You won \033[0m"<<endl;
				players.account_bal += players.bid_amount*5;    // bid amount multiplied by 5 times
				fwon.open("WonList.txt",ios::in|ios::out|ios::app);
				// File to store the details of players won
				if(fwon.fail())
				{
					perror("wonfile:");
					cout<<players.playername<<"\t Unable to store your info into file"<<endl;
				}				
				fwon<<" Player:  "<<players.playername<<"\tamount won : "<<players.bid_amount*5;
				fwon<<"\tguess is :"<<guess<<endl;
			}

			else     // if player lost.....
			{
				cout<<"\x1B[31m"<<players.playername<<"\tYou lost...Better luck next time\033[0m"<<endl;
				players.account_bal -= players.bid_amount;    // deducing bid amount from player's account 
				flost.open("LostList.txt",ios::in|ios::out|ios::app);
				//file to store the details of players lost
				if(flost.fail())
				{
					perror("lostfile:");
					cout<<players.playername<<"\t unable to store your info into file"<<endl;
				}				
				flost<<"Player :"<<players.playername<<"\tamount lost : "<<players.bid_amount<<endl;
			}
		}

		else if(players.account_bal < players.bid_amount)
		{
			cout<<"\x1B[31m"<<playername<<"  You don't have enough amount You can't enter game \033[0m"<<endl;
		}
	}
	//	fwon.close();                                            //closing the  files
	//	flost.close();
	if(counter==n)
	{
		vector<int>choice;
		for(auto &players :players)
		{
			cout<<players.playername<<" Select an option"<<endl;
			cout<<"\t 1.Play again  \n\t  2.Increase Bid amount  \n\t  3.Decrease bid amount";
			cout<<"\n\t 4.Balance Enquiry \n\t 5.exit"<<endl;
			cin>>opt;
			while(opt <= 0 || opt > 6)
			{
				cout<<players.playername<<"\t Warning .. please choose within range"<<endl;
				cin>>opt;
			}

			if(opt == 2)
			{
				while(opt == 2)
				{
					cout<<players.playername<<"  Enter how much you need to increase bid amount"<<endl;
					cin>>extra_amount;                 
					players.bid_amount += extra_amount;
					cout<<players.playername<<" Select an option"<<endl;
					cout<<"\t 1.Play again  \n\t  2.Increase Bid amount  \n\t  3.Decrease bid amount";
					cout<<"\n\t 4.Balance Enquiry \n\t 5.exit"<<endl;
					cin>>opt;
				}
			}
			if(opt == 3)
			{
				while(opt == 3)
				{
					cout<<players.playername<<"  Enter how much you need to reduce bid amount"<<endl;
					cin>>extra_amount;
					players.bid_amount -=extra_amount;        
					cout<<players.playername<<" Select an option"<<endl;
					cout<<"\t 1.Play again  \n\t  2.Increase Bid amount  \n\t  3.Decrease bid amount";
					cout<<"\n\t 4.Balance Enquiry \n\t 5.exit"<<endl;
					cin>>opt;
				}
			}
			if(opt == 4)
			{
				while(opt == 4)
				{
					int r=0;     //  used to count how many times password was incorrect
					string passcode;
					cout<<players.playername<<"  To check Balance first you need to verify passcode"<<endl;
					cin.ignore();
					getline(cin,passcode);  // inputing psscode from user to confirm
					while(players.password != passcode && r < 3)
					{
						if(r == 3)
							return;
						r++;
						cout<<" Wrong password please enter correct password"<<endl;
						cin>>passcode;
					}
					if(players.password == passcode)
						cout<<players.playername<<"  Available account balance  : "<<players.account_bal<<endl;
					cout<<"_________________________________________________"<<endl;				
					cout<<players.playername<<" Select an option"<<endl;
					cout<<"\t 1.Play again  \n\t  2.Increase Bid amount  \n\t  3.Decrease bid amount";
					cout<<"\n\t 4.Balance Enquiry \n\t 5.exit"<<endl;
					cin>>opt;
				}
			}
			if(opt == 5)
			{
				cout<<"\x1B[31m"<<players.playername<<"  You choosen to exit the game....... \033[0m"<<endl;
				continue ;
			}


			else /* if(opt!=2 && opt!=3 && opt!=4)  */
			{
				choice.emplace_back(opt);
			}
		}
		select_option(players,choice,n);
	}
}

// function for players to choose an option
//==========================================

void CasinoGame::select_option(vector<PlayerInfo>& players,vector<int>&choice,int n)
{
	for(auto it = choice.begin() ; it != choice.end() ; it++)
	{
		switch(*it)
		{
			case 1:
				{ 
					if(*it == 1)
					{
						cout<<playername<<"\tYou choosed to enter the game again"<<endl;
						playgame(players,n);
					}
				}
				break;
				/*			case 5:
							{
							if(*it == 5)
							{
							cout<<"\x1B[31m"<<playername<<"  You choosen to exit the game....... \033[0m"<<endl;
							return ;
							}
							}
							break;
				 */
		}
	}
}

int main()
{

	// .........................................Welcome message............................................  
	cout<<"================================Welcome To Game====================================\033[0m  "<<endl;    

	cout<<"  RULES OF CASINO NUMBER GUESSING GAME"<<endl;
	cout<<" 1. You need to guess a number between 1 - 10 "<<endl;
	cout<<"\x1B[32m 2. If your guess is correct your bid amount will be multiplied by 5 times \033[0m"<<endl;
	cout<<"\x1B[31m 3. If your guess was wrong you will loose your bid amount \033[0m "<<endl;
	cout<<" 4. Invest and play at your own Risks"<<endl;
	cout<<"----------------------------------------------------------------------------------------"<<endl;

	PlayerInfo p;                               // creating object for above class 
	vector<PlayerInfo> players;                              // creating an vector 
	int n;                                              // number of players
	cout<<"enter no.of players"<<endl;
	cin>>n;
	for(int i=0;i<n;i++)                                // taking input from user and storing into vector 
	{
		p.set_details();
		p.set_bidAmount();
		players.emplace_back(p);                // inserting object by object into vector    
	}
	//cout<<p.size()<<endl;
	for(auto &players :players)                                       // loop to get player details
									  //for(int i=0;i<n;i++)
	{
		players.get_details();
	}
	cout<<"-----------------------------------------------"<<endl;

	CasinoGame game(players);                          //passing vector as arguments
	game.playgame(players,n);
	//	if(counter==n)                                             // checking for round 1 completion or not
	//		for(auto &p :p) 
	//
	//			p.select_option(n);                      // displaying options for players to continue
}


