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
        int s, t, len;
        struct sockaddr_un remote;
        char str[128];

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
        printf("This is mynewfs command\n Enter \"mynewfs\" to format the drive\n");
        printf("Enter Q or q to quit\n");

        while(printf("> "), fgets(str, 128, stdin), !feof(stdin))
        {
            		char *cmdmain1;
            		string command;
			char data[128],result[0];
			int files=0,looper;
			

            		cmdmain1 = strtok(str,"\n");
            		command = make_string(cmdmain1);

            		if(command == "mynewfs")
            		{
				str[0]='f';

				if (send(s, str, 1, 0) == -1)
				{
					perror("send");
					exit(1);
            			}

            			if ((t=recv (s, result, 1, 0)) > 0)
				{
					switch(result[0])
					{
						case '0':
							printf("Disk Successfully formatted\n");
							break;
					}
				}
				else
				{
					if (t < 0) perror("recv");
					else printf("Server closed connection...Quitting Client\n");
					exit(1);
				}
			}

            		else if(str[0]=='q' || str[0]=='Q')
			{
				printf("Quitting Client\n");
				if (send(s, str, 1, 0) == -1)
				{
					perror("send");
					exit(1);
            			}				
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
