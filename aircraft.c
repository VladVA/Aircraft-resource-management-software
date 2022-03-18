#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include<ctype.h>
#define MAX_LENGTH 20
#define INT_MAX 99999
#define MIN 99999

typedef struct Resursa
{
    char *nume;
    int cantitate;
} Resursa;


typedef struct Island
{
    char *nume;
    int nrResurse;
    Resursa *inventarResurse;
    int nrAvioane;
    int *avioane;
    int tolAvioane;
} Island;

typedef struct // stuctura pentru grafuri
{
    int V,E;
    int **a;
} Graph;

int numar_insula(char buf[MAX_LENGTH]) //scoate Island din numele unei insule pentru a ii afla numarul
{
    int m;
    char c[MAX_LENGTH];
    strcpy(c,buf +6);
    m=atoi(c)-1;
    return m;
}

Graph * create_empty_graph (int n) //aloca memorie pentru un graf cu o matrice de adiacenta cu nici o legatura
{
    int i, j;
    Graph *g = malloc (sizeof(Graph));
    g->V=n;
    if (g==NULL)
    {
        printf ( "no memory for this graph");
        return NULL;
    }
    g->a =malloc(g->V*sizeof(int*));
    for (i=0; i<g->V; i++)
        g->a[i] =calloc(g->V, sizeof(int));
    if (g->a==NULL)
    {
        printf ( " no memory for this matrix");
        return NULL;
    }
    g->E = 0;

    for (i=0; i<g->V; i++)
        for (j=0; j<i; j++)
        {
            g->a[i][j]=0;
            g->a[j][i]=g->a[i][j];
            g->E+=2;
        }
    return g;
}

void printGraph(Graph *g) //afisez graful
{
    int i, j;
    for (i=0; i<g->V; i++)
    {
        for (j=0; j<g->V; j++)
            printf( " %d ",g->a[i][j]);
        printf("\n");
    }

}

void adauga_zbor(Graph *g,int m1, int m2, int cost) //adaug o ramura intre doua noduri pentru a conecta doua insule
{
    int i, j;
    for (i=0; i<g->V; i++)
    {
        for (j=0; j<g->V; j++)
        {
            if(i == m1 && j == m2) //atunci cand gaseste nodurile
            {
                g->a[i][j]=cost; //adauga in matricea de adiacenta costul (durata drumului)
                g->a[j][i]=g->a[i][j];
            }
        }
    }

}

void anulare_zbor(Graph *g,int m1, int m2) //elimin o ramura dintre doua noduri pentru a disconecta doua insule
{
    int i, j;
    for (i=0; i<g->V; i++)
    {
        for (j=0; j<g->V; j++)
        {
            if(i == m1 && j == m2) //atunci cand gaseste nodurile
            {
                g->a[i][j]=0; //inlocuieste valoarea drumului cu 0
                g->a[j][i]=g->a[i][j];
            }
        }
    }

}

void conexiune(Graph *g,int k, int l,FILE *rez) //verifica daca exista conexiune intre doua noduri
{

    if(g->a[k][l] !=0 || g->a[l][k] !=0)
        fprintf(rez,"OK\n");
    else
        fprintf(rez,"NO\n");


}

void legatura(Graph *g,int x,FILE *rez) //afiseaza conexiunea directa, daca exista, dintre doua insule
{
    int j,k=0;
    for (j=0; j<g->V; j++)
        if(g->a[x][j] !=0)
        {
            fprintf(rez,"Island%d ",j+1);
            k++;
        }
    fprintf(rez,"\n"); //daca nu exista legatura, nu afiseaza nimic

}

