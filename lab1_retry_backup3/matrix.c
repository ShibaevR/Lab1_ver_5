#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "matrix.h"


void MatrixPrint(Matrix **matrix){

    printf("\n");
    for (int i=0; i < (*matrix)->size; i++){
        printf("   ");
        for (int j=0; j < (*matrix)->size; j++) {
            (*matrix)->type_info->m_print((char*)(*matrix)->data + (i * (*matrix)->size * (*matrix)->type_info->element_size) + j * (*matrix)->type_info->element_size);
        }
        printf("\n");
    }
    printf("\n");
}


void MatrixFind(MatrixCollection *collection, Matrix **matrix, char *name, int *response, size_t *size){

    for(int i=0; i < collection->size; i++){
        if (strcmp (collection->matrices[i].name, name)==0){
            (*response)++;
            *matrix = collection->matrices[i].matrix;
            *size = (*matrix)->size;

            break;
        }
    }
}


void MatrixNameFind(MatrixCollection *collection, char *name, int *response){

    for(int i=0; i < collection->size; i++){
        if (strcmp (collection->matrices[i].name, name)==0){
            (*response)++;
            return;
        }
    }
}


void MatrixCreate(MatrixCollection *collection, Matrix **matrix, char *name, size_t size, FieldInfo *element_type){

    *matrix = (Matrix *) malloc(sizeof(Matrix));
    (*matrix)->data = malloc(size * size * element_type->element_size);

    if(*matrix && (*matrix)->data){
        for (int i=0; i < size; i++){
            for (int j=0; j < size; j++){
                printf("\nEnter element in row %d column %d:", i+1, j+1);
                element_type->m_scan((char*)(*matrix)->data + (i * size * element_type->element_size) + j * element_type->element_size);
            }
        }
    }
    else{
        printf("\nError in MatrixCreate: Couldn't allocate memory space for matrix\n");
        exit(-1);
    }

    (*matrix)->size = size;
    (*matrix)->type_info = element_type;

    collection->matrices = realloc( collection->matrices, ( collection->size + 1) *  sizeof(NamedMatrix));

    if(collection->matrices==NULL){
        printf("\nError in MatrixCreate: Couldn't allocate memory space for collection\n");
        exit(-1);
    }

    collection->matrices[collection->size].matrix = *matrix;
    strncpy( collection->matrices[collection->size].name, name, sizeof(collection->matrices[collection->size].name));
    collection->size++;
}


void MatrixAutoCreate(MatrixCollection *collection, Matrix **matrix, char *name, size_t size, FieldInfo *element_type, void *max_number){

    srand((unsigned int)time(NULL));

    *matrix = (Matrix *) malloc(sizeof(Matrix));
    (*matrix)->data = malloc(size * size * element_type->element_size);

    if(*matrix && (*matrix)->data){
        for (int i=0; i < size; i++){
            for (int j=0; j < size; j++){
                element_type->m_random( (char*)(*matrix)->data + (i * size * element_type->element_size) + j * element_type->element_size, max_number);
            }
        }
    }
    else{
        printf("\nError in MatrixAutoCreate: Couldn't allocate memory space for matrix\n");
        exit(-1);
    }

    (*matrix)->size = size;
    (*matrix)->type_info = element_type;

    collection->matrices = realloc( collection->matrices, ( collection->size + 1) * sizeof(NamedMatrix));

    if(collection->matrices==NULL){
        printf("\nError in MatrixAutoCreate: Couldn't allocate memory space for collection\n");
        exit(-1);
    }

    collection->matrices[collection->size].matrix = *matrix;
    strncpy( collection->matrices[collection->size].name, name, sizeof(collection->matrices[collection->size].name));\
    collection->size++;
}


void MatrixRename(MatrixCollection *collection, char *name_1, char *name_2){

    for(int i=0; i < collection->size; i++){
        if (strcmp( collection->matrices[i].name, name_1)==0){
            strncpy( collection->matrices[i].name, name_2, sizeof(collection->matrices[i].name));
            break;
        }
    }
}


