/*
	Compile: g++ NotParalel.cpp -o NotParalel
	Run    : ./NotParalel
*/
#include <iostream>
#include <math.h>
#include <time.h>
#include <sys/time.h>
using namespace std;
int main()
{
	struct timeval t0,t1;
	int tahun;
	float nominal,kupon, diskonto, hkupon, sum=0, duration;
	
	gettimeofday(&t0, NULL);
	kupon=0.08;
	diskonto=0.07;
	nominal=1000;
	tahun=8;
	hkupon=kupon*nominal;
	
	float pv[tahun],nilai[tahun];
	for (int i=0;i<tahun;i++){
		pv[i] = 1/ pow((diskonto+1),(i+1));
		if(i==tahun-1)
		{
		nilai[i]=(hkupon+nominal)*pv[i];
		}else{
		nilai[i]=hkupon*pv[i];
		}
		sum+=nilai[i];
	}	
	cout<<"Nilai Obligasi: "<<sum<<endl;
	gettimeofday(&t1, NULL);
	duration = ((float)(t1.tv_usec - t0.tv_usec) / 100000) + (float)(t1.tv_sec-t0.tv_sec);
	cout<<"Duration : "<<duration<<" s"<<endl;
	return 0;
}
