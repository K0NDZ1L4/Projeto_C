#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <iup.h>

Ihandle *multitext_dia, *multitext_mes, *multitext_ano, *multitext_quantia, *multitext_categoria,*multitext_categoria2, *multitext_comentario, *multitext_cpf, *multitext_nome, *multitext_senha;
char resultado[9000];



struct gasto{
  int dia;
  int mes;
  int ano;
  int qnt_gasto;
  char tipo_gasto [30];
  char detalhes[100];
};



struct receita{
  int dia;
  int mes;
  int ano;
  int qnt_receita;
  char tipo_receita[30];
  char detalhes[100];
};



struct pessoa{
  char nome[40];
  char cpf[100];
  char senha[100];
};



void criar_lista_pessoas(struct pessoa *lista_pessoas){

	/*************|Cria uma lista com as pessoas do arquivo "Pessoas.txt"|*************/

  FILE* fp = fopen("Pessoas.txt","r");
  int i;
  for(i =0; i < 1000;i++){
    fscanf(fp,"%s",lista_pessoas[i].nome);
    fscanf(fp,"%s",lista_pessoas[i].cpf);
    fscanf(fp,"%s",lista_pessoas[i].senha);
  }
  fclose(fp);
}



void criar_pessoa(Ihandle* self){
  struct pessoa lista_pessoas[1000];  
  criar_lista_pessoas(lista_pessoas);
  
  /*************|Le caixas de texto|*************/
  
  char* cpf = IupGetAttribute(multitext_cpf, "VALUE");
  char* nome = IupGetAttribute(multitext_nome, "VALUE");
  char* senha = IupGetAttribute(multitext_senha, "VALUE");
  
  
  /*************|Verifica conta criada ou nao|*************/
  int i;
  FILE* fp = fopen("Pessoas.txt","a");
  for (i = 0 ; i < 1000  ;i++){
  	if((strcmp( lista_pessoas[i].cpf, cpf )) != 0){
  		IupMessage("Aviso!", "Pessoa criada com sucesso!!");
  		fprintf(fp,"%s    %s    %s\n",nome, cpf, senha);
  		break;
  	}if(strcmp(cpf, lista_pessoas[i].cpf) == 0){
  		IupMessage("Aviso!", "CPF ja utilizado!!");
  		break;
    	
  	};
  }
  fclose(fp);  
}



void criar_receita(Ihandle* self){
  struct pessoa lista_pessoas[1000];  
  
  criar_lista_pessoas(lista_pessoas);
  
  /*************|Le caixas de texto|*************/
  
  char* cpf = IupGetAttribute(multitext_cpf, "VALUE");
  
  int* dia = IupGetInt(multitext_dia, "VALUE");    

  int* mes = IupGetInt(multitext_mes, "VALUE");    

  int* ano = IupGetInt(multitext_ano, "VALUE");

  int* quantia = IupGetInt(multitext_quantia, "VALUE");

  char* categoria = IupGetAttribute(multitext_categoria, "VALUE");

  char* comentario = IupGetAttribute(multitext_comentario, "VALUE");
	
	/*************|Troca espaco por underline|*************/
	
  int i;
  for (i=0;i<1000;i++){
    if(strcmp(lista_pessoas[i].cpf, cpf)==0){      
      for(i=0;i<100;i++){
      	if(comentario[i]==' '){
      		comentario[i]='_';
		}else{
			continue;
		}
	}
	
	/*************|Verifica receita criada ou nao|*************/
	
      FILE* file =fopen(strcat(cpf,"_receita.txt"), "a");
      fprintf(file,"%d %d %d		%d		%s		%s\n",dia, mes, ano, quantia, categoria, comentario);      
      fclose(file);
      IupMessage("Aviso!", "Receita criada com sucesso!!");
      break;
      
    }else{
  		IupMessage("Aviso!", "CPF invalido!!");
  		break;
  	}
    
  }
}



void criar_gasto(Ihandle* self){
  struct pessoa lista_pessoas[1000];  
  criar_lista_pessoas(lista_pessoas);
  
  /*************|Le caixas de texto|*************/
  
  char* cpf = IupGetAttribute(multitext_cpf, "VALUE");
  
  char* senha = IupGetAttribute(multitext_senha, "VALUE");
  
  int* dia = IupGetInt(multitext_dia, "VALUE");    

  int* mes = IupGetInt(multitext_mes, "VALUE");    

  int* ano = IupGetInt(multitext_ano, "VALUE");

  int* quantia = IupGetInt(multitext_quantia, "VALUE");

  char* categoria = IupGetAttribute(multitext_categoria, "VALUE");

  char* comentario = IupGetAttribute(multitext_comentario, "VALUE");
  
  /*************|Troca espaco por underline|*************/
  
  
  int i;
  for (i=0;i<1000;i++){
    if(strcmp(lista_pessoas[i].cpf, cpf)==0){
      if(strcmp(lista_pessoas[i].senha, senha)==0){
      	for(i=0;i<100;i++){
      		if(comentario[i]==' '){
      			comentario[i]='_';
			  }else{
			  	continue;
			  }
		  }
		  
		  /*************|Verifica gasto criado ou nao|*************/
		  
      	FILE* file =fopen(strcat(cpf,"_receita.txt"),"a");
      	fprintf(file,"%d %d %d		-%d		%s		%s\n",dia, mes, ano, quantia, categoria, comentario);
      	fclose(file);
      	IupMessage("Aviso!", "Gasto criado com sucesso!!");
		break;
	  }
    }else{
    	IupMessage("Aviso!", "CPF ou Senha invalidos!!");
    	break;
	}
  }
}



