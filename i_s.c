#include "inf117209_config.c"

void Clear(struct users *u, struct group *g)
{		
	int i = 0;
	int j = 0;
	int w = 0;
	for(i=0; i<18; i++)
	{	
		strcpy(u[i].nick, "");
		u[i].pid = 0;
	}
	for(j=0; j<3; j++)
	{
		strcpy(g[j].group_name, "");
		for (w = 0; w<10; w++)
		{
			g[j].users_pids[w] = 0;
		}
	}
}

void LoadGroupsNames(struct group *g) 
{
	FILE *f = fopen("groupslist.txt", "r");

	if (f == NULL)
	{
		printf("Otwieranie pliku nie powiodlo sie. \n");
	}

	int i = 0; 		
	for (i=0; i<3; i++)
	{
		fscanf(f, "%s", g[i].group_name);
	}
	fclose(f);
}

void Login(struct group *g, struct msgbuf m, struct users *u, int id)
{
	m.status = 0;
	m.cmd = 1;
	m.type = m.pid;
	int i = 0;
	int impossible = 0;  

	//Check if already signed in
	for(i = 0; i < 18; i++)
	{
		if(strcmp(m.nick, u[i].nick) == 0) //if nick is used
		{	
			m.status = 1;
			msgsnd(id, &m, sizeof(m), 0);
			impossible = 1;
			break;
		}
		if(m.pid == u[i].pid)	//signed in
		{
			m.status = 2;
			strcpy(m.nick, u[i].nick);
			msgsnd(id, &m, sizeof(m), 0);
			impossible = 1;
			break;
		}
	}

	//Add user
	if(!impossible)
	{
	int j = 0;
	for(j = 0; j < 18; j++)
		{
		if( strcmp(u[j].nick, "") == 0 )
			{
				strcpy(u[j].nick, m.nick);
				u[j].pid = m.pid;	
				m.status = 0;
				msgsnd(id, &m, sizeof(m), 0);
				impossible = 1;
				break;
			}
		}
	}

	if(!impossible)
	{
		m.status = 3;
		msgsnd(id, &m, sizeof(m), 0);
	}

}

void ShowUsers(struct users *u, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 2;
	int i = 0;
	strcpy(m.text, "");

	for(i = 0; i<18; i++)
	{
		if(strcmp(u[i].nick, "") != 0)
		{	//Do not print empty user's nicks
			strcat(m.text, u[i].nick);
			strcat(m.text, ";\n");
		}
	}
	m.status = 0;
	msgsnd(id, &m, sizeof(m), 0);
}

void ShowGroups(struct group *g, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 3;
	int i=0;
	strcpy(m.text, "");
	for(i=0; i<3; i++)
	{
		if(strlen(g[i].group_name) != 0)
		{
			strcat(m.text, g[i].group_name);
			strcat(m.text, "\n");
		}
	}
	msgsnd(id, &m, sizeof(m), 0);
}

void ChangeName(struct users *u, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 4;
	int i = 0;
	int j = 0;

	for(j = 0; j<18; j++)
	{
		if(u[i].nick == m.nick) //if nick exists
		{
			m.status = 1;
			msgsnd(id, &m, sizeof(m), 0);
		}
	}

	for(i = 0; i<18; i++)
	{
		if(u[i].pid == m.pid) //O.K.
		{
			strcpy(u[i].nick, m.nick);
			m.status = 0; //if works
			msgsnd(id, &m, sizeof(m), 0);
			break;
		}
		else //Not logged
		{
			m.status = 8;
			msgsnd(id, &m, sizeof(m), 0);
		}
	}
}

