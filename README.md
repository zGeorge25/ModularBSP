# Satellite BSP - GPIO Driver

Acest proiect reprezinta o implementare a unui Board Support Package (BSP) pentru controlul pinilor GPIO, conceput pentru a rula atat pe un mediu simulat (Linux/Windows PC) cat si pe hardware-ul tinta (ATSAMV71).

## Arhitectura si Rationament

Arhitectura respecta principiul de **Interfata Unificata**.

1.  **Interfata Comuna (`gpio.h`)**:
    - Defineste abstractizarea pe care aplicatia o foloseste (`configure`, `set`, `read`).
    - Nu contine detalii specifice hardware-ului.
    - Foloseste `enum`-uri pentru a abstrage modurile pinilor si configuratiile (Input, Output, Pull-Up).

2.  **Backends (Implementari Specifice)**:
    - **Mock (`gpio_mock.cpp`)**: Folosit pentru dezvoltare si testare pe PC. Simuleaza comportamentul hardware-ului prin log-uri in consola si stocheaza starea pinilor in memorie.
    - **Target (`gpio_target.cpp`)**: Contine codul specific microcontrolerului ATSAMV71 (scrierea in registrii PIO). Este selectat la compilare.

3.  **Decuplare**:
    - Aplicatia (`main.cpp`) nu stie pe ce hardware ruleaza.
    - Selectia se face exclusiv prin sistemul de build (CMake).

## Instructiuni de Compilare

### Varianta 1: Cu CMake (Recomandat)

Daca aveti CMake instalat:

```bash
mkdir build
cd build

# 1. Configurare pentru Simulare PC (MOCK) - Default
cmake -DBSP_BACKEND=MOCK ..
# Sau specificand generatorul dorit:
cmake -G "<GENERATOR>" -DBSP_BACKEND=MOCK ..

# 2. Configurare pentru Hardware Real (TARGET - ATSAMV71)
cmake -DBSP_BACKEND=TARGET ..
# Sau cu generator specific:
cmake -G "<GENERATOR>" -DBSP_BACKEND=TARGET ..

# 3. Compilare
cmake --build .
```

### Varianta 2: Manual (g++)

Daca CMake nu este disponibil, puteti compila direct sursele pentru varianta Mock:

```bash
g++ -o satellite_bsp.exe src/main.cpp src/bsp/backend/mock/gpio_mock.cpp -I src/bsp/include
```

## Rulare si Verificare

Rulati executabilul generat (`satellite_bsp.exe` sau `./satellite_bsp`).
Veti vedea log-uri care confirma configurarea pinilor:

```text
[MOCK] GPIO Config: PB12 -> Mode: INPUT | Pull: UP
[MOCK] GPIO Config: PC9 -> Mode: OUTPUT
...
```

## Fluxul de Executie si Configurarea Pinilor

Fluxul de executie al aplicatiei este liniar si determinist:

1.  **Initializare**: Functia `main()` incepe executia.
2.  **Configurare Pini**: Aplicatia apeleaza secvential `BSP::GPIO::configure()` pentru fiecare pin definit in cerinte.
    - Driverul verifica validitatea pinului.
    - Driverul activeaza resursele hardware necesare (ex: ceasul perifericului in PMC pentru Target).
    - Driverul scrie in registrii de configurare (ex: directie, pull-up, multiplexare).

### Configurarea Specifica a Pinilor

Implementarea respecta cu strictete cerintele hardware:

- **PB12 (Input + Pull-Up)**:
  - Configurat ca intrare digitala activa.
  - Rezistenta interna de **Pull-Up** este conectata.
  - _Specific Target:_ Bitul din `CCFG_SYSIO` este setat pentru a dezactiva functia de System Erase.
- **PC9 & PC10 (Output)**:
  - Configurati ca iesiri digitale (Push-Pull).
- **PA9 & PA10 (UART0)**:
  - Configurati in modul **Periferic** (Functia A) pentru a conecta pinii la controllerul intern UART.
- **PB1 (ADC - AFEC1)**:
  - Configurat in modul **Periferic** (Functia B/Analog) pentru esantionarea semnalelor analogice.

## Concluzie

Acest proiect demonstreaza o arhitectura software robusta si modulara, punand accent pe
testabilitate si portabilitate. Prin separarea logicii hardware-ului de aplicatie, se reduce riscul erorilor si se
permite dezvoltarea iterativa rapida.

## Autor

Proiect realizat de: **[Stanca George]**
