
#include <stdio.h>

int main()
{
int i;                                                         // Variablen deklarieren  
float v, a, t, v_positiv, s, abschnitt, abschnitt_meter, differenz; 
float unterschieda, gesamtergebnisa = 248.80; 
float zeit, gesamtzeit, v_neu, umrechnung, gesamtergebnis = 0; // Variablen werden deklariert und initialisiert 
float strecke [7]= {9.5, 9.9, 11.2, 11.4, 11.6, 12.4, 12.5};   // Werte für die Streckenbegrenzung 
float strecke2 [7]= {7.8, 9.5, 9.9, 11.2, 11.4, 11.6, 12.4};   // Werte für die Streckenbegrenzung
float geschwindigkeit [7]= {100, 80, 50, 45, 50, 75, 50};      // Werte für Geschwindigkeit der Abschnitte 
float geschwindigkeit2 [7] = {80, 50, 45, 50, 75, 50, 50};     // Werte für Geschwindigkeit der Abschnitte


for (i = 0; i<= 6; i++){ 
   v = geschwindigkeit[i] - geschwindigkeit2[i]; /* Geschwindigkeitsdifferenz wird berechnet auf die gebremst bzw. 
                                                    beschleunigt werden muss*/ 
    if (v < 0){                                  // Fall für Beschleunigungsvorgang 
        a = 1.2;                                 // Beschleunigungswert für Beschleunigungsvorgang 
        v_positiv= -v;                           // Damit nicht mit negativer Geschwindigkeit weiter gerechnet wird 
        v_neu = v_positiv / 3.6;                 // Geschwindigkeit wird in m/s umgerechnet 
        t = v_neu / a;                           // Zeit für Beschleunigungsvorgang wird berechnet 
        s = 0.5 * a *(t*t);                      // Strecke für gleichmäßig beschleunigte Bewegung wird berechnet 
        abschnitt = strecke[i] - strecke2[i];    // Differenz des Streckenabschnitts 
        abschnitt_meter = abschnitt * 1000;      // Strecke in Metern umgerechnet 
        differenz = abschnitt_meter - s;         // Strecke die mit normaler Geschwindigkeit gefahren werden kann 
        umrechnung = geschwindigkeit[i] / 3.6;   // Geschwindigkeit wird in m/s umgerechnet 
        zeit = differenz / umrechnung;           // Zeit für die restliche Strecke wird berechnet 
        gesamtzeit = zeit + t;                   /* Zeit für Beschleunigungsvorgang und Zeit der restlichen Strecke 
                                                    wird addiert*/ 
        
       }
       
    else if (v > 0){     // Fall für Bremsvorgang 
        a = 0.9;         // Bremsverzögerungswert für Bremsvorgang 
        v_neu = v / 3.6; // Ab hier gilt die gleiche Rechnung wie bei der Beschleunigung 
        t = v_neu / a; 
        s = 0.5 * a *(t*t); 
        abschnitt = strecke[i] - strecke2[i];
        abschnitt_meter = abschnitt * 1000; 
        differenz = abschnitt_meter - s; 
        umrechnung = geschwindigkeit[i] / 3.6; 
        zeit = differenz / umrechnung; 
        gesamtzeit = zeit + t; 
       
       }
       
    else if(v == 0){     // Fall ohne Veränderung der Geschwindigkeit 
        abschnitt = strecke[i] - strecke2[i]; 
        abschnitt_meter = abschnitt * 1000; 
        umrechnung = geschwindigkeit[i] / 3.6; 
        zeit = abschnitt / umrechnung; 
        gesamtzeit = zeit + t; 
    
    }
        unterschieda = gesamtergebnis - gesamtergebnisa; 
   
printf("%.2lf ist die Zeit für Abschnitt %d\n", gesamtzeit, i); // Gibt die Zeit des Streckenabschnitts aus 
   gesamtergebnis += gesamtzeit;                                   /* Addiert die Zeit des Streckenabschnitts 
                                                                      zum vorherigen Ergebnis*/
}
printf ("%.2lfs ist die Gesamtzeit aller Streckenabschnitte\n", gesamtergebnis); /* Gesamtzeit aller Streckenabschnitte
                                                                                 wird ausgegeben*/
printf("Der Unterschied zu a beträgt %.2lfs",unterschieda); 

    return 0;
}
