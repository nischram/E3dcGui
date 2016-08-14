#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <limits>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

char l01[20], l02[20], l03[20], l04[20], l05[20], l06[20], l07[20], l08[20], l09[20];
char l10[20], l11[20], l12[20], l13[20], l14[20], l15[20], l16[20], l17[20], l18[20], l19[20];
char l20[20], l21[20], l22[20], l23[20], l24[20], l25[20], l26[20], l27[20], l28[20], l29[20];
char l30[20], l31[20], l32[20], l33[20], l34[20], l35[20], l36[20], l37[20], l38[20], l39[20];
char l40[20], l41[20], l42[20], l43[20], l44[20], l45[20], l46[20], l47[20], l48[20], l49[20];
char l50[20], l51[20], l52[20], l53[20], l54[20], l55[20], l56[20], l57[20], l58[20], l59[20];
char l60[20], l61[20], l62[20], l63[20], l64[20], l65[20], l66[20], l67[20], l68[20], l69[20];
char l70[20], l71[20], l72[20], l73[20], l74[20], l75[20], l76[20], l77[20], l78[20], l79[20];
char l80[20], l81[20], l82[20], l83[20], l84[20], l85[20], l86[20], l87[20], l88[20], l89[20];
char l90[20], l91[20], l92[20], l93[20], l94[20], l95[20], l96[20], l97[20];

