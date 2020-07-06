//////////////////////////////////////////////////////////////////////////////////////////
/*
   La valise : Traitement des données
   Création d'un livre en pdf à partir des données enregistrées.
   Version d'origine avec Processing 2 - 2013.11.02
   Version actuelle avec Processing 3 - 2020.07.06
   http://entropie.org
*/
//////////////////////////////////////////////////////////////////////////////////////////

int nbMesuresMaxi = 14000;  // 8000 : In preferences, increase maximum available memory to 2048 MB
                            // Choisir un multiple de nbImages.
int nbMesures = 0;
// Les données en provenance de la carte SD
int[] NumeroMesure = new int[nbMesuresMaxi];
long[] Millisecondes = new long[nbMesuresMaxi];
String[] Date = new String[nbMesuresMaxi];
String[] Heure = new String[nbMesuresMaxi];
int[] A0 = new int[nbMesuresMaxi]; // Entrée analogique 0 de la carte Arduino
int[] A1 = new int[nbMesuresMaxi]; // Entrée analogique 1 de la carte Arduino

// Paramètres pour les images
int nbImages=6;
int width;   // La largeur de la fenêtre dépend du nombre de mesures
int height;  // La hauteur de la fenêtre dépend du nombre de mesures

// Paramètres du livre
import processing.pdf.*;
PGraphicsPDF pdf;
int marge;
int tailleTexte;
PFont myFont;
String titre="Le marcheur";
String auteur="Eric Sérandour";
String editeur="Entropie";
String date="novembre 2013";

//////////////////////////////////////////////////////////////////////////////////////////

void settings() {
  // On récupère les données 
  String lignes[] = loadStrings("données.txt"); // LoadStrings() découpe le fichier texte
                                                // sur la base des retours à la ligne
  nbMesures=lignes.length-1; // On enlève la ligne qui contient l'entête
  if (nbMesures > nbMesuresMaxi) {
    nbMesures = nbMesuresMaxi;
  }
  for (int i=1; i<=nbMesures; i++) {
    String infos[] = split(lignes[i]," ");  // On sépare les informations (le séparateur
                                            // est ici l'espace)
    NumeroMesure[i-1]=int(infos[0]);        // On convertit une chaine en entier
    Millisecondes[i-1]=int(infos[1]);       // On convertit une chaine en entier
    Date[i-1]=infos[2];
    Heure[i-1]=infos[3];
    A0[i-1]=int(infos[4]);                  // On convertit une chaine en entier
    A1[i-1]=int(infos[5]);                  // On convertit une chaine en entier

    print(NumeroMesure[i-1]); print(" ");
    print(Millisecondes[i-1]); print(" ");
    print(Date[i-1]); print(" ");
    print(Heure[i-1]); print(" ");
    print(A0[i-1]); print(" ");
    println(A1[i-1]);
  }  

  // Paramètres pour les images
  width = nbMesures / nbImages;   // Largeur de la fenêtre
  height = nbMesures / nbImages ;  // Hauteur de la fenêtre
  smooth(); // Pour lisser le tracé (anti-aliasing)
  
  // Le livre
  size(width ,height, PDF, "Livre.pdf");
}

//////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  myFont = createFont("Bitstream Charter", 48);
  marge=width/10;
  tailleTexte = width/30;
  pdf = (PGraphicsPDF) g;
 
  debutLivre(); // Couverture
  fill(255);
  stroke(10);
}

//////////////////////////////////////////////////////////////////////////////////////////

void draw()
{
  if (frameCount <= nbImages) {
    background(255);                         // Fond noir
    stroke(0);                               // Courbe en blanc
    int xmax = width-1;
    int ymax = height-1;
    // Tracé de la courbe à l'écran de l'entrée A0
    for (int x=0; x<=xmax; x++) {                                 // Normallement < et non pas <=
                                                                  // Mais cela permet que le tracé fasse toute la largeur de la page
      line(x  , ymax - ymax*A0[(frameCount-1)*width+x]/1023,      // values[ ] est codée sur 10 bits
           x+1, ymax - ymax*A0[(frameCount-1)*width+x+1]/1023);   // 2^10-1  = 1023
    }
    // Tracé de la courbe à l'écran de l'entrée A1 
    for (int x=0; x<=xmax; x++) {                                 // Normallement < et non pas <=
                                                                  // Mais cela permet que le tracé fasse toute la largeur de la page
      line(x  , ymax - ymax*A1[(frameCount-1)*width+x]/1023,      // values[ ] est codée sur 10 bits
           x+1, ymax - ymax*A1[(frameCount-1)*width+x+1]/1023);   // 2^10-1  = 1023
    }
    pdf.nextPage();
  }
  else {
    finLivre();
    print("width : "); print (width); print(" - height : "); println (height);
    println("fin");
    exit();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////


void debutLivre() { // cree la couverture

 int tailleTitre = width/10;
 int tailleAuteur = width/20;

 background(255);
 fill(0);

 textAlign(CENTER);
 textFont(myFont,tailleTitre);
 text(titre,width/2,(height/2)-1.5*tailleTitre);
 textFont(myFont,tailleAuteur);
 text(auteur,width/2,(height/2)-1.5*tailleTitre+marge);
 textFont(myFont,tailleTexte);
 text(editeur,width/2,height-marge);
 pdf.nextPage();
 pdf.nextPage();
}

///////////////////////////////////////////////////////////////////////////

void finLivre() {
  
 pdf.nextPage(); // une page blanche
 fill(0);
 // cree une page avec la date de création
 textAlign(CENTER);
 textFont(myFont,tailleTexte);  
 text("Enregistrement de " + date, width/2,height-marge-1.5*tailleTexte);
 String commentaire="Accéléromètre ADXL322";
 text(commentaire,width/2,height-marge);
}

//////////////////////////////////////////////////////////////////////////////////////////

void keyPressed() {
  if (key == ESC) {
    finLivre();
    exit();   // Stops the program
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
