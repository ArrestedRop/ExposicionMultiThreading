#include <iostream>
#include <thread>
#include <string>
//1.META
void goal() 
{
    std::string msg = "Programa simple para ver el funcionamiento \n";
    msg += "de los hilos.";

    std::cout << msg << std::endl;
}

//2.DATA -> NO APLICAN EN ESTE EJEMPLO
//3.PROCESOS -> NO APLICAN EN ESTE EJEMPLO

//4.SALIDA
//Metodo que sera creado en el thread
void pruebaThread(int maxValue) 
{
    int count = 0;

    while(count++ < maxValue)
    {
        //Utilizamos el get_id para acceder al numero asignado al hilo
        std::cout<<"\nEL HILO["<<std::this_thread::get_id()<<"]ESTA TRABAJANDO...\n";
    }
    std::cout << "\nEl hilo ["<<std::this_thread::get_id()<<"] dejo de trabajar\n";
}

//5.NAVEGACION
int entradaNavegacion()
{
    int capValue;
    std::string msg = "\nOTORGUE LA CANTIDAD DE MUESTRAS QUE";
        msg += "QUIERE VER EN EL HILO O ESCRIBA 0 PARA TERMINAR\n";

    std::cout << msg;
    std::cin >> capValue;
    return capValue;
}

int main()
{
    int maxValue;

    goal();
    //Navegacion simple
    while(true)
    {   
        maxValue = entradaNavegacion();
        if (maxValue <= 0)
        {
            std::cout << "Saliendo del programa\n";
            break;
        }
        //Iniciamos el hilo de prueba
        std::thread hiloDePrueba([maxValue](){pruebaThread(maxValue);});
        //Unimos el hilo secundario al principal
        hiloDePrueba.join();
    }
    
    return 0;
}