void MatrixAdd(Matrix **m_sum, Matrix **m_1, Matrix **m_2, MatrixCollection *collection, char *name){

    *m_sum = (Matrix *) malloc(sizeof(Matrix));
    (*m_sum)->data = malloc((*m_1)->size * (*m_1)->size * (*m_1)->type_info->element_size);

    if(*m_sum && (*m_sum)->data){

        (*m_sum)->size = (*m_1)->size;
        (*m_sum)->type_info = (*m_1)->type_info;

        for (int i=0; i < (*m_1)->size; i++){
            for (int j=0; j < (*m_1)->size; j++){
                (*m_sum)->type_info->m_addition((char*)(*m_sum)->data + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size,
                                                (char*)(*m_1)->data + + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size,
                                                (char*)(*m_2)->data + + (i * (*m_2)->size * (*m_2)->type_info->element_size) + j * (*m_2)->type_info->element_size);
            }
        }
    }
    else{
        printf("\nError in MatrixAdd: Couldn't allocate memory space for matrix\n");
        exit(-1);
    }

    collection->matrices = realloc( collection->matrices, ( collection->size + 1) * sizeof(NamedMatrix));

    if(collection->matrices==NULL){
        printf("\nError in MatrixAdd: Couldn't allocate memory space for collection\n");
        exit(-1);
    }

    collection->matrices[collection->size].matrix = *m_sum;
    strncpy( collection->matrices[collection->size].name, name, sizeof(collection->matrices[collection->size].name));
    collection->size++;
}


void MatrixMultiply(Matrix **m_product, Matrix **m_1, Matrix **m_2, MatrixCollection *collection, char *name){

    *m_product = (Matrix*)malloc(sizeof(Matrix));
    (*m_product)->data = malloc((*m_1)->size * (*m_1)->size * (*m_1)->type_info->element_size);

    if(*m_product && (*m_product)->data){

        (*m_product)->size = (*m_1)->size;
        (*m_product)->type_info = (*m_1)->type_info;

        for (int i=0; i < (**m_1).size; i++){
            for (int j=0; j < (**m_1).size; j++){
                (*m_product)->type_info->m_multiplication((char*)(*m_product)->data + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size,
                                                          (char*)(*m_1)->data + + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size,
                                                          (char*)(*m_2)->data + + (i * (*m_2)->size * (*m_2)->type_info->element_size) + j * (*m_2)->type_info->element_size);
            }
        }
    }
    else{
        printf("\nError in MatrixMultiply: Couldn't allocate memory space for matrix\n");
        exit(-1);
    }

    collection->matrices = realloc( collection->matrices, ( collection->size + 1) * sizeof(NamedMatrix));

    if(collection->matrices==NULL){
        printf("\nError in MatrixMultiply: Couldn't allocate memory space for collection\n");
        exit(-1);
    }

    collection->matrices[collection->size].matrix = *m_product;
    strncpy( collection->matrices[collection->size].name, name, sizeof(collection->matrices[collection->size].name));
    collection->size++;
}


void MatrixMultiplyByScalar(Matrix **m_product, Matrix **m_1, MatrixCollection *collection, char *name, void *multiplier){

    *m_product = (Matrix*)malloc(sizeof(Matrix));
    (*m_product)->data = malloc((*m_1)->size * (*m_1)->size * (*m_1)->type_info->element_size);

    if(*m_product && (*m_product)->data){

        (*m_product)->size = (*m_1)->size;
        (*m_product)->type_info = (*m_1)->type_info;

        for (int i=0; i < (*m_1)->size; i++){
            for (int j=0; j < (*m_1)->size; j++){
                (*m_product)->type_info->m_multiplication((char*)(*m_product)->data + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size,
                                                          (char*)(*m_1)->data + + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size, multiplier);
            }
        }
    }
    else{
        printf("\nError in MatrixMultiplyByScalar: Couldn't allocate memory space for matrix\n");
        exit(-1);
    }

    collection->matrices = realloc( collection->matrices, ( collection->size + 1) * sizeof(NamedMatrix));

    if(collection->matrices==NULL){
        printf("\nError in MatrixMultiplyByScalar: Couldn't allocate memory space for collection\n");
        exit(-1);
    }

    collection->matrices[collection->size].matrix = *m_product;
    strncpy( collection->matrices[collection->size].name, name, sizeof(collection->matrices[collection->size].name));
    collection->size++;
}


