// super_trunfo_novato.c — MateCheck: Super Trunfo (Nível Novato)
// Requisitos atendidos:
// - Cadastro interativo de 2 cartas (estado, código, cidade, população, área, PIB, pontos turísticos).
// - Comparação por UM atributo escolhido no código (maior vence; exceto densidade: menor vence).
// - Exibição organizada dos dados e do vencedor.
// - Comentários e funções p/ manutenibilidade.
//
// Dica: altere a constante SELECTED_ATTR para trocar o atributo de comparação.
//
// Atributos disponíveis:
//   POPULACAO, AREA, PIB, PONTOS_TURISTICOS, DENSIDADE
//
// Observação: densidade populacional = população / área (menor vence)

#include <stdio.h>
#include <string.h>

#define STR_MAX 64

typedef struct {
    char estado[STR_MAX];      // ex.: "SP"
    char codigo[STR_MAX];      // ex.: "A01"
    char cidade[STR_MAX];      // ex.: "São Paulo"
    long long populacao;       // habitantes
    double area;               // km²
    double pib;                // bilhões (ou unidade que desejar)
    int pontos_turisticos;     // contagem
} Carta;

typedef enum {
    POPULACAO = 1,
    AREA = 2,
    PIB = 3,
    PONTOS_TURISTICOS = 4,
    DENSIDADE = 5
} Atributo;

// ======= CONFIGURAÇÃO: escolha o atributo para comparar =======
const Atributo SELECTED_ATTR = DENSIDADE;
// ===============================================================

static void ler_linha(const char *rotulo, char *buf, size_t max) {
    printf("%s", rotulo);
    if (fgets(buf, (int)max, stdin) == NULL) {
        // entrada encerrada
        buf[0] = '\0';
        return;
    }
    // remove '\n'
    size_t n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n') buf[n - 1] = '\0';
}

static long long ler_longlong(const char *rotulo) {
    char tmp[STR_MAX];
    long long v = 0;
    for (;;) {
        printf("%s", rotulo);
        if (!fgets(tmp, sizeof(tmp), stdin)) return 0;
        if (sscanf(tmp, "%lld", &v) == 1) return v;
        printf("Valor inválido. Tente novamente.\n");
    }
}

static double ler_double(const char *rotulo) {
    char tmp[STR_MAX];
    double v = 0.0;
    for (;;) {
        printf("%s", rotulo);
        if (!fgets(tmp, sizeof(tmp), stdin)) return 0.0;
        if (sscanf(tmp, "%lf", &v) == 1) return v;
        printf("Valor inválido. Tente novamente.\n");
    }
}

static Carta cadastrar_carta(int idx) {
    Carta c;
    printf("\n=== Cadastro da Carta %d ===\n", idx);

    ler_linha("Estado (ex: SP): ", c.estado, sizeof(c.estado));
    ler_linha("Código da carta (ex: A01): ", c.codigo, sizeof(c.codigo));
    ler_linha("Nome da cidade: ", c.cidade, sizeof(c.cidade));
    c.populacao = ler_longlong("População (habitantes): ");
    c.area = ler_double("Área (km²): ");
    c.pib = ler_double("PIB (ex: em bilhões): ");
    c.pontos_turisticos = (int)ler_longlong("Pontos turísticos (inteiro): ");

    return c;
}

static double densidade(const Carta *c) {
    // Proteção contra divisão por zero
    return (c->area > 0.0) ? ( (double)c->populacao / c->area ) : 0.0;
}

static void exibir_carta(const Carta *c, const char *titulo) {
    printf("\n--- %s ---\n", titulo);
    printf("Estado: %s\n", c->estado);
    printf("Código: %s\n", c->codigo);
    printf("Cidade: %s\n", c->cidade);
    printf("População: %lld\n", c->populacao);
    printf("Área (km²): %.3f\n", c->area);
    printf("PIB: %.3f\n", c->pib);
    printf("Pontos turísticos: %d\n", c->pontos_turisticos);
    printf("Densidade (hab/km²): %.3f\n", densidade(c));
}

static const char* nome_atributo(Atributo a) {
    switch (a) {
        case POPULACAO:        return "População (maior vence)";
        case AREA:             return "Área (maior vence)";
        case PIB:              return "PIB (maior vence)";
        case PONTOS_TURISTICOS:return "Pontos Turísticos (maior vence)";
        case DENSIDADE:        return "Densidade (menor vence)";
        default:               return "Desconhecido";
    }
}

// Retorna: -1 se carta1 vence, +1 se carta2 vence, 0 se empate
static int comparar(const Carta *c1, const Carta *c2, Atributo a) {
    switch (a) {
        case POPULACAO:
            if (c1->populacao > c2->populacao) return -1;
            if (c1->populacao < c2->populacao) return +1;
            return 0;
        case AREA:
            if (c1->area > c2->area) return -1;
            if (c1->area < c2->area) return +1;
            return 0;
        case PIB:
            if (c1->pib > c2->pib) return -1;
            if (c1->pib < c2->pib) return +1;
            return 0;
        case PONTOS_TURISTICOS:
            if (c1->pontos_turisticos > c2->pontos_turisticos) return -1;
            if (c1->pontos_turisticos < c2->pontos_turisticos) return +1;
            return 0;
        case DENSIDADE: {
            double d1 = densidade(c1);
            double d2 = densidade(c2);
            // MENOR vence
            if (d1 < d2) return -1;
            if (d1 > d2) return +1;
            return 0;
        }
        default:
            return 0;
    }
}

int main(void) {
    printf("===== Super Trunfo — Nível Novato =====\n");
    printf("Atributo de comparação: %s\n", nome_atributo(SELECTED_ATTR));

    // 1) Cadastro interativo
    Carta c1 = cadastrar_carta(1);
    Carta c2 = cadastrar_carta(2);

    // 2) Exibição organizada
    exibir_carta(&c1, "Carta 1");
    exibir_carta(&c2, "Carta 2");

    // 3) Comparação conforme regra
    int res = comparar(&c1, &c2, SELECTED_ATTR);

    // 4) Resultado
    printf("\n=== Resultado da Comparação (%s) ===\n", nome_atributo(SELECTED_ATTR));
    if (res < 0) {
        printf("Vencedora: Carta 1 — %s (%s - %s)\n", c1.cidade, c1.estado, c1.codigo);
    } else if (res > 0) {
        printf("Vencedora: Carta 2 — %s (%s - %s)\n", c2.cidade, c2.estado, c2.codigo);
    } else {
        printf("Empate técnico!\n");
    }

    printf("\nFim (Nível Novato).\n");
    return 0;
}
