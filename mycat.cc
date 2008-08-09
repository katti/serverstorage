    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/un.h>
	#include <sys/wait.h>
	#include <unistd.h>

#include<iostream>
using namespace std;

    #define SOCK_PATH "echo_socket"

string make_string(char* coming)
{
	string data(coming);
	return coming;
}


    int main(void)
    {
        int s, t, len,zzzz;
        struct sockaddr_un remote;
        char str[100];

        if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        printf("Trying to connect...\n");

        remote.sun_family = AF_UNIX;
        strcpy(remote.sun_path, SOCK_PATH);
        len = strlen(remote.sun_path) + sizeof(remote.sun_family);
        if (connect(s, (struct sockaddr *)&remote, len) == -1) {
            perror("connect");
            exit(1);
        }

        printf("Connected.\n");
        printf("This is mycat command\n Enter command in \"mycat filename\" format\n");
        printf("Enter Q or q to quit\n");

        while(printf("> "), fgets(str, 100, stdin), !feof(stdin))
        {

            		char read_data[1000];
            		char *cmdmain1;
            		string command;

            		cmdmain1 = strtok(str," ");
            		command = make_string(cmdmain1);
            		//printf("command %s\n",command);
            		if(command == "mycat")
            		{
				cmdmain1=strtok(NULL,"\n");
				printf("filename %s\n",cmdmain1);
				sprintf(str,"r %s",cmdmain1);
				if (send(s, str, strlen(str), 0) == -1)
				{
					perror("send");
					exit(1);
            			}

            			if ((t=recv (s, read_data, 1000, 0)) > 0)
				{
					switch(read_data[0])
					{
						case '0':
							printf("File read successfully with status %c\n The file contents are\n",read_data[0]);
							for(zzzz=1; zzzz < strlen(read_data);zzzz++)
								printf("%c",read_data[zzzz]);
							printf("\n");
							break;
						case '1':
							printf("File Does not exist\n");
							break;
						case '2':
							printf("No data to read in the file\n");
							break;
					}
				}
				else
				{
					if (t < 0) perror("recv");
					else printf("Server closed connection.... Quitting Client\n");
					exit(1);
				}


			}

            		else if(str[0]=='q' || str[0]=='Q')
            		{
						printf("Quitting Client\n");
						break;
			}
			else
			{
				printf("Invalid command\n");
			}

	 	strcpy(str,"");
        }//end of while loop

        close(s);

        return 0;
    }
