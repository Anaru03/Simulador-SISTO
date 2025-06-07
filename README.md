Sincronización de Procesos en C (GTK)

Este proyecto simula algoritmos de planificación y mecanismos de sincronización (mutex y semáforos) utilizando una interfaz gráfica en C con GTK. Fue desarrollado como parte del curso de *Sistemas Operativos*.

---

## 📦 Estructura del Proyecto

- main.c – Archivo principal de entrada.
- gui.c / gui.h – Interfaz gráfica con GTK.
- loader.c / loader.h – Carga y parseo de archivos .txt.
- simulation.c / simulation.h – Núcleo de simulación (planificación/sincronización).
- process.c / process.h – Lógica de procesos.
- action.c / action.h – Lógica de acciones sobre recursos.
- sync.c / sync.h – Manejo de mutex y semáforos.
- resources.c / resources.h – Lógica de recursos disponibles.
- assets/ – Archivos .txt de entrada.
- Makefile – Compilación rápida.

---

## 🛠️ Requisitos

- *Sistema operativo:* Linux o WSL.
- *Compilador:* gcc
- *GTK 3:* Instálalo con:

```bash
sudo apt install libgtk-3-dev

```

---

## Compilar
```bash
Make
```
---

## Ejecutar

```bash
./programa
