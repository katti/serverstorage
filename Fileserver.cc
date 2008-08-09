	#include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/un.h>
	#include <sys/wait.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <ctype.h>
	#include <iostream>
	#include <math.h>
	#include <sstream>

    #define SOCK_PATH "echo_socket"

    using namespace std;

    FILE *hdd ;

        int HDD_CYLINDERS = 0;
	int HDD_SECTORS = 0;
	int TRACK_TIME = 0;

    struct my_block
    {
		int cylinder;
		int sector;
		int next;
		int prev;
		string filename;
		int used;
		int length;
   } ;

void write_to_file(int c, int s, char d[], int till)
{
        int offset = (HDD_SECTORS*128) * (c) + 128 * (s);
	cout<<"offset= "<<offset<<endl;

	printf("Sleeping for the simulation of track to track time\n");
	if ( usleep((useconds_t)( (HDD_SECTORS*(c)*TRACK_TIME)+ ( (s)*TRACK_TIME) ) ) == 0)
	{
		printf("Completed Sleep - Disk Head now ready to write\n");
	}	
	if( fseek(hdd, offset, SEEK_SET) != 0 )
	{
		perror("seek error for offset\n");
                exit(1);
	}
	int data_loop, remaining;
	if(till<128)
	{
		for(data_loop=0; data_loop < strlen(d);data_loop++)
		{
			fputc(d[data_loop], hdd);
		}
		remaining = 128 -  strlen(d) ;
		for(data_loop=0; data_loop < remaining ; data_loop++)
		{
			fputc('0',hdd);
		}

	}
	else if(till==128)
	{
		cout<<"i am in first block"<<endl;
		if(strlen(d) >= 128)
		{
			for(data_loop=0; data_loop < 128;data_loop++)
			{
				fputc(d[data_loop], hdd);
			}		
		}
		else
		{
			for(data_loop=0; data_loop < strlen(d);data_loop++)
			{
				fputc(d[data_loop], hdd);
			}
			remaining = 128 -  strlen(d) ;
			for(data_loop=0; data_loop < remaining ; data_loop++)
			{
				fputc('0',hdd);
			}
		}
	}
	else 
	{	
	cout<<"i am in second block"<<endl;
		if(strlen(d) >= till)
		{
			for(data_loop = (till-128); data_loop < till ; data_loop++)
			{
				fputc(d[data_loop], hdd);
			}
		}
		else
		{			
			for(data_loop = 0; data_loop < 128 ; data_loop++)
			{
				fputc('0', hdd);
			}
		}
	}
	cout<<"Completed writing of a block into file\n";
}

void copy_to_file(int c, int s, char d[], int till)
{
        int offset = (HDD_SECTORS*128) * (c) + 128 * (s);
	cout<<"inside copy file offset= "<<offset<<endl;

	if ( usleep((useconds_t)( (HDD_SECTORS*(c)*TRACK_TIME)+ ( (s)*TRACK_TIME) ) ) == 0)
	{
	}	
	if( fseek(hdd, offset, SEEK_SET) != 0 )
	{
		perror("seek error for offset\n");
                exit(1);
	}
	int data_loop;
	for(data_loop = (till-128); data_loop < till ; data_loop++)
	{
		fputc(d[data_loop], hdd);
	}
	cout<<"Completed writing of a block into file\n";
}

void append_to_file(int c, int s, char d[], int till,int inner_offset)
{
	cout<<"append to file\n";
        int offset = (HDD_SECTORS*128) * (c) + 128 * (s)+inner_offset;
	cout<<" c "<<c<<" "<<" s "<<s<<endl;
	cout<<"offset in append file method= "<<offset<<endl;


	printf("Sleeping for the simulation of track to track time\n");
	if ( usleep((useconds_t)( (HDD_SECTORS*(c)*TRACK_TIME)+ ( (s)*TRACK_TIME) ) ) == 0)
	{
		printf("Completed Sleep - Disk Head now ready to write\n");
	}	
	if( fseek(hdd, offset, SEEK_SET) != 0 )
	{
		perror("seek error for offset\n");
                exit(1);
	}
	int data_loop, remaining;
	if(till<128)
	{
		cout<<"till 1= "<<till<<endl;
		for(data_loop=0; data_loop < strlen(d);data_loop++)
		{
			fputc(d[data_loop], hdd);
		}
		remaining = 128 -  strlen(d) - inner_offset ;
		for(data_loop=0; data_loop < remaining ; data_loop++)
		{
			fputc('0',hdd);
		}

	}
	else if(till==128)
	{
		cout<<"i am in first block"<<endl;
		if(strlen(d) >= 128)
		{
			for(data_loop=0; data_loop < 128;data_loop++)
			{
				fputc(d[data_loop], hdd);
			}		
		}
		else
		{
					cout<<"till 2= "<<till<<endl;
			cout<<"i am inserting in here ------------\n";
			for(data_loop=0; data_loop < strlen(d);data_loop++)
			{				
				fputc(d[data_loop], hdd);
			}
			remaining = 128 -  strlen(d)  - inner_offset;
			for(data_loop=0; data_loop < remaining ; data_loop++)
			{
				fputc('0',hdd);
			}
		}
	}
	else 
	{	
	cout<<"i am in second block"<<endl;
		if(strlen(d) >= till)
		{
			for(data_loop = (till-128); data_loop < till ; data_loop++)
			{
				fputc(d[data_loop], hdd);
			}
		}
		else
		{			
				cout<<"till 3= "<<till<<endl;
			for(data_loop = 0; data_loop < 128 ; data_loop++)
			{
				fputc('0', hdd);
			}
		}
	}
	cout<<"Completed writing of a block into file\n";
}

