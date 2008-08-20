/*
 * Copyright (c) 2008 Samir Katti(samir.katti@gmail.com)
 * myls.cc
 * This file is lists all the files in the disk.
 *
 * FileStorage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FileStorage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 *You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
    
#include <stdio.h>
    #include <arpa/inet.h>
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
        printf("This is myls command\n Enter \"myls\" to get the directory list\n");
        printf("Enter Q or q to quit\n");

        while(printf("> "), fgets(str, 128, stdin), !feof(stdin))
        {
            		char *cmdmain1;
            		string command;
			char data[128],read_data[0],listing[200];
			int files=0,looper;
			

            		cmdmain1 = strtok(str,"\n");
            		command = make_string(cmdmain1);

            		if(command == "myls")
            		{
				sprintf(str,"l %d",1);

				if (send(s, str, strlen(str), 0) == -1)
				{
					perror("send");
					exit(1);
            			}

            			if ((t=recv (s, &files, sizeof(int), 0)) > 0)
				{
					files = ntohl(files);
					printf("Number of files present =  %d\n",files);
				}

				else
				{
					if (t < 0) perror("recv");
					else printf("Server closed connection....Quitting Client\n");
					exit(1);
				}
				for(looper=0;looper<files;looper++)
				{
					if ((t=recv (s, listing, 200, 0)) > 0)
					{
						printf("File%d= %s\n",looper,listing);
						printf("----------------------------------------------------------------\n");
					}
					else
					{
						if (t < 0) perror("recv");
						else printf("Server closed connection...Quitting Client\n");
						exit(1);
					}
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
