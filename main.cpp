#include <algorithm>
#include <chrono>
#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#include <array>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
struct Move { int a,b,p=0; };
struct Board {
 array<char,64> s{}; bool white=true; int ep=-1; int halfmove=0; string castle="KQkq";
 void fen(string f){istringstream in(f);string pos,turn,cast,eps;in>>pos>>turn>>cast>>eps;in>>halfmove;s.fill('.');int r=7,c=0;for(char x:pos){if(x=='/'){--r;c=0;}else if(isdigit(x))c+=x-'0';else if(r>=0&&c<8)s[r*8+c++]=x;}white=turn=="w";castle=cast=="-"?"":cast;ep=eps=="-"?-1:(eps[0]-'a'+8*(eps[1]-'1'));}
 static bool own(char x,bool w){return x!='.'&&(isupper(x)!=0)==w;}
 static bool enemy(char x,bool w){return x!='.'&&!own(x,w);}
 bool attacked(int sq,bool bywhite)const{int r=sq/8,c=sq%8;int dir=bywhite?-1:1;for(int dc:{-1,1}){int rr=r+dir,cc=c+dc;if(rr>=0&&rr<8&&cc>=0&&cc<8&&s[rr*8+cc]==(bywhite?'P':'p'))return true;}
 int nr[]={2,2,1,1,-1,-1,-2,-2},nc[]={1,-1,2,-2,2,-2,1,-1};for(int i=0;i<8;i++){int rr=r+nr[i],cc=c+nc[i];if(rr>=0&&rr<8&&cc>=0&&cc<8&&s[rr*8+cc]==(bywhite?'N':'n'))return true;}
 for(int dr=-1;dr<=1;dr++)for(int dc=-1;dc<=1;dc++){if(!dr&&!dc)continue;int rr=r+dr,cc=c+dc,dist=1;while(rr>=0&&rr<8&&cc>=0&&cc<8){char x=s[rr*8+cc];if(x!='.'){if(own(x,bywhite)){char t=toupper(x);if((dist==1&&t=='K')||(t=='Q')||(t=='R'&&(dr==0||dc==0))||(t=='B'&&dr!=0&&dc!=0))return true;}break;}rr+=dr;cc+=dc;dist++;}}
 return false;}
 bool check(bool w)const{char k=w?'K':'k';for(int i=0;i<64;i++)if(s[i]==k)return attacked(i,!w);return true;}
 Board play(Move m)const{Board b=*this;char x=b.s[m.a],y=b.s[m.b];b.s[m.b]=x;b.s[m.a]='.';if(toupper(x)=='P'&&m.b==ep&&y=='.')b.s[m.b+(white?-8:8)]='.';if(m.p)b.s[m.b]=white?char(toupper(m.p)):char(tolower(m.p));if(toupper(x)=='K'&&abs(m.b-m.a)==2){int rookfrom=m.b>m.a?m.a+3:m.a-4,rookto=(m.a+m.b)/2;b.s[rookto]=b.s[rookfrom];b.s[rookfrom]='.';}
 for(char ch: string("KQkq")){int home=ch=='K'?7:ch=='Q'?0:ch=='k'?63:56;if(m.a==home||m.b==home||((ch=='K'||ch=='Q')&&x=='K')||((ch=='k'||ch=='q')&&x=='k'))b.castle.erase(remove(b.castle.begin(),b.castle.end(),ch),b.castle.end());}
 b.halfmove=(toupper(x)=='P'||y!='.'||m.b==ep)?0:halfmove+1;b.ep=toupper(x)=='P'&&abs(m.b-m.a)==16?(m.a+m.b)/2:-1;b.white=!white;return b;}
 vector<Move> moves()const{vector<Move> out;auto add=[&](int a,int b,int promo=0){if(b<0||b>=64||own(s[b],white)||toupper(s[b])=='K')return;Move m{a,b,promo};if(!play(m).check(white))out.push_back(m);};
 for(int a=0;a<64;a++){char x=s[a];if(!own(x,white))continue;int r=a/8,c=a%8;char t=toupper(x);if(t=='P'){int d=white?1:-1,rr=r+d;if(rr>=0&&rr<8){int b=rr*8+c;auto pawnadd=[&](int target){if(rr==0||rr==7){for(char p:string("qrbn"))add(a,target,p);}else add(a,target);};if(s[b]=='.'){pawnadd(b);if(r==(white?1:6)&&s[(r+2*d)*8+c]=='.')add(a,(r+2*d)*8+c);}for(int dc:{-1,1})if(c+dc>=0&&c+dc<8){int z=rr*8+c+dc;if(enemy(s[z],white)||z==ep)pawnadd(z);}}}
 else if(t=='N'){int dr[]={2,2,1,1,-1,-1,-2,-2},dc[]={1,-1,2,-2,2,-2,1,-1};for(int i=0;i<8;i++){int rr=r+dr[i],cc=c+dc[i];if(rr>=0&&rr<8&&cc>=0&&cc<8)add(a,rr*8+cc);}}
 else {for(int dr=-1;dr<=1;dr++)for(int dc=-1;dc<=1;dc++){if(!dr&&!dc)continue;if(t=='B'&&(dr==0||dc==0))continue;if(t=='R'&&dr&&dc)continue;int rr=r+dr,cc=c+dc;while(rr>=0&&rr<8&&cc>=0&&cc<8){int b=rr*8+cc;if(own(s[b],white))break;add(a,b);if(s[b]!='.'||t=='K')break;rr+=dr;cc+=dc;}}if(t=='K'&&!check(white)){int base=white?0:56;if(a==base+4){for(int side=0;side<2;side++){char flag=white?(side?'K':'Q'):(side?'k':'q');int rook=base+(side?7:0),step=side?1:-1;if(castle.find(flag)==string::npos||s[rook]!=(white?'R':'r'))continue;bool clear=true;for(int j=a+step;j!=rook;j+=step)if(s[j]!='.')clear=false;if(clear&&!attacked(a+step,!white)&&!attacked(a+2*step,!white))add(a,a+2*step);}}}}
 }return out;}
};
const string START="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
string notation(Move m){string z;z+=char('a'+m.a%8);z+=char('1'+m.a/8);z+=char('a'+m.b%8);z+=char('1'+m.b/8);if(m.p)z+=m.p;return z;}
int pieceValue(char x){switch(toupper(x)){case 'P':return 100;case 'N':return 320;case 'B':return 330;case 'R':return 500;case 'Q':return 900;default:return 0;}}
int evaluate(const Board& b){
 int score=0;
 for(int i=0;i<64;i++){char x=b.s[i];if(x=='.')continue;int v=pieceValue(x),r=i/8,c=i%8;
 int center=6-abs(3-c)-abs(3-r);
 if(toupper(x)=='N'||toupper(x)=='B')v+=center*5;
 if(toupper(x)=='P')v+=(isupper(x)?r:7-r)*7;
 score+=(isupper(x)?v:-v);
 }
 return b.white?score:-score;
}
int moveScore(const Board& b,Move m){
 int victim=pieceValue(b.s[m.b]);if(m.b==b.ep&&toupper(b.s[m.a])=='P')victim=100;
 return victim*16-pieceValue(b.s[m.a])+(m.p?pieceValue(m.p)*16:0);
}
void orderMoves(const Board& b,vector<Move>& ms){
 stable_sort(ms.begin(),ms.end(),[&](Move a,Move c){return moveScore(b,a)>moveScore(b,c);});
}
int quiescence(const Board& b,int alpha,int beta,int ply){
 if(ply>=12)return evaluate(b);
 bool inCheck=b.check(b.white);
 int stand=evaluate(b);
 if(!inCheck){if(stand>=beta)return beta;alpha=max(alpha,stand);}
 auto ms=b.moves();
 if(ms.empty())return inCheck?-30000+ply:0;
 orderMoves(b,ms);
 for(auto m:ms){
  if(!inCheck&&b.s[m.b]=='.'&&m.b!=b.ep&&!m.p)continue;
  int v=-quiescence(b.play(m),-beta,-alpha,ply+1);
  if(v>=beta)return beta;
  alpha=max(alpha,v);
 }
 return alpha;
}
int search(const Board& b,int depth,int alpha,int beta,int ply=0){
 if(depth<=0)return quiescence(b,alpha,beta,ply);
 auto ms=b.moves();
 if(ms.empty())return b.check(b.white)?-30000+ply:0;
 orderMoves(b,ms);
 for(auto m:ms){
  int v=-search(b.play(m),depth-1,-beta,-alpha,ply+1);
  if(v>=beta)return beta;
  alpha=max(alpha,v);
 }
 return alpha;
}
using Clock=chrono::steady_clock;
struct TTEntry { int depth, score, flag; };
unordered_map<string,TTEntry> tt;
Clock::time_point deadline;
uint64_t nodes=0;
bool timed=false;
string keyOf(const Board& b){
 string k(b.s.begin(),b.s.end());
 k+=b.white?'w':'b';k+=b.castle;k+='|';k+=to_string(b.ep);k+='|';k+=to_string(b.halfmove);
 return k;
}
void tick(){if((++nodes&2047)==0&&timed&&Clock::now()>=deadline)throw runtime_error("time");}
int qsearch(const Board& b,int alpha,int beta,int ply){
 tick();if(b.halfmove>=100)return 0;if(ply>=16)return evaluate(b);
 bool checked=b.check(b.white);int stand=evaluate(b);
 if(!checked){if(stand>=beta)return beta;alpha=max(alpha,stand);}
 auto ms=b.moves();if(ms.empty())return checked?-30000+ply:0;
 orderMoves(b,ms);
 for(auto m:ms){if(!checked&&b.s[m.b]=='.'&&m.b!=b.ep&&!m.p)continue;
 int v=-qsearch(b.play(m),-beta,-alpha,ply+1);
 if(v>=beta)return beta;alpha=max(alpha,v);}
 return alpha;
}
int negamax(const Board& b,int depth,int alpha,int beta,int ply){
 tick();if(b.halfmove>=100)return 0;
 if(depth<=0)return qsearch(b,alpha,beta,ply);
 string key=keyOf(b);int originalAlpha=alpha;
 auto it=tt.find(key);
 if(it!=tt.end()&&it->second.depth>=depth){
  auto e=it->second;
  if(e.flag==0)return e.score;
  if(e.flag==1)alpha=max(alpha,e.score);
  if(e.flag==2)beta=min(beta,e.score);
  if(alpha>=beta)return e.score;
 }
 auto ms=b.moves();if(ms.empty())return b.check(b.white)?-30000+ply:0;
 orderMoves(b,ms);int best=-32000;
 for(auto m:ms){int v=-negamax(b.play(m),depth-1,-beta,-alpha,ply+1);
 best=max(best,v);alpha=max(alpha,v);if(alpha>=beta)break;}
 if(tt.size()>150000)tt.clear();
 tt[key]={depth,best,best<=originalAlpha?2:(best>=beta?1:0)};
 return best;
}
uint64_t perft(const Board& b,int depth){
 if(depth==0)return 1;
 auto ms=b.moves();if(depth==1)return ms.size();
 uint64_t n=0;for(auto m:ms)n+=perft(b.play(m),depth-1);return n;
}
int main(){
 ios::sync_with_stdio(false);cin.tie(nullptr);
 Board b;b.fen(START);string line;
 while(getline(cin,line)){
  istringstream in(line);string cmd;in>>cmd;
  if(cmd=="uci")cout<<"id name Alokhin-Alekhine Engine 0.4\\nid author AA Engine Project\\nuciok\\n";
  else if(cmd=="isready")cout<<"readyok\\n";
  else if(cmd=="ucinewgame"){b.fen(START);tt.clear();}
  else if(cmd=="position"){
   string kind;in>>kind;
   if(kind=="startpos")b.fen(START);
   else if(kind=="fen"){string fen,part;for(int i=0;i<6&&in>>part;i++)fen+=(i?" ":"")+part;b.fen(fen);}
   string word;while(in>>word){if(word=="moves")continue;
    for(auto m:b.moves())if(notation(m)==word){b=b.play(m);break;}
   }
  }else if(cmd=="go"){
   int depth=5,movetime=-1,wtime=-1,btime=-1,winc=0,binc=0,movestogo=30;string token;
   bool depthGiven=false;
   while(in>>token){
    if(token=="depth"){in>>depth;depthGiven=true;}
    else if(token=="movetime")in>>movetime;
    else if(token=="wtime")in>>wtime;
    else if(token=="btime")in>>btime;
    else if(token=="winc")in>>winc;
    else if(token=="binc")in>>binc;
    else if(token=="movestogo")in>>movestogo;
   }
   int remain=b.white?wtime:btime,inc=b.white?winc:binc;
   if(movetime<0&&remain>=0)movetime=max(10,min(remain/2,remain/max(1,movestogo)+inc/2));
   timed=movetime>=0;nodes=0;
   if(timed)deadline=Clock::now()+chrono::milliseconds(max(1,movetime-20));
   auto ms=b.moves();orderMoves(b,ms);
   if(ms.empty()){cout<<"bestmove 0000\\n";continue;}
   Move best=ms[0];
   int maxdepth=depthGiven?clamp(depth,1,32):32;
   if(!timed&&!depthGiven)maxdepth=4;
   for(int d=1;d<=maxdepth;d++){
    try{
     int score=-32000;Move candidate=best;
     for(auto m:ms){
      int v=-negamax(b.play(m),d-1,-32000,32000,1);
      if(v>score){score=v;candidate=m;}
     }
     best=candidate;
     cout<<"info depth "<<d<<" score cp "<<score<<" nodes "<<nodes<<" pv "<<notation(best)<<"\\n";
     if(timed&&Clock::now()>=deadline)break;
    }catch(const runtime_error&){break;}
   }
   cout<<"bestmove "<<notation(best)<<"\\n";
  }else if(cmd=="perft"){
   int d=1;in>>d;if(d>=0&&d<=5)cout<<"perft "<<d<<" nodes "<<perft(b,d)<<"\\n";
  }else if(cmd=="quit")break;
  cout.flush();
 }
}
