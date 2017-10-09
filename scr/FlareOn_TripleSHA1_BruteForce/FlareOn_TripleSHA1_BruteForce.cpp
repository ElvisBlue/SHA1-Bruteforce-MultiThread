// FlareOn_TripleSHA1_BruteForce.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <cmath>
#include "sha1.h"
using namespace std;

char *const Table = "abcdefghijklmnopqrstuvwxyz_1234";

char Hash1[20] = {0x3C,0xAB,0x24,0x65,0xE9,0x55,0xB7,0x8E,0x1D,0xC8,0x4A,0xB2,0xAA,0xD1,0x77,0x36,0x41,0xEF,0x6C,0x29}; 
char Hash2[20] = {0x4A,0x1B,0xF8,0xBD,0x1E,0x91,0xF3,0x59,0x3A,0x6C,0xCC,0x9C,0xC9,0xB2,0xD5,0x68,0x2E,0x62,0x24,0x4F};
char Hash3[20] = {0x9E,0x60,0x61,0xA3,0x62,0x50,0xE1,0xC4,0x7E,0x69,0xF0,0x31,0x2D,0xB4,0xE5,0x61,0x52,0x8A,0x1F,0xB5};
char HashResult0[7];
char HashResult1[7];
char HashResult2[7];

char TestHash[20] = {0x1F,0x76,0x43,0x6F,0x70,0x0F,0x4F,0x10,0x52,0xC8,0x63,0x07,0x17,0x37,0x6F,0x41,0x2A,0x54,0x65,0x63};

int Thread[32]; // Start string of every Thread
bool HashValid[4];
unsigned int TotalHash=0;
int	ThreadCnt=1;

int TripleSHA1(char Mess[], char Memory[], int Len)
{
	SHA1Context SHA1;
	char Temp1[20], Temp2[20];

	SHA1Reset(&SHA1);
	SHA1Input(&SHA1, (uint8_t *)Mess, Len);
	SHA1Result(&SHA1, (uint8_t *)Temp1);

	SHA1Reset(&SHA1);
	SHA1Input(&SHA1, (uint8_t *)Temp1, 20);
	SHA1Result(&SHA1, (uint8_t *)Temp2);

	SHA1Reset(&SHA1);
	SHA1Input(&SHA1, (uint8_t *)Temp2, 20);
	SHA1Result(&SHA1, (uint8_t *)Memory);


	return 1;
}

void PrintInfor()
{
	system("cls");
	cout<<"Hash: "<<TotalHash<<endl;
	if (HashValid[1]==true)
		cout<<"Hash 1 found: "<<HashResult1;

	if (HashValid[2]==true)
		cout<<"Hash 2 found: "<<HashResult2;

	//if (HashValid[0]==true) // for test
	//	cout<<"Test hash found: "<<HashResult0;
}

void BruteForce(int ThreadID)
{
	char Temp[7];
	char SHA1[20];

	int i1, i2, i3, i4, i5, i6, max;
	if (ThreadID==ThreadCnt)
		max = 31; // sideof(Table)
	else
		max = Thread[ThreadID+1];

	for (i1=Thread[ThreadID]; i1<max; i1++)
		for (i2=0; i2<31; i2++)
			for (i3=0; i3<31; i3++)
				for (i4=0; i4<31; i4++)
					for (i5=0; i5<31; i5++)
						for (i6=0; i6<31; i6++)
						{
							Temp[0]=Table[i1];
							Temp[1]=Table[i2];
							Temp[2]=Table[i3];
							Temp[3]=Table[i4];
							Temp[4]=Table[i5];
							Temp[5]=Table[i6];
							Temp[6]=0;
							TripleSHA1(Temp, SHA1, 6);

							if (memcmp(&Hash1, &SHA1, 20)==0)
							{
								HashValid[1]=true; 
								memcpy(HashResult1, Temp, 6);
							}
							if (memcmp(&Hash2, &SHA1, 20)==0)
							{
								HashValid[2]=true; 
								memcpy(HashResult2, Temp, 6);
							}
							/*if (memcmp(TestHash, SHA1, 20)==0) // For Testing
							{
								HashValid[0]=true; 
								memcpy(HashResult0, Temp, 6);
							}*/
							TotalHash++;
						}
}

int InstallThread()
{
	for (int i=1;i<=3;i++)
		HashValid[i]=false;

	HashValid[0]=false; // For test hash

	for (int i=0; i<ThreadCnt; i++)
	{
		Thread[i+1] = i * (31/ThreadCnt);
		cout<<"Thread "<<i+1<<" Start: "<<Thread[i+1]<<endl;
	}
	return 1;
}

int RunThread()
{
	for (int i=1; i<=ThreadCnt; i++)
		CreateThread(0,0,(LPTHREAD_START_ROUTINE)BruteForce,(LPVOID)i,0,0);
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char TestSHA[20];
	
	cout<<"SHA1 Triple Brute Force";
Again:
	cout<<"\nHow many thread? (max = 31)\n Thread = ";
	cin>>ThreadCnt;
	if (ThreadCnt<1 || ThreadCnt>31)
		goto Again;
	
	InstallThread();
	cout<<"Press any key to start brute forcing...\n";
	system("pause");
	RunThread();
	while (HashValid[1]==false || HashValid[2]==false)
	{
		PrintInfor();
		Sleep(1000);
	}
	PrintInfor();
	system("pause");
	return 0;
}