void criar_lista_receita(struct receita *lista_receitas,char CPF[100]){
	
	/*************|Cria lista com todas as receitas/gastos do arquivo "CPF_receita.txt"|*************/
	
	char x[100];
	strcpy(x, CPF);
	FILE* fp = fopen(strcat(x,"_receita.txt"),"r");
    int i;
	  for(i =0; i<1000;i++){
	  	fscanf(fp,"%d %d %d %d %s %s",&lista_receitas[i].dia,&lista_receitas[i].mes,&lista_receitas[i].ano,&lista_receitas[i].qnt_receita,lista_receitas[i].tipo_receita,lista_receitas[i].detalhes);
	}
	fclose(fp);
}



void filtrar_por_categoria(Ihandle* self){
	
	struct pessoa lista_pessoas[1000];
	criar_lista_pessoas(lista_pessoas);
	
	struct receita receita_pesquisada;
	struct receita lista_receita[1000];
	
	/*************|Le caixas de texto|*************/
	
	char* cpf = IupGetAttribute(multitext_cpf, "VALUE");
	char* categoria = IupGetAttribute(multitext_categoria, "VALUE");
	
	/*************|Verifica as categorias|*************/
	
	criar_lista_receita(lista_receita, cpf);
	FILE* file= fopen("lista.txt","w");
	int i;
	for(i=0;i<1000;i++){
		if(strcmp(categoria, lista_receita[i].tipo_receita)==0){
			fprintf(file,"%d_%d_%d________%d________%s________%s!",lista_receita[i].dia,lista_receita[i].mes,lista_receita[i].ano,lista_receita[i].qnt_receita,lista_receita[i].tipo_receita,lista_receita[i].detalhes);
		}
	}
	fclose(file);
	
	/*************|Cria um arquivo com todas as receitas/gastos da categoria solicitada|*************/
	
	FILE*fp = fopen("lista.txt","r");
	fscanf(fp,"%s",resultado);
	for(i = 0;i<9000;i++){
		if(resultado[i]=='_'){
			resultado[i]=' ';
		}
		if(resultado[i]=='!'){
			resultado[i]='\n';
		}
	};
	fclose(fp);
	
	/*************|Verifica cpf ou categoria validos|*************/
	
	for(i=0;i<1000;i++){
		printf(lista_pessoas[i].cpf);
		if(strcmp(lista_pessoas[i].cpf, cpf)==0){
			exibeHistorico();
			break;
		}else{
			IupMessage("Aviso!", "CPF ou Categoria invalidos!!!");
			i++;
		}
	}
	
}



void filtrar_mes_selecionado(Ihandle* self){
	struct pessoa lista_pessoas[1000];
	criar_lista_pessoas(lista_pessoas);
	
	struct receita receita_pesquisada;
	struct receita lista_receita[1000];
	
	/*************|Le caixas de texto|*************/
	
	char* cpf = IupGetAttribute(multitext_cpf, "VALUE");
	int* mes = IupGetInt(multitext_mes, "VALUE");
	int* ano = IupGetInt(multitext_ano, "VALUE");
	
	/*************|Verifica o mes|*************/
	
	criar_lista_receita(lista_receita, cpf);
	FILE* file= fopen("lista.txt","w");
	int i;
	for(i=0;i<1000;i++){
		if(ano == lista_receita[i].ano && mes == lista_receita[i].mes){
			fprintf(file,"%d_%d_%d________%d________%s________%s!",lista_receita[i].dia,lista_receita[i].mes,lista_receita[i].ano,lista_receita[i].qnt_receita,lista_receita[i].tipo_receita,lista_receita[i].detalhes);
		}
	}
	
	fclose(file);
	
	/*************|Cria um arquivo com todas as receitas/gastos da categoria solicitada|*************/
	
	FILE*fp = fopen("lista.txt","r");
	fscanf(fp,"%s",resultado);
	for(i = 0;i<9000;i++){
		if(resultado[i]=='_'){
			resultado[i]=' ';
		}
		if(resultado[i]=='!'){
			resultado[i]='\n';
		}
	};
	fclose(fp);
	
	/*************|Verifica cpf ou categoria validos|*************/
	

	for(i=0;i<1000;i++){
		if(strcmp(lista_pessoas[i].cpf, cpf)==0){
			exibeHistorico();
			break;
		}else{
			IupMessage("Aviso!", "CPF ou Categoria invalidos!!!");
			break;
		}
	}
}



