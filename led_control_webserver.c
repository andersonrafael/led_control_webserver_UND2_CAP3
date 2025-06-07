//****************************************************
//****************************************************
//***************** COMNUNICAÇÃO HTTP ****************
//****************************************************
//****************************************************

// Autor: Francisco Anderson Rafael da Silva
// Data: 20/10/2023
// Projeto: Controle de LEDs via Web Server

/* #include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/netif.h" // Para acessar netif_default e IP

// Configurações de Wi-Fi
#define WIFI_SSID "RAFAEL 2G"
#define WIFI_PASSWORD "cLzV5tyWq1"

// Definição dos pinos dos LEDs
#define LED_PIN CYW43_WL_GPIO_LED_PIN
#define LED_BLUE_PIN 12  // GPIO12 - LED azul
#define LED_GREEN_PIN 11 // GPIO11 - LED verde
#define LED_RED_PIN 13   // GPIO13 - LED vermelho

// Função de callback para processar requisições HTTP
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    char *request = (char *)malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = '\0';

    printf("Request: %s\n", request);

    // Controle dos LEDs
    if (strstr(request, "GET /blue_on") != NULL)
    {
        gpio_put(LED_BLUE_PIN, 1);
    }
    else if (strstr(request, "GET /blue_off") != NULL)
    {
        gpio_put(LED_BLUE_PIN, 0);
    }
    else if (strstr(request, "GET /green_on") != NULL)
    {
        gpio_put(LED_GREEN_PIN, 1);
    }
    else if (strstr(request, "GET /green_off") != NULL)
    {
        gpio_put(LED_GREEN_PIN, 0);
    }
    else if (strstr(request, "GET /red_on") != NULL)
    {
        gpio_put(LED_RED_PIN, 1);
    }
    else if (strstr(request, "GET /red_off") != NULL)
    {
        gpio_put(LED_RED_PIN, 0);
    }
    else if (strstr(request, "GET /on") != NULL)
    {
        cyw43_arch_gpio_put(LED_PIN, 1);
    }
    else if (strstr(request, "GET /off") != NULL)
    {
        cyw43_arch_gpio_put(LED_PIN, 0);
    }

    // Leitura da temperatura interna
    adc_select_input(4);
    uint16_t raw_value = adc_read();
    const float conversion_factor = 3.3f / (1 << 12);
    float temperature = 27.0f - ((raw_value * conversion_factor) - 0.706f) / 0.001721f;

    // Cria a resposta HTML
    char html[1024];

    snprintf(html, sizeof(html),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<!DOCTYPE html>\n"
             "<html>\n"
             "<head>\n"
             "<title>LED Control</title>\n"
             "<style>\n"
             "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
             "h1 { font-size: 64px; margin-bottom: 30px; }\n"
             "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
             ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<h1>LED Control</h1>\n"
             "<form action=\"./blue_on\"><button>Ligar Azul</button></form>\n"
             "<form action=\"./blue_off\"><button>Desligar Azul</button></form>\n"
             "<form action=\"./green_on\"><button>Ligar Verde</button></form>\n"
             "<form action=\"./green_off\"><button>Desligar Verde</button></form>\n"
             "<form action=\"./red_on\"><button>Ligar Vermelho</button></form>\n"
             "<form action=\"./red_off\"><button>Desligar Vermelho</button></form>\n"
             "<p class=\"temperature\">Temperatura Interna: %.2f &deg;C</p>\n"
             "</body>\n"
             "</html>\n",
             temperature);

    /*       snprintf(html, sizeof(html),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<!DOCTYPE html>\n"
             "<html>\n"
             "<head>\n"
             "<title>LED Control</title>\n"
             "<style>\n"
             "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
             "h1 { font-size: 64px; margin-bottom: 30px; }\n"
             "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
             ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<h1>LED Control</h1>\n"
             "<form action=\"./blue_on\"><button style=\"background-color: #2196F3; color: white;\">Ligar Azul</button></form>\n"
             "<form action=\"./blue_off\"><button style=\"background-color: #BBDEFB; color: black;\">Desligar Azul</button></form>\n"
             "<form action=\"./green_on\"><button style=\"background-color: #4CAF50; color: white;\">Ligar Verde</button></form>\n"
             "<form action=\"./green_off\"><button style=\"background-color: #C8E6C9; color: black;\">Desligar Verde</button></form>\n"
             "<form action=\"./red_on\"><button style=\"background-color: #F44336; color: white;\">Ligar Vermelho</button></form>\n"
             "<form action=\"./red_off\"><button style=\"background-color: #FFCDD2; color: black;\">Desligar Vermelho</button></form>\n"
             "<p class=\"temperature\">Temperatura Interna: %.2f &deg;C</p>\n"
             "</body>\n"
             "</html>\n",
             temperature);
    */

