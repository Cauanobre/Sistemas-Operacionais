# Simulador de Gerência de Memória 🧠

Este projeto é um simulador de alocação de memória utilizando partições dinâmicas. Ele permite observar como diferentes algoritmos lidam com a fragmentação externa e a ocupação do espaço.

## 🛠️ Algoritmos Implementados

O simulador permite comparar três estratégias clássicas de alocação:

- **First Fit:** Aloca o processo no primeiro bloco livre que for grande o suficiente. É o mais rápido, mas pode deixar fragmentos no início da memória.
- **Best Fit:** Procura o menor bloco livre que ainda seja suficiente para o processo. Minimiza a sobra de espaço no bloco escolhido, mas pode gerar muitos fragmentos minúsculos ("buracos") inúteis.
- **Worst Fit:** Aloca no maior bloco livre disponível. A ideia é que a sobra seja grande o suficiente para ser útil para outro processo futuramente.

## 🔍 Conceitos de SO Demonstrados

- **Fragmentação Externa:** Ocorre quando há memória total livre suficiente, mas ela está dividida em blocos pequenos e não contíguos.
- **Coalescência de Memória:** Técnica para fundir dois ou mais blocos livres adjacentes em um único bloco maior após a liberação de um processo (`mem_free`).
- **Split de Bloco:** Divisão de um bloco livre em dois: um ocupado e um novo bloco livre com a sobra.
- **Mapa de Memória:** Visualização textual do estado físico da memória.

## 🕹️ Comandos Disponíveis

**init** Inicializa o vetor que simula a memória física e cria o primeiro bloco livre com o tamanho especificado. 
**alloc** Executa a alocação de memória usando o algoritmo selecionado (First Fit, Best Fit ou Worst Fit).
**free_id** Libera um bloco previamente alocado com base em seu identificador.
**show** Exibe o estado atual da memória em duas linhas: uso físico (onde `#` é ocupado e `.` é livre) e os identificadores de cada bloco.
**stats** Calcula e exibe métricas de uso, fragmentação interna e externa. 

## 📝 Exemplo de Uso

```bash
> init 64 
> alloc 10 first 
> alloc 8 first 
> freeid 2 
> alloc 6 best 
> show
````

## 🚀 Como Executar

Não é necessário compilar, basta ter o Python 3 instalado.

```bash

python3 main.py
