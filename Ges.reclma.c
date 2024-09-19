#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX 100

typedef struct {
    int id;
    char clientName[50];
    char reason[100];
    char description[255];
    char category[50];
    char status[20];
    char date[20];
} Claim;

typedef struct {
    char username[20];
    char password[20];
    int isAgent;
} User;

Claim claims[MAX];
User users[MAX];
int count = 0;
int userCount = 0;


int validatePassword(char *username, char *password) {
    int upper = 0, lower = 0, digit = 0, special = 0;
    for (int i = 0; i < strlen(password); i++) {
        if (isupper(password[i])) upper = 1;
        else if (islower(password[i])) lower = 1;
        else if (isdigit(password[i])) digit = 1;
        else if (ispunct(password[i])) special = 1;
    }
    return (strlen(password) >= 8 && upper && lower && digit && special && !strstr(password, username));
}

void signup() {
    char username[20], password[20];
    printf("veuillez entrer votre username : ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("veuillez entrer votre mot de pass : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    if (validatePassword(username, password)) {
        strcpy(users[userCount].username, username);
        strcpy(users[userCount].password, password);
        users[userCount].isAgent = 0;
        userCount++;
        printf("votre compt a est bien cree\n");
    } else {
        printf("le mot de passe ne respecte pas les requirements .\n");
    }
}

int signin(char *clientName) {
    char username[20], password[20];
    printf("veuillez entrer votre username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("veuillez entrer votre mot de pass : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            strcpy(clientName, username);
            return users[i].isAgent;
        }
    }
    printf("nom d'utilisateur ou mot de passe invalide \n");
    return -1;
}

void addClaim(char *clientName) {
    if (count >= MAX) {
        printf("impossible ajouter autres revendications, limite de memoire atteinte.\n");
        return;
    }

    Claim newClaim;
    newClaim.id = count + 1;
    strcpy(newClaim.clientName, clientName);

    printf("veuillez entrer reclamation reason : ");
    fgets(newClaim.reason, sizeof(newClaim.reason), stdin);
    newClaim.reason[strcspn(newClaim.reason, "\n")] = 0;

    printf("veuillez entrer reclamation description : ");
    fgets(newClaim.description, sizeof(newClaim.description), stdin);
    newClaim.description[strcspn(newClaim.description, "\n")] = 0;

    printf("veuillez entrer reclamation category : ");
    fgets(newClaim.category, sizeof(newClaim.category), stdin);
    newClaim.category[strcspn(newClaim.category, "\n")] = 0;

    strcpy(newClaim.status, "pending");

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(newClaim.date, sizeof(newClaim.date), "%Y-%m-%d", t);

    claims[count++] = newClaim;
    printf("reclamation ajoutee avec succes avec ID: %d\n", newClaim.id);
}

void viewMyClaims(char *clientName) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(claims[i].clientName, clientName) == 0) {
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

void modifyMyClaim(char *clientName) {
    int claimID;
    printf("veuillez entrer reclamation ID pour modify: ");
    scanf("%d", &claimID);
    getchar();

    for (int i = 0; i < count; i++) {
        if (claims[i].id == claimID && strcmp(claims[i].clientName, clientName) == 0) {
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

void deleteMyClaim(char *clientName) {
    int claimID;
    printf("veuillez entrer reclamation ID pour suprimee: ");
    scanf("%d", &claimID);
    getchar();

    for (int i = 0; i < count; i++) {
        if (claims[i].id == claimID && strcmp(claims[i].clientName, clientName) == 0) {
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

void viewAllClaims() {
    if (count == 0) {
        printf("aucune reclamation disponible\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("reclamation ID: %d\nClient Nom: %s\nReason: %s\nDescription: %s\nCategory: %s\nStatus: %s\nDate: %s\n",
               claims[i].id, claims[i].clientName, claims[i].reason, claims[i].description, claims[i].category, claims[i].status, claims[i].date);
        printf("-------------------------\n");
    }
}

void processClaim() {
    int claimID;
    printf("entrez l'ID de la reclamation pour traiter: ");
    scanf("%d",&claimID);
    getchar();

    for (int i = 0; i < count; i++) {
        if (claims[i].id == claimID) {
            printf("statut actuel: %s\n", claims[i].status);
            printf("veuillez entrer nouveau statut (Pending/In Progress/Resolved/Closed): ");
            fgets(claims[i].status, sizeof(claims[i].status), stdin);
            claims[i].status[strcspn(claims[i].status, "\n")] = 0;
            printf("Claim status updated successfully.\n");
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
        printf("0. deconnexion\n");
        printf("veuillez entrer votre choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                viewAllClaims();
                break;
            case 2:
                processClaim();
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
                            printf("role agent attribue à l'utilisateur %s.\n", username);
                        } else {
                            printf("role d'agent supprime de l'utilisateur %s.\n", username);
                        }
                        break;
                    }
                }
                break;
            }
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

        switch (choice) {
            case 1:
                addClaim(clientName);
                break;
            case 2:
                viewMyClaims(clientName);
                break;
            case 3:
                modifyMyClaim(clientName);
                break;
            case 4:
                deleteMyClaim(clientName);
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
        getchar();

        switch (choice) {
            case 1:
                viewAllClaims();
                break;
            case 2:
                processClaim();
                break;
            case 0:
                printf("deconnexion ...\n");
                break;
            default:
                printf("invalid choice. essayer a nouveau\n");
        }
    } while (choice != 0);
}

int signinAsAdmin() {
    char username[20];
    char password[20];
    printf("veuillez entrer admin username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("veuillez entrer admin mot de pass : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    if (strcmp(username, "admin") == 0 && strcmp(password, "Admin@1234") == 0) {
        printf("Connexion admini reussie.\n");
        printf("----------------------------\n");
        return 1;
    } else {
        printf("La connexion de admin a echoue.\n");
        printf("----------------------------\n");
        return 0;
    }
}

int main() {
    int choice;
    do {
        printf("\n============================ MENU ============================\n");
        printf("1. S'inscrire en tant que admin\n");
        printf("2. S'inscrire en tant que client\n");
        printf("3. Connectez-vous en tant que client\n");
        printf("0. Exit\n");
        printf("===============================================================\n");
        printf("Entrez votre choice: ");
        scanf("%d",&choice);
        getchar();

        switch (choice) {
            case 1:
                if (signinAsAdmin()) {
                    adminMenu();
                } else {
                    printf("La connexion de admin a echouc. essayer a nouveau.\n");
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
