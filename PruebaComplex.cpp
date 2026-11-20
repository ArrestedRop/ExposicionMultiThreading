#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <string>

std::queue<std::string> colaEventos;
std::mutex mtx;
std::condition_variable cv;
bool finalizado = false;
//1. META
void meta() 
{
    std::cout << "Programa que muestra un uso de hilos mas complejo\n";
    std::cout << "utilizando herramientas como: \n";
    std::cout << "|chrono| -> Utilizado para congelar un hilo";
    std::cout << "|mutex| -> Se utiliza para bloquear la impresion ";
    std::cout << "|condition_variable| -> Se usa para apagar los hilos";
    std::cout << "|queue| -> Estructura FIFO (Primero en entrar, primero en salir)";
}
//2.DATOS -> NO APLICAN EN ESTE EJEMPLO
//3. PROCESOS
void productor()
{
    std::string eventos[] = {"RPM: 3500", "Temp: 90C", 
                             "Freno: ON", "Presion: 2.1bar"};
    
    for (const auto& evento : eventos)
    {
        // Simulamos lectura de hardware
        std::this_thread::sleep_for(std::chrono::milliseconds(10000)); 
        {
            // Bloqueamos la cola para escribir de forma segura
            std::lock_guard<std::mutex> lock(mtx);
            colaEventos.push(evento);
            std::cout << "[Sensor] Dato capturado y encolado: " << evento;
            std::cout << "\n";
        } 
        // Desbloqueamos el mutex ANTES de notificar
        cv.notify_one(); //Despierta a un hilo consumidor que esté durmiendo
    }
    // Proceso de apagado seguro
    {
        std::lock_guard<std::mutex> lock(mtx);
        finalizado = true;
    }
    cv.notify_all(); // Despertamos a todos para que vean que ya se acabó
}

//4.- Salida
void consumidorProcesador() {
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        // El hilo se congela aquí y suelta el mutex. No consume CPU.
        // Solo despierta si: 1) Recibe un notify() Y 2)
        // La cola no está vacía o ya finalizamos.
        cv.wait(lock, [] { return !colaEventos.empty() || finalizado;});
        // Condición de salida:
        // Si terminamos y la cola está vacía, rompemos el ciclo
        if (finalizado && colaEventos.empty())
        {
            break;
        }
        // Extraemos el evento de la cola
        std::string eventoActual = colaEventos.front();
        colaEventos.pop();
        // Soltamos el candado para
        //Asi no bloqueamos al productor mientras procesamos el dato
        lock.unlock();

        std::cout << "   [Procesador " << std::this_thread::get_id() << "]";
        std::cout << "Calculando fisicas para -> " << eventoActual << "\n";
        // Se simula procesamiento pesado
        std::this_thread::sleep_for(std::chrono::milliseconds(400)); 
    }
}

int main() {
    std::cout << "Iniciando sistema de bus de eventos " ;
    std::cout << "(Productor-Consumidor)...\n\n";
    // Levantamos los hilos
    std::thread hiloProductor(productor);
    std::thread hiloConsumidor1(consumidorProcesador);
    // Un segundo consumidor para mayor paralelismo
    std::thread hiloConsumidor2(consumidorProcesador); 

    // Esperamos a que la rutina termine
    hiloProductor.join();
    hiloConsumidor1.join();
    hiloConsumidor2.join();

    std::cout << "\nSistema apagado correctamente.\n";
    std::cout << "Sin fugas de memoria ni hilos huerfanos.";

    return 0;
}

//5. NAVEGACION -> NO APLICAN EN ESTE EJEMPLO