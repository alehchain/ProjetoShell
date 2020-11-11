//Bibliotecas

#include <sys/wait.h>
#include <sys/types.h> // define pid_t
#include <unistd.h>  //fork exec
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*----------------- File: .c -------------------------+
|DESCRICAO DO ARQUIVO
|
|
|
| Implementado por Alexandre Chain  3924	
|                  Ana Clara        4820
|	           Agadir           4887
|
+-----------------------------------------------------+ */



// Funções para executar os comandos SHELL

int shellAjuda(char **argm);
int shellQuit(char **argm);

// possiveis comandos 
char *Comandos_string[] = {
  
  "help",
  "quit"
  
};

int (*FuncComados[]) (char **) = {
  	   	  
	  &shellAjuda,
	  &shellQuit

};



void shell_ls(){

	execlp("/bin/ls","ls",NULL);	
	return;
  	
}


int shellComTam() {

  	return sizeof(Comandos_string) / sizeof(char *);
}


// tipos de comando que o usuario pode usar
int shellAjuda(char **argm){

	 printf("\nBem vindo a Centra de Ajuda MeuShell");
	 printf("\nLISTA DE COMANDOS :");
	 printf("\n ls");
	 printf("\n /bin/ls");
	 printf("\n ls -l");
	 printf("\n cat [arq]");
	 printf("\n quit");
	 printf("\n poweroff");
	    
	  return 1;

}

// Função comando para sair do MeuShell 
int shellQuit(char **argm){

	printf("\nBye Bye !! : Obrigado por utilizar Meu Shell :) \n");	
	return 0; // fim de execução
}



/*A ideia é criar os processos e executar comandos 

que o usuario solicitou.*/


int initMeuShell(char **argm){


  	pid_t pid; // criando um processo filho 

  	int i,status;

  	pid = fork(); //nesse momento o processo abre em processo pai e filho
  
  	if (pid == 0) {
  	
    	
    	//Aqui é o processo filho 
    	if (execvp(argm[0], argm) == -1) {
    
      		printf("\nMeuShell >> Erro: Comando nao é valido!"); // retorna ao usuario que o comando que ele digitou nao é valido
    	}
    		exit(EXIT_FAILURE);
  
  	}else if (pid < 0) {
    
    		// O fork falhou
    		printf("\nFalha no fork! ");
    
  	} else {
  
    	// Aqui é Processo pai 
	    	do {
	    
		      	waitpid(pid, &status, WUNTRACED); // aguardando o processo filho
		      			
	       
	    	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  	}

	// apos a espera do pai..
	// Aqui o Processo filho foi executado com sucesso retornando 1
	
	printf("\nProcesso Finalizado com Sucesso!"); 
		
	return 1;
}



// executa um programa ou incializa o shell
int MeuShell_execute(char **argm){


	  int i;
	  
	  // verifica se o shell deve continuar rodando ou se ele será finalizado 
	  // retornando para função de laço de repetição da shell 

	  if (argm[0] == NULL) {
	    
	    return 1;
	  }

	  for (i = 0; i < shellComTam(); i++) {
	    if (strcmp(argm[0], Comandos_string[i]) == 0) {
	      return (*FuncComados[i])(argm);
	    }
	  }

	  return initMeuShell(argm);	  
}


char *MeuShellLerfrase(void){ 

// ler a frase de entrada 

#ifdef MeuShellGetfrase
  
	  char *frase = NULL;
	  ssize_t TamBuffer = 0; // tam do buffer
	  
	  if (getfrase(&frase, &TamBuffer, stdin) == -1) {
	    if (feof(stdin)) {
	    
	    	printf("\nMeuShell >>Finalizado com sucesso.");
	    	return 1;
	      
	    } else  {
	      printf("\nMeuShell >> Erro!");
	      return 0;
	    }
	  }
	  
	  return frase;
	  
#else
#define MeuShellTamBuffer 100
  
	  int c; // caractere 
	  int pos = 0; // posicao
	  int TamBuffer = MeuShellTamBuffer;


	  char *buffer = malloc(sizeof(char) * TamBuffer);


	  if (!buffer) {
	    fprintf(stderr, "MeuShell >> Falha na alocação!");
	    exit(EXIT_SUCCESS);
	  }

	  while (1) {
	    
	    c = getchar(); // função que pega os caracter

	    if (c == EOF) { 
	      
	      printf("\nFim de caracter lido com sucesso.");
	      exit(EXIT_SUCCESS);
	      
	    } else if (c == '\n') {
	      
	      buffer[pos] = '\0';
	      return buffer;
	    
	    } else {
	      buffer[pos] = c; // recebe na posição o caracter
	    }
	    pos++; //passa para o proximo caracter

	    
	    
	    if (pos >= TamBuffer) { // caso a posicao for maior que o buffer 
	      
	      TamBuffer += MeuShellTamBuffer;
	      buffer = realloc(buffer, TamBuffer); // realoca
	      
	      if (!buffer) {
		
		fprintf(stderr, "MeuShell >> Falha na alocação!"); // retorna uma falha para o usuario e fecha
		exit(EXIT_FAILURE);
	      }
	    }
	  }
#endif
}

#define MeuShellMerTamBuffer 64
#define MeuShellSimbDel " \t\r\n\a"


char **MeuShellDIVfrase(char *frase){


	  int TamBuffer = MeuShellMerTamBuffer, pos = 0;
	  char *Simb, **SalvarSimbs;
	  
	  
	  char **Simbs = malloc(TamBuffer * sizeof(char*));
	  

	  if (!Simbs) {
	    
	    	fprintf(stderr, "MeuShell >> Falha na alocação!");
		exit(EXIT_FAILURE);
	  }

  
	Simb = strtok(frase, MeuShellSimbDel);
 
	while (Simb != NULL) {
	    
	    Simbs[pos] = Simb;
	    pos++;

	    if (pos >= TamBuffer) { // caso a posicao for maior que o buffer 
	    
	      TamBuffer += MeuShellMerTamBuffer;
	      SalvarSimbs = Simbs;
	      Simbs = realloc(Simbs, TamBuffer * sizeof(char*));
	      
	      if (!Simbs) {
		
		free(SalvarSimbs); // libera o que ficou salvo
		fprintf(stderr, "MeuShell >> Falha na alocação!");
		exit(EXIT_FAILURE);
	      }
	    }

	    Simb = strtok(NULL, MeuShellSimbDel);
	  }
	  
	  Simbs[pos] = NULL;
	  return Simbs;
}


 
 
// Laço de repetição para executar o shell 
void shell_rep(void){

	int status;	

  	char *frase;
  	char **argm;
  

  do {
  
    printf("\nMeu--Shell >> ");
    
    frase = MeuShellLerfrase();
    argm = MeuShellDIVfrase(frase);
    status = MeuShell_execute(argm);

    
    // limpa frase e argumentos alocados
    free(frase);
    free(argm);
  
  
  } while (status); // executa ate o fim 

}



int main(int argc, char **argv){ // contador de argumentos e de vetor 

  // executa a função laço de repetição 
  shell_rep();

  
  return 0;
  
}
