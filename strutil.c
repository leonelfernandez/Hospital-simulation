#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *substr(const char *str, size_t n){
    if(!str) return "";
    size_t len = strlen(str), k = 0;
    char* cadena = malloc(sizeof(char)*(n+1));
    if(!cadena) return NULL;
    if(len == 0){
        cadena[0] = '\0';
        return cadena;
    }
    for (size_t i = 0; i < len && i < n; i++){
        cadena[i] = str[i];
        k++;
    }
    cadena[k] = '\0';
    return cadena;
}

char **split(const char *str, char sep){
    if(!str) return NULL;
    size_t tamanio = 0,len = strlen(str);
    for (size_t i = 0; i <= len; i++){
        if(str[i] == sep) tamanio ++;
    }
    char** array = malloc(sizeof(char*)*(tamanio+2));
    if(!array) return NULL;
    size_t pos = 0, desde = 0;
    for (size_t i = 0; i <= len; i++){
        if(str[i] == sep || i == len){
            array[pos] = substr(str + desde, (i - desde));
            desde = (i + 1);
            pos ++;
        }
    }
    array[pos] = NULL;
    return array;
}
void concatenar(char* cadena, char** strv){
    for(size_t t = 0; strv[t]; t++){
        char* palabra = strv[t];
        char* p_final;
        if(t != 0){
            p_final = strcpy(p_final, palabra);
        }else{
            p_final = strcpy(cadena, palabra);
        }
    }
}
char *join(char **strv, char sep){
    size_t tamanio = 0;
    size_t cant_sep = 0;
    if(strv){
        for (size_t i = 0; strv[i]; i++){
            tamanio += strlen(strv[i]);
            tamanio ++;
            cant_sep ++;
        }
    }
    if(!tamanio){
        char* cadena = calloc(1, sizeof(char));
        return cadena;
    }
    char* cadena = calloc(tamanio, sizeof(char));
    if(!cadena) return NULL;
    size_t k = 0;
    for (size_t i = 0; strv[i]; i++){
        strcpy(cadena + k, strv[i]);
        k += strlen(strv[i]);
        if(sep != '\0' && i != cant_sep - 1){
            cadena[k] = sep;
            k++;
        } 
    }
    return cadena;
}

void free_strv(char *strv[]){
    for (size_t i = 0; strv[i] != NULL; i++){
        free(strv[i]);
    }
    free(strv);
}