/*****************************************************************************/
/*                                                                           */
/*                           Fronton3.c                                      */
/*                                                                           */
/*  Programa inicial d'exemple per a les practiques 2 i 3 d'ISO.	     */
/*                                                                           */
/*  Compilar i executar:					  	     */
/*     El programa invoca les funcions definides a "winsuport.c", les        */
/*     quals proporcionen una interficie senzilla per crear una finestra     */
/*     de text on es poden escriure caracters en posicions especifiques de   */
/*     la pantalla (basada en CURSES); per tant, el programa necessita ser   */
/*     compilat amb la llibreria 'curses':				     */
/*									     */
/*	   $ gcc -c winsuport2.c -o winsuport2.o				*/
/*	   $ gcc fronton0.c winsuport.o -o fronton0 -lcurses		     */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>	/* incloure definicions de funcions estandard */
#include <stdlib.h>
#include <string.h>
#include "winsuport2.h"	/* incloure definicions de funcions propies */
#include "memoria.h"
#include "semafor.h"
#include "missatge.h"

/* programa principal                               */
int main(int n_args, char *ll_args[])
{
	int f_h, c_h;
	char rh,rv,rd;
	int index = atoi(ll_args[1]);
	int f_pil, c_pil;
	float pos_f, pos_c;
	float vel_f, vel_c;
	void *p_win;
	int mida_camp, n_fil, n_col;
	int retard;							/* valor del retard de moviment, en mil.lisegons */
	int id_fi1, *fi1, id_fi2, *fi2, id_fiRebots, *fiRebots, id_sem;		/* convertim les condicions de fi en globals*/
	int MAX_REBOTS= atoi(ll_args[15]);
	int *id_bustia, id_bustia_mem;
	char missatge[100];
	int pilota_rebot;


//fprintf(stderr,"\n%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",(ll_args[1]),(ll_args[2]),(ll_args[3]),(ll_args[4]),(ll_args[5]),(ll_args[6]),(ll_args[7]),(ll_args[8]),(ll_args[9]),(ll_args[10]),(ll_args[11]));

	pos_f= atof(ll_args[6]);  
	pos_c= atof(ll_args[7]);
	vel_f= atof(ll_args[8]);
	vel_c= atof(ll_args[9]);
	f_pil = atoi(ll_args[10]);
	c_pil = atoi(ll_args[11]);
	id_fi1 = atoi(ll_args[12]);
	id_fi2 = atoi(ll_args[13]);
	id_fiRebots = atoi(ll_args[14]);

	setbuf(stderr, NULL);

	fi1 = map_mem(id_fi1);		/* obtenir adres. de mem. compartida */
	fi2 = map_mem(id_fi2);		/* obtenir adres. de mem. compartida */
	fiRebots = map_mem(id_fiRebots);		/* obtenir adres. de mem. compartida */

	mida_camp = atoi(ll_args[2]);
	p_win = map_mem(mida_camp);		/*direcció mem compartida*/

	n_fil = atoi(ll_args[3]);		/* obtenir dimensions del camp de joc */
	n_col = atoi(ll_args[4]);

	win_set(p_win,n_fil,n_col);	/* crea acces a finestra oberta pel proces pare */

	retard = atoi(ll_args[5]);
	
	id_sem = atoi(ll_args[16]);	/*Obtenim l'identificador del semàfor*/
	
	id_bustia_mem = atoi(ll_args[17]);
	id_bustia = map_mem(id_bustia_mem);		/*direcció mem compartida*/

int tipus_xoc;
float nova_vel_f, nova_vel_c;
int anterior=-1, pendent=0;

	do
	{
		tipus_xoc=0;
		sprintf(missatge, "%i-%f-%f-%i", index, vel_f, vel_c, tipus_xoc);
		sendM(id_bustia[index], missatge, 100);			/*enviem un missatge a la bustia pròpia de la pilota per evitar bloquejos*/
		pilota_rebot=-1;


		while(pilota_rebot!=index){	/*mentes no llegim el missatge propi anem llegin de la bústia*/

			receiveM(id_bustia[index], missatge);
			sscanf(missatge, "%i-%f-%f-%i", &pilota_rebot, &nova_vel_f, &nova_vel_c, &tipus_xoc);	/*llegim missatge*/

			//if(tipus_xoc!=0)fprintf(stderr, "%i: %i %f %f %i\n",index, pilota_rebot, nova_vel_f, nova_vel_c, tipus_xoc); //error

			if(tipus_xoc!=0)pendent=0; /*si hem rebut misatge d'un xoc pendent, podem iniciar un altre xoc*/

			if(tipus_xoc==1){		/*xoc vertical*/
				if(anterior!=pilota_rebot){			/*si no hem iniciat el xoc, enviem missatge a l'altra pilota*/
					sprintf(missatge, "%i-%f-%f-%i", index, vel_f, vel_c, 1);
					sendM(id_bustia[pilota_rebot],missatge, 100);
				}
				else {
					anterior=-1;				/*si hem iniciat el xoc, tanquem el cicle*/
					//pendent=0;
				}
				vel_f=nova_vel_f;				/*actualitzem velocitat*/
			}

			if(tipus_xoc==2){		/*xoc horitzontal*/
				if(anterior!=pilota_rebot){			/*si no hem iniciat el xoc, enviem missatge a l'altra pilota*/
					sprintf(missatge, "%i-%f-%f-%i", index, vel_f, vel_c, 2);
					sendM(id_bustia[pilota_rebot],missatge, 100);
				}
				else {
					anterior=-1;				/*si hem iniciat el xoc, tanquem el cicle*/
					//pendent=0;
				}
				vel_c=nova_vel_c;				/*actualitzem velocitat*/
			}

			if(tipus_xoc==3){		/*xoc diagonal*/
				if(anterior!=pilota_rebot){			/*si no hem iniciat el xoc, enviem missatge a l'altra pilota*/
					sprintf(missatge, "%i-%f-%f-%i", index, vel_f, vel_c, 3);
					sendM(id_bustia[pilota_rebot],missatge, 100);
				}						
				else {
					anterior=-1;				/*si hem iniciat el xoc, tanquem el cicle*/
					//pendent=0;
				}
				vel_f=nova_vel_f;				/*actualitzem velocitats*/
				vel_c=nova_vel_c;
			}
		}	
		f_h = pos_f+vel_f;		/* posicio hipotetica de la pilota (entera) */
		c_h = pos_c+vel_c;
		rh = rv = rd = ' ';
		if ((f_h != f_pil) || (c_h != c_pil))
		{		/* si posicio hipotetica no coincideix amb la posicio actual */
			if (f_h != f_pil) 		/* provar rebot vertical */
			{	
				waitS(id_sem);			/* tanquem el semàfor*/	
				rv = win_quincar(f_h,c_pil);	/* veure si hi ha algun obstacle */
				signalS(id_sem);		/* obrim el semàfor*/
				if (rv != ' ')			/* si hi ha alguna cosa */
				{   
					if(rv == '0' || rv =='+'){
						vel_f = -vel_f;		/* canvia sentit velocitat vertical */
					    	f_h = pos_f+vel_f;		/* actualitza posicio hipotetica */
						//fprintf(stderr, "%i rebot paret\n", index);
					    	if (rv == '0')
					    	{ 
							waitS(id_sem);		/* tanquem el semàfor*/	
							(*fiRebots)++;
							signalS(id_sem);	/* obrim el semàfor*/
		    			    	}			
					}
					else{						
						if(pendent==0){
							pilota_rebot = rv - '1';	/*capturem index amb la pilota en la que xoquem*/
							tipus_xoc=1;			/*definim el tipus de xoc: vertical=1*/
							anterior=pilota_rebot;		/*variable per evitar un intercanvi de missatges infinit*/
							sprintf(missatge, "%i-%f-%f-%i", index, vel_f, vel_c, tipus_xoc);
							sendM(id_bustia[pilota_rebot],missatge, 100);	/*enviem missatge*/
							pendent=1;			/*variable per evitar més d'un xoc amb la mateixa variable sense actualitzar el primer*/
						}
					}
				}
    			}
    			if (c_h != c_pil) 			/* provar rebot horitzontal */
    			{	
				waitS(id_sem);			/* tanquem el semàfor*/	
				rh = win_quincar(f_pil,c_h);	/* veure si hi ha algun obstacle */
				signalS(id_sem);		/* obrim el semàfor*/

				if (rh != ' ')			/* si hi ha alguna cosa */
				{   
					if(rh == '0' || rh =='+'){
						vel_c = -vel_c;		/* canvia sentit vel. horitzontal */
	     			     		c_h = pos_c+vel_c;		/* actualitza posicio hipotetica */
					    	if (rh == '0')
					    	{ 
							waitS(id_sem);		/* tanquem el semàfor*/	
							(*fiRebots)++;
							signalS(id_sem);	/* obrim el semàfor*/
		    			    	}			
					}
					else{						
						if(pendent==0){						
							pilota_rebot = rh - '1';	/*capturem index amb la pilota en la que xoquem*/
							tipus_xoc=2;			/*definim el tipus de xoc: horizontal=2*/
							anterior=pilota_rebot;		/*variable per evitar un intercanvi de missatges infinit*/
							sprintf(missatge, "%i-%f-%f-%i", index, vel_f, vel_c, tipus_xoc);
							sendM(id_bustia[pilota_rebot],missatge, 100);	/*enviem missatge*/
							pendent=1;			/*variable per evitar més d'un xoc amb la mateixa variable sense actualitzar el primer*/
						}
					}
				}	

    			}
    			if ((f_h != f_pil) && (c_h != c_pil))		/* provar rebot diagonal */
    			{	
				waitS(id_sem);				/* tanquem el semàfor*/	
				rd = win_quincar(f_h,c_h);
				signalS(id_sem);			/* obrim el semàfor*/	

				if (rd != ' ')			/* si hi ha alguna cosa */
				{   
					if(rd == '0' || rd =='+'){
						vel_c = -vel_c;		/* canvia sentit vel. horitzontal */
						vel_f = -vel_f;
	     			     		c_h = pos_c+vel_c;		/* actualitza posicio hipotetica */
						f_h = pos_f+vel_f;
					    	if (rd == '0')
					    	{ 
							waitS(id_sem);		/* tanquem el semàfor*/	
							(*fiRebots)++;
							signalS(id_sem);	/* obrim el semàfor*/
		    			    	}			
					}
					else{						
						if(pendent==0){						
							pilota_rebot = rd - '1';	/*capturem index amb la pilota en la que xoquem*/
							tipus_xoc=3;			/*definim el tipus de xoc: diagontal=3*/
							anterior=pilota_rebot;		/*variable per evitar un intercanvi de missatges infinit*/
							sprintf(missatge, "%i-%f-%f-%i", index, vel_f, vel_c, tipus_xoc);
							sendM(id_bustia[pilota_rebot],missatge, 100);	/*enviem missatge*/
							pendent=1;			/*variable per evitar més d'un xoc amb la mateixa variable sense actualitzar el primer*/
						}
					}
				}	
    			}
    			waitS(id_sem);				/* tanquem el semàfor*/	
    			if (win_quincar(f_h,c_h) == ' ')	/* verificar posicio definitiva */
   			{					/* si no hi ha obstacle */
				win_escricar(f_pil,c_pil,' ',NO_INV);  	/* esborra pilota */		
				pos_f += vel_f; pos_c += vel_c;
				f_pil = f_h; c_pil = c_h;		/* actualitza posicio actual */
				if (c_pil != 0){	 		/* si ho surt del taulell, */	
					win_escricar(f_pil,c_pil, '1'+index ,INVERS); /* imprimeix pilota */	
				}
				else
				{
					(*fi2)=1;		/* codi de finalitzacio de partida */
 			  	}
				signalS(id_sem);		/* obrim el semàfor*/	
 			}
			else{}	signalS(id_sem);		/* obrim el semàfor*/
  		}
  		else { pos_f += vel_f; pos_c += vel_c; }	
  		win_retard(retard);
	}while (!(*fi1) && !(*fi2) && (*fiRebots)<MAX_REBOTS);

	return(0);			/* retorna sense errors d'execucio */
}
