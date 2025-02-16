//* Projeto final do EMBARCATECH 2024.2
//* Desenvolvido por: Késia Lais dos Santos Souza
//* Descrição: Projeto de um sistema para alertar os funcionários do caixa sobre a presença de produtos frios deixados por clientes, para que sejam recolhidos e devolvidos antes que se tornem avarias. O sistema é composto por um microcontrolador Raspberry Pi Pico, um display OLED SSD1306, dois botões de pressão, um LED vermelho e um LED verde. O sistema deve exibir mensagens no display OLED e acionar os LEDs de acordo com o estado dos botões de pressão. O botão A indica que há produtos frios no caixa, e o botão B indica que foi recolhido o produto. Quando o botão A é pressionado, o LED vermelho deve ser aceso e a mensagem "Frios no Caixa 3." deve ser exibida no display OLED. Quando o botão B é pressionado, o LED verde deve ser aceso e a mensagem "Frio Recolhido cx 3." deve ser exibida no display OLED. O sistema deve ser implementado em linguagem C e utilizando a biblioteca Pico SDK.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint LED_R = 13;
const uint LED_G = 11;
const uint BOTAO_A = 5;
const uint BOTAO_B = 6;

int main()
{
    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Configuração dos LEDs
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Configuração dos botões
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // Zerar o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    while (true) {
        // Verificar se o botão A está pressionado
        if (!gpio_get(BOTAO_A)) {
         // Ligar o LED do vermelho
            gpio_put(LED_R, 1);
            gpio_put(LED_G,0);

        // Exibir mensagem no display OLED
            ssd1306_draw_string(ssd, 0, 0, "Frios no Caixa 3.");
            render_on_display(ssd, &frame_area);
        }

        // Verificar se o botão B está pressionado
        if (!gpio_get(BOTAO_B)) {
            // Ligar o LED verde
            gpio_put(LED_G, 1);
            gpio_put(LED_R,0);

        // Exibir mensagem no display OLED
            ssd1306_draw_string(ssd, 0, 1, " Recolhido cx 3.");
            render_on_display(ssd, &frame_area);
        }

        sleep_ms(100); // Intervalo para evitar bouncing dos botões
    }

    return 0;
}
