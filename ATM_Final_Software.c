/*****************************************************
* GROUP # 16 - THE ATM                               *
* Jacob Rampertab                                    *
* Baraa Hamodi                                       *
* Hammad Mirza                                       *
* Arthur Dabu                                        *
*****************************************************/

/*****************************************************
* We had issues with pow(10, 2) returning 99 instead *
* of 100, so the function was overridden.            *
*****************************************************/
float pow(int b, int e)
{
	float r = 1;
	if(e==-1)
		return (1/b);
	for(int i = 0; i < e; i++)
	{
		r *= b;
	}
	return r;
}

/*****************************************************
* This is the bankAccount struct that contains all   *
* of the information about a single bank account.    *
*****************************************************/
typedef struct
{
	float balance;
	int pin;
	int cardCode;
} bankAccount;
bankAccount accounts[5];
float stock = 300;

const string sFileName = "accountInfo.txt";
TFileIOResult nIoResult;
TFileHandle hFileHandle;
int nFileSize = 100;
/*****************************************************
* This function is for setting the default values to *
* the bank accounts when the file does not exist.    *
*****************************************************/
void defaults()
{
	// Jacob Rampertab
	accounts[0].balance = 1000.00;
	accounts[0].pin = 6497;
	accounts[0].cardCode = 532;

	// Baraa Hamodi
	accounts[1].balance = 545.54;
	accounts[1].pin = 7777;
	accounts[1].cardCode = 245;

	// Hammad Mirza
	accounts[2].balance = 45.91;
	accounts[2].pin = 5951;
	accounts[2].cardCode = 325;

	// Arthur
	accounts[3].balance = 333.66;
	accounts[3].pin = 8008;
	accounts[3].cardCode = 453;

	// Wonderful Professor or TA
	accounts[4].balance = 9999.00;
	accounts[4].pin = 1111;
	accounts[4].cardCode = 434;
}

/*****************************************************
* This function is for setting the default values of *
* the bank accounts when the file does exist.        *
*****************************************************/
void readIn()
{
	CloseAllHandles(nIoResult);
	OpenRead(hFileHandle, nIoResult, sFileName, nFileSize);

	for(int i = 0; i < 5; i++)
	{
		ReadFloat(hFileHandle, nIoResult, accounts[i].balance);
		ReadLong(hFileHandle, nIoResult, accounts[i].pin);
		ReadLong(hFileHandle, nIoResult, accounts[i].cardCode);
	}

	Close(hFileHandle, nIoResult);
}

/*****************************************************
* This function is for updating the account values   *
* in the file when a trasaction is complete.         *
*****************************************************/
void writeOut()
{
	Delete(sFileName, nIoResult);
  CloseAllHandles(nIoResult);
	OpenWrite(hFileHandle, nIoResult, sFileName, nFileSize);

	for(int i = 0; i < 5; i++)
	{
		WriteFloat(hFileHandle, nIoResult, accounts[i].balance);
		WriteLong(hFileHandle, nIoResult, accounts[i].pin);
		WriteLong(hFileHandle, nIoResult, accounts[i].cardCode);
	}

	Close(hFileHandle, nIoResult);
}

/*****************************************************
* This function is for displaying the scrolling menu *
* for the enterPin function.                         *
*****************************************************/
void dispInt(int n, int l)
{
nxtDisplayCenteredTextLine(l+2, "%d         %d",
							(n==0? 9 : n-1), (n==9? 0 : n+1));
	nxtDisplayCenteredBigTextLine(l, "%d", n);
}


