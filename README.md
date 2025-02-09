#Respoo![IMG_1152](https://github.com/user-attachments/assets/fe03c6b4-c388-49da-bc7b-b94e83e49636)
lerm
🚀 **Respooler** es una máquina compacta para rebobinar filamento de impresora 3D de manera eficiente. Está basada en el diseño **V-Spooler**, pero con mejoras como un **motor paso a paso** y un **sensor de filamento** para facilitar su uso.

## 📌 Características

- Diseño compacto y optimizado.
- Soporte para diferentes poleas y correas HDT de 10 mm.
- Motor con ajuste fácil de tensión de correa.
- Eje de bobina más grueso para mayor resistencia.
- Compatible con **Maker's Supply**.
- Sensores que detienen el motor automáticamente cuando se completa el proceso.

## 🛠️ Hardware y Electrónica Necesaria

- Motor **NEMA 17**.
- Sensor de filamento.
- **Arduino Nano** para el control.
- Driver de motor paso a paso.
- Varias piezas impresas en 3D (recomendado **PETG o ABS** para mayor resistencia).
- Tornillos y soportes mecánicos.

Para comprar kits de hardware, visita: [LTS-Design Shop](https://lts-design.com/)

## 🔧 Ensamblaje

1. Instala la base del motor y fija el **NEMA 17** con tornillos M3 x 6.
2. Inserta tu **PCB** (si tienes el kit) y conecta el driver de motor.
3. Conecta los cables del **sensor de filamento** y **botón de inicio** a la PCB.
4. Asegura el regulador de potencia y ajusta la tensión si es necesario.
5. Ensambla el **soporte de la bobina** y el **sistema de poleas**.
6. Ajusta la correa y alinea los engranajes correctamente.
7. Carga el código en el Arduino desde la sección de **Documentación**.

📜 **Diagrama de conexión** disponible en la sección de documentación.

## 📹 Video en Acción

Mira el LTS Respooler en funcionamiento: [YouTube](https://youtu.be/B1Um_I1KRLo)

## ⚙️ Cómo Usarlo

1. Alimenta el filamento desde la bobina superior a través del tubo **PTFE**.
2. El LED derecho se encenderá (si no hay filamento, el motor no inicia).
3. Sujeta el filamento en la bobina inferior y presiona el **botón de inicio**.
4. El motor girará automáticamente hasta completar el proceso.
5. Para detener manualmente, presiona el botón nuevamente.

## 📜 Notas de Impresión 3D

- Las piezas negras deben imprimirse con **PETG o ABS** para mayor durabilidad.
- Algunas piezas requieren **soportes** que deben ser removidos tras la impresión.
- Los tornillos se ajustan directamente en el plástico (sin inserts), ¡no los aprietes demasiado!

## 🏪 Dónde Comprar Kits y Electrónica

Para facilitar la construcción, puedes adquirir diferentes kits en:

🔗 [LTS-Design Store](https://lts-design.com/)

- **PCB solo**: [Comprar aquí](https://lts-design.com/products/lts-respooler-pcb)
- **PCB + Electrónica**: [Comprar aquí](https://lts-design.com/products/lts-respooler-pcb-and-electronics)
- **Power Pack Completo**: [Comprar aquí](https://lts-design.com/products/lts-respooler-power-pack)

## 🛠️ Mejoras y Versiones

### 🔹 V3
- Soporte para **Maker's Supply**.
- Compatibilidad con **correas HDT de 10 mm**.
- Eje de bobina más grueso.
- Ajuste mejorado de la tensión del motor.

### 🔹 V2
- Nuevo diseño de base y estructura para mejor impresión.

### 🔹 V1.5
- Caja de electrónica rediseñada para **mejor ventilación** y menos soportes.

### 🔹 V1
- **Montura de motor ajustable**.

---

📌 **Este proyecto es de código abierto**. Si tienes mejoras, pull requests son bienvenidas. ¡Felices rebobinados! 🎉

---