void filtrar_dia_selecionado(Ihandle* self){
	struct pessoa lista_pessoas[1000];
	criar_lista_pessoas(lista_pessoas);
	
	struct receita receita_pesquisada;
	struct receita lista_receita[1000];
	
	/*************|Le caixas de texto|*************/
	
	
	char* cpf = IupGetAttribute(multitext_cpf, "VALUE");
	int* dia = IupGetInt(multitext_dia, "VALUE");
	int* mes = IupGetInt(multitext_mes, "VALUE");
	int* ano = IupGetInt(multitext_ano, "VALUE");
	
	/*************|Verifica o dia|*************/
	
	criar_lista_receita(lista_receita, cpf);
	FILE* file= fopen("lista.txt","w");
	int i;
	for(i=0;i<1000;i++){
		if(ano == lista_receita[i].ano && mes == lista_receita[i].mes && dia==lista_receita[i].dia){
			fprintf(file,"%d_%d_%d________%d________%s________%s!",lista_receita[i].dia,lista_receita[i].mes,lista_receita[i].ano,lista_receita[i].qnt_receita,lista_receita[i].tipo_receita,lista_receita[i].detalhes);
		}
	}
	fclose(file);
	
	/*************|Cria um arquivo com todas as receitas/gastos da categoria solicitada|*************/
	
	FILE*fp = fopen("lista.txt","r");
	fscanf(fp,"%s",resultado);
	for(i = 0;i<9000;i++){
		if(resultado[i]=='_'){
			resultado[i]=' ';
		}
		if(resultado[i]=='!'){
			resultado[i]='\n';
		}
	};
	fclose(fp);

	/*************|Verifica cpf ou categoria validos|*************/

	for(i=0;i<1000;i++){
		if(strcmp(lista_pessoas[i].cpf, cpf)==0){
			exibeHistorico();
			break;
		}else{
			IupMessage("Aviso!", "CPF ou Categoria invalidos!!!");
			break;
		}
	}
}



void filtrar_mesesdoze_selecionado(Ihandle* self){
	struct pessoa lista_pessoas[1000];
	criar_lista_pessoas(lista_pessoas);
	
	struct receita receita_pesquisada;
	struct receita lista_receita[1000];
	
	/*************|Le caixas de texto|*************/
	
	char* cpf = IupGetAttribute(multitext_cpf, "VALUE");
	int* mes = IupGetInt(multitext_mes, "VALUE");
	int* ano = IupGetInt(multitext_ano, "VALUE");
	
	/*************|Verifica os 12 meses anteriores ao informado|*************/
	
	criar_lista_receita(lista_receita, cpf);
	FILE* file= fopen("lista.txt","w");
	int i;
	for(i=0;i<1000;i++){
		if((ano == lista_receita[i].ano) && (mes < lista_receita[i].mes)){
			fprintf(file,"%d/%d/%d________%d________%s________%s!",lista_receita[i].dia,lista_receita[i].mes,lista_receita[i].ano,lista_receita[i].qnt_receita,lista_receita[i].tipo_receita,lista_receita[i].detalhes);
		}else if( ((ano - lista_receita[i].ano) == 1) && ( lista_receita[i].mes > mes) && (( ano - lista_receita[i].ano) > 0) ){
			fprintf(file,"%d/%d/%d________%d________%s________%s!",lista_receita[i].dia,lista_receita[i].mes,lista_receita[i].ano,lista_receita[i].qnt_receita,lista_receita[i].tipo_receita,lista_receita[i].detalhes);
		}
	}
	fclose(file);
	
	/*************|Cria um arquivo com todas as receitas/gastos da categoria solicitada|*************/
	
	FILE*fp = fopen("lista.txt","r");
	fscanf(fp,"%s",resultado);
	for(i = 0;i<9000;i++){
		if(resultado[i]=='_'){
			resultado[i]=' ';
		}
		if(resultado[i]=='!'){
			resultado[i]='\n';
		}
	};
	fclose(fp);

	/*************|Verifica cpf ou categoria validos|*************/

	for(i=0;i<1000;i++){
		if(strcmp(lista_pessoas[i].cpf, cpf)==0){
			exibeHistorico();
			break;
		}else{
			IupMessage("Aviso!", "CPF ou Categoria invalidos!!!");
			break;
		}
	}
}

/****************************INTERFACE*****************************/

int fecharJanela(Ihandle *fechar){
  IupHide(IupGetDialog(fechar));
  return IUP_DEFAULT;
}