void clear_contents(int c, int s)
{
	int offset = (HDD_SECTORS*128) * (c) + 128 * (s);
	cout<<"offset= "<<offset<<endl;

	printf("Sleeping for the simulation of track to track time\n");
	if ( usleep((useconds_t)( (HDD_SECTORS*(c)*TRACK_TIME)+ ( (s)*TRACK_TIME) ) ) == 0)
	{
		printf("Completed Sleep - Disk Head now ready to delete\n");
	}	
	if( fseek(hdd, offset, SEEK_SET) != 0 )
	{
		perror("seek error for offset\n");
                exit(1);
	}	
	for(int data_loop = 0;data_loop<128;data_loop++)
	{
		fputc(' ',hdd);
	}
}

string make_string(char* coming)
{
	string data(coming);
	return coming;
}
    int main(int argc, char *argv[])
    {

		if(argc != 4)
		{
			printf("Please enter 4 arguments in this order: \n1. No. of Cylinders\n2. No. of Sectors per cylinder\n3. Track-to-Track time\n");
			exit(1);
		}

		int cyl, sec;
		HDD_CYLINDERS = atoi(argv[1]);
		HDD_SECTORS = atoi(argv[2]);
		TRACK_TIME = atoi(argv[3]);
		int total_blocks = HDD_CYLINDERS  * HDD_SECTORS ;
		struct my_block table[total_blocks];
		
		FILE *file;
		
		int actual_block;
		char line[30];
		char *cmdmain1;
		int looping,inner_looping;
		
	

	int s, s2, t, len;
        struct sockaddr_un local, remote;
        char str[128];

        if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        local.sun_family = AF_UNIX;
        strcpy(local.sun_path, SOCK_PATH);
        unlink(local.sun_path);
        len = strlen(local.sun_path) + sizeof(local.sun_family);
        if (bind(s, (struct sockaddr *)&local, len) == -1) {
            perror("bind");
            exit(1);
        }
        if (listen(s, 5) == -1) {
            perror("listen");
            exit(1);
        }

        for(;;) {

		
			int  n;
            cout<<"Waiting for a connection...\n";
            t = sizeof(remote);
            if ((s2 = accept(s, (struct sockaddr *)&remote, (socklen_t*)&t)) == -1) {
                perror("accept");
                exit(1);
            }

            cout<<"Connected.\n";

	    hdd = fopen("hdd1.txt", "r+");
		if(hdd==NULL)
		{
			cout<<"could not create file\n";
			perror("file creation");
			exit(1);
		}


	    /*code to read HDD*/
		file = fopen("table.txt", "r+");
		/*file doesnt exist, initializing the block structure values*/
		if(file==NULL)
		{

		       cout<< "Table file doesnt exist, initializing the block structure values\n";

		       actual_block=0;
		       for( looping = 0; looping < HDD_CYLINDERS; looping++)
		       	for( inner_looping = 0; inner_looping < HDD_SECTORS; inner_looping++)
			{
				table[actual_block].cylinder = looping;
				table[actual_block].sector = inner_looping;
				table[actual_block].next = -5000;
				table[actual_block].prev = -5000;
				table[actual_block].filename = "";
				table[actual_block].used = 0;
				table[actual_block].length = 0;
				actual_block++;
			}
		}

		/*table exists, load all structures onto memory*/
		else
		{
			actual_block=0;
			
			while(actual_block < total_blocks)
			{
				char line[200];	
				fgets(line,200,file);
				
				cmdmain1 =  strtok(line," ");
				table[actual_block].cylinder = atoi(cmdmain1);

				cmdmain1 = strtok(NULL," ");
				table[actual_block].sector = atoi(cmdmain1);

				cmdmain1 = strtok(NULL," ");
				table[actual_block].next = atoi(cmdmain1);

				cmdmain1 = strtok(NULL," ");
				table[actual_block].prev = atoi(cmdmain1);

				cmdmain1 = strtok(NULL," ");
				string data(cmdmain1);
				table[actual_block].filename = cmdmain1;
				if(table[actual_block].filename=="unused")
					table[actual_block].filename = "";

				cmdmain1 = strtok(NULL," ");
				table[actual_block].used = atoi(cmdmain1);

				cmdmain1 = strtok(NULL,"\n");	
				table[actual_block].length = atoi(cmdmain1);

				//cout<<actual_block<<endl;
				cout<<table[actual_block].cylinder<<" "<<table[actual_block].sector<<" "<<table[actual_block].next<<" "<<table[actual_block].prev<<" "<<table[actual_block].filename<<" "<<table[actual_block].used<<" " <<table[actual_block].length<<endl;
				actual_block++;

				if(actual_block == total_blocks)
					break;
				
			}
		}
		/*end code to read HDD*/

            int  looper, done=0;
	    
            do
            {	
	                n = recv(s2, str, 128, 0);

			if (n < 0) perror("recv");
			else if(n==0) done = 1;

			if(!done)
			{
				int break_check = 0 ;
				string fileName, str_data,fileName2;
				char data[128];
				int free_blocks;
				int so_far=0;
				int first_time = 1;
				int prev_written_block = -1;
				int choice;
				int info[2];
				int Length;

				char listing[30];

				switch(str[0])
				{
					case 'f':
					case 'F':
						for( looping = 0; looping < total_blocks; looping++)
						{
							if(table[looping].length > 0)
							{
								clear_contents(table[looping].cylinder,table[looping].sector);
							}

							table[looping].next = -5000;
							table[looping].prev = -5000;
							table[looping].filename = "";
							table[looping].used = 0;
							table[looping].length = 0;
							
						}
						str[0] = '0';
						if ( send (s2, str, 1, 0) <0 )
							perror("send");
						strcpy(str,"");
						break;
					case 'c':
					case 'C':
						break_check = 0 ;
						cmdmain1 = strtok(str," ");
						cmdmain1 = strtok(NULL,"\n");

						fileName= make_string(cmdmain1);

						looping = 0;
						while(looping < total_blocks)
						{
							if(table[looping].filename == fileName)
							{
								cout<<"cannot create file name " <<fileName<<" because it already exists\n";
								break_check = 1;
								str[0] = '1';
								if ( send (s2, str, 1, 0) <0 )
									perror("send");
								strcpy(str,"");
								break;
							}

							looping++;
						}
						if(!break_check)
						{
							looping = 0 ;
							while(table[looping].used != 0 && looping < total_blocks)
							{
								looping++;

							}
							if(looping == (total_blocks  ) )
							{
								cout<<"Disk Full Cannot create file\n";
								str[0] = '2';
								if ( send (s2, str, 1, 0) <0 )
									perror("send");
								strcpy(str,"");
							}
							else
							{
								table[looping].filename = fileName;
								table[looping].used = 1;								
								table[looping].prev = -1;//make this the first block for the file
								table[looping].next = -1;
								str[0] = '0';
								cout<<"File "<<table[looping].filename<<" created in cylinder# "<<table[looping].cylinder<<" and sector# "<<table[looping].sector <<endl;
								if ( send (s2, str, 1, 0) <0 )
									perror("send");
								strcpy(str,"");
							}
						}
					break;

				     case 'd':
				     case 'D':
						
						cmdmain1 = strtok(str," ");

						cmdmain1 = strtok(NULL,"\n");
						fileName = make_string(cmdmain1);/*get file name*/
						
						break_check = 0 ;
						looping=0;
						while(looping < total_blocks)
						{
							if(table[looping].filename == fileName)
							{
								/*Initialize all values pertaining to this file making its space available for other files*/
								table[looping].next = -5000;
								table[looping].prev = -5000;
								table[looping].filename = "";
								table[looping].used = 0;
								table[looping].length = 0;

								break_check  = 1 ;
								clear_contents(table[looping].cylinder,table[looping].sector);
							}
							looping++;
						}
						if(break_check == 0)
						{
							str[0] = '1';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							cout<<"File "<<fileName<<" does not exist. Deletion not possible"<<endl;
						}
						else
						{
							str[0] = '0';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							cout<<"File "<<fileName<<" deleted. Deletion successfull"<<endl;

						}
						strcpy(str,"");
						printf("after clearing str=%s %d\n",str,strlen(str));
					break;
				     case 'l':
				     case 'L':
						cmdmain1 = strtok(str," ");
						cmdmain1 = strtok(NULL,"\n");
						choice = atoi(cmdmain1);

						looping = 0;
						inner_looping = 0;
						while(looping < total_blocks )
						{
							if(table[looping].filename != "" && table[looping].prev==-1)
							{
								inner_looping++;
							}
							looping++;
						}
						inner_looping = htonl(inner_looping);
						if ( send (s2, &inner_looping, sizeof(int), 0) <0 )
							perror("send");

						looping = 0;
						while(looping < total_blocks )
						{
							if(table[looping].filename != "" && table[looping].prev==-1)
							{	cout<<"table[looping].filename="<<table[looping].filename<<endl;
								sprintf(listing,"%s",table[looping].filename.c_str());
								if ( send (s2, listing, 30, 0) <0 )
									perror("send");
								if(choice == 1)
								{
									info[0] = htonl(table[looping].length);
									info[1] = htonl(looping);
									if ( send (s2, info, 2* sizeof(int), 0) <0 )
										perror("send");
								}									
							}
							looping++;
						}
						strcpy(str,"");
					break;

				     case 'r':
				     case 'R':
					{
						cmdmain1 = strtok(str," ");
						cmdmain1 = strtok(NULL,"\n");
						fileName = make_string(cmdmain1);/*get file name*/
						

						looping = 0;
						/*get the block at which this file is created*/
						while(looping<total_blocks)
						{
							if(table[looping].filename == fileName && table[looping].prev==-1)
							{
								if(table[looping].length > 0)
								{
									int  no_of_times = 0, offset,i,size;
									so_far = 1;

									no_of_times = ceil((double)table[looping].length/128);
									size = no_of_times * 128 ;
									cout<<"size="<<size<<"no of times = "<<no_of_times<<endl;
									char read_data[size];
									read_data[0]='0';
									while (  no_of_times != 0 )
									{
										/*insert code to read all blocks belonging to this file*/
										
										offset = (HDD_SECTORS*128) * (table[looping].cylinder) + 128 * (table[looping].sector);
										if ( usleep((useconds_t)( (HDD_SECTORS*(table[looping].cylinder)*TRACK_TIME)+ ( (table[looping].sector)*TRACK_TIME) ) ) == 0)
										{
										}	
										if( fseek(hdd, offset, SEEK_SET) != 0 )
										{
											perror("seek error for offset\n");
											exit(1);
										}
										
										if(table[looping].length < 128)
										{
											for(i=1;i<table[looping].length+1;i++)
											{
												read_data[i] = fgetc(hdd);
											}
										}
										else if(no_of_times==1)
										{
											for(i=0;i<(table[looping].length%128);i++)
											{											
												read_data[so_far]=fgetc(hdd);
												so_far++;
											}
										}
										else
										{
											for(i=0;i<128;i++)
											{
												read_data[so_far]=fgetc(hdd);
												so_far++;
											}
										}
										cout<<"looping= "<<looping<<endl;
										looping = table[looping].next;
										no_of_times--;
									}//end of while	
									cout<<"data= "<<read_data<< " "<<strlen(read_data)<<endl;
									if ( send (s2, read_data, strlen(read_data), 0) <0 )
										perror("send");
									break;
								}
								else
								{
									str[0] = '2';
									if ( send (s2, str, 1, 0) <0 )
										perror("send");
									cout<<"no data to read\n";
									break;
								}
							}//end of if
							looping++;
						}//end of while	
						if(looping==total_blocks)
						{
							str[0] = '1';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							cout<<"Filename "<<fileName<<" does not exist"<<endl;
						}
						strcpy(str,"");
					}
					break;


				     case 'w':
				     case 'W':
						break_check = 0 ;
						cmdmain1 = strtok(str," ");

						cmdmain1 = strtok(NULL," ");
						fileName = make_string(cmdmain1);/*get file name*/

						Length = atoi(strtok(NULL," "));/*get length*/
						
						cmdmain1 = strtok(NULL,"\n");
						str_data = make_string(cmdmain1);
						sprintf(data,"%s",str_data.c_str());
						
						if(Length < strlen(data))
						{
							str[0] = '3';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							strcpy(str,"");
							break;
						}
							
						 /*Get the (cyl,sec) at which the filename is created*/
        					looping = 0;
						while(looping < total_blocks)
						{
							if(table[looping].filename == fileName)
							{
								if(table[looping].prev == -1)
								{
									cout<<"filename "<<fileName<<" found in cylinder#: "<<table[looping].cylinder<<" and sector#: "<<table[looping].sector<<endl;
									table[looping].next = -5000;
									table[looping].length = 0;
									
								}
								/*make all the other blocks unused coz the new data will be written into these blocks if 
								new data is longer than the previous data or old data needs to be wiped out if new data is shorted than
								the old data*/
								else
								{
									table[looping].next = -5000;
									table[looping].prev = -5000;
									table[looping].filename = "";
									table[looping].used = 0;
									table[looping].length = 0;
									
								}
						
								break_check = 1 ;
							}

							looping++;
						}
						if(!break_check)
						{
							if(looping == total_blocks)
							{
			  					 cout<<"file "<<fileName<<" not found, cannot complete write, please create the file first\n";
								 str[0] = '1';
								 if ( send (s2, str, 1, 0) <0 )
									perror("send");
								 strcpy(str,"");
                   						 break;
        						}
						}

						/*check the number of free blocks available in the HDD*/
						cout<<"-------1"<<endl;
						free_blocks =0;
						break_check = 0;
						for(inner_looping = 0; inner_looping<total_blocks;inner_looping++)
						{
							if(table[inner_looping].used == 1 && table[inner_looping].filename == fileName && table[inner_looping].prev==-1)
							{
								free_blocks += 1;
							}

							else if(table[inner_looping].used == 0)
							{
								free_blocks +=1;
								
							}
							
						}
						cout<<"free blocks= "<<free_blocks<<endl;
						if(free_blocks >= ceil((double)Length/128))
						{									
							break_check = 1;
						}

						/*find the free blocks to write*/
						
						if(break_check == 1)
						{
							looping = 0;
							while(table[looping].filename!=fileName)
							{	
								looping++;
							}
							cout<<"filename "<<fileName<<" found in position "<<looping<<endl;
							so_far = 0;
							cout<<"so far "<<so_far<<endl;
							first_time=1;
							while(so_far < Length)
							{	
								if(first_time)
								{
									table[looping].prev = -1; /*make it the first block written*/
									table[looping].length = Length ; /*store the length of the data that is being written*/
									first_time = 0;
									prev_written_block = looping;
									
									if(Length <= 128)
									{
										table[looping].next = -1;//make this the last block
										so_far = Length;
										//call the function to write the data into the flat file
										write_to_file(table[looping].cylinder, table[looping].sector, data,so_far);
										break;
									}
									else
									{
										so_far = 128;
										//call the function to write the data into the flat file
										write_to_file(table[looping].cylinder, table[looping].sector, data,so_far);
									}
									
								}
								else
								{
									inner_looping = 0;
									while(table[inner_looping].used != 0 && inner_looping < total_blocks)
									{
										inner_looping++;
									}
									table[prev_written_block].next = inner_looping;
									table[inner_looping].prev = prev_written_block ;
									prev_written_block = inner_looping;

									table[inner_looping].filename = fileName;
									table[inner_looping].used = 1 ;
									table[inner_looping].length = Length ;
									
									//If we are writing the last block
									if(so_far + 128 >= Length)
									{
										table[inner_looping].next = -1;//make this the last block
										so_far+=128;
										write_to_file(table[inner_looping].cylinder, table[inner_looping].sector, data,so_far);
										break;
									}
									else
									{
										so_far+=128;
										write_to_file(table[inner_looping].cylinder, table[inner_looping].sector, data,so_far);
									}									
								}
							}//end of while where we are writing data to disk
							str[0] = '0';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
						}//break checking flag IF end
						else
						{
							cout<<"Disk Full. Cannot write large data\n";
							cout<<"Suggestions: \n";
							cout<<"Maximum data size that can be written as of now is "<<128*free_blocks<<"bytes"<<endl;
							str[0] = '2';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
						}
						strcpy(str,"");

						for(int testing = 0; testing < total_blocks;testing++)
						{
							cout<<"cyl:"<<table[testing].cylinder <<" ";
							cout<<"sec:"<<table[testing].sector <<" ";
							cout<<"filename:"<<table[testing].filename <<" ";
							cout<<"next:"<<table[testing].next <<" ";
							cout<<"prev:"<<table[testing].prev <<" ";
							cout<<"used:"<<table[testing].used <<" ";
							cout<<"length:"<<table[testing].length <<" ";
							cout<<endl;
						}
					break;
				case 'a':
				case 'A':
					break_check = 0 ;
					cmdmain1 = strtok(str," ");

					cmdmain1 = strtok(NULL," ");
					fileName = make_string(cmdmain1);/*get file name*/

					Length = atoi(strtok(NULL," "));/*get length*/
						
					cmdmain1 = strtok(NULL,"\n");
					str_data = make_string(cmdmain1);
					sprintf(data,"%s",str_data.c_str());
					cout<<"data to write "<<data<<endl;
					if(Length < strlen(data))
					{
						str[0] = '3';
						if ( send (s2, str, 1, 0) <0 )
							perror("send");
						strcpy(str,"");
						break;
					}
					
					 /*Get the (cyl,sec) at which the filename is created*/
					looping = 0;
					while(looping < total_blocks)
					{
						if(table[looping].filename == fileName)
						{		
							break_check = 1 ;
							break;
						}

						looping++;
					}
					if(!break_check)
					{
						if(looping == total_blocks)
						{
							 cout<<"file "<<fileName<<" not found, cannot complete append, please create the file first\n";
							 str[0] = '1';
							 if ( send (s2, str, 1, 0) <0 )
								perror("send");
							 strcpy(str,"");
							 break;
						}
					}

					/*check the number of free blocks available in the HDD*/
					free_blocks =0;
					break_check = 0;
					//Count all completely free blocks
					for(inner_looping = 0; inner_looping<total_blocks;inner_looping++)
					{
						if(table[inner_looping].used == 0)
						{
							free_blocks +=1;							
						}
					}
					//get that block which can fit the remaining data
					for(inner_looping = 0; inner_looping<total_blocks;inner_looping++)
					{
						
						if(table[inner_looping].used == 1 && table[inner_looping].filename == fileName && ( (Length %128) < (128-(table[inner_looping].length % 128) ) ) )
						{
							free_blocks +=1;
						}
					}
					/*find the free blocks to write*/
					if(free_blocks >= ceil((double)Length/128))
					{
						cout<<"Appending to file with length= "<<Length<<" is possible\n";
						break_check = 1;						
					}
					
					if(break_check == 1)
					{						
						for(looping = 0; looping < total_blocks ; looping++)
						{	
							if(table[looping].filename == fileName && table[looping].next == -1)
							{
								break;
							}
						}
						cout<<"looping where append should begin that is the LAST block= "<<looping<<endl;
						cout<<"filename "<<fileName<<" last block found in position "<<looping<<endl;
						cout<<"filename "<<fileName<<" last block found in position "<<looping<<endl;
						int upper_limit;
						so_far=0;
						first_time = 1;
						int new_length=0;
						while(so_far < Length)
						{	
							if(first_time==1)
							{
								first_time = 0;
								prev_written_block = looping;
								upper_limit = (table[looping].length / 128) + 1;
								if(table[looping].length + Length <= (upper_limit * 128) && table[looping].length % 128 !=0)
								{
									table[looping].next = -1;//make this the last block
									so_far = Length;
									//call the function to write the data into the flat file
									append_to_file(table[looping].cylinder, table[looping].sector, data,so_far,(table[looping].length % 128));
									table[looping].length += Length;
									new_length = table[looping].length;
									break;
								}
								else
								{
									so_far=128;
									append_to_file(table[looping].cylinder, table[looping].sector, data,so_far,(table[looping].length % 128));
									new_length = table[looping].length + Length ;
									table[looping].length = new_length;


									inner_looping = 0;
									while(table[inner_looping].used != 0 && inner_looping < total_blocks)
									{
										inner_looping++;
									}
									table[looping].next = inner_looping;
									table[inner_looping].prev = looping ;
									prev_written_block = inner_looping;

									table[inner_looping].filename = fileName;
									table[inner_looping].used = 1 ;
									table[inner_looping].length += Length;
									
									//If we are writing the last block
									if(so_far +128 >= Length)
									{
										table[inner_looping].next = -1;//make this the last block										
										so_far+=128;
										cout<<"breakin out due to last block\n"<<so_far<<endl;
										//call the function to write the data into the flat file
										append_to_file(table[inner_looping].cylinder, table[inner_looping].sector, data,so_far,0);
										break;
									}
									else
									{
										so_far+=128;
										//call the function to write the data into the flat file
										append_to_file(table[inner_looping].cylinder, table[inner_looping].sector, data,so_far,0);
									}
								}
								
							}
							else
							{
								inner_looping = 0;
								while(table[inner_looping].used != 0 && inner_looping < total_blocks)
								{
									inner_looping++;
								}
								table[prev_written_block].next = inner_looping;
								table[inner_looping].prev = prev_written_block ;
								prev_written_block = inner_looping;

								table[inner_looping].filename = fileName;
								table[inner_looping].used = 1 ;
								
								//If we are writing the last block
								if(so_far +128 >= Length)
								{
									table[inner_looping].next = -1;//make this the last block										
									so_far+=128;
									cout<<"breakin out due to last block\n"<<so_far<<endl;
									//call the function to write the data into the flat file
									append_to_file(table[inner_looping].cylinder, table[inner_looping].sector, data,so_far,0);
									break;
								}
								else
								{
									so_far+=128;
									//call the function to write the data into the flat file
									append_to_file(table[inner_looping].cylinder, table[inner_looping].sector, data,so_far,0);
								}								
							}
						}//end of while where we are writing data to disk
						for(int hello = 0; hello< total_blocks;hello++)
						{
							if(table[hello].filename == fileName)
								table[hello].length = new_length;
						}
						str[0] = '0';
						if ( send (s2, str, 1, 0) <0 )
							perror("send");
					}//break checking flag IF end
					else
					{
						cout<<"Disk Full. Cannot append large data\n";
						cout<<"Suggestions: \n";
						cout<<"Maximum data size that can be written as of now is "<<128*free_blocks<<"bytes"<<endl;
						str[0] = '2';
						if ( send (s2, str, 1, 0) <0 )
							perror("send");
					}

					strcpy(str,"");
					
					for(int testing = 0; testing < total_blocks;testing++)
					{
						cout<<"cyl:"<<table[testing].cylinder <<" ";
						cout<<"sec:"<<table[testing].sector <<" ";
						cout<<"filename:"<<table[testing].filename <<" ";
						cout<<"next:"<<table[testing].next <<" ";
						cout<<"prev:"<<table[testing].prev <<" ";
						cout<<"used:"<<table[testing].used <<" ";
						cout<<"length:"<<table[testing].length <<" ";
						cout<<endl;
					}

					break;
				
				case 'p':
				case 'P':
				{
						cmdmain1 = strtok(str," ");

						cmdmain1 = strtok(NULL," ");
						fileName = make_string(cmdmain1);/*get file name*/

						cmdmain1 = strtok(NULL,"\n");
						fileName2 = make_string(cmdmain1);/*get file name*/

						break_check = 0 ;
						looping=0;
						while(looping < total_blocks)
						{
							if(table[looping].filename == fileName)
							{
								Length = table[looping].length;
								break_check  = 1 ;
								break;
							}
							looping++;
						}

						if(break_check == 0)
						{
							str[0] = '1';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							cout<<"File "<<fileName<<" does not exist. Copying not possible"<<endl;
						}
						else
						{
							looping = 0;
							free_blocks = 0;
							cout<<fileName2<<endl;
							while(looping < total_blocks)
							{
								if(table[looping].used == 0)
								{
									free_blocks++;
								}
								else if(table[looping].filename == fileName2)
								{
									free_blocks++;
								}
								looping++;
							}
							cout<<free_blocks<<endl;
							if(free_blocks < ceil((double)Length/128))
							{	
								cout<<"Not enough space in HDD"<<endl<<"Free space left= "<<free_blocks*128<<"bytes"<<endl;
								strcpy(str,"");
								str[0] = '2';
								if ( send (s2, str, 1, 0) <0 )
									perror("send");
								break;
							}
							
							looping=0;
							break_check = 0;
							int where_created = -50;
							while(looping < total_blocks)
							{
								if(table[looping].filename == fileName2)
								{
									break_check = 1;
									if(table[looping].prev==-1)
									{
										table[looping].next = -1;
										where_created = looping;
									}
									else
									{
										table[looping].next = -5000;
										table[looping].prev = -5000;
										table[looping].filename = "";
										table[looping].used = 0;
									}
									table[looping].length = 0;

									clear_contents(table[looping].cylinder,table[looping].sector);
								}
								looping++;
							}
							/*filename 2 doesnt exist. create it*/
							if(break_check == 0)
							{
								cout<<"filename "<<fileName2<<" doesnt exist. creating it"<<endl;
								looping = 0 ;
								while(table[looping].used != 0 && looping < total_blocks)
								{
									looping++;
								}
								table[looping].filename = fileName2;
								table[looping].used = 1;								
								table[looping].prev = -1;//make this the first block for the file
								where_created = looping;
								cout<<"File "<<table[looping].filename<<" created in cylinder# "<<table[looping].cylinder<<" and sector# "<<table[looping].sector <<endl;								table[looping].next = -1;

							}//end of creation of filename2
					
					/*Read from filename 1*/
					if(Length > 0)
					{
						looping = 0;
						/*get the block at which this file is created*/
						while(looping<total_blocks)
						{
							if(table[looping].filename == fileName && table[looping].prev==-1)
							{
									int  no_of_times = 0, offset,i,size;
									so_far = 0;

									no_of_times = ceil((double)table[looping].length/128);
									size = no_of_times * 128;
									
									char read_data[size];
									read_data[0]='0';
									while (  no_of_times != 0 )
									{
										/*insert code to read all blocks belonging to this file*/
										
										offset = (HDD_SECTORS*128) * (table[looping].cylinder) + 128 * (table[looping].sector);
										if ( usleep((useconds_t)( (HDD_SECTORS*(table[looping].cylinder)*TRACK_TIME)+ ( (table[looping].sector)*TRACK_TIME) ) ) == 0)
										{
										}	
										if( fseek(hdd, offset, SEEK_SET) != 0 )
										{
											perror("seek error for offset\n");
											exit(1);
										}
									
										for(i=0;i<128;i++)
										{
											read_data[so_far]=fgetc(hdd);
											so_far++;
										}
										
										cout<<"looping= "<<looping<<endl;
										looping = table[looping].next;


										no_of_times--;
									}//end of while	for reading filename 1 information
									cout<<"data= "<<read_data<< " "<<strlen(read_data)<<endl;
									/*End reading of data from filename 1*/
/*write whatever read so far from filename 1 to filename 2*/
//------------------------------------------------------------------------------------------------------------------------------------------
first_time=1;
so_far=0;
looping = where_created ;
while(so_far < Length)
{	
	if(first_time)
	{
		table[looping].prev = -1; /*make it the first block written*/
		table[looping].length = Length ; /*store the length of the data that is being written*/
		first_time = 0;
		prev_written_block = looping;
		
		if(Length <= 128)
		{
			table[looping].next = -1;//make this the last block
			so_far = Length;
			//call the function to write the data into the flat file
			copy_to_file(table[looping].cylinder, table[looping].sector, read_data,so_far);
			break;
		}
		else
		{
			so_far = 128;
			//call the function to write the data into the flat file
			copy_to_file(table[looping].cylinder, table[looping].sector, read_data,so_far);
		}
		
	}
	else
	{
		inner_looping = 0;
		while(table[inner_looping].used != 0 && inner_looping < total_blocks)
		{
			inner_looping++;
		}
		table[prev_written_block].next = inner_looping;
		table[inner_looping].prev = prev_written_block ;
		prev_written_block = inner_looping;

		table[inner_looping].filename = fileName2;
		table[inner_looping].used = 1 ;
		table[inner_looping].length = Length ;
		
		//If we are writing the last block
		if(so_far + 128 >= Length)
		{
			table[inner_looping].next = -1;//make this the last block
			so_far+=128;
			copy_to_file(table[inner_looping].cylinder, table[inner_looping].sector, read_data,so_far);
			break;
		}
		else
		{
			so_far+=128;
			copy_to_file(table[inner_looping].cylinder, table[inner_looping].sector, read_data,so_far);
		}									
	}
}//end of while where we are writing data to disk
//------------------------------------------------------------------------------------------------------------------------------------------
/*End of write*/
									//breaking due to all info already read
									break;
								
							}//end of if
							looping++;
						}//end of while	for the total number of blocks
							
					}//end of if loop where i am checking whether any data is available to copy from filename1


							str[0] = '0';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							cout<<"File "<<fileName<<" copied. Copying successfull"<<endl;

						}//end of IF-ELSE loop where i am checking whether filename 1 exists or not
						cout<<"strlen of "<<str<<" before clearing= "<<strlen(str)<<endl;
						strcpy(str,"");
						printf("after clearing str=%s %d\n",str,strlen(str));
					}//end of case
					break;
				
				case 'm':
				case 'M':
				{
						cmdmain1 = strtok(str," ");

						cmdmain1 = strtok(NULL," ");
						fileName = make_string(cmdmain1);/*get file name*/

						cmdmain1 = strtok(NULL,"\n");
						fileName2 = make_string(cmdmain1);/*get file name*/

						break_check = 0 ;
						looping=0;
						while(looping < total_blocks)
						{
							if(table[looping].filename == fileName)
							{
								break_check  = 1 ;
								break;
							}
							looping++;
						}

						if(break_check == 0)
						{
							str[0] = '1';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							cout<<"File "<<fileName<<" does not exist. Renaming not possible"<<endl;
						}
						else
						{
							looping=0;
							while(looping < total_blocks)
							{
								if(table[looping].filename == fileName2)
								{
									table[looping].next = -5000;
									table[looping].prev = -5000;
									table[looping].filename = "";
									table[looping].used = 0;									
									table[looping].length = 0;

									clear_contents(table[looping].cylinder,table[looping].sector);
								}
								looping++;
							}
							looping=0;
							while(looping < total_blocks)
							{
								if(table[looping].filename == fileName)
								{
									table[looping].filename = fileName2;
								}
								looping++;
							}
							str[0] = '0';
							if ( send (s2, str, 1, 0) <0 )
								perror("send");
							cout<<"File "<<fileName<<" has been renamed to "<<fileName2<<endl;
						}
					}
					break;
				
				case 'q':
				case 'Q':
					 close(s2);					 
		   			 fclose(file);
					 fclose(hdd);
					 int index=0;
					string temp;
					strcpy(str,"");
					file=fopen("table.txt","w+");
					if(file==NULL)
					{
						cout<<"System Error"<<endl;
						exit(1);
					}
					for(index=0;index<total_blocks;index++)
					{
						stringstream stream;
						stream << table[index].cylinder;
						stream << " ";
						stream << table[index].sector;
						stream << " ";
						stream << table[index].next;
						stream << " ";
						stream << table[index].prev;
						stream << " ";
						if(table[index].filename == "")
							stream << "unused";
						else
							stream << table[index].filename;
						stream <<" ";
						stream << table[index].used;
						stream << " ";
						stream << table[index].length;
						//stream << "\n";
						temp=stream.str();
						cout<< temp<<endl;
						fputs(temp.c_str(),file);
							fputs("\n",file);
						temp="";
					}	
		               		 fclose(file);
					 done = 1;
				    break;
					
				}//switch
			}//if done

            } while (!done);

           
        }//end of for loop

        return 0;
    }
