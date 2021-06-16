#include "proceso.h"

Proceso::Proceso()
{

}

char Proceso::getOperation() const
{
    return operation;
}

void Proceso::setOperation(char value)
{
    operation = value;
}

int Proceso::getDigit1() const
{
    return digit1;
}

void Proceso::setDigit1(int value)
{
    digit1 = value;
}

int Proceso::getDigit2() const
{
    return digit2;
}

void Proceso::setDigit2(int value)
{
    digit2 = value;
}

int Proceso::getTimeMax() const
{
    return timeMax;
}

void Proceso::setTimeMax(int value)
{
    timeMax = value;
}

int Proceso::getId() const
{
    return id;
}

void Proceso::setId(int value)
{
    id = value;
}

int Proceso::getResult() const
{
    return result;
}

void Proceso::setResult(int value)
{
    result = value;
}

int Proceso::getTt() const
{
    return tt;
}

void Proceso::setTt(int value)
{
    tt = value;
}

int Proceso::getLlegada() const
{
    return llegada;
}

void Proceso::setLlegada(int value)
{
    llegada = value;
}

int Proceso::getFinal() const
{
    return final;
}

void Proceso::setFinal(int value)
{
    final = value;
}

int Proceso::getRetorno() const
{
    return retorno;
}

void Proceso::setRetorno(int value)
{
    retorno = value;
}

int Proceso::getRespuesta() const
{
    return respuesta;
}

void Proceso::setRespuesta(int value)
{
    respuesta = value;
}

int Proceso::getEspera() const
{
    return espera;
}

void Proceso::setEspera(int value)
{
    espera = value;
}

int Proceso::getServicio() const
{
    return servicio;
}

void Proceso::setServicio(int value)
{
    servicio = value;
}

bool Proceso::getFirst() const
{
    return first;
}

void Proceso::setFirst(bool value)
{
    first = value;
}

bool Proceso::getBlocked() const
{
    return blocked;
}

void Proceso::setBlocked(bool value)
{
    blocked = value;
}

bool Proceso::getError() const
{
    return error;
}

void Proceso::setError(bool value)
{
    error = value;
}

int Proceso::getContBlocked() const
{
    return contBlocked;
}

void Proceso::setContBlocked(int value)
{
    contBlocked = value;
}

bool Proceso::getExec() const
{
    return exec;
}

void Proceso::setExec(bool value)
{
    exec = value;
}

bool Proceso::getWorking() const
{
    return working;
}

void Proceso::setWorking(bool value)
{
    working = value;
}

int Proceso::getMemory() const
{
    return memory;
}

void Proceso::setMemory(int newMemory)
{
    memory = newMemory;
}