int exibeHistorico(Ihandle *historicoCategoria){
  Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(historicoCategoria, "FIND_DIALOG");
  if (!caixaGeral){
    
  Ihandle *label1, *labeldata, *labelgasto, *labelcategoria, *labeldetalhe, *labelhistorico;
  Ihandle *buttonSair;
  Ihandle *vbox;
  
  
  /*************Labels*************/
  
  
  /*****Registro*****/
  label1 = IupLabel("Historico");
  
  
  /*****Data*****/
  labeldata = IupLabel("DD/MM/AA");
  
  
  /*****Gasto/Receita*****/
  labelgasto = IupLabel("Gasto/Receita");
  

  /*****Categoria*****/
  labelcategoria = IupLabel("Categoria");
  
  
  /*****Detalhe*****/
  labeldetalhe = IupLabel("Detalhe");
  
  
  /*****Detalhe*****/
  labelhistorico = IupLabel(resultado);
  
  
  /*************Botoes*************/
  
  
  /*****Sair*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "40");
  
  
  IupSetCallback(buttonSair, "ACTION", (Icallback)fecharJanela);
  
  
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  label1,
  IupHbox(labeldata, labelgasto, labelcategoria, labeldetalhe, NULL),
  labelhistorico,
  IupHbox(buttonSair, NULL),
  NULL);

  IupSetAttribute(vbox, "GAP", "10");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x140"); 
  
  
  
  /*************Loop*************/
  
 }

  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;
}



int janelaCategorias(Ihandle* categoria){
 
  Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(categoria, "FIND_DIALOG");
  if (!caixaGeral){  	
  Ihandle *buttonProcurar, *buttonSair;
  Ihandle *labelcategoria, *labelcpf, *labelcategoria2;
  Ihandle *vbox;
  
  /*************Labels*************/
  
  
  labelcategoria2 = IupLabel("[Moradia] [Trabalho] [Transporte] [Comida] [Estudos]");
  
  labelcpf = IupLabel("CPF:");
  
  labelcategoria = IupLabel("Categoria:");
  
  
  /*************Botoes*************/
    
  
  
  /*****Moradia*****/
  buttonProcurar = IupButton("Procurar", NULL);
  IupSetAttribute(buttonProcurar, "SIZE", "80x20");  
  IupSetCallback(buttonProcurar, "ACTION", (Icallback)filtrar_por_categoria);
  
  
  
  /*****Menu*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "80x20");  
  IupSetCallback(buttonSair, "ACTION", (Icallback)fecharJanela);
  
  
  /*************Multitext*************/
  
  /*****Categoria*****/
  multitext_categoria = IupText(NULL);
  
  IupSetAttribute(multitext_categoria, "SIZE", "95x10");
  
  
  /*****CPF*****/
  multitext_cpf = IupText(NULL);
  
  IupSetAttribute(multitext_cpf, "SIZE", "95x10");
  
  
  
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  IupHbox(labelcpf, multitext_cpf, NULL),
  labelcategoria2,
  IupHbox(labelcategoria, multitext_categoria, NULL),  
  IupHbox(buttonProcurar, buttonSair, NULL),  
  NULL);

  IupSetAttribute(vbox, "GAP", "15");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x130"); 
  
  
  
  /*************Loop*************/
  }
  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;
}



int janelaMesesDoze(Ihandle* meses){
 
  Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(meses, "FIND_DIALOG");
  if (!caixaGeral){  	
  Ihandle *label1, *labelcpf, *labelsenha, *labelano, *labelmes;
  Ihandle *buttonProcurar, *buttonSair;
  Ihandle *vbox;
  
  
  /*************Labels*************/
  
  /*****CPF*****/
  labelcpf = IupLabel("CPF:");
  
  
  /*****Ano*****/
  labelano = IupLabel("Ano(Atual):");
  
  /*****Mes*****/
  labelmes = IupLabel("Mes(Atual):");
  
  
  
  /*************Botoes*************/
    
  
  /*****Exibir*****/
  buttonProcurar = IupButton("Procurar", NULL);
  IupSetAttribute(buttonProcurar, "SIZE", "80x20");
  
  
  IupSetCallback(buttonProcurar, "ACTION", (Icallback) filtrar_mesesdoze_selecionado);
  
  
  /*****Menu*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "80x20");
  
  
  IupSetCallback(buttonSair, "ACTION", (Icallback)fecharJanela);
  
  
  /*************Multitext*************/
  
  
  /*****CPF*****/
  multitext_cpf = IupText(NULL);
  
  IupSetAttribute(multitext_cpf, "SIZE", "60x10");
  
  
  /*****Ano*****/  
  multitext_ano = IupText(NULL);
  
  IupSetAttribute(multitext_ano, "SIZE", "60x10");
  
  
  /*****Mes*****/  
  multitext_mes = IupText(NULL);
  
  IupSetAttribute(multitext_mes, "SIZE", "60x10");
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  label1,
  IupHbox(labelcpf, multitext_cpf, NULL),
  IupHbox(labelmes, multitext_mes, NULL),
  IupHbox(labelano, multitext_ano, NULL),
  IupHbox(IupFill(), buttonProcurar, buttonSair, NULL),
  NULL);

  IupSetAttribute(vbox, "GAP", "15");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x160"); 
  
  
  
  /*************Loop*************/
  }
  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;
}



