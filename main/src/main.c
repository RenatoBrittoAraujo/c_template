#include <stdlib.h>
#include <controle.h>
#include <interface.h>
#include <proto.h>
#include <shared_util.h>
#include <time_util.h>

int main()
{
    set_env();
    set_level(LEVEL_DEBUG);
    set_time_wait_ignore(0);

    EstadoEstacionamento *e = NULL;
    while (1)
    {
        e = controla(e);
        e = processar_interface(e);
    }
}