void AddToGroup(struct group *g, struct users *u, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 5;
	int i = 0;
	int j = 0;
	int k = 0;
	int k1 = 0;
	int is_logged = 0;
	int exist = 0;
	int user_number = 0;
	int already_in_group = 0;
	int group_index = 0;
	int free_slot = 0;

	for(i=0; i<18; i++)
	{
		if (u[i].pid == m.pid) //if logged
		{
			is_logged = 1;
			break;
		}
	}

	if(is_logged == 1) //if logged
	{
		for(j=0; j<3; j++)
		{
			if( strcmp(g[j].group_name, m.nick) == 0) 
			{
				exist = 1;
				group_index = j;
				break;
			}
		}
	}
	else //if not logged
	{
		m.status = 8;
		msgsnd(id, &m, sizeof(m), 0);
	}

	if(exist == 1)
	{
		for(k=0; k<10; k++)
		{
			if(g[group_index].users_pids[k] != 0)
			{
				user_number++;
			}
		}
	}
	else //if group doesn't exist
	{
		m.status = 5;
		msgsnd(id, &m, sizeof(m), 0);
	}

	if(user_number == 10) //10 is max number of users
	{
		m.status = 3;
		msgsnd(id, &m, sizeof(m), 0); //Array is full
	}
	else
	{
		for(k1=0; k1<10; k1++)
		{
			if(g[group_index].users_pids[k1] == m.pid)
			{
				already_in_group = 1;
				break;
			}
		}
	}

	if(already_in_group == 1)
	{
		m.status = 4;
		msgsnd(id, &m, sizeof(m), 0); //You are already in group
	}
	else
	{	
		for(i=0; i<10; i++)
		{
			if(g[group_index].users_pids[i] == 0) //first free slot in array
			{
				free_slot = i;
				break;
			}
		}

		g[group_index].users_pids[free_slot] = m.pid;
		m.status = 0; 
		msgsnd(id, &m, sizeof(m), 0); //Not already in group
	}
}

void QuitFromGroup(struct users *u, struct group *g, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 6;
	int i = 0;
	int is_logged = 0;
	int j = 0;
	int in_group = 0;
	int group_not_exist = 0;

	for(i=0; i<18; i++)
	{
		if (u[i].pid == m.pid) //if logged
		{
			is_logged = 1;
			break;
		}
	}
	if(is_logged == 1)
	{
		for(i=0; i<3; i++)
		{
			if( strcmp(g[i].group_name, m.nick) == 0 )
			{
				group_not_exist = 1;
				for(j=0; j<10; j++)
				{
					if(g[i].users_pids[j] == m.pid)
					{
						g[i].users_pids[j] = 0;
						m.status = 0;
						msgsnd(id, &m, sizeof(m), 0);
						in_group = 1;
						break;
					}
				}	
				if(in_group == 0) 
				{	
					m.status = 5;
					msgsnd(id, &m, sizeof(m), 0);
				}
			}
		}
		if(group_not_exist == 1)
		{
			//if group doesn't exists
				m.status = 6;
				msgsnd(id, &m, sizeof(m), 0);
		}
	}
	else
	{ //if not logged
		m.status = 8;
		msgsnd(id, &m, sizeof(m), 0);
	}
}

void SendMessage(struct users *u, struct group *g, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 8;
	int i = 0;
	int j = 0;
	int is_logged = 0;
	int user_exist = 0;

	for(i=0; i<18; i++) //check if logged - working
	{
		if(m.pid == u[i].pid)
		{
			is_logged = 1;
			break;
		}
	}

	if(is_logged == 1)
	{
		for(j=0; j<18; j++)
		{
			if( strcmp(m.nick, u[j].nick) == 0 ) //nick founded
			{
				m.status = 0;
				user_exist = 1;
				m.type = m.pid; //to user who sent
				msgsnd(id, &m, sizeof(m), 0);

				//to user who gets the message
				m.type = u[j].pid;
				
				for(i=0; i<18; i++)
				{
					if(u[i].pid == m.pid)
					{
						strcpy(m.nick, u[i].nick);
					}
				}
				msgsnd(id, &m, sizeof(m), 0);
				break;
			}
		}
		if(user_exist != 1)	//nick not founded
		{	
			m.status = 7;
			msgsnd(id, &m, sizeof(m), 0);
		}
	}
	else
	{
		//not logged - working
		m.status = 8;
		msgsnd(id, &m, sizeof(m), 0);

	}
	
}

