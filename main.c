#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PLANET_COUNT 8
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// menu
void showMenu() {
    printf("\n==============================\n");
    printf("Deney Menusu (Risk Analizi Modu)\n");
    printf("1 - Serbest Dusme Deneyi\n");
    printf("2 - Yukari Atis Deneyi\n");
    printf("3 - Agirlik Deneyi\n");
    printf("4 - Kutlecekim Potansiyel Enerji Deneyi\n");
    printf("5 - Hidrostatik Basinc Deneyi\n");
    printf("6 - Arsimet Kaldirma Kuvveti Deneyi\n");
    printf("7 - Basit Sarkac Periyodu Deneyi\n");
    printf("8 - Sabit Ip Gerilmesi Deneyi\n");
    printf("9 - Asansor Deneyi\n");
    printf("Cikis icin \"-1\" tuslayin\n");
    printf("==============================\n");
    printf("Seciminizi giriniz: ");
}

double absT(double x) {
    return (x < 0) ? -x : x;
}

// scanf sonrasi satir temizleme
void clearLine(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

// Guvenli double okuma + mutlak deger
int readDoubleAbs(const char *prompt, double *out) {
    printf("%s", prompt);
    if (scanf("%lf", out) != 1) {
        printf("Gecersiz giris! Sayi giriniz.\n");
        clearLine();
        return 0;
    }
    clearLine();
    *out = absT(*out);
    return 1;
}

// Guvenli int okuma
int readInt(const char *prompt, int *out) {
    printf("%s", prompt);
    if (scanf("%d", out) != 1) {
        printf("Gecersiz giris! Sayi giriniz.\n");
        clearLine();
        return 0;
    }
    clearLine();
    return 1;
}

// Z-score tabanli risk etiketi
const char* riskLabelZ(double value, double mean, double std) {
    double v = absT(value);

    if (std < 1e-12) return "ORTA";

    double z = absT(v - mean) / std;

    // Z-score esikleri
    return (z < 0.5) ? "DUSUK" : (z < 1.5) ? "ORTA" : "YUKSEK";
}

// Risk modeli: Z-score
void runOnPlanets(
    const char *expName,
    const char *riskTag,
    const char *symbol,
    char scientistName[],
    double *gPtr,
    char **pPtr,
    int planetCount,
    double (*calc)(double g, void *ctx),
    void *ctx,
    const char *unit
) {
    printf("\n====================================\n");
    printf("RISK ANALIZI KONSOLU\n");
    printf("Bilim Insani: %s\n", scientistName);
    printf("Senaryo: Olasi Tehlike Senaryolari\n");
    printf("Deney: %s\n", expName);
    printf("Uyari: %s\n", riskTag);
    printf("Risk modeli: Z-score (istatistiksel sapma)\n");
    printf("------------------------------------\n");

    double *results = (double*)malloc(sizeof(double) * planetCount);
    if (!results) {
        printf("HATA: Bellek ayrilamadi!\n");
        return;
    }

    // Ortalama (mean)
    double sum = 0.0;
    for (int i = 0; i < planetCount; i++) {
        double g = *(gPtr + i);
        double r = calc(g, ctx);
        *(results + i) = r;
        sum += absT(r);
    }
    double mean = sum / (double)planetCount;

    // Standart sapma
    double varSum = 0.0;
    for (int i = 0; i < planetCount; i++) {
        double diff = absT(*(results + i)) - mean;
        varSum += diff * diff;
    }
    double variance = varSum / (double)planetCount; // population variance
    double std = sqrt(variance);

    for (int i = 0; i < planetCount; i++) {
        const char *planet = *(pPtr + i);
        double g = *(gPtr + i);
        double result = *(results + i);

        const char *risk = riskLabelZ(result, mean, std);

        printf(">>> %s yuzeyinde analiz calistiriliyor...\n", planet);
        printf("    Yercekimi ivmesi (g): %.2f m/s^2\n", g);
        printf("    Hesaplanan %s = %.4f %s  |  RISK: [%s]\n\n", symbol, result, unit, risk);
    }

    free(results);
    printf("====================================\n");
}

typedef struct { double t; } FreeFallCtx;

double calcFreeFall(double g, void *ctx) {
    FreeFallCtx *c = (FreeFallCtx*)ctx;
    return 0.5 * g * c->t * c->t;
}

void freeFallExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    FreeFallCtx ctx;

    printf("\n[Serbest Dusme Deneyi - Dusme Riski Analizi]\n");
    if (!readDoubleAbs("Sureyi giriniz t (s): ", &ctx.t)) return;

    runOnPlanets(
        "Serbest Dusme (h = 1/2 * g * t^2)",
        "Dusme/yaralanma riski: Ayni surede daha fazla yol alinabilir.",
        "h",
        scientistName,
        gPtr, pPtr, planetCount,
        calcFreeFall, &ctx,
        "m"
    );
}

typedef struct { double v0; } UpThrowCtx;

