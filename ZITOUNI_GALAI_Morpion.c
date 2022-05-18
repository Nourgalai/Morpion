#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>


#define CONTINUE 0
#define FINISHED 1



//declaration des structures

/*typedef struct memory
{
   uint16_t poss_grille;
   uint64_t *billes;
   uint8_t ouverte;
   struct box_matches *next;
    
} memory;*/



typedef struct box_matches
{
   uint16_t poss_grille;
   uint64_t *billes;
   uint8_t ouverte;
   struct box_matches *next;
    
} box_matches;


typedef struct liste
{
    uint32_t taille;
    box_matches *tete;
    box_matches *queue;
    
} liste;

typedef struct tab_hachage
{
    liste **tab;
    uint32_t taille;

} tab_hachage;



typedef enum transformation{ID, ROT_90, ROT_180, ROT_270, MIROIR_VERT, MIROIR_HORIZ} transformation;

char print_value(uint8_t value);
void appliquer_transformation_base(uint8_t grille[3][3], transformation t);
void print_grille_2d(uint8_t grille[3][3]);
void print_grille_1d(uint8_t grille[3][3], FILE *f);
uint8_t next_configuration(uint8_t grille[3][3]);

//fonction des box_matchess

box_matches *new_box_matches(uint16_t d);

//fonction des listes
liste *new_liste(void);
void add_tete(liste *l, uint16_t d);
_Bool est_vide(liste *l);
void vider(liste *l);
uint32_t rem_tete(liste *l);

//fonction de la table de hachage
tab_hachage* new_tab_hachage(uint32_t taille);
void vider_th(tab_hachage *th);
uint8_t fnc_hachage(uint8_t grille[3][3]);
void add_tete_th(tab_hachage *th,uint8_t grille[3][3]);
void fill_hash_tab(tab_hachage *hash_tab);
//void edit_hash_tab(tab_hachage *hash_tab);



//fonction du remplissage des grilles
uint8_t symetrie(uint16_t poss_grille, uint8_t g[3][3]);
uint8_t impossible_config(uint8_t g[3][3]);
uint8_t end_game(uint8_t g[3][3]);

//initialisation nombre de billes
//void init_billes(FILE *config, tab_hachage *hash_tab);
uint16_t codage(uint8_t g[3][3]);
void decodage(uint16_t data, uint32_t* binary);
void case_jouer(uint16_t data, uint64_t *tab);
void init_billes(FILE *config, tab_hachage *hash_tab);


//fonctions de jeu
uint8_t random_select(box_matches *e);
void user_play(uint8_t g[3][3]);
void menace_play(tab_hachage *hash_tab, uint8_t g[3][3]);
uint8_t game_choice(void);
void menace_user(tab_hachage *hash_tab);
void game_start(tab_hachage *hash_tab);
uint8_t nmbre_coup(uint8_t g[3][3]);



char print_value(uint8_t value)
{
    switch(value)
    {
        case (0):
            return ' ';
        case (1):
            return 'o';
        case (2):
            return 'x';
        default:
            assert(0);
    }
}

void appliquer_transformation_base(uint8_t grille[3][3], transformation tr)
{
    uint8_t t;
    switch(tr)
    {
        case(ID):
            break;

        case(ROT_90):
            t = grille[0][0];
            grille[0][0] = grille[2][0];
            grille[2][0]=grille[2][2];
            grille[2][2]=grille[0][2];
            grille[0][2]=t;

            t = grille[0][1];
            grille[0][1] = grille[1][0];
            grille[1][0]=grille[2][1];
            grille[2][1]=grille[1][2];
            grille[1][2]=t;
            break;

        case(ROT_180):
            appliquer_transformation_base(grille, ROT_90);
            appliquer_transformation_base(grille, ROT_90);
            break;

        case(ROT_270):
            appliquer_transformation_base(grille, ROT_90);
            appliquer_transformation_base(grille, ROT_90);
            appliquer_transformation_base(grille, ROT_90);
            break;

        case(MIROIR_VERT):
            t = grille[0][0];
            grille[0][0] = grille[0][2];
            grille[0][2] = t;

            t = grille[1][0];
            grille[1][0] = grille[1][2];
            grille[1][2] = t;

            t = grille[2][0];
            grille[2][0] = grille[2][2];
            grille[2][2] = t;
            break;

        case(MIROIR_HORIZ):
            appliquer_transformation_base(grille, ROT_90);
            appliquer_transformation_base(grille, MIROIR_VERT);
            appliquer_transformation_base(grille, ROT_270);
    }
}