void ReadWriteData(char filePath[100], int OUT15){
  fstream datei(filePath);
  if (datei.is_open()) {
    datei.getline(l01	,20, '\n');
    datei.getline(l02	,20, '\n');
    datei.getline(l03	,20, '\n');
    datei.getline(l04	,20, '\n');
    datei.getline(l05	,20, '\n');
    datei.getline(l06	,20, '\n');
    datei.getline(l07	,20, '\n');
    datei.getline(l08	,20, '\n');
    datei.getline(l09	,20, '\n');
    datei.getline(l10	,20, '\n');
    datei.getline(l11	,20, '\n');
    datei.getline(l12	,20, '\n');
    datei.getline(l13	,20, '\n');
    datei.getline(l14	,20, '\n');
    datei.getline(l15	,20, '\n');
    datei.getline(l16	,20, '\n');
    datei.getline(l17	,20, '\n');
    datei.getline(l18	,20, '\n');
    datei.getline(l19	,20, '\n');
    datei.getline(l20	,20, '\n');
    datei.getline(l21	,20, '\n');
    datei.getline(l22	,20, '\n');
    datei.getline(l23	,20, '\n');
    datei.getline(l24	,20, '\n');
    datei.getline(l25	,20, '\n');
    datei.getline(l26	,20, '\n');
    datei.getline(l27	,20, '\n');
    datei.getline(l28	,20, '\n');
    datei.getline(l29	,20, '\n');
    datei.getline(l30	,20, '\n');
    datei.getline(l31	,20, '\n');
    datei.getline(l32	,20, '\n');
    datei.getline(l33	,20, '\n');
    datei.getline(l34	,20, '\n');
    datei.getline(l35	,20, '\n');
    datei.getline(l36	,20, '\n');
    datei.getline(l37	,20, '\n');
    datei.getline(l38	,20, '\n');
    datei.getline(l39	,20, '\n');
    datei.getline(l40	,20, '\n');
    datei.getline(l41	,20, '\n');
    datei.getline(l42	,20, '\n');
    datei.getline(l43	,20, '\n');
    datei.getline(l44	,20, '\n');
    datei.getline(l45	,20, '\n');
    datei.getline(l46	,20, '\n');
    datei.getline(l47	,20, '\n');
    datei.getline(l48	,20, '\n');
    datei.getline(l49	,20, '\n');
    datei.getline(l50	,20, '\n');
    datei.getline(l51	,20, '\n');
    datei.getline(l52	,20, '\n');
    datei.getline(l53	,20, '\n');
    datei.getline(l54	,20, '\n');
    datei.getline(l55	,20, '\n');
    datei.getline(l56	,20, '\n');
    datei.getline(l57	,20, '\n');
    datei.getline(l58	,20, '\n');
    datei.getline(l59	,20, '\n');
    datei.getline(l60	,20, '\n');
    datei.getline(l61	,20, '\n');
    datei.getline(l62	,20, '\n');
    datei.getline(l63	,20, '\n');
    datei.getline(l64	,20, '\n');
    datei.getline(l65	,20, '\n');
    datei.getline(l66	,20, '\n');
    datei.getline(l67	,20, '\n');
    datei.getline(l68	,20, '\n');
    datei.getline(l69	,20, '\n');
    datei.getline(l70	,20, '\n');
    datei.getline(l71	,20, '\n');
    datei.getline(l72	,20, '\n');
    datei.getline(l73	,20, '\n');
    datei.getline(l74	,20, '\n');
    datei.getline(l75	,20, '\n');
    datei.getline(l76	,20, '\n');
    datei.getline(l77	,20, '\n');
    datei.getline(l78	,20, '\n');
    datei.getline(l79	,20, '\n');
    datei.getline(l80	,20, '\n');
    datei.getline(l81	,20, '\n');
    datei.getline(l82	,20, '\n');
    datei.getline(l83	,20, '\n');
    datei.getline(l84	,20, '\n');
    datei.getline(l85	,20, '\n');
    datei.getline(l86	,20, '\n');
    datei.getline(l87	,20, '\n');
    datei.getline(l88	,20, '\n');
    datei.getline(l89	,20, '\n');
    datei.getline(l90	,20, '\n');
    datei.getline(l91	,20, '\n');
    datei.getline(l92	,20, '\n');
    datei.getline(l93	,20, '\n');
    datei.getline(l94	,20, '\n');
    datei.getline(l95	,20, '\n');
    datei.getline(l96	,20, '\n');
    datei.getline(l97	,20, '\n');
    datei.close();
  }
  ofstream fout(filePath);
  if (fout.is_open()) {
    fout <<l02<<"\n"<<l03<<"\n"<<l04<<"\n"<<l05<<"\n"<<l06<<"\n"<<l07<<"\n"<<l08<<"\n"<<l09<<"\n"
    <<l10<<"\n"<<l11<<"\n"<<l12<<"\n"<<l13<<"\n"<<l14<<"\n"<<l15<<"\n"<<l16<<"\n"<<l17<<"\n"<<l18<<"\n"<<l19<<"\n"
    <<l20<<"\n"<<l21<<"\n"<<l22<<"\n"<<l23<<"\n"<<l24<<"\n"<<l25<<"\n"<<l26<<"\n"<<l27<<"\n"<<l28<<"\n"<<l29<<"\n"
    <<l30<<"\n"<<l31<<"\n"<<l32<<"\n"<<l33<<"\n"<<l34<<"\n"<<l35<<"\n"<<l36<<"\n"<<l37<<"\n"<<l38<<"\n"<<l39<<"\n"
    <<l40<<"\n"<<l41<<"\n"<<l42<<"\n"<<l43<<"\n"<<l44<<"\n"<<l45<<"\n"<<l46<<"\n"<<l47<<"\n"<<l48<<"\n"<<l49<<"\n"
    <<l50<<"\n"<<l51<<"\n"<<l52<<"\n"<<l53<<"\n"<<l54<<"\n"<<l55<<"\n"<<l56<<"\n"<<l57<<"\n"<<l58<<"\n"<<l59<<"\n"
    <<l60<<"\n"<<l61<<"\n"<<l62<<"\n"<<l63<<"\n"<<l64<<"\n"<<l65<<"\n"<<l66<<"\n"<<l67<<"\n"<<l68<<"\n"<<l69<<"\n"
    <<l70<<"\n"<<l71<<"\n"<<l72<<"\n"<<l73<<"\n"<<l74<<"\n"<<l75<<"\n"<<l76<<"\n"<<l77<<"\n"<<l78<<"\n"<<l79<<"\n"
    <<l80<<"\n"<<l81<<"\n"<<l82<<"\n"<<l83<<"\n"<<l84<<"\n"<<l85<<"\n"<<l86<<"\n"<<l87<<"\n"<<l88<<"\n"<<l89<<"\n"
    <<l90<<"\n"<<l91<<"\n"<<l92<<"\n"<<l93<<"\n"<<l94<<"\n"<<l95<<"\n"<<l96<<"\n"<<l97<<"\n"<<OUT15;
    fout.close();
    }
  else cerr << "Konnte Datei nicht erstellen!";
  cout << l97 << ";" << OUT15 << "\n";
}
