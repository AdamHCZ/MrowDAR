# MrowDAR - Avance Semanal del Proyecto

## Descripción General del Proyecto

**MrowDAR** es un proyecto de sistemas embebidos orientado al desarrollo de una plataforma de adquisición y comunicación de datos usando un **microcontrolador STM32L432KC**, un sensor **MPU6050**, un módulo **ESP32**, comunicación **I2C**, comunicación **UART** y un programa en **Python** para lectura y almacenamiento de datos.

Hasta esta etapa, el proyecto integra tres partes principales:

1. **Firmware STM32**

   * Configuración del MPU6050 mediante I2C.
   * Lectura de datos del acelerómetro.
   * Transmisión de valores por UART.
   * Uso de interrupciones por temporizador.
   * Separación parcial de funciones en archivos auxiliares.

2. **Programas de apoyo para ESP32**

   * Lectura de valores RSSI de una red Wi-Fi.
   * Comunicación UART entre ESP32 y STM32.

3. **Programa de lectura en Python**

   * Recepción de datos seriales enviados desde el STM32.
   * Almacenamiento de valores del acelerómetro en un archivo CSV.

Este documento resume el avance realizado durante la semana y describe la funcionalidad implementada hasta el momento.

---

## Archivos Relevantes del Proyecto

Los archivos más importantes para el avance actual son los siguientes:

