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
		int index=vbuf.size();
		vmap[index_PN(vX, nX)]=index;
		ibuf.push_back(index);
		vbuf.resize(vbuf.size()+1);
		vbuf.back().normal  =n[nX];
		vbuf.back().position=v[vX];
	}
	else
	{
		int index=found->second;
		ibuf.push_back(index);
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
					AddIndex(v0-1,n0-1);
					AddIndex(v1-1,n1-1);
					AddIndex(v2-1,n2-1);
				}
			}
		}
		printf("Positions %i Normals %i Vertices %i Faces %i\n", (int)v.size(), (int)n.size(), (int)vbuf.size(), (int)ibuf.size());
		fclose(f);
		
		{
			FILE* f=fopen("model.h", "wb");
			fprintf(f, "float vb[]={\n");
			for(unsigned int i=0;i<vbuf.size();i++)
			{
				fprintf(f, "\t% 8.6f, % 8.6f, % 8.6f, % 8.6f, % 8.6f, % 8.6f, % 8.6f, % 8.6f,\n",
					vbuf[i].position.x,vbuf[i].position.y,vbuf[i].position.z,
					vbuf[i].normal.x  ,vbuf[i].normal.y  ,vbuf[i].normal.z,
					0.0,0.0
				);
			}
			fprintf(f, "};\n");
			fprintf(f, "int ib[]={\n\t");
			for(unsigned int i=0;i<ibuf.size();i++)
			{
				if(i&&(i%3==0))
					fprintf(f, "\n\t");
				fprintf(f, "% 4i,", ibuf[i]);
			}
			fprintf(f, "};\n");
			fclose(f);
		}
	}
	return 0;
}
