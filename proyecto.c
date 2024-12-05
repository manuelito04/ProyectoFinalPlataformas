// El presente programa tiene como funcion obtener coeficientes de dos funciones cuadraticas y un porcentaje
// de error ingresado por el usuario, y que a traves de esto se encuentra el subdominio en donde la diferencia 
// entre ambas funciones no supera este porcentaje de error ingresado.

// Realizado por Manuel Alejandro Otarola Madrigal - C05740.

// Incluir librerias:
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Definir los dominios de las funciones y la diferencia entre cada valor.
#define STEP 0.1
#define DOMAIN_MIN -100
#define DOMAIN_MAX 100

// Funciones para evaluar en cada punto cada funcion cuadratica.
double evaluar_funcion1(double a, double b, double c, double x) {
    return a * x * x + b * x + c;
}
double evaluar_funcion2(double d, double e, double f, double x) {
    return d * x * x + e * x + f;
}

// Funcion principal que controla el programa.
int main() {

    // Definir los valores necesarios para las funciones.
    double a, b, c, d, e, f, porcentaje_error;

    // Definir variables para cálclo de dominio y error relativo.
    double x, y1, y2, error;

    // Definir valores para obtener el subdominio requerido.
    double subdominio_inicio = 0, subdominio_final = 0;

    int dentro_subdominio = 0;  // Bandera para rastrear si se esta dentro del subdominio

    // Empezar a escribir el archivo para poder graficar con GNUplot.
    FILE *gnuplot_data;

    // Pedirle al usuario que ingrese los diferentes valores necesarios, coeficientes y error.
    printf("Ingrese los coeficientes de la primera función separados por un espacio (a b c): ");
    scanf("%lf %lf %lf", &a, &b, &c);
    printf("Ingrese los coeficientes de la segunda función separados por un espacio (d e f): ");
    scanf("%lf %lf %lf", &d, &e, &f);
    printf("Ingrese el porcentaje de error deseado (de 0 a 100): ");
    scanf("%lf", &porcentaje_error);

    // Abrir archivo para guardar datos necesarios para graficar en GNUplot.
    gnuplot_data = fopen("data.txt", "w");
    if (gnuplot_data == NULL) {
        perror("Error al abrir el archivo para gnuplot.");
        return 1; // Terminar la ejecucion ante cualquier error.
    }

    // Calcular el subdominio y escribir los datos en los archivos para la graficacion de GNUplot
    fprintf(gnuplot_data, "# x y1 y2 within_error\n");

    double subdominios[100][2]; // Se crea un arreglo para guardar los subdominios encontrados para vada iteracion por realizar.
    int contador_subdominio = 0; // Establecer un contador para iniciar a crear el subdominio.

    // Iterar sobre cada punto del ranfo para encontrar el subdominio requerido.
    for (x = DOMAIN_MIN; x <= DOMAIN_MAX; x += STEP) {

        // Evaluar las funciones para cada punto.
        y1 = evaluar_funcion1(a, b, c, x);
        y2 = evaluar_funcion2(d, e, f, x);

        // Funcion para caluclar el error de acuerdo al enunciado y evitar una posible divisón por cero.
        if (fabs(y1) > 1e-6) {
            error = 100.0 * fabs(y1 - y2) / fabs(y1);
        } else {
            error = 100.0 * fabs(y1 - y2);
        }

        // Escribir los datos en el archivo para la graficacion en GNUplot.
        fprintf(gnuplot_data, "%lf %lf %lf %d\n", x, y1, y2, error <= porcentaje_error ? 1 : 0);

        // Identificar el subdominio valido donde el error no excede el porcentaje permitido.
        if (error <= porcentaje_error) {
            if (!dentro_subdominio) {
                subdominio_inicio = x;
                dentro_subdominio = 1;
            }
            subdominio_final = x;  // Se actualiza el final del subdominio.
        } else if (dentro_subdominio) {
            subdominios[contador_subdominio][0] = subdominio_inicio;
            subdominios[contador_subdominio][1] = subdominio_final;
            contador_subdominio++;
            dentro_subdominio = 0;
        }
    }

    if (dentro_subdominio) {
        subdominios[contador_subdominio][0] = subdominio_inicio;
        subdominios[contador_subdominio][1] = subdominio_final;
        contador_subdominio++;
    }

    fclose(gnuplot_data);

    // Mostrar el subdominio encontrado.
    if (contador_subdominio > 1) {
        double limite_inferior_subdominio = subdominios[0][1];
        double limite_superior_subdominio = subdominios[1][0];
        printf("Subdominio encontrado: [%.2f, %.2f]\n", limite_inferior_subdominio, limite_superior_subdominio);
    } else {
        printf("No hay suficientes subdominios para calcular las entradas de interes.\n"); // Imprimir en caso de existir error.
    }

    // Generar la grafica con GNUplot.
    FILE *gnuplot_script = fopen("plot.gnuplot", "w");
    if (gnuplot_script == NULL) {
        perror("Error al crear el script de gnuplot.");
        return 1;
    }

    // Atributos para la graficacion.
    fprintf(gnuplot_script,
            "set terminal qt size 800,600\n"
            "set title 'Comparación de Ecuaciones Cuadráticas'\n"
            "set xlabel 'x'\n"
            "set ylabel 'y'\n"
            "set yrange [-100:100]\n"
            "set grid\n"
            "plot 'data.txt' using 1:2 with lines title 'Función 1', \\\n"
            "     'data.txt' using 1:3 with lines title 'Función 2', \\\n"
            "     'data.txt' using 1:(($4 == 1) ? $2 : 1/0) with points pointtype 7 title 'Fuera del error'\n");
    fclose(gnuplot_script);

    // Ejecutar grafica de GNUplot.
    int ret = system("gnuplot -p plot.gnuplot");
    if (ret == -1) {
        perror("Error al ejecutar gnuplot."); // Mostrar error en caso de existirlo.
        return 1;
    }
    return 0; // Terminar el programa con exito.
}

