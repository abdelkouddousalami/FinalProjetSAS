#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define MAX 100

typedef struct {
    int id;
    char clientnom[50];
    char reason[100];
    char description[255];
    char category[50];
    char priorite[20];
    char status[20];
    char date[20];
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

int checkUsernameInPassword(char *username, char *password) {
    if (strstr(password, username) != NULL) {
        printf("le mot de passe ne doit pas contenir le nom d'utilisateur.\n");
        return 0;
    }
    return 1;
}

void signup() {
    char username[20], password[20];

    printf("entrer votre username : ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("entrer votre mot de passe : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (validatePassword(username, password)) {
        printf("Compte cree avec succes\n");
    } else {
        printf("le mot de passe ne respecte pas les requis \n");
    }
}
int signin(char *clientnom) {
    char username[20], password[20];
    int n = 0;

    while (n < 3) {
        printf("Veuillez entrer votre username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
        printf("Veuillez entrer votre mot de passe: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                strcpy(clientnom, username);
                return users[i].isAgent;
            }
        }

        n++;
        printf("\nnom utilisateur ou mot de passe incorrect.\n\n");
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
    if (strstr(reclamation->description, "urgent") != NULL) {
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
        printf("impossible ajouter autres revendications, limite de memoire atteinte.\n");
        return;
    }

    recla newrecla;
    newrecla.id = count + 1;
    strcpy(newrecla.clientnom, clientName);

    printf("veuillez entrer reclamation reason : ");
    fgets(newrecla.reason, sizeof(newrecla.reason), stdin);
    newrecla.reason[strcspn(newrecla.reason, "\n")] = 0;

    printf("veuillez entrer reclamation description : ");
    fgets(newrecla.description, sizeof(newrecla.description), stdin);
    newrecla.description[strcspn(newrecla.description, "\n")] = 0;

    printf("veuillez entrer reclamation category : ");
    fgets(newrecla.category, sizeof(newrecla.category), stdin);
    newrecla.category[strcspn(newrecla.category, "\n")] = 0;

    strcpy(newrecla.status, "en cours");
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

            case 3 : printf("entrer votre username : ");
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
    for (int i = 0; i < count; i++) {
        if (strcmp(claims[i].clientnom, clientName) == 0) {
            printf("reclamation ID: %d\nReason: %s\nDescription: %s\nCategory: %s\nStatut: %s\nDate: %s\n",
                   claims[i].id, claims[i].reason, claims[i].description, claims[i].category, claims[i].status, claims[i].date);
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
    printf("veuillez entrer reclamation ID pour modify: ");
    scanf("%d", &claimID);
    getchar();

    for (int i = 0; i < count; i++) {
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
    printf("reclamation introuvable ou appartenant pas a %s.\n", clientName);
}

void deleteMyrecla(char *clientName) {
    int claimID;
    printf("veuillez entrer reclamation ID pour suprimee: ");
    scanf("%d", &claimID);
    getchar();

    for (int i = 0; i < count; i++) {
        if (claims[i].id == claimID && strcmp(claims[i].clientnom, clientName) == 0) {
            for (int j = i; j < count - 1; j++) {
                claims[j] = claims[j + 1];
            }
            count--;
            printf("reclamation supprimee \n");
            return;
        }
    }
    printf("reclamation introuvable ou n'appartenant pas a %s.\n", clientName);
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

    printf("\n=== reclamations triees par priorite ===\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d\nNom du client: %s\nDescription: %s\nCategorie: %s\nStatut: %s\nDate: %s\n",
               claims[i].id, claims[i].clientnom, claims[i].description, claims[i].category, claims[i].status, claims[i].date);
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
            printf("reclamation status modified succes.\n");
            return;
        }
    }
    printf("reclamation non trouvee\n");
}

void adminMenu() {
    int choice;
    do {
        printf("\n========== Admin Menu ==========\n");
        printf("1. afficher toutes les reclamations\n");
        printf("2. traiter la reclamation\n");
        printf("3. ajouter un nouveau client\n");
        printf("4. ajouter/Supprimer un role d'agent\n");
        printf("5. search par ID ou category ou username ou date\n");
        printf("0. deconnexion\n");
        printf("veuillez entrer votre choice: ");
        scanf("%d", &choice);
        getchar();
        printf("\n*************************\n\n");

        switch (choice) {
            case 1:
                viewAllrecla();
                break;
            case 2:
                processrecla();
                break;
            case 3:
                signup();
                break;
            case 4: {
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
        printf("\n========== Client Menu ==========\n");
        printf("1. ajouter une reclamation\n");
        printf("2. afficher mes reclamations\n");
        printf("3. modifier ma reclamation\n");
        printf("4. supprimer ma reclamation\n");
        printf("0. deconnexion\n");
        printf("veuillez entrer votre choice: ");
        scanf("%d", &choice);
        getchar();
        printf("\n*************************\n\n");

        switch (choice) {
            case 1:
                addrecla(clientName);
                break;
            case 2:
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
        printf("\n========== Agent Menu ==========\n");
        printf("1. Afficher toute les reclamations\n");
        printf("2. Traiter la reclamation\n");
        printf("0. deconnexion\n");
        printf("veuillez entrer votre choice: ");
        scanf("%d", &choice);
        printf("\n*************************\n\n");
        getchar();

        switch (choice) {
            case 1:
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
    printf("veuillez entrer admin username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("veuillez entrer admin mot de pass : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    if (strcmp(username, "abdo") == 0 && strcmp(password, "Abdo@3214") == 0) {
        printf("Connexion admini reussie.\n");
        printf("----------------------------\n");
        return 1;
    } else {
        printf("\nLa connexion de admin a echoue essayez au nouveau \n");
        printf("----------------------------------------------------\n");
        return 0;
    }
}

int main() {

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
                    adminMenu();
                }
                break;
            case 2:
                signup();
                break;
            case 3: {
                char clientName[20];
                int role = signin(clientName);
                if (role == 0) {
                    clientMenu(clientName);
                } else if (role == 1) {
                    agentMenu(clientName);
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
