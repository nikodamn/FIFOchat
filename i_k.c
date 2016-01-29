#include "inf117209_config.c"


struct msgbuf InitTasks(struct msgbuf m, int task)
{
	m.type = 1;
	m.cmd = task;
	m.pid = getpid();
	return m;
}

void InitMenu()
{
	printf("1 - Zaloguj sie \n");
	printf("2 - Uzytkownicy \n");
	printf("3 - Grupy \n");
	printf("4 - Zmien nick \n");
	printf("5 - Dolacz do grupy \n");
	printf("6 - Opusc grupe \n");
	printf("8 - Pokaz chat \n");
	printf("9 - Wyslij wiadomosc do grupy \n");
	printf("10 - Wyloguj \n");
}

void ShowChat(struct msgbuf message, struct msgbuf m[], int id)
{				
				int i = 0;
				int end = 0;
				int j = 0;
				char symbol;
				message.type = 1;
				message.cmd = 8;
				time_t czas;
			    time(&czas);
			    strcpy(message.date,ctime(&czas));


				while(end != -1) //copy content
				{
					end = msgrcv(id, &message, sizeof(message), getpid(), IPC_NOWAIT);
					if (end > 0)
					{
						if(message.pid != getpid()) 
						{
							for(i=0; i<20; i++)
							{
								if(strcmp(m[i].nick, "") == 0) //first empty
								{
									strcpy(m[i].nick, message.nick);
									strcpy(m[i].text, message.text);
									strcpy(m[i].date, message.date);

									break;	
								}
							}
						}
					}
				}
				//show content	
				for(i=0; i<20; i++)
				{
					if(strcmp(m[i].text, "") == 0)
					{
						//printf(" *** PUSTA WIADOMOSC *** \n");
					}
					else
					{
						printf("%s at %s %s \n", m[i].nick, m[i].date, m[i].text); //add date
					}	
				}
				strcpy(message.nick, "");
				printf("Podaj nick odbiorcy wiadomosci:\n ");
				scanf("%s", message.nick);
				j=0;
				strcpy(message.text, ""); //clear old msg
				while( (symbol = getchar()) != '\n')
				{
					//clean buffer
				}
                printf("Podaj wiadomosc: \n");
                symbol=getchar();
                while(( (symbol != '\n') && (j <256) )){
                    message.text[j++]=symbol;
                    symbol = getchar();
                }
                message.text[j]=0;


				msgsnd(id, &message, sizeof(message), 0);
}

void CheckStatus(struct msgbuf message)
{
		if (message.status == 0)
		{
			printf("KOMUNIKAT: Wszystko OK! \n");
		}
		if (message.status == 1)
		{
			printf("KOMUNIKAT: Taki nick juz istnieje! \n");
		}
		if (message.status == 2)
		{
			printf("KOMUNIKAT: Jestes juz zalogowany! \n");
		}
		if (message.status == 3)
		{
			printf("KOMUNIKAT: Przepelnienie tablicy! \n");
		}
		if (message.status == 4)
		{
			printf("KOMUNIKAT: Jestes juz w grupie! \n");
		}
		if (message.status == 5)
		{
			printf("KOMUNIKAT: Grupa nie istnieje! \n");
		}
		if (message.status == 6)
		{
			printf("KOMUNIKAT: Nie jestes w grupie! \n");
		}
		if (message.status == 7)
		{
			printf("KOMUNIKAT: Nie istnieje taki nick! \n");
		}
		if (message.status == 8)
		{
			printf("KOMUNIKAT: Nie jestes zalogowany! \n");
		}
		if (message.status == 10)
		{
			printf("KOMUNIKAT: Wylogowano! \n");
		}
}

void CheckCMD(struct msgbuf message)
{
		if (message.cmd == 2)
		{
			printf("*** Uzytkownicy: *** \n");
			printf("%s", message.text);
		}
		if (message.cmd == 3)
		{
			printf("*** Grupy: *** \n");
			printf("%s", message.text);
		}

}

int main(int argc, char* argv[])
{	
	int i = 0;
	char symbol;
	int end_app = 0;
	int task = 0;
	int server = msgget(15071410, 0644 | O_EXCL);
	struct msgbuf message;
	struct msgbuf m[20];

	for(i=0; i<20; i++)
	{
		strcpy(m[i].text, "");
		strcpy(m[i].nick, "");
		strcpy(m[i].date, "");
	}

	if (server == -1)
	{
		printf("Blad komunikacji z serwerem! \n");
	}

	InitMenu();

	while(!end_app)
	{
		printf("\nPodaj ID zadania: ");
		scanf("%d",&task);

		switch(task)
		{
			case 1:
				printf("Logowanie. Podaj nick: \n");
				scanf("%s", message.nick);
				break;
			case 2:
				//printf("Uzytkownicy: \n");
				break;
			case 3:
				//printf("Grupy: \n");
				break;
			case 4:
				printf("Zmien nick: \n");
				scanf("%s", message.nick);
				break;
			case 5:
				printf("Dolacz do grupy. Podaj nazwe grupy: \n");
				scanf("%s", message.nick);
				break;
			case 6:
				printf("Wypisz sie z grupy. Podaj nazwe grupy: \n");
				scanf("%s", message.nick);
				break;
			case 8:
				printf("Wyswietl chat. \n ");
				ShowChat(message, m, server);
				break;
			case 9:
				printf("Wyslij wiadomosc do grupy. Podaj nazwe grupy: \n");
				scanf("%s", message.nick);
				
				i=0;
				strcpy(message.text, ""); //clear old msg
				while( (symbol = getchar()) != '\n')
				{
					//clean buffer
				}
                printf("Podaj wiadomosc: \n");
                symbol=getchar();
                while(( (symbol != '\n') && (i <256) )){
                	printf("Moje i to: %d", i);
                    message.text[i++]=symbol;
                    symbol = getchar();
                }
                message.text[i]=0;
				break;
			case 10:
				printf("Wyloguj \n");
				break;
		}
		message = InitTasks(message, task);
		msgsnd(server, &message, sizeof(message), 0);
		msgrcv(server, &message, sizeof(message), getpid(), MSG_NOERROR);
		CheckStatus(message);
		CheckCMD(message);
	}

	return 0;
}