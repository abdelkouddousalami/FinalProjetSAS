#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define MAX 100

                                    //admin username : abdo  || admin mot de pass : Abdo@3214

typedef struct {
    int id;
    char clientnom[50];
    char reason[100];
    char description[150];
    char category[50];
    char priorite[20];
    char status[20];
    char date[20];
    time_t delai;

} recla;

typedef struct {
    char username[20];
    char password[20];
    int isAgent;
} User;

recla claims[MAX];
User users[MAX];
int count = 0;
int userCount = 0;
time_t lastrappo =0;
time_t moy =0;

int validatePassword(const char *username, const char *password) {
    if (username == NULL || password == NULL) {
        return 0;
    }

    int len = strlen(password),i;
    if (len < 8) {
        return 0;
    }

    int upper = 0, lower = 0, digit = 0, special = 0;

    for ( i = 0; i < len; i++) {
        if (isupper(password[i])) {
            upper = 1;
        } else if (islower(password[i])) {
            lower = 1;
        } else if (isdigit(password[i])) {
            digit = 1;
        } else if (ispunct(password[i])) {
            special = 1;
        }
    }
    if (strstr(password, username) != NULL) {
        return 0;
    }

    return upper && lower && digit && special;
}

void signup() {
    char username[20], password[20];
    printf("Veuillez entrer votre nom d'utilisateur : ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    printf("Veuillez entrer votre mot de passe : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (validatePassword(username, password)) {
        strcpy(users[userCount].username, username);
        strcpy(users[userCount].password, password);
        users[userCount].isAgent = '\0';
        userCount++;
        printf("Votre compte a ete bien cree\n");
    } else {
        printf("Le mot de passe ne respecte pas les requirements .\n");
    }
}

int signin(char *clientName) {
    char username[20], password[20];
    int n = 0;

    while (n < 3) {
        printf("Veuillez entrer votre nom d'utilisateure : ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
        printf("Veuillez entrer votre mot de passe : ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                strcpy(clientName, username);
                return users[i].isAgent;
            }
        }

        n++;
        printf("\nNom d'utilisateur ou mot de passe incorrect\n\n");
        if(n==3)
        printf("\t\n--------------------------------------------------\n");

        if (n == 3) {
            printf("\nattendez 10 secondes ...\n");
            Sleep(10000);
        }
    }

    return -1;

}

    void Priorite(recla *reclamation) {
    if (strstr(reclamation->description, "urgent") != NULL)  {
        strcpy(reclamation->priorite, "haute");
        strcpy(reclamation->status, "en cours");
    } else if (strstr(reclamation->description, "rapide") != NULL) {
        strcpy(reclamation->priorite, "moyenne");
        strcpy(reclamation->status, "en cours");
    } else {
        strcpy(reclamation->priorite, "basse");
        strcpy(reclamation->status, "en cours");
    }
}


void addrecla(char *clientName) {
    if (count >= MAX) {
        printf("Impossible ajouter d'autres revendications, limite de memoire atteinte.\n");
        return;
    }

    recla newrecla;
    newrecla.id = count + 1;
    strcpy(newrecla.clientnom, clientName);

    printf("veuillez entrer reclamation motif : ");
    fgets(newrecla.reason, sizeof(newrecla.reason), stdin);
    newrecla.reason[strcspn(newrecla.reason, "\n")] = 0;

    printf("veuillez entrer reclamation description : ");
    fgets(newrecla.description, sizeof(newrecla.description), stdin);
    newrecla.description[strcspn(newrecla.description, "\n")] = 0;

    printf("veuillez entrer reclamation category : ");
    fgets(newrecla.category, sizeof(newrecla.category), stdin);
    newrecla.category[strcspn(newrecla.category, "\n")] = 0;

    strcpy(newrecla.status, "en cours");
    time(&moy);
    Priorite(&newrecla);
     time_t now = time(NULL);
     struct tm *t = localtime(&now);
      strftime(newrecla.date, sizeof(newrecla.date), "%Y-%m-%d", t);
      claims[count++] = newrecla;

    printf("reclamation ajoute succes pour ID: %d et priorite : %s\n", newrecla.id, newrecla.priorite);
}


void searchBYidCat() {
    int choice;
    char searchCat[50];
    char username[50];
    char date[50];
    int searchID;
    int found = 0;

    printf("\nrecherche une reclamation\n");
    printf("1 - par ID de reclamation\n");
    printf("2 - par categorie\n");
    printf("3 - par username\n");
    printf("4 - par Date\n");
    printf("veuillez entrer votre choice: ");
    scanf("%d",&choice);
    getchar();

    switch (choice) {
        case 1:

            printf("veuillez entrer ID de reclamation: ");
            scanf("%d",&searchID);
            getchar();
            for (int i = 0; i < count; i++) {
                if (claims[i].id == searchID) {
                    printf("reclamation trouve:\n");
                    printf("ID: %d\nNom du client: %s\nMotif: %s\nDescription: %s\nCategorie: %s\nStatut: %s\nDate: %s\n",
                           claims[i].id, claims[i].clientnom, claims[i].reason, claims[i].description, claims[i].category, claims[i].status, claims[i].date);
                    found = 1;
                    break;
                }
            }
            break;

        case 2:

            printf("veuillez entrer categorie: ");
            fgets(searchCat, sizeof(searchCat), stdin);
            searchCat[strcspn(searchCat, "\n")] = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(claims[i].category, searchCat) == 0) {
                    printf("reclamation trouve:\n");
                    printf("ID: %d\nNom du client: %s\nMotif: %s\nDescription: %s\nCategorie: %s\nStatut: %s\nDate: %s\n",
                           claims[i].id, claims[i].clientnom, claims[i].reason, claims[i].description, claims[i].category, claims[i].status, claims[i].date);
                    found = 1;
                }
            }
            break;

            case 3 : printf("Veuillez entrer votre nom d'utilisateur : ");
                        fgets(username,sizeof(username),stdin);
                        username[strcspn(username,"\n")] = 0;

                    for (int i = 0; i < count; i++) {
                        if (strcmp(claims[i].clientnom, username) == 0) {
                            printf("reclamation trouve:\n");
                            printf("ID: %d, Client: %s, Motif: %s, Description: %s, Categorie: %s, Statut: %s, Date: %s\n",
                            claims[i].id, claims[i].clientnom, claims[i].reason, claims[i].description, claims[i].category, claims[i].status, claims[i].date);
            }
        }
        case  4 :
            printf("entrer la date de la reclamation (format YYYY-MM-DD): ");
            fgets(date, sizeof(date), stdin);
            date[strcspn(date, "\n")] = 0;
                    for (int i = 0; i < count; i++) {
                        if (strcmp(claims[i].date, date) == 0) {
                        printf("reclamation trouvee:\n");
                        printf("ID: %d, Client: %s, Motif: %s, Description: %s, Categorie: %s, Statut: %s, Date: %s\n",
                        claims[i].id, claims[i].clientnom, claims[i].reason, claims[i].description, claims[i].category, claims[i].status, claims[i].date);
            }
        }

        default:
            printf("Choice invalide. Veuillez ressayer.\n");
            return;
    }

    if (!found) {
        printf("aucune reclamation trouve pour le critere specifie.\n");
    }
}