/*****************************************************
* This function is used for getting the pin number   *
* from the user through the NXT input methods and    *
* returns it as a single interger value.             *
*****************************************************/
int enterPin()
{
	eraseDisplay();
	nxtDisplayTextLine(0, "Please enter pin");
	int pin=0, n=1;
	dispInt(n, 3);
	for(int digit=0; digit<4; digit++)
	{
		string s = "";
		for(int j = 0; j<digit; j++)
			s = s + "*";
		s = s + "_";
		nxtDisplayTextLine(7, s);

		while(nNxtButtonPressed != 3)
		{
			if(nNxtButtonPressed == 1)
			{
				PlayImmediateTone(650, 10);
				n++;
				if(n==10)
					n=0;
				dispInt(n,3);
				while(nNxtButtonPressed  == 1){}
			}
			else if(nNxtButtonPressed == 2)
			{
				PlayImmediateTone(650, 10);
				n--;
				if(n==-1)
					n=9;
				dispInt(n,3);
				while(nNxtButtonPressed  == 2){}
			}
		}
		PlayImmediateTone(700, 10);
		while(nNxtButtonPressed == 3){}
		pin += n* pow(10, 3-digit);
	}
	nxtDisplayTextLine(7, "****");
	wait1Msec(1000);
	eraseDisplay();
	return pin;
}

/*****************************************************
* This function is used for getting the sequence of  *
* colours on a debit card which corresponds to a     *
* certain account number and balance and then        *
* returns it as a single integer value.              *
*****************************************************/
int swipeCard()
{
	eraseDisplay();
	nxtDisplayCenteredTextLine(3, "Please swipe");
	nxtDisplayCenteredTextLine(4, "card");
	nxtDisplayCenteredTextLine(5, "<------");
	int oldColour = -2, newColour, cardCode = 0;
	while(!SensorValue[S2]){}
	while(SensorValue[S3]){}
	newColour = SensorValue[S1];
	for (int i=0; i < 3; i++)
	{
		while(newColour==oldColour)
		{
			newColour = SensorValue[S1];
		}
		wait1Msec(100);
		newColour = SensorValue[S1];
		cardCode = cardCode + newColour*pow(10, 2-i);
		oldColour = newColour;
	}
	PlayImmediateTone(700, 50);
	return cardCode;
}

/*****************************************************
* This function is used for comparing the card code  *
* and PIN combination against all other accounts     *
* stored in the array and returns the index number   *
* of the matching account.                           *
*****************************************************/
int selectAccount()
{
	int c = swipeCard();
	int p = enterPin();

	for(int i = 0; i < 4; i++)
	{
		if(accounts[i].pin==p && accounts[i].cardCode==c)
			return i;
	}
	return -1;
}

/*****************************************************
* This function is used for dispensing/outputting    *
* the amount of dollars requested by the user.       *
*****************************************************/
void dispense(int n)
{
	eraseDisplay();
	nxtDisplayCenteredTextLine(4, "Dispensing...");

	for(int a = n/20; a>0; a--)
	{

		nMotorEncoder[motorA] = 0;
		motor[motorA] = -15;
		while(nMotorEncoder[motorA] > -267){}
		motor[motorA] = 0;
		wait1Msec(200);
		PlayImmediateTone(700, 50);
	}
	nMotorEncoder[motorA] = 0;
	motor[motorA] = 15;
	while(nMotorEncoder[motorA] < 40){}
	motor[motorA] = 0;
	eraseDisplay();

	nxtDisplayString(1, "Dispensed:");
	nxtDisplayCenteredBigTextLine(4, "$%d", n);
	wait1Msec(2500);
	eraseDisplay();
}

