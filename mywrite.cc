/*
 * Copyright (c) 2008 Samir Katti(samir.katti@gmail.com)
 * mywrite.cc
 * This file is the write file.
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
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
    
#include <stdio.h>
    #include <stdlib.h>
    #include <arpa/inet.h>
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
        printf("This is mywrite command\n Enter command in \"mywrite filename\" format\n");
        printf("Enter Q or q to quit\n");

        while(printf("> "), fgets(str, 128, stdin), !feof(stdin))
        {
            		char *cmdmain1;
            		string command,filename;
			char data[128],read_data[0];

            		cmdmain1 = strtok(str," ");
            		command = make_string(cmdmain1);

            		if(command == "mywrite")
            		{
				cmdmain1=strtok(NULL,"\n");
				filename = make_string(cmdmain1);
				printf("filename %s\n",cmdmain1);
				
				printf("Enter text to write into file\n");
				fgets(data,128,stdin);

				sprintf(str,"w %s %d %s",filename.c_str(),strlen(data),data);

				if (send(s, str, strlen(str), 0) == -1)
				{
					perror("send");
					exit(1);
            			}

            			if ((t=recv (s, read_data, 1, 0)) > 0)
				{
					switch(read_data[0])
					{
						case '0':
								printf("File Written Successfully\n");
								break;
						case '1':
								printf("File Name does not exist\n");
								break;
						case '2':
								printf("Disk Full\n");
								break;
						case '3':
								printf("Length cannot be less than the amount of data entered\n");
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
