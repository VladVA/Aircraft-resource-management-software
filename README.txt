README

int numar_insula(char buf[MAX_LENGTH])
Functie care citeste numele unei insule si extrage numarul ei.

Graph * create_empty_graph (int n)
Functie pentru a crea si aloca un graf fara legaturi intre noduri.

void printGraph(Graph *g)
Functie pentru a afisa matricea de adiacenta a grafului, folosita pentru verificare.


Etapa de proiectare:

void adauga_zbor(Graph *g,int m1, int m2, int cost)
Functie pentru a adauga o muchie in graful neorientat si a modifica matricea de adiacenta cu costul drumului.

void anulare_zbor(Graph *g,int m1, int m2)
Functie pentru a inlatura o muchie din graf.

void conexiune(Graph *g,int k, int l,FILE *rez)
Functie care verifica daca exista o muchie intre doua noduri.

void legatura(Graph *g,int x,FILE *rez)
Functie care verifica daca exista conexiune directa intre doua noduri (insule).

void max_resurse(Island *insule, int n,FILE *rez)
Functie care afiseaza numarul de resurse diferite din arhipelag si care afiseaza si resursele respective.

void max_cantitate(Island *insule, char buf[MAX_LENGTH],int n,FILE *rez)
Functie care afiseza insulele cu cantitatea maxima dintr-o anumita resursa.


Etapa de analiza:

void drum_zbor(Graph *g,int n,int s,int f,FILE *rez)
Functia este algoritmul lui dijkstra modificat pentru a afisa drumul intre doua insule(noduri) in oridinea parcurgerii de la insula de unde pleaca 
avionul la insula destinatie. 

void timp_zbor(Graph *g,int n,int s,int f,FILE *rez)
Functia este algoritmul lui dijkstra modificat pentru a afisa costul drumului intre doua insule(noduri) in conditia in care avioanele nu
stationeza in insulele intermediare.

void min_zbor(Graph *g,int n,int s,int f,FILE *rez)
Functia este algoritmul lui dijkstra modificat pentru a afisa costul drumului intre doua insule(noduri) in conditia in care avioanele
stationeza in insulele intermediare.


Etapa de gestiune:
void print_insule_3(Island *insule,int n,FILE *rez)
Functie pentru a afisa insulele dupa etapa de gestiune.

void gestiune(Island *insule,int n,Graph *g,FILE *rez) 
Functie care redistribuie avioanele care depasesc toleranta insulei in care se afla.
Daca nu este depasita toleranta nici unei insule se afiseaza id-urile avioanelor din insule.
Daca sunt destule avioane astfel incat este depasita toleranta arhipelagului se afiseaza "Stack overflow!".
In cazul in care trebuie distribuite avioane, daca distributia se face fara conflicte, sunt afisate toate variantele 
in care se pot distribui avioanele unei insule respectand prioritatea avioanelor cu id mai mare.
Daca exista conflicte la distributie, avioanele sunt distribuite in asa fel incat sa fie afisate 
toate variantele posibile de distributie a avioanelor in mai multe insule.
Daca nu se pot distribui avioanele in insulele cu care are legatura directa, se cauta daca exista locuri libere in insulele 
aflate in continuare, daca sunt gasite locuri libere, se muta avioanele din insula de legatura in insula cu locuri libere, 
iar avioanele din insula cu exces sunt puse in insula de legtura. 


In main(int argc, char **argv) sunt citite datele din fisierele aferente si se executa functiile ale caror cerinte se regasec in fisierul de date.