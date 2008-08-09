#include <stdio.h>
#include <arpa/inet.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/un.h>

    #define SOCK_PATH "echo_socket"

    int main(void)
    {
        int s, t, len,zzzz;
        struct sockaddr_un remote;
        char str[100], result[1];

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

        while(printf("> "), fgets(str, 100, stdin), !feof(stdin)) {

	
	if((sizeof(str)/sizeof(*str)) > 1 && str[1] > 32) {
		printf("incorrect command\n");
		exit(0);
        } 
	
            if (send(s, str, strlen(str), 0) == -1) {
                perror("send");
                exit(1);
            }
            		char listing[30];
            		int looper,  files=0, info[2];
            		char read_data[1000];
            		char *cmdmain1;
            		int choice;
            		if(str[0]=='q' || str[0]=='Q')
            		{
						printf("Quitting Client\n");
						break;
					}
            		switch(str[0])
            		{
						case 'f':
						case 'F':
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
							break;
						case 'c':
						case 'C':

								if ((t=recv (s, result, 1, 0)) > 0)
								{
										switch(result[0])
										{
											case '0':
													printf("File Created Successfully\n");
													break;
											case '1':
													printf("File Name Already exists\n");
													break;
											case '2':
													printf("Disk Full\n");
													break;
										}
								}
								else
								{
									if (t < 0) perror("recv");
									else printf("Server closed connection....Quitting Client\n");
									exit(1);
								}
							break;
						case 'd':
						case 'D':
								if ((t=recv (s, result, 1, 0)) > 0)
								{
										switch(result[0])
										{
											case '0':
													printf("File Deleted Successfully\n");
													break;
											case '1':
													printf("File Name Does Not exist\n");
													break;
										}
								}
								else
								{
									if (t < 0) perror("recv");
									else printf("Server closed connection....Quitting Client\n");
									exit(1);
								}
							break;
						case 'l':
						case 'L':
							cmdmain1 = strtok(str," ");
							cmdmain1 = strtok(NULL,"\n");
							choice = atoi(cmdmain1);
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
								if ((t=recv (s, listing, 30, 0)) > 0)
								{
										printf("File%d= %s\n",looper,listing);

										if(choice == 1)
										{
											if ((t=recv (s, info, 2*sizeof(int), 0)) > 0)
											{
													printf("File%d Length= %d\n",looper,ntohl(info[0]));
													printf("File%d found at block %d\n",looper,ntohl(info[1]));
											}
											else
											{
												if (t < 0) perror("recv");
												else printf("Server closed connection....Quitting Client\n");
												exit(1);
											}
										}
										printf("----------------------------------------------------------------\n");
								}
								else
								{
									if (t < 0) perror("recv");
									else printf("Server closed connection...Quitting Client\n");
									exit(1);
								}
							}
							break;

						case 'r':
						case 'R':

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
										strcpy(read_data,"");
								}
								else
								{
									if (t < 0) perror("recv");
									else printf("Server closed connection.... Quitting Client\n");
									exit(1);
								}
							break;

						case 'w':
						case 'W':
						case 'A':
						case 'a':
							  if ((t=recv (s, result, 1, 0)) > 0)
								{
										switch(result[0])
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
									else printf("Server closed connection .... Quitting Client\n");
									exit(1);
								}
							break;

						case 'p':
						case 'P':
								if ((t=recv (s, result, 1, 0)) > 0)
								{
										switch(result[0])
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
									else printf("Server closed connection .... Quitting Client\n");
									exit(1);
								}
							break;

					}//end of switch for command
					strcpy(str,"");
        }//end of while loop

        close(s);

        return 0;
    }
