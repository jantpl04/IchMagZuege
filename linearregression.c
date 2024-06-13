#include <stdio.h>
#include <stdlib.h>

void linearRegression(double x[], double y[], int n, double *slope, double *intercept);
void delete_most_deviation(double x[], double y[], int* n, double slope, double intercept, int neg_or_pos);
void quadraticRegression(double x[], double y[], int n, double *a, double *b, double *c);

int main()
{
    
    double offsettemp[2000], offsetvalue[2000];
    double senstemp[2000], sensvalue[2000];

    FILE* f_offsetfile, *f_sensitivityfile;
    f_offsetfile = fopen("OffsetRawData.txt", "r");
    f_sensitivityfile = fopen("SensitivityRawData.txt", "r");


    //daten aus dateien einlesen
    int n_offset = 0, n_sens = 0, j = 1;
    while(n_offset<2000 && j > 0){
        j = fscanf(f_offsetfile, "%lf %lf", &offsettemp[n_offset], &offsetvalue[n_offset]);
        n_offset++;
    }
    n_offset--;
    j = 1;
    
    while(n_sens<2000 && j > 0){
        j = fscanf(f_sensitivityfile, "%lf %lf", &senstemp[n_sens], &sensvalue[n_sens]);
        n_sens++;
    }
    n_sens--;

    fclose(f_offsetfile);
    fclose(f_sensitivityfile);

    printf("Anzahl Offsetwerte: %d\n", n_offset);

    //Plausibilitätsprüfung
    double temp_in_c;
    for(int i = 0; i < n_offset; i++){
        temp_in_c = offsettemp[i] * 0.3665 - 280;
        if(temp_in_c < -40 || temp_in_c > 60){
            printf("Messwert Nr %d ist ungueltig\n", i+1);
            offsettemp[i] = offsettemp[n_offset-1], offsetvalue[i] = offsetvalue[n_offset-1];
            n_offset--;
        }
    }

    printf("Anzahl Offsetwerte: %d\n", n_offset);

    printf("Anzahl Sensitivitywerte: %d\n", n_sens);

    //Plausibilitätsprüfung
    for(int i = 0; i < n_sens; i++){
        temp_in_c = senstemp[i] * 0.3665 - 280;
        if(temp_in_c < -40 || temp_in_c > 60){
            printf("Messwert Nr %d ist ungueltig\n", i+1);
            senstemp[i] = senstemp[n_sens-1], sensvalue[i] = sensvalue[n_sens-1];
            n_sens--;
        }
    }

    printf("Anzahl Sensitivitywerte: %d\n", n_sens);


    //Lineare Regression
    double slope, intercept;

    linearRegression(offsettemp, offsetvalue, n_offset, &slope, &intercept);
    printf("Offset:\nSteigung: %lf\nAchsenabschnitt: %lf\n", slope, intercept);

    delete_most_deviation(offsettemp, offsetvalue, &n_offset, slope, intercept, 1);
    delete_most_deviation(offsettemp, offsetvalue, &n_offset, slope, intercept, 1);
    delete_most_deviation(offsettemp, offsetvalue, &n_offset, slope, intercept, -1);
    delete_most_deviation(offsettemp, offsetvalue, &n_offset, slope, intercept, -1);
    printf("Number of Offsetvalues left: %d\n", n_offset);

    linearRegression(senstemp, sensvalue, n_sens, &slope, &intercept);
    printf("Sens:\nSteigung: %lf\nAchsenabschnitt: %lf\n", slope, intercept);

    delete_most_deviation(senstemp, sensvalue, &n_sens, slope, intercept, 1);
    delete_most_deviation(senstemp, sensvalue, &n_sens, slope, intercept, 1);
    delete_most_deviation(senstemp, sensvalue, &n_sens, slope, intercept, -1);
    delete_most_deviation(senstemp, sensvalue, &n_sens, slope, intercept, -1);
    printf("Number of Sensvalues left: %d\n", n_sens);

    //Quadratische Regression
    double a, b, c;
    
    quadraticRegression(offsettemp, offsetvalue, n_offset, &a, &b, &c);
    printf("Quadratische Regression: %lf * x^2 + %lf * x + %lf", a,b,c);



}



void linearRegression(double x[], double y[], int n, double *slope, double *intercept) {
    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
    
    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumXX += x[i] * x[i];
    }
    
    double meanX = sumX / n;
    double meanY = sumY / n;
    
    *slope = (sumXY - n * meanX * meanY) / (sumXX - n * meanX * meanX);
    *intercept = meanY - (*slope) * meanX;
}

void delete_most_deviation(double x[], double y[], int* n, double slope, double intercept, int neg_or_pos) //neg_or_pos: 1 for positive deviation, -1 for negative deviation
{
    double max_deviation = 0, current_deviation;
    int max_i = 0;

    for(int i = 0; i < *n; i++){
        current_deviation = (y[i] - (slope * x[i] + intercept)) * neg_or_pos;
        if(current_deviation > max_deviation){
            max_deviation = current_deviation;
            max_i = i;
        }
    }
    printf("Most %s deviation: Number %d with temp %lf and value %lf\n", (neg_or_pos == 1?"positive":"negative"), max_i, x[max_i], y[max_i]);
    x[max_i] = x[*n-1], y[max_i] = y[*n-1];
    (*n)--;
}

void quadraticRegression(double x[], double y[], int n, double *a, double *b, double *c) {
    double sumX = 0, sumY = 0, sumX2 = 0, sumX3 = 0, sumX4 = 0;
    double sumXY = 0, sumX2Y = 0;

    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumX2 += x[i] * x[i];
        sumX3 += x[i] * x[i] * x[i];
        sumX4 += x[i] * x[i] * x[i] * x[i];
        sumXY += x[i] * y[i];
        sumX2Y += x[i] * x[i] * y[i];
    }

    double denominator = n*sumX2*sumX4 + sumX*sumX3*sumX2 + sumX2*sumX*sumX3 -
                         sumX2*sumX2*sumX2 - n*sumX3*sumX3 - sumX*sumX*sumX4;

    *a = (n*sumX2*sumX2Y + sumX*sumX3*sumY + sumX2*sumXY*sumX2 -
          sumX2*sumX2*sumY - n*sumX3*sumXY - sumX*sumX2*sumX2Y) / denominator;

    *b = (n*sumXY*sumX4 + sumX*sumX2Y*sumX2 + sumX2*sumX*sumX2Y -
          sumX2*sumX2*sumX2Y - n*sumX2Y*sumX3 - sumX*sumXY*sumX4) / denominator;

    *c = (sumY*sumX2*sumX4 + sumX*sumX2Y*sumX3 + sumX2*sumXY*sumX3 -
          sumX2*sumX2*sumX2Y - sumY*sumX3*sumX3 - sumX*sumXY*sumX4) / denominator;
}