void viewMyrecla(char *clientName) {
    int found = 0;
    printf("\n=============================================\n");
    printf("       RECLAMATIONS DE : %s ***\n",clientName);
    printf("=============================================\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(claims[i].clientnom, clientName) == 0) {
            printf("| Reclamation ID : %d \n", claims[i].id);
            printf("| Reason         : %s \n", claims[i].reason);
            printf("| Description    : %s \n", claims[i].description);
            printf("| Categorie      : %s \n", claims[i].category);
            printf("| Statut         : %s \n", claims[i].status);
            printf("| Date           : %s \n", claims[i].date);
            printf("=============================================\n");
            printf("-------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("aucune reclamation trouvee pour %s.\n", clientName);
    }
}

void modifyMyrecla(char *clientName) {
    int claimID;
    printf("veuillez entrer reclamation ID pour modifer: ");
    scanf("%d",&claimID);
    getchar();


    for (int i = 0; i < count; i++) {
             time_t currtime = time(NULL);
            double diff = (difftime(currtime,(time_t)claims[i].date))/3600.0;

            if(diff<24){
                printf("tu ne peux pas modifer votre reclamtion apres 24h ! votre reclamation a cree a %s ",claims[i].date);
                return ;
            }else{

        if (claims[i].id == claimID && strcmp(claims[i].clientnom, clientName) == 0) {
            printf("veuillez entrer nouveau reason pour reclamation: ");
            fgets(claims[i].reason, sizeof(claims[i].reason), stdin);
            claims[i].reason[strcspn(claims[i].reason, "\n")] = 0;

            printf("veuillez entrer nouvelle description pour reclamation : ");
            fgets(claims[i].description, sizeof(claims[i].description), stdin);
            claims[i].description[strcspn(claims[i].description, "\n")] = 0;

            printf("reclamation modified succes.\n");
            return;
        }
    }
    }
    printf("reclamation introuvable ou appartenant pas a %s.\n", clientName);
}

void deleteMyrecla(char *clientnom) {
    int claimID;
    printf("veuillez entrer reclamation ID pour suprimee: ");
    scanf("%d", &claimID);
    getchar();

    for (int i = 0; i < count; i++) {
            time_t cuurtime = time(NULL);
             double diff = difftime(cuurtime,(time_t)claims[i].date)/3600;
            if(diff<24){
                printf("ne paux pas suprimer votre reclamation apre 24h ! votre reclamtion a cree a : %s",claims[i].date);
                return;
            }else{

        if (claims[i].id == claimID && strcmp(claims[i].clientnom, clientnom) == 0) {
            for (int j = i; j < count - 1; j++) {
                claims[j] = claims[j + 1];
            }
            count--;
            printf("reclamation supprimee \n");
            return;
        }
            }
    }
    printf("reclamation introuvable ou appartenant pas a %s.\n", clientnom);
}
int cate(const char *description) {
    if (strstr(description, "urgent") != NULL) {
        return 1;
    } else if (strstr(description, "rapide") != NULL) {
        return 2;
    } else if (strstr(description, "normale") != NULL) {
        return 3;
    } else {
        return 4;
    }
}

void sortrecla() {
    recla temp;
        for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (cate(claims[i].description) > cate(claims[j].description)) {
                temp = claims[i];
                claims[i] = claims[j];
                claims[j] = temp;
            }
        }
    }

    printf("\n=============================================\n");
    printf("    RECLAMATIONS TRIEES PAR PRIORITE          |\n");
    printf("=============================================\n");
    for (int i = 0; i < count; i++) {
        printf("| ID          : %d  \n", claims[i].id);
        printf("| Nom du client: %s \n", claims[i].clientnom);
        printf("| Description  : %s \n", claims[i].description);
        printf("| Categorie    : %s \n", claims[i].category);
        printf("| Statut       : %s \n", claims[i].status);
        printf("| Date         : %s \n", claims[i].date);
        printf("=============================================\n");
        printf("-------------------------\n");
    }
}


