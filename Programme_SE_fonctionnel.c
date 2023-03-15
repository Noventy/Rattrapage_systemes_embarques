#include <stdio.h>
#include <stdlib.h>


typedef struct parametre
{
    bool LUMIN;
    int LUMIN_LOW;
    int LUMIN_HIGH;

    bool TEMP_AIR;
    int MIN_TEMP_AIR;
    int MAX_TEMP_AIR;

    bool HYGR;
    int HYGR_MINT;
    int HYGR_MAXT;

    int LOG_INTERVALL;
    int TIME_OUT;
    int FILE_MAX_SIZE;

}parametre_a;

parametre_a CONFIG;

void setup()
{
  Serial.begin(9600);
  config_mode(&CONFIG);
}

void loop()
{

}

void config_mode(parametre_a *CONFIG)
{
  int entree=NULL;
  Serial.println("Choissisez un paramètre à modifier");
  Serial.println("LUMIN : tapez 1");
  Serial.println("LUMIN_LOW : tapez 2");
  Serial.println("LUMIN_HIGH : tapez 3");
  Serial.println("TEMP_AIR : tapez 4");
  Serial.println("MIN_TEMP_AIR : tapez 5");
  Serial.println("MAX_TEMP_AIR : tapez 6");
  Serial.println("HYGR : tapez 7");
  Serial.println("HYGR_MINT : tapez 8");
  Serial.println("HYGR_MAXT : tapez 9");
  Serial.println("LOG_INTERVALL : tapez 10");
  Serial.println("TIME_OUT : tapez 11");
  Serial.println("FILE_MAX_SIZE : tapez 12");
  Serial.println("RESET : tapez 13");
  Serial.println("VERSION : tapez 14");
  if (Serial.available()>0);
  {
    
    while (entree==NULL)
    {
      entree = Serial.parseInt();
    }
    Serial.print("Vous avez sélectionné le paramètre ");
    Serial.println(entree);
    if (entree==1)
    {
      Serial.println("Vous allez modifier le capteur LUMIN");
      Serial.println("Rentrer la modification souhaité");
      int valeur=NULL;
      Serial.println("Pour désactiver le capteur tapez 1, 2 pour l'activer");
      while (valeur==NULL)
      {
        valeur = Serial.parseInt();
      }
      if (valeur==1 && CONFIG->LUMIN == 0)
      {
        Serial.println("Le capteur LUMIN est déjà à 0");
      }
      else if (valeur==2 && CONFIG->LUMIN == 0)
      {
        CONFIG->LUMIN = valeur;
        Serial.println(CONFIG->LUMIN);
        Serial.println("Le capteur LUMIN a été modifié à 1");
      }
      else
      {
        Serial.println("ERREUR");
      }
      entree=NULL;
    }
/////////////////////////////////////////////////////////////////////////
    else if (entree==2)
    {
      Serial.println("Vous allez modifier LUMIN_LOW");
      Serial.println("Rentrer la modification souhaité");
      int valeur=NULL;
      while (valeur==NULL)
      {
        valeur = Serial.parseInt();
      }
        CONFIG->LUMIN_LOW = valeur;
        Serial.println(CONFIG->LUMIN_LOW);
        Serial.print("Le parametre LUMIN_LOW a été modifié à ");
        Serial.println(CONFIG->LUMIN_LOW);

      entree=NULL;
  } 
  /////////////////////////////////////////////////////////////////////////////

    else if (entree==3)
    {
      Serial.println("Vous allez modifier LUMIN_HIGH");
      Serial.println("Rentrer la modification souhaité");
      int valeur=NULL;
      while (valeur==NULL)
      {
        valeur = Serial.parseInt();
      }
      CONFIG->LUMIN_HIGH = valeur;
        Serial.println(CONFIG->LUMIN_HIGH);
        Serial.print("Le parametre LUMIN_HIGH a été modifié à ");
        Serial.println(CONFIG->LUMIN_HIGH);
      entree=NULL;
    } 
 //////////////////////////////////////////////////////////
    else if (entree==4)
      {
        Serial.println("Vous allez modifier le capteur TEMP_AIR");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        Serial.println("Pour désactiver le capteur tapez 1, 2 pour l'activer");
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
        if (valeur==1 && CONFIG->TEMP_AIR == 0)
        {
          Serial.println("Le capteur TEMP_AIR est déjà à 0");
        }
        else if (valeur==2 && CONFIG->TEMP_AIR == 0)
        {
          CONFIG->TEMP_AIR = valeur;
          Serial.println(CONFIG->TEMP_AIR);
          Serial.println("Le capteur TEMP a été modifié à 1");
        }
        else
        {
          Serial.println("ERREUR");
        }
        entree=NULL;
      }
    //////////////////////////////////////////////////////
    else if (entree==5)
    {
      Serial.println("Vous allez modifier MIN_TEMP_AIR");
      Serial.println("Rentrer la modification souhaité");
      int valeur=NULL;
      while (valeur==NULL)
      {
        valeur = Serial.parseInt();
      }
        CONFIG->MIN_TEMP_AIR = valeur;
        Serial.println(CONFIG->MIN_TEMP_AIR);
        Serial.print("Le parametre MIN_TEMP_AIR a été modifié à ");
        Serial.println(CONFIG->MIN_TEMP_AIR);
      
      entree=NULL;
    } 
/////////////////////////////////////////////////////////////////////////
    else if (entree==6)
      {
        Serial.println("Vous allez modifier MAX_TEMP_AIR");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
          CONFIG->MAX_TEMP_AIR = valeur;
          Serial.println(CONFIG->MAX_TEMP_AIR);
          Serial.print("Le parametre MAX_TEMP_AIR a été modifié à ");
          Serial.println(CONFIG->MAX_TEMP_AIR);

        entree=NULL;
      } 
////////////////////////////////////////////////////////////////////
    else if (entree==7)
      {
        Serial.println("Vous allez modifier le capteur HYGR");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        Serial.println("Pour désactiver le capteur tapez 1, 2 pour l'activer");
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
        if (valeur==1 && CONFIG->HYGR == 0)
        {
          Serial.println("Le capteur HYGR est déjà à 0");
        }
        else if (valeur==2 && CONFIG->HYGR == 0)
        {
          CONFIG->HYGR = valeur;
          Serial.println(CONFIG->HYGR);
          Serial.println("Le capteur HYGR a été modifié à 1");
        }
        entree=NULL;
      }
////////////////////////////////////////////////////////////////////
      else if (entree==8)
      {
        Serial.println("Vous allez modifier HYGR_MINT");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
          CONFIG->HYGR_MINT = valeur;
          Serial.println(CONFIG->HYGR_MINT);
          Serial.print("Le parametre HYGR_MINT a été modifié à ");
          Serial.println(CONFIG->HYGR_MINT);

        entree=NULL;
      } 
///////////////////////////////////////////////////////////////////////
      else if (entree==9)
      {
        Serial.println("Vous allez modifier HYGR_MAXT");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
          CONFIG->HYGR_MAXT = valeur;
          Serial.println(CONFIG->HYGR_MAXT);
          Serial.print("Le parametre HYGR_MAXT a été modifié à ");
          Serial.println(CONFIG->HYGR_MAXT);

        entree=NULL;
      }

/////////////////////////////////////////////////////////////////////
      else if (entree==10)
      {
        Serial.println("Vous allez modifier LOG_INTERVALL");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
          CONFIG->LOG_INTERVALL = valeur;
          Serial.println(CONFIG->LOG_INTERVALL);
          Serial.print("Le parametre LOG_INTERVALL a été modifié à ");
          Serial.println(CONFIG->LOG_INTERVALL);

        entree=NULL;
      }
/////////////////////////////////////////////////////////////////////////
      else if (entree==11)
      {
        Serial.println("Vous allez modifier TIME_OUT");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
          CONFIG->TIME_OUT = valeur;
          Serial.println(CONFIG->TIME_OUT);
          Serial.print("Le parametre TIME_OUT a été modifié à ");
          Serial.println(CONFIG->TIME_OUT);
        entree=NULL;
      }

////////////////////////////////////////////////////////////////////////
      else if (entree==12)
      {
        Serial.println("Vous allez modifier FILE_MAX_SIZE");
        Serial.println("Rentrer la modification souhaité");
        int valeur=NULL;
        while (valeur==NULL)
        {
          valeur = Serial.parseInt();
        }
          CONFIG->FILE_MAX_SIZE = valeur;
          Serial.println(CONFIG->FILE_MAX_SIZE);
          Serial.print("Le parametre FILE_MAX_SIZE a été modifié à ");
          Serial.println(CONFIG->FILE_MAX_SIZE);
      
        entree=NULL;
      }

      else if (entree==13)
      {
        Serial.println("Vous avez réinitialiser les paramètres");
        CONFIG->LUMIN = 1;
        CONFIG->LUMIN_LOW = 255;
        CONFIG->LUMIN_HIGH = 768;
        CONFIG->TEMP_AIR = 1;
        CONFIG->MIN_TEMP_AIR = -10;
        CONFIG->MAX_TEMP_AIR = 60;
        CONFIG->HYGR = 1;
        CONFIG->HYGR_MINT = 0;
        CONFIG->HYGR_MAXT = 50;
        CONFIG->LOG_INTERVALL = 10;
        CONFIG->TIME_OUT = 30;
        CONFIG->FILE_MAX_SIZE = 4096;
      }
      else if (entree==14)
      {
        Serial.println("La version est 1.0 et la numéro de série 6523");
      }
      else
      {
        Serial.println("Le paramètre que vous avez entré n'existe pas");
      }
    int param = NULL;
    Serial.println("Souhaitez vous modifier d'autres paramètres ?");
    Serial.println("Si oui, tapez 1 sinon tapez 2");
    while (param==NULL)
    {
      param = Serial.parseInt();
      if (param == 1)
      {
        config_mode(CONFIG);
      }
      else if (param==2)
      {
        Serial.println("Retour au mode standard");
        //retour ne mode standard
      }
    }
    
}
}
