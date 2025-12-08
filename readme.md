# Instrumentação e Avaliação de Desempenho de Estruturas de Dados  
## Tabela Hash × ABB/AVL  
**Disciplina:** Algoritmos e Estruturas de Dados II  
**Módulo:** 03  
**Tema:** Implementação e Análise de Dicionários  

---

# 1. Introdução

Este projeto tem como objetivo a *instrumentação* e análise experimental de duas estruturas clássicas utilizadas na implementação de dicionários:  
**Tabela Hash com encadeamento** e **Árvore Binária de Busca (ABB), incluindo AVL**.

A instrumentação consiste em incorporar ao código mecanismos para coletar métricas internas, tais como o **número de comparações realizadas**, **altura da ABB**, **clusterização da Hash**, além de medir os efeitos de **redimensionamento** e **movimentações** internas.  

A partir das métricas coletadas, será conduzido um estudo comparativo entre Hash e ABB/AVL, considerando diferentes tamanhos de instância e padrões de distribuição de dados.

---

# 2. Fundamentação Teórica

## 2.1. Tabelas Hash e Medidas de Desempenho

Tabelas Hash oferecem operações eficientes em tempo esperado constante, desde que a distribuição das chaves seja uniforme. O nível de dispersão pode ser analisado pelo **coeficiente de agrupamento**:

$$
C = \sum_i \left( \frac{x_i^2}{n} \right) - \alpha,
$$

onde:

- \($x_i$\): número de elementos no bucket \($i$\);  
- \($n$\): número total de elementos;  
- \($\alpha = n/m$\): fator de carga da tabela.

Interpretação:

- \($C = 1$\): distribuição uniforme;  
- \($C > 1$\): formação de agrupamentos (clusterização);  
- \($C < 1$\): dispersão superior à aleatória (raro).

Durante redimensionamentos, todos os elementos são remapeados, resultando em **movimentações** que devem ser contabilizadas.

---

## 2.2. Árvore Binária de Busca (ABB) e AVL

Uma ABB organiza seus elementos de maneira ordenada, o que permite buscas eficientes quando a altura da árvore é adequada. A altura de um nó é definida como:

$$
h(a)=
\begin{cases}
-1, & a = \text{null}, \\
\max\{h(a.sae), h(a.sad)\} + 1, & \text{caso contrário}.
\end{cases}
$$

ABB pode atingir altura linear no pior caso. Já a AVL mantém a altura em \($O(\log n)$\), oferecendo desempenho mais estável e previsível.

---

# 3. Descrição da Atividade

O estudante deverá instrumentar:

1. **Tabela Hash com encadeamento**  
2. **Árvore Binária de Busca (ABB ou AVL)**  

A instrumentação deve registrar **somente comparações entre elementos armazenados**.

## 3.1. Operações a Instrumentar — Hash

- Inserção  
- Busca  
- Remoção  
- Redimensionamento  
  - incluir contabilização de **movimentações** (elementos realocados)

## 3.2. Operações a Instrumentar — ABB/AVL

- Inserção  
- Busca  
- Remoção  

Registrar adicionalmente:

- altura da árvore,  
- rotações (se AVL),  
- custo agregado das operações.

---

# 4. Base de Dados

As instâncias consistem em registros de sensores ambientais contendo cinco campos:
```
id (inteiro)
tipo (inteiro)
valor (union)
horário (HH:MM:SS)
alpha (real)
```

- **id**: identificador único do sensor  
- **tipo**: categoria do sensor  
- **valor**: leitura (inteiro ou real)  
- **horário**: instante da coleta  
- **alpha**: parâmetro de suavização

Os dados não possuem ordenação prévia.

---

# 5. Consolidação e Média Móvel

Para registros com mesmo `id` e mesma hora arredondada (`HH:00:00`), calcule a **média móvel exponencial**:

$$
m_t = 
\begin{cases}
m, & t = 1 \\
(1 - \alpha)m_t + \alpha m, & t > 1,
\end{cases}
$$

onde:

- \($$m$\): medição atual  
- \($m_t$\): média suavizada  
- \($\alpha$\): peso associado ao tipo

Os dados consolidados devem ser salvos em arquivos `.cons` com o mesmo nome da instância.

---

# 6. Metodologia Experimental

Utilize o script `gerador.py` para construir instâncias de quatro tamanhos:

| Amostra | Tamanho \(n\) | Instâncias Geradas |
|---------|----------------|--------------------|
| 1       | \($2^5$\)        | \($24 \times 2^5$\)  |
| 2       | \($2^{10}$\)     | \($24 \times 2^{10}$\) |
| 3       | \($2^{15}$\)     | \($24 \times 2^{15}$\) |
| 4       | \($2^{20}$\)     | \($24 \times 2^{20}$\) |

Execute o código com diretiva ```--help`` para ver as formas de gerar as entradas.

Para cada instância:

1. Inserir todos os dados na Hash e na ABB/AVL  
2. Registrar:  
   - comparações totais e médias  
   - clusterização  
   - fator de carga  
   - redimensionamentos e movimentações  
   - altura da ABB/AVL  
3. Gerar gráficos:  
   - comparações × tamanho  
   - altura × tamanho  
   - clusterização × tamanho  
   - total de comparações por algoritmo

---

# 7. Resultados Esperados

O relatório deve apresentar:

- análise gráfica das comparações por algoritmo,  
- comportamento do fator de carga,  
- análise de clusterização,  
- comportamento da altura da ABB/AVL,  
- impacto dos redimensionamentos na Hash,  
- arquivos `.cons` gerados a partir da média móvel.

Gráficos de linhas devem ilustrar o crescimento das comparações à medida que o tamanho da instância aumenta.

---

# 8. Relatório Técnico

O relatório (`relatorio.pdf`) deve conter:

1. **Introdução**  
2. **Contextualização**  
3. **Metodologia**  
4. **Resultados**  
5. **Conclusões**

---

# 9. Arquivos a Entregar

1. `medidas_2_05.cons`  
2. `medidas_2_10.cons`  
3. `medidas_2_15.cons`  
4. `medidas_2_20.cons`  
5. `relatorio.pdf`  
6. Código-fonte completo (`*.c`)

Incluir identificação e função dos participantes, exemplo:

```
João – Instrumentação da Hash
Maria – Instrumentação da ABB/AVL
Pedro – Consolidação dos dados e geração dos gráficos
```


# 10. Observações Finais

- Registre somente comparações envolvendo elementos armazenados.  
- Movimentações em redimensionamentos devem ser contabilizadas.  
- Os dados `.cons` devem estar corretamente consolidados e ordenados.  
- A análise crítica dos resultados é parte essencial da avaliação.

---


