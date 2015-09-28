#include <stdio.h>
#include <unistd.h>


typedef struct
{
	float x;
	float y;
	float z;
}vertex, normal;

int main(int argc, char **argv)
{
	printf("hello world\n");
	char wd[256];
	getcwd(wd, 256);
	printf("%s\n",wd);
	char* fname="wt_teapot.obj";
//	fname=argv[1];
	FILE* f=fopen(fname,"rb");
	if(f)
	{
		vertex* v;
		normal* n;
		
		char string[256];
		while(true)
		{
			char* res=fgets(string,255,f);
			if(!res)
				break;
//			puts(string);
			if((string[0]=='v')&&(string[1]==' '))
			{
				float x;
				float y;
				float z;
				int nf=sscanf(string+2, "%f %f %f", &x, &y, &z);
				printf("Vertex %i %f %f %f\n", nf, x, y, z);
			}
			if((string[0]=='v')&&(string[1]=='n'))
			{
				float x;
				float y;
				float z;
				int nf=sscanf(string+2, "%f %f %f", &x, &y, &z);
				printf("Normal %i %f %f %f\n", nf, x, y, z);
			}
			if((string[0]=='f')&&(string[1]==' '))
			{
				int v0;
				int v1;
				int v2;
				int n0;
				int n1;
				int n2;
				int nf=sscanf(string+2, "%i//%i %i//%i %i//%i", &v0, &n0, &v1, &n1, &v2, &n2);
				printf("t %i//%i %i//%i %i//%i\n", v0, n0, v1, n1, v2, n2);
			}
		}
		fclose(f);
	}
	return 0;
}