void viewAllrecla() {
    if (count == 0) {
        printf("aucune reclamation disponible\n");
        return;
    }
    sortrecla();

}

void processrecla() {
    int claimID;
    printf("entrez l'ID de la reclamation pour traiter: ");
    scanf("%d",&claimID);
    getchar();

    for (int i = 0; i < count; i++) {
        if (claims[i].id == claimID) {
            printf("statut actuel: %s\n", claims[i].status);
            printf("veuillez entrer nouveau statut (en cours/resolue/fermee): ");
            fgets(claims[i].status, sizeof(claims[i].status), stdin);
            claims[i].status[strcspn(claims[i].status, "\n")] = 0;

            if (strcmp(claims[i].status, "resolue") == 0) {
             claims[i].delai = time(NULL);
           }

            printf("reclamation status modified succes.\n");
            return;
        }
    }
    printf("reclamation non trouvee\n");
}
void taux() {
    int resolvedCount = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(claims[i].status, "resolue") == 0) {
            resolvedCount++;
        }
    }
    if (count > 0) {
        float rate = ((float)resolvedCount / count) * 100;
        printf("taux de resolution: %.2f%%\n",rate);
    } else {
        printf("aucune reclamation soumise.\n");
    }
}
void delai() {


    double total = 0;
    int resolvedClaimsCount = 0;
    double secondsDifference;
    for (int i = 0; i < count; i++) {
        if (strcmp(claims[i].status, "resolue") == 0) {
            struct tm tm = {0};
            sscanf(claims[i].date, "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;
            time_t dateClaimed = mktime(&tm);

            secondsDifference = difftime(claims[i].delai,moy);
            total += secondsDifference;
            resolvedClaimsCount++;
        }
    }

    if (resolvedClaimsCount > 0) {
        double averageTime = secondsDifference / resolvedClaimsCount;
        printf("Le delai moyen de traitement des reclamations resolues est: %.2f secondes.\n", averageTime);
    } else {
        printf("Aucune reclamation resolue pour calculer le delai moyen.\n");
    }
}
void rapport() {
                printf("\n=========================== taux =============================\n");
                printf("\n");
                taux();
                printf("\n======================= delai moyenne =======================\n\n");
                delai();
                printf("\n\n================================================================\n\n");
}

 void generateDailyReport() {
    time_t currentTime;
    time(&currentTime);

    if (difftime(currentTime, lastrappo) >= 86400) {           //86400 seconde = 24h
        FILE *reportFile = fopen("daily_report.txt", "w");
        if (reportFile == NULL) {
            printf("error.\n");
            return;
        }

        fprintf(reportFile, "==================Daily Report %s=====================\n", ctime(&currentTime));
        fprintf(reportFile, "----------------------------------\n");
        fprintf(reportFile, "New reclamation:\n");

        for (int i = 0; i < count; i++) {
            if (strcmp(claims[i].status, "resolue") == 0) {
                fprintf(reportFile, "Reclamation ID : %d\nClient : %s\nMotif : %s\nDescription: %s\nDate : %s\n\n",
                        claims[i].id, claims[i].clientnom, claims[i].reason, claims[i].description, claims[i].date);
            }
        }

        fprintf(reportFile, "----------------------------------\n");
        fprintf(reportFile, "reclamation resolue:\n");

        for (int i = 0; i < count; i++) {
            if (strcmp(claims[i].status, "resolue") == 0) {
                fprintf(reportFile, "Reclamation ID : %d\nClient : %s\nMotif : %s\nDescription : %s\nDate : %s\n\n",
                        claims[i].id, claims[i].clientnom, claims[i].reason, claims[i].description, claims[i].date);
            }
        }

        fclose(reportFile);
        printf("Daily report generated.\n");

        lastrappo = currentTime;
    } else {
        printf("Ce nest pas encore lheure du rapport , rapport dernier jour a ete generated\n");
    }
}

void adminMenu() {
    int choice;
    do {
        printf("\n==================== Admin Menu ====================\n");
        printf("1 - afficher toutes les reclamations\n");
        printf("2 - traiter la reclamation\n");
        printf("3 - ajouter un nouveau client\n");
        printf("4 - ajouter/Supprimer un role d'agent\n");
        printf("5 - search par ID ou category ou username ou date\n");
        printf("6 - generer une rapport\n");
        printf("0. deconnexion\n");
        printf("=======================================================\n");
        printf("veuillez entrer votre choice: ");
        scanf("%d", &choice);
        getchar();
        printf("\n*************************\n\n");

        switch (choice) {
            case 1:
                system("@cls||clear");
                printf("\n");
                viewAllrecla();
                break;
            case 2:
                system("@cls||clear");
                printf("\n");
                processrecla();
                break;
            case 3:
                system("@cls||clear");
                printf("\n");
                signup();
                break;
            case 4: {
                system("@cls||clear");
                char username[20];
                printf("saisissez le nom d'utilisateur pour attribuer/supprimer le role d'agent : ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                for (int i = 0; i < userCount; i++) {
                    if (strcmp(users[i].username, username) == 0) {
                        users[i].isAgent = !users[i].isAgent;
                        if (users[i].isAgent) {
                            printf("role agent attribue a l'utilisateur %s.\n", username);
                        } else {
                            printf("role d'agent supprime de l'utilisateur %s.\n", username);
                        }
                        break;
                    }
                }
                break;
            }
            case 5 :
                searchBYidCat();
                break;
            case 6 :
                rapport();
                generateDailyReport();
                break;
            case 0:
                printf("deconnexion comme admin...\n");
                break;
            default:
                printf("invalid choice. essayer a nouveau\n");
        }
    } while (choice != 0);
}

void clientMenu(char *clientName) {
    int choice;
    do {
        printf("\n==================== Client Menu ====================\n");
        printf("1. ajouter une reclamation\n");
        printf("2. afficher mes reclamations\n");
        printf("3. modifier ma reclamation\n");
        printf("4. supprimer ma reclamation\n");
        printf("0. deconnexion\n");
        printf("=======================================================\n");
        printf("\nveuillez entrer votre choice : ");
        scanf("%d",&choice);
        getchar();
        printf("\n*************************\n\n");

        switch (choice) {
            case 1:
                addrecla(clientName);
                break;
            case 2:
                system("@cls||clear");
                viewMyrecla(clientName);
                break;
            case 3:
                modifyMyrecla(clientName);
                break;
            case 4:
                deleteMyrecla(clientName);
                break;
            case 0:
                printf("deconnexion ...\n");
                break;
            default:
                printf("invalid choice. essayer a nouveau\n");
        }
    } while (choice != 0);
}

void agentMenu(char *agentName) {
    int choice;
    do {
        printf("\n==================== Agent Menu ====================\n");
        printf("1. Afficher toute les reclamations\n");
        printf("2. Traiter la reclamation\n");
        printf("0. deconnexion\n");
        printf("======================================================\n");
        printf("veuillez entrer votre choice: ");
        scanf("%d",&choice);
        printf("\n*************************\n\n");
        getchar();

        switch (choice) {
            case 1:
                system("@cls||clear");
                viewAllrecla();
                break;
            case 2:
                processrecla();
                break;
            case 0:
                printf("deconnexion ...\n");
                break;
            default:
                printf("invalid choice. essayer a nouveau\n");
        }
    } while (choice != 0);
}

int signinAdmin() {
    char username[20];
    char password[20];
    int n =0;
    while(n<3){
    printf("veillez entrer admin username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("veuillez entrer admin mot de pass : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    if (strcmp(username, "abdo") == 0 && strcmp(password, "Abdo@3214") == 0) {
        printf("Connexion comme admin reussie.\n");
        printf("----------------------------\n");
        return 1;
    } else {
        printf("\nla connexion de admin a echoue essayez au nouveau \n");
        n++;
        printf("----------------------------------------------------\n");

    }
    if (n == 3) {
            printf("\nattendez 10 secondes ...\n");
            Sleep(10000);
        }

    }
    return 0;

}

int main() {
    time(&lastrappo);

    int choice;
    do {
        printf("\n============================ MENU ============================\n");
        printf("-     1 - Connectez vous en tant que admin                   -\n");
        printf("-     2 - S'inscrire en tant que client                      -\n");
        printf("-     3 - Connectez vous en tant que client                  -\n");
        printf("-     4 - Exit                                               -\n");
        printf("==============================================================\n");
        printf("Entrez votre choice: ");
        scanf("%d",&choice);
        getchar();
        printf("\n*************************\n\n");

        switch (choice) {
            case 1:
                if (signinAdmin()) {
                        system("@cls||clear");
                        adminMenu();
                }
                break;
            case 2:
                signup();
                break;
            case 3: {
                char clientnom[20];
                int role = signin(clientnom);
                if (role == 0) {
                    system("@cls||clear");
                    clientMenu(clientnom);
                } else if (role == 1) {
                    system("@cls||clear");
                    agentMenu(clientnom);
                }
                break;
            }
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("invalid choice. essayer a nouveau.\n");
        }
    } while (choice != 0);

    return 0;
}