int janelaMes(Ihandle* mes){
	Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(mes, "FIND_DIALOG");
  if (!caixaGeral){  	
  Ihandle *label1, *labelcpf, *labelmes, *labelano;
  Ihandle *buttonProcurar, *buttonSair;
  Ihandle *vbox;
  
  
  /*************Labels*************/
  
  /*****CPF*****/
  labelcpf = IupLabel("CPF:");
  
  
  /*****Ano*****/
  labelano = IupLabel("Ano:");
  
  /*****Mes*****/
  labelmes = IupLabel("Mes:");
  
  
  
  /*************Botoes*************/
    
  
  /*****Exibir*****/
  buttonProcurar = IupButton("Procurar", NULL);
  IupSetAttribute(buttonProcurar, "SIZE", "80x20");
  
  
  IupSetCallback(buttonProcurar, "ACTION", (Icallback) filtrar_mes_selecionado);
  
  
  /*****Menu*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "80x20");
  
  
  IupSetCallback(buttonSair, "ACTION", (Icallback)fecharJanela);
  
  
  /*************Multitext*************/
  
  
  /*****CPF*****/
  multitext_cpf = IupText(NULL);
  
  IupSetAttribute(multitext_cpf, "SIZE", "60x10");
  
  
  /*****Ano*****/  
  multitext_ano = IupText(NULL);
  
  IupSetAttribute(multitext_ano, "SIZE", "60x10");
  
  
  /*****Mes*****/  
  multitext_mes = IupText(NULL);
  
  IupSetAttribute(multitext_mes, "SIZE", "60x10");
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  label1,
  IupHbox(labelcpf, multitext_cpf, NULL),
  IupHbox(labelmes, multitext_mes, NULL),
  IupHbox(labelano, multitext_ano, NULL),
  IupHbox(IupFill(), buttonProcurar, buttonSair, NULL),
  NULL);

  IupSetAttribute(vbox, "GAP", "15");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x160"); 
  
  
  
  /*************Loop*************/
  }
  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;
}



int janelaDia(Ihandle* dia){
	Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(dia, "FIND_DIALOG");
  if (!caixaGeral){  	
  Ihandle *label1, *labelcpf, *labelmes, *labelano, *labeldia;
  Ihandle *buttonProcurar, *buttonSair;
  Ihandle *vbox;
  
  
  /*************Labels*************/
  
  /*****CPF*****/
  labelcpf = IupLabel("CPF:");
  
  
  /*****Ano*****/
  labelano = IupLabel("Ano:");
  
  /*****Mes*****/
  labelmes = IupLabel("Mes:");
  
  
  /*****Dia*****/
  labeldia = IupLabel("Dia:");
  
  
  
  /*************Botoes*************/
    
  
  /*****Exibir*****/
  buttonProcurar = IupButton("Procurar", NULL);
  IupSetAttribute(buttonProcurar, "SIZE", "80x20");
  
  
  IupSetCallback(buttonProcurar, "ACTION", (Icallback) filtrar_dia_selecionado);
  
  
  /*****Menu*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "80x20");
  
  
  IupSetCallback(buttonSair, "ACTION", (Icallback)fecharJanela);
  
  
  /*************Multitext*************/
  
  
  /*****CPF*****/
  multitext_cpf = IupText(NULL);
  
  IupSetAttribute(multitext_cpf, "SIZE", "60x10");
  
  
  /*****Ano*****/  
  multitext_ano = IupText(NULL);
  
  IupSetAttribute(multitext_ano, "SIZE", "60x10");
  
  
  /*****Mes*****/  
  multitext_mes = IupText(NULL);
  
  IupSetAttribute(multitext_mes, "SIZE", "60x10");
  
  
  
  /*****Dia*****/  
  multitext_dia = IupText(NULL);
  
  IupSetAttribute(multitext_dia, "SIZE", "60x10");
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  label1,
  IupHbox(labelcpf, multitext_cpf, NULL),
  IupHbox(labeldia, multitext_dia, NULL),
  IupHbox(labelmes, multitext_mes, NULL),
  IupHbox(labelano, multitext_ano, NULL),
  IupHbox(IupFill(), buttonProcurar, buttonSair, NULL),
  NULL);

  IupSetAttribute(vbox, "GAP", "15");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x200"); 
  
  
  
  /*************Loop*************/
  }
  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;
}