void max_resurse(Island *insule, int n,FILE *rez) //afiseaza numarul de resurse din arhipelag si toate resursele diferite ale arhipelagului
{
    int i,j,h,ok,k=0;
    char max[20][MAX_LENGTH],aux[MAX_LENGTH];
    for(i=0; i<n; i++)
    {
        for(j=0; j<insule[i].nrResurse; j++)
        {
            if(i==0)
            {
                strcpy(max[k],insule[i].inventarResurse[j].nume); //in matricea de caractere introduce resursele din prima insula
                k++; //si numara resursele
            }

            else
            {
                ok=1;
                for(h=0; h<k; h++)   //pentru urmatoarele insule dupa prima
                {
                    if(strcmp(max[h],insule[i].inventarResurse[j].nume)==0) //verifica daca mai exista resursa in matricea de caractere
                    {
                        ok=0;
                        break;
                    }

                }
                if(ok==1)
                {
                    strcpy(max[k],insule[i].inventarResurse[j].nume); //daca resursa nu mai exista, este adaugata in metricea de caractere
                    k++; //si creste numarul total de insule diferite
                }
            }

        }
    }


    for(i=0; i<k; i++)  //ordoneaza resuresele in ordine lexicografica
        for(j=i+1; j<k; j++)
        {
            if(strcmp(max[i],max[j]) > 0)
            {
                strcpy(aux,max[i]);
                strcpy(max[i],max[j]);
                strcpy(max[j],aux);
            }
        }
    fprintf(rez,"%d ",k);
    for(i=0; i<k; i++)
        fprintf(rez,"%s ",max[i]);
    fprintf(rez,"\n");
}


void max_cantitate(Island *insule, char buf[MAX_LENGTH],int n,FILE *rez) //afiseaza insulele cu numarul maxim dintr-o resursa
{
    int i,j,k=0,max=0,m=0;
    for(i=0; i<n; i++)
    {
        for(j=0; j<insule[i].nrResurse; j++)
        {
            if(strcmp(insule[i].inventarResurse[j].nume,buf)==0) //daca insula are resursa cautata
            {
                m=insule[i].inventarResurse[j].cantitate;
                if(m>max) //compara cu ultimul maxim
                    max=m; //si inlocuieste daca este cazul
            }
        }
    }
    for(i=0; i<n; i++)
    {
        for(j=0; j<insule[i].nrResurse; j++)
        {
            if(insule[i].inventarResurse[j].cantitate==max && strcmp(insule[i].inventarResurse[j].nume,buf)==0) //verifica daca insulele care au resursa au cantitatea maxima
                fprintf(rez,"%s ",insule[i].nume);
        }
    }
    fprintf(rez,"\n");
}


void drum_zbor(Graph *g,int n,int s,int f,FILE *rez) //afiseaza insulele prin care trece un avion cand trebuie sa ajunga intr-o insula (drumul minim intre insule)
{
    int dist[g->V],pred[g->V],cost[g->V][g->V];
    int sps[g->V],count,mindist,nextnode,i,j,k,v[g->V],l,ok;

    for(i=0; i<n; i++) //creez o matrice cu costuri pentru a nu modifica matricea de adiacenta a grafului
        for(j=0; j<n; j++)
            if(g->a[i][j]==0)
                cost[i][j]=MIN;
            else
                cost[i][j]=g->a[i][j];

    for(i=0; i<n; i++) //initializez vectorii de distanta, predecesori si noduri vizitate
    {
        dist[i]=cost[s][i];
        pred[i]=s;
        sps[i]=0;
    }

    dist[s]=0;
    sps[s]=0;
    count=1; //numarul de noduri prin care a trecut

    while(count<n-1)
    {
        mindist=MIN;

        for(i=0; i<n; i++)
            if(dist[i]<mindist && !sps[i]) //cauta nodul la distanta minima
            {
                mindist=dist[i];
                nextnode=i;
            }
            //verifica daca exista un drum mai bun
        sps[nextnode]=1;
        for(i=0; i<n; i++)
            if(!sps[i])
                if(mindist+(cost[nextnode][i]) < dist[i])
                {
                    dist[i]=mindist+(cost[nextnode][i]);
                    pred[i]=nextnode;
                }
        count++;
    }
    ok=0;
    if (dist[f] == MIN)
    {
        fprintf(rez,"NO\n");
        ok=1;
    }
    else
        for(i=0; i<n; i++)
            if(i!=s && i==f) //afisez drumul pentru inulele date
            {
                k=0;
                v[k]=i+1;
                j=i;
                k++;
                do
                {
                    j=pred[j];
                    v[k]=j+1;
                    k++;
                }
                while(j!=s);

                for(l=k-2; l!=-1; l--)
                    fprintf(rez,"Island%d ",v[l]);

            }
    if(!ok)
        fprintf(rez,"\n");
}

