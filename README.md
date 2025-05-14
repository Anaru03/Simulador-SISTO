# Simulador de Calendarización y Sincronización

## Universidad del Valle de Guatemala  
**Curso:** Sistemas Operativos – Sección 30  
**Docente:** Juan Carlos Canteo Boix  
**Integrantes:**  
- Ruth de León - [Anaru03](https://github.com/Anaru03) 
- Isabella Miralles - [Isabella-22293](https://github.com/Isabella-22293) 
- Año de creación: marzo 2025


---

## Descripción General

Este proyecto implementa un simulador visual y funcional de algoritmos de planificación de procesos y mecanismos de sincronización, utilizando el lenguaje C o C++. Está dividido en dos partes principales:

1. Simulación de algoritmos de calendarización.
2. Simulación de mecanismos de sincronización (mutex y semáforos).

El objetivo es reforzar los conceptos fundamentales de sistemas operativos, incluyendo scheduling, concurrencia y sincronización de procesos.

---

## Funcionalidades Principales

### A. Simulación de Calendarización
- Algoritmos soportados: FIFO, SJF, SRT, Round Robin (quantum configurable), Priority (con envejecimiento).
- Carga dinámica de procesos desde archivo de texto.
- Visualización dinámica del diagrama de Gantt con scroll horizontal.
- Identificación por nombre y color de procesos.
- Cálculo y despliegue de métricas como tiempo promedio de espera y tiempo promedio de finalización.

### B. Simulación de Sincronización
- Mecanismos soportados: Mutex y Semáforos.
- Carga dinámica de procesos, recursos y acciones desde archivos de texto.
- Visualización dinámica de acciones sobre recursos con identificación de estados (`ACCESSED`, `WAITING`).
- Línea de tiempo con scroll horizontal y diferenciación visual entre accesos exitosos y bloqueos.
