#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include <bitset>
using namespace std;

unsigned int hextodec(string s){
	unsigned int un = 0;
	stringstream ss;
	ss << hex << s;
	ss >> un;
	return un;
}
void requeue(int *cache,int col,int from,int tag){
	int i = 0;
	for(i=from;i<col-1;i++){
		if(cache[i+1]==-1){
			break;
		}
		cache[i] = cache[i+1];
	}
	cache[i] = tag;
}
int victim(int *cache,unsigned tag,int col){
	int anstag = cache[0];
	int i = 0;
	for(i=0;i<col-1;i++){
		cache[i]=cache[i+1];
	}
	cache[col-1]=tag;		

	return anstag;
}

int findtag(int *cache,int policy,int col,int tag){
	int anstag = 0;
		for(int i=0;i<col;i++){
			if(cache[i]==-1){
				cache[i] = tag;
				return -1;
			}else if(cache[i]==tag){
				if(policy!=0)
					requeue(cache,col,i,tag);
				return -1;
			}
		}

		anstag = victim(cache,tag,col);
		
	return anstag;
}
unsigned long bintodec(unsigned long s,int from,int to){
	unsigned long num = 0;
	string str="";
	bitset<32> bs(s);
	str = bs.to_string();
	bitset<32> bin(str,from,to);
	num = bin.to_ulong();
	return num;
}

int main(int argc,char *argv[]){
	int cachesize,blocksize,associativity,policy,indexsize,row,col,tagsize,offset,out,line=0;
	unsigned long address,tag,index;
	int **cache;
	fstream fin,fout;
	string s,stag,sindex,tmp;
	
	fin.open(argv[1],ios::in);
	fout.open(argv[2],ios::out);

//	while(fin.peek()!=EOF){
		if(line==0){
			fin>>cachesize>>blocksize>>associativity>>policy;
//			cout << policy << endl;
			if(associativity==0){	//direct map
				indexsize=log2(cachesize*1024/blocksize);
				offset=log2(blocksize);
				tagsize=32-indexsize-offset;
				row = cachesize*1024/blocksize;
				col = 1;
			}else if(associativity==1){	//4-ways
				indexsize=log2(cachesize*256/blocksize);
				offset=log2(blocksize);
				tagsize=32-indexsize-offset;
				row = cachesize*256/blocksize;
				col = 4;
			}else{	//full
				indexsize= 0;
				offset=(int)log2(blocksize);
				tagsize=32-indexsize-offset;
				row = 1;
				col =(int)cachesize*1024/blocksize;
			}

			cache = new int*[row];
			for(int i=0;i<row;i++)
				cache[i] = new int[col];

			for(int i=0;i<row;i++)
				for(int j=0;j<col;j++)
					cache[i][j] = -1;
		}
		line += 1;
		 	while(fin >> s){
//				cout << s << endl;
				address = hextodec(s);		
				index = bintodec(address,tagsize,indexsize);
				tag = bintodec(address,0,tagsize);	
				out = findtag(cache[index],policy,col,tag);
				fout << out << endl;
			}

		
//		line+=1;			
//	}

	fin.close();
	fout.close();
}
