#include<iostream>
using namespace std;
void main(){
	int a,b
	cin>>a;
	cin>>b;
	int c*=&a;
	_asm{
		mov eax,c
		mov eax,[eax]
		mov ebx,b
		lea eax,[eax+ebx]
		mov a,eax
	} 
	cout<<a;
}