| Archivo                                                                                                                               | Descripción                                                                                                                                                       |
| ------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`Src/Mrowdar.c`](https://github.com/AdamHCZ/MrowDAR/blob/master/Src/Mrowdar.c)                                                       | Archivo principal del firmware STM32. Contiene la configuración de I2C, UART, temporizadores, inicialización del MPU6050 y transmisión de datos del acelerómetro. |
| [`Inc/main.h`](https://github.com/AdamHCZ/MrowDAR/blob/master/Inc/main.h)                                                             | Archivo de cabecera principal. Contiene definiciones generales del STM32, configuraciones del OLED y registros del MPU6050.                                       |
| [`Inc/functions.h`](https://github.com/AdamHCZ/MrowDAR/blob/master/Inc/functions.h)                                                   | Archivo de cabecera para funciones reutilizables de configuración de periféricos.                                                                                 |
| [`Src/functions.c`](https://github.com/AdamHCZ/MrowDAR/blob/master/Src/functions.c)                                                   | Archivo fuente con funciones reutilizables para GPIO, ADC, PWM, temporizadores e interrupciones.                                                                  |
| [`ESP32_Config/RSSI_Values/RSSI_Values.ino`](https://github.com/AdamHCZ/MrowDAR/blob/master/ESP32_Config/RSSI_Values/RSSI_Values.ino) | Programa del ESP32 para conectarse a una red Wi-Fi y leer valores RSSI.                                                                                           |
| [`ESP32_Config/UART_CODE/UART_CODE.ino`](https://github.com/AdamHCZ/MrowDAR/blob/master/ESP32_Config/UART_CODE/UART_CODE.ino)         | Programa del ESP32 para comunicación UART con el STM32.                                                                                                           |
| [`Lecture_Program/Lecture_cv.py`](https://github.com/AdamHCZ/MrowDAR/blob/master/Lecture_Program/Lecture_cv.py)                       | Script en Python para leer datos seriales y guardarlos en un archivo CSV.                                                                                         |
| [`Lecture_Program/Accelerometer_data.csv`](https://github.com/AdamHCZ/MrowDAR/blob/master/Lecture_Program/Accelerometer_data.csv)     | Archivo CSV destinado al almacenamiento de los datos del acelerómetro.                                                                                            |

---

## Resumen del Avance Semanal

Durante esta semana, el proyecto avanzó desde pruebas individuales de periféricos hacia una estructura más integrada. Actualmente, el STM32 puede comunicarse con el sensor MPU6050 mediante I2C, obtener datos del acelerómetro, convertir los datos crudos en valores firmados y transmitirlos por UART en un formato compatible con CSV.

También se añadieron programas de prueba para el ESP32. Uno permite leer el valor RSSI de una red Wi-Fi, mientras que el otro permite probar comunicación UART entre el ESP32 y el STM32. Además, se agregó un script en Python para recibir datos desde el puerto serial y almacenarlos en un archivo CSV.

---

## Avances en el Firmware STM32

### Archivo Principal: `Mrowdar.c`

El archivo `Src/Mrowdar.c` es actualmente el archivo principal del firmware del STM32. En este archivo se implementaron las siguientes funcionalidades:

---

### 1. Inicialización del Sistema

El programa inicializa la librería HAL de STM32 y configura el reloj del sistema. La configuración del reloj utiliza el PLL con MSI como fuente, permitiendo que el sistema trabaje a una frecuencia de 80 MHz.

Flujo principal de inicialización:

```c
HAL_Init();
SystemClock_Config();
```

---

### 2. Configuración I2C para el MPU6050

Se configuró el periférico **I2C1** para la comunicación con el sensor MPU6050.

Pines configurados:

| Pin STM32 | Función  |
| --------- | -------- |
| PB6       | I2C1_SCL |
| PB7       | I2C1_SDA |

La inicialización se realiza mediante:

```c
MX_I2C_MspInit();
MX_I2C1_Init();
```

Esto permite que el STM32 pueda leer y escribir registros internos del MPU6050 usando funciones HAL como:

```c
HAL_I2C_Mem_Read();
HAL_I2C_Mem_Write();
```

---

### 3. Definición de Registros del MPU6050

Los registros principales del MPU6050 se encuentran definidos en `Inc/main.h`.

Registros importantes utilizados actualmente:

| Registro                | Dirección | Función                                    |
| ----------------------- | --------: | ------------------------------------------ |
| `MPU6050_REG_WHO_AM_I`  |    `0x75` | Registro de identificación del dispositivo |
| `MPU6050_REG_PWR_MGMT1` |    `0x6B` | Registro de administración de energía      |
| `MPU6050_REG_SMPLRT`    |    `0x19` | Divisor de frecuencia de muestreo          |
| `MPU6050_REG_CONFIG`    |    `0x1A` | Configuración del filtro digital           |
| `MPU6050_REG_GYRO_CFG`  |    `0x1B` | Configuración del giroscopio               |
| `MPU6050_REG_ACCEL_CFG` |    `0x1C` | Configuración del acelerómetro             |
| `MPU6050_REG_ACCEL_XH`  |    `0x3B` | Primer registro de salida del acelerómetro |
| `MPU6050_REG_GYRO_XH`   |    `0x43` | Primer registro de salida del giroscopio   |

La dirección I2C del MPU6050 se define como:

```c
#define MPU6050_ADDR (0x68 << 1)
```

Esta forma es adecuada para las funciones HAL de STM32, ya que estas esperan la dirección I2C desplazada un bit hacia la izquierda.

---

### 4. Inicialización del MPU6050

La función `MPU6050_Init()` configura el sensor escribiendo valores en sus registros internos.

Configuración actual:

| Configuración                  | Registro       |  Valor | Descripción                                    |
| ------------------------------ | -------------- | -----: | ---------------------------------------------- |
| Activación del sensor          | `PWR_MGMT_1`   | `0x00` | Desactiva el modo sleep                        |
| Frecuencia de muestreo         | `SMPLRT_DIV`   | `0x07` | Configura el divisor de muestreo               |
| Filtro digital                 | `CONFIG`       | `0x00` | Configura el filtro digital interno            |
| Configuración del giroscopio   | `GYRO_CONFIG`  | `0x08` | Define el rango del giroscopio                 |
| Configuración del acelerómetro | `ACCEL_CONFIG` | `0x00` | Configura el acelerómetro en rango por defecto |

Esta configuración permite preparar el MPU6050 para entregar datos del acelerómetro mediante el bus I2C.

---

### 5. Lectura de Datos del Acelerómetro

La función `MPU6050_Values()` realiza la lectura de seis bytes desde el registro:

```c
MPU6050_REG_ACCEL_XH
```

Los seis bytes corresponden a los valores de aceleración en los tres ejes:

| Bytes                        | Dato                 |
| ---------------------------- | -------------------- |
| `raw_data[0]`, `raw_data[1]` | Aceleración en eje X |
| `raw_data[2]`, `raw_data[3]` | Aceleración en eje Y |
| `raw_data[4]`, `raw_data[5]` | Aceleración en eje Z |

Luego, los bytes se convierten en valores enteros con signo de 16 bits:

```c
x_acc = ((int16_t)raw_data[0] << 8) + raw_data[1];
y_acc = ((int16_t)raw_data[2] << 8) + raw_data[3];
z_acc = ((int16_t)raw_data[4] << 8) + raw_data[5];
```

Estos valores representan la salida cruda del acelerómetro del MPU6050.

---

### 6. Comunicación UART2 hacia la Computadora

Se configuró **UART2** para transmitir datos hacia la computadora mediante el puerto virtual COM del ST-LINK.

Pines configurados:

| Pin STM32 | Función   |
| --------- | --------- |
| PA2       | USART2_TX |
| PA15      | USART2_RX |

Configuración UART2:

| Parámetro           | Valor    |
| ------------------- | -------- |
| Baud rate           | `115200` |
| Longitud de palabra | 8 bits   |
| Bits de parada      | 1        |
| Paridad             | Ninguna  |
| Modo                | TX/RX    |

Además, se redireccionó la función `printf()` hacia UART2 mediante:

```c
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
```

Esto permite imprimir valores del STM32 directamente en una terminal serial o enviarlos hacia el programa en Python.

---

### 7. Salida UART Compatible con CSV

Los valores del acelerómetro se transmiten usando un formato separado por comas:

```c
printf("%d,%d,%d \r\n", x_acc, y_acc, z_acc);
```

Este formato es importante porque el programa en Python separa los datos usando comas.

Ejemplo de salida esperada:

```text
120,-80,16384
118,-75,16390
125,-70,16370
```

De esta forma, los datos pueden almacenarse directamente como filas dentro de un archivo CSV.

---

### 8. Lectura Periódica mediante Temporizador

Se configuró un temporizador para ejecutar lecturas periódicas del MPU6050.

La función utilizada es:

```c
TIM_interrupt(1000, 8000, TIM2, TIM2_IRQn, &TimInt2);
```

Dentro del manejador de interrupción `TIM2_IRQHandler()`, se llama a la función:

```c
MPU6050_Values();
```

Esto permite realizar lecturas del acelerómetro de forma periódica sin depender directamente del ciclo principal `while(1)`.

Flujo actual:

1. Se genera una interrupción por TIM2.
2. Se ejecuta el manejador de interrupción.
3. Se leen los datos del MPU6050.
4. Se convierten los datos crudos a valores X, Y y Z.
5. Se transmiten los valores por UART2.

---

### 9. Configuración de USART1 para Comunicación con ESP32

También se configuró **USART1** para una futura comunicación directa entre el STM32 y el ESP32.

Pines configurados:

| Pin STM32 | Función   |
| --------- | --------- |
| PA9       | USART1_TX |
| PA10      | USART1_RX |

Configuración USART1:

| Parámetro           | Valor   |
| ------------------- | ------- |
| Baud rate           | `9600`  |
| Longitud de palabra | 8 bits  |
| Bits de parada      | 1       |
| Paridad             | Ninguna |
| Modo                | TX/RX   |

En el código existen pruebas comentadas para:

* Enviar mensajes por UART1.
* Recibir datos provenientes del ESP32.
* Reenviar datos recibidos.

Esto muestra que la comunicación STM32-ESP32 ya está preparada para futuras pruebas de integración.

---

## Funciones Modulares del Proyecto

Los archivos `functions.h` y `functions.c` permiten separar parte de la configuración de periféricos del archivo principal.

Actualmente se incluyen funciones reutilizables para:

---

### Configuración de Pines GPIO

Se creó una función para configurar pines GPIO en diferentes modos:

* Salida digital.
* Función alterna.
* Pull-up o pull-down.
* Velocidad del GPIO.
* Selección de función alterna.

Esto evita repetir configuraciones GPIO en diferentes partes del código.

---

### Configuración de Temporizadores con Interrupción

Se creó una función genérica para configurar temporizadores en modo interrupción:

```c
TIM_interrupt(...)
```

Esta función recibe:

* Periodo.
* Prescaler.
* Instancia del temporizador.
* Número de interrupción.
* Handle del temporizador.

La función habilita el reloj del temporizador correspondiente, configura la interrupción en NVIC, inicializa el temporizador y lo inicia en modo interrupción.

Temporizadores considerados actualmente:

* TIM1
* TIM2
* TIM6
* TIM7
* TIM15
* TIM16

---

### Configuración PWM

Se desarrolló una función genérica para PWM, permitiendo configurar:

* Temporizador.
* Canal.
* Prescaler.
* Periodo.
* Pulso inicial.
* Handle del temporizador.

Esta función habilita el reloj del temporizador, inicializa el modo PWM, configura el canal y arranca la señal PWM.

---

### Configuración ADC

Se creó una función genérica para configurar el ADC, permitiendo definir:

* Instancia ADC.
* Resolución.
* Canal.
* Tiempo de muestreo.
* Handle ADC.
* Configuración del canal ADC.

Esta función prepara el ADC para conversiones individuales y configura el canal seleccionado.

---

## Avances en la Configuración del ESP32

Dentro de la carpeta `ESP32_Config` se encuentran dos programas principales.

---

### 1. Programa de Lectura RSSI

Archivo:

[`ESP32_Config/RSSI_Values/RSSI_Values.ino`](https://github.com/AdamHCZ/MrowDAR/blob/master/ESP32_Config/RSSI_Values/RSSI_Values.ino)

Este programa permite que el ESP32 se conecte a una red Wi-Fi y lea el valor RSSI.

Funcionalidades actuales:

* Inicializa la comunicación serial a `115200` baudios.
* Se conecta a una red Wi-Fi configurada.
* Imprime el progreso de conexión.
* Muestra la dirección IP asignada al ESP32.
* Lee valores RSSI usando:

```cpp
WiFi.RSSI()
```

* Imprime el valor RSSI periódicamente.

Este programa representa el primer paso para incorporar la medición de intensidad de señal Wi-Fi dentro del sistema MrowDAR.

Nota importante: las credenciales Wi-Fi aparecen directamente en el código. Para futuras versiones, se recomienda reemplazarlas por valores genéricos o moverlas a un archivo privado de configuración.

---

### 2. Programa de Comunicación UART del ESP32

Archivo:

[`ESP32_Config/UART_CODE/UART_CODE.ino`](https://github.com/AdamHCZ/MrowDAR/blob/master/ESP32_Config/UART_CODE/UART_CODE.ino)

Este programa configura el UART2 del ESP32 para comunicarse con el STM32.

Pines UART configurados en el ESP32:

| Pin ESP32 | Función |
| --------- | ------- |
| GPIO16    | RXD2    |
| GPIO17    | TXD2    |

Configuración actual:

| Interfaz    | Baud rate | Uso                             |
| ----------- | --------: | ------------------------------- |
| USB Serial  |  `115200` | Comunicación con la computadora |
| UART2 ESP32 |    `9600` | Comunicación con el STM32       |

Funcionalidades actuales:

* Inicializa la comunicación serial USB.
* Inicializa UART2.
* Envía un mensaje de prueba al STM32.
* Lee datos provenientes del STM32.
* Reenvía los datos recibidos hacia la computadora mediante el monitor serial.

Este programa permite probar al ESP32 como puente de comunicación entre el STM32 y la computadora.

---

## Programa de Lectura en Python

Archivo:

[`Lecture_Program/Lecture_cv.py`](https://github.com/AdamHCZ/MrowDAR/blob/master/Lecture_Program/Lecture_cv.py)

Este programa permite recibir datos del STM32 por puerto serial y guardarlos en un archivo CSV.

Funcionalidades actuales:

* Abre un puerto serial.
* Lee líneas recibidas por UART.
* Decodifica los bytes recibidos como texto.
* Elimina saltos de línea.
* Separa los valores usando comas.
* Guarda los datos en un archivo CSV.
* Muestra los datos recibidos en la terminal.

El archivo CSV usa la siguiente cabecera:

```csv
x_axis,y_axis,z_axis
```

Esta estructura coincide con la salida generada por el STM32:

```text
x_acc,y_acc,z_acc
```

Ejemplo:

```text
120,-80,16384
```

Ese dato se convierte en una fila CSV con tres columnas:

| x_axis | y_axis | z_axis |
| -----: | -----: | -----: |
|    120 |    -80 |  16384 |

---

## Flujo de Datos Actual

El flujo principal implementado hasta ahora es:

```text
MPU6050
   ↓ I2C
STM32L432KC
   ↓ UART2 / printf
Computadora por puerto serial
   ↓ Script Python
Accelerometer_data.csv
```

El flujo previsto para la comunicación con ESP32 es:

```text
ESP32
   ↓ UART
STM32L432KC
   ↓ UART2 / USB
Computadora
```

El flujo de medición RSSI es:

```text
ESP32
   ↓ Lectura RSSI Wi-Fi
Monitor serial / futura integración con STM32
```

---

## Estado Actual del Proyecto

Hasta el final de esta semana, el proyecto cuenta con los siguientes avances:

* Estructura base del proyecto STM32 en Keil y HAL.
* Configuración I2C para comunicación con el MPU6050.
* Inicialización del MPU6050 mediante registros internos.
* Lectura de datos crudos del acelerómetro.
* Conversión de datos crudos en valores X, Y y Z.
* Configuración de UART2 para salida por USB serial.
* Redirección de `printf()` hacia UART2.
* Transmisión de datos en formato compatible con CSV.
* Uso de temporizador para lectura periódica del sensor.
* Preparación de USART1 para comunicación STM32-ESP32.
* Programa ESP32 para lectura RSSI.
* Programa ESP32 para comunicación UART.
* Programa Python para lectura serial.
* Archivo CSV preparado para almacenamiento de datos.

---

## Problemas Conocidos y Mejoras Pendientes

Aunque el proyecto ya tiene una base funcional, se identifican los siguientes puntos para mejorar:

1. **Baud rate del programa Python**

   * El programa Python debe usar el mismo baud rate que UART2 del STM32.
   * Si el STM32 usa:

     ```c
     115200
     ```
   * Python también debe usar:

     ```python
     BAUD_RATE = 115200
     ```

2. **Manejo de errores en I2C**

   * Actualmente se requiere mejorar el comportamiento cuando el MPU6050 se desconecta.
   * Se recomienda implementar una estrategia de reconexión y verificación mediante `WHO_AM_I`.

3. **Evitar procesos pesados dentro de interrupciones**

   * Actualmente la lectura del MPU6050 y la transmisión UART se realizan dentro del manejador de interrupción.
   * En futuras versiones, es mejor que la interrupción solo active una bandera y que el procesamiento se realice dentro del `while(1)`.

4. **Integración del ESP32**

   * Los programas de RSSI y UART del ESP32 aún están separados.
   * El siguiente paso será unir la lectura RSSI con el envío de datos por UART hacia el STM32.

5. **Credenciales Wi-Fi**

   * Las credenciales de red aparecen directamente en el código del ESP32.
   * Para un repositorio público, es mejor reemplazarlas por valores genéricos o excluirlas del repositorio.

6. **Modularización del código**

   * Ya existen funciones auxiliares en `functions.c`.
   * Se recomienda mover más lógica desde `Mrowdar.c` hacia funciones reutilizables para mejorar el orden del proyecto.

---

## Trabajo Planeado para la Próxima Semana

Para la siguiente semana se plantea avanzar en:

* Verificar y corregir el baud rate del script Python.
* Mejorar el manejo de errores y reconexión del MPU6050.
* Validar que los datos del acelerómetro se guarden correctamente en `Accelerometer_data.csv`.
* Integrar la lectura RSSI del ESP32 con la comunicación UART.
* Enviar valores RSSI desde el ESP32 hacia el STM32.
* Definir un formato final de datos para acelerómetro y RSSI.
* Mejorar la estructura modular del firmware STM32.
* Agregar más comentarios técnicos al código.
* Validar la estabilidad de comunicación entre STM32, ESP32 y computadora.

---

## Conclusión Semanal

Durante esta semana se estableció la base funcional del proyecto MrowDAR. El STM32 ya cuenta con comunicación I2C para inicializar y leer datos del MPU6050, además de transmisión UART en formato compatible con CSV. También se prepararon pruebas iniciales en el ESP32 para lectura RSSI y comunicación UART, y se desarrolló un programa en Python para almacenar los datos recibidos en un archivo CSV.

La siguiente etapa estará enfocada en hacer el sistema más robusto, mejorar la reconexión del sensor, integrar los datos RSSI del ESP32 y preparar el proyecto para pruebas de medición más completas.