double calcUpThrow(double g, void *ctx) {
    UpThrowCtx *c = (UpThrowCtx*)ctx;
    return (c->v0 * c->v0) / (2.0 * g);
}

void upThrowExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    UpThrowCtx ctx;

    printf("\n[Yukari Atis Deneyi - Yerden Kopma/Ziplamanin Sonucu]\n");
    if (!readDoubleAbs("Ilk hizi giriniz v0 (m/s): ", &ctx.v0)) return;

    runOnPlanets(
        "Yukari Atis (hmax = v0^2 / (2g))",
        "Kontrol kaybi riski: Dusuk g ortaminda beklenenden fazla yukselme olabilir.",
        "h_max",
        scientistName,
        gPtr, pPtr, planetCount,
        calcUpThrow, &ctx,
        "m"
    );
}

typedef struct { double m; } WeightCtx;

double calcWeight(double g, void *ctx) {
    WeightCtx *c = (WeightCtx*)ctx;
    return c->m * g;
}

void weightExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    WeightCtx ctx;

    printf("\n[Agirlik Deneyi - Tasima/Yuk Riski]\n");
    if (!readDoubleAbs("Kutleyi giriniz m (kg): ", &ctx.m)) return;

    runOnPlanets(
        "Agirlik (G = m * g)",
        "Ekipman tasima riski: Yuk arttikca hareket kabiliyeti azalir.",
        "G",
        scientistName,
        gPtr, pPtr, planetCount,
        calcWeight, &ctx,
        "N"
    );
}

typedef struct { double m; double h; } PotentialEnergyCtx;

double calcPotentialEnergy(double g, void *ctx) {
    PotentialEnergyCtx *c = (PotentialEnergyCtx*)ctx;
    return c->m * g * c->h;
}

void potentialEnergyExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    PotentialEnergyCtx ctx;

    printf("\n[Kutlecekim Potansiyel Enerji Deneyi - Dusme Enerjisi/Risk]\n");
    if (!readDoubleAbs("Kutleyi giriniz m (kg): ", &ctx.m)) return;
    if (!readDoubleAbs("Yuksekligi giriniz h (m): ", &ctx.h)) return;

    runOnPlanets(
        "Potansiyel Enerji (Ep = m * g * h)",
        "Hasar riski: Enerji buyudukce dusme/carpma etkisi artar.",
        "E_p",
        scientistName,
        gPtr, pPtr, planetCount,
        calcPotentialEnergy, &ctx,
        "J"
    );
}

typedef struct { double rho; double h; } HydroPressureCtx;

double calcHydroPressure(double g, void *ctx) {
    HydroPressureCtx *c = (HydroPressureCtx*)ctx;
    return c->rho * g * c->h;
}

void hydroPressureExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    HydroPressureCtx ctx;

    printf("\n[Hidrostatik Basinc Deneyi - Sivi Ortam Tehlikesi]\n");
    if (!readDoubleAbs("Sivinin yogunlugunu giriniz rho (kg/m^3): ", &ctx.rho)) return;
    if (!readDoubleAbs("Derinligi giriniz h (m): ", &ctx.h)) return;

    runOnPlanets(
        "Hidrostatik Basinc (P = rho * g * h)",
        "Basinc riski: Derinlik arttikca insan/ekipman uzerindeki etki artar.",
        "P",
        scientistName,
        gPtr, pPtr, planetCount,
        calcHydroPressure, &ctx,
        "Pa"
    );
}

typedef struct { double rho; double V; } BuoyantForceCtx;

double calcBuoyantForce(double g, void *ctx) {
    BuoyantForceCtx *c = (BuoyantForceCtx*)ctx;
    return c->rho * g * c->V;
}

void buoyantForceExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    BuoyantForceCtx ctx;

    printf("\n[Arsimet Kaldirma Kuvveti Deneyi - Batma/Yuzme Analizi]\n");
    if (!readDoubleAbs("Sivinin yogunlugunu giriniz rho (kg/m^3): ", &ctx.rho)) return;
    if (!readDoubleAbs("Cismin batan hacmini giriniz V (m^3): ", &ctx.V)) return;

    runOnPlanets(
        "Arsimet (F_k = rho * g * V)",
        "Stabilite riski: Kaldirma kuvveti degisirse batma/yuzme durumu farklilasir.",
        "F_k",
        scientistName,
        gPtr, pPtr, planetCount,
        calcBuoyantForce, &ctx,
        "N"
    );
}

typedef struct { double L; } PendulumCtx;

double calcPendulum(double g, void *ctx) {
    PendulumCtx *c = (PendulumCtx*)ctx;
    return 2.0 * M_PI * sqrt(c->L / g);
}

void pendulumExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    PendulumCtx ctx;

    printf("\n[Basit Sarkac Periyodu Deneyi - Zamanlama/Kontrol Riski]\n");
    if (!readDoubleAbs("Sarkac uzunlugunu giriniz L (m): ", &ctx.L)) return;

    runOnPlanets(
        "Basit Sarkac (T = 2pi * sqrt(L/g))",
        "Kontrol riski: Mekanik zamanlama sistemleri gezegene gore farkli calisabilir.",
        "T",
        scientistName,
        gPtr, pPtr, planetCount,
        calcPendulum, &ctx,
        "s"
    );
}