/*****************************************************
* This function is used for displaying the detailed  *
* menu for withdrawing money. It also calls the      *
* dispenser function and displays appropiriate       *
* messages to the user.                              *
*****************************************************/
void withdraw(int i)
{
	bool exit = false;
	int amount=0;
	eraseDisplay();
	nxtDisplayCenteredBigTextLine(5, "$%d.00", amount);
	while(nNxtButtonPressed!=-1){}
	while(!exit)
	{
	nxtDisplayTextLine(1, "Enter amount you");
	nxtDisplayTextLine(2, "wish to withdraw");
	nxtDisplayCenteredBigTextLine(5, "$%d.00", amount);
		while(nNxtButtonPressed != 3)
		{
			if(nNxtButtonPressed == 1)
			{
				PlayImmediateTone(650, 10);
				amount+=20;
				if(amount>stock||amount>accounts[i].balance)
					amount-=20;
				while(nNxtButtonPressed  == 1){}
				nxtDisplayCenteredBigTextLine(5, "$%d.00", amount);
			}
			else if(nNxtButtonPressed == 2)
			{
				PlayImmediateTone(650, 10);
				amount-=20;
				if(amount<0)
					amount=0;
				while(nNxtButtonPressed  == 2){}
				nxtDisplayCenteredBigTextLine(5, "$%d.00", amount);
			}
		}
		eraseDisplay();
		if(accounts[i].balance >= amount && stock >= amount)
		{
			accounts[i].balance -= amount;
			stock -= amount;
			dispense(amount);
			nxtDisplayTextLine(4, "Withdrawal Successful");
			wait1Msec(1000);
			exit = true;
		}
		else
			nxtDisplayTextLine(6, "Error");
	}
}

/*****************************************************
* This function is used for security purposes and    *
* when called will lockdown the robot. This occurs   *
* after 3 consecutive incorrect PINS. The only way   *
* deactivate this lockdown is to press the orange    *
* button and enter the correct security PIN.         *
*****************************************************/
void safetyLockdown()
{
	nMotorEncoder[motorB] = 0;
	bool exit = false;
	while(!exit)
	{
		motor[motorB] = -40;
		while(nMotorEncoder[motorB] > -90){}
		motor[motorC] = 0;
		time1[T1] = 0;

		bool state = true;
		eraseDisplay();
		while (nNxtButtonPressed != 3)
		{
			if(time1[T1] >= 500)
			{
				state=!state;
				time1[T1]=0;
				if(state)
				{
					nxtDisplayCenteredBigTextLine(3, "WARNING!");
					PlayImmediateTone(900, 50);
				}
				else
				{
					eraseDisplay();
				}
			}
		}
		eraseDisplay();
		while(nNxtButtonPressed==3){}

		motor[motorB] = 20;
		while(nMotorEncoder[motorB] < 0){}
		motor[motorB] = 0;
		exit = (enterPin()==1234);
	}
	nxtDisplayCenteredBigTextLine(3, "ATM");
	nxtDisplayCenteredBigTextLine(5, "RESTORED!");
	wait1Msec(2000);
	eraseDisplay();
}

/*****************************************************
* This function is used for displaying the detailed  *
* menu for depositing money.                         *
*****************************************************/
void dispDeposit(int n, int l)
{
	char c;
	if(n==10)
	{
		c='.';
		nxtDisplayCenteredTextLine(l+2, "9         D");
	}
	else if(n==11)
	{
		c='D';
		nxtDisplayCenteredTextLine(l+2, ".         0");
	}
	else
	{
		c = n+48;
	nxtDisplayCenteredTextLine(l+2, "%c         %c",
				(c=='0'? 'D' : n+47), (c=='9'? '.' : n+49));
	}


	nxtDisplayCenteredBigTextLine(l, "%c", c);
}

