#ifdef TEST_MODE

// CONFIG DE TESTE

#define BCM2835_GPIO_FSEL_OUTP 1
#define BCM2835_GPIO_FSEL_INPT 0

void bcm2835_gpio_fsel(uint8_t pin, uint8_t mode) { log_print("[BCM MOCK] bcm2835_gpio_fsel()\n", LEVEL_DEBUG); }
int bcm2835_close(void) { log_print("[BCM MOCK] bcm2835_close()\n", LEVEL_DEBUG); }
int bcm2835_init(void)
{
    log_print("[BCM MOCK] bcm2835_init()\n", LEVEL_DEBUG);
    return 1;
}
int bcm2835_gpio_lev(uint8_t pin) { log_print("[BCM MOCK] bcm2835_gpio_lev()\n", LEVEL_DEBUG); }
void bcm2835_gpio_write(uint8_t pin, uint8_t on) { log_print("[BCM MOCK] bcm2835_gpio_write()\n", LEVEL_DEBUG); }

#else

// CONFIG DE PROD

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <shared/inc/threads.h>
#include <shared/inc/tcp_ip.h>
#include <shared/inc/shared_util.h>
#include <shared/inc/time_util.h>

#endif

int main()
{
    int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY); // Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = 'H';
    *p_tx_buffer++ = 'e';
    *p_tx_buffer++ = 'l';
    *p_tx_buffer++ = 'l';
    *p_tx_buffer++ = 'o';

    printf("Buffers de memória criados!\n");

    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer, (p_tx_buffer - &tx_buffer[0]));
        if (count <= 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void *)rx_buffer, 255); // Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            printf("Erro na leitura.\n"); // An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            printf("Nenhum dado disponível.\n"); // No data waiting
        }
        else
        {
            // Bytes received
            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos : %s\n", rx_length, rx_buffer);
        }
    }

    close(uart0_filestream);
    return 0;
}