void appliquer_transformation_base_billes(uint64_t grille[3][3], transformation tr)
{
    uint64_t t;
    switch(tr)
    {
        case(ID):
            break;

        case(ROT_90):
            t = grille[0][0];
            grille[0][0] = grille[2][0];
            grille[2][0]=grille[2][2];
            grille[2][2]=grille[0][2];
            grille[0][2]=t;

            t = grille[0][1];
            grille[0][1] = grille[1][0];
            grille[1][0]=grille[2][1];
            grille[2][1]=grille[1][2];
            grille[1][2]=t;
            break;

        case(ROT_180):
            appliquer_transformation_base_billes(grille, ROT_90);
            appliquer_transformation_base_billes(grille, ROT_90);
            break;

        case(ROT_270):
            appliquer_transformation_base_billes(grille, ROT_90);
            appliquer_transformation_base_billes(grille, ROT_90);
            appliquer_transformation_base_billes(grille, ROT_90);
            break;

        case(MIROIR_VERT):
            t = grille[0][0];
            grille[0][0] = grille[0][2];
            grille[0][2] = t;

            t = grille[1][0];
            grille[1][0] = grille[1][2];
            grille[1][2] = t;

            t = grille[2][0];
            grille[2][0] = grille[2][2];
            grille[2][2] = t;
            break;

        case(MIROIR_HORIZ):
            appliquer_transformation_base_billes(grille, ROT_90);
            appliquer_transformation_base_billes(grille, MIROIR_VERT);
            appliquer_transformation_base_billes(grille, ROT_270);
    }
}








void print_grille_2d(uint8_t grille[3][3])
{
    printf("|%c|%c|%c|\n", print_value(grille[0][0]), print_value(grille[0][1]), print_value(grille[0][2]));
    printf("|%c|%c|%c|\n", print_value(grille[1][0]), print_value(grille[1][1]), print_value(grille[1][2]));
    printf("|%c|%c|%c|\n", print_value(grille[2][0]), print_value(grille[2][1]), print_value(grille[2][2]));
}

void print_grille_1d(uint8_t grille[3][3], FILE *f)
{
    fprintf(f, "%c%c%c%c%c%c%c%c%c", print_value(grille[0][0]), print_value(grille[0][1]), print_value(grille[0][2]), print_value(grille[1][0]), print_value(grille[1][1]), print_value(grille[1][2]), print_value(grille[2][0]), print_value(grille[2][1]), print_value(grille[2][2]));
}

void print_all_transformations_1d(uint8_t g[3][3], FILE *f)
{
    print_grille_1d(g,f);
    fprintf(f, "|");

    appliquer_transformation_base(g, ROT_90);
    print_grille_1d(g,f);
    fprintf(f, "|");

    appliquer_transformation_base(g, ROT_90);
    print_grille_1d(g,f);
    fprintf(f, "|");

    appliquer_transformation_base(g, ROT_90);
    print_grille_1d(g,f);
    fprintf(f, "|");

    appliquer_transformation_base(g, ROT_90);
    appliquer_transformation_base(g, MIROIR_VERT);
    print_grille_1d(g,f);
    fprintf(f, "|");

    appliquer_transformation_base(g, ROT_90);
    print_grille_1d(g,f);
    fprintf(f, "|");

    appliquer_transformation_base(g, ROT_90);
    print_grille_1d(g,f);
    fprintf(f, "|");

    appliquer_transformation_base(g, ROT_90);
    print_grille_1d(g,f);

    //On fait une derniere transformation pour remettre la grille dans son état initial
    appliquer_transformation_base(g, ROT_90);
    appliquer_transformation_base(g, MIROIR_VERT);
    fprintf(f, "\n");
}

uint8_t next_configuration(uint8_t grille[3][3])
{
    uint8_t i, j;

    grille[0][0]++;
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
        {
            if(grille[i][j]==3)
            {
                grille[i][j]=0;
                if(j<2) grille[i][j+1]++;
                else if(i<2) grille[i+1][0]++;
                else return FINISHED;
            }
        }
    }

    return CONTINUE;
}