/*
tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
tcp_output(tpcb);

free(request);
pbuf_free(p);

return ERR_OK;
}

// Função de callback ao aceitar conexões TCP
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
tcp_recv(newpcb, tcp_server_recv);
return ERR_OK;
}

// Função principal
int main()
{
stdio_init_all();

// Configuração dos LEDs como saída
gpio_init(LED_BLUE_PIN);
gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
gpio_put(LED_BLUE_PIN, false);

gpio_init(LED_GREEN_PIN);
gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
gpio_put(LED_GREEN_PIN, false);

gpio_init(LED_RED_PIN);
gpio_set_dir(LED_RED_PIN, GPIO_OUT);
gpio_put(LED_RED_PIN, false);

// cyw43_arch_deinit(); // Desativa o Wi-Fi

while (cyw43_arch_init())
{
    printf("Falha ao inicializar Wi-Fi\n");
    sleep_ms(100);
    return -1;
}

cyw43_arch_gpio_put(LED_PIN, 0);
cyw43_arch_enable_sta_mode();

printf("Conectando ao Wi-Fi...\n");
while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000))
{
    printf("Falha ao conectar ao Wi-Fi\n");
    sleep_ms(100);
    return -1;
}

printf("Conectado ao Wi-Fi\n");

if (netif_default)
{
    printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
}

// Configura o servidor TCP
struct tcp_pcb *server = tcp_new();
if (!server)
{
    printf("Falha ao criar servidor TCP\n");
    return -1;
}

if (tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK)
{
    printf("Falha ao associar servidor TCP à porta 80\n");
    return -1;
}

server = tcp_listen(server);
tcp_accept(server, tcp_server_accept);

printf("Servidor ouvindo na porta 80\n");

// Inicializa o ADC
adc_init();
adc_set_temp_sensor_enabled(true);

while (true)
{
    cyw43_arch_poll();
}

cyw43_arch_deinit();
return 0;
}

*/



//    ********************************
//    ********************************

//     UNIDADE 2 CAPITULO 3 - PROJETO SENSOR TEMPERATURA

//    ********************************
//    ********************************


#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <stdio.h>
#include <string.h>

#include "lwip/pbuf.h"
#include "lwip/udp.h"

// --- CONFIGURAÇÕES ---
//  rede Wi-Fi
#define WIFI_SSID "RAFAEL 2G"
#define WIFI_PASSWORD "cLzV5tyWq1"

//  IP do computador
#define SERVER_IP "192.168.100.19"
#define SERVER_PORT 1234 // Porta para comunicação
#define POLL_TIME_S 1  // Intervalo de envio em segundos

// --- PINOS DO HARDWARE (BitDogLab) ---
#define BUTTON_PIN 22 // Pino do botão na placa BitDogLab

// Variáveis globais para o UDP
static struct udp_pcb *udp_pcb;
ip_addr_t server_addr;

// Função para enviar os dados via UDP
void send_sensor_data()
{
    // 1. Ler o status do botão
    bool is_button_pressed = !gpio_get(BUTTON_PIN);

    // 2. Ler a temperatura interna
    adc_select_input(4);
    uint16_t raw_value = adc_read();
    const float conversion_factor = 3.3f / (1 << 12);
    float temperature = 27.0f - ((raw_value * conversion_factor) - 0.706f) / 0.001721f;

    // 3. Cria pacote de dados (payload) em formato JSON
    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"botao_pressionado\": %s, \"temperatura_c\": %.2f}",
             is_button_pressed ? "true" : "false",
             temperature);

    // 4. Alocar um buffer para o pacote UDP
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, strlen(payload), PBUF_RAM);
    if (p != NULL)
    {
        // Copiar os dados para o buffer
        memcpy(p->payload, payload, strlen(payload));

        // Enviar o pacote para o servidor
        udp_sendto(udp_pcb, p, &server_addr, SERVER_PORT);

        // Liberar o buffer
        pbuf_free(p);

        printf("Dados enviados: %s\n", payload);
    }
    else
    {
        printf("Erro ao alocar buffer UDP\n");
    }
}

