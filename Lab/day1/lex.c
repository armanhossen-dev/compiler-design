#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX 100

// List of keywords
char *keywords[] = {"int", "return", "include", "printf"};
int num_keywords = 4;

// Check if a string is a keyword
int isKeyword(char str[]) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Main Lexical Analyzer
void lexicalAnalyzer(FILE *fp) {
    char c;
    char token[MAX];
    int k = 0;

    while ((c = fgetc(fp)) != EOF) {
        // Ignore whitespace
        if (isspace(c)) continue;

        //comment detection
        else if(c=='/'){
                k=0;
                token[k++]=c;
                c=fgetc(fp);

                if(c=='/'){
                //single line
                    token[k++]=c;
                    c=fgetc(fp);
                    while(c!='\n' && k<MAX-1){
                    token[k++]=c;
                    c=fgetc(fp);
                    }
                    token[k]='\0';
                    printf("Token: %-20s \tType:Single Line COMMENT\n", token);

                 }
                 //multi comment detection
                 else if( c=='*'){
                        token[k++]=c;
                        c=fgetc(fp);
                        while(k<MAX-1){
                        token[k++]=c;
                        c=fgetc(fp);
                            if(c=='*'){
                                token[k++]=c;
                                c=fgetc(fp);
                                if(c=='/'){
                                    token[k++]=c;
                                    token[k] = '\0';
                                    break;
                                }
                            }
                        }
                        printf("Token: %-20s \t\t\t\tType:Multi line COMMENT\n", token);
                    }

        }
        // Identifiers or keywords
        else if (isalpha(c) || c == '_') {
            k = 0;
            token[k++] = c;
            while ((isalnum(c = fgetc(fp)) || c == '_') && k < MAX - 1) {
                token[k++] = c;
            }
            token[k] = '\0';
            if (c != EOF) ungetc(c, fp); //bujini

            if (isKeyword(token))
                printf("Token: %-20s \tType: KEYWORD\n", token);
            else
                printf("Token: %-20s \tType: IDENTIFIER\n", token);
        }

        // Numbers
        else if (isdigit(c)) {
            k = 0;
            token[k++] = c;
            while (isdigit(c = fgetc(fp)) && k < MAX - 1) {
                token[k++] = c;
            }
            token[k] = '\0';
            if (c != EOF) ungetc(c, fp);

            printf("Token: %-20s \tType: NUMBER\n", token);
        }

        // String literals
        else if (c == '"') {
            k = 0;
            token[k++] = '"'; // store starting quote
            while ((c = fgetc(fp)) != '"' && c != EOF && k < MAX - 1) {
                token[k++] = c;
            }
            token[k++] = '"'; // store ending quote
            token[k] = '\0';

            printf("Token: %-20s \tType: STRING_LITERAL\n", token);
        }

        // Operators         
        else if(c == '+' || c == '/' || c == '-' || c == '*' || c == '%'){
                    token[0] = c;
                    token[1] = '\0';
                    printf("Token: %-20s \tType: OPERATOR\n", token);

        }
        
        
        else if( c == '<'){
             c = fgetc(fp); //s
            k = 0;
            token[k++] = c;
            while ((c = fgetc(fp)) != '>' && c != EOF && k < MAX - 1) {
                if(c == '.'){
                    token[k++] = c;
                    c = fgetc(fp);
                    if(c == 'h'){
                        token[k++] = c;
                        c = fgetc(fp);
                        if(c == '>'){
                            token[k] = '\0';
                            break;
                        }
                    }
                }
                token[k++] = c;
            }
            printf("Token: %-20s \tType: Header File\n", token);
        }
        
        
        else {
            token[0] = c;
            token[1] = '\0';
            printf("Token: %-20s \tType: SPECIAL_SYMBOL\n", token);
        }
    }
}

int main() {
    FILE *fp = fopen("hudai.c", "r");
    if (!fp) {
        printf("Cannot open file\n");
        return 1;
    }

    printf("Lexical Analysis Output:\n\n");
    lexicalAnalyzer(fp);

    fclose(fp);
    return 0;
}