int main() {


    tab_hachage *hach = new_tab_hachage(4);
    fill_hash_tab(hach);
    game_start(hach);
    vider_th(hach);
    
  return EXIT_SUCCESS;
}



box_matches *new_box_matches(uint16_t d)
{
    
    box_matches *m = malloc(sizeof(box_matches));
    m->billes = (uint64_t*)malloc(9*sizeof(uint64_t));

    if(m==NULL)
    {
        assert(0);
    }
    
    m->poss_grille = d;

    case_jouer(d, m->billes);
    return m ;
}

liste *new_liste(void)
{
    
    liste *r = malloc(sizeof(liste));
    if (r==NULL)
    {
        assert(0);
    }
    r->taille = 0;
    r->tete = NULL;
    r->queue = NULL;
    return r;
}

void add_tete(liste *l, uint16_t d)
{ 
    box_matches *m = new_box_matches(d);
    m->next = l->tete;
    l->tete = m;

    if (l->taille == 0)
    {
        l->queue = m;
    }

    l->taille += 1;

}

_Bool est_vide(liste *l)
{
    return l->taille == 0;

}

uint32_t rem_tete(liste *l)
{
    box_matches *t = l->tete;
    uint64_t *b = t->billes;
    uint32_t r = t->poss_grille;
    free(b);
    l->tete = l->tete->next;
    free(t);
    l->taille -= 1;

    if (l->taille == 0)
    {
        l->queue = NULL;
    }
    return r;
}

void vider(liste *l)
{
    while(!est_vide(l))
    {
        rem_tete(l);
    }
    free(l);
}


tab_hachage* new_tab_hachage(uint32_t taille)
{
    uint32_t i;

    tab_hachage *th = malloc(sizeof(tab_hachage));
    assert(th !=NULL);

    th->tab = malloc(taille*sizeof(liste*));
    assert(th->tab !=NULL);

    for (i = 0; i < taille; i++)
    {
        th->tab[i] = new_liste();
    }

    th->taille = taille;
    return th;

}

void vider_th(tab_hachage *th)
{
    uint32_t i;

    for (i = 0; i < th->taille; i++)
    {
        vider(th->tab[i]);
    }

    free(th->tab);
    free(th);
}


// à modifier
uint8_t fnc_hachage(uint8_t g[3][3] )
{
    uint8_t r =0;
    for (uint8_t i = 0; i < 3; ++i)
    {
        for (uint8_t j = 0; j < 3; ++j)
        {
            if(g[i][j])
               r++;
        }
    }

    return r;
    // SHA1 et MD5 sont des fonctions de hachage souvent utilisées, librairie openssl
}



void add_tete_th(tab_hachage *th, uint8_t g[3][3] )
{
    uint8_t p = fnc_hachage (g);
    p=p/2;
    uint16_t poss_grille = g[2][2]+g[2][1]*3+g[2][0]*9+g[1][2]*27+g[1][1]*81+g[1][0]*243+g[0][2]*729+g[0][1]*2187+g[0][0]*6561; 
    //printf(" %u\n",poss_grille);

    add_tete(th->tab[p], poss_grille);
}



