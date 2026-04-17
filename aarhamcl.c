#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512

// =====================
// VARIABLE SYSTEM
// =====================

typedef struct Var {
    char name[64];
    char value[256];
    struct Var* next;
} Var;

Var* head = NULL;

void setVar(const char* name, const char* value) {
    Var* v = head;

    while (v != NULL) {
        if (strcmp(v->name, name) == 0) {
            strncpy(v->value, value, 255);
            return;
        }
        v = v->next;
    }

    Var* newVar = (Var*)malloc(sizeof(Var));
    strncpy(newVar->name, name, 63);
    strncpy(newVar->value, value, 255);
    newVar->next = head;
    head = newVar;
}

char* getVar(const char* name) {
    Var* v = head;

    while (v != NULL) {
        if (strcmp(v->name, name) == 0) {
            return v->value;
        }
        v = v->next;
    }

    return NULL;
}

// =====================
// PARSER HELPERS
// =====================

void trimNewline(char* s) {
    s[strcspn(s, "\n")] = 0;
}

// =====================
// COMMAND ENGINE
// =====================

void runLine(char* line) {
    trimNewline(line);

    if (strlen(line) == 0) return;

    // UI
    if (strncmp(line, "text ", 5) == 0) {
        printf("%s\n", line + 6);
        return;
    }

    if (strncmp(line, "button ", 7) == 0) {
        printf("[BUTTON] %s\n", line + 8);
        return;
    }

    if (strncmp(line, "input ", 6) == 0) {
        printf("[INPUT] %s\n", line + 7);
        return;
    }

    if (strncmp(line, "image ", 6) == 0) {
        printf("[IMAGE] %s\n", line + 7);
        return;
    }

    if (strncmp(line, "page ", 5) == 0) {
        printf("\n===== PAGE: %s =====\n", line + 6);
        return;
    }

    // VARIABLES
    if (strncmp(line, "set ", 4) == 0) {
        char name[64];
        char value[256];

        sscanf(line + 4, "%s %[^\n]", name, value);
        setVar(name, value);
        return;
    }

    if (strncmp(line, "show ", 5) == 0) {
        char* val = getVar(line + 5);

        if (val != NULL) {
            printf("%s\n", val);
        } else {
            printf("%s\n", line + 5);
        }
        return;
    }

    // EVENTS
    if (strncmp(line, "on click ", 9) == 0) {
        printf("[EVENT CLICK] %s\n", line + 9);
        return;
    }

    if (strncmp(line, "on hover ", 9) == 0) {
        printf("[EVENT HOVER] %s\n", line + 9);
        return;
    }

    if (strncmp(line, "on input ", 9) == 0) {
        printf("[EVENT INPUT] %s\n", line + 9);
        return;
    }

    if (strncmp(line, "on load", 7) == 0) {
        printf("[EVENT LOAD]\n");
        return;
    }

    // LOGIC
    if (strncmp(line, "if ", 3) == 0) {
        printf("[IF] %s\n", line + 3);
        return;
    }

    if (strncmp(line, "else", 4) == 0) {
        printf("[ELSE]\n");
        return;
    }

    if (strncmp(line, "repeat ", 7) == 0) {
        printf("[REPEAT] %s times\n", line + 7);
        return;
    }

    // MATH
    if (strncmp(line, "add ", 4) == 0) {
        printf("[ADD] %s\n", line + 4);
        return;
    }

    if (strncmp(line, "sub ", 4) == 0) {
        printf("[SUB] %s\n", line + 4);
        return;
    }

    if (strncmp(line, "mul ", 4) == 0) {
        printf("[MUL] %s\n", line + 4);
        return;
    }

    if (strncmp(line, "div ", 4) == 0) {
        printf("[DIV] %s\n", line + 4);
        return;
    }

    // WEB
    if (strncmp(line, "fetch ", 6) == 0) {
        printf("[FETCH] %s\n", line + 6);
        return;
    }

    if (strncmp(line, "link ", 5) == 0) {
        printf("[LINK] %s\n", line + 5);
        return;
    }

    if (strncmp(line, "redirect ", 9) == 0) {
        printf("[REDIRECT] %s\n", line + 9);
        return;
    }

    // DEBUG
    if (strncmp(line, "log ", 4) == 0) {
        printf("[LOG] %s\n", line + 4);
        return;
    }

    printf("[ERROR] Unknown command: %s\n", line);
}

// =====================
// MAIN
// =====================

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: aarhamcl file.acl\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");

    if (!file) {
        printf("Error: cannot open file\n");
        return 1;
    }

    printf("=== AarhamCL v2 STARTED ===\n\n");

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file)) {
        runLine(line);
    }

    fclose(file);

    return 0;
}