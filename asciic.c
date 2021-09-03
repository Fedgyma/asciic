/*
    asciic: create c header file with initialized string from text file.
    Copyright (C) 2021 Fedgyma https://github.com/Fedgyma

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    
    Feel free to contact myself at fedgyma@gmail.com
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	FILE *fd_in;
	FILE *fd_out;
	char *in_name;
	char *out_name;
	char *usage;

	usage = "USAGE: <input ascii text> <output header>\n";
	
	if (argc != 3 ) {
		printf(usage);
		return 1;
	}

	in_name = argv[1];
	out_name = argv[2];

	if (access(in_name, F_OK) != 0) {
		printf("ERROR: file doesn't exist\n");
		return 1;
	}	
	
	fd_in = fopen(in_name, "r");
	fd_out = fopen(out_name, "w");

	char *begin_header;
	char *end_header;
	char *var_def;
	char *text = NULL;
	char *end_null;
	
	begin_header = "#ifndef ASCII_ART_H\n#define ASCII_ART_H\n\n";
	end_header = "\n\n#endif";
	var_def = "const char ascii_art[] = {";
	end_null = "0x00};";

	char *buffer = 0;
	long length;
	
	if (fd_in)
	{
		fseek(fd_in, 0, SEEK_END);
		length = ftell(fd_in);
		fseek(fd_in, 0, SEEK_SET);
		buffer = malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, fd_in);
		}
		fclose(fd_in);
	}
	
	
	char tmp[length*6];
	int i;
	int count;
	int count2;
	int count3;
	
	count = 80 - strlen(var_def);
	count3 = 0;
	count2 = 0;
	if (strlen(var_def) <= 80) {
		while(count > 6) {
			count -= 6;
			count3++;
		}
	}

	for (i = 0; i < length; i++) {
		
		if (count3 > 0) {
			count3--;
			sprintf(tmp + i*6, "0x%02x, ", buffer[i]);
		}
		else {
			break;
		}
	}

	tmp[strlen(tmp)-1] = 0x0a;
	
	for (; i < length; i++) {
		count2 += 6;
		if (count2 < 72) {
			sprintf(tmp + i*6, "0x%02x, ", buffer[i]);
		
		} else {
			sprintf(tmp + i*6, "0x%02x,\n", buffer[i]);
			count2 = 0;
		}

	}
	size_t text_len = strlen(begin_header) + strlen(var_def)\
		      + strlen(tmp) + strlen(end_header) + strlen(end_null); 
	
	text = (char*) malloc(text_len + 1);
	strcpy(text, begin_header);
	strcat(text, var_def);
	strcat(text, tmp);
	strcat(text, end_null);
	strcat(text, end_header);
	fputs(text, fd_out);
	
	free(text);	
	
	fclose(fd_out);

	return 0;
}
