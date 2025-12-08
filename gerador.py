#!/usr/bin/env python3
"""
Gerador de bases de dados para o projeto de instrumentação
em Algoritmos e Estruturas de Dados II.

Gera arquivos .in com linhas no formato:

    id  tipo  valor  horario  alpha

onde:
    - id     : identificador único do sensor (inteiro > 0)
    - tipo   : tipo do sensor (inteiro, ex.: 1, 2, 3, 4)
    - valor  : leitura do sensor (inteiro ou real)
    - horario: horário da coleta (HH:MM:SS)
    - alpha  : parâmetro de suavização (real em (0,1))

Ao ser executado sem argumentos, gera:
    medidas_2_05.in
    medidas_2_10.in
    medidas_2_15.in
    medidas_2_20.in

com 24 * 2^k linhas em cada arquivo.
"""

import argparse
import math
import random
from datetime import timedelta


# Mapeamento de tipo de sensor -> alpha (pode ser ajustado conforme o enunciado)
ALPHA_POR_TIPO = {
    1: 0.22,  # por exemplo: temperatura
    2: 0.25,  # por exemplo: umidade
    3: 0.30,  # por exemplo: luminosidade
    4: 0.40,  # por exemplo: presença/ocupação
}

TIPOS_DISPONIVEIS = sorted(ALPHA_POR_TIPO.keys())


def gerar_horario_aleatorio() -> str:
    """Gera um horário aleatório no formato HH:MM:SS."""
    total_segundos = random.randint(0, 24 * 60 * 60 - 1)
    t = timedelta(seconds=total_segundos)
    # Formatar como HH:MM:SS
    horas = total_segundos // 3600
    minutos = (total_segundos % 3600) // 60
    segundos = total_segundos % 60
    return f"{horas:02d}:{minutos:02d}:{segundos:02d}"


def gerar_valor(tipo: int) -> float:
    """
    Gera um valor de leitura coerente com o tipo de sensor.
    Ajuste as faixas conforme desejar.
    """
    if tipo == 1:
        # Temperatura (ex.: entre 15 e 35 °C)
        return round(random.uniform(15.0, 35.0), 3)
    elif tipo == 2:
        # Umidade (ex.: 0 a 100 %)
        return random.randint(0, 100)
    elif tipo == 3:
        # Luminosidade (ex.: 0 a 100000 lx)
        return random.randint(0, 100_000)
    elif tipo == 4:
        # Presença (ex.: 0 ou 100; aqui escolhemos 100 para "presença")
        # Poderia ser também random.choice([0, 100])
        return 100
    else:
        # Tipo desconhecido: valor genérico
        return random.randint(0, 1000)


def gerar_sensores(num_sensores: int):
    """
    Gera uma tabela de sensores:
        id_sensor -> (tipo, alpha)
    Garantindo:
        - id único para cada sensor
        - tipo fixo para cada id
    """
    sensores = {}
    for sensor_id in range(1, num_sensores + 1):
        tipo = random.choice(TIPOS_DISPONIVEIS)
        alpha = ALPHA_POR_TIPO[tipo]
        sensores[sensor_id] = (tipo, alpha)
    return sensores


def gerar_arquivo_instancia(n_base: int,
                            multiplicador: int,
                            nome_arquivo: str,
                            seed: int | None = None):
    """
    Gera um arquivo de instância com:
        total_linhas = n_base * multiplicador

    Cada linha no formato:
        id  tipo  valor  horario  alpha
    """
    if seed is not None:
        random.seed(seed)

    total_linhas = n_base * multiplicador

    # Número de sensores distintos:
    # Escolha de compromisso para ter repetição de IDs mas também variedade.
    # Ajuste se desejar.
    num_sensores = min(1000, max(10, n_base // 4))

    sensores = gerar_sensores(num_sensores)

    with open(nome_arquivo, "w", encoding="utf-8") as f:
        for _ in range(total_linhas):
            # Escolhe um sensor aleatório
            sensor_id = random.randint(1, num_sensores)
            tipo, alpha = sensores[sensor_id]

            valor = gerar_valor(tipo)
            horario = gerar_horario_aleatorio()

            # Importante: aqui usamos ponto como separador decimal.
            # Se quiser vírgula (0,22), basta pós-processar ou adaptar o print.
            if isinstance(valor, float):
                valor_str = f"{valor:.3f}"
            else:
                valor_str = str(valor)

            alpha_str = f"{alpha:.2f}"

            linha = f"{sensor_id}\t{tipo}\t{valor_str}\t{horario}\t{alpha_str}\n"
            f.write(linha)


def gerar_instancias_padrao(seed: int | None = None):
    """
    Gera as quatro instâncias padrão:
        medidas_2_05.in
        medidas_2_10.in
        medidas_2_15.in
        medidas_2_20.in
    cada uma com 24 * 2^k linhas.
    """
    exp_list = [5, 10, 15, 20]
    multiplicador = 24

    for exp in exp_list:
        n_base = 2 ** exp
        nome = f"medidas_2_{exp:02d}.in"
        print(f"Gerando {nome} com n_base={n_base} e total={n_base * multiplicador} linhas...")
        gerar_arquivo_instancia(n_base, multiplicador, nome, seed=seed)


def main():
    parser = argparse.ArgumentParser(
        description="Gerador de bases de dados (.in) para o projeto de instrumentação."
    )
    parser.add_argument(
        "--single",
        action="store_true",
        help="Gera apenas uma instância (usar com --n_base e --out). "
             "Se omitido, gera as quatro instâncias padrão."
    )
    parser.add_argument(
        "--n_base",
        type=int,
        default=32,
        help="Tamanho base n (número de elementos antes do multiplicador) para o modo --single."
    )
    parser.add_argument(
        "--mult",
        type=int,
        default=24,
        help="Multiplicador de n_base para definir o total de linhas no modo --single."
    )
    parser.add_argument(
        "--out",
        type=str,
        default="medidas_custom.in",
        help="Nome do arquivo de saída para o modo --single."
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=None,
        help="Semente aleatória (opcional) para reprodutibilidade."
    )

    args = parser.parse_args()

    if args.single:
        print(
            f"Gerando instância única em {args.out} "
            f"com n_base={args.n_base}, mult={args.mult}, "
            f"total={args.n_base * args.mult} linhas..."
        )
        gerar_arquivo_instancia(args.n_base, args.mult, args.out, seed=args.seed)
    else:
        gerar_instancias_padrao(seed=args.seed)


if __name__ == "__main__":
    main()