void timp_zbor(Graph *g,int n,int s,int f,FILE *rez) //afiseza timpul minim de zbor in cazul in care avioanele nu stationeza in nici o insula
{
    int dist[g->V],pred[g->V],cost[g->V][g->V];
    int sps[g->V],count,mindist,nextnode,i,j,k,v[g->V],l;

    for(i=0; i<n; i++) //creez o matrice cu costuri pentru a nu modifica matricea de adiacenta a grafului
        for(j=0; j<n; j++)
            if(g->a[i][j]==0)
                cost[i][j]=MIN;
            else
                cost[i][j]=g->a[i][j];

    for(i=0; i<n; i++) //initializez vectorii de distanta, predecesori si noduri vizitate
    {
        dist[i]=cost[s][i];
        pred[i]=s;
        sps[i]=0;
    }

    dist[s]=0;
    sps[s]=0;
    count=1; //numarul de noduri prin care a trecut

    while(count<n-1)
    {
        mindist=MIN;

        for(i=0; i<n; i++)
            if(dist[i]<mindist && !sps[i]) //cauta nodul la distanta minima
            {
                mindist=dist[i];
                nextnode=i;
            }
            //verifica daca exista un drum mai bun
        sps[nextnode]=1;
        for(i=0; i<n; i++)
            if(!sps[i])
                if(mindist+(cost[nextnode][i]) < dist[i])
                {
                    dist[i]=mindist+(cost[nextnode][i]);
                    pred[i]=nextnode;
                }
        count++;
    }
    if (dist[f] == MIN)
        fprintf(rez,"INF\n");
    else
        for(i=0; i<n; i++) //afisez costul minim daca nu stationeza avioanele
            if(i!=s && i==f)
                fprintf(rez,"%d \n",dist[i]);

}

void min_zbor(Graph *g,int n,int s,int f,FILE *rez) //afiseza timpul minim de zbor in cazul in care avioanele stationeza in insulele intermediare
{
    int dist[g->V],pred[g->V],cost[g->V][g->V];
    int sps[g->V],count,mindist,nextnode,i,j,k,v[g->V],l;

    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
        {
            if(g->a[i][j]==0)
                cost[i][j]=MIN;
            else
            {
                if(i==s || j==s)
                    cost[i][j]=g->a[i][j];
                else
                    cost[i][j]=(g->a[i][j])+15; //adaug timpul de stationare pe toate drumurile care pleaca din insulele posibil intermediare
            }
        }


    for(i=0; i<n; i++) //initializez vectorii de distanta, predecesori si noduri vizitate
    {
        dist[i]=cost[s][i];
        pred[i]=s;
        sps[i]=0;
    }

    dist[s]=0;
    sps[s]=0;
    count=1; //numarul de noduri prin care a trecut

    while(count<n-1)
    {
        mindist=MIN;

        for(i=0; i<n; i++)
            if(dist[i]<mindist && !sps[i]) //cauta nodul la distanta minima
            {
                mindist=dist[i];
                nextnode=i;
            }
            //verifica daca exista un drum mai bun
        sps[nextnode]=1;
        for(i=0; i<n; i++)
            if(!sps[i])
                if(mindist+(cost[nextnode][i]) < dist[i])
                {
                    dist[i]=mindist+(cost[nextnode][i]);
                    pred[i]=nextnode;
                }
        count++;
    }

    for(i=0; i<n; i++)
        if(i!=s && i==f) //afiseaza costul drumului daca avioanele stationeaza in insulele intermediare
            fprintf(rez,"%d ",dist[i]);
    fprintf(rez,"\n");
}

