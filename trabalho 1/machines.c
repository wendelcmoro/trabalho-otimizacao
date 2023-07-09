#include <stdio.h>

#define MAX_TIME 540

int main() {
    int machines, orders = 0;
    if (!scanf( "%d %d", &machines, &orders)) {
        return 1;
    }

    // verify if the machines number or the number of orders is negative
    if (machines <= 0 || orders <= 0) {
        printf("o número de máquinas e o de pedidos precisam ser maior do que zero\n");
        return 1;
    }

    int ordersArray[orders];

    int maxTime = MAX_TIME;

    int times[orders];

    for (int i = 0; i < orders; i++) {
        if (!scanf( "%d %d", &times[i], &ordersArray[i])) {
            return 1;
        }
    }

    // verify if any of the times from the input is greater than 540 minutes or less equal than zero
    // verify also if any of the order multipliers are negative
    for (int i = 0; i < orders; i++) {
        if (ordersArray[i] > MAX_TIME || ordersArray[i] <= 0 || times[i] < 0) {
            printf("Erro: Um ou mais pedidos estão com tempo maior do que o permitido(540 minutos), menor igual a zero ou estão com seu multiplicador negativo.\n");
            return 1;
        }
    }

    // get the maximum div by each of the orders
    int maxDiv[orders];    
    for (int i = 0; i < orders; i++) {
       maxDiv[i] = maxTime/ordersArray[i];
    }
    
    // get the minimum time from the input
    int minimum = ordersArray[0];
    for (int i = 0; i < orders; i++) {
        if (minimum > ordersArray[i]) {
            minimum = ordersArray[i];
        }
    }   

    // generates the possible combinations
    int coefs[100000][orders];
    int solution = 0;
    int remainder = 0;
    int auxRemainder = 0;

    for (int i = orders - 1; i >= 0; i--) {
        for (int k = maxDiv[i]; k > 0; k--) {

            coefs[solution][i] = k;
            
            remainder = maxTime - ordersArray[i]*k;

            auxRemainder = remainder;

            if (remainder >= minimum) {
                for (int p = orders - 1; p >= 0; p--) {
                    for (int n = orders-1; n >= 0; n--) {
                        if (n != i) {
                            coefs[solution][n] = 0;
                        }
                    }
                    
                    for (int n = p; n >= 0; n--) {
                        if (n != i) {
                            for (int l = maxDiv[n]; l >= 0; l--) {
                                if (ordersArray[n]*l <= auxRemainder) {
                                    auxRemainder -= ordersArray[n]*l;

                                    coefs[solution][n] = l;

                                    break;
                                }
                            }
                            if (auxRemainder < minimum) {
                                solution++;
                                break;
                            }
                        }
                    }
                    coefs[solution][i] = k;
                    auxRemainder = remainder;
                }
            }
            else {
                for (int n = orders - 1; n > 0; n--) {
                    if (n != i) {
                        coefs[solution][n] = 0;
                    }
                }
                solution++;           
            }
        }
    }

    // mark which combinations have repeated
    int repeated[solution];
    int hasRepeated = -1;    
    for (int i = 1; i < solution; i++) {
        for (int k = 0;  k < solution; k++) {
            hasRepeated = 0;            
            if (k != i) {
                repeated[k] = 0;
                for (int j = 0; j < orders; j++) {
                    if (coefs[i][j] == coefs[k][j]) {
                        hasRepeated = k;
                    }
                    else {
                        hasRepeated = 0;
                        break;
                    }
                }

                if (hasRepeated != 0) {
                    for (int n = 0; n < orders; n++) {
                        coefs[hasRepeated][n] = 0;
                    }

                    repeated[k] = 1;
                }
            }
        }        
    }

    // count how many combinations have repeated
    hasRepeated = 0;
    for (int i = 0; i < solution; i++) {
        if (repeated[i] == 1) {
            hasRepeated++;
        }
    }
    
    // print the objective function
    printf("min : ");
    for (int i = 1; i <= solution - hasRepeated; i++) {
        printf("x%d", i);
        if (i != solution - hasRepeated) {
            printf(" + ");
        }
    }
    printf(";\n\n");
    
    // print the constraints
    int coef = 1;
    int lastCoef = 0;
    for (int i = 0; i < orders; i++) {
        coef = 1;
        lastCoef = 0;
        for (int j = 0; j < solution; j++) {
            if (repeated[j] != 1) {                
                if (coefs[j][i] != 0) {
                    if (lastCoef > 0 && coef > 1) {
                        printf(" + ");
                    }               
                    if (coefs[j][i] == 1) {
                        printf("x%d", coef);
                    }
                    else {
                        printf("%dx%d", coefs[j][i], coef);  
                    }

                    lastCoef = coefs[j][i];
                }        
                coef++;
            }
        }
        printf(" >= %d;\n", times[i]);
    }

    printf("\n");

    // for each of constraints, the value must be great than zero
    for (int i = 1; i < coef; i++) {
        printf("x%d > 0;\n", i);
    }
    return 0;
}