int main()
{
    stdio_init_all();

    // Inicializa o Wi-Fi com o código do país (Brasil)
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL))
    {
        printf("Falha ao inicializar Wi-Fi\n");
        return -1;
    }
    printf("Wi-Fi inicializado.\n");

    cyw43_arch_enable_sta_mode();

    // Conecta à rede Wi-Fi
    printf("Conectando ao Wi-Fi '%s'...\n", WIFI_SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Falha ao conectar.\n");
        return -1;
    }
    printf("Conectado com sucesso!\n");
    printf("IP da Placa: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));

    // Configuração do pino do botão
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Habilita resistor de pull-up interno

    // Configuração do ADC para o sensor de temperatura
    adc_init();
    adc_set_temp_sensor_enabled(true);

    // Configuração do cliente UDP
    udp_pcb = udp_new();
    ipaddr_aton(SERVER_IP, &server_addr);

    // Loop principal
    absolute_time_t next_poll_time = get_absolute_time();
    while (true)
    {
        // Tarefa que deve ser executada periodicamente para manter o Wi-Fi
        cyw43_arch_poll();

        // Verifica se já passou o tempo para o próximo envio
        if (absolute_time_diff_us(get_absolute_time(), next_poll_time) < 0)
        {
            send_sensor_data(); // Chama a função de envio
            // Agenda o próximo envio
            next_poll_time = make_timeout_time_ms(POLL_TIME_S * 1000);
        }
    }

    cyw43_arch_deinit();
    return 0;
}



//****************************************************
//****************************************************
//    UNIDADE 2 CAPITULO  - JOYSTICK CONTROLLER
//****************************************************
//****************************************************
// Autor: Francisco Anderson Rafael da Silva

/*
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include <stdio.h>
#include <string.h>

#include "lwip/pbuf.h"
#include "lwip/udp.h"

// --- CONFIGURAÇÕES ---
//  rede Wi-Fi
#define WIFI_SSID "RAFAEL 2G"
#define WIFI_PASSWORD "cLzV5tyWq1"

//  IP do computador
#define SERVER_IP "192.168.100.19"
#define SERVER_PORT 12345
#define POLL_TIME_MS 200 // Intervalo de envio em milissegundos (5x por segundo)

// --- PINOS DO HARDWARE ---
#define JOYSTICK_X_PIN 26 // ADC0 para o eixo X
#define JOYSTICK_Y_PIN 27 // ADC1 para o eixo Y

// Variáveis globais para o UDP
static struct udp_pcb *udp_pcb;
ip_addr_t server_addr;

// ⭐ FUNÇÃO DO DESAFIO: Converte a posição X/Y em uma direção da Rosa dos Ventos
const char *get_compass_direction(uint16_t x, uint16_t y)
{
    // O ADC de 12 bits vai de 0 a 4095. O centro é ~2048.
    // Criamos uma "zona morta" para o joystick em repouso.
    const int threshold = 500;
    const int center = 2048;
    const int low_limit = center - threshold;
    const int high_limit = center + threshold;

    bool north = y > high_limit;
    bool south = y < low_limit;
    bool east = x > high_limit;
    bool west = x < low_limit;

    if (north && east)
        return "Nordeste";
    if (north && west)
        return "Noroeste";
    if (south && east)
        return "Sudeste";
    if (south && west)
        return "Sudoeste";
    if (north)
        return "Norte";
    if (south)
        return "Sul";
    if (east)
        return "Leste";
    if (west)
        return "Oeste";

    return "Centro";
}

// Função para enviar os dados via UDP
void send_joystick_data()
{
    // 1. Ler os valores analógicos do Joystick
    adc_select_input(0); // Seleciona ADC0 (GP26)
    uint16_t raw_x = adc_read();

    adc_select_input(1); // Seleciona ADC1 (GP27)
    uint16_t raw_y = adc_read();

    // 2. Obter a direção da rosa dos ventos
    const char *direction = get_compass_direction(raw_x, raw_y);

    // 3. Criar o pacote de dados (payload) em formato JSON
    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"x\": %d, \"y\": %d, \"direcao\": \"%s\"}",
             raw_x, raw_y, direction);

    // 4. Enviar o pacote via UDP
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, strlen(payload), PBUF_RAM);
    if (p != NULL)
    {
        memcpy(p->payload, payload, strlen(payload));
        udp_sendto(udp_pcb, p, &server_addr, SERVER_PORT);
        pbuf_free(p);
        printf("Enviado: %s\n", payload);
    }
}

int main()
{
    stdio_init_all();

    // Inicializa o ADC
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Conecta ao Wi-Fi
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL))
    {
        printf("Falha ao inicializar Wi-Fi\n");
        return -1;
    }
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Falha ao conectar ao Wi-Fi.\n");
        return -1;
    }
    printf("Conectado! IP da Placa: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));

    // Configura o cliente UDP
    udp_pcb = udp_new();
    ipaddr_aton(SERVER_IP, &server_addr);

    // Loop principal
    absolute_time_t next_poll_time = get_absolute_time();
    while (true)
    {
        cyw43_arch_poll(); // Mantém a conexão Wi-Fi ativa

        if (absolute_time_diff_us(get_absolute_time(), next_poll_time) < 0)
        {
            send_joystick_data();
            next_poll_time = make_timeout_time_ms(POLL_TIME_MS);
        }
    }

    cyw43_arch_deinit();
    return 0;
}


*/