void print_insule_3(Island *insule,int n,FILE *rez) //afisaza insulele dupa etapa de gestiune in cazul in care nu trebuie mutat nici un avion
{
    int i,j;
    for(i=0; i<n; i++)
    {
        fprintf(rez,"Island%d\n",i+1);
        for(j=0; j<insule[i].nrAvioane; j++)
            fprintf(rez,"%d ",insule[i].avioane[j]);
        fprintf(rez,"\n");
    }
}

void gestiune(Island *insule,int n,Graph *g,FILE *rez) //etapa de gestiune
{
    int i,j,l=0,k=0,sum=0,tol=0,*rest,r=0,ins,prim=0,primi=0,z=0,x=0,a[100][100],q,p,ok,max[20],v[20],aux;
    for(i=0; i<n; i++)
    {
        if(insule[i].nrAvioane > insule[i].tolAvioane) //verifica daca nici o insula nu isi depaseste toleranta de avioane
            k=1;
    }
    if(k==0)
        {
            print_insule_3(insule,n,rez); //in cazul acesta afiseza avioanele din fiecare insula fara nici o schimbare
            return;
        }


    for(i=0; i<n; i++)
    {
        sum += insule[i].nrAvioane; //calculeaza numarul total de avioane din arhipelag
        tol += insule[i].tolAvioane; //calculeaza toleranta intregului arhipelag
    }
    if(sum > tol)
    {
        fprintf(rez,"Stack overflow!"); //afiseaza daca sunt mai multe avioane in arhipelag dacat poate maxim tolera
        return;
    }
    for(i=0; i<n; i++)
    {
        if(insule[i].nrAvioane > insule[i].tolAvioane)
        {
            r=insule[i].nrAvioane-insule[i].tolAvioane;//calulaza numarul de avioane in exces
            ins=i;
        }
    }
    rest=malloc(sizeof(int)*r);
    for(i=0; i<n; i++)
    {
        if(insule[i].nrAvioane > insule[i].tolAvioane)
        {
            //printf("nr:%d tol:%d\n",insule[i].nrAvioane,insule[i].tolAvioane);
            for(j=insule[i].tolAvioane; j<insule[i].nrAvioane; j++)
            {
                rest[l]=insule[i].avioane[j]; //introduce id-urile avioanelor in exces intr-un vector
                l++;
            }
            insule[i].nrAvioane=insule[i].tolAvioane;//elimina avioanele in exces din insula respectiva
        }
    }

    for(i=0; i<n; i++)
    {
        if(g->a[ins][i]==1 && insule[i].tolAvioane-r >= insule[i].nrAvioane)
        {
            primi++; //numarul de insule in care au loc complet avionele in exces
            // printf("Insula%d ",i+1);
        }
        if(g->a[ins][i]==1)
        {
            prim++; //numarul de insule cu care are legaturi insula cu aviane in exces
        }
    }

    if(prim==primi ) //daca numarul de insule adiacente este egal cu numarul de insule in care se pot distribui complet avioanele
    {
        for(p=0; p<n; p++) //parcurge insulele
        {
            if(g->a[ins][p]==1 && insule[p].tolAvioane-r >= insule[p].nrAvioane) //daca mai au loc de avioane si sunt adiacente insulei cu exces de avioane
            {
                fprintf(rez,"Island%d\n",p+1);
                for(i=r; i>=0; i--)
                {
                    for(j=r-i; j>=0; j--)
                    {
                        ok=0;
                        for(q=0; q<insule[p].nrAvioane; q++)
                            fprintf(rez,"%d ",insule[p].avioane[q]);
                        for(l=j; l<j+i; l++)                       //afiseaza toate varinatele cum se pot distribui avioanele in exces in insulele adiacente
                        {
                            fprintf(rez,"%d ",rest[l]);
                            ok=1;
                        }
                        fprintf(rez,"\n");
                        if(!ok)
                            break;
                        x++;
                    }

                }
            }
            else
            {
                fprintf(rez,"Island%d\n",p+1);             //daca nu mai au loc avioane in insula sau nu este adiacenta insulei cu exces de avioane
                for(j=0; j<insule[p].nrAvioane; j++)
                    fprintf(rez,"%d ",insule[p].avioane[j]); //afiseaza id-urile avioanelor neschimbate
                fprintf(rez,"\n");
            }
        }
    }
    else if(primi!=0)
    {
        for(p=0; p<n; p++) //parcurge insulele
        {
            if(g->a[ins][p]==1 && insule[p].tolAvioane != insule[p].nrAvioane)
            {
                fprintf(rez,"Island%d\n",p+1);
                for(i=r; i>=0; i--)
                {

                    if ((insule[p].tolAvioane-insule[p].nrAvioane)%2 != 0 && (insule[p].tolAvioane-insule[p].nrAvioane) > 1) //daca numarul de avioane care poate fi pus intr-o insula este impar
                        for(j=r-i; j>=0; j--)                                                                              //si daca au loc mai mult de un avion
                        {
                            ok=0;
                            if(i>1 || (i==1 && j%2==0))   //afisaza avioanele luand prioritaile id-urilor in calcul
                            {
                                for(q=0; q<insule[p].nrAvioane; q++)
                                    fprintf(rez,"%d ",insule[p].avioane[q]);

                                for(l=j; l<j+i; l++)
                                {
                                    fprintf(rez,"%d ",rest[l]);
                                    ok=1;
                                }
                                fprintf(rez,"\n");
                                if(!ok)
                                    break;
                            }
                        }
                    else
                        for(j=r-i; j>=0; j--)
                        {
                            ok=0;
                            if(j==0 && i==r)                //verifica sa nu trimite mai multe avioane decat pot fi situate in insula
                                fprintf(rez,"");
                            else
                            {
                                if(i>1 || (i==1 && j%2==0) || i==0)   //afisaza avioanele luand prioritaile id-urilor in calcul
                                {
                                    for(q=0; q<insule[p].nrAvioane; q++)
                                        fprintf(rez,"%d ",insule[p].avioane[q]);


                                    for(l=j; l<j+i; l++)
                                    {
                                        fprintf(rez,"%d ",rest[l]);
                                        ok=1;
                                    }
                                    fprintf(rez,"\n");
                                    if(!ok)
                                        break;
                                }
                            }
                        }

                }
            }
            else
            {
                fprintf(rez,"Island%d\n",p+1);             //daca nu este adiacenta insulei cu exces de avioane
                for(j=0; j<insule[p].nrAvioane; j++)
                    fprintf(rez,"%d ",insule[p].avioane[j]); //afiseaza id-urile avioanelor neschimbate
                fprintf(rez,"\n");
            }
        }
    }

    if(primi==0)
    {
        k=0;
        p=0;
        q=0;
        for(i=0; i<n; i++) //creez un vector in care retin insulele care sunt conectate la insula cu exces de avioane
            if(g->a[ins][i]==1)
            {
                max[k]=i;
                k++;
            }

        for(i=0; i<k; i++)  //pentru insulele adiacente insulei cu exces de avioane, caut o insula care sa primeasca avioanele
        {
            for(j=0; j<n; j++)
            {
                if(g->a[max[i]][j] && j!=ins && insule[j].nrAvioane<insule[j].tolAvioane)
                {
                    for(l=insule[max[i]].nrAvioane-r; l<insule[max[i]].nrAvioane; l++) //din insula adiacenta plina, retin avioanele ce trebuie mutate pentru a face loc
                    {
                        v[p]=insule[max[i]].avioane[l];
                        p++;
                    }
                    for(l=insule[max[i]].nrAvioane-r; l<insule[max[i]].nrAvioane; l++) //mut in insula de legatura avioanele din insula cu exces de avioane
                    {
                        insule[max[i]].avioane[l]=rest[q];
                        q++;
                    }

                    for(l=0; l<insule[max[i]].nrAvioane; l++) //le ordonez crescator
                        for(q=l+1; q<insule[max[i]].nrAvioane; q++)
                            if (insule[max[i]].avioane[l] > insule[max[i]].avioane[q])
                            {
                                aux=insule[max[i]].avioane[l];
                                insule[max[i]].avioane[l]=insule[max[i]].avioane[q];
                                insule[max[i]].avioane[q]=aux;
                            }

                    q=0;
                    for(l=0; l<insule[j].nrAvioane+p; l++) //mut avioanele din insula de legatura in insula care avea locuri libere
                    {
                        if(l>=insule[j].nrAvioane)
                        {
                            insule[j].avioane[l]=v[q];
                            q++;
                        }
                    }
                    insule[j].nrAvioane +=p; //maresc numarul de avioane din insula
                    for(l=0; l<insule[j].nrAvioane; l++)//le ordonez crescator
                        for(q=l+1; q<insule[j].nrAvioane; q++)
                            if (insule[j].avioane[l] > insule[j].avioane[q])
                            {
                                aux=insule[j].avioane[l];
                                insule[j].avioane[l]=insule[j].avioane[q];
                                insule[j].avioane[q]=aux;
                            }
                }

            }
        }
        print_insule_3(insule,n,rez);
    }
}

