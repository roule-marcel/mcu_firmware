#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char name[1024];

void head_write();
void ram_write(char * filename);
void mid_write();
void init_write(char * filename);
void foot_write();

int main(int argc, char *argv[])
{
	char *filename;
	int i;

	if (argc<2) {
		printf("usage: %s file.bin\n", argv[0]);
		return -1;
	}

	filename = argv[1];


	head_write();
	for (i = 1 ; i < argc ; i++)
		ram_write(argv[i]);
	mid_write();
	for (i = 1 ; i < argc ; i++)
		init_write(argv[i]);
	foot_write();

	return 0;
}

void head_write()
{
	printf("library ieee;\n");
	printf("use ieee.std_logic_1164.all;\n");
	printf("\n"),
	printf("library work;\n");
	printf("use work.ram16_pkg.all;\n");
	printf("\n");
	printf("package init_ram16_pkg is\n");
}

void ram_write(char * filename)
{
	int count;
	FILE * in;
	char * slash;
	unsigned int data;
	unsigned int address = 0;
	int ndepth;
	int nwidth = 16;

	in = fopen(filename,"rb");

	if (!in) {
		printf("file: %s not found\n",filename);
		exit(EXIT_FAILURE);
	}

	slash = strrchr(filename,'/');
	if (slash != NULL)
	{
		strncpy(name, slash+1, strlen(slash+1)-4);
		name[strlen(slash+1)-4] = 0;
	}
	else
	{
		strncpy(name, filename, strlen(filename)-4);
		name[strlen(filename)-4] = 0;
	}

	fseek(in,0,SEEK_END);
	ndepth = ftell(in)/2;
	fseek(in,0,SEEK_SET);
	
	printf("\tconstant %s_l : ram8_t := (\n", name);

	address = 0;
	data=0;
	while (count = fread(&data,2,1,in)) {
		if (address < ndepth-1)
			printf("\t\tx\"%02x\",\t--%d\n", data & 0xFF, address);
		else
			printf("\t\tx\"%02x\");\t--%d\n", data & 0xFF, address);
		address++;
	}

	fseek(in,0,SEEK_SET);

	printf("\tconstant %s_h : ram8_t := (\n", name);

	address = 0;
	data=0;
	while (count = fread(&data,2,1,in)) {
		if (address < ndepth-1)
			printf("\t\tx\"%02x\",\t--%d\n", (data >> 8) & 0xFF, address);
		else
			printf("\t\tx\"%02x\");\t--%d\n", (data >> 8) & 0xFF, address);
		address++;
	}

	fclose(in);
}

void mid_write()
{
	printf("\n");
	printf("\tfunction init_ram8 (\n");
	printf("\t\tdepth : positive;\n");
	printf("\t\tposition : position_t;\n");
	printf("\t\tinit_file : string\n");
	printf("\t) return ram8_t;\n");
	printf("end package init_ram16_pkg;\n");
	printf("\n");
	printf("package body init_ram16_pkg is\n");
	printf("\tfunction init_ram8 (\n");
	printf("\t\tdepth : positive;\n");
	printf("\t\tposition : position_t;\n");
	printf("\t\tinit_file : string\n");
	printf("\t) return ram8_t is\n");
	printf("\t\tvariable ram : ram8_t(0 to depth-1) := (others => (others => '0'));\n");
	printf("\tbegin\n");
}

void init_write(char * filename)
{
	char * slash;
	slash = strrchr(filename,'/');
	if (slash != NULL)
	{
		strncpy(name, slash+1, strlen(slash+1)-4);
		name[strlen(slash+1)-4] = 0;
	}
	else
	{
		strncpy(name, filename, strlen(filename)-4);
		name[strlen(filename)-4] = 0;
	}

	printf("\t\tif (init_file = \"%s\") then\n",name);
	printf("\t\t\tif (position = 'h') then\n");
	printf("\t\t\t\tif (depth > %s_h'length) then\n", name);
	printf("\t\t\t\t\tram(0 to %s_h'length-1) := %s_h;\n", name, name);
	printf("\t\t\t\t\treturn ram;\n");
	printf("\t\t\t\telsif (depth = %s_h'length) then\n", name);
	printf("\t\t\t\t\treturn %s_h;\n", name);
	printf("\t\t\t\telse\n");
	printf("\t\t\t\t\treport \"init_ram8 : memory to small\" severity error;\n");
	printf("\t\t\t\t\treturn ram;\n");
	printf("\t\t\t\tend if;\n");
	printf("\t\t\telse\n");
	printf("\t\t\t\tif (depth > %s_l'length) then\n", name);
	printf("\t\t\t\t\tram(0 to %s_l'length-1) := %s_l;\n", name, name);
	printf("\t\t\t\t\treturn ram;\n");
	printf("\t\t\t\telsif (depth = %s_l'length) then\n", name);
	printf("\t\t\t\t\treturn %s_l;\n", name);
	printf("\t\t\t\telse\n");
	printf("\t\t\t\t\treport \"init_ram8 : memory to small\" severity error;\n");
	printf("\t\t\t\t\treturn ram;\n");
	printf("\t\t\t\tend if;\n");
	printf("\t\t\tend if;\n");
	printf("\t\tend if;\n");
}

void foot_write()
{
	printf("\n");
	printf("\t\treturn ram;\n");
	printf("\tend function init_ram8;\n");
	printf("end package body init_ram16_pkg;\n");
}
