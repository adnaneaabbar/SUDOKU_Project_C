#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<string.h>
#include<conio.h>
#define N 4
#define Nom 30
#define Pass 30
#define Question 20
#define	Pseudo 20
#define Vide 0
#define Nbrdetest 2
#define SommeLC 10
#define Scorefacile 5
#define Scoremoyen 10
#define	Scoredifficile 20
#define Facile 1
#define Moyen 2
#define Difficile 3
#define aSauvegarde 15
#define nonSauvegarde 16
#define Retourner 5


typedef struct
{
	char pseudo[Pseudo];
	char nom[Nom];
	char pass[Pass];
	int difficulte;
	int sauvegarde;
	//int indice_question;
	//char question[Question];
	int sudoku_init[N][N];
	int sudoku_jeu[N][N];
	int score;
}joueur;

// Variable Globale
int DIFFICULTE;
int VALIDATION = 0;
int NBRJOUEUR = 0 ;
int JOUEUR_COURANT=-1;
int INDICETOP10[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int A_JOUEE=0;
//joueur joueuronline; declaration mais affectation a la connexion
joueur *TABLEJOUEUR;

int sudoku_init[N][N]={
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {0, 0, 0, 0}
	};
int sudoku_jeu[N][N]={
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {0, 0, 0, 0}
	};
// Prototypes
void delay(int nombre_de_seconds);
void Exporter();
void Importer();
void Saisie_de_Mot_de_passe(char pass[Pass]); // permet la saisie de mot de passe en mode ***** avec la possibilite de supprimer votre saisie
int Joeur_in_list_Top_10(int joueur_courant); // savoir si le joueur est dans la liste de top 10 car s'il  le joueur existe dans la liste il faut juste updater sa place
char getcharandflush();						  // permet de flusher le stdin et revoit un charactere de stdin ( input )
void Difficulte();							  // fonction avec menu difficulte qui questionne l'utilisateur sur la difficulte du jeu
bool estValide_et_solution (int sudoku[N][N], int position); //fonction qui cree une solution
void Connexion();											 //permet la connexion
void Menu_compte();											 //Menu apres connexion avec choix et affichage
void Creation_Compte();										 // creation compte et connexion immediate il include la fonction de saisie de mot de passe
void Affichage_ListTop10();
void Jouer();												 //fonction qui cree les table et questionne le joueur s'il veut continuer le jeu sauvgarde
void Changer_Pass();
void Deconnexion();											 //deconnexion et update list top 10 et demande de sauvegarde du jeu si le joueur a lancer le choix jouer sans valider
int Pseudo_exist(char pseudo[Pseudo]);						 // revoie l'emplacement sur la table joeur du joeur de pseudo "pseudo" et -1 s'il n'existe pas
void Maj_liste_Top10();										 // j'ai rectifie quelques modification puisque si le joueur est deja dans la liste top 10 faut pas decaler tous
															 // c'est pour cette raison la creation de la fonction Joeur_in_list_Top_10
int _1pow(int entier);										 // renvoie (-1)^(entier)
bool Diag_Valid(int entier,int ligne,int col,int sudoku[N][N]);
bool Test_creation(int entier,int ligne,int col,int sudoku_init[N][N]);
void Zero(int sudoku[N][N]);								 // vide les tables sudoku , car le fonction remlissage remplie les tables , mais si un joueur a jouer un parte la seconde partie sera la table initiale remplie une autre fos
void Remplissage();
bool Case_Vide(int ligne,int col);
bool Insertion_Valid(int entier ,int ligne,int col);
void Insertion(int entier);
void Affichage(int sudoku[N][N]);
void Validation();
bool absentSurLigne(int k, int sudoku[N][N], int i);
bool absentSurColonne(int k, int sudoku[N][N], int j);
bool absentSurBloc(int k, int sudoku[N][N], int i, int j);
void Menu_principale();

void delay(int nombre_de_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * nombre_de_seconds;

    // Storing start time
    clock_t debut_temp = clock();

    // looping till required time is not acheived
    while (clock() < debut_temp + milli_seconds);
}


void Exporter()
{
	printf("Exportation....");
	delay(5);
    FILE *fichier_save = fopen("file_save.bin", "wb");
    fwrite (&NBRJOUEUR , sizeof(int), 1, fichier_save);
    fwrite (INDICETOP10 , sizeof(int), 10, fichier_save);
    fwrite (TABLEJOUEUR , sizeof(joueur), NBRJOUEUR, fichier_save);
    fclose(fichier_save);

    return ;
}

void Importer()
{
    FILE *fichier_save = fopen("file_save.bin", "rb");
    if(fichier_save){
    	printf("Importation....");
    	delay(5);
    	fread (&NBRJOUEUR , sizeof(int), 1, fichier_save);
    	fread (INDICETOP10 , sizeof(int), 10, fichier_save);
    	TABLEJOUEUR = (joueur *)realloc(TABLEJOUEUR, NBRJOUEUR*sizeof(joueur));
    	fread (TABLEJOUEUR , sizeof(joueur), NBRJOUEUR, fichier_save);
    }
    fclose(fichier_save);

    return ;
}

void Saisie_de_Mot_de_passe(char pass[Pass])
{
	char ch;
	int characterposition = 0;
	for(int i = 0;1;i++){

		ch = getch();
		if(ch==13)//enter
			break;
		else if(ch==8){
			if(characterposition > 0){
			characterposition--;
			pass[characterposition]='\0';
			printf("\b \b");}
			continue;
		}//pas de back space dans le password
		else if(ch==32 || ch == 9){//space or tab
			continue;
		}else{
			if(characterposition < Pass)
				{
				pass[characterposition]=ch;
				characterposition++;
				printf("*");
				}
			else{
				//printf("\nTon Mot de passe a depasse le maximum, just les %d characters premier vont etre considere .",Pass);
				break;
			}
		}
	}
	if(characterposition == 0){
		printf("\nSaisir un mot de passe valide : ");
		Saisie_de_Mot_de_passe(pass);
	}
	else{
		pass[characterposition]='\0';
	}
	return;
}

int Joeur_in_list_Top_10(int joueur_courant)
{
	int indicejoueur_dans_top_10=-1;//on suppose que le joueur n'existe pas dans la table top 10
	for(int i=0;i<10 && indicejoueur_dans_top_10==-1 ;i++)
	{
		if(INDICETOP10[i]==joueur_courant)
			indicejoueur_dans_top_10=i;
	}
	return indicejoueur_dans_top_10; // -1 si le joueur n'existe pas et son indice dans la table top 10 s'il existe
}

void Difficulte()
{   int flag=0;
    system("cls");
    printf("--------------------------------------------------------------\n");
    printf("-                                                            -\n");
    printf("-                           Difficulte                       -\n");
    printf("-                                                            -\n");
    printf("--------------------------------------------------------------\n\n");
    printf("--------------------------------------------------------------\n");
    printf("-                                                            -\n");
    printf("-                      1- Facile                             -\n");
    printf("-                                                            -\n");
    printf("-                      2- Moyen                              -\n");
    printf("-                                                            -\n");
    printf("-                      3- Difficile                          -\n");
    printf("-                                                            -\n");
    printf("--------------------------------------------------------------\n\n\n");
    do{ (flag==0)?printf("-                      Saisir Votre Choix                    -\n                       "):printf("-                      Saisir un Choix Valide                -\n                       ");
        DIFFICULTE = getcharandflush()-'0';
        fflush(stdin); //clear buffer
        flag++;}
    while(DIFFICULTE!=Facile && DIFFICULTE!=Moyen && DIFFICULTE!=Difficile);
    return ;
}

char getcharandflush()
{
	char ch=getchar();
	fflush(stdin);
	return ch;
}

int _1pow(int entier)
{
	return (entier % 2 == 0) ? 1 : -1 ;
}

bool Diag_Valid(int entier,int ligne,int col,int sudoku[N][N])
{
	return (entier==sudoku[ligne+_1pow(ligne)][col+_1pow(col)]) ?  0  :   1 ; //
}

bool Test_creation(int entier,int ligne,int col,int sudoku_init[N][N])
{
	int i;
	bool insertion = 1;/*on supposse qu'on peut inserer l'element*/
		if(sudoku_init[ligne][col] == Vide){ /* si le rand n'a pas remplie la case*/
			for(i=0;i<4 && insertion == 1;i++){
				if( sudoku_init[ligne][i] == entier || sudoku_init[i][col] == entier || !(Diag_Valid(entier,ligne,col,sudoku_init))) /* si on trouve une occurence dans la ligne*/
					insertion = 0;
			}
		}
		else{
			insertion = 0;
		}
	return insertion;
}

void Zero(int sudoku[N][N])
{
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			sudoku[i][j]=0;
	return;
}
void Remplissage(/*difficulte*/){
	// on appelle la fonction Difficulte() pour savoir la valeur de DIFFICULTE ( variable globale )
	int i=0;
	int entier,col,ligne;
	int counter;
	//vidage tu table
	Zero(sudoku_init);
	Zero(sudoku_jeu);
	counter=DIFFICULTE;
	srand(time(0));
	while(i<counter){
		entier=rand()%4+1;
		col=rand()%4;
		ligne=rand()%4;
		if(Test_creation(entier,ligne,col,sudoku_init)){
			sudoku_init[ligne][col]=entier;
			sudoku_jeu[ligne][col]=entier;
			i++;
		}
		}
	return ;
	}

bool Case_Vide(int ligne,int col)
{
	return (sudoku_init[ligne][col] == Vide ) ? 1 : 0 ;
}

bool Insertion_Valid(int entier ,int ligne,int col)
{
	return (( 1 <= entier && entier <=4 && 0 <= ligne && ligne <=3 && 0 <= col && col <=3) ? 1 : 0 );
}

void Insertion(int entier)
{	int ligne,col;
	printf("Saisir La ligne puis la colonne : ");
	scanf("%d %d",&ligne,&col);
	fflush(stdin);
	if(Insertion_Valid(entier,ligne,col) && Case_Vide(ligne,col))
			sudoku_jeu[ligne][col]=entier;
	else{
			printf("Ceci n'est pas valide : ");
			if(0 > entier || entier >4)
				printf("l'entier ");
			if(0 > ligne || ligne >3)
				printf("la ligne ");
			if(0 > col || col >3)
				printf("la colonne ");
			if(!Case_Vide(ligne,col))
				printf("la case est non vide ");
			printf(".");}
	return ;
}

void Affichage(int sudoku[N][N])
{
	int i,j;
	printf("      0-1----2-3 |\n");
	for(i=0;i<4;i++)
	{			if(i==2){
			printf("| |   ------------\n|%d|   ",i);
		}
		else
			printf("|%d|   ",i);
		for(j=0;j<4;j++)
		{
			if(j==2)
				printf("|| ");
			printf("%d ",sudoku[i][j]);
		}
		printf("|\n");
	}
	return ;
}

void Validation()
{
	int sommecol,sommeligne,compteur,i,j;
	compteur = 0;

	for(i=0;i<N;i++)
    {	sommecol=0;
    	sommeligne=0;
    	for(j=0;j<N;j++){
    		sommecol+=sudoku_jeu[j][i];
  			sommeligne+=sudoku_jeu[i][j];}
    if(sommecol!=SommeLC)
    	compteur++;
 	if(sommeligne!=SommeLC)
    	compteur++;
    }

 	if(compteur==0)
 	{
 	printf("Votre Solution est juste .\n");//initialisation de la table jeu et init
   	switch(DIFFICULTE){
   		case Facile:
   			TABLEJOUEUR[JOUEUR_COURANT].score+=Scorefacile;
   		break;
   		case Moyen:
   			TABLEJOUEUR[JOUEUR_COURANT].score+=Scoremoyen;
   		break;
   		case Difficile:
   			TABLEJOUEUR[JOUEUR_COURANT].score+=Scoredifficile;
   		break;
 	}
 	}
  	else{
    	printf("Votre Solution est Fausse .\n");
    	estValide_et_solution(sudoku_init,0);
 		Affichage(sudoku_init);
  	}

	return ;
}

bool absentSurLigne(int k, int sudoku[N][N], int i)
{
    for (int j=0; j < N; j++)
        if (sudoku[i][j] == k)
            return false;
    return true;
}

bool absentSurColonne(int k, int sudoku[N][N], int j)
{
    for (int i=0; i < N; i++)
        if (sudoku[i][j] == k)
            return false;
    return true;
}

bool absentSurBloc(int k, int sudoku[N][N], int i, int j)
{
    int _i = i-(i%2), _j = j-(j%2);  // ou encore : _i = 2*(i/2), _j = 2*(j/2);
    for (i=_i; i < _i+2; i++)
        for (j=_j; j < _j+2; j++)
            if (sudoku[i][j] == k)
                return false;
    return true;
}

bool estValide_et_solution(int sudoku[N][N], int position)
{
    if (position == N*N)
        return true;

    int i = position/N, j = position%N;

    if (sudoku[i][j] != 0)
        return estValide_et_solution(sudoku, position+1);

    for (int k=1; k <= N; k++)
    {
        if (absentSurLigne(k,sudoku,i) && absentSurColonne(k,sudoku,j) && absentSurBloc(k,sudoku,i,j))
        {
            sudoku[i][j] = k;

            if ( estValide_et_solution (sudoku, position+1) )
                return true;
        }
    }
    sudoku[i][j] = 0;

    return false;
}
void Menu_principale()
{
	int choix;
	system("cls");
	printf("--------------------------------------------------------------\n");
	printf("-                                                            -\n");
	printf("-                           SUDOKU                           -\n");
	printf("-                                                            -\n");
	printf("--------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------\n");
	printf("-                                                            -\n");
	printf("-                      1- Se Connecter                       -\n");
	printf("-                                                            -\n");
	printf("-                      2- Creer un compte                    -\n");
	printf("-                                                            -\n");
	printf("-                      3- Voir La Liste Top 10               -\n");
	printf("-                                                            -\n");
	printf("-                      Autre- Quitter Le Jeu                 -\n");
	printf("-                                                            -\n");
	printf("--------------------------------------------------------------\n\n\n");
	printf("-                      Saisir Votre Choix                    -\n");
	printf("                       ");
	choix = getcharandflush()-'0';

	switch(choix){
		case 1:
		Connexion();
		if(JOUEUR_COURANT==-1)
			Menu_principale();
		else
			Menu_compte();
		break;

		case 2:
		Creation_Compte();
		Menu_compte();
		break;

		case 3:
		Affichage_ListTop10();
		system("pause");
		Menu_principale();
		break;
		default:
		Exporter();
		exit(1);
		break;
	}
	return;
}


void Menu_compte()
{
	int choix;
	int recommencer=0;
	system("cls");
	int flag=0;
	printf("--------------------------------------------------------------\n");
	printf("-                                                            -\n");
	printf("-                           SUDOKU                           -\n");
	printf("-                                                            -\n");
	printf("--------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------\n");
	printf("-                                                            -\n");
	printf("-                      1- Jouer                              -\n");
	printf("-                                                            -\n");
	printf("-                      2- Changer le Mot de passe            -\n");
	printf("-                                                            -\n");
	printf("-                      3- Se Deconnecter                     -\n");
	printf("-                                                            -\n");
	printf("--------------------------------------------------------------\n\n\n");
	printf("-                      Saisir Votre Choix                    -\n");
	printf("                       ");
	choix = getcharandflush()-'0';
	system("cls");

	switch(choix)
	{
		case 1:
		Jouer();
		A_JOUEE=1;
		do{
			if(flag)
				{if(recommencer==1){
					Jouer();
					A_JOUEE=1;}
				else
					Menu_compte();}

			system("cls");
			Affichage(sudoku_jeu);
			printf("0 Pour Valider ,1-4 entier a saisir,autre pour retourner au votre Compte : ");
			choix = getcharandflush()-'0';
			switch(choix)
			{
				case 0:
				flag=1;
				VALIDATION = 1;
				Validation();
				printf("Voulez Vous recommencer ? (1) oui (0) sinon : ");
				scanf("%d",&recommencer);
				fflush(stdin);
				break;

				case 1:
				case 2:
				case 3:
				case 4:
				Insertion(choix);
				fflush(stdin);
				break;
			}
		}
		while(choix<Retourner);
		Menu_compte();
		break;
		case 2:
		Changer_Pass();
		Menu_compte();
		break ;
		case 3:
		Deconnexion();
		Menu_principale();
		break;

	}
	return;
}
void Creation_Compte()
{
	joueur Nouveau_Joueur;
	int flag=0;
	do{
		system("cls");
		(flag==0) ? printf("Saisir Votre Pseudo : ") : printf("Ce Pseudo est deja pris : ");
		scanf("%s",Nouveau_Joueur.pseudo);
		fflush(stdin);
		flag++;}
	while(Pseudo_exist(Nouveau_Joueur.pseudo) != -1);

	printf("Saisir votre Nom et Prenom : ");
	scanf("%s",Nouveau_Joueur.nom);
	fflush(stdin);

	printf("Saisir Votre Mot de Passe : ");
	Saisie_de_Mot_de_passe(Nouveau_Joueur.pass);
	fflush(stdin);

	NBRJOUEUR++;
	TABLEJOUEUR = (joueur *)realloc(TABLEJOUEUR, NBRJOUEUR*sizeof(joueur));//

	JOUEUR_COURANT=NBRJOUEUR-1;
	memcpy(TABLEJOUEUR[JOUEUR_COURANT].pseudo,Nouveau_Joueur.pseudo,strlen(Nouveau_Joueur.pseudo)+1);
	memcpy(TABLEJOUEUR[JOUEUR_COURANT].nom,Nouveau_Joueur.nom,strlen(Nouveau_Joueur.nom)+1);
	memcpy(TABLEJOUEUR[JOUEUR_COURANT].pass,Nouveau_Joueur.pass,strlen(Nouveau_Joueur.pass)+1);
	TABLEJOUEUR[JOUEUR_COURANT].score = 0 ;

	//faut initaliser les tables sudoku et non sauvegarde et pas de probleme dans la variable difficulte

	printf("\nVotre Compte est Cree avec succes .");
	delay(4);
	system("cls");
	printf("Connexion...");
	delay(4);// un delai de 4 secondes

	return ;
}

int Pseudo_exist(char pseudo[Pseudo])
{
	int exist =-1;
	int valid = 1;
	for(int i=0;i < NBRJOUEUR && valid;i++)
		if(strcmp(pseudo,TABLEJOUEUR[i].pseudo)==0){
			exist = i;
			valid = 0;}
	return exist;//-1 s'il n'existe pas et son indice s'il existe
}

void Connexion()
{
	char pseudo[Pseudo];
	char pass[Pass];
	int indicejoueur;
	system("cls");
	printf("Saisir Votre Pseudo : ");
	scanf("%s",pseudo);
	fflush(stdin);
	indicejoueur=Pseudo_exist(pseudo);
	if(indicejoueur == -1)
	{
		printf("Ce Pseudo n'existe pas .");
		delay(4);
	}
	else{
		printf("Saisir Votre Mot de Passe : ");
		Saisie_de_Mot_de_passe(pass);
		fflush(stdin);
		if(strcmp(pass,TABLEJOUEUR[indicejoueur].pass) == 0)
		{	JOUEUR_COURANT = indicejoueur;
			printf("\nVotre Mot de Passe est juste .");
			delay(4);
			system("cls");
			printf("Connexion...");
			delay(4);
		}
		else{
			printf("\nLe Mot de Passe saisie est faux .");
			delay(4);
		}
	}
	return ;
}

void Deconnexion(){
	int sauvegarde;
	Maj_liste_Top10();
	if(VALIDATION!=1 && A_JOUEE){//le probleme ici si le joueur se connecte sans joeur donc faut pas afficher cette question
		printf("Voulez-Vous Sauvegarder Votre Partie ? (1) oui (0) sinon .\n");
		scanf("%d",&sauvegarde);
		fflush(stdin);

		if(sauvegarde==1)
		{
			TABLEJOUEUR[JOUEUR_COURANT].sauvegarde=aSauvegarde;
			memcpy(TABLEJOUEUR[JOUEUR_COURANT].sudoku_init,sudoku_init,sizeof(sudoku_init));
			memcpy(TABLEJOUEUR[JOUEUR_COURANT].sudoku_jeu,sudoku_jeu,sizeof(sudoku_jeu));
			TABLEJOUEUR[JOUEUR_COURANT].difficulte=DIFFICULTE;
		}
		else
		{
			TABLEJOUEUR[JOUEUR_COURANT].sauvegarde=nonSauvegarde;
		}
	}
	else
		TABLEJOUEUR[JOUEUR_COURANT].sauvegarde=nonSauvegarde;
		JOUEUR_COURANT = -1;
		A_JOUEE=0;// initialisation des variable
	system("cls");
	printf("Deconnexion...");
	delay(4);
	return ;

	//fonction qui controle l'entree et traite selon les cas
	// le menu recoit une deconnexion de la variable JOUEUR_COURANT qui est repositionnée à -1
}

void Maj_liste_Top10()
{
	int i=0;
	int indice_debut_decalage;
	int score_temp;
	int score_joueur=TABLEJOUEUR[JOUEUR_COURANT].score;

	do{
		if(INDICETOP10[i]==-1)
			score_temp=-1;
		else
			score_temp=TABLEJOUEUR[INDICETOP10[i]].score;
		i++;}
	while( i < 10  && score_joueur < score_temp);
	i--;
	if(i < 10){
		if(Joeur_in_list_Top_10(JOUEUR_COURANT)!= -1)
			indice_debut_decalage = Joeur_in_list_Top_10(JOUEUR_COURANT);
		else
			indice_debut_decalage = 9;
		for(int j=indice_debut_decalage;j > i;j--)
			INDICETOP10[j]=INDICETOP10[j-1];
	}
	INDICETOP10[i]=JOUEUR_COURANT; // avant la deconnexion
	return ;
}

void Affichage_ListTop10()
{
	system("cls");
	printf("--------------------------------------------------------------\n");
	printf("-                                                            -\n");
	printf("-                       ScoreBoard                           -\n");
	printf("-                                                            -\n");
	printf("--------------------------------------------------------------\n\n");
	printf("--------------------------------------------------------------\n");
	printf(" \tRank \t \tPseudo \t \tScore \n");
	for(int i=0;i < 10 && INDICETOP10[i]!=-1;i++)
	{
		printf(" \t%d \t \t%s \t \t%d \n\n",
			i+1,TABLEJOUEUR[INDICETOP10[i]].pseudo,TABLEJOUEUR[INDICETOP10[i]].score);
	}
	printf("--------------------------------------------------------------\n");
	return ;
}
void Changer_Pass()
{
	char pass_1[Pass];
	char pass_2[Pass];
	printf("Saisir Votre Nouveau Mot de Passe : \n");
	Saisie_de_Mot_de_passe(pass_1);
	fflush(stdin);

	if(strcmp(TABLEJOUEUR[JOUEUR_COURANT].pass,pass_1)==0){
		printf("\nVous avez saisi l'ancien Mot de Passe .\n");
		Changer_Pass();}
	else{
		printf("\nConfirmez Votre Nouveau Mot de Passe : \n");
		Saisie_de_Mot_de_passe(pass_2);
		fflush(stdin);
		if(strcmp(pass_1,pass_2)==0){
			strcpy(TABLEJOUEUR[JOUEUR_COURANT].pass,pass_1);
			printf("\nVotre Mot de Passe a ete bien change .\n");
		    delay(4);
		}
		else{
			printf("\nLes deux Mots de passe ne sont pas identiques .\n");
			Changer_Pass();}
		}

	return ;
}

void Jouer()
{
	int continuer;
	VALIDATION = 0;
	if(TABLEJOUEUR[JOUEUR_COURANT].sauvegarde == aSauvegarde)
	{
		TABLEJOUEUR[JOUEUR_COURANT].sauvegarde = nonSauvegarde;
		printf("Voulez-Vous continuer l'ancien jeu (1) ou (0)");
		scanf("%d",&continuer);
		fflush(stdin);
		if(continuer == true){
			memcpy(sudoku_init,TABLEJOUEUR[JOUEUR_COURANT].sudoku_init,sizeof(sudoku_init));//sudoku_init joueur_courant to sudoku_init
			memcpy(sudoku_jeu,TABLEJOUEUR[JOUEUR_COURANT].sudoku_jeu,sizeof(sudoku_jeu));
			DIFFICULTE=TABLEJOUEUR[JOUEUR_COURANT].difficulte;
		}
		else{// le joueur a entre pour jouer
			Difficulte();
			Remplissage();// difficulte a ajouter , avec une variable
		}
	}
	else{
		Difficulte();
		Remplissage();// difficulte a ajouter , avec une variable
	}
	system("cls");
	return ;
}

int main()
{
    Importer();
   Menu_principale();
   return 0;
}

