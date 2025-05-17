# ImageResizer
# 🖼️ Proiect Procesare Imagini – Zoom & Shrink

Proiect realizat in C++ cu opencv, care permite marirea 🔍 si micsorarea 🔽 imaginilor grayscale pe 8 biti folosind diferite metode de interpolare.

## 📌 1. managementul proiectului

### 🎯 1.1 tema aleasa
- marirea si micsorarea imaginilor digitale la orice scala
- lucram cu imagini grayscale (8 biti)
- testam si comparam mai multe metode de interpolare 📊

### 🎯 1.2 scopul proiectului
- sa implementez tehnici de redimensionare (zoom si shrink) pentru imagini digitale
- algoritmul permite scalare libera (ex: 60%, 120%, 960x420 px etc)
- atat zoom (ex: 200%) cat si shrink sunt suportate ✅

### 🧠 1.3 domenii de aplicabilitate
- in camere de supraveghere si recunoastere faciala 🎥
- pe site-uri web (pentru viteza si adaptare pe ecran) 🌐
- cand tiparim sau incarcam poze pe retele sociale 🖨️📱

### ❗ 1.4 problema rezolvata
- cand marim, imaginea devine neclara 👎
- cand micsoram prea mult, pierdem detalii 🫥
- scopul este sa mentinem claritatea si detaliile 🔧

### 💡 1.5 intuitia metodei
- marirea/micsorarea presupune adaugarea sau eliminarea de pixeli
- noii pixeli sunt "ghiciti" folosind informatia vecinilor 🔎
- folosim interpolare pentru a estima corect valorile 📐

---

## 🛠️ 2. planificarea proiectului

### 🧑‍💻 2.1 pasul 1 – implementarea algoritmilor
- folosesc c++ si opencv
- codul transforma teoria in practica
- rezultatul: un program care redimensioneaza imagini corect
- probleme posibile: erori de cod, incompatibilitati cu opencv 🐞

### 📐 2.2 alegerea metodelor de interpolare
folosesc 4 metode:

#### 1️⃣Nnearest neighbor interpolation
- cel mai simplu: pixelul nou copiaza cel mai apropiat vecin
- rapid dar nu foarte precis
- ideal pt forme simple 🟦🟥

#### 2️⃣ bilinear interpolation
- foloseste media celor 4 pixeli vecini
- mai neted decat metoda anterioara
- bun pentru imagini cu detalii medii

#### 3️⃣ bicubic interpolation
- foloseste 16 pixeli in jur
- se bazeaza pe spline-ul catmull-rom pentru netezime
- rezultate de calitate superioara, ideal pentru detalii 🎨

#### 4️⃣ gaussian interpolation
- media ponderata a pixelilor vecini
- cei apropiati au greutate mai mare
- folosesc metoda backward pt aplicare in acest proiect 🎯

---

## 📂 fisiere

- `main.cpp` – codul principal (citire, afisare, apeluri metode)
- `project.h` – header cu functii de redimensionare
- `project.cpp` – implementare metode interpolare

---

## 🖥️ cerinte

- compilator c++ (g++, clang etc)
- opencv instalat ✅
- imagine input in format grayscale

---