int janelaHistorico(Ihandle *historico){
  
  Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(historico, "FIND_DIALOG");
  if (!caixaGeral){
  
  Ihandle *buttonMeses, *buttonCategoria, *buttonDia, *buttonMes, *buttonSair;
  Ihandle *vbox;
  
  
  
  /*************Botoes*************/
    
  
  /*****12 meses*****/
  buttonMeses = IupButton("Ultimos 12 meses", NULL);
  IupSetAttribute(buttonMeses, "SIZE", "80x20");
  
  IupSetCallback(buttonMeses, "ACTION", (Icallback)janelaMesesDoze);
  
  
  
  /*****Categoria*****/
  buttonCategoria = IupButton("Por categoria", NULL);
  IupSetAttribute(buttonCategoria, "SIZE", "80x20");
  
  IupSetCallback(buttonCategoria, "ACTION", (Icallback)janelaCategorias);
  
  
  /*****Categoria*****/
  buttonDia = IupButton("Por Dia", NULL);
  IupSetAttribute(buttonDia, "SIZE", "80x20");
  
  IupSetCallback(buttonDia, "ACTION", (Icallback)janelaDia);
  
  
  /*****Categoria*****/
  buttonMes = IupButton("Por Mes", NULL);
  IupSetAttribute(buttonMes, "SIZE", "80x20");
  
  IupSetCallback(buttonMes, "ACTION", (Icallback)janelaMes);
  
  
  
  /*****Sair*****/
  buttonSair = IupButton("Fechar", NULL);
  IupSetAttribute(buttonSair, "SIZE", "80x20");
  
  
  
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  IupHbox(buttonMeses, buttonCategoria, NULL),
  IupHbox(buttonDia, buttonMes, NULL),
  buttonSair,
  NULL);

  IupSetAttribute(vbox, "GAP", "15");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
  
  
  
  /*************Callback*************/
  
  
  IupSetCallback(buttonMeses, "ACTION", (Icallback)janelaMesesDoze);
  
  IupSetCallback(buttonSair, "ACTION", (Icallback) fecharJanela);
  
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x150"); 
  
  
  
  /*************Loop*************/
  }
  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;

}



int janelaGasto(Ihandle *gasto){
  Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(gasto, "FIND_DIALOG");
  if (!caixaGeral){  
  Ihandle *label1, *labeldia, *labelmes, *labelano, *labelquantia, *labelcategoria, *labelcategoria2, *labelcomentario, *labelcpf, *labelsenha;
  Ihandle *buttonCriar, *buttonSair;
  Ihandle *vbox;
  
  
  
  /*************Labels*************/
  
  
  /*****Registro*****/
  label1 = IupLabel("Cadastrar Gasto");
  
  
  
  /*****CPF*****/
  labelcpf = IupLabel("CPF:");
  
  
  /*****Senha*****/
  labelsenha = IupLabel("Senha:");  
  
  
  /*****Dia*****/
  labeldia = IupLabel("Dia:");
  
  
  /*****Mes*****/
  labelmes = IupLabel("Mes:");
  
  
  /*****Ano*****/
  labelano = IupLabel("Ano:");
  
  
  /*****Quantia*****/
  labelquantia = IupLabel("Valor (R$) :");
  
  
  /*****Categoria*****/
  labelcategoria = IupLabel("Categoria:");
  
  
  /*****Categoria2*****/
  labelcategoria2 = IupLabel("[Moradia] [Trabalho] [Transporte] [Comida] [Estudos]");
  
  
  /*****Comentario*****/
  labelcomentario = IupLabel("Comentario:");
  
  
  
  
  /*************Botoes*************/
    
  

  /*****Registrar*****/
  buttonCriar = IupButton("Criar", NULL);
  IupSetAttribute(buttonCriar, "SIZE", "40");
  
  IupSetCallback(buttonCriar, "ACTION", (Icallback)criar_gasto);
  
  
  
  /*****Menu*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "40");
  
  IupSetCallback(buttonSair, "ACTION", fecharJanela);
  
  /*************Caixas de Texto*************/
  
  /*****CPF*****/
  multitext_cpf = IupText(NULL);
  
  IupSetAttribute(multitext_cpf, "SIZE", "60x10");
  
  
  /*****Senha*****/
  multitext_senha = IupText(NULL);
  
  IupSetAttribute(multitext_senha, "SIZE", "60x10");
  
  
  /*****Dia*****/
  multitext_dia = IupText(NULL);
  
  IupSetAttribute(multitext_dia, "SIZE", "30x10");
  
  
  /*****Mes*****/
  multitext_mes = IupText(NULL);
  
  IupSetAttribute(multitext_mes, "SIZE", "30x10");  
  
  
  /*****Ano*****/
  multitext_ano = IupText(NULL);
  
  IupSetAttribute(multitext_ano, "SIZE", "30x10");
  
  
  /*****Quantia*****/
  multitext_quantia = IupText(NULL);
  
  IupSetAttribute(multitext_quantia, "SIZE", "60x10");
  
  
  /*****Categoria*****/
  multitext_categoria = IupText(NULL);
  
  IupSetAttribute(multitext_categoria, "SIZE", "95x10");
  
  
  /*****Comentario*****/
  multitext_comentario = IupText(NULL);
  
  IupSetAttribute(multitext_comentario, "SIZE", "215x30");
  
  
  
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  label1,
  IupHbox(labelcpf, multitext_cpf, labelsenha, multitext_senha, NULL),
  IupHbox(labeldia, multitext_dia, labelmes, multitext_mes, labelano, multitext_ano, NULL),
  IupHbox(labelquantia, multitext_quantia, NULL),
  IupHbox(labelcategoria, multitext_categoria,labelcategoria2, NULL),
  IupHbox(labelcomentario, multitext_comentario, NULL),
  IupHbox(IupFill(), buttonCriar, buttonSair, NULL),
  NULL);

  IupSetAttribute(vbox, "GAP", "10");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  
  
  
  /*************Callback*************/
  
  
  
  
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "390x220"); 
  
  
  
  /*************Loop*************/
  
  
  }
  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;
}



