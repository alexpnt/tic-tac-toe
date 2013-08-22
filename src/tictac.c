/**************************************************************/
/****** Projeto de Computacao Grafica - 2012/2013        ******/
/******             -- Jogo do Galo --                   ******/
/****** Elaborado por:                                   ******/
/****** Alexandre Rui Santos Fonseca Pinto  2010131853   ******/
/****** Carlos Miguel Rosa Avim             2000104864   ******/
/**************************************************************/

#include <stdio.h>
#include <string.h>
#include "tictac.h"


int check_game(int grid[4][4]){

	int i,j;

	/*X ganhou?*/
	if(checkH(1,grid) || checkV(1,grid) || checkD(1,grid)){
		return 1;
	}

	/*O ganhou?*/
	if(checkH(2,grid) || checkV(2,grid) || checkD(2,grid)){
		return 2;
	}

	/*O jogo ja acabou?*/
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(grid[i][j]==0){
				return 0;
			}
		}
	}

	/*Empate*/
	return 3;
}


/*verifica se um dado jogador ganhou com uma linha na horizontal*/
int checkH(int player, int grid[4][4]){
	int i,j,counter;

	for(i=0;i<3;i++){
		counter=0;
		for(j=0;j<3;j++){
			if(grid[i][j]==player){
				counter++;
			}
			else
				break;
		}
		if(counter==3){
			return 1;
		}
	}

	return 0;
}

/*verifica se um dado jogador ganhou com uma linha na vertical*/
int checkV(int player,int grid[4][4]){
	int i,j,counter;

	for(j=0;j<3;j++){
		counter=0;
		for(i=0;i<3;i++){
			if(grid[i][j]==player){
				counter++;
			}
			else
				break;
		}
		if(counter==3){
			return 1;
		}
	}

	return 0;
}

/*verifica se um dado jogador ganhou com uma linha na diagonal*/
int checkD(int player,int grid[4][4]){
	int i,j,counter=0;

	j=0;
	for(i=0;i<3;i++){
		if(grid[i][j]==player)
				counter++;
		j++;
	}
	if(counter==3){
		return 1;
	}

	j=2;
	counter=0;
	for(i=0;i<3;i++){
		if(grid[i][j]==player)
				counter++;
		j--;
	}
	if(counter==3){
		return 1;
	}

	return 0;
}
