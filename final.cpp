#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

int gr_c[4]={0,1,3,2};
struct group{
	vector<pair<int,int>>blocks;
	string term;
};

int kmap[4][4];
int vi[4][4];

//functions to generate the groups required using indices:

pair<int,int>fold(int r, int c){
	return {(r+4)%4,(c+4)%4};
}

vector<vector<pair<int,int>>>makegroups(int r,int c){
	vector<pair<int,int>>box;
	box.push_back(fold(r,c));
	box.push_back(fold(r+1,c));
	box.push_back(fold(r,c+1));
	box.push_back(fold(r+1,c+1));
	
	vector<pair<int,int>>col;
	for(int i=0;i<4;i++){
		col.push_back(fold(i,c));
	}
	
	vector<pair<int,int>>row;
	for(int j=0;j<4;j++){
		row.push_back(fold(r,j));
	}
	
	vector<vector<pair<int,int>>>ans;
	
	ans.push_back(box);
	ans.push_back(row);
	ans.push_back(col);
	
	return ans;
	
}

//function to check wether group can be used or elements are already visited
bool grp_usable (vector<pair<int,int>>&grp){
   bool ans=false;
	for(auto it:grp){
		auto [r,c]=it;
		if(kmap[r][c]==0) return false;
		else if(kmap[r][c] == 1 && vi[r][c] == 0)ans=true;
	}
	return ans;
}

//Function to mark the visited groups
void visitgroup(vector<pair<int,int>>&grp){
	for(auto it:grp){
	auto [r,c]=it;
	if(kmap[r][c]==1) vi[r][c]=1;
	}
}

//functions to obtain terms from finally selected ans groups:

string makebin(int num){
string ans="";
	for(int i=3;i>=0;i--){
	char a=(num &(1<<i)) ? '1':'0';
	ans+=a;
	}
return ans;
}

string term(vector<pair<int,int>>&blocks, int v){

vector<string>binvalues;

for(int i=0;i<blocks.size();i++){
	auto[r,c]=blocks[i];
	int no = (gr_c[r]<<2) | gr_c[c];
	binvalues.push_back(makebin(no));
}

string ch="";
for(int i=0;i<v;i++){
	bool found=true;
	char b=binvalues[0][i];
	for(int j=1;j<binvalues.size();j++){
		if(b!=binvalues[j][i]) {
		found =false;
		break;
		}
	}
	if(found){
	if(b=='0')ch+=(char)('A'+i),ch+="'";
	else ch+=(char)('A'+i);
	}
}

return ch;

}


// The main function of kmap solver
int main(){

int v,m_no,d_no;

map<int,pair<int,int>>kmap_bin;


memset(kmap,0,sizeof(kmap));
memset(vi,0,sizeof(vi));

cout<<"Input no of variables in circuit (2-4): ";
cin>>v;
if(v<2 || v>4 ){
	cout<<"Provided no is not in feasable range program terminated\n";
	return 0;
}
cout<<"Input no of min terms: ";
cin>>m_no;
if(m_no<0 || pow(2,v)<m_no ){
	cout<<"Provided no is not in feasable range program terminated\n";
	return 0;
}
cout<<"Provide the min terms: ";

set<int>min_terms;

for(int r=0;r<4;r++){
	for(int c=0;c<4;c++){
		int val = (gr_c[r]<<2) | gr_c[c];
		kmap_bin[val]={r,c};
	}
}


//Generation of kmap using the min terms provided:
for(int i=0;i<m_no;i++){
	int a;
	cin>>a;
	if(a<0 || a>pow(2,v)-1) {
	     cout<<"Invalid min term"<<a <<" skipping it\n";
	     continue;
	}
	min_terms.insert(a);
	auto[r,c]=kmap_bin[a];
	kmap[r][c]=1; 
}

cout<<"Input no of dont care terms: ";
cin>>d_no;

cout<<"Provide the dont care terms: ";

//Addition of dont care terms to the kmap generated:
for(int i=0;i<d_no;i++){
	int d;
	cin>>d;
	if(d<0 || d>pow(2,v)-1) {
		cout<<"Invalid term provide"<<d <<" skipping it\n";
	        continue;
	}
	if(min_terms.count(d)){
		cout<<"Provided term"<<d<<" exists as min term, skipping it\n";
		continue;
	}
	auto[r,c]=kmap_bin[d];
	kmap[r][c]=2;
}

vector<group> ans_groups;

for(int r=0;r<4;r++){
	for(int c=0;c<4;c++){
		vector<vector<pair<int,int>>>grps;
		if(!vi[r][c] && kmap[r][c]==1) grps=makegroups(r,c);
		sort(grps.begin(),grps.end(),[](auto &a,auto &b){
			return b.size()<a.size();
		});
		if(grps.size()>0){
		for(auto it:grps){
			if(grp_usable(it)){
				visitgroup(it);
				ans_groups.push_back({it,term(it,v)});
				break;
			}
		}
		}
	}

}

cout<<"Final boolean expression is: ";

if(!ans_groups.size()){
	cout<<"Always ans remains 0 / GND\n";
}else{
int c=1;
for(auto [f,s]:ans_groups){
 if(c==0)cout<<"+";
 c=0;
cout<<s;
}
cout<<endl;
}


}
