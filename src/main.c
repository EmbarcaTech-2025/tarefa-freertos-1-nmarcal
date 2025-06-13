#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "task.h"

// === Definições dos pinos ===
#define LED_RED_PIN       13
#define LED_GREEN_PIN     11
#define LED_BLUE_PIN      12
#define BUZZER_A_PIN      21
#define BUZZER_B_PIN      10
#define BUTTON_A_PIN      5
#define BUTTON_B_PIN      6

// === Handles das tarefas ===
TaskHandle_t xTaskHandleLED = NULL;
TaskHandle_t xTaskHandleBuzzer = NULL;

// === Inicialização dos GPIOs ===
void gpioInit() {
    // LEDs
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);

    // Buzzer
    gpio_init(BUZZER_A_PIN);
    gpio_set_dir(BUZZER_A_PIN, GPIO_OUT);

    gpio_init(BUZZER_B_PIN);
    gpio_set_dir(BUZZER_B_PIN, GPIO_OUT);

    // Botões
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN); // importante para evitar flutuação

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}

// === Controle do LED RGB ===
void setLEDColor(int r, int g, int b) {
    gpio_put(LED_RED_PIN, r);
    gpio_put(LED_GREEN_PIN, g);
    gpio_put(LED_BLUE_PIN, b);
}

// === Controle do buzzer ===
void beep() {
    gpio_put(BUZZER_A_PIN, 1);
    gpio_put(BUZZER_B_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_put(BUZZER_A_PIN, 0);
    gpio_put(BUZZER_B_PIN, 0);
}

// === Leitura dos botões ===
int readButtonA() {
    return !gpio_get(BUTTON_A_PIN); // botão com pull-up
}
int readButtonB() {
    return !gpio_get(BUTTON_B_PIN); // botão com pull-up
}

// === Tarefa do LED RGB ===
void vTaskLED(void *pvParameters) {
    while (1) {
        setLEDColor(1, 0, 0); vTaskDelay(pdMS_TO_TICKS(500));
        setLEDColor(0, 1, 0); vTaskDelay(pdMS_TO_TICKS(500));
        setLEDColor(0, 0, 1); vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// === Tarefa do Buzzer ===
void vTaskBuzzer(void *pvParameters) {
    while (1) {
        beep();
        vTaskDelay(pdMS_TO_TICKS(900));
    }
}

// === Tarefa dos botões ===
void vTaskButtons(void *pvParameters) {
    int prevA = 0, prevB = 0;

    while (1) {
        int btnA = readButtonA();
        int btnB = readButtonB();

        if (btnA && !prevA) {
            if (eTaskGetState(xTaskHandleLED) == eSuspended) {
                vTaskResume(xTaskHandleLED);
            } else {
                vTaskSuspend(xTaskHandleLED);
            }
        }

        if (btnB && !prevB) {
            if (eTaskGetState(xTaskHandleBuzzer) == eSuspended) {
                vTaskResume(xTaskHandleBuzzer);
            } else {
                vTaskSuspend(xTaskHandleBuzzer);
            }
        }

        prevA = btnA;
        prevB = btnB;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// === Função principal ===
int main() {
    stdio_init_all();
    gpioInit();

    // Cria tarefas
    xTaskCreate(vTaskLED, "LED", 256, NULL, 1, &xTaskHandleLED);
    xTaskCreate(vTaskBuzzer, "Buzzer", 256, NULL, 1, &xTaskHandleBuzzer);
    xTaskCreate(vTaskButtons, "Buttons", 256, NULL, 2, NULL);

    // Inicia FreeRTOS
    vTaskStartScheduler();

    while (1); // nunca deve chegar aqui
}