void LinearCombinationInitialize(Matrix **linear_combination, Matrix **matrix){

    *linear_combination = (Matrix*)malloc(sizeof(Matrix));
    (*linear_combination)->data = malloc((*matrix)->size * (*matrix)->size * (*matrix)->type_info->element_size);

    if(*linear_combination && (*linear_combination)->data ){

        (*linear_combination)->size = (*matrix)->size;
        (*linear_combination)->type_info = (*matrix)->type_info;

        for (int i=0; i < (*linear_combination)->size; i++){
            for (int j=0; j < (*linear_combination)->size; j++){
                memset((char*)(*linear_combination)->data + (i * (*linear_combination)->size * (*linear_combination)->type_info->element_size) + j * (*linear_combination)->type_info->element_size,
                       0, (*linear_combination)->type_info->element_size);
            }
        }
    }
}


void MatrixAddLinearCombination(Matrix **m_result, int row_number, Matrix **m_1, MatrixCollection *collection, char *name, Matrix **linear_combination, void *max_number){

    srand((unsigned int)time(NULL));

    *m_result = (Matrix*)malloc(sizeof(Matrix));
    (*m_result)->data = malloc((*m_1)->size * (*m_1)->size * (*m_1)->type_info->element_size);

    if(*m_result && (*m_result)->data){

        (*m_result)->size = (*m_1)->size;
        (*m_result)->type_info = (*m_1)->type_info;

        for (int i=0; i < (*m_1)->size; i++){

            if(i+1 != row_number){

                void *multiplier = malloc( (*m_1)->type_info->element_size);
                if(multiplier == NULL){
                    printf("\nError in MatrixAddLinearCombination: Couldn't allocate memory space for multiplier\n");
                    exit(-1);
                }

                (*m_1)->type_info->m_random(multiplier, max_number);

                for (int j=0; j < (*m_1)->size; j++){

                    void *temporary_result = malloc(  (*m_1)->type_info->element_size);
                    if(temporary_result == NULL){
                        printf("\nError in MatrixAddLinearCombination: Couldn't allocate memory space for temporary_result\n");
                        exit(-1);
                    }

                    (*linear_combination)->type_info->m_multiplication(temporary_result, (char*)(*m_1)->data + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size, multiplier);

                    (*linear_combination)->type_info->m_addition( (char*)(*linear_combination)->data + j * (*linear_combination)->type_info->element_size,
                                                                  (char*)(*linear_combination)->data + j * (*linear_combination)->type_info->element_size, temporary_result);

                    memcpy( (char*)(*m_result)->data + (i * (*m_result)->size * (*m_result)->type_info->element_size) + j * (*m_result)->type_info->element_size,
                            (char*)(*m_1)->data + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size,
                            (*m_result)->type_info->element_size);

                    free(temporary_result);

                }
                free(multiplier);
            }
        }
    }
    else{
        printf("\nError in MatrixAddLinearCombination: Couldn't allocate memory space for matrix\n");
        exit(-1);
    }

    for (int i=0; i < (*m_1)->size; i++){
        for (int j=0; j < (*m_1)->size; j++){
            if(i+1 == row_number){
                (*m_result)->type_info->m_addition((char*)(*m_result)->data + (i * (*m_result)->size * (*m_result)->type_info->element_size) + j * (*m_result)->type_info->element_size,
                                                   (char*)(*m_1)->data + (i * (*m_1)->size * (*m_1)->type_info->element_size) + j * (*m_1)->type_info->element_size,
                                                   (char*)(*linear_combination)->data + j * (*linear_combination)->type_info->element_size);

            }
        }
    }

    collection->matrices = realloc( collection->matrices, ( collection->size + 1) * sizeof(NamedMatrix));

    if(collection->matrices==NULL){
        printf("\nError in MatrixMultiplyByScalar: Couldn't allocate memory space for collection\n");
        exit(-1);
    }

    collection->matrices[collection->size].matrix = *m_result;
    strncpy( collection->matrices[collection->size].name, name, sizeof(collection->matrices[collection->size].name));
    collection->size++;
}


void MatrixFree(Matrix **m){
    (*m)->size = 0;
    free((*m)->data);
    free(*m);
}


void MatrixElementTypeCompare(Matrix **m_1, Matrix **m_2, int *response){
    if(strcmp( (*m_1)->type_info->type_name, (*m_2)->type_info->type_name) != 0){
        (*response)++;
    }
}