/*****************************************************
* This function is used for using the detailed       *
* menu for withdrawing money. It also turns on the   *
* motor to deposit money and displays appropiriate   *
* messages to the user.                              *
*****************************************************/
void deposit(int i)
{
	nxtDisplayCenteredTextLine(0, "Enter amount");
	bool afterDec = false;
	float amount=0, adec=0;
	int n = 0, bdec=0, amax=1, bmax=1;
	dispDeposit(n, 5);
	nxtDisplayCenteredBigTextLine(2, "$%.2f", amount);
	while(nNxtButtonPressed!=-1){}
	while(n!=11 && amax<=2)
	{
		while(nNxtButtonPressed != 3)
		{
			if(nNxtButtonPressed == 1)
			{
				PlayImmediateTone(650, 10);
				n++;
				if(n==12)
					n=0;
				dispDeposit(n,5);
				while(nNxtButtonPressed  == 1){}
			}
			else if(nNxtButtonPressed == 2)
			{
				PlayImmediateTone(650, 10);
				n--;
				if(n==-1)
					n=11;
				dispDeposit(n,5);
				while(nNxtButtonPressed  == 2){}
			}
		}
		PlayImmediateTone(700, 10);
		while(nNxtButtonPressed == 3){}

		if(n==10 || bmax > 4)
			afterDec=true;
		if(n>=0 && n <= 9)
		{
			if(afterDec)
			{
				adec = adec + n/(1.0*pow(10, amax));
				amax++;
			}
			else
			{
				bdec = bdec*10 + n;
				bmax++;
			}
			amount = bdec+adec;
		}
		nxtDisplayCenteredBigTextLine(2, "$%.2f", amount);
	}
	wait1Msec(500);
	PlayImmediateTone(650, 10);
	wait1Msec(100);
	PlayImmediateTone(800, 10);
	accounts[i].balance += amount;
	eraseDisplay();
	nxtDisplayCenteredTextLine(3, "Please insert");
	nxtDisplayCenteredTextLine(4, "money in envelop");
	nxtDisplayCenteredTextLine(5, "into the slot");
	wait1Msec(5000);
	motor[motorC] = -50;
	wait1Msec(10000);
	motor[S1] = 0;
	eraseDisplay();
	nxtDisplayCenteredTextLine(4, "Deposit");
	nxtDisplayCenteredTextLine(5, "Successful");
	wait1Msec(2500);
}

/*****************************************************
* This is the main function.                         *
*****************************************************/
task main()
{
	int accInd, nErrors=0;
	TFileIOResult fr;
	TFileHandle fh;
	string fn;
	int fs;
	FindFirstFile(fh, fr, "accountInfo.txt", fn, fs);
	if(fr!=0)
		defaults();
	else
		readIn();
	Close(fh, fr);
	SensorType[S1] = sensorCOLORFULL;
	SensorType[S2] = sensorTouch;
	SensorType[S3] = sensorTouch;
	while(true)
	{
		nxtDisplayCenteredTextLine(1, "Welcome User");
		nxtDisplayCenteredTextLine(4, "Press orange");
		nxtDisplayCenteredTextLine(5, "button to begin");
		while(nNxtButtonPressed!=3){if(SensorValue[S3]){stock=300;}}
		PlayImmediateTone(700, 10);
		eraseDisplay();

		do{
			accInd = selectAccount();
			if(accInd==-1)
			{
				eraseDisplay();
				nxtDisplayCenteredTextLine(3, "Bad swipe or pin");
				nErrors++;
				wait1Msec(3000);
			}
			if(nErrors >= 3)
				safetyLockdown();
			else
				nxtDisplayCenteredTextLine(5, "Try Again");
		}while(accInd == -1);
		bool sameAccount = true;
		while(sameAccount)
		{
			eraseDisplay();
			nxtDisplayCenteredTextLine(1, "Bal: %.2f", accounts[accInd].balance);
			nxtDisplayCenteredTextLine(3, "Choose action:");
			nxtDisplayCenteredTextLine(6, "Deposit Withdraw");
			while(nNxtButtonPressed!=1 && nNxtButtonPressed!=2){}
			PlayImmediateTone(700, 10);
			eraseDisplay();
			if(nNxtButtonPressed==1)
			{withdraw(accInd);}
			else if(nNxtButtonPressed==2)
			{deposit(accInd);}

			eraseDisplay();
			nxtDisplayCenteredTextLine(1, "Another transaction?");
			nxtDisplayCenteredTextLine(6, "Yes       No");
			while(nNxtButtonPressed!=1 && nNxtButtonPressed!=2){}
			PlayImmediateTone(700, 10);
			if(nNxtButtonPressed==1)
			{sameAccount=false;}
			while(nNxtButtonPressed!=-1){}
		}
		writeOut();
		eraseDisplay();
	}
}
