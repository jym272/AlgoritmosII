
Informe

El informe arranca directamente mencionando diferencias respecto al TP 0. Tengan cuidado con esto: deberían tener una estructura más ordenada y plantear una introducción al problema y luego pasar a discutir las cuestiones de implementación y diseño. Las diferencias respecto del TP 0 deberían aparecer en todo caso en una subsección en este punto.

Página 10: se pone una cota asintótica O(n) para SY pero no se indica a qué refiere n. El símbolo ≃ que usan convendría reemplazarlo por = (la igualdad matemática vale perfectamente en esos casos, y además a priori no está claro el significado de ≃ para comparar asintóticos).

Página 11: “chequear esto último” era algo interno de ustedes?

Página 12: ídem variables en notación asintótica. Además hay una mezcla entre n y N. 

En el análisis de Karatsuba, cuáles son las otras operaciones de costo lineal que se hacen? Cuántas son?

Peor caso de todo el programa: qué pasa si el input está formado por N operaciones distintas y en todas ellas los operandos tienen θ(N) dígitos? Qué pasa si hay una operación con N multiplicaciones que involucran operandos de θ(N) dígitos? Y si las N operaciones son todas como esta última?

Programa

-----> Con -h sale el help del TP 0. <<<COMPLETADO
/*  cout << "tp1 [-i file] [-o file] [-m standard | karatsuba]"
/*

Código

-----> Por qué tienen una precisión máxima? El TP debería operar con una precisión virtualmente infinita (agregando dígitos por demanda).<<<COMPLETADO

La unica imposicion que se tiene en el maximo es la memoria del sistema, ya que bignum usa un array de unsigned short, c++ puede No, C++ does not impose any limits for the dimensions of an array. But as the array has to be stored somewhere in memory, so memory-related limits imposed by other parts of the computer system app.

de la misma forma el string que se usa para procesar la cadena de caracters en istream y oustream 
There is no official limit on the size of a string. The software will ask your system for memory and, as long as it gets it, it will be able to add characters to your string.

Se habia colocado un apreciso maxima debido a los test, para un numero extremadamente alto 100000, el programa se vuelve ineficiente 
para realizar operaciones, esta restriccion se elimina. El unico limite ahora es la memoria del sistema.

----->Por qué los operadores para comparar bignums son privados?<<<COMPLETADO
en primera instancia eran usados unicamente por el metodo de division de bignum, pero ahora son publicas para que el usuario
pueda comparar bignums agregandole funcionalidad al programa.

----->operator>>: no está del todo bien (si uno lo quisiera usar en otro contexto fuera de lo que es el TP, no funcionaría). En resumen, debería poder leer e interpretar un número a partir de un stream de entrada sin tener ningún tipo de información de contexto. <<<COMPLETADO
El operator>> recibe un stream de entrada, itera sobre estos elementos en busca de un bignum valido, de otra manera
guarda un cero en el bignum 
   * Cualquier argumento incorrecto guarda un bignum 0.
    * Ej: -12 --> -12   /  -123aqw  ---> -123  /  -q1233 ---> 0 


El Strategy no está bien aprovechado en el TP. El operator* repite código y define ahí mismo el algoritmo a utilizar, cuando esto podría estar definido de antemano una única vez (puesto que la opción viene al momento de iniciar el programa). Una forma de mejorar el diseño es instanciar a los bignums con el Product a utilizar, y que el operator* simplemente delegue su ejecución en este objeto. También llevaría el código de cada algoritmo al objeto correspondiente, por una cuestión de honrar la estructura de las clases y la semántica que le queremos dar a los objetos.

----->La clase precision no parece tener un buen nombre (dadas las responsabilidades que tiene y las tareas que lleva a cabo).<<<COMPLETADO
Se cambia el nombre de la clase a decodificar usando el algoritmo de shunting yard.
   decodificar(istream &,ostream &);
    ~decodificar();
    bool shunting_yard();