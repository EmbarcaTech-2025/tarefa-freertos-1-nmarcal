
# Tarefa: Roteiro de FreeRTOS #1 - EmbarcaTech 2025

Autor: **Nícolas Augusto Pichinelli Marçal**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, Junho de 2025

---

## Objetivo do projeto

Desenvolver um sistema multitarefa embarcado usando a BitDogLab, programando com FreeRTOS em linguagem C no VSCode. Esse sistema deve controlar três periféricos da placa de forma concorrente:
- **LED RGB**: alterna ciclicamente entre vermelho, verde e azul.
- **Buzzer**: emite bipes periodicamente.
- **Dois botões**:
  - **Botão A**: suspende ou retoma a tarefa do LED.
  - **Botão B**: suspende ou retoma a tarefa do buzzer.
---

## Lógica 

Este sistema multitarefa foi desenvolvido para a **BitDogLab** utilizando **FreeRTOS** em linguagem **C**. O código roda no ambiente **VSCode** e realiza o controle concorrente de periféricos.

### Estrutura do Sistema

#### Tarefas Criadas
- **Tarefa do LED RGB**
  - Alterna ciclicamente entre vermelho, verde e azul a cada 500 ms.
  - Usa a função `setLEDColor` para controlar os pinos dos LEDs.

- **Tarefa do Buzzer**
  - Emite um bipe de 100 ms seguido de 900 ms de silêncio.
  - Usa a função `beep` para acionar os pinos do buzzer.

- **Tarefa dos Botões**
  - Monitora dois botões:
    - **Botão A**
      - Suspende ou retoma a tarefa do LED quando pressionado.
    - **Botão B**
      - Suspende ou retoma a tarefa do buzzer quando pressionado.
  - A leitura dos botões é feita com debounce simples, verificando mudanças de estado a cada 100 ms.

### Inicialização
- Configura GPIOs dos LEDs, buzzer e botões.
- Cria as três tarefas com `xTaskCreate`.
- Inicia o agendador do FreeRTOS com `vTaskStartScheduler`.

### Controle de Fluxo
- As tarefas do LED e do buzzer rodam em loop infinito com `vTaskDelay` para definir os tempos.
- A tarefa dos botões verifica o estado e controla as outras tarefas com `vTaskSuspend` e `vTaskResume`.

### Prioridade das Tarefas
- **Tarefa LED:** prioridade 1
- **Tarefa Buzzer:** prioridade 1
- **Tarefa Botões:** prioridade 2 (maior para garantir resposta rápida às entradas)
---

## Componentes:

![componentes_FreeRTOS](https://github.com/EmbarcaTech-2025/tarefa-freertos-1-nmarcal/blob/b772de47cfdf4ecfdf44aff78a742a12a1cbd766/assets/lista_componentes_FreeRTOS.jpg?raw=true)

---

## Execução:

1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código;

---

## 📜 Licença
MIT License - MIT GPL-3.0.

---
### sdkVersion 2.1.1

