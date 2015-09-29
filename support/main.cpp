#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <map>

using namespace std;

typedef struct s_float3
{
	float x;
	float y;
	float z;
	void Set(const float& ix, const float& iy, const float& iz)
	{
		x=ix;
		y=iy;
		z=iz;
	}
}float3;

typedef struct s_vertex_PN
{
	float3 position;
	float3 normal;
}vertex_PN;

typedef struct s_index_PN
{
	int pindex;
	int nindex;
	s_index_PN(int pidx, int nidx): pindex(pidx), nindex(nidx){}
}index_PN;

bool operator<(const s_index_PN& left, const s_index_PN& right)
{
	// Quick and dirty: assume that object has less than 65536 verts.
	return ((left.pindex+left.nindex*65536)<(right.pindex+right.nindex*65536));
}

vector<float3>v;
vector<float3>n;
vector<vertex_PN>vbuf;
vector<unsigned int>ibuf;
typedef map<index_PN, int>Tvmap;
Tvmap vmap;

void AddIndex(int vX, int nX)
{
	Tvmap::iterator found=vmap.find(index_PN(vX, nX));
	if(found==vmap.end())
	{
		vmap[index_PN(vX, nX)]=vbuf.size();
		ibuf.push_back(vbuf.size());
		vbuf.resize(vbuf.size()+1);
		vbuf.back().normal  =n[nX];
		vbuf.back().position=v[vX];
	}
	else
	{
		ibuf.push_back(vbuf.size()-1);
	}
}


int main(int argc, char **argv)
{
	printf("hello world\n");
	char wd[256];
	getcwd(wd, 256);
	printf("%s\n",wd);
	const char* fname="wt_teapot.obj";
//	fname=argv[1];
	FILE* f=fopen(fname,"rb");
	if(f)
	{
	
		
		char string[256];
		while(true)
		{
			char* res=fgets(string,255,f);
			if(!res)
				break;
			if((string[0]=='v')&&(string[1]==' '))
			{
				float x;
				float y;
				float z;
				int nf=sscanf(string+2, "%f %f %f", &x, &y, &z);
				if(nf==3)
				{
					v.resize(v.size()+1);
					v.back().Set(x,y,z);
				}
			}
			if((string[0]=='v')&&(string[1]=='n'))
			{
				float x;
				float y;
				float z;
				int nf=sscanf(string+2, "%f %f %f", &x, &y, &z);
				if(nf==3)
				{
					n.resize(n.size()+1);
					n.back().Set(x,y,z);
				}
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
				if(nf==6)
				{
					AddIndex(v0,n0);
					AddIndex(v1,n1);
					AddIndex(v2,n2);
				}
			}
		}
		printf("Positions %i Normals %i Vertices %i Faces %i\n", v.size(), n.size(), vbuf.size(), ibuf.size());
		fclose(f);
		
		{
			FILE* f=fopen("model.h", "wb");
			fprintf("")
			
			
			fclose(f);
		}
	}
	return 0;
}