int main(int argc, char **argv)
{
    FILE *data,*rez;
    int n,i,j,p,cost,m1,m2,in1,in2,ni,*v,tol,val;
    Island *insule,aux;
    char buf[MAX_LENGTH],c1[MAX_LENGTH],c2[MAX_LENGTH];

    data=fopen(argv[1],"rt");
    if(data==NULL)
    {
        puts("Fisierul nu s-a deschis.");
        exit (0);
    }

    rez=fopen(argv[2],"wt");
    if(rez==NULL)
    {
        puts("Fisierul nu s-a deschis.");
        exit (0);
    }

    fscanf(data,"%d",&n); //citesc numarul de tari
    insule=malloc(sizeof(Island)*n); //aloc memoria necesara vectorului de insule
    for(i=0; i<n; i++)
    {
        fscanf(data,"%s",buf); //citesc numele insulelor
        insule[i].nume=malloc(sizeof(char)*(strlen(buf)+1));
        strcpy(insule[i].nume,buf);

        fscanf(data,"%d",&insule[i].nrResurse); //citesc numarul de resurse

        insule[i].inventarResurse=malloc(sizeof(Resursa)*(insule[i].nrResurse));
        for(j=0; j<insule[i].nrResurse; j++)
        {
            fscanf(data,"%s",buf); //citesc resursa
            insule[i].inventarResurse[j].nume=malloc(sizeof(char)*(strlen(buf)+1));
            strcpy(insule[i].inventarResurse[j].nume,buf);
            fscanf(data,"%d",&insule[i].inventarResurse[j].cantitate); //si cantitatea ei
        }
    }

    fscanf(data,"%d",&p);//citesc numarul de conexiuni

    Graph *g=create_empty_graph(n); //initializez un graf cu nici o conexiune

    for(i=0; i<p; i++) //introduc conexiunile initiale intre insule
    {
        fscanf(data,"%s",buf);
        m1=numar_insula(buf);

        fscanf(data,"%s",buf);

        fscanf(data,"%s",buf);
        m2=numar_insula(buf);

        fscanf(data,"%d",&cost);

        adauga_zbor(g,m1,m2,cost);
    }

    while(!feof(data)) //citeste cate o linie
    {
        fscanf(data,"%s",buf);
        if(isalpha(buf[0]) != 0) //daca linia incepe cu o cerinta, verifica care este cerinta care trebuie rezolvata
        {
            if(strcmp(buf,"conexiune")==0)
            {
                fscanf(data,"%s",buf);
                in1=numar_insula(buf);
                fscanf(data,"%s",buf);
                in2=numar_insula(buf);
                conexiune(g,in1,in2,rez);
                in1=0;
                in2=0;
            }
            else if(strcmp(buf,"legatura")==0)
            {
                fscanf(data,"%s",buf);
                in1=numar_insula(buf);
                legatura(g,in1,rez);
                in1=0;
            }
            else if(strcmp(buf,"adauga_zbor")==0)
            {
                fscanf(data,"%s",buf);
                in1=numar_insula(buf);

                fscanf(data,"%s",buf);
                in2=numar_insula(buf);

                fscanf(data,"%d",&cost);

                adauga_zbor(g,in1,in2,cost);
                in1=0;
                in2=0;
            }

            else if(strcmp(buf,"anulare_zbor")==0)
            {
                fscanf(data,"%s",buf);
                in1=numar_insula(buf);

                fscanf(data,"%s",buf);
                in2=numar_insula(buf);

                anulare_zbor(g,in1,in2);
            }
            else if(strcmp(buf,"max_resurse")==0)
                max_resurse(insule,n,rez);
            else if(strcmp(buf,"max_cantitate")==0)
            {
                fscanf(data,"%s",buf);
                max_cantitate(insule,buf,n,rez);

            }
            else if(strcmp(buf,"drum_zbor")==0)
            {
                fscanf(data,"%s",buf);
                in1=numar_insula(buf);
                fscanf(data,"%s",buf);
                in2=numar_insula(buf);
                drum_zbor(g,n,in1,in2,rez);
            }
            else if(strcmp(buf,"timp_zbor")==0)
            {
                fscanf(data,"%s",buf);
                in1=numar_insula(buf);
                fscanf(data,"%s",buf);
                in2=numar_insula(buf);
                timp_zbor(g,n,in1,in2,rez);
            }
            else if(strcmp(buf,"min_zbor")==0)
            {
                fscanf(data,"%s",buf);
                in1=numar_insula(buf);
                fscanf(data,"%s",buf);
                in2=numar_insula(buf);
                min_zbor(g,n,in1,in2,rez);
            }

        }
        else if(isdigit(buf[0]) != 0)//daca la incepul liniei a intalnit o cifra, ci nu o cerinta, incepe citirea pentru etapa 3
        {
            free(insule); //elibereaza memoria alocata vectorului de insule
            free(g); //eliberaza memoria alocata grafului
            ni=atoi(buf);
            insule=malloc(sizeof(Island)*ni);//aloc memoria necesara noului vector de insule
            fscanf(data,"%d",&tol); //citeste tolerantele insulelor
            for(i=0; i<ni; i++)
                insule[i].tolAvioane=tol;
            v=malloc(sizeof(int)*ni);
            for(i=0; i<ni; i++)
                fscanf(data,"%d",&insule[i].nrAvioane); //citeste numarul de avioane dintr-o insula
            for(i=0; i<ni; i++)
            {
                insule[i].avioane=malloc(sizeof(int)*insule[i].nrAvioane);
                for(j=0; j<insule[i].nrAvioane; j++)
                    fscanf(data,"%d",&insule[i].avioane[j]); //citeste avioanele dintr-o insula
            }
            g=create_empty_graph(ni); //initializez un nou graf fara conexiuni
            for(i=0; i<ni; i++) //adauga conexiunile in graf
            {
                for(j=0; j<ni; j++)
                {
                    fscanf(data,"%d",&val);
                    g->a[i][j]=val;
                }
            }

            gestiune(insule,ni,g,rez); //etapa de gestiune
            fprintf(rez, "\n");
            break;
        }

    }
    fclose(data);
    fclose(rez);
}