int janelaReceita(Ihandle *receita){
	
  Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(receita, "FIND_DIALOG");
  if (!caixaGeral){
  
  Ihandle *label1, *labeldia, *labelmes, *labelano, *labelquantia, *labelcategoria, *labelcategoria2, *labelcomentario, *labelcpf;
  Ihandle *buttonCriar, *buttonSair;
  Ihandle *vbox;
  
  
  
  /*************Labels*************/
  
  
  /*****Registro*****/
  label1 = IupLabel("Cadastrar Receita");
  
  
  
  /*****CPF*****/
  labelcpf = IupLabel("CPF:");  
  
  
  /*****Dia*****/
  labeldia = IupLabel("Dia:");
  
  
  /*****Mes*****/
  labelmes = IupLabel("Mes:");
  
  
  /*****Ano*****/
  labelano = IupLabel("Ano:");
  
  
  /*****Quantia*****/
  labelquantia = IupLabel("Valor (R$) :");
  
  
  /*****Categoria*****/
  labelcategoria = IupLabel("Categoria:");
  
  
  /*****Categoria2*****/
  labelcategoria2 = IupLabel("[Moradia] [Trabalho] [Transporte] [Comida] [Estudos]");
  
  
  /*****Comentario*****/
  labelcomentario = IupLabel("Comentario:");
  
  
  
  
  /*************Botoes*************/
    
  

  /*****Registrar*****/
  buttonCriar = IupButton("Criar", NULL);
  IupSetAttribute(buttonCriar, "SIZE", "40");
  
  IupSetCallback(buttonCriar, "ACTION", (Icallback)criar_receita);
  
  
  
  /*****Sair*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "40");
  
  IupSetCallback(buttonSair, "ACTION", (Icallback)fecharJanela);
  
  
  
  /*************Caixas de Texto*************/
  
  /*****CPF*****/
  multitext_cpf = IupText(NULL);
  
  IupSetAttribute(multitext_cpf, "SIZE", "60x10");
  
  
  /*****Dia*****/
  multitext_dia = IupText(NULL);
  
  IupSetAttribute(multitext_dia, "SIZE", "30x10");
  
  
  /*****Mes*****/
  multitext_mes = IupText(NULL);
  
  IupSetAttribute(multitext_mes, "SIZE", "30x10");  
  
  
  /*****Ano*****/
  multitext_ano = IupText(NULL);
  
  IupSetAttribute(multitext_ano, "SIZE", "30x10");
  
  
  /*****Quantia*****/
  multitext_quantia = IupText(NULL);
  
  IupSetAttribute(multitext_quantia, "SIZE", "60x10");
  
  
  /*****Categoria*****/
  multitext_categoria = IupText(NULL);
  
  IupSetAttribute(multitext_categoria, "SIZE", "95x10");
  
  
  /*****Comentario*****/
  multitext_comentario = IupText(NULL);
  
  IupSetAttribute(multitext_comentario, "SIZE", "215x30");
  
  
  
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  label1,
  IupHbox(labelcpf, multitext_cpf, NULL),
  IupHbox(labeldia, multitext_dia, labelmes, multitext_mes, labelano, multitext_ano, NULL),
  IupHbox(labelquantia, multitext_quantia, NULL),
  IupHbox(labelcategoria, multitext_categoria,labelcategoria2, NULL),
  IupHbox(labelcomentario, multitext_comentario, NULL),
  IupHbox(IupFill(), buttonCriar, buttonSair, NULL),
  NULL);

  IupSetAttribute(vbox, "GAP", "10");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "390x220"); 
  
  
  
  /*************Loop*************/
  
 }
  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);

  return IUP_DEFAULT;
}



