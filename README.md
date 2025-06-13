
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
  - Emite um bipe de 100 ms a cada 1 segundo.
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

## Análise do Sistema Multitarefa com FreeRTOS

### O que acontece se todas as tarefas tiverem a mesma prioridade?

Se todas as tarefas tiverem a mesma prioridade no FreeRTOS:

- **Compartilhamento do tempo**: o escalonador usará o método round-robin para alternar entre as tarefas quando houver mais de uma pronta para executar.
- **Respeito ao vTaskDelay**: no caso deste projeto, como todas as tarefas usam `vTaskDelay`, o sistema se comporta corretamente porque as tarefas voluntariamente liberam o processador ao entrar em estado bloqueado (delay).
- **Sem preempção por prioridade**: nenhuma tarefa teria preferência sobre outra em situações onde duas ou mais estão prontas ao mesmo tempo, o que pode causar uma resposta mais lenta a eventos críticos, como pressionamento de botões.

### Qual tarefa consome mais tempo da CPU?

No projeto apresentado:

- A **tarefa dos botões** (`vTaskButtons`) tende a consumir mais tempo da CPU em termos de frequência de execução porque:
  - Ela roda em um laço com delay de apenas 100 ms, verificando constantemente os estados dos botões.
  - As tarefas do LED e do buzzer passam mais tempo bloqueadas (`vTaskDelay` de 500 ms e 900 ms, respectivamente).
- Apesar de consumir mais tempo ativo, o processamento da tarefa dos botões é leve (apenas leitura de GPIOs e controle de tarefas).

### Quais seriam os riscos de usar polling sem prioridades?

Se o sistema usasse apenas polling sem prioridades:

- **Baixa responsividade**: eventos críticos (como o acionamento de um botão) poderiam demorar a ser detectados, já que o código precisaria percorrer rotinas de polling de todos os periféricos até encontrar o evento.
- **Desperdício de CPU**: polling contínuo sem `vTaskDelay` ou `yield` faria o processador trabalhar o tempo todo, consumindo energia desnecessária e podendo prejudicar o desempenho em aplicações embarcadas de baixo consumo.
- **Fome de tarefas (starvation)**: sem o uso de prioridades e preempção, tarefas importantes poderiam não ter tempo adequado de execução se uma tarefa de polling monopolizasse o processador.
- **Dificuldade de escalabilidade**: aumentar o número de periféricos ou tarefas complicaria a manutenção do código e reduziria ainda mais a eficiência.

### Conclusão

O uso de prioridades e `vTaskDelay` no FreeRTOS melhora o desempenho e a eficiência do sistema, permitindo controle adequado do tempo da CPU e resposta a eventos assíncronos como o pressionamento de botões.


---

## 📜 Licença
MIT License - MIT GPL-3.0.

---
### sdkVersion 2.1.1