typedef struct { double m; } RopeTensionCtx;

double calcRopeTension(double g, void *ctx) {
    RopeTensionCtx *c = (RopeTensionCtx*)ctx;
    return c->m * g;
}

void ropeTensionExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    RopeTensionCtx ctx;

    printf("\n[Sabit Ip Gerilmesi Deneyi - Halat/Kopma Riski]\n");
    if (!readDoubleAbs("Kutleyi giriniz m (kg): ", &ctx.m)) return;

    runOnPlanets(
        "Sabit Ip Gerilmesi (T = m * g)",
        "Guv. riski: Gerilme artarsa ip/aski sistemi kopma riski tasir.",
        "T",
        scientistName,
        gPtr, pPtr, planetCount,
        calcRopeTension, &ctx,
        "N"
    );
}

typedef struct {
    double m;
    double a;
    int mode;
} ElevatorCtx;

double calcElevator(double g, void *ctx) {
    ElevatorCtx *c = (ElevatorCtx*)ctx;
    double Nplus  = c->m * (g + c->a);
    double Nminus = c->m * (g - c->a);
    return (c->mode == 1) ? Nplus : Nminus;
}

void elevatorExperiment(char scientistName[], double *gPtr, char **pPtr, int planetCount) {
    ElevatorCtx ctx;

    printf("\n[Asansor Deneyi - Ani Ivme Riski]\n");
    if (!readDoubleAbs("Kutleyi giriniz m (kg): ", &ctx.m)) return;
    if (!readDoubleAbs("Asansor ivmesini giriniz a (m/s^2): ", &ctx.a)) return;

    printf("\nDurumu seciniz:\n");
    printf("1 - Yukari ivmelenerek hizlaniyor / asagi ivmelenerek yavasliyor  (N = m(g + a))\n");
    printf("2 - Asagi ivmelenerek hizlaniyor / yukari ivmelenerek yavasliyor  (N = m(g - a))\n");
    if (!readInt("Seciminiz (1/2): ", &ctx.mode)) return;

    if (ctx.mode != 1 && ctx.mode != 2) {
        printf("Gecersiz secim! Varsayilan 1 alindi.\n");
        ctx.mode = 1;
    }

    if (ctx.mode == 1) {
        runOnPlanets(
            "Asansor (N = m * (g + a))",
            "Ivme riski: Hissedilen agirlik artabilir, dengesizlik/yaralanma riski dogabilir.",
            "N",
            scientistName,
            gPtr, pPtr, planetCount,
            calcElevator, &ctx,
            "N"
        );
    } else {
        runOnPlanets(
            "Asansor (N = m * (g - a))",
            "Ivme riski: Hissedilen agirlik azalabilir, savrulma/denge kaybi olusabilir.",
            "N",
            scientistName,
            gPtr, pPtr, planetCount,
            calcElevator, &ctx,
            "N"
        );
    }
}

// main
int main() {
    char scientistName[50];
    int choice;

    double gravity[PLANET_COUNT] = {
        3.7,   // Merkur
        8.87,  // Venus
        9.81,  // Dunya
        3.71,  // Mars
        24.79, // Jupiter
        10.44, // Saturn
        8.69,  // Uranus
        11.15  // Neptun
    };

    char *planetNames[PLANET_COUNT] = {
        "Merkur","Venus","Dunya","Mars","Jupiter","Saturn","Uranus","Neptun"
    };

    double *gPtr = gravity;
    char **pPtr = planetNames;

    printf("Bilim insaninin adini giriniz: ");
    fgets(scientistName, sizeof(scientistName), stdin);
    for (char *p = scientistName; *p; ++p) {
        if (*p == '\n') { *p = '\0'; break; }
    }

    printf("\nHosgeldiniz %s!\n", scientistName);
    printf("Gunes Sistemi Fizik Deneyleri Simulasyonu baslatildi.\n");
    printf("Mod: Olasi Tehlike Senaryolari (Risk Analizi)\n");

    do {
        showMenu();

        if (!readInt("", &choice)) {
            continue;
        }

        if (choice == -1) {
            printf("\nProgram sonlandiriliyor. Iyi calismalar %s!\n", scientistName);
            break;
        }

        switch (choice) {
            case 1: freeFallExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 2: upThrowExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 3: weightExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 4: potentialEnergyExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 5: hydroPressureExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 6: buoyantForceExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 7: pendulumExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 8: ropeTensionExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            case 9: elevatorExperiment(scientistName, gPtr, pPtr, PLANET_COUNT);
                break;
            default:
                printf("\nGecersiz secim! Lutfen tekrar deneyiniz.\n");
        }

    } while (choice != -1);

    return 0;
}
