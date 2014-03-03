#include "monomer.h"
#include "gimcs/CSMAgent.h"
#include "gimcs/Mysql.h"

int main(void)
{
    Mysql mysql;
    mysql.SetDBArgs("localhost", "root", "huangk", "Monomer");
    CSMAgent ma(1, 0.9, 0.01);
    ma.LoadMemoryFromStorage(&mysql);

    Monomer mono("Monomer");
    mono.ConnectAgent(&ma);
    mono.Launch();

    ma.DeleteState(6);
    ma.DeleteState(9);

    ma.DumpMemoryToStorage(&mysql);
}
