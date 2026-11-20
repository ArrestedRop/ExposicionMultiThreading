// ESTE CODIGO ESTA HECHO MAL A PROPOSITO,
// LO REALIZAMOS CON LA PRIMICIA DE QUE LA TERMINAL SE VOLVERA LOCA PARA 
// ASI DEMOSTRAR LAS CAPACIDADES QUE TIENE UN THREAD DE MATAR EL RENDIMIENTO
// DE UN DISPOSITIVO
// PARA VER LA VERSION LIMPIA DONDE SE MANEJAN VARIOS THREADS PORFAVOR VISITAR
// EL ARCHIVO PruebaComplex.cpp
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
        std::cout<<"\nEL HILO["<<std::this_thread::get_id()<<"]ESTA TRABAJANDO..." << std::endl;
    }
    std::cout << "\nEl hilo ["<<std::this_thread::get_id()<<"] dejo de trabajar" << std::endl;
}

//5.NAVEGACION
int entradaNavegacion()
{
    int capValue;
    std::string msg = "OTORGUE LA CANTIDAD DE MUESTRAS QUE";
        msg += "QUIERE VER EN EL HILO O ESCRIBA 0 PARA TERMINAR";

    std::cout << msg << std::endl;
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
        //Iniciamos un hilo de prueba
        std::thread hiloDePrueba([maxValue](){pruebaThread(maxValue);});
        //Separamos los hilos secundario al principal
        hiloDePrueba.detach();
    }
    
    return 0;
}