int janelaRegistro(Ihandle *registro){
  Ihandle* caixaGeral = (Ihandle*)IupGetAttribute(registro, "FIND_DIALOG");
  if (!caixaGeral){  
  Ihandle *label1, *labelcpf, *labelsenha, *labelnome;
  Ihandle *buttonRegistrar, *buttonSair;
  Ihandle *vbox;

  
  
  
  
  
  
  /*************Labels*************/
  
  
  /*****Registro*****/
  label1 = IupLabel("Registro");
  
  
  /*****Nome*****/
  labelnome = IupLabel("Nome:");
  
  
  /*****CPF*****/
  labelcpf = IupLabel("CPF:");
  

  /*****Senha*****/
  labelsenha = IupLabel("Senha:");
  
  
  /*************Botoes*************/
    
  

  /*****Registrar*****/
  buttonRegistrar = IupButton("Registrar", NULL);
  IupSetAttribute(buttonRegistrar, "SIZE", "40");
  
  
  
  
  IupSetCallback(buttonRegistrar, "ACTION", (Icallback)criar_pessoa);
  
  
  /*****Sair*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "40");
  
  
  IupSetCallback(buttonSair, "ACTION", (Icallback)fecharJanela);
  
  
  /*************Caixas de Texto*************/
  
  
  /*****Nome*****/
  multitext_nome = IupText(NULL);
  
  IupSetAttribute(multitext_nome, "SIZE", "95x10");
  
  
  /*****CPF*****/
  multitext_cpf = IupText(NULL);
  
  IupSetAttribute(multitext_cpf, "SIZE", "95x10");
  
  
  
  /*****Senha*****/
  multitext_senha = IupText(NULL);
  
  IupSetAttribute(multitext_senha, "SIZE", "95x10");
  
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  label1,
  IupHbox(labelnome, multitext_nome, NULL),
  IupHbox(labelcpf, multitext_cpf, NULL),
  IupHbox(labelsenha, multitext_senha, NULL),
  IupHbox(IupFill(), buttonRegistrar, buttonSair, NULL),
  NULL);

  IupSetAttribute(vbox, "GAP", "10");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x140"); 
  
  
  
  /*************Loop*************/
  
  
  }

  IupShowXY(caixaGeral, IUP_CURRENT, IUP_CURRENT);
  return IUP_DEFAULT;
}



int main(int argc, char **argv){	

  IupOpen(&argc, &argv);  
  
  Ihandle *buttonRegistrar, *buttonReceita, *buttonHistorico, *buttonGasto, *buttonSair;
  Ihandle *caixaGeral;
  Ihandle *vbox;
  
  
  
  /*************Botoes*************/

  
  /*****Registrar*****/
  buttonRegistrar = IupButton("Registrar-se", NULL);
  IupSetAttribute(buttonRegistrar, "SIZE", "80x20");
  
  IupSetCallback(buttonRegistrar, "ACTION", (Icallback) janelaRegistro);
  
  
  
  /*****Receita*****/
  buttonReceita = IupButton("Adicionar Receita", NULL);
  IupSetAttribute(buttonReceita, "SIZE", "80x20");
  
  IupSetCallback(buttonReceita, "ACTION", (Icallback) janelaReceita);
  
  
  
  /*****Gasto*****/
  buttonGasto = IupButton("Adicionar Gasto", NULL);
  IupSetAttribute(buttonGasto, "SIZE", "80x20");
  
  IupSetCallback(buttonGasto, "ACTION", (Icallback) janelaGasto);
  
  
  /*****Historico*****/
  buttonHistorico = IupButton("Ver Historico", NULL);
  IupSetAttribute(buttonHistorico, "SIZE", "80x20");
  
  IupSetCallback(buttonHistorico, "ACTION", (Icallback) janelaHistorico);
  
  
  /*****Sair*****/
  buttonSair = IupButton("Sair", NULL);
  IupSetAttribute(buttonSair, "SIZE", "80x20");
  
  
  IupSetCallback(buttonSair, "ACTION", (Icallback) fecharJanela);
  
  
  
  /*************Box*************/
  
  
  vbox = IupVbox(
  IupHbox(buttonRegistrar, buttonHistorico, NULL),
  IupHbox(buttonReceita, buttonGasto, NULL),
  buttonSair,
  NULL);

  IupSetAttribute(vbox, "GAP", "15");
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
  
  
  
  /*************Janela*************/
  
  
  caixaGeral = IupDialog(vbox);
  IupSetAttribute(caixaGeral, "TITLE", "Gerenciador de Receitas");
  IupSetAttribute(caixaGeral, "SIZE", "210x140"); 
  
  
  
  /*************Loop*************/
  
  
  IupShowXY(caixaGeral, IUP_CENTER, IUP_CENTER);
  IupSetAttribute(caixaGeral, "USERSIZE", NULL); 
  IupMainLoop();
  IupClose();  
  
  return EXIT_SUCCESS;
}