void SendToGroup(struct users *u, struct group *g, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 9;
	int i = 0;
	int j = 0;
	int is_logged = 0;
	int user_in_group = 0;

	for(i=0; i<18; i++) //check if logged
	{
		if(m.pid == u[i].pid)
		{
			is_logged = 1;
			break;
		}
	}

	if (is_logged == 1)
	{
		for(i=0; i<3; i++)
		{
			if (strcmp(g[i].group_name, m.nick) == 0)
			{

				for(j=0; j<10; j++)
				{
					if (g[i].users_pids[j] == m.pid)
					{
						user_in_group = 1;

						m.type = g[i].users_pids[j]; //to user who sent message
						m.status = 0;
						msgsnd(id, &m, sizeof(m), 0);

						break;
					}
				}

			}
			else
			{
				//group doesn't exists
				m.status = 5;
				msgsnd(id, &m, sizeof(m), 0);
			}
		}
	}
	else
	{
		m.status = 8;
		msgsnd(id, &m, sizeof(m), 0);
	}

	if (user_in_group == 1)
	{

		for(i=0; i<3; i++)
		{
			for(j=0; j<10; j++)
			{
				if (g[i].users_pids[j] != 0) //if there's user
				{
					
					m.type = g[i].users_pids[j]; //to other users in group
					msgsnd(id, &m, sizeof(m), 0);

				}
			}
		}

	}
	else
	{	//user not in group
		m.status = 6;
		msgsnd(id, &m, sizeof(m), 0);
	}
}

void Logout(struct users *u, struct group *g, struct msgbuf m, int id)
{
	m.type = m.pid;
	m.cmd = 1;
	int i = 0;
	int j = 0;
	int is_logged = 0;

	for(i=0; i<18; i++) //check if logged - working
	{
		if(m.pid == u[i].pid)
		{
			is_logged = 1;
			break;
		}
	}

	if(is_logged == 1)
	{
		for(i=0; i<18; i++) //delte from users
		{
			if (u[i].pid == m.pid)
			{
				u[i].pid = 0;
				strcpy(u[i].nick, "");
				break;
			}
		}

		for(i=0; i<3; i++) //delete from groups
		{
			for(j=0; j<10; j++)
			{
				if (g[i].users_pids[j] == m.pid )
				{
					g[i].users_pids[j] = 0;
					break;
				}
			}
		}
		m.status = 0;
		msgsnd(id, &m, sizeof(m), 0);

	}
	else
	{	//if not logged
		m.status = 8;
		msgsnd(id, &m, sizeof(m), 0);
	}
}

int main(int argc, char* argv[])
{
	struct msgbuf message;
	struct group groups[3];
	struct users users[18];
	Clear(users, groups);
	LoadGroupsNames(groups);

	int detect = msgget(15071410, 0644 | IPC_CREAT);
	int end_app = 0;
	int response = 0;

	while(!end_app)
	{
		response = msgrcv(detect, &message, sizeof(message), 1, MSG_NOERROR);

		if (response == -1)
		{
			printf("Blad serwera! \n");
		}
		else
		{
			switch(message.cmd)
			{
				case 0:
					break;
				case 1:
					Login(groups, message, users, detect);
					break;
				case 2:
					ShowUsers(users, message, detect);
					break;
				case 3:
					ShowGroups(groups, message, detect);
					break;
				case 4:
					ChangeName(users, message, detect);
					break;
				case 5:
					AddToGroup(groups, users, message, detect);
					break;
				case 6:
					QuitFromGroup(users, groups, message, detect);
					break;
				case 8:
					SendMessage(users, groups, message, detect);
					break;
				case 9:
					SendToGroup(users, groups, message, detect);
				case 10:
					Logout(users, groups, message, detect);
					break;
			}
		}
	}

	return 0;
}