void fill_hash_tab(tab_hachage *hash_tab)
{  
    uint8_t key=0;
    uint16_t c = 1;
    uint8_t g[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    add_tete_th(hash_tab, g);
  


    box_matches *e = NULL;

    
    while(next_configuration(g)==CONTINUE)
    {   

      key = fnc_hachage(g);
      if (impossible_config(g)==0 && end_game(g)==0 && key%2==0 && key<8){
         key = key/2;
        e = hash_tab->tab[key]->tete;

        while( e != NULL && symetrie(e->poss_grille, g )==0)
       {
           e = e->next;
       }

        if (e == NULL)
        {
            add_tete_th(hash_tab, g);
            c++;
    
        }

     }
   
    }

  //printf("Number of match boxes is = %u\n", c);
}





uint8_t symetrie(uint16_t poss_grille, uint8_t g[3][3]){
    
    uint16_t tmp=0;
    uint8_t s=0;
    
    for (uint8_t i = 0; i < 7; i++)
    {

        appliquer_transformation_base(g, ROT_90);
        if(i==3)
          appliquer_transformation_base(g, MIROIR_VERT);
       

        tmp = g[2][2]+g[2][1]*3+g[2][0]*9+g[1][2]*27+g[1][1]*81+g[1][0]*243+g[0][2]*729+g[0][1]*2187+g[0][0]*6561; 
        if (poss_grille==tmp )
            s=1;
    }

     appliquer_transformation_base(g, ROT_90);
     appliquer_transformation_base(g, MIROIR_VERT);

    return s;
}


uint8_t impossible_config(uint8_t g[3][3]){
    uint8_t x= 0;
    uint8_t o=0;

    for (uint8_t i = 0; i < 3; i++)
    {
       for (uint8_t j = 0; j < 3; j++)
       {
           if (g[i][j]==1)
           {
               o++;
           }

           else if (g[i][j]==2)
           {
               x++;
           }
       }
    }

  if (abs(x-o)==1 || abs(x-o)==0 )
  {
     return 0;
  }

  else
    return 1;
  
}

uint8_t end_game(uint8_t g[3][3]){
    uint8_t x= 0;
    uint8_t y= 0;
    uint8_t d=0;
    uint8_t dd=0;
    uint8_t w=0;
    
    for (uint8_t i = 0; i < 3; i++)
    {
       x=g[i][0]+g[i][1]+g[i][2]; // x=6 ligne
       y=g[0][i]+g[1][i]+g[2][i]; // column
       d=d+g[i][i];
       dd=dd+g[i][2-i];
       

       if(g[i][0] ==0 ||g[i][1]==0 || g[i][2]==0 )
        x=0;
       if(g[0][i] ==0 ||g[1][i]==0 || g[2][i]==0 )
        y=0;

       if(x==6 || x==3 || y==6 || y==3  )
         w =1;

       x=0;
       y=0;
       if(g[i][i]==0 )
         d=10;
      if(g[i][2-i]==0 )
         dd=10;

    }
    
    if(d==6 || d==3 || dd==6 || dd==3 )
        w=1;

    
  return w;
}



void decodage(uint16_t data, uint32_t *binary){
    for(uint32_t i=0; i<9;i++){
         binary[i]=0;
        } 
       
    uint32_t c=0;
    uint32_t remainder, quotient;
    quotient = data /3;
    uint32_t data_ = data;
    if (quotient==0){
       
        for(uint32_t i=0; i<9;i++){
          if(i==0)
            binary[i]= data_;
          else 
            binary[i]=0;
        } 
       
    }
   else{
    remainder = data%3;
    binary[c] = remainder;
    //printf("%u\n", res);
    while(quotient!=0){
       c++;
       data = quotient;
       quotient = data /3;
       remainder = data%3;
       binary[c] = remainder;
      // printf("%llu\n", c);
      }
   }

}

uint8_t nmbre_coup(uint8_t g[3][3]){
    uint8_t coup=0;
    for (uint8_t i = 0; i < 3; ++i)
    {
        for (uint8_t j = 0; j < 3; ++j)
        {
           if (g[i][j]!=0)
           {
               coup++;
           }
        }
    }

    return coup;
}




void case_jouer(uint16_t data, uint64_t* billes){
  
    uint32_t i=8;
    uint32_t *binary  =  (uint32_t*)malloc(9*sizeof(uint32_t));
   
   decodage(data, binary);
   
    for (uint64_t tmp= 0; tmp <9; tmp++)
    {
        //tmp = binary%10;
        if(binary[i]>0)
            billes[tmp] = 0;
        else
            billes[tmp] = 1;

       i--;
    }
    free(binary);
    
    
   printf("\n");
    
}


uint8_t random_select(box_matches *e){
   srand(time(0));
   uint32_t total_billes=0;
   uint32_t random_bille=0;
   uint8_t j=0;
   for (uint64_t i = 0; i < 9; ++i)
   {
      total_billes = total_billes + e->billes[i];
   }

  // printf("%u\n", total_billes);
   
   random_bille = (rand()%total_billes)+1;

   //printf("%u\n", random_bille);

   while(random_bille>0)
   { 
     random_bille = random_bille - e->billes[j];
     //printf("%u\n", random_bille);

     j++;

   }
  // printf("%u\n", j);
   return j-1;

}


uint16_t codage(uint8_t g[3][3]){

 uint16_t tmp = g[2][2]+g[2][1]*3+g[2][0]*9+g[1][2]*27+g[1][1]*81+g[1][0]*243+g[0][2]*729+g[0][1]*2187+g[0][0]*6561; 
 return tmp;

}

void user_play(uint8_t g[3][3]){
    uint8_t i,j;
    char k,l;
    printf("choisis la ligne\n");
    scanf(" %c", &k);
    printf("choisis la colonne \n");
    scanf(" %c", &l);
    i=k-48;
    j=l-48;

   // printf("%u%u",k,l);
    
    g[i][j]= 2;
    print_grille_2d(g);
    
    if(end_game(g))
      printf("le joueur a gagné");
}

void menace_play(tab_hachage *hash_tab, uint8_t g[3][3]){

   uint64_t billes_nv[3][3];

   uint8_t key = fnc_hachage(g); //bon
   key=key/2;
   uint16_t poss_gri = codage(g);  // bon
   uint8_t tmp_grille[3][3];
   uint32_t *decodage_grille  =  (uint32_t*)malloc(9*sizeof(uint32_t));

   uint8_t menace_choice = 0; // bon 

   box_matches *e;
   box_matches *nv;
   
   if(key<4)
   e = hash_tab->tab[key]->tete;
 

   uint32_t c=0;
   uint64_t k=0;
   uint8_t s=0;
 
     
   while( e != NULL && key<4)
       {   

            nv=e;
            k=0;
   
            for (uint64_t i = 0; i < 3; i++)
           {
                for (uint64_t j = 0; j< 3; j++)
               {
                  billes_nv[i][j] = e->billes[k];
                  k++;
               }
            }

    
        decodage(e->poss_grille, decodage_grille );
        c=8;
       for (uint8_t i = 0; i < 3; i++)
      {
          for (uint8_t j = 0; j < 3; j++)
          {
             tmp_grille[i][j] = decodage_grille[c];
             c--;
          }
       }

      if(e->poss_grille==poss_gri){
             nv=e;
      }
           
    else{


      for (uint8_t i = 0; i < 7; i++)
      {
          appliquer_transformation_base(tmp_grille, ROT_90);
          appliquer_transformation_base_billes(billes_nv, ROT_90);

          if(i==3){
             appliquer_transformation_base(tmp_grille, MIROIR_VERT);
             appliquer_transformation_base_billes(billes_nv, MIROIR_VERT);
          }
     

          e->poss_grille = codage(tmp_grille);
          if (poss_gri==e->poss_grille){
             nv->poss_grille = poss_gri;
             s=1;
             k=0;
             //nv->poss_grille = poss_gri;
             for (uint64_t i = 0; i < 3; i++)
             {
                for (uint64_t j = 0; j< 3; j++)
                {
                   nv->billes[k] =  billes_nv[i][j];
                   k++;
                }

            }
            break;
         }
     }
   
     

    }
    if(s==1)
    break;

   e = e->next;
  }


    uint8_t m;
    uint8_t l;

   if(key<4){
    menace_choice = random_select(nv);
    g[menace_choice/3][menace_choice%3] = 1;
   }
   else{
       for(uint8_t i=0; i<3;i++){
           for(uint8_t j=0; j<3;j++){
              if(g[i][j]<1){
                  m=i;
                  l=j;
                  }   
          }
         
       }
       
       
        g[m][l] = 1;
    }   
    //printf("menace_choice = %u\n",menace_choice );
  

    print_grille_2d(g);

    free(decodage_grille);
    
    if(end_game(g))
      printf("la menace a gagné");

}


 
uint8_t game_choice(void){
    uint8_t res=0;
    char choix;
    printf("Bienvenue au morpion\n");
    printf("1 : menace contre joueur\n");
    printf("2 : menace contre menace\n");
    scanf("%c", &choix);
    res = choix-48;
 return res;
}


void menace_user(tab_hachage *hash_tab){
    
   uint8_t g[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
   //print_grille_2d(g);


   while(!end_game(g))
   { 
      menace_play(hash_tab,g);
      printf("\n\n");
      if(end_game(g))
        break;
      if(fnc_hachage(g)==9){
          
          printf("Egalité");
         break;
       }
      
      user_play(g);
      printf("\n\n");

   }
   

  

}

void game_start(tab_hachage *hash_tab){
   
 
   //uint8_t choix = game_choice();
   if(game_choice()==1){
     system("clear");
     menace_user(hash_tab);
     }
  // else
}



